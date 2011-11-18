#include "ISession.h"
#include "xDistributedSessionClass.h"
#include "xDistributedSession.h"

#include "IDistributedObjectsInterface.h"

#include "xNetInterface.h"
#include "xDistributedClient.h"
#include "xDistributedInteger.h"
#include "xLogger.h"
#include "vtLogTools.h"

void ISession::deleteSession(int sessionID)
{
	xNetInterface *netInterface = getNetInterface();
	if ( netInterface && netInterface->IsServer())
	{
		xDistributedSession *session  = get(sessionID);
		if (session)
		{

			IDistributedObjects *doInterface  = netInterface->getDistObjectInterface();
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
							if (distClient && distClient->getSessionID() == sessionID )
							{
								distClient->setSessionID(-1);
								disableFlag(distClient->getClientFlags(),E_CF_SESSION_JOINED);

							}
						}
					}
				}
				begin++;
			}
		
			doInterface->deleteObject((xDistributedObject*)session);

		}else
			xLogger::xLog(XL_START,ELOGERROR,E_LI_SESSION,"couldn't find session %d",sessionID);
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
void ISession::unlockSession(int userID,int sessionID)
{

	xNetInterface *netInterface = getNetInterface();

	if ( netInterface && !netInterface->IsServer() && netInterface->isValid())
	{
		xDistributedSession *session  = get(sessionID);
		if ( session)
		{

			if (session->getUserID() != userID)
			{
				xLogger::xLog(XL_START,ELOGERROR,E_LI_SESSION,"User %d must be session master !",userID);
				return;
			}
			
			int sessionLocked = 0;
			xDistributedInteger *pLocked= (xDistributedInteger *)session->getProperty(E_DC_S_NP_LOCKED);
			pLocked->updateValue(sessionLocked,0);
			pLocked->setFlags(E_DP_NEEDS_SEND);
			return ;
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
void ISession::lockSession(int userID,int sessionID)
{

	xNetInterface *netInterface = getNetInterface();

	if ( netInterface && !netInterface->IsServer() && netInterface->isValid())
	{
		xDistributedSession *session  = get(sessionID);
		if ( session)
		{

			if (session->getUserID() != userID)
			{
				xLogger::xLog(XL_START,ELOGERROR,E_LI_SESSION,"User %d must be session master !",userID);
				return;
			}
			int sessionLocked = 1;
			xDistributedInteger *pLocked= (xDistributedInteger *)session->getProperty(E_DC_S_NP_LOCKED);
			pLocked->updateValue(sessionLocked,0);
			pLocked->setFlags(E_DP_NEEDS_SEND);
			return ;
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
int ISession::removeClient(xDistributedClient*client,int sessionID,bool destroy/* =false */)
{

	xNetInterface *netInterface = getNetInterface();
	if (client && netInterface && !netInterface->IsServer() && netInterface->isValid())
	{
		vtConnection *con  = netInterface->getConnection();
		xDistributedSession *session  = get(sessionID);
		xDistributedClient *myClient  = netInterface->getMyClient();
		if ( con && session && myClient )
		{

			if (session->getUserID() !=myClient->getUserID() && myClient->getUserID()!=client->getUserID() )
			{
				xLogger::xLog(XL_START,ELOGERROR,E_LI_SESSION,"You must be session master to remove a client !");
				return -1;
			}

			xLogger::xLog(XL_START,ELOGINFO,E_LI_SESSION,"Removing user :%d from session %d",client->getUserID(),sessionID);
			//////////////////////////////////////////////////////////////////////////
			con->c2sLeaveSession_RPC(client->getUserID(),sessionID,destroy);
			client->getClientFlags().set(1 << E_CF_DELETING,true);
			client->getClientFlags().set(1 << E_CF_DELETED,false);
		}
	}else 
	{
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	//its our self who leaves the session
	//we must remove all other clients from the dist array : 
	xDistributedClient *myClient  = netInterface->getMyClient();
	if (myClient && myClient->getUserID() == client->getUserID() && netInterface->getCurrentSession() )
	{

	
		IDistributedObjects *doInterface  = netInterface->getDistObjectInterface();
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
						if (distClient)
						{
							if (distClient->getUserID()  != myClient->getUserID() &&
								distClient->getSessionID()==sessionID )
							{
								distClient->getClientFlags().set(1 << E_CF_DELETING,true);
								distClient->getClientFlags().set(1 << E_CF_DELETED,false);
							}
						}
					}
				}
			}
			begin++;
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
int ISession::joinClient(xDistributedClient*client, int sessionID,xNString password)
{

	xNetInterface *netInterface = getNetInterface();
	
	if (netInterface && !netInterface->IsServer() && netInterface->isValid())
	{
		xDistributedSession *session  = get(sessionID);
		if (session)
		{
			vtConnection *con  = netInterface->getConnection();
			if ( con)
			{
				con->c2sJoinSession_RPC(client->getUserID(),sessionID,password);
				return 0;
			}
		}
	}
	return -1;
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
xDistributedSession*ISession::get(int sessionID)
{

	if (!getNetInterface())	{		return NULL;	}
	if (!getNetInterface()->getDistributedObjects())return NULL;

	xDistributedObjectsArrayType *distObjects = getNetInterface()->getDistributedObjects();
	xDistObjectIt begin = distObjects->begin();
	xDistObjectIt end = distObjects->end();
	while (begin!=end)
	{
		xDistributedObject* dobj = *begin;
		if (dobj  && dobj->getDistributedClass() && dobj->getDistributedClass()->getEnitityType() == E_DC_BTYPE_SESSION )
		{
			xDistributedSession *session  = static_cast<xDistributedSession*>(dobj);
			if (session && session->getSessionID()  == sessionID)
				return session;
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
xDistributedSession*ISession::getByUserID(int userID)
{

	if (!getNetInterface())	{		return NULL;	}
	if (!getNetInterface()->getDistributedObjects())return NULL;
	return static_cast<xDistributedSession*>(getNetInterface()->getDistObjectInterface()->getByUserID(userID,E_DC_BTYPE_SESSION));

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
xDistributedSession* ISession::get(xNString name)
{

	if (!getNetInterface())	{		return NULL;	}
	if (!getNetInterface()->getDistributedObjects())return NULL;

	xDistributedObjectsArrayType *distObjects = getNetInterface()->getDistributedObjects();
	return static_cast<xDistributedSession*>(getNetInterface()->getDistObjectInterface()->get(name,E_DC_BTYPE_SESSION));

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
ISession::create(xNString name,int maxUsers,xNString password,int type)
{

	int result  = -1 ;

	if (getNetInterface() && getNetInterface()->isValid() && !getNetInterface()->IsServer() )
	{
		vtConnection *con  = getNetInterface()->getConnection();
		con->c2sCreateSession_RPC(name,type,maxUsers,password);
	}

	return result ;

}

