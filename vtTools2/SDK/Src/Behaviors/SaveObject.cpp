#include "CKAll.h"

CKObjectDeclaration	*FillBehaviorSaveObjectsDecl();
CKERROR CreateSaveObjectsProto(CKBehaviorPrototype **pproto);
int SaveObjects(const CKBehaviorContext& behcontext);


CKERROR ZipCallBack(const CKBehaviorContext& behcontext);
CKObjectDeclaration	*FillBehaviorSaveObjectsDecl(){
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Save Objects");	
	od->SetDescription("");
	od->SetCategory("Narratives/Files");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x6f12495d,0x1aff17f4));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateSaveObjectsProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}



CKERROR CreateSaveObjectsProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Save Objects");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("Save");
	
	proto->DeclareOutput("Saved");
	proto->DeclareInParameter("Filename", CKPGUID_STRING);
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);

	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)(CKBEHAVIOR_VARIABLEPARAMETERINPUTS));
	proto->SetFunction(SaveObjects);
	*pproto = proto;
	return CK_OK;
}
int SaveObjects(const CKBehaviorContext& behcontext)
{
	
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	
	
	XString filename((CKSTRING) beh->GetInputParameterReadDataPtr(0));

	CKBeObject *beo;
	CKObjectArray* oa = CreateCKObjectArray();
	for (int i = 1  ;i<beh->GetInputParameterCount();i++)
	{
		beo =  (CKBeObject *)beh->GetInputParameterObject(i);
		oa->InsertAt(beo->GetID());
	}
	
	ctx->Save(filename.Str(),oa,0xFFFFFFFF,NULL);
	
	beh->ActivateOutput(0);
	return CKBR_OK;

	return CKBR_OK;
}

