#ifndef _XDISTRIBUTED_POINT4F_H_
#define _XDISTRIBUTED_POINT4F_H_

#include "xDistributedProperty.h"
#include "xQuat.h"

class xDistributedQuatF : public xDistributedProperty
{

	public:


		typedef xDistributedProperty Parent;

		xDistributedQuatF ( xDistributedPropertyInfo *propInfo, xTimeType maxWriteInterval, xTimeType maxHistoryTime ) : xDistributedProperty(propInfo)
		{
			
			mLastValue = QuatF(0.0f,0.0f,0.0f,0.0f);
			mCurrentValue= QuatF(0.0f,0.0f,0.0f,0.0f);
			mDifference = QuatF(0.0f,0.0f,0.0f,0.0f);
			mLastTime = 0;
			mCurrentTime = 0;
			mLastDeltaTime = 0 ; 
			mFlags = 0;
			mThresoldTicker = 0;
		}

		~xDistributedQuatF(){}
		

		QuatF mLastValue;
		QuatF mCurrentValue;
		QuatF mDifference;

		QuatF mLastServerValue;
		QuatF mLastServerDifference;
        
		bool updateValue(QuatF value,xTimeType currentTime);
		QuatF getDiff(QuatF value);
        void pack(xNStream *bstream);
		void unpack(xNStream *bstream,float sendersOneWayTime);
		void updateGhostValue(xNStream *stream);
		void updateFromServer(xNStream *stream);
		virtual uxString print(TNL::BitSet32 flags);

        
		

};

#endif
