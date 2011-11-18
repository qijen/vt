#include <StdAfx.h>
#include "vtPhysXAll.h"
#include <xDebugTools.h>

void pRigidBody::updateMaterialSettings(pMaterial& material,CK3dEntity*shapeReference/* =NULL */)
{

	if (shapeReference==NULL)
		assert (getMainShape());

	NxShape *shape = getSubShape(shapeReference);
	if (shape)
	{

		pMaterial &bMaterial = material;
		if (bMaterial.xmlLinkID !=0)
		{
			int bIndex = bMaterial.xmlLinkID;
			XString nodeName = vtAgeia::getEnumDescription(GetPMan()->GetContext()->GetParameterManager(),VTE_XML_MATERIAL_TYPE,bMaterial.xmlLinkID);
			bool err = pFactory::Instance()->loadFrom(bMaterial,nodeName.Str(), pFactory::Instance()->getDefaultDocument() );
		}
		iAssertW( bMaterial.isValid(),pFactory::Instance()->findSettings(bMaterial,shapeReference),
				"Material settings invalid, try to find material attributes");

		iAssertW( bMaterial.isValid(),bMaterial.setToDefault(),
				"Material settings were still invalid : ");

		NxMaterialDesc nxMatDescr;
		pFactory::Instance()->copyTo(nxMatDescr,bMaterial);
		NxMaterial *nxMaterial  = getWorld()->getScene()->createMaterial(nxMatDescr);
		if (nxMaterial)
		{
			shape->setMaterial(nxMaterial->getMaterialIndex());
		}
	}
}

void pRigidBody::updatePivotSettings(pPivotSettings pivot,CK3dEntity*shapeReference/* =NULL */)
{

	if (shapeReference==NULL)
		assert (getMainShape());

	NxShape *shape = getSubShape(shapeReference);
	if (shape)
	{
        
		// Referential
		CK3dEntity* pivotRef = (CK3dEntity*)GetPMan()->GetContext()->GetObject(pivot.pivotReference);

		VxVector pivotLocalOut = pivot.localPosition;
		//----------------------------------------------------------------
		//
		// position
		//
		if (pivotRef)
		{
			pivotRef->Transform(&pivotLocalOut,&pivotLocalOut);
			shape->setGlobalPosition(getFrom(pivotLocalOut));
		}else
		{
			shape->setLocalPosition(getFrom(pivotLocalOut));
		}
		//----------------------------------------------------------------
		//
		// rotational offset  : todo
		//
		VxMatrix mat;
		Vx3DMatrixFromEulerAngles(mat,pivot.localOrientation.x,pivot.localOrientation.y,pivot.localOrientation.z);

		VxQuaternion outQuat;
		outQuat.FromMatrix(mat);
		VxQuaternion referenceQuat=outQuat;
		if (pivotRef)
		{
			pivotRef->GetQuaternion(&referenceQuat,NULL);
			shape->setLocalOrientation(getFrom(referenceQuat));
		}else{

			shape->setLocalOrientation(getFrom(outQuat));
		}
	}
}

void pRigidBody::saveToAttributes(pObjectDescr* oDescr)
{
	if (!oDescr)
		return;
		

	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;


	int attTypeActor   = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_ACTOR);
	int attTypeMaterial = GetPMan()->getAttributeTypeByGuid(VTS_MATERIAL);
	int attTypeOptimization = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_ACTOR_OPTIMIZATION);
	int attTypeCCD = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_CCD_SETTINGS);

	//----------------------------------------------------------------
	//
	// disable attribute callback 
	//
	CKAttributeManager *aMan = GetPMan()->GetContext()->GetAttributeManager();
	if (aMan)
	{
		//aMan->SetAttributeCallbackFunction(attTypeActor)
	}

	CK3dEntity *referenceObject = GetVT3DObject();

	//////////////////////////////////////////////////////////////////////////
	// we remove the old physic attribute : 
	if (referenceObject->HasAttribute(attTypeActor))
	{
		referenceObject->RemoveAttribute(attTypeActor);
	}
	referenceObject->SetAttribute(attTypeActor);

	CKParameterOut* actorAttribute = referenceObject->GetAttributeParameter(attTypeActor);

	//----------------------------------------------------------------
	//
	// Common Settings
	//
	CKParameterOut *parBCommon =  GetParameterFromStruct(actorAttribute,PS_COMMON_SETTINGS);
	if (parBCommon)
	{
		SetParameterStructureValue<int>(parBCommon,PS_BC_HULL_TYPE,oDescr->hullType);
		SetParameterStructureValue<int>(parBCommon,PS_BC_FLAGS,oDescr->flags);
		SetParameterStructureValue<float>(parBCommon,PS_BC_DENSITY,oDescr->density);
		SetParameterStructureValue<CK_ID>(parBCommon,PS_BC_WORLD,oDescr->worlReference);
	}

	//----------------------------------------------------------------
	//
	// Collision Setting
	//
	CKParameterOut *parBCollision =  GetParameterFromStruct(actorAttribute,PS_COLLISION_SETTINGS);
	CKParameterOut *parGroupsMask = GetParameterFromStruct(parBCollision,PS_BC_GROUPSMASK);
	if (parBCollision)
	{
		SetParameterStructureValue<int>(parBCollision,PS_BC_GROUP,oDescr->collisionGroup);
		SetParameterStructureValue<float>(parBCollision,PS_BC_SKINWITDH,oDescr->skinWidth);

		if (parGroupsMask)
		{
			SetParameterStructureValue<int>(parGroupsMask,0,oDescr->groupsMask.bits0);
			SetParameterStructureValue<int>(parGroupsMask,1,oDescr->groupsMask.bits1);
			SetParameterStructureValue<int>(parGroupsMask,2,oDescr->groupsMask.bits2);
			SetParameterStructureValue<int>(parGroupsMask,3,oDescr->groupsMask.bits3);
		}
	}

	//----------------------------------------------------------------
	//
	// Optimization
	//
	if (oDescr->mask & OD_Optimization)
	{
		if (referenceObject->HasAttribute(attTypeOptimization))
		{
			referenceObject->RemoveAttribute(attTypeOptimization);
		}

		referenceObject->SetAttribute(attTypeOptimization);

		
		CKParameterOut *parBOptimization =  referenceObject->GetAttributeParameter(attTypeOptimization);
		if (parBOptimization)
		{
			SetParameterStructureValue<int>(parBOptimization,PS_BO_LOCKS,oDescr->optimization.transformationFlags);
			SetParameterStructureValue<int>(parBOptimization,PS_BO_SOLVER_ITERATIONS,oDescr->optimization.solverIterations);
			SetParameterStructureValue<int>(parBOptimization,PS_BO_DOMINANCE_GROUP,oDescr->optimization.dominanceGroup);
			SetParameterStructureValue<int>(parBOptimization,PS_BO_COMPARTMENT_ID,oDescr->optimization.compartmentGroup);
		}

		//----------------------------------------------------------------
		//
		// sleeping
		//
		CKParameterOut *parBSleeping =  GetParameterFromStruct(parBOptimization,PS_BO_SLEEPING);
		if (parBSleeping)
		{
			SetParameterStructureValue<float>(parBSleeping,PS_BS_ANGULAR_SLEEP,oDescr->optimization.angSleepVelocity);
			SetParameterStructureValue<float>(parBSleeping,PS_BS_LINEAR_SLEEP,oDescr->optimization.linSleepVelocity);
			SetParameterStructureValue<float>(parBSleeping,PS_BS_THRESHOLD,oDescr->optimization.sleepEnergyThreshold);
		}

		//----------------------------------------------------------------
		//
		// damping
		//
		CKParameterOut *parBDamping =  GetParameterFromStruct(parBOptimization,PS_BO_DAMPING);
		if (parBDamping)
		{
			SetParameterStructureValue<float>(parBDamping,PS_BD_ANGULAR,oDescr->optimization.angDamping);
			SetParameterStructureValue<float>(parBDamping,PS_BD_LINEAR,oDescr->optimization.linDamping);
		}
	}

	//----------------------------------------------------------------
	//
	// CCD
	//
	if (oDescr->mask & OD_CCD )
	{
		if (referenceObject->HasAttribute(attTypeCCD))
		{
			referenceObject->RemoveAttribute(attTypeCCD);
		}
		
		referenceObject->SetAttribute(attTypeCCD);

		CKParameterOut *parBCCD =  referenceObject->GetAttributeParameter(attTypeCCD);
		if (parBCCD)
		{
			SetParameterStructureValue<float>(parBCCD,PS_B_CCD_MOTION_THRESHOLD,oDescr->ccd.motionThresold);
			SetParameterStructureValue<int>(parBCCD,PS_B_CCD_FLAGS,oDescr->ccd.flags);
			SetParameterStructureValue<float>(parBCCD,PS_B_CCD_SCALE,oDescr->ccd.scale);
			SetParameterStructureValue<CK_ID>(parBCCD,PS_B_CCD_MESH_REFERENCE,oDescr->ccd.meshReference);

		}
	}
	//----------------------------------------------------------------
	//
	// Material
	//
	if (oDescr->mask & OD_Material )
	{
		if (referenceObject->HasAttribute(attTypeMaterial))
		{
			referenceObject->RemoveAttribute(attTypeMaterial);
		}

		referenceObject->SetAttribute(attTypeMaterial);

		CKParameterOut *parBMaterial =  referenceObject->GetAttributeParameter(attTypeMaterial);
		if (parBMaterial)
		{
			pFactory::Instance()->copyTo(parBMaterial,oDescr->material);
		}
	}


 }



 void pRigidBody::updateOptimizationSettings(pOptimization optimization)
{
	
	lockTransformation(optimization.transformationFlags);
	setSolverIterationCount(optimization.solverIterations);
	getActor()->setDominanceGroup(optimization.dominanceGroup);
	setAngularDamping(optimization.angDamping);
	setLinearDamping(optimization.linDamping);
	setSleepAngularVelocity(optimization.angSleepVelocity);
	setSleepLinearVelocity(optimization.linSleepVelocity);
	setSleepEnergyThreshold(optimization.sleepEnergyThreshold);
}
void pRigidBody::setSleepAngularVelocity(float threshold)
{
	getActor()->setSleepAngularVelocity(threshold);

}
void pRigidBody::setSleepLinearVelocity(float threshold)
{
	getActor()->setSleepLinearVelocity(threshold);
}
void pRigidBody::setSleepEnergyThreshold(float threshold)
{
	getActor()->setSleepEnergyThreshold(threshold);
}

void pRigidBody::setDominanceGroup(int dominanceGroup)
{
	getActor()->setDominanceGroup(dominanceGroup);
}

void pRigidBody::setSolverIterationCount(int count)
{

	

	if (getActor())
	{
		if (count > 0)
		{
			getActor()->setSolverIterationCount(count);
		}
	}
}

void pRigidBody::checkForOptimization()
{

	int att = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_ACTOR_OPTIMIZATION);
	CK3dEntity *ent = (CK3dEntity*)ctx()->GetObject(mEntID);
	if ( !ent )
		return;

	if (!ent->HasAttribute(att)) 
		return;



	CKParameterOut *optPar = ent->GetAttributeParameter(att);
	if(!optPar)
		return;


	using namespace vtTools::ParameterTools;
	using namespace vtTools::AttributeTools;

	//////////////////////////////////////////////////////////////////////////
	//
	//		Lock transformation degrees :
	//
	int transformationlockFlags = GetValueFromAttribute<int>(ent,att,PS_BO_LOCKS);
	lockTransformation(transformationlockFlags);

	//////////////////////////////////////////////////////////////////////////
	//
	//		Damping
	//
	CKParameterOut * dampPar= GetParameterFromStruct(optPar,PS_BO_DAMPING);

	float linDamp  = GetValueFromParameterStruct<float>(dampPar,PS_BD_LINEAR);
	float angDamp  = GetValueFromParameterStruct<float>(dampPar,PS_BD_ANGULAR);
	if ( linDamp !=0.0f)
		setLinearDamping(linDamp);

	if ( angDamp!=0.0f)
		setAngularDamping(angDamp);

	///////////////////////////////////////////////////////////////////////////
	//
	//		Sleeping Settings
	//

	CKParameterOut * sleepPar= GetParameterFromStruct(optPar,PS_BO_SLEEPING);
	float linSleep  = GetValueFromParameterStruct<float>(dampPar,PS_BS_LINEAR_SLEEP);


	//////////////////////////////////////////////////////////////////////////
	//
	//		Solver Iterations 
	//
	//CKParameterOut * sleepPar= GetParameterFromStruct(optPar,);
	int solverIterations  = GetValueFromParameterStruct<float>(optPar,PS_BO_SOLVER_ITERATIONS);
	if (solverIterations !=0)
	{
		setSolverIterationCount(solverIterations);
	}

	//////////////////////////////////////////////////////////////////////////
	//
	// Dominance 
	//
	int dGroup  = GetValueFromParameterStruct<int>(optPar,PS_BO_DOMINANCE_GROUP);
	if (dGroup!=0)
	{
		getActor()->setDominanceGroup(dGroup);
	}



}
void pRigidBody::checkDataFlags()
{

	using namespace vtTools::AttributeTools;
	int att = GetPMan()->GetPAttribute();
	int att_damping  = GetPMan()->att_damping;
	int att_ss = GetPMan()->att_sleep_settings;
	int att_surface =GetPMan()->att_surface_props;
	int att_deformable =GetPMan()->att_deformable;

	xBitSet& dFlags = getDataFlags();
	CK3dEntity *ent = GetVT3DObject();

	if (!ent)	{		return;	}

	if (ent->HasAttribute(att_surface))
		enableFlag(dFlags,EDF_MATERIAL_PARAMETER);
	else		disableFlag(dFlags,EDF_MATERIAL_PARAMETER);

	if (ent->HasAttribute(att_damping))
		enableFlag(dFlags,EDF_DAMPING_PARAMETER);
	else	disableFlag(dFlags,EDF_DAMPING_PARAMETER);

	if (ent->HasAttribute(att_ss))
		enableFlag(dFlags,EDF_SLEEPING_PARAMETER);
	else disableFlag(dFlags,EDF_SLEEPING_PARAMETER);

	if (ent->HasAttribute(att_deformable))
		enableFlag(dFlags,EDF_DEFORMABLE_PARAMETER);
	else disableFlag(dFlags,EDF_DEFORMABLE_PARAMETER);


	if (ent->HasAttribute( GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_ACTOR_OPTIMIZATION) ))
		enableFlag(dFlags,EDF_OPTIMIZATION_PARAMETER);
	else disableFlag(dFlags,EDF_OPTIMIZATION_PARAMETER);

}
void pRigidBody::destroy()
{
	if(!getActor())
		return;

	NxU32 nbShapes = getActor()->getNbShapes();


	/*
	CKSTRING name  = GetVT3DObject()->GetName();
	while(nbShapes )
	{

	NxShape *s =  ((NxShape **)getActor()->getShapes())[0];
	pSubMeshInfo *sinfo = static_cast<pSubMeshInfo*>(s->userData);

	//////////////////////////////////////////////////////////////////////////
	//		Wheel attached !
	if (sinfo  && s->isWheel() && sinfo->wheel )
	{
	pWheel2 *wheel = (pWheel2*)sinfo->wheel;
	delete wheel;
	wheel = NULL;
	sinfo->wheel =NULL;
	}

	if (sinfo)
	{
	delete sinfo;
	sinfo = NULL;
	}

	s->userData = NULL;

	if (getActor()->isDynamic() || (	!getActor()->isDynamic() && nbShapes >1 )	)
	{
	getActor()->releaseShape(*s);
	s=NULL;
	destroy();
	}break;
	}

	*/


	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	pVehicle *v = getVehicle();
	if ( v )
	{
		getVehicle()->getWheels().clear();
		getVehicle()->setActor(NULL);
		getVehicle()->setBody(NULL);

		delete v;
		setVehicle(NULL);
	}

	pCloth *cloth = getCloth();
	if (cloth)
	{
		cloth->releaseReceiveBuffers();
		getActor()->getScene().releaseCloth(*cloth->getCloth());
	}
	getActor()->getScene().releaseActor(*getActor());
	setActor(NULL);
}

int pRigidBody::getTransformationsLockFlags()
{

	int result = 0 ; 
	if (getActor()->readBodyFlag(NX_BF_FROZEN_POS_X))result|=NX_BF_FROZEN_POS_X;
	if (getActor()->readBodyFlag(NX_BF_FROZEN_POS_Y))result|=NX_BF_FROZEN_POS_Y;
	if (getActor()->readBodyFlag(NX_BF_FROZEN_POS_Z))result|=NX_BF_FROZEN_POS_Z;

	if (getActor()->readBodyFlag(NX_BF_FROZEN_ROT_X))result|=NX_BF_FROZEN_ROT_X;
	if (getActor()->readBodyFlag(NX_BF_FROZEN_ROT_Y))result|=NX_BF_FROZEN_ROT_Y;
	if (getActor()->readBodyFlag(NX_BF_FROZEN_ROT_Z))result|=NX_BF_FROZEN_ROT_Z;

	return result;

}
void pRigidBody::lockTransformation(int flags)
{

	
	if (getActor() && getActor()->isDynamic())
	{
		if (flags & NX_BF_FROZEN_POS_X)
			getActor()->raiseBodyFlag(NX_BF_FROZEN_POS_X);
		else
			getActor()->clearBodyFlag(NX_BF_FROZEN_POS_X);


		if (flags & NX_BF_FROZEN_POS_Y)
			getActor()->raiseBodyFlag(NX_BF_FROZEN_POS_Y);
		else
			getActor()->clearBodyFlag(NX_BF_FROZEN_POS_Y);

		if (flags & NX_BF_FROZEN_POS_Z)
			getActor()->raiseBodyFlag(NX_BF_FROZEN_POS_Z);
		else
			getActor()->clearBodyFlag(NX_BF_FROZEN_POS_Z);


		if (flags & NX_BF_FROZEN_ROT_X)
			getActor()->raiseBodyFlag(NX_BF_FROZEN_ROT_X);
		else
			getActor()->clearBodyFlag(NX_BF_FROZEN_ROT_X);

		if (flags & NX_BF_FROZEN_ROT_Y)
			getActor()->raiseBodyFlag(NX_BF_FROZEN_ROT_Y);
		else
			getActor()->clearBodyFlag(NX_BF_FROZEN_ROT_Y);

		if (flags & NX_BF_FROZEN_ROT_Z)
			getActor()->raiseBodyFlag(NX_BF_FROZEN_ROT_Z);
		else
			getActor()->clearBodyFlag(NX_BF_FROZEN_ROT_Z);
	}
}
bool pRigidBody::isSleeping()const
{
	if (getActor())
	{
		return getActor()->isSleeping();
	}
	return true;
}

void pRigidBody::setSleeping(bool sleeping)
{
	if (getActor())
	{
		if (sleeping)
		{
			getActor()->putToSleep();
		}else{
			getActor()->wakeUp();
		}
	}
}
void pRigidBody::enableGravity(bool enable)
{

	if (getActor())
	{
		if (!enable)
		{
			getActor()->raiseBodyFlag(NX_BF_DISABLE_GRAVITY);
		}else{
			getActor()->clearBodyFlag(NX_BF_DISABLE_GRAVITY);
		}
	}
}

bool pRigidBody::isAffectedByGravity()const
{
	if (getActor())
	{
		return !getActor()->readBodyFlag(NX_BF_DISABLE_GRAVITY);
	}
	return false;
}
void pRigidBody::setKinematic(bool enabled)
{
	if (getActor())
	{
		if (enabled)
			getActor()->raiseBodyFlag(NX_BF_KINEMATIC);
		else
			getActor()->clearBodyFlag(NX_BF_KINEMATIC);
	}
}
bool pRigidBody::isKinematic()const
{
	if (getActor())
	{
		return getActor()->readBodyFlag(NX_BF_KINEMATIC);
	}
	return false;
}

bool pRigidBody::isValid()const { 	
	
	return GetPMan()->GetContext()->GetObject(getEntID())  ? true : false && getActor() ? true : false;
}
int pRigidBody::getHullType(){		return m_HullType;}
void pRigidBody::setHullType(int _type){ 	m_HullType= _type;}
void pRigidBody::recalculateFlags(int _flags)
{
	if (!isValid())
	{
		return;
	}

	int current = m_sFlags;

	if (getActor()->isDynamic() && !getActor()->readBodyFlag(NX_BF_DISABLE_GRAVITY))
	{
		current |=BF_Gravity;
	}


	if (!getActor()->readActorFlag(NX_AF_DISABLE_COLLISION))
	{
		current |=BF_Collision;
	}

	if (getActor()->isSleeping())
	{
		current |=BF_Sleep;
	}

	if (getActor()->getContactReportFlags() & NX_NOTIFY_ON_TOUCH )
	{
		current |=BF_CollisionNotify;
	}

	if (isKinematic())
	{
		current |=BF_Kinematic;
	}
	
	m_sFlags = current;
}

void pRigidBody::updateFlags( int _flags,CK3dEntity*shapeReference/*=NULL*/ )
{

	if (!isValid())return;

	setKinematic(_flags & BF_Kinematic);
	enableCollision(_flags & BF_Collision,shapeReference);
	enableCollisionsNotify(_flags & BF_CollisionNotify);
	enableGravity(_flags & BF_Gravity);
	setSleeping(_flags & BF_Sleep);
	enableTriggerShape(_flags & BF_TriggerShape,shapeReference);
	m_sFlags |=_flags;

}

int pRigidBody::getFlags(){	
	
	return m_sFlags;;
}
void pRigidBody::setFlags(int _flags){	m_sFlags = _flags;}
void pRigidBody::retrieveSettingsFromAttribute()
{

	assert(getWorld());
	assert(getWorld()->getScene());
	assert(getWorld()->getReference());
	assert(GetVT3DObject());
	
	using namespace vtTools::AttributeTools;
	setDataFlags(0x000);

	int att = GetPMan()->GetPAttribute();

	//----------------------------------------------------------------
	//
	//	Old version 
	//

	CK_ID id = getWorld()->getReference()->GetID();
	SetAttributeValue<CK_ID>(GetVT3DObject(),att,E_PPS_WORLD,&id);
	setHullType(GetValueFromAttribute<int>(GetVT3DObject(),att,E_PPS_HULLTYPE));

	
	//Sets some flags like movable , etc...
	setFlags(GetValueFromAttribute<int>(GetVT3DObject(),att,E_PPS_BODY_FLAGS));
	setDensity(GetValueFromAttribute<float>(GetVT3DObject(),att, E_PPS_DENSITY));
	setMassOffset(GetValueFromAttribute<VxVector>(GetVT3DObject(),att, E_PPS_MASS_OFFSET));
	setPivotOffset(GetValueFromAttribute<VxVector>(GetVT3DObject(),att, E_PPS_SHAPE_OFFSET));
	setSkinWidth(GetValueFromAttribute<float>(GetVT3DObject(),att, E_PPS_SKIN_WIDTH));



}
int pRigidBody::isBodyFlagOn(int flags)
{
	if (getActor() && getActor()->isDynamic())
	{
		return getActor()->readBodyFlag((NxBodyFlag)flags);
	}
	return -1;
}
void pRigidBody::wakeUp(float wakeCounterValue/* =NX_SLEEP_INTERVAL */)
{
	getActor()->wakeUp(wakeCounterValue);

}



