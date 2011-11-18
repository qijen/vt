#include <StdAfx.h>
#include "pCommon.h"

CKObjectDeclaration	*FillBehaviorJD6SetParametersDecl();
CKERROR CreateJD6SetParametersProto(CKBehaviorPrototype **pproto);
int JD6SetParameters(const CKBehaviorContext& behcontext);
CKERROR JD6SetParametersCB(const CKBehaviorContext& behcontext);

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
// Method:    FillBehaviorJD6SetParametersDecl
// FullName:  FillBehaviorJD6SetParametersDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorJD6SetParametersDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJD6SetParameters");	
	od->SetCategory("Physic/D6");
	od->SetDescription("Sets parameters in a D6 joint.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x4b8207b2,0x57964805));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateJD6SetParametersProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreateJD6SetParametersProto
// FullName:  CreateJD6SetParametersProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreateJD6SetParametersProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJD6SetParameters");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In0");
	proto->DeclareOutput("Out0");

	
	
	proto->SetBehaviorCallbackFct( JD6SetParametersCB );
	
	
	proto->DeclareInParameter("Body B",CKPGUID_3DENTITY,"ball2");
	proto->DeclareInParameter("Drive Linear Velocity",CKPGUID_VECTOR);
	proto->DeclareInParameter("Drive Angular Velocity",CKPGUID_VECTOR);
	proto->DeclareInParameter("Drive Position",CKPGUID_VECTOR);
	proto->DeclareInParameter("Drive Orientation",CKPGUID_QUATERNION);
	
	proto->DeclareInParameter("Ratio",CKPGUID_FLOAT,"0");
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(JD6SetParameters);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    JD6SetParameters
// FullName:  JD6SetParameters
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int JD6SetParameters(const CKBehaviorContext& behcontext)
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
			VxVector linVel = GetInputParameterValue<VxVector>(beh,1); 
			VxVector aVel = GetInputParameterValue<VxVector>(beh,2); 
			VxVector pos = GetInputParameterValue<VxVector>(beh,3); 
			VxQuaternion rot = GetInputParameterValue<VxQuaternion>(beh,4); 

			float ratio = GetInputParameterValue<float>(beh,5); 
			
			
			joint->setDriveLinearVelocity(linVel);
			joint->setDriveAngularVelocity(aVel);
			joint->setRatio(ratio);
			
			joint->setDrivePosition(pos);
			joint->setDriveRotation(rot);



		}
	
		beh->ActivateOutput(0);
	}
	return 0;
}
//************************************
// Method:    JD6SetParametersCB
// FullName:  JD6SetParametersCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR JD6SetParametersCB(const CKBehaviorContext& behcontext)
{
	return CKBR_OK;
}