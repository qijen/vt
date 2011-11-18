#include <StdAfx.h>
#include "vtPhysXAll.h"

#include <xDebugTools.h>

void pWheel2::_tick(float dt)
{
	NxWheelShape *wShape = getWheelShape();
	if (!wShape) return;


	NxVec3 _localVelocity;
	bool _breaking=false;
	//////////////////////////////////////////////////////////////////////////
	//
	//
	//
	NxWheelContactData wcd; 
	NxShape* contactShape = wShape->getContact(wcd);

	if (contactShape)
	{

		NxVec3 relativeVelocity;
		if ( !contactShape->getActor().isDynamic())
		{
			relativeVelocity = getActor()->getLinearVelocity();
		} else {
			relativeVelocity = getActor()->getLinearVelocity() - contactShape->getActor().getLinearVelocity();
		}
		NxQuat rotation = getActor()->getGlobalOrientationQuat();

		_localVelocity = relativeVelocity;
		rotation.inverseRotate(_localVelocity);
		_breaking = false; //NxMath::abs(_localVelocity.z) < ( 0.1 );
		//					wShape->setAxleSpeed()
	}


	float rollAngle = getWheelRollAngle();
	
	rollAngle+=wShape->getAxleSpeed() * (dt* 0.01f);
	//rollAngle+=wShape->getAxleSpeed() * (1.0f/60.0f /*dt* 0.01f*/);

	while (rollAngle > NxTwoPi)	//normally just 1x
		rollAngle-= NxTwoPi;
	while (rollAngle< -NxTwoPi)	//normally just 1x
		rollAngle+= NxTwoPi;

	setWheelRollAngle(rollAngle);

	NxMat34& wheelPose = wShape->getGlobalPose();


	NxReal  stravel = wShape->getSuspensionTravel();
	NxReal radius = wShape->getRadius();


	//have ground contact?
	if( contactShape && wcd.contactPosition <=  (stravel + radius) ) {
		wheelPose.t = NxVec3( wheelPose.t.x, wcd.contactPoint.y + getRadius(), wheelPose.t.z );
	}
	else {
		wheelPose.t = NxVec3( wheelPose.t.x, wheelPose.t.y - getSuspensionTravel(), wheelPose.t.z );
	}


	float rAngle = getWheelRollAngle();
	float steer = wShape->getSteerAngle();

	NxVec3 p0;
	NxVec3 dir;
	/*
	getWorldSegmentFast(seg);
	seg.computeDirection(dir);
	dir.normalize();
	*/
	NxReal r = wShape->getRadius();
	NxReal st = wShape->getSuspensionTravel();
	NxReal steerAngle = wShape->getSteerAngle();
	p0 = wheelPose.t;  //cast from shape origin
	wheelPose.M.getColumn(1, dir);
	dir = -dir;	//cast along -Y.
	NxReal castLength = r + st;	//cast ray this long


	NxMat33 rot, axisRot, rollRot;
	rot.rotY( wShape->getSteerAngle() );
	axisRot.rotY(0);
	rollRot.rotX(rAngle);
	wheelPose.M = rot * wheelPose.M * axisRot * rollRot;
	setWheelPose(wheelPose);
}

pWheelContactData* pWheel2::getContact()
{
	NxWheelShape *wShape = getWheelShape();
	if (!wShape)
	{
		return new pWheelContactData();
	}
	
	NxWheelContactData wcd; 
	NxShape* contactShape = wShape->getContact(wcd);
	
	pWheelContactData result;

	if (contactShape)
	{

		result.contactForce = wcd.contactForce;
		result.contactNormal = getFrom(wcd.contactNormal);
		result.contactPoint= getFrom(wcd.contactPoint);
		result.contactPosition= wcd.contactPosition;

		
		result.lateralDirection= getFrom(wcd.lateralDirection);
		result.lateralImpulse= wcd.lateralImpulse;
		result.lateralSlip = wcd.lateralSlip;
		
		result.longitudalDirection = getFrom(wcd.longitudalDirection);
		result.longitudalImpulse = wcd.longitudalImpulse;
		result.longitudalSlip= wcd.longitudalSlip;

		pSubMeshInfo *sInfo  = static_cast<pSubMeshInfo*>(contactShape->userData);
		if (sInfo->entID)
		{
			CKObject *obj = (CKObject*)GetPMan()->m_Context->GetObject(sInfo->entID);
			if (obj)
			{
				result.contactEntity = (CK3dEntity*)obj;
			}else
			{
				result.contactEntity = NULL;
			}
		}

		result.otherShapeMaterialIndex = contactShape->getMaterial();
		
		NxMaterial* otherMaterial = contactShape->getActor().getScene().getMaterialFromIndex(contactShape->getMaterial());
		if (otherMaterial)
		{
			pFactory::Instance()->copyTo(result.otherMaterial,otherMaterial);
		}
	}
	return &result;
}

void pWheel2::_updateVirtoolsEntity(bool position,bool rotation)
{

	CK3dEntity *ent  = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(getEntID()));
	if (ent && position) 
	{

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

			NxWheelShape *wShape = getWheelShape();
			NxMat34& wheelPose = wShape->getGlobalPose();

/*			NxWheelContactData wcd;
			NxShape* cShape = wShape->getContact(wcd);	
			NxReal  stravel = wShape->getSuspensionTravel();
			NxReal radius = wShape->getRadius();

			VxVector gPos  = getFrom(getWheelPose().t);

			/*
			if( cShape && wcd.contactPosition <=  (stravel + radius) ) 
			{
			}*/

			//////////////////////////////////////////////////////////////////////////

			/*VxVector gPos  = getFrom(getWheelPose().t);
			//gPos*=-1.0f;
			gPos -=getWheelPos();
			V	3.
				xVector gPos2  = getFrom(getWheelShape()->getLocalPose().t);
			ent->SetPosition(&gPos2,getBody()->GetVT3DObject());
			*/
		}else
		{
//			VxVector gPos  = getWheelPos();
//			ent->SetPosition(&gPos,getBody()->GetVT3DObject());
		}
	}
	if (ent && rotation)
	{


		//float rollAngle = getWheelRollAngle();
		//rollAngle+=wShape->getAxleSpeed() * (dt* 0.01f);
        
		VxQuaternion rot  = pMath::getFrom( getWheelPose().M );
		ent->SetQuaternion(&rot,NULL);
	}


	/*



	NxWheelShape *wShape = getWheelShape();

	


	while (rollAngle > NxTwoPi)	//normally just 1x
	rollAngle-= NxTwoPi;
	while (rollAngle< -NxTwoPi)	//normally just 1x
	rollAngle+= NxTwoPi;

	setWheelRollAngle(rollAngle);


	NxMat34& wheelPose = wShape->getGlobalPose();

	NxWheelContactData wcd;
	NxShape* cShape = wShape->getContact(wcd);	
	NxReal  stravel = wShape->getSuspensionTravel();
	NxReal radius = wShape->getRadius();


	//have ground contact?
	if( cShape && wcd.contactPosition <=  (stravel + radius) ) {
	wheelPose.t = NxVec3( wheelPose.t.x, wcd.contactPoint.y + getRadius(), wheelPose.t.z );
	}
	else {
	wheelPose.t = NxVec3( wheelPose.t.x, wheelPose.t.y - getSuspensionTravel(), wheelPose.t.z );
	}

	float rAngle = rollAngle;
	float steer = wShape->getSteerAngle();

	NxMat33 rot, axisRot, rollRot;
	rot.rotY( wShape->getSteerAngle() );
	axisRot.rotY(0);
	rollRot.rotX(rollAngle);
	wheelPose.M = rot * wheelPose.M * axisRot * rollRot;
	setWheelPose(wheelPose);

	*/
}

void pWheel2::_updateAgeiaShape(bool position,bool rotation)
{

}

float pWheel2::getRadius()const
{
	return mWheelShape->getRadius();
}

float pWheel2::getRpm() const
{
	float a = NxMath::abs(mWheelShape->getAxleSpeed())/NxTwoPi * 60.0f;
	return NxMath::abs(mWheelShape->getAxleSpeed())/NxTwoPi * 60.0f;
}
VxVector pWheel2::getWheelPos() const
{
	return getFrom(mWheelShape->getLocalPosition());
}

void pWheel2::setAngle(float angle)
{
	mWheelShape->setSteerAngle(-angle);
}

NxActor*pWheel2::getTouchedActor()const
{
	NxWheelContactData wcd;
	NxShape * s = mWheelShape->getContact(wcd);	
	return s ? &s->getActor() : NULL;
}
float pWheel2::getAxleSpeed()const
{
	if (mWheelShape)
	{
		return mWheelShape->getAxleSpeed();
	}
	return -1.f;

}
bool pWheel2::hasGroundContact() const
{
	return getTouchedActor() != NULL; 
}
void pWheel2::tick(bool handBrake, float motorTorque, float brakeTorque, float dt)
{
	motorTorque *= 10.0f;
	brakeTorque *= 50.0f;
	if(handBrake && getWheelFlag(WF_AffectedByHandbrake))
		brakeTorque = 1000.0f;
		
	
	if(getWheelFlag(WF_Accelerated)) 
		mWheelShape->setMotorTorque(motorTorque);
	mWheelShape->setBrakeTorque(brakeTorque);


	XString errMessage;
	//errMessage.Format("mT:%f | bT:%f",motorTorque,brakeTorque);
	//xInfo(errMessage.Str());


	NxWheelShape *wShape = getWheelShape();

	float rollAngle = getWheelRollAngle();
	rollAngle+=wShape->getAxleSpeed() * (dt* 0.01f);
	//rollAngle+=wShape->getAxleSpeed() * (1/60);
	

	while (rollAngle > NxTwoPi)	//normally just 1x
		rollAngle-= NxTwoPi;
	while (rollAngle< -NxTwoPi)	//normally just 1x
		rollAngle+= NxTwoPi;

	setWheelRollAngle(rollAngle);


	NxMat34& wheelPose = wShape->getGlobalPose();

	NxWheelContactData wcd;
	NxShape* cShape = wShape->getContact(wcd);	
	NxReal  stravel = wShape->getSuspensionTravel();
	NxReal radius = wShape->getRadius();


	//have ground contact?
	if( cShape && wcd.contactPosition <=  (stravel + radius) ) {
		wheelPose.t = NxVec3( wheelPose.t.x, wcd.contactPoint.y + getRadius(), wheelPose.t.z );
	}
	else {
		wheelPose.t = NxVec3( wheelPose.t.x, wheelPose.t.y - getSuspensionTravel(), wheelPose.t.z );
	}

	float rAngle = rollAngle;
	float steer = wShape->getSteerAngle();

	NxMat33 rot, axisRot, rollRot;
	rot.rotY( wShape->getSteerAngle() );
	axisRot.rotY(0);
	rollRot.rotX(rollAngle);
	wheelPose.M = rot * wheelPose.M * axisRot * rollRot;
	setWheelPose(wheelPose);
}
VxVector pWheel2::getGroundContactPos()const
{
	VxVector pos   = getWheelPos()+VxVector(0, -mWheelShape->getRadius(), 0);

	if (pos.Magnitude())
	{
		int op2 = 0 ; 
		op2++;
	}

	return pos;
}

float pWheel2::getSuspensionTravel()const
{

	if (mWheelShape)
	{
		return mWheelShape->getSuspensionTravel();
	}
	return 0.0f;
}


bool pWheel2::setSuspensionSpring(const pSpring& spring)
{

	NxSpringDesc sLimit;	sLimit.damper = spring.damper;sLimit.spring=spring.spring;sLimit.targetValue=spring.targetValue;
	if (!sLimit.isValid())return false;
	NxWheelShape *wShape = getWheelShape();
	if (!wShape)
	{
		return false;
	}
	wShape->setSuspension(sLimit);
	return true;
}

void pWheel2::setAxleSpeed(float speed)
{

	getWheelShape()->setAxleSpeed(speed);
}

void pWheel2::setMotorTorque(float torque)
{
	getWheelShape()->setMotorTorque(torque);
}
void pWheel2::setBreakTorque(float torque)
{
	getWheelShape()->setBrakeTorque(torque);
}

void pWheel2::setSuspensionTravel(float travel)
{
	getWheelShape()->setSuspensionTravel(travel);
}


pWheel2::pWheel2(pRigidBody *body, pWheelDescr *descr) : pWheel(body,descr)
{
	this->setBody(body);
	mWheelShape = NULL;
	mWheelFlags = descr->wheelFlags;
	_wheelRollAngle = 0.0f;

}