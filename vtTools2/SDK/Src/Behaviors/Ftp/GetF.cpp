#include "CKAll.h"


#include "FTP4W.H"


	
//#pragma comment(lib, "ftp4w32.lib")



CKObjectDeclaration	*FillBehaviorGetFileDecl();
CKERROR CreateGetFileProto(CKBehaviorPrototype **pproto);
int GetFile(const CKBehaviorContext& behcontext);


CKObjectDeclaration	*FillBehaviorGetFileDecl(){
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Get File");	
	od->SetDescription("");
	od->SetCategory("Narratives/Files");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x2e1c1420,0x1ecd1db0));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateGetFileProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}


CKERROR CreateGetFileProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Get File");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("Start Download");
	
	proto->DeclareOutput("Download Started");

	proto->DeclareOutput("Finish");

	proto->DeclareOutput("Error");

	proto->DeclareInParameter("RemoteFile", CKPGUID_STRING);
	proto->DeclareInParameter("LocalFile", CKPGUID_STRING);
	
	proto->DeclareOutParameter("Length in Bytes", CKPGUID_INT);
	proto->DeclareOutParameter("Current Download in Bytes", CKPGUID_INT);
	proto->DeclareOutParameter("Current Download in %", CKPGUID_PERCENTAGE);
	proto->DeclareOutParameter("Error Code", CKPGUID_INT);
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(GetFile);
	*pproto = proto;
	return CK_OK;
}

int GetFile(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;

	int Length=0;

	// Start by In0
	if( beh->IsInputActive(0)){
		beh->ActivateInput(0,FALSE);

		
		XString RemoteFile((CKSTRING) beh->GetInputParameterReadDataPtr(0));
		XString LocalFile((CKSTRING) beh->GetInputParameterReadDataPtr(1));
		
		char *Type = "Type_B";
		int Get = FtpRecvFile (	RemoteFile.Str(),LocalFile.Str(),*Type, FALSE,NULL,NULL);
		Length = FtpBytesToBeTransferred();

		if ( Get !=0 ){
			beh->SetOutputParameterValue(3,&Get);
			beh->ActivateOutput(2);
			return CKBR_OK;
		}

		beh->SetOutputParameterValue(0,&Length);
		beh->ActivateOutput(0);
		return CKBR_ACTIVATENEXTFRAME;
	}


	beh->GetOutputParameterValue(0,&Length);

	int down=FtpBytesTransferred(); 
	beh->SetOutputParameterValue(1,&down);
	float progress=(float)(down*100.0f/Length); // percentage of file downloaded
	progress /=100.0f;
	beh->SetOutputParameterValue(2,&progress);


	
	if ( down == Length){

			beh->ActivateOutput(1);
			return CKBR_OK;
	}

	return CKBR_ACTIVATENEXTFRAME;
	
}
