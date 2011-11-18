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


CKObjectDeclaration	*FillBehaviorDOControlDecl();
CKERROR CreateDOControlProto(CKBehaviorPrototype **);
int DOControl(const CKBehaviorContext& behcontext);
CKERROR DOControlCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1


CKObjectDeclaration	*FillBehaviorDOControlDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("DOControl");	
	od->SetDescription("Creates an distributed object");

	od->SetCategory("TNL/Distributed Objects");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x59f62787,0x78a73262));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateDOControlProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


CKERROR CreateDOControlProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("DOControl");
	if(!proto) return CKERR_OUTOFMEMORY;



	proto->DeclareInput("In");


	proto->DeclareOutput("Exit In");
	proto->DeclareOutput("Out");
	proto->DeclareOutput("Error");


	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	proto->DeclareInParameter("Object", CKPGUID_BEOBJECT, "test");
	
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "Ok");
	
	proto->DeclareSetting("Timeout", CKPGUID_TIME, "0");
	proto->DeclareLocalParameter("elapsed time", CKPGUID_FLOAT, "0.0f");
	

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS  ));
	proto->SetFunction(DOControl);
	proto->SetBehaviorCallbackFct(DOControlCB);

	*pproto = proto;
	return CK_OK;

}


int DOControl(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;

	//////////////////////////////////////////////////////////////////////////
	//connection id : 
	int connectionID = vtTools::BehaviorTools::GetInputParameterValue<int>(beh,0);

	//////////////////////////////////////////////////////////////////////////
	//the object :
	CK3dEntity *obj= (CK3dEntity*)beh->GetInputParameterObject(1);
	if (!obj)
	{
		beh->ActivateOutput(2);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	//network ok  ? 
	xNetInterface *cin  = GetNM()->GetClientNetInterface();
	if (!cin)
	{
		CKParameterOut *pout = beh->GetOutputParameter(0);
		XString errorMesg("distributed object creation failed,no network connection !");
		pout->SetStringValue(errorMesg.Str());
		beh->ActivateOutput(2);
		return 0;
	}

	IDistributedObjects*doInterface  = cin->getDistObjectInterface();

	xDistributedObject *dobj = doInterface->getByEntityID(obj->GetID());
	if (!dobj)
	{
		CKParameterOut *pout = beh->GetOutputParameter(0);
		XString errorMesg("There is no such an object");
		pout->SetStringValue(errorMesg.Str());
		beh->ActivateOutput(2);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	//we come in by input 0 : 
	if (beh->IsInputActive(0))
	{
		beh->ActivateInput(0,FALSE);

		//reset elapsed time :
		float elapsedTime = 0.0f;
		beh->SetLocalParameterValue(1,&elapsedTime);

		if (!dobj->isOwner() /*&& !dobj->getOwnershipState().test(E_DO_OS_REQUEST) */)
		{
			dobj->getOwnershipState().set( 1<< E_DO_OS_REQUEST,true );
			if (cin->isValid())
			{
				cin->getConnection()->c2sDORequestOwnerShip(cin->getConnection()->getUserID(),dobj->getServerID());
			}
			beh->ActivateOutput(0);
		}
		return CKBR_ACTIVATENEXTFRAME;
	}

	//////////////////////////////////////////////////////////////////////////
	//we come in by loop : 

	if ( dobj->getOwnershipState().test(E_DO_OS_REQUEST) )
	{
		//we requested a dist object already, check the its timeout :
		float elapsedTime  = 0.0f;
		beh->GetLocalParameterValue(1,&elapsedTime);
		
		//pickup creations timeout settings : 
		float timeOut=0.0f;
		beh->GetLocalParameterValue(0,&timeOut);

		//////////////////////////////////////////////////////////////////////////
		//timeout reached : reset everything back 
		if (elapsedTime > timeOut)
		{
			//reset output server id : 
			int ids = -1;
			beh->SetOutputParameterValue(0,&ids);

			//output an error string :
			CKParameterOut *pout = beh->GetOutputParameter(1);
			XString errorMesg("Ownership request failed,timeout");
			pout->SetStringValue(errorMesg.Str());

			//finish, activate error output trigger: 
			beh->ActivateOutput(2);
			return 0; 

		}

		float dt = ctx->GetTimeManager()->GetLastDeltaTime();
		elapsedTime+=dt;
		beh->SetLocalParameterValue(1,&elapsedTime);
	
		//////////////////////////////////////////////////////////////////////////
		//we are within the timeout range, check we have a successfully created object by the server : 
		//we are owner now  ? 
		if ( dobj->getOwnershipState().test(1<<E_DO_OS_REQUEST) && dobj->getOwnershipState().test(1<<E_DO_OS_OWNERCHANGED) && dobj->isOwner() )
		{
			//xLogger::xLog(ELOGINFO,XL_START,"Got ownership");
			//clear the change bit : 
			//dobj->getOwnershipState().set( 1<<E_DO_OS_OWNERCHANGED ,false );
			//clear in-request bit :
			dobj->getOwnershipState().set( 1<< E_DO_OS_REQUEST,false);
			beh->ActivateOutput(1);
			return 0; 
		}
	}
	return CKBR_ACTIVATENEXTFRAME;
}

CKERROR DOControlCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;

	if (behcontext.CallbackMessage==CKM_BEHAVIOREDITED) 
	{

	}

	return CKBR_OK;
}