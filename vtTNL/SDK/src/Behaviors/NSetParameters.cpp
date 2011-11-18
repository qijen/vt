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

#include "ISession.h"
#include "IMessages.h"
#include "xLogger.h"
#include "vtLogTools.h"
#include "xMessageTypes.h"


CKObjectDeclaration	*FillBehaviorNSetParametersDecl();
CKERROR CreateNSetParametersProto(CKBehaviorPrototype **);
int NSetParameters(const CKBehaviorContext& behcontext);
CKERROR NSetParametersCB(const CKBehaviorContext& behcontext);


typedef enum BB_IT
{
	BB_IT_IN,
};

typedef enum BB_INPAR
{
	BB_IP_MIN_MANAGER,
	BB_IP_MIN_MESSAGE_TICK,
	BB_IP_MESSAGE_TIMEOUT,
	BB_IP_CONNECTION_TIMEOUT,
	BB_IP_PACKET_LOSS,
	BB_IP_LATENCY,
	BB_IP_MIN_SEND_PERIOD,
	BB_IP_MIN_RECV_PERIOD,
	BB_IP_MAX_SEND_BANDWITH,
	BB_IP_MAX_RECV_BANDWITH

};

typedef enum BB_OT
{
	BB_O_ERROR,
};
typedef enum BB_OP
{
	BB_OP_ERROR,
};

CKObjectDeclaration	*FillBehaviorNSetParametersDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("NSetParameters");	
	od->SetDescription("Sends a Message to a Network.");
	
	od->SetCategory("TNL/Misc");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x7dc37c6f,0x524f7732));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateNSetParametersProto);

	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


CKERROR CreateNSetParametersProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("NSetParameters");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	
	proto->DeclareInParameter("Min Tick Time Manager", CKPGUID_FLOAT, "50");
	proto->DeclareInParameter("Min Tick Time Message Send", CKPGUID_FLOAT, "50");
	proto->DeclareInParameter("Message Timeout", CKPGUID_FLOAT, "300");
	proto->DeclareInParameter("Connection Timeout", CKPGUID_FLOAT, "3000");
	proto->DeclareInParameter("Packet Loss", CKPGUID_FLOAT, "0.1");
	proto->DeclareInParameter("Latency",CKPGUID_INT, "100");
	proto->DeclareInParameter("MinPacketSendPeriod",CKPGUID_INT,"50");
	proto->DeclareInParameter("MinPacketRecvPeriod",CKPGUID_INT,"50");
	proto->DeclareInParameter("MaxSendBandwidth",CKPGUID_INT,"2000");
	proto->DeclareInParameter("MaxRecvBandwidth",CKPGUID_INT,"2000");

	proto->DeclareSetting("Only Client", CKPGUID_BOOL, "FALSE");



	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	
	proto->SetFunction(NSetParameters);
	proto->SetBehaviorCallbackFct(NSetParametersCB);

	*pproto = proto;
	return CK_OK;

}


int NSetParameters(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());

	using namespace vtTools::BehaviorTools;

	bbNoError(E_NWE_OK);
	beh->ActivateInput(0,FALSE);

	float minManagerTick  = GetInputParameterValue<float>(beh,BB_IP_MIN_MANAGER);
	float messageTick  = GetInputParameterValue<float>(beh,BB_IP_MIN_MESSAGE_TICK);
	float messageTimeout  = GetInputParameterValue<float>(beh,BB_IP_MESSAGE_TIMEOUT);
	float connectionTimeout  = GetInputParameterValue<float>(beh,BB_IP_CONNECTION_TIMEOUT);
	float packetLoss  = GetInputParameterValue<float>(beh,BB_IP_PACKET_LOSS);
	int latency  = GetInputParameterValue<int>(beh,BB_IP_LATENCY);

	int MinPacketSendPeriod  = GetInputParameterValue<int>(beh,BB_IP_MIN_SEND_PERIOD);
	int MinPacketRecvPeriod  = GetInputParameterValue<int>(beh,BB_IP_MIN_RECV_PERIOD);
	int MaxSendBandwidth  = GetInputParameterValue<int>(beh,BB_IP_MAX_SEND_BANDWITH);
	int MaxRecvBandwidth  = GetInputParameterValue<int>(beh,BB_IP_MAX_RECV_BANDWITH);

	int clientOnly = false;
	beh->GetLocalParameterValue(0,&clientOnly);

	//void NetConnection::
	//setFixedRateParameters(U32 minPacketSendPeriod, U32 minPacketRecvPeriod, U32 maxSendBandwidth, U32 maxRecvBandwidth)
	//setFixedRateParameters(50, 50, 2000, 2000);

	if (minManagerTick < 50)
	{
		minManagerTick = 50.0f;
	}
	if (messageTick < 50)
	{
		messageTick = 50.0f;
	}
	if (messageTimeout< 150)
	{
		messageTimeout = 150.0f;
	}
	if (connectionTimeout < 200.0f)
	{
		connectionTimeout = 200.0f;
	}
	if (latency < 0)
	{
		latency = 0;
	}
	if (packetLoss < 0.0f)
	{
		packetLoss = 0.0f;
	}
	if (packetLoss > 1.0f)
	{
		packetLoss= 1.0f;
	}


	GetNM()->setMinTickTime(minManagerTick);
	xNetInterface *cin  = GetNM()->GetClientNetInterface();
	if (cin)
	{
		if (cin->isValid())
		{
			vtConnection *con = cin->getConnection();
			if (con)
			{
				con->setSimulatedNetParams(packetLoss,latency);
				con->setPingTimeouts(connectionTimeout,3);
				cin->setFixedRateParameters(MinPacketSendPeriod,MinPacketRecvPeriod,MaxSendBandwidth,MaxRecvBandwidth,false);
			}
		}
		cin->getMessagesInterface()->setMessageTimeout(messageTimeout);
		cin->getMessagesInterface()->setMinSendTime(messageTick);
	}

	if (!clientOnly)
	{
		cin  = GetNM()->GetServerNetInterface();
		if (cin)
		{
			cin->getMessagesInterface()->setMessageTimeout(messageTimeout);
			cin->getMessagesInterface()->setMinSendTime(messageTick);
			cin->setFixedRateParameters(MinPacketSendPeriod,MinPacketRecvPeriod,MaxSendBandwidth,MaxRecvBandwidth,true);
		}
	}
		
	
	beh->ActivateOutput(0);

	return CKBR_OK;
}

CKERROR NSetParametersCB(const CKBehaviorContext& behcontext)
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