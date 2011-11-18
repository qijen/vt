#include "CKAll.h"
#include <windows.h>


CKObjectDeclaration	*FillBehaviorCopyFileBBDecl();
CKERROR CreateCopyFileBBProto(CKBehaviorPrototype **pproto);
int CopyFileBB(const CKBehaviorContext& behcontext);


CKERROR ZipCallBack(const CKBehaviorContext& behcontext);
CKObjectDeclaration	*FillBehaviorCopyFileBBDecl(){
	CKObjectDeclaration *od = CreateCKObjectDeclaration("File Copy");	
	od->SetDescription("");
	od->SetCategory("Narratives/Files");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x149e1e15,0x23536f5a));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateCopyFileBBProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}



CKERROR CreateCopyFileBBProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("File Copy");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("start");
	
	proto->DeclareOutput("end");
	proto->DeclareOutput("error");

	proto->DeclareInParameter("source", CKPGUID_STRING);
	proto->DeclareInParameter("target", CKPGUID_STRING);


	proto->DeclareInParameter("overwrite", CKPGUID_BOOL);

	proto->DeclareOutParameter("overwrite", CKPGUID_STRING);




	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);

	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)(CKBEHAVIOR_VARIABLEPARAMETERINPUTS));
	proto->SetFunction(CopyFileBB);
	*pproto = proto;
	return CK_OK;
}

int CopyFileBB(const CKBehaviorContext& behcontext)
{
	
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;

	XString sfilename((CKSTRING) beh->GetInputParameterReadDataPtr(0));
	XString tfilename((CKSTRING) beh->GetInputParameterReadDataPtr(1));

//	XPath mdirs( sfilename );


	bool ow;
	beh->GetInputParameterValue(2,&ow);

	

	ow=!ow;

	CopyFile(sfilename.Str() , tfilename.Str() ,ow );
	beh->ActivateOutput(0);


	return CKBR_OK;
}

