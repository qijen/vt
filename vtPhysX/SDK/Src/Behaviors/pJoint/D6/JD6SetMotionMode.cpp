#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorJD6SetMotionModeDecl();
CKERROR CreateJD6SetMotionModeProto(CKBehaviorPrototype **pproto);
int JD6SetMotionMode(const CKBehaviorContext& behcontext);
CKERROR JD6SetMotionModeCB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_BodyA=0,
	bbI_BodyB,
	bbI_Anchor,
	bbI_AnchorRef,
	bbI_Axis,
	bbI_AxisRef
};

//************************************
// Method:    FillBehaviorJD6SetMotionModeDecl
// FullName:  FillBehaviorJD6SetMotionModeDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorJD6SetMotionModeDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJD6SetMotionMode");	
	od->SetCategory("Physic/D6");
	od->SetDescription("Sets the motion freedom mode  in a D6 joint.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x62dc4a7e,0x20736a5e));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateJD6SetMotionModeProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreateJD6SetMotionModeProto
// FullName:  CreateJD6SetMotionModeProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreateJD6SetMotionModeProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJD6SetMotionMode");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In0");
	proto->DeclareOutput("Out0");

	
	
	proto->SetBehaviorCallbackFct( JD6SetMotionModeCB );
	
	
	proto->DeclareInParameter("Body B",CKPGUID_3DENTITY,"ball2");
	proto->DeclareInParameter("Motion Axis",VTE_JOINT_MOTION_MODE_AXIS,0);
	proto->DeclareInParameter("Motion Mode",VTE_JOINT_MOTION_MODE,0);



	
	
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(JD6SetMotionMode);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    JD6SetMotionMode
// FullName:  JD6SetMotionMode
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int JD6SetMotionMode(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;
	
	if( beh->IsInputActive(0) )
	{
		beh->ActivateInput(0,FALSE);

		//////////////////////////////////////////////////////////////////////////
		//the object A: 
		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		CK3dEntity *targetB = (CK3dEntity *) beh->GetInputParameterObject(0);
		
		if (!pFactory::Instance()->jointCheckPreRequisites(target,targetB,JT_D6))
		{

			return CK_OK;
		}
		
		
		
		// the world :  
		pWorld *worldA=GetPMan()->getWorldByBody(target); 
		pWorld *worldB=GetPMan()->getWorldByBody(targetB); 
		if (!worldA)
		{
			worldA = worldB;
		}
		
		if (!worldA)
		{
			beh->ActivateOutput(0);
			return 0;
		}
		
		// the physic object A : 
		pRigidBody*bodyA= worldA->getBody(target);
		pRigidBody*bodyB= worldA->getBody(targetB);

		if(bodyA || bodyB)
		{

			pJointD6 *joint =static_cast<pJointD6*>(worldA->getJoint(target,targetB,JT_D6));
			int motionAxis = GetInputParameterValue<int>(beh,1); 
			D6MotionMode motionMode = GetInputParameterValue<D6MotionMode>(beh,2);

			if (joint)
			{
				switch(motionAxis)
				{
				
					case D6MA_Twist:
						joint->setTwistMotionMode(motionMode);
						break;
					case D6MA_Swing1:
						joint->setSwing1MotionMode(motionMode);
						break;
					case D6MA_Swing2:
						joint->setSwing2MotionMode(motionMode);
						break;
					case D6MA_X:
						joint->setXMotionMode(motionMode);
						break;
					case D6MA_Y:
						joint->setYMotionMode(motionMode);
						break;
					case D6MA_Z:
						joint->setZMotionMode(motionMode);
						break;
				}
			}
		}
		
		beh->ActivateOutput(0);
	}
	return 0;
}
//************************************
// Method:    JD6SetMotionModeCB
// FullName:  JD6SetMotionModeCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR JD6SetMotionModeCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	

	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
			DWORD fmax;
			beh->GetLocalParameterValue(0,&fmax);
			if (fmax  )
			{
				beh->EnableInputParameter(6,fmax);
				beh->EnableInputParameter(7,fmax);
			
			}else
			{
				beh->EnableInputParameter(6,fmax);
				beh->EnableInputParameter(7,fmax);
				//////////////////////////////////////////////////////////////////////////
				//the object A: 
				CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
				CK3dEntity *targetB = (CK3dEntity *) beh->GetInputParameterObject(0);

				if (target && targetB)
				{
					// the world :  
					pWorld *world=GetPMan()->getWorldByBody(target); 
					if (!world)
						return CKBR_OK;

					// the physic object A and B : 
					pRigidBody*bodyA= world->getBody(target);
					pRigidBody*bodyB= world->getBody(targetB);

					if(bodyA && bodyB)
					{
//						pJointHinge2 *joint = static_cast<pJointHinge2*>(bodyA->isConnected(targetB));
						/*
						if (joint && joint->GetFeedBack())
						{
							joint->SetFeedBack(NULL,0,0);
						}*/
					}
				}
			}
		}
		break;
	}
	return CKBR_OK;
}
