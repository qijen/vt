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
CKObjectDeclaration	*FillBehaviorNGetInterfacesDecl();
CKERROR CreateNGetInterfacesProto(CKBehaviorPrototype **);
int NGetInterfaces(const CKBehaviorContext& behcontext);
CKERROR NGetInterfacesCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1




CKObjectDeclaration	*FillBehaviorNGetInterfacesDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("NGetInterfaces");	
	od->SetDescription("Returns all systems network devices");
	
	od->SetCategory("TNL/Server");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x5a6a0ae6,0x11f86ec3));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateNGetInterfacesProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


typedef enum BB_IT
{
	BB_IT_IN,
	BB_IT_NEXT,
};

typedef enum BB_OT
{
	BB_O_OUT,
	BB_O_INTERFACE,
	BB_O_ERROR,

};

typedef enum BB_OP
{
	BB_OP_INTERFACE_ADDRESS,
	BB_OP_ERROR
};


CKERROR CreateNGetInterfacesProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("NGetInterfaces");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareInput("Next");
	
	proto->DeclareOutput("Exit");
	proto->DeclareOutput("Interface");
	proto->DeclareOutput("Error");

	proto->DeclareOutParameter("Interface", CKPGUID_STRING, "FALSE");
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "Ok");


	proto->DeclareLocalParameter("current result", CKPGUID_POINTER, "0");
	proto->DeclareLocalParameter("currentArrayIndex", CKPGUID_INT,0 );

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(NGetInterfaces);
	proto->SetBehaviorCallbackFct(NGetInterfacesCB);

	*pproto = proto;
	return CK_OK;

}

typedef TNL::Vector<TNL::Address>interfaceList;

int NGetInterfaces(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());
	using namespace vtTools::BehaviorTools;

	
	if (beh->IsInputActive(0))
	{
		
		beh->ActivateInput(0,FALSE);

		bool isTemporary = false;
		

		xNetInterface *cin  = GetNM()->GetServerNetInterface();
		if (!cin)
		{
			cin = GetNM()->GetClientNetInterface();
		}
		
		if (!cin)
		{

			GetNM()->CreateClient(true,0,NULL);
			cin = GetNM()->GetClientNetInterface();
			isTemporary = true;
		}

		interfaceList *sResults = NULL;
		beh->GetLocalParameterValue(0,&sResults);
		if (!sResults)
		{
			sResults = new interfaceList();
		}else
			sResults->clear();


		int currentIndex=0;
		beh->SetLocalParameterValue(1,&currentIndex);

		if (cin)
		{
			TNL::Socket *socket  = &cin->getSocket();
			if (socket)
			{
				if (socket->isValid())
				{
					socket->getInterfaceAddresses(sResults);
				}
			}
		}
		
		if (isTemporary)
		{
			GetNM()->SetClientNetInterface(NULL);
		}

		beh->SetLocalParameterValue(0,&sResults);

		if (sResults->size())
		{
			beh->ActivateInput(BB_IT_NEXT);
		}else
		{
			beh->ActivateOutput(BB_O_OUT);
			return 0;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if (beh->IsInputActive(1))
	{
		beh->ActivateInput(1,FALSE);

		int currentIndex=0;
		beh->GetLocalParameterValue(1,&currentIndex);


		interfaceList *sResults = NULL;	
		beh->GetLocalParameterValue(0,&sResults);
		if (!sResults)
		{
			beh->ActivateOutput(0);
			return 0;
		}

		if (currentIndex>=sResults->size())
		{
			sResults->clear();
			beh->ActivateOutput(BB_O_OUT);
			return 0;
		}


		TNL::Vector<TNL::Address>& list = *sResults;
		TNL::Address *addr = &list[currentIndex];
		if (addr)
		{
			CKParameterOut *pout = beh->GetOutputParameter(BB_OP_INTERFACE_ADDRESS);
			pout->SetStringValue(const_cast<char*>(addr->toString()));
		}

		currentIndex++;
		beh->SetLocalParameterValue(1,&currentIndex);
		beh->ActivateOutput(1);

	}


	return CKBR_OK;
}

CKERROR NGetInterfacesCB(const CKBehaviorContext& behcontext)
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