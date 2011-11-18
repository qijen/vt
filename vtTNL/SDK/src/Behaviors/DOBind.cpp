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

CKObjectDeclaration	*FillBehaviorDOBindDecl();
CKERROR CreateDOBindProto(CKBehaviorPrototype **);
int DOBind(const CKBehaviorContext& behcontext);
CKERROR DOBindCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1


CKObjectDeclaration	*FillBehaviorDOBindDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("DOBind");	
	od->SetDescription("Binds an distributed object to a BeObject");
	
	od->SetCategory("TNL/Distributed Objects");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x4691107e,0x59ae518d));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateDOBindProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}

enum bbIn
{

	bbI_ON,
};

enum bbOut
{
	bbO_ON,
	bbO_ERROR
};

enum bbPO_TIME
{
	bbPO_ERROR
};


CKERROR CreateDOBindProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("DOBind");
	if(!proto) return CKERR_OUTOFMEMORY;

	

	proto->DeclareInput("On");
	
	proto->DeclareOutput("Exit On");
	proto->DeclareOutput("Error");

	
	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	proto->DeclareInParameter("Distributed Object ID", CKPGUID_INT, "0");
	proto->DeclareInParameter("Object", CKPGUID_BEOBJECT, "0");
	
	proto->DeclareOutParameter("Error", CKPGUID_STRING, "0");
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS  ));
	proto->SetFunction(DOBind);
	proto->SetBehaviorCallbackFct(DOBindCB);

	*pproto = proto;
	return CK_OK;

}


int DOBind(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	CKParameterOut *pout = beh->GetOutputParameter(bbPO_ERROR);
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
		beh->ActivateOutput(bbO_ERROR);
		return 0;
	}

	//we come in by input off : 
	if (beh->IsInputActive(bbI_ON))
	{
		beh->ActivateInput(bbI_ON,FALSE);
		int doID = vtTools::BehaviorTools::GetInputParameterValue<int>(beh,1);
		CKBeObject *beObject  = (CKBeObject*)beh->GetInputParameterObject(2);
		if (!beObject)
		{
			errorMesg  = "No Object given !";
			pout->SetStringValue(errorMesg.Str());
			beh->ActivateOutput(bbO_ERROR);
			return 0;
		}
		if (beObject)
		{

			CKSTRING oName  = beObject->GetName();
			int oID = beObject->GetID();
			IDistributedObjects *doInterface  = cin->getDistObjectInterface();
			xDistributedObject *dObject = doInterface->get(doID);
			if (dObject)
			{
				//////////////////////////////////////////////////////////////////////////
				switch(beObject->GetClassID())
				{
					case CKCID_3DENTITY:
						{
							if (dObject->getDistributedClass()->getEnitityType() == E_DC_BTYPE_3D_ENTITY )
							{
								dObject->setEntityID(oID);
								DWORD doFlags = dObject->getInterfaceFlags();
								doFlags |=E_DO_BINDED;
								dObject->setInterfaceFlags(doFlags);
								errorMesg  = "No error";
								pout->SetStringValue(errorMesg.Str());
								beh->ActivateOutput(bbO_ON);
								xLogger::xLog(ELOGINFO,XL_START,"binding object:% to dist object :%d",beObject->GetName(),doID);
								return 0;
								//beh->ActivateOutput(bbO_ERROR);
							}else
							{
								errorMesg  = "Distributed object class doesn't match BeObject class";
								pout->SetStringValue(errorMesg.Str());
								beh->ActivateOutput(bbO_ERROR);
								return 0;
							}
							break;
						}
				}				
			}else
			{
				errorMesg  = "No distributed object found with this ID!";
				pout->SetStringValue(errorMesg.Str());
				beh->ActivateOutput(bbO_ERROR);
				return 0;
			}
		}
		beh->ActivateOutput(bbO_ON);
	}
/*

	
	vtDistributedObjectsArrayType *distObjects = cin->getDistributedObjects();
	vtDistObjectIt begin = distObjects->begin();
	vtDistObjectIt end = distObjects->end();
	while (begin!=end)
	{
		xDistributedObject* dobj = *begin;
		if (dobj)
		{
			xDistributedClass *_class = dobj->GetDistributedClass();
			
			if (_class)
			{
				XString name(dobj->GetName().getString()); 


				if (_class->GetEnitityType() == E_DC_BTYPE_3D_ENTITY )
				{
					if (dobj->getInterfaceFlags() == E_DO_CREATED)
					{

						//output do's creation time
						float cTime = dobj->getCreationTime()/1000.0f;
						beh->SetOutputParameterValue(bbPO_TIME,&cTime);


						//output do's name 
						CKParameterOut *pout = beh->GetOutputParameter(bbPO_NAME);
						pout->SetStringValue(name.Str());
	                    
						//output do's network id 
						int serverID  = dobj->GetServerID();
						beh->SetOutputParameterValue(bbPO_OID,&serverID);


						//output do's network id 
						int userID  = dobj->GetUserID();
						beh->SetOutputParameterValue(bbPO_UID,&userID);

						//set do's interface flags  
						dobj->setInterfaceFlags(E_DO_PROCESSED);
	                    
						CKParameterOut *poutE = beh->GetOutputParameter(bbPO_ERROR);
						XString errorMesg("No Error");
						poutE->SetStringValue(errorMesg.Str());
						beh->ActivateOutput(bbO_OBJECT);
					}
				}
			}
		}
		begin++;
	}
*/
	return 0;
	
}

CKERROR DOBindCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	
	if (behcontext.CallbackMessage==CKM_BEHAVIOREDITED) 
	{
	
	}

	return CKBR_OK;
}