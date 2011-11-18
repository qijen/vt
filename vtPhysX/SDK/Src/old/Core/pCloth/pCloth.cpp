#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "Stream.h"
#include "cooking.h"



pCloth::~pCloth()
{



	if (!getCloth())
	{
		return;
	}

	getWorld()->getScene()->releaseCloth(*mCloth);
	//getWorld()->getScene()->
	releaseReceiveBuffers();
	setEntityID(-1);


}
void pCloth::detachFromShape(CKBeObject *shape)
{
	if (!shape)
	{
		return;
	}
	NxShape *aShape = getWorld()->getShapeByEntityID(shape->GetID());
	if (aShape)
	{
		getCloth()->detachFromShape(aShape);
	}

}
void pCloth::dominateVertex(int vertexId, float expirationTime, float dominanceWeight)
{
	getCloth()->dominateVertex(vertexId,expirationTime,dominanceWeight);
}

void pCloth::freeVertex(const int vertexId)
{
	getCloth()->freeVertex(vertexId);
}

void pCloth::attachVertexToGlobalPosition(const int vertexId, const VxVector &pos)
{

	getCloth()->attachVertexToGlobalPosition(vertexId,getFrom(pos));

}
void pCloth::attachVertexToShape(int vertexId, CKBeObject *shape, const VxVector &localPos, int attachmentFlags)
{

	if (!shape)
	{
		return;
	}

	if (!getWorld())
		return;

	NxShape *aShape = getWorld()->getShapeByEntityID(shape->GetID());
	if (aShape)
	{
		getCloth()->attachVertexToShape(vertexId,aShape,getFrom(localPos),attachmentFlags);
	}
}

void pCloth::attachToCore(CK3dEntity *body, float impulseThreshold, float penetrationDepth, float maxDeformationDistance)
{

	pRigidBody *rbody = GetPMan()->getBody(body);
	if (!body)
	{
		return ;
	}

	if (rbody->getWorld() != getWorld() )
	{
		return;
	}

	getCloth()->attachToCore(rbody->getActor(),impulseThreshold,penetrationDepth,maxDeformationDistance);


}

void pCloth::attachToCollidingShapes(int attachmentFlags)
{
	mCloth->attachToCollidingShapes(attachmentFlags);
}
void pCloth::attachToShape(CKBeObject *shape, int attachmentFlags)
{

	if (!shape)
	{
		return;
	}
	
	if (!getWorld())
		return;

	NxShape *aShape = getWorld()->getShapeByEntityID(shape->GetID());
	if (aShape)
	{
		getCloth()->attachToShape(aShape,attachmentFlags);
	}
	

}

void pCloth::setCollisionResponseCoefficient(float coefficient)
{
	mCloth->setCollisionResponseCoefficient(coefficient);
}
void pCloth::addDirectedForceAtPos(const VxVector& position, const VxVector& force, float radius, ForceMode mode /* = FM_Force */)
{
	mCloth->addDirectedForceAtPos(getFrom(position),getFrom(force),(NxForceMode)mode );
}
void pCloth::addForceAtVertex(const VxVector& force, int vertexId, ForceMode mode /* = FM_Force */)
{
	mCloth->addForceAtVertex(getFrom(force),vertexId,(NxForceMode)mode);
}

void pCloth::addForceAtPos(const VxVector& position, float magnitude, float radius, ForceMode mode /* = FM_Force */)
{
	mCloth->addForceAtPos(getFrom(position),magnitude,radius,(NxForceMode)mode);
}

void pCloth::wakeUp(float wakeCounterValue /* = NX_SLEEP_INTERVAL */)
{
	mCloth->wakeUp(wakeCounterValue);
}

void pCloth::putToSleep()
{
	mCloth->putToSleep();
}

void pCloth::setFlags(int flags)
{
	mCloth->setFlags(flags);
}
void pCloth::setSleepLinearVelocity(float threshold)
{
	mCloth->setSleepLinearVelocity(threshold);
}
void pCloth::setExternalAcceleration(VxVector acceleration)
{
	mCloth->setExternalAcceleration(getFrom(acceleration));
}
void pCloth::setWindAcceleration(VxVector acceleration)
{
	mCloth->setWindAcceleration(getFrom(acceleration));
}
void pCloth::setMinAdhereVelocity(float velocity)
{
	mCloth->setMinAdhereVelocity(velocity);
}
void pCloth::setToFluidResponseCoefficient(float coefficient)
{
	mCloth->setToFluidResponseCoefficient(coefficient);
}
void pCloth::setFromFluidResponseCoefficient(float coefficient)
{
	mCloth->setFromFluidResponseCoefficient(coefficient);
}
void pCloth::setAttachmentResponseCoefficient(float coefficient)
{
	mCloth->setAttachmentResponseCoefficient(coefficient);
}
void pCloth::setVelocity(const VxVector& velocity, int vertexId)
{
	mCloth->setVelocity(getFrom(velocity),vertexId);
}
void pCloth::setValidBounds(const VxBbox& validBounds)
{
	NxBounds3 box;
	box.set( getFrom(validBounds.Min), getFrom(validBounds.Max));
	mCloth->setValidBounds(box);
}

void pCloth::setGroup(int collisionGroup)
{
	mCloth->setGroup(collisionGroup);
}
void pCloth::setSolverIterations(int iterations)
{
	mCloth->setSolverIterations(iterations);
}
void pCloth::setThickness(float thickness)
{
	mCloth->setThickness(thickness);
}
void pCloth::setAttachmentTearFactor(float factor)
{
	mCloth->setAttachmentTearFactor(factor);
}
void pCloth::setTearFactor(float factor)
{
	mCloth->setTearFactor(factor);
}
void pCloth::setPressure(float pressure)
{
	mCloth->setPressure(pressure);
}
void pCloth::setFriction(float friction)
{
	mCloth->setFriction(friction);
}
void pCloth::setDampingCoefficient(float dampingCoefficient)
{
	mCloth->setDampingCoefficient(dampingCoefficient);
}

void pCloth::setStretchingStiffness(float stiffness)
{
	mCloth->setStretchingStiffness(stiffness);
}
void pCloth::setBendingStiffness(float stiffness)
{
	mCloth->setBendingStiffness(stiffness);
}

void pCloth::updateVirtoolsMesh()
{

	NxMeshData *data  = getReceiveBuffers();
	CK3dEntity *srcEntity =(CK3dEntity*) GetPMan()->GetContext()->GetObject(getEntityID());
	if (!srcEntity)
	{
		return;
	}

	/*
	if (getCloth()->isSleeping())
	{
		return;
	}

	*/
	CKMesh *mesh  = srcEntity->GetCurrentMesh();
	NxReal *vertices = (NxReal*)data->verticesPosBegin;
	

	VxVector pos;
	srcEntity->GetPosition(&pos);
	for (int i  = 0 ; i< mesh->GetVertexCount() ; i++  )
	{
		VxVector v;
		v.x = vertices[i * 3];
		v.y = vertices[i * 3 + 1];
		v.z  = vertices[i * 3 + 2];
		VxVector outIV;
		srcEntity->InverseTransform(&outIV,&v);
		mesh->SetVertexPosition(i,&outIV);
	}
	int t  = 3;

}

bool pCloth::cookMesh(NxClothMeshDesc* desc)
{

	// we cook the mesh on the fly through a memory stream
	// we could also use a file stream and pre-cook the mesh
	MemoryWriteBuffer wb;
	int dValid  = desc->isValid();


	bool status = InitCooking();
	if (!status) {
		xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Couldn't initiate cooking lib!");
		return NULL;
	}

	bool success = CookClothMesh(*desc, wb);

	if (!success) 
		return false;

	MemoryReadBuffer rb(wb.data);

	mClothMesh = GetPMan()->getPhysicsSDK()->createClothMesh(rb);

	CloseCooking();

	return true;

}

bool pCloth::generateMeshDesc(pClothDesc cDesc,NxClothMeshDesc *desc, CKMesh*mesh)
{

	if (!mesh)
	{
		return false;
	}

	int numVerts = mesh->GetVertexCount();
	int numFaces = mesh->GetFaceCount();

	// allocate flag buffer
	if(desc->vertexFlags == 0)
		desc->vertexFlags = malloc(sizeof(NxU32)*desc->numVertices);

	// create tear lines
	NxU32* flags = (NxU32*)desc->vertexFlags;

	NxReal *vertices = new float[3 * numVerts];
	//NxVec3  *verts = new NxVec3[numVerts];

	for (int i  = 0 ; i< numVerts ; i++  )
	{
		VxVector v;
		mesh->GetVertexPosition(i,&v);
		vertices[i * 3] = v.x;
		vertices[i * 3 + 1] =v.y;
		vertices[i * 3 + 2] = v.z;

		

		if (desc->flags & NX_CLOTH_MESH_TEARABLE)
		{
			DWORD vColor = mesh->GetVertexColor(i);
			DWORD cColor = RGBAFTOCOLOR(&cDesc.tearVertexColor);
			if (vColor == cColor )
			{
				int op2 =0;
				flags[i]  = NX_CLOTH_VERTEX_TEARABLE;
				int k  = 0 ;
				k++;
			}
		}
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
	desc->numVertices			= numVerts;
	desc->pointStrideBytes	=  sizeof(NxReal)*3;
	desc->points				= vertices;
	desc->numTriangles		= numFaces;
	desc->triangles			= indices2;
	desc->triangleStrideBytes	= sizeof(NxU32)*3;
	desc->flags				= 0;
	desc->vertexMasses				= 0;
	desc->vertexFlags				= 0;
	desc->flags						= NX_CLOTH_MESH_WELD_VERTICES;
	desc->weldingDistance			= 0.0001f;

	return true;


}


void pCloth::allocateClothReceiveBuffers(int numVertices, int numTriangles)
{

	// here we setup the buffers through which the SDK returns the dynamic cloth data
		// we reserve more memory for vertices than the initial mesh takes
		// because tearing creates new vertices
		// the SDK only tears cloth as long as there is room in these buffers
		
	
	mReceiveBuffers  = new NxMeshData();

	NxU32 maxVertices = 3 * numVertices;
	mReceiveBuffers->verticesPosBegin = (NxVec3*)malloc(sizeof(NxVec3)*maxVertices);
	mReceiveBuffers->verticesNormalBegin = (NxVec3*)malloc(sizeof(NxVec3)*maxVertices);
	mReceiveBuffers->verticesPosByteStride = sizeof(NxVec3);
	mReceiveBuffers->verticesNormalByteStride = sizeof(NxVec3);
	mReceiveBuffers->maxVertices = maxVertices;
	mReceiveBuffers->numVerticesPtr = (NxU32*)malloc(sizeof(NxU32));

	// the number of triangles is constant, even if the cloth is torn
	NxU32 maxIndices = 3*numTriangles;
	mReceiveBuffers->indicesBegin = (NxU32*)malloc(sizeof(NxU32)*maxIndices);
	mReceiveBuffers->indicesByteStride = sizeof(NxU32);
	mReceiveBuffers->maxIndices = maxIndices;
	mReceiveBuffers->numIndicesPtr = (NxU32*)malloc(sizeof(NxU32));

	// the parent index information would be needed if we used textured cloth
	NxU32 maxParentIndices = maxVertices;
	mReceiveBuffers->parentIndicesBegin = (NxU32*)malloc(sizeof(NxU32)*maxParentIndices);
	mReceiveBuffers->parentIndicesByteStride = sizeof(NxU32);
	mReceiveBuffers->maxParentIndices = maxParentIndices;
	mReceiveBuffers->numParentIndicesPtr = (NxU32*)malloc(sizeof(NxU32));

	// init the buffers in case we want to draw the mesh 
	// before the SDK as filled in the correct values
	*mReceiveBuffers->numVerticesPtr = 0;
	*mReceiveBuffers->numIndicesPtr = 0;
}

void pCloth::releaseReceiveBuffers()
{

	// Parent Indices is always allocated
/*	free (mReceiveBuffers.parentIndicesBegin);

	mReceiveBuffers.setToDefault();*/

}

void pCloth::releaseMeshDescBuffers(const NxClothMeshDesc* desc)
{

	NxVec3* p = (NxVec3*)desc->points;
	NxU32* t = (NxU32*)desc->triangles;
	NxReal* m = (NxReal*)desc->vertexMasses;
	NxU32* f = (NxU32*)desc->vertexFlags;
	free(p);
	free(t);
	free(m);
	free(f);
}


pCloth::pCloth()
{

}

/*----------------------------------------------------------------------------*/

pClothDesc::pClothDesc()
{
	//setToDefault();

}

/*----------------------------------------------------------------------------*/

void pClothDesc::setToDefault()
{
	thickness = 0.01f;
	density = 1.0f;
	bendingStiffness = 1.0f;

	stretchingStiffness = 1.0f;
	dampingCoefficient = 0.5f;
	friction = 0.5f;

	pressure = 1.0f;
	tearFactor = 1.5f;
	attachmentTearFactor = 1.5f;

	attachmentResponseCoefficient = 0.2f;
	attachmentTearFactor = 1.5f;
	collisionResponseCoefficient = 0.2f;
	toFluidResponseCoefficient = 1.0f;

	fromFluidResponseCoefficient = 1.0f;
	minAdhereVelocity = 1.0f;
	flags = PCF_Gravity|PCF_CollisionTwoway;

	solverIterations = 5;
	wakeUpCounter = NX_SLEEP_INTERVAL;
	sleepLinearVelocity = -1.0f;
	collisionGroup = 0;
	forceFieldMaterial = 0;
	externalAcceleration  =VxVector(0.0f, 0.0f, 0.0f);
	windAcceleration=VxVector(0.0f, 0.0f, 0.0f);
	groupsMask.bits0 = 0;
	groupsMask.bits1 = 0;
	groupsMask.bits2 = 0;
	groupsMask.bits3 = 0;
	validBounds = VxBbox();
	relativeGridSpacing = 0.25f;
	tearVertexColor.Set(1.0f);


}

/*----------------------------------------------------------------------------*/

bool pClothDesc::isValid() const
{
	//	if (flags & NX_CLF_SELFCOLLISION) return false;	// not supported at the moment


	if(thickness < 0.0f) return false;
	if(density <= 0.0f) return false;
	if(bendingStiffness < 0.0f || bendingStiffness > 1.0f) return false;
	if(stretchingStiffness <= 0.0f || stretchingStiffness > 1.0f) return false;
	if(pressure < 0.0f) return false;
	if(tearFactor <= 1.0f) return false;
	if(attachmentTearFactor <= 1.0f) return false;
	if(solverIterations < 1) return false;
	if(friction < 0.0f || friction > 1.0f) return false;
	if(dampingCoefficient < 0.0f || dampingCoefficient > 1.0f) return false;
	if(collisionResponseCoefficient < 0.0f) return false;
	if(wakeUpCounter < 0.0f) return false;
	if(attachmentResponseCoefficient < 0.0f || attachmentResponseCoefficient > 1.0f) return false;
	if(toFluidResponseCoefficient < 0.0f) return false;
	if(fromFluidResponseCoefficient < 0.0f) return false;
	if(minAdhereVelocity < 0.0f) return false;
	if(relativeGridSpacing < 0.01f) return false;
	if(collisionGroup >= 32) return false; // We only support 32 different collision groups
	return true;
}
