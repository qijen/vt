
#include "xNetInterface.h"
#include "vtConnection.h"



#include <tnlAsymmetricKey.h>
//#include "windows.h"

TNL::RefPtr<xNetInterface>m_NetInterfaceServer = NULL;
TNL::RefPtr<xNetInterface>GetServerInterface(){ return m_NetInterfaceServer;}
const char *localBroadcastAddress = "IP:broadcast:28999";
const char *localHostAddress = "IP:127.0.0.1:28999";
const char *localServerAddress ="IP:Any:28999";

class DedicatedServerLogConsumer : public TNL::LogConsumer
{
public:
	void logString(const char *string)
	{
		//ctx()->OutputToConsoleEx("%s\n",string);
		printf("%s\n\n", string);
	}
} gDedicatedServerLogConsumer;

using namespace TNL;

int main(int argc, const char **argv)
{


	//////////////////////////////////////////////////////////////////////////
	//set up logging : 
	//TNLLogEnable(LogGhostConnection, true);
	TNLLogEnable(LogNetInterface,true);

	//////////////////////////////////////////////////////////////////////////
	//create a server : 
	//xNetworkFactory::xSCreateServerInterface(m_NetInterfaceServer,localServerAddress,localBroadcastAddress);
	TNL::Address *add = new TNL::Address("IP:Any:28999");
	TNL::Address *addBC=new TNL::Address("IP:broadcast:28999");
	m_NetInterfaceServer =  new xNetInterface(true,*add,*addBC);
	TNL::AsymmetricKey *theKey = new TNL::AsymmetricKey(32);
	m_NetInterfaceServer->setPrivateKey(theKey);
	m_NetInterfaceServer->setRequiresKeyExchange(false);
//	m_NetInterfaceServer->GetDistObjectInterface()->SetNetworkInterface(m_NetInterfaceServer);

	if (m_NetInterfaceServer)
	{
		m_NetInterfaceServer->setAllowsConnections(true);
		//m_NetInterfaceServer->GetDistObjectInterface()->SetNetworkInterface(m_NetInterfaceServer);
	}

	//Sleep(2000);
	if (GetServerInterface())
	{
		logprintf("\t Server Created");
	}
	
	for (;;)
	{

		GetServerInterface()->tick();
		TNL::Platform::sleep(1);
	}


	return 0;
}
