#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPManagerDecl();
CKERROR CreatePManagerProto(CKBehaviorPrototype **pproto);
int PManager(const CKBehaviorContext& behcontext);
CKERROR PManagerCB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_ReloadXML,
};


#define BB_SSTART 0

BBParameter pInMapM[]  =
{


	BB_SPIN(bbI_ReloadXML,CKPGUID_STRING,"XML File","None"),

};

#define gPIMAP pInMapM

//************************************
// Method:    FillBehaviorPManagerDecl
// FullName:  FillBehaviorPManagerDecl
// Access:    public
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPManagerDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PManager");
	od->SetCategory("Physic/Manager");
	od->SetDescription("Calls various functions in the manager.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x57295d90,0x11da3970));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePManagerProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);

	return od;
}

//************************************
// Method:    CreatePManagerProto
// FullName:  CreatePManagerProto
// Access:    public
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePManagerProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PManager");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	
	proto->SetBehaviorCallbackFct( PManagerCB );

	BB_EVALUATE_SETTINGS(gPIMAP)


	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);

	proto->SetFunction(PManager);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PManager
// FullName:  PManager
// Access:    public
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PManager(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();

	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;

	if( beh->IsInputActive(0) )
	{
		beh->ActivateInput(0,FALSE);


		BB_DECLARE_PIMAP;



		/************************************************************************/
		/* retrieve settings state  */
		/************************************************************************/
		BBSParameter(bbI_ReloadXML);
		

		/************************************************************************/
		/* retrieve values                                                                     */
		/************************************************************************/

		
			//////////////////////////////////////////////////////////////////////////
		//	load some settings from XML
		if(sbbI_ReloadXML)
		{
			CKSTRING xmlFile = GetInputParameterValue<CKSTRING>(beh,BB_IP_INDEX(bbI_ReloadXML));
			pFactory::Instance()->reloadConfig(xmlFile);

		}

		
	


	}
	beh->ActivateOutput(0);
	return 0;
}

//************************************
// Method:    PManagerCB
// FullName:  PManagerCB
// Access:    public
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PManagerCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	BB_DECLARE_PMAP;

	switch(behcontext.CallbackMessage) {

	case CKM_BEHAVIORCREATE:
	case CKM_BEHAVIORLOAD:
		{
			BB_LOAD_PIMAP(gPIMAP,BB_SSTART);
			break;
		}
		case CKM_BEHAVIORDETACH:
		{
			BB_DESTROY_PIMAP;
			break;
		}

		case CKM_BEHAVIORATTACH:
		{
			BB_INIT_PIMAP(gPIMAP,BB_SSTART);
			break;
		}
		case CKM_BEHAVIORSETTINGSEDITED:
		{
			BB_REMAP_PIMAP(gPIMAP,BB_SSTART);
			break;
		}
	}
	return CKBR_OK;

}

