#include "CKAll.h"
CKObjectDeclaration	*FillBehaviorGetCurrentPathDecl();
CKERROR CreateGetCurrentPathProto(CKBehaviorPrototype **pproto);
int GetCurrentPath(const CKBehaviorContext& behcontext);


/************************************************************************/
/*																																			  */
/************************************************************************/

CKObjectDeclaration	*FillBehaviorGetCurrentPathDecl(){
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Get Current Path");	
	od->SetDescription("Add Objects");
	od->SetCategory("Narratives/Files");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x41676403,0x5d3d10c4));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateGetCurrentPathProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}
CKERROR CreateGetCurrentPathProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Get Current Path");
	if(!proto) return CKERR_OUTOFMEMORY;
	proto->DeclareInput("Create Zip File");
	proto->DeclareOutput("Zip File created");
	proto->DeclareOutParameter("Path",CKPGUID_STRING);

	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(GetCurrentPath);
	*pproto = proto;
	return CK_OK;
}

#include <WINDOWS.H>

int GetCurrentPath(const CKBehaviorContext& behcontext)
{

	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	char Ini[MAX_PATH];
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char szPath[MAX_PATH];

	
//	GetModuleFileName(NULL,szPath,_MAX_PATH);
	
	//_splitpath(szPath, drive, dir, NULL, NULL );
		
	//sprintf(Current.Str(),"%s%s",drive,dir);
	
	VxGetCurrentDirectory(szPath);
	CKParameterOut *pout = beh->GetOutputParameter(0);
	pout->SetStringValue(szPath);
	beh->ActivateOutput(0);

	return CKBR_OK;

}

