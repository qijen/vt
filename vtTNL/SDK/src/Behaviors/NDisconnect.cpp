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
#include "xLogger.h"
#include "vtLogTools.h"


CKObjectDeclaration	*FillBehaviorNDisconnectDecl();
CKERROR CreateNDisconnectProto(CKBehaviorPrototype **);
int NDisconnect(const CKBehaviorContext& behcontext);
CKERROR NDisconnectCB(const CKBehaviorContext& behcontext);


CKObjectDeclaration	*FillBehaviorNDisconnectDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("NDisconnect");	
	od->SetDescription("Disconnects from the server");
	
	od->SetCategory("TNL/Server");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x76531dd9,0x4a317862));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateNDisconnectProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}



typedef enum BB_OT
{

	BB_O_OUT,
	BB_O_ERROR
};
typedef enum BB_OP
{
	BB_OP_ERROR
};

CKERROR CreateNDisconnectProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("NDisconnect");
	if(!proto) return CKERR_OUTOFMEMORY;

	

	proto->DeclareInput("On");
	
	proto->DeclareOutput("Exit On");
	proto->DeclareOutput("Error");

	
	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "0");
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	
	proto->SetFunction(NDisconnect);
	

	*pproto = proto;
	return CK_OK;

}


int NDisconnect(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	CKParameterOut *pout = beh->GetOutputParameter(0);
	XString errorMesg("No network connection !");
	//////////////////////////////////////////////////////////////////////////
	//connection id : 
	int connectionID = vtTools::BehaviorTools::GetInputParameterValue<int>(beh,0);

	//////////////////////////////////////////////////////////////////////////
	//network ok  ? 
	xNetInterface *cin  = GetNM()->GetClientNetInterface();
	if (!cin)
	{
		errorMesg  = "No network connection !";
		pout->SetStringValue(errorMesg.Str());
//		Error(beh,"No connection at the moment",BB_OP_ERROR,TRUE,BB_O_ERROR);
		//xLogger::xLog(ELOGERROR,"No Connection available. See Script :%s --> Building Block : %s",beh->GetOwnerScript()->GetName(),"NSCreate" );
		xLogger::xLog(ELOGERROR,E_LI_CONNECTION,"No connection at the moment");
		XLOG_BB_INFO;

		beh->ActivateOutput(0);
		return 0;
	}

	if (!cin->IsServer())
	{
		cin->disconnect(connectionID);
	}


	return 0;
	
}