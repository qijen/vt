#include <StdAfx.h>
#include "vtPhysXAll.h"


#define  BB_SET_POSITION_GUID CKGUID(0xe456e78a, 0x456789aa) 
#define  BB_SET_ORIENTATION_GUID CKGUID(0x625874aa, 0xaa694132)
#define  BB_ROTATE_GUID CKGUID(0xffffffee, 0xeeffffff)
#define  BB_TRANSLATE_GUID CKGUID(0x000d000d, 0x000d000d)

#define  BB_SET_EORIENTATION_GUID CKGUID(0xc4966d8,0x6c0c6d14)


CKBEHAVIORFCT BBSetEOri;

int BB_SetEOrientationNew(const CKBehaviorContext& context)
{
	CKBehavior	*behaviour = context.Behavior;
	CKContext	*ctx = context.Context;

	BBSetEOri(context);

	int pUpdate=0;
	behaviour->GetLocalParameterValue(1,&pUpdate);
	if (pUpdate)
	{
		CK3dEntity *ent = (CK3dEntity *) behaviour->GetTarget();
		if( !ent ) 
			return CKBR_OWNERERROR;
		pRigidBody *solid = GetPMan()->getBody(ent);
		if (solid)
		{

			VxVector pos,scale;	
			VxQuaternion quat;
			Vx3DDecomposeMatrix(ent->GetWorldMatrix(),quat,pos,scale);
			solid->setRotation(quat);

			VxVector vectorN(0,0,0);

			int pResetF=0;
			behaviour->GetLocalParameterValue(2,&pResetF);
			int pResetT=0;
			behaviour->GetLocalParameterValue(3,&pResetT);

			int pResetLV=0;
			behaviour->GetLocalParameterValue(4,&pResetLV);
			int pResetAV=0;
			behaviour->GetLocalParameterValue(5,&pResetAV);

			if (pResetF)
			{
				solid->setLinearMomentum(vectorN);
			}

			if (pResetT)
			{
				solid->setAngularMomentum(vectorN);
			}

			if (pResetLV)
			{	
				solid->setLinearVelocity(vectorN);
			}

			if (pResetAV)
			{	
				solid->setAngularVelocity(vectorN);
			}

		}else
		{
			pWorld *world  = GetPMan()->getWorldByBody(ent);
			if (world)
			{
				solid  = world->getBodyFromSubEntity(ent);
				if (solid)
				{
					solid->updateSubShapes(false,false,true);
				}
			}
		}
	}
	return CK_OK;
}
/************************************************************************/
/* Set Position :                                                                      */
/************************************************************************/
CKBEHAVIORFCT BBSetPos;
int BB_SetPosNew(const CKBehaviorContext& context)
{

	
	CKBehavior	*behaviour = context.Behavior;
	CKContext	*ctx = context.Context;

	BBSetPos(context);

	int pUpdate=0;
	behaviour->GetLocalParameterValue(0,&pUpdate);


	
	if (pUpdate)
	{
		CK3dEntity *ent = (CK3dEntity *) behaviour->GetTarget();
		if( !ent ) 
			return CKBR_OWNERERROR;
		
		pWorld *world = GetPMan()->getWorldByBody(ent);
		if (!world)
		{
			return 0;  
		}

		pRigidBody *solid = world->getBody(ent);
		if (solid)
		{
				VxVector vector(0,0,0);
				//behaviour->GetInputParameterValue(0,&vector);
				ent->GetPosition(&vector);
				solid->setPosition(vector,ent);

				VxVector vectorN(0,0,0);

				int pResetF=0;
				behaviour->GetLocalParameterValue(1,&pResetF);
				int pResetT=0;
				behaviour->GetLocalParameterValue(2,&pResetT);

				int pResetLV=0;
				behaviour->GetLocalParameterValue(3,&pResetLV);
				int pResetAV=0;
				behaviour->GetLocalParameterValue(4,&pResetAV);

				if (pResetF)
				{
					solid->setLinearMomentum(vectorN);
				}

				if (pResetT)
				{
					solid->setAngularMomentum(vectorN);
				}

				if (pResetLV)
				{	
					solid->setLinearVelocity(vectorN);
				}

				if (pResetAV)
				{	
					solid->setAngularVelocity(vectorN);
				}
		}else
		{
			pWorld *world  = GetPMan()->getWorldByBody(ent);
			if (world)
			{
				solid  = world->getBodyFromSubEntity(ent);
				if (solid)
				{
					solid->updateSubShapes(false,true,false);
				}
			}
		}

	}
	return CK_OK;
	
}

/************************************************************************/
/* Set Orientation :                                                                      */
/************************************************************************/
CKBEHAVIORFCT BBSetOri;
int BB_SetOrientationNew(const CKBehaviorContext& context)
{
	CKBehavior	*behaviour = context.Behavior;
	CKContext	*ctx = context.Context;

	BBSetOri(context);

	int pUpdate=0;
	behaviour->GetLocalParameterValue(0,&pUpdate);
	if (pUpdate)
	{
		CK3dEntity *ent = (CK3dEntity *) behaviour->GetTarget();
		if( !ent ) 
			return CKBR_OWNERERROR;
		pRigidBody *solid = GetPMan()->getBody(ent);
		if (solid)
		{
			
			VxVector pos,scale;	
			VxQuaternion quat;
			Vx3DDecomposeMatrix(ent->GetWorldMatrix(),quat,pos,scale);
			solid->setRotation(quat);

			VxVector vectorN(0,0,0);

			int pResetF=0;
			behaviour->GetLocalParameterValue(1,&pResetF);
			int pResetT=0;
			behaviour->GetLocalParameterValue(2,&pResetT);

			int pResetLV=0;
			behaviour->GetLocalParameterValue(3,&pResetLV);
			int pResetAV=0;
			behaviour->GetLocalParameterValue(4,&pResetAV);

			if (pResetF)
			{
				solid->setLinearMomentum(vectorN);
			}

			if (pResetT)
			{
				solid->setAngularMomentum(vectorN);
			}

			if (pResetLV)
			{	
				solid->setLinearVelocity(vectorN);
			}

			if (pResetAV)
			{	
				solid->setAngularVelocity(vectorN);
			}

		}else
		{
			pWorld *world  = GetPMan()->getWorldByBody(ent);
			if (world)
			{
				solid  = world->getBodyFromSubEntity(ent);
				if (solid)
				{
					solid->updateSubShapes(false,false,true);
				}
			}
		}
	}
	return CK_OK;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CKBEHAVIORFCT BBRotate;
int BB_RotateNew(const CKBehaviorContext& context)
{

	
	CKBehavior	*behaviour = context.Behavior;
	CKContext	*ctx = context.Context;

	BBRotate(context);

	int pUpdate=0;
	behaviour->GetLocalParameterValue(0,&pUpdate);
	if (pUpdate)
	{
		CK3dEntity *ent = (CK3dEntity *) behaviour->GetTarget();
		if( !ent ) 
			return CKBR_OWNERERROR;

		pRigidBody *solid = GetPMan()->getBody(ent);
		if (solid)
		{
			VxVector pos,scale;	
			VxQuaternion quat;
			Vx3DDecomposeMatrix(ent->GetWorldMatrix(),quat,pos,scale);
			solid->setRotation(quat);

			VxVector vectorN(0,0,0);

			int pResetF=0;
			behaviour->GetLocalParameterValue(1,&pResetF);
			int pResetT=0;
			behaviour->GetLocalParameterValue(2,&pResetT);

			int pResetLV=0;
			behaviour->GetLocalParameterValue(3,&pResetLV);
			int pResetAV=0;
			behaviour->GetLocalParameterValue(4,&pResetAV);

			if (pResetF)
			{
				solid->setLinearMomentum(vectorN);
			}

			if (pResetT)
			{
				solid->setAngularMomentum(vectorN);
			}

			if (pResetLV)
			{	
				solid->setLinearVelocity(vectorN);
			}

			if (pResetAV)
			{	
				solid->setAngularVelocity(vectorN);
			}

		}else
		{
			pWorld *world  = GetPMan()->getWorldByBody(ent);
			if (world)
			{
				solid  = world->getBodyFromSubEntity(ent);
				if (solid)
				{
					solid->updateSubShapes(false,false,true);
				}
			}
		}

	}
	return CK_OK;
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
CKBEHAVIORFCT BBTranslate;
int BB_TranslateNew(const CKBehaviorContext& context)
{


	CKBehavior	*behaviour = context.Behavior;
	CKContext	*ctx = context.Context;

	BBTranslate(context);

	int pUpdate=0;
	behaviour->GetLocalParameterValue(0,&pUpdate);
	if (pUpdate)
	{
		CK3dEntity *ent = (CK3dEntity *) behaviour->GetTarget();
		if( !ent ) 
			return CKBR_OWNERERROR;
		pRigidBody *solid = GetPMan()->getBody(ent);
		if (solid)
		{
			VxVector pos,scale;	
			VxQuaternion quat;
			Vx3DDecomposeMatrix(ent->GetWorldMatrix(),quat,pos,scale);
			solid->setPosition(pos,ent);

			VxVector vectorN(0,0,0);

			int pResetF=0;
			behaviour->GetLocalParameterValue(1,&pResetF);
			int pResetT=0;
			behaviour->GetLocalParameterValue(2,&pResetT);

			int pResetLV=0;
			behaviour->GetLocalParameterValue(3,&pResetLV);
			int pResetAV=0;
			behaviour->GetLocalParameterValue(4,&pResetAV);

			if (pResetF)
			{
				solid->setLinearMomentum(vectorN);
			}

			if (pResetT)
			{
				solid->setAngularMomentum(vectorN);
			}

			if (pResetLV)
			{	
				solid->setLinearVelocity(vectorN);
			}
			
			if (pResetAV)
			{	
				solid->setAngularVelocity(vectorN);
			}
		}else
		{
			pWorld *world  = GetPMan()->getWorldByBody(ent);
			if (world)
			{
				solid  = world->getBodyFromSubEntity(ent);
				if (solid)
				{
					solid->updateSubShapes(false,true,false);
				}
			}
		}

	}
	return CK_OK;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
CKERROR PhysicManager::_Hook3DBBs()
{
	
	CKBehaviorManager *bm = m_Context->GetBehaviorManager();
	
	CKBehaviorPrototype *bproto = CKGetPrototypeFromGuid(BB_SET_EORIENTATION_GUID);
	if(bproto)
	{
		bproto->DeclareSetting("Update Physics",CKPGUID_BOOL,"true");
		bproto->DeclareSetting("Reset Force",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Torque",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Linear Velocity",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Angular Velocity",CKPGUID_BOOL,"false");
		BBSetEOri = bproto->GetFunction();
		bproto->SetFunction(BB_SetEOrientationNew);

	}

	bproto = CKGetPrototypeFromGuid(BB_SET_POSITION_GUID);
	if(bproto)
	{
		bproto->DeclareSetting("Update Physics",CKPGUID_BOOL,"true");
		bproto->DeclareSetting("Reset Force",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Torque",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Linear Velocity",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Angular Velocity",CKPGUID_BOOL,"false");
		BBSetPos = bproto->GetFunction();
		bproto->SetFunction(BB_SetPosNew);

	}

	bproto = CKGetPrototypeFromGuid(BB_SET_ORIENTATION_GUID);
	if(bproto)
	{
		bproto->DeclareSetting("Update Physics",CKPGUID_BOOL,"true");
		bproto->DeclareSetting("Reset Force",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Torque",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Linear Velocity",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Angular Velocity",CKPGUID_BOOL,"false");
		BBSetOri = bproto->GetFunction();
		bproto->SetFunction(BB_SetOrientationNew);
	}

	bproto = CKGetPrototypeFromGuid(BB_ROTATE_GUID);
	if(bproto)
	{
		bproto->DeclareSetting("Update Physics",CKPGUID_BOOL,"true");
		bproto->DeclareSetting("Reset Force",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Torque",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Linear Velocity",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Angular Velocity",CKPGUID_BOOL,"false");
		BBRotate = bproto->GetFunction();
		bproto->SetFunction(BB_RotateNew);
	}
	bproto = CKGetPrototypeFromGuid(BB_TRANSLATE_GUID);
	if(bproto)
	{
		bproto->DeclareSetting("Update Physics",CKPGUID_BOOL,"true");
		bproto->DeclareSetting("Reset Force",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Torque",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Linear Velocity",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Angular Velocity",CKPGUID_BOOL,"false");
		BBTranslate = bproto->GetFunction();
		bproto->SetFunction(BB_TranslateNew);
	}
	return CK_OK;
}
