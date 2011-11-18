#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPClothFreeVertexDecl();
CKERROR CreatePClothFreeVertexProto(CKBehaviorPrototype **pproto);
int PClothFreeVertex(const CKBehaviorContext& behcontext);
CKERROR PClothFreeVertexCB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_VertexIndex,
	
};

//************************************
// Method:    FillBehaviorPClothFreeVertexDecl
// FullName:  FillBehaviorPClothFreeVertexDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPClothFreeVertexDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PClothFreeVertex");	
	od->SetCategory("Physic/Cloth");
	od->SetDescription("Frees a previously attached cloth point.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x718e795d,0x5006d9d));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePClothFreeVertexProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePClothFreeVertexProto
// FullName:  CreatePClothFreeVertexProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePClothFreeVertexProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PClothFreeVertex");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PClothFreeVertex  

	PClothFreeVertex is categorized in \ref Clothes

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Frees a previously attached cloth point.<br>

	@see pCloth::freeVertex()


	<h3>Technical Information</h3>

	\image html PClothFreeVertex.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	
	
	<SPAN CLASS="pin">Target: </SPAN>Target cloth reference.
	<BR>
	<BR>

	<BR>
	<BR>
	<SPAN CLASS="pin">Vertex Index: </SPAN>Index of the vertex to free.
	<BR>
	<BR>

	*/

	proto->SetBehaviorCallbackFct( PClothFreeVertexCB );

	proto->DeclareInParameter("Vertex Index",CKPGUID_INT);

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PClothFreeVertex);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PClothFreeVertex
// FullName:  PClothFreeVertex
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PClothFreeVertex(const CKBehaviorContext& behcontext)
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

		pWorld *world  = GetPMan()->getDefaultWorld();
		if (!world)
		{
			beh->ActivateOutput(0);
			return CKBR_PARAMETERERROR;
		}
		
		pCloth *cloth  = world->getCloth(target);
		if (!cloth)
		{
			beh->ActivateOutput(0);
			return CKBR_PARAMETERERROR;
		}

		int vertexIndex = GetInputParameterValue<int>(beh,bbI_VertexIndex);
		cloth->freeVertex(vertexIndex);

		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PClothFreeVertexCB
// FullName:  PClothFreeVertexCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PClothFreeVertexCB(const CKBehaviorContext& behcontext)
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