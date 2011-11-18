#include <StdAfx.h>
#include "vtPhysXAll.h"

//////////////////////////////////////////////////////////////////////////	//////////////////////////////////////////////////////////////////////////
struct clist2
{

	CK3dEntity *part;
	CK3dEntity *obstacle;
	CK3dEntity *sub_obstacle;
	VxVector pos;
	VxVector normal;
	float depth;
	clist2(CK3dEntity* p , CK3dEntity *o,CK3dEntity*so, VxVector po, VxVector n , float d ) :
	part(p) , obstacle(o), sub_obstacle(so), pos(po) , normal (n) , depth (d){}

};
