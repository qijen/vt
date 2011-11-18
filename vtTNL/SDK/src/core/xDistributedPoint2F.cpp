#include "xDistributedPoint2F.h"
#include "xMathStream.h"
#include "xPredictionSetting.h"
#include "xDistributedPropertyInfo.h"


uxString 
xDistributedPoint2F::print(TNL::BitSet32 flags)
{
	return uxString();
}
void 
xDistributedPoint2F::updateFromServer(TNL::BitStream *stream)
{

	xMath::stream::mathRead(*stream,&mLastServerValue);
	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_PREDICTED)
		xMath::stream::mathRead(*stream,&mLastServerDifference);
	setValueState(E_DV_UPDATED);
}

void
xDistributedPoint2F::updateGhostValue(TNL::BitStream *stream)
{

	xMath::stream::mathWrite(*stream,mCurrentValue);
	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_PREDICTED)
		xMath::stream::mathWrite(*stream,mDifference);
}

/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
void xDistributedPoint2F::unpack(TNL::BitStream *bstream,float sendersOneWayTime)
{
	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_PREDICTED)
	{
		sendersOneWayTime *= 0.001f;

		
		Point2F p;
		xMath::stream::mathRead(*bstream,&p);
		Point2F v;	
		xMath::stream::mathRead(*bstream,&v);

		Point2F predictedPos  = p + v * sendersOneWayTime;
		setOwnersOneWayTime(sendersOneWayTime);
		
		mLastValue = mCurrentValue;
		mCurrentValue = predictedPos;
		mDifference= mCurrentValue - mLastValue;
	}
	
	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_RELIABLE)
	{
		Point2F p;
		xMath::stream::mathRead(*bstream,&p);
		mLastValue = mCurrentValue;
		mLastServerValue =p;
		mCurrentValue = p;
	}


}
/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
void xDistributedPoint2F::pack(TNL::BitStream *bstream)
{
	
	xMath::stream::mathWrite(*bstream,mCurrentValue);
	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_PREDICTED)
		xMath::stream::mathWrite(*bstream,mDifference);
	
	int flags = getFlags();
	flags &= (~E_DP_NEEDS_SEND);
	setFlags(flags);
}
/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/

bool
xDistributedPoint2F::updateValue(Point2F value,xTimeType currentTime)
{
	bool result  = false;
	
	mLastTime = mCurrentTime;
	mCurrentTime = currentTime;
	mLastDeltaTime = mCurrentTime - mLastTime;
	
	mLastValue = mCurrentValue;
    mCurrentValue = value;
	mDifference = mCurrentValue - mLastValue;
	
	mThresoldTicker +=mLastDeltaTime;

	float lengthDiff = fabsf(mDifference.len());
	float timeDiffMs = ((float)mThresoldTicker) / 1000.f;

	int flags  = getFlags();
	flags =E_DP_OK;
	

	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_PREDICTED)
	{
		if (lengthDiff > getPredictionSettings()->getMinDifference() )
		{
			if (mThresoldTicker2 > getPredictionSettings()->getMinSendTime())
			{
				
				flags =E_DP_NEEDS_SEND;	
				result   = true ;
			}
		}

		Point2F serverDiff  = mCurrentValue-mLastServerValue ;
		
		if (  fabsf( serverDiff.len() ) > getPredictionSettings()->getMinDifference() )
		{
			if (mThresoldTicker2 > (getPredictionSettings()->getMinSendTime() * 0.2f) )
			{
				flags =E_DP_NEEDS_SEND;	
				result   = true ;
			}
		}

		if (mThresoldTicker2 > getPredictionSettings()->getMinSendTime())
		{
			mThresoldTicker2  = 0 ;
			mThresoldTicker  = 0;
		}
	}
	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_RELIABLE)
	{
		flags =E_DP_NEEDS_SEND;	
		result   = true ;
		mLastValue = value;
		mCurrentValue = value;
	}
	setFlags(flags);
	
	return result;
}