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



CKObjectDeclaration	*FillBehaviorNSRemoveUserDecl();
CKERROR CreateNSRemoveUserProto(CKBehaviorPrototype **);
int NSRemoveUser(const CKBehaviorContext& behcontext);
CKERROR NSRemoveUserCB(const CKBehaviorContext& behcontext);



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
	BB_IP_USER_ID,
};

typedef enum BB_OT
{
	BB_O_LEFT,
	BB_O_ERROR,
};

typedef enum BB_OP
{
	BB_OP_SESSION_ERROR
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
CKObjectDeclaration	*FillBehaviorNSRemoveUserDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("NSRemoveUser");	
	od->SetDescription("Iterates through all server sessions");

	od->SetCategory("TNL/Sessions");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x38c8759d,0x725b5e27));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateNSRemoveUserProto);
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
CKERROR CreateNSRemoveUserProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("NSRemoveUser");
	if(!proto) return CKERR_OUTOFMEMORY;



	proto->DeclareInput("In");
	
	proto->DeclareOutput("Removed");
	proto->DeclareOutput("Error");

	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	proto->DeclareInParameter("User ID", CKPGUID_INT, "0");
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "Ok");
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(NSRemoveUser);
	proto->SetBehaviorCallbackFct(NSRemoveUserCB);

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
int NSRemoveUser(const CKBehaviorContext& behcontext)
{
	using namespace vtTools::BehaviorTools;


	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	
	//network ok  ? 
	xNetInterface *cin  = GetNM()->GetClientNetInterface();

	if (!cin)
	{
		Error(beh,"No connection at the moment",BB_OP_SESSION_ERROR,TRUE,BB_O_ERROR);
		return 0;
	}

	IDistributedObjects *oInterface  = cin->getDistObjectInterface();
	ISession *sInterface =  cin->getSessionInterface();
	int connectionID = GetInputParameterValue<int>(beh,BB_IP_CONNECTION_ID);
	int userID = GetInputParameterValue<int>(beh,BB_IP_USER_ID);
	
	//////////////////////////////////////////////////////////////////////////
	//
	//		Check states : 
	//

		

	xDistributedClient *myClient = cin->getMyClient();
	xDistributedClient *client  = (xDistributedClient*)oInterface->getByUserID(userID,E_DC_BTYPE_CLIENT);
	
	xDistributedSession *session  = cin->getCurrentSession();

	if (!session)
	{
		Error(beh,"There doesn't exist such session",BB_OP_SESSION_ERROR,TRUE,BB_O_ERROR);
		xLogger::xLog(ELOGERROR,E_LI_SESSION,"There doesn't exist such session!");
		XL_BB_SIGNATURE;
		return 0;
	}

	if (!client)
	{
		Error(beh,"There doesn't exist such client object",BB_OP_SESSION_ERROR,TRUE,BB_O_ERROR);
		xLogger::xLog(ELOGERROR,E_LI_SESSION,"There doesn't exist such client object :%d",userID);
		XL_BB_SIGNATURE;
		return 0;
	}

	if (!session->isClientJoined(userID))
	{
		Error(beh,"Client not joint on this session!",BB_OP_SESSION_ERROR,TRUE,BB_O_ERROR);
		xLogger::xLog(ELOGERROR,E_LI_SESSION,"Client not joint on this session! %d",userID);
		XL_BB_SIGNATURE;
		return 0;
	}

	if (beh->IsInputActive(0))
	{
		beh->ActivateInput(0,FALSE);
        sInterface->removeClient(client,session->getSessionID(),false);
		beh->ActivateOutput(0);

	}

	return 0;
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
CKERROR NSRemoveUserCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;

	if (behcontext.CallbackMessage==CKM_BEHAVIOREDITED) 
	{

	}

	return CKBR_OK;
}
