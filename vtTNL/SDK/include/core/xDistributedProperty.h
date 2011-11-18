#ifndef __xDistributedProperty_h
#define __xDistributedProperty_h


#include "xNetTypes.h"

namespace TNL
{
	class BitStream;
}

class xPredictionSetting;
class xDistributedPropertyInfo;

class xDistributedProperty 
{

	public:
	

		xDistributedProperty(xDistributedPropertyInfo *propInfo);
		virtual ~xDistributedProperty(){}
		
		xDistributedPropertyInfo *m_PropertyInfo;

		xDistributedPropertyInfo * getPropertyInfo() { return m_PropertyInfo; }
		void setPropertyInfo(xDistributedPropertyInfo * val) { m_PropertyInfo = val; }
		
		xTimeType mLastTime;
		xTimeType getLastTime() const { return mLastTime; }
		void setLastTime(xTimeType val) { mLastTime = val; }
		
		xTimeType mThresoldTicker;
		
		xTimeType getThresoldTicker() const { return mThresoldTicker; }
		void setThresoldTicker(xTimeType val) { mThresoldTicker = val; }

		xTimeType mCurrentTime;
		xTimeType mLastDeltaTime;
		xTimeType getCurrentTime() const { return mCurrentTime; }
		void setCurrentTime(xTimeType val) { mCurrentTime = val; }

		int  mFlags;
		TNL::U32 mGhostId;
		
		TNL::U32 getGhostId() const { return mGhostId; }
		void setGhostId(TNL::U32 val) { mGhostId = val; }
		
		int getFlags() const { return mFlags; }
		void setFlags(int val) { mFlags = val; }
		
		virtual void pack(TNL::BitStream *bstream);
		virtual void unpack(TNL::BitStream *bstream,float sendersOneWayTime);
		virtual int getBlockIndex(){return m_BlockIndex;}
		virtual void setBlockIndex(int index){m_BlockIndex = index;}
		int m_BlockIndex;

		float mLastTime2;
		float mLastDeltaTime2;
		float mThresoldTicker2;
		void advanceTime(xTimeType time1,float lastDeltaTime2 );
		xPredictionSetting *m_PredictionSettings;
		
		xPredictionSetting *getPredictionSettings() const { return m_PredictionSettings; }
		void setPredictionSettings(xPredictionSetting * val) { m_PredictionSettings = val; }

		virtual void updateGhostValue(TNL::BitStream *stream){}
		virtual void updateFromServer(TNL::BitStream *stream){}
        
		float m_ownersOneWayTime;
		float getOwnersOneWayTime() const { return m_ownersOneWayTime; }
		void setOwnersOneWayTime(float val) { m_ownersOneWayTime = val; }

		int mValueState;
		int getValueState() const { return mValueState; }
		void setValueState(int val) { mValueState = val; }
		virtual uxString print(TNL::BitSet32 flags);

		


};

#endif
