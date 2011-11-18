#include "xDistributedString.h"

uxString 
xDistributedString::print(TNL::BitSet32 flags)
{
	return uxString();
}
void 
xDistributedString::updateFromServer(xNStream *stream)
{
	mLastValue = mCurrentValue;
	char value[256];stream->readString(value);
	mCurrentValue = value;
	setValueState(E_DV_UPDATED);
}

void
xDistributedString::updateGhostValue(xNStream *stream)
{
	
	stream->writeString(mCurrentValue.getString(),strlen(mCurrentValue.getString()));
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
void xDistributedString::unpack(xNStream *bstream,float sendersOneWayTime)
{

	mLastValue = mCurrentValue;
	char value[256];bstream->readString(value);
	mCurrentValue = value;

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
void xDistributedString::pack(xNStream *bstream)
{
	bstream->writeString(mCurrentValue.getString(),strlen(mCurrentValue.getString()));
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
xDistributedString::updateValue(TNL::StringPtr value,xTimeType currentTime)
{
	mLastTime = mCurrentTime;
	mCurrentTime = currentTime;
	mLastDeltaTime = mCurrentTime - mLastTime;
	
	mLastValue=TNL::StringPtr(mCurrentValue.getString());
    mCurrentValue = value;
	mThresoldTicker +=mLastDeltaTime;

	

	int flags  = getFlags();
	flags =E_DP_OK;
	bool result  = false;

	if ( strcmp(mCurrentValue,mLastValue) )
	{
		if (mThresoldTicker2 > 50 )
		{
		
			flags =E_DP_NEEDS_SEND;	
			result   = true ;
		}
	}

	if (mThresoldTicker2 > 50 )
	{
		mThresoldTicker2  = 0 ;
		mThresoldTicker  = 0;
	}
	setFlags(flags);
	return result;
}