#include "xDistributedObject.h"
#include "xDistributed3DObject.h"
#include "xDistributedBaseClass.h"
#include "xNetInterface.h"

#include "xDistributedProperty.h"
#include "xDistributedPropertyInfo.h"
#include "IDistributedClasses.h"
#include <xLogger.h>

#include <uxString.h>


uxString
xDistributedObject::print(TNL::BitSet32 flags)
{

	using namespace std;
	uxString result(" ");
	char buffer[4096];
	//////////////////////////////////////////////////////////////////////////
	//name ? 
	if (isFlagOn(flags,E_OPF_NAME))
	{
		result << " Name:" << GetName().getString();
	}
	

	
	//ghostID
	if (isFlagOn(flags,E_OPF_GHOST_ID))
	{
		result << " GhostID:";
		int gID = getOwnerConnection() ? getOwnerConnection()->getGhostIndex((TNL::NetObject*)this) :-1 ;
		result <<gID ;
	}

	//userID?
	if (isFlagOn(flags,E_OPF_USER_ID))
	{
		result << " UserID:" << getUserID();
	}

	//userID?
	if (isFlagOn(flags,E_OPF_CLASS))
	{
		result << " Class : " << uxString(getDistributedClass() ? getDistributedClass()->getClassName().getString() : "NOCLASS");
		int EType = getDistributedClass() ? getDistributedClass()->getEnitityType() : -1;
		result << " : EType:" << EType;
	}
	
	//SessionID?
	if (isFlagOn(flags,E_OPF_SESSION_ID))
	{
		result << " SessionID:" << getSessionID();
	}


	return result;
	//TNL::logprintf(result.CStr());
//	xLogger::xLogExtro(0,"UserName : %s \n\tSessionID:%d \n\tIsJoined:%d \n\tGhostIndex:%d",sobj->getUserName().getString(),sobj->getSessionID(),isFlagOn(sobj->getClientFlags(),E_CF_SESSION_JOINED) ? 1 : 0,gIndex  );


}

TNL::U32 
xDistributedObject::packUpdate(TNL::GhostConnection *connection, TNL::U32 updateMask, TNL::BitStream *stream)
{


	xNetInterface *netInterface  = (xNetInterface*) connection->getInterface();

	//write out name :
	const char* name  = GetName().getString();
	stream->writeString(name,strlen(name));
	//write class : 
	xDistributedClass *distClass = getDistributedClass();
	if (distClass)
	{
		//write out the class name : 
		stream->writeString(distClass->getClassName().getString());
		//write out the class base type : 
		stream->writeInt(distClass->getEnitityType(),32);

	}else
		xLogger::xLog(XL_START,ELOGWARNING,E_LI_DISTRIBUTED_BASE_OBJECT,"initial pack : no class found");
	
	//write user id
	stream->writeInt(getUserID(),32);
	//write session id 
	stream->writeInt(getSessionID(),32);

	setNetInterface(netInterface);
	xLogger::xLog(XL_START,ELOGTRACE,E_LI_DISTRIBUTED_BASE_OBJECT,"initial pack");

	return 0;
}


void 
xDistributedObject::unpackUpdate(TNL::GhostConnection *connection, TNL::BitStream *stream)
{

	xNetInterface *netInterface  = (xNetInterface*) connection->getInterface();
	vtConnection *vconnection  = (vtConnection*)connection;

	char oName[256];stream->readString(oName);	// retrieve objects name : 
	char cName[256];stream->readString(cName);	// retrieve objects dist class name : 
	int type  = stream->readInt(32);		
	
	setUserID(stream->readInt(32));
	setSessionID(stream->readInt(32));
	SetName(oName);
	
	
	//find and store the dist class : 
	xDistributedClassesArrayType *_classes = netInterface->getDistributedClassInterface()->getDistrutedClassesPtr();
	xDistributedClass *classTemplate = netInterface->getDistributedClassInterface()->get(cName);
	if (!classTemplate)
	{
		classTemplate  = netInterface->getDistributedClassInterface()->createClass(cName,type);
	}else
		xLogger::xLog(XL_START,ELOGWARNING,E_LI_DISTRIBUTED_BASE_OBJECT,"initial unpack : no class found");
	setDistributedClass(classTemplate);
	
	
	xLogger::xLog(XL_START,ELOGTRACE,E_LI_DISTRIBUTED_BASE_OBJECT,"initial unpack");
	
	setNetInterface(netInterface);



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
xDistributedProperty* xDistributedObject::getProperty(int nativeType)
{
	xDistributedProperty* result  = NULL;
	if (getDistributedClass())
	{
		xDistributedClass *_class  = getDistributedClass();
		xDistributedPropertyArrayType &props = *getDistributedPorperties();
		for (unsigned int i = 0  ;  i < props.size() ; i++ )
		{
			xDistributedProperty *prop  = props[i];

			if (prop->getPropertyInfo()->mNativeType == nativeType)
			{
				return prop;
			}
		}
	}
	return result;
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
xDistributedProperty* xDistributedObject::getUserProperty(const char*name)
{
	xDistributedProperty* result  = NULL;
	if (getDistributedClass())
	{
		xDistributedClass *_class  = getDistributedClass();
		xDistributedPropertyArrayType &props = *getDistributedPorperties();
		for (unsigned int i = 0  ;  i < props.size() ; i++ )
		{
			xDistributedProperty *prop  = props[i];

			if (!strcmp(prop->getPropertyInfo()->mName.getString(),name))
			{
				return prop;
			}
		}
	}
	return result;
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
void xDistributedObject::onGhostAvailable(TNL::GhostConnection *theConnection)
{
	xNetInterface *netInterface  = (xNetInterface*) theConnection->getInterface();
	if (netInterface)
	{
		if (netInterface->IsServer())
		{
			IDistributedObjects *distInterface = netInterface->getDistObjectInterface();
			if (distInterface)
			{
				setServerID(theConnection->getGhostIndex(this));
			}
		}
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
bool xDistributedObject::onGhostAdd(TNL::GhostConnection *theConnection)
{
	xNetInterface *netInterface  = (xNetInterface*) theConnection->getInterface();
	if (netInterface)
	{
		if (!netInterface->IsServer())
		{
			xDistributedObjectsArrayType *distObjects = netInterface->getDistributedObjects();
			distObjects->push_back(this);
			setNetInterface(netInterface);
		}
	}
	return true;
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
xDistributedObject::writeControlState(xNStream *stream)
{
	
}

void
xDistributedObject::changeOwnership(int newUserID,int state)
{

	setUserID(newUserID);
	
	if (newUserID == getUserID())
	{
		setOwnershipState(E_DO_OS_OWNER);
	}

	if (newUserID !=getUserID())
	{
		setOwnershipState(E_DO_OS_OTHER);
	}

}

void
xDistributedObject::getOwnership()
{
	getOwnershipState().set( 1<< E_DO_OS_REQUEST,true );
/*
		xNetInterface *nInterface = getNetInterface();
		if (nInterface)
		{
			if (nInterface->isValid())
			{
				nInterface->getConnection()->c2sDORequestOwnerShip()
			}
		}*/
	

	/*
	if (  && getOwnershipState() !=E_DO_OS_REQUEST)
	{
		setOwnershipState(E_DO_OS_REQUEST);
	}
	*/
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
xDistributedObject::pack(xNStream *bstream)
{
	bstream->writeSignedInt(this->getUpdateBits().getMask(),32);
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
void xDistributedObject::unpack( xNStream *bstream,float sendersOneWayTime )
{
	int uFlags  = bstream->readSignedInt(32);
	setUpdateBits(uFlags);

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
xDistributedObject::prepare()
{
	//TNL::PacketStream stream;
	/*pack(&stream);
	stream.setBytePosition(0);
	unpack(&stream);*/
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
xDistributedObject::initProperties()
{
    
	//////////////////////////////////////////////////////////////////////////
	//we iterate through the dist classes properties
	if (getDistributedClass())
	{

		xDistributedClass *_class  = getDistributedClass();
		xDistributedPropertiesListType *props  = _class->getDistributedProperties();
		if (props)
		{


		}
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
xDistributed3DObject* xDistributedObject::cast(xDistributedObject* _in){ return static_cast<xDistributed3DObject*>(_in);  }

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
xDistributedObject::destroy()
{
	xNetInterface *netInterface  = getNetInterface();
	if (netInterface)
	{
		xDistributedObjectsArrayType *distObjects = netInterface->getDistributedObjects();
		xDistObjectIt begin = distObjects->begin();
		xDistObjectIt end = distObjects->end();
		while (begin!=end)
		{
			if (*begin==this)
			{
				distObjects->erase(begin);
			}
			begin++;
		}
	}
	Parent::Destroy();
	m_DistributedClass=NULL;

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
xDistributedObject::xDistributedObject() : xNetObject()
{
	m_DistributedClass=NULL;
	m_EntityID = 0;
	m_InterfaceFlags =0;
	m_ObjectFlags = 0;
	m_UpdateFlags = 0;
	m_UpdateState = 0;
	m_GhostDebugID = 0;
	m_GhostUpdateBits = 0;
	m_DistrtibutedPorperties = new xDistributedPropertyArrayType(); 
	m_OwnershipState.clear();
	mSessionID = -1;
	mObjectStateFlags =0;



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
xDistributedObject::~xDistributedObject()
{

	Parent::Destroy();

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
/*
void xDistributedObject::performScopeQuery(TNL::GhostConnection *connection)
{
	

	xNetInterface *netInterface  = GetNetInterface();
	if (!netInterface)
	{
		return;
	}
	vtDistributedObjectsArrayType *distObjects = netInterface->getDistributedObjects();
	if (!distObjects)
	{ 
		return;
	}
	vtDistObjectIt begin = distObjects->begin();
	vtDistObjectIt end = distObjects->end();

	while (begin != end)
	{
		xDistributedObject*dobject = *begin;
		if (dobject)
		{	
			connection->objectInScope((TNL::NetObject*) dobject );
		}
		begin++;
	}
}*/
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
/*
void xDistributedObject::onGhostAvailable(TNL::GhostConnection *theConnection)
{
	xNetInterface *netInterface  = (xNetInterface*) theConnection->getInterface();
	if (netInterface)
	{
		if (netInterface->IsServer())
		{
			IDistributedObjects *distInterface = netInterface->getDistObjectInterface();
			if (distInterface)
			{
				//cast a dist object :
//				xDistributedObject *distObject = static_cast<xDistributedObject *>(this);
//				const char *className =distObject->GetDistributedClass()->ClassName().CStr();
				//distInterface->cCreateDistObject(distObject->GetServerID(),this->GetUserID(),this->GetName().getString(),className);
				logprintf("OnGhostAdd");
			}
		}
	}
}*/
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
/*
bool xDistributedObject::onGhostAdd(TNL::GhostConnection *theConnection)
{
	
	xNetInterface *netInterface  = (xNetInterface*) theConnection->getInterface();
	if (netInterface)
	{
		if (!netInterface->IsServer())
		{
			vtDistributedObjectsArrayType *distObjects = netInterface->getDistributedObjects();
			distObjects->PushBack(this);
			SetNetInterface(netInterface);
			logprintf("OnGhostAdd");
		
		}
	}
	return TRUE;
}
*/
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
/*
TNL::U32 
xDistributedObject::packUpdate(TNL::GhostConnection *connection, TNL::U32 updateMask, xNStream *stream)
{

	xNetInterface *netInterface  = (xNetInterface*) connection->getInterface();
	if (netInterface)
	{

		//server side only : 
		if (netInterface->IsServer())
		{
			//the first time  ? : we write out all necessary attributes for the client : 
			if(stream->writeFlag(updateMask & InitialMask))
			{
				//write out name :
				const char* name  = this->GetName().getString();
				stream->writeString(name,strlen(name));
				//retrieve its class : 
				xDistributedClass *distClass = GetDistributedClass();
				if (distClass)
				{
					//write out the class name : 
					stream->writeString(distClass->ClassName().CStr());
					//write out the class base type : 
					stream->writeSignedInt(distClass->GetEnitityType(),16);
					vtConnection *con  = (vtConnection*)connection;
					stream->writeSignedInt(con->GetUserID(),16);
					
				}else
				{
					//write out the class name : 
					stream->writeString("NOCLASS");
					//write out the class base type : 
					stream->writeSignedInt(100,16);
					vtConnection *con  = (vtConnection*)connection;
					stream->writeSignedInt(con->GetUserID(),16);
				}
				logprintf("server:init pack update");
				SetNetInterface(netInterface);
			}
		}
	}
	return 0;
}
*/
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
/*
void 
xDistributedObject::unpackUpdate(TNL::GhostConnection *connection, xNStream *stream)
{

	
	xNetInterface *netInterface  = (xNetInterface*) connection->getInterface();
	if (netInterface)
	{
		//client side only : 
		if (!netInterface->IsServer())
		{
			//////////////////////////////////////////////////////////////////////////
			//initial update  ? 
			if(stream->readFlag())
			{

				char oName[256];stream->readString(oName);	// retrieve objects name : 
				char cName[256];stream->readString(cName);	// retrieve objects dist class name : 
				int type  = stream->readInt(16);							//read the dist class base type : 

				//////////////////////////////////////////////////////////////////////////
				//now we store all in the dist object :
				SetName(oName);

				//find and store the dist class : 
				XDistributedClassesArrayType *_classes = netInterface->getDistributedClassInterface()->getDistrutedClassesPtr();
				xDistributedClass *classTemplate = NULL;
				xDistClassIt it  = _classes->Find(cName);
				if (it != _classes->End())
				{
					classTemplate = *it;
					//store the class :
					//distObject->SetDistributedClass(classTemplate);
				}
				//store server id : 
				int serverID  = connection->getGhostIndex((NetObject*)this);
				SetServerID(serverID);
				SetObjectFlags(E_DO_CREATION_CREATED);

				this->SetNetInterface(netInterface);

				vtConnection *con  = (vtConnection*)connection;
				int userID = stream->readInt(16);	
				SetUserID(userID);

				logprintf("DO initial update received by server : %s | class : %s",oName,cName );
				//store it in our array : 
				//vtDistributedObjectsArrayType *distObjects = netInterface->GetDistributedObjects();
				//distObjects->PushBack(distObject);
			}
		}
	}
}
*/
