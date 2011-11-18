#ifndef __xDistributedString_H
#define __xDistributedString_H

#include "xDistributedProperty.h"

class xDistributedString : public xDistributedProperty
{

	public:


		typedef xDistributedProperty Parent;

		xDistributedString ( xDistributedPropertyInfo *propInfo, xTimeType maxWriteInterval, xTimeType maxHistoryTime ) : xDistributedProperty(propInfo)
		{
			
			
			mLastTime = 0;
			mCurrentTime = 0;
			mLastDeltaTime = 0 ; 
			mFlags = 0;
			mThresoldTicker = 0;
			mLastValue = xNString("");
			mCurrentValue = xNString("");

		}

		~xDistributedString(){}

		xNString mLastValue;
		xNString mCurrentValue;
		
		bool updateValue(xNString value,xTimeType currentTime);
        void pack(xNStream *bstream);
		void unpack(xNStream *bstream,float sendersOneWayTime);
		void updateGhostValue(xNStream *stream);
		void updateFromServer(xNStream *stream);
        virtual uxString print(TNL::BitSet32 flags);
		

};

#endif
