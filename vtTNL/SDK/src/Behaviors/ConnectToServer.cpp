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
#include "xLogger.h"

#include "vtLogTools.h"


CKObjectDeclaration	*FillBehaviorConnectToServerDecl();
CKERROR CreateConnectToServerProto(CKBehaviorPrototype **);
int ConnectToServer(const CKBehaviorContext& behcontext);
CKERROR ConnectToServerCB(const CKBehaviorContext& behcontext);

CKObjectDeclaration	*FillBehaviorConnectToServerDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("NConnect");	
	od->SetDescription("Connects to a Server");
	od->SetCategory("TNL/Server");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x69b86522,0x34cf3563));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateConnectToServerProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


typedef enum BB_INPAR
{
	BB_IP_HOSTS,
	BB_IP_MODULES,
	BB_IP_LOGIN,
	BB_IP_PASSWORD
};

typedef enum BB_OT
{

	BB_O_CONNECTED,
	BB_O_WAITING,
	BB_O_TIME_OUT,
	BB_O_ERROR,
	BB_O_IN_USE,

};
typedef enum BB_OP
{

	BB_OP_CONNECTION_ID,
	BB_OP_HOST,
	BB_OP_PORT,
	BB_OP_ERROR,
	BB_OP_MAX_PLAYERS,
	BB_OP_CLIENT_ID
};

CKERROR CreateConnectToServerProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("NConnect");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("Connect");
	
	proto->DeclareOutput("Connected");
	proto->DeclareOutput("Waiting for Answer");
	proto->DeclareOutput("Timeout");
	proto->DeclareOutput("Error");
	proto->DeclareOutput("Connection In Use");
	
	proto->DeclareInParameter("Hosts", CKPGUID_DATAARRAY, "Hosts");
	proto->DeclareInParameter("Modules", CKPGUID_DATAARRAY, "Modules");
	proto->DeclareInParameter("Login", CKPGUID_STRING, "Player");
	proto->DeclareInParameter("Password", CKPGUID_STRING, "Player");
	
	
	proto->DeclareOutParameter("Connection ID", CKPGUID_INT, "-1");
	proto->DeclareOutParameter("Host", CKPGUID_STRING, "None");
	proto->DeclareOutParameter("Port", CKPGUID_INT, "-1");
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "0");
	proto->DeclareOutParameter("Client ID", CKPGUID_INT, "-1");

	proto->DeclareSetting("Connection Timeout", CKPGUID_FLOAT,"2000");
	proto->DeclareLocalParameter("currentArrayIndex", CKPGUID_INT,0 );

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(ConnectToServer);
	proto->SetBehaviorCallbackFct(ConnectToServerCB);

	*pproto = proto;
	return CK_OK;

}

#include "vtLogTools.h"


int ConnectToServer(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;


	using namespace vtTools::BehaviorTools;
	vtNetworkManager *nman = GetNM();
	bbNoError(E_NWE_OK);
	CKDataArray* hostsarray = static_cast<CKDataArray*>(beh->GetInputParameterObject(0));
	if (!hostsarray) 
	{
		bbError(E_NWE_INVALID_PARAMETER);
		return FALSE;
	}


	int arrayIndex = 0;
	beh->GetLocalParameterValue(1,&arrayIndex);

	float timeOut = 0.0f;
	beh->GetLocalParameterValue(0,&timeOut);

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	
	if (!nman->GetClientNetInterface())
	{
		nman->CreateClient(true,0,NULL);
		nman->GetClientNetInterface()->setConnectionTimeOut(timeOut);
	}

	float elapsed  = nman->GetClientNetInterface()->getElapsedConnectionTime() + behcontext.DeltaTime;
	nman->GetClientNetInterface()->setElapsedConnectionTime(elapsed);
	
	if (arrayIndex > hostsarray->GetRowCount() -1 )
	{
		arrayIndex  = 0;
		beh->SetLocalParameterValue(1,&arrayIndex);
		bbError(E_NWE_NO_SERVER);
		return FALSE;
	}

	if (nman->GetClientNetInterface()->isConnected())
	{
        beh->ActivateOutput(4);
		return FALSE;
	}
	
	/************************************************************************/
	/* first start   ?									                                                                     */
	/************************************************************************/
	if (!nman->GetClientNetInterface()->isConnectionInProgress())
	{
		CKCHAR fixedBuffer[256];
		CKSTRING hosts =  fixedBuffer;
		hostsarray->GetElementStringValue(arrayIndex,0,hosts);
		TNL::Address addr(hosts);

		if (strlen(hosts))
		{
			xLogger::xLog(ELOGINFO,E_LI_CONNECTION,"connecting to server:%s",fixedBuffer);
		}
		nman->ConnectToServer(true,hosts);
		nman->GetClientNetInterface()->sendPing(hosts);
		nman->GetClientNetInterface()->setConnectionInProgress(true);
	}

	/************************************************************************/
	/* we are already trying to connect :                                                                      */
	/************************************************************************/
	if(nman->GetClientNetInterface()->isConnectionInProgress())
	{
		if (nman->GetClientNetInterface()->getConnection())
		{
			/////////////////////////////////////////////////////////////////////////
			//connection established ?
			if (nman->GetClientNetInterface()->isValid())
			{
				nman->GetClientNetInterface()->setConnectionInProgress(false);
				TNL::Address addr  = nman->GetClientNetInterface()->getConnection()->getNetAddress();
				arrayIndex  = 0;
				beh->SetLocalParameterValue(1,&arrayIndex);
				nman->GetClientNetInterface()->setConnected(true);

				/************************************************************************/
				/* we set the connection parameters :			                                                                       */
				/************************************************************************/
				CKCHAR fixedBuffer[256];
				CKSTRING hosts =  fixedBuffer;
				hostsarray->GetElementStringValue(arrayIndex,0,hosts);
				CKParameterOut *pout = beh->GetOutputParameter(1);
				pout->SetStringValue(hosts);
				
				//////////////////////////////////////////////////////////////////////////
				int userID    =  nman->GetClientNetInterface()->getConnection()->getUserID();
				

				int connectionID  = nman->GetClientNetInterface()->getConnectionList().size();
				beh->SetOutputParameterValue(0,&connectionID);
				beh->SetOutputParameterValue(4,&userID);



				beh->ActivateOutput(0);
				return CKBR_OK;
			}

			//////////////////////////////////////////////////////////////////////////
			// we check the time out
			if (nman->GetClientNetInterface()->getElapsedConnectionTime() > nman->GetClientNetInterface()->getConnectionTimeOut()  )
			{
				//
				nman->GetClientNetInterface()->setElapsedConnectionTime(0.0f);
				nman->GetClientNetInterface()->setConnectionInProgress(false);
				
				//we delete all our  data : 
				if (nman->GetClientNetInterface()->getConnection())
				{
					nman->GetClientNetInterface()->getConnection()->disconnect("keiner");
					nman->GetClientNetInterface()->setConnection(NULL);
				}

				//more hosts    ? 
				if (arrayIndex < hostsarray->GetRowCount() )
				{
					arrayIndex++;
					beh->SetLocalParameterValue(1,&arrayIndex);
					//beh->ActivateOutput(2);
				}else
				{
					xLogger::xLog(ELOGWARNING,E_LI_CONNECTION,"No server was available");
					bbError(E_NWE_NO_SERVER);
					arrayIndex  = 0;
					beh->SetLocalParameterValue(1,&arrayIndex);
					beh->ActivateOutput(2);
					return FALSE;
				}

			}else
			{
				beh->ActivateOutput(1);
			}

		}

	}

	return CKBR_ACTIVATENEXTFRAME;
}

CKERROR ConnectToServerCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKBeObject *beo = (CKBeObject *)beh->GetTarget();
	if (!beo) {
		return CKBR_OWNERERROR;
	}
  
	if (behcontext.CallbackMessage==CKM_BEHAVIORATTACH) 
	{
		int arrayIndex = 0;
		beh->SetLocalParameterValue(1,&arrayIndex);

	}

	return CKBR_OK;
}