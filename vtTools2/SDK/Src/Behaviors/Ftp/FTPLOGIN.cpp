#include "pch.h"
#include "CKAll.h"

#include "FTP4W.H"


CKObjectDeclaration	*FillBehaviorFTPLoginDecl();
CKERROR CreateFTPLoginProto(CKBehaviorPrototype **pproto);
int FTPLogin(const CKBehaviorContext& behcontext);


CKObjectDeclaration	*FillBehaviorFTPLoginDecl(){
	CKObjectDeclaration *od = CreateCKObjectDeclaration("FTP Login");	
	od->SetDescription("");
	od->SetCategory("Narratives/Files");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x3d100c46,0x206c6bc2));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateFTPLoginProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}


CKERROR CreateFTPLoginProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("FTP Login");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("Login");
	proto->DeclareInput("Logout");

	proto->DeclareOutput("Login Exit");
	proto->DeclareOutput("Logout Exit");
	proto->DeclareOutput("Error");

	proto->DeclareInParameter("Host", CKPGUID_STRING,"127.0.0.1");
	proto->DeclareInParameter("User", CKPGUID_STRING,"ich");
	proto->DeclareInParameter("Password", CKPGUID_STRING,"ich");
	proto->DeclareInParameter("Port", CKPGUID_INT,"21");
	

	proto->DeclareOutParameter("Error Code 0=ok", CKPGUID_INT,"0");
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(FTPLogin);
	*pproto = proto;
	return CK_OK;
}
#define  LOG_FILE       "c:\\ftp4w.log"

int FTPLogin(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;

		if( beh->IsInputActive(0)){
			beh->ActivateInput(0,FALSE);
	
			HWND win = (HWND)ctx->GetMainWindow();
			FtpInit(win);
			//HFILE hLogFile =  _lcreat (LOG_FILE, 0);
			//FtpLogTo (hLogFile);
			
			
	
			FtpSetDefaultTimeOut (30);
			FtpSetPassiveMode(TRUE);
			FtpSetAsynchronousMode();
	
			int Port;
			beh->GetInputParameterValue(3,&Port);
			


			XString Host((CKSTRING) beh->GetInputParameterReadDataPtr(0));
			XString User((CKSTRING) beh->GetInputParameterReadDataPtr(1));
			XString Pw((CKSTRING) beh->GetInputParameterReadDataPtr(2));
			
			int Login = FtpLogin(Host.Str(),User.Str(),Pw.Str(),win,0);
			beh->SetOutputParameterValue(0,&Login);
			

			if (Login == 0)beh->ActivateOutput(0);
				else{
					beh->ActivateOutput(2);
					return CKBR_OK;
				}
				return CKBR_ACTIVATENEXTFRAME;
			}

		if( beh->IsInputActive(1)){
			beh->ActivateInput(1,FALSE);

			FtpCloseConnection();
			FtpRelease ();
			beh->ActivateOutput(1);
			return CKBR_OK;
		}
	return CKBR_ACTIVATENEXTFRAME;
}
