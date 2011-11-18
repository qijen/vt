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

#include "vtLogTools.h"

CKObjectDeclaration	*FillBehaviorDOCreateDistributedObjectDecl();
CKERROR CreateDOCreateDistributedObjectProto(CKBehaviorPrototype **);
int DOCreateDistributedObject(const CKBehaviorContext& behcontext);
CKERROR DOCreateDistributedObjectCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1

typedef enum BB_INPAR
{
	BB_IP_CONNECTION_ID,
	BB_IP_OBJECT,
	BB_IP_OBJECT_NAME,
};

typedef enum BB_OT
{

	BB_O_EXIT,
	BB_O_CREATED,
	BB_O_ERROR
};
typedef enum BB_OP
{

	BB_OP_OBJECT_ID,
	BB_OP_ERROR
};


CKObjectDeclaration	*FillBehaviorDOCreateDistributedObjectDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("DOCreate");	
	od->SetDescription("Creates an distributed object");

	od->SetCategory("TNL/Distributed Objects");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x4b134dfa,0xaca30cb));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateDOCreateDistributedObjectProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


CKERROR CreateDOCreateDistributedObjectProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("DOCreate");
	if(!proto) return CKERR_OUTOFMEMORY;



	proto->DeclareInput("In");


	proto->DeclareOutput("Exit In");
	proto->DeclareOutput("Created");
	proto->DeclareOutput("Error");


	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	proto->DeclareInParameter("Object", CKPGUID_BEOBJECT, "test");
	proto->DeclareInParameter("Object Name", CKPGUID_STRING, "test");

	proto->DeclareOutParameter("Distributed Object ID", CKPGUID_INT, "0");
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "Ok");

	proto->DeclareSetting("Class", CKPGUID_STRING, "My3DClass");
	proto->DeclareSetting("Timeout", CKPGUID_TIME, "0");
	proto->DeclareLocalParameter("elapsed time", CKPGUID_FLOAT, "0.0f");
	proto->DeclareLocalParameter("Creation Status", CKPGUID_INT, "0");

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS  ));
	proto->SetFunction(DOCreateDistributedObject);
	proto->SetBehaviorCallbackFct(DOCreateDistributedObjectCB);

	*pproto = proto;
	return CK_OK;

}


int DOCreateDistributedObject(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;

	using namespace vtTools::BehaviorTools;

	//////////////////////////////////////////////////////////////////////////
	//connection id : 
	int connectionID = vtTools::BehaviorTools::GetInputParameterValue<int>(beh,0);

	//////////////////////////////////////////////////////////////////////////
	//the object :
	CK3dEntity *obj= (CK3dEntity*)beh->GetInputParameterObject(1);
	if (!obj)
	{
		bbError(E_NWE_INVALID_PARAMETER);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	//network ok  ? 
	xNetInterface *cin  = GetNM()->GetClientNetInterface();
	if (!cin)
	{
		bbError(E_NWE_NO_CONNECTION);
		return 0;
	}

	//use objects name, if not specified : 
	CKSTRING name=vtTools::BehaviorTools::GetInputParameterValue<CKSTRING>(beh,2);
	if (!strlen(name))
	{
		name = obj->GetName();
	}

	
	IDistributedObjects*doInterface  = cin->getDistObjectInterface();
	IDistributedClasses*cInterface = cin->getDistributedClassInterface();
    
	XString className((CKSTRING) beh->GetLocalParameterReadDataPtr(0));
	xDistributedClass *classTemplate = cInterface->get(className.CStr());
	
	
	//////////////////////////////////////////////////////////////////////////
	//dist class ok  ? 
	if (!classTemplate)
	{
		bbError(E_NWE_INTERN);
		return 0;
	}


	int classType  = classTemplate->getEnitityType();

	//////////////////////////////////////////////////////////////////////////
	//we come in by input 0 : 
	if (beh->IsInputActive(0))
	{
		beh->ActivateInput(0,FALSE);

		//reset elapsed time :
		float elapsedTime = 0.0f;
		beh->SetLocalParameterValue(2,&elapsedTime);

		//reset status to incomplete
		int creationStatus = E_DO_CREATION_INCOMPLETE;
		beh->SetLocalParameterValue(3,&creationStatus);
		
		
		xDistributedObject *dobj = doInterface->get(name);
		if (!dobj)
		{
			doInterface->create(name,className.CStr());
			beh->ActivateOutput(0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//we come in by loop : 


	//pickup the creations state : 
	int creationState=0;
	beh->GetLocalParameterValue(3,&creationState);


	if (creationState == E_DO_CREATION_INCOMPLETE )
	{
		//we requested a dist object already, check the its timeout :
		float elapsedTime  = 0.0f;
		beh->GetLocalParameterValue(2,&elapsedTime);
		
		//pickup creations timeout settings : 
		float timeOut=0.0f;
		beh->GetLocalParameterValue(1,&timeOut);

		//////////////////////////////////////////////////////////////////////////
		//timeout reached : reset everything back 
		if (elapsedTime > timeOut)
		{
			//reset output server id : 
			int ids = -1;
			beh->SetOutputParameterValue(0,&ids);

			//output an error string :
			CKParameterOut *pout = beh->GetOutputParameter(1);
			XString errorMesg("distributed object creation failed, timeout reached");
			pout->SetStringValue(errorMesg.Str());


			//store state : 
			int state = E_DO_CREATION_NONE;
			beh->SetLocalParameterValue(3,&state);

			//finish, activate error output trigger: 
			beh->ActivateOutput(2);
			return 0; 

		}
		else	// increase and store elapsed time : 
		{

			float dt = ctx->GetTimeManager()->GetLastDeltaTime();
			elapsedTime+=dt;
			beh->SetLocalParameterValue(2,&elapsedTime);
		}

		//////////////////////////////////////////////////////////////////////////
		//we are within the timeout range, check we have a successfully created object by the server : 

		xDistributedObject *dobj = doInterface->get(name);
		if (dobj && isFlagOn(dobj->getObjectStateFlags(),E_DOSF_UNPACKED) &&
			!isFlagOn(dobj->getObjectStateFlags(),E_DOSF_SHOWN))
		{

			enableFlag(dobj->getObjectStateFlags(),E_DOSF_SHOWN);
			xLogger::xLog(XL_START,ELOGTRACE,E_LI_3DOBJECT,"distributed object creation finish");

			if (obj)
			{
				dobj->setEntityID(obj->GetID());
				xDistributed3DObject *dobj3D = static_cast<xDistributed3DObject*>(dobj);
				if (dobj3D)
				{

				}
			}

			//store state : 
			int state = E_DO_CREATION_NONE;
			beh->SetLocalParameterValue(3,&state);

			//store elapsed time : 
			elapsedTime = 0.0f;
			beh->SetLocalParameterValue(2,&elapsedTime);

			//output server id : 
			int index  =  dobj->getServerID();
			beh->SetOutputParameterValue(0,&index);

			CKParameterOut *pout = beh->GetOutputParameter(1);
			XString errorMesg("No Error");
			pout->SetStringValue(errorMesg.Str());

			//return : 
			beh->ActivateOutput(1);
			return 0; 
		}

	}
	return CKBR_ACTIVATENEXTFRAME;
}

CKERROR DOCreateDistributedObjectCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;

	if (behcontext.CallbackMessage==CKM_BEHAVIOREDITED) 
	{

	}

	return CKBR_OK;
}