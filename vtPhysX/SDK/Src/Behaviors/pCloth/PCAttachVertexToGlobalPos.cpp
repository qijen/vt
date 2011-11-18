#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPClothAttachVertexToPositionDecl();
CKERROR CreatePClothAttachVertexToPositionProto(CKBehaviorPrototype **pproto);
int PClothAttachVertexToPosition(const CKBehaviorContext& behcontext);
CKERROR PClothAttachVertexToPositionCB(const CKBehaviorContext& behcontext);

enum bInputs
{

	bbI_VertexIndex,
	bbI_GlobalPosition
};

//************************************
// Method:    FillBehaviorPClothAttachVertexToPositionDecl
// FullName:  FillBehaviorPClothAttachVertexToPositionDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPClothAttachVertexToPositionDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PClothAttachVertexToPosition");	
	od->SetCategory("Physic/Cloth");
	od->SetDescription("Attaches a cloth vertex to a position in world space");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x6f502320,0x48065a45));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePClothAttachVertexToPositionProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePClothAttachVertexToPositionProto
// FullName:  CreatePClothAttachVertexToPositionProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePClothAttachVertexToPositionProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PClothAttachVertexToPosition");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PClothAttachVertexToPosition  

	PClothAttachVertexToPosition is categorized in \ref Clothes

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Attaches a cloth vertex to a position in world space.<br>

	@see pCloth::attachVertexToGlobalPosition()


	<h3>Technical Information</h3>

	\image html PClothAttachVertexToPosition.png

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
	<SPAN CLASS="pin">Vertex Index: </SPAN>Index of the vertex to attach.
	<BR>
	<BR>

	<SPAN CLASS="pin">Global Position: </SPAN>TThe position in world space.
	<BR>
	<BR>

	*/

	proto->SetBehaviorCallbackFct( PClothAttachVertexToPositionCB );

	proto->DeclareInParameter("Vertex Index",CKPGUID_INT);
	proto->DeclareInParameter("Global Position",CKPGUID_VECTOR);


	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PClothAttachVertexToPosition);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PClothAttachVertexToPosition
// FullName:  PClothAttachVertexToPosition
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PClothAttachVertexToPosition(const CKBehaviorContext& behcontext)
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
		VxVector localPosition = GetInputParameterValue<VxVector>(beh,bbI_GlobalPosition);

		cloth->attachVertexToGlobalPosition(vertexIndex,localPosition);

		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PClothAttachVertexToPositionCB
// FullName:  PClothAttachVertexToPositionCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PClothAttachVertexToPositionCB(const CKBehaviorContext& behcontext)
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