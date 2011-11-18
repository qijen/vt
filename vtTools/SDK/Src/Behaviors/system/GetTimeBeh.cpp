#include "CKAll.h"

CKObjectDeclaration	*FillBehaviorGetTime2Decl();
CKERROR CreateGetTime2Proto(CKBehaviorPrototype **pproto);
int GetTime2(const CKBehaviorContext& behcontext);

/************************************************************************/
/*                                                                      */
/************************************************************************/

CKObjectDeclaration	*FillBehaviorGetTime2Decl(){
	CKObjectDeclaration *od = CreateCKObjectDeclaration("GetTime");	
	od->SetDescription("Logics/Tools");
	od->SetCategory("a");
	
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x33ce78ad,0x6e4a5d37));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateGetTime2Proto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}
CKERROR CreateGetTime2Proto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("GetTime");
	if(!proto) return CKERR_OUTOFMEMORY;
	proto->DeclareInput("Create Zip File");

	proto->DeclareOutput("Zip File created");
	
	
	proto->DeclareOutParameter("Hour", CKPGUID_INT);
	proto->DeclareOutParameter("Minutes", CKPGUID_INT);
	proto->DeclareOutParameter("Seconds", CKPGUID_INT);

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(GetTime2);
	*pproto = proto;
	return CK_OK;
}

int GetTime2(const CKBehaviorContext& behcontext)
{
	
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	
	
	//int hour,minute,second;
	//SYSTEMTIME lpSystemTime;
	/*
	GetSystemTime( &lpSystemTime );
	
	hour = lpSystemTime.wHour;
	minute = lpSystemTime.wMinute;
	second = lpSystemTime.wSecond;
	beh->SetOutputParameterValue(0,&hour);
	beh->SetOutputParameterValue(1,&minute);
	beh->SetOutputParameterValue(2,&second);
	beh->ActivateOutput(0);
*/
	return 0;
	
}


