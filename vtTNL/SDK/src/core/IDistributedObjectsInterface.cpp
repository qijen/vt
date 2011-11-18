#include "IDistributedObjectsInterface.h"
#include "IDistributedClasses.h"
#include "xDistributedBaseClass.h"

#include "xDistributedObject.h"
#include "xNetInterface.h"
#include "vtConnection.h"
#include <assert.h>

#include "xLogger.h"

/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
xDistributedObject* IDistributedObjects::getByUserID(int userID,int classType)
{

	xDistributedObjectsArrayType *distObjects = getNetInterface()->getDistributedObjects();
	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();
	while (begin!=end)
	{
		xDistributedObject* dobj = *begin;
		if (dobj  && dobj->getUserID() == userID && dobj->getDistributedClass() && dobj->getDistributedClass()->getEnitityType() == classType )
		{
            return dobj;			
		}
		begin++;
	}
	return NULL;
}

/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
void
IDistributedObjects::cCreateDistObject(int objectID,int ownerID,const char* objectName,const char*className)
{

}
/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
void IDistributedObjects::create(const char* objectName,const char*className)
{

	//////////////////////////////////////////////////////////////////////////
	//sanity checks  :  
	if (!strlen(objectName) || !strlen(className) ) return;
	if (!getNetInterface()) return;
	if (getNetInterface()->IsServer()) return;
	if (!getNetInterface()->getConnection())return;
	if (!getNetInterface()->getDistributedClassInterface())return;
	
	IDistributedClasses *cInterface = getNetInterface()->getDistributedClassInterface();
	xDistributedClass *_class  = cInterface->get(className);
	if (!_class)return;

	//////////////////////////////////////////////////////////////////////////
	//check we have it already :
	if (get(objectName))return;

	//////////////////////////////////////////////////////////////////////////
	//call via connection rpc a dist object creation : 
	int userID = getNetInterface()->getConnection()->getUserID();
	getNetInterface()->getConnection()->c2sDOCreate(userID,objectName,className,_class->getEnitityType());
    
}

/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
void IDistributedObjects::Destroy()
{

	if (!getNetInterface())	{		return;	}
	if (!getNetInterface()->getDistributedObjects())return;

	xDistributedObjectsArrayType *distObjects = getNetInterface()->getDistributedObjects();
	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();
/*	
	while (begin!=end)
	{
		int size  = distObjects->Size();
		xDistributedObject* dobj = *begin;
		if (dobj)
		{
			dobj->Destroy();
			distObjects->Remove(dobj);
			delete dobj;
			dobj = NULL;
			begin = distObjects->begin();
			
			//continue;
		}
		begin++;
	}
*/
	distObjects->clear();

}

/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
xDistributedObject* IDistributedObjects::get(const char*name)
{

#ifdef _DEBUG
	assert(strlen(name));
#endif
	if (!strlen(name))return NULL;

	xDistributedObjectsArrayType *distObjects = getNetInterface()->getDistributedObjects();
	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();
	while (begin!=end)
	{
		xDistributedObject* dobj = *begin;
		if (dobj)
		{
			if(!strcmp(const_cast<char*>(dobj->GetName().getString()),name))
			{
				return dobj;
			}
		}
		begin++;
	}
	return NULL;
}
/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
xDistributedObject* IDistributedObjects::get(const char*name,int classType)
{

#ifdef _DEBUG
	assert(strlen(name));
#endif
	if (!strlen(name))return NULL;

	xDistributedObjectsArrayType *distObjects = getNetInterface()->getDistributedObjects();
	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();
	while (begin!=end)
	{
		xDistributedObject* dobj = *begin;
		if (dobj)
		{
			xDistributedClass *_class  = dobj->getDistributedClass();
			if (!_class)
			{
				xLogger::xLog(ELOGERROR,E_LI_DISTRIBUTED_BASE_OBJECT,"Found distributed object without class template: %s",name);
				return NULL;
			}
			int classType  = _class->getEnitityType();
			if(!strcmp(const_cast<char*>(dobj->GetName().getString()),name) && 
				classType == classType )
			{
				return dobj;
			}
		}
		begin++;
	}
	return NULL;
}
/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
xDistributedObject* IDistributedObjects::get(int serverID)
{

#ifdef _DEBUG
	assert( getNetInterface() );
#endif
	if (!serverID)return NULL;

	xDistributedObjectsArrayType *distObjects = getNetInterface()->getDistributedObjects();
	int server  = getNetInterface()->m_IsServer;
	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();
	while (begin!=end)
	{
		xDistributedObject* dobj = *begin;
		if (dobj)
		{
			int serverID2 = dobj->getServerID();
			if(dobj->getServerID()  == serverID)
			{
				return dobj;
			}
		}
		begin++;
	}

	return NULL;
}
/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
xDistributedObject* IDistributedObjects::getByEntityID(int entityID)
{

#ifdef _DEBUG
	assert( getNetInterface() );
#endif

	if (!entityID)return NULL;
	xDistributedObjectsArrayType *distObjects = getNetInterface()->getDistributedObjects();
	int server  = getNetInterface()->m_IsServer;
	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();
	while (begin!=end)
	{
		xDistributedObject* dobj = *begin;
		if (dobj)
		{
			if(dobj->getEntityID()  == entityID)
			{
				return dobj;
			}
		}
		begin++;
	}

	return NULL;
}

/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
xDistributedObject * IDistributedObjects::create(int objID,const char *templateClassName,const char* objectName/*=""*/ )
{


	
	if (!objID)return NULL;
	if (!getNetInterface())return NULL;
	

	//////////////////////////////////////////////////////////////////////////
	//the dist class : 
	xDistributedClassesArrayType *_classes = getNetInterface()->getDistributedClassInterface()->getDistrutedClassesPtr();

	//look it exist : 
	xDistClassIt it  = _classes->find(templateClassName);

	xDistributedClass *classTemplate = it->second;
	xDistributedObject*distObject = get(objID);

	/////////////////////////////////////////////////////////////////////////
	//the network update :
	xNetInterface *cin  = getNetInterface();
	if (!cin)
	{
		//xLogger::xLog(ELOGERROR,XL_SESSION,"
		return NULL;
	}

	vtConnection *con = cin->getConnection();
	//if (!con)return NULL;

	//////////////////////////////////////////////////////////////////////////
	//the dist object : 

	xDistributedObjectsArrayType *distObjects = cin->getDistributedObjects();
	if (!distObjects)return NULL;

	
	if ( !distObject /*&& classTemplate->GetEnitityType() == E_DC_BTYPE_3D_ENTITY */)
	{
		
		distObject  = new xDistributedObject();
		//distObject->SetDistributedClass(classTemplate);
		distObject->setObjectFlags(E_DO_CREATION_INCOMPLETE);
		distObject->setEntityID(objID);
		distObject->setServerID(-1);
		distObject->setCreationTime((float)TNL::Platform::getRealMilliseconds());
		distObject->SetName(objectName);
		distObjects->push_back(distObject);
		xLogger::xLog(ELOGINFO,XL_START,"dist object created : %s",objectName);
		return distObject;
	}
	return NULL;
}
/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
IDistributedObjects::IDistributedObjects()
{
	m_NetworkInterface = NULL;

}
/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
IDistributedObjects::IDistributedObjects(xNetInterface *netInterface) : m_NetworkInterface(netInterface)
{

}
/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
IDistributedObjects::~IDistributedObjects()
{

}

/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
void
IDistributedObjects::Clean()
{
	/*if(GetNetworkInterface())
		GetNetworkInterface()->GetDistributedObjects()->Clear();*/
}

/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
void
IDistributedObjects::deleteObject(int serverID)
{

	TNL::logprintf("___delete object : %d",serverID);

	xDistributedObject *distObject  = get(serverID);
	if (distObject)
	{
		removeObject(distObject);

		xDistributedClass *_class  = distObject->getDistributedClass();
		if (_class)
		{
			xLogger::xLog(XL_START,ELOGINFO,E_LI_DISTRIBUTED_BASE_OBJECT,"Distributed object deleted,server id  : %d class : %s",serverID,_class->getClassName().getString());
			
			if (_class->getEnitityType()  == E_DC_BTYPE_SESSION )
			{
				getNetInterface()->getDistributedClassInterface()->destroyClass(_class);
			}
			delete distObject;
		}else
			xLogger::xLog(XL_START,ELOGERROR,E_LI_DISTRIBUTED_BASE_OBJECT,"Object %d has no class!",serverID);
	}else{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_DISTRIBUTED_BASE_OBJECT,"Couldn't find distributed object with server id  : %d",serverID);
	}
}
void
IDistributedObjects::deleteObject(xDistributedObject*object)
{

	//TNL::logprintf("___delete object : %d",serverID);
	if (!object)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_DISTRIBUTED_BASE_OBJECT,"invalid object");
		return;
	}
	
	if (object)
	{
		removeObject(object);

		xDistributedClass *_class  = object->getDistributedClass();
		if (_class)
		{
			xLogger::xLog(XL_START,ELOGINFO,E_LI_DISTRIBUTED_BASE_OBJECT,"deleted %s | class : %s",object->GetName().getString(),_class->getClassName().getString());

			if (_class->getEnitityType()  == E_DC_BTYPE_SESSION )
			{
				getNetInterface()->getDistributedClassInterface()->destroyClass(_class);
			}
			delete object;
		}else
			xLogger::xLog(XL_START,ELOGERROR,E_LI_DISTRIBUTED_BASE_OBJECT,"Object %d has no class!");
	}
}
/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
void IDistributedObjects::removeObject(xDistributedObject* object)
{

	if (!object)return;
	if (!getNetInterface())	{		return;	}
	if (!getNetInterface()->getDistributedObjects())return;
	
	xDistributedObjectsArrayType *distObjects = getNetInterface()->getDistributedObjects();


	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();
	while (begin!=end)
	{
		xDistributedObject* dobj = *begin;
		if (dobj && dobj==object)
		{

			xLogger::xLog(ELOGINFO,E_LI_DISTRIBUTED_BASE_OBJECT,"Removed : server id : %d | class : %s ",object->getServerID(),object->getDistributedClass() ? object->getDistributedClass()->getClassName().getString() : "NOCLASS");
			getNetInterface()->getDistributedObjects()->erase(begin);

			xDistributedClass *_class  = object->getDistributedClass();
			if (_class && _class->getEnitityType()  == E_DC_BTYPE_SESSION)
			{
					getNetInterface()->getDistributedClassInterface()->destroyClass(_class);
			}else
				xLogger::xLog(XL_START,ELOGERROR,E_LI_DISTRIBUTED_BASE_OBJECT,"Object %d : %s has no class!",object->getServerID(),object->GetName().getString());

			return;
	
		}
		begin++;
	}
}

/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
void 
IDistributedObjects::removeAll(xDistributedObjectsArrayType *distObjects)
{
	if (!distObjects)return;

	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();
	while (begin!=end)
	{
		xDistributedObject* dobj = *begin;
		if (dobj)
		{
			//distObjects->erase(begin);
			delete dobj;
		}
		//begin  = distObjects->begin();
		begin++;
	}

	distObjects->clear();

}


