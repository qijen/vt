#ifndef __XNETINTERFACE_H_
#define __XNETINTERFACE_H_

#include "xNetTypes.h"
#include "tnl.h"
#include "tnlNetInterface.h"

#include "vtConnection.h"

class IMessages;
class IDistributedObjects;
class IDistributedClasses;

struct xClientInfo;
struct xDistDeleteInfo;

class xDistributedObject;
class xDistributedClient;
class xDistributedSession;
class ISession;


class xNetInterface  : public TNL::NetInterface
{
	   
	typedef TNL::NetInterface Parent;
	public:


		ISession* getSessionInterface() const { return mISession; }
		void setSessionInterface(ISession* val) { mISession = val; }

		xSessionArrayType& getSessions() { return *mSessions; }

		//////////////////////////////////////////////////////////////////////////
		xNetworkMessageArrayType *m_NetworkMessages;
		xNetworkMessageArrayType& getNetworkMessages()  { return *m_NetworkMessages; }
		IMessages * getMessagesInterface() { return m_IMessages; }
		void setMessagesInterface(IMessages * val) { m_IMessages = val; }



		/************************************************************************/
		/*                                                                      */
		/************************************************************************/

		xMessageArrayType* mMessages; 
		xMessageArrayType* getMessages() { return mMessages; }
		 

		//////////////////////////////////////////////////////////////////////////
		//user : 

		TNL::Vector<xClientInfo*>m_ClientInfoTable;
		TNL::Vector<xClientInfo*>&getClientInfoTable() { return m_ClientInfoTable; }
		
		TNL::Vector<xDistDeleteInfo*>m_DistDeleteTable;
		TNL::Vector<xDistDeleteInfo*>&getDistDeleteTable() { return m_DistDeleteTable; }
		void addDeleteObject(xDistributedObject* object);

		//////////////////////////////////////////////////////////////////////////
		// Distributed Classes Interface
		IDistributedClasses *getDistributedClassInterface();
		void setDistributedClassInterface(IDistributedClasses *distClassInterface);

		//////////////////////////////////////////////////////////////////////////
		//Distributed Objects
		xDistributedObjectsArrayType * getDistributedObjects() { return m_DistributedObjects; }
		void setDistributedObjects(xDistributedObjectsArrayType * val) { m_DistributedObjects = val; }

		IDistributedObjects * getDistObjectInterface() { return m_DistObjectInterface; }
		void setDistObjectInterface(IDistributedObjects * val) { m_DistObjectInterface = val; }


		typedef TNL::NetInterface Parent;
		/// Constants used in this NetInterface
		enum Constants {
			PingDelayTime     = 2000,                    ///< Milliseconds to wait between sending GamePingRequest packets.
			GamePingRequest   = FirstValidInfoPacketId,  ///< Byte value of the first byte of a GamePingRequest packet.
			GamePingResponse,                            ///< Byte value of the first byte of a GamePingResponse packet.
			ScanPingRequest,
			ScanPingResponse
		};

		bool pingingServers; ///< True if this is a client that is pinging for active servers.
		TNL::U32 lastPingTime;    ///< The last platform time that a GamePingRequest was sent from this network interface.
		bool m_IsServer;       ///< True if this network interface is a server, false if it is a client.
		bool IsServer() const { return m_IsServer; }
		void IsServer(bool val) { m_IsServer = val; }
		TNL::SafePtr<vtConnection>connectionToServer; ///< A safe pointer to the current connection to the server, if this is a client.
		
		

		vtConnection* getConnection() const { return connectionToServer; }
		void setConnection(vtConnection* val);
        
		TNL::Address pingAddress; ///< Network address to ping in searching for a server.  This can be a specific host address or it can be a LAN broadcast address.

		xNetInterface();
		/// Constructor for this network interface, called from the constructor for TestGame.
		/// The constructor initializes and binds the network interface, as well as sets
		/// parameters for the associated game and whether or not it is a server.
		xNetInterface(bool server, const TNL::Address &bindAddress, const TNL::Address &pingAddr);
		
		/// handleInfoPacket overrides the method in the NetInterface class to handle processing
		/// of the GamePingRequest and GamePingResponse packet types.
		void handleInfoPacket(const TNL::Address &address, TNL::U8 packetType, TNL::BitStream *stream);

		/// sendPing sends a GamePingRequest packet to pingAddress of this TestNetInterface.
		void sendPing();

		/// Tick checks to see if it is an appropriate time to send a ping packet, in addition
		/// to checking for incoming packets and processing connections.
		void tick();

		void *m_UserData;
		void * GetUserData() const { return m_UserData; }
		void SetUserData(void * val) { m_UserData = val; }

		void processPacket(const TNL::Address &sourceAddress, TNL::BitStream *pStream);
		
		float elapsedConnectionTime;
		float getElapsedConnectionTime() const { return elapsedConnectionTime; }
		void setElapsedConnectionTime(float val) { elapsedConnectionTime = val; }

		float connectionTimeOut;
		float getConnectionTimeOut() const { return connectionTimeOut; }
		void setConnectionTimeOut(float val) { connectionTimeOut = val; }

		bool connectionInProgress;
		bool isConnectionInProgress() const { return connectionInProgress; }
		void setConnectionInProgress(bool val) { connectionInProgress = val; }

		void sendPing(TNL::Address addr,int type=GamePingRequest);
		bool isValid();
		bool m_isConnected;
		bool isConnected() const { return m_isConnected; }
		void setConnected(bool val) { m_isConnected = val; }

		void destroy();

		void createScopeObject();

		/************************************************************************/
		/* user table                                                                      */
		/************************************************************************/
		

		/************************************************************************/
		/* clean funcs :												                                                                       */
		/************************************************************************/
		int mConnectionIDCounter;
		int getConnectionIDCounter() const { return mConnectionIDCounter; }
		void setConnectionIDCounter(int val) { mConnectionIDCounter = val; }

		int m_DOCounter;
		int getDOCounter() const { return m_DOCounter; }
		void setDOCounter(int val) { m_DOCounter = val; }

		TNL::U32& getObjectUpdateCounter() { return m_ObjectUpdateCounter; }
		void setObjectUpdateCounter(TNL::U32 val) { m_ObjectUpdateCounter = val; }
		
		TNL::U32 m_ObjectUpdateCounter;
		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		void updateDistributedObjects(int flags,float deltaTime);

		float mLastTime;
		float mDeltaTime;
		float mLastSendTime;
		
		float getLastOneWayTime()  { return mLastSendTime; }
		
		float mLastRoundTripTime;
		
		float getLastRoundTripTime() { return mLastRoundTripTime; }
		
		
		int mLastTime2;
		float mLastDeltaTime2;
		float getLastDeltaTime2() const { return mLastDeltaTime2; }
		void setLastDeltaTime2(float val) { mLastDeltaTime2 = val; }
		void advanceTime(xTimeType timeNow,float deltaTime);
        
		float GetConnectionTime(){ return mLastSendTime ;}
		void writeDistributedProperties(TNL::BitStream *bstream);
		void calculateUpdateCounter();
        
		float mCurrentThresholdTicker;
		float& getCurrentThresholdTicker(){ return mCurrentThresholdTicker; }
		void setCurrentThresholdTicker(float val) { mCurrentThresholdTicker = val; }

		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		void removeClient(int clientID);
			
		virtual void checkConnections();
		int getNumConnections();
		int getNumSessions();
		void checkOwners();
		void disconnect(int connectionID);
		int getNumDistributedObjects(int templateType);

		bool mCheckObjects;
		bool getCheckObjects() const { return mCheckObjects; }
		void setCheckObjects(bool val) { mCheckObjects = val; }
		void checkObjects();

		void initBaseClasses(int flags);

		void deploySessionClasses(TNL::GhostConnection *connection);


		xDistributedSession * getCurrentSession() { return mCurrentSession; }
		void setCurrentSession(xDistributedSession * val) { mCurrentSession = val; }

		int getMyUserID() const { return mMyUserID; }
		void setMyUserID(int val) { mMyUserID = val; }
		
		xDistributedClient * getMyClient() { return mMyClient; }
		void setMyClient(xDistributedClient * val) { mMyClient = val; }
		void checkSessions();
		void printObjects(int type, int flags);


		std::vector<xNString>& getLocalLanServers() { return mLocalLanServers; }
		void setLocalLanServers(std::vector<xNString> val) { mLocalLanServers = val; }

		void enableLogLevel(int type,int verbosity,int value);
		TNL::BitSet32& getInterfaceFlags() { return mInterfaceFlags; }
		void setInterfaceFlags(TNL::BitSet32 val) { mInterfaceFlags = val; }

		void setFixedRateParameters(TNL::U32 minPacketSendPeriod, TNL::U32 minPacketRecvPeriod, TNL::U32 maxSendBandwidth, TNL::U32 maxRecvBandwidth,bool global=false);

	protected:
	private:

		//////////////////////////////////////////////////////////////////////////
		//  [5/5/2008 mc007] : final
		IDistributedClasses *m_DistributedClassInterface;
		IDistributedObjects *m_DistObjectInterface;
		xDistributedObjectsArrayType *m_DistributedObjects;
		IMessages *m_IMessages;
		ISession*mISession;
		xSessionArrayType* mSessions;
		xDistributedSession *mCurrentSession;
		int mMyUserID;
		xDistributedClient *mMyClient;

		std::vector<xNString>mLocalLanServers;
		TNL::BitSet32 mInterfaceFlags;
		
		
		
		
		
		
		
};

#endif
