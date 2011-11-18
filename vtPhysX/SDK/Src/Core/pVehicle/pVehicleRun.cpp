#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "pVehicleMotor.h"
#include "pVehicleGears.h"
#include "pSteer.h"
#include "pGearbox.h"
#include <xDebugTools.h>
#include "NxArray.h"

#include "pVehicleAll.h"


#include "virtools/vtTools.h"

using namespace vtTools::AttributeTools;
using namespace vtTools::ParameterTools;
using namespace vtTools::BehaviorTools;

void pVehicle::step(float dt)
{


	if (isValidEngine())
	{
		//----------------------------------------------------------------
		//
		// update user controls
		//
		steer->SetInput(_cSteering);doSteering();

		engine->updateUserControl(_cAcceleration);

		if (	(_currentStatus & VS_IsAcceleratedForward ) ||
			(_currentStatus & VS_IsAcceleratedBackward)
			)
		{
			driveLine->SetInput(1000.0f,_cHandbrake);
		}
		if (!(_currentStatus & VS_IsAccelerated) )
		{
			driveLine->SetInput(0.0f,_cHandbrake);
		}

		//----------------------------------------------------------------
		//
		// 
		//

		preAnimate();
		
		
		engine->CalcForces();
		for(NxU32 i = 0; i < _wheels.size(); i++)
		{
			pWheel2 *cW = (pWheel2*)_wheels[i];
			if (cW->isAxleSpeedFromVehicle() || cW->isTorqueFromVehicle() )
				cW->calcForces();
		}

		driveLine->CalcForces();


		//////////////////////////////////////////////////////////////////////////
	
			// Now that engine and wheel forces are unknown, check the diffs
			for(int i=0;i<differentials;i++)
				differential[i]->CalcForces();
		
		//////////////////////////////////////////////////////////////////////////

		driveLine->CalcAccelerations();


		for(NxU32 i = 0; i < _wheels.size(); i++)
		{
			pWheel2 *cW = (pWheel2*)_wheels[i];
			if (cW->isAxleSpeedFromVehicle() || cW->isTorqueFromVehicle() )
				cW->CalcAccelerations();
		}
		//////////////////////////////////////////////////////////////////////////
		#ifdef OBS_RPM_IS_NEW
				// Engine RPM is related to the rotation of the powered wheels,
				// since they are physically connected, somewhat
				// Take the minimal rotation
				float minV=99999.0f,maxV=0;
				for(int i=0;i<_wheels.size();i++)
				{
					pWheel2 *cW = (pWheel2*)_wheels[i];
					if (cW && (cW->getWheelFlag(WF_Accelerated) && ( cW->getWheelShape()->getWheelFlags() &  WSF_AxleSpeedOverride))						)
					{
						if(cW->GetRotationV()>maxV)
							maxV=cW->GetRotationV();
							
							#ifdef OBS_HMM
								if(cW[i]->GetRotationV()<minV)
									minV=cW->GetRotationV();
							#endif

					}
				}
				/*engine->
				engine->ApplyWheelRotation(maxV);*/

		#endif
		//////////////////////////////////////////////////////////////////////////
		steer->Integrate();

		for(NxU32 i = 0; i < _wheels.size(); i++)
		{
			pWheel2 *cW = (pWheel2*)_wheels[i];
			if (cW->isAxleSpeedFromVehicle() || cW->isTorqueFromVehicle() )
				cW->Integrate();
		}

		
		driveLine->Integrate();

		
		for(int i=0;i<differentials;i++)
		{
			getDifferential(i)->Integrate();
		}

		gearbox->processFutureGear();

	}
}
void pVehicle::processPreScript()
{

}
void pVehicle::processPostScript()
{
	CKBehavior * beh  = (CKBehavior*)GetPMan()->GetContext()->GetObject(postScript);
	if (beh)
	{
		SetInputParameterValue<int>(beh,0,5);
		beh->Execute(_lastDT);
		 GetOutputParameterValue<int>(beh,0);
	}
}
int pVehicle::onPostProcess()
{

	/*
	if (getBody()->isSleeping())
		getBody()->wakeUp();
	*/



	_lastDT = lastStepTimeMS;


	_currentStatus = _calculateCurrentStatus();
	
	VehicleStatus status = (VehicleStatus)_currentStatus;
	float bTorque = calculateBraking(_lastDT);
	
	if (_cSteering != 0 || _cAcceleration != 0 || _cHandbrake)
		getActor()->wakeUp(0.05);

	if (isValidEngine())		//new vehicle code
		step(_lastDT);
	else						//old
		_performSteering(_lastDT );


	if ( !(flags & VF_UseAdvance ))
	{
		_performAcceleration(_lastDT);
		postStep();
	}else
	{
		for(NxU32 i = 0; i < _wheels.size(); i++) 
		{

			pWheel2* wheel = (pWheel2*)_wheels[i];
			

			if( wheel->isTorqueFromVehicle())
				wheel->applyTorqueToPhysics();

			if(	wheel->isAxleSpeedFromVehicle() )
			{
/*					float v = wheel->rotationV.x;
					v = v *  getEngine()->getEndRotationalFactor() * getEngine()->getTimeScale();	

*/					//wheel->getWheelShape()->setAxleSpeed(v);

/*
					pDifferential *diff = getDifferential(0);
					float tOutOne = diff->GetTorqueOut(wheel[0]->differentialSide);
					float tOutSeconf = diff->GetTorqueOut(wheel[1]->differentialSide);


*/

			}
		}
	}
	for(NxU32 i = 0; i < _wheels.size(); i++) 
	{
		pWheel2* wheel = (pWheel2*)_wheels[i];
		wheel->updateSteeringPose( wheel->getWheelRollAngle(),wheel->getWheelShape()->getSteerAngle(),_lastDT);
		wheel->updatePosition();
	}

	
	setVSFlags(_currentStatus);

	processPostScript();

	updateControl(0,false,0,false,false);


	return 1;
}

int pVehicle::onPreProcess()
{
	_lastDT = lastStepTimeMS;

	
	return 1;
}



void pVehicle::updateVehicle( float lastTimeStepSize )
{

	return;

	_lastDT = lastTimeStepSize;

	_currentStatus = _calculateCurrentStatus();

	VehicleStatus status = (VehicleStatus)_currentStatus;


	if (_cSteering != 0 || _cAcceleration != 0 || _cHandbrake)
		getActor()->wakeUp(0.05);

	
	float bTorque = calculateBraking(lastTimeStepSize);
	
	

	_performSteering(lastTimeStepSize);
	if (engine && gearbox && driveLine )
	{
		step(lastTimeStepSize);
	}


	if ( !(flags & VF_UseAdvance ))
	{
		_performAcceleration(lastTimeStepSize);
		postStep();
	}else
	{
		for(NxU32 i = 0; i < _wheels.size(); i++) 
		{

			
			pWheel2* wheel = (pWheel2*)_wheels[i];
/*			if( (wheel->getWheelFlag(WF_Accelerated)) && (wheel->getWheelShape()->getWheelFlags() & WSF_AxleSpeedOverride ))
			{
				
				float v = wheel->rotationV.x *= 1.0f;

				wheel->getWheelShape()->setAxleSpeed(  v );
			}
			*/
		}
	}
	for(NxU32 i = 0; i < _wheels.size(); i++) 
	{
		pWheel2* wheel = (pWheel2*)_wheels[i];
		wheel->updateSteeringPose( wheel->getWheelRollAngle(),wheel->getWheelShape()->getSteerAngle(),lastTimeStepSize);
		wheel->updatePosition();
	}

	if (getMotor())
	{
		_currentStatus |= E_VSF_HAS_MOTOR;
	}

	if (getGears())
	{
		_currentStatus |= E_VSF_HAS_GEARS;
	}

	setVSFlags(_currentStatus);

	
	
	return;

	//----------------------------------------------------------------
	//
	// old code 
	//

	//control(_cSteering,_cAnalogSteering,_cAcceleration,_cAnalogAcceleration,_cHandbrake);
	//printf("updating %x\n", this);

	NxReal distanceSteeringAxisCarTurnAxis = _steeringSteerPoint.x  - _steeringTurnPoint.x;
	NX_ASSERT(_steeringSteerPoint.z == _steeringTurnPoint.z);
	NxReal distance2 = 0;
	if (NxMath::abs(_steeringWheelState) > 0.01f)
		distance2 = distanceSteeringAxisCarTurnAxis / NxMath::tan(_steeringWheelState * _steeringMaxAngleRad);
	
	NxU32 nbTouching = 0;
	NxU32 nbNotTouching = 0;    
	NxU32 nbHandBrake = 0;
	int wSize = _wheels.size();
	for(NxU32 i = 0; i < _wheels.size(); i++)
	{
		pWheel* wheel = _wheels[i];
		if(wheel->getWheelFlag(WF_SteerableInput))
		{
			if(distance2 != 0)
			{
				NxReal xPos = wheel->getWheelPos().x;
				NxReal xPos2 = _steeringSteerPoint.x- wheel->getWheelPos().x;
				NxReal zPos = wheel->getWheelPos().z;
				NxReal dz = -zPos + distance2;
				NxReal dx = xPos - _steeringTurnPoint.x;
				float atan3 = NxMath::atan(dx/dz);

				float angle =(NxMath::atan(dx/dz));
				if (dx < 0.0f)
				{
					angle*=-1.0f;
				}
				wheel->setAngle(angle);
				//errMessage.Format("w%d dz:%f dx:%f dx2%f distance:%f atan3:%f",i,dz,dx,xPos2,distance2,atan3);
				//xInfo(errMessage.Str());


			} else {
				wheel->setAngle(0.0f);
			}
			//printf("%2.3f\n", wheel->getAngle());

		} else if(wheel->getWheelFlag(WF_SteerableAuto))
		{
			NxVec3 localVelocity = getActor()->getLocalPointVelocity(getFrom(wheel->getWheelPos()));
			NxQuat local2Global = getActor()->getGlobalOrientationQuat();
			local2Global.inverseRotate(localVelocity);
			//			printf("%2.3f %2.3f %2.3f\n", wheel->getWheelPos().x,wheel->getWheelPos().y,wheel->getWheelPos().z);
			localVelocity.y = 0;
			if(localVelocity.magnitudeSquared() < 0.1f)
			{
				wheel->setAngle(0.0f);
			} else {
				localVelocity.normalize();
				//				printf("localVelocity: %2.3f %2.3f\n", localVelocity.x, localVelocity.z);
				if(localVelocity.x < 0)
					localVelocity = -localVelocity;
				NxReal angle = NxMath::clamp((NxReal)atan(localVelocity.z / localVelocity.x), 0.3f, -0.3f);
				wheel->setAngle(angle);
			}
		}

		// now the acceleration part
		if(!wheel->getWheelFlag(WF_Accelerated))
			continue;

		if(_handBrake && wheel->getWheelFlag(WF_AffectedByHandbrake))
		{
			nbHandBrake++;
		} 
		else
		{
			if (!wheel->hasGroundContact())
			{
				nbNotTouching++;
			} else {
				nbTouching++;
			}
		}
	}

	NxReal motorTorque = 0.0; 
	float _acc = NxMath::abs(_accelerationPedal);
	
	XString errMessage;
	if(nbTouching && NxMath::abs(_accelerationPedal) > 0.1f ) 
	{
		NxReal axisTorque = _computeAxisTorque();
		NxReal wheelTorque = axisTorque / (NxReal)(_wheels.size() - nbHandBrake);
		NxReal wheelTorqueNotTouching = nbNotTouching>0?wheelTorque*(NxMath::pow(0.5f, (NxReal)nbNotTouching)):0;
		NxReal wheelTorqueTouching = wheelTorque - wheelTorqueNotTouching;
		motorTorque = wheelTorqueTouching / (NxReal)nbTouching; 
	} else {
		_updateRpms();
	}
	

}

