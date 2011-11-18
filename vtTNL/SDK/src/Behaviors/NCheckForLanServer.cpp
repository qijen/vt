#include "CKAll.h"
#include "vtNetworkManager.h"
#include <vtTools.h>
#include <virtools/vtcxglobal.h>
#include "xDistTools.h"
#include "xDistTypesAll.h"
#include "xMathTools.h"
#include "xDistributed3DObject.h"
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
#include "ISession.h"
#include "IMessages.h"
#include "xLogger.h"
#include "vtLogTools.h"
#include "xMessageTypes.h"
#include "xDistributedClient.h"

CKObjectDeclaration	*FillBehaviorNCheckForLanServerDecl();
CKERROR CreateNCheckForLanServerProto(CKBehaviorPrototype **);
int NCheckForLanServer(const CKBehaviorContext& behcontext);
CKERROR NCheckForLanServerCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1



typedef enum BB_IT
{
	BB_IT_IN,
	BB_IT_STOP,
	BB_IT_NEXT
};

typedef enum BB_INPAR
{
	BB_IP_CONNECTION_ID,
	BB_IP_MESSAGE,
};

typedef enum BB_OT
{
	BB_O_OUT,
	BB_O_MESSAGE,
	BB_O_ERROR,
};
typedef enum BB_OP
{
	BB_OP_SENDER_ID,
	BB_OP_PRIVATE,
	BB_OP_ERROR,
};


CKObjectDeclaration	*FillBehaviorNCheckForLanServerDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("NCheckForLanServer");	
	od->SetDescription("Waits for a Message from the Network.");
	
	od->SetCategory("TNL/Server");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x4bca58d9,0x3f4739fc));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateNCheckForLanServerProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


CKERROR CreateNCheckForLanServerProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("NCheckForLanServer");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInParameter("Timeout", CKPGUID_TIME, "5000");
	

	proto->DeclareInput("In");
	proto->DeclareInput("Off");
	
	proto->DeclareOutput("Out");
	proto->DeclareOutput("Server");
	
	
	proto->DeclareOutParameter("Server Address", CKPGUID_STRING, "");
	proto->DeclareLocalParameter("elapsed time", CKPGUID_FLOAT,0);

	

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS  ));
	proto->SetFunction(NCheckForLanServer);
	proto->SetBehaviorCallbackFct(NCheckForLanServerCB);

	*pproto = proto;
	return CK_OK;

}


int NCheckForLanServer(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());

	vtNetworkManager *nman = GetNM();
	//////////////////////////////////////////////////////////////////////////
	//connection id + session name : 

	using namespace vtTools::BehaviorTools;


	//////////////////////////////////////////////////////////////////////////
	//network ok  ? 
	xNetInterface *cin  = GetNM()->GetClientNetInterface();

	float elapsedTime = 0.0f;
	beh->GetLocalParameterValue(0,&elapsedTime);

	if (!nman->GetClientNetInterface())
	{
		nman->CreateClient(true,0,NULL);
	}

	if (beh->IsInputActive(0))
	{
		beh->ActivateInput(0,FALSE);

		//reset elapsed time :
		elapsedTime = 0.0f;
		beh->SetLocalParameterValue(0,&elapsedTime);
		
		//TNL::Address addr("IP:255.255.255.0:28999");
		TNL::Address broadcastAddress(IPProtocol, Address::Broadcast, 28999);
		if (GetNM()->GetClientNetInterface())
		{
			//cin->getLocalLanServers().clear();
//			nman->GetClientNetInterface()->sendPing(broadcastAddress,xNetInterface::Constants::ScanPingRequest);

		}
		
		

	}


	return CKBR_ACTIVATENEXTFRAME;
}

CKERROR NCheckForLanServerCB(const CKBehaviorContext& behcontext)
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