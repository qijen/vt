#ifndef _XDISTRIBUTED_3D_OBJECT_CLASS_H_
#define _XDISTRIBUTED_3D_OBJECT_CLASS_H_

#include "xDistributedBaseClass.h"

class xDistributed3DObjectClass : public xDistributedClass
{


	 public : 
		 xDistributed3DObjectClass();
		 int getFirstUserField();
		 int getUserFieldBitValue(int walkIndex);
		 int getInternalUserFieldIndex(int inputIndex);
		 int getUserFieldCount();
		 
		 void addProperty(const char*name,int type,int predictionType);
		 void addProperty(int nativeType,int predictionType);


	protected : 

};

#endif
