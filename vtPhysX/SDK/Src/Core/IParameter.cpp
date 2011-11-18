#include "StdAfx.h"
#include "vtPhysXAll.h"

#include "IParameter.h"

#include "vtStructHelper.h"


static IParameter* gIPar = NULL;

#include "pMisc.h"
#include "pVehicleAll.h"
using namespace vtTools::ParameterTools;


int IParameter::copyTo(pLinearInterpolation& dst,CKParameter*src)
{
	if (!src)
		return false;

	int gear = 0;
	float ratio = 0.0f;


	CKStructHelper sHelper(src);
	if (sHelper.GetMemberCount() ==0 )//happens when dev is being opened and loads a cmo with physic objects.
		return -1;

	for (int i = 0 ; i < sHelper.GetMemberCount() ; i++)
	{
		CK_ID* paramids = static_cast<CK_ID*>(src->GetReadDataPtr());
		CKParameter * sub = static_cast<CKParameterOut*>(GetPMan()->m_Context->GetObject(paramids[i]));
		if (sub)
		{
			
			dst.insert(GetValueFromParameterStruct<float>(sub,0,false),
				GetValueFromParameterStruct<float>(sub,1,false)
				);
		}
	}
	return true;
}


int IParameter::copyTo(pGearBox *dst,CKParameter*src)
{
	if (!src || !dst)
		return false;
	
	int gear = 0;
	float ratio = 0.0f;

	for (int i = 0 ; i < 7 ; i++)
	{
		CK_ID* paramids = static_cast<CK_ID*>(src->GetReadDataPtr());
		CKParameter * sub = static_cast<CKParameterOut*>(GetPMan()->m_Context->GetObject(paramids[i]));
		if (sub)
		{
			dst->gearRatio[i] = 	GetValueFromParameterStruct<float>(sub,0,false);
			dst->gearInertia[i] =	GetValueFromParameterStruct<float>(sub,1,false);

			dst->getGearRatios().insert((float)i,dst->gearRatio[i]);
			dst->getGearTensors().insert((float)i,dst->gearInertia[i]);
		}
	}
	return true;
}

int IParameter::copyTo(pVehicleBrakeTable& dst,CKParameter*src)
{
	
	int result = 0;

	for (int i  = 0 ; i < BREAK_TABLE_ENTRIES ; i ++)
		dst.brakeEntries[i] = GetValueFromParameterStruct<float>(src,i);

	return true;


}
int IParameter::copyTo(CKParameter*dst,pWheelDescr src)
{
	if (!dst)return false;
	using namespace vtTools::ParameterTools;

	int result = 0;

	SetParameterStructureValue<float>(dst,E_WD_SPRING_BIAS,src.springBias,false);
	SetParameterStructureValue<float>(dst,E_WD_SPRING_RES,src.springRestitution,false);
	SetParameterStructureValue<float>(dst,E_WD_DAMP,src.springDamping,false);
	SetParameterStructureValue<float>(dst,E_WD_MAX_BFORCE,src.maxBrakeForce,false);
	SetParameterStructureValue<float>(dst,E_WD_FFRONT,src.frictionToFront,false);
	SetParameterStructureValue<float>(dst,E_WD_FSIDE,src.frictionToSide,false);
	SetParameterStructureValue<int>(dst,E_WD_FLAGS,src.wheelFlags,false);
	SetParameterStructureValue<float>(dst,E_WD_INVERSE_WHEEL_MASS,src.inverseWheelMass,false);
	SetParameterStructureValue<int>(dst,E_WD_SFLAGS,src.wheelShapeFlags,false);
	SetParameterStructureValue<float>(dst,E_WD_SUSPENSION,src.wheelSuspension,false);


    

}

int IParameter::copyTo(pWheelDescr& dst,CKParameter*src)
{
	int result  = 1;
	if (!src)
	{
		return NULL;
	}
	using namespace vtTools::ParameterTools;
	
	dst.wheelSuspension = GetValueFromParameterStruct<float>(src,E_WD_SUSPENSION);
	dst.springRestitution= GetValueFromParameterStruct<float>(src,E_WD_SPRING_RES);
	dst.springBias = GetValueFromParameterStruct<float>(src,E_WD_SPRING_BIAS);
	dst.springDamping= GetValueFromParameterStruct<float>(src,E_WD_DAMP);

	dst.maxBrakeForce= GetValueFromParameterStruct<float>(src,E_WD_MAX_BFORCE);
	dst.frictionToSide= GetValueFromParameterStruct<float>(src,E_WD_FSIDE);
	dst.frictionToFront= GetValueFromParameterStruct<float>(src,E_WD_FFRONT);

	CKParameterOut *pOld  = GetParameterFromStruct(src,E_WD_INVERSE_WHEEL_MASS);
	if (pOld)
	{
		if (pOld->GetGUID()  == CKPGUID_FLOAT)
		{
			dst.inverseWheelMass= GetValueFromParameterStruct<float>(src,E_WD_INVERSE_WHEEL_MASS);
		}

		if (pOld->GetGUID()  == CKPGUID_INT)
		{
			dst.wheelApproximation= GetValueFromParameterStruct<int>(src,E_WD_INVERSE_WHEEL_MASS);
		}
	}

	
	
	dst.wheelFlags= (WheelFlags)GetValueFromParameterStruct<int>(src,E_WD_FLAGS);
	dst.wheelShapeFlags=(WheelShapeFlags) GetValueFromParameterStruct<int>(src,E_WD_SFLAGS);


	CKParameterOut *parLatFunc = GetParameterFromStruct(src,E_WD_LAT_FUNC);
	CKParameterOut *parLongFunc = GetParameterFromStruct(src,E_WD_LONG_FUNC);



	/************************************************************************/
	/* XML Setup ?                                                                     */
	/************************************************************************/
	int xmlLinkId= GetValueFromParameterStruct<int>(src,E_WD_XML);
	bool wIsXML=false;
	bool latIsXML= false;
	bool longIsXML=false;

	XString nodeName;
	if ( xmlLinkId !=0 )
	{
		nodeName = vtAgeia::getEnumDescription(GetPMan()->GetContext()->GetParameterManager(),VTE_XML_WHEEL_SETTINGS,xmlLinkId);
		pFactory::Instance()->loadWheelDescrFromXML(dst,nodeName.CStr(),pFactory::Instance()->getDefaultDocument());
		wIsXML =true;
		if (!dst.isValid())
		{
			xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Wheel Description was invalid");
		}
		if (dst.latFunc.xmlLink!=0)
		{
			latIsXML=true;
		}

		if (dst.longFunc.xmlLink!=0)
		{
			longIsXML=true;
		}
	}

	if (!latIsXML)
	{
		dst.latFunc = pFactory::Instance()->createTireFuncFromParameter(parLatFunc);
	}

	if (!longIsXML)
	{
		dst.longFunc= pFactory::Instance()->createTireFuncFromParameter(parLongFunc);
	}

	if (wIsXML)
	{
		IParameter::Instance()->copyTo((CKParameterOut*)src,dst);

	}
	if (longIsXML)
	{
		pFactory::Instance()->copyTo(GetParameterFromStruct(src,E_WD_LONG_FUNC),dst.longFunc);
	}

	if (latIsXML)
	{
		pFactory::Instance()->copyTo(GetParameterFromStruct(src,E_WD_LAT_FUNC),dst.latFunc);
	}

	return result;
}
int IParameter::copyTo(pConvexCylinderSettings& dst,CKParameter*src)
{
	//----------------------------------------------------------------
	//
	// Sanity checks
	//
#ifdef _DEBUG
	assert(src);
#endif 

	using namespace vtTools;
	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;

	copyTo(dst.radius, GetParameterFromStruct(src,PS_CC_RADIUS_REFERENCED_VALUE),true);
	copyTo(dst.height, GetParameterFromStruct(src,PS_CC_HEIGHT_REFERENCED_VALUE),true);

	dst.approximation = GetValueFromParameterStruct<int>(src,PS_CC_APPROXIMATION);
	dst.buildLowerHalfOnly = GetValueFromParameterStruct<int>(src,PS_CC_BUILD_LOWER_HALF_ONLY);
	dst.convexFlags = (pConvexFlags)GetValueFromParameterStruct<int>(src,PS_CC_EXTRA_SHAPE_FLAGS);


	//----------------------------------------------------------------
	//
	// Calculate Forward Axis, optionally referenced by an entity 
	//
	dst.forwardAxis = GetValueFromParameterStruct<VxVector>(src,PS_CC_FORWARD_AXIS);
	dst.forwardAxisRef = GetValueFromParameterStruct<CK_ID>(src,PS_CC_FORWARD_AXIS_REF);

	CK3dEntity *f = (CK3dEntity*)GetPMan()->m_Context->GetObject(dst.forwardAxisRef);
	if (f)
	{
		VxVector dir,up,right;
		f->GetOrientation(&dir,&up,&right);
		f->TransformVector(&dst.forwardAxis,&dir);
		dst.forwardAxis.Normalize();
	}


	
	//----------------------------------------------------------------
	//
	// Calculate Down Axis, optionally referenced by an entity 
	//
	dst.downAxis = GetValueFromParameterStruct<VxVector>(src,PS_CC_DOWN_AXIS);
	dst.downAxisRef = GetValueFromParameterStruct<CK_ID>(src,PS_CC_DOWN_AXIS_REF);
	
	CK3dEntity *d = (CK3dEntity*)GetPMan()->m_Context->GetObject(dst.downAxisRef);
	if (d)
	{
		VxVector dir,up,right;
		d->GetOrientation(&dir,&up,&right);
		d->TransformVector(&dst.downAxis,&up);
		dst.downAxis.Normalize();
	}

	
	//----------------------------------------------------------------
	//
	// Calculate Right Axis, optionally referenced by an entity 
	//
	dst.rightAxis = GetValueFromParameterStruct<VxVector>(src,PS_CC_RIGHT_AXIS);
	
	dst.rightAxisRef = GetValueFromParameterStruct<CK_ID>(src,PS_CC_RIGHT_AXIS_REF);
	CK3dEntity *r = (CK3dEntity*)GetPMan()->m_Context->GetObject(dst.rightAxisRef);
	if (r)
	{
		VxVector dir,up,right;
		r->GetOrientation(&dir,&up,&right);
		r->TransformVector(&dst.rightAxis,&right);
		dst.rightAxis.Normalize();
	}


	


	return 1;
}

int IParameter::copyTo(CKParameter*dst,pConvexCylinderSettings& src)
{

	copyTo(src.radius, GetParameterFromStruct(dst,PS_CC_RADIUS_REFERENCED_VALUE),true);
	copyTo(src.height, GetParameterFromStruct(dst,PS_CC_HEIGHT_REFERENCED_VALUE),true);

	SetParameterStructureValue<int>(dst,PS_CC_APPROXIMATION,src.approximation,false);
	SetParameterStructureValue<int>(dst,PS_CC_BUILD_LOWER_HALF_ONLY,src.buildLowerHalfOnly,false);
	SetParameterStructureValue<int>(dst,PS_CC_EXTRA_SHAPE_FLAGS,src.convexFlags,false);
	
	SetParameterStructureValue<VxVector>(dst,PS_CC_FORWARD_AXIS,src.forwardAxis,false);
	SetParameterStructureValue<VxVector>(dst,PS_CC_DOWN_AXIS,src.downAxis,false);
	SetParameterStructureValue<VxVector>(dst,PS_CC_RIGHT_AXIS,src.rightAxis,false);

	CK3dEntity *f = (CK3dEntity*)GetPMan()->m_Context->GetObject(src.forwardAxisRef);
	if (f)
		SetParameterStructureValue<CK_ID>(dst,PS_CC_FORWARD_AXIS_REF,f->GetID(),false);

	
	CK3dEntity *d = (CK3dEntity*)GetPMan()->m_Context->GetObject(src.downAxisRef);
	if (d)
		SetParameterStructureValue<CK_ID>(dst,PS_CC_DOWN_AXIS_REF,d->GetID(),false);


	CK3dEntity *r = (CK3dEntity*)GetPMan()->m_Context->GetObject(src.rightAxisRef);
	if (r)
		SetParameterStructureValue<CK_ID>(dst,PS_CC_RIGHT_AXIS_REF,r->GetID(),false);

	return 1;


}

int IParameter::copyTo(pMassSettings& dst,CKParameter*src)
{
	//----------------------------------------------------------------
	//
	// Sanity checks
	//
#ifdef _DEBUG
	assert(src);
#endif 

	using namespace vtTools;
	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;

	dst.newDensity= GetValueFromParameterStruct<float>(src,PS_BM_DENSITY,false);
	dst.totalMass= GetValueFromParameterStruct<float>(src,PS_BM_TOTAL_MASS,false);
	dst.localPosition = GetValueFromParameterStruct<VxVector>(src,PS_BM_PIVOT_POS,false);
	dst.localOrientation = GetValueFromParameterStruct<VxVector>(src,PS_BM_PIVOT_ROTATION,false);
	dst.massReference= GetValueFromParameterStruct<CK_ID>(src,PS_BM_PIVOT_REFERENCE,false);
	return 1;
}

int IParameter::copyTo(CKParameter*dst,pMassSettings src)
{
	//----------------------------------------------------------------
	//
	// Sanity checks
	//
#ifdef _DEBUG
	assert(dst);
#endif 

	using namespace vtTools;
	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;

	
	SetParameterStructureValue<float>(dst,PS_BM_DENSITY,src.newDensity,false);
	SetParameterStructureValue<float>(dst,PS_BM_TOTAL_MASS,src.totalMass,false);
	SetParameterStructureValue<VxVector>(dst,PS_BM_PIVOT_POS,src.localPosition,false);
	SetParameterStructureValue<VxVector>(dst,PS_BM_PIVOT_ROTATION,src.localOrientation,false);
	SetParameterStructureValue<CK_ID>(dst,PS_BM_PIVOT_REFERENCE,src.massReference,false);

	return 1;

}


int IParameter::copyTo(CKParameter*dst,pPivotSettings src)
{
	//----------------------------------------------------------------
	//
	// Sanity checks
	//
#ifdef _DEBUG
	assert(dst);
#endif 

	using namespace vtTools;
	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;

	SetParameterStructureValue<VxVector>(dst,PS_BP_LINEAR,src.localPosition,false);
	SetParameterStructureValue<VxVector>(dst,PS_BP_ANGULAR,src.localOrientation,false);
	SetParameterStructureValue<CK_ID>(dst,PS_BP_REFERENCE,src.pivotReference,false);

	/*
	dst.localPosition = GetValueFromParameterStruct<VxVector>(src,PS_BP_LINEAR,false);
	dst.localOrientation = GetValueFromParameterStruct<VxVector>(src,PS_BP_ANGULAR,false);
	dst.pivotReference = GetValueFromParameterStruct<CK_ID>(src,PS_BP_REFERENCE,false);

	*/

	return 1;

}

int IParameter::copyTo(pPivotSettings& dst,CKParameter*src)
{
	//----------------------------------------------------------------
	//
	// Sanity checks
	//
#ifdef _DEBUG
	assert(src);
#endif 

	using namespace vtTools;
	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;

	dst.localPosition = GetValueFromParameterStruct<VxVector>(src,PS_BP_LINEAR,false);
	dst.localOrientation = GetValueFromParameterStruct<VxVector>(src,PS_BP_ANGULAR,false);
	dst.pivotReference = GetValueFromParameterStruct<CK_ID>(src,PS_BP_REFERENCE,false);
	return 1;

}

int IParameter::copyTo(pObjectDescr&dst,CK3dEntity*src,int copyFlags)
{

	pRigidBody *srcBody = GetPMan()->getBody(src);
	NxActor *actor = NULL;
	pObjectDescr*initDescr = NULL;
	NxShape *mainShape= NULL;

	//----------------------------------------------------------------
	//
	// sanity checks
	//
	if (!src || !srcBody || !srcBody->getMainShape())
		return 0;


	initDescr  = srcBody->getInitialDescription();
	actor = srcBody->getActor();
	mainShape = srcBody->getMainShape();



	if (!initDescr || !actor )
		return 0;


	//----------------------------------------------------------------
	//
	// general settings
	//
	dst.hullType = (HullType)vtAgeia::getHullTypeFromShape(srcBody->getMainShape());
    
	
	srcBody->recalculateFlags(0);
	dst.flags = (BodyFlags)srcBody->getFlags();
	dst.worlReference = srcBody->getWorld()->getReference()->GetID();
	dst.density = initDescr->density;
	dst.mask = initDescr->mask;


	//----------------------------------------------------------------
	//
	// specific override : optimization
	//
	if ( (copyFlags & PB_CF_OPTIMIZATION) )
	{

		//----------------------------------------------------------------
		//
		// damping + sleeping
		//
		dst.optimization.angDamping = actor->getAngularDamping();
		dst.optimization.linDamping = actor->getLinearDamping();
		dst.optimization.linSleepVelocity = actor->getSleepLinearVelocity();
		dst.optimization.angSleepVelocity = actor->getSleepAngularVelocity();
		dst.optimization.sleepEnergyThreshold = actor->getSleepEnergyThreshold();

		//----------------------------------------------------------------
		//
		// transformation flags
		//

		dst.optimization.transformationFlags = (BodyLockFlags)srcBody->getTransformationsLockFlags();


		dst.optimization.solverIterations = actor->getSolverIterationCount();
		dst.optimization.dominanceGroup = actor->getDominanceGroup();
		dst.optimization.compartmentGroup = initDescr->optimization.compartmentGroup;
	}
		
	//----------------------------------------------------------------
	//
	// specific override : collision
	//
	//if ( (dst.mask & OD_Optimization ) )
	if ( (copyFlags & PB_CF_COLLISION )  )
	{
		dst.collisionGroup = mainShape->getGroup();

		NxGroupsMask nxMask  = mainShape->getGroupsMask();
		dst.groupsMask.bits0 = nxMask.bits0;
		dst.groupsMask.bits1 = nxMask.bits1;
		dst.groupsMask.bits2 = nxMask.bits2;
		dst.groupsMask.bits3 = nxMask.bits3;
		dst.skinWidth = mainShape->getSkinWidth();
	}

	//----------------------------------------------------------------
	//
	// specific override : ccd
	//
	//if ( (dst.mask & OD_CCD) )
	if ( (copyFlags & OD_CCD ) && (dst.mask & OD_CCD) )
	{

		dst.ccd.flags = initDescr->ccd.flags;
		dst.ccd.meshReference = initDescr->ccd.meshReference;
		dst.ccd.motionThresold = initDescr->ccd.motionThresold;
		dst.ccd.scale =  initDescr->ccd.scale;
	}

	//----------------------------------------------------------------
	//
	// specific override : capsule
	//
	if ( (copyFlags && OD_Capsule) && (dst.mask & OD_Capsule) )
		dst.capsule = initDescr->capsule;

	//----------------------------------------------------------------
	//
	// specific override : convex cylinder
	//
	if ( (copyFlags && OD_ConvexCylinder) && (dst.mask & OD_ConvexCylinder) )
		dst.convexCylinder = initDescr->convexCylinder;

	//----------------------------------------------------------------
	//
	// specific override : material
	//
	if ( (copyFlags && OD_Material) && (dst.mask & OD_Material) )
		dst.material = initDescr->material;

	//----------------------------------------------------------------
	//
	// specific override : pivot
	//
	if ( (copyFlags && OD_Pivot) )
	{
		//dst.pivotOffsetAngular	= getFrom(mainShape->getLocalOrientation() );
		dst.pivotOffsetLinear	= getFrom( mainShape->getLocalPosition() );
		//dst.pivotOffsetReference= initDescr->pivotOffsetReference;
	}

	if ( (copyFlags && PB_CF_MASS_SETTINGS)  )
	{
		//dst.pivotOffsetAngular	= getFrom(mainShape->getLocalOrientation() );
		dst.massOffsetLinear	= getFrom(actor->getCMassLocalPosition());
		//dst.pivotOffsetReference= initDescr->pivotOffsetReference;
	}

	//----------------------------------------------------------------
	//
	// correct data mask :
	//

	DWORD mask  = (DWORD)(dst.mask);
	mask &=~((OD_Pivot));



	if ( !(copyFlags & PB_CF_PIVOT_SETTINGS) )
		mask &=~((OD_Pivot));

	if ( !(copyFlags & PB_CF_MASS_SETTINGS) )
		mask &=~(OD_Mass);

	if ( !(copyFlags & PB_CF_OPTIMIZATION) )
		mask &=~(OD_Optimization);

	if ( !(copyFlags & PB_CF_COLLISION) )
		mask &=~(OD_Collision);

	if ( !(copyFlags & PB_CF_CCD) )
		mask &=~(OD_CCD);

	if ( !(copyFlags & PB_CF_CAPSULE) )
		mask &=~(OD_Capsule);

	
	if ( !(copyFlags & PB_CF_CONVEX_CYLINDER) )
		mask&=~(OD_ConvexCylinder);
	
	if ( !(copyFlags & PB_CF_MATERIAL) )
		mask &=~(OD_Material);

	return 1;
}




int IParameter::copyTo(pAxisReferencedLength&dst,CKParameter*src,bool evaluate)
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
		return -1;

	dst.reference = object;

	if (!evaluate)return -1;


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
	return 1;
}

int IParameter::copyTo(CKParameter *dst, pAxisReferencedLength &src, bool evaluate)
{
	SetParameterStructureValue<float>(dst,PS_ARL_VALUE,src.value,false);
	SetParameterStructureValue<int>(dst,PS_ARL_REF_OBJECT_AXIS,src.referenceAxis,false);
	if (src.reference)
	{
		SetParameterStructureValue<CK_ID>(dst,PS_ARL_REF_OBJECT,src.reference->GetID(),false);
	}
	return 1;
}



int IParameter::copyTo(pCapsuleSettingsEx& dst,CKParameter*src)
{
	//SetParameterStructureValue<float>(dst,PS_BM_DENSITY,src.newDensity,false);

	//----------------------------------------------------------------
	//
	// Sanity checks
	//
	#ifdef _DEBUG
		assert(src);
	#endif 

	using namespace vtTools;
	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;

	pFactory::Instance()->copyTo(dst.radius, GetParameterFromStruct(src,PS_BCAPSULE_RADIUS_REFERENCED_VALUE),false);
	pFactory::Instance()->copyTo(dst.height, GetParameterFromStruct(src,PS_PCAPSULE_HEIGHT_REFERENCED_VALUE),false);

	return 1;
}
int IParameter::copyTo(CKParameter*dst,pCapsuleSettingsEx src)
{
	//----------------------------------------------------------------
	//
	// Sanity checks
	//
#ifdef _DEBUG
	assert(dst);
#endif 

	using namespace vtTools;
	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;

	copyTo(GetParameterFromStruct(dst,PS_BCAPSULE_RADIUS_REFERENCED_VALUE),src.radius,false);
	copyTo(GetParameterFromStruct(dst,PS_PCAPSULE_HEIGHT_REFERENCED_VALUE),src.height,false);


	return 1;
}

int IParameter::copyTo(pCCDSettings& dst,CKParameter*src)
{
	//----------------------------------------------------------------
	//
	// Sanity checks
	//
#ifdef _DEBUG
	assert(src);
#endif 

	using namespace vtTools;
	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;

	//----------------------------------------------------------------

	//
	// general
	//

	dst.flags = GetValueFromParameterStruct<int>(src,PS_B_CCD_FLAGS,false);
	dst.motionThresold = GetValueFromParameterStruct<float>(src,PS_B_CCD_MOTION_THRESHOLD,false);
	dst.scale= GetValueFromParameterStruct<float>(src,PS_B_CCD_SCALE,false);
	dst.meshReference= GetValueFromParameterStruct<CK_ID>(src,PS_B_CCD_MESH_REFERENCE,false);


	return 1;
}

int IParameter::copyTo(pOptimization& dst,CKParameter*src)
{

	//----------------------------------------------------------------
	//
	// Sanity checks
	//
	#ifdef _DEBUG
		assert(src);
	#endif 

	using namespace vtTools;
	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;


	//----------------------------------------------------------------
	//
	// Retrieve all sub parameters : 
	//
	CKParameterOut *parSleeping =  GetParameterFromStruct(src,PS_BO_SLEEPING);
	CKParameterOut *parDamping =  GetParameterFromStruct(src,PS_BO_DAMPING);

	//----------------------------------------------------------------
	//
	// general
	//
	
	dst.transformationFlags = (BodyLockFlags) GetValueFromParameterStruct<int>(src,PS_BO_LOCKS,false);
	
	dst.solverIterations = GetValueFromParameterStruct<int>(src,PS_BO_SOLVER_ITERATIONS,false);
	dst.compartmentGroup = GetValueFromParameterStruct<int>(src,PS_BO_COMPARTMENT_ID,false);
	dst.dominanceGroup	= GetValueFromParameterStruct<int>(src,PS_BO_DOMINANCE_GROUP,false);

	//----------------------------------------------------------------
	//
	// Sleeping
	//
	dst.angSleepVelocity = GetValueFromParameterStruct<float>(parSleeping,PS_BS_ANGULAR_SLEEP,false);
	dst.linSleepVelocity = GetValueFromParameterStruct<float>(parSleeping,PS_BS_LINEAR_SLEEP,false);
	dst.sleepEnergyThreshold = GetValueFromParameterStruct<float>(parSleeping,PS_BS_THRESHOLD,false);

	//----------------------------------------------------------------
	//
	// Damping
	//
	dst.angDamping = GetValueFromParameterStruct<float>(parDamping,PS_BD_ANGULAR,false);
	dst.linDamping= GetValueFromParameterStruct<float>(parDamping,PS_BD_LINEAR,false);

	return 1;

}

int IParameter::copyTo(CKParameter*dst,pObjectDescr*src)
{

	//----------------------------------------------------------------
	//
	// Sanity checks
	//
	#ifdef _DEBUG
		assert(dst);
		assert(src);
	#endif 

		
	
	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;

	//----------------------------------------------------------------
	//
	// Retrieve all sub parameters : 
	//
	CKParameterOut *parBCommon =  GetParameterFromStruct(dst,PS_COMMON_SETTINGS);
	CKParameterOut *parBCollision =  GetParameterFromStruct(dst,PS_COLLISION_SETTINGS);
		//CKParameterOut *parBCCD =  GetParameterFromStruct(parBCollision,PS_BC_CCD_SETUP);
		CKParameterOut *parGroupsMask = GetParameterFromStruct(parBCollision,PS_BC_GROUPSMASK);
	
	#ifdef _DEBUG
		assert(parBCommon);
		//assert(parBPivot);
		//assert(parBCCD);
		assert(parBCollision);
	#endif 

	//////////////////////////////////////////////////////////////////////////	//////////////////////////////////////////////////////////////////////////
	//
	//	transfer		
	//


	//----------------------------------------------------------------
	//
	// common 
	//
	SetParameterStructureValue<int>(parBCommon,PS_BC_HULL_TYPE,src->hullType);
	SetParameterStructureValue<float>(parBCommon,PS_BC_DENSITY,src->density);
	SetParameterStructureValue<int>(parBCommon,PS_BC_FLAGS,src->flags);
	SetParameterStructureValue<CK_ID>(parBCommon,PS_BC_WORLD,src->worlReference);

	//----------------------------------------------------------------
	//
	// XML
	//

	//----------------------------------------------------------------
	//
	// Collision
	//
	SetParameterStructureValue<int>(parBCollision,PS_BC_GROUP,src->collisionGroup);
	SetParameterStructureValue<float>(parBCollision,PS_BC_SKINWITDH,src->skinWidth);


	SetParameterStructureValue<int>(parGroupsMask,0,src->groupsMask.bits0);
	SetParameterStructureValue<int>(parGroupsMask,1,src->groupsMask.bits1);
	SetParameterStructureValue<int>(parGroupsMask,2,src->groupsMask.bits2);
	SetParameterStructureValue<int>(parGroupsMask,3,src->groupsMask.bits3);

	//----------------------------------------------------------------
	//
	// CCD
	//

	/*
	SetParameterStructureValue<float>(parBCCD,PS_B_CCD_MOTION_THRESHOLD,src->ccdMotionThresold);
	SetParameterStructureValue<int>(parBCCD,PS_B_CCD_FLAGS,src->ccdFlags);
	SetParameterStructureValue<int>(parBCCD,PS_B_CCD_MESH_REFERENCE,src->ccdMeshReference);
	SetParameterStructureValue<float>(parBCCD,PS_B_CCD_SCALE,src->ccdScale);

	*/
	return 1;

}


int IParameter::copyTo(pCollisionSettings &dst,CKParameter*src)
{

	//----------------------------------------------------------------
	//
	// Sanity checks
	//
	#ifdef _DEBUG
		assert(src);
	#endif 


	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;

	CKParameterOut *parGroupsMask = GetParameterFromStruct(src,PS_BC_GROUPSMASK);
	
	// Sanity checks
	#ifdef _DEBUG
		assert(parGroupsMask);
	#endif 
	//----------------------------------------------------------------
	//
	// Collision Setup 
	//
	dst.collisionGroup = GetValueFromParameterStruct<int>(src,PS_BC_GROUP,false);

	//	
	dst.groupsMask.bits0  = GetValueFromParameterStruct<int>(parGroupsMask,0);
	dst.groupsMask.bits1  = GetValueFromParameterStruct<int>(parGroupsMask,1);
	dst.groupsMask.bits2  = GetValueFromParameterStruct<int>(parGroupsMask,2);
	dst.groupsMask.bits3  = GetValueFromParameterStruct<int>(parGroupsMask,3);

	dst.skinWidth = GetValueFromParameterStruct<float>(src,PS_BC_SKINWITDH,false);
	return 1;
}

int IParameter::copyTo(pObjectDescr*dst,CKParameter*src)
{

	//----------------------------------------------------------------
	//
	// Sanity checks
	//
	#ifdef _DEBUG
		assert(dst);
		assert(src);
	#endif 

	//----------------------------------------------------------------
	//
	// Possible this function is invoked due the loading of a cmo file whilst core is not started yet : 
	//
	CKStructHelper sHelper(src);
	if(sHelper.GetMemberCount() == 0)
		return 0;

	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;

	//----------------------------------------------------------------
	//
	// Retrieve all sub parameters : 
	//
	CKParameterOut *parBCommon =  GetParameterFromStruct(src,PS_COMMON_SETTINGS);
	//CKParameterOut *parBPivot =  GetParameterFromStruct(src,PS_PIVOT);
	//CKParameterOut *parBMass =  GetParameterFromStruct(src,PS_MASS);
	CKParameterOut *parBCollision =  GetParameterFromStruct(src,PS_COLLISION_SETTINGS);
		//CKParameterOut *parBCCD =  GetParameterFromStruct(parBCollision,PS_BC_CCD_SETUP);
		CKParameterOut *parGroupsMask = GetParameterFromStruct(parBCollision,PS_BC_GROUPSMASK);

	#ifdef _DEBUG
		assert(parBCommon);
		//assert(parBPivot);
		//assert(parBMass);
		assert(parBCollision);
	#endif 


	//----------------------------------------------------------------
	//
	// Common settings as hull type, body flags, ...
	//

	dst->hullType =(HullType)GetValueFromParameterStruct<int>(parBCommon,PS_BC_HULL_TYPE,false);
	dst->flags =  (BodyFlags)GetValueFromParameterStruct<int>(parBCommon,PS_BC_FLAGS,false);
	/*dst->transformationFlags = GetValueFromParameterStruct<int>(parBCommon,PS_BC_TFLAGS,false);*/
	dst->worlReference = GetValueFromParameterStruct<CK_ID>(parBCommon,PS_BC_WORLD,false);
	dst->density = GetValueFromParameterStruct<float>(parBCommon,PS_BC_DENSITY,false);


	//----------------------------------------------------------------
	//
	// Pivot Offset
	//

	/*
	dst->pivotOffsetLinear = GetValueFromParameterStruct<VxVector>(parBPivot,PS_BP_LINEAR,false);
	dst->pivotOffsetAngular = GetValueFromParameterStruct<VxVector>(parBPivot,PS_BP_ANGULAR,false);

	//----------------------------------------------------------------
	//
	// Mass	Offset
	//
	
	dst->density = GetValueFromParameterStruct<float>(parBMass,PS_BM_DENSITY,false);
	dst->totalMass = GetValueFromParameterStruct<float>(parBMass,PS_BM_TOTAL_MASS,false);
	dst->massOffsetLinear = GetValueFromParameterStruct<VxVector>(parBMass,PS_BM_PIVOT_POS,false);
	*/
	//dst->massOffsetAngular = GetValueFromParameterStruct<VxVector>(parBMass,PS_BM_PIVOT_ROTATION,false);


	
	//----------------------------------------------------------------
	//
	// Collision Setup 
	//
	dst->collisionGroup = GetValueFromParameterStruct<int>(parBCollision,PS_BC_GROUP,false);
	
	//	
	dst->groupsMask.bits0  = GetValueFromParameterStruct<int>(parGroupsMask,0);
	dst->groupsMask.bits1  = GetValueFromParameterStruct<int>(parGroupsMask,1);
	dst->groupsMask.bits2  = GetValueFromParameterStruct<int>(parGroupsMask,2);
	dst->groupsMask.bits3  = GetValueFromParameterStruct<int>(parGroupsMask,3);

	dst->skinWidth = GetValueFromParameterStruct<float>(parBCollision,PS_BC_SKINWITDH,false);
	
	dst->collision.groupsMask.bits0 = GetValueFromParameterStruct<int>(parGroupsMask,0);
	dst->collision.groupsMask.bits1 = GetValueFromParameterStruct<int>(parGroupsMask,1);
	dst->collision.groupsMask.bits2 = GetValueFromParameterStruct<int>(parGroupsMask,2);
	dst->collision.groupsMask.bits3 = GetValueFromParameterStruct<int>(parGroupsMask,3);
	
	dst->collision.skinWidth = GetValueFromParameterStruct<float>(parBCollision,PS_BC_SKINWITDH,false);
	dst->collision.collisionGroup = GetValueFromParameterStruct<int>(parBCollision,PS_BC_GROUP,false);

	//----------------------------------------------------------------
	//
	// CCD Settings
	//
	/*dst->ccdFlags = GetValueFromParameterStruct<int>(parBCCD,PS_B_CCD_FLAGS,false);
	dst->ccdMotionThresold= GetValueFromParameterStruct<float>(parBCCD,PS_B_CCD_MOTION_THRESHOLD,false);
	dst->ccdScale = GetValueFromParameterStruct<float>(parBCCD,PS_B_CCD_SCALE,false);
	dst->ccdMeshReference = GetValueFromParameterStruct<CK_ID>(parBCCD,PS_B_CCD_MESH_REFERENCE,false);
	*/

	dst->mask << OD_Collision;

	//----------------------------------------------------------------
	//
	// Misc
	//
	dst->version  = pObjectDescr::E_OD_VERSION::OD_DECR_V1;

	return 1;
}
IParameter* IParameter::Instance()
{
	if (!gIPar)
	{
		gIPar  = new IParameter(GetPMan());
	}
	return gIPar;
}



IParameter::IParameter(PhysicManager*_pManager) : mManager(_pManager)
{
	gIPar = this;

}