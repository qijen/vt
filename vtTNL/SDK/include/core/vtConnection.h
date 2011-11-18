#ifndef __VTCONNECTION_H_
#define __VTCONNECTION_H_
#include "xNetTypes.h"
#include "tnl.h"
#include "tnlGhostConnection.h"

class xNetInterface;
class xDistributedObject;
class xDistributedClient;





class vtConnection : public TNL::GhostConnection
{
	typedef TNL::GhostConnection Parent;
public:

	vtConnection();
	TNL_DECLARE_RPC(c2sDOCreate, (TNL::Int<16>userSrcID,TNL::StringPtr objectName,TNL::StringPtr className,TNL::Int<16>classType));
	TNL_DECLARE_RPC(c2sDODestroy, (TNL::Int<16>userSrcID,TNL::Int<16>serverID,TNL::StringPtr className,TNL::Int<16>classType));
	TNL_DECLARE_RPC(c2sDORequestOwnerShip, (TNL::S32 userSrcID,TNL::Int<16>serverID));
	TNL_DECLARE_RPC(s2cDOChangeOwnershipState, (TNL::Int<16>serverID,TNL::S32 newOwnerID,TNL::S32 state));


	TNL_DECLARE_RPC(s2cSetUserDetails, (TNL::Int<16>userID));
	TNL_DECLARE_RPC(c2sDeployDistributedClass,(
		TNL::StringPtr className,
		TNL::Int<16>entityType,
		TNL::Vector<TNL::StringPtr>propertyNames,
		TNL::Vector<TNL::Int<16> >propertyNativeTypes,
		TNL::Vector<TNL::Int<16> >propertyValueTypes,
		TNL::Vector<TNL::Int<16> >predictionTypes));

	TNL_DECLARE_RPC(s2cDeployDistributedClass,(
		TNL::StringPtr className,
		TNL::Int<16>entityType,
		TNL::Vector<TNL::StringPtr>propertyNames,
		TNL::Vector<TNL::Int<16> >propertyNativeTypes,
		TNL::Vector<TNL::Int<16> >propertyValueTypes,
		TNL::Vector<TNL::Int<16> >predictionTypes));
    
	void connect(TNL::NetInterface *theInterface, const TNL::Address &address, bool requestKeyExchange, bool requestCertificate);
	void onConnectTerminated(TNL::NetConnection::TerminationReason reason, const char *rejectionString);
	void onConnectionTerminated(TNL::NetConnection::TerminationReason reason, const char *string);
	void onConnectionEstablished(); 
	bool isDataToTransmit();
	int getUserID() const { return m_UserID; }
	void setUserID(int val) { m_UserID = val; }
	const char* GetUserName() const { return m_UserName.getString(); }
	void SetUserName(const char*val);
	void setClientName(const char *string) { mClientName = string; }
	int GetConnectionID() const { return m_ConnectionID; }
	void SetConnectionID(int val) { m_ConnectionID = val; }

	//////////////////////////////////////////////////////////////////////////
	//overwrites : 
	void writePacket(TNL::BitStream *bstream,TNL::NetConnection::PacketNotify *notify);
	void readPacket(TNL::BitStream *bstream);
	
	void packetReceived(TNL::GhostConnection::PacketNotify *notify);
	struct GamePacketNotify : public TNL::GhostConnection::GhostPacketNotify
	{
		TNL::U32 firstUnsentMoveIndex;
		TNL::U32 updateType;
		GamePacketNotify() { firstUnsentMoveIndex =  0;updateType=0; }
	};
	 PacketNotify *allocNotify() { return new GamePacketNotify; }

	TNL::U32 getControlCRC();

	int m_UserID;
	xNString m_UserName;
	int m_ConnectionID;
	TNL::RefPtr<xNetInterface> m_NetInterface;
	TNL::Address localAddress;
	TNL::StringTableEntry mClientName;
	TNL_DECLARE_NETCONNECTION(vtConnection);
	TNL::SafePtr<xDistributedClient>scopeObject;

	enum {
		GHOST_UPDATE = 61,
		NO_UPDATE = 58,
	};
	enum {
			MaxPendingMoves = 63,
			MaxMoveTimeCredit = 512,
	};

	
	int mWriteTypeTracker;
	int mReadType;

	int getWriteTypeTracker() const { return mWriteTypeTracker; }
	void setWriteTypeTracker(int val) { mWriteTypeTracker = val; }

	TNL::Vector<xDistributedObject*>pendingObjects;
	void addUpdateObject(xDistributedObject*object);
	TNL::SafePtr<xDistributedObject> controlObject;

	TNL::U32 mLastClientControlCRC;
	bool mCompressPointsRelative;

	TNL::U32 firstMoveIndex;
	TNL::U32 highSendIndex[3];
	TNL::U32 mMoveTimeCredit;


	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	

	TNL_DECLARE_RPC(c2sCreateSession_RPC,(TNL::StringPtr name,TNL::Int<16>type,TNL::Int<16>maxUsers,TNL::StringPtr password));
	TNL_DECLARE_RPC(c2sDeleteSession_RPC,(TNL::Int<16>sessionID));
	TNL_DECLARE_RPC(c2sJoinSession_RPC,(TNL::Int<16>userID,TNL::Int<16>sessionID,TNL::StringPtr password));
	TNL_DECLARE_RPC(s2cUserJoinedSession_RPC,(TNL::Int<16>userID,TNL::Int<16>sessionID));
	
	TNL_DECLARE_RPC(c2sLeaveSession_RPC,(TNL::Int<16>userID,TNL::Int<16>sessionID,TNL::Int<16>destroy));
	TNL_DECLARE_RPC(s2cUserLeftSession_RPC,(TNL::Int<16>userID,TNL::Int<16>sessionID));
	TNL_DECLARE_RPC(c2sLockSession_RPC,(TNL::Int<16>sessionID));

	TNL_DECLARE_RPC(s2cAddServerLogEntry,(TNL::StringPtr entry));



	virtual void disconnectFromServer();

	//////////////////////////////////////////////////////////////////////////
	int mSessionID;
	int getSessionID() const { return mSessionID; }
	void setSessionID(int val) { mSessionID = val; }


	
};
#endif

























