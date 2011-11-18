#include <StdAfx.h>
#include "pCommon.h"




CKObjectDeclaration	*FillBehaviorPClothAddForceAtVertexDecl();
CKERROR CreatePClothAddForceAtVertexProto(CKBehaviorPrototype **pproto);
int PClothAddForceAtVertex(const CKBehaviorContext& behcontext);
CKERROR PClothAddForceAtVertexCB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_VertexIndex,
	bbI_Force,
	bbI_ForceMode
};



//************************************
// Method:    FillBehaviorPClothAddForceAtVertexDecl
// FullName:  FillBehaviorPClothAddForceAtVertexDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPClothAddForceAtVertexDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PClothAddForceAtVertex");	
	od->SetCategory("Physic/Cloth");
	od->SetDescription("Applies a force (or impulse) defined in the global coordinate frame, to a particular vertex of the cloth");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x506e7b29,0xd3a2720));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePClothAddForceAtVertexProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePClothAddForceAtVertexProto
// FullName:  CreatePClothAddForceAtVertexProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePClothAddForceAtVertexProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PClothAddForceAtVertex");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PClothAddForceAtVertex  

	PClothAddForceAtVertex is categorized in \ref Clothes

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Applies a force (or impulse) defined in the global coordinate frame, to a particular 
	vertex of the cloth.<br>


	Because forces are reset at the end of every timestep, 
	you can maintain a total external force on an object by calling this once every frame.


	@see pCloth::addForceAtVertex()


	<h3>Technical Information</h3>

	\image html PClothAddForceAtVertex.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	
	
	<SPAN CLASS="pin">Target: </SPAN>Target cloth reference.
	<BR>
	<SPAN CLASS="pin">Vertex Index: </SPAN>Number of the vertex to add the force at.
	<BR>
	<SPAN CLASS="pin">Force: </SPAN>Force/impulse to add, defined in the global frame. 
	<BR>
	<SPAN CLASS="pin">Force Mode: </SPAN>The mode to use when applying the force/impulse.
	(see #ForceMode, supported modes are FM_Force, FM_Impulse, FM_Acceleration, FM_VelocityChange)
	<BR>
	
	*/



	proto->SetBehaviorCallbackFct( PClothAddForceAtVertexCB );
	proto->DeclareInParameter("Vertex Index",CKPGUID_INT);
	proto->DeclareInParameter("Force",CKPGUID_VECTOR);
	proto->DeclareInParameter("Force Mode",VTE_BODY_FORCE_MODE);

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PClothAddForceAtVertex);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PClothAddForceAtVertex
// FullName:  PClothAddForceAtVertex
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PClothAddForceAtVertex(const CKBehaviorContext& behcontext)
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
		
		int vertexIndex = GetInputParameterValue<int>(beh,bbI_VertexIndex);
		VxVector force = GetInputParameterValue<VxVector>(beh,bbI_Force);
		int forceMode = GetInputParameterValue<int>(beh,bbI_ForceMode);

		cloth->addForceAtVertex(force,vertexIndex,(ForceMode)forceMode);



		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PClothAddForceAtVertexCB
// FullName:  PClothAddForceAtVertexCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PClothAddForceAtVertexCB(const CKBehaviorContext& behcontext)
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