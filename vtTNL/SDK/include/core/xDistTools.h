#ifndef __XDIST_TOOLS_H_
#define __XDIST_TOOLS_H_

#include "xNetTypes.h"

namespace xDistTools
{
	TNL::StringPtr NativeTypeToString(int nativeType);
	int NativeTypeToValueType(int nativeType);
	int ValueTypeToSuperType(int valueType);
	int SuperTypeToValueType(int superType);
	xNString ValueTypeToString(int valueType);

};


#endif
