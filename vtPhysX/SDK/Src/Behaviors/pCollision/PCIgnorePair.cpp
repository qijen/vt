#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPCIgnorePairDecl();
CKERROR CreatePCIgnorePairProto(CKBehaviorPrototype **pproto);
int PCIgnorePair(const CKBehaviorContext& behcontext);
CKERROR PCIgnorePairCB(const CKBehaviorContext& behcontext);


CKObjectDeclaration	*FillBehaviorPCIgnorePairDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PCIgnorePair");	
	od->SetCategory("Physic/Collision");
	od->SetDescription("Enables/Disables collision between two bodies.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x63f648f7,0x13c7097f));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePCIgnorePairProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePCIgnorePairProto
// FullName:  CreatePCIgnorePairProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePCIgnorePairProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PCIgnorePair");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In0");
	proto->DeclareOutput("Out0");

	
	proto->SetBehaviorCallbackFct( PCIgnorePairCB );
	
	proto->DeclareInParameter("Object A",CKPGUID_3DENTITY);
	proto->DeclareInParameter("Object B",CKPGUID_3DENTITY);
	proto->DeclareInParameter("Ignore",CKPGUID_BOOL,"0");



	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PCIgnorePair);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PCIgnorePair
// FullName:  PCIgnorePair
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PCIgnorePair(const CKBehaviorContext& behcontext)
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
		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		if( !target ) return CKBR_OWNERERROR;


		CK3dEntity *targetA = (CK3dEntity *) beh->GetInputParameterObject(0);
		CK3dEntity *targetB = (CK3dEntity *) beh->GetInputParameterObject(1);
		int ignore = GetInputParameterValue<int>(beh,2);
		//////////////////////////////////////////////////////////////////////////
		// the world :  
		pWorld *world=GetPMan()->getWorld(target); 
		
		if(world){
			world->cIgnorePair(targetA,targetB,ignore);
		}
		
		beh->ActivateInput(0,FALSE);
		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PCIgnorePairCB
// FullName:  PCIgnorePairCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PCIgnorePairCB(const CKBehaviorContext& behcontext)
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

