#ifndef _XDISTRIBUTED_SESSION_CLASS_H_
#define _XDISTRIBUTED_SESSION_CLASS_H_

#include "xDistributedBaseClass.h"

class xDistributedSessionClass : public xDistributedClass
{


	 public : 
		 xDistributedSessionClass();
		 int getFirstUserField();
		 int getUserFieldBitValue(int walkIndex);
		 int getInternalUserFieldIndex(int inputIndex);
		 int getUserFieldCount();
		 
		 void addProperty(const char*name,int type,int predictionType);
		 void addProperty(int nativeType,int predictionType);

		 xNString NativeTypeToString(int nativeType);
		 int NativeTypeToValueType(int nativeType);

	protected : 

};

#endif
