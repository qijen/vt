#include "CKAll.h"
#include <windows.h>
#include <iostream>
#include "Shlwapi.h"
#pragma comment (lib,"SHLWAPI.LIB")


CKObjectDeclaration	*FillBehaviorGetLastFileNameDecl();
CKERROR CreateGetLastFileNameProto(CKBehaviorPrototype **pproto);
int GetLastFileName(const CKBehaviorContext& behcontext);


/************************************************************************/
/*																																			  */
/************************************************************************/

CKObjectDeclaration	*FillBehaviorGetLastFileNameDecl(){
	CKObjectDeclaration *od = CreateCKObjectDeclaration("GetLastFileName");	
	od->SetDescription("Add Objects");
	od->SetCategory("Narratives/Objects");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0xa2e4a61,0x170735ce));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateGetLastFileNameProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}
CKERROR CreateGetLastFileNameProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("GetLastFileName");
	if(!proto) return CKERR_OUTOFMEMORY;
	proto->DeclareInput("Create Zip File");
	proto->DeclareOutput("Zip File created");
	
	
	proto->DeclareOutParameter("FilePath",CKPGUID_STRING);
	proto->DeclareOutParameter("Path",CKPGUID_STRING);
	

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(GetLastFileName);
	*pproto = proto;
	return CK_OK;
}


int GetLastFileName(const CKBehaviorContext& behcontext)
{

	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;

	XString Inpath (ctx->GetLastCmoLoaded());


		
	CKParameterOut *pout = beh->GetOutputParameter(0);
	pout->SetStringValue(Inpath.Str());
	
	PathRemoveFileSpec(Inpath.Str());

	
	CKParameterOut *pout2 = beh->GetOutputParameter(1);
	pout2->SetStringValue(Inpath.Str());
	
	

	beh->ActivateOutput(0);
	

return 0;
	
}