#include <StdAfx.h>
#include "vtPhysXAll.h"


BOOL PhysicManager::checkDemo(CK3dEntity* a)
{
	return true;

	/*
	if (!a)
	{
		return false;
	}
	//VxVector scale  = vtODE::math::BoxGetZero(a);
	/*if ( scale.x > 400 || scale.y > 100 || scale.z > 400 )
	{
		return false;
	}

	if (GetPMan()->DefaultWorld() && GetPMan()->DefaultWorld()->NumBodies() > 30 )
	{
		return false;
	}
	//return true;
	CKMesh *mesh = (CKMesh *)a->GetCurrentMesh();

	if (mesh)
	{
		int vcount = mesh->GetVertexCount();
		if (vcount ==960 ||vcount ==559 || vcount ==4096 ||vcount ==106 ||vcount ==256 || vcount ==17  || vcount  ==24 ||  vcount == 266 || vcount == 841 )
		{
			return true;
		}
	}
	return false;
	*/
}