#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPClothDominateVertexDecl();
CKERROR CreatePClothDominateVertexProto(CKBehaviorPrototype **pproto);
int PClothDominateVertex(const CKBehaviorContext& behcontext);
CKERROR PClothDominateVertexCB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_VertexIndex,
	bbI_expireTime,
	bbI_domWeight,

};

//************************************
// Method:    FillBehaviorPClothDominateVertexDecl
// FullName:  FillBehaviorPClothDominateVertexDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPClothDominateVertexDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PClothDominateVertex");	
	od->SetCategory("Physic/Cloth");
	od->SetDescription("Changes the weight of a vertex in the cloth solver for a period of time.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x92979b9,0x4ced0f82));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePClothDominateVertexProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePClothDominateVertexProto
// FullName:  CreatePClothDominateVertexProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePClothDominateVertexProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PClothDominateVertex");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PClothDominateVertex  

	PClothDominateVertex is categorized in \ref Clothes

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Changes the weight of a vertex in the cloth solver for a period of time.<br>

	If this method is called for some vertex, the cloth solver will, during a time
	period of length expirationTime, assign a different weight to the vertex
	while internal cloth constraints (i.e. bending & stretching) are being resolved.

	With a high dominanceWeight, the modified vertex will force neighboring vertices
	to strongly accommodate their positions while its own is kept fairly constant. 
	The reverse holds for smaller dominanceWeights. 

	Using a dominanceWeight of +infinity has a similar effect as temporarily attaching 
	the vertex to a global position. However, unlike using attachments, the velocity 
	of the vertex is kept intact when using this method.

	\note The current implementation will not support the full range of dominanceWeights.
	All dominanceWeights > 0.0 are treated equally as being +infinity.

	\note An expiration time of 0.0 is legal and will result in dominance being
	applied throughout one substep before being discarded immediately.

	\note Having a large number of vertices dominant at once may result in a performance penalty.



	@see pCloth::dominateVertex()


	<h3>Technical Information</h3>

	\image html PClothDominateVertex.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	
	
	<SPAN CLASS="pin">Target: </SPAN>Target cloth reference.
	<BR>
	<BR>

	<SPAN CLASS="pin">Vertex Index: </SPAN>Index of the vertex.
	<BR>
	
	<SPAN CLASS="pin">Expiration Time: </SPAN>Time period where dominance will be active for this vertex.
	<BR>
	<BR>

	<SPAN CLASS="pin">Dominance Weight: </SPAN>Dominance weight for this vertex.
	<BR>
	<BR>

	*/

	proto->SetBehaviorCallbackFct( PClothDominateVertexCB );

	proto->DeclareInParameter("Vertex Index",CKPGUID_INT);
	proto->DeclareInParameter("Expiration Time",CKPGUID_FLOAT);
	proto->DeclareInParameter("Dominance Weight",CKPGUID_FLOAT);


	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PClothDominateVertex);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PClothDominateVertex
// FullName:  PClothDominateVertex
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PClothDominateVertex(const CKBehaviorContext& behcontext)
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
		float expTime = GetInputParameterValue<float>(beh,bbI_expireTime);
		float domWeight  = GetInputParameterValue<float>(beh,bbI_domWeight);
		cloth->dominateVertex(vertexIndex,expTime,domWeight);


		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PClothDominateVertexCB
// FullName:  PClothDominateVertexCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PClothDominateVertexCB(const CKBehaviorContext& behcontext)
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