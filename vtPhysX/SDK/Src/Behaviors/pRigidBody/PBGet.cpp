#include <StdAfx.h>
#include "pCommon.h"

CKObjectDeclaration	*FillBehaviorPBGetDecl();
CKERROR CreatePBGetProto(CKBehaviorPrototype **pproto);
int PBGet(const CKBehaviorContext& behcontext);
CKERROR PBGetCB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_Vel=0,
	bbI_AVel,
	bbI_Torque,
	bbI_Force,
};

//************************************
// Method:    FillBehaviorPBGetDecl
// FullName:  FillBehaviorPBGetDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPBGetDecl()
{
	
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBGet-Obsolete");	
	od->SetCategory("Physic/Body");
	od->SetDescription("Retrieves information of a rigid body.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x44f65362,0x36d558af));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBGetProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePBGetProto
// FullName:  CreatePBGetProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePBGetProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBGet-Obsolete");
	if(!proto) return CKERR_OUTOFMEMORY;
	

	/*
	PBGet  

	PBGet is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Retrieves various physical informations.<br>
	
	<h3>Technical Information</h3>

	\image html PBGet.png

	<SPAN CLASS="in">In:</SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out:</SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Target: </SPAN>The 3D Entity associated to the rigid body.
	<BR>
	<BR>
	<SPAN CLASS="pout">Linear Velocity: </SPAN>The linear velocity.See pRigidBody::getLinearVelocity().
	<BR>
	<SPAN CLASS="pout">Angular Velocity: </SPAN>The angular velocity.See pRigidBody::getAngularVelocity().
	<BR>
	<SPAN CLASS="pout">Agular Momentum: </SPAN>The angular momentum.See pRigidBody::getAngularMomentum().
	<BR>
	<SPAN CLASS="pout">Linear Momentum: </SPAN>The linear momentum.See pRigidBody::getLinearMomentum(). 
	<BR>
	<BR>
	<SPAN CLASS="setting">Linear Velocity: </SPAN>Enables output for linear velocity.
	<BR>
	<SPAN CLASS="setting">Angular Velocity: </SPAN>Enables output for angular velocity.
	<BR>
	<SPAN CLASS="setting">Agular Momentum: </SPAN>Enables output for angular momentum.
	<BR>
	<SPAN CLASS="setting">Linear Momentum: </SPAN>Enables output for linear momentum. 
	<BR>
	<BR>
	
	<BR>
	<h3>Warning</h3>
		The body must be dynamic.
	<br>
	<br>

	<h4>Note</h4>
	Is utilizing #pRigidBody #pWorld #PhysicManager .

	*/

	proto->DeclareInput("In0");
	proto->DeclareOutput("Out0");

	proto->DeclareSetting("Velocity",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Angular Velocity",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Torque",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Force",CKPGUID_BOOL,"FALSE");
	proto->SetBehaviorCallbackFct( PBGetCB );
	proto->DeclareOutParameter("Linear Velocity",CKPGUID_VECTOR,"0.0f,0.0f,0.0f");
	proto->DeclareOutParameter("Angular Velocity",CKPGUID_VECTOR,"0.0f,0.0f,0.0f");
	proto->DeclareOutParameter("Torque",CKPGUID_VECTOR,"0.0f,0.0f,0.0f");
	proto->DeclareOutParameter("Force",CKPGUID_VECTOR,"0.0f,0.0f,0.0f");
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PBGet);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PBGet
// FullName:  PBGet
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PBGet(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;
	
	if( beh->IsInputActive(0) )
	{

		//////////////////////////////////////////////////////////////////////////
		//the object : 
		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		if( !target ) return CKBR_OWNERERROR;


		//////////////////////////////////////////////////////////////////////////
		// the world :  
		pWorld *world=GetPMan()->getWorldByBody(target); 
		if (!world)
		{
			beh->ActivateOutput(0);
			return 0;
		}

		// body exists already  ?  clean and delete it : 
		pRigidBody*result = world->getBody(target);
		if(result)
		{
			//////////////////////////////////////////////////////////////////////////
			//velocity : 
			DWORD vel;
			beh->GetLocalParameterValue(bbI_Vel,&vel);
			if (vel)
			{
				VxVector vel_  = result->getLinearVelocity();
				beh->SetOutputParameterValue(bbI_Vel,&vel_);
			}

			//////////////////////////////////////////////////////////////////////////
			//angular velocity 
			DWORD avel;
			beh->GetLocalParameterValue(bbI_AVel,&avel);
			if (avel)
			{
				VxVector avel_  = result->getAngularVelocity();
				beh->SetOutputParameterValue(bbI_Vel,&avel_);
			}

			//////////////////////////////////////////////////////////////////////////
			//torque
			DWORD torque;
			beh->GetLocalParameterValue(bbI_Torque,&avel);
			if (avel)
			{
				VxVector torque_  = result->getAngularMomentum();
				beh->SetOutputParameterValue(bbI_Torque,&torque_);
			}

			//////////////////////////////////////////////////////////////////////////
			//force
			DWORD force;
			beh->GetLocalParameterValue(bbI_Force,&force);
			if (avel)
			{
				VxVector force_ = result->getLinearMomentum();
				beh->SetOutputParameterValue(bbI_Force,&force_);
			}

		}
		
		beh->ActivateInput(0,FALSE);
		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PBGetCB
// FullName:  PBGetCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PBGetCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	
	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
			DWORD vel;
			beh->GetLocalParameterValue(bbI_Vel,&vel);
			beh->EnableOutputParameter(bbI_Vel,vel);
			
			DWORD avel;
			beh->GetLocalParameterValue(bbI_AVel,&avel);
			beh->EnableOutputParameter(bbI_AVel,avel);

			DWORD torque;
			beh->GetLocalParameterValue(bbI_Torque,&torque);
			beh->EnableOutputParameter(bbI_Torque,torque);

			DWORD force;
			beh->GetLocalParameterValue(bbI_Force,&force);
			beh->EnableOutputParameter(bbI_Force,force);
		
		}
		break;
	}
	return CKBR_OK;

}

