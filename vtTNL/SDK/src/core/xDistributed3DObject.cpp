#include "xDistributed3DObject.h"
#include "xDistributedProperty.h"
#include "xDistributedPropertyInfo.h"

#include "xNetInterface.h"
#include "vtConnection.h"


#include "xDistributedBaseClass.h"
#include "xDistributed3DObjectClass.h"
#include "IDistributedClasses.h"
#include "xDOStructs.h"

#include "IDistributedObjectsInterface.h"

#include "xDistributedPoint4F.h"
#include "xDistributedPoint3F.h"
#include "xDistributedPoint2F.h"
#include "xDistributedPoint1F.h"
#include "xDistributedString.h"
#include "xDistributedInteger.h"
#include "xDistributedClient.h"
#include "xLogger.h"


TNL_IMPLEMENT_NETOBJECT(xDistributed3DObject); 

uxString
xDistributed3DObject::print(TNL::BitSet32 flags)
{

	return Parent::print(flags);

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
xDistributed3DObject::pack(TNL::BitStream *bstream)
{

	//writes update bits : !
	Parent::pack(bstream);

	int updateBites = getUpdateBits().getMask();

	int packCounter = 0 ;
	xDistributedPropertyArrayType &props = *getDistributedPorperties();
	for (unsigned int i = 0  ;  i < props.size() ; i++ )
	{
		xDistributedProperty *prop  = props[i];
		xDistributedPropertyInfo*propInfo  = prop->getPropertyInfo();
		if (propInfo)
		{
			int blockIndex = prop->getBlockIndex();
			int blockIndexF = BIT(blockIndex);
			int blockIndexF2 = 1 << blockIndex;

			if (prop->getFlags() & E_DP_NEEDS_SEND)
			{
				prop->pack(bstream);
				packCounter++;
				//xLogger::xLog(ELOGINFO,XL_START,"packing byflag");
			}

			if (getUpdateBits().test( 1 << (prop->getBlockIndex())))
			{
			
			}
		}
	}
	//xLogger::xLog(ELOGINFO,XL_START,"packing %d props with mask ",packCounter,getUpdateBits().getMask() );
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
void xDistributed3DObject::unpack( TNL::BitStream *bstream,float sendersOneWayTime )
{
	Parent::unpack(bstream,sendersOneWayTime);
		
	int updateBites = getUpdateBits().getMask();
	
	int unpackCounter =  0 ;
	xDistributedPropertyArrayType &props = *getDistributedPorperties();
	for (unsigned int i = 0  ;  i < props.size() ; i++ )
	{
		xDistributedProperty *prop  = props[i];
		xDistributedPropertyInfo*propInfo  = prop->getPropertyInfo();
		int blockIndex = prop->getBlockIndex();
		
		if (propInfo)
		{
			
			if (getUpdateBits().testStrict(1<<blockIndex))
			{
				prop->unpack(bstream,sendersOneWayTime);
				unpackCounter++;
				//xLogger::xLog(ELOGINFO,XL_START,"\t unpacking prop:%s",propInfo->mName.getString());
				setMaskBits(1<<blockIndex);
			}
		}
	}

	//xLogger::xLog(ELOGINFO,XL_START,"\t unpacking %d props with mask %d",unpackCounter,getUpdateBits().getMask() );
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
TNL::U32 xDistributed3DObject::packUpdate(TNL::GhostConnection *connection, TNL::U32 updateMask, TNL::BitStream *stream)
{
	xNetInterface *netInterface  = (xNetInterface*) connection->getInterface();
	if (netInterface)
	{
		//the first time  ? : we write out all necessary attributes for the client : 
		if(stream->writeFlag(updateMask & InitialMask))
		{

			if(stream->writeFlag(true))
			{
				//write out name :
				stream->writeString(GetName().getString(),strlen(GetName().getString()));

				//retrieve its class : 
				xDistributedClass *distClass = getDistributedClass();
				if (distClass)
				{
					//write out the class name : 
					stream->writeString(distClass->getClassName().getString(),strlen(distClass->getClassName().getString()));

					//write out the class base type : 
					stream->writeInt(distClass->getEnitityType(),32);

					//write out users id :
					vtConnection *con  = (vtConnection*)connection;
					int uid = getUserID();
					stream->writeInt(getUserID(),32);

					//write out server side id  : 
					int serverID = connection->getGhostIndex(this);
					stream->writeInt(connection->getGhostIndex(this),32);
					setServerID(connection->getGhostIndex(this));

					float time = getCreationTime();
					//write out creation time
					stream->write(getCreationTime());
					//xLogger::xLog(ELOGINFO,XL_START,"server:init pack update : %f , %d , ",time,uid);
					xLogger::xLog(XL_START,ELOGTRACE,E_LI_3DOBJECT,"initial pack!");
					
				}
				setNetInterface(netInterface);
			}
		}
		/************************************************************************/
		/*																			                                                                   */
		/************************************************************************/
		stream->writeSignedInt(getUpdateBits().getMask(),32);
		int unpackCounter =  0 ;
		xDistributedPropertyArrayType &props = *getDistributedPorperties();
		for (unsigned int i = 0  ;  i < props.size() ; i++ )
		{
			xDistributedProperty *prop  = props[i];
			xDistributedPropertyInfo*propInfo  = prop->getPropertyInfo();
			int blockIndex = prop->getBlockIndex();
			if (propInfo)
			{
				if (getUpdateBits().testStrict(1<<blockIndex))
				{
					prop->updateGhostValue(stream);
					//xLogger::xLog(ELOGINFO,XL_START,"server is updating ghosts %d",blockIndex);
				}
			}
		}
	}
	return 0;
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
xDistributed3DObject::unpackUpdate(TNL::GhostConnection *connection, TNL::BitStream *stream)
{

	xNetInterface *netInterface  = (xNetInterface*) connection->getInterface();
	if (netInterface)
	{
		//////////////////////////////////////////////////////////////////////////
		//initial update  ? 
		if(stream->readFlag())
		{
			if(stream->readFlag())
			{
				char oName[256];stream->readString(oName);	// retrieve objects name : 
				char cName[256];stream->readString(cName);	// retrieve objects dist class name : 
				int type  = stream->readInt(32);							//read the dist class base type : 
				int userID = stream->readInt(32);	
				int serverID2 = stream->readInt(32);	
				float creationTime = 0.0f;
				stream->read(&creationTime);

	            
				//////////////////////////////////////////////////////////////////////////
				//now we store all in the dist object :
				
				//find and store the dist class : 
				xDistributedClassesArrayType *_classes = netInterface->getDistributedClassInterface()->getDistrutedClassesPtr();
				xDistributedClass *classTemplate = netInterface->getDistributedClassInterface()->get(cName);
				if (!classTemplate)
				{
					classTemplate  = netInterface->getDistributedClassInterface()->createClass(cName,type);
					xLogger::xLog(XL_START,ELOGERROR,E_LI_3DOBJECT,"No class found %s",oName);
				}
				setDistributedClass(classTemplate);
				SetName(oName);
				setServerID(connection->getGhostIndex(this));
				setObjectFlags(E_DO_CREATION_CREATED);
				setUserID(userID);
				getOwnershipState().set( 1<<E_DO_OS_OWNER,  getUserID() == ((vtConnection*)connection)->getUserID() );
				setNetInterface(netInterface);
				setCreationTime(creationTime);
				setInterfaceFlags(E_DO_CREATED);
				initProperties();
				setSessionID((netInterface->getMyClient()->getSessionID()));

				enableFlag(getObjectStateFlags(),E_DOSF_UNPACKED);

				
				xLogger::xLog(XL_START,ELOGTRACE,E_LI_3DOBJECT,"Retrieved initial state %s | sessionID:%d",oName,netInterface->getMyClient()->getSessionID());
			}
		}

	
		
		int updateBits = stream->readSignedInt(32);
		TNL::BitSet32 updateBitsMask(updateBits);
		setGhostUpdateBits(updateBits);

		xDistributedPropertyArrayType &props = *getDistributedPorperties();
		for (unsigned int i = 0  ;  i < props.size() ; i++ )
		{
			xDistributedProperty *prop  = props[i];
			xDistributedPropertyInfo*propInfo  = prop->getPropertyInfo();
			int blockIndex = prop->getBlockIndex();
			if (updateBitsMask.testStrict(1<<blockIndex))
			{
				prop->updateFromServer(stream);
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
void xDistributed3DObject::destroy()
{
	Parent::destroy();
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
xDistributed3DObject::xDistributed3DObject() : xDistributedObject()
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
xDistributed3DObject::~xDistributed3DObject()
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
void xDistributed3DObject::performScopeQuery(TNL::GhostConnection *connection)
{
	xNetInterface *netInterface  = getNetInterface();
	if (!netInterface)
	{
		return;
	}
	xDistributedObjectsArrayType *distObjects = netInterface->getDistributedObjects();
	if (!distObjects)
	{ 
		return;
	}
	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();

	while (begin != end)
	{
		xDistributedObject*dobject = *begin;
		if (dobject)
		{	
			//if (getSessionID() == dobject->getSessionID())
			{
				connection->objectInScope((TNL::NetObject*) dobject );
			}
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
xDistributed3DObject::doInitUpdate()
{
	xDistributedPropertyArrayType &props = *getDistributedPorperties();
	for (unsigned int i = 0  ;  i < props.size() ; i++ )
	{
		xDistributedProperty *prop  = props[i];
		xDistributedPropertyInfo*propInfo  = prop->getPropertyInfo();
		int blockIndex = prop->getBlockIndex();
		if (propInfo)
		{
			prop->setFlags(E_DP_NEEDS_SEND );
		}
	}
	setUpdateState(E_DO_US_PENDING);

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
xDistributed3DObject::updateAll()
{

	xDistributedPropertyArrayType &props = *getDistributedPorperties();
	for (unsigned int i = 0  ;  i < props.size() ; i++ )
	{
		xDistributedProperty *prop  = props[i];
		xDistributedPropertyInfo*propInfo  = prop->getPropertyInfo();
		int blockIndex = prop->getBlockIndex();
		if (propInfo)
		{
			setMaskBits(1<<blockIndex);
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
void xDistributed3DObject::onGhostAvailable(TNL::GhostConnection *theConnection)
{
	xNetInterface *netInterface  = (xNetInterface*) theConnection->getInterface();
	vtConnection *con = (vtConnection*)theConnection;
	xDistributedClient *client  = (xDistributedClient*)netInterface->getDistObjectInterface()->getByUserID(con->getUserID(),E_DC_BTYPE_CLIENT);
	
	if (netInterface)
	{
		if (netInterface->IsServer())
		{
			IDistributedObjects *distInterface = netInterface->getDistObjectInterface();
			if (distInterface)
			{
				setServerID(theConnection->getGhostIndex(this));
				setSessionID(client->getSessionID());
				//doInitUpdate();
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
bool xDistributed3DObject::onGhostAdd(TNL::GhostConnection *theConnection)
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
xDistributed3DObject::initProperties()
{
	//////////////////////////////////////////////////////////////////////////
	//we iterate through the dist classes properties and create a real 
	//data holding dist prop in the object !

	//xLogger::xLog(ELOGINFO,XL_START,"init props of dist object : %s",this->GetName().getString());
	if (getDistributedClass())
	{
		xDistributedClass *_class  = getDistributedClass();
		xDistributedPropertiesListType &props  = *_class->getDistributedProperties();
		int userTypeCounter  = 0;

		
		for (unsigned int i  = 0 ; i < props.size() ; i ++ )
		{
			xDistributedProperty *prop = NULL;
			xDistributedPropertyInfo *dInfo  = props[i];


			xLogger::xLog(XL_START,ELOGTRACE,E_LI_3DOBJECT,"\t attaching prop : %s | native type : %d",dInfo->mName.getString(),dInfo->mNativeType);
			switch(dInfo->mValueType)
			{
				case E_DC_PTYPE_3DVECTOR :
				{
					prop = new xDistributedPoint3F(dInfo,30,3000);
					getDistributedPorperties()->push_back(prop);
					//xLogger::xLog(ELOGINFO,XL_START,"\t attaching prop : %s | native type : %d",dInfo->mName.getString(),dInfo->mNativeType);
				}
				break;

				case E_DC_PTYPE_FLOAT:
				{
					prop = new xDistributedPoint1F(dInfo,30,3000);
					getDistributedPorperties()->push_back(prop);
					//xLogger::xLog(ELOGINFO,XL_START,"\t attaching prop : %s | native type : %d",dInfo->mName.getString(),dInfo->mNativeType);
					break;
				}
				case E_DC_PTYPE_2DVECTOR:
				{
					prop = new xDistributedPoint2F(dInfo,30,3000);
					getDistributedPorperties()->push_back(prop);
					//xLogger::xLog(ELOGINFO,XL_START,"\t attaching prop : %s | native type : %d",dInfo->mName.getString(),dInfo->mNativeType);
					break;
				}
				case E_DC_PTYPE_QUATERNION:
				{
					prop = new xDistributedQuatF(dInfo,30,3000);
					getDistributedPorperties()->push_back(prop);
					//xLogger::xLog(ELOGINFO,XL_START,"\t attaching prop : %s | native type : %d",dInfo->mName.getString(),dInfo->mNativeType);
					break;
				}
				case E_DC_PTYPE_STRING:
				{
					prop = new xDistributedString(dInfo,30,3000);
					getDistributedPorperties()->push_back(prop);
					//xLogger::xLog(ELOGINFO,XL_START,"\t attaching prop : %s | native type : %d",dInfo->mName.getString(),dInfo->mNativeType);
					break;
				}
				case E_DC_PTYPE_INT:
				{
					prop = new xDistributedInteger(dInfo,30,3000);
					getDistributedPorperties()->push_back(prop);
					//xLogger::xLog(ELOGINFO,XL_START,"\t attaching prop : %s | native type : %d",dInfo->mName.getString(),dInfo->mNativeType);
					break;
				}

			}
		
			if (dInfo->mNativeType == E_DC_3D_NP_USER )
			{
				int blockID = _class->getFirstUserField() + userTypeCounter;
				userTypeCounter++;
				prop->setBlockIndex(blockID);
			}else
			{
				prop->setBlockIndex(dInfo->mNativeType);
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
xDistributedProperty*
xDistributed3DObject::getProperty(int nativeType)
{
	//return Parent::getProperty(nativeType);
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
xDistributedProperty*
xDistributed3DObject::getUserProperty(const char*name)
{
	//return Parent::getUserProperty(name);
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
void
xDistributed3DObject::update(float time)
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
xDistributed3DObject::prepare()
{
	Parent::prepare();
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
TNL_IMPLEMENT_NETOBJECT_RPC(xDistributed3DObject,c2sGetOwnerShip,
							(TNL::U32 userSrcID),(userSrcID),
							TNL::NetClassGroupGameMask, TNL::RPCGuaranteedOrdered,TNL::RPCToGhostParent, 0)
{

	//setMaskBits(PositionMask);
}

void
xDistributed3DObject::onGhostRemove()
{
	if (getNetInterface())
	{

		if(!getNetInterface()->IsServer())
		{
			getNetInterface()->addDeleteObject(this);
		}
		xLogger::xLog(XL_START,ELOGTRACE,E_LI_3DOBJECT,"removing...");
		getNetInterface()->getDistObjectInterface()->removeObject(this);
	}
}
