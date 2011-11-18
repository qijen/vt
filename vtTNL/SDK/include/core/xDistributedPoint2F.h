#ifndef __xDistributedPoint2F_H
#define __xDistributedPoint2F_H
#include "xPoint.h"
#include "xDistributedProperty.h"
#include "xTNLInternAll.h"
#include <xNetworkTypes.h>

class xDistributedPoint2F : public xDistributedProperty
{

	public:


		typedef xDistributedProperty Parent;

		xDistributedPoint2F ( xDistributedPropertyInfo *propInfo, xTimeType maxWriteInterval, xTimeType maxHistoryTime ) : xDistributedProperty(propInfo)
		{
			
			mLastValue = Point2F(0.0f,0.0f);
			mCurrentValue= Point2F(0.0f,0.0f);
			mDifference = Point2F(0.0f,0.0f);
			mLastTime = 0;
			mCurrentTime = 0;
			mLastDeltaTime = 0 ; 
			mFlags = 0;
			mThresoldTicker = 0;
		}

		~xDistributedPoint2F(){}

		Point2F mLastValue;
		Point2F mCurrentValue;
		Point2F mDifference;

		Point2F mLastServerValue;
		Point2F mLastServerDifference;
        
		bool updateValue(Point2F value,xTimeType currentTime);
		Point2F getDiff(Point2F value);
        void pack(TNL::BitStream *bstream);
		void unpack(TNL::BitStream *bstream,float sendersOneWayTime);
		void updateGhostValue(TNL::BitStream *stream);
		void updateFromServer(TNL::BitStream *stream);
		virtual uxString print(TNL::BitSet32 flags);
        
		

};

#endif
