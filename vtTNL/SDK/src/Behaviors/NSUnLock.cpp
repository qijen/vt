#include "CKAll.h"
#include "vtNetworkManager.h"
#include <vtTools.h>
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
#include "xDistributedSessionClass.h"
#include "xDistributedSession.h"
#include "xDistributedClient.h"

#include "xLogger.h"
#include "vtLogTools.h"
CKObjectDeclaration	*FillBehaviorNSUnlockObjectDecl();
CKERROR CreateNSUnlockObjectProto(CKBehaviorPrototype **);
int NSUnlockObject(const CKBehaviorContext& behcontext);
CKERROR NSUnlockObjectCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 5
#define BEH_OUT_MIN_COUNT 2
#define BEH_OUT_MAX_COUNT 1

typedef enum BB_IT
{
	BB_IT_IN
};

typedef enum BB_INPAR
{
	BB_IP_CONNECTION_ID,
};

typedef enum BB_OT
{
	BB_O_UNLOCKED,
	BB_O_WAITING,
	BB_O_ERROR
	
};

typedef enum BB_OP
{
	BB_OP_ERROR,
};

/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
CKObjectDeclaration	*FillBehaviorNSUnlockObjectDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("NSUnlock");	
	od->SetDescription("Locks a session");

	od->SetCategory("TNL/Sessions");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0xdf841a5,0x11475a9c));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateNSUnlockObjectProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}

/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
CKERROR CreateNSUnlockObjectProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("NSUnlock");
	if(!proto) return CKERR_OUTOFMEMORY;



	proto->DeclareInput("In");
	
	proto->DeclareOutput("Unlocked");
	proto->DeclareOutput("Waiting For Answer");
	proto->DeclareOutput("Error");

	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "Ok");
	proto->DeclareSetting("Wait For Answer", CKPGUID_BOOL, "0");
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(NSUnlockObject);
	proto->SetBehaviorCallbackFct(NSUnlockObjectCB);

	*pproto = proto;
	return CK_OK;

}

typedef std::vector<xDistributedSession*>xSessions;
/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
int NSUnlockObject(const CKBehaviorContext& behcontext)
{
	using namespace vtTools::BehaviorTools;

	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	bbNoError(E_NWE_OK);

	//network ok  ? 
	xNetInterface *cin  = GetNM()->GetClientNetInterface();
	if (!cin)
	{
		bbError(E_NWE_NO_CONNECTION);
		return 0;
	}

	if (!cin->getMyClient())
	{
		bbError(E_NWE_INTERN);
		return 0;
	}

		
	ISession *sInterface =  cin->getSessionInterface();
	int connectionID = GetInputParameterValue<int>(beh,BB_IP_CONNECTION_ID);
	
	//////////////////////////////////////////////////////////////////////////
	//
	//		Check states : 
	//

	xDistributedClient *myClient = cin->getMyClient();
	if (!myClient)
	{
		bbError(E_NWE_INTERN);
		return 0;
	}

	if (!myClient->getClientFlags().test(1 << E_CF_SESSION_JOINED))
	{
		bbError(E_NWE_NO_SESSION);
		return 0;
	}

	xDistributedSession *session  = cin->getCurrentSession();
	if (!session)
	{
		bbError(E_NWE_NO_SUCH_SESSION);
		return 0;
	}

	if (beh->IsInputActive(0))
	{
		beh->ActivateInput(0,FALSE);
		if (myClient->getUserID() != session->getUserID())
		{
			bbError(E_NWE_NOT_SESSION_MASTER);
			return 0;
		}
		beh->ActivateInput(0,FALSE);
        sInterface->unlockSession(myClient->getUserID(),session->getSessionID());
	}


	if (session->isLocked())
	{
		beh->ActivateOutput(BB_O_UNLOCKED);
		return 0;
	}else
	{
		beh->ActivateOutput(BB_O_WAITING);
	}
	return CKBR_ACTIVATENEXTFRAME;
}

/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
CKERROR NSUnlockObjectCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;

	if (behcontext.CallbackMessage==CKM_BEHAVIOREDITED) 
	{

	}

	return CKBR_OK;
}
