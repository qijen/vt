#include "CKAll.h"
CKObjectDeclaration	*FillBehaviorGetAdaptersModieDecl();
CKERROR CreateGetAdaptersModieProto(CKBehaviorPrototype **pproto);
int GetAdaptersModie(const CKBehaviorContext& behcontext);


/************************************************************************/
/*																																			  */
/************************************************************************/

CKObjectDeclaration	*FillBehaviorGetAdaptersModieDecl(){
	CKObjectDeclaration *od = CreateCKObjectDeclaration("GetAdaptersModie");	
	od->SetDescription("Add Objects");
	od->SetCategory("Narratives/System");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x17ad4ca4,0x5bab3ee7));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("mw");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateGetAdaptersModieProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}
CKERROR CreateGetAdaptersModieProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("GetAdaptersModie");
	if(!proto) return CKERR_OUTOFMEMORY;
	proto->DeclareInput("Start");
	proto->DeclareInput("Finish");
	proto->DeclareOutput("Finish");
	proto->DeclareOutput("LoopOut");


	
	proto->DeclareInParameter("driver index",CKPGUID_INT);

		
	proto->DeclareOutParameter("modie index",CKPGUID_INT);
	proto->DeclareOutParameter("width",CKPGUID_INT);
	proto->DeclareOutParameter("height",CKPGUID_INT);

	proto->DeclareOutParameter("bpp",CKPGUID_INT);
	proto->DeclareOutParameter("refresh rate",CKPGUID_INT);
	proto->DeclareOutParameter("modie count",CKPGUID_INT);


	proto->SetFunction(GetAdaptersModie);
	*pproto = proto;
	return CK_OK;
}

int indexDD = 0;

int GetAdaptersModie(const CKBehaviorContext& behcontext)
{

	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;


	CKPluginManager* ThePluginManager=CKGetPluginManager();
	CKRenderManager *rm = (CKRenderManager *)ctx->GetRenderManager();


	int DriverIndex=0;
	beh->GetInputParameterValue(0, &DriverIndex);
	VxDriverDesc *desc=rm->GetRenderDriverDescription(DriverIndex);
	int mocount = desc->DisplayModes.Size();

	int countDD =rm->GetRenderDriverCount();
	
	if( beh->IsInputActive(0) )
	{
			beh->ActivateInput(0, FALSE);
			indexDD  = 0;
			beh->SetOutputParameterValue(5,&mocount);
			return CKBR_OK;
	}

	if ( indexDD >=mocount-1 /*|| DriverIndex < countDD */)
	{
		indexDD = 0;
		beh->ActivateOutput(0,TRUE);
		return CKBR_OK;
	}

	

	if( beh->IsInputActive(1) )
	{
		beh->ActivateInput(1, FALSE);
		beh->SetOutputParameterValue(1,&desc->DisplayModes[indexDD].Width);
		beh->SetOutputParameterValue(2,&desc->DisplayModes[indexDD].Height);
		beh->SetOutputParameterValue(3,&desc->DisplayModes[indexDD].Bpp);
		beh->SetOutputParameterValue(4,&desc->DisplayModes[indexDD].RefreshRate);
		beh->SetOutputParameterValue(0,&indexDD);
		indexDD++;
		beh->ActivateOutput(1,TRUE);
		return CKBR_OK;
		
	}
	
	return CKBR_OK;
}


