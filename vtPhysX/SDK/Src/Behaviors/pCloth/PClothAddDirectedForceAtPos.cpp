#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPClothAddDirectedForceAtPosDecl();
CKERROR CreatePClothAddDirectedForceAtPosProto(CKBehaviorPrototype **pproto);
int PClothAddDirectedForceAtPos(const CKBehaviorContext& behcontext);
CKERROR PClothAddDirectedForceAtPosCB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_Pos,
	bbI_Force,
	bbI_Radius,
	bbI_ForceMode,
};



//************************************
// Method:    FillBehaviorPClothAddDirectedForceAtPosDecl
// FullName:  FillBehaviorPClothAddDirectedForceAtPosDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPClothAddDirectedForceAtPosDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PClothAddDirectedForceAtPos");	
	od->SetCategory("Physic/Cloth");
	od->SetDescription("Applies a directed force (or impulse) at a particular position. All vertices within radius will be affected with a quadratic drop-off. ");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x439b04ff,0x252950fc));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePClothAddDirectedForceAtPosProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePClothAddDirectedForceAtPosProto
// FullName:  CreatePClothAddDirectedForceAtPosProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePClothAddDirectedForceAtPosProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PClothAddDirectedForceAtPos");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PClothAddDirectedForceAtPos  

	PClothAddDirectedForceAtPos is categorized in \ref Clothes

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Applies a directed force (or impulse) at a particular position. All vertices
	within radius will be affected with a quadratic drop-off. <br>

	Because forces are reset at the end of every timestep, 
	you can maintain a total external force on an object by calling this once every frame.


	@see pCloth::addDirectedForceAtPos()


	<h3>Technical Information</h3>

	\image html PClothAddDirectedForceAtPos.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	
	
	<SPAN CLASS="pin">Target: </SPAN>Target cloth reference.
	<BR>
	<SPAN CLASS="pin">Position: </SPAN>Position to apply force at.
	<BR>
	<SPAN CLASS="pin">Force: </SPAN>Force to apply.
	<BR>
	<SPAN CLASS="pin">Magnitude: </SPAN>Magnitude of the force/impulse to apply.
	<BR>
	<SPAN CLASS="pin">Radius: </SPAN>The sphere radius in which particles will be affected.
	<BR>
	<SPAN CLASS="pin">Force Mode: </SPAN>The mode to use when applying the force/impulse.
	(see #ForceMode, supported modes are FM_Force, FM_Impulse, FM_Acceleration, FM_VelocityChange).
	<BR>
	
	*/



	proto->SetBehaviorCallbackFct( PClothAddDirectedForceAtPosCB );
	proto->DeclareInParameter("Position",CKPGUID_VECTOR);
	proto->DeclareInParameter("Magnitude",CKPGUID_FLOAT);
	proto->DeclareInParameter("Radius",CKPGUID_FLOAT);
	proto->DeclareInParameter("Force Mode",VTE_BODY_FORCE_MODE);

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PClothAddDirectedForceAtPos);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PClothAddDirectedForceAtPos
// FullName:  PClothAddDirectedForceAtPos
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PClothAddDirectedForceAtPos(const CKBehaviorContext& behcontext)
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
		VxVector force = GetInputParameterValue<VxVector>(beh,bbI_Force);
		float radius= GetInputParameterValue<float>(beh,bbI_Radius);
		int forceMode = GetInputParameterValue<int>(beh,bbI_ForceMode);

		cloth->addDirectedForceAtPos(position,force,radius,(ForceMode)forceMode);


		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PClothAddDirectedForceAtPosCB
// FullName:  PClothAddDirectedForceAtPosCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PClothAddDirectedForceAtPosCB(const CKBehaviorContext& behcontext)
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