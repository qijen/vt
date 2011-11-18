#ifndef _XDISTRIBUTED_SESSION_H_
#define _XDISTRIBUTED_SESSION_H_


#include "xNetTypes.h"
#include "xDistributedObject.h"
class xDistributedClient;


class xDistributedSession : public xDistributedObject
{

		
	typedef xDistributedObject Parent;
	public :
		xDistributedSession();
		~xDistributedSession();


		enum MaskBits {
			InitialMask = BIT(0),  ///< This mask bit is never set explicitly, so it can be used for initialization data.
			PositionMask = BIT(1), ///< This mask bit is set when the position information changes on the server.
		};
	
		

		int getSessionType() const { return mSessionType; }
		void setSessionType(int val) { mSessionType = val; }
		
		bool isLocked();
		
		xNString getPassword();
		void setPassword(xNString val);

		
		int getMaxUsers();
		void setMaxUsers(int val);
		
		int getSessionID() const { return mSessionID; }
		void setSessionID(int val) { mSessionID = val; }

		bool isPrivate();
		int getNumUsers();
		void setNumUsers(int val);
		bool isFull();


		TNL::BitSet32& getSessionFlags() { return mSessionFlags; }
		void setSessionFlags(TNL::BitSet32 val) { mSessionFlags = val; }

		xClientArrayType *mClientTable;
		xClientArrayType&getClientTable()  { return *mClientTable; }
		bool isClientJoined(int userID);
		void addUser(xDistributedClient *client);
		void removeUser(xDistributedClient *client);


		void onGhostRemove();
		bool onGhostAdd(TNL::GhostConnection *theConnection);
		void onGhostAvailable(TNL::GhostConnection *theConnection);
		TNL::U32 packUpdate(TNL::GhostConnection *connection, TNL::U32 updateMask, TNL::BitStream *stream);
		void unpackUpdate(TNL::GhostConnection *connection, TNL::BitStream *stream);
		void performScopeQuery(TNL::GhostConnection *connection);


		void pack(TNL::BitStream *bstream);
		void unpack(TNL::BitStream *bstream,float sendersOneWayTime);
		void prepare();

		void initProperties();
		xDistributedProperty *getProperty(int nativeType);
		xDistributedProperty *getUserProperty(const char*name);

		int mSessionType;
		int mMaxUsers;
		xNString mPassword;
		xNString mSessionMaster;
		bool mPrivate;
		bool mLocked;
		int mSessionID;
		
		int mUsers;
		TNL::BitSet32 mSessionFlags;
	

		uxString print(TNL::BitSet32 flags);

public:
	TNL_DECLARE_CLASS(xDistributedSession);

};

#endif