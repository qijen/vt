#include <StdAfx.h>
#include "pCommon.h"

CKObjectDeclaration	*FillBehaviorPClothAttachVertexToShapeDecl();
CKERROR CreatePClothAttachVertexToShapeProto(CKBehaviorPrototype **pproto);
int PClothAttachVertexToShape(const CKBehaviorContext& behcontext);
CKERROR PClothAttachVertexToShapeCB(const CKBehaviorContext& behcontext);

enum bInputs
{

	bbI_ShapeReference, 
	bbI_AttachmentFlags,
	bbI_VertexIndex,
	bbI_LocalPosition
};


enum bSettings
{
	bbS_USE_DEFAULT_WORLD,
	bbS_ADD_ATTRIBUTES
};

//************************************
// Method:    FillBehaviorPClothAttachVertexToShapeDecl
// FullName:  FillBehaviorPClothAttachVertexToShapeDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPClothAttachVertexToShapeDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PClothAttachVertexToShape");	
	od->SetCategory("Physic/Cloth");
	od->SetDescription("Attaches a cloth vertex to another shape.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x27970ee2,0x1247425d));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePClothAttachVertexToShapeProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePClothAttachVertexToShapeProto
// FullName:  CreatePClothAttachVertexToShapeProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePClothAttachVertexToShapeProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PClothAttachVertexToShape");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PClothAttachVertexToShape  

	PClothAttachVertexToShape is categorized in \ref Clothes

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Attaches a cloth vertex to a shape .<br>

	@see pCloth::attachVertexToShape()


	<h3>Technical Information</h3>

	\image html PClothAttachVertexToShape.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	
	
	<SPAN CLASS="pin">Target: </SPAN>Target cloth reference.
	<BR>
	<BR>

	<SPAN CLASS="pin">Shape Reference: </SPAN>Shape to which the cloth should be attached to.

	@see pCloth::attachToShape()
	<BR>
	<BR>

	<SPAN CLASS="pin">Attachment Flags: </SPAN>One or two way interaction, tearable or non-tearable

	<b>Default:</b>  PCAF_ClothAttachmentTwoway

	@see pClothAttachmentFlag
	<BR>
	<BR>

	<BR>
	<BR>
	<SPAN CLASS="pin">Vertex Index: </SPAN>Index of the vertex to attach.
	<BR>
	<BR>

	<SPAN CLASS="pin">Local Position: </SPAN>The position relative to the pose of the shape.
	<BR>
	<BR>



	
	<br>
	<h3>Note</h3>
	Is utilizing #pRigidBody #pWorld #PhysicManager.<br>

	<h3>VSL</h3><br>
	<SPAN CLASS="NiceCode">
	\include pCloth.cpp
	</SPAN>
	*/



	proto->SetBehaviorCallbackFct( PClothAttachVertexToShapeCB );

	proto->DeclareInParameter("Shape Reference",CKPGUID_3DENTITY);
	proto->DeclareInParameter("Attachment Flags",VTE_CLOTH_ATTACH_FLAGS);
	proto->DeclareInParameter("Vertex Index",CKPGUID_INT);
	proto->DeclareInParameter("Local Position",CKPGUID_VECTOR);


	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PClothAttachVertexToShape);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PClothAttachVertexToShape
// FullName:  PClothAttachVertexToShape
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PClothAttachVertexToShape(const CKBehaviorContext& behcontext)
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

		CK3dEntity*shapeReference  = (CK3dEntity *) beh->GetInputParameterObject(bbI_ShapeReference);
		if (!shapeReference)
		{
			beh->ActivateOutput(0);
			return CKBR_PARAMETERERROR;
		}

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
		
		

		int flags = GetInputParameterValue<int>(beh,bbI_AttachmentFlags);
		int vertexIndex = GetInputParameterValue<int>(beh,bbI_VertexIndex);
		VxVector localPosition = GetInputParameterValue<VxVector>(beh,bbI_LocalPosition);


		NxShape *shape  = world->getShapeByEntityID(shapeReference->GetID());
		if(shape)
		{
			cloth->attachVertexToShape(vertexIndex,(CKBeObject*)shapeReference,localPosition,flags);
		}
		
		beh->ActivateOutput(0);


		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PClothAttachVertexToShapeCB
// FullName:  PClothAttachVertexToShapeCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PClothAttachVertexToShapeCB(const CKBehaviorContext& behcontext)
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

