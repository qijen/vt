#include <StdAfx.h>
#include "pCommon.h"

CKObjectDeclaration	*FillBehaviorPClothAttachToShapeDecl();
CKERROR CreatePClothAttachToShapeProto(CKBehaviorPrototype **pproto);
int PClothAttachToShape(const CKBehaviorContext& behcontext);
CKERROR PClothAttachToShapeCB(const CKBehaviorContext& behcontext);

enum bInputs
{

	bbI_ShapeReference, 
	bbI_AttachmentFlags,
	bbI_AttachToColliding,
};


enum bSettings
{
	bbS_USE_DEFAULT_WORLD,
	bbS_ADD_ATTRIBUTES
};

//************************************
// Method:    FillBehaviorPClothAttachToShapeDecl
// FullName:  FillBehaviorPClothAttachToShapeDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPClothAttachToShapeDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PClothAttachToShape");	
	od->SetCategory("Physic/Cloth");
	od->SetDescription("Attaches a cloth to a shape.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x2ca5453a,0x7dc1598f));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePClothAttachToShapeProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePClothAttachToShapeProto
// FullName:  CreatePClothAttachToShapeProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePClothAttachToShapeProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PClothAttachToShape");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PClothAttachToShape  

	PClothAttachToShape is categorized in \ref Clothes

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Attaches a cloth to a shape .<br>

	<h3>Technical Information</h3>

	\image html PClothAttachToShape.png

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
	<SPAN CLASS="pin">Attach to colliding shapes: </SPAN>Attaches the cloth to all shapes, currently colliding. 

	This method only works with primitive and convex shapes. Since the inside of a general 
	triangle mesh is not clearly defined.

	@see pCloth::attachToCollidingShapes()

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



	proto->SetBehaviorCallbackFct( PClothAttachToShapeCB );

	proto->DeclareInParameter("Shape Reference",CKPGUID_3DENTITY);
	proto->DeclareInParameter("Attachment Flags",VTE_CLOTH_ATTACH_FLAGS);
	proto->DeclareInParameter("Attach to colliding shapes",CKPGUID_BOOL);

	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PClothAttachToShape);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PClothAttachToShape
// FullName:  PClothAttachToShape
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PClothAttachToShape(const CKBehaviorContext& behcontext)
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
		int attachToColliders = GetInputParameterValue<int>(beh,bbI_AttachToColliding);

		NxShape *shape  = world->getShapeByEntityID(shapeReference->GetID());
		if(shape)
		{
			cloth->attachToShape((CKBeObject*)shapeReference,flags);
		}


		if (attachToColliders)
		{
			cloth->attachToCollidingShapes(flags);
		}

		
		beh->ActivateOutput(0);


		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PClothAttachToShapeCB
// FullName:  PClothAttachToShapeCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PClothAttachToShapeCB(const CKBehaviorContext& behcontext)
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