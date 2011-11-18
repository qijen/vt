#include <StdAfx.h>
#include "pMisc.h"
#include "pCommon.h"

namespace vtAgeia
{

int getHullTypeFromShape(NxShape *shape)
{

	int result = - 1; 
	if (!shape)
	{
		return result;
	}

	int nxType  = shape->getType();
	switch (nxType)
	{
		case NX_SHAPE_PLANE:
			return HT_Plane;
		
		case NX_SHAPE_BOX:
			return HT_Box;
		
		case NX_SHAPE_SPHERE:
			return HT_Sphere;
		
		case NX_SHAPE_CONVEX: 
			return HT_ConvexMesh;
		
		case NX_SHAPE_CAPSULE:
			return HT_Capsule;
		
		case NX_SHAPE_MESH:
			return HT_Mesh;
	}

	return -1;

}


XString getEnumDescription(CKParameterManager* pm,CKGUID parGuide,int parameterSubIndex)
{

	XString result="None";
	int pType = pm->ParameterGuidToType(parGuide);
	CKEnumStruct *enumStruct = pm->GetEnumDescByType(pType);
	if ( enumStruct )
	{
		for (int i = 0 ; i < enumStruct->GetNumEnums() ; i ++ )
		{
			if(i == parameterSubIndex)
			{
				result = enumStruct->GetEnumDescription(i);
			}
		}
	}
	return result;
}

//************************************
// Method:    BoxGetZero
// FullName:  vtAgeia::BoxGetZero
// Access:    public 
// Returns:   VxVector
// Qualifier:
// Parameter: vt3DObject ent
//************************************
VxVector BoxGetZero(CK3dEntity* ent)
{

	VxVector box_s= VxVector(1,1,1);
	if (ent)
	{

		VxMatrix mat = ent->GetWorldMatrix();
		VxVector g;
		Vx3DMatrixToEulerAngles(mat,&g.x,&g.y,&g.z);
		SetEulerDirection(ent,VxVector(0,0,0));
		CKMesh *mesh = ent->GetCurrentMesh();


		if (mesh!=NULL)
		{
			box_s  = mesh->GetLocalBox().GetSize();
		}
		SetEulerDirection(ent,g);                                                                      
	}
	return box_s;
}

//************************************
// Method:    SetEulerDirection
// FullName:  vtAgeia::SetEulerDirection
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CK3dEntity* ent
// Parameter: VxVector direction
//************************************
void SetEulerDirection(CK3dEntity* ent,VxVector direction)
{

	VxVector dir,up,right;
	VxMatrix mat;
	Vx3DMatrixFromEulerAngles(mat,direction.x,direction.y,direction.z);
	dir=(VxVector)mat[2];
	up=(VxVector)mat[1];
	right=(VxVector)mat[0];
	ent->SetOrientation(&dir,&up,&right,NULL,FALSE);

}


}
