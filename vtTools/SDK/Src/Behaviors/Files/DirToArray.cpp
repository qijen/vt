#include "CKAll.h"

CKObjectDeclaration	*FillBehaviorDirToArrayDecl();
CKERROR CreateDirToArrayProto(CKBehaviorPrototype **pproto);
int DirToArray(const CKBehaviorContext& behcontext);


CKObjectDeclaration	*FillBehaviorDirToArrayDecl(){
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Dir to Array");	
	od->SetDescription("");
	od->SetCategory("Narratives/Files");
	
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x4be0703f,0x208b5a7f));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateDirToArrayProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}


CKERROR CreateDirToArrayProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Dir to Array");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("Fill");
	proto->DeclareInput("Loop In");

	proto->DeclareOutput("Reseted");
	proto->DeclareOutput("Loop Out");

	proto->DeclareInParameter("Directory", CKPGUID_STRING,"0");
	proto->DeclareInParameter("Mask", CKPGUID_STRING,"0");
	proto->DeclareInParameter("Recursive", CKPGUID_BOOL,"0");


	
	proto->DeclareOutParameter("entry", CKPGUID_STRING);
	proto->DeclareOutParameter("counter", CKPGUID_INT);

	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(DirToArray);
	*pproto = proto;
	return CK_OK;
}




#include <vector>
std::vector<XString>flist;


int counter = 0 ;

int DirToArray(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;

	XString filename((CKSTRING) beh->GetInputParameterReadDataPtr(0));
	XString Mask((CKSTRING) beh->GetInputParameterReadDataPtr(1));
		
	int rec;

	beh->GetInputParameterValue(2,&rec);

	

	if( beh->IsInputActive(0) ){
		beh->ActivateInput(0,FALSE);

		flist.erase(flist.begin(),flist.end());

		CKDirectoryParser MyParser(filename.Str(),Mask.Str(),rec);
		char* str = NULL;
		while(str = MyParser.GetNextFile())
			flist.push_back(XString(str));

		counter = 0;
		beh->ActivateInput(1,TRUE);

		
	}


	if( beh->IsInputActive(1) ){
		beh->ActivateInput(1,FALSE);

		if ( counter < flist.size() ){
			XString entry = flist.at(counter);
			CKParameterOut * pout = beh->GetOutputParameter(0);
			pout->SetStringValue(entry.Str() );
				
			counter++;
			beh->SetOutputParameterValue(1,&counter);
			beh->ActivateOutput(1);

		}else{

			beh->SetOutputParameterValue(1,&counter);
			counter = 0 ; 
			beh->ActivateOutput(0);
		}
	}
	return 0;
}
