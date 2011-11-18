#include <StdAfx.h>
#include "vtPhysXAll.h"

#include <xDebugTools.h>
#include "pVehicleAll.h"
#define RR_RAD_DEG_FACTOR	57.29578f	// From radians to degrees"


float pWheel2::getSteerAngle()
{

	return mWheelShape->getSteerAngle();

}
void pWheel2::setSteering(float angle)
{
	if(angle>lock)angle=lock;
	else if(angle<-lock)angle=-lock;
	rotation.y=angle+toe;
	// Apply Ackerman effect
	if((ackermanFactor<0&&angle<0)||(ackermanFactor>0&&angle>0))
		rotation.y*=fabs(ackermanFactor);

	float a = rotation.y;
	
	float steerangle= rotation.y * lastStepTimeSec;

	
	while (steerangle > NxTwoPi)	//normally just 1x
		steerangle-= NxTwoPi;
	while (steerangle< -NxTwoPi)	//normally just 1x
		steerangle+= NxTwoPi;

	getWheelShape()->setSteerAngle(steerangle);




}


bool pWheel2::getContact(pWheelContactData&dst)
{

	NxWheelContactData wcd; 
	NxShape* contactShape = mWheelShape->getContact(wcd);
	dst.contactEntity = NULL;
	if (contactShape)
	{

		dst.contactForce = wcd.contactForce;
		dst.contactNormal = getFrom(wcd.contactNormal);
		dst.contactPoint= getFrom(wcd.contactPoint);
		dst.contactPosition= wcd.contactPosition;


		dst.lateralDirection= getFrom(wcd.lateralDirection);
		dst.lateralImpulse= wcd.lateralImpulse;
		dst.lateralSlip = wcd.lateralSlip;

		dst.longitudalDirection = getFrom(wcd.longitudalDirection);
		dst.longitudalImpulse = wcd.longitudalImpulse;
		dst.longitudalSlip= wcd.longitudalSlip;

		pSubMeshInfo *sInfo  = static_cast<pSubMeshInfo*>(contactShape->userData);
		if (sInfo->entID)
		{
			CKObject *obj = (CKObject*)GetPMan()->m_Context->GetObject(sInfo->entID);
			if (obj)
			{
				dst.contactEntity = (CK3dEntity*)obj;
			}else
			{
				dst.contactEntity = NULL;
			}
		}

		dst.otherShapeMaterialIndex = contactShape->getMaterial();

		NxMaterial* otherMaterial = contactShape->getActor().getScene().getMaterialFromIndex(contactShape->getMaterial());
		if (otherMaterial)
		{
			pFactory::Instance()->copyTo(dst.otherMaterial,otherMaterial);
		}

		return true;

	}
	
	return false;
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
	result.contactEntity = NULL;


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


	NxWheelShape *wShape = getWheelShape();

	//NxWheelShape *wShape = getWheelShape();
/*
	float rollAngle = getWheelRollAngle();
	//rollAngle+=wShape->getAxleSpeed() * (dt* 0.01f);
	rollAngle+=wShape->getAxleSpeed() * ((1/60) * 0.01f);
*/
	

	

	/*

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
	if (getVehicle())
	{

		float b = NxMath::abs(mWheelShape->getAxleSpeed())/NxTwoPi * getVehicle()->_lastDT;
		return  b;
	}
	
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
	if (s)
	{
		if (&s->getActor())
		{
			return &s->getActor();
		}else
		{
			return NULL;
		}
		
	}else
	{
		return NULL;
	}

	return NULL;
	//return s ? &s->getActor() : NULL;
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
	if(handBrake && getWheelFlag(WF_AffectedByHandbrake))
		brakeTorque = 1000.0f;

	if(getWheelFlag(WF_Accelerated)) 
		mWheelShape->setMotorTorque(motorTorque);
	
	mWheelShape->setBrakeTorque(brakeTorque);

	/*
	NxWheelShape *wShape = getWheelShape();
	float rollAngle = getWheelRollAngle();
	rollAngle+=wShape->getAxleSpeed() * (dt* 0.01f);

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
pWheel2::pWheel2(pRigidBody *body,pWheelDescr *descr,CK3dEntity *wheelShapeReference): pWheel(body,descr) ,xEngineObjectAssociation<CK3dEntity*>(wheelShapeReference,wheelShapeReference->GetID())
{
	this->setBody(body);
	this->setEntity(wheelShapeReference);




	brakingFactor = 0.05f;
	frictionCoeff = 1.0f;
	rollingCoeff = 1.0f;

	mWheelShape = NULL;
	mWheelFlags = descr->wheelFlags;
	_wheelRollAngle = 0.0f;
	mVehicle = NULL;
	lastContactData = new pWheelContactData();
	differentialSide=0;
	differential = NULL;
	hadContact = false;


	slip2FCVelFactor = 1.0f;

	
	tanSlipAngle=0;
	  slipAngle=0;
	  slipRatio=0;
	  lastU=0;
	  signU=1;         // u>=0
	  differentialSlipRatio=0;

	maxBrakingTorque = 400.0;


	
	
	
	lock = 80.0f;
	ackermanFactor=1.1f;
	tireRate = 180000;
	mass = 10.0f;


	
	radius = descr->radius.value;
	radiusLoaded = descr->radius.value;

	SetInertia(5.3f);
				
	relaxationLengthLat=0.91f;
	relaxationLengthLong=0.091f;
	
	dampingSpeed = 0.15f;
	dampingCoefficientLong = 1.5f;
	dampingFactorLat=0.75;
	dampingFactorLong=1.5f;
	dampingCoefficientLat = 1.066f;


	toe =-(1.0f/RR_RAD_DEG_FACTOR);

	rotation.Set(0,0,0);
	rotation.y = toe;

	//----------------------------------------------------------------
	//
	// pacejka
	//
	pacejka.setToDefault();
	//Statistical data (SR, SA in radians)
	optimalSR=0.09655f;
	optimalSA=0.18296f;





	stateFlags = 0;
	lowSpeedFactor = 0.0f;
	slipAngle = 0.0f;
	slipRatio = 0.0f;
	
	overrideMask = 0;
	callMask.set(CB_OnPostProcess,1);
	callMask.set(CB_OnPreProcess,1);


	preScript = postScript = 0;
	entity = NULL;
	
	wheelContactModifyCallback =NULL;

	differential = NULL;
	differentialSide =-1;




}
void pWheel2::setDifferential(pDifferential *diff,int side)
{
	differential=diff; differentialSide=side; 

}
float pWheel2::getEndBrakingTorqueForWheel()
{
	if(differential)
	{
		return differential->GetBreakTorqueOut(differentialSide);
	}
	return 0.0f;
}
float pWheel2::getEndTorqueForWheel()
{
	if(differential)
	{
//*2*PI/60.0f
		return differential->GetTorqueOut(differentialSide);
	}
	return 0.0f;
}

float pWheel2::getEndAccForWheel()
{
	if(differential)
	{
		return differential->GetAccOut(differentialSide);
	}
	return 0.0f;
}

float pWheel2::getWheelBreakTorque()
{
	if(mWheelShape)
	{
		return mWheelShape->getBrakeTorque();
	}
	return 0.0f;
}
float pWheel2::getWheelTorque()
{
	if(mWheelShape)
	{
		return mWheelShape->getMotorTorque();
	}
	return 0.0f;
}
