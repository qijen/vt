#ifndef __XNEtWORK_TYPES_H_
#define __XNEtWORK_TYPES_H_
#include <Prereqs.h>

//#include "CKAll.h"
#include "tnl.h"
#include "tnlTypes.h"
#include "tnlVector.h"

typedef unsigned int xTimeType;

enum vtNetObjectType
{

	UnknownType       = BIT(0),
	ShipType          = BIT(1),
	BarrierType       = BIT(2),
	MoveableType      = BIT(3),
	Type2D          = BIT(5),
	Type3D          = BIT(5),
	ResourceItemType  = BIT(6),
	MineType          = BIT(10),
	TestItemType      = BIT(11),
	FlagType          = BIT(12),
	DeletedType       = BIT(30),
	MotionTriggerTypes= ShipType | ResourceItemType | TestItemType,
	AllObjectTypes    = 0xFFFFFFFF,
};
#endif
