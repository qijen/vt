#ifndef __PREREQS_H_
#define __PREREQS_H_


struct xNetworkSettings;
class xNetInterface;
class vtConnection;
class xNetworkFactory;
class xNetObject;
class IMessages;



class xDistributedClass;
class xDistributed3DObjectClass;
class IDistributedClasses;

class xDistributedProperty;
class xDistributedPropertyInfo;

class xDistributedObject;
class xDistributedClient;
class xDistributed3DObject;

struct x3DObjectData;

class vtNetworkManager;
class IDistributedObjects;
class xPredictionSetting;
//class Extrapolator;


//class vtDistributedObjectsArrayType;


namespace TNL
{
	class NetInterface;
	class GhostConnection;
	class BitStream;

}
#include "tnlNetBase.h"
#include <tnlString.h>
//using namespace TNL;

typedef TNL::StringPtr xNString;

#include "stdlib.h"
#include "map"
#include <vector>
#include <deque>

#endif
