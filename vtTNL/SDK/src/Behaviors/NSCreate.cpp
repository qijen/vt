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


CKObjectDeclaration	*FillBehaviorNSCreateObjectDecl();
CKERROR CreateNSCreateObjectProto(CKBehaviorPrototype **);
int NSCreateObject(const CKBehaviorContext& behcontext);
CKERROR NSCreateObjectCB(const CKBehaviorContext& behcontext);


#include "xLogger.h"
#include "vtLogTools.h"


#define BEH_IN_INDEX_MIN_COUNT 5
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1


typedef enum BB_INPAR
{
	BB_IP_CONNECTION_ID,
	BB_IP_SESSION_TYPE,
	BB_IP_SESSION_NAME,
	BB_IP_MAX_PLAYERS,
	BB_IP_PASSWORD
};

typedef enum BB_OT
{

	BB_O_CREATED,
	BB_O_WAITING,
	BB_O_ERROR
};
typedef enum BB_OP
{

	BB_OP_SESSION_ID,
	BB_OP_MAX_PLAYERS,
	BB_OP_ERROR
};

CKObjectDeclaration	*FillBehaviorNSCreateObjectDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("NSCreate");	
	od->SetDescription("Creates a session");

	od->SetCategory("TNL/Sessions");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x5efe2bbd,0x20525241));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateNSCreateObjectProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


CKERROR CreateNSCreateObjectProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("NSCreate");
	if(!proto) return CKERR_OUTOFMEMORY;



	proto->DeclareInput("Create");

	proto->DeclareOutput("Created");
	proto->DeclareOutput("Waiting For Answer");
	proto->DeclareOutput("Error");


	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	proto->DeclareInParameter("Session Type", CKPGUID_INT, "-1");
	proto->DeclareInParameter("Session Name", CKPGUID_STRING, "test");
	proto->DeclareInParameter("Maximum Players", CKPGUID_INT, "3");
	proto->DeclareInParameter("Password", CKPGUID_STRING, "pass");



	proto->DeclareOutParameter("Session ID", CKPGUID_INT, "0");
	proto->DeclareOutParameter("Maximum Players", CKPGUID_INT, "0");
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "Ok");
	

	proto->DeclareSetting("Long Session", CKPGUID_BOOL, "TRUE");
	proto->DeclareLocalParameter("Creation Status", CKPGUID_INT, "0");

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETERINPUTS));
	proto->SetFunction(NSCreateObject);
	proto->SetBehaviorCallbackFct(NSCreateObjectCB);

	*pproto = proto;
	return CK_OK;

}


int NSCreateObject(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;

	//////////////////////////////////////////////////////////////////////////
	//connection id + session name : 

	using namespace vtTools::BehaviorTools;
	
	int connectionID = GetInputParameterValue<int>(beh,0);
	CKSTRING sessionName = GetInputParameterValue<CKSTRING>(beh,BB_IP_SESSION_NAME);

	//////////////////////////////////////////////////////////////////////////
	//network ok  ? 
	xNetInterface *cin  = GetNM()->GetClientNetInterface();
	if (!cin)
	{
		bbError(E_NWE_NO_CONNECTION);
		return 0;
	}
	


	//////////////////////////////////////////////////////////////////////////
	//we create one session class per session object : 
	ISession *sInterface =  cin->getSessionInterface();
	xDistributedSession *session = sInterface->get(sessionName);
	IDistributedClasses *cInterface = cin->getDistributedClassInterface();
	
	int creationStatus = E_DO_CREATION_INCOMPLETE;
	beh->SetLocalParameterValue(1,&creationStatus);

    
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	if (beh->IsInputActive(0))
	{
		beh->ActivateInput(0,FALSE);

		bbNoError(E_NWE_OK);
		xDistributedClient *myClient  = cin->getMyClient();
		if (!myClient)
		{
			bbError(E_NWE_INTERN);
				return 0;
		}
		if (isFlagOn(myClient->getClientFlags(),E_CF_SESSION_JOINED ))
		{
			bbError(E_NWE_SESSION_ALREADY_JOINED);
			return 0;
		}

		//we attach the  building blocks parameter 
		CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());
		for (int i = BEH_IN_INDEX_MIN_COUNT  ; i < beh->GetInputParameterCount() ; i++ )
		{

			CKParameterIn *ciIn = beh->GetInputParameter(i);
			int sType = vtTools::ParameterTools::GetVirtoolsSuperType(ctx,pam->ParameterTypeToGuid(ciIn->GetType()));
			if (xDistTools::SuperTypeToValueType(sType)  == E_DC_PTYPE_UNKNOWN)
			{
				bbError(E_NWE_INVALID_PARAMETER);
				return 0;

			}

		}

		/************************************************************************/
		/*			SESSION CLASS CREATION !															                                                                     */
		/************************************************************************/

		//pickup data

		int sessionType = GetInputParameterValue<int>(beh,BB_IP_SESSION_TYPE);
		int MaxPlayers  = GetInputParameterValue<int>(beh,BB_IP_MAX_PLAYERS);
		CKSTRING password = GetInputParameterValue<CKSTRING>(beh,BB_IP_PASSWORD);

		//we create a session class, according sessions name and attached user parameters
		xDistributedSessionClass*sessionClass = (xDistributedSessionClass*)cInterface->get(sessionName);
		if (!sessionClass)
		{
			sessionClass = (xDistributedSessionClass*)cInterface ->createClass(sessionName,E_DC_BTYPE_SESSION);
			
			//we enable all native properties : 
			sessionClass->addProperty(E_DC_S_NP_MAX_USERS,E_PTYPE_RELIABLE);
			if (strlen(password))
				sessionClass->addProperty(E_DC_S_NP_PASSWORD,E_PTYPE_RELIABLE);
			sessionClass->addProperty(E_DC_S_NP_TYPE,E_PTYPE_RELIABLE);
			sessionClass->addProperty(E_DC_S_NP_LOCKED,E_PTYPE_RELIABLE);
			sessionClass->addProperty(E_DC_S_NP_NUM_USERS,E_PTYPE_RELIABLE);

            
			
			for (int i = BEH_IN_INDEX_MIN_COUNT  ; i < beh->GetInputParameterCount() ; i++ )
			{
				CKParameterIn *ciIn = beh->GetInputParameter(i);
				int sType = vtTools::ParameterTools::GetVirtoolsSuperType(ctx,pam->ParameterTypeToGuid(ciIn->GetType()));
				sessionClass->addProperty(ciIn->GetName(),xDistTools::SuperTypeToValueType(sType),E_PTYPE_RELIABLE);
			}
		}
		//now we deploy it on the server : 
		cInterface->deployClass(sessionClass);

		
		/************************************************************************/
		/*			SESSION OBJECT CREATION					                                                                       */
		/************************************************************************/
	
		//create a session via rpc : 

		sInterface->create(sessionName,MaxPlayers,password,sessionType);

		return CKBR_ACTIVATENEXTFRAME;
	}


	/************************************************************************/
	/*																		                                                                       */
	/************************************************************************/
	if ( session 
		&& session->getSessionFlags().test(1<< E_SF_INCOMPLETE) 
		&& session->getUserID() == cin->getConnection()->getUserID()
		&& !session->getSessionFlags().test( 1 << E_SF_POPULATE_PARAMETERS )
		)
	{
		

		session->getSessionFlags().set( 1 << E_SF_POPULATE_PARAMETERS );

		//////////////////////////////////////////////////////////////////////////
		int MaxPlayers  = GetInputParameterValue<int>(beh,BB_IP_MAX_PLAYERS);
		xDistributedInteger *pMaxUsers = (xDistributedInteger *)session->getProperty(E_DC_S_NP_MAX_USERS);
		pMaxUsers->updateValue(MaxPlayers,0);
		pMaxUsers->setFlags(E_DP_NEEDS_SEND);
		//////////////////////////////////////////////////////////////////////////
		CKSTRING password = GetInputParameterValue<CKSTRING>(beh,BB_IP_PASSWORD);
		xDistributedString *pPass= (xDistributedString*)session->getProperty(E_DC_S_NP_PASSWORD);
		if (pPass)
		{
			pPass->updateValue(xNString(password),0);
			pPass->setFlags(E_DP_NEEDS_SEND);
		}
		//////////////////////////////////////////////////////////////////////////
		int sessionType  = GetInputParameterValue<int>(beh,BB_IP_SESSION_TYPE);
		xDistributedInteger *pType= (xDistributedInteger *)session->getProperty(E_DC_S_NP_TYPE);
		pType->updateValue(sessionType,0);
		pType->setFlags(E_DP_NEEDS_SEND);
		//////////////////////////////////////////////////////////////////////////
		int sessionLocked = 0;
		xDistributedInteger *pLocked= (xDistributedInteger *)session->getProperty(E_DC_S_NP_LOCKED);
		pLocked->updateValue(sessionLocked,0);
		pLocked->setFlags(E_DP_NEEDS_SEND);
		//////////////////////////////////////////////////////////////////////////

		if ( beh->GetInputParameterCount() > BEH_IN_INDEX_MIN_COUNT )
		{
    		xDistributedSessionClass *_class  = (xDistributedSessionClass*)session->getDistributedClass();

			CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());

			for (int i = BEH_IN_INDEX_MIN_COUNT  ; i < beh->GetInputParameterCount() ; i++ )
			{
				CKParameterIn *ciIn = beh->GetInputParameter(i);
				CKParameterType pType  = ciIn->GetType();
				int sType = vtTools::ParameterTools::GetVirtoolsSuperType(ctx,pam->ParameterTypeToGuid(pType));	
				xDistributedPropertyArrayType &props = *session->getDistributedPorperties();

				int propID = _class->getInternalUserFieldIndex(i - BEH_IN_INDEX_MIN_COUNT);
				int startIndex = _class->getFirstUserField();
				int pSize = props.size();
				if (propID==-1 ||  propID > props.size() )
				{
					beh->ActivateOutput(1);
					return 0;
				}

				xDistributedProperty *prop  = props[propID];
				if (prop)
				{



					//we set the update flag in the prop by hand : 
					xDistributedPropertyInfo*propInfo  = prop->getPropertyInfo();
					if (propInfo)
					{
											
						int dType = xDistTools::ValueTypeToSuperType(propInfo->mValueType);
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
										VxVector vvalue;
										beh->GetInputParameterValue(i,&vvalue);
										bool update = dpoint3F->updateValue(xMath::getFrom(vvalue),currentTime);
									}
									break;
								}
								case E_DC_PTYPE_FLOAT:
								{

									xDistributedPoint1F * dpoint3F  = (xDistributedPoint1F*)prop;
									if (dpoint3F)
									{
										float vvalue;
										beh->GetInputParameterValue(i,&vvalue);
										bool update = dpoint3F->updateValue(vvalue,currentTime);
									}
									break;
								}
								case E_DC_PTYPE_2DVECTOR:
								{

									xDistributedPoint2F * dpoint3F  = (xDistributedPoint2F*)prop;
									if (dpoint3F)
									{
										Vx2DVector vvalue;
										beh->GetInputParameterValue(i,&vvalue);
										bool update = dpoint3F->updateValue(xMath::getFrom(vvalue),currentTime);
									}
									break;
								}
								case E_DC_PTYPE_QUATERNION:
								{

									xDistributedQuatF * dpoint3F  = (xDistributedQuatF*)prop;
									if (dpoint3F)
									{
										VxQuaternion vvalue;
										beh->GetInputParameterValue(i,&vvalue);
										bool update = dpoint3F->updateValue(xMath::getFrom(vvalue),currentTime);
									}
									break;
								}
								case E_DC_PTYPE_STRING:
								{
									xDistributedString * dpoint3F  = (xDistributedString*)prop;
									if (dpoint3F)
									{
										CKParameter *pin = beh->GetInputParameter(i)->GetRealSource();
										if (pin)
										{
											VxScratch sbuffer(256);
											CKSTRING buffer = (CKSTRING)sbuffer.Mem();
											pin->GetStringValue(buffer);
											bool update = dpoint3F->updateValue(TNL::StringPtr(buffer),currentTime);

										}

									}
									break;
								}
								case E_DC_PTYPE_INT:
								{
									xDistributedInteger * dpoint3F  = (xDistributedInteger*)prop;
									if (dpoint3F)
									{
										int vvalue;
										beh->GetInputParameterValue(i,&vvalue);
										bool update = dpoint3F->updateValue(vvalue,currentTime);
									}
									break;
								}
							}
							prop->setFlags(E_DP_NEEDS_SEND);
						}
					}
				}
			}
		}
	}


	
	
	/************************************************************************/
	/*           SESSION COMPLETE						                                                           */
	/************************************************************************/
	if ( 
		session 
		&& session->getSessionFlags().test(1<< E_SF_COMPLETE ) 
		&& session->getUserID() == cin->getConnection()->getUserID()		
		)
	{

		session->getSessionFlags().set( 1 << E_SF_INCOMPLETE,false );
		session->getSessionFlags().set( 1 << E_SF_POPULATE_PARAMETERS,false );

		int sessionID = session->getSessionID();
		beh->SetOutputParameterValue(BB_OP_SESSION_ID,&sessionID);

		beh->ActivateOutput(BB_O_CREATED);
		return 0;


	}

	beh->ActivateOutput(BB_O_WAITING);
	return CKBR_ACTIVATENEXTFRAME;

}

CKERROR NSCreateObjectCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;

	if (behcontext.CallbackMessage==CKM_BEHAVIOREDITED) 
	{

	}

	return CKBR_OK;
}
