#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPJPrismaticDecl();
CKERROR CreatePJPrismaticProto(CKBehaviorPrototype **pproto);
int PJPrismatic(const CKBehaviorContext& behcontext);
CKERROR PJPrismaticCB(const CKBehaviorContext& behcontext);

enum bbInputs
{

	bI_ObjectB,
	bI_Anchor,
	bI_AnchorRef,
	bI_Axis,
	bI_AxisRef,
	bI_Coll
};

//************************************
// Method:    FillBehaviorPJPrismaticDecl
// FullName:  FillBehaviorPJPrismaticDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPJPrismaticDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJPrismatic");	
	od->SetCategory("Physic/Joints");
	od->SetDescription("Sets/modifies a prismatic joint.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x2c270788,0x2d886425));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePJPrismaticProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);

	return od;
}

//************************************
// Method:    CreatePJPrismaticProto
// FullName:  CreatePJPrismaticProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePJPrismaticProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJPrismatic");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In0");
	proto->DeclareOutput("Out0");
	proto->SetBehaviorCallbackFct( PJPrismaticCB );
	proto->DeclareInParameter("Body B",CKPGUID_3DENTITY,"ball2");
	
	proto->DeclareInParameter("Anchor",CKPGUID_VECTOR);
	proto->DeclareInParameter("Anchor Reference",CKPGUID_3DENTITY);
	
	proto->DeclareInParameter("Axis",CKPGUID_VECTOR);
	proto->DeclareInParameter("Axis Up Reference",CKPGUID_3DENTITY);
	proto->DeclareInParameter("Collision",CKPGUID_BOOL);
	/*
	
	proto->DeclareInParameter("Spring",VTS_JOINT_SPRING);

	proto->DeclareInParameter("High Limit",VTS_JLIMIT);
	proto->DeclareInParameter("Low Limit",VTS_JLIMIT);


	

	proto->DeclareInParameter("Motor",VTS_JOINT_MOTOR);
*/
/*
	proto->DeclareSetting("Spring",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Limit",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Motor",CKPGUID_BOOL,"FALSE");
*/	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);

	proto->SetFunction(PJPrismatic);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PJPrismatic
// FullName:  PJPrismatic
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PJPrismatic(const CKBehaviorContext& behcontext)
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
		CK3dEntity *targetB = (CK3dEntity *) beh->GetInputParameterObject(bI_ObjectB);
		
		if (!pFactory::Instance()->jointCheckPreRequisites(target,targetB,JT_Prismatic))
		{
			return CK_OK;
		}
		
		// the world :  
		pWorld *worldA=GetPMan()->getWorldByBody(target); 
		pWorld *worldB=GetPMan()->getWorldByBody(targetB); 
		if (!worldA && ! worldB )
		{
			return 0;
		}
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



		//anchor : 
		VxVector anchor  = GetInputParameterValue<VxVector>(beh,bI_Anchor);
		VxVector anchorOut  = anchor;
		CK3dEntity*anchorReference  =  (CK3dEntity *) beh->GetInputParameterObject(bI_AnchorRef);
		if (anchorReference)
		{
			anchorReference->Transform(&anchorOut,&anchor);
		}





		//swing axis
		VxVector Axis  = GetInputParameterValue<VxVector>(beh,bI_Axis);
		VxVector axisOut  = Axis;
		CK3dEntity*axisReference  =  (CK3dEntity *) beh->GetInputParameterObject(bI_AxisRef);

		if (axisReference)
		{
			VxVector dir,up,right;
			axisReference->GetOrientation(&dir,&up,&right);
			axisReference->TransformVector(&axisOut,&up);
		}

		//////////////////////////////////////////////////////////////////////////
		int col  = GetInputParameterValue<int>(beh,bI_Coll);
		//////////////////////////////////////////////////////////////////////////
		//
		pJointPrismatic *joint = static_cast<pJointPrismatic*>(worldA->getJoint(target,targetB,JT_Prismatic));
		if(bodyA || bodyB)
		{
			//////////////////////////////////////////////////////////////////////////
			//joint create ? 
			if (!joint)
			{
				joint   = static_cast<pJointPrismatic*>(pFactory::Instance()->createPrismaticJoint(target,targetB,anchorOut,axisOut));
			}
			////////////////////////////////////////////////////////////////////////// Modification : 
			if (joint)
			{

				joint->setGlobalAxis(axisOut);
				joint->setGlobalAnchor(anchorOut);
				joint->enableCollision(col);
			}
		}
		
		beh->ActivateOutput(0);
	}
	return 0;
}
//************************************
// Method:    PJPrismaticCB
// FullName:  PJPrismaticCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PJPrismaticCB(const CKBehaviorContext& behcontext)
{
	
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	
	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
		
/*		
			DWORD twistLimit;
			beh->GetLocalParameterValue(1,&twistLimit);
			beh->EnableInputParameter(bbI_HighLimit,twistLimit);
			beh->EnableInputParameter(bbI_LowLimit,twistLimit);

			DWORD springSwing;
			beh->GetLocalParameterValue(0,&springSwing);
			beh->EnableInputParameter(bbI_Spring,springSwing);

			DWORD motor;
			beh->GetLocalParameterValue(2,&motor);
			beh->EnableInputParameter(bbI_Motor,motor);
*/
		break;
	}
	}
	return CKBR_OK;
}