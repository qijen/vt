#ifndef _XDISTRIBUTED_3F_H_
#define _XDISTRIBUTED_3F_H_

#include "xDistributedProperty.h"
#include "xPoint.h"
class xDistributedPoint3F : public xDistributedProperty
{

	public:

//		typedef xDistributedPropertyBase<Point3F>DataBase;

		typedef xDistributedProperty Parent;

		xDistributedPoint3F ( xDistributedPropertyInfo *propInfo, xTimeType maxWriteInterval, xTimeType maxHistoryTime ) : xDistributedProperty(propInfo)
		{
			mLastValue = Point3F(0,0,0);
			mCurrentValue= Point3F(0,0,0);
			mDifference = Point3F(0,0,0);
			mLastTime = 0;
			mCurrentTime = 0;
			mLastDeltaTime = 0 ; 
			mFlags = 0;
			mThresoldTicker = 0;
		}

		virtual ~xDistributedPoint3F(){}

		Point3F mLastValue;
		Point3F mCurrentValue;
		Point3F mDifference;

		Point3F mLastServerValue;
		Point3F mLastServerDifference;
        
		bool updateValue(Point3F value,xTimeType currentTime);
		Point3F getDiff(Point3F value);
        void pack(xNStream *bstream);
		void unpack(xNStream *bstream,float sendersOneWayTime);
		void updateGhostValue(xNStream *stream);
		void updateFromServer(xNStream *stream);
		virtual uxString print(TNL::BitSet32 flags);
        
		

};

#endif
