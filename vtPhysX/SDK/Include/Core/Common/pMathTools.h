#ifndef __XMATH_TOOLS_H__
#define __XMATH_TOOLS_H__

#include "VxMath.h"
#include "NxQuat.h"

namespace pMath
{

VxVector getFromStream(NxVec3 source);
VxQuaternion getFromStream(NxQuat source);

NxVec3 getFromStream(VxVector source);
NxQuat getFromStream(VxQuaternion source);




/*__inline NxVec3 getFrom(const VxVector& sourcein)
{

	return NxVec3(sourcein.x,sourcein.y,sourcein.z);
}*/

__inline NxVec3 getFrom(VxVector source)
{
	NxVec3 target(0.0f,0.0f,0.0f);
	target.x = source.x;
	target.y = source.y;
	target.z = source.z;
	return NxVec3(source.x,source.y,source.z);
}

NxQuat getFrom(VxQuaternion source);
VxQuaternion getFrom(NxQuat source);

__inline VxVector getFrom(NxVec3 source)
{
	VxVector result;
	source.get(result.v);
	return result;
}

}

#endif
