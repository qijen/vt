#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "pWorldSettings.h"
#include "NxUserNotify.h"
#include "NxUserContactReport.h"
#include "pWorldCallbacks.h"


struct MyUserNotify : public NxUserNotify    
{        
public:        
	

	/*virtual bool onJointBreak(NxReal breakingImpulse, NxJoint & brokenJoint)    
	{
		return false;
	}*/
	
	virtual bool onJointBreak(NxReal breakingImpulse, NxJoint & brokenJoint)    
	{        

		
		pJoint *joint  = static_cast<pJoint*>(brokenJoint.userData);
		if (joint)
		{
			pBrokenJointEntry *entry = new pBrokenJointEntry();
			//entry->joint = joint;
			entry->impulse = breakingImpulse;
			
			entry->jType =brokenJoint.getType();

			if(joint->GetVTEntA())
				entry->mAEnt = joint->GetVTEntA()->GetID();

			if(joint->GetVTEntB())
				entry->mBEnt = joint->GetVTEntB()->GetID();


			GetPMan()->getJointFeedbackList().PushBack(entry);
			
			//xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Joint break!");
			//////////////////////////////////////////////////////////////////////////
			//
			//	retrieve body pointers and call callback scripts
			NxActor *a = NULL; pRigidBody *ab=NULL;
			NxActor *b = NULL; pRigidBody *bb=NULL;

			brokenJoint.getActors(&a,&b);
			bool isRemoved = false;

			if(a)
			{
				ab= static_cast<pRigidBody*>(a->userData);
			}
			if(b)
				bb= static_cast<pRigidBody*>(b->userData);
			
			if(ab)
			{
				//ab->onJointBreak(entry);
				//ab->hasBrokenJoint = true;
				if(!isRemoved)
				{
					isRemoved = true;
				}
			}

			if(bb)
			{
				//bb->onJointBreak(entry);
				//bb->hasBrokenJoint= true;
			}

			//joint->getJoint()->appData= NULL;
			//joint->getJoint()->userData= NULL;

		}
		return false;
	}    

	virtual void onSleep(NxActor **actors, NxU32 count)
	{
		/*
		NX_ASSERT(count > 0);
				while (count--)
				{
					NxActor *thisActor = *actors;
					//Tag the actor as sleeping
					size_t currentFlag = (size_t)thisActor->userData;
					currentFlag |= gSleepFlag;
					thisActor->userData = (void*)currentFlag;
		
					actors++;
				}*/
		if (count >0)
		{
			while (count--)
			{
				NxActor *thisActor = *actors;
				if (thisActor)
				{
					pRigidBody *body = static_cast<pRigidBody*>(thisActor->userData);
					if (body)
					{

					int flags = body->getFlags();
					flags |=BF_Sleep;
					body->setFlags(flags);

					}
				}
			}
		}
	}

	virtual void onWake(NxActor **actors, NxU32 count)
	{
		
		if (count >0)
		{
			while (count--)
			{
				NxActor *thisActor = *actors;
				if (thisActor)
				{
					pRigidBody *body = static_cast<pRigidBody*>(thisActor->userData);
					if (body)
					{
						body->getCollisions().Clear();
						body->getTriggers().Clear();

						int flags = body->getFlags();
						flags &=~BF_Sleep;
						body->setFlags(flags);
					}
				}
				actors++;
			}
		}
	}

}myNotify;


pWorld*pFactory::createWorld(CK3dEntity* referenceObject, pWorldSettings *worldSettings,pSleepingSettings *sleepSettings)
{


	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;

	
	//////////////////////////////////////////////////////////////////////////
	//sanity checks : 
	if (!referenceObject || !GetPMan() )
	{
		return NULL;
	}


	if (!getPhysicSDK())
	{
		//xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"No physic sdk loaded");
		return NULL;
	}

	int worldAtt = GetPMan()->att_world_object;
	int surfaceAttribute = GetPMan()->att_surface_props;

	//exists ? Delete it !
	pWorld *w = GetPMan()->getWorld(referenceObject->GetID());
	if (w)
	{
		GetPMan()->deleteWorld(referenceObject->GetID());
	}
		
	//our new world : 
	pWorld *result  = new pWorld(referenceObject);
	GetPMan()->getWorlds()->InsertUnique(referenceObject,result);

	result->initUserReports();
	



	//////////////////////////////////////////////////////////////////////////

	//there is no world settings attribute  : 
	if (!referenceObject->HasAttribute(worldAtt) )
	{
		referenceObject->SetAttribute(worldAtt);
		using namespace vtTools;
		VxVector grav = worldSettings->getGravity();
		float sWith  = worldSettings->getSkinWith();
		AttributeTools::SetAttributeValue<VxVector>(referenceObject,worldAtt,0,&grav);
		AttributeTools::SetAttributeValue<float>(referenceObject,worldAtt,1,&sWith);
	}

	worldSettings = pFactory::Instance()->createWorldSettings(XString("Default"),GetPMan()->getDefaultConfig());
	if (!worldSettings)
	{
		worldSettings = new pWorldSettings();
	}

	GetPMan()->setPhysicFlags(worldSettings->getPhysicFlags());

	//////////////////////////////////////////////////////////////////////////
	//pSDK Scene creation  : 

	// Create a scene
	NxSceneDesc sceneDesc;
	NxHWVersion hwCheck = getPhysicSDK()->getHWVersion();

	if(GetPMan()->physicFlags & PMF_DONT_USE_HARDWARE )
	{
		hwCheck = NX_HW_VERSION_NONE;
	}

	if(hwCheck != NX_HW_VERSION_NONE)
	{
		sceneDesc.simType = NX_SIMULATION_HW;
	}
	
	//SCE_SEARCHFORCOMPUTERS.
	sceneDesc.gravity				= pMath::getFrom(worldSettings->getGravity());
	sceneDesc.upAxis = 1;
	//sceneDesc.flags |=NX_SF_ENABLE_ACTIVETRANSFORMS/*|NX_SF_ENABLE_MULTITHREAD|NX_SF_SIMULATE_SEPARATE_THREAD*/;
	sceneDesc.flags = worldSettings->getSceneFlags();
	sceneDesc.userNotify =&myNotify;
	sceneDesc.userContactReport = result->contactReport;
	sceneDesc.userContactModify = result->contactModify;
	NxScene *scene = NULL;
	if (getPhysicSDK())
	{
		scene  = getPhysicSDK()->createScene(sceneDesc);
		if(scene == NULL) 
		{
            xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Couldn't create scene!");
			return NULL;
		}
	}
	NxCompartment *comp = NULL;
	if(hwCheck != NX_HW_VERSION_NONE)
	{
		NxCompartmentDesc cdesc;

		cdesc.type = NX_SCT_RIGIDBODY;
		#if defined(WIN32) && !defined(_XBOX)
				cdesc.deviceCode = NX_DC_PPU_AUTO_ASSIGN;
		#else
				cdesc.deviceCode = NX_DC_CPU;
		#endif
		comp = scene->createCompartment(cdesc);
		if (!comp)
			xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Couldn't create scene's compartment");
	}


	result->setScene(scene);
	scene->setUserContactReport(result->contactReport);
	scene->setUserTriggerReport(result->triggerReport);

	NxMaterialDesc *materialDescr = NULL;
	NxMaterial *material = NULL;
	if (referenceObject->HasAttribute(surfaceAttribute))
	{
		materialDescr  = createMaterialFromEntity(referenceObject);
		material =  result->getScene()->createMaterial(*materialDescr);
		material->userData = (void*)GetValueFromParameterStruct<int>(referenceObject->GetAttributeParameter(surfaceAttribute) ,E_MS_XML_TYPE);
	}else{
		
		if (getDefaultDocument())
		{
			
			materialDescr = createMaterialFromXML("Default",getDefaultDocument());
		}
		
		if (materialDescr)
		{
			material = result->getScene()->createMaterial(*materialDescr);
		}

		if (!material)
		{
			materialDescr = new NxMaterialDesc();
			materialDescr->setToDefault();
			material = result->getScene()->getMaterialFromIndex(0); 
			material->loadFromDesc(*materialDescr);
		}

		if(material)
		{
			pMaterial *bmaterial = new pMaterial();
			bmaterial->setToDefault();

			copyTo(*bmaterial,material);

			material->userData = (void*)bmaterial;

		}
	}
	
	
	NxMaterial *zeroMaterial  = result->getScene()->getMaterialFromIndex(0);
	zeroMaterial->setDirOfAnisotropy(material->getDirOfAnisotropy());
	zeroMaterial->setStaticFriction(material->getStaticFriction());
	zeroMaterial->setDynamicFriction(material->getDynamicFriction());
	zeroMaterial->setStaticFrictionV(material->getStaticFrictionV());
	zeroMaterial->setDynamicFrictionV(material->getDynamicFrictionV());
	zeroMaterial->setFrictionCombineMode(material->getFrictionCombineMode());
	zeroMaterial->setRestitutionCombineMode(material->getRestitutionCombineMode());
	zeroMaterial->setFlags(material->getFlags());


	
	if (!material)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Couldn't create default material!");
	}
	result->setDefaultMaterial(material);
	

	scene->userData = result;
	result->_checkForDominanceConstraints();
	result->_construct();
	if (comp)
	{
		result->setCompartment(comp);
	}
	return result;
}
//************************************
// Method:    CreateDefaultWorld
// FullName:  vtODE::pFactory::CreateDefaultWorld
// Access:    public 
// Returns:   pWorld*
// Qualifier:
// Parameter: XString name
//************************************
pWorld*pFactory::createDefaultWorld(XString name)
{
	

	CK3dEntity *defaultWorldFrame = createFrame("pDefaultWorld");
	pWorldSettings *wSettings = getManager()->getDefaultWorldSettings();

	pWorld* world  = createWorld(defaultWorldFrame,wSettings,NULL);
	getManager()->setDefaultWorld(world);
	return world;
}

