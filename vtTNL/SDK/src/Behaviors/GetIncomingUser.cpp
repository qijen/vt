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
#include <vtTools.h>
#include "xLogger.h"
#include "vtLogTools.h"


CKObjectDeclaration	*FillBehaviorGetIncomingUserDecl();
CKERROR CreateGetIncomingUserProto(CKBehaviorPrototype **);
int GetIncomingUser(const CKBehaviorContext& behcontext);
CKERROR GetIncomingUserCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1




CKObjectDeclaration	*FillBehaviorGetIncomingUserDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("NIncomingUser");	
	od->SetDescription("User enters a session");
	
	od->SetCategory("TNL/User Management");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x7a770be7,0x77130778));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("G�nter Baumgart");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateGetIncomingUserProto);
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
	BB_O_INCOMING,
	BB_O_ERROR,

};

typedef enum BB_OP
{
	BB_OP_USER_ID,
	BB_OP_USER_NAME,
	BB_OP_ERROR
};

CKERROR CreateGetIncomingUserProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("NIncomingUser");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareInput("Stop");
	proto->DeclareInput("Next");

	proto->DeclareOutput("Out");
	proto->DeclareOutput("Incoming");

	proto->DeclareOutput("Error");

	
	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	
	proto->DeclareOutParameter("User ID", CKPGUID_INT, "-1");
	proto->DeclareOutParameter("User Name", CKPGUID_STRING, "-1");
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "-1");

	

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS  ));
	proto->SetFunction(GetIncomingUser);
	proto->SetBehaviorCallbackFct(GetIncomingUserCB);

	*pproto = proto;
	return CK_OK;

}


int GetIncomingUser(const CKBehaviorContext& behcontext)
{
	using namespace vtTools::BehaviorTools;

	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());

	
	int connectionID=-1;
	beh->GetInputParameterValue(0,&connectionID);
	
	XString userName((CKSTRING) beh->GetInputParameterReadDataPtr(1));
	
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	beh->ActivateInput(0,FALSE);
	bbNoError(E_NWE_OK);

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


	IDistributedObjects *doInterface  = cin->getDistObjectInterface();
	vtConnection *con = cin->getConnection();
	xDistributedObjectsArrayType *distObjects = cin->getDistributedObjects();
	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();
	while (begin!=end)
	{
		xDistributedObject* dobj = *begin;
		if (dobj)
		{
			xDistributedClass *_class = dobj->getDistributedClass();

			if (_class)
			{
				if (_class->getEnitityType() == E_DC_BTYPE_CLIENT)
				{
					xDistributedClient *client  = static_cast<xDistributedClient*>(dobj);
					if (client)
					{
						xDistributedClient *myClient = cin->getMyClient();
						xDistributedSession *session  = cin->getCurrentSession();

						if ( isFlagOn(client->getClientFlags(),E_CF_ADDING) && client->getUserID() != con->getUserID()  )
						{

							disableFlag(client->getClientFlags(),E_CF_ADDING);
							enableFlag(client->getClientFlags(), E_CF_ADDED);
							
							if (isFlagOn(myClient->getClientFlags(),E_CF_SESSION_JOINED))
							{
								SetOutputParameterValue<int>(beh,BB_OP_USER_ID,client->getUserID());
								SetOutputParameterValue<const char*>(beh,BB_OP_USER_NAME,client->getUserName().getString());
								
								//xLogger::xLog(ELOGINFO,E_LI_CLIENT,"Incoming user  %d",client->getUserID());
								//XLOG_BB_INFO;
								beh->ActivateOutput(1);
								return CKBR_ACTIVATENEXTFRAME;
							}
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

CKERROR GetIncomingUserCB(const CKBehaviorContext& behcontext)
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