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


CKObjectDeclaration	*FillBehaviorDOUserValueModifiedDecl();
CKERROR CreateDOUserValueModifiedProto(CKBehaviorPrototype **);
int DOUserValueModified(const CKBehaviorContext& behcontext);
CKERROR DOUserValueModifiedCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1


CKObjectDeclaration	*FillBehaviorDOUserValueModifiedDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("DOUserValueModified");	
	od->SetDescription("Gets a user value on a distributed object");

	od->SetCategory("TNL/Distributed Objects");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x349c10e0,0x7982264c));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateDOUserValueModifiedProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


CKERROR CreateDOUserValueModifiedProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("DOUserValueModified");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareInput("Off");
	proto->DeclareInput("Next Value");



	proto->DeclareOutput("Exit In");
	proto->DeclareOutput("Exit Off");
	proto->DeclareOutput("New Value");
	proto->DeclareOutput("Error");


	
	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	proto->DeclareInParameter("Object", CKPGUID_BEOBJECT, "test");
	
	proto->DeclareOutParameter("Time", CKPGUID_TIME, "0");
	proto->DeclareOutParameter("Value", CKPGUID_STRING, "No Error");
	proto->DeclareOutParameter("Error", VTE_NETWORK_ERROR, "Ok");

	proto->DeclareSetting("Class", CKPGUID_STRING, "My3DClass");
	proto->DeclareSetting("Parameter Name", CKPGUID_STRING, "test");

	
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS  ));
	proto->SetFunction(DOUserValueModified);
	proto->SetBehaviorCallbackFct(DOUserValueModifiedCB);

	*pproto = proto;
	return CK_OK;

}


int DOUserValueModified(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;

	//////////////////////////////////////////////////////////////////////////
	//connection id : 
	int connectionID = vtTools::BehaviorTools::GetInputParameterValue<int>(beh,0);





	if (beh->IsInputActive(1))
	{
		beh->ActivateInput(1,FALSE);
		beh->ActivateOutput(1);
		return 0;
	}



	//////////////////////////////////////////////////////////////////////////
	//the object :
	CK3dEntity *obj= (CK3dEntity*)beh->GetInputParameterObject(1);
	if (!obj)
	{
		beh->ActivateOutput(3);
		return CKBR_ACTIVATENEXTFRAME;
	}

	//////////////////////////////////////////////////////////////////////////
	//network ok  ? 
	xNetInterface *cin  = GetNM()->GetClientNetInterface();
	if (!cin)
	{
		CKParameterOut *pout = beh->GetOutputParameter(2);
		XString errorMesg("distributed object creation failed,no network connection !");
		pout->SetStringValue(errorMesg.Str());
		beh->ActivateOutput(3);
		return CKBR_ACTIVATENEXTFRAME;
	}

	//use objects name, if not specified : 
	CKSTRING name= obj->GetName();
	
	IDistributedObjects*doInterface  = cin->getDistObjectInterface();
	IDistributedClasses*cInterface = cin->getDistributedClassInterface();
    
	XString className((CKSTRING) beh->GetLocalParameterReadDataPtr(0));
	XString parameterName((CKSTRING) beh->GetLocalParameterReadDataPtr(1));

	xDistributedClass *_class = cInterface->get(className.CStr());
	
	
	//////////////////////////////////////////////////////////////////////////
	//dist class ok  ? 
	if (_class==NULL)
	{
		beh->ActivateOutput(3);
		ctx->OutputToConsoleEx("Distributed Class doesn't exists : %s",className.CStr());
		return CKBR_ACTIVATENEXTFRAME;
	}
	const char * cNAme  = _class->getClassName().getString();

	int classType  = _class->getEnitityType();
	int bcount = beh->GetInputParameterCount();

	//////////////////////////////////////////////////////////////////////////
	//we come in by input 0 : 

	

	xDistributedObject *dobjDummy = NULL;
	xDistributedObject *dobj = doInterface->getByEntityID(obj->GetID());
	if (!dobj)
	{
		beh->ActivateOutput(3);
		return CKBR_ACTIVATENEXTFRAME;
	}

	if (dobj)
	{
		xDistributedProperty * prop  = dobj->getUserProperty(parameterName.CStr());
		if (prop)
		{
			if ( dobj->getGhostUpdateBits().testStrict(1 << prop->getBlockIndex()))
			{
				CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());
				CKParameterOut *ciIn = beh->GetOutputParameter(1);
				CKParameterType pType  = ciIn->GetType();

				int sType = vtTools::ParameterTools::GetVirtoolsSuperType(ctx,pam->ParameterTypeToGuid(pType));	
				int userType = xDistTools::ValueTypeToSuperType(prop->getPropertyInfo()->mValueType);
				if ( userType ==sType )
				{
					beh->ActivateOutput(2);
					dobj->getGhostUpdateBits().set(1 << prop->getBlockIndex(),false);
					switch(xDistTools::ValueTypeToSuperType(prop->getPropertyInfo()->mValueType))
					{
						case vtFLOAT:
						{
							xDistributedPoint1F *propValue  = static_cast<xDistributedPoint1F*>(prop);
							if (propValue)
							{
								float ovalue = propValue->mLastServerValue;
								beh->SetOutputParameterValue(1,&ovalue);
								break;
							}
						}

						case vtVECTOR2D:
						{
							xDistributedPoint2F *propValue  = static_cast<xDistributedPoint2F*>(prop);
							if (propValue)
							{
								Point2F ovalue = propValue->mLastServerValue;
								Vx2DVector ovaluex(ovalue.x,ovalue.y);
								beh->SetOutputParameterValue(1,&ovaluex);
								break;
							}
						}
						case vtVECTOR:
						{
							xDistributedPoint3F *propValue  = static_cast<xDistributedPoint3F*>(prop);
							if (propValue)
							{
								Point3F ovalue = propValue->mLastServerValue;
								VxVector ovaluex(ovalue.x,ovalue.y,ovalue.z);
								beh->SetOutputParameterValue(1,&ovaluex);
								break;
							}
						}
						case vtQUATERNION:
						{
							xDistributedQuatF*propValue  = static_cast<xDistributedQuatF*>(prop);
							if (propValue)
							{
								QuatF ovalue = propValue->mLastServerValue;
								VxQuaternion ovaluex(ovalue.x,ovalue.y,ovalue.z,ovalue.w);
								beh->SetOutputParameterValue(1,&ovaluex);
								break;
							}
						}
						case vtSTRING:
						{
							xDistributedString*propValue  = static_cast<xDistributedString*>(prop);
							if (propValue)
							{
								TNL::StringPtr ovalue = propValue->mCurrentValue;
								CKParameterOut *pout = beh->GetOutputParameter(1);
								XString errorMesg(ovalue.getString());
								pout->SetStringValue(errorMesg.Str());
								break;
							}
						}
						case vtINTEGER:
						{
							xDistributedInteger*propValue  = static_cast<xDistributedInteger*>(prop);
							if (propValue)
							{
								int ovalue = propValue->mLastServerValue;
								beh->SetOutputParameterValue(1,&ovalue);
								break;
							}
						}
					}
				}
			}
		}
	}

	return CKBR_ACTIVATENEXTFRAME;
	if (beh->IsInputActive(0))
	{
		beh->ActivateOutput(0);
	}



	/*
	//////////////////////////////////////////////////////////////////////////
	//we come in by input 0 : 
	if (beh->IsInputActive(0))
	{
		beh->ActivateOutput(0);

	
		for (int i = BEH_IN_INDEX_MIN_COUNT  ; i < beh->GetInputParameterCount() ; i++ )
		{
			beh->ActivateInput(0,FALSE);

			xDistributedObject *dobj = doInterface->get(name);
			if (!dobj)
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
						}
					}
				}
			}
		}
			
		beh->ActivateOutput(0);
	}

	
	*/
	//////////////////////////////////////////////////////////////////////////
	//we come in by loop : 
	return 0;
}

CKERROR DOUserValueModifiedCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;

	if (behcontext.CallbackMessage==CKM_BEHAVIOREDITED) 
	{

	}

	return CKBR_OK;
}