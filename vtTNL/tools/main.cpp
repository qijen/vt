#include "Prereqs.h"
#include <stdio.h>
#include "tnl.h"
#include "tnlNetBase.h"
#include "xNetInterface.h"
#include "xNetworkFactory.h"

#include "tnlLog.h"

using namespace TNL;
static RefPtr<xNetInterface> myInterFace=NULL;
static const char *localBroadcastAddress = "IP:broadcast:28999";
static const char *localHostAddress = "IP:127.0.0.1:28999";

class DedicatedServerLogConsumer : public TNL::LogConsumer
{
public:
	void logString(const char *string)
	{
		printf("%s\n", string);
	}
} gDedicatedServerLogConsumer;


int main(int argc, const char **argv)
{
	TNLLogEnable(LogNetInterface, true);

	myInterFace  = xNetworkFactory::CreateNetworkInterface(true,28999,localHostAddress);


	myInterFace->setAllowsConnections(true);
	myInterFace->setRequiresKeyExchange(false);

	//serverInterface->setPrivateKey(new AsymmetricKey(32));
	//NetInterfaceTestConnection *clientConnection = new NetInterfaceTestConnection;

	//Address addr("IP:127.0.0.1:25001");
	//clientConnection->connect(clientInterface, &addr, true, false);

	for(;;)
	{
		myInterFace->checkIncomingPackets();
		myInterFace->processConnections();
		//clientInterface->checkIncomingPackets();
		//clientInterface->processConnections();
	}
}