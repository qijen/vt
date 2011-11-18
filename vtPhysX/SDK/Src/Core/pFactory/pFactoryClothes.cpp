#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "cooking.h"
#include "Stream.h"

#include "pConfig.h"


void pFactory::copyTo(pDeformableSettings &dst,CKParameter*par)
{

	if (!par)
	{
		return;
	}

	using namespace vtTools::ParameterTools;
	dst.ImpulsThresold = GetValueFromParameterStruct<float>(par,0);
	dst.PenetrationDepth = GetValueFromParameterStruct<float>(par,1);
	dst.MaxDeform = GetValueFromParameterStruct<float>(par,2);

}

pCloth* pFactory::createCloth(CK3dEntity *srcReference,pClothDesc descr)
{


#ifdef DONLGE
	if(!GetPMan()->DongleHasAdvancedVersion)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Cannot create cloth : no license !");
		return NULL;
	}
#endif

	if (!srcReference)
	{
		return NULL;
	}

	if (!srcReference->GetCurrentMesh())
	{
		return NULL;
	}

	if (!descr.isValid())
	{
		return NULL;
	}

	CK3dEntity*worldReference = (CK3dEntity*)GetPMan()->m_Context->GetObject(descr.worldReference);
	
	if (!worldReference)
	{
		if (GetPMan()->getDefaultWorld())
		{
			worldReference = GetPMan()->getDefaultWorld()->getReference();
		}else
			return NULL;
	}

	pWorld*world  = GetPMan()->getWorld(worldReference->GetID());
	if (!world)
	{
		return NULL;

	}


	pRigidBody *body   = GetPMan()->getBody(srcReference);
/*	if (!body)
	{
		return NULL;
	}
*/

	NxClothMeshDesc meshDesc;
	
	// if we want tearing we must tell the cooker
	// this way it will generate some space for particles that will be generated during tearing
	if (descr.flags & PCF_Tearable)
		meshDesc.flags |= NX_CLOTH_MESH_TEARABLE;

	pCloth *result =  new pCloth();

	result->generateMeshDesc(descr,&meshDesc,srcReference->GetCurrentMesh());
	int mValid  = meshDesc.isValid();

	if (!result->cookMesh(&meshDesc))
	{
		delete result;
		return NULL;
	}

	result->releaseMeshDescBuffers(&meshDesc);
	result->allocateClothReceiveBuffers(meshDesc.numVertices, meshDesc.numTriangles);

	NxClothDesc cDescr;
	copyToClothDescr(&cDescr,descr);
	cDescr.clothMesh = result->getClothMesh();
	cDescr.meshData  = *result->getReceiveBuffers();

	VxMatrix v_matrix ;
	VxVector position,scale;		
	VxQuaternion quat;	



	v_matrix = srcReference->GetWorldMatrix();
	Vx3DDecomposeMatrix(v_matrix,quat,position,scale);

	cDescr.globalPose.t = getFrom(position);
	cDescr.globalPose.M  = getFrom(quat);
	
	

	
	NxCloth *cloth   = world->getScene()->createCloth(cDescr);
	if (cloth)
	{
		result->setCloth(cloth);
		result->setEntityID(srcReference->GetID());
		result->setWorld(GetPMan()->getDefaultWorld());
		cloth->userData  = result;

		if (meshDesc.points)
		{
//			delete []meshDesc.points;
//			delete []meshDesc.triangles;
		}



		if (descr.flags & PCF_AttachToParentMainShape )
		{
			if (srcReference->GetParent())
			{
				CK3dEntity *bodyReference = getMostTopParent(srcReference);
				if (bodyReference)
				{
					pRigidBody *body  = GetPMan()->getBody(bodyReference);
					if (body)
					{
						result->attachToShape((CKBeObject*)bodyReference,descr.attachmentFlags);
					}
				}
			}
		}

		if (descr.flags & PCF_AttachToCollidingShapes)
		{
			result->attachToCollidingShapes(descr.attachmentFlags);
		}

		if (descr.flags & PCF_AttachToCore)
		{
/*

			NxBodyDesc  coreBodyDesc;
			coreBodyDesc.linearDamping = 0.2f;
			coreBodyDesc.angularDamping = 0.2f;

			NxActorDesc coreActorDesc;
			coreActorDesc.density = 0.1f;
			coreActorDesc.body = &coreBodyDesc;

			coreActorDesc.shapes.pushBack(new NxBoxShapeDesc());

			NxActor *coreActor = world->getScene()->createActor(coreActorDesc);
			coreActor->userData =NULL;
			

			NxReal impulseThreshold  = 50.0f;
			NxReal penetrationDepth  = 0.5f;
			NxReal maxDeformationDistance = 0.5f;

			cloth->attachToCore(coreActor, impulseThreshold, penetrationDepth, maxDeformationDistance); 
*/
//			result->attachToCollidingShapes(descr.attachmentFlags);
		}

		return result;
	}

	if (meshDesc.points)
	{
//		delete []meshDesc.points;
//		delete []meshDesc.triangles;
	}
	
	return result;

}
pClothDesc* pFactory::createClothDescrFromParameter(CKParameter *par)
{

	if (!par)
		return NULL;

	pClothDesc *descr  = new pClothDesc();
	
	using namespace vtTools::ParameterTools;
	
	descr->density = GetValueFromParameterStruct<float>(par,E_CS_DENSITY,false);
	descr->thickness = GetValueFromParameterStruct<float>(par,E_CS_THICKNESS,false);
	descr->bendingStiffness = GetValueFromParameterStruct<float>(par,E_CS_BENDING_STIFFNESS,false);
	descr->stretchingStiffness = GetValueFromParameterStruct<float>(par,E_CS_STRETCHING_STIFFNESS,false);
	descr->dampingCoefficient = GetValueFromParameterStruct<float>(par,E_CS_DAMPING_COEFFICIENT,false);
	descr->friction = GetValueFromParameterStruct<float>(par,E_CS_FRICTION,false);
	
	descr->pressure = GetValueFromParameterStruct<float>(par,E_CS_PRESSURE,false);
	descr->tearFactor = GetValueFromParameterStruct<float>(par,E_CS_TEAR_FACTOR,false);
	descr->collisionResponseCoefficient = GetValueFromParameterStruct<float>(par,E_CS_COLLISIONRESPONSE_COEFFICIENT,false);
	
	descr->attachmentResponseCoefficient = GetValueFromParameterStruct<float>(par,E_CS_ATTACHMENTRESPONSE_COEFFICIENT,false);
	descr->attachmentTearFactor = GetValueFromParameterStruct<float>(par,E_CS_ATTACHMENT_TEAR_FACTOR,false);
	descr->toFluidResponseCoefficient = GetValueFromParameterStruct<float>(par,E_CS_TO_FLUID_RESPONSE_COEFFICIENT,false);

	descr->fromFluidResponseCoefficient = GetValueFromParameterStruct<float>(par,E_CS_FROM_FLUIDRESPONSE_COEFFICIENT,false);
	descr->minAdhereVelocity = GetValueFromParameterStruct<float>(par,E_CS_MIN_ADHERE_VELOCITY,false);
	descr->solverIterations = GetValueFromParameterStruct<int>(par,E_CS_SOLVER_ITERATIONS,false);
	
	descr->externalAcceleration = GetValueFromParameterStruct<VxVector>(par,E_CS_EXTERN_ALACCELERATION,false);
	descr->windAcceleration = GetValueFromParameterStruct<VxVector>(par,E_CS_WIND_ACCELERATION,false);
	descr->wakeUpCounter = GetValueFromParameterStruct<float>(par,E_CS_WAKE_UP_COUNTER,false);
	
	descr->sleepLinearVelocity = GetValueFromParameterStruct<float>(par,E_CS_SLEEP_LINEAR_VELOCITY,false);
	descr->collisionGroup = GetValueFromParameterStruct<int>(par,E_CS_COLLISIONG_ROUP,false);
	
	descr->validBounds = GetValueFromParameterStruct<VxBbox>(par,E_CS_VALID_BOUNDS,false);
	descr->relativeGridSpacing = GetValueFromParameterStruct<float>(par,E_CS_RELATIVE_GRID_SPACING,false);
	descr->flags = GetValueFromParameterStruct<int>(par,E_CS_FLAGS,false);
	descr->tearVertexColor = GetValueFromParameterStruct<VxColor>(par,E_CS_TEAR_VERTEX_COLOR,false);
	
	descr->worldReference = GetValueFromParameterStruct<CK_ID>(par,E_CS_WORLD_REFERENCE,false);

	return descr;
	
}

void pFactory::copyToClothDescr(NxClothDesc* dst,pClothDesc src )
{

	dst->density = src.density ;
	dst->thickness = src.thickness ;
	dst->bendingStiffness = src.bendingStiffness;
	dst->stretchingStiffness = src.stretchingStiffness;
	dst->dampingCoefficient = src.dampingCoefficient;

	dst->friction = src.friction;
	dst->pressure = src.pressure;
	dst->tearFactor = src.tearFactor ;
	dst->collisionResponseCoefficient = src.collisionResponseCoefficient ;
	dst->attachmentResponseCoefficient = src.attachmentResponseCoefficient ;
	dst->attachmentTearFactor = src.attachmentTearFactor ;
	dst->toFluidResponseCoefficient = src.toFluidResponseCoefficient ;
	dst->fromFluidResponseCoefficient= src.fromFluidResponseCoefficient ;
	dst->minAdhereVelocity = src.minAdhereVelocity ;
	dst->solverIterations = src.solverIterations ;
	dst->externalAcceleration = getFrom(src.externalAcceleration) ;
	dst->windAcceleration  = getFrom(src.windAcceleration);
	dst->wakeUpCounter = src.wakeUpCounter ;
	dst->sleepLinearVelocity = src.sleepLinearVelocity ;
	dst->collisionGroup  = src.collisionGroup;
	dst->validBounds.set(getFrom(src.validBounds.Min) , getFrom(src.validBounds.Max));
	dst->relativeGridSpacing = src.relativeGridSpacing;
	dst->flags  =src.flags;
	
}