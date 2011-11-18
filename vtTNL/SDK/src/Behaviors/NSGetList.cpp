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


CKObjectDeclaration	*FillBehaviorNSGetListObjectDecl();
CKERROR CreateNSGetListObjectProto(CKBehaviorPrototype **);
int NSGetListObject(const CKBehaviorContext& behcontext);
CKERROR NSGetListObjectCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 5
#define BEH_OUT_MIN_COUNT 10
#define BEH_OUT_MAX_COUNT 1

typedef enum BB_IT
{
	BB_IT_IN,
	BB_IT_NEXT
};

typedef enum BB_INPAR
{
	BB_IP_CONNECTION_ID,
	BB_IP_SESSION_TYPE,
	BB_IP_LIST_FULL,
	BB_IP_LIST_PRIVATE,
	BB_IP_LIST_LOCKED,
};

typedef enum BB_OT
{
	BB_O_OUT,
	BB_O_WAITING,
	BB_O_ERROR,
	BB_O_SESSION
};
typedef enum BB_OP
{
	BB_OP_SESSIONS,
	BB_OP_SESSION_ID,
    BB_OP_SESSION_NAME,
	BB_OP_SESSION_MASTER,
	BB_OP_SESSION_USERS,
	BB_OP_SESSION_MAX_PLAYERS,
	BB_OP_SESSION_FULL,
	BB_OP_SESSION_PRIVATE,
	BB_OP_SESSION_LOCKED,
	BB_OP_ERROR

};


#include "xLogger.h"
#include "vtLogTools.h"


CKObjectDeclaration	*FillBehaviorNSGetListObjectDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("NSGetList");	
	od->SetDescription("Iterates through all server sessions");

	od->SetCategory("TNL/Sessions");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x16760924,0x29200470));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateNSGetListObjectProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


CKERROR CreateNSGetListObjectProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("NSGetList");
	if(!proto) return CKERR_OUTOFMEMORY;



	proto->DeclareInput("In");
	proto->DeclareInput("Next");

	proto->DeclareOutput("Out");
	proto->DeclareOutput("Waiting For Answer");
	proto->DeclareOutput("Error");
	proto->DeclareOutput("Session");


	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	proto->DeclareInParameter("Session Type", CKPGUID_INT, "-1");
	proto->DeclareInParameter("List Full Sessions", CKPGUID_BOOL, "TRUE");
	proto->DeclareInParameter("List Private Sessions", CKPGUID_BOOL, "TRUE");
	proto->DeclareInParameter("List Locked Sessions", CKPGUID_BOOL, "TRUE");
	



	proto->DeclareOutParameter("Sessions", CKPGUID_INT, "-1");
	proto->DeclareOutParameter("Session ID", CKPGUID_INT, "-1");
	proto->DeclareOutParameter("Session Name", CKPGUID_STRING, "");
	proto->DeclareOutParameter("Session Master", CKPGUID_STRING, "");
	proto->DeclareOutParameter("Users", CKPGUID_INT, "-1");
	proto->DeclareOutParameter("Max Players", CKPGUID_INT, "-1");
	proto->DeclareOutParameter("Full", CKPGUID_BOOL, "FALSE");
	proto->DeclareOutParameter("Private", CKPGUID_BOOL, "FALSE");
	proto->DeclareOutParameter("Locked", CKPGUID_BOOL, "FALSE");
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "Ok");
	
	proto->DeclareLocalParameter("current result", CKPGUID_POINTER, "0");


	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS));
	proto->SetFunction(NSGetListObject);
	proto->SetBehaviorCallbackFct(NSGetListObjectCB);

	*pproto = proto;
	return CK_OK;

}

typedef std::vector<xDistributedSession*>xSessions;
#include "vtLogTools.h"



int NSGetListObject(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;

	
	//////////////////////////////////////////////////////////////////////////
	//connection id + session name : 

	using namespace vtTools::BehaviorTools;
	
	int connectionID = GetInputParameterValue<int>(beh,0);
	
	
	//////////////////////////////////////////////////////////////////////////
	//network ok  ? 
	xNetInterface *cin  = GetNM()->GetClientNetInterface();
	if (!cin)
	{
		bbError(E_NWE_NO_CONNECTION);
		return 0;
	}

	
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	if (beh->IsInputActive(0))
	{
		bbNoError(E_NWE_OK);
		beh->ActivateInput(0,FALSE);

		//////////////////////////////////////////////////////////////////////////
		//we reset our session counter
		int sessionIndex=-1;
		beh->SetOutputParameterValue(BB_OP_SESSIONS,&sessionIndex);


		xSessions *sResults = NULL;
		beh->GetLocalParameterValue(0,&sResults);
		if (!sResults)
		{
			sResults = new xSessions();
		}else
			sResults->clear();

		beh->SetLocalParameterValue(0,&sResults);


		int sessionType = GetInputParameterValue<int>(beh,BB_IP_SESSION_TYPE);
		int listFull  = GetInputParameterValue<int>(beh,BB_IP_LIST_FULL);
		int listPrivate = GetInputParameterValue<int>(beh,BB_IP_LIST_PRIVATE);
		int listLocked  = GetInputParameterValue<int>(beh,BB_IP_LIST_LOCKED);

		//////////////////////////////////////////////////////////////////////////
		//we iterate though all session objects and add it in a filtered temporary array : 
		ISession *sInterface =  cin->getSessionInterface();
		IDistributedObjects *doInterface  = cin->getDistObjectInterface();
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
					if (_class->getEnitityType() == E_DC_BTYPE_SESSION )
					{
						xDistributedSession *session  =  static_cast<xDistributedSession*>(dobj);
						if (session && session->getSessionFlags().test(1<< E_SF_COMPLETE ) )
						{
							if ( session->isPrivate() && !listPrivate)
							{
								begin++;
								continue;
							}
							if ( session->isLocked() && !listLocked)
							{
								begin++;
								continue;
							}
							if ( session->isFull() && !listFull)
							{
								begin++;
								continue;
							}

							sResults->push_back(session);
                            
						}
					}
				}
			}
			begin++;
		}
		if (sResults->size())
		{
			beh->ActivateInput(1);
		}else
		{
			beh->ActivateOutput(BB_O_OUT);
			return 0;
		}
	}

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	if (beh->IsInputActive(1))
	{
		beh->ActivateInput(1,FALSE);
	
		
		int currentIndex=0;	CKParameterOut *pout = beh->GetOutputParameter(BB_OP_SESSIONS);		pout->GetValue(&currentIndex);
		currentIndex++;
		

		
		xSessions *sResults = NULL;	beh->GetLocalParameterValue(0,&sResults);
		if (!sResults)		{			beh->ActivateOutput(BB_O_OUT);			return 0;		}

		if (currentIndex>=sResults->size())
		{
			sResults->clear();
			beh->ActivateOutput(BB_O_OUT);
			return 0;
		}

		xDistributedSession * session =  sResults->at(currentIndex);
		if (session!=NULL)
		{
			
			int sIndex = currentIndex+1;
			beh->SetOutputParameterValue(BB_OP_SESSIONS,&sIndex);



			/************************************************************************/
			/* output bas data  :                                                                      */
			/************************************************************************/
			int sessionID =  session->getSessionID();beh->SetOutputParameterValue(BB_OP_SESSION_ID,&sessionID);
			
			CKSTRING sesssionName  = const_cast<char*>(session->GetName().getString());
			CKParameterOut *poutName = beh->GetOutputParameter(BB_OP_SESSION_NAME);
			poutName->SetStringValue(sesssionName);

			int maxPlayers  =  session->getMaxUsers();
			beh->SetOutputParameterValue(BB_OP_SESSION_MAX_PLAYERS,&maxPlayers);

			int isFull  = session->isFull();
			beh->SetOutputParameterValue(BB_OP_SESSION_FULL,&isFull);

			int isPrivate  = session->isPrivate();
			beh->SetOutputParameterValue(BB_OP_SESSION_PRIVATE,&isPrivate);

			int isLocked  = session->isLocked();
			beh->SetOutputParameterValue(BB_OP_SESSION_LOCKED,&isLocked);

			int numUsers = session->getNumUsers();
			beh->SetOutputParameterValue(BB_OP_SESSION_USERS,&numUsers);

			beh->SetOutputParameterValue(BB_OP_SESSIONS,&currentIndex);
			beh->ActivateOutput(BB_O_SESSION);


			/************************************************************************/
			/*output additional properties                                                                       */
			/************************************************************************/

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
		}
	}
	return 0;
}

CKERROR NSGetListObjectCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;

	if (behcontext.CallbackMessage==CKM_BEHAVIOREDITED) 
	{

	}

	return CKBR_OK;
}
