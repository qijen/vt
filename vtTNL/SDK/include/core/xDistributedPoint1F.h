#ifndef __xDistributedPoint1F_H
#define __xDistributedPoint1F_H

#include "xDistributedProperty.h"


class xDistributedPoint1F : public xDistributedProperty
{

	public:


		typedef xDistributedProperty Parent;

		xDistributedPoint1F ( xDistributedPropertyInfo *propInfo, xTimeType maxWriteInterval, xTimeType maxHistoryTime ) : xDistributedProperty(propInfo)
		{
			
			mLastValue = 0.0f;
			mCurrentValue= 0.0f;
			mDifference = 0.0f;
			mLastTime = 0;
			mCurrentTime = 0;
			mLastDeltaTime = 0 ; 
			mFlags = 0;
			mThresoldTicker = 0;
		}

		~xDistributedPoint1F(){}
		

		TNL::F32 mLastValue;
		TNL::F32 mCurrentValue;
		TNL::F32 mDifference;

		TNL::F32 mLastServerValue;
		TNL::F32 mLastServerDifference;
        
		bool updateValue(TNL::F32 value,xTimeType currentTime);
		TNL::F32 getDiff(TNL::F32 value);
        void pack(TNL::BitStream *bstream);
		void unpack(TNL::BitStream *bstream,float sendersOneWayTime);
		void updateGhostValue(TNL::BitStream *stream);
		void updateFromServer(TNL::BitStream *stream);
		virtual uxString print(TNL::BitSet32 flags);
        
		

};

#endif
