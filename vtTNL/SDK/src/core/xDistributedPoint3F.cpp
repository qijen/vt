#include "xDistributedPoint3F.h"
#include "xMathStream.h"
#include "xPredictionSetting.h"
#include "xDistributedPropertyInfo.h"


uxString 
xDistributedPoint3F::print(TNL::BitSet32 flags)
{
	return uxString();
}
void 
xDistributedPoint3F::updateFromServer(xNStream *stream)
{


	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_PREDICTED)
	{
		TNL::Point3f p;
		stream->readPointCompressed(&p,1.0f);
		Point3F realPos(p.x,p.y,p.z);

		TNL::Point3f v;
		stream->readPointCompressed(&v,1.0f);
		Point3F velocity(v.x,v.y,v.z);

		mLastServerValue = realPos;
		mLastServerDifference = velocity;
	}

	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_RELIABLE)
	{
		TNL::Point3f p;
		stream->readPointCompressed(&p,1.0f);
		Point3F realPos(p.x,p.y,p.z);
		mLastServerValue = realPos;
	}

	setValueState(E_DV_UPDATED);


	//xLogger::xLog(ELOGINFO,XL_START,"\n vel: %f,%f,%f",velocity.x,velocity.y,velocity.z );
}

void
xDistributedPoint3F::updateGhostValue(xNStream *stream)
{
	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_PREDICTED)
	{
		TNL::Point3f p;

		p.x  = mCurrentValue.x;
		p.y  = mCurrentValue.y;
		p.z = mCurrentValue.z;
		stream->writePointCompressed(p,1.0f);

		TNL::Point3f v;
		v.x  = mDifference.x;
		v.y  = mDifference.y;
		v.z = mDifference.z;
		stream->writePointCompressed(v,1.0f);
	}

	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_RELIABLE)
	{
		TNL::Point3f p;

		p.x  = mCurrentValue.x;
		p.y  = mCurrentValue.y;
		p.z = mCurrentValue.z;
		stream->writePointCompressed(p,1.0f);
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
void xDistributedPoint3F::unpack(xNStream *bstream,float sendersOneWayTime)
{
	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_PREDICTED)
	{
		sendersOneWayTime *= 0.001f;

		TNL::Point3f p;
		bstream->readPointCompressed(&p,1.0f);
		Point3F realPos(p.x,p.y,p.z);


		TNL::Point3f v;
		bstream->readPointCompressed(&v,1.0f);
		Point3F velocity(v.x,v.y,v.z);

		Point3F predictedPos  = realPos + velocity * sendersOneWayTime;
		
		
		setOwnersOneWayTime(sendersOneWayTime);

		mLastValue = mCurrentValue;
		mCurrentValue = predictedPos;
		mDifference= mCurrentValue - mLastValue;
	}

	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_RELIABLE)
	{

		TNL::Point3f p;
		bstream->readPointCompressed(&p,1.0f);
		Point3F realPos(p.x,p.y,p.z);

		mLastValue = realPos;
		mLastServerValue = realPos;
		mCurrentValue = realPos;
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
void xDistributedPoint3F::pack(xNStream *bstream)
{

	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_PREDICTED)
	{

		TNL::Point3f p;

		p.x  = mCurrentValue.x;
		p.y  = mCurrentValue.y;
		p.z = mCurrentValue.z;
		bstream->writePointCompressed(p,1.0f);

		TNL::Point3f v;
		v.x  = mDifference.x;
		v.y  = mDifference.y;
		v.z = mDifference.z;
		bstream->writePointCompressed(v,1.0f);
	}


	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_RELIABLE)
	{
		TNL::Point3f p;

		p.x  = mCurrentValue.x;
		p.y  = mCurrentValue.y;
		p.z = mCurrentValue.z;
		bstream->writePointCompressed(p,1.0f);

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
xDistributedPoint3F::updateValue(Point3F value,xTimeType currentTime)
{


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
	bool result  = false;

	if (this->getPropertyInfo()->mPredictionType == E_PTYPE_PREDICTED)
	{
		if (lengthDiff > getPredictionSettings()->getMinDifference() )
		{
			if (mThresoldTicker2 > getPredictionSettings()->getMinSendTime())
			{
				
				flags =E_DP_NEEDS_SEND;	
				result   = true ;
				

				//xLogger::xLog(ELOGINFO,XL_START,"mThresoldTicker2: %f",mThresoldTicker2);
			}
		}

		Point3F serverDiff  = mCurrentValue-mLastServerValue ;
		
		if (  fabsf( serverDiff.len()) > getPredictionSettings()->getMinDifference() )
		{
			if (mThresoldTicker2 > (getPredictionSettings()->getMinSendTime() * 0.2f) )
			{
				flags =E_DP_NEEDS_SEND;	
				result   = true ;
				//xLogger::xLog(ELOGINFO,XL_START,"mThresoldTicker2: %f",mThresoldTicker2);
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
		mLastValue = value;
		mCurrentValue = value;
	}

	setFlags(flags);
	return result;
}