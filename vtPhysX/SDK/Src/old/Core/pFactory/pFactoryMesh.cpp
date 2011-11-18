#include <StdAfx.h>
#include "vtPhysXAll.h"

NxCCDSkeleton *pFactory::createCCDSkeleton(CKBeObject *meshReference,int flags)
{

	#ifdef _DEBUG
		assert(meshReference);
	#endif // _DEBUG


	NxCCDSkeleton *result = NULL;
	CKMesh *mesh = NULL;
	
	if (meshReference->GetClassID()==CKCID_MESH )
		mesh = static_cast<CKMesh*>(meshReference);
	else if(	meshReference->GetClassID() == CKCID_3DOBJECT && 
				((CK3dEntity*)meshReference)->GetCurrentMesh() )
		mesh  = ((CK3dEntity*)meshReference)->GetCurrentMesh();


	if (!mesh)
		return NULL;


	int numVerts = mesh->GetVertexCount();
	int numFaces = mesh->GetFaceCount();


	NxReal *vertices = new float[3 * numVerts];
	NxVec3  *verts = new NxVec3[numVerts];

	for (int i  = 0 ; i< numVerts ; i++  )
	{
		VxVector v;
		mesh->GetVertexPosition(i,&v);
		vertices[i * 3] = v.x;
		vertices[i * 3 + 1] =v.y;
		vertices[i * 3 + 2] = v.z;
	}

	NxU32 *indices2  = new NxU32[numFaces*3];

	for(int  j = 0 ; j < numFaces ; j++)
	{

		int findicies[3];
		mesh->GetFaceVertexIndex(j,findicies[0],findicies[1],findicies[2]);
		indices2[ j *3 ]			= findicies[0];
		indices2[ j *3 + 1 ]	= findicies[1];
		indices2[ j *3 + 2 ]	= findicies[2];
	}

	NxSimpleTriangleMesh descr;
	descr.numVertices			= numVerts;
	descr.pointStrideBytes	=  sizeof(NxReal)*3;
	descr.points				= vertices;
	descr.numTriangles		= numFaces;
	descr.triangles			= indices2;
	descr.triangleStrideBytes	= sizeof(NxU32)*3;
	descr.flags				= NX_MF_FLIPNORMALS;

	if (GetPMan()->getPhysicsSDK())
	{
		result  = GetPMan()->getPhysicsSDK()->createCCDSkeleton(descr);
	}


	delete [] vertices;
	delete [] verts;
	delete [] indices2;

	return result;

	
}
void pFactory::createConvexMesh(NxScene *scene,CKMesh *mesh,NxConvexMeshDesc&descr)
{


	
	if (!scene || !mesh)
	{
		return;
	}

	int numVerts = mesh->GetVertexCount();
	int numFaces = mesh->GetFaceCount();
	

	NxReal *vertices = new float[3 * numVerts];
	NxVec3  *verts = new NxVec3[numVerts];

	for (int i  = 0 ; i< numVerts ; i++  )
	{
		VxVector v;
		mesh->GetVertexPosition(i,&v);
		vertices[i * 3] = v.x;
		vertices[i * 3 + 1] =v.y;
		vertices[i * 3 + 2] = v.z;
	}

	NxU32 *indices2  = new NxU32[numFaces*3];
	
	for(int  j = 0 ; j < numFaces ; j++)
	{

		int findicies[3];
		mesh->GetFaceVertexIndex(j,findicies[0],findicies[1],findicies[2]);
		indices2[ j *3 ]			= findicies[0];
		indices2[ j *3 + 1 ]	= findicies[1];
		indices2[ j *3 + 2 ]	= findicies[2];
	}
	descr.numVertices			= numVerts;
	descr.pointStrideBytes	=  sizeof(NxReal)*3;
	descr.points				= vertices;
	descr.numTriangles		= numFaces;
	descr.triangles			= indices2;
	descr.triangleStrideBytes	= sizeof(NxU32)*3;
	descr.flags				= NX_CF_COMPUTE_CONVEX;

/*
	delete [] vertices;
	delete [] verts;
	delete [] indices2;*/

}
void
pFactory::createMesh(NxScene *scene,CKMesh *mesh,NxTriangleMeshDesc&descr)
{

	if (!scene || !mesh)
	{
		return;
	}

	int numVerts = mesh->GetVertexCount();
	int numFaces = mesh->GetFaceCount();
	

	NxReal *vertices = new float[3 * numVerts];
	NxVec3  *verts = new NxVec3[numVerts];

	for (int i  = 0 ; i< numVerts ; i++  )
	{
		VxVector v;
		mesh->GetVertexPosition(i,&v);
		vertices[i * 3] = v.x;
		vertices[i * 3 + 1] =v.y;
		vertices[i * 3 + 2] = v.z;
	}

	NxU32 *indices2  = new NxU32[numFaces*3];
	
	for(int  j = 0 ; j < numFaces ; j++)
	{

		int findicies[3];
		mesh->GetFaceVertexIndex(j,findicies[0],findicies[1],findicies[2]);
		indices2[ j *3 ]			= findicies[0];
		indices2[ j *3 + 1 ]	= findicies[1];
		indices2[ j *3 + 2 ]	= findicies[2];
	}
	descr.numVertices			= numVerts;
	descr.pointStrideBytes	=  sizeof(NxReal)*3;
	descr.points				= vertices;
	descr.numTriangles		= numFaces;
	descr.triangles			= indices2;
	descr.triangleStrideBytes	= sizeof(NxU32)*3;
	descr.flags				= 0;


	/*
	
	delete [] vertices;
	delete [] verts;
	delete [] indices2;

	*/

}
