#include "IMessages.h"
#include "xNetInterface.h"
#include "vtConnection.h"
#include "xMessageTypes.h"
#include "xDistributedClient.h"
#include "xLogger.h"
#include "vtLogTools.h"
#include "xDistributedProperty.h"
#include "xDistributedPropertyInfo.h"

#include "xDistributedPoint4F.h"
#include "xDistributedPoint3F.h"
#include "xDistributedPoint2F.h"
#include "xDistributedPoint1F.h"
#include "xDistributedString.h"
#include "xDistributedInteger.h"

int msgIDCounter=0;
/*
{
	if (getThresoldTicker() < getMinSendTime())
	{
		TNL::logprintf("flood detected  : %f",getThresoldTicker());
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	xMessage*currentMessage = myClient->getCurrentMessage();
	if (currentMessage)
	{
		if (isFlagOn(currentMessage->getFlags(),E_MF_SENT))
		{
			//xLogger::xLog(XL_START,ELOGINFO,E_LI_MESSAGES,"message was sent by client,removing");
			enableFlag(currentMessage->getFlags(),E_MF_DELETED);
			currentMessage =NULL;
			myClient->setCurrentOutMessage(NULL);
			// 			enableFlag(myClient->getClientFlags(),E_CF_NM_SENT);
			// 			disableFlag(myClient->getClientFlags(),E_CF_NM_SENDING);
			// 			setThresoldTicker(getMinSendTime() + 10.0f);
		}
	}

	xMessage *mOut = getNextOutMessage();
	if (mOut ==NULL)
	{
		return;
	}

	TNL::logprintf("msg :ticker time  : %f",getThresoldTicker());

	if (isFlagOn(myClient->getClientFlags(),E_CF_NM_SENDING))
	{
		//xLogger::xLog(XL_START,ELOGWARNING,E_LI_MESSAGES,"busy!");
		return;
	}else
	{
		setThresoldTicker(0.0f);
	}

	//////////////////////////////////////////////////////////////////////////


	enableFlag(mOut->getFlags(),E_MF_SENDING);
	enableFlag(myClient->getClientFlags(),E_CF_NM_SENDING);
	if (mOut->getDstUser()==-1)
	{
		enableFlag(mOut->getFlags(),E_MF_SEND_TO_ALL);
	}

	myClient->setCurrentOutMessage(mOut);

}*/
void
IMessages::writeToStream(xMessage *msg,TNL::BitStream *bstream,TNL::BitSet32 flags)
{

	bstream->writeRangedU32(msg->getParameters().size(),0,16);
	bstream->writeString(msg->getName().getString(),strlen(msg->getName().getString()));
	
	if (isFlagOn(flags,E_MWF_SEND_SRC_USER_ID))
	{
		bstream->writeInt(msg->getSrcUser(), GhostConnection::GhostIdBitSize);
	}


	bstream->writeFlag(isFlagOn(msg->getFlags(),E_MF_SEND_TO_ALL));
	bstream->writeFlag(msg->getIgnoreSessionMaster());

	if (!isFlagOn(msg->getFlags(),E_MF_SEND_TO_ALL) 
		&& isFlagOn(flags,E_MWF_SEND_TARGET_USER_ID))
		bstream->writeInt(msg->getDstUser(),GhostConnection::GhostIdBitSize);


	//////////////////////////////////////////////////////////////////////////
	xDistributedPropertyArrayType *props = &msg->getParameters();
	for (unsigned int i = 0  ;  i < props->size() ; i++ )
	{
		xDistributedProperty *prop  = props->at(i);
		xDistributedPropertyInfo*propInfo  = prop->getPropertyInfo();

		bstream->writeRangedU32(propInfo->mValueType,0,10);
	
		if (isFlagOn(flags,E_MWF_UPDATE_SERVER))
		{
			prop->pack(bstream);
			
		}

		if (isFlagOn(flags,E_MWF_UPDATE_GHOST))
		{
			prop->updateGhostValue(bstream);
			
		}
	}
	
	/*xDistributedClient *myClient  = getNetInterface()->getMyClient();
	if (myClient )
	{
	
		enableFlag(myClient->getClientFlags(),E_CF_NM_SENT);
		disableFlag(myClient->getClientFlags(),E_CF_NM_SENDING);
	}*/

	setThresoldTicker(getMinSendTime() + 10.0f);
	msg->setSendCount(msg->getSendCount() +1 );

	//TNL::logprintf("msg written :%d  tTime: %f | dst : %d",msg->getMessageID(),getThresoldTicker(),msg->getDstUser());
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
xMessage* IMessages::readFromStream(TNL::BitStream *bstream,TNL::BitSet32 flags)
{

	//////////////////////////////////////////////////////////////////////////
	xMessage *msg  = new xMessage();

	int parSize  = bstream->readRangedU32(0,16);
	char mName[256];bstream->readString(mName);
	msg->setName(mName);

	U32 ghostSrcIndex = 0 ; 
	S32 ghostDstIndex =0;
	
	//////////////////////////////////////////////////////////////////////////
	if (isFlagOn(flags,E_MRF_READ_SRC_USER_ID))
	{
		ghostSrcIndex = bstream->readInt(GhostConnection::GhostIdBitSize);
		//TNL::logprintf("reading msg: \t reading src id :%d",ghostSrcIndex);
		msg->setSrcUser(ghostSrcIndex);
	}

	//////////////////////////////////////////////////////////////////////////
	bool sendToAll  =  bstream->readFlag();
	bool ignoreSessionMaster=  bstream->readFlag();
	msg->setIgnoreSessionMaster(ignoreSessionMaster);
	if (sendToAll)
	{
		enableFlag(msg->getFlags(),E_MF_SEND_TO_ALL);
		msg->setDstUser(-1);
	}

	if (	!isFlagOn(msg->getFlags(),E_MF_SEND_TO_ALL)  && 
				isFlagOn(flags,E_MRF_READ_TARGET_USER_ID) )
	{
		ghostDstIndex = 	 bstream->readInt(GhostConnection::GhostIdBitSize);
		msg->setDstUser(ghostDstIndex);
	}

	//////////////////////////////////////////////////////////////////////////
	for (int i  = 0 ; i < parSize ; i ++ )
	{

		int valType = bstream->readRangedU32(0,10);

		xDistributedPropertyInfo *dInfo  = new xDistributedPropertyInfo( "NoName" , valType , -1 ,E_PTYPE_RELIABLE);

		xDistributedProperty *prop = NULL;
		
		switch(valType)
		{
			case E_DC_PTYPE_STRING :
			{
				prop = new xDistributedString(dInfo,30,3000);
				break;
			}
			
			case E_DC_PTYPE_FLOAT:
			{
				prop = new xDistributedPoint1F(dInfo,30,3000);
				break;
			}
			case E_DC_PTYPE_INT:
			{
				prop = new xDistributedInteger(dInfo,30,3000);
				break;
			}
			case E_DC_PTYPE_2DVECTOR:
			{
				prop = new xDistributedPoint2F(dInfo,30,3000);
				break;
			}
			case E_DC_PTYPE_3DVECTOR :
			{
				prop = new xDistributedPoint3F(dInfo,30,3000);
				break;
			}
			case E_DC_PTYPE_QUATERNION:
			{
				prop = new xDistributedQuatF(dInfo,30,3000);
				break;
			}
			
		}
		
		if (isFlagOn(flags,E_MRF_UPDATE_BY_GHOST))
		{
			prop->unpack(bstream,0.0f);

		}

		if (isFlagOn(flags,E_MRF_SERVER_UPDATE))
		{
			prop->updateFromServer(bstream);

		}
		msg->getParameters().push_back(prop);
			
	}

	//TNL::logprintf("msg recieved :%d",msg->getMessageID());
	return msg;
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
IMessages::addMessage(xMessage*msg)
{


	xDistributedClient *myClient  = getNetInterface()->getMyClient();
	if (!myClient )
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MESSAGES,"no client object");
		return;
	}

	if (!msg)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MESSAGES,"invalid input message");
		return;
	}

	enableFlag(msg->getFlags(),E_MF_NEW);
	enableFlag(msg->getFlags(),E_MF_OUTGOING);

	getMessages()->push_back(msg);
	msg->setMessageID(msgIDCounter);
	msgIDCounter ++;


	if (msg->getDstUser()==-1)
	{
		enableFlag(msg->getFlags(),E_MF_SEND_TO_ALL);
	}

	return;

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
void IMessages::advanceTime(float deltaTime)
{

	xMessageArrayType *messages = getMessages();
	
	float time  = getThresoldTicker() +deltaTime ;
	setThresoldTicker( time );

	xMessageArrayIterator begin = messages->begin();
	xMessageArrayIterator end = messages->end();
	while (begin!=end)
	{
		xMessage* _cmsg  = *begin;
		if (_cmsg && !isFlagOn(_cmsg->getFlags(),E_MF_DELETED) )
		{
			_cmsg->setLifeTime( _cmsg->getLifeTime() + deltaTime );
		}
		begin++;
	}
}

int IMessages::getNumOutMessages()
{

	xMessageArrayType *messages = getMessages();


	xMessageArrayIterator begin = messages->begin();
	xMessageArrayIterator end = messages->end();
	int result  = 0;
	
	while (begin!=end)
	{
		xMessage* _cmsg  = *begin;
		if (_cmsg )
		{
			if (!isFlagOn(_cmsg->getFlags(),E_MF_DELETED)  &&
				isFlagOn(_cmsg->getFlags(),E_MF_NEW) && 
				isFlagOn(_cmsg->getFlags(),E_MF_OUTGOING) && 
				!isFlagOn(_cmsg->getFlags(),E_MF_SENT) )
			{
				result ++;
			}
	
		}
		begin++;
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
void IMessages::checkMessages()
{
	xMessageArrayType *messages = getMessages();


	xMessageArrayIterator begin = messages->begin();
	xMessageArrayIterator end = messages->end();
	while (begin!=end)
	{
		xMessage* _cmsg  = *begin;
		if (_cmsg )
		{
			if (  _cmsg->getLifeTime() > getMessageTimeout() )
			{
				xLogger::xLog(XL_START,ELOGTRACE,E_LI_MESSAGES,"msg expired %f | timout : %f",_cmsg->getLifeTime(),getMessageTimeout() );
				enableFlag(_cmsg->getFlags(),E_MF_DELETED); 
			}
		}
		begin++;
	}

	if (!getNetInterface()->IsServer() && !getNumOutMessages())
	{
		if (getNetInterface()->getMyClient())
		{
			xDistributedClient *client  = getNetInterface()->getMyClient();
			
			disableFlag(client->getClientFlags(),E_CF_NM_SENDING);
			client->setCurrentOutMessage(NULL);
			setThresoldTicker(getMinSendTime() + 10.0f);
		}
	}
	
	if (!getNetInterface()->IsServer() &&getNumOutMessages())
	{
		if (getNetInterface()->getMyClient())
		{
			xDistributedClient *client  = getNetInterface()->getMyClient();
			xMessage *mOut = getNextOutMessage();
			if (mOut && getThresoldTicker() > getMinSendTime() ) 
			{
				//TNL::logprintf("push next message %d",mOut->getMessageID());
				//client->setUpdateState(E_DO_US_PENDING);
				client->setCurrentOutMessage(mOut);
				setThresoldTicker(0.0f);
			}
		}
	}

	if (getNetInterface()->IsServer())
	{
	
		if (getNumOutMessages())
		{
			xMessage *mOut = getNextOutMessage();
			xDistributedClient *client  = mOut->getClientSource();
			if (mOut && client && client->getCurrentMessage()==NULL && getThresoldTicker() > getMinSendTime() ) 
			{
				//TNL::logprintf("push next message %d",mOut->getMessageID());
				if (mOut->getClientSource())
				{
					mOut->getClientSource()->setCurrentOutMessage(mOut);
					mOut->getClientSource()->setMaskBits(1<<xDistributedClient::MessageMask);
					setThresoldTicker(0.0f);
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
int
IMessages::getNumMessagesOfType(int type)
{
	int counter  = 0 ;
	xMessageArrayType *messages = getMessages();

	xMessageArrayIterator begin = messages->begin();
	xMessageArrayIterator end = messages->end();
	while (begin!=end)
	{
		xMessage* _cmsg  = *begin;
		if (_cmsg )
		{
			if ( isFlagOn(_cmsg->getFlags(),type) )
			{

				counter++;
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
void IMessages::deleteAllOldMessages()
{

	int numMessages = getNumMessagesOfType(E_MF_DELETED);
	if (!numMessages)
	{
		return;
	}


	xMessageArrayType *messages = getMessages();
	xMessageArrayIterator begin = messages->begin();
	xMessageArrayIterator end = messages->end();
	while (begin!=end)
	{
		xMessage* _cmsg  = *begin;
		if (_cmsg )
		{
			if ( isFlagOn(_cmsg->getFlags(),E_MF_DELETED) )
			{

				
				if (isFlagOn(_cmsg->getFlags(),E_MF_OUTGOING) && !isFlagOn(_cmsg->getFlags(),E_MF_SENT))
				{
					xLogger::xLog(XL_START,ELOGTRACE,E_LI_MESSAGES,"message never sent %d %f",_cmsg->getMessageID(),_cmsg->getLifeTime());
				}
				
				if (getNetInterface()->IsServer())
				{
					xDistributedClient *client   = _cmsg->getClientSource();
					if (client)
					{
						client->setCurrentOutMessage(NULL);
					}
				}
				
				getMessages()->erase(begin);
				delete _cmsg;
				_cmsg = NULL;
				
				if (getNumMessagesOfType(E_MF_DELETED))
				{
				//	xLogger::xLog(XL_START,ELOGTRACE,E_LI_MESSAGES,"check again");
					deleteAllOldMessages();
				}
				return;
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
IMessages::deleteMessage(xMessage*msg)
{
	if (!msg )return;

	
	enableFlag(msg->getFlags(),E_MF_DELETED); 
	return;

	/*
	xDistributedPropertyArrayIterator begin = msg.getParameters().begin();
	xDistributedPropertyArrayIterator end = msg.getParameters().end();

	while (begin!=end)
	{
		xDistributedProperty *prop = *begin;
		if (prop)
		{

			xDistributedPropertyInfo *info = prop->m_PropertyInfo;
			if (info)
			{
				delete info;
				prop->m_PropertyInfo=NULL;
			}
			
			msg.getParameters().erase(begin);
			delete prop;
			begin = msg.getParameters().begin();
			int size = msg.getParameters().size();
			if (size)
				continue;
			else
				break;

		}
		begin++;
	}*/
	/*msg.getParameters().clear();
	delete msg.getParameters();*/
	delete msg;

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
IMessages::removeMessage(xMessage*msg)
{
	if (!msg) return;

	xMessageArrayType *messages = getMessages();

	xMessageArrayIterator begin = messages->begin();
	xMessageArrayIterator end = messages->end();
	while (begin!=end)
	{
		xMessage* _cmsg  = *begin;
		if (_cmsg && _cmsg ==msg)
		{
			enableFlag(_cmsg->getFlags(),E_MF_DELETED); 
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
xMessage*IMessages::getNextInMessage(xNString mName)
{

	for(int i = 0 ; i < getMessages()->size() ; i++)
	{
		xMessage *msg = getMessages()->at(i);
		if (isFlagOn(msg->getFlags(),E_MF_NEW) && 
			isFlagOn(msg->getFlags(),E_MF_INCOMING) &&
			isFlagOn(msg->getFlags(),E_MF_FINISH) &&
			!strcmp(msg->getName().getString(),mName.getString() ) )
		{
			return msg;
		}
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
xMessage*IMessages::getNextInMessage()
{

	for(int i = 0 ; i < getMessages()->size() ; i++)
	{
		xMessage *msg = getMessages()->at(i);
		if (isFlagOn(msg->getFlags(),E_MF_NEW) && 
			isFlagOn(msg->getFlags(),E_MF_INCOMING) &&
			isFlagOn(msg->getFlags(),E_MF_FINISH) )
		{
			return msg;
		}
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
xMessage*IMessages::getNextOutMessage()
{

	for(int i = 0 ; i < getMessages()->size() ; i++)
	{
		xMessage *msg = getMessages()->at(i);
		if (isFlagOn(msg->getFlags(),E_MF_NEW) && 
			isFlagOn(msg->getFlags(),E_MF_OUTGOING) &&
			!isFlagOn(msg->getFlags(),E_MF_SENT) &&
			!isFlagOn(msg->getFlags(),E_MF_DELETED) )
		{
			return msg;
		}
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
xMessageArrayType*
IMessages::getMessages()
{
	if (getNetInterface())
	{
		return getNetInterface()->getMessages();
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
IMessages::IMessages(xNetInterface *ninterface) : mNetInterface(ninterface)
{

	setMessageTimeout(5000);
	mMinSendTime = 30;
	mThresoldTicker = getMinSendTime() + 10.0f;
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

