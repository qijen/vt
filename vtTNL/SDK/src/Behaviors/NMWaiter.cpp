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
#include "IMessages.h"
#include "xLogger.h"
#include "vtLogTools.h"
#include "xMessageTypes.h"
#include "xDistributedClient.h"


CKObjectDeclaration	*FillBehaviorNMWaiterDecl();
CKERROR CreateNMWaiterProto(CKBehaviorPrototype **);
int NMWaiter(const CKBehaviorContext& behcontext);
CKERROR NMWaiterCB(const CKBehaviorContext& behcontext);



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


CKObjectDeclaration	*FillBehaviorNMWaiterDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("NMWaiter");	
	od->SetDescription("Waits for a Message from the Network.");
	
	od->SetCategory("TNL/Message");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x5707744b,0x43b2301c));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateNMWaiterProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


CKERROR CreateNMWaiterProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("NMWaiter");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "-1");
	proto->DeclareInParameter("Message", CKPGUID_MESSAGE, "OnClick");

	proto->DeclareInput("In");
	proto->DeclareInput("Stop");
	proto->DeclareInput("Next");

	proto->DeclareOutput("Out");
	proto->DeclareOutput("Message");
	proto->DeclareOutput("Error");

	
	proto->DeclareOutParameter("Sender ID", CKPGUID_INT, "0");
	proto->DeclareOutParameter("Private", CKPGUID_BOOL, "false");
	proto->DeclareOutParameter("Error", CKPGUID_INT);

	

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS  ));
	proto->SetFunction(NMWaiter);
	proto->SetBehaviorCallbackFct(NMWaiterCB);

	*pproto = proto;
	return CK_OK;

}


int NMWaiter(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());

	//////////////////////////////////////////////////////////////////////////
	//connection id + session name : 

	using namespace vtTools::BehaviorTools;

	int connectionID = GetInputParameterValue<int>(beh,BB_IP_CONNECTION_ID);
	int Msg=-1;
	beh->GetInputParameterValue(BB_IP_MESSAGE,&Msg);
	XString msgType(mm->GetMessageTypeName(Msg));

	//////////////////////////////////////////////////////////////////////////
	//network ok  ? 
	xNetInterface *cin  = GetNM()->GetClientNetInterface();
	if (!cin)
	{
				
		//bbError(E_NWE_NO_CONNECTION);
		return CKBR_ACTIVATENEXTFRAME;
	}
	if (!cin->getMyClient())
	{
		//bbError(E_NWE_INTERN);
		//Error(beh,"No client object created",BB_OP_SESSION_ERROR,TRUE,BB_O_ERROR);
		return 0;
	}

	ISession *sInterface =  cin->getSessionInterface();
	IMessages *mInterface = cin->getMessagesInterface();


	xDistributedClient *myClient = cin->getMyClient();
	if (!myClient)
	{
		//bbError(E_NWE_INTERN);
		return CKBR_ACTIVATENEXTFRAME;
	}

	/*
	xDistributedSession *session  = sInterface->get(myClient->getSessionID());
	if (!session)
	{

		bbError(E_NWE_NO_SESSION);
		return 0;
	}*/

	if (beh->IsInputActive(BB_IT_STOP))
	{
		beh->ActivateInput(BB_IT_STOP,FALSE);
		beh->ActivateOutput(BB_O_OUT);
		return CKBR_OK;
	}


		xMessage *msg = mInterface->getNextInMessage(msgType.CStr());
		if (!msg)return CKBR_ACTIVATENEXTFRAME;

		if ( beh->GetOutputParameterCount() > BEH_OUT_MIN_COUNT )
		{

			CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());

			for (int i = BEH_OUT_MIN_COUNT  ; i < beh->GetOutputParameterCount() ; i++ )
			{
				CKParameterOut *ciIn = beh->GetOutputParameter(i);
				CKParameterType pType  = ciIn->GetType();
				int sType = vtTools::ParameterTools::GetVirtoolsSuperType(ctx,pam->ParameterTypeToGuid(pType));	

				int propID = i - BEH_OUT_MIN_COUNT;
				if (propID>msg->getNumParameters() )
				{
					goto MESSAGE_END;
				}

				xDistributedProperty *prop  = msg->getParameters().at(propID);
				if (!prop) goto MESSAGE_END;
				
				xDistributedPropertyInfo*propInfo  = prop->getPropertyInfo();
				if (!propInfo) goto MESSAGE_END;
				if (xDistTools::ValueTypeToSuperType(propInfo->mValueType) !=sType )
					continue;

				switch(propInfo->mValueType)
				{
					case E_DC_PTYPE_STRING:
					{
						xDistributedString * propValue  = (xDistributedString*)prop;
						if (propValue)
						{
							TNL::StringPtr ovalue = propValue->mCurrentValue;
							CKParameterOut *pout = beh->GetOutputParameter(i);
							XString errorMesg(ovalue.getString());
							pout->SetStringValue(errorMesg.Str());
						}
						break;
					}
					case E_DC_PTYPE_FLOAT:
					{
						xDistributedPoint1F * dpoint3F  = (xDistributedPoint1F*)prop;
						if (dpoint3F)
						{
							float vvalue  = dpoint3F->mLastServerValue;
							beh->SetOutputParameterValue(i,&vvalue);
						}
						break;
					}
					case E_DC_PTYPE_3DVECTOR:
					{
						xDistributedPoint3F * dpoint3F  = (xDistributedPoint3F*)prop;
						if (dpoint3F)
						{
							VxVector vvalue  = xMath::getFrom(dpoint3F->mLastServerValue);
							beh->SetOutputParameterValue(i,&vvalue);
						}
						break;
					}
					case E_DC_PTYPE_2DVECTOR:
					{

						xDistributedPoint2F * dpoint3F  = (xDistributedPoint2F*)prop;
						if (dpoint3F)
						{
							Vx2DVector vvalue  = xMath::getFrom(dpoint3F->mLastServerValue);
							beh->SetOutputParameterValue(i,&vvalue);
						}
						break;
					}

					case E_DC_PTYPE_INT:
					{
						xDistributedInteger * dpoint3F  = (xDistributedInteger*)prop;
						if (dpoint3F)
						{
							int ovalue = dpoint3F->mLastServerValue;
							beh->SetOutputParameterValue(i,&ovalue);
						}
						break;
					}
					case E_DC_PTYPE_QUATERNION:
					{

						xDistributedQuatF * dpoint3F  = (xDistributedQuatF*)prop;
						if (dpoint3F)
						{
							VxQuaternion vvalue  = xMath::getFrom(dpoint3F->mLastServerValue);
							beh->SetOutputParameterValue(i,&vvalue);
						}
						break;
					}
					case E_DC_PTYPE_UNKNOWN:
						{

							bbError(E_NWE_INVALID_PARAMETER);
							break;
						}
					default :
						bbError(E_NWE_INVALID_PARAMETER);
						return CKBR_BEHAVIORERROR;
				}
				
			}
		}
        
		goto MESSAGE_END;

		MESSAGE_END:
		{

			//IDistributedObjects *doInterface  = cin->getDistObjectInterface();
			//xDistributedClient *dstClient = static_cast<xDistributedClient*>(doInterface->get(msg->getSrcUser()));
			//if (dstClient)
			//{
				SetOutputParameterValue<int>(beh,BB_OP_SENDER_ID,msg->getSrcUser());

			//}

			enableFlag(msg->getFlags(),E_MF_DELETED); 
			beh->ActivateOutput(BB_O_MESSAGE);

		}

	return CKBR_ACTIVATENEXTFRAME;
}

CKERROR NMWaiterCB(const CKBehaviorContext& behcontext)
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