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
CKObjectDeclaration	*FillBehaviorGetOutgoingUserUserDecl();
CKERROR CreateGetOutgoingUserUserProto(CKBehaviorPrototype **);
int GetOutgoingUserUser(const CKBehaviorContext& behcontext);
CKERROR GetOutgoingUserUserCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1




CKObjectDeclaration	*FillBehaviorGetOutgoingUserUserDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("NGetOutgoingUser");	
	od->SetDescription("Gets information about the user joining the current session");
	
	od->SetCategory("TNL/User Management");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x7d5d07b6,0xa295e91));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Günter Baumgart");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateGetOutgoingUserUserProto);
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
	BB_IP_CONNECTION_ID,
};

typedef enum BB_OT
{
	BB_O_OUT,
	BB_O_OUTGOING,
	BB_O_ERROR,

};

typedef enum BB_OP
{
	BB_OP_USER_ID,
	BB_OP_ERROR
};


CKERROR CreateGetOutgoingUserUserProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("NGetOutgoingUser");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareInput("Stop");
	proto->DeclareInput("Next");

	proto->DeclareOutput("Out");
	proto->DeclareOutput("Outgoing");
	

	proto->DeclareOutput("Error");

	
	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	
	proto->DeclareOutParameter("User ID", CKPGUID_INT, "-1");
	
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "Ok");

	

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS  ));
	proto->SetFunction(GetOutgoingUserUser);
	proto->SetBehaviorCallbackFct(GetOutgoingUserUserCB);

	*pproto = proto;
	return CK_OK;

}


int GetOutgoingUserUser(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());
	using namespace vtTools::BehaviorTools;

	
	int connectionID=-1;
	beh->GetInputParameterValue(0,&connectionID);
	
	XString userName((CKSTRING) beh->GetInputParameterReadDataPtr(1));
	
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	bbNoError(E_NWE_OK);

	beh->ActivateInput(0,FALSE);
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

	xDistributedClient *myClient = cin->getMyClient();
	if (!myClient)
	{

		bbError(E_NWE_INTERN);
		return 0;
	}

	/*
	xDistributedSession *session  = cin->getCurrentSession();
	if (!session)
	{
		bbError(E_NWE_NO_SUCH_SESSION);
		return 0;
	}

	if (!myClient->getClientFlags().test(1 << E_CF_SESSION_JOINED))
	{
		bbError(E_NWE_NO_SESSION);
		return 0;
	}
	*/



	//////////////////////////////////////////////////////////////////////////
	//for the case we have real disconnect of a client :
	TNL::Vector<xClientInfo*>& clientTable = cin->getClientInfoTable();
	for (int i  = 0  ; i < clientTable.size() ; i++  )
	{
		xClientInfo *cInfo = clientTable[i];
		if (cInfo->userFlag == USER_DELETED)
		{
			int userID = cInfo->userID;
			beh->SetOutputParameterValue(0,&userID);
			beh->ActivateOutput(1);
			clientTable.erase(i);
			xLogger::xLog(ELOGINFO,E_LI_CLIENT,"Outgoing user  by terminate %d",userID);
			//XLOG_BB_INFO;
			return CKBR_ACTIVATENEXTFRAME;
		}
	}

	

	//////////////////////////////////////////////////////////////////////////
	//for the case, some body just left a session : 
	

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

					xDistributedClient *myClient = cin->getMyClient();
					xDistributedSession *session  = cin->getCurrentSession();

					if (myClient && session  )
					{
						if (distClient->getUserID()  != myClient->getUserID() && 
							isFlagOn(distClient->getClientFlags(),E_CF_DELETING) &&
							(distClient->getSessionID() == myClient->getSessionID() || 
							isFlagOn(distClient->getClientFlags(),E_CF_SESSION_DESTROYED) ||
							isFlagOn(distClient->getClientFlags(),E_CF_REMOVED )) 
							)
//							isFlagOn(myClient->getClientFlags(),E_CF_SESSION_JOINED) )
						{

							if (isFlagOn(distClient->getClientFlags(),E_CF_SESSION_DESTROYED) || isFlagOn(distClient->getClientFlags(),E_CF_REMOVED))
							{
								distClient->setSessionID(-1);
							}
							

							//xLogger::xLog(ELOGINFO,E_LI_CLIENT,"Outgoing user  by session %d",distClient->getUserID());
							//XLOG_BB_INFO;
							

//							xLogger::xLog(ELOGINFO,XL_START,"outgoing user %d | %d \t s: %d | %d ",distClient->getUserID() ,distClient->getSessionID(),myClient->getUserID(),myClient->getSessionID() );
							disableFlag(distClient->getClientFlags(),E_CF_DELETING);
							disableFlag(distClient->getClientFlags(),E_CF_SESSION_DESTROYED);
							enableFlag(distClient->getClientFlags(),E_CF_DELETED);
							int userID = distClient->getUserID();
							beh->SetOutputParameterValue(0,&userID);
							beh->ActivateOutput(1);
							return CKBR_ACTIVATENEXTFRAME;
						}
					}
				}
			}
		}
		begin++;
	}
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	if (beh->IsInputActive(1))
	{
		beh->ActivateInput(1,FALSE);
		beh->ActivateOutput(0);
		return CKBR_OK;
	}
	return CKBR_ACTIVATENEXTFRAME;
}

CKERROR GetOutgoingUserUserCB(const CKBehaviorContext& behcontext)
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