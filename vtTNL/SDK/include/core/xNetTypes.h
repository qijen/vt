#ifndef _XNET_TYPES_H_
#define _XNET_TYPES_H_

#include "vcWarnings.h"


#ifndef _TNL_TYPES_H_
	#include "tnlTypes.h"
#endif


#ifndef _XNET_ENUMERATIONS_H_
	#include "xNetEnumerations.h"
#endif

#ifndef __UX_STRING_H__
	#include <uxString.h>
#endif


using namespace TNL;


#ifndef xTimeType
	typedef unsigned int xTimeType;
#endif

#ifndef StringPtr
	#include "tnlNetBase.h"
	#include "tnlString.h"
#endif



#ifndef xNStream
	#ifndef _TNL_BITSTREAM_H_
		#include "tnlBitStream.h"
	#endif
	typedef TNL::BitStream xNStream;
#endif

__inline bool isFlagOn(TNL::BitSet32 value,int flag)
{
	return value.test(1 << flag );
}
__inline bool isFlagOff(TNL::BitSet32 value,int flag)
{
	return value.test(1 << flag ) ? false : true;
}

__inline void enableFlag(TNL::BitSet32 &value,int flag)
{
	value.set(1 << flag,true );
}
__inline void disableFlag(TNL::BitSet32 &value,int flag)
{
	value.set(1 << flag,false);
}

#ifndef xNString
	typedef TNL::StringPtr xNString;
#endif



#include <stdlib.h>

#include <map>
#include <vector>

//#include <deque>

	class xDistributedClass;
class xDistributed3DObjectClass;
class xDistributedObject;
class xDistributedProperty;
class xDistributedPropertyInfo;
class xDistributedSession;
class xDistributedClient;


class xNetMessage;
class xNetworkMessage;
class xMessageType;
class xMessage;


typedef std::vector<xDistributedProperty*>xDistributedPropertyArrayType;
typedef std::vector<xDistributedProperty*>::iterator xDistributedPropertyArrayIterator;
typedef std::vector<xDistributedPropertyInfo*>xDistributedPropertiesListType;

typedef std::vector<xDistributedObject*>xDistributedObjectsArrayType;
typedef std::vector<xDistributedObject*>::iterator xDistributedObjectsArrayIterator;
typedef std::vector<xDistributedObject*>::iterator xDistObjectIt;

typedef std::map<xNString,xDistributedClass*>xDistributedClassesArrayType;
typedef std::map<xNString,xDistributedClass*>::iterator xDistributedClassesArrayIterator;
typedef xDistributedClassesArrayIterator xDistClassIt;

typedef std::vector<xNetworkMessage*>NetworkMessagesType;
typedef std::vector<xNetworkMessage*>::iterator NetworkMessagesTypeIterator;

typedef std::vector<xNetworkMessage*>xNetworkMessageArrayType;
typedef std::vector<xNetworkMessage*>::iterator xNetworkMessageArrayTypeIterator;

typedef std::vector<xDistributedSession*>xSessionArrayType;
typedef std::vector<xDistributedClient*>xClientArrayType;

typedef std::vector<xMessage*>xMessageArrayType;
typedef std::vector<xMessage*>::iterator xMessageArrayIterator;

typedef std::vector<xMessageType*>xMessageTypeArrayType;
typedef std::vector<xMessageType*>::iterator xMessageTypeArrayIterator;




class xNetworkMessage
{
public :

	typedef std::map<int,xNetMessage*> MessageParameterArrayType;
	typedef std::map<int,xNetMessage*>::iterator MessageParameterArrayTypeIterator;


	xNetworkMessage()
	{

		m_RecievedParameters = new MessageParameterArrayType();

	}
	float lifeTime;
	bool complete;
	int targetUserID;
	int srcUserID;
	int numParameters;
	int messageID;


	virtual ~xNetworkMessage()
	{

	}
	xNString name;


	MessageParameterArrayType *m_RecievedParameters;
};


struct xClientInfo
{
	int userFlag;
	//USERNAME_FLAG userNameFlag;
	int userID;
	xNString userName;
};

struct xDistDeleteInfo
{
	int serverID;
	int entityID;
	int deleteState;

};
#endif
