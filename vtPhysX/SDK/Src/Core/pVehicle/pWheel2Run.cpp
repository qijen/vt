#include <StdAfx.h>
#include "vtPhysXAll.h"

#include <xDebugTools.h>
#include "pWorldCallbacks.h"
#include "pCallbackSignature.h"




#include "virtools/vtTools.h"
#include "pVehicleAll.h"

using namespace vtTools::AttributeTools;
using namespace vtTools::ParameterTools;
using namespace vtTools::BehaviorTools;

bool pWheel2::isTorqueFromVehicle()
{
	return	getWheelFlag(WF_Accelerated)  && 
			getVehicle() && getVehicle()->isValidEngine() && 
			getDifferential() ;
}
bool pWheel2::isAxleSpeedFromVehicle()
{
	return	(	getWheelFlag(WF_Accelerated) ) && 
			(	getWheelShape()->getWheelFlags() & WSF_AxleSpeedOverride ) &&
				getVehicle() && getVehicle()->isValidEngine() && getDifferential() ;
}
void pWheel2::applyTorqueToPhysics()
{
	if (getDifferential())
	{

		pDifferential *diff = getDifferential();

		float finalTorqueOut = diff->GetTorqueOut(differentialSide);

		mWheelShape->setMotorTorque(finalTorqueOut);
		





		// Add torque to body because of the accelerating drivetrain
		// This gives a bit of the GPL effect where your car rolls when
		// you throttle with the clutch disengaged.
		
		float tr=getVehicle()->getEngine()->GetTorqueReaction();
		if(tr>0)
		{
//			VxVector torque(0,0,diff->GetAccIn()*diff->inertiaIn*tr);
//			getBody()->addTorque(torque);
		}

	/*	if(	(wheel->getWheelShape()->getWheelFlags() & WSF_AxleSpeedOverride ) )
		{
			float v = wheel->rotationV.x;
			v = v *  getEngine()->getEndRotationalFactor() * getEngine()->getTimeScale();	
		}
		*/
	}
}

void pWheel2::_createInternalContactModifyCallback()
{

	if (mWheelShape)
	{
		if (!wheelContactModifyCallback)
		{
			wheelContactModifyCallback = new pWheelContactModify();
			wheelContactModifyCallback->setWheel(this);
			mWheelShape->setUserWheelContactModify((NxUserWheelContactModify*)wheelContactModifyCallback);

		}
		
		//----------------------------------------------------------------
		//track information about callback	
		if (getBody())
			getBody()->getCallMask().set(CB_OnWheelContactModify,true);
	}


}

bool pWheelContactModify::onWheelContact(NxWheelShape* wheelShape, 
										 NxVec3& contactPoint, 
										 NxVec3& contactNormal, 
										 NxReal& contactPosition,
										 NxReal& normalForce, 
										 NxShape* otherShape, 
										 NxMaterialIndex& otherShapeMaterialIndex, 
										 NxU32 otherShapeFeatureIndex)
{
	pWheel2 *wheel  = static_cast<pWheel2*>(getWheel());
	if (!getWheel())
		return true;

	
	int contactModifyFlags = 0 ;
	bool createContact=true;


	//----------------------------------------------------------------
	//
	// store compact : 
	//
	if (wheel->getBody()->getCallMask().test(CB_OnWheelContactModify))
	{
		pWheelContactModifyData &contactData = lastData;
		contactData.object = getEntityFromShape(wheelShape);
		contactData.contactNormal = getFrom(contactNormal);
		contactData.contactPoint = getFrom(contactPoint);
		contactData.contactPosition  = contactPosition;
		contactData.normalForce = normalForce;
		contactData.otherMaterialIndex = otherShapeMaterialIndex;
		
		createContact = wheel->onWheelContactModify(contactModifyFlags,&contactData);


		if (!createContact)
			return false;

		if (contactModifyFlags==0)
			return true;

		//----------------------------------------------------------------
		//
		// copy result back to sdk
		//
		contactNormal = getFrom(contactData.contactNormal);
		contactPoint = getFrom(contactData.contactPoint);
		contactPosition = contactData.contactPosition;
		normalForce = contactData.normalForce;


	}

	//xWarning("whatever");
	return true;
}
bool pWheel2::onWheelContactModify(int& changeFlags,pWheelContactModifyData* contact)
{



	bool result = true;
	//----------------------------------------------------------------
	//
	// sanity checks
	//
	if (!contact)
		return true;
	
	//----------------------------------------------------------------
	//
	// keep some informationens for our self
	//
	if( getProcessOptions() & pVPO_Wheel_UsePHYSX_Load &&
		getProcessOptions() & pVPO_Wheel_UsePHYSX_CONTACT_DATA && getVehicle() 
		)
	{
		load = contact->contactNormal.y;

		if (load > 1500)
			load = 1500;

		if (load <= 0)
			load = 1000;


	}

	CKBehavior * beh  = (CKBehavior*)GetPMan()->GetContext()->GetObject(getWheelContactScript());
	if (beh && CKGetObject(ctx(),getEntID()))
	{
		
		SetInputParameterValue<CK_ID>(beh,bbIWC_SrcObject,getEntID());

		SetInputParameterValue<VxVector>(beh,bbIWC_Point,contact->contactPoint);
		SetInputParameterValue<VxVector>(beh,bbIWC_Normal,contact->contactNormal);
		SetInputParameterValue<float>(beh,bbIWC_Position,contact->contactPosition);
		SetInputParameterValue<float>(beh,bbIWC_NormalForce,contact->normalForce);
		SetInputParameterValue<int>(beh,bbIWC_OtherMaterialIndex,contact->otherMaterialIndex);
		//----------------------------------------------------------------
		//
		// execute:
		//
		beh->Execute(lastStepTimeSec);

		//----------------------------------------------------------------
		//
		// refuse contact 
		//
		result = GetOutputParameterValue<int>(beh,bbOWC_CreateContact);
		if (!result)
			return false;

		//----------------------------------------------------------------
		//
		// nothing changed, return true
		//
		changeFlags = GetOutputParameterValue<int>(beh,bbOWC_ModificationFlags);
		if (changeFlags == 0 )
		{
			return true;
		}

		//----------------------------------------------------------------
		//
		// pickup data, according to change flags
		//
		if (changeFlags & CWCM_ContactPoint )
			contact->contactPoint = GetOutputParameterValue<VxVector>(beh,bbOWC_Point);

		if (changeFlags & CWCM_ContactNormal)
			contact->contactNormal= GetOutputParameterValue<VxVector>(beh,bbOWC_Normal);
		
		if (changeFlags & CWCM_ContactPosition )
			contact->contactPosition= GetOutputParameterValue<float>(beh,bbOWC_Position);

		if (changeFlags & CWCM_NormalForce )
			contact->normalForce = GetOutputParameterValue<float>(beh,bbOWC_NormalForce);
	}

		

	return true;

}

bool pWheel2::onWheelContact(CK3dEntity* wheelShapeReference, VxVector& contactPoint, VxVector& contactNormal, float& contactPosition, float& normalForce, CK3dEntity* otherShapeReference, int& otherShapeMaterialIndex)
{
	//NxUserAllocator
	return true;
}

void pWheel2::setWheelContactScript(int val)
{

	CKBehavior * beh  = (CKBehavior*)GetPMan()->GetContext()->GetObject(val);
	if (!beh)
		return;

	XString errMessage;
	if (!GetPMan()->checkCallbackSignature(beh,CB_OnWheelContactModify,errMessage))
	{
		xError(errMessage.Str());
		return;
	}
	
	pCallbackObject::setWheelContactScript(val);

	wheelContactModifyCallback  = new pWheelContactModify();
	wheelContactModifyCallback->setWheel(this);

	getWheelShape()->setUserWheelContactModify((NxUserWheelContactModify*)wheelContactModifyCallback);


	//----------------------------------------------------------------
	//track information about callback	
	getBody()->getCallMask().set(CB_OnWheelContactModify,true);


}
void pWheel2::processPreScript()
{

}
void pWheel2::processPostScript()
{

}
int pWheel2::onPostProcess()
{

	return 1;
}
int pWheel2::onPreProcess()
{

	return 1;
}

void pWheel2::_tick(float dt)
{


	
	float dt2 = dt;


	NxWheelShape *wShape = getWheelShape();
	if (!wShape) return;


	NxVec3 _localVelocity;
	bool _breaking=false;

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
	
	rollAngle+=wShape->getAxleSpeed() * (dt);
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
