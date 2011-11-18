#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPClothDestroyDecl();
CKERROR CreatePClothDestroyProto(CKBehaviorPrototype **pproto);
int PClothDestroy(const CKBehaviorContext& behcontext);
CKERROR PClothDestroyCB(const CKBehaviorContext& behcontext);

enum bInputs
{

	bbI_ShapeReference, 

};



//************************************
// Method:    FillBehaviorPClothDestroyDecl
// FullName:  FillBehaviorPClothDestroyDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPClothDestroyDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PClothDestroy");	
	od->SetCategory("Physic/Cloth");
	od->SetDescription("Destroys a physical cloth.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x8d604ec,0x2e926408));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePClothDestroyProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePClothDestroyProto
// FullName:  CreatePClothDestroyProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePClothDestroyProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PClothDestroy");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PClothDestroy  

	PClothDestroy is categorized in \ref Clothes

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Destroys a physical cloth.<br>

	@see pWorld::destroyCloth()

	<h3>Technical Information</h3>

	\image html PClothDestroy.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	
	
	<SPAN CLASS="pin">Target: </SPAN>Target cloth reference.
	<BR>
	<BR>

	*/

	proto->SetBehaviorCallbackFct( PClothDestroyCB );

	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PClothDestroy);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PClothDestroy
// FullName:  PClothDestroy
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PClothDestroy(const CKBehaviorContext& behcontext)
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

		pWorld *world = cloth->getWorld();
		world->destroyCloth(target->GetID());

		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PClothDestroyCB
// FullName:  PClothDestroyCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PClothDestroyCB(const CKBehaviorContext& behcontext)
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