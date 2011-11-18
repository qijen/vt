
#include "CKAll.h"



CKObjectDeclaration	*FillBehaviorGetAdaptersDecl();
CKERROR CreateGetAdaptersProto(CKBehaviorPrototype **pproto);
int GetAdapters(const CKBehaviorContext& behcontext);


/************************************************************************/
/*																																			  */
/************************************************************************/

CKObjectDeclaration	*FillBehaviorGetAdaptersDecl(){
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Get Adapters");	
	od->SetDescription("Add Objects");
	od->SetCategory("Narratives/System");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x2ab2796a,0x24c15af7));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("mw");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateGetAdaptersProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}
CKERROR CreateGetAdaptersProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Get Adapters");
	if(!proto) return CKERR_OUTOFMEMORY;
	proto->DeclareInput("Get Next");
	proto->DeclareInput("Get Prev");
	
	proto->DeclareOutput("Finish");
	proto->DeclareOutput("LoopOut");
	
	proto->DeclareOutParameter("Name",CKPGUID_STRING);
	proto->DeclareOutParameter("Index",CKPGUID_INT);
	proto->DeclareOutParameter("Count",CKPGUID_INT);
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);

	proto->SetFunction(GetAdapters);
	*pproto = proto;
	return CK_OK;
}

int indexD = 0;
int countD = 0;


int GetAdapters(const CKBehaviorContext& behcontext)
{

	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;


	CKPluginManager* ThePluginManager=CKGetPluginManager();
	CKRenderManager *rm = (CKRenderManager *)ctx->GetRenderManager();

	
	if( beh->IsInputActive(0) ){
			beh->ActivateInput(0, FALSE);

			
			countD=rm->GetRenderDriverCount();
			indexD = 0;

			beh->ActivateInput(1, TRUE);

	}

	if( beh->IsInputActive(1) ){
			beh->ActivateInput(1, FALSE);
		
			if (indexD > (countD-1)){
				indexD = 0;
				beh->ActivateOutput(0,TRUE);
				return CKBR_OK;
			}


			beh->SetOutputParameterValue(1,&indexD);
			VxDriverDesc *desc=rm->GetRenderDriverDescription(indexD);

			indexD++;

			

			CKParameterOut *pout2 = beh->GetOutputParameter(0);
			pout2->SetStringValue(desc->DriverName.Str() );
			beh->ActivateOutput(1);


	}

	


	
	return CKBR_OK;

}


