#include "xDistributedPoint1F.h"
#include "xPredictionSetting.h"
#include "xDistributedPropertyInfo.h"
#include "xMathFnc2.h"

uxString 
xDistributedPoint1F::print(TNL::BitSet32 flags)
{
	return uxString();
}
void 
xDistributedPoint1F::updateFromServer(TNL::BitStream *stream)
{
	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_PREDICTED)
	{
		TNL::F32 p;	stream->read(&p);
		TNL::F32 v;	stream->read(&v);
	    
		mLastServerValue = p;
		mLastServerDifference = v;
	}
	
	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_RELIABLE)
	{
		TNL::F32 p;	stream->read(&p);
		mCurrentValue = p;
		mLastServerValue = p;
	}
	setValueState(E_DV_UPDATED);

}

void
xDistributedPoint1F::updateGhostValue(TNL::BitStream *stream)
{

	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_PREDICTED)
	{
		stream->write(mCurrentValue);
		stream->write(mDifference);
	}

	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_RELIABLE)
	{
		stream->write(mCurrentValue);
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
void xDistributedPoint1F::unpack(TNL::BitStream *bstream,float sendersOneWayTime)
{
	
	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_PREDICTED)
	{
		sendersOneWayTime *= 0.001f;
		

		TNL::F32 p;	bstream->read(&p);
		TNL::F32 v;	bstream->read(&v);

		TNL::F32 predictedPos  = p + v * sendersOneWayTime;
		setOwnersOneWayTime(sendersOneWayTime);

		mLastValue = mCurrentValue;
		mCurrentValue = predictedPos;
		mDifference= mCurrentValue - mLastValue;
	}

	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_RELIABLE)
	{

		TNL::F32 p;	bstream->read(&p);
		mLastValue = p;
		mLastServerValue = p;
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
void xDistributedPoint1F::pack(TNL::BitStream *bstream)
{
	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_PREDICTED)
	{
		bstream->write(mCurrentValue);
		bstream->write(mDifference);
	}

	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_RELIABLE)
	{
		bstream->write(mCurrentValue);
	}


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
xDistributedPoint1F::updateValue(TNL::F32 value,xTimeType currentTime)
{
	mLastTime = mCurrentTime;
	mCurrentTime = currentTime;
	mLastDeltaTime = mCurrentTime - mLastTime;
	
	mLastValue = mCurrentValue;
    mCurrentValue = value;
	mDifference = mCurrentValue - mLastValue;
	
	mThresoldTicker +=mLastDeltaTime;

	float lengthDiff = fabsf(mDifference);
	float timeDiffMs = ((float)mThresoldTicker) / 1000.f;

	int flags  = getFlags();
	flags =E_DP_OK;
	bool result  = false;

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

		TNL::F32 serverDiff  = mCurrentValue-mLastServerValue ;
		
		if (  fabsf( serverDiff) > getPredictionSettings()->getMinDifference() )
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

	if (getPropertyInfo()->mPredictionType == E_PTYPE_RELIABLE)
	{
		flags =E_DP_NEEDS_SEND;	
		result   = true ;
	}

	setFlags(flags);
	return result;
}