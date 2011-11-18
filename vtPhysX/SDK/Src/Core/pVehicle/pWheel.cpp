#include <StdAfx.h>
#include "vtPhysXAll.h"
#include <xDebugTools.h>


NxActor*pWheel::getTouchedActor()const{	return NULL;}

void pWheelDescr::setToDefault()
{

	userData = NULL;
	wheelFlags =(WheelFlags)0;

	//radius.setToDefault();
	springBias = 0;
	springRestitution = 1.f;
	springDamping = 0.f;

	wheelSuspension = 1.f;
	maxBrakeForce = 0.0f;
	frictionToSide = 1.0f;
	frictionToFront = 1.0f;
	latFuncXML_Id=0;
	longFuncXML_Id=0;
	inverseWheelMass = 0.1f;
	wheelShapeFlags =(WheelShapeFlags)0;

	latFunc.setToDefault();
	longFunc.setToDefault();
	




}
bool pWheelDescr::isValid() const
{

	/*if(!NxMath::isFinite(radius))	return false;
	if(radius<=0.0f)				return false;*/
	
	bool result = true;
	int a=X_NEGATE(NxMath::isFinite(wheelSuspension));
	//iAssertWR(X_NEGATE(NxMath::isFinite(wheelSuspension)),"",result );
	iAssertWR(inverseWheelMass > 0.0f,"",result );
	iAssertWR(X_NEGATE(inverseWheelMass<0.0f),"",result );
	//iAssertWR(X_NEGATE(brakeTorque<0.0f),"",result );
	//iAssertWR(X_NEGATE(NxMath::isFinite(steerAngle)),"",result );
	//iAssertWR(X_NEGATE(brakeTorque<0.0f),"",result );
	iAssertWR(longFunc.isValid(),"",result );
	iAssertWR(latFunc.isValid(),"",result );

/*	if (!suspension.isValid()) return false;
	if (!longitudalTireForceFunction.isValid()) return false;
	if (!lateralTireForceFunction.isValid()) return false;
*/
	//if (NxMath::abs(1-wheelAxis.magnitudeSquared()) > 0.001f)
	//	return false;
	if (wheelApproximation > 0 && wheelApproximation < 4) {

		return false;
	}
	if ((wheelFlags & WF_SteerableAuto) && (wheelFlags & WF_SteerableInput)) 
	{
		return false;
	}
	return result;
}

int pWheel::_constructWheel(NxActor *actor,pObjectDescr *descr,pWheelDescr *wheelDescr,CKMesh *mesh,VxVector localPos,VxQuaternion localRotation)
{
	return 1;
}

void pWheel::setFlags(int flags)
{
	mWheelFlags = flags;
}

pWheel::pWheel(pRigidBody *body,pWheelDescr *descr)
{

	mBody = body;
	mWheelFlags = descr->wheelFlags;
	_wheelRollAngle = 0.0f;
	mActor= body->getActor();



}

pWheel1*pWheel::castWheel1()
{
	return dynamic_cast<pWheel1*>(this);
}
pWheel2*pWheel::castWheel2()
{

	return dynamic_cast<pWheel2*>(this);
}




