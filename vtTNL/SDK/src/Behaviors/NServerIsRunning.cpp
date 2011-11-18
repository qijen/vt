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
CKObjectDeclaration	*FillBehaviorNServerIsRunningDecl();
CKERROR CreateNServerIsRunningProto(CKBehaviorPrototype **);
int NServerIsRunning(const CKBehaviorContext& behcontext);
CKERROR NServerIsRunningCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1




CKObjectDeclaration	*FillBehaviorNServerIsRunningDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("NServerIsRunning");	
	od->SetDescription("Checks for the embedded servers state");
	
	od->SetCategory("TNL/Server");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x3d40087,0xe265210));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateNServerIsRunningProto);
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


CKERROR CreateNServerIsRunningProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("NServerIsRunning");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Yes");
	proto->DeclareOutput("No");

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(NServerIsRunning);
	proto->SetBehaviorCallbackFct(NServerIsRunningCB);

	*pproto = proto;
	return CK_OK;

}


int NServerIsRunning(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());
	using namespace vtTools::BehaviorTools;

	
	if (beh->IsInputActive(0))
	{
		
		beh->ActivateInput(0,FALSE);
		
		xNetInterface *cin  = GetNM()->GetServerNetInterface();
		if (cin)
		{

			TNL::Socket *socket  = &cin->getSocket();
			if (socket)
			{
				if (socket->isValid())
				{
					beh->ActivateOutput(0);
					return CKBR_OK;
				}
			}
		}
		
		beh->ActivateOutput(1);
		
		
		return CKBR_OK;
	}
	return CKBR_OK;
}

CKERROR NServerIsRunningCB(const CKBehaviorContext& behcontext)
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