#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "vtAttributeHelper.h"


using namespace xUtils;
using namespace vtAgeia;
using namespace vtTools::ParameterTools;
using namespace vtTools::AttributeTools;


int registerJD6(CK3dEntity *target,int attributeType,bool set,bool isPostJob)
{
	return 0;
}
int registerJD6Drive(CK3dEntity *target,int attributeType,bool set,bool isPostJob)
{
	return 0;
}
int registerJLimitPlane(CK3dEntity *target,int attributeType,bool set,bool isPostJob)
{
	
	
	//----------------------------------------------------------------
	//
	// get and check objects
	//
	if (!target)
		return -1;

	CKParameterOut *attParameter = target->GetAttributeParameter(attributeType);
	if (!attParameter)
		return -1;

	CKStructHelper sHelper(target->GetAttributeParameter(attributeType));
	if (sHelper.GetMemberCount() ==0 )//happens when dev is being opened and loads a cmo with physic objects.
		return -1;


	pRigidBody *body = GetPMan()->getBody(target);
	if (!body)
		return -1;

	CK_ID bodyBId = GetValueFromParameterStruct<CK_ID>(attParameter,PS_JLP_BODY_B_REF);
	CK3dEntity *bodyBEnt = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(bodyBId));

	if (set)
	{
		//----------------------------------------------------------------
		//
		// post pone i ck-start-up or loading
		//
		if (isPostJob  && GetPMan()->GetContext()->IsPlaying())
		{
			pAttributePostObject postAttObject(target->GetID(),registerJLimitPlane,attributeType);
			GetPMan()->getAttributePostObjects().PushBack(postAttObject);
			return true;
		}

		int jointType = GetValueFromParameterStruct<int>(attParameter,PS_JLP_JOINT_TYPE);

		//try to get the joint 
		pJoint *joint = GetPMan()->getJoint(target,bodyBEnt,(JType)jointType);
		if (!joint)
			return -1;

		//----------------------------------------------------------------
		//
		// we are on ck-play or SetAttribute-BB	: Retrieve all parameters from the attribute : 
		//

		VxVector limitPoint = GetValueFromParameterStruct<VxVector>(attParameter,PS_JLP_LIMIT_POINT);

		CK_ID limitPointRefID = GetValueFromParameterStruct<CK_ID>(attParameter,PS_JLP_LIMIT_POINT_REF);
		CK3dEntity *limiPointRef = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(limitPointRefID));
		if (limiPointRef)		
			limiPointRef->Transform(&limitPoint,&limitPoint);


		int isOnBodyB = GetValueFromParameterStruct<int>(attParameter,PS_JLP_IS_ON_BODY_B);
		float length  = limitPoint.SquareMagnitude();
		if (XAbs(limitPoint.SquareMagnitude()) >=0.01f)
			joint->setLimitPoint(limitPoint,isOnBodyB);

		
		//	-	Limit Point Normal 
		VxVector limitPointNormal = GetValueFromParameterStruct<VxVector>(attParameter,PS_JLP_NORMAL);

		CK_ID limitPointNormalRefID = GetValueFromParameterStruct<CK_ID>(attParameter,PS_JLP_NORMAL_REF);
		CK3dEntity *limitPointNormalRef = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(limitPointNormalRefID));
		
		if (limitPointNormalRef)
		{
			VxVector dir,up,right;
			limitPointNormalRef->GetOrientation(&dir,&up,&right);
			limitPointNormalRef->TransformVector(&limitPointNormal,&up);
		}

		//	-	Point in Plane 
		VxVector PointInPlane = GetValueFromParameterStruct<VxVector>(attParameter,PS_JLP_PT_IN_PLANE);

		CK_ID PointInPlaneRefID = GetValueFromParameterStruct<CK_ID>(attParameter,PS_JLP_PT_IN_PLANE_REF);
		CK3dEntity *PointInPlaneRef = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(PointInPlaneRefID));
		if (PointInPlaneRef)		
			PointInPlaneRef->Transform(&PointInPlane,&PointInPlane);

		//	-	Restitution
		float res = GetValueFromParameterStruct<float>(attParameter,PS_JLP_RESTITUTION);

		//	-	Execute 

		int result  = joint->addLimitPlane(limitPointNormal,PointInPlane,res);
		return result;
	}

}


int registerJPointOnLine(CK3dEntity *target,int attributeType,bool set,bool isPostJob)
{
	using namespace vtTools::ParameterTools;
	CKParameterOut *pointInPlaneParameter = target->GetAttributeParameter(attributeType);

	CKStructHelper sHelper(target->GetAttributeParameter(attributeType));
	if (sHelper.GetMemberCount() ==0 )//happens when dev is being opened and loads a cmo with physic objects.
		return -1;

	if (pointInPlaneParameter )
	{

		pRigidBody *body = GetPMan()->getBody(target);
		if (!body)return -1;

		CK_ID bodyBId = GetValueFromParameterStruct<CK_ID>(pointInPlaneParameter,PS_JPOL_BODY_B);
		CK3dEntity *bodyBEnt = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(bodyBId));
		pRigidBody *bodyB = GetPMan()->getBody(bodyBEnt);
		if (bodyB && bodyB->getWorld() != body->getWorld() )
			return -1;
		if (set)
		{

			if (isPostJob  && GetPMan()->GetContext()->IsPlaying())
			{
				pAttributePostObject postAttObject(target->GetID(),registerJPointOnLine,attributeType);
				GetPMan()->getAttributePostObjects().PushBack(postAttObject);

				return true;

			}

			if (bodyB && body->isConnected(bodyBEnt) )
				return -1;

			VxVector anchor0 = GetValueFromParameterStruct<VxVector>(pointInPlaneParameter,PS_JPOL_ANCHOR);
			CK_ID anchor0RefID = GetValueFromParameterStruct<CK_ID>(pointInPlaneParameter,PS_JPOL_ANCHOR_REF);



			//////////////////////////////////////////////////////////////////////////
			//anchor :
			CK3dEntity *a0Ref = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(anchor0RefID));
			if (a0Ref)		a0Ref->Transform(&anchor0,&anchor0);
			int collision = GetValueFromParameterStruct<int>(pointInPlaneParameter,PS_JPOL_COLLISION);


			//////////////////////////////////////////////////////////////////////////
			//axis
			VxVector axis  = GetValueFromParameterStruct<VxVector>(pointInPlaneParameter,PS_JPOL_AXIS);
			CK_ID axisRefID = GetValueFromParameterStruct<CK_ID>(pointInPlaneParameter,PS_JPOL_AXIS_REF);
			CK3dEntity *axisRef = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(axisRefID));
			if (axisRef)
			{
				VxVector dir,up,right;
				axisRef->GetOrientation(&dir,&up,&right);
				axisRef->TransformVector(&axis,&up);
			}

			pJointPointOnLine*joint   = static_cast<pJointPointOnLine*>(pFactory::Instance()->createPointOnLineJoint(target,bodyBEnt,anchor0,axis));
			if (joint)
			{
				joint->enableCollision(collision);

				float maxForce  = GetValueFromParameterStruct<float>(pointInPlaneParameter,PS_JPOL_MAX_FORCE);
				float maxTorque  = GetValueFromParameterStruct<float>(pointInPlaneParameter,PS_JPOL_MAX_TORQUE);
				joint->setBreakForces(maxForce,maxTorque);
				return 1;
			}

			return 0;
		}else
		{


			pJoint *joint = body->isConnected(bodyBEnt);
			if ( joint)
			{
				body->getWorld()->deleteJoint(joint);
			}
			pJoint *joint2 = body->getWorld()->getJoint(target,bodyBEnt,(JType)JT_PointOnLine);
			if (joint2)
			{
				body->getWorld()->deleteJoint(joint);
			}

			return 1;
		}
	}

	return 0;

}

int registerJPointInPlane(CK3dEntity *target,int attributeType,bool set,bool isPostJob)
{
	using namespace vtTools::ParameterTools;
	CKParameterOut *pointInPlaneParameter = target->GetAttributeParameter(attributeType);

	CKStructHelper sHelper(target->GetAttributeParameter(attributeType));
	if (sHelper.GetMemberCount() ==0 )//happens when dev is being opened and loads a cmo with physic objects.
		return -1;

	if (pointInPlaneParameter )
	{

		pRigidBody *body = GetPMan()->getBody(target);
		if (!body)return -1;

		CK_ID bodyBId = GetValueFromParameterStruct<CK_ID>(pointInPlaneParameter,PS_JPIP_BODY_B);
		CK3dEntity *bodyBEnt = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(bodyBId));
		pRigidBody *bodyB = GetPMan()->getBody(bodyBEnt);
		if (bodyB && bodyB->getWorld() != body->getWorld() )
			return -1;
		if (set)
		{

			if (isPostJob  && GetPMan()->GetContext()->IsPlaying())
			{
				pAttributePostObject postAttObject(target->GetID(),registerJPointInPlane,attributeType);
				GetPMan()->getAttributePostObjects().PushBack(postAttObject);
				
				return true;

			}

			if (bodyB && body->isConnected(bodyBEnt) )
				return -1;

			VxVector anchor0 = GetValueFromParameterStruct<VxVector>(pointInPlaneParameter,PS_JPIP_ANCHOR);
			CK_ID anchor0RefID = GetValueFromParameterStruct<CK_ID>(pointInPlaneParameter,PS_JPIP_ANCHOR_REF);



			//////////////////////////////////////////////////////////////////////////
			//anchor :
			CK3dEntity *a0Ref = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(anchor0RefID));
			if (a0Ref)		a0Ref->Transform(&anchor0,&anchor0);
			int collision = GetValueFromParameterStruct<int>(pointInPlaneParameter,PS_JPIP_COLLISION);


			//////////////////////////////////////////////////////////////////////////
			//axis
			VxVector axis  = GetValueFromParameterStruct<VxVector>(pointInPlaneParameter,PS_JPIP_AXIS);
			CK_ID axisRefID = GetValueFromParameterStruct<CK_ID>(pointInPlaneParameter,PS_JPIP_AXIS_REF);
			CK3dEntity *axisRef = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(axisRefID));
			if (axisRef)
			{
				VxVector dir,up,right;
				axisRef->GetOrientation(&dir,&up,&right);
				axisRef->TransformVector(&axis,&up);
			}
			//pJointCylindrical*pFactory::createCylindricalJoint(CK3dEntity*a,CK3dEntity*b,VxVector anchor,VxVector axis)
			pJointPointInPlane*joint   = static_cast<pJointPointInPlane*>(pFactory::Instance()->createPointInPlaneJoint(target,bodyBEnt,anchor0,axis));
			if (joint)
			{
				joint->enableCollision(collision);

				float maxForce  = GetValueFromParameterStruct<float>(pointInPlaneParameter,PS_JPIP_MAX_FORCE);
				float maxTorque  = GetValueFromParameterStruct<float>(pointInPlaneParameter,PS_JPIP_MAX_TORQUE);
				joint->setBreakForces(maxForce,maxTorque);
				return 1;
			}

			return 0;
		}else
		{


			pJoint *joint = body->isConnected(bodyBEnt);
			if ( joint)
			{
				body->getWorld()->deleteJoint(joint);
			}
			pJoint *joint2 = body->getWorld()->getJoint(target,bodyBEnt,(JType)JT_PointInPlane);
			if (joint2)
			{
				body->getWorld()->deleteJoint(joint);
			}

			return 1;
		}
	}

	return 0;

}


int registerJRevolute(CK3dEntity *target,int attributeType,bool set,bool isPostJob)
{
	
	int s  = GetPMan()->getAttributePostObjects().Size();
	using namespace vtTools::ParameterTools;
	CKParameterOut *distanceParameter = target->GetAttributeParameter(attributeType);

	CKStructHelper sHelper(target->GetAttributeParameter(attributeType));
	if (sHelper.GetMemberCount() ==0 )//happens when dev is being opened and loads a cmo with physic objects.
		return -1;

	if (distanceParameter )
	{

		pRigidBody *body = GetPMan()->getBody(target);
		if (!body)return -1;

		CK_ID bodyBId = GetValueFromParameterStruct<CK_ID>(distanceParameter,PS_JREVOLUTE_BODY_B);
		CK3dEntity *bodyBEnt = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(bodyBId));
		pRigidBody *bodyB = GetPMan()->getBody(bodyBEnt);
		if (bodyB && bodyB->getWorld() != body->getWorld() )
			return -1;
		if (set)
		{

			if (isPostJob  && GetPMan()->GetContext()->IsPlaying())
			{
				pAttributePostObject postAttObject(target->GetID(),registerJRevolute,attributeType);
				GetPMan()->getAttributePostObjects().PushBack(postAttObject);
				return true;

			}

			if (bodyB && body->isConnected(bodyBEnt) )
				return -1;

			VxVector anchor0 = GetValueFromParameterStruct<VxVector>(distanceParameter,PS_JREVOLUTE_ANCHOR);
			CK_ID anchor0RefID = GetValueFromParameterStruct<CK_ID>(distanceParameter,PS_JREVOLUTE_ANCHOR_REF);
			CK3dEntity *a0Ref = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(anchor0RefID));
			if (a0Ref)		a0Ref->Transform(&anchor0,&anchor0);
			//////////////////////////////////////////////////////////////////////////
			//axis
			VxVector axis  = GetValueFromParameterStruct<VxVector>(distanceParameter,PS_JREVOLUTE_AXIS);
			CK_ID axisRefID = GetValueFromParameterStruct<CK_ID>(distanceParameter,PS_JREVOLUTE_AXIS_REF);
			CK3dEntity *axisRef = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(axisRefID));
			if (axisRef)
			{
				VxVector dir,up,right;
				axisRef->GetOrientation(&dir,&up,&right);
				axisRef->TransformVector(&axis,&up);
			}

			int collision = GetValueFromParameterStruct<int>(distanceParameter,PS_JREVOLUTE_COLLISION);

			int projectionMode = GetValueFromParameterStruct<int>(distanceParameter,PS_JREVOLUTE_PROJ_MODE);
			float projectionDistance= GetValueFromParameterStruct<float>(distanceParameter,PS_JREVOLUTE_PROJ_DISTANCE);
			float projectionAngle = GetValueFromParameterStruct<float>(distanceParameter,PS_JREVOLUTE_PROJ_ANGLE);

			

			pJointLimit limitHigh = pFactory::Instance()->createLimitFromParameter(vtTools::ParameterTools::GetParameterFromStruct(distanceParameter,PS_JREVOLUTE_LIMIT_HIGH));
			pJointLimit limitLow= pFactory::Instance()->createLimitFromParameter(vtTools::ParameterTools::GetParameterFromStruct(distanceParameter,PS_JREVOLUTE_LIMIT_LOW));
			pSpring spring = pFactory::Instance()->createSpringFromParameter(vtTools::ParameterTools::GetParameterFromStruct(distanceParameter,PS_JREVOLUTE_SPRING));
			pMotor motor = pFactory::Instance()->createMotorFromParameter(vtTools::ParameterTools::GetParameterFromStruct(distanceParameter,PS_JREVOLUTE_MOTOR));

			pJointRevolute*joint   = static_cast<pJointRevolute*>(pFactory::Instance()->createRevoluteJoint(target,bodyBEnt,anchor0,axis));
			if (joint)
			{
				
				joint->enableCollision(collision);
				joint->setLowLimit(limitLow);
				joint->setHighLimit(limitHigh);
				joint->setSpring(spring);
				
				if (projectionMode!=0)
				{
					joint->setProjectionMode((ProjectionMode)projectionMode);
					joint->setProjectionDistance(projectionDistance);
					joint->setProjectionAngle(projectionAngle);
				}

				float maxForce  = GetValueFromParameterStruct<float>(distanceParameter,PS_JREVOLUTE_MAX_FORCE);
				float maxTorque  = GetValueFromParameterStruct<float>(distanceParameter,PS_JREVOLUTE_MAX_TORQUE);
				if (maxTorque !=0.0f || maxForce!=0.0f)
				{
					joint->setBreakForces(maxForce,maxTorque);
				}
				
				return true;
			}

			return 0;
		}else
		{


			pJoint *joint = body->isConnected(bodyBEnt);
			if ( joint)
			{
				body->getWorld()->deleteJoint(joint);
			}
			pJoint *joint2 = body->getWorld()->getJoint(target,bodyBEnt,(JType)JT_Revolute);
			if (joint2)
			{
				body->getWorld()->deleteJoint(joint);
			}
		}
	}
	return 0;

}


int registerJCylindrical(CK3dEntity *target,int attributeType,bool set,bool isPostJob)
{


	using namespace vtTools::ParameterTools;
	CKParameterOut *distanceParameter = target->GetAttributeParameter(attributeType);


	CKStructHelper sHelper(target->GetAttributeParameter(attributeType));
	if (sHelper.GetMemberCount() ==0 )//happens when dev is being opened and loads a cmo with physic objects.
		return -1;

	if (distanceParameter )
	{

		pRigidBody *body = GetPMan()->getBody(target);
		if (!body)return -1;

		CK_ID bodyBId = GetValueFromParameterStruct<CK_ID>(distanceParameter,PS_JCYLINDRICAL_BODY_B);
		CK3dEntity *bodyBEnt = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(bodyBId));
		pRigidBody *bodyB = GetPMan()->getBody(bodyBEnt);
		if (bodyB && bodyB->getWorld() != body->getWorld() )
			return -1;
		if (set)
		{

			if (isPostJob  && GetPMan()->GetContext()->IsPlaying())
			{
				pAttributePostObject postAttObject(target->GetID(),registerJCylindrical,attributeType);
				GetPMan()->getAttributePostObjects().PushBack(postAttObject);
				return true;

			}

			if (bodyB && body->isConnected(bodyBEnt) )
				return -1;

			VxVector anchor0 = GetValueFromParameterStruct<VxVector>(distanceParameter,PS_JCYLINDRICAL_ANCHOR);
			CK_ID anchor0RefID = GetValueFromParameterStruct<CK_ID>(distanceParameter,PS_JCYLINDRICAL_ANCHOR_REF);



			//////////////////////////////////////////////////////////////////////////
			//anchor :
			CK3dEntity *a0Ref = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(anchor0RefID));
			if (a0Ref)		a0Ref->Transform(&anchor0,&anchor0);
			int collision = GetValueFromParameterStruct<int>(distanceParameter,PS_JCYLINDRICAL_COLLISION);


			//////////////////////////////////////////////////////////////////////////
			//axis
			VxVector axis  = GetValueFromParameterStruct<VxVector>(distanceParameter,PS_JCYLINDRICAL_AXIS);
			CK_ID axisRefID = GetValueFromParameterStruct<CK_ID>(distanceParameter,PS_JCYLINDRICAL_AXIS_REF);
			CK3dEntity *axisRef = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(axisRefID));
			if (axisRef)
			{
				VxVector dir,up,right;
				axisRef->GetOrientation(&dir,&up,&right);
				axisRef->TransformVector(&axis,&up);
			}
			//pJointCylindrical*pFactory::createCylindricalJoint(CK3dEntity*a,CK3dEntity*b,VxVector anchor,VxVector axis)
			pJointCylindrical*joint   = static_cast<pJointCylindrical*>(pFactory::Instance()->createCylindricalJoint(target,bodyBEnt,anchor0,axis));
			if (joint)
			{
				joint->enableCollision(collision);

				float maxForce  = GetValueFromParameterStruct<float>(distanceParameter,PS_JCYLINDRICAL_MAX_FORCE);
				float maxTorque  = GetValueFromParameterStruct<float>(distanceParameter,PS_JCYLINDRICAL_MAX_TORQUE);
				//joint->setBreakForces(maxForce,maxTorque);
				return true;
			}

			return 0;
		}else
		{


			pJoint *joint = body->isConnected(bodyBEnt);
			if ( joint)
			{
				body->getWorld()->deleteJoint(joint);
			}
			pJoint *joint2 = body->getWorld()->getJoint(target,bodyBEnt,(JType)JT_Cylindrical);
			if (joint2)
			{
				body->getWorld()->deleteJoint(joint);
			}
		}
	}

	return 0;

}


int registerJPrismatic(CK3dEntity *target,int attributeType,bool set,bool isPostJob)
{
	using namespace vtTools::ParameterTools;
	CKParameterOut *distanceParameter = target->GetAttributeParameter(attributeType);

	CKStructHelper sHelper(target->GetAttributeParameter(attributeType));
	if (sHelper.GetMemberCount() ==0 )//happens when dev is being opened and loads a cmo with physic objects.
		return -1;

	if (distanceParameter )
	{

		pRigidBody *body = GetPMan()->getBody(target);
		if (!body)return -1;

		CK_ID bodyBId = GetValueFromParameterStruct<CK_ID>(distanceParameter,PS_JPRISMATIC_BODY_B);
		CK3dEntity *bodyBEnt = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(bodyBId));
		pRigidBody *bodyB = GetPMan()->getBody(bodyBEnt);
		if (bodyB && bodyB->getWorld() != body->getWorld() )
			return -1;
		if (set)
		{

			if (isPostJob  && GetPMan()->GetContext()->IsPlaying())
			{
				pAttributePostObject postAttObject(target->GetID(),registerJPrismatic,attributeType);
				GetPMan()->getAttributePostObjects().PushBack(postAttObject);
				return true;

			}

			if (bodyB && body->isConnected(bodyBEnt) )
				return -1;

			VxVector anchor0 = GetValueFromParameterStruct<VxVector>(distanceParameter,PS_JPRISMATIC_ANCHOR);
			CK_ID anchor0RefID = GetValueFromParameterStruct<CK_ID>(distanceParameter,PS_JPRISMATIC_ANCHOR_REF);

			

			//////////////////////////////////////////////////////////////////////////
			//anchor :
			CK3dEntity *a0Ref = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(anchor0RefID));
			if (a0Ref)		a0Ref->Transform(&anchor0,&anchor0);
			int collision = GetValueFromParameterStruct<int>(distanceParameter,PS_JPRISMATIC_COLLISION);
			
			
			//////////////////////////////////////////////////////////////////////////
			//axis
			VxVector axis  = GetValueFromParameterStruct<VxVector>(distanceParameter,PS_JPRISMATIC_AXIS);
			CK_ID axisRefID = GetValueFromParameterStruct<CK_ID>(distanceParameter,PS_JPRISMATIC_AXIS_REF);
			CK3dEntity *axisRef = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(axisRefID));
			if (axisRef)
			{
				VxVector dir,up,right;
				axisRef->GetOrientation(&dir,&up,&right);
				axisRef->TransformVector(&axis,&up);
			}
			//pJointCylindrical*pFactory::createCylindricalJoint(CK3dEntity*a,CK3dEntity*b,VxVector anchor,VxVector axis)
			pJointCylindrical*joint   = static_cast<pJointCylindrical*>(pFactory::Instance()->createCylindricalJoint(target,bodyBEnt,anchor0,axis));
			if (joint)
			{
				joint->enableCollision(collision);

				float maxForce  = GetValueFromParameterStruct<float>(distanceParameter,PS_JCYLINDRICAL_MAX_FORCE);
				float maxTorque  = GetValueFromParameterStruct<float>(distanceParameter,PS_JCYLINDRICAL_MAX_TORQUE);
				joint->setBreakForces(maxForce,maxTorque);

				return true;
			}

			return 0;
		}else
		{


			pJoint *joint = body->isConnected(bodyBEnt);
			if ( joint)
			{
				body->getWorld()->deleteJoint(joint);
			}
			pJoint *joint2 = body->getWorld()->getJoint(target,bodyBEnt,(JType)JT_Prismatic);
			if (joint2)
			{
				body->getWorld()->deleteJoint(joint);
			}
		}
	}
	return 0;
}

int registerJBall(CK3dEntity *target,int attributeType,bool set,bool isPostJob)
{
	using namespace vtTools::ParameterTools;
	CKParameterOut *distanceParameter = target->GetAttributeParameter(attributeType);


	CKStructHelper sHelper(target->GetAttributeParameter(attributeType));
	if (sHelper.GetMemberCount() ==0 )//happens when dev is being opened and loads a cmo with physic objects.
		return -1;

	if (distanceParameter )
	{

		pRigidBody *body = GetPMan()->getBody(target);
		if (!body)return -1;

		CK_ID bodyBId = GetValueFromParameterStruct<CK_ID>(distanceParameter,PS_JBALL_BODY_B);
		CK3dEntity *bodyBEnt = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(bodyBId));
		pRigidBody *bodyB = GetPMan()->getBody(bodyBEnt);
		if (bodyB && bodyB->getWorld() != body->getWorld() )
			return -1;
		if (set)
		{

			if (isPostJob  && GetPMan()->GetContext()->IsPlaying())
			{
				pAttributePostObject postAttObject(target->GetID(),registerJBall,attributeType);
				GetPMan()->getAttributePostObjects().PushBack(postAttObject);
				return true;

			}

			if (bodyB && body->isConnected(bodyBEnt) )
				return -1;

			VxVector anchor0 = GetValueFromParameterStruct<VxVector>(distanceParameter,PS_JBALL_ANCHOR);
			CK_ID anchor0RefID = GetValueFromParameterStruct<CK_ID>(distanceParameter,PS_JBALL_ANCHOR_REF);
			CK3dEntity *a0Ref = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(anchor0RefID));
			if (a0Ref)		a0Ref->Transform(&anchor0,&anchor0);

			VxVector swingAxis = GetValueFromParameterStruct<VxVector>(distanceParameter,PS_JBALL_LIMIT_SWING_AXIS);

			int collision = GetValueFromParameterStruct<int>(distanceParameter,PS_JBALL_COLLISION);
			
			int projectionMode = GetValueFromParameterStruct<int>(distanceParameter,PS_JBALL_PROJ_MODE);
			float minDist = GetValueFromParameterStruct<float>(distanceParameter,PS_JBALL_PROJ_DISTANCE);

			pJointLimit swingLimit = pFactory::Instance()->createLimitFromParameter(vtTools::ParameterTools::GetParameterFromStruct(distanceParameter,PS_JBALL_SWING_LIMIT));
			pJointLimit twistHighLimit = pFactory::Instance()->createLimitFromParameter(vtTools::ParameterTools::GetParameterFromStruct(distanceParameter,PS_JBALL_TWIST_HIGH));
			pJointLimit twistHighLow = pFactory::Instance()->createLimitFromParameter(vtTools::ParameterTools::GetParameterFromStruct(distanceParameter,PS_JBALL_TWIST_LOW));

			pSpring swingSpring = pFactory::Instance()->createSpringFromParameter(vtTools::ParameterTools::GetParameterFromStruct(distanceParameter,PS_JBALL_SWING_SPRING));
			pSpring twistSpring = pFactory::Instance()->createSpringFromParameter(vtTools::ParameterTools::GetParameterFromStruct(distanceParameter,PS_JBALL_TWIST_SPRING));
			pSpring jointSpring = pFactory::Instance()->createSpringFromParameter(vtTools::ParameterTools::GetParameterFromStruct(distanceParameter,PS_JBALL_JOINT_SPRING));

			pJointBall *joint   = static_cast<pJointBall*>(pFactory::Instance()->createBallJoint(target,bodyBEnt,anchor0,swingAxis));
			if (joint)
			{
				joint->setProjectionMode((ProjectionMode)projectionMode);
				joint->setProjectionDistance(minDist);
				joint->enableCollision(collision);


				float maxForce  = GetValueFromParameterStruct<float>(distanceParameter,PS_JBALL_MAX_FORCE);
				float maxTorque  = GetValueFromParameterStruct<float>(distanceParameter,PS_JBALL_MAX_TORQUE);
				joint->setBreakForces(maxForce,maxTorque);



				if (swingLimit.value !=-1.0f)
				{
					joint->setSwingLimit(swingLimit);
					joint->enableFlag(NX_SJF_SWING_LIMIT_ENABLED,true);
				}

				joint->setSwingLimit(swingLimit);
				
				
				if (twistHighLimit.value != 0.0f || twistHighLow.value !=0.0f )
				{
				
					joint->setTwistHighLimit(twistHighLimit);
					joint->setTwistLowLimit(twistHighLow);
					joint->enableFlag(NX_SJF_TWIST_LIMIT_ENABLED,true);
				}

				if (swingSpring.spring !=0.0f)
				{
					joint->setSwingSpring(swingSpring);
					joint->enableFlag(NX_SJF_SWING_SPRING_ENABLED,true);
				}


				if (twistSpring.spring =0.0f)
				{
					joint->setTwistSpring(twistSpring);
					joint->enableFlag(NX_SJF_TWIST_SPRING_ENABLED,true);
				}

				if (jointSpring.spring !=0.0f)
				{
					joint->setJointSpring(jointSpring);
					joint->enableFlag(NX_SJF_JOINT_SPRING_ENABLED,true);
				}

				return true;
			}

			return 0;
		}else
		{


			pJoint *joint = body->isConnected(bodyBEnt);
			if ( joint)
			{
				body->getWorld()->deleteJoint(joint);
			}
			pJoint *joint2 = body->getWorld()->getJoint(target,bodyBEnt,(JType)JT_Spherical);
			if (joint2)
			{
				body->getWorld()->deleteJoint(joint);
			}
		}
	}
	return 0;
}

int registerJFixed(CK3dEntity *target,int attributeType,bool set,bool isPostJob)
{
	using namespace vtTools::ParameterTools;

	CKStructHelper sHelper(target->GetAttributeParameter(attributeType));
	if (sHelper.GetMemberCount() ==0 )//happens when dev is being opened and loads a cmo with physic objects.
		return -1;


	CKParameterOut *distanceParameter = target->GetAttributeParameter(attributeType);
	if (distanceParameter )
	{

		pRigidBody *body = GetPMan()->getBody(target);
		if (!body)return -1;

		CK_ID bodyBId = GetValueFromParameterStruct<CK_ID>(distanceParameter,PS_JFIXED_BODY_B);
		CK3dEntity *bodyBEnt = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(bodyBId));
		pRigidBody *bodyB = GetPMan()->getBody(bodyBEnt);
		if (bodyB && bodyB->getWorld() != body->getWorld() )
			return -1;
		if (set)
		{

			if ( isPostJob  && GetPMan()->GetContext()->IsPlaying() )
			{
				pAttributePostObject postAttObject(target->GetID(),registerJFixed,attributeType);
				GetPMan()->getAttributePostObjects().PushBack(postAttObject);
				return true;

			}

			if (bodyB && body->isConnected(bodyBEnt) )
				return -1;


			pJointFixed *joint = static_cast<pJointFixed*>(pFactory::Instance()->createFixedJoint(target,bodyBEnt));
			if (joint)
			{

				float maxForce  = GetValueFromParameterStruct<float>(distanceParameter,PS_JFIXED_MAX_FORCE);
				float maxTorque  = GetValueFromParameterStruct<float>(distanceParameter,PS_JFIXED_MAX_TORQUE);
				joint->setBreakForces(maxForce,maxTorque);

				return true;
			}
			return 0;
		}else
		{


			pJoint *joint = body->isConnected(bodyBEnt);
			if ( joint)
			{
				body->getWorld()->deleteJoint(joint);
			}
			pJoint *joint2 = body->getWorld()->getJoint(target,bodyBEnt,(JType)JT_Fixed);
			if (joint2)
			{
				body->getWorld()->deleteJoint(joint);
			}
		}
	}

	return 0;


}



int registerJDistance(CK3dEntity *target,int attributeType,bool set,bool isPostJob)
{
	using namespace vtTools::ParameterTools;


	CKStructHelper sHelper(target->GetAttributeParameter(attributeType));
	if (sHelper.GetMemberCount() ==0 )//happens when dev is being opened and loads a cmo with physic objects.
		return -1;

	CKParameterOut *distanceParameter = target->GetAttributeParameter(attributeType);
	if (distanceParameter )
	{

		pRigidBody *body = GetPMan()->getBody(target);
		if (!body)return -1;

		CK_ID bodyBId = GetValueFromParameterStruct<CK_ID>(distanceParameter,PS_JDISTANCE_BODY_B);
		CK3dEntity *bodyBEnt = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(bodyBId));
		pRigidBody *bodyB = GetPMan()->getBody(bodyBEnt);
		if (bodyB && bodyB->getWorld() != body->getWorld() )
			return -1;
		
		if (set)
		{
			
			if (isPostJob  && GetPMan()->GetContext()->IsPlaying())
			{
				pAttributePostObject postAttObject(target->GetID(),registerJDistance,attributeType);
				GetPMan()->getAttributePostObjects().PushBack(postAttObject);
				
				int s = GetPMan()->getAttributePostObjects().Size();

				return true;

			}

			if (bodyB && body->isConnected(bodyBEnt) )
				return -1;
			
			VxVector anchor0 = GetValueFromParameterStruct<VxVector>(distanceParameter,PS_JDISTANCE_LOCAL_ANCHOR_A_POS);
			CK_ID anchor0RefID = GetValueFromParameterStruct<CK_ID>(distanceParameter,PS_JDISTANCE_LOCAL_ANCHOR_A_REF);
			CK3dEntity *a0Ref = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(anchor0RefID));
			if (a0Ref)		a0Ref->TransformVector(&anchor0,&anchor0,a0Ref);

			VxVector anchor1 = GetValueFromParameterStruct<VxVector>(distanceParameter,PS_JDISTANCE_LOCAL_ANCHOR_B_POS);
			CK_ID anchor1RefID = GetValueFromParameterStruct<CK_ID>(distanceParameter,PS_JDISTANCE_LOCAL_ANCHOR_B_REF);
			CK3dEntity *a1Ref = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(anchor1RefID));
			if (a1Ref)		a1Ref->TransformVector(&anchor1,&anchor1,a1Ref);

			float maxDist = GetValueFromParameterStruct<float>(distanceParameter,PS_JDISTANCE_MAX_DISTANCE);
			float minDist = GetValueFromParameterStruct<float>(distanceParameter,PS_JDISTANCE_MIN_DISTANCE);

			pSpring spring = pFactory::Instance()->createSpringFromParameter(  GetParameterFromStruct(distanceParameter,PS_JDISTANCE_SPRING) );

			int collision = GetValueFromParameterStruct<int>(distanceParameter,PS_JDISTANCE_COLL);


			pJointDistance *joint = static_cast<pJointDistance*>(pFactory::Instance()->createDistanceJoint(target,bodyBEnt,anchor0,anchor1,minDist,maxDist,spring));
			if (joint)
			{

				float maxForce  = GetValueFromParameterStruct<float>(distanceParameter,PS_JDISTANCE_MAX_FORCE);
				float maxTorque  = GetValueFromParameterStruct<float>(distanceParameter,PS_JDISTANCE_MAX_TORQUE);
				joint->setBreakForces(maxForce,maxTorque);
				joint->enableCollision(collision);


				#ifdef _DEBUG 

					XString _errorStr;

					XString attName = GetPMan()->GetContext()->GetAttributeManager()->GetAttributeNameByType(attributeType);
					
					_errorStr << "Distance joint created on " << target->GetName() << " for attribute type : " << attName.Str();

					xLogger::xLog(XL_START,ELOGINFO,E_LI_MANAGER,_errorStr.Str());

				#endif

				return true;
			}
			return 0;
		}else
		{


			pJoint *joint = body->isConnected(bodyBEnt);
			if ( joint)
			{
				body->getWorld()->deleteJoint(joint);
			}
			pJoint *joint2 = body->getWorld()->getJoint(target,bodyBEnt,(JType)JT_Distance);
			if (joint2)
			{
				body->getWorld()->deleteJoint(joint);
			}
		}
	}
	return 0;

}


void PObjectAttributeCallbackFunc(int AttribType,CKBOOL Set,CKBeObject *obj,void *arg)
{

	CK3dEntity *ent = static_cast<CK3dEntity*>(obj);
	if (!ent)return;

	ObjectRegisterFunction myFn = static_cast<ObjectRegisterFunction>(arg);
	if (ent , myFn)
	{
		bool isPlaying = GetPMan()->GetContext()->IsPlaying();
		int success = (*myFn)(ent,AttribType,Set,true);

		if (!success)
		{
			CKAttributeManager* attman = GetPMan()->GetContext()->GetAttributeManager();
			XString error;
			error << "Registration Function failed for attribute" << attman->GetAttributeNameByType(AttribType) << "  for Object : " << ent->GetName() ;
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,error.Str());
		}else{

			CKAttributeManager* attman = GetPMan()->GetContext()->GetAttributeManager();
			XString error;
			error << "Registration Function succeeded for attribute" << attman->GetAttributeNameByType(AttribType) << "  for Object : " << ent->GetName() ;
			xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,error.Str());
		}
	}
}
