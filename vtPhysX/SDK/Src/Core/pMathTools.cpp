#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "pMathTools.h"

namespace  pMath
{

VxQuaternion getFromStream(NxQuat source)
{

	VxQuaternion result;

	result.x = source.x;
	result.y  = source.z;
	result.z  = source.y;
	result.w  = source.w;

	return result;


}
VxVector getFromStream(NxVec3 source)
{

	VxVector result;
	result.x = source.x;
	result.y  = source.z;
	result.z  = source.y;
	return result;

}


NxQuat getFrom(VxQuaternion source)
{
	NxQuat result;
	result.setx(-source.x);
	result.sety(-source.y);
	result.setz(-source.z);
	result.setw(source.w);
	return result;
}
 VxQuaternion getFrom(NxQuat source)
{

	VxQuaternion result;
	source.getXYZW(result.v);
	result.x  = -result.x;
	result.z  = -result.z;
	result.y  = -result.y;
	return result;
}

}

