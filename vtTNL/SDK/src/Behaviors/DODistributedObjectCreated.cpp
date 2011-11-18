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
#include "IMessages.h"
#include "xLogger.h"
#include "vtLogTools.h"
#include "xMessageTypes.h"

CKObjectDeclaration	*FillBehaviorDODistributedObjectCreatedDecl();
CKERROR CreateDODistributedObjectCreatedProto(CKBehaviorPrototype **);
int DODistributedObjectCreated(const CKBehaviorContext& behcontext);
CKERROR DODistributedObjectCreatedCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1


CKObjectDeclaration	*FillBehaviorDODistributedObjectCreatedDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("DOCreated");	
	od->SetDescription("Creates an distributed object");
	
	od->SetCategory("TNL/Distributed Objects");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x778b6a11,0x29892907));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateDODistributedObjectCreatedProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}

enum bbIn
{

	bbI_ON,
	bbI_OFF,
	bbI_NEXT
};

enum bbOut
{
	BB_O_ON,
	BB_O_OFF,
	BB_O_OBJECT,
	BB_O_ERROR
};

enum bbPO_TIME
{
	BB_OP_TIME,
	BB_OP_NAME,
	BB_OP_OID,
	BB_OP_UID,
	BB_OP_ERROR
};


CKERROR CreateDODistributedObjectCreatedProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("DOCreated");
	if(!proto) return CKERR_OUTOFMEMORY;

	

	proto->DeclareInput("On");
	proto->DeclareInput("Off");
	proto->DeclareInput("Next");
	
	proto->DeclareOutput("Exit On");
	proto->DeclareOutput("Exit Off");
	proto->DeclareOutput("Object");
	proto->DeclareOutput("Error");

	
	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	
	proto->DeclareOutParameter("Distributed Object Creation Time", CKPGUID_TIME, "0");
	proto->DeclareOutParameter("Object Name", CKPGUID_STRING, "0");
	proto->DeclareOutParameter("Distributed Object ID", CKPGUID_INT, "0");
	proto->DeclareOutParameter("Owner ID", CKPGUID_INT, "0");
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "Ok");

	proto->DeclareSetting("Class", CKPGUID_STRING, "My3DClass");
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS  ));
	proto->SetFunction(DODistributedObjectCreated);
	proto->SetBehaviorCallbackFct(DODistributedObjectCreatedCB);

	*pproto = proto;
	return CK_OK;

}


int DODistributedObjectCreated(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	

	using namespace vtTools::BehaviorTools;

	//////////////////////////////////////////////////////////////////////////
	//connection id : 
	int connectionID = vtTools::BehaviorTools::GetInputParameterValue<int>(beh,0);

	//////////////////////////////////////////////////////////////////////////
	//network ok  ? 
	xNetInterface *cin  = GetNM()->GetClientNetInterface();
	if (!cin)
	{
		bbError(E_NWE_NO_CONNECTION);
		return 0;
	}


	//we come in by input off : 
	if (beh->IsInputActive(bbI_OFF))
	{
		beh->ActivateInput(bbI_OFF,FALSE);
		beh->ActivateOutput(BB_O_OFF);
		return 0;
	}

	//we come in by input off : 
	if (beh->IsInputActive(bbI_ON))
	{
		beh->ActivateInput(bbI_ON,FALSE);
		beh->ActivateOutput(BB_O_ON);
//		return 0;
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
				XString name(dobj->GetName().getString()); 


				if (_class->getEnitityType() == E_DC_BTYPE_3D_ENTITY )
				{
					//we only output objects created by remote side :
					if (dobj->getUserID() != cin->getConnection()->getUserID() )
					{
						DWORD  iFlags = dobj->getInterfaceFlags();
						if (!isFlagOn(dobj->getObjectStateFlags(),E_DOSF_SHOWN) )
						{
							//output do's creation time
							enableFlag(dobj->getObjectStateFlags(),E_DOSF_SHOWN);
						
							SetOutputParameterValue<float>(beh,BB_OP_TIME,dobj->getCreationTime()/1000.0f);

							//output do's name 
							SetOutputParameterValue<CKSTRING>(beh,BB_OP_NAME,const_cast<char*>(dobj->GetName().getString()));

		                    
							//output do's network id 
							SetOutputParameterValue<int>(beh,BB_OP_OID,dobj->getServerID());

							//output do's network id 
							SetOutputParameterValue<int>(beh,BB_OP_UID,dobj->getUserID());


							bbNoError(E_NWE_OK);
							beh->ActivateOutput(BB_O_OBJECT);
							dobj->setInterfaceFlags(E_DO_PROCESSED);
							xLogger::xLog(XL_START,ELOGTRACE,E_LI_3DOBJECT,"DObject created");

						}
					}
				}
			}
		}
		begin++;
	}
	return CKBR_ACTIVATENEXTFRAME;
}

CKERROR DODistributedObjectCreatedCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	
	if (behcontext.CallbackMessage==CKM_BEHAVIOREDITED) 
	{
	
	}

	return CKBR_OK;
}