#include <StdAfx.h>
#include "vtPhysXAll.h"

pWheelContactData*
pWheel1::getContact(){

	return new pWheelContactData();
}

float pWheel1::getRpm()const{	return NxMath::abs(_turnVelocity * 60.f);}

NxActor *pWheel1::getTouchedActor(){ 	return contactInfo->otherActor; }

void pWheel1::_tick(float dt)
{

	if(!hasGroundContact())
		updateContactPosition();
	
	//################################################################
	//
	// Calculate the wheel rotation around the roll axis
	//
	updateAngularVelocity(dt*0.001f, false);

	float motorTorque=0.0;

	if(getWheelFlag(WF_Accelerated)) 
	{
		/*if (handBrake && getWheelFlag(NX_WF_AFFECTED_BY_HANDBRAKE)) 
		{
			// Handbrake, blocking!
		}*/
		
		if (hasGroundContact()) 
		{
			// Touching, force applies
			NxVec3 steeringDirection;
			getSteeringDirection(steeringDirection);
			steeringDirection.normalize();
			NxReal localTorque = motorTorque;
			NxReal wheelForce = localTorque / _radius;
			steeringDirection *= wheelForce;
			wheelCapsule->getActor().addForceAtPos(steeringDirection, contactInfo->contactPosition);
			if(contactInfo->otherActor->isDynamic())
				contactInfo->otherActor->addForceAtPos(-steeringDirection, contactInfo->contactPosition);
		} 
	}
	
	NxMat34& wheelPose = getWheelCapsule()->getGlobalPose();
	NxMat33 rot, axisRot, rollRot;
	rot.rotY( _angle );
	axisRot.rotY(0);
	rollRot.rotX(_turnAngle);
	wheelPose.M = rot * wheelPose.M * axisRot * rollRot;

	float a = _angle;
	float b = getWheelRollAngle();

	
	setWheelPose(wheelPose);
	//setWheelOrientation(wheelPose.M);


	contactInfo->reset();
}


void pWheel1::_updateVirtoolsEntity(bool position,bool rotation)
{

	CK3dEntity *ent  = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(getEntID()));
	if (ent && position)
	{

		/*
		NxWheelShape *wShape = getWheelShape();
		NxMat34 pose = wShape->getGlobalPose();
		NxWheelContactData wcd; 
		NxShape* contactShape = wShape->getContact(wcd);
		NxVec3 suspensionOffsetDirection;
		pose.M.getColumn(1, suspensionOffsetDirection);
		suspensionOffsetDirection =-suspensionOffsetDirection;

		if (contactShape && wcd.contactForce > -1000)
		{
			NxVec3 toContact = wcd.contactPoint - pose.t;
			double alongLength = suspensionOffsetDirection.dot(toContact);
			NxVec3 across = toContact - alongLength * suspensionOffsetDirection;
			double r = wShape->getRadius();
			double pullBack = sqrt(r*r - across.dot(across));
			pose.t += (alongLength - pullBack) * suspensionOffsetDirection;
		} else {
			pose.t += wShape->getSuspensionTravel() * suspensionOffsetDirection;
		}

		VxVector oPos  = getFrom(pose.t);
		ent->SetPosition(&oPos);

		if (hasGroundContact())
		{

		
		}else
		{
			//			VxVector gPos  = getWheelPos();
			//			ent->SetPosition(&gPos,getBody()->GetVT3DObject());
		}

		*/
	}
	if (ent && rotation)
	{
		//float rollAngle = getWheelRollAngle();
		//rollAngle+=wShape->getAxleSpeed() * (dt* 0.01f);

		VxQuaternion rot  = pMath::getFrom( getWheelPose().M );
		ent->SetQuaternion(&rot,NULL);
	}

}




void pWheel1::updateContactPosition() 
{
	contactInfo->contactPositionLocal = getFrom(_maxPosition) - NxVec3(0, _maxSuspension+_radius, 0);
}
void pWheel1::setAngle(float angle) 
{
	_angle = angle;

	NxReal Cos, Sin;
	NxMath::sinCos(_angle, Sin, Cos);
	NxMat33 wheelOrientation = wheelCapsule->getLocalOrientation();
	wheelOrientation.setColumn(0,  NxVec3( Cos, 0, Sin ));
	wheelOrientation.setColumn(2,  NxVec3( Sin, 0,-Cos ));
	setWheelOrientation(wheelOrientation);

}

void pWheel1::updateAngularVelocity(float lastTimeStepSize, bool handbrake) 
{
	if((mWheelFlags & WF_AffectedByHandbrake) && handbrake) 
	{
		_turnVelocity = 0;
	} 
	else if (contactInfo->isTouching()) 
	{
		NxReal wheelPerimeter = NxTwoPi * _radius;
		NxReal newTurnVelocity = contactInfo->relativeVelocity / wheelPerimeter;
		_turnVelocity = newTurnVelocity;
		_turnAngle += _turnVelocity * lastTimeStepSize * NxTwoPi;
	} 
	else 
	{
		_turnVelocity *= 0.99f;
		_turnAngle += _turnVelocity;
	}
	
	while(_turnAngle >= NxTwoPi)
		_turnAngle -= NxTwoPi;
	while (_turnAngle < 0)
		_turnAngle += NxTwoPi;

	setWheelRollAngle(_turnAngle);

}

void pWheel1::getSteeringDirection(NxVec3& dir) 
{
	if(mWheelFlags & (WF_SteerableInput | WF_SteerableAuto)) 
	{
		wheelCapsule->getGlobalOrientation().getColumn(0, dir);
	} 
	else 
	{
		wheelCapsule->getActor().getGlobalOrientation().getColumn(0, dir);
	}
}

void pWheel1::tick(bool handbrake, float motorTorque, float brakeTorque, float dt)
{

	if(getWheelFlag(WF_Accelerated)) 
	{
		if (handbrake && getWheelFlag(WF_AffectedByHandbrake)) 
		{
			// Handbrake, blocking!
		}
		else if (hasGroundContact()) 
		{
			// Touching, force applies
			NxVec3 steeringDirection;
			getSteeringDirection(steeringDirection);
			steeringDirection.normalize();
			NxReal localTorque = motorTorque;
			NxReal wheelForce = localTorque / _radius;
			steeringDirection *= wheelForce;
			wheelCapsule->getActor().addForceAtPos(steeringDirection, contactInfo->contactPosition);
			if(contactInfo->otherActor->isDynamic())
				contactInfo->otherActor->addForceAtPos(-steeringDirection, contactInfo->contactPosition);
		} 
	}

	NxReal OneMinusBreakPedal = 1-brakeTorque;

	/*
	if(handBrake && getWheelFlag(WF_AffectedByHandbrake)) 
	{
	material->setDynamicFrictionV(1);
	material->setStaticFrictionV(4);
	material->setDynamicFriction(0.4f);
	material->setStaticFriction(1.0f);
	} 
	else 
	{
	NxReal newv = OneMinusBreakPedal * _frictionToFront + brakeTorque;
	NxReal newv4= OneMinusBreakPedal * _frictionToFront + brakeTorque*4;
	material->setDynamicFrictionV(newv);
	material->setDynamicFriction(_frictionToSide);

	material->setStaticFrictionV(newv*4);
	material->setStaticFriction(2);
	}*/

	if(!hasGroundContact())
		updateContactPosition();
	updateAngularVelocity(dt, handbrake);

	contactInfo->reset();
}
VxVector pWheel1::getWheelPos()const{ 	return getFrom(wheelCapsule->getLocalPosition()); }
void pWheel1::setWheelOrientation(const NxMat33& m)
{ 
	wheelCapsule->setLocalOrientation(m); 
	if (wheelConvex != NULL) 
		wheelConvex->setLocalOrientation(m); 
}

void pWheel1::_updateAgeiaShape(bool position,bool rotation)
{

}

int pWheel1::_constructWheel(NxActor *actor,pObjectDescr *descr,pWheelDescr *wheelDescr,CKMesh *mesh,VxVector localPos,VxQuaternion localRotation)
{
	return 1;
}
pWheel1::pWheel1(pRigidBody *body, pWheelDescr *descr) : pWheel(body,descr)
{
	wheelCapsule = NULL;
	wheelConvex = NULL;

	contactInfo = new ContactInfo();


}



