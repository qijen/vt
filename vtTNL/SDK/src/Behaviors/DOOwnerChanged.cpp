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


CKObjectDeclaration	*FillBehaviorDOOwnerChangedDecl();
CKERROR CreateDOOwnerChangedProto(CKBehaviorPrototype **);
int DOOwnerChanged(const CKBehaviorContext& behcontext);
CKERROR DOOwnerChangedCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1


CKObjectDeclaration	*FillBehaviorDOOwnerChangedDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("DOOwnerChanged");	
	od->SetDescription("Returns the current owner of a distributed object");

	od->SetCategory("TNL/Distributed Objects");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x34f668cc,0x43c51c58));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateDOOwnerChangedProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


CKERROR CreateDOOwnerChangedProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("DOOwnerChanged");
	if(!proto) return CKERR_OUTOFMEMORY;



	proto->DeclareInput("On");
	proto->DeclareInput("Off");
	
	proto->DeclareOutput("Exit On");
	proto->DeclareOutput("Exit Off");
	proto->DeclareOutput("New Owner");
	proto->DeclareOutput("Object Released");
	proto->DeclareOutput("Error");



	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	proto->DeclareInParameter("Object", CKPGUID_BEOBJECT, "test");
	
	proto->DeclareOutParameter("Owner ID", CKPGUID_ID, "-1");
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "Ok");


	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS  ));
	proto->SetFunction(DOOwnerChanged);
	proto->SetBehaviorCallbackFct(DOOwnerChangedCB);

	*pproto = proto;
	return CK_OK;

}


int DOOwnerChanged(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;

	//////////////////////////////////////////////////////////////////////////
	//connection id : 
	int connectionID = vtTools::BehaviorTools::GetInputParameterValue<int>(beh,0);

	//////////////////////////////////////////////////////////////////////////
	//we come in by input 0 : 
	if (beh->IsInputActive(0))
	{
		beh->ActivateOutput(0);
		beh->ActivateInput(0,FALSE);
	}

	if (beh->IsInputActive(1))
	{
		beh->ActivateOutput(1);
		beh->ActivateInput(1,FALSE);
		return 0;
	}
	
	//////////////////////////////////////////////////////////////////////////
	//the object :
	CK3dEntity *obj= (CK3dEntity*)beh->GetInputParameterObject(1);
	if (!obj)
	{
		beh->ActivateOutput(4);
		return CKBR_ACTIVATENEXTFRAME;
	}

	//////////////////////////////////////////////////////////////////////////
	//network ok  ? 
	xNetInterface *cin  = GetNM()->GetClientNetInterface();
	if (!cin)
	{
		CKParameterOut *pout = beh->GetOutputParameter(1);
		XString errorMesg("distributed object creation failed,no network connection !");
		pout->SetStringValue(errorMesg.Str());
		beh->ActivateOutput(4);
		return CKBR_ACTIVATENEXTFRAME;
	}


	IDistributedObjects*doInterface  = cin->getDistObjectInterface();
	//////////////////////////////////////////////////////////////////////////
	//we come in by input 0 : 
	xDistributedObject *dobj = doInterface->getByEntityID(obj->GetID());
	if (!dobj)
	{
		CKParameterOut *pout = beh->GetOutputParameter(1);
		XString errorMesg("There is no such an object");
		pout->SetStringValue(errorMesg.Str());
		beh->ActivateOutput(4);
	}else
	{
		if (dobj->getOwnershipState().test(1 << E_DO_OS_RELEASED) && dobj->getOwnershipState().test( 1 << E_DO_OS_OWNERCHANGED) )
		{
			beh->ActivateOutput(3);
		}

		if (dobj->getOwnershipState().testStrict(1 << E_DO_OS_OWNERCHANGED))
		{
			dobj->getOwnershipState().set( 1<<E_DO_OS_OWNERCHANGED ,false );
			int newCLientID   = dobj->getUserID();
			beh->SetOutputParameterValue(0,&newCLientID);
			beh->ActivateOutput(2);
		}
	}
	return CKBR_ACTIVATENEXTFRAME;
}

CKERROR DOOwnerChangedCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;

	if (behcontext.CallbackMessage==CKM_BEHAVIOREDITED) 
	{

	}

	return CKBR_OK;
}