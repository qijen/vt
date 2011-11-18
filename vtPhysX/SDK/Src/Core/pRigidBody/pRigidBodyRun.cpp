#include <StdAfx.h>
#include "vtPhysXAll.h"
#include <xDebugTools.h>
#include "pCallbackObject.h"
#include "pCallbackSignature.h"
#include <xBitSet.h>

#include "virtools/vtTools.h"
#include "vtInterfaceEnumeration.h"
#include "vtAttributeHelper.h"

static float incrTimer = 0.0f;

using namespace vtTools::AttributeTools;
using namespace vtTools::ParameterTools;
using namespace vtTools::BehaviorTools;

int getJointType(int attributeIndex)
{
	switch(attributeIndex)
	{
		case 1:
			return JT_Distance;
		case 2:
			return JT_Fixed;
		case 3:
			return JT_Spherical;
		case 4:
			return JT_Prismatic;
		case 5:
			return JT_PointInPlane;
		case 6:
			return JT_PointOnLine;
		case 7:
			return JT_Cylindrical;
		case 8:
			return JT_Revolute;
		case 9:
			return JT_D6;
		default:
				return -1;
	}

	return -1;

}

void pRigidBody::onICRestore(CK3dEntity* parent,pRigidBodyRestoreInfo *restoreInfo)
{


	if (!parent)
		return;

	CKScene *level_scene  = GetPMan()->GetContext()->GetCurrentLevel()->GetLevelScene();
	if(level_scene)
	{
		if(level_scene)
		{
			CKStateChunk *chunk = level_scene->GetObjectInitialValue(parent);
			if (chunk)
			{
				CKReadObjectState(parent,chunk);
			}
		}
	}
	

	VxVector pos(0,0,0);
	VxVector scale;	
	VxQuaternion quat;
	Vx3DDecomposeMatrix(parent->GetWorldMatrix(),quat,pos,scale);


	VxVector vectorN(0,0,0);
	setLinearMomentum(vectorN);
	setAngularMomentum(vectorN);
	setLinearVelocity(vectorN);
	setAngularVelocity(vectorN);
	parent->GetPosition(&pos);
	
	setPosition(pos);
	setRotation(quat);
	
	if (level_scene && restoreInfo->hierarchy)
	{
	
		CK3dEntity* subEntity = NULL;
		XArray<NxShape*>SrcObjects;
		while (subEntity= parent->HierarchyParser(subEntity) )
		{
			CKStateChunk *chunk = level_scene->GetObjectInitialValue(subEntity);
			if (chunk)
			{
				CKReadObjectState(subEntity,chunk);
			}
		}
	}
	
	onSubShapeTransformation(false,true,true,parent,restoreInfo->hierarchy);

	bool hadJoints = false;
	
	if (restoreInfo->removeJoints)
	{
		CKAttributeManager* attman = GetPMan()->GetContext()->GetAttributeManager();
		int sizeJFuncMap = ATT_FUNC_TABLE_SIZE;//(sizeof(*getRegistrationTable()) / sizeof((getRegistrationTable())[0]));
		for (int fIndex = 1  ; fIndex  < sizeJFuncMap ; fIndex ++)
		{
			std::vector<int>attributeIdList;
			pFactory::Instance()->findAttributeIdentifiersByGuid(GetPMan()->getRegistrationTable()[fIndex].guid,attributeIdList);
			int attCount = attributeIdList.size();
			for (int i = 0 ; i < attCount ; i++ )
			{
				int currentAttType = attributeIdList.at(i);
				const XObjectPointerArray& Array = attman->GetAttributeListPtr( attributeIdList.at(i) );
				for (CKObject** it = Array.Begin(); it != Array.End(); ++it)
				{	
					CK3dEntity *target = static_cast<CK3dEntity*>(*it);
					if (target == parent  && fIndex != -1  )
					{
						using namespace vtTools::ParameterTools;
						CKParameterOut *attributeParameter = target->GetAttributeParameter(currentAttType);
						if (attributeParameter)
						{
							CKStructHelper sHelper(target->GetAttributeParameter(currentAttType));
							if (sHelper.GetMemberCount() ==0 )//happens when dev is being opened and loads a cmo with physic objects.
								continue;

							// get body b :
							CK_ID bodyBId = GetValueFromParameterStruct<CK_ID>(attributeParameter,0);
							CK3dEntity *bodyBEnt = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(bodyBId));

							pJoint *joint = getJoint(bodyBEnt,(JType)getJointType(fIndex));
							if (joint)
							{
								XString error;
								XString bodyBName;
								if (joint->GetVTEntB())
								{
									bodyBName.Format(" %s ",joint->GetVTEntB()->GetName());
								}
								error.Format("Deleting joint by attribute from : %s with attribute %s connected to :%s",target->GetName(),attman->GetAttributeNameByType(currentAttType),bodyBName.Str());
								xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,error.CStr() );
								deleteJoint(bodyBEnt, (JType)getJointType(fIndex));
								hadJoints = true;
								int b = getNbJoints();
								b++;
							}
						}
						/*
						XString error;
						error.Format("Deleting joint by attribute from : %s with %s",target->GetName(),attman->GetAttributeNameByType(currentAttType));
						xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,error.CStr() );*/
					}
				}
			}
		}
	}
	
	if(hadJoints)
		GetPMan()->getCheckList().PushBack(getEntID());

	//GetPMan()->checkPhysics = true;

	//////////////////////////////////////////////////////////////////////////
	// check for constraint attributes
	//GetPMan()->_checkObjectsByAttribute();
}

bool pRigidBody::onMove(bool position/* =true */,bool rotation/* =true */,VxVector pos,VxQuaternion quad)
{

	/************************************************************************/
	/*
	+	determine whether the specified "children" object have sub shapes !
	- recursive call of updateSubShape

	+ exit if any object has been updated

	*/
	/************************************************************************/


	//pSubMeshInfo *info =  static_cast<pSubMeshInfo*>(subShape->userData);
	
	
	CK3dEntity* subEntity = NULL;
	XArray<NxShape*>SrcObjects;
	//SrcObjects.PushBack( subShape );

	
	CK_ID id = getEntID();
	CK3dEntity * thisEnt  = (CK3dEntity*)GetPMan()->GetContext()->GetObject(id);
	if(thisEnt)
		return false;
	while (subEntity= thisEnt->HierarchyParser(subEntity))
	{

		CKSTRING name =subEntity->GetName();
/*		NxShape *childShape = getSubShape(subEntity);
		if (childShape)
		{s
			pSubMeshInfo *childInfo =  static_cast<pSubMeshInfo*>(childShape->userData);
			if (childInfo)
			{
				CK3dEntity *child = (CK3dEntity*)ctx()->GetObject(childInfo->entID);
				if ( child && isSubShape(child)  && childInfo->initDescription.flags & BF_SubShape  )
				{
					//SrcObjects.PushBack(child);//

				}
			}
		}
*/

		//  body  ? 
		pRigidBody *body = GetPMan()->getBody(subEntity);
		if (body && body!=this )
		{
			body->updateSubShapes();
		}
	}

	/*
	for(int i = 0;i<SrcObjects.Size(); ++i) 
	{
		//subEntity = *SrcObjects.At(i);

		NxShape *s=  (static_cast<NxShape*>(*SrcObjects.At(i)));
		if (s)
		{
			pSubMeshInfo *childInfo =  static_cast<pSubMeshInfo*>(s->userData);
			CK3dEntity *ent = (CK3dEntity*)ctx()->GetObject(childInfo->entID);
			if (!ent)
				continue;

			if (position)
			{

				VxVector relPos;
				ent->GetPosition(&relPos,GetVT3DObject());
				s->setLocalPosition(getFrom(relPos));

				//onSubShapeTransformation(fromPhysicToVirtools,position,false,childObject);

			}

			if (rotation)
			{
				/*
				VxQuaternion refQuad2;
				ent->GetQuaternion(&refQuad2,GetVT3DObject());
				s->setLocalOrientation(getFrom(refQuad2));
				
				//onSubShapeTransformation(fromPhysicToVirtools,false,rotation,childObject);

			}
		}
	}

	*/
	return true;
}
bool pRigidBody::onSubShapeTransformation(bool fromPhysicToVirtools/* =true */,bool position/* =true */,bool rotation/* =true */,CK3dEntity*parent,bool children/* =true */)
{

	/************************************************************************/
	/*
		+	determine whether the specified "children" object have sub shapes !
			- recursive call of updateSubShape

		+ exit if any object has been updated

	*/
	/************************************************************************/

	if ( !parent  )
		return false;

	NxShape *subShape = getSubShape(parent);
	if (!subShape)
		return false;

	int count = parent->GetChildrenCount();

	if (subShape == getMainShape())
		return false;

	pSubMeshInfo *info =  static_cast<pSubMeshInfo*>(subShape->userData);
	CK3dEntity* subEntity = NULL;

	XArray<NxShape*>SrcObjects;
	SrcObjects.PushBack( subShape );
	
	while (subEntity= parent->HierarchyParser(subEntity) )
	{

		CKSTRING name =subEntity->GetName();
		NxShape *childShape = getSubShape(subEntity);
		if (childShape)
		{
			pSubMeshInfo *childInfo =  static_cast<pSubMeshInfo*>(childShape->userData);
			if (childInfo)
			{
				CK3dEntity *child = (CK3dEntity*)ctx()->GetObject(childInfo->entID);
				if ( child && isSubShape(child)  && childInfo->initDescription.flags & BF_SubShape  )
				{
					SrcObjects.PushBack( childShape );
				}
			}
		}
	}

	for(int i = 0;i<SrcObjects.Size(); ++i) 
	{
		NxShape *s=  (static_cast<NxShape*>(*SrcObjects.At(i)));
		if (s)
		{
			pSubMeshInfo *childInfo =  static_cast<pSubMeshInfo*>(s->userData);
			CK3dEntity *ent = (CK3dEntity*)ctx()->GetObject(childInfo->entID);
			if (!ent)
				continue;

			if (position)
			{
				
				VxVector relPos;
				ent->GetPosition(&relPos,GetVT3DObject());
				s->setLocalPosition(getFrom(relPos));
				//onSubShapeTransformation(fromPhysicToVirtools,position,false,childObject);

			}

			if (rotation)
			{
				VxQuaternion refQuad2;
				ent->GetQuaternion(&refQuad2,GetVT3DObject());
				s->setLocalOrientation(getFrom(refQuad2));
				//onSubShapeTransformation(fromPhysicToVirtools,false,rotation,childObject);

			}
		}
	}

	return true;
}
bool pRigidBody::onRayCastHit(NxRaycastHit *report)
{

	return true;
}
void pRigidBody::setRayCastScript(int val)
{
	CKBehavior * beh  = (CKBehavior*)GetPMan()->GetContext()->GetObject(val);
	if (!beh)
		return;

	XString errMessage;
	if (!GetPMan()->checkCallbackSignature(beh,CB_OnRayCastHit,errMessage))
	{
		xError(errMessage.Str());
		return;
	}
	pCallbackObject::setRayCastScript(val);
}
bool pRigidBody::onContactConstraint(int& changeFlags,CK3dEntity *sourceObject,CK3dEntity *otherObject,pContactModifyData *data)
{

	bool result = true;
	//----------------------------------------------------------------
	//
	// sanity checks
	//
	if (sourceObject != GetVT3DObject())
		return true;

	CKBehavior * beh  = (CKBehavior*)GetPMan()->GetContext()->GetObject(getContactModificationScript());
	if (!beh)
		return true;

	if (!data)
		return true;

	//----------------------------------------------------------------
	//
	// store data in behaviors inputs
	//

	if (sourceObject)
		SetInputParameterValue<CK_ID>(beh,bbICM_SrcObject,sourceObject->GetID());
	if (otherObject)
		SetInputParameterValue<CK_ID>(beh,bbICM_OtherObject,otherObject->GetID());

	SetInputParameterValue<float>(beh,bbICM_MinImpulse,data->minImpulse);
	SetInputParameterValue<float>(beh,bbICM_MaxImpulse,data->maxImpulse);
	
	SetInputParameterValue<VxVector>(beh,bbICM_Error,data->error);
	SetInputParameterValue<VxVector>(beh,bbICM_Target,data->target);
	
	SetInputParameterValue<VxVector>(beh,bbICM_LP0,data->localpos0);
	SetInputParameterValue<VxVector>(beh,bbICM_LP1,data->localpos1);
	
	SetInputParameterValue<VxQuaternion>(beh,bbICM_LO0,data->localorientation0);
	SetInputParameterValue<VxQuaternion>(beh,bbICM_LO1,data->localorientation1);

	SetInputParameterValue<float>(beh,bbICM_SF0,data->staticFriction0);
	SetInputParameterValue<float>(beh,bbICM_SF1,data->staticFriction1);

	SetInputParameterValue<float>(beh,bbICM_DF0,data->dynamicFriction0);
	SetInputParameterValue<float>(beh,bbICM_DF1,data->dynamicFriction1);

	SetInputParameterValue<float>(beh,bbICM_Restitution,data->restitution);

	//----------------------------------------------------------------
	//
	// execute:
	//
	beh->Execute(lastStepTimeMS);

	//----------------------------------------------------------------
	//
	// refuse contact 
	//
	result = GetOutputParameterValue<int>(beh,bbOCM_CreateContact);
	if (!result)
		return false;

	//----------------------------------------------------------------
	//
	// nothing changed, return true
	//
	changeFlags = GetOutputParameterValue<int>(beh,bbOCM_ModifyFlags);
	if (changeFlags == CMM_None )
	{
		return true;
	}

	//----------------------------------------------------------------
	//
	// pickup data, according to change flags
	//
	if (changeFlags & CMM_MinImpulse )
		data->minImpulse = GetOutputParameterValue<float>(beh,bbOCM_MinImpulse);

	if (changeFlags & CMM_MaxImpulse)
		data->maxImpulse = GetOutputParameterValue<float>(beh,bbOCM_MaxImpulse);

	if (changeFlags & CMM_Error )
		data->error = GetOutputParameterValue<VxVector>(beh,bbOCM_Error);
	
	if (changeFlags & CMM_Target )
		data->target = GetOutputParameterValue<VxVector>(beh,bbOCM_Target);


	if (changeFlags & CMM_StaticFriction0)
		data->staticFriction0 = GetOutputParameterValue<float>(beh,bbOCM_SF0);
	if (changeFlags & CMM_StaticFriction1)
		data->staticFriction1 = GetOutputParameterValue<float>(beh,bbOCM_SF1);

	if (changeFlags & CMM_DynamicFriction0)
		data->dynamicFriction0 = GetOutputParameterValue<float>(beh,bbOCM_DF0);
	if (changeFlags & CMM_DynamicFriction1)
		data->dynamicFriction1 = GetOutputParameterValue<float>(beh,bbOCM_DF1);

	if (changeFlags & CMM_LocalPosition0)
		data->localpos0 = GetOutputParameterValue<VxVector>(beh,bbOCM_LP0);
	if (changeFlags & CMM_LocalPosition1)
		data->localpos1 = GetOutputParameterValue<VxVector>(beh,bbOCM_LP1);

	if (changeFlags & CMM_LocalOrientation0)
		data->localorientation0 = GetOutputParameterValue<VxQuaternion>(beh,bbOCM_LO0);
	if (changeFlags & CMM_LocalOrientation1)
		data->localorientation1 = GetOutputParameterValue<VxQuaternion>(beh,bbOCM_LO1);

	if (changeFlags & CMM_Restitution)
		data->restitution = GetOutputParameterValue<float>(beh,bbOCM_Restitution);

	return true;
}

void pRigidBody::setContactModificationScript(int behaviorID)
{
	CKBehavior * beh  = (CKBehavior*)GetPMan()->GetContext()->GetObject(behaviorID);
	if (!beh)
		return;

	XString errMessage;
	if (!GetPMan()->checkCallbackSignature(beh,CB_OnContactModify,errMessage))
	{
		xError(errMessage.Str());
		return;
	}

	pCallbackObject::setContactModificationScript(behaviorID);
	enableContactModification(true);


}

void pRigidBody::setJointBreakScript(int behaviorID,CK3dEntity *shapeReference)
{
	CKBehavior * beh  = (CKBehavior*)GetPMan()->GetContext()->GetObject(behaviorID);
	if (!beh)
		return;

	XString errMessage;
	if (!GetPMan()->checkCallbackSignature(beh,CB_OnJointBreak,errMessage))
	{
		xError(errMessage.Str());
		return;
	}

	pCallbackObject::setJointBreakScript(behaviorID);
	
}


void pRigidBody::setTriggerScript(int behaviorID,int eventMask,CK3dEntity *shapeReference)
{
	/*if (descr.flags & BF_TriggerShape )
	{
		result->setFlag(NX_TRIGGER_ENABLE,TRUE);
	}
	*/
	CKBehavior * beh  = (CKBehavior*)GetPMan()->GetContext()->GetObject(behaviorID);
	if (!beh)
		return;

	XString errMessage;
	if (!GetPMan()->checkCallbackSignature(beh,CB_OnTrigger,errMessage))
	{
		xError(errMessage.Str());
		return;
	}

	NxShape * shape = getSubShape(shapeReference);
	//----------------------------------------------------------------
	//
	// check mask 
	//
	pTriggerFlags tFlags = (pTriggerFlags)eventMask;

	if	(	(eventMask & TF_OnEnter) ||
			(eventMask & TF_OnStay) ||
			(eventMask & TF_OnLeave)
		)
	{
		shape->setFlag(NX_TRIGGER_ON_ENTER,eventMask & TF_OnEnter);
		shape->setFlag(NX_TRIGGER_ON_STAY,eventMask & TF_OnStay);
		shape->setFlag(NX_TRIGGER_ON_LEAVE,eventMask & TF_OnLeave);
		pCallbackObject::setTriggerScript(behaviorID,eventMask);
	}else
	{
		shape->setFlag(NX_TRIGGER_ENABLE,false);
		pCallbackObject::setTriggerScript(-1,0);
	}
}
int pRigidBody::onTrigger(pTriggerEntry *report)
{
	CKBehavior * beh  = (CKBehavior*)GetPMan()->GetContext()->GetObject(getTriggerScript());
	if (!beh)
		return -1;

	if (!report)
		return -1;

	if (getTriggerEventMask() & report->triggerEvent )
	{
		if (report->triggerBody)
			SetInputParameterValue<CK_ID>(beh,bbIT_SrcObject,report->triggerBody->GetID());
	
		if (report->otherObject)
			SetInputParameterValue<CK_ID>(beh,bbIT_OtherObject,report->otherObject->GetID());

		SetInputParameterValue<int>(beh,bbIT_EventType,report->triggerEvent);
		report->triggered = true;
		beh->ActivateInput(0,TRUE);
		beh->Execute(lastStepTimeMS);

		return 1;


	}

	return -1;
}

void pRigidBody::setContactScript(int behaviorID,int eventMask)
{ 

	CKBehavior * beh  = (CKBehavior*)GetPMan()->GetContext()->GetObject(behaviorID);
	if (!beh)
		return;

	XString errMessage;
	if (!GetPMan()->checkCallbackSignature(beh,CB_OnContactNotify,errMessage))
	{
		xError(errMessage.Str());
		return;
	}

	pCallbackObject::setContactScript(behaviorID,eventMask);
	
	if (getActor())
		getActor()->setContactReportFlags(eventMask);

}

void pRigidBody::onContactNotify(pCollisionsEntry *collisionData)
{

	CKBehavior * beh  = (CKBehavior*)GetPMan()->GetContext()->GetObject(getContactScript());
	if (!beh)
		return;

	if (!collisionData)
		return;

	if (getCollisionEventMask() & collisionData->eventType )
	{

		if (collisionData->bodyA != this )
			XSwap(collisionData->bodyA,collisionData->bodyB);

		SetInputParameterValue<CK_ID>(beh,bbI_SrcObject,collisionData->bodyA->GetVT3DObject()->GetID());
		SetInputParameterValue<int>(beh,bbI_EventType,collisionData->eventType);
		SetInputParameterValue<VxVector>(beh,bbI_NormalForce,collisionData->sumNormalForce);
		SetInputParameterValue<VxVector>(beh,bbI_FForce,collisionData->sumFrictionForce);
		SetInputParameterValue<VxVector>(beh,bbI_Point,collisionData->point);
		SetInputParameterValue<float>(beh,bbI_PointNormalForce,collisionData->pointNormalForce);
		SetInputParameterValue<VxVector>(beh,bbI_FaceNormal,collisionData->faceNormal);
		SetInputParameterValue<int>(beh,bbI_FaceIndex,collisionData->faceIndex);
		SetInputParameterValue<float>(beh,bbI_Distance,collisionData->distance);


		if (collisionData->bodyB && collisionData->bodyB->GetVT3DObject() )
		{
			SetInputParameterValue<CK_ID>(beh,bbI_OtherObject,collisionData->bodyB->GetVT3DObject()->GetID());
		}
		beh->Execute(lastStepTimeMS);
	}
}




int pRigidBody::onJointBreak(pBrokenJointEntry *entry)
{


	CKBehavior * beh  = (CKBehavior*)GetPMan()->GetContext()->GetObject(getJointBreakScript());
	if (!beh)
		return -1;

	if (!entry)
		return -1;

	CK3dEntity *bodyAEnt = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(entry->mAEnt));
	CK3dEntity *bodyBEnt = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(entry->mBEnt));

	if (bodyAEnt)
	{
		SetInputParameterValue<CK_ID>(beh,bbJB_SrcObject,entry->mAEnt);
	}else
		SetInputParameterValue<CK_ID>(beh,bbJB_SrcObject,0);

	if(bodyBEnt)
		SetInputParameterValue<CK_ID>(beh,bbJB_OtherObject,entry->mBEnt);
	else
		SetInputParameterValue<CK_ID>(beh,bbJB_OtherObject,0);

	SetInputParameterValue<float>(beh,bbJB_Force,entry->impulse);
	
	float 	elapsedTime = GetPMan()->GetContext()->GetTimeManager()->GetLastDeltaTimeFree();
	beh->Execute(elapsedTime);

/*	XString log = "joint break at : " ;
	log+= elapsedTime;


	if (bodyAEnt)
	{
		log+=bodyAEnt->GetName();
	}

	if (bodyBEnt)
	{
		log= log + "  : with : " + bodyBEnt->GetName();
	}

	xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,log.Str());
*/
	//xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"joint break");
	GetPMan()->getJointFeedbackList().RemoveAt( GetPMan()->getJointFeedbackList().GetPosition(entry) );
	return 1;
}

float pRigidBody::getContactReportThreshold()
{
	if (getActor())
	{
		return getActor()->getContactReportThreshold();
	}
	return -1.0f;
}



void pRigidBody::setContactReportThreshold(float threshold)
{
	if (getActor())
	{
		getActor()->setContactReportThreshold(threshold);
	}
}

void pRigidBody::setContactReportFlags(pContactPairFlags flags)
{
	if (getActor())
	{
		getActor()->setContactReportFlags((NxU32)flags);
	}
}
int pRigidBody::getContactReportFlags()
{
	if (getActor())
	{
		return getActor()->getContactReportFlags();
	}
	return -1;

}



void pRigidBody::processScriptCallbacks()
{
	//----------------------------------------------------------------
	//
	// Collision Callbacks
	//

	/*if ( !(getCallMask() & CB_OnContactNotify) )
		return;
*/
}