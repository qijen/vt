#ifndef __xDistributedPropertyInfo_h
#define __xDistributedPropertyInfo_h

#include "xNetTypes.h"

class xDistributedPropertyInfo
{

	public:
		xDistributedPropertyInfo() :  mName(NULL) , mValueType(-1) , mNativeType(-1) , mPredictionType(-1)		{		}
		xDistributedPropertyInfo( xNString name , int valueType,int nativeType, int predictionType ) :  mName(name) , mValueType(valueType) , mNativeType(nativeType) , mPredictionType(predictionType)
		{
		}
		virtual ~xDistributedPropertyInfo(){}
		xNString mName;
		int mValueType;
		int mNativeType;
		int mPredictionType;

};

#endif
