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


CKObjectDeclaration	*FillBehaviorDOReleaseDecl();
CKERROR CreateDOReleaseProto(CKBehaviorPrototype **);
int DORelease(const CKBehaviorContext& behcontext);
CKERROR DOReleaseCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1


CKObjectDeclaration	*FillBehaviorDOReleaseDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("DORelease");	
	od->SetDescription("Release an ownership of a distributed object");

	od->SetCategory("TNL/Distributed Objects");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x4be460ae,0x412f2916));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateDOReleaseProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


CKERROR CreateDOReleaseProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("DORelease");
	if(!proto) return CKERR_OUTOFMEMORY;



	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	proto->DeclareOutput("Error");



	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	proto->DeclareInParameter("Object", CKPGUID_BEOBJECT, "test");
	
	proto->DeclareOutParameter("Owner ID", CKPGUID_ID, "-1");
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "Ok");


	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS  ));
	proto->SetFunction(DORelease);
	proto->SetBehaviorCallbackFct(DOReleaseCB);

	*pproto = proto;
	return CK_OK;

}


int DORelease(const CKBehaviorContext& behcontext)
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
		beh->ActivateOutput(1);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	//network ok  ? 
	xNetInterface *cin  = GetNM()->GetClientNetInterface();
	if (!cin)
	{
		CKParameterOut *pout = beh->GetOutputParameter(1);
		XString errorMesg("distributed object creation failed,no network connection !");
		pout->SetStringValue(errorMesg.Str());
		beh->ActivateOutput(1);
		return 0;
	}

	IDistributedObjects*doInterface  = cin->getDistObjectInterface();
	//////////////////////////////////////////////////////////////////////////
	//we come in by input 0 : 
	if (beh->IsInputActive(0))
	{
		beh->ActivateInput(0,FALSE);

		xDistributedObject *dobj = doInterface->getByEntityID(obj->GetID());
		if (!dobj)
		{
			CKParameterOut *pout = beh->GetOutputParameter(1);
			XString errorMesg("There is no such an object");
			pout->SetStringValue(errorMesg.Str());
			beh->ActivateOutput(1);
			return 0;
		}

		if (dobj->isOwner())
		{
			/*dobj->getOwnershipState().set( 1<< E_DO_OS_OWNER,false);
			dobj->getOwnershipState().set( 1<< E_DO_OS_RELEASED,true );
			dobj->getOwnershipState().set( 1<< E_DO_OS_RELEASED,true );*/
			if (cin->isValid())
			{
				cin->getConnection()->c2sDORequestOwnerShip(-1,dobj->getServerID());
			}
			beh->ActivateOutput(0);
		}
	}
	return 0;
}

CKERROR DOReleaseCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;

	if (behcontext.CallbackMessage==CKM_BEHAVIOREDITED) 
	{

	}

	return CKBR_OK;
}