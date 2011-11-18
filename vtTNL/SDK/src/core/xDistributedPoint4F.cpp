#include "xDistributedPoint4F.h"
#include "xMathStream.h"
#include "xPredictionSetting.h"
#include "xDistributedPropertyInfo.h"



uxString 
xDistributedQuatF::print(TNL::BitSet32 flags)
{
	return uxString();
}
void 
xDistributedQuatF::updateFromServer(xNStream *stream)
{

	xMath::stream::mathRead(*stream,&mLastServerValue);
	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_PREDICTED)
	{
		xMath::stream::mathRead(*stream,&mLastServerDifference);
	}
	
	setValueState(E_DV_UPDATED);
}

void
xDistributedQuatF::updateGhostValue(xNStream *stream)
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
void xDistributedQuatF::unpack(xNStream *bstream,float sendersOneWayTime)
{
	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_PREDICTED)
	{
		sendersOneWayTime *= 0.001f;
		
		QuatF p;
		xMath::stream::mathRead(*bstream,&p);
		QuatF v;	
		xMath::stream::mathRead(*bstream,&v);

		mLastValue = mCurrentValue;
		mCurrentValue = p;
		mDifference= v;


		QuatF predictedPos;// = p + v * sendersOneWayTime;
		setOwnersOneWayTime(sendersOneWayTime);
	}

	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_RELIABLE)
	{
		QuatF p;
		xMath::stream::mathRead(*bstream,&p);
	
		mLastValue = p;
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
void xDistributedQuatF::pack(xNStream *bstream)
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
xDistributedQuatF::updateValue(QuatF value,xTimeType currentTime)
{
	bool result  = false;
	
	mLastTime = mCurrentTime;
	mCurrentTime = currentTime;
	mLastDeltaTime = mCurrentTime - mLastTime;
	
	mLastValue = mCurrentValue;
    mCurrentValue = value;
	mDifference = mCurrentValue - mLastValue;
	
	mThresoldTicker +=mLastDeltaTime;

	float lengthDiff  = fabsf(mCurrentValue.angleBetween(mLastValue));
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
		
		QuatF serverDiff  = mCurrentValue-mLastServerValue ;
		float serverDiff2   = fabsf(mCurrentValue.angleBetween(mLastServerValue));



		if (  serverDiff2 > getPredictionSettings()->getMinDifference() )
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