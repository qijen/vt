#include "xNetInterface.h"
#include "vtConnection.h"



#include "IDistributedClasses.h"
#include "xDistributedBaseClass.h"
#include "xDistributed3DObjectClass.h"

#include "xDistributed3DObject.h"
#include "xDistributedClient.h"
#include "IDistributedObjectsInterface.h"
#include "IMessages.h"


#include "xDistributedProperty.h"
#include "xDistributedPropertyInfo.h"


#include "ISession.h"
#include "xDistributedSession.h"
#include "xDistributedSessionClass.h"
#include "xLogger.h"
#include "xNetConstants.h"

void
xNetInterface::setFixedRateParameters(TNL::U32 minPacketSendPeriod, TNL::U32 minPacketRecvPeriod, TNL::U32 maxSendBandwidth, TNL::U32 maxRecvBandwidth,bool global/* =false */)
{

	if (minPacketRecvPeriod < 10)
	{
		minPacketRecvPeriod =10;
	}
	if (minPacketSendPeriod < 10)
	{
		minPacketSendPeriod = 10;
	}

	if (maxRecvBandwidth < 100 )	{		maxRecvBandwidth = 100;	}
	if (maxRecvBandwidth > 5000 )	{		maxRecvBandwidth = 5000;	}

	if (maxSendBandwidth< 100 )	{		maxSendBandwidth = 100;	}
	if (maxSendBandwidth > 5000 )	{		maxSendBandwidth = 5000;	}
    
	if (!IsServer())
	{
		if (getConnection())
		{
			getConnection()->setFixedRateParameters(minPacketSendPeriod,minPacketRecvPeriod,maxSendBandwidth,maxRecvBandwidth);
		}
	}

	if (global && IsServer())
	{

		TNL::Vector<TNL::NetConnection* > con_list = getConnectionList();
		for(int i = 0; i < con_list.size(); i++)
		{
			vtConnection *con = (vtConnection *)con_list[i];
			con->setFixedRateParameters(minPacketSendPeriod,minPacketRecvPeriod,maxSendBandwidth,maxRecvBandwidth);
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
void
xNetInterface::enableLogLevel(int type,int verbosity,int value)
{
	xLogger::GetInstance()->enableLoggingLevel(type,verbosity,value);
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
xNetInterface::initBaseClasses(int flags)
{

	IDistributedClasses *cInterface = getDistributedClassInterface();
	//////////////////////////////////////////////////////////////////////////
	//dist class 

	xDistributedSessionClass*classTemplate = (xDistributedSessionClass*)cInterface->get("Session Class");
	//if (!classTemplate)
	//	classTemplate = (xDistributedSessionClass*)cInterface ->createClass("Session Class",E_DC_BTYPE_SESSION);

	/*
	classTemplate->addProperty(E_DC_S_NP_MAX_USERS,E_PTYPE_RELIABLE);
	classTemplate->addProperty(E_DC_S_NP_PASSWORD,E_PTYPE_RELIABLE);
	classTemplate->addProperty(E_DC_S_NP_TYPE,E_PTYPE_RELIABLE);
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
void xNetInterface::updateDistributedObjects( int flags,float deltaTime )
{


	IDistributedObjects *doInterface  = getDistObjectInterface();
	xDistributedObjectsArrayType *distObjects = getDistributedObjects();
	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();
	while (begin!=end)
	{
		xDistributedObject* dobj = *begin;
		if (dobj)
		{
			xDistributedClass *_class = dobj->getDistributedClass();

			if (_class)
			{
				if (_class->getEnitityType() == E_DC_BTYPE_3D_ENTITY )
				{
					xDistributed3DObject *dobj3D = static_cast<xDistributed3DObject*>(dobj);
					if (dobj3D)
					{
						dobj3D->update(deltaTime);
					}
					
				}
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
void xNetInterface::createScopeObject()
{
    return;
	if (!IsServer())
	{
/*		vtConnection *con  = getConnection();
		if (con)
		{
			con->scopeObject = new xDistributedObject();
			con->scopeObject->SetObjectFlags(E_DO_CREATION_COMPLETE);
			con->scopeObject->SetEntityID(-1);
			con->scopeObject->SetServerID(-1);
			con->scopeObject->SetCreationTime(0.0f);
			con->scopeObject->SetName("Client");

			vtDistributedObjectsArrayType *distObjects = getDistributedObjects();
			distObjects->PushBack(con->scopeObject);
			con->setScopeObject(con->scopeObject);
			logprintf("scope object created");
		}
	*/
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
void xNetInterface::setConnection(vtConnection* val)
{
	
	if (!IsServer() && getConnection())
	{
		getConnection()->disconnect("asdasd");
	}
	if (!IsServer())
	{
		connectionToServer = val;
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
bool xNetInterface::isValid()
{

	if (getConnection())
	{
		if (getConnection()->getConnectionState() == TNL::NetConnection::Connected )
		{
			return true;
		}
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
xNetInterface::xNetInterface(bool server, const TNL::Address &bindAddress, const TNL::Address &pingAddr) : TNL::NetInterface(bindAddress)
{
	
	m_IsServer       = server;
	pingingServers = !server;
	lastPingTime   = 0;
	pingAddress    = pingAddr;
	elapsedConnectionTime = 0.0f;
	connectionTimeOut = 2000.0f;
	connectionInProgress = false;
	m_isConnected = false;
	m_DistributedClassInterface  = new IDistributedClasses(this);
		getDistributedClassInterface()->createClass("CLIENT CLASS",E_DC_BTYPE_CLIENT);


	m_DOCounter =0;

	m_DistObjectInterface = new IDistributedObjects(this);
	m_DistributedObjects  =  new xDistributedObjectsArrayType();

	m_ObjectUpdateCounter  = 0 ;
	m_NetworkMessages = new xNetworkMessageArrayType();
	m_IMessages = new IMessages(this);
	mCheckObjects  = false;
	connectionToServer   = NULL;
	
	
	mISession = new ISession(this);
	mSessions  = new xSessionArrayType();
	mCurrentSession  = NULL;
	mMyUserID = -1;
	mMyClient = NULL;

	mMessages = new xMessageArrayType();

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
void xNetInterface::destroy()
{

	//////////////////////////////////////////////////////////////////////////
	//disconnect : 
	if (IsServer())
	{
		TNL::Vector<TNL::NetConnection* > con_list = getConnectionList();
		for(int i = 0; i < con_list.size(); i++)
		{
			vtConnection *con = (vtConnection *)con_list[i];
			con->disconnect("destroyed");
		}
	}else
	{
		if (connectionToServer)
		{
			connectionToServer->disconnect("destroyed");
			delete connectionToServer;
			connectionToServer = NULL;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//cleanup dist objects : 
	if (m_DistObjectInterface!=NULL)
	{
		m_DistObjectInterface->Destroy();
		delete m_DistObjectInterface;
		m_DistObjectInterface = NULL;
	}
	if (m_DistributedObjects!=NULL)
	{
		m_DistributedObjects->clear();
		delete m_DistributedObjects;
		m_DistributedObjects = NULL;
	}
	if (m_DistributedClassInterface)
	{
		m_DistributedClassInterface->getDistrutedClassesPtr()->clear();
		m_DistributedClassInterface->destroy();
		delete m_DistributedClassInterface;
		m_DistributedClassInterface = NULL;
	}
	if (getMessagesInterface())
	{
		getMessagesInterface()->deleteAllOldMessages();

		if (mMessages)
		{
			delete mMessages;
			mMessages = NULL;
		}
	}

	mMyClient = NULL;
	mMyUserID = -1;
	mInterfaceFlags  = 0 ;

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
void xNetInterface::sendPing(TNL::Address addr,int type)
{
	TNL::PacketStream writeStream;

	writeStream.write(TNL::U8(type));
	writeStream.sendto(mSocket, addr);
	xLogger::xLog(ELOGINFO,XL_START,"%s - sending ping ", addr.toString());

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
void xNetInterface::sendPing()
{
	TNL::PacketStream writeStream;
	writeStream.write(TNL::U8(GamePingRequest));
	writeStream.sendto(mSocket, pingAddress);
	xLogger::xLog(ELOGINFO,XL_START,"%s - sending ping ", pingAddress.toString());
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
void xNetInterface::tick()
{
	TNL::U32 currentTime = TNL::Platform::getRealMilliseconds();

	if(pingingServers && (lastPingTime + PingDelayTime < currentTime))
	{
		lastPingTime = currentTime;
	}


	float timeDelta = (currentTime - mLastTime) / 1000.f;
	mLastTime = (float)currentTime;
	mDeltaTime  = mDeltaTime + timeDelta;

	if (!IsServer())
	{
		if (getConnection() && isValid())
		{
			if (getCurrentThresholdTicker() < 50)
			{
				calculateUpdateCounter();
			}
			mLastSendTime  = getConnection()->getOneWayTime();
			mLastRoundTripTime = getConnection()->getRoundTripTime();
		}
	}
	

	checkIncomingPackets();
	processConnections();

	if (getCheckObjects())
	{
		checkObjects();
		setCheckObjects(false);
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
void xNetInterface::advanceTime(xTimeType timeNow,float deltaTime)
{

	//return;
	mLastTime2 = timeNow;
	mLastDeltaTime2 = deltaTime;

	mCurrentThresholdTicker +=getCurrentThresholdTicker() +deltaTime;
	if (getCurrentThresholdTicker() > 50)
	{
		mCurrentThresholdTicker = 0.0f;
	}

	IDistributedObjects *doInterface  = getDistObjectInterface();
	xDistributedObjectsArrayType *distObjects = getDistributedObjects();
	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();

	while (begin!=end)
	{
		xDistributedObject* dobj = *begin;
		xDistributedClass *_class = dobj->getDistributedClass();
		if (_class && dobj )
		{
			if (_class->getEnitityType() != E_DC_BTYPE_CLIENT )
			{
				xDistributedPropertyArrayType &props = *dobj->getDistributedPorperties();
				for (unsigned int i = 0  ;  i < props.size() ; i++ )
				{
					xDistributedProperty *prop  = props[i];
					prop->advanceTime(mLastTime2,mLastDeltaTime2);
				}
			}
			
		}
		begin++;
	}

		
	getMessagesInterface()->advanceTime(deltaTime);


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
void xNetInterface::handleInfoPacket(const TNL::Address &address, TNL::U8 packetType, TNL::BitStream *stream)
{
		
	
	if (IsServer())
	{
		TNL::PacketStream writeStream;
		if(packetType == GamePingRequest && IsServer())
		{
			writeStream.write(TNL::U8(GamePingResponse));
			writeStream.sendto(mSocket, address);
			xLogger::xLog(ELOGINFO,E_LI_CONNECTION,"Server : %s - sending ping response.", address.toString());
		}
		if(packetType == ScanPingRequest && IsServer())
		{
			writeStream.write(TNL::U8(ScanPingResponse));
			writeStream.sendto(mSocket, address);
			xLogger::xLog(ELOGINFO,E_LI_CONNECTION,"Server : %s - sending scan ping response.", address.toString());
		}
		else if(packetType == GamePingResponse && pingingServers )
		{
			
			xLogger::xLog(ELOGINFO,E_LI_CONNECTION,"Client : %s - received ping response.", address.toString());
		
			vtConnection *conn  = new vtConnection();
			conn->connect(this, address,true,false); // connect to the server through the game's network interface
			setConnection(conn);
			conn->m_NetInterface = this;
			conn->setInterface(this);
			xLogger::xLog(ELOGINFO,E_LI_CONNECTION,"Connecting to server: %s", address.toString());

			pingingServers = false;
		}
		else if(packetType == ScanPingResponse )
		{
			xLogger::xLog(ELOGINFO,E_LI_CONNECTION,"Client : %s - received scan ping response.", address.toString());
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
void 
xNetInterface::processPacket(const TNL::Address &sourceAddress, TNL::BitStream *pStream)
{
	Parent::processPacket(sourceAddress,pStream);

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
xNetInterface::removeClient(int clientID)
{

	if(IsServer())
	{
		IDistributedObjects *doInterface  = getDistObjectInterface();
		if (!doInterface)return;
		xDistributedObjectsArrayType *distObjects = getDistributedObjects();
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
						if (distClient)
						{
							if (distClient->getUserID()  == clientID )
							{
								delete distClient;
								return;
							}
						}
					}
				}
			}
			begin++;
		}
	}else
	{
		xClientInfo *clientInfo   = new xClientInfo();
		clientInfo->userFlag = USER_DELETED;
		clientInfo->userID= clientID;
		getClientInfoTable().push_back(clientInfo);
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
xNetInterface::addDeleteObject(xDistributedObject* object)
{

	if(!IsServer() && object)
	{

		xDistDeleteInfo* dinfo = new xDistDeleteInfo();
		dinfo->entityID  = object->getEntityID();
		dinfo->serverID = object->getServerID();
		dinfo->deleteState = E_DO_DS_DELETED ;
		getDistDeleteTable().push_back(dinfo);
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
xNetInterface::writeDistributedProperties(TNL::BitStream *bstream)
{

	if(IsServer()) return;

	IDistributedObjects *doInterface  = getDistObjectInterface();
	if (!doInterface)return;
	xDistributedObjectsArrayType *distObjects = getDistributedObjects();
	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();
	if (getObjectUpdateCounter())
	{
		bstream->writeFlag(E_C_CLIENT_OBJECT_UPDATE);
		bstream->writeRangedU32(getObjectUpdateCounter(), 0, 63 );
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
IDistributedClasses*xNetInterface::getDistributedClassInterface(){return m_DistributedClassInterface;}

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
xNetInterface::calculateUpdateCounter()
{
	if (IsServer())
	{
		return;
	}
	IDistributedObjects *doInterface  = getDistObjectInterface();
	xDistributedObjectsArrayType *distObjects = getDistributedObjects();
	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();
	
	setObjectUpdateCounter(0);


	while (begin!=end)
	{
		xDistributedObject* dobj = *begin;
		xDistributedClass *_class = dobj->getDistributedClass();
		if (_class && dobj )
		{
			
			if (_class->getEnitityType() != E_DC_BTYPE_CLIENT )
			{
				xDistributedPropertyArrayType &props = *dobj->getDistributedPorperties();
				for (unsigned int i = 0  ;  i < props.size() ; i++ )
				{
					xDistributedProperty *prop  = props[i];
					int blockIndex =  prop->getBlockIndex();
					dobj->getUpdateBits().set(BIT(blockIndex),prop->getFlags() & E_DP_NEEDS_SEND );
				}
			}

			if (_class->getEnitityType() == E_DC_BTYPE_CLIENT )
			{
				xDistributedClient *client  = (xDistributedClient*)dobj;
				client->calculateUpdateBits();

			}

			if (dobj->getUpdateBits().getMask())
			{
				getObjectUpdateCounter()++;
				dobj->setUpdateState(E_DO_US_PENDING);
			}else
			{
				dobj->setUpdateState(E_DO_US_OK);
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
int xNetInterface::getNumConnections()
{
	int i  = 0 ; 
	TNL::Vector<TNL::NetConnection* > con_list = getConnectionList();
	for(i = 0; i < con_list.size(); i++)
	{
	}
	return i;
}

void
xNetInterface::checkConnections()
{
	/*if (getNumConnections()==0)
	{
		getDistObjectInterface()->removeAll(getDistributedObjects());
		//xLogger::xLog(ELOGINFO,XL_START,"no clients anymore: remove all objects!");
	}*/
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
void xNetInterface::disconnect(int connectionID)
{
	if (IsServer())return;

	if (getConnection())
	{
		getConnection()->disconnect("no reason");
		setConnected(false);
		if (connectionToServer)
		{
			delete connectionToServer;
			connectionToServer = NULL;
		}

	}
	getDistObjectInterface()->removeAll(getDistributedObjects());
	getClientInfoTable().clear();

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
int xNetInterface::getNumDistributedObjects(int templateType)
{

	int counter = 0 ; 
	IDistributedObjects *doInterface  = getDistObjectInterface();

	xDistributedObjectsArrayType *distObjects = getDistributedObjects();
	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();


	while (begin!=end)
	{
		xDistributedObject* dobj = *begin;
		xDistributedClass *_class = dobj->getDistributedClass();
		if (_class && dobj )
		{
			if (_class->getEnitityType() == templateType )
			{
				counter ++;
			}
		}
		begin++;
	}
	return counter;
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
xNetInterface::checkObjects()
{
	
	xLogger::xLog(ELOGINFO,E_LI_CONNECTION,"Server makes an object validation");
	if (getConnectionList().size() ==0)
	{
		getDistObjectInterface()->removeAll(getDistributedObjects());
		getDistributedClassInterface()->getDistrutedClassesPtr()->clear();
		xLogger::xLog(ELOGINFO,E_LI_CONNECTION,"Server : No clients anymore, ..delete all");
	
	}
	checkSessions();
	
	setCheckObjects(false);
}


void xNetInterface::checkSessions()
{

	xLogger::xLog(ELOGINFO,E_LI_SESSION,"Server : Looking for invalid sessions ");
	IDistributedObjects *doInterface  = getDistObjectInterface();
	xDistributedObjectsArrayType *distObjects = getDistributedObjects();
	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();

	while (begin!=end)
	{
		xDistributedObject* dobj = *begin;
		xDistributedClass *_class = dobj->getDistributedClass();
		if (_class && dobj )
		{
			if (_class->getEnitityType() == E_DC_BTYPE_SESSION )
			{

				xDistributedSession *session = (xDistributedSession*)dobj;
				xDistributedClient *client  = (xDistributedClient*)doInterface->getByUserID(dobj->getUserID(),E_DC_BTYPE_CLIENT);
				if (!client && session )
				{
					xLogger::xLog(ELOGWARNING,E_LI_SESSION,"Server : Found invalid session object %s : deleting",session->GetName().getString());
					ISession *sInterface =  getSessionInterface();
					sInterface->deleteSession(session->getSessionID());
					begin =distObjects->begin();
					return;

					//continue;
				}
			}
		}
		begin++;
	}
	setCheckObjects(false);
}

void
xNetInterface::printObjects(int type, int flags)
{
	switch(type)
	{
	case E_OT_DIST_OBJECT:
		{

			xLogger::xLog(ELOGINFO,E_LI_CPP,"Print all Objects : Num : %d",getDistributedObjects()->size());
			xDistributedObjectsArrayType *distObjects = getDistributedObjects();
			xDistObjectIt begin = distObjects->begin();
			xDistObjectIt end = distObjects->end();
			while (begin!=end)
			{
				xDistributedObject* dobj = *begin;
				if (dobj)
				{
					char out[4096];
					//xDistributedClass *_class  = dobj->getDistributedClass();
					TNL::BitSet32 opflags = xBIT(E_OPF_NAME);

					xBitSet stylePrintFlags = 0;
					stylePrintFlags.set(E_PSF_PRINT_LOG_TYPE,true);
					stylePrintFlags.set(E_PSF_PRINT_COMPONENT,true);
					//enableFlag(stylePrintFlags,E_PSF_PRINT_LOG_TYPE); 
					//enableFlag(stylePrintFlags,E_PSF_PRINT_COMPONENT);

					enableFlag(opflags,E_OPF_NAME);
					enableFlag(opflags,E_OPF_GHOST_ID);
					/*
					enableFlag(opflags,E_OPF_USER_ID);
					enableFlag(opflags,E_OPF_SESSION_ID);
					enableFlag(opflags,E_OPF_CLASS);*/

					uxString objString  = dobj->print(opflags);
					//sprintf(out,"%s | usrID:%d | serverID:%d | sessionID:%d | class:%s | props :%d",dobj->GetName().getString(),dobj->getUserID(),dobj->getServerID(),dobj->getSessionID,_class ? _class->getClassName().getString() : "NOCLASS!",dobj->getDistributedPorperties()->size() );
					//TNL::logprintf("\nsessionID:%d",dobj->getSessionID());
					//xLogger::xLogExtro(0,"%s",objString.CStr());
					xLogger::xLog(stylePrintFlags,ELOGINFO,E_LI_CPP,"%s",objString.CStr());
					/*switch(_class->getEnitityType())
					{

						case E_DC_BTYPE_CLIENT : 
						{
							xDistributedClient *sobj = (xDistributedClient*)dobj;
							if (sobj)
							{

								int gIndex = dobj->getOwnerConnection() ? dobj->getOwnerConnection()->getGhostIndex(dobj) : getConnection()->getGhostIndex(dobj);
								xLogger::xLogExtro(0,"\n\t  -->UserName : %s \n\tSessionID:%d \n\tIsJoined:%d \n\tGhostIndex:%d",sobj->getUserName().getString(),sobj->getSessionID(),isFlagOn(sobj->getClientFlags(),E_CF_SESSION_JOINED) ? 1 : 0,gIndex  );
								//xLogger::xLogExtro(0,"\n\t  -->HasAddingFlag:%d",sobj->getUserName().getString(),sobj->getLocalAddress().getString(),sobj->getSessionID(),isFlagOn(sobj->getClientFlags(),E_CF_ADDING) ? 1 : 0);
								//xLogger::xLogExtro(0,"\n\t  -->HasAddedFlag:%d",sobj->getUserName().getString(),sobj->getLocalAddress().getString(),sobj->getSessionID(),isFlagOn(sobj->getClientFlags(),E_CF_ADDED) ? 1 : 0);
							}
						}break;
						//////////////////////////////////////////////////////////////////////////
						case E_DC_BTYPE_SESSION: 
						{
							xDistributedSession*sobj = (xDistributedSession*)dobj;
							if (sobj)
							{
								xLogger::xLogExtro(0,"\n\t  -->Session ID : %d \tLocked:%d \tNumUsers %d \tPrivate : %d  \tMaster :%d \n\tFull:%d \n\tMaxPlayers:%d",sobj->getSessionID(),sobj->isLocked(),sobj->getNumUsers(),sobj->isPrivate(),sobj->getUserID(),sobj->isFull(),sobj->getMaxUsers()  );
								sobj->
							}
						}break;
					}*/
				}
				begin++;
			}
		}
		break;

	case E_OT_CLASS:
		{

			xLogger::xLog(ELOGINFO,E_LI_CPP,"Print all Classes : Num : %d ",getDistributedClassInterface()->getNumClasses());

			xDistributedClassesArrayType *_classes = getDistributedClassInterface()->getDistrutedClassesPtr();
			xDistClassIt begin = _classes->begin();
			xDistClassIt end = _classes->end();
			while (begin!=end)
			{
				xDistributedClass *_class = begin->second;
				if (_class)
				{
					char out[4096];
					sprintf(out,"\t  %s | type :%d | props : %d",_class->getClassName().getString(),_class->getEnitityType(),_class->getDistributedProperties()->size() );
					xLogger::xLogExtro(0,"CLASS : %s",out);
				}
				begin++;
			}
		}
	}



	IMessages *mInterface = getMessagesInterface();
	xLogger::xLog(ELOGINFO,E_LI_CPP,"Print Messages : Num : %d",mInterface->getMessages()->size());


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
xNetInterface::deploySessionClasses(TNL::GhostConnection *connection)
{

	IDistributedClasses *cInterface  = getDistributedClassInterface();
	xDistributedClassesArrayType *_classes = cInterface->getDistrutedClassesPtr();
	xDistClassIt begin = _classes->begin();
	xDistClassIt end = _classes->end();

	while (begin!=end)
	{
		xDistributedClass *_class = begin->second;
		if (_class && _class->getEnitityType() == E_DC_BTYPE_SESSION )
		{

			TNL::StringPtr className(_class->getClassName().getString());
		
			xLogger::xLog(ELOGINFO,E_LI_DISTRIBUTED_CLASS_DESCRIPTORS,"\n deploying session class %s ",_class->getClassName().getString());
			TNL::Int<16>entityType  = _class->getEnitityType();

			TNL::Vector<TNL::StringPtr>propertyNames;
			TNL::Vector<TNL::Int<16> >nativeTypes;
			TNL::Vector<TNL::Int<16> >valueTypes;
			TNL::Vector<TNL::Int<16> >predictionTypes;

			xDistributedPropertiesListType &props  = *_class->getDistributedProperties();
			for (unsigned int i  = 0 ; i < props.size() ; i ++ )
			{
				xDistributedPropertyInfo *dInfo  = props[i];
				propertyNames.push_back( TNL::StringPtr(dInfo->mName) );
				nativeTypes.push_back(dInfo->mNativeType);
				valueTypes.push_back(dInfo->mValueType);
				predictionTypes.push_back(dInfo->mPredictionType);
			}
			((vtConnection*)connection)->s2cDeployDistributedClass(className,entityType,propertyNames,nativeTypes,valueTypes,predictionTypes);
		}
		begin++;
	}
}