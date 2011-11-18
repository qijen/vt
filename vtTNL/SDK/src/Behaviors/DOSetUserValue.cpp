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

CKObjectDeclaration	*FillBehaviorDOSetUserValueDecl();
CKERROR CreateDOSetUserValueProto(CKBehaviorPrototype **);
int DOSetUserValue(const CKBehaviorContext& behcontext);
CKERROR DOSetUserValueCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1


CKObjectDeclaration	*FillBehaviorDOSetUserValueDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("DOSetUserValue");	
	od->SetDescription("Sets a user value on distributed object");

	od->SetCategory("TNL/Distributed Objects");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x3b345be9,0x2851018b));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateDOSetUserValueProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


CKERROR CreateDOSetUserValueProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("DOSetUserValue");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");


	proto->DeclareOutput("Exit In");
	proto->DeclareOutput("Out");
	
	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	proto->DeclareInParameter("Object", CKPGUID_BEOBJECT, "test");
	
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "Ok");
	proto->DeclareSetting("Class", CKPGUID_STRING, "My3DClass");
	
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETERINPUTS  ));
	proto->SetFunction(DOSetUserValue);
	proto->SetBehaviorCallbackFct(DOSetUserValueCB);

	*pproto = proto;
	return CK_OK;

}


int DOSetUserValue(const CKBehaviorContext& behcontext)
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

	//use objects name, if not specified : 
	CKSTRING name= obj->GetName();
	
	IDistributedObjects*doInterface  = cin->getDistObjectInterface();
	IDistributedClasses*cInterface = cin->getDistributedClassInterface();
    
	XString className((CKSTRING) beh->GetLocalParameterReadDataPtr(0));
	xDistributedClass *_class = cInterface->get(className.CStr());
	
	
	//////////////////////////////////////////////////////////////////////////
	//dist class ok  ? 
	if (_class==NULL)
	{
		beh->ActivateOutput(1);
		ctx->OutputToConsoleEx("Distributed Class doesn't exists : %s",className.CStr());
		return 0;
	}
	const char * cNAme  = _class->getClassName().getString();

	int classType  = _class->getEnitityType();

	/*using namespace vtTools;
	using namespace vtTools::Enums;*/
	int bcount = beh->GetInputParameterCount();

	//////////////////////////////////////////////////////////////////////////
	//we come in by input 0 : 
	if (beh->IsInputActive(0))
	{


		CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());

		for (int i = BEH_IN_INDEX_MIN_COUNT  ; i < beh->GetInputParameterCount() ; i++ )
		{
			beh->ActivateInput(0,FALSE);

			xDistributedObject *dobj = doInterface->getByEntityID(obj->GetID());
			
			if (!dobj  )
			{
				beh->ActivateOutput(0);
				return 0;
			}

			int dOID = dobj->getUserID(); 
			int clientID = cin->getConnection()->getUserID();
			if (dOID !=clientID)
			{

				beh->ActivateOutput(0);
				return 0;

			}
			


			CKParameterIn *ciIn = beh->GetInputParameter(i);
			CKParameterType pType  = ciIn->GetType();
			int sType = vtTools::ParameterTools::GetVirtoolsSuperType(ctx,pam->ParameterTypeToGuid(pType));	
			xDistributedPropertyArrayType &props = *dobj->getDistributedPorperties();
			
			int propID = _class->getInternalUserFieldIndex(i - BEH_IN_INDEX_MIN_COUNT);
			if (propID==-1 ||  propID > props.size() )
			{
				beh->ActivateOutput(1);
				return 0;
			}
			
			xDistributedProperty *prop  = props[propID];
			if (prop)
			{
				xDistributedPropertyInfo*propInfo  = prop->getPropertyInfo();
				if (propInfo)
				{
					
					if (xDistTools::ValueTypeToSuperType(propInfo->mValueType) ==sType )
					{
						TNL::U32 currentTime = TNL::Platform::getRealMilliseconds();

						switch(propInfo->mValueType)
						{
							case E_DC_PTYPE_3DVECTOR:
							{

								xDistributedPoint3F * dpoint3F  = (xDistributedPoint3F*)prop;
								if (dpoint3F)
								{
									VxVector vvalue;
									beh->GetInputParameterValue(i,&vvalue);
									bool update = dpoint3F->updateValue(xMath::getFrom(vvalue),currentTime);
								}
                                break;
							}

							case E_DC_PTYPE_FLOAT:
							{

								xDistributedPoint1F * dpoint3F  = (xDistributedPoint1F*)prop;
								if (dpoint3F)
								{
									float vvalue;
									beh->GetInputParameterValue(i,&vvalue);
									bool update = dpoint3F->updateValue(vvalue,currentTime);
								}
								break;
							}
							case E_DC_PTYPE_2DVECTOR:
							{

								xDistributedPoint2F * dpoint3F  = (xDistributedPoint2F*)prop;
								if (dpoint3F)
								{
									Vx2DVector vvalue;
									beh->GetInputParameterValue(i,&vvalue);
									bool update = dpoint3F->updateValue(xMath::getFrom(vvalue),currentTime);
								}
								break;
							}
							case E_DC_PTYPE_QUATERNION:
							{

								xDistributedQuatF * dpoint3F  = (xDistributedQuatF*)prop;
								if (dpoint3F)
								{
									VxQuaternion vvalue;
									beh->GetInputParameterValue(i,&vvalue);
									bool update = dpoint3F->updateValue(xMath::getFrom(vvalue),currentTime);
								}
								break;
							}
							case E_DC_PTYPE_STRING:
							{

								xDistributedString * dpoint3F  = (xDistributedString*)prop;
								if (dpoint3F)
								{
									CKParameter *pin = beh->GetInputParameter(i)->GetRealSource();
									if (pin)
									{
										VxScratch sbuffer(256);
										CKSTRING buffer = (CKSTRING)sbuffer.Mem();
										pin->GetStringValue(buffer);
										bool update = dpoint3F->updateValue(TNL::StringPtr(buffer),currentTime);

									}
									
								}
								break;
							}
							case E_DC_PTYPE_INT:
							{

								xDistributedInteger * dpoint3F  = (xDistributedInteger*)prop;
								if (dpoint3F)
								{
									int vvalue;
									beh->GetInputParameterValue(i,&vvalue);
									bool update = dpoint3F->updateValue(vvalue,currentTime);
								}
								break;
							}
						}
					}
				}
			}






		}
			
		beh->ActivateOutput(0);
	}

	//////////////////////////////////////////////////////////////////////////
	//we come in by loop : 
	return 0;
}

CKERROR DOSetUserValueCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;

	if (behcontext.CallbackMessage==CKM_BEHAVIOREDITED) 
	{

	}

	return CKBR_OK;
}