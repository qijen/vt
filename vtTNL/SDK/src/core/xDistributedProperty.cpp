#include "xDistributedProperty.h"
#include "xPredictionSetting.h"


uxString 
xDistributedProperty::print(TNL::BitSet32 flags)
{
	return uxString();
}
xDistributedProperty::xDistributedProperty(xDistributedPropertyInfo *propInfo) : m_PropertyInfo( propInfo)
{
	m_BlockIndex = 0;
	mLastTime = 0;
	mThresoldTicker =0;
	mCurrentTime =0;
	mLastDeltaTime =0;

	mThresoldTicker2 =0.0f;
	mLastTime2  = 0.0f;
	mLastDeltaTime2 =0.0;
	m_PredictionSettings  = new xPredictionSetting();
	m_PredictionSettings->setMinDifference(1.0f);
	m_PredictionSettings->setMinSendTime(50.0f);
	m_PredictionSettings->setPredictionFactor( 10.0f);

}

void
xDistributedProperty::advanceTime(xTimeType time1,float lastDeltaTime2 )
{

	mLastTime2 = mLastTime2 + lastDeltaTime2;
	mLastDeltaTime2  = lastDeltaTime2;
	mThresoldTicker2  = mThresoldTicker2 + lastDeltaTime2;

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
void xDistributedProperty::unpack( xNStream *bstream,float sendersOneWayTime )
{

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
void xDistributedProperty::pack(xNStream *bstream)
{

}
