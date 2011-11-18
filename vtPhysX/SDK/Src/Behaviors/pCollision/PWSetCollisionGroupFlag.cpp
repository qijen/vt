#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPWSetCollisionGroupFlagDecl();
CKERROR CreatePWSetCollisionGroupFlagProto(CKBehaviorPrototype **pproto);
int PWSetCollisionGroupFlag(const CKBehaviorContext& behcontext);
CKERROR PWSetCollisionGroupFlagCB(const CKBehaviorContext& behcontext);


CKObjectDeclaration	*FillBehaviorPWSetCollisionGroupFlagDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PWSetCollisionGroupFlag");	
	od->SetCategory("Physic/Collision");
	od->SetDescription("Enables collision between two collisions groups.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x44961cc7,0x2d104ced));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePWSetCollisionGroupFlagProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePWSetCollisionGroupFlagProto
// FullName:  CreatePWSetCollisionGroupFlagProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePWSetCollisionGroupFlagProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PWSetCollisionGroupFlag");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	
	proto->SetBehaviorCallbackFct( PWSetCollisionGroupFlagCB );
	
	proto->DeclareInParameter("Collisions Group A",CKPGUID_INT,"0");
	proto->DeclareInParameter("Collisions Group A",CKPGUID_INT,"0");
	proto->DeclareInParameter("Enabled",CKPGUID_BOOL,"0");


	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PWSetCollisionGroupFlag);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PWSetCollisionGroupFlag
// FullName:  PWSetCollisionGroupFlag
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PWSetCollisionGroupFlag(const CKBehaviorContext& behcontext)
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


		//////////////////////////////////////////////////////////////////////////
		// the world :  
		pWorld *world=GetPMan()->getWorld(target); 
		int collGA = GetInputParameterValue<int>(beh,0);
		int collGB = GetInputParameterValue<int>(beh,1);
		int value = GetInputParameterValue<int>(beh,2);
		if(world)
			world->cSetGroupCollisionFlag(collGA,collGB,value);
		
		beh->ActivateInput(0,FALSE);
		beh->ActivateOutput(0);
	}
	return 0;
}

//************************************
// Method:    PWSetCollisionGroupFlagCB
// FullName:  PWSetCollisionGroupFlagCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PWSetCollisionGroupFlagCB(const CKBehaviorContext& behcontext)
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

