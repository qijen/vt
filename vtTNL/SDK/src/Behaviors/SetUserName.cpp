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



CKObjectDeclaration	*FillBehaviorSetUserNameDecl();
CKERROR CreateSetUserNameProto(CKBehaviorPrototype **);
int SetUserName(const CKBehaviorContext& behcontext);
CKERROR SetUserNameCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1




CKObjectDeclaration	*FillBehaviorSetUserNameDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("NSetUserName");	
	od->SetDescription("Sets your user name");
	
	od->SetCategory("TNL/User Management");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x68be0952,0xb376cd4));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Günter Baumgart");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateSetUserNameProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


typedef enum BB_IT
{
	BB_IT_IN,
};

typedef enum BB_INPAR
{
	BB_IP_CONNECTION_ID,
};

typedef enum BB_OT
{
	BB_O_OUT,
	BB_O_ERROR,

};

typedef enum BB_OP
{
	BB_OP_ERROR
};

CKERROR CreateSetUserNameProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("NSetUserName");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	proto->DeclareOutput("Error");

	
	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	proto->DeclareInParameter("User Name ", CKPGUID_STRING, "MyNetName");
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "Ok");
	

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS  ));
	proto->SetFunction(SetUserName);
	proto->SetBehaviorCallbackFct(SetUserNameCB);

	*pproto = proto;
	return CK_OK;

}


int SetUserName(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());

	
	
	beh->ActivateInput(0,FALSE);



	int connectionID=-1;
	beh->GetInputParameterValue(0,&connectionID);
	
	XString userName((CKSTRING) beh->GetInputParameterReadDataPtr(1));
	
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/


	using namespace vtTools::BehaviorTools;
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
						if (client->getUserID() == cin->getConnection()->getUserID() )
						{
                            client->rpcSetName(userName.CStr());
							break;
						}
					}
				}
			}
		}
		begin++;
	}
	beh->ActivateOutput(0);
	return CKBR_OK;
}

CKERROR SetUserNameCB(const CKBehaviorContext& behcontext)
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