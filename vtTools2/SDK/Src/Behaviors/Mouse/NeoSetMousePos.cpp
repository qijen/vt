//////////////////////////////////////////////////////////////////////////////////////////////////////////
//		            NeoSetMousePos
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//#include <winuser.h>
#include "CKAll.h"
#include "windows.h"

CKObjectDeclaration	*FillBehaviorNeoSetMousePosDecl();
CKERROR CreateNeoSetMousePosProto(CKBehaviorPrototype **);
int NeoSetMousePos(const CKBehaviorContext& BehContext);
int NeoSetMousePosCallBack(const CKBehaviorContext& BehContext);

CKObjectDeclaration	*FillBehaviorNeoSetMousePosDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Set Mouse Pos");	
	
	od->SetType(CKDLL_BEHAVIORPROTOTYPE);
	od->SetVersion(0x0000001);
	od->SetCreationFunction(CreateNeoSetMousePosProto);
	od->SetDescription("Set Mouse Position");
	od->SetCategory("Controllers/Mouse");
	od->SetGuid(CKGUID(0xa72d87d4, 0x882b89a6));
	od->SetAuthorGuid(CKGUID(0x56495254,0x4f4f4c53));
	od->SetAuthorName("Neo");
	od->SetCompatibleClassId(CKCID_BEOBJECT);

	return od;
}

CKERROR CreateNeoSetMousePosProto(CKBehaviorPrototype** pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Set Mouse Pos");
	if (!proto) {
		return CKERR_OUTOFMEMORY;
	}

//---     Inputs declaration
	proto->DeclareInput("Set");

//---     Outputs declaration
	proto->DeclareOutput("Done");

//---     Input Parameters declaration
	proto->DeclareInParameter("X", CKPGUID_INT,"100");
	proto->DeclareInParameter("Y", CKPGUID_INT,"100");
	proto->DeclareInParameter("Keep Active", CKPGUID_BOOL, "TRUE");
//	proto->DeclareSetting("Keep Active",CKPGUID_BOOL,"TRUE");
//	proto->DeclareLocalParameter("KeepActive", CKPGUID_BOOL);


//----	Local Parameters Declaration

//----	Settings Declaration

	proto->SetBehaviorCallbackFct(NeoSetMousePosCallBack, NULL);
	proto->SetFunction(NeoSetMousePos);

	*pproto = proto;
	return CK_OK;
}

int NeoSetMousePos(const CKBehaviorContext& BehContext)
{
	CKBehavior* beh = BehContext.Behavior;
	int x, y;
	
 	beh->GetInputParameterValue(0,&x);
	beh->GetInputParameterValue(1,&y);
	SetCursorPos(x,y);
	beh->ActivateOutput(0);
	CKBOOL keepActive;
	beh->GetInputParameterValue(2,&keepActive);
	if(keepActive)return CKBR_ACTIVATENEXTFRAME;
	return CKBR_OK;
}

int NeoSetMousePosCallBack(const CKBehaviorContext& BehContext)
{
	return CKBR_OK;
}


