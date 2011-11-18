#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "vtAttributeHelper.h"

#include "pCommon.h"
#include "IParameter.h"
#include "vtBBHelper.h"
#include "xDebugTools.h"

void PhysicManager::_cleanOrphanedJoints()
{

	pWorldMapIt it  = getWorlds()->Begin();
	while(it != getWorlds()->End())
	{
		pWorld *w = *it;
		NxU32	jointCount = w->getScene()->getNbJoints();
		if (jointCount)
		{
			NxArray< NxJoint * >joints;
			w->getScene()->resetJointIterator();
			for	(NxU32 i = 0;	i	<	jointCount;	++i)
			{
				NxJoint	*j = w->getScene()->getNextJoint();
				NxActor *a= NULL;
				NxActor *b= NULL;
				if (a == NULL  && b==NULL)
				{
					w->getScene()->releaseJoint(*j);
					pJoint *mJoint = static_cast<pJoint*>( j->userData );
					/*if(mJoint)
						mJoint->setJoint(NULL);
					SAFE_DELETE(mJoint);
					*/

				}
			}
		}
		it++;
	}
}


void PhysicManager::_removeObjectsFromOldScene(CKScene *lastScene)
{
	if (!lastScene)
		return;

	CKSceneObjectIterator objIt = lastScene->GetObjectIterator();
	CKObject *tpObj;
	while(!objIt.End())
	{	
		CK3dEntity *tpObj = (CK3dEntity *)GetContext()->GetObject(objIt.GetObjectID());
		if (tpObj)
		{
			pRigidBody *body = getBody(tpObj);
			if (body)
			{
				body->getWorld()->deleteBody(body);
			}
		}
		objIt++;
	}
}

void PhysicManager::copyToAttributes( pObjectDescr src,CK3dEntity *dst )
{

	if (!dst)
		return;

	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;


	int attTypeActor   = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_ACTOR);
	int attTypeMaterial = GetPMan()->getAttributeTypeByGuid(VTS_MATERIAL);
	int attTypeOptimization = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_ACTOR_OPTIMIZATION);
	int attTypeCCD = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_CCD_SETTINGS);
	int attTypePivot = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_PIVOT_OFFSET);
	int attTypeMass = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_MASS_SETUP);
	int attTypeCapsule= GetPMan()->getAttributeTypeByGuid(VTS_CAPSULE_SETTINGS_EX);
	int attTypeCCyl= GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_CONVEX_CYLDINDER_WHEEL_DESCR);

	//----------------------------------------------------------------
	//
	// disable attribute callback 
	//
	CKAttributeManager *aMan = GetPMan()->GetContext()->GetAttributeManager();
	if (aMan)
	{
		//aMan->SetAttributeCallbackFunction(attTypeActor)
	}

	CK3dEntity *referenceObject = dst;

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
		SetParameterStructureValue<int>(parBCommon,PS_BC_HULL_TYPE,src.hullType);
		SetParameterStructureValue<int>(parBCommon,PS_BC_FLAGS,src.flags);
		SetParameterStructureValue<float>(parBCommon,PS_BC_DENSITY,src.density);
		SetParameterStructureValue<CK_ID>(parBCommon,PS_BC_WORLD,src.worlReference);
	}

	//----------------------------------------------------------------
	//
	// Collision Setting
	//
	CKParameterOut *parBCollision =  GetParameterFromStruct(actorAttribute,PS_COLLISION_SETTINGS);
	CKParameterOut *parGroupsMask = GetParameterFromStruct(parBCollision,PS_BC_GROUPSMASK);
	if (parBCollision)
	{
		SetParameterStructureValue<int>(parBCollision,PS_BC_GROUP,src.collisionGroup);
		SetParameterStructureValue<float>(parBCollision,PS_BC_SKINWITDH,src.skinWidth);

		if (parGroupsMask)
		{
			SetParameterStructureValue<int>(parGroupsMask,0,src.groupsMask.bits0);
			SetParameterStructureValue<int>(parGroupsMask,1,src.groupsMask.bits1);
			SetParameterStructureValue<int>(parGroupsMask,2,src.groupsMask.bits2);
			SetParameterStructureValue<int>(parGroupsMask,3,src.groupsMask.bits3);
		}
	}

	//----------------------------------------------------------------
	//
	// Optimization
	//
	if (src.mask & OD_Optimization)
	{
		if (referenceObject->HasAttribute(attTypeOptimization))
		{
			referenceObject->RemoveAttribute(attTypeOptimization);
		}

		referenceObject->SetAttribute(attTypeOptimization);


		CKParameterOut *parBOptimization =  referenceObject->GetAttributeParameter(attTypeOptimization);
		if (parBOptimization)
		{
			SetParameterStructureValue<int>(parBOptimization,PS_BO_LOCKS,src.optimization.transformationFlags);
			SetParameterStructureValue<int>(parBOptimization,PS_BO_SOLVER_ITERATIONS,src.optimization.solverIterations);
			SetParameterStructureValue<int>(parBOptimization,PS_BO_DOMINANCE_GROUP,src.optimization.dominanceGroup);
			SetParameterStructureValue<int>(parBOptimization,PS_BO_COMPARTMENT_ID,src.optimization.compartmentGroup);
		}

		//----------------------------------------------------------------
		//
		// sleeping
		//
		CKParameterOut *parBSleeping =  GetParameterFromStruct(parBOptimization,PS_BO_SLEEPING);
		if (parBSleeping)
		{
			SetParameterStructureValue<float>(parBSleeping,PS_BS_ANGULAR_SLEEP,src.optimization.angSleepVelocity);
			SetParameterStructureValue<float>(parBSleeping,PS_BS_LINEAR_SLEEP,src.optimization.linSleepVelocity);
			SetParameterStructureValue<float>(parBSleeping,PS_BS_THRESHOLD,src.optimization.sleepEnergyThreshold);
		}

		//----------------------------------------------------------------
		//
		// damping
		//
		CKParameterOut *parBDamping =  GetParameterFromStruct(parBOptimization,PS_BO_DAMPING);
		if (parBDamping)
		{
			SetParameterStructureValue<float>(parBDamping,PS_BD_ANGULAR,src.optimization.angDamping);
			SetParameterStructureValue<float>(parBDamping,PS_BD_LINEAR,src.optimization.linDamping);
		}
	}

	//----------------------------------------------------------------
	//
	// CCD
	//
	if (src.mask & OD_CCD )
	{
		if (referenceObject->HasAttribute(attTypeCCD))
		{
			referenceObject->RemoveAttribute(attTypeCCD);
		}

		referenceObject->SetAttribute(attTypeCCD);

		CKParameterOut *parBCCD =  referenceObject->GetAttributeParameter(attTypeCCD);
		if (parBCCD)
		{
			SetParameterStructureValue<float>(parBCCD,PS_B_CCD_MOTION_THRESHOLD,src.ccd.motionThresold);
			SetParameterStructureValue<int>(parBCCD,PS_B_CCD_FLAGS,src.ccd.flags);
			SetParameterStructureValue<float>(parBCCD,PS_B_CCD_SCALE,src.ccd.scale);
			SetParameterStructureValue<CK_ID>(parBCCD,PS_B_CCD_MESH_REFERENCE,src.ccd.meshReference);

		}
	}
	//----------------------------------------------------------------
	//
	// Material
	//
	if (src.mask & OD_Material )
	{
		if (referenceObject->HasAttribute(attTypeMaterial))
		{
			referenceObject->RemoveAttribute(attTypeMaterial);
		}

		referenceObject->SetAttribute(attTypeMaterial);

		CKParameterOut *parBMaterial =  referenceObject->GetAttributeParameter(attTypeMaterial);
		if (parBMaterial)
		{
			pFactory::Instance()->copyTo(parBMaterial,src.material);
		}
	}


	//----------------------------------------------------------------
	//
	// Pivot
	//
	if (src.mask & OD_Pivot )
	{
		if (referenceObject->HasAttribute(attTypePivot))
		{
			referenceObject->RemoveAttribute(attTypePivot);
		}
		referenceObject->SetAttribute(attTypePivot);
		CKParameterOut *parBPivot =  referenceObject->GetAttributeParameter(attTypePivot);
		if (parBPivot)
		{
			IParameter::Instance()->copyTo(parBPivot,src.pivot);
		}
	}

	//----------------------------------------------------------------
	//
	// Mass
	//
	if (src.mask & OD_Mass )
	{
		if (referenceObject->HasAttribute(attTypeMass))
		{
			referenceObject->RemoveAttribute(attTypeMass);
		}
		referenceObject->SetAttribute(attTypeMass);
		CKParameterOut *parBMass =  referenceObject->GetAttributeParameter(attTypeMass);
		if (parBMass)
		{
			IParameter::Instance()->copyTo(parBMass,src.mass);
		}
	}
	//----------------------------------------------------------------
	//
	// Capsule
	//
	if (src.mask & OD_Capsule)
	{

		if (referenceObject->HasAttribute(attTypeCapsule))
		{
			referenceObject->RemoveAttribute(attTypeCapsule);
		}
		referenceObject->SetAttribute(attTypeCapsule);
		CKParameterOut *parBCapsule =  referenceObject->GetAttributeParameter(attTypeCapsule);
		if (parBCapsule)
		{
			IParameter::Instance()->copyTo(parBCapsule,src.capsule);
		}
	}

	//----------------------------------------------------------------
	//
	// convex Cylinder
	//
	if (src.mask & OD_ConvexCylinder)
	{

		if (referenceObject->HasAttribute(attTypeCCyl))
		{
			referenceObject->RemoveAttribute(attTypeCCyl);
		}
		referenceObject->SetAttribute(attTypeCCyl);
		CKParameterOut *parBCCyl=  referenceObject->GetAttributeParameter(attTypeCCyl);
		if (parBCCyl)
		{
			IParameter::Instance()->copyTo(parBCCyl,src.convexCylinder);
		}
	}
}

pVehicle*PhysicManager::getVehicle(CK3dEntity*bodyReference)
{

	pRigidBody *body=getBody(bodyReference);
	if (bodyReference && body )
	{
		return body->getVehicle();
	}
	return NULL;
}

pWheel2*PhysicManager::getWheel(CK3dEntity*wheelReference)
{

	pRigidBody *body=getBody(wheelReference);
	if (wheelReference && body )
	{
		return static_cast<pWheel2*>(body->getWheel(wheelReference));
	}
	return NULL;
}

NxShape *PhysicManager::getSubShape(CK3dEntity*referenceObject)
{
	if (!referenceObject)
		return NULL;

	pRigidBody *body = getBody(referenceObject);
	if (!body)
		return NULL;

	
	return body->getSubShape(referenceObject);


}
NxCCDSkeleton* PhysicManager::getCCDSkeleton(CKBeObject *shapeReference)
{

	pWorldMapIt it  = getWorlds()->Begin();
	int s = getWorlds()->Size();


	while(it != getWorlds()->End())
	{
		pWorld *w = *it;
		NxActor** actors = w->getScene()->getActors();
		int nbActors = w->getScene()->getNbActors();
		while(nbActors--)
		{
			NxActor* actor = *actors++;
			if(actor->userData != NULL)
			{
				NxU32 nbShapes = actor->getNbShapes();
				if ( nbShapes )
				{
					NxShape ** slist = (NxShape **)actor->getShapes();
					for (NxU32 j=0; j<nbShapes; j++)
					{
						NxShape *s = slist[j];
						pSubMeshInfo *sInfo = static_cast<pSubMeshInfo*>(s->userData);
						if (sInfo)
						{

						}

					}
				}
			}
		}
	}

	return NULL;
}

void PhysicManager::createWorlds(int flags)
{

#ifdef _DEBUG
	//assert(m_DefaultSleepingSettings());
//	assert(getWorlds());
#endif

	CKAttributeManager* attman = m_Context->GetAttributeManager();
	const XObjectPointerArray& Array = attman->GetAttributeListPtr(att_world_object);
	for (CKObject** it = Array.Begin(); it != Array.End(); ++it)
	{	
		CK3dEntity*target = static_cast<CK3dEntity*>(*it);
		pWorld *world  = getWorld(target->GetID());

		const char*name = target->GetName();
		if (world  == getDefaultWorld() )
		{
			continue;
		}
		//////////////////////////////////////////////////////////////////////////
		//world not registered :
		if (!world)
		{
			
			//at first we check for an attached sleeping settings attribute,  when not, we use this->DefaultDefaultSleepingSettings 
//			pSleepingSettings *sleepSettings  = target->HasAttribute(att_sleep_settings)   ? 
//				pFactory::Instance()->GetSleepingSettingsFromEntity(target) : &this->DefaultSleepingSettings(); 

			//we also retrieve objects world settings
			//pWorldSettings * worldSettings = pFactory::Instance()->GetWorldSettingsFromEntity(target);

			//now we can create the final world,the function initiates the world and also it inserts the world 
			//in our m_pWorlds array !
			//world   = pFactory::Instance()->createWorld(target,worldSettings,sleepSettings);
		}
	}
};
int PhysicManager::getNbWorlds(){	return getWorlds()->Size();}
void PhysicManager::checkClothes()
{

	if (!getNbObjects())
		return;

	if (!isValid())	performInitialization();
	if (!isValid())
	{

		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Couldn't make manager valid");
		return;
	}


	//////////////////////////////////////////////////////////////////////////
	// we iterate through all entities tagged with the physic attribute 
	CKAttributeManager* attman = m_Context->GetAttributeManager();
	const XObjectPointerArray& Array = attman->GetAttributeListPtr(att_clothDescr);
	for (CKObject** it = Array.Begin(); it != Array.End(); ++it)
	{	

		CK3dEntity *target = static_cast<CK3dEntity*>(*it);
		if (target)
		{
			const char *bName = target->GetName();
			// is the body registered in any world  ? 
		

			CK_ID wID = vtTools::AttributeTools::GetValueFromAttribute<CK_ID>(target,att_clothDescr,E_CS_WORLD_REFERENCE);
			CK3dEntity *worldReference   = (CK3dEntity*)m_Context->GetObject(wID);
			pWorld *world  = getWorld(wID);

			if (!worldReference)
			{
				continue;
			}
			
			if (getBody(target))
			{
				continue;
			}

			if (world->getShapeByEntityID(target->GetID()))
			{
				continue;
			}

			if (world && worldReference)
			{
				pCloth *cloth = world->getCloth(target);
				if (!cloth)
				{
					pClothDesc *descr = pFactory::Instance()->createClothDescrFromParameter(target->GetAttributeParameter(att_clothDescr));
					cloth  = pFactory::Instance()->createCloth(target,*descr);
					if(cloth)
					{
						//////////////////////////////////////////////////////////////////////////
						if (descr->flags & PCF_AttachToParentMainShape )
						{
							if (target->GetParent())
							{
								CK3dEntity *bodyReference = pFactory::Instance()->getMostTopParent(target);
								if (bodyReference)
								{
									pRigidBody *body  = GetPMan()->getBody(bodyReference);
									if (body)
									{
										cloth->attachToShape((CKBeObject*)bodyReference,descr->attachmentFlags);
									}
								}
							}
						}
						//////////////////////////////////////////////////////////////////////////
						if (descr->flags & PCF_AttachToCollidingShapes)
						{
							cloth->attachToCollidingShapes(descr->attachmentFlags);
						}
					}
				}
			}
		}
	}

}


bool isSceneObject(CK3dEntity *object)
{

	CKLevel *level = GetPMan()->GetContext()->GetCurrentLevel();
	if(level)
	{
		for(int i = 0 ; i < level->GetSceneCount()  ; i++ )
		{
			CKScene *scene = level->GetScene(i);
			if(scene && scene->IsObjectHere(object))
				return true;			
		}
	}
	return false;
}
void PhysicManager::checkWorlds()
{


	CKScene *currentScene  = NULL;

	CKLevel *level = GetContext()->GetCurrentLevel();
	if(level)
	{
		currentScene = level->GetCurrentScene();
	}

	if (!getNbObjects())
		return;

	if (!isValid())	performInitialization();
	if (!isValid())
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Couldn't make manager valid");
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	// we iterate through all entities tagged with the physic attribute 
	CKAttributeManager* attman = m_Context->GetAttributeManager();
	const XObjectPointerArray& Array = attman->GetAttributeListPtr(att_physic_object);
	for (CKObject** it = Array.Begin(); it != Array.End(); ++it)
	{	

		CK3dEntity *target = static_cast<CK3dEntity*>(*it);
		if (target && !isSceneObject(target))
		{
			const char *bName = target->GetName();
			// is the body registered in any world  ? 
			pRigidBody* body = getBody(target);
			if(!body)
			{
				//we retrieve the bodies target world : 
				CK_ID wID = vtTools::AttributeTools::GetValueFromAttribute<CK_ID>(target,GetPAttribute(),E_PPS_WORLD);
				int flags = vtTools::AttributeTools::GetValueFromAttribute<CK_ID>(target,GetPAttribute(),E_PPS_BODY_FLAGS);
				if (flags & BF_SubShape)
				{
					continue;
				}
				pWorld *world  = getWorld(wID) ? getWorld(wID) : getDefaultWorld();
				if(world)
				{
					
					//now create the final rigid body : 
					body  = pFactory::Instance()->createRigidBodyFull(target,world->getReference());
					GetPMan()->getCheckList().PushBack(target->GetID());
				}
			}
		}
	}

	
	checkClothes();
	_checkObjectsByAttribute(currentScene);



	//////////////////////////////////////////////////////////////////////////
	pWorldMapIt itx  = getWorlds()->Begin();
	while(itx != getWorlds()->End())
	{
		pWorld *w = *itx;
		if (w)
		{
			w->checkList();
		}
		itx++;
	}
}
pWorld *PhysicManager::getWorld(CK3dEntity *_o, CK3dEntity *body)
{
	pWorld *result=NULL;

	
	//////////////////////////////////////////////////////////////////////////
	//
	// Priorities : 
	//		1. the given world by _o
	//		2. DefaultWorld
	//		3. body physic attribute
	//		4. created default world
	//
	//////////////////////////////////////////////////////////////////////////
	// we try to get it over _o : 
	if (_o)
	{
		result  =  getWorld(_o->GetID());
		if (result  && getWorld(_o->GetID())->getReference() )
		{
			_o = result->getReference();
			return result;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//still nothing, we try to get the default world :
	result  = getDefaultWorld();
	if (result && getDefaultWorld()->getReference() )
	{
		CK_ID id  = getDefaultWorld()->getReference()->GetID();
		_o = result->getReference();
		return result;
	}

	//////////////////////////////////////////////////////////////////////////
	//we try to get it over the bodies attribute : 
	if (body)
	{
		if (body->HasAttribute(GetPAttribute()))
		{
			CK_ID id  = vtTools::AttributeTools::GetValueFromAttribute<CK_ID>(body,GetPMan()->GetPAttribute(),E_PPS_WORLD);
			_o  = static_cast<CK3dEntity*>(ctx()->GetObject(id));
			if (_o)
			{
				result  = getWorld(_o->GetID());
				if (result && getWorld(_o->GetID())->getReference())
				{
					_o = result->getReference();
					return result;
				}
				_o = NULL;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//still nothing 
	if (!getDefaultWorld())
	{
		return NULL;
		//result  = pFactory::Instance()->createDefaultWorld("pDefaultWorld");
		/*
		if (result)
		{
			_o  = getDefaultWorld()->getReference();
		}else{
			
			GetPMan()->performInitialization();
			result = GetPMan()->getDefaultWorld();
			_o = result->getReference();
		}

		*/
	}
	return result;
}
void PhysicManager::destroyWorlds()
{

	pWorldMapIt it  = getWorlds()->Begin();
	int s = getWorlds()->Size();


	while(it != getWorlds()->End())
	{
		pWorld *w = *it;
		if (w)
		{
			w->destroy();

			if (w->getReference())
			{
				xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"World destroyed :%s",w->getReference()->GetName());
			}

			getWorlds()->Remove(w->getReference());
			delete w;
			w = NULL;
			
			if (getWorlds()->Size())
			{
				destroyWorlds();
			}
		}
	}
	getWorlds()->Clear();
}
pWorld*PhysicManager::getWorld(CK_ID _o)
{
	
	pWorld *result  = NULL;
	CK3dEntity *obj = static_cast<CK3dEntity*>(GetContext()->GetObject(_o));
	if (obj)
	{
		if (getWorlds()->FindPtr(obj))
		{
			return *getWorlds()->FindPtr(obj);
		}
	}
	return 0;
}
void PhysicManager::deleteWorld(CK_ID _o)
{
	
	CK3dEntity *obj = static_cast<CK3dEntity*>(GetContext()->GetObject(_o));
	if (obj)
	{
		pWorld *w = getWorld(_o);
		if (w)
		{
			w->destroy();
			getWorlds()->Remove(obj);
			delete w;
		}
	}
}



pWorld*PhysicManager::getWorldByShapeReference(CK3dEntity *shapeReference)
{

	if (!shapeReference)
	{
		return NULL;
	}
	
	for (pWorldMapIt it  = getWorlds()->Begin(); it!=getWorlds()->End(); it++)
	{
		pWorld *w = *it;
		if(w)
		{
			pRigidBody *body = w->getBody(pFactory::Instance()->getMostTopParent(shapeReference));
			if (body)
			{
				if (body->isSubShape((CKBeObject*)shapeReference))
				{
					return w;
				}
			}
		}
	}
	return 0;
}
pWorld*PhysicManager::getWorldByBody(CK3dEntity*ent)
{

	if (!ent)
	{
		return NULL;
	}

	for (pWorldMapIt it  = getWorlds()->Begin(); it!=getWorlds()->End(); it++)
	{
		pWorld *w = *it;
		if(w)
		{
			//pRigidBody *body = w->getBody(pFactory::Instance()->getMostTopParent(ent));
			pRigidBody *body = w->getBody(ent);
			if (body)
			{
				return w;
			}
		}
	}
	return 0;
}

pCloth *PhysicManager::getCloth(CK_ID entityID)
{

	CK3dEntity *entity  = (CK3dEntity*)m_Context->GetObject(entityID);
	if (!entityID)
	{
		return NULL;
	}


	for (pWorldMapIt it  = getWorlds()->Begin(); it!=getWorlds()->End(); it++)
	{
		pWorld *w = *it;
		if(w)
		{
			pCloth *cloth = w->getCloth(entity);
			if (cloth)
			{
				return cloth;
			}
		}
	}
    
	return NULL;
}
int PhysicManager::getNbObjects(int flags /* =0 */)
{

	CKAttributeManager* attman = ctx()->GetAttributeManager();

	int testAtt  = -1;

	if (flags == 0) testAtt = GetPMan()->GetPAttribute();

	int newPhysicObjectAttributeType = getAttributeTypeByGuid(VTS_PHYSIC_ACTOR);


	//  [3/31/2009 master]
	
	const XObjectPointerArray& Array = attman->GetAttributeListPtr(testAtt);
	int startSize = 0;
	startSize +=Array.Size();
	
	startSize+=attman->GetAttributeListPtr(newPhysicObjectAttributeType).Size();


	/*
		pWorldMapIt it  = getWorlds()->Begin();
		while(it != getWorlds()->End())
		{
			pWorld *w = *it;
			if (w)
			{
				w->checkList();
			}
			it++;
		}
	*/
	return startSize;
}