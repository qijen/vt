#include "vtConnection.h"
#include "xNetInterface.h"


#include "xDistributed3DObject.h"
#include "xDistributedClient.h"
#include "xDistributedProperty.h"
#include "xDistributedPropertyInfo.h"

#include "IDistributedObjectsInterface.h"

#include "IDistributedClasses.h"
#include "xDistributedBaseClass.h"
#include "xDistributed3DObjectClass.h"
#include "xDistTools.h"

#include "xDistributedSession.h"
#include "xDistributedSessionClass.h"
#include "xLogger.h"


TNL_IMPLEMENT_NETCONNECTION(vtConnection, TNL::NetClassGroupGame, true);
int userIDCounter = 0;
//extern int sessionIDCounter;

#include "ISession.h"
#include "xDistributedSession.h"





TNL_IMPLEMENT_RPC(vtConnection, s2cAddServerLogEntry, 
				  (TNL::StringPtr entry),
				  (entry),
				  NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirServerToClient, 0)
{


}

TNL_IMPLEMENT_RPC(vtConnection, c2sDeleteSession_RPC, 
				  (TNL::Int<16>sessionID),
				  (sessionID),
				  NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirClientToServer, 1)
{
	xNetInterface *nInterface = static_cast<xNetInterface *>(getInterface());
	IDistributedClasses *cInterface = nInterface->getDistributedClassInterface();
	ISession *sInterface =  nInterface->getSessionInterface();
	IDistributedObjects *oInterface  = nInterface->getDistObjectInterface();

	//xLogger::xLog(ELOGINFO,XL_START,"server : deleting session%d",sessionID);
	xDistributedSession *session = sInterface->get(sessionID);

	if (!session)
	{
		xLogger::xLog(ELOGERROR,XL_START,"client : leaving user %d  fails : session doesn't exists",getUserID());
	}
	if (session)
	{

		if (!session->isClientJoined(getUserID()))
		{
			xLogger::xLog(ELOGINFO,XL_START,"client : deleting session fails, user not joined :%d ",getUserID());
			return;
		}
		
		oInterface->deleteObject(session->getServerID());
		
		//xLogger::xLog(ELOGINFO,XL_START,"client : user %d left session : %d ",client->getUserID(),session->getSessionID());
		/*client->setSessionID(-1);
		client->getClientFlags().set(1 << E_CF_SESSION_JOINED,false);
		client->getClientFlags().set(1 << E_CF_DELETING,true);
		client->setUserFlags(USER_DELETED);*/
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
TNL_IMPLEMENT_RPC(vtConnection, c2sLockSession_RPC, 
				  (TNL::Int<16>sessionID),
				  (sessionID),
				  NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirClientToServer, 1)
{
	xNetInterface *nInterface = static_cast<xNetInterface *>(getInterface());
	IDistributedClasses *cInterface = nInterface->getDistributedClassInterface();
	ISession *sInterface =  nInterface->getSessionInterface();
	IDistributedObjects *oInterface  = nInterface->getDistObjectInterface();

	/*
	xDistributedClient *client  = (xDistributedClient*)oInterface->getByUserID(userID,E_DC_BTYPE_CLIENT);
	if (!client)
	{
		xLogger::xLog(ELOGINFO,XL_START,"server : leaving user %d  fails : client doesn't exists",userID);

	}
	if (client)
	{
		xDistributedSession *session = sInterface->get(sessionID);
		if (session)
		{

			if (!session->isClientJoined(userID))
			{
				xLogger::xLog(ELOGINFO,XL_START,"server : leaving user fails :%d on session : %d , user not joined",client->getUserID(),session->getSessionID());
				return;
			}

			session->removeUser(client);
			client->setSessionID(-1);
			setSessionID(-1);
			client->getClientFlags().set(1 << E_CF_SESSION_JOINED,false);

			xLogger::xLog(ELOGINFO,XL_START,"server : user %d left session %d",userID,sessionID);

			TNL::Vector<TNL::NetConnection* > con_list = getInterface()->getConnectionList();
			for(int i = 0; i < con_list.size(); i++)
			{
				vtConnection *con = (vtConnection *)con_list[i];
				con->s2cUserLeftSession_RPC(client->getUserID(),sessionID);
			}
		}
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
TNL_IMPLEMENT_RPC(vtConnection, s2cUserLeftSession_RPC, 
				  (TNL::Int<16>userID,TNL::Int<16>sessionID),
				  (userID,sessionID),
				  NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirServerToClient, 0)
{
	xNetInterface *nInterface = static_cast<xNetInterface *>(getInterface());
	IDistributedClasses *cInterface = nInterface->getDistributedClassInterface();
	ISession *sInterface =  nInterface->getSessionInterface();
	IDistributedObjects *oInterface  = nInterface->getDistObjectInterface();


	if (!nInterface->getCurrentSession())
	{
		xLogger::xLog(ELOGERROR,E_LI_SESSION,"Client : leaving user %d  failed : You not joined any Session",userID);
	}


	xDistributedClient *client  = (xDistributedClient*)oInterface->getByUserID(userID,E_DC_BTYPE_CLIENT);
	if (!client)
	{
		xLogger::xLog(ELOGERROR,E_LI_SESSION,"Removing user %d  from session %d failed : client object doesn't exists",userID,sessionID);
	}


	if (client)
	{
		xDistributedSession *session = sInterface->get(sessionID);
		if (!session)
		{
			xLogger::xLog(ELOGERROR,E_LI_SESSION,"Removing user %d  from session %d failed : session doesn't exists",userID,sessionID);
		}
		if (session)
		{
			
			if (!session->isClientJoined(userID))
			{
				
				xLogger::xLog(ELOGERROR,E_LI_SESSION,"Removing user %d  from session %d failed : \n\t %s",userID,sessionID,"user not present in this session !");
				return;
			}

			session->removeUser(client);


			//xLogger::xLog(ELOGINFO,XL_START,"client : user %d left session : %d ",client->getUserID(),session->getSessionID());
			xLogger::xLog(ELOGINFO,E_LI_SESSION,"Removed user %d  from session %d !",userID,sessionID);

			client->setSessionID(-1);
			disableFlag(client->getClientFlags(),E_CF_SESSION_JOINED);
			enableFlag(client->getClientFlags(),E_CF_DELETING);

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
TNL_IMPLEMENT_RPC(vtConnection, c2sLeaveSession_RPC, 
				  (TNL::Int<16>userID,TNL::Int<16>sessionID,TNL::Int<16>destroy),
				  (userID,sessionID,destroy),
				  NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirClientToServer, 1)
{

	xNetInterface *nInterface = static_cast<xNetInterface *>(getInterface());
	IDistributedClasses *cInterface = nInterface->getDistributedClassInterface();
	ISession *sInterface =  nInterface->getSessionInterface();
	IDistributedObjects *oInterface  = nInterface->getDistObjectInterface();

	xDistributedClient *client  = (xDistributedClient*)oInterface->getByUserID(userID,E_DC_BTYPE_CLIENT);
	if (client)
	{
		xDistributedSession *session = sInterface->get(sessionID);
		if (session)
		{
		
			if (getUserID()!=session->getUserID() && getUserID()!=userID )
			{
				xLogger::xLog(ELOGERROR,E_LI_SESSION,"Server : removing user %d from session :%d failed ! :  you must be session master ! ",client->getUserID(),session->getSessionID());
				return;
			}

			if (!session->isClientJoined(userID))
			{
				xLogger::xLog(ELOGERROR,E_LI_SESSION,"Server : leaving user fails :%d on session : %d , user not joined",client->getUserID(),session->getSessionID());
				return;
			}

			session->removeUser(client);
			client->setSessionID(-1);
			setSessionID(-1);
            disableFlag(client->getClientFlags(),E_CF_SESSION_JOINED);
			
			//xLogger::xLog(ELOGINFO,XL_START,"server : user %d left session %d",userID,sessionID);
			xLogger::xLog(ELOGINFO,E_LI_SESSION,"Server : user %d left session %d",userID,sessionID);

			TNL::Vector<TNL::NetConnection* > con_list = getInterface()->getConnectionList();
			for(int i = 0; i < con_list.size(); i++)
			{
				vtConnection *con = (vtConnection *)con_list[i];
				con->s2cUserLeftSession_RPC(client->getUserID(),sessionID);
			}
			
			if (destroy && getUserID()==session->getUserID() )
			{
				xLogger::xLog(ELOGINFO,E_LI_SESSION,"Server : session master %d left !, deleting session %d",userID,sessionID);
				sInterface->deleteSession(session->getSessionID());
				//sessionIDCounter--;
			}
			
		}
		else
		{
			xLogger::xLog(ELOGERROR,E_LI_SESSION,"Server : joining user %d on session : %d failed :\n\t		\
											 , couldn't find session object !",userID,sessionID);
		}
	}else
	{
		xLogger::xLog(ELOGERROR,E_LI_SESSION,"Server : joining user %d on session : %d failed :\n\t		\
											, couldn't find client object ",userID,sessionID);
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
TNL_IMPLEMENT_RPC(vtConnection, c2sJoinSession_RPC, 
				  (TNL::Int<16>userID,TNL::Int<16>sessionID,TNL::StringPtr password),
				  (userID,sessionID,password),
				  NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirClientToServer, 1)
{
	xNetInterface *nInterface = static_cast<xNetInterface *>(getInterface());
	IDistributedClasses *cInterface = nInterface->getDistributedClassInterface();
	ISession *sInterface =  nInterface->getSessionInterface();
	IDistributedObjects *oInterface  = nInterface->getDistObjectInterface();
    
	xDistributedClient *client  = (xDistributedClient*)oInterface->getByUserID(userID,E_DC_BTYPE_CLIENT);
	if (client)
	{
		xDistributedSession *session = sInterface->get(sessionID);
		if (session)
		{

			if (session->isPrivate())
			{
				
				if ( strcmp(session->getPassword().getString() , password.getString()  ) )
				{
					//xLogger::xLog(ELOGINFO,XL_START,"server : joining user fails :%d on session : %d , password incorrect",client->getUserID(),session->getSessionID());
					xLogger::xLog(ELOGERROR,E_LI_SESSION,"Server : joining user %d on session : %d failed :\n\t , password incorrect",client->getUserID(),session->getSessionID());
					return;
				}
			}
			
			if (session->isLocked())
			{
				xLogger::xLog(ELOGWARNING,E_LI_SESSION,"Server : joining user %d on session : %d failed :, session locked",client->getUserID(),session->getSessionID());
				return;

			}
			if (session->isClientJoined(userID))
			{
				xLogger::xLog(ELOGWARNING,E_LI_SESSION,"Server : joining user %d on session %d failed : \n\t	 user already joined",client->getUserID(),session->getSessionID());
				return;
			}
			session->addUser(client);
			TNL::Vector<TNL::NetConnection* > con_list = getInterface()->getConnectionList();
			for(int i = 0; i < con_list.size(); i++)
			{
				vtConnection *con = (vtConnection *)con_list[i];
				con->s2cUserJoinedSession_RPC(client->getUserID(),sessionID);
			}

			client->setSessionID(sessionID);
			setSessionID(sessionID);
			client->getClientFlags().set(1 << E_CF_SESSION_JOINED,true);

			//xLogger::xLog(ELOGINFO,XL_START,"server : joining user :%d on session : %d ",client->getUserID(),session->getSessionID());
			xLogger::xLog(ELOGINFO,E_LI_SESSION,"Server : user :%d on session %d joined",client->getUserID(),session->getSessionID());

		}else{
			xLogger::xLog(ELOGERROR,E_LI_SESSION,"Server : joining user %d on session : %d failed :\n\t , couldn't find session object !",userID,sessionID);
		}
	}else
	{
		xLogger::xLog(ELOGERROR,E_LI_SESSION,"Server : joining user %d on session : %d failed :\n\t	, couldn't find client object ",userID,sessionID);
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
TNL_IMPLEMENT_RPC(vtConnection, s2cUserJoinedSession_RPC, 
				  (TNL::Int<16>userID,TNL::Int<16>sessionID),
				  (userID,sessionID),
				  NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirServerToClient, 0)
{
	xNetInterface *nInterface = static_cast<xNetInterface *>(getInterface());
	IDistributedClasses *cInterface = nInterface->getDistributedClassInterface();
	ISession *sInterface =  nInterface->getSessionInterface();
	IDistributedObjects *oInterface  = nInterface->getDistObjectInterface();

	
	xDistributedClient *client  = (xDistributedClient*)oInterface->getByUserID(userID,E_DC_BTYPE_CLIENT);
	if (client)
	{
		xDistributedSession *session = sInterface->get(sessionID);
		if (session)
		{
			xLogger::xLog(XL_START,ELOGINFO,E_LI_SESSION,"session user size : %d ",session->getClientTable().size());
		}
		if (session && !session->isClientJoined(client->getUserID()))
		{
			session->addUser(client);
			
			client->getClientFlags().set(1 << E_CF_SESSION_JOINED,true);
			client->getClientFlags().set(1 << E_CF_ADDING,true);
			client->getClientFlags().set(1 << E_CF_SESSION_DESTROYED,false);
			client->setSessionID(sessionID);
		
			if (userID  == getUserID() )
			{
				nInterface->setCurrentSession(session);
				xLogger::xLog(XL_START,ELOGINFO,E_LI_SESSION,"we entered session, user size : %d ",session->getClientTable().size());



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
/*								if (session->isClientJoined(distClient->getUserID()))
								{
									xLogger::xLog(ELOGINFO,E_LI_SESSION,"client %d is in table",distClient->getUserID());
								}
								if (isFlagOn(distClient->getClientFlags(),E_CF_SESSION_JOINED))
								{
									xLogger::xLog(ELOGINFO,E_LI_SESSION,"client %d is has joined flag",distClient->getUserID());
								}
*/
								if (distClient && distClient->getSessionID() ==session->getSessionID() )
								{
									enableFlag(distClient->getClientFlags(),E_CF_SESSION_JOINED);
									enableFlag(distClient->getClientFlags(),E_CF_ADDING);
								}
							}
						}
					}
					begin++;
				}
			}

			xLogger::xLog(ELOGINFO,E_LI_SESSION,"Client : user :%d joined on session : %d ,table size : %d",client->getUserID(),session->getSessionID(),session->getClientTable().size());
		}
		/*xLogger::xLog(ELOGERROR,E_LI_SESSION,"Client : joining user %d on session %d failed : \n\t	\
											 Couldn't find Client object for user %d",client->getUserID(),session->getSessionID());*/
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
TNL_IMPLEMENT_RPC(vtConnection, c2sCreateSession_RPC, 
				  (TNL::StringPtr name,TNL::Int<16>type,TNL::Int<16>maxUsers,TNL::StringPtr password),
				  (name,type,maxUsers,password),
				  NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirClientToServer, 1)
{
	xNetInterface *nInterface = static_cast<xNetInterface *>(getInterface());
	IDistributedClasses *cInterface = nInterface->getDistributedClassInterface();
	
	
	//////////////////////////////////////////////////////////////////////////
	//dist class 

	xDistributedSessionClass *classTemplate = (xDistributedSessionClass*)cInterface->get(name.getString());
	if (!classTemplate)
	{
		xLogger::xLog(ELOGERROR,E_LI_SESSION,"Server : creating session %s by user %d failed : \n\t %s"		\
			,name.getString(),getUserID(),"Session class not found ");
		return;
	}

	xLogger::xLog(ELOGINFO,E_LI_SESSION,"Server : Deploying session class for session :%s",classTemplate->getClassName().getString());
	
	//////////////////////////////////////////////////////////////////////////
	TNL::Vector<TNL::NetConnection* > con_list = getInterface()->getConnectionList();
	for(int i = 0; i < con_list.size(); i++)
	{
		vtConnection *con = (vtConnection *)con_list[i];
		if (con->getUserID()  != getUserID() )
		{
			nInterface->deploySessionClasses(con);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//the distObject :
	IDistributedObjects *doInterface  = nInterface->getDistObjectInterface();
	xDistributedSession *distObject = (xDistributedSession*)doInterface->get( name.getString(),E_DC_BTYPE_SESSION);
	if (distObject)
	{
		xLogger::xLog(ELOGERROR,E_LI_SESSION,"Server :session %s already exists!",name.getString());
		return;
	}
	
	distObject = new xDistributedSession();
	distObject->setDistributedClass(classTemplate);
	distObject->setObjectFlags(E_DO_CREATION_COMPLETE);

	distObject->setEntityID(-1);
	distObject->setServerID(getGhostIndex(distObject));

	nInterface->setDOCounter(nInterface->getDOCounter()+1);
	distObject->SetName(name.getString());
	distObject->setNetInterface(nInterface);

    
	
	distObject->setUserID(getUserID());

	nInterface->getDistributedObjects()->push_back(distObject);
	distObject->initProperties();
	distObject->setMaxUsers(maxUsers);
	distObject->setPassword(password);

	((xDistributedSession*)distObject)->setPassword(password);

	float time = (float)Platform::getRealMilliseconds();
	int uid = getUserID();
	distObject->setCreationTime(time);
	
	xLogger::xLog(ELOGINFO,E_LI_SESSION,"Server : Client created session %s",name.getString());
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
TNL_IMPLEMENT_RPC(vtConnection, c2sDORequestOwnerShip, 
				  (TNL::S32 userID,TNL::Int<16>serverID),
				  (userID,serverID),
				  NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirClientToServer, 1)
{

	

	xNetInterface *nInterface = static_cast<xNetInterface *>(getInterface());
	IDistributedClasses *cInterface = nInterface->getDistributedClassInterface();

	//////////////////////////////////////////////////////////////////////////
	//the distObject :
	IDistributedObjects *doInterface  = nInterface->getDistObjectInterface();
	xDistributedObject *distObject = doInterface->get( serverID );
	if (distObject)
	{
		xLogger::xLog(ELOGINFO,XL_START,"user : %d is requesting ownership for object :%d|currentOwner:%d",getUserID(),serverID,distObject->getUserID());
		distObject->setUserID(userID);

	}
	

	if (distObject /*&& distObject->getUserID() != getUserID() */)
	{

		TNL::Vector<TNL::NetConnection* > con_list = getInterface()->getConnectionList();
		for(int i = 0; i < con_list.size(); i++)
		{
			vtConnection *con = (vtConnection *)con_list[i];
			con->s2cDOChangeOwnershipState(serverID,userID,E_DO_OS_OWNERCHANGED);
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
TNL_IMPLEMENT_RPC(vtConnection, s2cDOChangeOwnershipState, 
				  (TNL::Int<16>serverID,TNL::S32 newOwnerID,TNL::S32 state),
				  (serverID,newOwnerID,state),
				  NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirServerToClient, 0)
{

	

	xNetInterface *nInterface = static_cast<xNetInterface *>(getInterface());
	IDistributedClasses *cInterface = nInterface->getDistributedClassInterface();

	//////////////////////////////////////////////////////////////////////////
	//the distObject :
	IDistributedObjects *doInterface  = nInterface->getDistObjectInterface();
	xDistributedObject *distObject = doInterface->get( serverID );
	if (distObject)
	{
		xLogger::xLog(ELOGINFO,XL_START,"user. %d : ownerchange retrieved for obj :%d from user %d",getUserID(),serverID,newOwnerID );
		
		//distObject->getOwnershipState().set( 1<<E_DO_OS_OWNERCHANGED ,distObject->getUserID() !=newOwnerID );
		distObject->getOwnershipState().set( 1<<E_DO_OS_OWNERCHANGED ,true);
		distObject->setUserID(newOwnerID);
		distObject->getOwnershipState().set( 1<<E_DO_OS_OWNER,  distObject->getUserID() == getUserID() );
		distObject->getOwnershipState().set( 1 << E_DO_OS_BIND , !distObject->isOwner()  );
        distObject->getOwnershipState().set( 1 << E_DO_OS_RELEASED, newOwnerID  ? true  : false ); 
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
void vtConnection::writePacket(TNL::BitStream *bstream,TNL::NetConnection::PacketNotify*notify)
{
	xNetInterface *nInterface = static_cast<xNetInterface *>(getInterface());

	if(isConnectionToServer())
	{
		U32 firstSendIndex = highSendIndex[0];
		if(firstSendIndex < firstMoveIndex)
			firstSendIndex = firstMoveIndex;

		bstream->write(getControlCRC());

		bstream->write(firstSendIndex);
		U32 skipCount = firstSendIndex - firstMoveIndex;
		U32 moveCount = 0 - skipCount;

		bstream->writeRangedU32(moveCount, 0, MaxPendingMoves);
		int updateCount  =nInterface->getObjectUpdateCounter();
		
		int flag = NO_UPDATE;
		if (updateCount)
		{
			mWriteTypeTracker=0;
			flag = GHOST_UPDATE;
		}else
		{
			((GamePacketNotify *) notify)->updateType=NO_UPDATE;
		}

		//sending update flag :
		bstream->writeRangedU32(flag, 0, MaxPendingMoves);

		if (flag == GHOST_UPDATE)
		{
			//Sending object count : 
			bstream->writeRangedU32(updateCount, 0, MaxPendingMoves);

			xDistributedObjectsArrayType *distObjects = nInterface->getDistributedObjects();
			xDistObjectIt begin = distObjects->begin();
			xDistObjectIt end = distObjects->end();
			while (begin!=end)
			{
				xDistributedObject *cobj = *begin;
				if (cobj  && cobj->getUpdateState()==E_DO_US_PENDING)
				{
					//////////////////////////////////////////////////////////////////////////
					//writing out objects id :  
					S32 ghostIndex =  getGhostIndex(cobj);
					bstream->writeInt(ghostIndex, GhostConnection::GhostIdBitSize);

					//////////////////////////////////////////////////////////////////////////
					//writing objects state : 
					cobj->pack(bstream);
					cobj->setUpdateState(E_DO_US_SEND);
				}
				begin++;
			}
		}
	
	}
	else
	{
		S32 ghostIndex = -1;
		if(scopeObject.isValid())
		{
			ghostIndex = getGhostIndex(scopeObject);
		}
		mCompressPointsRelative = bstream->writeFlag(ghostIndex != -1);
		if(bstream->writeFlag(getControlCRC() != mLastClientControlCRC))
		{
			if(ghostIndex != -1)
			{
				bstream->writeInt(ghostIndex, GhostConnection::GhostIdBitSize);
				scopeObject->writeControlState(bstream);
			}
		}
	}
		
	//xLogger::xLog(ELOGINFO,XL_START,"pkt w");
	Parent::writePacket(bstream, notify);
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
void vtConnection::readPacket(TNL::BitStream *bstream)
{
	//xLogger::xLog(ELOGINFO,XL_START,"pkt");

	xNetInterface *nInterface = static_cast<xNetInterface *>(getInterface());
	IDistributedObjects *doInterface  = nInterface->getDistObjectInterface();
	
	bool replayControlObjectMoves = false;


	if(isConnectionToClient())
	{
		bstream->read(&mLastClientControlCRC);

		U32 firstMove;
		bstream->read(&firstMove);
		U32 count = bstream->readRangedU32(0, MaxPendingMoves);
		count +=0;

		//retrieving update flag :
		U32 flag = bstream->readRangedU32(0, MaxPendingMoves);

		if ( flag == GHOST_UPDATE)
		{
			//xLogger::xLog(ELOGINFO,XL_START,"ghost update received");
			//retrieve object counter : 
			U32 ocounter = bstream->readRangedU32(0, MaxPendingMoves);
			
			for (unsigned int i  = 0 ; i<ocounter ; i++ )
			{
				U32 ghostIndex = bstream->readInt(GhostConnection::GhostIdBitSize);
				
				
				
				/*xDistributedClient *dstClient = static_cast<xDistributedClient*>(doInterface->getByUserID(getUserID(),E_DC_BTYPE_CLIENT));
				if (dstClient)
				{
					xLogger::xLog(ELOGINFO,XL_START,"ghost update received from %d",getUserID());
					dstClient->setLastUpdater(getUserID());
					dstClient->unpack(bstream,getOneWayTime());

				}*/

				
				xDistributedObject *dobj = static_cast<xDistributedObject*>(resolveGhostParent(ghostIndex));
				if (dobj)
				{
					dobj->setLastUpdater(ghostIndex);
					dobj->unpack(bstream,getOneWayTime());
					//xLogger::xLog(ELOGINFO,XL_START,"ghost update received from %d for object :%d | foundbyIndex %d",getUserID(),getGhostIndex(dobj),dobj->getUserID());
					
				}
			}
		}
	}
	else
	{
		bool controlObjectValid = bstream->readFlag();
		mCompressPointsRelative = controlObjectValid;

		// CRC mismatch...
		if(bstream->readFlag())
		{
			if(controlObjectValid)
			{
				U32 ghostIndex = bstream->readInt(GhostConnection::GhostIdBitSize);
				controlObject = (xDistributedClient*) resolveGhost(ghostIndex);
				controlObject->readControlState(bstream);
				replayControlObjectMoves = true;
			}
			else
			{
				controlObject = NULL;
			}
		}
	}
	Parent::readPacket(bstream);
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

TNL_IMPLEMENT_RPC(vtConnection,c2sDeployDistributedClass,
				  (	TNL::StringPtr className,
				  TNL::Int<16>entityType,
				  TNL::Vector<TNL::StringPtr>propertyNames,
				  TNL::Vector<TNL::Int<16> >propertyNativeTypes,
				  TNL::Vector<TNL::Int<16> >propertyValueTypes,
				  TNL::Vector<TNL::Int<16> >predictionTypes),
				  (className,
				  entityType,
				  propertyNames,
				  propertyNativeTypes,
				  propertyValueTypes,
				  predictionTypes),
				  NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirClientToServer, 1)
{

	unsigned int SizeProps  = propertyNames.size();
	xNetInterface *nInterface = static_cast<xNetInterface *>(getInterface());
	IDistributedClasses *cInterface = nInterface->getDistributedClassInterface();


	xDistributedClass *classTemplate = cInterface->get(className);
	if (!classTemplate)
	{
		classTemplate = cInterface ->createClass(className,entityType);
	}

	xLogger::xLog(XL_START,ELOGTRACE,E_LI_DISTRIBUTED_CLASS_DESCRIPTORS,"Server : Class deployment of %s received by user : %d" , className.getString(),getUserID() );

	for (unsigned int i  =   0  ;  i < SizeProps ; i ++)
	{
		xDistributedPropertyInfo *dInfo  = classTemplate->exists(propertyNames[i]);
		if (!dInfo)
		{
			dInfo = new xDistributedPropertyInfo( propertyNames[i] ,propertyValueTypes[i],propertyNativeTypes[i],predictionTypes[i]);
			classTemplate->getDistributedProperties()->push_back( dInfo);
			xLogger::xLog(XL_START,ELOGTRACE,E_LI_DISTRIBUTED_CLASS_DESCRIPTORS,"\t property attachment : %s | type : %s" , propertyNames[i].getString(), xDistTools::ValueTypeToString(propertyValueTypes[i]).getString());
//			xLogger::xLogExtro(0,"\t property attachment : %s | type : %s" , propertyNames[i].getString(), xDistTools::ValueTypeToString(propertyValueTypes[i]).getString());
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

TNL_IMPLEMENT_RPC(vtConnection,s2cDeployDistributedClass,
				  (	TNL::StringPtr className,
				  TNL::Int<16>entityType,
				  TNL::Vector<TNL::StringPtr>propertyNames,
				  TNL::Vector<TNL::Int<16> >propertyNativeTypes,
				  TNL::Vector<TNL::Int<16> >propertyValueTypes,
				  TNL::Vector<TNL::Int<16> >predictionTypes),
				  (className,
				  entityType,
				  propertyNames,
				  propertyNativeTypes,
				  propertyValueTypes,
				  predictionTypes),
				  NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirServerToClient, 0)
{

	unsigned int SizeProps  = propertyNames.size();
	xNetInterface *nInterface = static_cast<xNetInterface *>(getInterface());
	IDistributedClasses *cInterface = nInterface->getDistributedClassInterface();


	xDistributedClass *classTemplate = cInterface->get(className);
	if (!classTemplate)
		classTemplate = cInterface ->createClass(className,entityType);

	xLogger::xLog(ELOGINFO,E_LI_DISTRIBUTED_CLASS_DESCRIPTORS,"Class deployment of class %s recieved" , className.getString() );

	for (unsigned int i  =   0  ;  i < SizeProps ; i ++)
	{
		xDistributedPropertyInfo *dInfo  = classTemplate->exists(propertyNames[i]);
		if (!dInfo)
		{
			dInfo = new xDistributedPropertyInfo( propertyNames[i] ,propertyValueTypes[i],propertyNativeTypes[i],predictionTypes[i]);
			classTemplate->getDistributedProperties()->push_back( dInfo);
			//xLogger::xLog(ELOGINFO,XL_START,"\t property attachment : %s | type : %s" , propertyNames[i].getString(), xDistTools::ValueTypeToString(propertyValueTypes[i]).getString());
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
TNL_IMPLEMENT_RPC(vtConnection,c2sDOCreate,
				  (	TNL::Int<16>userSrcID,
				  TNL::StringPtr objectName,
				  TNL::StringPtr className,
				  TNL::Int<16>classType),
				  (userSrcID,
				  objectName,
				  className,
				  classType),
				  NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirClientToServer, 1)
{

	xNetInterface *nInterface = static_cast<xNetInterface *>(getInterface());
	IDistributedClasses *cInterface = nInterface->getDistributedClassInterface();
	//////////////////////////////////////////////////////////////////////////
	//dist class 

	xDistributedClass *classTemplate = cInterface->get(className);
	if (!classTemplate){
		xLogger::xLog(ELOGERROR,E_LI_DISTRIBUTED_BASE_OBJECT,"Server : creating dist object %s by user %d failed! class %s not found!"
			,objectName.getString(),getUserID(),className.getString());
		classTemplate = cInterface ->createClass(className,classType);
	}

	
	//////////////////////////////////////////////////////////////////////////
	//the distObject :
	IDistributedObjects *doInterface  = nInterface->getDistObjectInterface();
	xDistributedObject *distObject = doInterface->get( objectName.getString() );
	IDistributedObjects *oInterface  = nInterface->getDistObjectInterface();
	xDistributedClient *client  = (xDistributedClient*)oInterface->getByUserID(userSrcID,E_DC_BTYPE_CLIENT);
	if (!client)
		return;

	if (distObject) return;

	switch(classTemplate->getEnitityType())
	{
		case E_DC_BTYPE_3D_ENTITY:
		{
			distObject  = new xDistributed3DObject();
			distObject->setDistributedClass(classTemplate);
			distObject->setObjectFlags(E_DO_CREATION_COMPLETE);
			distObject->setEntityID(-1);
			distObject->setServerID(getGhostIndex(distObject));
				nInterface->setDOCounter(nInterface->getDOCounter()+1);
			
			distObject->SetName(objectName.getString());
			distObject->setNetInterface(nInterface);

			distObject->setUserID(getUserID());
			nInterface->getDistributedObjects()->push_back(distObject);
			distObject->initProperties();
			int op = client->getSessionID();
			distObject->setSessionID(client->getSessionID());

			float time = (float)Platform::getRealMilliseconds();
			int uid = getUserID();
			distObject->setCreationTime(time);
			xLogger::xLog(ELOGERROR,E_LI_3DOBJECT,"Server : 3d object created by user %d | sessionID :%d",client->getUserID(),client->getSessionID());
		}
		break;
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
TNL_IMPLEMENT_RPC(vtConnection,c2sDODestroy,
				  (	TNL::Int<16>userSrcID,
				  TNL::Int<16>serverID,
				  TNL::StringPtr className,
				  TNL::Int<16>classType),
				  (userSrcID,
				  serverID,
				  className,
				  classType),
				  NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirClientToServer, 1)
{

	xNetInterface *nInterface = static_cast<xNetInterface *>(getInterface());
	IDistributedClasses *cInterface = nInterface->getDistributedClassInterface();
	//////////////////////////////////////////////////////////////////////////
	//dist class 

	xDistributedClass *classTemplate = cInterface->get(className);
	if (!classTemplate)
		classTemplate = cInterface ->createClass(className,classType);

	//////////////////////////////////////////////////////////////////////////
	//the distObject :
	IDistributedObjects *doInterface  = nInterface->getDistObjectInterface();

	
	xDistributedObject *distObject = doInterface->get( serverID );
	if (!distObject) 
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_DISTRIBUTED_BASE_OBJECT,"Couldn't find object with server id :%d",serverID);
		return;
	}
	
	xLogger::xLog(ELOGINFO,E_LI_DISTRIBUTED_BASE_OBJECT,"Server : deleting dist object :%s  %d",distObject->GetName().getString(),distObject->getServerID());
	doInterface->deleteObject(distObject->getServerID());



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
void vtConnection::connect(TNL::NetInterface *theInterface, const TNL::Address &address, bool requestKeyExchange, bool requestCertificate)
{
	Parent::connect(theInterface,address,requestKeyExchange,requestCertificate);

	if (theInterface)
	{
		TNL::Vector<TNL::NetConnection* > con_list = getInterface()->getConnectionList();
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
vtConnection::vtConnection()
{
	
	setTranslatesStrings();
	m_UserName = "not set";
	m_UserID =0;
	m_ConnectionID = 0;
	//setIsAdaptive(); // <-- Uncomment me if you want to use adaptive rate instead of fixed rate...

	highSendIndex[0] = 0;
	highSendIndex[1] = 0;
	highSendIndex[2] = 0;
	mLastClientControlCRC = 0;
	firstMoveIndex = 1;
	mMoveTimeCredit = 0;
	mWriteTypeTracker=0; 
	mSessionID = -1;
    

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
bool vtConnection::isDataToTransmit()
{
	// we always want packets to be sent.
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
void vtConnection::onConnectTerminated(NetConnection::TerminationReason reason, const char *string)
{
	((xNetInterface *) getInterface())->pingingServers = true;
}

/*
*******************************************************************
* Function: onConnectionTerminated
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
void vtConnection::onConnectionTerminated(NetConnection::TerminationReason reason, const char *edString)
{

	//logprintf("%s - %s connection terminated - reason %d.", getNetAddressString(), isConnectionToServer() ? "server" : "client", reason);

	xLogger::xLog(ELOGINFO,E_LI_CONNECTION,"%s - %s connection terminated - reason %d.", getNetAddressString(), isConnectionToServer() ? "Server" : "Client", reason);

	int userID = getUserID();
	xNetInterface *ninterface = static_cast<xNetInterface*>(getInterface());
	if (!ninterface->IsServer())
	{
		xLogger::xLog(ELOGERROR,E_LI_CONNECTION,"Server connection terminated ! - reason %d.", reason);
		//ninterface->checkObjects();
		ninterface->setConnection(NULL);
		enableFlag(ninterface->getInterfaceFlags(),E_NI_DESTROYED_BY_SERVER);

	}

	if (ninterface->IsServer())
	{
		ISession *sInterface =  ninterface->getSessionInterface();
		IDistributedObjects *oInterface  = ninterface->getDistObjectInterface();
		xDistributedClient *client  = (xDistributedClient*)oInterface->getByUserID(userID,E_DC_BTYPE_CLIENT);
		if (client)
		{
			xDistributedSession *session  = sInterface->get(client->getSessionID());
			if (session)
			{
				//logprintf("removing client from session");
				session->removeUser(client);
			}else
				xLogger::xLog(ELOGWARNING,E_LI_CONNECTION,"User disconnected : couldn't find session object");
		}else
		{
			xLogger::xLog(ELOGERROR,E_LI_CONNECTION,"User disconnected : couldn't find client object");
		}
		//logprintf("removing client from database");
		ninterface->removeClient(userID);
		//userIDCounter--;//

	}
	if(isConnectionToServer())
		((xNetInterface *) getInterface())->pingingServers = true;

	if (ninterface)
	{
		ninterface->setCheckObjects(true);
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
void vtConnection::onConnectionEstablished()
{
	Parent::onConnectionEstablished();

	//setSimulatedNetParams(0.0, 0.1);

	//TNL::GhostConnection::setPingTimeouts(1000,3);

	xNetInterface *netInterface = (xNetInterface *)getInterface();
	if (!netInterface) return;
	if (!netInterface->getDistributedClassInterface())return;
	
	if(isInitiator())
	{
		setGhostFrom(false);
		setGhostTo(true);
		xLogger::xLog(ELOGINFO,E_LI_CONNECTION,"Client  - connected to server  : %s", getNetAddressString());
		netInterface->connectionToServer = this;
	}
	else
	{

		localAddress = TNL::Address(getNetAddressString());
		xLogger::xLog(ELOGINFO,E_LI_CONNECTION,"Server :	client %s connected", getNetAddressString());
		xLogger::xLog(ELOGINFO,E_LI_CONNECTION,"Server :	deploying session classes");

		netInterface->deploySessionClasses(this);

		scopeObject = new xDistributedClient();
		scopeObject->setObjectFlags(E_DO_CREATION_COMPLETE);
		scopeObject->setEntityID(userIDCounter);
		scopeObject->setUserID(userIDCounter);
		scopeObject->setServerID(-1);
		scopeObject->setCreationTime((float)Platform::getRealMilliseconds());
		scopeObject->SetName("Client");
		scopeObject->setOwnerConnection(this);

		setUserID(userIDCounter);
		scopeObject->setLocalAddress(getNetAddressString());
		scopeObject->setUserName(getNetAddressString());

		if (netInterface)
		{
			scopeObject->setNetInterface(netInterface);
		}
		
		xDistributedObjectsArrayType *distObjects = netInterface->getDistributedObjects();
		xDistributedClass *clientClass = netInterface->getDistributedClassInterface()->createClass("CLIENT CLASS",E_DC_BTYPE_CLIENT);
		scopeObject->setDistributedClass(clientClass);
		distObjects->push_back(scopeObject);
		
		setScopeObject(scopeObject);
		setGhostFrom(true);
		setGhostTo(false);
		activateGhosting();

		//we call SetUserDetails on the client : 
		s2cSetUserDetails(userIDCounter);

		userIDCounter++;
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
TNL_IMPLEMENT_RPC(vtConnection, s2cSetUserDetails, 
				  (TNL::Int<16>userID),
				  (userID),
				  NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirServerToClient, 0)
{
	
	setUserID(userID);
	((xNetInterface*)getInterface())->setMyUserID(userID);

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
void vtConnection::addUpdateObject(xDistributedObject*object)
{

	for (int i=0 ; i < pendingObjects.size() ; i ++ )
	{
		xDistributedObject *_cobj  = pendingObjects[i];
		if (_cobj)
		{
			if (object == _cobj)
				return;
		}
	}
	pendingObjects.push_back(object);
	object->setUpdateState( E_DO_US_PENDING );
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
TNL::U32 vtConnection::getControlCRC()
{

	PacketStream stream;
	xDistributedClient*co= (xDistributedClient*) getScopeObject();
	if(!co)
		return 0;

	stream.writeInt(getGhostIndex(co), GhostConnection::GhostIdBitSize);
	co->writeControlState(&stream);
	stream.zeroToByteBoundary();
	return stream.calculateCRC(0, stream.getBytePosition()); 
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
void vtConnection::packetReceived(TNL::GhostConnection::PacketNotify *notify)
{

	for(; firstMoveIndex < ((GamePacketNotify *) notify)->firstUnsentMoveIndex; firstMoveIndex++)
	{
		pendingObjects.erase(U32(0));
	}
	//mReadType  = ((GamePacketNotify *) notify)->updateType;
	Parent::packetReceived(notify);

	if (mReadType == GHOST_UPDATE)
	{
		//xLogger::xLog(ELOGINFO,XL_START,"ghost update pkt!");
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
void vtConnection::disconnectFromServer()
{
	if (getConnectionState() == TNL::NetConnection::Connected)
	{
		//disconnect("no reason");
	}
}