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


#include "xLogger.h"
#include "vtLogTools.h"


CKObjectDeclaration	*FillBehaviorUserNameModifiedDecl();
CKERROR CreateUserNameModifiedProto(CKBehaviorPrototype **);
int UserNameModified(const CKBehaviorContext& behcontext);
CKERROR UserNameModifiedCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1




CKObjectDeclaration	*FillBehaviorUserNameModifiedDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("NUserNameModified");	
	od->SetDescription("User changed name");
	
	od->SetCategory("TNL/User Management");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x6f94031a,0x3e3a1714));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Günter Baumgart");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateUserNameModifiedProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}

typedef enum BB_IT
{
	BB_IT_IN,
	BB_IT_STOP,
	BB_IT_NEXT
};

typedef enum BB_INPAR
{
	BB_IP_CONNECTION_ID,
	BB_IP_USER_ID,
};

typedef enum BB_OT
{
	BB_O_OUT,
	BB_O_NAME_CHANGED,
	BB_O_ERROR
};

typedef enum BB_OP
{
	BB_OP_USER_ID,
	BB_OP_USER_NAME,
	BB_OP_ERROR
};

CKERROR CreateUserNameModifiedProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("NUserNameModified");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareInput("Stop");
	proto->DeclareInput("Next");

	proto->DeclareOutput("Out");
	proto->DeclareOutput("User");

	proto->DeclareOutput("Error");

	
	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	
	proto->DeclareOutParameter("User ID", CKPGUID_INT, "-1");
	proto->DeclareOutParameter("User Name", CKPGUID_STRING, "-1");
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "Ok");

	

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS  ));
	proto->SetFunction(UserNameModified);
	proto->SetBehaviorCallbackFct(UserNameModifiedCB);

	*pproto = proto;
	return CK_OK;

}


int UserNameModified(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());

	
	int connectionID=-1;
	beh->GetInputParameterValue(0,&connectionID);
	
	XString userName((CKSTRING) beh->GetInputParameterReadDataPtr(1));
		
	using namespace vtTools::BehaviorTools;
	bbNoError(E_NWE_OK);
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	beh->ActivateInput(0,FALSE);

	
	using namespace vtTools::BehaviorTools;
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

	
	vtConnection *con = cin->getConnection();
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
				if (_class->getEnitityType() == E_DC_BTYPE_CLIENT)
				{
					xDistributedClient *client  = static_cast<xDistributedClient*>(dobj);
					if (client)
					{
						if (client->getUserFlags() == USERNAME_CHANGED && client->getUserID() !=con->getUserID() )
						{
							client->setUserFlags(USER_OK);
							int userID = client->getUserID();
							beh->SetOutputParameterValue(0,&userID);
							CKParameterOut *pout  = beh->GetOutputParameter(1);
							pout->SetStringValue(const_cast<char*>(client->getUserName().getString()));
							xLogger::xLog(ELOGINFO,E_LI_CLIENT,"User %d changed his name to %s",userID,client->getUserName().getString());
							beh->ActivateOutput(1);
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

CKERROR UserNameModifiedCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKBeObject *beo = (CKBeObject *)beh->GetTarget();
	if (!beo) {
		return CKBR_OWNERERROR;
	}
  
	if (behcontext.CallbackMessage==CKM_BEHAVIOREDITED) 
	{
	/*	CKParameterIn* pin = beh->GetInputParameter(0);
		if (!pin) {
			CKBehavior* pbeh = beh;
			XString name;
			while ((pbeh=pbeh->GetParent())) {
				name << pbeh->GetName() << "->";
			}
			name << beh->GetName();

			XString str;
			str << '[' << name << "] : failed to retrieve first input parameter";
			behcontext.Context->OutputToConsole(str.Str(),TRUE);
			return CKBR_PARAMETERERROR;
		}
		
		if (pin->GetGUID()!=CKPGUID_MESSAGE) {
			pin->SetGUID(CKPGUID_MESSAGE,FALSE);

			CKBehavior* pbeh = beh;
			XString name;
			while ((pbeh=pbeh->GetParent())) {
				name << pbeh->GetName() << "->";
			}
			name << beh->GetName();

			XString str;
			str << '[' << name << "] : first input parameter type must be \"Message\"!";
			behcontext.Context->OutputToConsole(str.Str(),TRUE);
		}

		pin = beh->GetInputParameter(1);
		if (!pin) {
			CKBehavior* pbeh = beh;
			XString name;
			while ((pbeh=pbeh->GetParent())) {
				name << pbeh->GetName() << "->";
			}
			name << beh->GetName();

			XString str;
			str << '[' << name << "] : failed to retrieve second input parameter";
			behcontext.Context->OutputToConsole(str.Str(),TRUE);
			return CKBR_PARAMETERERROR;
		}
	
		if (!behcontext.ParameterManager->IsDerivedFrom(pin->GetGUID(),CKPGUID_OBJECT)) {
			pin->SetGUID(CKPGUID_BEOBJECT,FALSE);

			CKBehavior* pbeh = beh;
			XString name;
			while ((pbeh=pbeh->GetParent())) {
				name << pbeh->GetName() << "->";
			}
			name << beh->GetName();

			XString str;
			str << '[' << name << "] : second input parameter type must derived from \"BeObject\"!";
			behcontext.Context->OutputToConsole(str.Str(),TRUE);
		}*/
	}

	return CKBR_OK;
}