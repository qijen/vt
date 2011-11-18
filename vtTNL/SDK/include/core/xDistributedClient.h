#ifndef __xDistributedClient_h
#define __xDistributedClient_h

#include "xDistributedObject.h"


class xDistributedClient : public xDistributedObject
{
	typedef xDistributedObject Parent;
	public:
		xDistributedClient();
		virtual ~xDistributedClient();

		
		enum MaskBits {
			InitialMask = (1 << 0),  ///< This mask bit is never set explicitly, so it can be used for initialization data.
			NameMask = (1 << 1), ///< This mask bit is set when the position information changes on the server.
			MessageMask = (1 << 2), ///< This mask bit is set when the position information changes on the server.
		};

		
		
		void onGhostRemove();
		bool onGhostAdd(TNL::GhostConnection *theConnection);
		void onGhostAvailable(TNL::GhostConnection *theConnection);
		TNL::U32 packUpdate(TNL::GhostConnection *connection, TNL::U32 updateMask, TNL::BitStream *stream);
		void unpackUpdate(TNL::GhostConnection *connection, TNL::BitStream *stream);
		void performScopeQuery(TNL::GhostConnection *connection);

		TNL::StringPtr getLocalAddress() const { return m_localAddress; }
		void setLocalAddress(TNL::StringPtr val) { m_localAddress = val; }
		virtual void destroy();
		void writeControlState(TNL::BitStream *stream);
		void  readControlState(TNL::BitStream * bstream);

		void pack(TNL::BitStream *bstream);
		void unpack(TNL::BitStream *bstream,float sendersOneWayTime);
		
		int mUserFlags;
		int getUserFlags() const { return mUserFlags; }
		void setUserFlags(int val) { mUserFlags = val; }
		TNL::StringPtr mUserName;
		TNL::StringPtr getUserName() const { return mUserName; }
		void setUserName(TNL::StringPtr val) { mUserName = val; }
		TNL_DECLARE_RPC(rpcSetName, (TNL::StringPtr name));

		TNL::BitSet32 mClientFlags;
		
		TNL::BitSet32& getClientFlags() { return mClientFlags; }
		void setClientFlags(TNL::BitSet32 val) { mClientFlags = val; }
		

		void setCurrentOutMessage(xMessage* msg);
		xMessage*getCurrentMessage(){ return mCurrentMessage ; }
		xMessage *mCurrentMessage;


		void calculateUpdateBits();
		
		uxString print(TNL::BitSet32 flags);

	protected:

		
		int m_UserID;
		TNL::StringPtr m_localAddress;

	public:
		TNL_DECLARE_CLASS(xDistributedClient);
};
#endif
