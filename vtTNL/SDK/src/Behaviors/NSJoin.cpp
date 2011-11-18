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

CKObjectDeclaration	*FillBehaviorNSJoinObjectDecl();
CKERROR CreateNSJoinObjectProto(CKBehaviorPrototype **);
int NSJoinObject(const CKBehaviorContext& behcontext);
CKERROR NSJoinObjectCB(const CKBehaviorContext& behcontext);



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
	BB_IP_SESSION_ID,
	BB_IP_SESSION_PASSWORD

};

typedef enum BB_OT
{
	BB_O_JOINED,
	BB_O_WAITING,
	BB_O_ERROR
};

typedef enum BB_OP
{
	BB_OP_MASTER_ID,
	BB_OP_ERROR
    
};

CKObjectDeclaration	*FillBehaviorNSJoinObjectDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("NSJoin");	
	od->SetDescription("Joins to a session");

	od->SetCategory("TNL/Sessions");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x5a42f45,0x49eb1634));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateNSJoinObjectProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


CKERROR CreateNSJoinObjectProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("NSJoin");
	if(!proto) return CKERR_OUTOFMEMORY;



	proto->DeclareInput("In");
	
	proto->DeclareOutput("Joined");
	proto->DeclareOutput("Waiting For Answer");
	proto->DeclareOutput("Error");

	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	proto->DeclareInParameter("Session ID", CKPGUID_INT, "-1");
	proto->DeclareInParameter("Password", CKPGUID_STRING, "none");

	proto->DeclareOutParameter("Master ID", CKPGUID_INT, "-1");
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "Ok");
	
	proto->DeclareLocalParameter("state", CKPGUID_INT, "0");


	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS));
	proto->SetFunction(NSJoinObject);
	proto->SetBehaviorCallbackFct(NSJoinObjectCB);

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
int NSJoinObject(const CKBehaviorContext& behcontext)
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

		
	ISession *sInterface =  cin->getSessionInterface();
	int connectionID = GetInputParameterValue<int>(beh,BB_IP_CONNECTION_ID);
	int sessionID = GetInputParameterValue<int>(beh,BB_IP_SESSION_ID);
	XString password ((CKSTRING) beh->GetInputParameterReadDataPtr(BB_IP_SESSION_PASSWORD));
	
	
	xDistributedSession *session  = sInterface->get(sessionID);
	if (!session)
	{
		bbError(E_NWE_NO_SUCH_SESSION);
		return 0;
	}


	xDistributedClient *myClient = cin->getMyClient();
	if (!myClient)
	{
		
		bbError(E_NWE_INTERN);
		return 0;
	}

	

	if (beh->IsInputActive(0))
	{
		beh->ActivateInput(0,FALSE);

		if (session && session->isFull())
		{
			bbError(E_NWE_SESSION_FULL);
			return 0;
		}
		if (session && session->isLocked())
		{
			
			bbError(E_NWE_SESSION_LOCKED);
			return 0;
		}
		
		if (session && session->isPrivate() && strcmp(password.CStr(),session->getPassword() ) )
		{
			bbError(E_NWE_SESSION_WRONG_PASSWORD);
			return 0;
		}
		
		if (isFlagOn(myClient->getClientFlags(),E_CF_SESSION_JOINED ))
		{
			
			bbError(E_NWE_SESSION_ALREADY_JOINED);
			return 0;
		}
        sInterface->joinClient(cin->getMyClient(),sessionID,xNString(password.Str()));
	}


	if (!myClient->getClientFlags().test(1 << E_CF_SESSION_JOINED ))
	{
		bbNoError(E_NWE_OK);
		beh->ActivateOutput(BB_O_WAITING);
	}

	

	if (session && myClient->getClientFlags().test(1 << E_CF_SESSION_JOINED) )
	{


		beh->ActivateOutput(BB_O_JOINED);

		//////////////////////////////////////////////////////////////////////////
		//we tag all existing users as new : 
		xDistributedClient *myClient  = cin->getMyClient();
		IDistributedObjects *doInterface  = cin->getDistObjectInterface();
		xDistributedObjectsArrayType *distObjects = cin->getDistributedObjects();
		xDistObjectIt begin = distObjects->begin();
		xDistObjectIt end = distObjects->end();

		while (begin!=end)
		{
			xDistributedObject *distObject  = *begin;
			if (distObject)
			{
				xDistributedClass *_class = distObject->getDistributedClass();
				if (_class)
				{
					if (_class->getEnitityType() == E_DC_BTYPE_CLIENT )
					{
						xDistributedClient *distClient  = static_cast<xDistributedClient*>(distObject);
						if (distClient && distClient->getSessionID() == sessionID)
						{
							if (isFlagOn(distClient->getClientFlags(),E_CF_SESSION_JOINED))
								enableFlag(distClient->getClientFlags(),E_CF_ADDING);
						}
					}
				}
			}
			begin++;
		}

		//we output all attached parameters:


		
		int sessionMasterID = session->getUserID();
		beh->SetOutputParameterValue(BB_OP_MASTER_ID,&sessionMasterID);


		if ( beh->GetOutputParameterCount() > BEH_OUT_MIN_COUNT )
		{
			xDistributedSessionClass *_class  = (xDistributedSessionClass*)session->getDistributedClass();

			CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());

			for (int i = BEH_OUT_MIN_COUNT  ; i < beh->GetOutputParameterCount() ; i++ )
			{
				CKParameterOut *ciIn = beh->GetOutputParameter(i);
				CKParameterType pType  = ciIn->GetType();
				int sType = vtTools::ParameterTools::GetVirtoolsSuperType(ctx,pam->ParameterTypeToGuid(pType));	
				xDistributedPropertyArrayType &props = *session->getDistributedPorperties();

				int propID = _class->getInternalUserFieldIndex(i - BEH_OUT_MIN_COUNT);
				int startIndex = _class->getFirstUserField();
				int pSize = props.size();
				if (propID==-1 ||  propID > props.size() )
				{
					beh->ActivateOutput(BB_O_ERROR);
					return 0;
				}

				xDistributedProperty *prop  = props[propID];
				if (prop)
				{

					//we set the update flag in the prop by hand : 
					xDistributedPropertyInfo*propInfo  = prop->getPropertyInfo();
					if (propInfo)
					{
						if (xDistTools::ValueTypeToSuperType(propInfo->mValueType) ==sType )
						{
							TNL::U32 currentTime = TNL::Platform::getRealMilliseconds();

							switch(propInfo->mValueType)
							{
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
							case E_DC_PTYPE_UNKNOWN:
								{
									bbError(E_NWE_INVALID_PARAMETER);
									break;
								}
							}
						}
					}
				}
			}
		}
		
		beh->ActivateOutput(BB_O_JOINED);
		return 0;
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
CKERROR NSJoinObjectCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;

	if (behcontext.CallbackMessage==CKM_BEHAVIOREDITED) 
	{

	}

	return CKBR_OK;
}
