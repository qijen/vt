#include <StdAfx.h>

#include "NxShape.h"
#include "Nxp.h"
#include "pMisc.h"

#include "PhysicManager.h"

namespace vtAgeia
{

bool isChildOf(CK3dEntity*parent,CK3dEntity*test)
{

	CK3dEntity* subEntity = NULL;
	while (subEntity= parent->HierarchyParser(subEntity) )
	{
		if (subEntity == test)
		{
			return true;
		}
	}
	return false;
}

CK3dEntity* findSimilarInSourceObject(CK3dEntity *parentOriginal,CK3dEntity* partentCopied,CK3dEntity *copiedObject,CK3dEntity*prev)
{


	if (!parentOriginal || !copiedObject )
		return NULL;

	if (parentOriginal->GetChildrenCount() < 1)
		return NULL;

	
	if ( prev && prev!=copiedObject && isChildOf(parentOriginal,prev) && !isChildOf(partentCopied,prev) )
			return prev;
			
	
	CK3dEntity *orginalObject= (CK3dEntity*)ctx()->GetObjectByNameAndClass(copiedObject->GetName(),CKCID_3DOBJECT,prev);
	if (orginalObject)
	{

		//----------------------------------------------------------------
		//
		// tests 
		//
		if ( orginalObject==copiedObject )
			findSimilarInSourceObject(parentOriginal,partentCopied,copiedObject,orginalObject);
			

		if ( !isChildOf(parentOriginal,orginalObject)) 
				findSimilarInSourceObject(parentOriginal,partentCopied,copiedObject,orginalObject);
	
		if( isChildOf(partentCopied,orginalObject) )
			findSimilarInSourceObject(parentOriginal,partentCopied,copiedObject,orginalObject);



		return orginalObject;
	}
	return NULL;
}


bool calculateOffsets(CK3dEntity*source,CK3dEntity*target,VxQuaternion &quat,VxVector& pos)
{
	if (!source && !target)
		return false;
	
	CK3dEntity* child = NULL;
	bool isChild = false;
	while (child = source->HierarchyParser(child) )
	{
		if (child  == target )
		{
			isChild = true;
		}
	}

	VxQuaternion refQuad2;
	target->GetQuaternion(&refQuad2,source);
	VxVector relPos;
	target->GetPosition(&relPos,source);
	
	pos = relPos;
	quat = refQuad2;

	return true;

}
int getNbOfPhysicObjects(CK3dEntity *parentObject,int flags/* =0 */)
{
	#ifdef _DEBUG
		assert(parentObject);
	#endif // _DEBUG

	int result = 0;
	//----------------------------------------------------------------
	//
	// parse hierarchy 
	//
	
	CK3dEntity* subEntity = NULL;
	while (subEntity= parentObject->HierarchyParser(subEntity) )
	{

		pRigidBody *body = GetPMan()->getBody(subEntity);
		if (body)
			result++;
	}
	return result;
}
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


int getEnumIndex(CKParameterManager* pm,CKGUID parGuide,XString enumValue)
{

	int pType = pm->ParameterGuidToType(parGuide);

	CKEnumStruct *enumStruct = pm->GetEnumDescByType(pType);
	if ( enumStruct )
	{
		for (int i = 0 ; i < enumStruct->GetNumEnums() ; i ++ )
		{

			if( !strcmp(enumStruct->GetEnumDescription(i),enumValue.Str()) ) 
				return i;
		}
	}
	return 0;
}

XString getEnumDescription(CKParameterManager* pm,CKGUID parGuide)
{

	XString result;
	int pType = pm->ParameterGuidToType(parGuide);
	CKEnumStruct *enumStruct = pm->GetEnumDescByType(pType);
	if ( enumStruct )
	{
		for (int i = 0 ; i < enumStruct->GetNumEnums() ; i ++ )
		{
				result << enumStruct->GetEnumDescription(i);
				result << "=";
				result << enumStruct->GetEnumValue(i);
				if (i < enumStruct->GetNumEnums() -1 )
				{
					result << ",";
				}
		}
	}
	return result;
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
