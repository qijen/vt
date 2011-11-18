#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "pVehicleMotor.h"
#include "pVehicleGears.h"


#include "pGearbox.h"


#include <xDebugTools.h>
#include "NxArray.h"

#include "virtools/vtTools.h"




int pVehicle::_calculateCurrentStatus()
{

	int result = 0;
	
	//----------------------------------------------------------------
	//
	// is moving ? 
	//
	{

		_computeLocalVelocity();
		if ( NxMath::abs(_localVelocity.z) > 0.01f)
			result |=VS_IsMoving;

		if (result & VS_IsMoving)
		{

			if(_localVelocity.z > 0.0f)
				result|=VS_IsRollingForward;

			if(_localVelocity.z < 0.0f)
				result|=VS_IsRollingBackward;
		}
	}
	NxVec3 _loc = _localVelocity;
	//----------------------------------------------------------------
	//
	// is accelerated ?
	//
	
	if ( _cAcceleration > 0.1f )
		result |=VS_IsAcceleratedForward;
	
	if ( _cAcceleration < 0.0f )
		result |=VS_IsAcceleratedBackward;
	
	if ( (result & VS_IsAcceleratedForward) || (result & VS_IsAcceleratedBackward) )
		result |=VS_IsAccelerated;

	//----------------------------------------------------------------
	//
	// is Braking ?
	//
	if ( (result & VS_IsMoving ) )
	{
		if ( _localVelocity.z > 0.0f && ( result & VS_IsAcceleratedBackward ) )
		{
			result |=VS_IsBraking;
		}

		if ( _localVelocity.z < 0.0f &&  (result & VS_IsAcceleratedForward ) )
		{
			result |=VS_IsBraking;
		}
	}

	//----------------------------------------------------------------
	//
	// is steering 
	//
	if( XAbs(_cSteering) > 0.01f )
		result|=VS_IsSteering;

	//----------------------------------------------------------------
	//
	// is falling + handbrake
	//
	
	_nbNotTouching =0;
	_nbTouching =0;
	_nbHandbrakeOn =0;
	
	int nbWheels = _wheels.size();

	for(NxU32 i = 0; i < _wheels.size(); i++)
	{
		pWheel* wheel = _wheels[i];
		if (!wheel->hasGroundContact())
		{
			_nbNotTouching++;
		} else {
			_nbTouching++;
		}
		
		if(_cHandbrake && wheel->getWheelFlag(WF_AffectedByHandbrake))
		{
			_nbHandbrakeOn++;
		} 
	}
	if (_nbNotTouching == _wheels.size())
		result |= VS_IsFalling;

	if (_cHandbrake && _nbHandbrakeOn )
	{
		result|=VS_Handbrake;
	}
	

	/*
	if ( !(result & VS_IsBraking) )
	{
		mBreakLastFrame  = false;
		mTimeBreaking  = 0.0f;
	}
	*/

	_accelerationPedal = _cAcceleration;
	return result;
}




void pVehicle::updateControl(float steering, bool analogSteering, float acceleration, bool analogAcceleration, bool handBrake)
{
	setControlState(E_VCS_ACCELERATION,acceleration);
	setControlState(E_VCS_HANDBRAKE,handBrake);
	setControlState(E_VCS_STEERING,steering);
	setControlMode(E_VCS_ACCELERATION,analogAcceleration ? E_VCSM_ANALOG : E_VCSM_DIGITAL);
	setControlMode(E_VCS_STEERING, analogSteering ? E_VCSM_ANALOG : E_VCSM_DIGITAL);


}
void pVehicle::control(float steering, bool analogSteering, float acceleration, bool analogAcceleration, bool handBrake)
{
	
	if (steering != 0 || acceleration != 0 || handBrake)
		getActor()->wakeUp(0.05);

	return;

	_controlSteering(steering, analogSteering);
	_computeLocalVelocity();

	NxVec3 locVel = _localVelocity;
	float lcx = locVel.x;
	float lcz = locVel.z;
	
	
	float test = _localVelocity.z * acceleration < ( NxMath::sign(-acceleration) );
	float test2 = _localVelocity.z * acceleration < ( -0.1f );
	float test3 = XAbs(_localVelocity.z) * acceleration < ( -0.1f );


	if (!_braking || _releaseBraking)
	{
		//_braking = _localVelocity.x * acceleration < (-0.1f /** NxMath::sign(-acceleration) */);
		_braking = _localVelocity.z * acceleration < ( -0.1 /*NxMath::sign(acceleration) */ );

		//_braking = _localVelocity.z * acceleration < ( NxMath::sign(acceleration));
		_releaseBraking = false;
	}

	if(_handBrake != handBrake)
	{
		_handBrake = handBrake;
		_brakePedalChanged;
	}
	//printf("Braking: %s, Handbrake: %s\n", _braking?"true":"false", handBrake?"true":"false");
	_controlAcceleration(acceleration, analogAcceleration);
}




pWheel*pVehicle::getWheel(CK3dEntity *wheelReference)
{

	if (!wheelReference)
	{
		return NULL;
	}
	
	for(NxU32 i = 0; i < _wheels.size(); i++)
	{
		pWheel* wheel = _wheels[i];
		if (wheel->getEntID()  == wheelReference->GetID())
		{
			return wheel;
		}
	}
	return NULL;
}
void pVehicle::handleContactPair(NxContactPair* pair, int carIndex)
{
	NxContactStreamIterator i(pair->stream);

	while(i.goNextPair())
	{
		NxShape * s = i.getShape(carIndex);

		while(i.goNextPatch())
		{
			const NxVec3& contactNormal = i.getPatchNormal();

			while(i.goNextPoint())
			{
				//user can also call getPoint() and getSeparation() here

				const NxVec3& contactPoint = i.getPoint();

				//add forces:

				//assuming front wheel drive we need to apply a force at the wheels.
				if (s->is(NX_SHAPE_CAPSULE) && s->userData != NULL) {
					//assuming only the wheels of the car are capsules, otherwise we need more checks.
					//this branch can't be pulled out of loops because we have to do a full iteration through the stream


					NxQuat local2global = s->getActor().getGlobalOrientationQuat();
					/*
					NxWheel* w = (NxWheel*)s->userData;
					if (!w->getWheelFlag(E_WF_USE_WHEELSHAPE))
					{
						NxWheel1 * wheel = static_cast<NxWheel1*>(w);
						wheel->contactInfo.otherActor = pair.actors[1-carIndex];
						wheel->contactInfo.contactPosition = contactPoint;

						wheel->contactInfo.contactPositionLocal = contactPoint;
						wheel->contactInfo.contactPositionLocal -= _bodyActor->getGlobalPosition();
						local2global.inverseRotate(wheel->contactInfo.contactPositionLocal);

						wheel->contactInfo.contactNormal = contactNormal;
						if (wheel->contactInfo.otherActor->isDynamic()) 
						{
							NxVec3 globalV = s->getActor().getLocalPointVelocity(wheel->getWheelPos());
							globalV -= wheel->contactInfo.otherActor->getLinearVelocity();
							local2global.inverseRotate(globalV);
							wheel->contactInfo.relativeVelocity = globalV.x;
							//printf("%2.3f (%2.3f %2.3f %2.3f)\n", wheel->contactInfo.relativeVelocity,
							//	globalV.x, globalV.y, globalV.z);
						} 
						else 
						{
							NxVec3 vel = s->getActor().getLocalPointVelocity(wheel->getWheelPos());
							local2global.inverseRotate(vel);
							wheel->contactInfo.relativeVelocity = vel.x;
							wheel->contactInfo.relativeVelocitySide = vel.z;
						}
						NX_ASSERT(wheel->hasGroundContact());
						//printf(" Wheel %x is touching\n", wheel);
					}
					*/
				}
			}
		}		
	}
	//printf("----\n");
}
float pVehicle::_computeAxisTorqueV2()
{
	if(_vehicleMotor != NULL)
	{
		NxReal rpm = _computeRpmFromWheels();
		NxReal motorRpm = _computeMotorRpm(rpm);
		_vehicleMotor->setRpm(motorRpm);
		float acc = _accelerationPedal;
		NxReal torque = _accelerationPedal * _vehicleMotor->getTorque();
		NxReal v = getActor()->getLinearVelocity().magnitude();

		/*
		printf("v: %2.3f m/s (%2.3f km/h)\n", v, v*3.6f);
		printf("rpm %2.3f, motorrpm %2.3f, torque %2.3f, realtorque %2.3f\n",
			rpm, motorRpm, torque, torque*_getGearRatio()*_differentialRatio*_transmissionEfficiency);
		*/
		return torque * _getGearRatio() * _differentialRatio * _transmissionEfficiency;
	} else {
		_computeRpmFromWheels();
		return _cAcceleration * _motorForce;
	}
}
float pVehicle::_computeRpmFromWheels()
{
	NxReal wheelRpms = 0;
	NxI32 nbWheels = 0;
	int nbAcc=0;
	int nbNotAcc=0;
	
	int s = _wheels.size();

	for(NxU32 i = 0; i < _wheels.size(); i++)
	{
		pWheel* wheel = _wheels[i];
		if (wheel->getWheelFlag(WF_Accelerated))
		{
			nbWheels++;
			wheelRpms += wheel->getRpm();
		}
		
		
	}
	return wheelRpms / (NxReal)nbWheels;
}


float pVehicle::_getGearRatio()
{
	if(_vehicleGears == NULL)
	{
		return 1;
	} else {
		return _vehicleGears->getCurrentRatio();
	}
}
void pVehicle::gearUp()
{
	if (_vehicleGears)
	{
		printf("Changing gear from %d to", _vehicleGears->getGear());
		_vehicleGears->gearUp();
		printf(" %d\n", _vehicleGears->getGear());
	} else {
		printf("gearUp not supported if no gears available\n");
	}
}
void pVehicle::gearDown()
{
	if(_vehicleGears)
	{
		printf("Changing gear from %d to", _vehicleGears->getGear());
		_vehicleGears->gearDown();
		printf(" %d\n", _vehicleGears->getGear());
	} else {
		printf("gearDown not supported if no gears available\n");
	}
}

void pVehicle::setAutomaticMode(bool autoMode)
{
	mAutomaticMode=autoMode;

}
float pVehicle::_computeMotorRpm(float rpm)
{
	NxReal temp = _getGearRatio() * _differentialRatio;
	NxReal motorRpm = rpm * temp;
	NxI32 change = -1;
	if(_vehicleMotor)
	{
		NxI32 change;
		if(_vehicleGears && (change = _vehicleMotor->changeGears(_vehicleGears, 0.2f)))
		{
			change = _vehicleMotor->changeGears(_vehicleGears, 0.2f);
			if(change == 1 && mAutomaticMode )
			{
				gearUp();
			} else {
				NX_ASSERT(change == -1);
				gearDown();
			}
		}
		temp = _getGearRatio() * _differentialRatio;
		motorRpm = NxMath::max(rpm * temp, _vehicleMotor->getMinRpm());
	}
	return motorRpm;
}
void pVehicle::_updateRpms()
{
	NxReal rpm = _computeRpmFromWheels();
	if(_vehicleMotor != NULL)
	{
		NxReal motorRpm = _computeMotorRpm(rpm);
		_vehicleMotor->setRpm(motorRpm);
	}
}
NxActor* pVehicle::getActor(){ return mActor; }



float pVehicle::getDriveVelocity()
{
	return NxMath::abs(_localVelocity.x);
}

const pWheel*pVehicle::getWheel(int i)
{
	NX_ASSERT(i < _wheels.size()); 
	return _wheels[i]; 

}






void pVehicle::_computeLocalVelocity()
{
	_computeMostTouchedActor();
	NxVec3 relativeVelocity;
	if (_mostTouchedActor == NULL || !_mostTouchedActor->isDynamic())
	{
		relativeVelocity = getActor()->getLinearVelocity();
	} else {
		relativeVelocity = getActor()->getLinearVelocity() - _mostTouchedActor->getLinearVelocity();
	}
	NxQuat rotation = getActor()->getGlobalOrientationQuat();
	NxQuat global2Local;
	_localVelocity = relativeVelocity;
	rotation.inverseRotate(_localVelocity);
	char master[512];

	//sprintf(master,"Velocity: %2.3f %2.3f %2.3f\n", _localVelocity.x, _localVelocity.y, _localVelocity.z);
	//OutputDebugString(master);
}
void pVehicle::_controlSteering(float steering, bool analogSteering)
{

	if(analogSteering)
	{
		_steeringWheelState = steering;
	} else if (NxMath::abs(steering) > 0.0001f) {
		_steeringWheelState += NxMath::sign(steering) * getDigitalSteeringDelta();
	} else if (NxMath::abs(_steeringWheelState) > 0.0001f) {
		_steeringWheelState -= NxMath::sign(_steeringWheelState) * getDigitalSteeringDelta();
	}
	_steeringWheelState = NxMath::clamp(_steeringWheelState, 1.f, -1.f);
}

void pVehicle::_computeMostTouchedActor()
{
	std::map<NxActor*, NxU32> actors;
	typedef std::map<NxActor*, NxU32> Map;
	for(NxU32 i = 0; i < _wheels.size(); i++)
	{
		NxActor* curActor = _wheels[i]->getTouchedActor();
		Map::iterator it = actors.find(curActor);
		if (it == actors.end())
		{
			actors[curActor] = 1;
		} else {
			it->second++;
		}
	}

	NxU32 count = 0;
	_mostTouchedActor = NULL;
	for(Map::iterator it = actors.begin(); it != actors.end(); ++it)
	{
		if(it->second > count)
		{
			count = it->second;
			_mostTouchedActor = it->first;
		}
	}
}


int pVehicle::initWheels(int flags)
{
	getWheels().clear();
	int nbShapes = getActor()->getNbShapes();
	NxShape ** slist = (NxShape **)getActor()->getShapes();
	for (NxU32 j=0; j<nbShapes; j++)
	{
		NxShape *s = slist[j];
		if (s)
		{
			pSubMeshInfo *sinfo = static_cast<pSubMeshInfo*>(s->userData);
			if (sinfo && sinfo->wheel !=NULL)
			{
				pWheel *wheel = sinfo->wheel;
				pWheel2* wheel2 = (pWheel2*)wheel;
				NxWheelShape *wShape = wheel2->getWheelShape();
				wheel2->setEntity(static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(wheel2->getEntID())));
				if (!wShape) continue;
				if (wheel2->getWheelFlag(WF_VehicleControlled) )
				{
					int& pOptions = getProcessOptions();
					wheel2->setProcessOptions(pOptions);
					
					if (  wShape->getWheelFlags() &  NX_WF_AXLE_SPEED_OVERRIDE )
						wheel2->_createInternalContactModifyCallback();

					getWheels().push_back(wheel);
					wheel2->setVehicle(this);
					
					
				}
			}
		}
	}

	return getWheels().size();


	/*
	int result =  0 ; 

	if (!getBody() || !getBody()->isValid() )
	{
		return result;
	}

	getWheels().clear();


	CK3dEntity* subEntity = NULL;
	while (subEntity= getBody()->GetVT3DObject()->HierarchyParser(subEntity) )
	{
		if (subEntity->HasAttribute(GetPMan()->GetPAttribute()))
		{
			pObjectDescr *subDescr = pFactory::Instance()->createPObjectDescrFromParameter(subEntity->GetAttributeParameter(GetPMan()->GetPAttribute()));
			if (subDescr->flags & BF_SubShape)
			{
				if (subDescr->hullType == HT_Wheel)
				{
					
					if (subEntity->HasAttribute(GetPMan()->att_wheelDescr ))
					{
						CKParameter *par = subEntity->GetAttributeParameter(GetPMan()->att_wheelDescr );
						if (par)
						{
							pWheelDescr *wDescr = pFactory::Instance()->createWheelDescrFromParameter(par);
							if (wDescr)
							{
								pWheel *wheel  = pFactory::Instance()->createWheel(getBody(),*wDescr);
								if (wheel)
								{
									NxWheelShape *wShape   = static_cast<NxWheelShape*>(getBody()->_getSubShapeByEntityID(subEntity->GetID()));

									if(wDescr->wheelFlags & E_WF_USE_WHEELSHAPE)
									{
										pWheel2 *wheel2 = static_cast<pWheel2*>(wheel);
										if (wheel2)
										{
											if(wShape)
												wheel2->setWheelShape(wShape);

										}
									}
									wheel->setEntID(subEntity->GetID());
									getWheels().push_back(wheel);
//									subEntity->SetParent(NULL);
							
								}
							}
						}
					}
				}
			}
		}
	}



	return getWheels().size();
	*/
}

void pVehicle::findDifferentialWheels(int& wheel1Index,int& wheel2Index)
{
	pWheel *wheel1 = NULL;
	pWheel* wheel2 = NULL;
	wheel1 = wheel2 = 0;
	for(NxU32 i = 0; i < _wheels.size(); i++)
	{


		pWheel *cW = _wheels[i];

		if (cW->getWheelFlag(WF_AffectedByDifferential))
		{
			if (!wheel1){
				wheel1 = cW; wheel1Index=i;	}
			else{
				wheel2 = cW;wheel2Index=i;	}
		}
	}
	if (!wheel1)
	{
		xWarning("couldn't find first differential wheel");
		wheel1Index = -1;
	}

	if (!wheel2)
	{
		xWarning("couldn't find second differential wheel");
		wheel2Index = -1;
	}
}

pVehicle::pVehicle()
{

	flags = 0;

	motorTorque = breakTorque = 0.0f;


}
void pVehicle::setControlState(float steering, bool analogSteering, float acceleration, bool analogAcceleration, bool handBrake)
{
	_cAcceleration = acceleration;
	_cSteering  = steering;

	_cAnalogAcceleration = analogAcceleration;
	_cAnalogSteering = analogSteering;
	_cHandbrake  = handBrake;

}
pVehicle::pVehicle(pVehicleDesc descr,CK3dEntity *referenceObject) : xEngineObjectAssociation<CK3dEntity*>(referenceObject,referenceObject->GetID())
{
	_digitalSteeringDelta  = descr.digitalSteeringDelta;
	_steeringSteerPoint = descr.steeringSteerPoint;
	_steeringTurnPoint = descr.steeringTurnPoint;
	_steeringMaxAngleRad = NxMath::degToRad(descr.steeringMaxAngle);
	_transmissionEfficiency  = descr.transmissionEfficiency;
	_differentialRatio  = descr.differentialRatio;
	_maxVelocity = descr.maxVelocity;
	_motorForce  = descr.motorForce;

	_cSteering  = 0.0f;
	_cAcceleration = 0.0f;
	_cAnalogAcceleration = false;
	_cAnalogSteering = false;
	_cHandbrake = false;
	mAutomaticMode = true;

	setBody(descr.body);

	_vehicleMotor = NULL;
	_vehicleGears = NULL;
	steer = NULL;

	//----------------------------------------------------------------
	//
	// Break settings
	//

	mBreakLastFrame = false;
	mTimeBreaking = 0.0f;
	mBrakeMediumThresold = 1.5f;
	mBrakeHighThresold = 3.0f;

	mBreakPressures[BL_Small] = 0.1f;
	mBreakPressures[BL_Medium] = 0.3f;
	mBreakPressures[BL_High] = 1.0f;

	useBreakTable = false;

	mSmallBrakeTable.brakeEntries[0] = 10.0f;
	mSmallBrakeTable.brakeEntries[1] = 20.0f;
	mSmallBrakeTable.brakeEntries[2] = 30.0f;
	mSmallBrakeTable.brakeEntries[3] = 40.0f;
	mSmallBrakeTable.brakeEntries[4] = 50.0f;
	mSmallBrakeTable.brakeEntries[5] = 100.0f;
	mSmallBrakeTable.brakeEntries[6] = 200.0f;
	mSmallBrakeTable.brakeEntries[7] = 400.0f;
	mSmallBrakeTable.brakeEntries[8] = 1000.0f;
	mSmallBrakeTable.brakeEntries[9] = 1000.0f;

	mMediumBrakeTable.brakeEntries[0] = 400.0f;
	mMediumBrakeTable.brakeEntries[1] = 450.0f;
	mMediumBrakeTable.brakeEntries[2] = 550.0f;
	mMediumBrakeTable.brakeEntries[3] = 650.0f;
	mMediumBrakeTable.brakeEntries[4] = 725.0f;
	mMediumBrakeTable.brakeEntries[5] = 900.0f;
	mMediumBrakeTable.brakeEntries[6] = 1050.0f;
	mMediumBrakeTable.brakeEntries[7] = 1000.0f;
	mMediumBrakeTable.brakeEntries[8] = 1000.0f;

	mHighBrakeTable.brakeEntries[0] = 700.0f;
	mHighBrakeTable.brakeEntries[1] = 775.0f;
	mHighBrakeTable.brakeEntries[2] = 950.0f;
	mHighBrakeTable.brakeEntries[3] = 1100.0f;
	mHighBrakeTable.brakeEntries[4] = 1200.0f;
	mHighBrakeTable.brakeEntries[5] = 1250.0f;
	mHighBrakeTable.brakeEntries[6] = 1500.0f;
	mHighBrakeTable.brakeEntries[7] = 2000.0f;
	mHighBrakeTable.brakeEntries[8] = 1000.0f;
	mHighBrakeTable.brakeEntries[9] = 1000.0f;

	breakConditionLevels[BC_NoUserInput]=BL_Small;
	breakConditionLevels[BC_DirectionChange]=BL_High;
	breakConditionLevels[BC_Handbrake]=BL_Medium;
	breakConditionLevels[BC_UserBreak]=BL_Medium;


	motorTorque = breakTorque = 0.0F;

	overrideMask = 0;
	callMask.set(  CB_OnPreProcess , true);
	callMask.set(  CB_OnPostProcess, true);

	preScript = postScript = 0;

	processOptions = 0;

	//setProcessOptions(GetPMan()->getProcessOptions());

}


pVehicleDesc::pVehicleDesc()	//constructor sets to default
{
	setToDefault();
}

void pVehicleDesc::setToDefault()
{
	userData = NULL;
	transmissionEfficiency = 1.0f;
	differentialRatio = 1.0f;
	maxVelocity = 80;
	motorForce = 100.0f;

	body = NULL;
	gearDescription = NULL;//new pVehicleGearDesc();
	motorDescr  = NULL;//new pVehicleMotorDesc();
	steeringMaxAngle = 30;
	steeringSteerPoint = VxVector(0,0,0);
	steeringTurnPoint  = VxVector(0,0,0);
	digitalSteeringDelta = 0.04f;
	processFlags = 0;

}


bool pVehicleDesc::isValid() const
{
	/*for (NxU32 i = 0; i < carWheels.size(); i++) {
		if (!carWheels[i]->isValid())
			return false;
	}
	*/
	if (mass < 0)
		return false;

	return true;
}


void pVehicle::setControlState(int parameter,float value)
{


	switch (parameter)
	{

	case E_VCS_GUP:
		_cShiftStateUp = (int)value;
		break;
	
	case E_VCS_GDOWN:
		_cShiftStateDown = (int)value;
		break;
	case E_VCS_ACCELERATION:
		if (_cAnalogSteering == 0.0f)
		{
			xWarning("no Acc");
		}
		_cAcceleration = value;
		break;

	case E_VCS_STEERING:
		_cSteering = value;
		break;

	case E_VCS_HANDBRAKE:
		_cHandbrake= (int)value;
		break;
	}
}

void pVehicle::setControlMode(int parameter,int mode)
{
	switch (parameter)
	{
	case E_VCS_ACCELERATION:
		_cAnalogAcceleration =  (mode == E_VCSM_ANALOG) ? true : false; 		
		break;

	case E_VCS_STEERING:
		_cAnalogSteering =  (mode == E_VCSM_ANALOG) ? true : false;
		break;
	}
}

float pVehicle::_computeAxisTorque()
{
	if(_vehicleMotor != NULL)
	{
		NxReal rpm = _computeRpmFromWheels();
		NxReal motorRpm = _computeMotorRpm(rpm);
		_vehicleMotor->setRpm(motorRpm);
		NxReal torque = _accelerationPedal * _vehicleMotor->getTorque();
		NxReal v = getActor()->getLinearVelocity().magnitude();
		//printf("v: %2.3f m/s (%2.3f km/h)\n", v, v*3.6f);
		//printf("rpm %2.3f, motorrpm %2.3f, torque %2.3f, realtorque %2.3f\n",
		//	rpm, motorRpm, torque, torque*_getGearRatio()*_differentialRatio*_transmissionEfficiency);
		return torque * _getGearRatio() * _differentialRatio * _transmissionEfficiency;
	} else {
		_computeRpmFromWheels();
		return _accelerationPedal * _motorForce;
	}
}
void pVehicle::getControlState(int parameter,float &value,int &mode)
{
	switch (parameter)
	{
	case E_VCS_ACCELERATION:
		value = _cAcceleration;
		mode = _cAnalogAcceleration;
		break;

	case E_VCS_STEERING:
		value = _cSteering;
		mode = _cAnalogSteering;
		break;

	case E_VCS_HANDBRAKE:
		value = ((float)_cHandbrake);
		mode = _cHandbrake;
		break;

	}

}