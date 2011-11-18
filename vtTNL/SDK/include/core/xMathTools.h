#ifndef __XMATH_TOOLS_H__
#define __XMATH_TOOLS_H__

#include "Prereqs.h"
#include "xTNLInternAll.h"
#include "xPoint.h"
#include "xQuat.h"
namespace xMath
{

__inline void convert(Point3F &target,VxVector source)
{
	target.x = source.x;
	target.y = source.y;
	target.z = source.z;
}
__inline Point3F getFrom(VxVector source)
{
	return Point3F(source.x,source.y,source.z);
}
__inline VxVector getFrom(Point3F source){return VxVector(source.x,source.y,source.z);}

//////////////////////////////////////////////////////////////////////////
__inline void convert(Point2F &target,Vx2DVector source)
{
	target.x = source.x;
	target.y = source.y;
}
__inline Point2F getFrom(Vx2DVector  source)
{
	return Point2F(source.x,source.y);
}
__inline Vx2DVector getFrom(Point2F source){return Vx2DVector(source.x,source.y);}
//////////////////////////////////////////////////////////////////////////

__inline QuatF getFrom(VxQuaternion  source)
{
	return QuatF(source.x,source.y,source.z,source.w);
}
__inline VxQuaternion getFrom(QuatF source){return VxQuaternion(source.x,source.y,source.z,source.w);}
__inline void convert(QuatF &target,VxQuaternion source)
{
	target.x = source.x;
	target.y = source.y;
	target.z = source.z;
	target.w = source.w;

}




}

#endif
