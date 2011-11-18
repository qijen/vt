#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "Stream.h"
#include "cooking.h"

#include "NXU_helper.h"  // NxuStream helper functions.
#include "NXU_PhysicsInstantiator.h"

#include "IParameter.h"

#include "xDebugTools.h"


pRigidBody*pFactory::cloneRigidBody(CK3dEntity *src,CK3dEntity *dst,CKDependencies *deps,int copyFlags,int bodyFlags/* =0 */)
{
	//src->Rest
	pRigidBody *result = GetPMan()->getBody(dst);
	pRigidBody *srcBody = GetPMan()->getBody(src);
	CK3dEntity *referenceObject = dst;

	XString errMsg;
	pObjectDescr oDescr;

	//----------------------------------------------------------------
	//
	// sanity checks
	//
	#ifdef _DEBUG
		assert(src);
		assert(dst);
	#endif // _DEBUG

	if (!(copyFlags & PB_CF_PHYSICS))
	{
		errMsg.Format("Nothing to copy, aborting");
		xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,errMsg.Str());
		return NULL;
	}

	//iAssertW(!result,"","Object :%s already physicalized");
		
	//----------------------------------------------------------------
	//
	// fill object description
	//
	if (!result && IParameter::Instance()->copyTo(oDescr,src,copyFlags))
	{
		pWorld *world  = GetPMan()->getWorld(oDescr.worlReference) ? GetPMan()->getWorld(oDescr.worlReference) : GetPMan()->getDefaultWorld();
		if(world)
		{

			if ( (copyFlags && PB_CF_OVRRIDE_BODY_FLAGS) )
				oDescr.flags  = (BodyFlags)bodyFlags;
			
			//now create the final rigid body : 
			result  = pFactory::Instance()->createRigidBody(dst,oDescr);
		}
	}

	if (!result){
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"cloning failed");
		return NULL;
	}

	//----------------------------------------------------------------
	//
	// clone joints
	//
	if ( (copyFlags & PB_CF_JOINTS) )
	{
		pFactory::cloneJoints(src,dst,copyFlags);

	}

	//----------------------------------------------------------------
	//
	// copy velocities
	//
	if ( (copyFlags & PB_CF_VELOCITIES) )
	{
		NxActor *actorSrc  = srcBody->getActor();
		NxActor *actorDst  = result->getActor();

		actorDst->setLinearVelocity( actorSrc->getLinearVelocity() );
		actorDst->setAngularVelocity( actorSrc->getAngularVelocity() );
	}

	//----------------------------------------------------------------
	//
	// copy forces
	//
	if ( (copyFlags & PB_CF_FORCE) )
	{
		NxActor *actorSrc  = srcBody->getActor();
		NxActor *actorDst  = result->getActor();
		actorDst->setLinearMomentum( actorSrc->getLinearMomentum() );
		actorDst->setAngularMomentum( actorSrc->getAngularMomentum() );
	}
	
	//----------------------------------------------------------------
	//
	// copy sub shapes if : 
	//
	//		"Copy Children In Dependencies" &&
	//		( copyFlags::OverrideBodyFlags & hierarchy &&  newBodyFlags & hierarchy  ) ||		
	//		( oldBodyFlags & hierarchy )
	if ( ((*deps->At(CKCID_3DENTITY)) & CK_DEPENDENCIES_COPY_3DENTITY_CHILDREN)  && 
		  ( (( copyFlags & PB_CF_OVRRIDE_BODY_FLAGS ) && (bodyFlags & BF_Hierarchy)) ||
		  ( oDescr.flags & BF_Hierarchy) )
		)
	{
		int dCount = dst->GetChildrenCount();
		CK3dEntity* subEntity = NULL;
		while (subEntity= dst->HierarchyParser(subEntity) )
		{
			if (subEntity==dst)
				continue;

			CK3dEntity *orginalObject = findSimilarInSourceObject(src,dst,subEntity);
			if (orginalObject)
			{
				iAssertW(cloneShape(orginalObject,subEntity,dst,copyFlags,bodyFlags),"","clone of sub shape failed");
			}
		}
	}
	return NULL;
}

pRigidBody*pFactory::createRigidBody(CK3dEntity *referenceObject,pObjectDescr& oDescr)
{
	CK3dEntity *worldReferenceObject = (CK3dEntity*)GetPMan()->GetContext()->GetObject(oDescr.worlReference);

	pWorld *world=getManager()->getWorld(worldReferenceObject,referenceObject); 

	if (!world)
	{
		xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"world object invalid, setting to default world");
		world  = GetPMan()->getDefaultWorld();
	}

	pRigidBody*result = world->getBody(referenceObject);

	//	create minimum object
	result  =  createBody(referenceObject,worldReferenceObject);

	if (!result)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"failed");
		return NULL;

	}
	result->setWorld(world);
	result->setFlags(oDescr.flags);
	result->setHullType(oDescr.hullType);

	if (oDescr.density <= 0.001f)
		oDescr.density = 1.0f;
	
	if (oDescr.skinWidth <= 0.001f)
		oDescr.skinWidth = 0.025f;

	
	result->setDensity(oDescr.density);

	bool hierarchy = (oDescr.flags & BF_Hierarchy);
	bool isDeformable = oDescr.flags & BF_Deformable;
	bool  trigger = oDescr.flags & BF_TriggerShape;


	//################################################################
	//
	// Deformable ?
	//


	pCloth *cloth = NULL;
	pClothDesc cDescr;

	if (isDeformable)
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"deformable feature disabled in this release");
		return NULL;

		cDescr.setToDefault();
		cDescr.worldReference = worldReferenceObject->GetID();

		if ( result->getFlags() & BF_Gravity )
		{
			cDescr.flags |= PCF_Gravity;
		}

		if ( result->getFlags() & BF_Collision )
		{

		}

		if (!cloth)
		{
			cloth = pFactory::Instance()->createCloth(referenceObject,cDescr);
			if (!cloth)
			{
				xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Cannot create cloth : factory object failed !");
			}
		}
	}


	float density = oDescr.density;
	VxVector box_s= BoxGetZero(referenceObject);


	VxMatrix v_matrix ;
	VxVector position,scale;		
	VxQuaternion quat;	


	v_matrix = referenceObject->GetWorldMatrix();
	Vx3DDecomposeMatrix(v_matrix,quat,position,scale);

	NxVec3 pos = pMath::getFrom(position);
	NxQuat rot = pMath::getFrom(quat);


	//----------------------------------------------------------------
	//
	// Fill NxActorDescr
	//
	

	NxActorDesc actorDesc;actorDesc.setToDefault();
	NxBodyDesc bodyDesc;bodyDesc.setToDefault();

	//
	//	Fix parameters to default values

	
	actorDesc.density  = oDescr.density;

	//skin width
	if (oDescr.skinWidth<=0.0001f)
		oDescr.skinWidth=0.025f;


	float radius = result->GetVT3DObject()->GetRadius();
	
	//----------------------------------------------------------------
	//
	//	Create the physic mesh. Externalizing this procedure will corrupt the
	//	actor description object.
	//
	
	switch(oDescr.hullType)
	{

		//////////////////////////////////////////////////////////////////////////
	case HT_Box:
		{
			NxBoxShapeDesc shape;

			if (! (oDescr.flags & BF_Deformable) )
			{
				shape.dimensions = pMath::getFrom(box_s)*0.5f;
			}
			shape.density  = density;
			shape.skinWidth = oDescr.skinWidth;
			actorDesc.shapes.pushBack(&shape);
			break;
		}
		//////////////////////////////////////////////////////////////////////////
	case HT_Sphere:
		{
			NxSphereShapeDesc shape;
			if (! (oDescr.flags & BF_Deformable) )
			{
				shape.radius = radius;
			}
			shape.density  = density;
			shape.skinWidth = oDescr.skinWidth;
			actorDesc.shapes.pushBack(&shape);

			break;
		}
		//////////////////////////////////////////////////////////////////////////
	case HT_Mesh:
		{

			NxTriangleMeshDesc myMesh;
			myMesh.setToDefault();

			pFactory::Instance()->createMesh(result->getWorld()->getScene(),result->GetVT3DObject()->GetCurrentMesh(),myMesh);

			NxTriangleMeshShapeDesc shape;
			bool status = InitCooking();
			if (!status) {
				xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Couldn't initiate cooking lib!");
				return false;
			}
			MemoryWriteBuffer buf;

			status = CookTriangleMesh(myMesh, buf);
			if (!status) {
				xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Couldn't cook mesh!");
				return false;
			}
			shape.meshData	= GetPMan()->getPhysicsSDK()->createTriangleMesh(MemoryReadBuffer(buf.data));
			shape.density  = density;
			shape.group = oDescr.collisionGroup;

			actorDesc.shapes.pushBack(&shape);
			shape.skinWidth = oDescr.skinWidth;
			CloseCooking();

			if (myMesh.points)
			{
				delete [] myMesh.points;
			}

			if (myMesh.triangles)
			{
				delete []myMesh.triangles;
			}

			break;
		}
		//////////////////////////////////////////////////////////////////////////
	case HT_ConvexMesh:
		{
		
			if (result->GetVT3DObject()->GetCurrentMesh())
			{
				if (result->GetVT3DObject()->GetCurrentMesh()->GetVertexCount()>=256 )
				{
					xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Only 256 vertices for convex meshes allowed, by Ageia!");
					return false;
				}
			}else
			{
				xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Object has no mesh!");
				return false;
			}

			NxConvexMeshDesc myMesh;
			myMesh.setToDefault();
			pFactory::Instance()->createConvexMesh(result->getWorld()->getScene(),result->GetVT3DObject()->GetCurrentMesh(),myMesh);

			NxConvexShapeDesc shape;
			bool status = InitCooking();
			if (!status) {
				xLogger::xLog(XL_START,ELOGERROR,E_LI_AGEIA,"Couldn't initiate cooking lib!");
				return false;

			}
			MemoryWriteBuffer buf;

			status = CookConvexMesh(myMesh, buf);
			if (!status) {
				xLogger::xLog(XL_START,ELOGERROR,E_LI_AGEIA,"Couldn't cook convex mesh!");
				return false;


			}
			shape.meshData	= GetPMan()->getPhysicsSDK()->createConvexMesh(MemoryReadBuffer(buf.data));
			shape.density  = density;
			actorDesc.shapes.pushBack(&shape);
			if (oDescr.skinWidth!=-1.0f)
				shape.skinWidth = oDescr.skinWidth;
			int h = shape.isValid();
			CloseCooking();

			if (myMesh.points)
			{
				delete [] myMesh.points;
			}

			if (myMesh.triangles)
			{
				delete []myMesh.triangles;
			}


			break;
		}

		//////////////////////////////////////////////////////////////////////////
	case HT_ConvexCylinder:
		{
			NxConvexShapeDesc shape;
			pConvexCylinderSettings &cSettings = oDescr.convexCylinder;
			
			iAssertW( (oDescr.mask & OD_ConvexCylinder),pFactory::Instance()->findSettings(cSettings,result->GetVT3DObject()),
				"Hull type has been set to convex cylinder but there is no descriptions passed or activated in the pObjectDescr::mask.Trying object attributes....");
			
			if (cSettings.radius.reference)
				cSettings.radius.evaluate(cSettings.radius.reference);

			if (cSettings.height.reference)
				cSettings.height.evaluate(cSettings.height.reference);


			iAssertW( cSettings.isValid() , cSettings.setToDefault(),"");

			cSettings.radius.value = cSettings.radius.value > 0.0f ? (cSettings.radius.value*0.5) : (box_s.v[cSettings.radius.referenceAxis] * 0.5f);
			cSettings.height.value = cSettings.height.value > 0.0f ? cSettings.height.value : (box_s.v[cSettings.height.referenceAxis] * 0.5f);

			

			bool resultAssert = true;
			iAssertWR( pFactory::Instance()->_createConvexCylinderMesh(&shape,cSettings,result->GetVT3DObject()),"",resultAssert);
			
			shape.density  = density;
			shape.skinWidth = oDescr.skinWidth;
			actorDesc.shapes.pushBack(&shape);
			break;
		}
		//////////////////////////////////////////////////////////////////////////
	case HT_Capsule:
		{

			NxCapsuleShapeDesc shape;
			pCapsuleSettingsEx &cSettings = oDescr.capsule;
			if (!( oDescr.mask & OD_Capsule) )
			{
				// try over attribute :
				pFactory::Instance()->findSettings(cSettings,result->GetVT3DObject());
			}
			
			bool resultAssert = true;
			
			if (cSettings.radius.reference)
				cSettings.radius.evaluate(cSettings.radius.reference);

			if (cSettings.height.reference)
				cSettings.height.evaluate(cSettings.height.reference);

			iAssertWR(cSettings.isValid(),cSettings.setToDefault(),resultAssert);

			

			shape.radius = cSettings.radius.value > 0.0f ? (cSettings.radius.value*0.5) : (box_s.v[cSettings.radius.referenceAxis] * 0.5f);
			shape.height = cSettings.height.value > 0.0f ? (cSettings.height.value-( 2*shape.radius)) : (box_s.v[cSettings.height.referenceAxis]  -  ( 2*shape.radius)) ;


			shape.density  = density;
			shape.skinWidth = oDescr.skinWidth;
			actorDesc.shapes.pushBack(&shape);
			break;
		}

	case HT_Wheel:
		{

			xLogger::xLog(XL_START,ELOGERROR,E_LI_AGEIA,"Wheel shape can be sub shape only!");
			return false;
		}
	}

	if ( !isDeformable)
	{
		actorDesc.globalPose.t = pos;	
		actorDesc.globalPose.M = rot;
	}



	//----------------------------------------------------------------
	//
	// Create the final NxActor
	//

	if (oDescr.flags & BF_Moving)
		actorDesc.body = &bodyDesc;

	NxActor *actor  = world->getScene()->createActor(actorDesc);
	if (!actor)
	{
		xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Couldn't create actor");
		delete result;
		return NULL;

	}

	//----------------------------------------------------------------
	//
	// 
	//
	result->setActor(actor);
	actor->setName(referenceObject->GetName());
	result->SetVT3DObject(referenceObject);
	actor->userData= result;


	//////////////////////////////////////////////////////////////////////////
	//Deformable : 
	if (isDeformable && cloth)
	{

		pDeformableSettings dSettings;
		dSettings.ImpulsThresold = 50.0f;
		dSettings.PenetrationDepth= 0.1f ;
		dSettings.MaxDeform = 2.0f;

		CKParameterOut *poutDS = referenceObject->GetAttributeParameter(GetPMan()->att_deformable);
		if (poutDS)
		{
			pFactory::Instance()->copyTo(dSettings,poutDS);
		}
		cloth->attachToCore(referenceObject,dSettings.ImpulsThresold,dSettings.PenetrationDepth,dSettings.MaxDeform);
		result->setCloth(cloth);
	}


	//////////////////////////////////////////////////////////////////////////
	//
	//	Extra settings : 
	//

	if (result->getFlags() & BF_Moving)
	{
		VxVector massOffsetOut;referenceObject->Transform(&massOffsetOut,&oDescr.massOffsetLinear);
		actor->setCMassOffsetGlobalPosition(pMath::getFrom(massOffsetOut));
	}

	if (result->getFlags() & BF_Kinematic)
	{
		result->setKinematic(true);
	}

	if (result->getFlags() & BF_Moving )
	{
		result->enableGravity(result->getFlags() & BF_Gravity);
	}

	if (result->getFlags() & BF_Sleep )
	{
		result->setSleeping(true);
	}

	if (oDescr.worlReference == 0)
	{
		if (GetPMan()->getDefaultWorld())
		{
			oDescr.worlReference = GetPMan()->getDefaultWorld()->getReference()->GetID();
		}
	}

	//----------------------------------------------------------------
	//
	// store mesh meta info in the first main mesh
	//
	NxShape *shape  = result->getShapeByIndex();
	if (shape)
	{
		pSubMeshInfo *sInfo  = new pSubMeshInfo();
		sInfo->entID = referenceObject->GetID();
		sInfo->refObject  = (CKBeObject*)referenceObject;
		shape->userData = (void*)sInfo;
		result->setMainShape(shape);
		shape->setName(referenceObject->GetName());

	}

	result->enableCollision( (result->getFlags() & BF_Collision), referenceObject );

	//----------------------------------------------------------------
	//
	// Adjust pivot 
	//

	if ( (oDescr.mask & OD_Pivot) )
	{
		iAssertW1( oDescr.pivot.isValid(),oDescr.pivot.setToDefault());
		result->updatePivotSettings(oDescr.pivot,referenceObject);
	}else if(pFactory::Instance()->findSettings(oDescr.collision,referenceObject))
		result->updatePivotSettings(oDescr.pivot,referenceObject);
	
	//----------------------------------------------------------------
	//
	// Optimization 
	//
	if ((oDescr.mask & OD_Optimization ))
	{
		iAssertW1( oDescr.optimization.isValid(),oDescr.optimization.setToDefault());
		result->updateOptimizationSettings(oDescr.optimization);
	}
	else{
		if(pFactory::Instance()->findSettings(oDescr.optimization,referenceObject))
		{
			iAssertW1( oDescr.optimization.isValid(),oDescr.optimization.setToDefault());
			result->updateOptimizationSettings(oDescr.optimization);
		}
	}

	//----------------------------------------------------------------
	//
	// Collision
	//
	if ((oDescr.mask & OD_Collision))
		result->updateCollisionSettings(oDescr.collision,referenceObject);
	else if(pFactory::Instance()->findSettings(oDescr.collision,referenceObject))
		result->updateCollisionSettings(oDescr.collision,referenceObject);


	//----------------------------------------------------------------
	//
	// Material 
	//

	NxMaterialDesc *materialDescr = NULL;
	NxMaterial *material = NULL;

	pMaterial &bMaterial = oDescr.material;

	if (oDescr.mask & OD_Material)
	{
		result->updateMaterialSettings(bMaterial,referenceObject);
	}else
	{
		bool hasMaterial = pFactory::Instance()->findSettings(bMaterial,referenceObject);
		if (!hasMaterial)
		{
			if (world->getDefaultMaterial())
			{
				int z = (int)world->getDefaultMaterial()->userData;
				shape->setMaterial(world->getDefaultMaterial()->getMaterialIndex());
			}
		}else{
			
			iAssertW( bMaterial.isValid(),bMaterial.setToDefault(),
				"Material settings were still invalid : ");

			NxMaterialDesc nxMatDescr;
			pFactory::Instance()->copyTo(nxMatDescr,bMaterial);
			NxMaterial *nxMaterial  = world->getScene()->createMaterial(nxMatDescr);
			if (nxMaterial)
			{
				shape->setMaterial(nxMaterial->getMaterialIndex());
			}
		}
	}


	xLogger::xLog(ELOGINFO,E_LI_MANAGER,"Rigid body creation successful : %s",referenceObject->GetName());

	result->setInitialDescription(&oDescr);

	//----------------------------------------------------------------
	//
	// Hierarchy mode fix : 
	//
	if ( (oDescr.flags & BF_Hierarchy)  )
		oDescr.hirarchy = true;

	if ( oDescr.hirarchy  )
		oDescr.flags << BF_Hierarchy;


	if ( (!oDescr.hirarchy) || !(oDescr.flags & BF_Hierarchy) ) 
		return result;

	//----------------------------------------------------------------
	//
	// Parse hirarchy 
	//
	CK3dEntity* subEntity = NULL;
	while (subEntity= referenceObject->HierarchyParser(subEntity) )
	{
		pObjectDescr *subDescr  = NULL;
		int attTypePBSetup  = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_ACTOR);
		if (subEntity->HasAttribute(attTypePBSetup))
		{
			subDescr  = new pObjectDescr();
			CKParameterOut *par =  subEntity->GetAttributeParameter(attTypePBSetup);
			IParameter::Instance()->copyTo(subDescr,par);
			subDescr->version  = pObjectDescr::E_OD_VERSION::OD_DECR_V1;
		}

		if (!subDescr)
			continue;

		if (subDescr->flags & BF_SubShape)
		{
			//////////////////////////////////////////////////////////////////////////
			//
			//			Regular Mesh : 
			//

			if (subDescr->hullType != HT_Cloth)
			{
				//result->addSubShape(NULL,*subDescr,subEntity);
				
				VxQuaternion refQuad;subEntity->GetQuaternion(&refQuad,referenceObject);
				VxVector relPos;subEntity->GetPosition(&relPos,referenceObject);
				
				shape  = pFactory::Instance()->createShape(referenceObject,*subDescr,subEntity,subEntity->GetCurrentMesh(),relPos,refQuad);
				
				//NxShape *shape = result->getSubShape(subEntity);
				if (shape)
				{
					//----------------------------------------------------------------
					//
					// check for collision setup
					//
					
					//try to get get from child attributes first
					/*
					if(pFactory::Instance()->findSettings(subDescr->collision,subEntity))
					{
						result->updateCollisionSettings(subDescr->collision,subEntity);
						continue;
					}
					if ( (subDescr->mask & OD_Optimization) )
					{
						//if (pFactory::Instance()->findSettings(subDescr->collision,subEntity))
						result->updateCollisionSettings(subDescr->collision,subEntity);
					}
					else if ( (oDescr.mask & OD_Optimization) )
					{
						result->updateCollisionSettings(oDescr.collision,subEntity);
					}else if(pFactory::Instance()->findSettings(subDescr->collision,subEntity))
					{
						result->updateCollisionSettings(subDescr->collision,subEntity);
					}
					*/
				}
				
			}
			//////////////////////////////////////////////////////////////////////////
			//
			//			Cloth Mesh : 
			//
			if (subDescr->hullType == HT_Cloth)
			{
				//pClothDesc *cloth = pFactory::Instance()->createPObjectDescrFromParameter(subEntity->GetAttributeParameter(GetPMan()->GetPAttribute()));
			}
		}
	}

	//----------------------------------------------------------------
	//
	// Adjust mass 
	//
	
	//----------------------------------------------------------------
	//
	// Collision
	//
	if ((oDescr.mask & OD_Mass))
		result->updateMassSettings(oDescr.mass);
	else if(pFactory::Instance()->findSettings(oDescr.mass,referenceObject))
		result->updateMassSettings(oDescr.mass);


	return result;
	
		nothing:

	return result;

}

pRigidBody*pFactory::createCapsule(CK3dEntity *referenceObject,CK3dEntity *worldReferenceObject,pObjectDescr*descr,int creationFlags)
{

	pWorld *world=getManager()->getWorld(worldReferenceObject,referenceObject); 
	pRigidBody*result = world->getBody(referenceObject);
	if(result)
	{
		result->destroy();
		delete result;
		result = NULL;
	}
	// we create our final body in the given world :
	result  =  createBody(referenceObject,worldReferenceObject);
	if (result)
	{
		result->setWorld(world);

		using namespace vtTools::AttributeTools;

		result->setFlags(descr->flags);
		result->setHullType(descr->hullType);
		result->setDataFlags(0x000);
		result->checkDataFlags();

		if (result->getSkinWidth()==-1.0f)
		{
			result->setSkinWidth(0.01f);
		}

		VxMatrix v_matrix ;
		VxVector position,scale;		
		VxQuaternion quat;	


		v_matrix = referenceObject->GetWorldMatrix();
		Vx3DDecomposeMatrix(v_matrix,quat,position,scale);
		VxVector box_s= BoxGetZero(referenceObject);

		NxVec3 pos = pMath::getFrom(position);
		NxQuat rot = pMath::getFrom(quat);

		float density = result->getDensity();
		float radius = referenceObject->GetRadius();
		if (referenceObject->GetRadius() < 0.001f )
		{
			radius  = 1.0f;
		}


		//////////////////////////////////////////////////////////////////////////
		//create actors description
		NxActorDesc actorDesc;actorDesc.setToDefault();
		NxBodyDesc bodyDesc;bodyDesc.setToDefault();

		//////////////////////////////////////////////////////////////////////////
		NxMaterialDesc *materialDescr = NULL;
		NxMaterial *material = NULL;
		if (isFlagOn(result->getDataFlags(),EDF_MATERIAL_PARAMETER))
		{
			NxMaterialDesc  entMatNull;entMatNull.setToDefault();
			NxMaterialDesc  *entMat  = createMaterialFromEntity(referenceObject);
			material  = world->getScene()->createMaterial(entMatNull);
			material->loadFromDesc(*entMat);
			result->setMaterial(material);
		}else{
			if (world->getDefaultMaterial())
			{
				result->setMaterial(world->getDefaultMaterial());
			}
		}
		//////////////////////////////////////////////////////////////////////////

		NxCapsuleShapeDesc capsuleShape;

		if ( creationFlags & E_OFC_DIMENSION )
		{
			capsuleShape.height = box_s.y - ((box_s.x));
			capsuleShape.radius = box_s.x*0.5f;
		}
		
		capsuleShape.density  = descr->density;

		
		capsuleShape.materialIndex = result->getMaterial()->getMaterialIndex();
		
		//shape.localPose.t = pMath::getFrom(shapeOffset);

		if (result->getSkinWidth()!=-1.0f)
			capsuleShape.skinWidth = result->getSkinWidth();
		
		actorDesc.shapes.pushBack(&capsuleShape);




		//////////////////////////////////////////////////////////////////////////
		//dynamic object  ? 
		if (result->getFlags() & BF_Moving){
			actorDesc.body = &bodyDesc;
		}
		else
			actorDesc.body = NULL;

		//////////////////////////////////////////////////////////////////////////
		//set transformations
		actorDesc.density  = descr->density;

		if (creationFlags & E_OFC_POSITION)
		{
			actorDesc.globalPose.t = pos;	
		}

		if (creationFlags & E_OFC_POSITION)
		{
			actorDesc.globalPose.M = rot;
		}

		//////////////////////////////////////////////////////////////////////////
		//create the actor

		int v = actorDesc.isValid();
		NxActor *actor  = world->getScene()->createActor(actorDesc);
		if (!actor)
		{
			xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Couldn't create actor");
			delete result;
			return NULL;

		}


		//////////////////////////////////////////////////////////////////////////
		//set additional settings : 
		result->setActor(actor);
		actor->setName(referenceObject->GetName());
		actor->userData= result;

		if (result->getFlags() & BF_Moving)
		{
			VxVector massOffsetOut;referenceObject->Transform(&massOffsetOut,&result->getMassOffset());
			actor->setCMassOffsetGlobalPosition(pMath::getFrom(massOffsetOut));
		}

		if (result->getFlags() & BF_Kinematic)
		{
			actor->raiseBodyFlag(NX_BF_KINEMATIC);
		}

		result->enableCollision((result->getFlags() & BF_Collision));
		if (result->getFlags() & BF_Moving)
		{
			if (!(result->getFlags() & BF_Gravity))
			{
				actor->raiseBodyFlag(NX_BF_DISABLE_GRAVITY);
			}
		}

		NxShape *shape  = result->getShapeByIndex();
		if (shape)
		{
			pSubMeshInfo *sInfo  = new pSubMeshInfo();
			sInfo->entID = referenceObject->GetID();
			sInfo->refObject  = (CKBeObject*)referenceObject;
			shape->userData = (void*)sInfo;
			result->setMainShape(shape);
			shape->setName(referenceObject->GetName());
		}
	}

	return result;
}

pRigidBody*pFactory::createBox(CK3dEntity *referenceObject,CK3dEntity *worldReferenceObject,pObjectDescr*descr,int creationFlags)
{

	pWorld *world=getManager()->getWorld(worldReferenceObject,referenceObject); 
	pRigidBody*result = world->getBody(referenceObject);
	if(result)
	{
		result->destroy();
		delete result;
		result = NULL;
	}
	// we create our final body in the given world :
	result  =  createBody(referenceObject,worldReferenceObject);
	if (result)
	{
		result->setWorld(world);

		using namespace vtTools::AttributeTools;

		result->setFlags(descr->flags);
		result->setHullType(descr->hullType);
		result->setDataFlags(0x000);
		result->checkDataFlags();

		if (result->getSkinWidth()==-1.0f)
		{
			result->setSkinWidth(0.01f);
		}

		VxMatrix v_matrix ;
		VxVector position,scale;		
		VxQuaternion quat;	


		v_matrix = referenceObject->GetWorldMatrix();
		Vx3DDecomposeMatrix(v_matrix,quat,position,scale);
		VxVector box_s= BoxGetZero(referenceObject);

		NxVec3 pos = pMath::getFrom(position);
		NxQuat rot = pMath::getFrom(quat);

		float density = result->getDensity();
		float radius = referenceObject->GetRadius();
		if (referenceObject->GetRadius() < 0.001f )
		{
			radius  = 1.0f;
		}


		//////////////////////////////////////////////////////////////////////////
		//create actors description
		NxActorDesc actorDesc;actorDesc.setToDefault();
		NxBodyDesc bodyDesc;bodyDesc.setToDefault();

		//////////////////////////////////////////////////////////////////////////
		NxMaterialDesc *materialDescr = NULL;
		NxMaterial *material = NULL;
		if (isFlagOn(result->getDataFlags(),EDF_MATERIAL_PARAMETER))
		{
			NxMaterialDesc  entMatNull;entMatNull.setToDefault();
			NxMaterialDesc  *entMat  = createMaterialFromEntity(referenceObject);
			material  = world->getScene()->createMaterial(entMatNull);
			material->loadFromDesc(*entMat);
			result->setMaterial(material);
		}else{
			if (world->getDefaultMaterial())
			{
				result->setMaterial(world->getDefaultMaterial());
			}
		}
		//////////////////////////////////////////////////////////////////////////

		NxBoxShapeDesc boxShape;
		if (creationFlags & E_OFC_DIMENSION )
		{
			boxShape.dimensions = pMath::getFrom(box_s)*0.5f;
		}
		boxShape.density  = descr->density;
		boxShape.materialIndex = result->getMaterial()->getMaterialIndex();
		if (result->getSkinWidth()!=-1.0f)
			boxShape.skinWidth = result->getSkinWidth();

		actorDesc.shapes.pushBack(&boxShape);

		//////////////////////////////////////////////////////////////////////////
		//dynamic object  ? 
		if (result->getFlags() & BF_Moving){
			actorDesc.body = &bodyDesc;
		}
		else
			actorDesc.body = NULL;

		//////////////////////////////////////////////////////////////////////////
		//set transformations
		actorDesc.density  = descr->density;

		if (creationFlags & E_OFC_POSITION)
		{
			actorDesc.globalPose.t = pos;	
		}

		if (creationFlags & E_OFC_POSITION)
		{
			actorDesc.globalPose.M = rot;
		}
		
				//////////////////////////////////////////////////////////////////////////
		//create the actor

		int v = actorDesc.isValid();
		NxActor *actor  = world->getScene()->createActor(actorDesc);
		if (!actor)
		{
			xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Couldn't create actor");
			delete result;
			return NULL;

		}


		//////////////////////////////////////////////////////////////////////////
		//set additional settings : 
		result->setActor(actor);
		actor->setName(referenceObject->GetName());
		actor->userData= result;

		if (result->getFlags() & BF_Moving)
		{
			VxVector massOffsetOut;referenceObject->Transform(&massOffsetOut,&result->getMassOffset());
			actor->setCMassOffsetGlobalPosition(pMath::getFrom(massOffsetOut));
		}

		if (result->getFlags() & BF_Kinematic)
		{
			actor->raiseBodyFlag(NX_BF_KINEMATIC);
		}

		result->enableCollision((result->getFlags() & BF_Collision));
		if (result->getFlags() & BF_Moving)
		{
			if (!(result->getFlags() & BF_Gravity))
			{
				actor->raiseBodyFlag(NX_BF_DISABLE_GRAVITY);
			}
		}

		NxShape *shape  = result->getShapeByIndex();
		if (shape)
		{
			pSubMeshInfo *sInfo  = new pSubMeshInfo();
			sInfo->entID = referenceObject->GetID();
			sInfo->refObject  = (CKBeObject*)referenceObject;
			shape->userData = (void*)sInfo;
			result->setMainShape(shape);
			shape->setName(referenceObject->GetName());
		}
	}
		
	return result;
}

pRigidBody*pFactory::createBody(CK3dEntity *referenceObject,CK3dEntity *worldReferenceObject,NXU::NxActorDesc *desc,int flags)
{

#ifdef _DEBUG
	assert(referenceObject);
	assert(desc);
#endif 
	
	/************************************************************************/
	/*                                                                      
	



	*/
	/************************************************************************/
	VxVector vpos;
	referenceObject->GetPosition(&vpos);

	VxQuaternion vquat;
	referenceObject->GetQuaternion(&vquat);

	NxQuat nrot  = desc->globalPose.M;
	NxVec3 npos = desc->globalPose.t;

	NxMat33 rotX;
	rotX.rotX( PI / 2.0f );
	NxMat33 rotZ;
	rotZ.rotZ( PI );

	NxMat34 rotMat;
	rotMat.M.multiply( rotZ, rotX );

	NxMat34 posMat( true );

/*	posMat.t.set(  -mTerrain->getPosition().x,
		mTerrain->getPosition().y,
		mTerrain->getPosition().z );
*/
	desc->globalPose.multiply( posMat, rotMat );


	NxQuat nrot2  = desc->globalPose.M;
	NxVec3 npos2 = desc->globalPose.t;

	VxQuaternion nvm  = getFromStream(nrot);

	VxVector nvpos = getFromStream(npos);

	

	for (NxU32 k=0; k<desc->mShapes.size(); k++)
	{
		NXU::NxShapeDesc *shape = desc->mShapes[k];
		NxVec3 locPos = shape->localPose.t;
		NxQuat localQuad = shape->localPose.M;
	}


	int op  = 2; 



	return NULL;
}



pRigidBody*pFactory::createRigidBodyFull(CK3dEntity *referenceObject, CK3dEntity *worldReferenceObject)
{


	//################################################################
	//
	// Sanity checks
	//
	pWorld *world=getManager()->getWorld(worldReferenceObject,referenceObject); 
	pRigidBody*result = world->getBody(referenceObject);
	

	if(result)
	{
		result->destroy();
		delete result;
		result = NULL;
	}
	
	
	//################################################################
	//
	// Construct the result
	//
	result  =  createBody(referenceObject,worldReferenceObject);
	if (result)
	{


		result->setWorld(world);


		//----------------------------------------------------------------
		//
		// Handle different attribute types (Object or pBSetup)
		//

		int attTypeOld = GetPMan()->GetPAttribute();
		int attTypeNew = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_ACTOR);

		pObjectDescr *oDescr = NULL;
		
		
		//----------------------------------------------------------------
		//
		// the old way : 
		//

		if (referenceObject->HasAttribute(attTypeOld))
		{
			result->retrieveSettingsFromAttribute();
			oDescr = pFactory::Instance()->createPObjectDescrFromParameter(referenceObject->GetAttributeParameter(GetPMan()->GetPAttribute()));

			

		}
		
		bool hierarchy = result->getFlags() & BF_Hierarchy;
		bool isDeformable = result->getFlags() & BF_Deformable;
		bool  trigger = (result->getFlags() & BF_TriggerShape);


		//----------------------------------------------------------------
		//
		// the new way 
		//

		if (referenceObject->HasAttribute(attTypeNew))
		{
			oDescr = new pObjectDescr();

		}

		
		result->checkDataFlags();

		
		

		/*
		pObjectDescr *oDescr = 
		if (!oDescr)
			return result;

		*/

		//################################################################
		//
		//	Older versions have the hierarchy mode settings not the body flags
		//	We migrate it :
		//

		if (oDescr->hirarchy)
		{
			result->setFlags( (result->getFlags() | BF_Hierarchy ));
		}
		if (hierarchy)
		{
			oDescr->hirarchy = hierarchy;
		}
		//################################################################
		//
		// Deformable ?
		//

		
		pCloth *cloth = NULL;
		pClothDesc cDescr;
		
		if (isDeformable)
		{
			cDescr.setToDefault();
			cDescr.worldReference = worldReferenceObject->GetID();

			if ( result->getFlags() & BF_Gravity )
			{
				cDescr.flags |= PCF_Gravity;
			}

			if ( result->getFlags() & BF_Collision )
			{

			}

			if (!cloth)
			{
				cloth = pFactory::Instance()->createCloth(referenceObject,cDescr);
				if (!cloth)
				{
					xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Cannot create cloth : factory object failed !");
				}
			}
		}

		
		//################################################################
		//
		// Some settings
		//
		if (result->getSkinWidth()==-1.0f)
		{
			result->setSkinWidth(0.01f);
		}

		float radius = referenceObject->GetRadius();
		if (referenceObject->GetRadius() < 0.001f )
		{
			radius  = 1.0f;
		}

		float density = result->getDensity();
		VxVector box_s= BoxGetZero(referenceObject);
		
		//################################################################
		//
		// Calculate destination matrix 
		//

		VxMatrix v_matrix ;
		VxVector position,scale;		
		VxQuaternion quat;	


		v_matrix = referenceObject->GetWorldMatrix();
		Vx3DDecomposeMatrix(v_matrix,quat,position,scale);
		
		NxVec3 pos = pMath::getFrom(position);
		NxQuat rot = pMath::getFrom(quat);


		//////////////////////////////////////////////////////////////////////////
		//create actors description
		NxActorDesc actorDesc;actorDesc.setToDefault();
		NxBodyDesc bodyDesc;bodyDesc.setToDefault();

		
		NxMaterialDesc *materialDescr = NULL;
		NxMaterial *material = NULL;

		

		
		switch(result->getHullType())
		{

			//////////////////////////////////////////////////////////////////////////
			case HT_Box:
			{
				NxBoxShapeDesc shape;

				if (! isDeformable )
				{
					shape.dimensions = pMath::getFrom(box_s)*0.5f;
				}
				
				shape.density  = density;
				//shape.localPose.t = pMath::getFrom(shapeOffset);
				if (result->getSkinWidth()!=-1.0f)
					shape.skinWidth = result->getSkinWidth();

				actorDesc.shapes.pushBack(&shape);

				
				break;
			}
			//////////////////////////////////////////////////////////////////////////
			case HT_Sphere:
			{
				NxSphereShapeDesc shape;
				
				
				if (! isDeformable )
				{
					shape.radius = radius;
				}
				
				shape.density  = density;
				//shape.localPose.t = pMath::getFrom(shapeOffset);
				if (result->getSkinWidth()!=-1.0f)
					shape.skinWidth = result->getSkinWidth();

				actorDesc.shapes.pushBack(&shape);
				
				break;
			}
			//////////////////////////////////////////////////////////////////////////
			case HT_Mesh:
			{

				if (! (result->getFlags() & BF_Deformable) )
				{

					//xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Can not use a mesh as de");
					//return NULL;

				}
	
				NxTriangleMeshDesc myMesh;
				myMesh.setToDefault();

				createMesh(world->getScene(),referenceObject->GetCurrentMesh(),myMesh);

				NxTriangleMeshShapeDesc shape;
				bool status = InitCooking();
				if (!status) {
						xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Couldn't initiate cooking lib!");
						return NULL;
				}
				MemoryWriteBuffer buf;

				status = CookTriangleMesh(myMesh, buf);
				if (!status) {
						xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Couldn't cook mesh!");
						return NULL;
				}
				shape.meshData	= getManager()->getPhysicsSDK()->createTriangleMesh(MemoryReadBuffer(buf.data));
				shape.density  = density;
			//	shape.materialIndex = result->getMaterial()->getMaterialIndex();
				actorDesc.shapes.pushBack(&shape);
				//shape.localPose.t = pMath::getFrom(shapeOffset);
				if (result->getSkinWidth()!=-1.0f)
					shape.skinWidth = result->getSkinWidth();
				CloseCooking();

				if (myMesh.points)
				{
					delete [] myMesh.points;
				}

				if (myMesh.triangles)
				{
					delete []myMesh.triangles;
				}

				break;
			}
			//////////////////////////////////////////////////////////////////////////
			case HT_ConvexMesh:
			{
				if (referenceObject->GetCurrentMesh())
				{
					if (referenceObject->GetCurrentMesh()->GetVertexCount()>=256 )
					{
						xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Only 256 vertices for convex meshs allowed, by Ageia!");
						goto nothing;
					}
				}else
				{
					xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Object has no mesh!");
					goto nothing;


				}

				NxConvexMeshDesc myMesh;
				myMesh.setToDefault();
				createConvexMesh(world->getScene(),referenceObject->GetCurrentMesh(),myMesh);

				NxConvexShapeDesc shape;
				bool status = InitCooking();
				if (!status) {
						xLogger::xLog(XL_START,ELOGERROR,E_LI_AGEIA,"Couldn't initiate cooking lib!");
						goto nothing;
						
				}
				MemoryWriteBuffer buf;

				status = CookConvexMesh(myMesh, buf);
				if (!status) {
						xLogger::xLog(XL_START,ELOGERROR,E_LI_AGEIA,"Couldn't cook convex mesh!");
						goto nothing;
				}
				shape.meshData	= getManager()->getPhysicsSDK()->createConvexMesh(MemoryReadBuffer(buf.data));
				shape.density  = density;
			//	shape.materialIndex = result->getMaterial()->getMaterialIndex();
				actorDesc.shapes.pushBack(&shape);
				//shape.localPose.t = pMath::getFrom(shapeOffset);
				if (result->getSkinWidth()!=-1.0f)
					shape.skinWidth = result->getSkinWidth();
				int h = shape.isValid();
				CloseCooking();


				if (myMesh.points)
				{
					delete [] myMesh.points;
				}

				if (myMesh.triangles)
				{
					delete []myMesh.triangles;
				}


				break;
			}

			//////////////////////////////////////////////////////////////////////////
			case HT_ConvexCylinder:
			{
				NxConvexShapeDesc shape;
				if (!_createConvexCylinder(&shape,referenceObject))
					xLogger::xLog(XL_START,ELOGERROR,E_LI_AGEIA,"Couldn't create convex cylinder mesh");

				shape.density  = density;
				if (result->getSkinWidth()!=-1.0f)
					shape.skinWidth = result->getSkinWidth();
				
				actorDesc.shapes.pushBack(&shape);
				break;
			}
			//////////////////////////////////////////////////////////////////////////
			case HT_Capsule:
			{

				NxCapsuleShapeDesc shape;

				if ( !isDeformable )
				{

					pCapsuleSettings cSettings;
					
					pFactory::Instance()->findSettings(cSettings,referenceObject);
					
					shape.radius = cSettings.radius > 0.0f ? cSettings.radius : box_s.v[cSettings.localRadiusAxis] * 0.5f;
					shape.height = cSettings.height > 0.0f ? cSettings.height : box_s.v[cSettings.localLengthAxis]  -  ( 2*shape.radius) ;
				}
				shape.density  = density;
			//	shape.materialIndex = result->getMaterial()->getMaterialIndex();
			//	shape.localPose.t = pMath::getFrom(shapeOffset);
				if (result->getSkinWidth()!=-1.0f)
					shape.skinWidth = result->getSkinWidth();
				actorDesc.shapes.pushBack(&shape);
				break;
			}

			case HT_Wheel:
			{
				//		xLogger::xLog(XL_START,ELOGERROR,E_LI_AGEIA,"Couldn't create convex cylinder mesh");


				/*
				NxWheelShapeDesc shape;
				shape.radius = box_s.z*0.5f;
				shape.density  = density;
				if (result->getSkinWidth()!=-1.0f)
					shape.skinWidth = result->getSkinWidth();

				if (referenceObject && referenceObject->HasAttribute(GetPMan()->att_wheelDescr ))
				{
					CKParameter *par = referenceObject->GetAttributeParameter(GetPMan()->att_wheelDescr );
					if (par)
					{
						pWheelDescr *wheelDescr = pFactory::Instance()->copyTo(par);
						if (wheelDescr)
						{

							float heightModifier = (wheelDescr->wheelSuspension + radius ) / wheelDescr->wheelSuspension;
							shape.suspension.damper = wheelDescr->springDamping * heightModifier;
							shape.suspension.targetValue = wheelDescr->springBias * heightModifier;
							shape.suspensionTravel = wheelDescr->wheelSuspension;

							shape.lateralTireForceFunction.stiffnessFactor *= wheelDescr->frictionToSide;
							shape.longitudalTireForceFunction.stiffnessFactor*=wheelDescr->frictionToFront;
							shape.inverseWheelMass = 0.1;
							int isValid = shape.isValid();
							

							actorDesc.shapes.pushBack(&shape);
						}
					}else
					{
						XString name = result->GetVT3DObject()->GetName();
						name << " needs to have an additional wheel attribute attached ! ";
						xLogger::xLog(XL_START,ELOGERROR,E_LI_AGEIA,name.CStr());
					}
				}
				*/
				break;
			}
		}

		
		//////////////////////////////////////////////////////////////////////////
		//dynamic object  ? 
		if (result->getFlags() & BF_Moving){
			actorDesc.body = &bodyDesc;
		}
		else
			actorDesc.body = NULL;

		//////////////////////////////////////////////////////////////////////////
		//set transformations
		actorDesc.density  = result->getDensity();


		if ( !isDeformable)
		{
			actorDesc.globalPose.t = pos;	
			actorDesc.globalPose.M = rot;
		}
		
		
		//////////////////////////////////////////////////////////////////////////
		//create the actor

		int v = actorDesc.isValid();
		NxActor *actor  = world->getScene()->createActor(actorDesc);
		if (!actor)
		{
			xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Couldn't create actor");
			delete result;
			return NULL;
			
		}

		//////////////////////////////////////////////////////////////////////////
		//set additional settings : 
		result->setActor(actor);
		actor->setName(referenceObject->GetName());
		actor->userData= result;

		//////////////////////////////////////////////////////////////////////////
		//Deformable : 
		if (isDeformable && cloth)
		{

			pDeformableSettings dSettings;
			dSettings.ImpulsThresold = 50.0f;
			dSettings.PenetrationDepth= 0.1f ;
			dSettings.MaxDeform = 2.0f;

			CKParameterOut *poutDS = referenceObject->GetAttributeParameter(GetPMan()->att_deformable);
			if (poutDS)
			{
				pFactory::Instance()->copyTo(dSettings,poutDS);
			}
			cloth->attachToCore(referenceObject,dSettings.ImpulsThresold,dSettings.PenetrationDepth,dSettings.MaxDeform);
			result->setCloth(cloth);
		}


		//////////////////////////////////////////////////////////////////////////
		//
		//	Extra settings : 
		//

		if (result->getFlags() & BF_Moving)
		{
			VxVector massOffsetOut;referenceObject->Transform(&massOffsetOut,&result->getMassOffset());
			actor->setCMassOffsetGlobalPosition(pMath::getFrom(massOffsetOut));
		}

		
		if (result->getFlags() & BF_Kinematic)
		{
			result->setKinematic(true);
		}

		
		
		if (result->getFlags() & BF_Moving )
		{
			result->enableGravity(result->getFlags() & BF_Gravity);
		}

		//----------------------------------------------------------------
		//
		// Special Parameters
		//

		//-	look for optimization attribute :
		result->checkForOptimization();

		

		
		//----------------------------------------------------------------
		//
		// store mesh meta info in the first main mesh
		//
		NxShape *shape  = result->getShapeByIndex();
		if (shape)
		{
			pSubMeshInfo *sInfo  = new pSubMeshInfo();
			sInfo->entID = referenceObject->GetID();
			sInfo->refObject  = (CKBeObject*)referenceObject;
			shape->userData = (void*)sInfo;
			result->setMainShape(shape);
			shape->setName(referenceObject->GetName());

			pMaterial bMaterial;
			bool hasMaterial = pFactory::Instance()->findSettings(bMaterial,referenceObject);
			if (!hasMaterial)
			{
				if (world->getDefaultMaterial())
				{
					int z = (int)world->getDefaultMaterial()->userData;
					shape->setMaterial(world->getDefaultMaterial()->getMaterialIndex());
					//pFactory::Instance()->copyTo(bMaterial,world->getDefaultMaterial());
				}
			}else{
				
				NxMaterialDesc nxMatDescr;
				pFactory::Instance()->copyTo(nxMatDescr,bMaterial);
				NxMaterial *nxMaterial  = world->getScene()->createMaterial(nxMatDescr);
				if (nxMaterial)
				{
					shape->setMaterial(nxMaterial->getMaterialIndex());
				}
			}
		}

		result->enableCollision( (result->getFlags() & BF_Collision), referenceObject );

        
		//- handle collisions setup
		if (oDescr->version == pObjectDescr::E_OD_VERSION::OD_DECR_V1)
			result->updateCollisionSettings(*oDescr,referenceObject);





		xLogger::xLog(ELOGINFO,E_LI_MANAGER,"Rigid body creation successful : %s",referenceObject->GetName());

		//----------------------------------------------------------------
		//
		// Parse hierarchy 
		//
		if (!oDescr->hirarchy)
			return result;

		CK3dEntity* subEntity = NULL;
		while (subEntity= referenceObject->HierarchyParser(subEntity) )
		{
			if (subEntity->HasAttribute(GetPMan()->GetPAttribute()))
			{
				pObjectDescr *subDescr = pFactory::Instance()->createPObjectDescrFromParameter(subEntity->GetAttributeParameter(GetPMan()->GetPAttribute()));
				if (subDescr->flags & BF_SubShape)
				{

					//////////////////////////////////////////////////////////////////////////
					//
					//			Regular Mesh : 
					//

					if (subDescr->hullType != HT_Cloth)
					{
						result->addSubShape(NULL,*oDescr,subEntity);
					}

					//////////////////////////////////////////////////////////////////////////
					//
					//			Cloth Mesh : 
					//
					if (subDescr->hullType == HT_Cloth)
					{
						//pClothDesc *cloth = pFactory::Instance()->createPObjectDescrFromParameter(subEntity->GetAttributeParameter(GetPMan()->GetPAttribute()));
					}
				}
			}
		}

		if (oDescr->hirarchy)
		{
		

			if (oDescr->newDensity!=0.0f || oDescr->totalMass!=0.0f )
			{
				result->updateMassFromShapes(oDescr->newDensity,oDescr->totalMass);
			}
		}


		return result;

	}
	nothing:

	

	return result;
}
pRigidBody*pFactory::createBody(CK3dEntity *referenceObject,CK3dEntity *worldReferenceObject)
{
	////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
	//
	//	Pseudo code : 
	//
	//	1. check the passed world, otherwise use+create the physic managers default world :
	//	1.  
	//////////////////////////////////////////////////////////////////////////

	if (!referenceObject)
	{
		return NULL;
	}

	pWorld *world=GetPMan()->getWorld(worldReferenceObject,referenceObject);
	if (world)
	{
		int p = 0; 
	}

	if (!world)
	{
		return NULL;
	}

	pRigidBody *result  = new pRigidBody(referenceObject,world);

	return result;

}






pRigidBody*pFactory::createBody(CK3dEntity *referenceObject,pObjectDescr description,CK3dEntity *worldReferenceObject/* =NULL */)
{
	using namespace vtTools::BehaviorTools;
	using namespace vtTools;
	using namespace vtTools::AttributeTools;

	pRigidBody *result  = NULL;

	if (!referenceObject)
		return result;

	pWorld *world=getManager()->getWorld(worldReferenceObject,referenceObject); 
	if (!world)
	{
		return result;
	}

	if (referenceObject->HasAttribute(GetPMan()->GetPAttribute()))
	{
		referenceObject->RemoveAttribute(GetPMan()->GetPAttribute());
	}
	referenceObject->SetAttribute(GetPMan()->GetPAttribute());


	int htype   = description.hullType;
	int flags  = description.flags;
	SetAttributeValue<xBool>(referenceObject,GetPMan()->GetPAttribute(),E_PPS_HIRARCHY,&description.hirarchy);
	SetAttributeValue<int>(referenceObject,GetPMan()->GetPAttribute(),E_PPS_HULLTYPE,&htype);
	SetAttributeValue<int>(referenceObject,GetPMan()->GetPAttribute(),E_PPS_BODY_FLAGS,&flags);
	SetAttributeValue<float>(referenceObject,GetPMan()->GetPAttribute(),E_PPS_DENSITY,&description.density);
	SetAttributeValue<float>(referenceObject,GetPMan()->GetPAttribute(),E_PPS_NEW_DENSITY,&description.newDensity);
	SetAttributeValue<float>(referenceObject,GetPMan()->GetPAttribute(),E_PPS_TOTAL_MASS,&description.totalMass);
	SetAttributeValue<VxVector>(referenceObject,GetPMan()->GetPAttribute(),E_PPS_MASS_OFFSET,&description.massOffset);

	CK_ID wid = world->getReference()->GetID();
	AttributeTools::SetAttributeValue<CK_ID>(referenceObject,GetPMan()->GetPAttribute(),E_PPS_WORLD,&wid);

	result = pFactory::Instance()->createRigidBodyFull(referenceObject,world->getReference());
	if (result)
	{
		result->translateLocalShapePosition(description.shapeOffset);
		result->setCMassOffsetLocalPosition(description.massOffset);
	}

	if ( ! (description.flags & BF_AddAttributes) )
	{
		referenceObject->RemoveAttribute(GetPMan()->GetPAttribute());
	}

	return result;


}

CK3dEntity *pFactory::getMostTopParent(CK3dEntity*ent)
{

	if (!ent)
	{
		return NULL;
	}

	CK3dEntity *result = ent->GetParent();
	if (result)
	{
		int attTypePBSetup  = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_ACTOR);
		//if ( (!result->HasAttribute(GetPMan()->GetPAttribute())) || !(result->HasAttribute(attTypePBSetup)))
		if ( (result->HasAttribute(GetPMan()->GetPAttribute())) || (result->HasAttribute(attTypePBSetup)))
		{
			return ent;
		}
		if (result->GetParent())
		{
			return getMostTopParent(result);
		}
		else
		{
			return result;
		}
	}else
	{
		return ent;
	}
}




/*
template<class T>class MODULE_API xImplementationObject2
{
public:

//xLinkedObjectStorage() : mInternalId(-1) , mClassId(-1) {};
typedef void* xImplementationObject2<T>::*StoragePtr;
xImplementationObject2(){}
xImplementationObject2(StoragePtr storage)
{ 

} 
void setStorage(StoragePtr* stPtr)
{
mStorage = stPtr;
}
T getImpl() { return mObject; }


protected:
private:
T mObject;
StoragePtr mStorage;
};



class ATest : public xImplementationObject2<NxActor*>
{

public: 

ATest();
};

NxActor *actor = NULL;

ATest::ATest() : xImplementationObject2<NxActor*>(actor->userData)
{


//	setStorage(actor->userData);

}
*/