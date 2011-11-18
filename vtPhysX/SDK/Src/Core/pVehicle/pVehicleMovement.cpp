#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "pVehicleMotor.h"
#include "pVehicleGears.h"
#include "pSteer.h"

#include <xDebugTools.h>
#include "NxArray.h"


float pVehicle::getBrakeAmountFromTable(pVehicleBreakLevel brakeLevel)
{

	int value = abs((int(getMPH()) / 10));
	if(value > BREAK_TABLE_ENTRIES - 1)
		value = BREAK_TABLE_ENTRIES - 1;
	if(value < 0 || value == BREAK_TABLE_ENTRIES)
	{
		return 1.0f;
	}
	
	switch(brakeLevel)
	{
		case BL_Small:
			return mSmallBrakeTable.brakeEntries[brakeLevel];
		case BL_Medium:
			return mMediumBrakeTable.brakeEntries[brakeLevel];
		case BL_High:
			return mHighBrakeTable.brakeEntries[brakeLevel];
	}

	return 1.0f;

}

pVehicleBreakCase pVehicle::calculateBreakCase(int currentAccelerationStatus)
{

	//----------------------------------------------------------------
	//
	// is rolling or no user input ?
	//
	if( !(currentAccelerationStatus & VS_Handbrake )	&&
		 (currentAccelerationStatus & VS_IsMoving )		&&
		!(currentAccelerationStatus & VS_IsAccelerated)
	  )
		return BC_NoUserInput;

	//----------------------------------------------------------------
	//
	// direction change ?
	//
	if( !(currentAccelerationStatus & VS_Handbrake )	&&
		 (currentAccelerationStatus & VS_IsBraking )	&& 
		 (currentAccelerationStatus & VS_IsMoving )		&&
		 (currentAccelerationStatus & VS_IsAccelerated)
	  )
		return BC_DirectionChange;

	//----------------------------------------------------------------
	//
	// handbrake 
	//
	if( (currentAccelerationStatus & VS_Handbrake ) )
		return BC_Handbrake;

	return BC_NoUserInput;

}


void pVehicle::doSteering()
{
	for(NxU32 i = 0; i < _wheels.size(); i++)
	{
		pWheel2* wheel = (pWheel2*)_wheels[i];
		if(wheel->getWheelFlag(WF_SteerableInput))
		{
			int i;
			// Send steering angle through to steering wheels
			float factor=steer->GetLock()/wheel->GetLock()*2.0f;
			float angle = steer->GetAngle();
			float rAngle = wheel->getWheelShape()->getSteerAngle();
			wheel->setSteering(steer->GetAngle()/factor);

		}
	}
}
int pVehicle::_performSteering(float dt)
{
	_controlSteering(_cSteering, _cAnalogSteering);

	NxReal distanceSteeringAxisCarTurnAxis = _steeringSteerPoint.x  - _steeringTurnPoint.x;
	//NX_ASSERT(_steeringSteerPoint.z == _steeringTurnPoint.z);
	NxReal distance2 = 0;
	if (NxMath::abs(_steeringWheelState) > 0.01f)
		distance2 = distanceSteeringAxisCarTurnAxis / NxMath::tan(_steeringWheelState * getMaxSteering());

	float tanS = NxMath::tan(_steeringWheelState * getMaxSteering());

	for(NxU32 i = 0; i < _wheels.size(); i++)
	{
		pWheel* wheel = _wheels[i];
		if(wheel->getWheelFlag(WF_SteerableInput))
		{
			if(distance2 != 0)
			{

				//NxReal xPos = wheel->getWheelPos().x;
				NxReal xPos = ((pWheel2*)wheel)->getWheelShape()->getLocalPosition().x;
				//NxReal xPos2 = _steeringSteerPoint.x- wheel->getWheelPos().x;
				//NxReal xPos2 = _steeringSteerPoint.x- ((pWheel2*)wheel)->getWheelShape()->getLocalPosition().z;
				NxReal zPos = ((pWheel2*)wheel)->getWheelShape()->getLocalPosition().z;
				//NxReal zPos = wheel->getWheelPos().z;

				NxReal dz = -zPos + distance2;
				NxReal dx = xPos - _steeringTurnPoint.x;
				float angle =(NxMath::atan(dx/dz));
				if (dx < 0.0f)
				{
					angle*=-1.0f;
				}
				wheel->setAngle(angle);
			} else {
				wheel->setAngle(0.0f);
			}

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
	}



	return 0;
}
float pVehicle::calculateBraking(float dt)
{

	float breakTorque = 0.0f;

	
	pVehicleBreakCase currentBreakCase = calculateBreakCase(_currentStatus);

	/*
	int currentBreakLevel = getBreaklevelForCase(currentBreakCase);
	*/
	bool calculateBreaking = false;
	XString errMessage;


	if( 
		((_currentStatus & VS_IsBraking ) && (_currentStatus & VS_IsMoving)) 
		)	calculateBreaking = true;


	if (	(_currentStatus & VS_IsMoving )		&& 
		(getBreakFlags() &  VBF_Autobreak)	&& 
		(currentBreakCase == BC_NoUserInput)&& 
		(_currentStatus & VS_IsMoving )		&&
		!(_currentStatus & VS_IsAccelerated )
		)
	{
		calculateBreaking = true;
		/*		errMessage.Format("autobreak");*/
		//xInfo(errMessage.Str());
	}


	if (_currentStatus & VS_Handbrake)
	{
		/*errMessage.Format("handbrake");
		xInfo(errMessage.Str());*/
		goto performAcceleration;
	}

	//----------------------------------------------------------------
	//
	// calculate break torque 
	//
	if ( calculateBreaking )
	{


		//----------------------------------------------------------------
		//
		// increase break time counter
		//
		if(mBreakLastFrame)
			mTimeBreaking+=dt;

		//----------------------------------------------------------------
		//
		// determine break amount by table 
		//
		if ( (getBreakFlags() & VBF_UseTable) )
		{

			if(mTimeBreaking < mBrakeMediumThresold)
			{
				breakTorque = getBrakeAmountFromTable(BL_Small);
				errMessage.Format("breaking at small : bt : %f at %f",breakTorque,mTimeBreaking);
				//xInfo(errMessage.Str());

			}else if(mTimeBreaking >= mBrakeMediumThresold && mTimeBreaking < mBrakeHighThresold )
			{
				breakTorque = getBrakeAmountFromTable(BL_Medium);
				errMessage.Format("breaking at medium : bt : %f at %f",breakTorque,mTimeBreaking);
				//xInfo(errMessage.Str());
			}else
			{
				breakTorque = getBrakeAmountFromTable(BL_High);
				errMessage.Format("breaking at high : bt : %f at %f",breakTorque,mTimeBreaking);
				//xInfo(errMessage.Str());
			}
		}else
		{

			//----------------------------------------------------------------
			//
			// use break pressures
			//
			float mCurrentBrakeTorque = getBrakeTorque()*0.01f;
			float AmountToBrakeFinal = 0.0f;
			if(mTimeBreaking < mBrakeMediumThresold)
			{
				breakTorque = mCurrentBrakeTorque * mBreakPressures[BL_Small];
				errMessage.Format("breaking at small : bt : %f at %f",breakTorque,mTimeBreaking);
				//xInfo(errMessage.Str());

			}
			else if(mTimeBreaking >= mBrakeMediumThresold && mTimeBreaking < mBrakeHighThresold )
			{
				breakTorque = mCurrentBrakeTorque * mBreakPressures[BL_Medium];
				errMessage.Format("breaking at medium : bt : %f at %f",breakTorque,mTimeBreaking);
				//xInfo(errMessage.Str());
			}
			else
			{
				breakTorque = mCurrentBrakeTorque * mBreakPressures[BL_High];
				errMessage.Format("breaking at high : bt : %f at %f",breakTorque,mTimeBreaking);
				//xInfo(errMessage.Str());
			}


			if (breakTorque > 1000.f)
			{
				breakTorque= 1000.0f;
			}

		}

	}else
	{
		mBreakLastFrame = false;
		mTimeBreaking = 0.0f;
	}


	performAcceleration:

	if (breakTorque > 0.0f)
	{
		mBreakLastFrame  = true;
	}

	return breakTorque;
}
int pVehicle::_performAcceleration(float dt)
{

	motorTorque = 0.0f;

	if ( _nbTouching && _currentStatus & VS_IsAccelerated )
	{
		NxReal axisTorque = _computeAxisTorqueV2();
		NxReal wheelTorque = axisTorque / (NxReal)(_wheels.size() - _nbHandbrakeOn );
		NxReal wheelTorqueNotTouching = _nbNotTouching > 0 ? wheelTorque * ( NxMath::pow(0.5f, (NxReal) _nbNotTouching )):0;
		NxReal wheelTorqueTouching = wheelTorque - wheelTorqueNotTouching;
		motorTorque = wheelTorqueTouching / (NxReal)_nbTouching; 
	}else
	{
		_updateRpms();
	}

	
	return 1;
}

void pVehicle::postStep()
{

	for(NxU32 i = 0; i < _wheels.size(); i++) 
	{
		//----------------------------------------------------------------
		//
		// determine break torque :
		//

		// is moving in the opposite direction than its accelerated 
		pWheel* wheel = _wheels[i];
		wheel->tick( ( _currentStatus & VS_Handbrake) , motorTorque, breakTorque  , _lastDT );

	}

}

void pVehicle::setBreakPressure(int breakLevel,float pressure)
{
	mBreakPressures[breakLevel]=pressure;
}
void pVehicle::setBreakCaseLevel(pVehicleBreakCase breakCase,pVehicleBreakLevel level)
{
	breakConditionLevels[breakCase]=level;
}

// ( X * 10 * 60) / (5280* 12) * 100 = X * 0.9469696
//return (-((10 * GetWheelSizeWidth() * NxPi * mOurWheels[BACK_LEFT]->getAxleSpeed() * 60) / (5280 * 12)) * 100);
//return -0.9469696 * GetWheelSizeWidth() * NxPi * mOurWheels[BACK_LEFT]->getAxleSpeed();

float pVehicle::getBrakeTorque()
{
	int nbAWheels =0;
	float radius = 0.0;
	float axleSpeed = 0.0;
	for(NxU32 i = 0; i < _wheels.size(); i++)
	{
		pWheel* wheel = _wheels[i];
		if(wheel->getWheelFlag(WF_Accelerated))
		{
			nbAWheels++;
			if (wheel->getRadius() > radius)
				radius = wheel->getRadius();

			pWheel2* w2 = (pWheel2*)wheel;
			if (w2->getWheelShape()->getAxleSpeed() > axleSpeed)
			{
				axleSpeed = w2->getWheelShape()->getAxleSpeed();
			}
		}
	}

	if(fabs(axleSpeed) != 0)
	{
		return ((5252 * getMotorForce()) / (fabs(axleSpeed) * 10));
	}

	return 0.0f;
}

float pVehicle::getMPH(int type)
{

	int nbAWheels =0;
	float radius = 0.0;
	float axleSpeed = 0.0;
	for(NxU32 i = 0; i < _wheels.size(); i++)
	{
		pWheel* wheel = _wheels[i];
		if(wheel->getWheelFlag(WF_Accelerated))
		{
			nbAWheels++;
			if (wheel->getRadius() > radius)
				radius = wheel->getRadius();

			pWheel2* w2 = (pWheel2*)wheel;
			if (w2->getWheelShape()->getAxleSpeed() > axleSpeed)
			{
				axleSpeed = w2->getWheelShape()->getAxleSpeed();
			}
		}
	}


	// ( X * 10 * 60) / (5280* 12) * 100 = X * 0.9469696
	//return (-((10 * GetWheelSizeWidth() * NxPi * mOurWheels[BACK_LEFT]->getAxleSpeed() * 60) / (5280 * 12)) * 100);
	return 0.9469696 * radius * NxPi * axleSpeed;

}
void pVehicle::_controlAcceleration(float acceleration, bool analogAcceleration)
{
	if(NxMath::abs(acceleration) < 0.001f)
	{
		_releaseBraking = true;
		//xInfo("set release breaking = true");
	}

	if(!_braking)
	{
		_accelerationPedal = NxMath::clamp(acceleration, 1.f, -1.f);
		_brakePedalChanged = _brakePedal == 0;
		_brakePedal = 0;
		//xInfo("breaking = false : clamp accPedal 1|-1");
	} else {
		//xInfo("breaking = true : accPeal = 0");
		_accelerationPedal = 0;
		NxReal newv = NxMath::clamp(NxMath::abs(acceleration), 1.f, 0.f);
		_brakePedalChanged = _brakePedal == newv;
		_brakePedal = newv;
	}
	char master[512];
	sprintf(master,"Acceleration: %2.3f, Braking %2.3f\n", _accelerationPedal, _brakePedal);
	xInfo(master);
	//OutputDebugString(master);


}




