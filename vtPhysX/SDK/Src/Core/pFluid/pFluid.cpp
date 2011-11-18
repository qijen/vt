#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "pFluid.h"
#include "pFluidEmitter.h"
#include "CK3dPointCloud.h"


#ifdef HAS_FLUIDS

pFluidEmitter*pFluid::createEmitter(const pFluidEmitterDesc& desc)
{

	NxFluidEmitterDesc eDesc ;
	eDesc.setToDefault();

	pFactory::Instance()->copyToEmitterDesc(eDesc,desc);

	int valid = eDesc.isValid();
	if (!valid)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Emitter Description not Valid !");
		return NULL;
	}

	CK3dEntity*entityReference = (CK3dEntity*)ctx()->GetObject(desc.entityReference);
	if (!entityReference)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"You must set a reference object ID in .referenceEntity");
		return NULL;
	}


	eDesc.relPose.M.id();
	eDesc.relPose.M.rotX(-NxHalfPiF32);
	eDesc.relPose.t = NxVec3(0,1.1f,0);

	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////

	NxFluidEmitter *emitter   = getFluid()->createEmitter(eDesc);

	if (!emitter)
		return NULL;
	
	pFluidEmitter * result  = new pFluidEmitter();
	result->setEmitter(emitter);
	result->setFluid(this);
	result->setEntityReference(entityReference->GetID());
	VxVector pos;
	if (desc.frameShape)
	{
		desc.frameShape->GetPosition(&pos);
	}
	emitter->setGlobalPosition(getFrom(pos));
	emitter->userData = result;

	//////////////////////////////////////////////////////////////////////////
	//
	//		Render Settings    :  
	//

	pFluidRenderSettings *rSettings  =  new pFluidRenderSettings(ctx(),desc.entityReference,"pFluidEmitter");
	rSettings->setToDefault();
	rSettings->setEmitter(result);

	return result;
	
}


CK3dEntity*pFluid::getParticleObject()
{

	CK3dEntity* result = (CK3dEntity*)ctx()->GetObject(entityID);
	return result;

}


void pFluid::updateVirtoolsMesh()
{

/*
	updateCloud();
	return;*/
	//todo, move this into a small billboard rendering lib.
	if (!mRenderBuffer)
	{
		unsigned sizeFloat = mMaxParticles * 3 * 4;
		mRenderBuffer = new float[sizeFloat];

		if (mTrackUserData)
		{
			mRenderBufferUserData = new float[mMaxParticles * 4 * 4];
		}

	}

	return ;

	CK3dEntity *dstEnt  = getParticleObject();
	CKMesh *mesh  = getParticleObject()->GetCurrentMesh();

	if (!dstEnt  || !mesh )
	{
		return;
	}
	
	if (mParticleBuffer)
	{

		for (int i = 0 ; i < mesh->GetVertexCount() ; i++)
		{
			
			pParticle *p  = &mParticleBuffer[i];
			if (p)
			{
				VxVector v  = getFrom(p->position);
				VxVector outIV;
				getParticleObject()->InverseTransform(&outIV,&v);
				mesh->SetVertexPosition(i,&outIV);

			}
		}
	}

	mesh->VertexMove();
}

pFluid::pFluid(NxFluidDesc &desc, bool trackUserData, bool provideCollisionNormals, const VxVector& color, float particleSize) :

	mParticleBufferNum(0),
	mParticleBuffer(NULL),
	mFluid(NULL),
	mTrackUserData(trackUserData),
	mMyParticleBuffer(NULL),
	mCreatedParticleIdsNum(0),
	mCreatedParticleIds(NULL),
	mDeletedParticleIdsNum(0),
	mDeletedParticleIds(NULL),
	mParticleColor(color),
	mParticleSize(particleSize),
	mRenderBuffer(NULL),
	mRenderBufferUserData(NULL)
{

	mMaxParticles = desc.maxParticles;
	mParticleBuffer = new pParticle[mMaxParticles];
	desc.userData = this;

	entityID = 0;


	//Setup particle write data.
	NxParticleData particleData;
	particleData.numParticlesPtr = &mParticleBufferNum;
	particleData.bufferPos = &mParticleBuffer[0].position.x;
	particleData.bufferPosByteStride = sizeof(pParticle);
	particleData.bufferVel = &mParticleBuffer[0].velocity.x;
	particleData.bufferVelByteStride = sizeof(pParticle);
	particleData.bufferDensity = &mParticleBuffer[0].density;
	particleData.bufferDensityByteStride = sizeof(pParticle);
	particleData.bufferLife = &mParticleBuffer[0].lifetime;
	particleData.bufferLifeByteStride = sizeof(pParticle);
	particleData.bufferId = &mParticleBuffer[0].id;
	particleData.bufferIdByteStride = sizeof(pParticle);

	if (provideCollisionNormals)
	{
		particleData.bufferCollisionNormal = &mParticleBuffer[0].collisionNormal.x;
		particleData.bufferCollisionNormalByteStride = sizeof(pParticle);
	}

	desc.particlesWriteData = particleData;

	//User data buffers
	if (mTrackUserData)
	{
		//mMyParticleBuffer = new MyParticle[mMaxParticles];
		mCreatedParticleIds = new NxU32[mMaxParticles];
		mDeletedParticleIds = new NxU32[mMaxParticles];

		//Setup id write data.
		NxParticleIdData idData;

		//Creation
		idData.numIdsPtr = &mCreatedParticleIdsNum;
		idData.bufferId = mCreatedParticleIds;
		idData.bufferIdByteStride = sizeof(NxU32);
		desc.particleCreationIdWriteData = idData;

		//Deletion
		idData.numIdsPtr = &mDeletedParticleIdsNum;
		idData.bufferId = mDeletedParticleIds;
		idData.bufferIdByteStride = sizeof(NxU32);
		desc.particleDeletionIdWriteData = idData;
	}

	
}



pFluidDesc::pFluidDesc()
{

	setToDefault();
}

void pFluidDesc::setToDefault()
{

	maxParticles				= 500;
	numReserveParticles			= 0;
	restParticlesPerMeter		= 50.0f;
	restDensity					= 1000.0f;
	kernelRadiusMultiplier		= 1.2f;
	motionLimitMultiplier		= 3.0f * kernelRadiusMultiplier;
	collisionDistanceMultiplier = 0.1f * kernelRadiusMultiplier;
	packetSizeMultiplier		= 16;
	stiffness					= 20.0f;
	viscosity					= 6.0f;
	surfaceTension				= 0.0f;
	damping						= 0.0f;
	fadeInTime					= 0.0f;
	externalAcceleration.Set(0.0f,0.0f,0.0f);
	projectionPlane.set(NxVec3(0.0f, 0.0f, 1.0f), 0.0f);
	restitutionForStaticShapes	= 0.5f;
	dynamicFrictionForStaticShapes = 0.05f;
	staticFrictionForStaticShapes = 0.05f;
	attractionForStaticShapes	= 0.0f;
	restitutionForDynamicShapes	= 0.5f;
	dynamicFrictionForDynamicShapes = 0.5f;
	staticFrictionForDynamicShapes = 0.5f;
	attractionForDynamicShapes	= 0.0f;
	collisionResponseCoefficient = 0.2f;

	simulationMethod			= (pFluidSimulationMethod)(NX_F_SPH);
	collisionMethod				=(pFluidCollisionMethod)(NX_F_STATIC|NX_F_DYNAMIC);
	collisionGroup				= 0;	
	groupsMask.bits0 = 0;
	groupsMask.bits1 = 0;
	groupsMask.bits2 = 0;
	groupsMask.bits3 = 0;


	flags						= (pFluidFlag)(NX_FF_ENABLED);
	flags &= ~NX_FF_HARDWARE;

	userData					= NULL;
	name						= NULL;
	worldReference  = 0 ;

}

bool pFluidDesc::isValid()const
{

	if (kernelRadiusMultiplier < 1.0f) return false;
	if (restDensity <= 0.0f) return false;
	if (restParticlesPerMeter <= 0.0f) return false;

	if (packetSizeMultiplier < 4) return false;
	if (packetSizeMultiplier & ( packetSizeMultiplier - 1 ) ) return false; 

	if (motionLimitMultiplier <= 0.0f) return false;
	if (motionLimitMultiplier > packetSizeMultiplier*kernelRadiusMultiplier) return false;

	if (collisionDistanceMultiplier <= 0.0f) return false;
	if (collisionDistanceMultiplier > packetSizeMultiplier*kernelRadiusMultiplier) return false;

	if (stiffness <= 0.0f) return false;
	if (viscosity <= 0.0f) return false;
	if (surfaceTension < 0.0f) return false;

	bool isNoInteraction = (simulationMethod & NX_F_NO_PARTICLE_INTERACTION) > 0;
	bool isSPH = (simulationMethod & NX_F_SPH) > 0;
	bool isMixed = (simulationMethod & NX_F_MIXED_MODE) > 0;
	if (!(isNoInteraction || isSPH || isMixed)) return false;
	if (isNoInteraction && (isSPH || isMixed)) return false;
	if (isSPH && (isNoInteraction || isMixed)) return false;
	if (isMixed && (isNoInteraction || isSPH)) return false;

	if (damping < 0.0f) return false;
	if (fadeInTime < 0.0f) return false;

	if (projectionPlane.normal.isZero()) return false;

	if (dynamicFrictionForDynamicShapes < 0.0f || dynamicFrictionForDynamicShapes > 1.0f) return false;
	if (staticFrictionForDynamicShapes < 0.0f || staticFrictionForDynamicShapes > 1.0f) return false;
	if (restitutionForDynamicShapes < 0.0f || restitutionForDynamicShapes > 1.0f) return false;
	if (attractionForDynamicShapes < 0.0f) return false;
	if (dynamicFrictionForStaticShapes < 0.0f || dynamicFrictionForStaticShapes > 1.0f) return false;
	if (staticFrictionForStaticShapes < 0.0f || staticFrictionForStaticShapes > 1.0f) return false;
	if (restitutionForStaticShapes < 0.0f || restitutionForStaticShapes > 1.0f) return false;
	if (attractionForStaticShapes < 0.0f) return false;
	if (collisionResponseCoefficient < 0.0f) return false;

	
	if (maxParticles > 32767) return false;
	if (maxParticles < 1) return false;

	if (numReserveParticles >= maxParticles) return false;

	if(collisionGroup >= 32) return false; // We only support 32 different collision groups

	return true;
}

void pFluid::updateCloud()
{

	CK3dPointCloud *cloud = getPointCloud();
	if (!cloud)
		return;

	CKMesh *mesh  = getParticleObject()->GetCurrentMesh();
	int count = mesh->GetVertexCount();

	VxVector *points  = new VxVector[count];
	if (mParticleBuffer)
	{
		for (int i = 0 ; i < mesh->GetVertexCount() ; i++)
		{

			pParticle *p  = &mParticleBuffer[i];
			if (p)
			{
				points[i]= getFrom(p->position);
			}
		}
	}


	VxVector prec(0.5,0.5,0.5);
	VxVector a[10];


	int b = cloud->CreateFromPointList(2,a,NULL,NULL,NULL,prec);
	if (b)
	{
		int op2=2;
		op2++;
	}

}

#endif // HAS_FLUIDS