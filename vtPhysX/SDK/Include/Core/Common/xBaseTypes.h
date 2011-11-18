/********************************************************************
	created:	2009/02/17
	created:	17:2:2009   8:23
	filename: 	x:\ProjectRoot\svn\local\vtPhysX\SDK\Include\Core\Common\xBaseTypes.h
	file path:	x:\ProjectRoot\svn\local\vtPhysX\SDK\Include\Core\Common 
	file base:	xBaseTypes
	file ext:	h
	author:		Günter Baumgart
	
	purpose:	Type definitions, Arrays, ...
*********************************************************************/
#ifndef __X_BASE_TYPES_H__
#define __X_BASE_TYPES_H__


//################################################################
//
// Float, Integers, Boolean
//

#ifndef u32
	typedef unsigned int u32;
#endif

namespace xBase
{
	typedef float xReal32;
	typedef int xS32;
	typedef unsigned int xU32;
	typedef bool xBool;
	typedef unsigned short		xU16;
}

using xBase::xS32;
using xBase::xU32;
using xBase::xReal32;
using xBase::xBool;
using xBase::xU16;

//################################################################
//
//	Containers 
//

#include<stdlib.h>
#include <map>
#include <vector>

#endif // __XBASETYPES_H__