#include <StdAfx.h>

#include "vtPhysXAll.h"
#include "pVehicleAll.h"
#include "pWorldSettings.h"


#include <stdlib.h>
#include <exception>


using namespace vtTools::AttributeTools;

void pWorld::_construct()
{
	//mAllocator = new UserAllocator;
	//mCManager = NxCreateControllerManager(mAllocator);
	compartment = NULL;

}

void pWorld::destroyCloth(CK_ID id)
{

	pCloth*cloth  = getCloth(id);
	if (cloth)
	{
		delete cloth;
	}
}

pCloth *pWorld::getCloth(CK_ID id)
{

	int nbClothes = getScene()->getNbCloths();
	NxCloth** clothes = getScene()->getCloths();
	while(nbClothes--)
	{
		NxCloth* cloth = *clothes++;
		if(cloth->userData != NULL)
		{
			pCloth *vCloth  = static_cast<pCloth*>(cloth->userData);
			if (vCloth)
			{
				if (id == vCloth->getEntityID() )
				{
					return vCloth;
				}
			}
		}
	}

	return NULL;
}

pCloth *pWorld::getCloth(CK3dEntity*reference)
{
	if (!reference)
	{
		return NULL;
	}

	int nbClothes = getScene()->getNbCloths();
	NxCloth** clothes = getScene()->getCloths();
	while(nbClothes--)
	{
		NxCloth* cloth = *clothes++;
		if(cloth->userData != NULL)
		{
			pCloth *vCloth  = static_cast<pCloth*>(cloth->userData);
			if (vCloth)
			{
				if (reference->GetID() == vCloth->getEntityID() )
				{
					return vCloth;
				}
			}
		}
	}
	return NULL;
}

void pWorld::updateClothes()
{
	int nbClothes = getScene()->getNbCloths();
	NxCloth** clothes = getScene()->getCloths();
	while(nbClothes--)
	{
		NxCloth* cloth = *clothes++;
		if(cloth->userData != NULL)
		{
			pCloth *vCloth  = static_cast<pCloth*>(cloth->userData);
			if (vCloth)
			{
				vCloth->updateVirtoolsMesh();
			}
		}
	}

}



int pWorld::getNbBodies()
{
	int result = 0;
	if (!getScene())
	{
		return result;
	}
	result+= getScene()->getNbActors();
	return result;
}
int pWorld::getNbJoints()
{
	int result = 0;
	if (!getScene())
	{
		return result;
	}
	result+= getScene()->getNbJoints();
	return result;
}



NxShape *pWorld::getShapeByEntityID(CK_ID id)
{

	if (!getScene() )
	{
		return NULL;
	}

	CK3dEntity *entA  = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(id));
	if (!entA)
	{
		return NULL;
	}


	int nbActors = getScene()->getNbActors();
	if (!nbActors)
	{
		return NULL;
	}
	NxActor** actors = getScene()->getActors();
	while(nbActors--)
	{
		NxActor* actor = *actors++;
		if(actor->userData != NULL)
		{
			pRigidBody* body =static_cast<pRigidBody*>(actor->userData);
			if (body)
			{
				NxU32 nbShapes = body->getActor()->getNbShapes();
				if ( nbShapes )
				{
					NxShape ** slist = (NxShape **)body->getActor()->getShapes();
					for (NxU32 j=0; j<nbShapes; j++)
					{
						NxShape *s = slist[j];
						if (s)
						{
							pSubMeshInfo *info =  static_cast<pSubMeshInfo*>(s->userData);
							if (info)
							{
								CK3dEntity *ent  = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(info->entID));
								if (ent == entA )
								{
									return s;
								}
							}
						}
					}
				}
			}
		}
	}
	return NULL;
}

pRigidBody* pWorld::getBodyFromSubEntity(CK3dEntity *sub)
{

	if (!getScene() )
	{
		return NULL;
	}

	int nbActors = getScene()->getNbActors();
	if (!nbActors)
	{
		return NULL;
	}
	NxActor** actors = getScene()->getActors();
	while(nbActors--)
	{
		NxActor* actor = *actors++;
		if(actor->userData != NULL)
		{
			pRigidBody* body =static_cast<pRigidBody*>(actor->userData);
			if (body)
			{
				NxU32 nbShapes = body->getActor()->getNbShapes();
				if ( nbShapes )
				{
					NxShape ** slist = (NxShape **)body->getActor()->getShapes();
					for (NxU32 j=0; j<nbShapes; j++)
					{
						NxShape *s = slist[j];
						if (s)
						{
							pSubMeshInfo *info =  static_cast<pSubMeshInfo*>(s->userData);
							if (info)
							{
								CK3dEntity *ent  = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(info->entID));
								if (ent)
								{
									if (sub == ent)
									{
										return body;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return NULL;
}
void pWorld::checkList()
{
	if (!getScene() )
	{
		return;
	}
	
	int nbActors = getScene()->getNbActors();
	if (!nbActors)
	{
		return;
	}
	
	NxActor** actors = getScene()->getActors();
	while(nbActors--)
	{
		NxActor* actor = *actors++;
		if(actor->userData != NULL)
		{
			pRigidBody* body = (pRigidBody*)actor->userData;
			if (body)
			{
				CK3dEntity * ent  = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(body->getEntID()));
				if (!ent)
				{
					deleteBody(body);
				}
			}
		}
	}


	int nbClothes = getScene()->getNbCloths();
	NxCloth** clothes = getScene()->getCloths();
	while(nbClothes--)
	{
		NxCloth* cloth = *clothes++;
		if(cloth->userData != NULL)
		{
			pCloth *vCloth  = static_cast<pCloth*>(cloth->userData);
			if (vCloth)
			{

				CK3dEntity * ent  = static_cast<CK3dEntity*>(GetPMan()->GetContext()->GetObject(vCloth->getEntityID()));
				if (!ent)
				{
					destroyCloth(vCloth->getEntityID());
				}
			}
		}
	}
}
void pWorld::deleteBody(pRigidBody*body)
{

	if(!body)
		return;

	CK_ID id = body->getEntID();
	CK3dEntity *ent = (CK3dEntity*)GetPMan()->GetContext()->GetObject(id);

	if(!ent)
		return;
	
	GetPMan()->getRemovalList().PushBack(id);
	return;

	if (body)
	{
		body->destroy();
		if (body->getWorld() && body->getWorld()->getScene() )
		{

			for (int i = 0 ; i < body->getWorld()->getJointFeedbackList().Size(); i++ )
			{
				pBrokenJointEntry *entry = *body->getWorld()->getJointFeedbackList().At(i);
				if (entry && entry->joint)
				{
				}
			}
		}
		SAFE_DELETE(body);
	}
	checkList();
}

void pWorld::deleteJoint(pJoint*joint)
{

	if (!joint)
	{
		return;
	}

	pWorld *world = joint->getWorld();
	if (world && world->getScene() )
	{

			
		for (int i = 0 ; i < GetPMan()->getJointFeedbackList().Size(); i++ )
		{
			pBrokenJointEntry *entry = *world->getJointFeedbackList().At(i);
			if (entry && entry->joint == joint)
			{
				GetPMan()->getJointFeedbackList().EraseAt(i);
				break;
			}
		}

		joint->getJoint()->userData = NULL;
		world->getScene()->releaseJoint(*joint->getJoint());
		delete joint;

	}
}

pJoint* pWorld::getJoint(CK3dEntity *_a, CK3dEntity *_b, JType type)
{
	
		if (!getScene())
		{
			return NULL;
		}

		pRigidBody *a  = GetPMan()->getBody(_a);
		pRigidBody *b  = GetPMan()->getBody(_b);


		//bodies have already a joint together ? 
		if ( !a && !b)
		{
			return NULL;
		}

		if ( a == b)
		{
			return NULL;
		}

		if (a && !a->isValid() )
		{
			return NULL;
		}

		if (a && !GetPMan()->getWorldByBody(_a))
		{
			return NULL;
		}

		if (b && !GetPMan()->getWorldByBody(_b) )
		{
			return NULL;
		}

		if (b && !b->isValid())
		{
			return NULL;
		}

		if ( a && b )
		{
			pWorld*worldA  = GetPMan()->getWorldByBody(_a);
			pWorld*worldB  = GetPMan()->getWorldByBody(_b);
			if (!worldA || !worldB || (worldA!=worldB) || !worldA->isValid() || !worldB->isValid() )
			{
				return NULL;
			}
		}

		NxU32	jointCount = getScene()->getNbJoints();
		if (jointCount)
		{
			NxArray< NxJoint * > joints;

			getScene()->resetJointIterator();
			for	(NxU32 i = 0;	i	<	jointCount;	++i)
			{
				NxJoint	*j = getScene()->getNextJoint();

				if (type == JT_Any)
				{

					pJoint *mJoint = static_cast<pJoint*>( j->userData );

					if ( mJoint )
					{
						if (mJoint->GetVTEntA() == _a && mJoint->GetVTEntB() == _b )
						{
							return mJoint;
						}

						CK3dEntity *inA = _b;
						CK3dEntity *inB = _a;
						if (mJoint->GetVTEntA() == inA && mJoint->GetVTEntB() == inB )
						{
							return mJoint;
						}
					}
				}
				

				if ( j->getType()  == type)
				{
					
					pJoint *mJoint = static_cast<pJoint*>( j->userData );

					if ( mJoint )
					{
						if (mJoint->GetVTEntA() == _a && mJoint->GetVTEntB() == _b )
						{
							return mJoint;
						}

						CK3dEntity *inA = _b;
						CK3dEntity *inB = _a;
						if (mJoint->GetVTEntA() == inA && mJoint->GetVTEntB() == inB )
						{
							return mJoint;
						}
					}
				}
			}
		}

		return NULL;
}

bool pWorld::isValid()
{
	return getScene() ? true : false;
}









pRigidBody*pWorld::getBody(CK3dEntity*ent)
{
	pRigidBody *result = NULL;
	
	if (!ent || !getScene() )
	{
		return NULL;
	}

	int nbActors = getScene()->getNbActors();
	NxActor** actors = getScene()->getActors();
	while(nbActors--)
	{
		NxActor* actor = *actors++;
		if(actor->userData != NULL)
		{
			pRigidBody* body = (pRigidBody*)actor->userData;
			if (body && body->getEntID() == ent->GetID() )
			{
				return body;
			}
			
			if (body && body->isSubShape(ent)  )
			{
				return body;
			}
		}
	}

	return NULL;
}

pWorld::pWorld(CK3dEntity* _o)
{

	m_bCompletedLastFrame =true;
	m_fTimeSinceLastCallToSimulate = 0.0f;

	m_SleepingSettings = NULL;
	m_worldSettings =NULL;
	m_vtReference = _o;
	mScene = NULL;
	contactReport = NULL;
	triggerReport = NULL;
	contactModify = NULL;
    
	callMask = XCM_PreProcess | XCM_PostProcess;
	overrideMask = 0 ;
	compartment = NULL;
}
pWorld::~pWorld()
{
	//m_worldSettings = NULL;
	//m_SleepingSettings =NULL;
	mScene = NULL;
	//m_vtReference =NULL;
}
void pWorld::destroy()
{
	if (getScene())
	{
		getScene()->setClothUserNotify(NULL);
		getScene()->setUserTriggerReport(NULL);
		getScene()->setUserContactModify(NULL);
		getScene()->setUserActorPairFiltering(NULL);
		getScene()->setUserContactReport(NULL);
		getScene()->setUserNotify(NULL);

		SAFE_DELETE(contactReport);
		SAFE_DELETE(triggerReport);
		SAFE_DELETE(raycastReport);
		SAFE_DELETE(contactModify);

		if(mDefaultMaterial)
			getScene()->releaseMaterial(*mDefaultMaterial);


		if(compartment)
		{
			//getScene()->re
		}
		//SAFE_DELETE()


		int nbActors = getScene()->getNbActors();
		if (!nbActors)return;


		NxActor** actors = getScene()->getActors();
		while(nbActors--)
		{
			NxActor* actor = *actors++;
			if(actor->userData != NULL)
			{
				pRigidBody* body = (pRigidBody*)actor->userData;
				if (body)
				{
					//body->getActor()->getCompartment()->
					body->destroy();
					if (body->GetVT3DObject())
					{
						xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"Body destroyed :");
					}
					SAFE_DELETE(body);
				}
			}
		}
	}

	if (GetPMan()->getPhysicsSDK())
	{
		GetPMan()->getPhysicsSDK()->releaseScene(*mScene);
		mScene = NULL;
	}

	if (m_SleepingSettings)
	{
		delete m_SleepingSettings;
		m_SleepingSettings = NULL;
	}

	if (m_worldSettings)
	{
		delete m_worldSettings;
		m_worldSettings = NULL;
	}
	
}
int pWorld::UpdateProperties(DWORD mode,DWORD flags){	return 0;}
VxVector pWorld::getGravity()
{ 
	NxVec3 grav;
	getScene()->getGravity(grav);
	return getFrom(grav);

}

void pWorld::setGravity(VxVector gravity)
{ 
	getScene()->setGravity(getFrom(gravity));
}
