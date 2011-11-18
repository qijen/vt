#include "CKAll.h"
#include "vtNetworkManager.h"
#include <vtTools.h>
#include "xDistTools.h"
#include "xDistTypesAll.h"
#include "xMathTools.h"
#include "xDistributed3DObject.h"
#include "xDistributedClient.h"
#include "xDistributedProperty.h"
#include "xDistributedPropertyInfo.h"

#include "xNetInterface.h"
#include "vtConnection.h"

#include "xDistributedBaseClass.h"
#include "xDistributed3DObjectClass.h"
#include "IDistributedClasses.h"
#include "xDOStructs.h"

#include "IDistributedObjectsInterface.h"

#include "xDistributedPoint4F.h"
#include "xDistributedPoint3F.h"
#include "xDistributedPoint2F.h"
#include "xDistributedPoint1F.h"
#include "xDistributedString.h"
#include "xDistributedInteger.h"
#include "xDistributedSession.h"
#include <vtTools.h>
#include "xLogger.h"
#include "vtLogTools.h"
CKObjectDeclaration	*FillBehaviorNServerStartDecl();
CKERROR CreateNServerStartProto(CKBehaviorPrototype **);
int NServerStart(const CKBehaviorContext& behcontext);
CKERROR NServerStartCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1




CKObjectDeclaration	*FillBehaviorNServerStartDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("NServerStart");	
	od->SetDescription("Starts an embedded server");
	
	od->SetCategory("TNL/Server");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x384359f7,0x14404b52));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateNServerStartProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


typedef enum BB_IT
{
	BB_IT_IN,
	BB_IT_OFF,
	BB_IT_NEXT,
};

typedef enum BB_INPAR
{
	BB_IP_ADDRESS,
	BB_IP_MAXCLIENTS,

};

typedef enum BB_OT
{
	BB_O_OUT,
	BB_O_ERROR,

};

typedef enum BB_OP
{
	BB_OP_SERVER_ADDRESS,
	BB_OP_ERROR
};


CKERROR CreateNServerStartProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("NServerStart");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	
	proto->DeclareOutput("Started");
	proto->DeclareOutput("Error");

	
	proto->DeclareInParameter("IP Address", CKPGUID_STRING, "IP:Any:28999");
	proto->DeclareInParameter("Max Clients", CKPGUID_INT, "32");
	
	
	proto->DeclareOutParameter("Server Address", CKPGUID_STRING, "-1");
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "Ok");
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(NServerStart);
	proto->SetBehaviorCallbackFct(NServerStartCB);

	*pproto = proto;
	return CK_OK;

}


int NServerStart(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());
	using namespace vtTools::BehaviorTools;

	XString serverAddress((CKSTRING) beh->GetInputParameterReadDataPtr(BB_IP_ADDRESS));
	using namespace vtTools::BehaviorTools;

	
	vtNetworkManager *nMan = GetNM();

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	bbNoError(E_NWE_OK);
	
	if (beh->IsInputActive(0))
	{
		bool result = true;
		beh->ActivateInput(0,FALSE);
		
		int error = nMan->CreateServer(true,0,serverAddress.CStr());
		
		if (error == E_NWE_OK)
		{
			bbNoError(E_NWE_OK);
			beh->ActivateOutput(0);
		}else
		{
			bbError(error);
		}
		
		return CKBR_OK;
	}
	return CKBR_OK;

}

CKERROR NServerStartCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKBeObject *beo = (CKBeObject *)beh->GetTarget();
	if (!beo) {
		return CKBR_OWNERERROR;
	}
  
	if (behcontext.CallbackMessage==CKM_BEHAVIOREDITED) 
	{
	
	}

	return CKBR_OK;
}