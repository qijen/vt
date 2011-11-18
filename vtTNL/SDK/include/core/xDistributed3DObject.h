#ifndef __xDistributed3DObject_h
#define __xDistributed3DObject_h


#include "xDistributedObject.h"
class x3DObjectData;
class xDistributedProperty;

namespace TNL
{
	class GhostConnection;
}



class xDistributed3DObject	: public xDistributedObject
{
	typedef xDistributedObject Parent;
	public:
		xDistributed3DObject();
		~xDistributed3DObject();

		
		enum MaskBits {
			InitialMask = BIT(0),  ///< This mask bit is never set explicitly, so it can be used for initialization data.
			PositionMask = BIT(1), ///< This mask bit is set when the position information changes on the server.
		};

		
		void doInitUpdate();
		void updateAll();
		void onGhostRemove();
		bool onGhostAdd(TNL::GhostConnection *theConnection);
		void onGhostAvailable(TNL::GhostConnection *theConnection);
		TNL::U32 packUpdate(TNL::GhostConnection *connection, TNL::U32 updateMask, TNL::BitStream *stream);
		void unpackUpdate(TNL::GhostConnection *connection, TNL::BitStream *stream);
		void performScopeQuery(TNL::GhostConnection *connection);

		TNL_DECLARE_RPC(c2sGetOwnerShip,(TNL::U32 userSrcID));
		
		virtual void destroy();
		virtual void update(float time);


		virtual void initProperties();
		void pack(TNL::BitStream *bstream);
		void unpack(TNL::BitStream *bstream,float sendersOneWayTime);
		void prepare();
		
		xDistributedProperty *getProperty(int nativeType);
		xDistributedProperty *getUserProperty(const char*name);
		uxString print(TNL::BitSet32 flags);

		
	protected:

		TNL::StringPtr m_localAddress;
		
	public:
		TNL_DECLARE_CLASS(xDistributed3DObject);
};
#endif
