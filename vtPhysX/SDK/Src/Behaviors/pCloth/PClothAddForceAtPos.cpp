#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPClothAddForceAtPosDecl();
CKERROR CreatePClothAddForceAtPosProto(CKBehaviorPrototype **pproto);
int PClothAddForceAtPos(const CKBehaviorContext& behcontext);
CKERROR PClothAddForceAtPosCB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_Pos,
	bbI_Magnitute,
	bbI_Radius,
	bbI_ForceMode,
};



//************************************
// Method:    FillBehaviorPClothAddForceAtPosDecl
// FullName:  FillBehaviorPClothAddForceAtPosDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPClothAddForceAtPosDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PClothAddForceAtPos");	
	od->SetCategory("Physic/Cloth");
	od->SetDescription("Applies a radial force (or impulse) at a particular position. All vertices within radius will be affected with a quadratic drop-off. ");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x3f47037b,0x737a60f0));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePClothAddForceAtPosProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePClothAddForceAtPosProto
// FullName:  CreatePClothAddForceAtPosProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePClothAddForceAtPosProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PClothAddForceAtPos");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PClothAddForceAtPos  

	PClothAddForceAtPos is categorized in \ref Clothes

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Applies a radial force (or impulse) at a particular position. All vertices
	within radius will be affected with a quadratic drop-off.<br>

	Because forces are reset at the end of every timestep, 
	you can maintain a total external force on an object by calling this once every frame.


	@see pCloth::addForceAtPos()


	<h3>Technical Information</h3>

	\image html PClothAddForceAtPos.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	
	
	<SPAN CLASS="pin">Target: </SPAN>Target cloth reference.
	<BR>
	<SPAN CLASS="pin">Position: </SPAN>Position to apply force at.
	<BR>
	<SPAN CLASS="pin">Magnitude: </SPAN>Magnitude of the force/impulse to apply.
	<BR>
	<SPAN CLASS="pin">Radius: </SPAN>The sphere radius in which particles will be affected.
	<BR>
	<SPAN CLASS="pin">Force Mode: </SPAN>The mode to use when applying the force/impulse.
	(see #ForceMode, supported modes are FM_Force, FM_Impulse, FM_Acceleration, FM_VelocityChange).
	<BR>
	
	*/



	proto->SetBehaviorCallbackFct( PClothAddForceAtPosCB );
	proto->DeclareInParameter("Position",CKPGUID_VECTOR);
	proto->DeclareInParameter("Magnitude",CKPGUID_FLOAT);
	proto->DeclareInParameter("Radius",CKPGUID_FLOAT);
	proto->DeclareInParameter("Force Mode",VTE_BODY_FORCE_MODE);

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PClothAddForceAtPos);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PClothAddForceAtPos
// FullName:  PClothAddForceAtPos
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PClothAddForceAtPos(const CKBehaviorContext& behcontext)
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
		//the object : 
		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		if( !target ) return CKBR_OWNERERROR;

		
		//////////////////////////////////////////////////////////////////////////
		pCloth *cloth  = GetPMan()->getCloth(target->GetID());
		if (!cloth)
		{
			beh->ActivateOutput(0);
			return CKBR_PARAMETERERROR;
		}
		
		VxVector position= GetInputParameterValue<VxVector>(beh,bbI_Pos);
		float magnitude= GetInputParameterValue<float>(beh,bbI_Magnitute);
		float radius= GetInputParameterValue<float>(beh,bbI_Radius);
		int forceMode = GetInputParameterValue<int>(beh,bbI_ForceMode);

		cloth->addForceAtPos(position,magnitude,radius,(ForceMode)forceMode);

		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PClothAddForceAtPosCB
// FullName:  PClothAddForceAtPosCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PClothAddForceAtPosCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	
	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
		}
		break;
	}
	return CKBR_OK;

}
