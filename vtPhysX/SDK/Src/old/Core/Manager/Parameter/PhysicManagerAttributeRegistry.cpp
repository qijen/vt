#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "vtAttributeHelper.h"

void PhysicManager::_checkObjectsByAttribute()
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
					xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,error.CStr() );
					(*getRegistrationTable()[fIndex].rFunc)(target,currentAttType,true,false);
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
