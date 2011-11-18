#include <StdAfx.h>

#include "vtPhysXAll.h"

#include <stdlib.h>
#include <exception>



#include "IParameter.h"


PhysicManager *manager = NULL;
using namespace xUtils;
using namespace vtAgeia;

void
PhysicManager::_destruct(xBitSet flags /* = 0 */)
{

	//################################################################
	//
	// some sanity checks
	//
	assert(mIParameter);
	assert(m_Worlds);
	assert(manager);

	//################################################################
	//
	// Clean instances :
	//
	SAFE_DELETE(mIParameter);
}
//////////////////////////////////////////////////////////////////////////
PhysicManager::PhysicManager(CKContext* context):CKPhysicManager(context,GUID_MODULE_MANAGER,VTCX_API_ENTRY("PhysicManager")) //Name as used in profiler
{


	m_Context->RegisterNewManager(this);
	m_Worlds = new pWorldMap();
	manager = this;

	
	_Hook3DBBs();
	_HookGenericBBs();

	_construct();

	int ss = xLogger::GetInstance()->getItemDescriptions().size();
	int ss2= xLogger::GetInstance()->getLogItems().size();
	xLogger::xLog(ELOGERROR,E_BB,"No Reference Object specified");

	timer  = 0.0f;
	mPhysicsSDK = NULL;
	DongleHasBasicVersion=0;
	DongleHasAdvancedVersion=0;

	_LogErrors = _LogInfo = _LogTrace = _LogWarnings = _LogToConsole = 0;


	mIParameter = new IParameter(this);


}
//////////////////////////////////////////////////////////////////////////
void PhysicManager::cleanAll()
{

	//////////////////////////////////////////////////////////////////////////
	//destroy all worlds : 
	if (getWorlds()->Size())
	{
		destroyWorlds();
		m_DefaultWorld = NULL;

	}

	//////////////////////////////////////////////////////////////////////////
	//destroy default objects :



	//////////////////////////////////////////////////////////////////////////
	//world settings : 
	if (getDefaultWorldSettings())
	{
		SAFE_DELETE(mDefaultWorldSettings);
		xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"Deleted default world settings");
	}

	
	//////////////////////////////////////////////////////////////////////////
	//default configuration : 
	if (m_DefaultDocument)
	{
		SAFE_DELETE(m_DefaultDocument);
		xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"Deleted default configuration");
	}

	if (pFactory::Instance())
	{
		pFactory::Instance()->reloadConfig("PhysicDefaults.xml");
	}

	if (getPhysicsSDK())
	{
		xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"Physic SDK released ");
		NxReleasePhysicsSDK(getPhysicsSDK());
		mPhysicsSDK = NULL;
	}



	_getManagerFlags()  = 0 ;

}

void PhysicManager::doInit()
{
	//CreateWorlds(0);
}
CKERROR PhysicManager::OnCKPause()
{
	return CK_OK;
}
CKERROR PhysicManager::OnCKPlay()
{
	int a = _LogErrors;
	int b = 0;

	
	xLogger::GetInstance()->enableLoggingLevel(E_LI_AGEIA,ELOGERROR,_LogErrors);
	xLogger::GetInstance()->enableLoggingLevel(E_LI_MANAGER,ELOGERROR,_LogErrors);

	
	xLogger::GetInstance()->enableLoggingLevel(E_LI_MANAGER,ELOGTRACE,_LogTrace);
	xLogger::GetInstance()->enableLoggingLevel(E_LI_MANAGER,ELOGWARNING,_LogWarnings);
	xLogger::GetInstance()->enableLoggingLevel(E_LI_MANAGER,ELOGINFO,_LogInfo);


	try {
		populateAttributeFunctions();
		_RegisterAttributeCallbacks();
	} catch(std::exception ex) {
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Error during Attribute List Populations");
	} catch(...) {
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Error during Attribute List Populations");
		//systemMessage("System Error");
	}
	if (getNbObjects())
	{
		_migrateOldCustomStructures(GetContext()->GetCurrentScene());

		if (
			isFlagOff(_getManagerFlags(),E_MF_PSDK_LOADED) && 
			isFlagOff(_getManagerFlags(),E_MF_DEFAULT_WORLD_CREATED) &&
			isFlagOff(_getManagerFlags(),E_MF_FACTORY_CREATED)
			)
		{
			performInitialization();
			checkWorlds();
		}
	}
	return CK_OK;
}
CKERROR PhysicManager::PostClearAll()
{
	return CK_OK;
}
CKERROR PhysicManager::OnCKInit()
{
	bindVariables();
	return CK_OK;
}


CKERROR PhysicManager::PreSave()
{
	return CK_OK;
}
CKContext* PhysicManager::GetContext()
{
	return manager->m_Context;
}
PhysicManager* PhysicManager::GetInstance()
{
	if (manager)
	{
		return manager;
	}
	return NULL;
}
PhysicManager::~PhysicManager(){}

CKERROR PhysicManager::OnPostCopy(CKDependenciesContext& context)
{
	CKDependenciesContext dependencies_ctx(m_Context);
//	dependencies_ctx.SetOperationMode(CK_DEPENDENCIES_SAVE);
/*
	dependencies_ctx.StartDependencies(iDep);

	//We scan the group and fill the dependencies context
	for (int i=0;i<iGrp->GetObjectCount();i++)
	{
		CKBeObject* object = iGrp->GetObject(i);
		object->PrepareDependencies(dependencies_ctx);
	}
*/
	// Build a list of id to save
	return CK_OK;
	XObjectArray dependencies_list = context.FillDependencies();
	int s  = dependencies_list.Size();
	

	
	//dependencies_list.PushBack(iGrp->GetID());//add group at the end

	//copy list of objects in ckobjectarray
	CKObjectArray* listToSave = CreateCKObjectArray();
	XObjectArray::Iterator it1 = dependencies_list.Begin();
	XObjectArray::Iterator it2 = dependencies_list.End();
	while (it1!=it2)
	{
		CKObject* object = m_Context->GetObject(*it1);
		/*CK_ID cID = object->GetID();
		listToSave->InsertRear(*it1++);
		CKSTRING name = object->GetName();
		
		CK_ID id2 = context.RemapID(cID);
		CK_ID id3 = context.RemapID(cID);*/

		
		
	}

	return CK_OK;


}
CKERROR PhysicManager::SequenceDeleted(CK_ID *objids,int count)
{


	
	if (getNbObjects())
	{
		if(GetContext()->IsPlaying())
			checkWorlds();
	}
	return CK_OK;
}

CKERROR PhysicManager::SequenceAddedToScene(CKScene *scn,CK_ID *objids,int count)
{
	int isInLoad=GetContext()->IsInLoad();
	int nbOfObjects = 0 ;
	if (!GetContext()->IsPlaying())
	{
		
		CKAttributeManager* attman = m_Context->GetAttributeManager();
		const XObjectPointerArray& Array = attman->GetAttributeListPtr(GetPAttribute());
		nbOfObjects = Array.Size();
		if (nbOfObjects)
		{
			_migrateOldCustomStructures(scn);
		}


	}

	if (getNbObjects())
	{
		if(GetContext()->IsPlaying()){

			
			/*
			for (int i  = 0 ; i < count ; i++ )
			{
				CK_ID dstId  = objids[i];
				CKBeObject * obj = GetContext()->GetObject()
			}
			*/
			checkWorlds();
		}
	}
	return CK_OK;
}
CKERROR PhysicManager::SequenceToBeDeleted(CK_ID *objids,int count)
{
	return CK_OK;
}

CKERROR PhysicManager::SequenceRemovedFromScene(CKScene *scn,CK_ID *objids,int count)
{

	if (getNbObjects())
	{
		if(GetContext()->IsPlaying())
			checkWorlds();
	}
	return CK_OK;
}


CKERROR PhysicManager::PreClearAll()
{
	
	return CK_OK;
}

CKERROR PhysicManager::OnCKReset()
{


	cleanAll();
	_RegisterDynamicParameters();

	return CK_OK;
}
CKERROR PhysicManager::OnCKEnd()
{
	
	unBindVariables();

	return 0;
}