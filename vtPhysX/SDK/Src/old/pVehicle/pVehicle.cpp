#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "pVehicleMotor.h"
#include "pVehicleGears.h"

#include <xDebugTools.h>
#include "NxArray.h"

enum VehicleStatus
{

	VS_IsMoving=(1<<0),
	VS_IsAccelerated=(1<<1),
	VS_IsAcceleratedForward=(1<<2),
	VS_IsAcceleratedBackward=(1<<3),
	VS_AllWheelsOnGround=(1<<4),
	VS_Handbrake=(1<<5),
	VS_IsBraking=(1<<6),

};

int pVehicle::_calculateCurrentStatus(){

	int result = 0;
	
	//----------------------------------------------------------------
	//
	// is moving ? 
	//
	{
		_computeLocalVelocity();
		if ( NxMath::abs(_localVelocity.z) > 0.1f)
			result |=VS_IsMoving;
	}

	//----------------------------------------------------------------
	//
	// is accelerated ?
	//
	{
		if ( _cAcceleration > 0.1f )
			result |=VS_IsAcceleratedForward;
		
		if ( _cAcceleration < 0.0f )
			result |=VS_IsAcceleratedBackward;
		
		if ( (result & VS_IsAcceleratedForward) || (result & VS_IsAcceleratedBackward) )
			result |=VS_IsAccelerated;
	}

	//----------------------------------------------------------------
	//
	// is falling + handbrake
	//
	{
		_nbNotTouching =0;
		_nbTouching =0;
		_nbHandbrakeOn =0;
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
		if (_nbTouching == _wheels.size())
			result |= VS_AllWheelsOnGround;

		if (_cHandbrake && _nbHandbrakeOn )
		{
			result|=VS_Handbrake;
		}
	}



	return NULL;
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
void pVehicle::control(float steering, bool analogSteering, float acceleration, bool analogAcceleration, bool handBrake)
{
	
	if (steering != 0 || acceleration != 0 || handBrake)
		getActor()->wakeUp(0.05);

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


void pVehicle::updateVehicle( float lastTimeStepSize )
{

	control(_cSteering,_cAnalogSteering,_cAcceleration,_cAnalogAcceleration,_cHandbrake);
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
	if (!nbTouching)
	{

		/*
		errMessage.Format("wheels in air ? : %d",nbTouching);
		xInfo(errMessage.Str());
		*/
	}

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
	

	if ( (NxMath::abs(_accelerationPedal) > 0.1f) && motorTorque == 0.0f)
	{
		
	}
	

	for(NxU32 i = 0; i < _wheels.size(); i++) 
	{
		pWheel* wheel = _wheels[i];
		wheel->tick(_handBrake, motorTorque, _brakePedal, lastTimeStepSize);
		//wheel->tick(_handBrake, motorTorque, _brakePedal, 1/60);
	}


	int flags =0;

	flags = 0;


	if (_brakePedal)
	{
		flags |= E_VSF_IS_BREAKPEDAL;
	}

	if (_braking)
	{
		flags |= E_VSF_IS_BRAKING;
	}

	if (_releaseBraking)
	{
		flags |= E_VSF_RELEASING_BRAKE_PEDAL;
	}

	if (_accelerationPedal > 0.01f )
	{
		flags |= E_VSF_ACC_PEDAL;
	}


	if (nbTouching)
	{
		flags |= E_VSF_HAS_GROUND;
	}

	if (getMotor())
	{
		flags |= E_VSF_HAS_MOTOR;
	}

	if (getGears())
	{
		flags |= E_VSF_HAS_GEARS;
	}

	if (_brakePedalChanged)
	{
		flags |= E_VSF_BREAKPEDAL_CHANGED;
	}

	if (_cHandbrake)
	{
		flags |= E_VCS_HANDBRAKE;
	}

	if (  (flags & E_VSF_BREAKPEDAL_CHANGED)  		&& 
		  (flags & E_VSF_IS_BREAKPEDAL)   && 
		  (NxMath::abs(_cAcceleration)>0.01f ) &&
		  (!_cHandbrake)
		)
	{
		_braking = false;
	}

	if (_cHandbrake)
	{
		_braking = true;
	}



	setVSFlags(flags);

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
float pVehicle::_computeRpmFromWheels()
{
	NxReal wheelRpms = 0;
	NxI32 nbWheels = 0;
	
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

float pVehicle::_computeMotorRpm(float rpm)
{
	NxReal temp = _getGearRatio() * _differentialRatio;
	NxReal motorRpm = rpm * temp;
	NxI32 change = -1;
/*	if(_vehicleMotor)
	{
		if(_vehicleGears && mAutomaticMode )
		{
		*/
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
	return -0.9469696 * radius * NxPi * axleSpeed;

}

// ( X * 10 * 60) / (5280* 12) * 100 = X * 0.9469696
//return (-((10 * GetWheelSizeWidth() * NxPi * mOurWheels[BACK_LEFT]->getAxleSpeed() * 60) / (5280 * 12)) * 100);
//return -0.9469696 * GetWheelSizeWidth() * NxPi * mOurWheels[BACK_LEFT]->getAxleSpeed();


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
		_steeringWheelState += NxMath::sign(steering) * _digitalSteeringDelta;
	} else if (NxMath::abs(_steeringWheelState) > 0.0001f) {
		_steeringWheelState -= NxMath::sign(_steeringWheelState) * _digitalSteeringDelta;
	}
	_steeringWheelState = NxMath::clamp(_steeringWheelState, 1.f, -1.f);
	//printf("SteeringWheelState: %2.3f\n", _steeringWheelState);
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
				if (!wShape) continue;

				if (wheel2->getWheelFlag(WF_VehicleControlled) )
				{
					getWheels().push_back(wheel);
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

pVehicle::pVehicle()
{

}
void pVehicle::setControlState(float steering, bool analogSteering, float acceleration, bool analogAcceleration, bool handBrake)
{
	_cAcceleration = acceleration;
	_cSteering  = steering;

	_cAnalogAcceleration = analogAcceleration;
	_cAnalogSteering = analogSteering;
	_cHandbrake  = handBrake;

}
pVehicle::pVehicle(pVehicleDesc descr)
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
	case E_VCS_ACCELERATION:
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