#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "vtAttributeHelper.h"


bool isSceneObject2(CK3dEntity *object)
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
void PhysicManager::_checkObjectsByAttribute(CKScene *newScene)
{

	CKAttributeManager* attman = m_Context->GetAttributeManager();
	int sizeJFuncMap = ATT_FUNC_TABLE_SIZE;//(sizeof(*getRegistrationTable()) / sizeof((getRegistrationTable())[0]));
	for (int fIndex = 0  ; fIndex  < sizeJFuncMap ; fIndex ++)
	{

		std::vector<int>attributeIdList;
		pFactory::Instance()->findAttributeIdentifiersByGuid(getRegistrationTable()[fIndex].guid,attributeIdList);
		int attCount = attributeIdList.size();

		for (int i = 0 ; i < attCount ; i++ )
		{
			int currentAttType = attributeIdList.at(i);
			const XObjectPointerArray& Array = attman->GetAttributeListPtr( attributeIdList.at(i) );
			for (CKObject** it = Array.Begin(); it != Array.End(); ++it)
			{	
				CK3dEntity *target = static_cast<CK3dEntity*>(*it);
				if (target)
				{
					XString error;
					error.Format("Registering :%s with %s",target->GetName(),attman->GetAttributeNameByType(currentAttType));
					//if(!strcmp( target->GetName(),"smutan3-3" ) )					{					//		xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"problem case" );					}

//					CKScene *levelScene = GetContext()->GetCurrentLevel()->GetCurrentScene();
					// we check as no scene is current in use 
					if (		(	
								GetContext()->GetCurrentLevel()->GetLevelScene() == newScene &&
								!isSceneObject2(target) 
							)
							||
							(	newScene && newScene->IsObjectHere(target) && newScene !=GetContext()->GetCurrentLevel()->GetLevelScene() )
							||
							(	
								newScene &&
								GetContext()->GetCurrentLevel()->GetCurrentScene() && 
								GetContext()->GetCurrentLevel()->GetCurrentScene() == newScene &&
								newScene !=GetContext()->GetCurrentLevel()->GetLevelScene() &&
								newScene->IsObjectHere(target)
							)
							||
							(
								(physicFlags & PMF_DONT_DELETE_SCENES) 
							)
						)
					{
							xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,error.CStr() );
							(*getRegistrationTable()[fIndex].rFunc)(target,currentAttType,true,false);
							GetPMan()->getCheckList().PushBack(target->GetID());
					}
				}
			}
		}
	}
}

void PhysicManager::_RegisterAttributeCallbacks()
{
	if (!getAttributeFunctions().Size())
	{
		return;
	}

	CKAttributeManager* attman = m_Context->GetAttributeManager();
	AttributeFunctionArrayIteratorType it  = getAttributeFunctions().Begin();

	while(it != getAttributeFunctions().End())
	{
		ObjectRegisterFunction myFn = (ObjectRegisterFunction)*it;
		if (myFn)
		{
			attman->SetAttributeCallbackFunction(it.GetKey(),PObjectAttributeCallbackFunc,myFn);
		}
		it++;
	}
}

void PhysicManager::cleanAttributePostObjects()
{
	using namespace vtTools::ParameterTools;

	if (!getAttributePostObjects().Size())
		return;
	
	CKAttributeManager* attman = m_Context->GetAttributeManager();
	PostRegistrationArrayIteratorType it  = getAttributePostObjects().Begin();

	if (getAttributePostObjects().Size())
	{
		if (!GetPMan()->isValid()){
			xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"Invalid world, performing initiation...");
			GetPMan()->performInitialization();
		}
	}
	while(it != getAttributePostObjects().End())
	{
		pAttributePostObject& post = *it;
		CK3dEntity *refObject = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(post.objectId));
		if (refObject)
		{

			ObjectRegisterFunction regFn = (ObjectRegisterFunction)post.func;
			if (regFn)
			{
				(*regFn)(refObject,post.attributeID,true,false);
			}
		}
		it++;
	}
	int s  = getAttributePostObjects().Size();

	getAttributePostObjects().Clear();
}

void PhysicManager::populateAttributeFunctions()
{

	getAttributeFunctions().Clear();

	int sizeJFuncMap = ATT_FUNC_TABLE_SIZE;// (sizeof(*getRegistrationTable()) / sizeof(ObjectRegistration));
	for (int fIndex = 0  ; fIndex  < sizeJFuncMap ; fIndex ++)
	{
		#ifdef _DEBUG 

		//XString _errorStr;
		//getRegistrationTable()[fIndex].rFunc.
		//xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,errString.Str());

		#endif

		std::vector<int>attributeIdList;
		pFactory::Instance()->findAttributeIdentifiersByGuid(getRegistrationTable()[fIndex].guid,attributeIdList);
		int attCount = attributeIdList.size();
		for (int i = 0 ; i < attCount ; i++ )
		{
			int currentAttType = attributeIdList.at(i);
			getAttributeFunctions().Insert(currentAttType,getRegistrationTable()[fIndex].rFunc);
		}
	}
}
ObjectRegistration*PhysicManager::getRegistrationTable()
{
	return attributeFunctionMap;
}
