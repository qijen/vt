#include "CKAll.h"
CKObjectDeclaration	*FillBehaviorSetMousPosDecl();

CKERROR CreateSetMousPosProto(CKBehaviorPrototype **pproto);
int SetMousPos(const CKBehaviorContext& behcontext);


CKObjectDeclaration	*FillBehaviorSetMousPosDecl(){
	CKObjectDeclaration *od = CreateCKObjectDeclaration("SetMousePos");	
	od->SetCategory("Narratives/Mouse");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x7938083c,0x32f03a3d));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateSetMousPosProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}
CKERROR CreateSetMousPosProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("SetMousePos");
	if(!proto) return CKERR_OUTOFMEMORY;
	proto->DeclareInput("Create Zip File");

	
	proto->DeclareOutput("Zip File created");

	proto->DeclareInParameter("IsServer",CKPGUID_FLOAT);
	proto->DeclareInParameter("IsServer",CKPGUID_FLOAT);
		
	proto->DeclareOutParameter("UP",CKPGUID_INT);
	
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(SetMousPos);
	*pproto = proto;
	return CK_OK;
}

#include <Windows.h>

int SetMousPos(const CKBehaviorContext& behcontext)
{
	

	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;

	if( beh->IsInputActive(0) ){
		beh->ActivateInput(0,FALSE);

 DWORD data = 0;
 DWORD flags = MOUSEEVENTF_ABSOLUTE;
 
 // - Has the pointer moved since the last event?
 
 
 flags |= MOUSEEVENTF_MOVE;

 mouse_event(flags, 100,100, data, 0);
 

		beh->ActivateOutput(0);
	

	}

	return 0;

}
