#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPBGet2Decl();
CKERROR CreatePBGet2Proto(CKBehaviorPrototype **pproto);
int PBGet2(const CKBehaviorContext& behcontext);
CKERROR PBGet2CB(const CKBehaviorContext& behcontext);

enum bInputs
{

	bbI_CollisionGroup,
	bbI_Kinematic,
	bbI_Gravity,
	bbI_Collision,
	bbI_MassOffset,
	bbI_ShapeOffset,


};

//************************************
// Method:    FillBehaviorPBGet2Decl
// FullName:  FillBehaviorPBGet2Decl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPBGet2Decl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBGetEx-Obsolete");	
	od->SetCategory("Physic/Body");
	od->SetDescription("Retrieves physic related parameters.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x234334d3,0x70d06f74));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBGet2Proto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePBGet2Proto
// FullName:  CreatePBGet2Proto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePBGet2Proto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBGetEx-Obsolete");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");


	/*
	PBGetEx  

	PBGet2 is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Retrieves various physical informations.<br>

	<h3>Technical Information</h3>

	\image html PBGet2.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Target: </SPAN>The 3D Entity associated to the rigid body.
	<BR>
	<BR>
	<SPAN CLASS="pout">Collisions Group: </SPAN>Which collision group this body is part of.See pRigidBody::getCollisionsGroup().
	<BR>
	<SPAN CLASS="pout">Kinematic Object: </SPAN>The kinematic state. See pRigidBody::isKinematic().
	<BR>
	<SPAN CLASS="pout">Gravity: </SPAN>The gravity state.See pRigidBody::isAffectedByGravity().
	<BR>
	<SPAN CLASS="pout">Collision: </SPAN>Determines whether the body reacts to collisions.See pRigidBody::isCollisionEnabled(). 
	<BR>
	<BR>
	<SPAN CLASS="setting">Collisions Group: </SPAN>Enables output for collisions group.
	<BR>
	<SPAN CLASS="setting">Kinematic Object: </SPAN>Enables output for kinematic object.
	<BR>
	<SPAN CLASS="setting">Gravity: </SPAN>Enables output for gravity.
	<BR>
	<SPAN CLASS="setting">Collision: </SPAN>Enables output for collision. 
	<BR>
	<BR>
	<BR>

	<BR>
	<h3>Warning</h3>
	The body must be dynamic.
	<br>
	<br>
	<h3>Note</h3>
	Is utilizing #pRigidBody #pWorld #PhysicManager.<br>

	<h3>VSL</h3><br>
	<SPAN CLASS="NiceCode">
		\include PBGetEx.cpp
	</SPAN>
	*/
	

	
	proto->DeclareSetting("Collisions Group",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Kinematic",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Gravity",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Collision",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Mass Offset",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Pivot Offset",CKPGUID_BOOL,"FALSE");

	proto->SetBehaviorCallbackFct( PBGet2CB );
	
	proto->DeclareOutParameter("Collisions Group",CKPGUID_INT,"0");
	proto->DeclareOutParameter("Kinematic Object",CKPGUID_BOOL,"FALSE");
	proto->DeclareOutParameter("Gravity",CKPGUID_BOOL,"FALSE");
	proto->DeclareOutParameter("Collision",CKPGUID_BOOL,"FALSE");
	proto->DeclareOutParameter("Mass Offset",CKPGUID_VECTOR,"0.0f");
	proto->DeclareOutParameter("Pivot Offset",CKPGUID_VECTOR,"0.0f");

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PBGet2);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PBGet2
// FullName:  PBGet2
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PBGet2(const CKBehaviorContext& behcontext)
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
			//linear damp : 
			DWORD cGroup;
			beh->GetLocalParameterValue(bbI_CollisionGroup,&cGroup);
			if (cGroup)
			{
				int val  = result->getCollisionsGroup();
				SetOutputParameterValue<int>(beh,bbI_CollisionGroup,val);
			}

			DWORD kine;
			beh->GetLocalParameterValue(bbI_Kinematic,&kine);
			if (kine)
			{
				int val  = result->isKinematic();
				SetOutputParameterValue<int>(beh,bbI_Kinematic,val);
			}

			
			DWORD gravity;
			beh->GetLocalParameterValue(bbI_Gravity,&gravity);
			if (gravity)
			{
				int val  = result->getActor()->readBodyFlag(NX_BF_DISABLE_GRAVITY);
				SetOutputParameterValue<int>(beh,bbI_Gravity,!val);
			}

				
			DWORD col;
			beh->GetLocalParameterValue(bbI_Collision,&col);
			if (col)
			{
				int val  = result->isCollisionEnabled();
				SetOutputParameterValue<int>(beh,bbI_Collision,val);
			}


			DWORD mass;
			beh->GetLocalParameterValue(bbI_MassOffset,&mass);
			if (mass)
			{
				VxVector val  = result->getMassOffset();
				SetOutputParameterValue<VxVector>(beh,bbI_MassOffset,val);
			}

			
			DWORD pivot;
			beh->GetLocalParameterValue(bbI_ShapeOffset,&pivot);
			if (mass)
			{
//				VxVector val  = result->getPivotOffset();
//				SetOutputParameterValue<VxVector>(beh,bbI_ShapeOffset,val);
			}
		}
		beh->ActivateInput(0,FALSE);
		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PBGet2CB
// FullName:  PBGet2CB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PBGet2CB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	
	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
			DWORD collGroup;
			beh->GetLocalParameterValue(bbI_CollisionGroup,&collGroup);
			beh->EnableOutputParameter(bbI_CollisionGroup,collGroup);

			DWORD kinematic;
			beh->GetLocalParameterValue(bbI_Kinematic,&kinematic);
			beh->EnableOutputParameter(bbI_Kinematic,kinematic);
			
			DWORD grav;
			beh->GetLocalParameterValue(bbI_Gravity,&grav);
			beh->EnableOutputParameter(bbI_Gravity,grav);

			DWORD coll;
			beh->GetLocalParameterValue(bbI_Collision,&coll);
			beh->EnableOutputParameter(bbI_Collision,coll);


			DWORD mOff;
			beh->GetLocalParameterValue(bbI_MassOffset,&mOff);
			beh->EnableOutputParameter(bbI_MassOffset,mOff);

			DWORD pOff;
			beh->GetLocalParameterValue(bbI_ShapeOffset,&pOff);
			beh->EnableOutputParameter(bbI_ShapeOffset,pOff);

		}
		break;
	}
	return CKBR_OK;

}

