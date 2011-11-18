#include "CKAll.h"

CKObjectDeclaration	*FillBehaviorExecBBDecl();
CKERROR CreateExecBBProto(CKBehaviorPrototype **pproto);
int ExecBB(const CKBehaviorContext& behcontext);


CKERROR ZipCallBack(const CKBehaviorContext& behcontext);
CKObjectDeclaration	*FillBehaviorExecBBDecl(){
	CKObjectDeclaration *od = CreateCKObjectDeclaration("ExecBB");	
	od->SetDescription("");
	od->SetCategory("Narratives/Files");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x6b62506d,0x3dd1067));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateExecBBProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}



CKERROR CreateExecBBProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("ExecBB");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("Execute");
	
	proto->DeclareOutput("Ok");
	proto->DeclareOutput("Failed");
	
	proto->DeclareInParameter("Filename", CKPGUID_STRING);
	proto->DeclareInParameter("Directory", CKPGUID_STRING);

	proto->DeclareOutParameter("last error", CKPGUID_INT);

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);

	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)(CKBEHAVIOR_VARIABLEPARAMETERINPUTS));
	proto->SetFunction(ExecBB);
	*pproto = proto;
	return CK_OK;
}

#include <windows.h>
	
BOOL RunAndForgetProcess(char *pCmdLine,
                         char *pRunningDir,
                         int *nRetValue)
{
	int nError;
	STARTUPINFO stInfo;
	PROCESS_INFORMATION prInfo;
	BOOL bResult;
	ZeroMemory( &stInfo, sizeof(stInfo) );
	stInfo.cb = sizeof(stInfo);
	stInfo.dwFlags=STARTF_USESHOWWINDOW;
	stInfo.wShowWindow=SW_SHOW;
	bResult = CreateProcess(NULL,
						 (LPSTR)pCmdLine,
						 NULL,
						 NULL,
						 TRUE,
						 CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP
//						 DETACHED_PROCESS
						 | NORMAL_PRIORITY_CLASS ,
						 NULL,
						 (LPSTR)pRunningDir ,
						 &stInfo,
						 &prInfo);
	*nRetValue = nError = GetLastError();
	// Don't write these two lines if you need
	CloseHandle(prInfo.hThread);
	// to use these handles
	CloseHandle(prInfo.hProcess);
	if (!bResult) return FALSE;
		return TRUE;
}
int ExecBB(const CKBehaviorContext& behcontext)
{
	
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	
	
	char* filename = ((CKSTRING) beh->GetInputParameterReadDataPtr(0));
	char* dirname = ((CKSTRING) beh->GetInputParameterReadDataPtr(1)) ;


	if (!strlen(dirname))
		dirname = NULL;//argh....... ?

	int ret  = 0;

	bool res  = RunAndForgetProcess(filename,dirname,&ret);
	
	
	beh->SetOutputParameterValue(0,&ret);
	
	if (res)
		beh->ActivateOutput(0);
	else 
		beh->ActivateOutput(1);

	
	return CKBR_OK;
}

