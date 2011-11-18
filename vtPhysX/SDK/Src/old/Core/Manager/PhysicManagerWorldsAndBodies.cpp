#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "vtAttributeHelper.h"

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
void PhysicManager::checkWorlds()
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
	const XObjectPointerArray& Array = attman->GetAttributeListPtr(att_physic_object);
	for (CKObject** it = Array.Begin(); it != Array.End(); ++it)
	{	

		CK3dEntity *target = static_cast<CK3dEntity*>(*it);
		if (target)
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
				}
			}
		}
	}

	
	checkClothes();
	_checkObjectsByAttribute();



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