#include <StdAfx.h>
#include "vtPhysXAll.h"

#include <xDebugTools.h>

#include "pDifferential.h"

#include "pVehicleAll.h"

#include "pWorldCallbacks.h"



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
#define DAMP_VERTICAL_VELOCITY_REVERSAL

// Damp the wheel when crossing the equilibrium position?
// As the wheel passes its center position, the spring force reverses. To
// avoid adding energy into the system, when passing this position, damping
// is used to avoid overaccelerating the tire to the other side.
// Note that this acts when the wheel is near its center (rest) position,
// contrast this with DAMP_VERTICAL_VELOCITY_REVERSAL.
//#define DAMP_VERTICAL_EQUILIBRIUM_REVERSAL

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
//#define USE_SA_DAMPING_AT_LOW_SPEED

// Wheel locking results in force going the direction of -slipVector?
//#define USE_WHEEL_LOCK_ADJUST


void pWheel2::CalcDamping()
{

	// Calculate damping of the tire at low speed, to avoid
	// oscillations.
	float u,v,b,B;

	if ( getVehicle()->getProcessOptions() & pVPO_Lat_Damping || getVehicle()->getProcessOptions() & pVPO_Long_Damping )
	{
		

		if(!hadContact)
			return;

		u=velWheelTC.z;
		v=velWheelTC.x;
	
		if (getVehicle()->getProcessOptions() & pVPO_Lat_Damping)
			b=relaxationLengthLat;
		
				
		if (getVehicle()->getProcessOptions() & pVPO_Long_Damping)
			B=relaxationLengthLong;
	}

	
	

		
	if ( getVehicle()->getProcessOptions() & pVPO_Lat_Damping  )
	{

	
	
		// Calculate damping force, to be added later
		// Assumes CalcLoad() has already been called (for 'load' to be valid)
		//qdbg("u=%f, v=%f\n",u,v);
		//if(velWheelTC.LengthSquared()<dampingSpeed*dampingSpeed&&load>0)
		if(fabs(v)<dampingSpeed)
			//if(velWheelTC.Length()<dampingSpeed)
			//if(car->GetBody()->GetLinVel()->Length()<dampingSpeed)
			//if(car->GetBody()->GetRotVel()->y<dampingSpeed&&load>0)
			//if(load>0)
			//if(IsLowSpeed()==TRUE&&load>0)
		{

			NxVec3 grav;getBody()->getActor()->getScene().getGravity(grav);

			forceDampingTC.x=2.0f*dampingCoefficientLat*v*
				sqrt((load*(pacejka.GetCorneringStiffness()) /-grav.y*b));
			forceDampingTC.x*=rollingCoeff;

	#ifdef OBS
			dampingFactorLat=(fabs(car->GetBody()->GetRotVel()->y))*1.0f;
	#endif
			//if(dampingFactorLat<0.1f)dampingFactorLat=0.1f;
			//(dampingSpeed-fabs(car->GetBody()->GetRotVel()->y))*1.0f;
	#ifdef OBS
			// Factor damping
			dampingCoefficient=1.0f;
			dampingFactorLat=dampingCoefficient*(dampingSpeed-fabs(v));
			if(dampingFactorLat>1)dampingFactorLat=1;

			//slipAngle*=dampingFactorLat;
			dampingFactorLat=0;
	#endif

			//dampingFactorLat=fabs(v)*3;
		}

		NxVec3 grav;getBody()->getActor()->getScene().getGravity(grav);

		// Natural frequency (debug)
		float natFreq;
		natFreq=sqrtf(-grav.y*pacejka.GetCorneringStiffness()/load*b);

//		#ifdef OBS 		qdbg("W%d; natFreq=%f ~ %f Hz\n",wheelIndex,natFreq,natFreq/6.28f);		car->GetBody()->GetLinVel()->DbgPrint("body linVel");#endif

		forceRoadTC.x=pacejka.GetFy();
		//qdbg("Flat=%.2f, dampFlat=%.2f, cornStiffness=%.f, load %.f, v=%f\n",
		//	forceRoadTC.x,forceDampingTC.x,pacejka.GetCorneringStiffness(),load,v);
	}

	if (getVehicle()->getProcessOptions() & pVPO_Long_Damping)
	{

		// Calculate damping force, to be added later
		// Assumes CalcLoad() has already been called (for 'load' to be valid)
		if(fabs(u)<dampingSpeed)
			//if(velWheelTC.Length()<dampingSpeed)
			//if(velWheelTC->LengthSquared()<dampingSpeed*dampingSpeed&&load>0)
		{
			
			//----------------------------------------------------------------
			//
			// get wheel load 
			//
		

				
			NxVec3 grav;getBody()->getActor()->getScene().getGravity(grav);
			forceDampingTC.z=2.0f*dampingCoefficientLong*u*sqrt((load*pacejka.GetLongitudinalStiffness())/(-grav.y*B));
			forceDampingTC.z*=rollingCoeff;
			#ifdef OBS
				qdbg("Fz=%f, dampFlon=%f, longStiffness=%.f, load %.f, u=%f, v=%f,\n",
				pacejka.GetFx(),forceDampingTC.z,pacejka.GetLongitudinalStiffness(),load,u,v);
			#endif
			
		}
			//qdbg("dampFactorLong=%f\n",dampingFactorLong);
	}
}


float pWheel2::getMass()
{
	return mass;

}
