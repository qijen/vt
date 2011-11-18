#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPClothDetachFromShapeDecl();
CKERROR CreatePClothDetachFromShapeProto(CKBehaviorPrototype **pproto);
int PClothDetachFromShape(const CKBehaviorContext& behcontext);
CKERROR PClothDetachFromShapeCB(const CKBehaviorContext& behcontext);

enum bInputs
{

	bbI_ShapeReference, 

};



//************************************
// Method:    FillBehaviorPClothDetachFromShapeDecl
// FullName:  FillBehaviorPClothDetachFromShapeDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPClothDetachFromShapeDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PClothDetachFromShape");	
	od->SetCategory("Physic/Cloth");
	od->SetDescription("Detaches the cloth from a shape it has been attached to before. ");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0xf4b0609,0x75b453a3));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePClothDetachFromShapeProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePClothDetachFromShapeProto
// FullName:  CreatePClothDetachFromShapeProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePClothDetachFromShapeProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PClothDetachFromShape");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PClothDetachFromShape  

	PClothDetachFromShape is categorized in \ref Clothes

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Detaches the cloth from a shape it has been attached to before<br>

	@see pCloth::detachFromShape()

	<h3>Technical Information</h3>

	\image html PClothDetachFromShape.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	
	
	<SPAN CLASS="pin">Target: </SPAN>Target cloth reference.
	<BR>
	<BR>

	<SPAN CLASS="pin">Shape Reference: </SPAN>Shape to which the cloth should be attached to.

	
	<BR>
	<BR>

	<SPAN CLASS="pin">Attachment Flags: </SPAN>One or two way interaction, tearable or non-tearable
	*/

	proto->SetBehaviorCallbackFct( PClothDetachFromShapeCB );
	proto->DeclareInParameter("Shape Reference",CKPGUID_3DENTITY);


	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PClothDetachFromShape);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PClothDetachFromShape
// FullName:  PClothDetachFromShape
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PClothDetachFromShape(const CKBehaviorContext& behcontext)
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

		pCloth *cloth  = GetPMan()->getCloth(target->GetID());
		if (!cloth)
		{
			beh->ActivateOutput(0);
			return CKBR_PARAMETERERROR;
		}

		
		NxShape *shape  = cloth->getWorld()->getShapeByEntityID(shapeReference->GetID());
		if(shape)
		{
			cloth->detachFromShape((CKBeObject*)shapeReference);
		}

		
		beh->ActivateOutput(0);


		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PClothDetachFromShapeCB
// FullName:  PClothDetachFromShapeCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PClothDetachFromShapeCB(const CKBehaviorContext& behcontext)
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
