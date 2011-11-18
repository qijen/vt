#include "xDistributedInteger.h"
#include "xPredictionSetting.h"
#include "xMathFnc2.h"

uxString 
xDistributedInteger::print(TNL::BitSet32 flags)
{
	return uxString();
}
void 
xDistributedInteger::updateFromServer(TNL::BitStream *stream)
{

	mLastServerValue = stream->readSignedInt(32);
	setValueState(E_DV_UPDATED);
}

void
xDistributedInteger::updateGhostValue(TNL::BitStream *stream)
{
	
	stream->writeSignedInt(mCurrentValue,32);
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
void xDistributedInteger::unpack(TNL::BitStream *bstream,float sendersOneWayTime)
{

	mLastValue = mCurrentValue;
	int value = bstream->readSignedInt(32);
	mCurrentValue = value;
	mDifference= mCurrentValue - mLastValue;
    
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
void xDistributedInteger::pack(TNL::BitStream *bstream)
{
		
	bstream->writeSignedInt(mCurrentValue,32);
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
xDistributedInteger::updateValue(TNL::S32 value,xTimeType currentTime)
{
	mLastTime = mCurrentTime;
	mCurrentTime = currentTime;
	mLastDeltaTime = mCurrentTime - mLastTime;
	
	mLastValue = mCurrentValue;
    mCurrentValue = value;
	mDifference = mCurrentValue - mLastValue;
	
	mThresoldTicker +=mLastDeltaTime;



	float lengthDiff = fabsf((float)mDifference +0.5f);

	int flags  = getFlags();
	flags =E_DP_OK;
	bool result  = false;

	if ( lengthDiff > getPredictionSettings()->getMinDifference() ) 
	{
		if (mThresoldTicker2 >  getPredictionSettings()->getMinSendTime() )
		{
			flags =E_DP_NEEDS_SEND;	
			result   = true ;
		}
	}

	float serverDiff  = (float)(mCurrentValue-mLastServerValue);

	if (  fabsf( serverDiff + .5f) > getPredictionSettings()->getMinDifference() )
	{
		if (mThresoldTicker2 > (getPredictionSettings()->getMinSendTime() * 0.2f) )
		{
			flags =E_DP_NEEDS_SEND;	
			result   = true ;
		}
	}

	if (mThresoldTicker2 >  getPredictionSettings()->getMinSendTime()  )
	{
		mThresoldTicker2  = 0 ;
		mThresoldTicker  = 0;
	}
	setFlags(flags);
	return result;
}