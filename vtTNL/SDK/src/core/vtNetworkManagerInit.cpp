#include "precomp.h"
#include "vtPrecomp.h"
#include "vtNetAll.h"

#include "xNetInterface.h"

#include "tnlRandom.h"
#include "tnlSymmetricCipher.h"
#include "tnlAsymmetricKey.h"
#include <pch.h>
#include <vtNetAll.h>


static const char *localBroadcastAddress = "IP:broadcast:28999";
static const char *localHostAddress = "IP:127.0.0.1:28999";

TNL::SafePtr<xNetInterface>m_NetInterfaceServer  = NULL;
TNL::SafePtr<xNetInterface>m_NetInterfaceClient  = NULL;

xNetInterface* GetNetInterfaceServer() 
{	
	return m_NetInterfaceServer ; 
}


void SetNetInterfaceClient(xNetInterface *cInterface)
{	

	if (m_NetInterfaceClient!=NULL)
	{
		m_NetInterfaceClient->destroy();
		delete m_NetInterfaceClient;
		m_NetInterfaceClient = NULL;
	}
	m_NetInterfaceClient =  cInterface;
}
void SetNetInterfaceServer(xNetInterface *cInterface)
{

	if (m_NetInterfaceServer!=NULL)
	{
		m_NetInterfaceServer->destroy();
		delete m_NetInterfaceServer;
		m_NetInterfaceServer = NULL;
	}
	m_NetInterfaceServer =  cInterface;
}

xNetInterface *GetNetInterfaceClient(){	return m_NetInterfaceClient;}



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
vtNetworkManager::Init()
{
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
int vtNetworkManager::ConnectToServer(bool deleteExisting,const char *address)
{
	int result =  0 ;
	if(GetNetInterfaceClient())
	{
		TNL::Address addr  = TNL::Address(address);
		GetNetInterfaceClient()->setConnection(new vtConnection());
		GetNetInterfaceClient()->getConnection()->connect(m_NetInterfaceClient,addr,true,false);
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
int vtNetworkManager::CreateClient(bool deleteExisting,int port,const char *address)
{
	if (m_NetInterfaceClient && !deleteExisting )
	{
		return 0;
	}


	if (deleteExisting)
	{
		delete m_NetInterfaceClient;
	}

	m_NetInterfaceClient = new xNetInterface(false,TNL::Address(TNL::IPProtocol, TNL::Address::Any, 0),TNL::Address(localBroadcastAddress));
	
	TNL::AsymmetricKey *theKey = new TNL::AsymmetricKey(32);
	m_NetInterfaceClient->setPrivateKey(theKey);
	m_NetInterfaceClient->setRequiresKeyExchange(true);
	m_NetInterfaceClient->initBaseClasses(0);

	
	if (m_NetInterfaceClient)
	{
		m_NetInterfaceClient->getDistObjectInterface()->setNetInterface(m_NetInterfaceClient);
		return 1;
	}else
		return 0;

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
int vtNetworkManager::CreateServer(bool deleteExisting,int port,const char *address)
{
	/*if (m_NetInterfaceServer && !deleteExisting )
	{
		return 0;
	}*/

	if (deleteExisting && m_NetInterfaceServer)
	{
		SetServerNetInterface(NULL);
	}

	TNL::Address add(address);
	m_NetInterfaceServer =  new xNetInterface(true,add,TNL::Address(localBroadcastAddress));
	if (m_NetInterfaceServer)
	{

		TNL::Socket *socket  = &m_NetInterfaceServer->getSocket();
		if (socket)
		{
			if (!socket->isValid())
			{
				SetServerNetInterface(NULL);
				return E_NWE_INVALID_PARAMETER;
				
			}
		}
	}

	TNL::AsymmetricKey *theKey = new TNL::AsymmetricKey(32);
	m_NetInterfaceServer->setPrivateKey(theKey);
	m_NetInterfaceServer->setRequiresKeyExchange(false);
	m_NetInterfaceServer->initBaseClasses(0);

	if (m_NetInterfaceServer)
	{
		GetNetInterfaceServer()->setAllowsConnections(true);

	}

	return E_NWE_OK;
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
vtNetworkManager::CreateLocalConnection()
{

	int result  = 0 ; 
	if (localConnection)
	{
		delete localConnection;
	}

	if (GetNetInterfaceServer())	
		CreateClient(true,0,localHostAddress);


	if (GetNetInterfaceClient() && GetNetInterfaceServer())
	{
		GetNetInterfaceClient()->setConnection(new vtConnection());
		result  = GetNetInterfaceClient()->getConnection()->connectLocal(m_NetInterfaceClient,m_NetInterfaceServer);
	}
	return result;
}
