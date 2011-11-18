#include "pch.h"
#include "CKAll.h"


#include "FTP4W.H"

CKObjectDeclaration	*FillBehaviorSendFileDecl();
CKERROR CreateSendFileProto(CKBehaviorPrototype **pproto);
int SendFile(const CKBehaviorContext& behcontext);


CKObjectDeclaration	*FillBehaviorSendFileDecl(){
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Send File");	
	od->SetDescription("");
	od->SetCategory("Narratives/Files");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x637664ae,0x57df7e83));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateSendFileProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}


CKERROR CreateSendFileProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Send File");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("Start Upload");
	
	proto->DeclareOutput("Upload Started");

	proto->DeclareOutput("Finish");

	proto->DeclareInParameter("LocalFile", CKPGUID_STRING);
	proto->DeclareInParameter("RemoteFile", CKPGUID_STRING);

	
	proto->DeclareOutParameter("Legth", CKPGUID_INT);
	proto->DeclareOutParameter("Current Download in Bytes", CKPGUID_INT);
	proto->DeclareOutParameter("Current Download in %", CKPGUID_PERCENTAGE);
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(SendFile);
	*pproto = proto;
	return CK_OK;
}

int SendFile(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;

	int Length=0;

	// Start by In0
	if( beh->IsInputActive(0)){
		beh->ActivateInput(0,FALSE);

		XString LocalFile((CKSTRING) beh->GetInputParameterReadDataPtr(0));
		XString RemoteFile((CKSTRING) beh->GetInputParameterReadDataPtr(1));
		
		
		char *Type = "Type_B";
		int Send = FtpSendFile(LocalFile.Str(),RemoteFile.Str(),*Type, FALSE,NULL,NULL);

		Length = FtpBytesToBeTransferred();

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
