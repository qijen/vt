#include "CKAll.h"
#include "vtNetworkManager.h"
#include <vtTools.h>
#include "xDistTools.h"
#include "xDistTypesAll.h"
#include "xMathTools.h"
#include "xDistributed3DObject.h"
#include "xDistributedClient.h"
#include "xDistributedSession.h"
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


CKObjectDeclaration	*FillBehaviorNMSendDecl();
CKERROR CreateNMSendProto(CKBehaviorPrototype **);
int NMSend(const CKBehaviorContext& behcontext);
CKERROR NMSendCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 0
#define BEH_OUT_MAX_COUNT 1


typedef enum BB_IT
{
	BB_IT_IN,
};

typedef enum BB_INPAR
{
	BB_IP_CONNECTION_ID,
	BB_IP_MESSAGE,
};

typedef enum BB_OT
{
	BB_O_OUT,
	BB_O_ERROR,
};
typedef enum BB_OP
{
	BB_OP_ERROR,
};

CKObjectDeclaration	*FillBehaviorNMSendDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("NMSend");	
	od->SetDescription("Sends a message over network");
	
	od->SetCategory("TNL/Message");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x51a072f,0x6aec1386));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateNMSendProto);

	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


CKERROR CreateNMSendProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("NMSend");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	proto->DeclareOutput("Error");
	
	proto->DeclareInParameter("Message", CKPGUID_MESSAGE, "OnClick");
	proto->DeclareInParameter("Target User ID", CKPGUID_INT, "0");
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR);
	proto->DeclareSetting("Ignore Session Master", CKPGUID_BOOL, "FALSE");

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETERINPUTS  ));
	proto->SetFunction(NMSend);
	proto->SetBehaviorCallbackFct(NMSendCB);

	*pproto = proto;
	return CK_OK;

}


int NMSend(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());

	using namespace vtTools::BehaviorTools;

	bbNoError(E_NWE_OK);
	beh->ActivateInput(0,FALSE);

	int Msg=-1;
	beh->GetInputParameterValue(0,&Msg);
	XString msgType(mm->GetMessageTypeName(Msg));
	xNString mName  = msgType.CStr();
	
	int targetID=0;
	beh->GetInputParameterValue(1,&targetID);

	
	int ignoreSessionMaster = 0;
	beh->GetLocalParameterValue(0,&ignoreSessionMaster);
	
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
	IMessages *mInterface = cin->getMessagesInterface();


	xDistributedClient *myClient = cin->getMyClient();
	if (!myClient)
	{
		bbError(E_NWE_INTERN);
		return 0;
	}

	xDistributedSession *session  = sInterface->get(myClient->getSessionID());
	if (!session)
	{
		bbError(E_NWE_NO_SUCH_SESSION);
		return 0;
	}

	if (!isFlagOn(myClient->getClientFlags(),E_CF_SESSION_JOINED))
	{	
		bbError(E_NWE_NO_SESSION);
		return 0;
	}
	
	if (session->getUserID() == myClient->getUserID() && session->getNumUsers()==1 )
	{
		bbError(E_NWE_INTERN);
		return 0;
	}

	int bcount = beh->GetInputParameterCount();
	int srcId = cin->getConnection()->getUserID();

	xMessage *msg  = new xMessage();
	msg->setName(mName);
	msg->setSrcUser( myClient->getServerID() );
	msg->setIgnoreSessionMaster(ignoreSessionMaster);
	msg->setDstUser(targetID);

	if (targetID !=-1)
	{
		IDistributedObjects *doInterface  = cin->getDistObjectInterface();
		xDistributedClient *dstClient = static_cast<xDistributedClient*>(doInterface->getByUserID(targetID,E_DC_BTYPE_CLIENT));
		if (dstClient)
		{
			if (dstClient->getUserID() == myClient->getUserID())
			{
				bbError(E_NWE_INTERN);
				delete msg;
				return 0;
			}

			msg->setDstUser(dstClient->getUserID());


		}else{
			bbError(E_NWE_NO_SUCH_USER);
			delete msg;

			return 0;
		}
	}

	
	
	for (int i = BEH_IN_INDEX_MIN_COUNT  ; i < beh->GetInputParameterCount() ; i++ )
	{

		CKParameterIn *ciIn = beh->GetInputParameter(i);
		CKParameterType pType  = ciIn->GetType();
		int sType = vtTools::ParameterTools::GetVirtoolsSuperType(ctx,pam->ParameterTypeToGuid(pType));	
		const char *pname  = pam->ParameterTypeToName(pType);
		
		xDistributedPropertyInfo *dInfo  = new xDistributedPropertyInfo( ciIn->GetName() , xDistTools::SuperTypeToValueType(sType) , -1 ,E_PTYPE_RELIABLE);


		switch (sType)
		{
		case vtSTRING:
			{
				CKParameter *pin = beh->GetInputParameter(i)->GetRealSource();
				if (pin)
				{
					xDistributedString *prop = new xDistributedString(dInfo,30,3000);
					VxScratch sbuffer(256);
					CKSTRING buffer = (CKSTRING)sbuffer.Mem();
					pin->GetStringValue(buffer);
					prop->updateValue(TNL::StringPtr(buffer),0);
					msg->getParameters().push_back(prop);
					prop->setFlags(E_DP_NEEDS_SEND);
				}

				break;
			}
		case vtFLOAT:
			{
				xDistributedPoint1F * prop  = new xDistributedPoint1F(dInfo,30,3000);
				if (prop)
				{
					float vvalue;
					beh->GetInputParameterValue(i,&vvalue);
					bool update = prop->updateValue(vvalue,0);
					msg->getParameters().push_back(prop);
					prop->setFlags(E_DP_NEEDS_SEND);
				}
				break;
			}
		case vtINTEGER:
			{

				xDistributedInteger * prop  = new xDistributedInteger(dInfo,30,3000);
				if (prop)
				{
					int vvalue;
					beh->GetInputParameterValue(i,&vvalue);
					bool update = prop->updateValue(vvalue,0);
					msg->getParameters().push_back(prop);
					prop->setFlags(E_DP_NEEDS_SEND);
				}
				break;
			}
		case vtVECTOR:
			{

				xDistributedPoint3F * prop = new xDistributedPoint3F(dInfo,30,3000);
				if (prop)
				{
					VxVector vvalue;
					beh->GetInputParameterValue(i,&vvalue);
					bool update = prop->updateValue(xMath::getFrom(vvalue),0);
					msg->getParameters().push_back(prop);
					prop->setFlags(E_DP_NEEDS_SEND);
				}
				break;
			}
		case vtVECTOR2D:
			{

				xDistributedPoint2F * prop  = new xDistributedPoint2F(dInfo,30,3000);
				if (prop)
				{
					Vx2DVector vvalue;
					beh->GetInputParameterValue(i,&vvalue);
					bool update = prop->updateValue(xMath::getFrom(vvalue),0);
					msg->getParameters().push_back(prop);
					prop->setFlags(E_DP_NEEDS_SEND);
				}
				break;
			}
		case vtQUATERNION:
			{
				xDistributedQuatF * prop  = new xDistributedQuatF(dInfo,30,3000);
				if (prop)
				{
					VxQuaternion vvalue;
					beh->GetInputParameterValue(i,&vvalue);
					bool update = prop->updateValue(xMath::getFrom(vvalue),0);
					msg->getParameters().push_back(prop);
					prop->setFlags(E_DP_NEEDS_SEND);
				}
				break;
			}

		case vtUNKNOWN:
			{
				bbError(E_NWE_INVALID_PARAMETER);
				break;
			}

		default :
			bbError(E_NWE_INVALID_PARAMETER);
			return CKBR_BEHAVIORERROR;
		}

	}
	

	enableFlag(msg->getFlags(),E_MF_OUTGOING);

	mInterface->addMessage(msg);
	
	beh->ActivateOutput(0);

	return CKBR_OK;
}

CKERROR NMSendCB(const CKBehaviorContext& behcontext)
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