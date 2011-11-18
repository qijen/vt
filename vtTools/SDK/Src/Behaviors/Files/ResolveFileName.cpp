#include "CKAll.h"
CKObjectDeclaration	*FillBehaviorResolveFileNameDecl();
CKERROR CreateResolveFileNameProto(CKBehaviorPrototype **pproto);
int ResolveFileName(const CKBehaviorContext& behcontext);


/************************************************************************/
/*																																			  */
/************************************************************************/

CKObjectDeclaration	*FillBehaviorResolveFileNameDecl(){
	CKObjectDeclaration *od = CreateCKObjectDeclaration("ResolveFileName");	
	od->SetDescription("Add Objects");
	od->SetCategory("Narratives/Files");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x55d13c2b,0xe030cbf));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateResolveFileNameProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}
CKERROR CreateResolveFileNameProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("ResolveFileName");
	if(!proto) return CKERR_OUTOFMEMORY;
	proto->DeclareInput("Create Zip File");
	proto->DeclareOutput("Zip File created");
	
	proto->DeclareInParameter("FileName",CKPGUID_STRING);
	proto->DeclareOutParameter("Path",CKPGUID_STRING);
	

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(ResolveFileName);
	*pproto = proto;
	return CK_OK;
}


int ResolveFileName(const CKBehaviorContext& behcontext)
{

	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;

	XString Inpath((CKSTRING) beh->GetInputParameterReadDataPtr(0));

	behcontext.Context->GetPathManager()->ResolveFileName(Inpath,BITMAP_PATH_IDX,-1);
	
//	Inpath =behcontext.Context->GetPathManager()->GetVirtoolsTemporaryFolder();


	CKParameterOut *pout = beh->GetOutputParameter(0);
	pout->SetStringValue(Inpath.Str());
	
	

	beh->ActivateOutput(0);

	return CKBR_OK;

}


