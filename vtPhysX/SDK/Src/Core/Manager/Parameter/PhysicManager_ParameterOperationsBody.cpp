#include <StdAfx.h>
#include "vtPhysXAll.h"


#define PARAM_OP_TYPE_BGET_VELOCITY CKGUID(0x3b2a778c,0x293b206d)
#define PARAM_OP_TYPE_BGET_AVELOCITY CKGUID(0x67ee7c4e,0x1e3b4d15)
#define PARAM_OP_TYPE_BGET_TORQUE CKGUID(0x1ecd7ee4,0x4f0b7eda)
#define PARAM_OP_TYPE_BGET_FORCE CKGUID(0x7dd13e61,0x40af4f99)
#define PARAM_OP_TYPE_BGET_FRICTION CKGUID(0x482b3611,0x4b0a168c)
#define PARAM_OP_TYPE_BGET_HTYPE CKGUID(0x68eb059e,0x26bb745a)
#define PARAM_OP_TYPE_BGET_FIXED CKGUID(0x7bcd7379,0x5c950897)
#define PARAM_OP_TYPE_BGET_KINEMATIC CKGUID(0x6cf414e6,0xf0a35ec)

#define PARAM_OP_TYPE_BGET_GRAVITY CKGUID(0x63f81d10,0xd532a5a)
#define PARAM_OP_TYPE_BGET_COLLISION CKGUID(0x57f61ee3,0xef1252a)
#define PARAM_OP_TYPE_BGET_COLLISION_GROUP CKGUID(0xeea6d63,0x2d8a032d)
#define PARAM_OP_TYPE_BGET_SLEEPING CKGUID(0x7ca42afe,0x2665435)

#define PARAM_OP_TYPE_BIS_SUB_SHAPE_OF CKGUID(0x7ed952ce,0x5160083)


#define PARAM_OP_TYPE_BGET_FLAGS CKGUID(0x19263740,0x5dd248a2)
#define PARAM_OP_TYPE_BGET_MATERIAL CKGUID(0x1306375d,0x2bcc3cab)
#define PARAM_OP_TYPE_BGET_ISPOBJECT CKGUID(0xa0d7467,0x76692667)


#define PARAM_OP_TYPE_BGET_PVEL CKGUID(0x46bc47fc,0x1a4e6a83)
#define PARAM_OP_TYPE_BGET_LPVEL CKGUID(0x1c7e07fa,0x430a084b)
#define PARAM_OP_TYPE_BGET_MASS CKGUID(0x257d5234,0x362841d4)

#define PARAM_OP_TYPE_BJ_ISCONNECTED CKGUID(0x51db16ef,0xfb772d0)
#define PARAM_OP_TYPE_BJ_NBJOINTS CKGUID(0x29e20d6c,0x6ebc03d2)



#define PARAM_OP_TYPE_BGET_LDAMP CKGUID(0x532052cd,0x97d4334)
#define PARAM_OP_TYPE_BGET_LDAMPT CKGUID(0x7e6623dd,0x3beb16d1)
#define PARAM_OP_TYPE_BGET_ADAMP CKGUID(0x6c620e3b,0x4c2344d8)
#define PARAM_OP_TYPE_BGET_ADAMPT CKGUID(0x148b07eb,0x563c4ff7)


void ParamOpBIsSubShape(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);

	CK_ID targetID_Sub;
	p2->GetValue(&targetID_Sub);

	int result=0;
	
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				
				if (target->isSubShape(static_cast<CK3dEntity*>(context->GetObject(targetID_Sub))))
				{
					result = 1;
				}
			}
		}
	}

	res->SetValue(&result);

}

//////////////////////////////////////////////////////////////////////////
void ParamOpBGetLVelocity(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				VxVector vec   = target->getLinearVelocity();
				res->SetValue(&vec);
			}
		}
	}
}

void ParamOpBGetAVelocity(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				VxVector vec   = target->getAngularVelocity();
				res->SetValue(&vec);
			}
		}
	}
}

void ParamOpBGetForce(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				VxVector vec   = target->getLinearMomentum();
				res->SetValue(&vec);
			}
		}
	}
}
void ParamOpBGetTorque(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				VxVector vec   = target->getAngularMomentum();
				res->SetValue(&vec);
			}
		}
	}
}

void ParamOpBisFixed(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				int value  = target->getActor()->isDynamic();
				res->SetValue(&value);
			}
		}
	}
}
void ParamOpBGetHType(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				int value  = target->getHullType();
				res->SetValue(&value);
			}
		}
	}
}

void ParamOpBisKinematic(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				int value  = target->isKinematic();
				res->SetValue(&value);
			}
		}
	}
}
void ParamOpBisCollider(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				int value  = target->isCollisionEnabled(ent);
				res->SetValue(&value);
			}
		}
	}
}
void ParamOpBisGravity(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				int value = target->isAffectedByGravity();
				res->SetValue(&value);
			}
		}
	}
}

void ParamOpBGetCollGroup(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				int value = target->getCollisionsGroup();
				res->SetValue(&value);
			}
		}
	}
}
void ParamOpBIsSleeping(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				int value = target->isSleeping();
				res->SetValue(&value);
			}
		}
	}
}
void ParamOpBGetFlags(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				target->recalculateFlags(0);
				int value = target->getFlags();
				res->SetValue(&value);
			}
		}
	}
}
void ParamOpBIsPObject(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	int result = 0;
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				result = 1;				
			}
		}
	}
	res->SetValue(&result);
}

void ParamOpBGetPVel(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				VxVector point;
				p2->GetValue(&point);
				VxVector vec   = target->getPointVelocity(point);
				res->SetValue(&vec);
			}
		}
	}
}
void ParamOpBGetLPVel(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				VxVector point;
				p2->GetValue(&point);
				VxVector vec   = target->getLocalPointVelocity(point);
				res->SetValue(&vec);
			}
		}
	}
}




void ParamOpBGetMass(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				float vec   = target->getMass();
				res->SetValue(&vec);
			}
		}
	}
}
void ParamOpBJIsConnected(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);

	CK_ID targetIDB;
	p2->GetValue(&targetIDB);

	int result  = - 1;
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	CK3dEntity *entB  = static_cast<CK3dEntity*>(context->GetObject(targetIDB));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				pJoint *joint = target->isConnected(entB);
				if (joint)
				{
					result = joint->getType();
				}
			}
		}
	}
	res->SetValue(&result);
}
void ParamOpBGetMaterial(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));

	CK_ID targetID2;
	p2->GetValue(&targetID2);
	CK3dEntity *ent2  = static_cast<CK3dEntity*>(context->GetObject(targetID2));

	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				pMaterial mat = target->getShapeMaterial(ent2);
				pFactory::Instance()->copyTo(res,mat);
			}
		}
	}
}


void ParamOpBGetNbJoints(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				int nbJoints = target->getNbJoints();
				res->SetValue(&nbJoints);
				
			}
		}
	}
}

void PhysicManager::_RegisterParameterOperationsBody()
{

	CKParameterManager *pm = m_Context->GetParameterManager();

	pm->RegisterOperationType(PARAM_OP_TYPE_BJ_NBJOINTS, "bNbJoints");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BJ_NBJOINTS,CKPGUID_INT,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBGetNbJoints);



	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_MATERIAL, "bMat");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_MATERIAL,VTS_MATERIAL,CKPGUID_3DENTITY,CKPGUID_3DENTITY,ParamOpBGetMaterial);

	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_VELOCITY, "bVel");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_VELOCITY,CKPGUID_VECTOR,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBGetLVelocity);

	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_AVELOCITY, "bAVel");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_AVELOCITY,CKPGUID_VECTOR,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBGetAVelocity);

	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_FORCE, "bForce");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_FORCE,CKPGUID_VECTOR,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBGetForce);

	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_TORQUE, "bTorque");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_TORQUE,CKPGUID_VECTOR,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBGetTorque);

	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_HTYPE, "bHullType");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_HTYPE,VTE_COLLIDER_TYPE,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBGetHType);

	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_FIXED, "bDynamic");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_FIXED,CKPGUID_BOOL,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBisFixed);

	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_KINEMATIC, "bKinematic");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_KINEMATIC,CKPGUID_BOOL,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBisKinematic);

	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_GRAVITY, "bGravity");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_GRAVITY,CKPGUID_BOOL,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBisGravity);

	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_COLLISION, "bCollider");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_COLLISION,CKPGUID_BOOL,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBisCollider);

	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_COLLISION_GROUP, "bCollGroup");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_COLLISION_GROUP,CKPGUID_INT,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBGetCollGroup);

	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_SLEEPING, "bSleeping");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_SLEEPING,CKPGUID_BOOL,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBIsSleeping);

	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_FLAGS, "bFlags");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_FLAGS,VTF_BODY_FLAGS,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBGetFlags);

	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_ISPOBJECT, "bRegistered");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_ISPOBJECT,CKPGUID_BOOL,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBIsPObject);

	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_PVEL, "bPointVel");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_PVEL,CKPGUID_VECTOR,CKPGUID_3DENTITY,CKPGUID_VECTOR,ParamOpBGetPVel);

	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_LPVEL, "bLPointVel");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_LPVEL,CKPGUID_VECTOR,CKPGUID_3DENTITY,CKPGUID_VECTOR,ParamOpBGetLPVel);

	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_MASS, "bMass");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_MASS,CKPGUID_FLOAT,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBGetMass);

	pm->RegisterOperationType(PARAM_OP_TYPE_BJ_ISCONNECTED, "bConnected");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BJ_ISCONNECTED,VTE_JOINT_TYPE,CKPGUID_3DENTITY,CKPGUID_3DENTITY,ParamOpBJIsConnected);

	pm->RegisterOperationType(PARAM_OP_TYPE_BIS_SUB_SHAPE_OF, "IsSubShape");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BIS_SUB_SHAPE_OF,CKPGUID_BOOL,CKPGUID_3DENTITY,CKPGUID_3DENTITY,ParamOpBIsSubShape);

}



