#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "pWorldSettings.h"
#include "NxUserNotify.h"
#include "NxUserContactReport.h"
#include "pWorldCallbacks.h"


struct MyUserNotify : public NxUserNotify    
{        
public:        
	
	virtual bool onJointBreak(NxReal breakingImpulse, NxJoint & brokenJoint)    
	{        
		pJoint *joint  = static_cast<pJoint*>(brokenJoint.userData);
		if (joint)
		{
			pBrokenJointEntry *entry = new pBrokenJointEntry();
			entry->joint = joint;
			entry->impulse = breakingImpulse;
			pWorld *world = joint->getWorld();
			if (world)
			{
				world->getJointFeedbackList().PushBack(entry);

				xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"Joint break!");
			}
		}
		return false;
		/*if((&brokenJoint) == gMyBreakableJoint)           
		{                cout << "BANG, The joint broke" << endl;        
		return true; //delete the joint        
		}            return false; //don't delete the joint    */
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

					}
				}
				//Tag the actor as non-sleeping
				/*size_t currentFlag = (size_t)thisActor->userData;
				currentFlag &= ~gSleepFlag;
				thisActor->userData = (void*)currentFlag;*/

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


	if (!worldSettings)
	{
		worldSettings = pFactory::Instance()->createWorldSettings(XString("Default"),GetPMan()->getDefaultConfig());
	}
	if (!worldSettings)
	{
		worldSettings = new pWorldSettings();
	}

	//////////////////////////////////////////////////////////////////////////
	//pSDK Scene creation  : 

	// Create a scene
	NxSceneDesc sceneDesc;
	sceneDesc.gravity				= pMath::getFrom(worldSettings->getGravity());
	sceneDesc.upAxis = 1;
	sceneDesc.flags |=NX_SF_ENABLE_ACTIVETRANSFORMS;
	sceneDesc.userNotify =&myNotify;
	sceneDesc.userContactReport = result->contactReport;
    
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
	}
	
	int z = (int)material->userData;
	NxMaterial *zeroMaterial  = result->getScene()->getMaterialFromIndex(0);
	zeroMaterial->setDirOfAnisotropy(material->getDirOfAnisotropy());
	zeroMaterial->setStaticFriction(material->getStaticFriction());
	zeroMaterial->setDynamicFriction(material->getDynamicFriction());
	zeroMaterial->setStaticFrictionV(material->getStaticFrictionV());
	zeroMaterial->setDynamicFrictionV(material->getDynamicFrictionV());
	zeroMaterial->setFrictionCombineMode(material->getFrictionCombineMode());
	zeroMaterial->setRestitutionCombineMode(material->getRestitutionCombineMode());
	zeroMaterial->setFlags(material->getFlags());
	zeroMaterial->userData = material->userData;




	if (!material)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Couldn't create default material!");
	}
	result->setDefaultMaterial(material);

	scene->userData = result;

	
	//NxConstraintDominance testDom(1.0, 1.0f);
	//result->getScene()->setDominanceGroupPair(1, 2,testDom );	//board - debris



	//////////////////////////////////////////////////////////////////////////
	//there is no world settings attribute  : 
	/*
	if (!referenceObject->HasAttribute(GetPMan()->att_sleep_settings) )
	{
		referenceObject->SetAttribute(GetPMan()->att_sleep_settings);
		using namespace vtTools;
		AttributeTools::SetAttributeValue<int>(referenceObject,GetPMan()->att_sleep_settings,0,&sSettings->m_SleepSteps);
		AttributeTools::SetAttributeValue<float>(referenceObject,GetPMan()->att_sleep_settings,1,&sSettings->m_AngularThresold);
		AttributeTools::SetAttributeValue<float>(referenceObject,GetPMan()->att_sleep_settings,2,&sSettings->m_LinearThresold);
		AttributeTools::SetAttributeValue<int>(referenceObject,GetPMan()->att_sleep_settings,3,&sSettings->m_AutoSleepFlag);
	}	
	*/
	

	/*
	result->SleepingSettings(sSettings);
	//////////////////////////////////////////////////////////////////////////

	result->Reference(referenceObject);
	
	result->Init();
*/


	result->_checkForDominanceConstraints();
	result->_construct();






	return result;
	

	//return NULL;

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




	
/*
	
	pSleepingSettings *sSettings = CreateSleepingSettings("Default",GetPMan()->DefaultDocument());
	pWorldSettings *wSettings =  createWorldSettings("Default",GetPMan()->DefaultDocument());

	GetPMan()->DefaultSleepingSettings(*sSettings);
	GetPMan()->DefaultWorldSettings(*wSettings);

	pWorld* world  = CreateWorld(defaultWorldFrame,&GetPMan()->DefaultWorldSettings(),&GetPMan()->DefaultSleepingSettings());
	GetPMan()->DefaultWorld(world);
	
*/
//	return world;

	return NULL;

}

