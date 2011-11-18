#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "IParameter.h"

bool pFactory::findSettings(pPivotSettings& dst,CKBeObject*src)
{
	if (!src)return false;

	int att = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_PIVOT_OFFSET);
	CKParameterOut *par = findSettingsParameter(src,VTS_PHYSIC_PIVOT_OFFSET);

	if (par)
		return IParameter::Instance()->copyTo(dst,(CKParameter*)par);

	return false;
}

CKParameterOut* pFactory::findSettings(pWheelDescr& dst,CKBeObject*src)
{
	if (!src)return false;

	int att = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_WHEEL_DESCR);
	CKParameterOut *par = findSettingsParameter(src,VTS_PHYSIC_WHEEL_DESCR);
	if (!par)
	{
        att = GetPMan()->GetContext()->GetAttributeManager()->GetAttributeTypeByName("Wheel");
		par = src->GetAttributeParameter(att);
	}
	bool copied = IParameter::Instance()->copyTo(dst,(CKParameter*)par);
	return par;
}

bool pFactory::findSettings(pCollisionSettings& dst,CKBeObject*src)
{
	if (!src)return false;

	int att = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_COLLISIONS_SETTINGS);
	CKParameterOut *par = findSettingsParameter(src,VTS_PHYSIC_COLLISIONS_SETTINGS);

	if (par)
		return IParameter::Instance()->copyTo(dst,(CKParameter*)par);

	return false;
}

bool pFactory::findSettings(pOptimization& dst,CKBeObject*src)
{
	if (!src)return false;

	int att = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_ACTOR_OPTIMIZATION);
	CKParameterOut *par = findSettingsParameter(src,VTS_PHYSIC_ACTOR_OPTIMIZATION);

	if (par)
		return IParameter::Instance()->copyTo(dst,(CKParameter*)par);

	return false;
}

bool pFactory::findSettings(pMassSettings& dst,CKBeObject*src)
{
	if (!src)return false;

	int att = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_MASS_SETUP);
	CKParameterOut *par = findSettingsParameter(src,VTS_PHYSIC_MASS_SETUP);

	if (par)
		return IParameter::Instance()->copyTo(dst,(CKParameter*)par);

	return false;
}

int pFactory::copyTo(CKParameter *src,pDominanceSetupItem&dst)
{

	int result  = 0 ; 

#ifdef _DEBUG
	assert(src);
#endif // _DEBUG

	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;

	CKParameter *contraintParameter = GetParameterFromStruct(src,PS_WD_CONSTRAINT);

	dst.constraint.dominanceA = GetValueFromParameterStruct<float>(contraintParameter,PS_WDC_A);
	dst.constraint.dominanceB = GetValueFromParameterStruct<float>(contraintParameter,PS_WDC_B);

	dst.dominanceGroup0 =  GetValueFromParameterStruct<int>(src,PS_WD_GROUP_A);
	dst.dominanceGroup1 =  GetValueFromParameterStruct<int>(src,PS_WD_GROUP_B);

	

	return 0;

}





bool pFactory::copyTo(pConvexCylinderSettings&dst,CKParameter*src,bool evaluate/* =true */)
{

	bool result  = false;

	#ifdef _DEBUG
		assert(src);
	#endif // _DEBUG

	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;

	//int att = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_MASS_SETUP);
	//CKParameterOut *par = findSettingsParameter(src,VTS_PHYSIC_MASS_SETUP);

	/*
	if (par)
		return IParameter::Instance()->copyTo(dst,(CKParameter*)src);
	*/



	copyTo(dst.radius, GetParameterFromStruct(src,PS_CC_RADIUS_REFERENCED_VALUE),true);
	copyTo(dst.height, GetParameterFromStruct(src,PS_CC_HEIGHT_REFERENCED_VALUE),true);

	dst.approximation = GetValueFromParameterStruct<int>(src,PS_CC_APPROXIMATION);
	

	//################################################################
	//
	// Calculate Forward Axis, optionally referenced by an entity 
	//
	dst.forwardAxis = GetValueFromParameterStruct<VxVector>(src,PS_CC_FORWARD_AXIS);
	dst.forwardAxisRef = (CK3dEntity*)GetPMan()->m_Context->GetObject(GetValueFromParameterStruct<CK_ID>(src,PS_CC_FORWARD_AXIS_REF));
	if (dst.forwardAxisRef)
	{
		VxVector dir,up,right;
		dst.forwardAxisRef->GetOrientation(&dir,&up,&right);
		dst.forwardAxisRef->TransformVector(&dst.forwardAxis,&dir);
		dst.forwardAxis.Normalize();
	}


	//################################################################
	//
	// Calculate Down Axis, optionally referenced by an entity 
	//
	dst.downAxis = GetValueFromParameterStruct<VxVector>(src,PS_CC_DOWN_AXIS);
	dst.downAxisRef = (CK3dEntity*)GetPMan()->m_Context->GetObject(GetValueFromParameterStruct<CK_ID>(src,PS_CC_DOWN_AXIS_REF));
	if (dst.downAxisRef)
	{
		VxVector dir,up,right;
		dst.downAxisRef->GetOrientation(&dir,&up,&right);
		dst.downAxisRef->TransformVector(&dst.downAxis,&up);
		dst.downAxis.Normalize();
	}

	//################################################################
	//
	// Calculate Right Axis, optionally referenced by an entity 
	//
	dst.rightAxis = GetValueFromParameterStruct<VxVector>(src,PS_CC_RIGHT_AXIS);
	dst.rightAxisRef = (CK3dEntity*)GetPMan()->m_Context->GetObject(GetValueFromParameterStruct<CK_ID>(src,PS_CC_RIGHT_AXIS_REF));
	if (dst.rightAxisRef)
	{
		VxVector dir,up,right;
		dst.rightAxisRef->GetOrientation(&dir,&up,&right);
		dst.rightAxisRef->TransformVector(&dst.rightAxis,&right);
		dst.rightAxis.Normalize();
	}

	dst.buildLowerHalfOnly = GetValueFromParameterStruct<int>(src,PS_CC_BUILD_LOWER_HALF_ONLY);
	dst.convexFlags = (pConvexFlags)GetValueFromParameterStruct<int>(src,PS_CC_EXTRA_SHAPE_FLAGS);

	return true;
}

bool pFactory::findSettings(pConvexCylinderSettings&dst,CKBeObject *src)
{

	bool result  = false;

#ifdef _DEBUG
	assert(src);
#endif // _DEBUG
	
	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;

	int attType = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_CONVEX_CYLDINDER_WHEEL_DESCR);
	CKParameterOut *pout = src->GetAttributeParameter(attType);
	if (!pout)
	{
		CKMesh *currentMesh = ((CK3dEntity*)src)->GetCurrentMesh();
		if (currentMesh)
		{
			pout = currentMesh->GetAttributeParameter(attType);
			if (!pout)
			{

				dst.forwardAxis.Set(1.0f,0.0f,0.0f);
				dst.downAxis.Set(0.0f,-1.0f,0.0f);
				dst.rightAxis.Set(0.0f,0.0f,-1.0f);
				
				dst.buildLowerHalfOnly = false;
				dst.approximation = 10;

				VxVector size(0.0f);
				
				if (src->GetClassID()  == CKCID_3DOBJECT )
				{
					CK3dEntity *ent3D   = (CK3dEntity*)src;
					if (ent3D)
					{
						size = ent3D->GetBoundingBox(true).GetSize();
					}
				}else if(src->GetClassID()  == CKCID_MESH)
				{
					CKMesh *mesh = (CKMesh*)src;
					if (mesh)
					{
						size = mesh->GetLocalBox().GetSize();
					}
				}

				dst.radius.value = size.x * 0.5f;
				dst.height.value = size.y;  

				return false;
			}
		}
	}
	return copyTo(dst,pout);
}

bool pFactory::findSettings(pCapsuleSettings&dst,CKBeObject *src)
{

	bool result  = false;
	if (!src)
	{
		return result;
	}

	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;

	CKParameterOut *pout = src->GetAttributeParameter(GetPMan()->att_capsule);
	if (!pout)
	{
		CKMesh *currentMesh = ((CK3dEntity*)src)->GetCurrentMesh();
		if (currentMesh)
		{
			pout = currentMesh->GetAttributeParameter(GetPMan()->att_capsule);
			if (!pout)
			{
				dst.localLengthAxis  = CKAXIS_Y;
				dst.localRadiusAxis  = CKAXIS_X;
				dst.height = -1.0f;
				dst.radius = -1.0f;
				return false;
			}
		}
	}
	return copyTo(dst,pout);
}


bool pFactory::findSettings(pCapsuleSettingsEx&dst,CKBeObject *src)
{

	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;

	if (!src)return false;

	int att = GetPMan()->getAttributeTypeByGuid(VTS_CAPSULE_SETTINGS_EX);
	CKParameterOut *par = findSettingsParameter(src,VTS_CAPSULE_SETTINGS_EX);

	if (par)
		return IParameter::Instance()->copyTo(dst,(CKParameter*)par);

	//dst.height.referenceAxis	= CKAXIS_Y;
	//dst.radius.referenceAxis	= CKAXIS_X;

	return false;
				
}

void pFactory::copyTo(pAxisReferencedLength&dst,CKParameter*src,bool evaluate)
{

#ifdef _DEBUG
	assert(src);
#endif

	using namespace vtTools::ParameterTools;


	//################################################################
	//
	// Retrieve the value :
	//
	dst.value = GetValueFromParameterStruct<float>(src,PS_ARL_VALUE);

	dst.referenceAxis = GetValueFromParameterStruct<int>(src,PS_ARL_REF_OBJECT_AXIS);


	
	

	
	//################################################################
	//
	// Calculate the value basing on the given objects local box and an axis. 
	//
	CK_ID idRef= GetValueFromParameterStruct<CK_ID>(src,PS_ARL_REF_OBJECT);
	CKBeObject *object  = (CKBeObject*)GetPMan()->GetContext()->GetObject(idRef);

	if (!object)
		return;

	dst.reference = object;

	if (!evaluate)return;
	

    VxVector size(0.0f);
	if (object->GetClassID()  == CKCID_3DOBJECT )
	{
		CK3dEntity *ent3D   = (CK3dEntity*)object;
		if (ent3D)
		{
			size = ent3D->GetBoundingBox(true).GetSize();
		}
	}else if(object->GetClassID()  == CKCID_MESH)
	{
		CKMesh *mesh = (CKMesh*)object;
		if (mesh)
		{
			size = mesh->GetLocalBox().GetSize();
		}
	}

	
	dst.value = size[dst.referenceAxis];
}


pWorldSettings*pFactory::getWorldSettings(CK3dEntity*ent)
{

	/*
	//////////////////////////////////////////////////////////////////////////
	//sanity checks : 
	if (!ent )
	{
	return NULL;
	}
	if (!ent->HasAttribute(GetPMan()->att_world_object ))
	{
	return NULL;
	}
	//////////////////////////////////////////////////////////////////////////
	pWorldSettings *result = new pWorldSettings();
	using namespace vtTools::AttributeTools;

	result->m_Gravity= GetValueFromAttribute<VxVector>(ent,GetPMan()->att_world_object,0);
	result->m_CFM= GetValueFromAttribute<float>(ent,GetPMan()->att_world_object,1);
	result->m_ERP= GetValueFromAttribute<float>(ent,GetPMan()->att_world_object,2);
	result->m_MaximumContactCorrectVelocity = GetValueFromAttribute<int>(ent,GetPMan()->att_world_object,3);
	result->m_ContactSurfaceLayer = GetValueFromAttribute<int>(ent,GetPMan()->att_world_object,4);
	return result;
	*/
	return NULL;
}

pObjectDescr* pFactory::createPObjectDescrFromParameter(CKParameter *par)
{

	if (!par)
		return NULL;

	//################################################################
	//
	// ATTENTION : 
	//

	pObjectDescr *descr = new pObjectDescr();
	using namespace vtTools::ParameterTools;

	
	int hType,flags,hierarchy,collGroup;
	VxVector mOffset,sOffset;
	float density,skinWidth,newDensity,totalMass;

	collGroup = GetValueFromParameterStruct<int>(par,E_PPS_COLL_GROUP,false);
	hierarchy = GetValueFromParameterStruct<int>(par,E_PPS_HIRARCHY,false);
	hType  = GetValueFromParameterStruct<int>(par,E_PPS_HULLTYPE,false);
	flags =  GetValueFromParameterStruct<int>(par,E_PPS_BODY_FLAGS,false);
	density = GetValueFromParameterStruct<float>(par,E_PPS_DENSITY,false);
	newDensity = GetValueFromParameterStruct<float>(par,E_PPS_NEW_DENSITY,false);
	totalMass = GetValueFromParameterStruct<float>(par,E_PPS_TOTAL_MASS,false);
	skinWidth = GetValueFromParameterStruct<float>(par,E_PPS_SKIN_WIDTH,false);
	mOffset = GetValueFromParameterStruct<VxVector>(par,E_PPS_MASS_OFFSET,false);
	sOffset = GetValueFromParameterStruct<VxVector>(par,E_PPS_MASS_OFFSET,false);


	
	descr->density = density;
	descr->skinWidth = skinWidth;
	descr->massOffset = mOffset;
	descr->shapeOffset = sOffset;
	descr->hullType = (HullType)hType;
	descr->flags = (BodyFlags)flags;
	descr->hirarchy = hierarchy;
	descr->newDensity = newDensity;
	descr->totalMass = totalMass;

	return descr;
	
	return NULL;
}

pSleepingSettings*pFactory::getSleepingSettings(CK3dEntity*ent)
{
	/*
	//////////////////////////////////////////////////////////////////////////
	//sanity checks : 
	if (!ent )
	{
	return NULL;
	}
	if (!ent->HasAttribute(GetPMan()->att_sleep_settings ))
	{
	return NULL;
	}
	//////////////////////////////////////////////////////////////////////////
	pSleepingSettings *result = new pSleepingSettings();
	using namespace vtTools::AttributeTools;
	result->m_SleepSteps= GetValueFromAttribute<int>(ent,GetPMan()->att_sleep_settings,0);
	result->m_AngularThresold= GetValueFromAttribute<float>(ent,GetPMan()->att_sleep_settings,1);
	result->m_LinearThresold = GetValueFromAttribute<float>(ent,GetPMan()->att_sleep_settings,2);
	result->m_AutoSleepFlag = GetValueFromAttribute<int>(ent,GetPMan()->att_sleep_settings,3);
	return result;
	*/
	return NULL;
}

bool pFactory::copyTo(pCapsuleSettings&dst,CKParameter*src)
{
	if (!src)	return false;

	using namespace vtTools::ParameterTools;

	dst.localLengthAxis = GetValueFromParameterStruct<int>(src,E_CS_LENGTH_AXIS);
	dst.localRadiusAxis = GetValueFromParameterStruct<int>(src,E_CS_RADIUS_AXIS);
	dst.radius = GetValueFromParameterStruct<float>(src,E_CS_RADIUS);
	dst.height = GetValueFromParameterStruct<float>(src,E_CS_LENGTH);

	return true;
}
int pFactory::copyTo(CKParameterOut*dst,pGroupsMask src)
{

	if (!dst)	return 0;



	using namespace vtTools::ParameterTools;
	using namespace vtTools::ParameterTools;

	int result = 0;

	SetParameterStructureValue<int>(dst,0,src.bits0,false);
	SetParameterStructureValue<int>(dst,1,src.bits1,false);
	SetParameterStructureValue<int>(dst,2,src.bits2,false);
	SetParameterStructureValue<int>(dst,3,src.bits3,false);

	return true;
}
int pFactory::copyTo(pGroupsMask &dst,CKParameter*src)
{

	if (!src)	return 0;
	using namespace vtTools::ParameterTools;
	using namespace vtTools::ParameterTools;
	dst.bits0  = GetValueFromParameterStruct<int>(src,0);
	dst.bits1  = GetValueFromParameterStruct<int>(src,1);
	dst.bits2  = GetValueFromParameterStruct<int>(src,2);
	dst.bits3  = GetValueFromParameterStruct<int>(src,3);
	return 1;
}

CKParameterOut *pFactory::findSettingsParameter(CKBeObject *src,CKGUID guid)
{


	if (!src)return NULL;

	int att = GetPMan()->getAttributeTypeByGuid(guid);
	CKParameterOut *par = src->GetAttributeParameter(att);
	if (par){
		return par;
	}
	//----------------------------------------------------------------
	//
	// Try sub objects
	//
	CK3dEntity *ent3D  = static_cast<CK3dEntity*>(src);
	if (ent3D)
	{
		CKMesh *mesh = ent3D->GetCurrentMesh();
		if (mesh)
		{
			par = mesh->GetAttributeParameter(att);
			if (!par)
			{
				for (int i  = 0 ; i  < mesh->GetMaterialCount() ; i++)
				{
					CKMaterial *entMaterial  = mesh->GetMaterial(i);
					par = entMaterial->GetAttributeParameter(att);
					if (par)
					{
						break;
					}
				}
			}
		}
	}
	return par;
}