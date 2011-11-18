#include "xDistributedSession.h"
#include "xDistributedProperty.h"
#include "xDistributedPropertyInfo.h"

#include "xNetInterface.h"
#include "vtConnection.h"


#include "xDistributedBaseClass.h"
#include "xDistributedSessionClass.h"
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

int sessionIDCounter = 20;

#include "xLogger.h"


uxString
xDistributedSession::print(TNL::BitSet32 flags)
{
	return Parent::print(flags);
}



TNL_IMPLEMENT_NETOBJECT(xDistributedSession); 





xDistributedSession::xDistributedSession()
{

	mSessionFlags = 0;
	mSessionID =sessionIDCounter;
	sessionIDCounter ++;
	mClientTable  = new xClientArrayType();

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
xDistributedSession::~xDistributedSession()
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
void xDistributedSession::pack(TNL::BitStream *bstream)
{

	//writes update bits : !
	Parent::pack(bstream);

	xDistributedPropertyArrayType &props = *getDistributedPorperties();
	for (unsigned int i = 0  ;  i < props.size() ; i++ )
	{
		xDistributedProperty *prop  = props[i];
		xDistributedPropertyInfo*propInfo  = prop->getPropertyInfo();
		if (propInfo)
		{
			if (prop->getFlags() & E_DP_NEEDS_SEND)
			{
				prop->pack(bstream);
				//xLogger::xLog(ELOGINFO,XL_START,"sending session parameter to server : %s",prop->getPropertyInfo()->mName.getString());
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
void xDistributedSession::unpack( TNL::BitStream *bstream,float sendersOneWayTime )
{
	Parent::unpack(bstream,sendersOneWayTime);


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
				//xLogger::xLog(ELOGINFO,XL_START,"\t unpacking session prop on server :%s",propInfo->mName.getString());
				setMaskBits(1<<blockIndex);
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
TNL::U32 xDistributedSession::packUpdate(TNL::GhostConnection *connection, TNL::U32 updateMask, TNL::BitStream *stream)
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
				//	xLogger::xLog(ELOGINFO,XL_START,"server:init pack update of %s: %f , %d , ",GetName().getString(),time,uid);
					stream->writeInt(getSessionID(),32);
					stream->writeInt(getMaxUsers(),32);
					stream->writeString(getPassword().getString(),strlen(getPassword().getString()));

					//netInterface->deploySessionClasses(connection);

				}
				setNetInterface(netInterface);
			}
		}
		/************************************************************************/
		/*																			                                                                   */
		/************************************************************************/
		stream->writeSignedInt(getUpdateBits().getMask(),32);
		//xLogger::xLog(ELOGINFO,XL_START,"server is updating ghosts %d", getUpdateBits().getMask() );
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
		//getUpdateBits().clear();
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
void xDistributedSession::unpackUpdate(TNL::GhostConnection *connection, TNL::BitStream *stream)
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
				int sessionID = stream->readInt(32);	
				int MaxUsers = stream->readInt(32);	
				char pass[256];stream->readString(pass);





				//////////////////////////////////////////////////////////////////////////
				//now we store all in the dist object :

				//find and store the dist class : 
				xDistributedClassesArrayType *_classes = netInterface->getDistributedClassInterface()->getDistrutedClassesPtr();
				xDistributedClass *classTemplate = netInterface->getDistributedClassInterface()->get(cName,type);
				if (!classTemplate)
				{
					xLogger::xLog(ELOGERROR,E_LI_SESSION,"Warning initial unpack of session %s failed :  no related class found  : %s",oName,cName);
					//xLogger::xLog(ELOGINFO,XL_START,"Warning Initial Unpack Update, no related class found  : %s",cName);
					//xLogger::xLog(ELOGERROR,XL_SESSION,"Warning Initial Unpack Update, no related class found  : %s",cName);
					classTemplate  = netInterface->getDistributedClassInterface()->createClass(cName,type);

				}
				setDistributedClass(classTemplate);
				SetName(oName);
				setServerID(connection->getGhostIndex(this));
				setObjectFlags(E_DO_CREATION_CREATED);
				setUserID(userID);
				setNetInterface(netInterface);
				setCreationTime(creationTime);
				setSessionID(sessionID);
                			
				
				getOwnershipState().set( 1<<E_DO_OS_OWNER,  getUserID() == ((vtConnection*)connection)->getUserID() );

				setInterfaceFlags(E_DO_CREATED);
				getSessionFlags().set( 1 << E_SF_INCOMPLETE );


				xLogger::xLog(ELOGTRACE,E_LI_SESSION,"Retrieved initial state of session %s",oName);
				initProperties();
				setMaxUsers(MaxUsers);
				setPassword(xNString(pass));
				vtConnection *con  = (vtConnection*)connection;
				setOwnerConnection(con);
			}
		}



		int updateBits = stream->readSignedInt(32);
		TNL::BitSet32 updateBitsMask(updateBits);
		setGhostUpdateBits(updateBits);

		xDistributedPropertyArrayType &props = *getDistributedPorperties();
		int propCounter = 0;
		for (unsigned int i = 0  ;  i < props.size() ; i++ )
		{
			xDistributedProperty *prop  = props[i];
			xDistributedPropertyInfo*propInfo  = prop->getPropertyInfo();
			int blockIndex = prop->getBlockIndex();
			if (updateBitsMask.testStrict(1<<blockIndex))
			{
				prop->updateFromServer(stream);
				xLogger::xLog(ELOGTRACE,E_LI_SESSION,"Client : retrieving session property : %s |pred :%d",prop->getPropertyInfo()->mName.getString(),prop->getPropertyInfo()->mPredictionType);
				//xLogger::xLog(ELOGINFO,XL_START,"client : retrieving session property : %s",prop->getPropertyInfo()->mName.getString());
				getSessionFlags().set( 1 << E_SF_COMPLETE );
				propCounter++;
			}
		}

		if (propCounter == props.size())
		{
			getSessionFlags().set( 1 << E_SF_COMPLETE );
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
void xDistributedSession::initProperties()
{
	//////////////////////////////////////////////////////////////////////////
	//we iterate through the dist classes properties and create a real 
	//data holding dist prop in the object !

	xLogger::xLog(ELOGTRACE,E_LI_SESSION,"Initiate properties of session : %s",this->GetName().getString());
	if (getDistributedClass())
	{
		xDistributedClass *_class  = getDistributedClass();
		xDistributedPropertiesListType &props  = *_class->getDistributedProperties();
		int userTypeCounter  = 0;


		for (unsigned int i  = 0 ; i < props.size() ; i ++ )
		{
			xDistributedProperty *prop = NULL;
			xDistributedPropertyInfo *dInfo  = props[i];


			xLogger::xLog(ELOGTRACE,E_LI_SESSION,"\t attaching prop : %s | native type : %d | valueType : %d | prediction type : %d ",dInfo->mName.getString(),dInfo->mNativeType,dInfo->mValueType,dInfo->mPredictionType);
			//xLogger::xLogExtro(0,"\t attaching prop : %s | native type : %d | valueType : %d | prediction type : %d ",dInfo->mName.getString(),dInfo->mNativeType,dInfo->mValueType,dInfo->mPredictionType);
			switch(dInfo->mValueType)
			{
			case E_DC_PTYPE_3DVECTOR :
				{
					prop = new xDistributedPoint3F(dInfo,30,3000);
					getDistributedPorperties()->push_back(prop);
					
					break;
				}
			case E_DC_PTYPE_FLOAT:
				{
					prop = new xDistributedPoint1F(dInfo,30,3000);
					getDistributedPorperties()->push_back(prop);
					break;
				}
			case E_DC_PTYPE_2DVECTOR:
				{
					prop = new xDistributedPoint2F(dInfo,30,3000);
					getDistributedPorperties()->push_back(prop);
					break;
				}
			case E_DC_PTYPE_QUATERNION:
				{
					prop = new xDistributedQuatF(dInfo,30,3000);
					getDistributedPorperties()->push_back(prop);
					break;
				}
			case E_DC_PTYPE_STRING:
				{
					prop = new xDistributedString(dInfo,30,3000);
					getDistributedPorperties()->push_back(prop);
					break;
				}
			case E_DC_PTYPE_INT:
				{
					prop = new xDistributedInteger(dInfo,30,3000);
					getDistributedPorperties()->push_back(prop);
					break;
				}

			}

			if (dInfo->mNativeType == E_DC_S_NP_USER )
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
void xDistributedSession::prepare()
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
void xDistributedSession::onGhostRemove()
{
	xNetInterface *netInterface  = getNetInterface();

	if (getNetInterface())
	{

		if(!getNetInterface()->IsServer())
		{
			xDistributedObjectsArrayType *distObjects = netInterface->getDistributedObjects();
			xDistObjectIt begin = distObjects->begin();
			xDistObjectIt end = distObjects->end();

			while (begin!=end)
			{
				xDistributedObject *distObject  = *begin;
				if (distObject)
				{
					xDistributedClass *_class = distObject->getDistributedClass();
					if (_class)
					{
						if (_class->getEnitityType() == E_DC_BTYPE_CLIENT )
						{
							xDistributedClient *distClient  = static_cast<xDistributedClient*>(distObject);
							if (distClient && distClient->getSessionID() == getSessionID() )
							{
								distClient->setSessionID(-1);
								disableFlag(distClient->getClientFlags(),E_CF_SESSION_JOINED);
								enableFlag(distClient->getClientFlags(),E_CF_DELETING);
								enableFlag(distClient->getClientFlags(),E_CF_SESSION_DESTROYED);

							}
						}
					}
				}
				begin++;
			}
		}
		
		if(getNetInterface()->IsServer())
		{
			xLogger::xLog(XL_START,ELOGINFO,E_LI_SESSION,"Server : Session is going to be deleted : %s",this->GetName().getString());
		}
		
		getNetInterface()->getDistObjectInterface()->removeObject(this);
		
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
bool xDistributedSession::onGhostAdd(TNL::GhostConnection *theConnection)
{
	return Parent::onGhostAdd(theConnection);

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

void xDistributedSession::onGhostAvailable(TNL::GhostConnection *theConnection)
{
	Parent::onGhostAvailable(theConnection);
	int conID = ((vtConnection*)theConnection)->getUserID();
	
	//xLogger::xLog(ELOGINFO,XL_START, "session ghost available %d",conID);
	
	xDistributedPropertyArrayType &props = *getDistributedPorperties();
	for (unsigned int i = 0  ;  i < props.size() ; i++ )
	{
		xDistributedProperty *prop  = props[i];
		xDistributedPropertyInfo*propInfo  = prop->getPropertyInfo();
		int blockIndex = prop->getBlockIndex();
		if (propInfo)
		{
			if (getUpdateBits().set(1<<blockIndex),true)
			{
				
			}
			setMaskBits(1<<blockIndex);

		}
	}

	xNetInterface *netInterface  = (xNetInterface*) theConnection->getInterface();
	if (netInterface->IsServer())
	{
		setServerID(theConnection->getGhostIndex((TNL::NetObject*)this));
		//xLogger::xLog(XL_START,ELOGINFO,E_LI_SESSION, "ServerID %d | Name : %s",getServerID(),GetName().getString());
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
void xDistributedSession::performScopeQuery(TNL::GhostConnection *connection)
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
			connection->objectInScope((TNL::NetObject*) dobject );
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
xDistributedProperty*xDistributedSession::getProperty(int nativeType)
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
xDistributedProperty*xDistributedSession::getUserProperty(const char*name)
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
bool xDistributedSession::isClientJoined(int userID)
{
	bool result  = false;
	for (int i  = 0  ;  i  < getClientTable().size() ; i ++)
	{
		xDistributedClient *client = getClientTable().at(i);
		if (client->getUserID()  == userID)
			return true;
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
void xDistributedSession::addUser(xDistributedClient *client)
{
	if (client)
	{
		if (!isClientJoined(client->getUserID()))
		{
			getClientTable().push_back(client);
			xDistributedInteger *numUser= (xDistributedInteger *)getProperty(E_DC_S_NP_NUM_USERS);
			numUser->updateValue(getClientTable().size(),0);
			setNumUsers(getClientTable().size());
			int blockIndex = numUser->getBlockIndex();
			getUpdateBits().set(1<<blockIndex,true);
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
void xDistributedSession::removeUser(xDistributedClient *client)
{
	xNetInterface *nInterface = getNetInterface();
	if (!nInterface)
	{
		return;
	}


	xDistributedClient *myClient  = nInterface->getMyClient();
	if (client && myClient && client->getUserID() == myClient->getUserID())
	{
		myClient->setSessionID(-1);
	}

	if (client)
	{
		if (isClientJoined(client->getUserID()))
		{

			//////////////////////////////////////////////////////////////////////////
			if (client->getUserID() == getUserID())
			{
				xLogger::xLog(XL_START,ELOGINFO,E_LI_SESSION,"Session master %d left session %d",client->getUserID(),getSessionID());
				
				if (nInterface)
				{

					//////////////////////////////////////////////////////////////////////////
					//we tag all existing users as new : 
					
					IDistributedObjects *doInterface  = nInterface->getDistObjectInterface();
					xDistributedObjectsArrayType *distObjects = nInterface->getDistributedObjects();
					xDistObjectIt begin = distObjects->begin();
					xDistObjectIt end = distObjects->end();

					while (begin!=end)
					{
						xDistributedObject *distObject  = *begin;
						if (distObject)
						{
							xDistributedClass *_class = distObject->getDistributedClass();
							if (_class)
							{
								if (_class->getEnitityType() == E_DC_BTYPE_CLIENT )
								{
									xDistributedClient *distClient  = static_cast<xDistributedClient*>(distObject);
									if (distClient && isClientJoined(distClient->getUserID()))
									{
										if (isFlagOn(distClient->getClientFlags(),E_CF_SESSION_JOINED))
										{

											//distClient->setSessionID(-1);
											disableFlag(distClient->getClientFlags(),E_CF_SESSION_JOINED);
											enableFlag(distClient->getClientFlags(),E_CF_DELETING);
											enableFlag(distClient->getClientFlags(),E_CF_SESSION_DESTROYED);
											xLogger::xLogExtro(0,"\tRemoving user %d from session",distClient->getUserID(),getSessionID());
										}
									}
								}
							}
						}
						begin++;
					}

					getClientTable().clear();
					goto ENDUPDATE;
					return;
				}
			}
		}
		if (client->getUserID() != getUserID())
		{
			enableFlag(client->getClientFlags(),E_CF_REMOVED);
			client->setSessionID(-1);
		}
		//////////////////////////////////////////////////////////////////////////
		for(int i = 0 ; i < getClientTable().size() ; i++)
		{

			std::vector<xDistributedClient*>::iterator begin = getClientTable().begin();
			std::vector<xDistributedClient*>::iterator end = getClientTable().end();
			while (begin !=end)
			{
				xDistributedClient *current = *begin;
				if (current && current->getUserID()  == client->getUserID() )
				{
                    getClientTable().erase(begin);						
				}
				begin++;
			}
		}

		goto ENDUPDATE;

		ENDUPDATE :
		xDistributedInteger *numUser= (xDistributedInteger *)getProperty(E_DC_S_NP_NUM_USERS);
		numUser->updateValue(getClientTable().size(),0);
		setNumUsers(getClientTable().size());
		int blockIndex = numUser->getBlockIndex();
		getUpdateBits().set(1<<blockIndex,true);
		setMaskBits(1<<blockIndex);
	}else
		xLogger::xLog(XL_START,ELOGERROR,E_LI_SESSION,"couldn't find client object");

	
	
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
xNString xDistributedSession::getPassword()
{

	if (getProperty(E_DC_S_NP_PASSWORD))
	{
		xDistributedString* passwordP= (xDistributedString*)getProperty(E_DC_S_NP_PASSWORD);
		if (passwordP)
		{
			return passwordP->mCurrentValue;
		}
	}
	return xNString("");
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
void xDistributedSession::setPassword(xNString val)
{



	if (getProperty(E_DC_S_NP_PASSWORD))
	{
		xDistributedString* passwordP= static_cast<xDistributedString*>(getProperty(E_DC_S_NP_PASSWORD));
		if (passwordP!=NULL)
		{
			passwordP->mCurrentValue  = val;

		}
	}
}

void xDistributedSession::setNumUsers(int val)
{

	if (getProperty(E_DC_S_NP_NUM_USERS))
	{
		xDistributedInteger* value = (xDistributedInteger*)getProperty(E_DC_S_NP_NUM_USERS);
		if (value)
		{
			value->mLastServerValue  = val;
			value->mCurrentValue  = val;

		}
	}
}
void xDistributedSession::setMaxUsers(int val)
{

	if (getProperty(E_DC_S_NP_MAX_USERS))
	{
		xDistributedInteger* maxUsers = (xDistributedInteger*)getProperty(E_DC_S_NP_MAX_USERS);
		if (maxUsers)
		{
			maxUsers->mLastServerValue  = val;
			maxUsers->mCurrentValue  = val;
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
bool
xDistributedSession::isPrivate()
{
	if (getProperty(E_DC_S_NP_PASSWORD))
	{
		return true;
	}
	return false;
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
bool 
xDistributedSession::isLocked()
{
	int locked   = -1;
	if (getProperty(E_DC_S_NP_LOCKED))
	{
		xDistributedInteger* isLocked = (xDistributedInteger*)getProperty(E_DC_S_NP_LOCKED);
		if (isLocked)
		{
			locked  = isLocked->mLastServerValue;
			return locked;
		}
	}
	return locked;
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
int xDistributedSession::getNumUsers()
{
	int numUsers   = -1;
	if (getProperty(E_DC_S_NP_LOCKED))
	{
		xDistributedInteger* num = (xDistributedInteger*)getProperty(E_DC_S_NP_NUM_USERS);
		if (num)
		{
			numUsers  = num->mLastServerValue;
			//TNL::logprintf("numPS:%d | numPC:%d",num->mLastServerValue,num->mCurrentTime);
			return numUsers;
		}
	}
	return numUsers;
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
bool
xDistributedSession::isFull()
{
	return getNumUsers() >= getMaxUsers();


	int maxplayers   = -1;
	if (getProperty(E_DC_S_NP_MAX_USERS))
	{
		xDistributedInteger* maxPlayersP = (xDistributedInteger*)getProperty(E_DC_S_NP_MAX_USERS);
		if (maxPlayersP)
		{
			maxplayers  = maxPlayersP->mLastServerValue;
			return getNumUsers() < getMaxUsers();
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
int xDistributedSession::getMaxUsers()
{
	int maxplayers   = -1;
	if (getProperty(E_DC_S_NP_MAX_USERS))
	{
		xDistributedInteger* maxPlayersP = (xDistributedInteger*)getProperty(E_DC_S_NP_MAX_USERS);
		if (maxPlayersP)
		{
			maxplayers  = maxPlayersP->mCurrentValue;
//			TNL::logprintf("maxPS:%d | maxPC:%d",maxPlayersP->mLastServerValue,maxPlayersP->mCurrentTime);
			return maxplayers;
		}
	}
	return true;
}