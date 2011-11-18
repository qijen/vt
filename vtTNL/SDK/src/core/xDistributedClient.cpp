#include "xDistributedClient.h"
#include "xDistributedBaseClass.h"
#include "xNetInterface.h"
#include "vtConnection.h"
#include "IDistributedClasses.h"
#include "IDistributedObjectsInterface.h"

#include "ISession.h"
#include "xDistributedSessionClass.h"
#include "xDistributedSession.h"
#include "xDistributedClient.h"

#include "xLogger.h"
#include "IMessages.h"
#include "xMessageTypes.h"
#include "vtLogTools.h"
#include "xDistributedProperty.h"
#include "xDistributedPropertyInfo.h"

#include "xDistributedPoint4F.h"
#include "xDistributedPoint3F.h"
#include "xDistributedPoint2F.h"
#include "xDistributedPoint1F.h"
#include "xDistributedString.h"
#include "xDistributedInteger.h"

TNL_IMPLEMENT_NETOBJECT(xDistributedClient); 


using namespace TNL;

int messageIDCounter2  = 0;
int messageINCounter  = 0;



uxString
xDistributedClient::print(TNL::BitSet32 flags)
{

	return Parent::print(flags);
}


void
xDistributedClient::setCurrentOutMessage(xMessage* msg)
{


	
	//xMessage *currentMsg = getCurrentMessage();

	/*if (currentMsg && isFlagOn(currentMsg->getFlags(),E_MF_SENT))
	{
	getNetInterface()->getMessagesInterface()->deleteMessage(currentMsg);
	}*/

	if (msg)
	{
		enableFlag(getClientFlags(),E_CF_NM_SENDING);
	}else
	{
		mCurrentMessage = NULL;
	}

	mCurrentMessage = msg;

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
xDistributedClient::pack(TNL::BitStream *bstream)
{
	//Parent::pack(bstream);
	xMessage *currentMsg = getCurrentMessage();

	IMessages *mInterface = getNetInterface()->getMessagesInterface();


	if (currentMsg)
	{
		TNL::BitSet32 writeFlags=0;
		enableFlag(writeFlags,E_MWF_UPDATE_SERVER);
		disableFlag(writeFlags,E_MWF_SEND_SRC_USER_ID);

		if (!isFlagOn(currentMsg->getFlags(),E_MF_SEND_TO_ALL))
		{
			enableFlag(writeFlags,E_MWF_SEND_TARGET_USER_ID);
		}
		
		
		mInterface->writeToStream(currentMsg,bstream,writeFlags);
		
		enableFlag(currentMsg->getFlags(),E_MF_SENT);
		enableFlag(currentMsg->getFlags(),E_MF_DELETED);
		
		setCurrentOutMessage(NULL);
		disableFlag(getClientFlags(),E_CF_NM_SENDING);

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
void xDistributedClient::unpack( TNL::BitStream *bstream,float sendersOneWayTime )
{

	IMessages *mInterface = getNetInterface()->getMessagesInterface();

	TNL::BitSet32 readFlags=0;
	enableFlag(readFlags,E_MRF_UPDATE_BY_GHOST);
	disableFlag(readFlags,E_MRF_READ_SRC_USER_ID);
	enableFlag(readFlags,E_MRF_READ_TARGET_USER_ID);

	xMessage *msg  = mInterface->readFromStream(bstream,readFlags);
	if (msg)
	{
		xDistributedClient *client = static_cast<xDistributedClient*>(getOwnerConnection()->resolveGhostParent(getLastUpdater()));
		if (client)
		{
			msg->setSrcUser(client->getUserID());
			msg->setClientSource(client);
			
			enableFlag(msg->getFlags(),E_MF_NEW);
			enableFlag(msg->getFlags(),E_MF_OUTGOING);
			
			mInterface->getMessages()->push_back(msg);

			xDistributedSession *session  =  NULL;

			session = getNetInterface()->getSessionInterface()->get(msg->getClientSource()->getSessionID());
			if (!session)
			{
				xLogger::xLog(XL_START,ELOGWARNING,E_LI_MESSAGES,"no session object");
			}

			//////////////////////////////////////////////////////////////////////////

			int numUsers  = session->getNumUsers() -1;


			if (isFlagOn(msg->getFlags(),E_MF_SEND_TO_ALL))
			{
		
				if (msg->getIgnoreSessionMaster() && client->getUserID() !=session->getUserID()  )
				{
                    numUsers--;
				}

/*				if (client->getUserID())
				{
				}
*/
			}else
			{
//				xDistributedClient *dstclient = (xDistributedClient*)client->getOwnerConnection()->resolveGhostParent(msg->getDstUser());
				xDistributedClient *dstclient = static_cast<xDistributedClient*>(getNetInterface()->getDistObjectInterface()->getByUserID(msg->getDstUser(),E_DC_BTYPE_CLIENT));
				//TNL::logprintf("server msg received from user : %d| to %d ",client->getUserID(),msg->getClientSource()->getOwnerConnection()->getGhostIndex(dstclient),msg->getDstUser());

				numUsers  = 1;
			}

			msg->setNumUsers(numUsers);
			//TNL::logprintf("server msg received from user : %d,num users : %d | dst :%d",client->getUserID(),numUsers,msg->getDstUser());
			
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
TNL::U32 
xDistributedClient::packUpdate(TNL::GhostConnection *connection, TNL::U32 updateMask, TNL::BitStream *stream)
{
	xNetInterface *netInterface  = (xNetInterface*) connection->getInterface();
	IMessages *mInterface = netInterface->getMessagesInterface();
	ISession *sInterface = netInterface->getSessionInterface();
	if (netInterface)
	{

		//server side only : 
		if (netInterface->IsServer())
		{
			
			//the first time  ? : we write out all necessary attributes for the client : 
			if(stream->writeFlag(updateMask & InitialMask))
			{
				if(stream->writeFlag(true))
				{
					Parent::packUpdate(connection,updateMask,stream);
					//write out users local address string :
					stream->writeString(getLocalAddress().getString());
					stream->writeString(getUserName().getString());
					
				}
			}

			//////////////////////////////////////////////////////////////////////////
			if(stream->writeFlag(updateMask & NameMask))
			{
				stream->writeString(getUserName().getString());
			}
			//////////////////////////////////////////////////////////////////////////
			xMessage*msg = getCurrentMessage();
			
			xDistributedSession *session  =  NULL;
			
			session = msg ?  getNetInterface()->getSessionInterface()->get(msg->getClientSource()->getSessionID()) : NULL;
			if (msg && !session)
			{
				xLogger::xLog(XL_START,ELOGERROR,E_LI_MESSAGES,"no session object");
				stream->writeFlag(false);
				enableFlag(msg->getFlags(),E_MF_SENT);
				enableFlag(msg->getFlags(),E_MF_DELETED);
				setCurrentOutMessage(NULL);
				return 0;

			}
			if (msg)
			{
				vtConnection *con = (vtConnection*)connection;
				
				//int ghostIndexFrom = con->getGhostIndex( msg->getClientSource() );		
				int ignore = msg->getIgnoreSessionMaster();  


				if (msg->getClientSource()->getOwnerConnection() == connection)
				{
					xLogger::xLog(XL_START,ELOGTRACE,E_LI_MESSAGES,"going to msg source :%d !",getUserID());
					stream->writeFlag(false);
					return 0;
				}

				if (ignore && session && con->getUserID() == session->getUserID())
				{

					xLogger::xLog(XL_START,ELOGTRACE,E_LI_MESSAGES,"going to session master :%d !",getUserID());
					stream->writeFlag(false);
					return 0;
				}

				if (!isFlagOn(msg->getFlags(),E_MF_SEND_TO_ALL))
				{
					IDistributedObjects *doInterface  = netInterface->getDistObjectInterface();
					xDistributedClient *dstClient = static_cast<xDistributedClient*>(doInterface->getByUserID(msg->getDstUser(),E_DC_BTYPE_CLIENT));
					if (con->getUserID() != msg->getDstUser() )
					{
						xLogger::xLog(XL_START,ELOGTRACE,E_LI_MESSAGES,"wrong  destination :%d !",con->getUserID());
						stream->writeFlag(false);
						return 0;
					}
				}
				
				stream->writeFlag(true);
                
				TNL::BitSet32 writeFlags=0;
				enableFlag(writeFlags,E_MWF_UPDATE_GHOST);
				disableFlag(writeFlags,E_MWF_SEND_TARGET_USER_ID);
				enableFlag(writeFlags,E_MWF_SEND_SRC_USER_ID);

				mInterface->writeToStream(msg,stream,writeFlags);

				//TNL::logprintf("msg stats : sendCount %d , numUsers %d",msg->getSendCount() , msg->getNumUsers() );

				
				if (msg->getSendCount() == msg->getNumUsers() )
				{
					enableFlag(msg->getFlags(),E_MF_SENT);
					enableFlag(msg->getFlags(),E_MF_DELETED);
					setCurrentOutMessage(NULL);
					xLogger::xLog(XL_START,ELOGTRACE,E_LI_MESSAGES,"message complete");
				}

			}else{
				
				stream->writeFlag(false);
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
xDistributedClient::unpackUpdate(TNL::GhostConnection *connection, TNL::BitStream *stream)
{

	xNetInterface *netInterface  = (xNetInterface*) connection->getInterface();
	vtConnection *vconnection  = (vtConnection*)connection;
	if (netInterface)
	{
		//client side only : 
		if (!netInterface->IsServer())
		{
			//////////////////////////////////////////////////////////////////////////
			//initial update  ? 
			if(stream->readFlag())
			{
				if(stream->readFlag())
				{

					Parent::unpackUpdate(connection,stream);
					char localAddress[256];stream->readString(localAddress);	// retrieve the local address
					char userName[256];stream->readString(userName);	// retrieve the user name 
					
					setServerID(connection->getGhostIndex((NetObject*)this));
					setObjectFlags(E_DO_CREATION_CREATED);

					setLocalAddress(localAddress);
					
					vtConnection *con  = (vtConnection*)connection;
					setOwnerConnection(con);


					if (con->getUserID() == getUserID())
						enableFlag(getClientFlags(),E_CF_ADDED);
					else
						enableFlag(getClientFlags(),E_CF_ADDING);

					setUserName(userName);
				}

			}

			//////////////////////////////////////////////////////////////////////////
			if(stream->readFlag())
			{
				char oName[256];stream->readString(oName);	// retrieve objects name : 
				setUserName(oName);
				setUserFlags(USERNAME_CHANGED);
				xLogger::xLog(XL_START,ELOGTRACE,E_LI_CLIENT,"name changed");

			}
			//////////////////////////////////////////////////////////////////////////
			
			bool msgRecieved  = stream->readFlag();
			if (msgRecieved)
			{
				IMessages *mInterface = getNetInterface()->getMessagesInterface();
				TNL::BitSet32 readFlags=0;
				enableFlag(readFlags,E_MRF_SERVER_UPDATE);
				disableFlag(readFlags,E_MRF_READ_TARGET_USER_ID);
				enableFlag(readFlags,E_MRF_READ_SRC_USER_ID);

				xMessage *msg  = mInterface->readFromStream(stream,readFlags);
				if (msg)
				{
					enableFlag(msg->getFlags(),E_MF_INCOMING);
					enableFlag(msg->getFlags(),E_MF_NEW);
					enableFlag(msg->getFlags(),E_MF_FINISH);
					msg->setMessageID(messageINCounter);
					messageINCounter++;
					mInterface->getMessages()->push_back(msg);
					//xDistributedObject *dobj = static_cast<xDistributedObject*>(getNetInterface()->getConnection()->(msg->getSrcUser()));
					//TNL::logprintf("msg by %d",dobj->getUserID());
				}
				
				
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


void
xDistributedClient::calculateUpdateBits()
{

	IMessages *mInterface = getNetInterface()->getMessagesInterface();
	xMessage *msg = getCurrentMessage();

	getUpdateBits().clear();

	if (msg && isFlagOn(msg->getFlags(),E_MF_NEW) && !isFlagOn(msg->getFlags(),E_MF_DELETED) )
	{

		getUpdateBits().set(BIT(1),true);
		//&& (msg->getLifeTime() < mInterface->getMessageTimeout())
		/*int index = 1 ; 
		for (int i =0 ;  msg->getParameters().size() ; i++)
		{

			xDistributedProperty *prop = msg->getParameters().at(i);
			getUpdateBits().set(BIT(index),prop->getFlags() & E_DP_NEEDS_SEND );
			

		}*/
		
		xDistributedPropertyArrayIterator begin = msg->getParameters().begin();
		xDistributedPropertyArrayIterator end = msg->getParameters().end();

		int index = 2 ; 
		while (begin!=end)
		{
			xDistributedProperty *prop = *begin;
			if (prop==NULL)
			{
				int op2 = 2;
			}
			getUpdateBits().set(BIT(index),prop->getFlags() & E_DP_NEEDS_SEND );
			index++;
			begin++;
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





TNL_IMPLEMENT_NETOBJECT_RPC(xDistributedClient,rpcSetName,
							(TNL::StringPtr name),(name),
							TNL::NetClassGroupGameMask, TNL::RPCGuaranteedOrdered,TNL::RPCToGhostParent, 0)
{

	setUserName(name);
	setMaskBits(NameMask);
	

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
xDistributedClient::onGhostRemove()
{
	xNetInterface *netInterface  = getNetInterface();
	if (!netInterface)
	{
		return;
	}
//	if (netInterface->IsServer())
//	{
		xLogger::xLog(XL_START,ELOGINFO,E_LI_CLIENT,"Remove clients ghost %d",getUserID());
//	}
	netInterface->removeClient(getUserID());
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
void xDistributedClient::writeControlState(TNL::BitStream *stream)
{
	stream->writeFlag(true);
	stream->writeRangedU32(10, 0, 63);
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
void  xDistributedClient::readControlState(TNL::BitStream * bstream)
{
	bstream->readFlag();
	U32 count = bstream->readRangedU32(0, 63);

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
void xDistributedClient::destroy()
{
	Parent::destroy();
	
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
xDistributedClient::xDistributedClient() : xDistributedObject()
{
	m_DistributedClass=NULL;
	m_EntityID = 0;
	mClientFlags=0;
	mCurrentMessage  = NULL;

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
xDistributedClient::~xDistributedClient()
{
	destroy();
    
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
void xDistributedClient::performScopeQuery(TNL::GhostConnection *connection)
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

			vtConnection *con  =(vtConnection*)connection;
			int conUID = ((vtConnection*)connection)->getUserID();
			int conUIDT = dobject->getUserID();

			int thisID = getSessionID();
			
			int targetID = dobject->getSessionID();
            
			if (dobject->getDistributedClass()->getEnitityType()  == E_DC_BTYPE_3D_ENTITY )
			{
				if (getSessionID() == dobject->getSessionID())
				{
					connection->objectInScope((TNL::NetObject*) dobject );
				}
			}
			if (dobject->getDistributedClass()->getEnitityType()  == E_DC_BTYPE_SESSION  )
			{
				connection->objectInScope((TNL::NetObject*) dobject );

			}

			if (conUID == conUIDT)
			{
				int op = 0;
				connection->objectInScope((TNL::NetObject*) dobject );
				begin++;
				continue;
			}
			if (getSessionID() >1 &&  dobject->getSessionID() >1 && (getSessionID() == dobject->getSessionID()))
			{
				connection->objectInScope((TNL::NetObject*) dobject );
			}else
			{
				//connection->objectInScope((TNL::NetObject*) dobject );
				int op2 = 2;
				op2++;
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

void xDistributedClient::onGhostAvailable(TNL::GhostConnection *theConnection)
{
	xNetInterface *netInterface  = (xNetInterface*) theConnection->getInterface();
	if (netInterface)
	{
		if (netInterface->IsServer())
		{
			if (getServerID()==-1)
			{
				setServerID(theConnection->getGhostIndex(this));
				xLogger::xLog(XL_START,ELOGTRACE,E_LI_CLIENT,"Server id %d",getServerID());
			}
			
			
		}
	}


	if (netInterface->IsServer())
	{
		//setServerID(theConnection->getGhostIndex((TNL::NetObject*)this));
		
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
bool xDistributedClient::onGhostAdd(TNL::GhostConnection *theConnection)
{
	xNetInterface *netInterface  = (xNetInterface*) theConnection->getInterface();
	vtConnection *con  = (vtConnection*)theConnection;
	if (netInterface)
	{
		if (!netInterface->IsServer())
		{
			xDistributedObjectsArrayType *distObjects = netInterface->getDistributedObjects();
			enableFlag(getClientFlags(),E_CF_ADDING);
			distObjects->push_back(this);
			setNetInterface(netInterface);
			if (  con->getUserID() == getUserID() )
			{
				netInterface->setMyClient(this);
			}
            
			int sessionID = getSessionID();
			ISession *sInterface =  netInterface->getSessionInterface();
			xDistributedSession *session  = sInterface->get(sessionID);
			if (session)
			{
				session->addUser(this);
				enableFlag(getClientFlags(),E_CF_SESSION_JOINED);
			}
		}else{

			setServerID(theConnection->getGhostIndex(this));
			TNL::logprintf("on add ServerID %d | Name : %s",getServerID(),GetName().getString());
		}
	}
	return true;
}

