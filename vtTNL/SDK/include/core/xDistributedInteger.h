#ifndef __xDistributedInteger_H
#define __xDistributedInteger_H

#include "xDistributedProperty.h"


class xDistributedInteger : public xDistributedProperty
{

	public:


		typedef xDistributedProperty Parent;

		xDistributedInteger ( xDistributedPropertyInfo *propInfo, xTimeType maxWriteInterval, xTimeType maxHistoryTime ) : xDistributedProperty(propInfo)
		{
			
			
			mLastValue=0;
			mCurrentValue=0;
			mLastTime = 0;
			mCurrentTime = 0;
			mLastDeltaTime = 0 ; 
			mFlags = 0;
			mThresoldTicker = 0;
			mLastServerValue = 0 ;
			mLastServerDifference = 0;
			mDifference  = 0;
		}

		~xDistributedInteger(){}

		TNL::S32 mLastValue;
		TNL::S32 mCurrentValue;
		TNL::S32 mDifference;
		TNL::S32 mLastServerValue;
		TNL::S32 mLastServerDifference;
		
		bool updateValue(TNL::S32 value,xTimeType currentTime);
        void pack(TNL::BitStream *bstream);
		void unpack(TNL::BitStream *bstream,float sendersOneWayTime);
		void updateGhostValue(TNL::BitStream *stream);
		void updateFromServer(TNL::BitStream *stream);
		virtual uxString print(TNL::BitSet32 flags);
        
		

};

#endif
