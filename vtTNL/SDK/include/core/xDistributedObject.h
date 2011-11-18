#ifndef __xDistributedObject_h
#define __xDistributedObject_h

#include "xNetObject.h"
#include "xNetTypes.h"

class xDistributed3DObject;
class xDistributedClass;
class xDistributedProperty;




class xDistributedObject : public xNetObject
{
	typedef xNetObject Parent;
	public:
		xDistributedObject();
		virtual ~xDistributedObject();

		enum MaskBits {
			InitialMask = (1 << 0),  ///< This mask bit is never set explicitly, so it can be used for initialization data.
		};

		int getEntityID() const { return m_EntityID; }
		void setEntityID(int val) { m_EntityID = val; }
		int getGhostDebugID() const { return m_GhostDebugID; }
		void setGhostDebugID(int val) { m_GhostDebugID = val; }

		void getOwnership();

		xDistributedClass * getDistributedClass() { return m_DistributedClass; }
		void setDistributedClass(xDistributedClass * val) { m_DistributedClass = val; }

		int getObjectFlags() { return m_ObjectFlags; }
		void setObjectFlags(int val) { m_ObjectFlags = val; }

		int getServerID() const { return m_ServerID; }
		void setServerID(int val) { m_ServerID = val; }

		float getCreationTime() const { return m_creationTime; }
		void setCreationTime(float val) { m_creationTime = val; }


		float getCreationTimeout() const { return m_creationTimeout; }
		void setCreationTimeout(float val) { m_creationTimeout = val; }
		
		int getInterfaceFlags() const { return m_InterfaceFlags; }
		void setInterfaceFlags(int val) { m_InterfaceFlags = val; }
		
		int& getUpdateFlags() { return m_UpdateFlags; }
		void setUpdateFlags(int val) { m_UpdateFlags = val; }

		TNL::BitSet32& getUpdateBits() { return m_UpdateBits; }
		void setUpdateBits(TNL::BitSet32 val) { m_UpdateBits = val; }

		TNL::BitSet32& getGhostUpdateBits(){ return m_GhostUpdateBits; }
		void setGhostUpdateBits(TNL::BitSet32 val) { m_GhostUpdateBits = val; }


		virtual void writeControlState(xNStream *stream);
		virtual void  readControlState(xNStream * bstream){}
		virtual void initProperties();

		virtual void destroy();
		
		xDistributed3DObject *cast(xDistributedObject* _in);
		xDistributedPropertyArrayType *m_DistrtibutedPorperties;
		xDistributedPropertyArrayType * getDistributedPorperties(){ return m_DistrtibutedPorperties; }
		void setDistributedPorperties(xDistributedPropertyArrayType * val) { m_DistrtibutedPorperties = val; }

		virtual void pack(xNStream *bstream);
		virtual void unpack(xNStream *bstream,float sendersOneWayTime);
		virtual void prepare();
		TNL::U32 getGhostID() const { return mGhostID; }
		void setGhostID(TNL::U32 val) { mGhostID = val; }
		int getUpdateState() const { return m_UpdateState; }
		void setUpdateState(int val) { m_UpdateState = val; }



		TNL::BitSet32& getOwnershipState() { return m_OwnershipState; }
		void setOwnershipState(TNL::BitSet32 val) { m_OwnershipState = val; }
		bool isOwner(){ return getOwnershipState().test( 1<<E_DO_OS_OWNER ); }
		
		void changeOwnership(int newUserID,int state);
		
		int getSessionID() const { return mSessionID; }
		void setSessionID(int val) { mSessionID = val; }
		
		//virtual void performScopeQuery(TNL::GhostConnection *connection);
		virtual bool onGhostAdd(TNL::GhostConnection *theConnection);
		virtual void onGhostAvailable(TNL::GhostConnection *theConnection);


		virtual xDistributedProperty *getProperty(int nativeType);
		virtual xDistributedProperty *getUserProperty(const char*name);


		int getLastUpdater() const { return mLastUpdater; }
		void setLastUpdater(int val) { mLastUpdater = val; }
		TNL::BitSet32& getObjectStateFlags()  { return mObjectStateFlags; }
		void setObjectStateFlags(TNL::BitSet32 val) { mObjectStateFlags = val; }
		virtual uxString print(TNL::BitSet32 flags);


		virtual TNL::U32 packUpdate(TNL::GhostConnection *connection, TNL::U32 updateMask, TNL::BitStream *stream);
		virtual void unpackUpdate(TNL::GhostConnection *connection, TNL::BitStream *stream);

	protected:

		int m_ObjectFlags;
		int m_InterfaceFlags;
		int m_UpdateFlags;
		int m_UpdateState;
		
		TNL::BitSet32 m_UpdateBits;
		TNL::BitSet32 m_GhostUpdateBits;

		TNL::U32 mGhostID;
		
		int m_EntityID;
		int m_GhostDebugID;
		int m_ServerID;
		float m_creationTime;
	
		TNL::BitSet32 m_OwnershipState;
		float m_creationTimeout;
		xDistributedClass *m_DistributedClass;
		TNL::BitSet32 mObjectStateFlags;
		
		int mSessionID;
		int mLastUpdater;
		
		

};
#endif
