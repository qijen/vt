#include <StdAfx.h>
#include "pCommon.h"



CKObjectDeclaration	*FillBehaviorPBDestroyDecl();
CKERROR CreatePBDestroyProto(CKBehaviorPrototype **pproto);
int PBDestroy(const CKBehaviorContext& behcontext);
CKERROR PBDestroyCB(const CKBehaviorContext& behcontext);


//************************************
// Method:    FillBehaviorPBDestroyDecl
// FullName:  FillBehaviorPBDestroyDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPBDestroyDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBDestroy");	
	od->SetCategory("Physic/Body");
	od->SetDescription("Removes an entity from the physic engine.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x72519cc,0x1f2d16da));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBDestroyProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePBDestroyProto
// FullName:  CreatePBDestroyProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePBDestroyProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBDestroy");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In0");
	proto->DeclareOutput("Out0");

	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	proto->SetBehaviorCallbackFct( PBDestroyCB );
	proto->SetFunction(PBDestroy);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PBDestroy
// FullName:  PBDestroy
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PBDestroy(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;
	
	if( beh->IsInputActive(0) )
	{

		//////////////////////////////////////////////////////////////////////////
		//the object : 
		CK3dEntity *referenceObject = (CK3dEntity *) beh->GetTarget();
		if( !referenceObject ) return CKBR_OWNERERROR;

		pWorld *world=GetPMan()->getWorldByBody(referenceObject); 
		if (world)
		{

			pRigidBody*bodyA= world->getBody(referenceObject);
			if (bodyA)
			{
				world->deleteBody(bodyA);
			}
		}
		beh->ActivateInput(0,FALSE);
		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PBDestroyCB
// FullName:  PBDestroyCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PBDestroyCB(const CKBehaviorContext& behcontext)
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

