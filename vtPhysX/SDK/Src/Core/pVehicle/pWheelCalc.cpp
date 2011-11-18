#include <StdAfx.h>
#include "vtPhysXAll.h"

#include <xDebugTools.h>

#include "pDifferential.h"

#include "pVehicleAll.h"

//+		velWheelCC	{x=66.735077 y=0.097698294 z=-215.35669 ...}	VxVector
//+		velWheelTC	{x=66.735077 y=0.097698294 z=-215.35669 ...}	VxVector


enum StateFlags
{
	ON_SURFACE=1,                       // Wheel is touching surface?
	ATTACHED=2,                         // Attached to suspension?
	LOW_SPEED=4                         // Wheel is turning slow
};


#define  FRICTION_COEFF 100.0f
#define  MASS 12
#define  TIRE_RATE 50000.0f
#define  WHEEL_PENETRATION_DEPTH -0.0258f
#define  WHEEL_SUSPENSION_FORCE VxVector(0,-1025.0f,0)
#define RR_EPSILON_VELOCITY	0.001		// Wheel velocity
#define  OPT_SLIPVECTOR_USES_TANSA



// Use full 3D patch for surface detection?
#define USE_3D_PATCH

#define ENV_INI      "env.ini"

// For method #3, slipVector (Gregor Veble), use tan(SA) or SA?
#define OPT_SLIPVECTOR_USES_TANSA

// Skid methods
//#define SKID_METHOD_SEPARATE_LON_LAT
//#define SKID_METHOD_SLIP_RA_VECTOR
#define SKID_METHOD_Z_GREGOR

// Point at which skidmarks appear
#define SKIDMARK_SLIPRATIO  0.2

// Apply damping to slipRatio/slipAngle differential eq's at low speed?
//#define DO_DAMPING_LAT
//#define DO_DAMPING_LONG


// Apply low-speed enhancements? (obsolete since 18-5-01)
#define DO_LOW_SPEED

// Distance (in m) to start with wheel ray-track intersection; this is
// the height at which the ray is started to avoid getting a ray
// that starts BENEATH the track surface and therefore not hitting it.
#define DIST_INTERSECT   1.0

// Define the next symbol to check for wheel velocity reversal (vertically),
// and if so, the wheel is stopped momentarily. Is used to rule out
// incredibly high damper forces from pushing the wheel to full up or down.
// Should perhaps not be needed anymore combined with the implicit integrator.
// Note that this acts at the outermost positions of the wheel.
//#define DAMP_VERTICAL_VELOCITY_REVERSAL

// Damp the wheel when crossing the equilibrium position?
// As the wheel passes its center position, the spring force reverses. To
// avoid adding energy into the system, when passing this position, damping
// is used to avoid overaccelerating the tire to the other side.
// Note that this acts when the wheel is near its center (rest) position,
// contrast this with DAMP_VERTICAL_VELOCITY_REVERSAL.
#define DAMP_VERTICAL_EQUILIBRIUM_REVERSAL

// Use implicit integration? This should be more stable with
// high damper rates.
#define INTEGRATE_IMPLICIT_VERTICAL

// Gregor Veble combined slip algorithm? (instead of Pacejka)
//#define DO_GREGOR
#ifdef DO_GREGOR
	#undef DO_DAMPING_LAT
	#undef DO_DAMPING_LONG
#endif

// Delayed slip angle?
//#define USE_SAE950311_LAT

// If not using SAE950311, damp SA at low speed? (to avoid jittering)
#define USE_SA_DAMPING_AT_LOW_SPEED

// Wheel locking results in force going the direction of -slipVector?
#define USE_WHEEL_LOCK_ADJUST

#define USE_NXWHEEL_CONTACT_DATA

#define USE_NXWHEEL_NORMALFORCE_LOAD




static int getFrictionMethod(){ return FC_SLIPVECTOR; }






void pWheel2::CalcPacejka()
{

	float normalForce;
	

	if (hadContact)
	{
		normalForce=forceRoadTC.y;
	}else{
		normalForce = 0.0f;
	}
	
/*
	if (hadContact)
	{
	normalForce=500.0f;
	}else{
		normalForce = 0.0f;
	}
*/	

	
	pacejka.SetCamber(0);

	bool FC_SLIPVECTOR =true;
	// Note our positive slipAngle is the reverse of SAE's definition
	if(FC_SLIPVECTOR)
	{
		// Gregor Veble's and also Brian Beckman's idea of mixing Pacejka
		// Note that Gregor's Veble 'z' is 'csSlipLen' here.
		// The load isn't touched
		pacejka.SetNormalForce(normalForce);
		// Lateral
		//qdbg("  csSlipLen=%f, oSA=%f,oSR=%f\n",csSlipLen,optimalSA,optimalSR);
		if(csSlipLen<D3_EPSILON)
		{
			//qdbg("  csSlipLen near 0; load %f\n",normalForce);
			pacejka.SetSlipAngle(0);
			pacejka.SetSlipRatio(0);
			// Calculate separate Flat/Flong (Fy/Fx)
			pacejka.Calculate();
		} else
		{
			if(slipAngle<0)
				pacejka.SetSlipAngle(csSlipLen*optimalSA);
			else
				pacejka.SetSlipAngle(-csSlipLen*optimalSA);
			// Longitudinal
			if(slipRatio<0)
				pacejka.SetSlipRatio(-csSlipLen*optimalSR);
			else
				pacejka.SetSlipRatio(csSlipLen*optimalSR);
			// Calculate separate Flat/Flong (Fy/Fx)
			pacejka.Calculate();
			// Combine

#ifdef OPT_SLIPVECTOR_USES_TANSA
			pacejka.SetFy( (fabs(tanSlipAngle)/(tanOptimalSA*csSlipLen))*
				pacejka.GetFy());
			pacejka.SetMz( (fabs(tanSlipAngle)/(tanOptimalSA*csSlipLen))*
				pacejka.GetMz());
#else
			// Use normal slip angle
			pacejka.SetFy( (fabs(slipAngle)/(optimalSA*csSlipLen))*pacejka.GetFy());
			pacejka.SetMz( (fabs(slipAngle)/(optimalSA*csSlipLen))*pacejka.GetMz());
#endif
			pacejka.SetFx( (fabs(slipRatio)/(optimalSR*csSlipLen))*pacejka.GetFx());

		}
	} else
	{
		// Calculate Fx and Fy really separate, and possible combine later
		pacejka.SetSlipAngle(-slipAngle);
		pacejka.SetSlipRatio(slipRatio);
		pacejka.SetNormalForce(normalForce);
		// Calculate separate Flat/Flong (Fy/Fx), and maximum force
		// Combined Pacejka (slipratio & slipangle) will be done later
		pacejka.Calculate();
	}

	//----------------------------------------------------------------
	//
	// 
	//
	
	
	// Adjust forces according to the surface
	// May also add here some Pacejka scaling to get quick grip changes
/*	RSurfaceType *st=surfaceInfo.surfaceType;
	if(st)
	{
		if(st->frictionFactor!=1.0f)
		{
	
			float frictionFactor = 0.1f;
			pacejka.SetFx(pacejka.GetFx()*frictionFactor);
			pacejka.SetFy(pacejka.GetFy()*frictionFactor);
			// Do the same to the aligning moment, although not scientifically
			// based (does aligning torque scale linearly with surface friction?)
			pacejka.SetMz(pacejka.GetMz()*frictionFactor);
			pacejka.SetMaxLongForce(pacejka.GetMaxLongForce()*frictionFactor);
			pacejka.SetMaxLatForce(pacejka.GetMaxLatForce()*frictionFactor);
		}
*/
	
	//forceRoadTC.z=pacejka.GetFx();
	
	//----------------------------------------------------------------
	//
	// old
	//
	
	/*
	float factor  = getVehicle()->getEngine()->getForceFeedbackScale();
	float longitudalImpulse = xCheckFloat(lastContactData->longitudalImpulse);
	if (lastContactData)
	{
		forceRoadTC.z= longitudalImpulse * factor;
	}*/

	// Put some results in appropriate variables
	#ifdef LTRACE
		qdbg("  pacejka.Fx=%f\n",pacejka.GetFx());
	#endif
	
	float fx = pacejka.GetFx();;
	forceRoadTC.z=pacejka.GetFx();
	
}




void ConvertTireToCarOrientation(pWheel2 * wheel , VxVector *from,VxVector *to)
// Convert vector 'from' from tire coordinates
// to car coordinates (into 'to')
// Assumes: from!=to
{
	if (!wheel)
		return;	
	
	float angle,sinAngle,cosAngle;

	// Note that the tire is constrained in 5 dimensions, so only
	// 1 rotation needs to be done

	// Heading
	angle=wheel->GetHeading();
	sinAngle=sin(angle);
	cosAngle=cos(angle);

	// Rotate around Y axis to get heading of car right
	to->x=from->z*sinAngle+from->x*cosAngle;
	to->y=from->y;
	to->z=from->z*cosAngle-from->x*sinAngle;
}

void ConvertCarToTireOrientation (pWheel2*wheel, VxVector *from,VxVector*to)
// Convert vector 'from' from car coordinates
// to tire coordinates (into 'to')
// Assumes: from!=to
{
	if (!wheel)
		return;	

	float angle,sinAngle,cosAngle;

	// Heading
	angle=-wheel->GetHeading();

	if(fabs(angle)<D3_EPSILON)
	{ *to=*from;

	return;
	}
	sinAngle=sin(angle);
	cosAngle=cos(angle);
	// Rotate around Y axis to get heading of car right
	to->x=from->z*sinAngle+from->x*cosAngle;
	to->y=from->y;
	to->z=from->z*cosAngle-from->x*sinAngle;
	//to->DbgPrint("C2T to");
}


void pWheel2::calcVerticalForces()
{
	VxVector downGravity;
	//float mass  = getVehicle()->getBody()->getMass();
	
	NxVec3 grav;
	getVehicle()->getBody()->getActor()->getScene().getGravity(grav);
	

	downGravity.y = -mass * -grav.y ;
	if (getEntity())
	{
		getEntity()->TransformVector(&forceGravityCC,&downGravity);
	}
	
	forceVerticalCC=forceGravityCC+forceRoadTC+WHEEL_SUSPENSION_FORCE;
	
	float inverseWheelMass = mWheelShape->getInverseWheelMass();


}
void pWheel2::calcLongForces()
{
	if (lastContactData)
	{

		//
		// LONGITUDINAL FORCES
		//

		// Differential will do resulting long. torques for the engine

		// Calculate longitudinal road reaction force using Pacejka's magic formula
		float pf;

		// Pacejka
		float factor  = getVehicle()->getEngine()->getForceFeedbackScale();
		float longitudalImpulse = xCheckFloat(lastContactData->longitudalImpulse);

		//qdbg("Pacejka Fx=%f, signU=%f\n",pf,signU);
		forceRoadTC.z= factor * longitudalImpulse * signU ;
	}
		
	


	
}

void pWheel2::calcBreakForces()
{


}


void pWheel2::preAnimate()
{
	
	//----------------------------------------------------------------
	//
	// store last contact
	//
	hadContact = getContact(*lastContactData);
	//----------------------------------------------------------------
	//
	// reset
	//
	forceRoadTC.Set(0,0,0);
	torqueTC.Set(0,0,0);
	torqueFeedbackTC.Set(0,0,0);
	dampingFactorLong=dampingFactorLat=0;
	forceVerticalCC.Set(0,0,0);
	forceDampingTC.Set(0,0,0);
	torqueTC.Set(0,0,0);
	torqueFeedbackTC.Set(0,0,0);
	velWheelTC.Set(0,0,0);
	velWheelCC.Set(0,0,0);





	//signU =  ( getVehicle()->_currentStatus & VS_IsRollingForward ) ? 1.0f : - 1.0f;

	CalcLoad();
	CalcSlipAngle();
	CalcSlipRatio(&velWheelTC);
	
	
	float tanOptimalSA=tan(0.18296f);
	float optimalSR=0.0965f;
	bool make =true;
	
	if(getFrictionMethod() == FC_SLIPVECTOR)
	{
		// Calculate SA/SR vector length (>1 = wanting more than tire supports)

		float lat,lon;
		if (getVehicle()->getProcessOptions() & pVPO_SV_Tansa )
			lat=tanSlipAngle/tanOptimalSA;
		else
			lat=slipAngle/optimalSA;


		lon=slipRatio/optimalSR;
		csSlipLen=sqrtf(lat*lat+lon*lon);
		
		//qdbg("  latNrm=%f, lonNrm=%f, csSlipLen=%f\n",lat,lon,csSlipLen);
	}
	


	CalcSlipVelocity();

	// Low speed detection


	if (getVehicle()->getProcessOptions() & pVPO_CheckLowSpeed)
	{
		float lowSpeed = 0.03f;
		if(rotationV.x>-lowSpeed&&rotationV.x<lowSpeed)
		{ 
			stateFlags|=LOW_SPEED;
			// Calculate factor from 0..1. 0 means real low, 1 is on the edge
			// of becoming high-enough speed.
			lowSpeedFactor=rotationV.x/lowSpeed;
			// Don't zero out all
			if(lowSpeedFactor<0.01f)
				lowSpeedFactor=0.01f;
		}
		else
		{ 
			stateFlags&=~LOW_SPEED;
		}
	}
	CalcPacejka();
	CalcDamping();

	//CalcSlipAngle();
	//CalcSlipRatio(&velWheelTC);
}

void pWheel2::CalcSlipVelocity()
{

	slipVectorTC.x=0;
	slipVectorTC.y=0;
	slipVectorTC.z=rotationV.x*radius;
	
	ConvertTireToCarOrientation(this,&slipVectorTC,&slipVectorCC);

	/*
	VxVector a0(1,2,3);
	VxVector a1(3,4,5);
	ConvertCarToTireOrientation(this,&a0,&a1);*/
	//a1 = {x=1.0522050 y=2.0000000 z=2.9820907 ...}




	slipVectorCC-=velWheelCC;

	// Calculate length of slip
	slipVectorLength=slipVectorCC.Magnitude();

	// Calculate resulting friction coefficient
	// This will be used to determine the tire's maximum force

	float coeff = slipVectorLength/slip2FCVelFactor;
	
	bool make = true;
	/*if(make)
		frictionCoeff=crvSlip2FC->GetValue(coeff);
	else*/
		this->frictionCoeff=1.0f;


	
}
void pWheel2::CalcLoad()
{
	if (hadContact)
	{
		//forceRoadTC.y=-50*0.057598811f;//	float; lastContactData->contactForce;

		if (	(getVehicle()->getProcessOptions() & pVPO_Wheel_UsePHYSX_Load) &&
				(getVehicle()->getProcessOptions() & pVPO_Wheel_UsePHYSX_CONTACT_DATA)
			)
		{
			forceRoadTC.y = load;
		}
		else{

			forceRoadTC.y=-tireRate * WHEEL_PENETRATION_DEPTH;//	float; lastContactData->contactForce;
		}

		stateFlags|=ON_SURFACE;
	}else
	{
		stateFlags&=~ON_SURFACE;
		forceRoadTC.y = 0.0f;
	}

	//load=forceRoadTC.y;

	radiusLoaded=getWheelShape()->getRadius();
}
void pWheel2::calcForces()
{

	if (hadContact)
	{

		  
		float len;


		float pf=0.0f;
		float radiusLoaded = getRadius();
		
		//----------------------------------------------------------------
		//
		// vertical forces
		//
		
		NxVec3 grav;getBody()->getActor()->getScene().getGravity(grav);
		
		VxVector forceGravityWC;
		forceGravityWC.y = getMass() * grav.y;

		if (getEntity())
			getEntity()->TransformVector(&forceGravityCC,&forceGravityWC);

		VxVector suspension;
		suspension.y = -load;
		forceVerticalCC=forceGravityCC+forceRoadTC+suspension;

		

		//----------------------------------------------------------------
		//
		// long forces
		//
		
		//
		// LONGITUDINAL FORCES
		//

		// Differential will do resulting long. torques for the engine

		// Calculate longitudinal road reaction force using Pacejka's magic formula
		// Pacejka
		//float factor  = getVehicle()->getEngine()->getForceFeedbackScale();
		//float longitudalImpulse = xCheckFloat(lastContactData->longitudalImpulse);
		//qdbg("Pacejka Fx=%f, signU=%f\n",pf,signU);
		//forceRoadTC.z= factor * longitudalImpulse * signU ;

		//----------------------------------------------------------------
		//
		// 
		//
		

		#ifdef DO_GREGOR
				pf=Fx;
		#else
				// Pacejka
				pf=pacejka.GetFx()*this->frictionCoeff;
		#endif

		//qdbg("Pacejka Fx=%f, signU=%f\n",pf,signU);
		forceRoadTC.z=signU*pf;
		

		if ( getVehicle()->getProcessOptions() & pVPO_Lat_Damping )
		{
				// Add damping force because of slipRatio vibrations
				// at low speed (SAE950311)
				forceRoadTC.z+=forceDampingTC.z;
				//qdbg("FroadTC.z=%f, Fdamp=%f\n",forceRoadTC.z,forceDampingTC.z);
		}


		//----------------------------------------------------------------
		//
		//	Calculate braking forces & torques (brakes & rolling resistance)
		//
		float curBrakingTorque;
		// Calculate braking torque more directly

		float bTorqueV = getVehicle()->calculateBraking(  getVehicle()->_lastDT );

		curBrakingTorque= bTorqueV * maxBrakingTorque/1000.0f;
		curBrakingTorque*=brakingFactor;
		// Apply braking torque in the reverse direction of the wheel's rotation
		if(rotationV.x>0)
		{ 
			torqueBrakingTC.x=curBrakingTorque;
		} 
		else
		{ 
			torqueBrakingTC.x=-curBrakingTorque;
		}
		forceBrakingTC.z=torqueBrakingTC.x/radiusLoaded;

		// Calculate feedback torque (goes back to differential)
		// This doesn't include braking and rolling resistance, which are
		// potential braking forces (which may not be fully used if the
		// wheel is not rotating), but only the forces which are always
		// there, like road reaction.
		torqueFeedbackTC.x=-forceRoadTC.z*radiusLoaded;

		//----------------------------------------------------------------
		//
		// 
		//
		// Calculate rolling resistance (from John Dixon, Fr=u*Fv
		// where Fv is the normal force, and 'u' the rolling coefficient)
		// Rolling coefficient may need to go up with speed though
		//forceRoadTC.y = pf;


		if(rotationV.x>0)
			torqueRollingTC.x=-rollingCoeff*forceRoadTC.y*radiusLoaded;
		else
			torqueRollingTC.x=rollingCoeff*forceRoadTC.y*radiusLoaded;

		// Calculate total braking torque (this is POTENTIAL, not always acted upon!)
		torqueBrakingTC.x=-forceBrakingTC.z*radiusLoaded;
		torqueBrakingTC.x+=torqueRollingTC.x;

		//
		// lateral forces :
		//
		/*float lateralForce = xCheckFloat(lastContactData->lateralImpulse);
		factor = getVehicle()->getEngine()->getForceFeedbackScale();
		forceRoadTC.x= signU*lateralForce * factor  ;
		*/

		#ifdef DO_GREGOR
				pf=Fy;
		#else
				pf=pacejka.GetFy()*frictionCoeff;
		#endif
				//pf=forceRoadTC.x;
		
		#ifdef OBS
				qdbg("Pacejka Fy(lat)=%f, SR=%f, SA=%f, load=%f\n",pf,slipRatio,slipAngle,load);
		#endif
				
		forceRoadTC.x=pf;

		
		if(getProcessOptions() & pVPO_Forces_No_Lateral)
		{
			// Cancel lateral forces
			pacejka.SetFy(0);
			forceRoadTC.x=0;
		}

		if (getProcessOptions() & pVPO_Lat_Damping )
		{
		
				// Add damping force because of slipAngle vibrations
				// at low speed (SAE950311)
				// Note the mix of 2 coordinate systems here
				forceRoadTC.x+=forceDampingTC.x;
				//qdbg("Damping fRoad.x=%f, Fdamp=%f\n",forceRoadTC.x,forceDampingTC.x);
		}
		
		// WHEEL LOCKING

		if(getVehicle()->getProcessOptions() & pVPO_Wheel_LockAdjust )
			goto skip_wla;
		
		
		
		if(slipRatio<0)
		{
			
			VxVector forceLockedCC,forceLockedTC;
			float slideFactor,oneMinusSlideFactor,lenSlip,lenNormal,y;

			// As the wheel is braked, more and more sliding kicks in.
			// At the moment of 100% slide, the braking force points
			// in the reverse direction of the slip velocity. Inbetween
			// SR=0 and SR=-1 (and beyond), there is more and more sliding,
			// so the force begins to point more and more like the slip vel.

			
			// Calculate sliding factor (0..1)
			if(slipRatio<-1.0f)
				slideFactor=1.0f;
			else               
				slideFactor=-slipRatio;

			//slideFactor*=.75f;
			oneMinusSlideFactor=1.0f-slideFactor;
			// Calculate 100% wheel lock direction
			forceLockedCC.x=slipVectorCC.x;
			forceLockedCC.y=0;
			forceLockedCC.z=slipVectorCC.z;
			// Make it match forceRoadTC's coordinate system
			
			ConvertCarToTireOrientation(this,&forceLockedCC,&forceLockedTC);


			// Calc magnitude of normal and locked forces
			lenSlip=forceLockedTC.Magnitude();

			y=forceRoadTC.y; forceRoadTC.y=0;
			lenNormal=forceRoadTC.Magnitude();
			forceRoadTC.y=y;
			if(lenSlip<D3_EPSILON)
			{
				// No force
				forceLockedTC.Set(0,0,0);
				} else
			{
				// Equalize force magnitude
			forceLockedTC *=(lenNormal/lenSlip);
			}

			// Interpolate between both extreme forces
			forceRoadTC.x=oneMinusSlideFactor*forceRoadTC.x+slideFactor*forceLockedTC.x;
			forceRoadTC.z=oneMinusSlideFactor*forceRoadTC.z+slideFactor*forceLockedTC.z;

		}



		skip_wla:

	
		// ALIGNING TORQUE

		aligningTorque=pacejka.GetMz();
		// Damp aligning torque at low speed to avoid jittering of wheel
		// Friction will take care of forces in that case
		len=velWheelCC.SquareMagnitude();
		if(len<1.0)
		{
			// Damp
			aligningTorque*=len;
		}

		//----------------------------------------------------------------
		//
		// aligning torque
		//


		// ALIGNING TORQUE

		aligningTorque=pacejka.GetMz();

		// Damp aligning torque at low speed to avoid jittering of wheel
		// Friction will take care of forces in that case
		
		len=velWheelCC.SquareMagnitude();
		if(len<1.0)
		{
			// Damp
			aligningTorque*=len;
		}
	

		if((stateFlags&LOW_SPEED))
		{
			//qdbg("LS: factor=%f\n",lowSpeedFactor);
			// Long. force is often too high at low speed
			forceRoadTC.z*=lowSpeedFactor;
		}
		
}

}

void pWheel2::Integrate()
{

	VxVector translation;
	float   oldVX;


	float mTorque = getVehicle()->getGearBox()->GetTorqueForWheel(this);

	//
	// ROTATIONAL acceleration
	//
	oldVX=rotationV.x;

	
	//----------------------------------------------------------------
	//
	// 
	//
	//
	// ROTATIONAL acceleration
	//
	//#define OBS
	#ifdef OBS
		// Check for locked wheel braking
		//float netForce= getVehicle()->getEngine()->forEngin .z+forceRoadTC.z;
		//qdbg("Fe=%f, Fr=%f, Fb=%f\n",forceEngineTC.z,forceRoadTC.z,forceBrakingTC.z);
		if(rotationV.x>-RR_EPSILON_VELOCITY&&rotationV.x<RR_EPSILON_VELOCITY)
		{
			if((netForce<0&&forceBrakingTC.z>-netForce)||
				(netForce>0&&forceBrakingTC.z<-netForce))
				//if(forceBrakingTC.z<-netForce||forceBrakingTC.z>netForce)
			{
				//qdbg("RWh:Int; braking force keeps wheel still\n");
				rotationV.x=0;
				goto skip_rot;
			}
		}
	#endif

	
		

	float timeStep= lastStepTimeSec;


	
	float a = rotationA.x*timeStep ;
	rotationV.x+=rotationA.x*timeStep;
	//skip_rot:

	// Check for wheel velocity reversal; in this case, damp the
	// velocity to a minimum to get rid of jittering wheels at low
	// speed and/or high braking.
	if(differential)
	{
		if(oldVX>0&&rotationV.x<0)
		{
			// Lock the side
			//qdbg("RWh%d; vel reversal, lock\n",wheelIndex);
			differential->Lock(differentialSide);
			rotationV.x=0;
			differentialSlipRatio=0;
		} else if(oldVX<0&&rotationV.x>0)
		{
			//qdbg("RWh%d; vel reversal, lock\n",wheelIndex);
			// Lock the side
			differential->Lock(differentialSide);
			rotationV.x=0;
			differentialSlipRatio=0;
		}
	} else
	{
		// No differential, use controlled jittering
		if(oldVX>=0&&rotationV.x<0)
		{
			// Just lift the rotation over the 0 barrier; this will induce
			// jittering near 0, but it's so small that it's unnoticeable.
			// If we keep it at 0, you get larger jitters either way (+/-)
			// until the wheel rotates again and starts the reversal again.
			rotationV.x=-0.0001;
		} else if(oldVX<=0&&rotationV.x>0)
		{
			rotationV.x=0.0001;
		}
	}
	//qdbg("  rotV after velocity reversal=%f\n",rotationV.x);

	// Wheel rotation (spinning)
	//

	float rotVx =rotation.x;
	rotation.x+=rotationV.x*timeStep;
	//rotation.x+=(rotationV.x+0.5f*rotationA.x*timeStep)*timeStep;
	//rotation.x+=rotationV.x*timeStep;
	// Keep rotation in limits
	while(rotation.x>=2*PI)
		rotation.x-=2*PI;
	while(rotation.x<0)
		rotation.x+=2*PI;

	// Friction reversal measures
	if( (oldVX<0&&rotationV.x>0) || (oldVX>0&&rotationV.x<0) )
	{ 
		rotationV.x=0;
	
		//qdbg("RWh:Int; friction reversal halt; %f -> %f\n",oldVX,rotationV.x);
	}
}

void pWheel2::CalcWheelAngAcc()
{
	if(differential)
	{
		// Use differential to determine acceleration
		if(differential->IsLocked(differentialSide))
		{
			// Wheel is held still by braking torques
			rotationA.x=0;
		} else
		{
			// Wheel is allowed to roll
			rotationA.x=differential->GetAccOut(differentialSide);
		}
		//qdbg("diff=%p, diffSide=%d\n",differential,differentialSide);
	} else
	{
		// Uses torque directly (no engine/differential forces)
		rotationA.x=(torqueFeedbackTC.x+torqueBrakingTC.x)/GetInertia();
	}
}

void pWheel2::CalcAccelerations()
{

	rotationA.Set(0,0,0);

	CalcWheelAngAcc();
	//CalcBodyForce();

	// Vertical forces; suspension, gravity, ground, tire rate
	//acceleration=forceVerticalCC/GetMass();
	acceleration.x=acceleration.z=0.0f;
	acceleration.y=forceVerticalCC.y/mass;



#ifdef OBS
	qdbg("RWh:AF; forceVerticalCC.y=%f, mass=%f\n",forceVerticalCC.y,MASS);
#endif

}



void pWheel2::CalcSlipRatio(VxVector *velWheelCC)
{

	//rfloat vGround,vFreeRolling;

	float lastTime =lastStepTimeSec;

	/*
	lastTime*=0.1f;
*/

	//velWheelTC->DbgPrint("velWheelTC in CalcSR");
	if(!hadContact)
	{
		//qdbg("CalcSR; not on surface\n");
		// Not touching the surface, no slip ratio
		slipRatio=0;
		// Tire vibrations stop
		differentialSlipRatio=0;
		return;
	}

	// SAE950311 algorithm
	float u,delta,B;
	u=velWheelTC.z;
	B=relaxationLengthLong;

	// Switch to normal slip ratio calculations when there is
	// speed (in the case when the velocities catch up on the timestep)
	// Notice this test can be optimized, because the timestep and B
	// are constants, so a 'turningPointU' can be defined.
	//qdbg("Threshold: %f\n",u*RR_TIMESTEP/B);
	
	float t0 = u*lastTime;
	bool tc = B>0.5  ? true : false;


	
	if(u*lastTime/B>0.5)
	{
		//----------------------------------------------------------------
		//
		// changed : 
		//
		//float wheelSlipRatio 
		
		slipRatio=rotationV.x*radiusLoaded/u-1;
		//slipRatio = lastContactData->longitudalSlip;
		
		return;
		// Use straightforward slip ratio
		slipRatio=rotationV.x*radiusLoaded/u-1;
		//qdbg("'u' big enough; straight SR %f\n",slipRatio);
		return;
	}

	if((lastU<0&&u>=0)||(lastU>=0&&u<0))
	{
		// 'u' changed sign, reverse slip
		//qdbg("'u' changed sign from %f to %f; SR=-SR\n",u,lastU);
		//differentialSlipRatio=-differentialSlipRatio;
		// Damp while reversing slip; we're close to a standstill.
		float dampSRreversal = 0.2f;
		differentialSlipRatio=-dampSRreversal*differentialSlipRatio;
	}
	lastU=u;
	// Detect sign of 'u' (real velocity of wheel)
	if(u<0)signU=-1.0f; else signU=1.0f;

	// Eq. 26
	//delta=(fabs(u)-rotationV.x*radius*signU)/B-(fabs(u)/B)*differentialSlipRatio;
	if(u<0)
	{
		// Eq. 25
		delta=(-u+rotationV.x*radius)/B+(u/B)*differentialSlipRatio;
	} else
	{
		// Eq. 20
		delta=(u-rotationV.x*radius)/B-(u/B)*differentialSlipRatio;
	}

	
	// Integrate
	differentialSlipRatio+=delta*lastTime;
	if(differentialSlipRatio>10)differentialSlipRatio=10;
	else if(differentialSlipRatio<-10)differentialSlipRatio=-10;
	// Set slip ratio for the rest of the sim. Note that
	// we use a different slip ratio definition from that of SAE950311
	//qdbg("  old SR=%f => new SR=%f\n",slipRatio,-differentialSlipRatio);
	slipRatio=-differentialSlipRatio;


}

void pWheel2::updatePosition()
{
	NxMat34 &wheelPose = getWheelPose();
	NxWheelContactData wcd; 
	NxShape* contactShape = mWheelShape->getContact(wcd);
	NxReal  stravel = mWheelShape->getSuspensionTravel();
	NxReal radius = mWheelShape->getRadius();
	//have ground contact?
	if( contactShape && wcd.contactPosition <=  (stravel + radius) ) {
		wheelPose.t = NxVec3( wheelPose.t.x, wcd.contactPoint.y + getRadius(), wheelPose.t.z );
	}
	else {
		wheelPose.t = NxVec3( wheelPose.t.x, wheelPose.t.y - getSuspensionTravel(), wheelPose.t.z );
	}
}

void pWheel2::updateSteeringPose(float rollangle,float steerAngle,float dt)
{
	
	float rollAngle = getWheelRollAngle();

	rollAngle+=getWheelShape()->getAxleSpeed() * (dt * 0.01f);

	
	while (rollAngle > NxTwoPi)	//normally just 1x
		rollAngle-= NxTwoPi;
	while (rollAngle< -NxTwoPi)	//normally just 1x
		rollAngle+= NxTwoPi;

	setWheelRollAngle(rollAngle);

	NxMat34& wheelPose = mWheelShape->getGlobalPose();


	NxReal  stravel = mWheelShape->getSuspensionTravel();
	NxReal radius = mWheelShape->getRadius();



	float rAngle = getWheelRollAngle();
	float steer = mWheelShape->getSteerAngle();


	NxVec3 dir;

	NxReal r = mWheelShape->getRadius();
	NxReal st = mWheelShape->getSuspensionTravel();

	
	wheelPose.M.getColumn(1, dir);
	dir = -dir;	//cast along -Y.
	NxReal castLength = r + st;	//cast ray this long


	NxMat33 rot, axisRot, rollRot;
	rot.rotY( mWheelShape->getSteerAngle() );
	axisRot.rotY(0);
	rollRot.rotX(rAngle);
	wheelPose.M = rot * wheelPose.M * axisRot * rollRot;

	setWheelPose(wheelPose);

}









void pWheel2::CalcSlipAngle()
// Based on the wheel world velocity and heading, calculate
// the angle from the wheel heading to the wheel's actual velocity
// Contrary to SAE, our Y-axis points up, and slipAngle>0 means
// in our case that the you're in a right turn (basically)
// 31-03-01: SAE950311 implementation (using relaxation lengths
// and a differential equation for the slip angle) for low-speed etc.
{


	VxVector velWheelWC;

	if(!hadContact)
	{
		//qdbg("CalcSA; not on surface\n");
		// Not touching the surface, no need to do slip angle
		slipAngle=0;
		// Tire springs back to 0 instantly (should be slowly actually)
		tanSlipAngle=0;
		velWheelTC.Set(0,0,0);
		velWheelCC.Set(0,0,0);
		return;
	}


	int options = getProcessOptions();
	
	if (options & pVPO_Wheel_UsePHYSX_CONTACT_DATA )
	{

		/*
		VxVector dir,up,right;
		getEntity()->GetOrientation(&dir,&up,&right);
		*/

		VxVector point= lastContactData->contactPoint;
		if( lastContactData->contactPosition <=  ( getWheelShape()->getSuspensionTravel() + radius) ) 
			point.y+=getRadius();
		else
			point.y = getWheelPose().t.y - getSuspensionTravel();

		//getEntity()->TransformVector(&point,&point);
/*
		getBody()->GetVT3DObject()->InverseTransformVector(&point,&point);

		/*
		
		getEntity()->InverseTransformVector(&contactLocal,&point);

		VxVector contactLocal2;
		getEntity()->InverseTransform(&contactLocal,&point);
*/

		getBody()->GetVT3DObject()->InverseTransform(&point,&point);

		velWheelCC = getBody()->getLocalPointVelocity(point);
		//getEntity()->TransformVector(&velWheelCC,&velWheelCC);
		ConvertCarToTireOrientation(this,&velWheelCC,&velWheelTC);


	}

	if ( options & pVPO_SA_Delay )
	{
		// Derive change in tan(slipAngle) using SAE950311
		// u=longitudinal velocity, v=lateral velocity
		float u,b,v,delta;
		u=velWheelTC.z;
		v=velWheelTC.x;
		b=relaxationLengthLat;

		if ( options  & pVPO_SA_DownSettle)
		{
				// Make sure SA settles down at low speeds
				float min=5.0f;
				if(u<min&&u>=0.0f)u=min;
				else if(u>-min&&u<=0)u=-min;
		}
			

		// Calculate change in tan(SA)
		delta=(v/b)-(fabs(u)/b)*tanSlipAngle;
		if(  getWheelFlag(WF_Accelerated)  && getVehicle() )
		{
			//qdbg("CSA; u=%f, v=%f, delta=%f, tanSA=%f, SA=%f\n",u,v,delta,tanSlipAngle,slipAngle);
			//	qdbg("delta=%f, tanSA=%f, SA=%f\n",delta,tanSlipAngle,atan(tanSlipAngle));
			//	Integrate
			tanSlipAngle+=delta*lastStepTimeMS;
			// Derive new slip angle from state variable 'tanSlipAngle'
			slipAngle=atanf(tanSlipAngle);

		}
	}else
	{

		// Calculate wheel velocity because of rotation around yaw (Y) axis
		
		/*velWheelCC.x+=car->GetBody()->GetRotVel()->y*cos(-angleCGM)*distCGM;
		velWheelCC.z+=car->GetBody()->GetRotVel()->y*sin(-angleCGM)*distCGM;
		ConvertCarToTireOrientation(&velWheelCC,&velWheelTC);
		*/

		/*
		// Get velocity of contact patch wrt the track (=world)
		DVector3 cpPos,*p;
		p=susp->GetPosition();
		// Taking the tire movement from really the contact patch location
		// seems to overrate the lateral movement the tire goes through.
		// This is due because the body rolls, but normally the suspension
		// won't roll the tire in exactly the same way, but instead,
		// the tire rotates with respect to the body, so the lateral
		// movements are actually quite small.
		// To simulate this, I approximate the movement with respect
		// to the suspension attachment point, which moves far less
		// laterally. This became apparent when dealing with much stiffer
		// tire and suspension spring rates, which made the car wobbly.
		// Taking a less laterally sensitive position solved this.
		cpPos.x=p->x;
		//cpPos.y=p->y+susp->GetLength()+radius;
		cpPos.y=p->y;
		cpPos.z=p->z;               // Kingpin effects are ignored
		car->GetBody()->CalcBodyVelForBodyPos(&cpPos,&velWheelCC);
		car->ConvertCarToWorldOrientation(&velWheelCC,&velWheelWC);
		ConvertCarToTireOrientation(&velWheelCC,&velWheelTC);
		*/

		//qdbg("velWheelTC.z=%f, rotZ=%f\n",velWheelTC.z,rotationV.x*radius);

		// Non-SAE950311 method (no lag in slip angle buildup)
		// Calculate slip angle as the angle between the wheel velocity vector
		// and the wheel direction vector
		
		if(velWheelCC.x>-RR_EPSILON_VELOCITY&&velWheelCC.x<RR_EPSILON_VELOCITY &&
			velWheelCC.z>-RR_EPSILON_VELOCITY&&velWheelCC.z<RR_EPSILON_VELOCITY)
		{  
			//Very low speed; no slip angle
			//slipAngle=-GetHeading();
			slipAngle=0;
			//qdbg("  RWh;CSA; LS => slipAngle=0\n");
		} 
		else
		{ 
			// Enough velocity, calculate real angle
			float h = GetHeading();

			float realSteer = mWheelShape->getSteerAngle();

			slipAngle=atan2(velWheelCC.x,velWheelCC.z)-GetHeading();

			if (getProcessOptions() & pVPO_SA_Damping )
			{
				// Damp down at low velocity
				if(fabs(velWheelCC.x)<0.3f&&fabs(velWheelCC.z)<0.3f)
				{
					float max=fabs(velWheelCC.x);
					if(fabs(velWheelCC.z)<max)
						max=fabs(velWheelCC.z);
					//qdbg("damp SA %f factor %f => %f\n",slipAngle*RR_RAD2DEG,max,slipAngle*max);
					slipAngle*=max*0.5f;
				}
			}
			// Keep slipAngle between -180..180 degrees
			if(slipAngle<-PI)
				slipAngle+=2*PI;
			else if(slipAngle>PI)
				slipAngle-=2*PI;
		
		}
		if ( options & pVPO_SV_Tansa)
		{
			bool make = true ;
			if(make)
			{
				// We need tan(SA) when combining Pacejka Fx/Fy later
				tanSlipAngle=tan(slipAngle);
				//qdbg("FCSV: tanSA=%f\n",tanSlipAngle);
			}
		}
	}
}