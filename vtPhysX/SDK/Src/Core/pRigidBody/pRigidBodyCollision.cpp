#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "Stream.h"
#include "cooking.h"
#include "IParameter.h"


bool pFactory::_createConvexCylinder(NxConvexShapeDesc* shape,
									 CK3dEntity*dstBodyReference,
									 pObjectDescr *oDescr)
{


	#ifdef _DEBUG
		assert(dstBodyReference);	// <- should never happen !
	#endif // _DEBUG

	bool result  = false;

	pConvexCylinderSettings cSettingsZero;
	
	pConvexCylinderSettings &cSettings = cSettingsZero;

	if (oDescr )
	{
		if( (oDescr->mask & OD_ConvexCylinder)	)
			cSettings = oDescr->convexCylinder;
	}else
	{
		findSettings(cSettings,dstBodyReference);
	}
	

	cSettings.radius.value *=0.5f;
	cSettings.height.value *=0.5f;

	NxArray<NxVec3> points;
	NxVec3 center(0,0,0);

	NxVec3 frontAxis =  getFrom(cSettings.forwardAxis);				// = wheelDesc->downAxis.cross(wheelDesc->wheelAxis);
	NxVec3 downAxis = getFrom(cSettings.downAxis);//downAxis *=-1.0;	//  = wheelDesc->downAxis;
	NxVec3 wheelAxis  = getFrom(cSettings.rightAxis);				// = wheelDesc->wheelAxis;
	
	
	//frontAxis.normalize();
	frontAxis *= cSettings.radius.value;
	//downAxis.normalize();
	downAxis *= cSettings.radius.value;
	//wheelAxis.normalize();
	wheelAxis *= cSettings.height.value;

	NxReal step;

	if(cSettings.buildLowerHalfOnly) 
	{
		if((cSettings.approximation& 0x1) == 0)
			cSettings.approximation++;

		step = (NxReal)(NxTwoPi) / (NxReal)(cSettings.approximation*2);
	}
	else 
	{
		step = (NxReal)(NxTwoPi) / (NxReal)(cSettings.approximation);
	}
	for(NxU32 i = 0; i < cSettings.approximation; i++) 
	{
		NxReal iReal;
		if(cSettings.buildLowerHalfOnly) 
		{
			iReal = (i > (cSettings.approximation >> 1))?(NxReal)(i+cSettings.approximation):(NxReal)i;
		} 
		else 
		{
			iReal = (NxReal)i;
		}
		NxReal Sin, Cos;
		NxMath::sinCos(step * iReal, Sin, Cos);
		NxVec3 insPoint = (downAxis * -Cos) + (frontAxis * Sin);
		points.pushBack(insPoint + wheelAxis);
		points.pushBack(insPoint - wheelAxis);
	}

	NxConvexMeshDesc convexDesc;
	convexDesc.numVertices			= points.size();
	convexDesc.pointStrideBytes		= sizeof(NxVec3);
	convexDesc.points				= &points[0].x;

	int cf = CF_ComputeConvex;
	cf |= cSettings.convexFlags;
	convexDesc.flags				|= cf;

	// Cooking from memory
	bool status = InitCooking();
	if (!status) {
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Couldn't initiate cooking lib!");
		return NULL;
	}
	MemoryWriteBuffer buf;
	int s  = convexDesc.isValid();
	if(CookConvexMesh(convexDesc, buf)) 
	{
		//NxConvexShapeDesc convexShapeDesc;
		
		shape->meshData = getPhysicSDK()->createConvexMesh(MemoryReadBuffer(buf.data));
		shape->localPose.t = center;
		shape->localPose.M.setColumn(0, NxVec3( 1, 0, 0));
		shape->localPose.M.setColumn(1, NxVec3( 0,-1, 0));
		shape->localPose.M.setColumn(2, NxVec3( 0, 0, -1));
		if(shape->meshData != NULL) 
		{
			result  = true;
		//	NxU32 shapeNumber = actor->getNbShapes();
		//	result = actor->createShape(convexShapeDesc)->isConvexMesh();
		//	if (!result) {
		//		xLogger::xLog(XL_START,ELOGERROR,E_LI_AGEIA,"Couldn't create convex cylinder mesh");
		//	}
			//wheel->wheelConvex->userData = wheel;
		}
	}
	CloseCooking();
	return result;
}

void pRigidBody::updateCollisionSettings(pCollisionSettings collision,CK3dEntity*shapeReference/* =NULL */)
{

	if (shapeReference==NULL)
		assert (getMainShape());

	NxShape *shape = getSubShape(shapeReference);
	if (shape)
	{
		//----------------------------------------------------------------
		//
		// Update groups mask 
		//
		NxGroupsMask mask1;

		mask1.bits0 = collision.groupsMask.bits0;
		mask1.bits1 = collision.groupsMask.bits1;
		mask1.bits2 = collision.groupsMask.bits2;
		mask1.bits3 = collision.groupsMask.bits3;

		shape->setGroupsMask(mask1);

		//----------------------------------------------------------------
		//
		// Collisions group
		//
		shape->setGroup(collision.collisionGroup);

		//----------------------------------------------------------------
		//
		// Skin Width
		//
		shape->setSkinWidth(collision.skinWidth);
	}
}

void pRigidBody::updateCollisionSettings(const pObjectDescr oDescr,CK3dEntity*shapeReference/* =NULL */)
{

	int v = oDescr.version;
	assert(oDescr.version == pObjectDescr::E_OD_VERSION::OD_DECR_V1);

	if (shapeReference==NULL)
		assert (getMainShape());

	NxShape *shape = getSubShape(shapeReference);
	if (shape)
	{
		//----------------------------------------------------------------
		//
		// Update groups mask 
		//
		NxGroupsMask mask1;

		mask1.bits0 = oDescr.groupsMask.bits0;
		mask1.bits1 = oDescr.groupsMask.bits1;
		mask1.bits2 = oDescr.groupsMask.bits2;
		mask1.bits3 = oDescr.groupsMask.bits3;

		shape->setGroupsMask(mask1);

		//----------------------------------------------------------------
		//
		// Collisions group
		//
		shape->setGroup(oDescr.collisionGroup);

		//----------------------------------------------------------------
		//
		// Skin Width
		//
		shape->setSkinWidth(oDescr.skinWidth);


		

		//----------------------------------------------------------------
		//
		// CCD Setup :
		//
		if (oDescr.ccdMeshReference)
		{

			NxCCDSkeleton *skeleton = NULL;
			if (oDescr.ccdFlags && CCD_Shared && GetPMan()->GetContext()->GetObject(oDescr.ccdMeshReference) )
				skeleton = GetPMan()->getCCDSkeleton(((CKBeObject*)(GetPMan()->GetContext()->GetObject(oDescr.ccdMeshReference))));

			if (skeleton == NULL )
				skeleton = pFactory::Instance()->createCCDSkeleton(shapeReference,oDescr.ccdFlags);

			XString errorString;

			if (!skeleton){
				errorString.Format("Creation of CCD skeleton for %s failed!",GetVT3DObject()->GetName());
				xLogger::xLog(ELOGERROR,E_LI_MANAGER,errorString.Str());
				return;
			}

			shape->setCCDSkeleton(skeleton);

			//----------------------------------------------------------------
			//
			// update sub mesh info : 
			//

			pSubMeshInfo *sInfo = static_cast<pSubMeshInfo*>(shape->userData);
			if (sInfo){
				sInfo->ccdReference  = oDescr.ccdMeshReference;
				sInfo->ccdSkeleton = skeleton;
			}

			//----------------------------------------------------------------
			//
			// update actors ccd motion threshold
			//
			if(getActor())
				getActor()->setCCDMotionThreshold(oDescr.ccdMotionThresold);


			//NX_SF_DYNAMIC_DYNAMIC_CCD


			//----------------------------------------------------------------
			//
			// Check we have CCD enabled at all, if not then produce warning
			//

			NxPhysicsSDK *sdk  =  GetPMan()->getPhysicsSDK();
			if (sdk)
			{
				float ccdParameter   = sdk->getParameter(NX_CONTINUOUS_CD);
				if (ccdParameter < 0.5f)
				{
					errorString.Format("CCD Skeleton for %s created successfully but CCD is not enabled.\n Please goto Variable Manager and set ´Continues Collision Detection´ to 1.0f",GetVT3DObject());
					xLogger::xLog(ELOGWARNING,E_LI_MANAGER,errorString.Str());
				}
			}
		}
	}
}

int pRigidBody::handleContactPair(NxContactPair* pair,int shapeIndex) 
{

	handleContactPairWheel(pair,shapeIndex);

	return 0;
}


int pRigidBody::handleContactPairWheel(NxContactPair* pair,int shapeIndex) 
{
	if (!hasWheels())
		return 0;
	
	NxContactStreamIterator i(pair->stream);

	while(i.goNextPair())
	{
		NxShape * s = i.getShape(shapeIndex);

		while(i.goNextPatch())
		{
			const NxVec3& contactNormal = i.getPatchNormal();

			while(i.goNextPoint())
			{

				const NxVec3& contactPoint = i.getPoint();

				//assuming front wheel drive we need to apply a force at the wheels.
				if (s->is(NX_SHAPE_CAPSULE) && s->userData != NULL) 
				{
					//assuming only the wheels of the car are capsules, otherwise we need more checks.
					//this branch can't be pulled out of loops because we have to do a full iteration through the stream

					NxQuat local2global = s->getActor().getGlobalOrientationQuat();
					
					pSubMeshInfo *sInfo  = static_cast<pSubMeshInfo*>(s->userData);
					if (!sInfo)return 0;
					
					pWheel1* wheel =  dynamic_cast<pWheel1*>(sInfo->wheel);
					if (!wheel)return 0;
					
					
					/*
					if (!wheel->getWheelFlag(WF_UseWheelShape))
					{
						wheel->contactInfo->otherActor = pair->actors[1-shapeIndex];
						wheel->contactInfo->contactPosition = contactPoint;

						wheel->contactInfo->contactPositionLocal = contactPoint;
						wheel->contactInfo->contactPositionLocal -= wheel->getActor()->getGlobalPosition();
						local2global.inverseRotate(wheel->contactInfo->contactPositionLocal);

						wheel->contactInfo->contactNormal = contactNormal;

						if (wheel->contactInfo->otherActor->isDynamic()) 
						{
							NxVec3 globalV = s->getActor().getLocalPointVelocity( getFrom(wheel->getWheelPos()) );
							globalV -= wheel->contactInfo->otherActor->getLinearVelocity();
							local2global.inverseRotate(globalV);
							wheel->contactInfo->relativeVelocity = globalV.x;
						//printf("%2.3f (%2.3f %2.3f %2.3f)\n", wheel->contactInfo.relativeVelocity,
						//	globalV.x, globalV.y, globalV.z);
						} 
						else 
						{
							NxVec3 vel = s->getActor().getLocalPointVelocity(  getFrom(wheel->getWheelPos()));
							local2global.inverseRotate(vel);
							wheel->contactInfo->relativeVelocity = vel.x;
							wheel->contactInfo->relativeVelocitySide = vel.z;
						}

						//NX_ASSERT(wheel->hasGroundContact());
						//printf(" Wheel %x is touching\n", wheel);
					}
					*/
				}
			}
		}		
	}
	//printf("----\n");

	return 0;
}