#include <StdAfx.h>
#include "vtPhysXAll.h"

#define  BB_SET_POSITION_GUID CKGUID(0xe456e78a, 0x456789aa) 
#define  BB_SET_ORIENTATION_GUID CKGUID(0x625874aa, 0xaa694132)
#define  BB_ROTATE_GUID CKGUID(0xffffffee, 0xeeffffff)
#define  BB_TRANSLATE_GUID CKGUID(0x000d000d, 0x000d000d)

#define  BB_SET_EORIENTATION_GUID CKGUID(0xc4966d8,0x6c0c6d14)
#define  BB_PLAY_GLOBAL_ANIMATION CKGUID(0x1c9236e1,0x42f40996)
#define  BB_SET_LOCAL_MATRIX CKGUID(0x21f5f30d, 0x08d5a1db)
#define  BB_SET_WORLD_MATRIX CKGUID(0xaa4aa6f0, 0xddefdef4)
#define  BB_RESTORE_IC CKGUID(0x766e4e44,0x4fac6d52)
#define  BB_LAUNCH_SCENE CKGUID(0x188d6d43,0x169613dd)



CKBEHAVIORFCT BBSetEOri;
CKBEHAVIORFCT BBSetPos;
CKBEHAVIORFCT BBSetOri;
CKBEHAVIORFCT BBRotate;
CKBEHAVIORFCT BBTranslate;
CKBEHAVIORFCT BBPlayGlobalAnimation;
CKBEHAVIORFCT BBSetLocalMatrix;
CKBEHAVIORFCT BBSetWorldMatrix;
CKBEHAVIORFCT BBRestoreIC;
CKBEHAVIORFCT BBLaunchScene;



#define BRESET VxVector vectorN(0,0,0);\
	int pResetF=0;behaviour->GetLocalParameterValue(1,&pResetF);\
	int pResetT=0;behaviour->GetLocalParameterValue(2,&pResetT);\
	int pResetLV=0;behaviour->GetLocalParameterValue(3,&pResetLV);\
	int pResetAV=0;behaviour->GetLocalParameterValue(4,&pResetAV);\
	if (pResetF)	solid->setLinearMomentum(vectorN);\
	if (pResetT)solid->setAngularMomentum(vectorN);\
	if (pResetLV)solid->setLinearVelocity(vectorN);\
	if (pResetAV)solid->setAngularVelocity(vectorN);\


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


			BRESET;
			VxVector pos,scale;	VxQuaternion quat;
			Vx3DDecomposeMatrix(ent->GetWorldMatrix(),quat,pos,scale);
			int hierarchy = vtTools::BehaviorTools::GetInputParameterValue<int>(behaviour,1);
			if (  (solid->getFlags()  & BF_Hierarchy ) && solid->GetVT3DObject() !=ent )
			{
				solid->onSubShapeTransformation(false,true,true,ent,hierarchy);
			}else{
				VxVector vector(0,0,0);
				ent->GetPosition(&vector);
				solid->setPosition(vector,ent);
				
				VxVector pos,scale;	
				VxQuaternion quat;
				Vx3DDecomposeMatrix(ent->GetWorldMatrix(),quat,pos,scale);
				solid->setRotation(quat);
			}
			//solid->updateSubShapes(false,false,true,ent);

		}
	}
	return CK_OK;
}
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
			BRESET;
			VxVector pos,scale;	VxQuaternion quat;
			Vx3DDecomposeMatrix(ent->GetWorldMatrix(),quat,pos,scale);
			int hierarchy = vtTools::BehaviorTools::GetInputParameterValue<int>(behaviour,1);
			if (  (solid->getFlags()  & BF_Hierarchy ) && solid->GetVT3DObject() !=ent )
			{
				solid->onSubShapeTransformation(false,true,false,ent,hierarchy);
			}else{
				VxVector vector(0,0,0);
				ent->GetPosition(&vector);
				solid->setPosition(vector,ent);
			}
		}
	}
	return CK_OK;
	
}


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
			
			BRESET
			int hierarchy = vtTools::BehaviorTools::GetInputParameterValue<int>(behaviour,2);
			if (  (solid->getFlags()  & BF_Hierarchy ) && solid->GetVT3DObject() !=ent )
			{
				solid->onSubShapeTransformation(false,true,true,ent,hierarchy);
			}else{
			
				VxVector pos,scale;	
				VxQuaternion quat;
				Vx3DDecomposeMatrix(ent->GetWorldMatrix(),quat,pos,scale);
				solid->setRotation(quat);
			}
			//solid->updateSubShapes(false,true,true,ent);
		}
	}
	return CK_OK;
}

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

		pRigidBody *solid = GetPMan()->getBody(ent,true);
		if (solid)
		{
			BRESET;
			int hierarchy = vtTools::BehaviorTools::GetInputParameterValue<int>(behaviour,3);
			if (  (solid->getFlags()  & BF_Hierarchy ) && solid->GetVT3DObject() !=ent )
			{
				solid->onSubShapeTransformation(false,true,true,ent,hierarchy);
			}else{

				VxVector pos,scale;	
				VxQuaternion quat;
				Vx3DDecomposeMatrix(ent->GetWorldMatrix(),quat,pos,scale);
				solid->setRotation(quat);
			}
		}
	}
	return CK_OK;
}

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
			BRESET;
			VxVector pos,scale;	VxQuaternion quat;
			Vx3DDecomposeMatrix(ent->GetWorldMatrix(),quat,pos,scale);
			int hierarchy = vtTools::BehaviorTools::GetInputParameterValue<int>(behaviour,1);
			if (  (solid->getFlags()  & BF_Hierarchy ) && solid->GetVT3DObject() !=ent )
			{
				solid->onSubShapeTransformation(false,true,true,ent,hierarchy);
			}else{
				solid->setPosition(pos,ent);
			}
		}
	}
	return CK_OK;
}

int BBPlayGlobalAnimation_New(const CKBehaviorContext& context)
{


	CKBehavior	*behaviour = context.Behavior;
	CKContext	*ctx = context.Context;

	int returnValue= BBPlayGlobalAnimation(context);
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
			

			BRESET
			if (  (solid->getFlags()  & BF_Hierarchy ) && solid->GetVT3DObject() !=ent )
			{
				solid->onSubShapeTransformation(false,true,true,ent,true);
			}else{
				VxVector pos,scale;	
				VxQuaternion quat;
				Vx3DDecomposeMatrix(ent->GetWorldMatrix(),quat,pos,scale);
				solid->setPosition(pos,ent);
			
			}
		}/*
		else
				{
					pWorld *world  = GetPMan()->getWorldByBody(ent);
					if (world)
					{
						solid  = world->getBodyFromSubEntity(ent);
						if (solid)
						{
							solid->updateSubShapes(false,true,true);
						}
					}
				}*/
		
	}
	return returnValue;
}

int BBSetLocalMatrix_New(const CKBehaviorContext& context)
{
	CKBehavior	*behaviour = context.Behavior;
	CKContext	*ctx = context.Context;

	BBSetLocalMatrix(context);

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

			BRESET;
				int hierarchy = vtTools::BehaviorTools::GetInputParameterValue<int>(behaviour,1);
			if (  (solid->getFlags()  & BF_Hierarchy ) && solid->GetVT3DObject() !=ent )
			{
				solid->onSubShapeTransformation(false,true,true,ent,hierarchy);
			}else{
				VxVector pos,scale;	
				VxQuaternion quat;
				Vx3DDecomposeMatrix(ent->GetWorldMatrix(),quat,pos,scale);
				solid->setPosition(pos,ent);
			}
		}
	}
	return CK_OK;
}

int BBSetWorldMatrix_New(const CKBehaviorContext& context)
{
	CKBehavior	*behaviour = context.Behavior;
	CKContext	*ctx = context.Context;

	BBSetWorldMatrix(context);

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

			//solid->getActor()->

			BRESET
				int hierarchy = vtTools::BehaviorTools::GetInputParameterValue<int>(behaviour,1);
			if (  (solid->getFlags()  & BF_Hierarchy ) && solid->GetVT3DObject() !=ent )
			{
				solid->onSubShapeTransformation(false,true,true,ent,hierarchy);
			}else{
				VxVector pos,scale;	
				VxQuaternion quat;
				Vx3DDecomposeMatrix(ent->GetWorldMatrix(),quat,pos,scale);
				solid->setPosition(pos,ent);
			}
		}
	}
	return CK_OK;
}

int BBRestoreIC_New(const CKBehaviorContext& context)
{
	CKBehavior	*behaviour = context.Behavior;
	CKContext	*ctx = context.Context;

	BBRestoreIC(context);

	int pUpdate=0;
	behaviour->GetLocalParameterValue(0,&pUpdate);
	CK3dEntity *ent = (CK3dEntity *) behaviour->GetTarget();
	int hierarchy=0;behaviour->GetLocalParameterValue(5,&hierarchy);
	int restoreJoints=0;behaviour->GetLocalParameterValue(6,&restoreJoints);
	bool done= false;
	if (pUpdate)
	{
		if( !ent ) 
			return CKBR_OWNERERROR;
		pRigidBody *solid = GetPMan()->getBody(ent);
		if (solid)
		{
			BRESET

			if (ent && ent->GetClassID() != CKCID_3DOBJECT)
			{
				return CK_OK;
			}

			/*
			if (  (solid->getFlags()  & BF_Hierarchy ) && solid->GetVT3DObject() !=ent )
			{
				solid->onSubShapeTransformation(false,true,true,ent,(solid->getFlags()  & BF_Hierarchy ));
			}else{
				VxVector pos,scale;	
				VxQuaternion quat;
				Vx3DDecomposeMatrix(ent->GetWorldMatrix(),quat,pos,scale);
				solid->setPosition(pos,ent);
			}
			*/

			pRigidBodyRestoreInfo *rInfo = new pRigidBodyRestoreInfo();
			rInfo->hierarchy = hierarchy;
			rInfo->removeJoints= restoreJoints;
			GetPMan()->_getRestoreMap()->Insert(ent->GetID(),rInfo);
			done = true;
		}
	}

	if (hierarchy && !done)
	{
		CKScene *scene  = GetPMan()->GetContext()->GetCurrentLevel()->GetLevelScene();
		if(scene)
		{		
			CK3dEntity* subEntity = NULL;
			XArray<NxShape*>SrcObjects;
			while (subEntity= ent->HierarchyParser(subEntity) )
			{
				CKStateChunk *chunk = scene->GetObjectInitialValue(subEntity);
				if (chunk)
				{
					CKReadObjectState(subEntity,chunk);
				}
			}
		}
	}
	return CK_OK;
}



int BBLaunchScene_New(const CKBehaviorContext& context)
{
	CKBehavior	*behaviour = context.Behavior;
	CKContext	*ctx = context.Context;


	CKScene *currentScene= GetPMan()->GetContext()->GetCurrentLevel()->GetCurrentScene();
	CKScene *nextScene = NULL;
	BBLaunchScene(context);
/*
	int pUpdate=0;
	behaviour->GetLocalParameterValue(0,&pUpdate);
	nextScene = (CKScene*)behaviour->GetInputParameterObject(0);
	if (pUpdate)
	{
		if(GetPMan()->GetContext()->IsPlaying())
		{		
			GetPMan()->_removeObjectsFromOldScene(currentScene);
			GetPMan()->_checkObjectsByAttribute(nextScene);
		}
	}
	*/
	return CK_OK;
}







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


	
	bproto = CKGetPrototypeFromGuid(BB_PLAY_GLOBAL_ANIMATION);
	if(bproto)
	{
		bproto->DeclareSetting("Update Physics",CKPGUID_BOOL,"true");
		bproto->DeclareSetting("Reset Force",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Torque",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Linear Velocity",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Angular Velocity",CKPGUID_BOOL,"false");
		BBPlayGlobalAnimation = bproto->GetFunction();
		bproto->SetFunction(BBPlayGlobalAnimation_New);
	}
	

	bproto = CKGetPrototypeFromGuid(BB_SET_LOCAL_MATRIX);
	if(bproto)
	{
		bproto->DeclareSetting("Update Physics",CKPGUID_BOOL,"true");
		bproto->DeclareSetting("Reset Force",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Torque",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Linear Velocity",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Angular Velocity",CKPGUID_BOOL,"false");
		BBSetLocalMatrix = bproto->GetFunction();
		bproto->SetFunction(BBSetLocalMatrix_New);
	}

	bproto = CKGetPrototypeFromGuid(BB_SET_WORLD_MATRIX);
	if(bproto)
	{
		bproto->DeclareSetting("Update Physics",CKPGUID_BOOL,"true");
		bproto->DeclareSetting("Reset Force",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Torque",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Linear Velocity",CKPGUID_BOOL,"false");
		bproto->DeclareSetting("Reset Angular Velocity",CKPGUID_BOOL,"false");
		BBSetWorldMatrix = bproto->GetFunction();
		bproto->SetFunction(BBSetWorldMatrix_New);
	}

	bproto = CKGetPrototypeFromGuid(BB_RESTORE_IC);
	if(bproto)
	{
		
		bproto->DeclareSetting("Update Physics",CKPGUID_BOOL,"true");
		bproto->DeclareSetting("Reset Force",CKPGUID_BOOL,"true");
		bproto->DeclareSetting("Reset Torque",CKPGUID_BOOL,"true");
		bproto->DeclareSetting("Reset Linear Velocity",CKPGUID_BOOL,"true");
		bproto->DeclareSetting("Reset Angular Velocity",CKPGUID_BOOL,"true");

		bproto->DeclareSetting("Hierarchy",CKPGUID_BOOL,"true");
		bproto->DeclareSetting("Restore Joints from Attribute",CKPGUID_BOOL,"true");


		BBRestoreIC = bproto->GetFunction();
		bproto->SetFunction(BBRestoreIC_New);
	}

	bproto = CKGetPrototypeFromGuid(BB_LAUNCH_SCENE);
	if(bproto)
	{

		//bproto->DeclareSetting("Destroy Old Physics",CKPGUID_BOOL,"true");
		BBLaunchScene = bproto->GetFunction();
		bproto->SetFunction(BBLaunchScene_New);
	}

	return CK_OK;
}
