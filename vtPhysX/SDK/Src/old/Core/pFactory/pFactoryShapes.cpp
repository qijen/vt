#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "Stream.h"
#include "cooking.h"
#include "IParameter.h"

#include <xDebugTools.h>


NxShape *pFactory::cloneShape(CK3dEntity *src,CK3dEntity *dst,CK3dEntity*dstBodyReference,int copyFlags,int bodyFlags/* =0 */)
{

	if (!src || !dst )
		return NULL;

	pRigidBody *srcBody  = GetPMan()->getBody(src);
	if (!srcBody)
		return NULL;

	pRigidBody *dstBody  = GetPMan()->getBody(dstBodyReference);
	if (!dstBody)
		return NULL;
	
	if (dstBody->isSubShape(dst))
		return NULL;


	NxShape *srcShape = srcBody->getSubShape(src);
	if (!srcShape)
		return NULL;

	bool  isSubShape=srcBody->isSubShape(src);


	pObjectDescr oDescr;
	int attTypePBSetup  = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_ACTOR);


	//----------------------------------------------------------------
	//
	// Try to copy it directly from the source object
	//
	if ( !src->HasAttribute(attTypePBSetup) && isSubShape )
	{
		pSubMeshInfo* sInfo = (pSubMeshInfo*)srcShape->userData;
		if (sInfo )
		{
			const pObjectDescr &srcDescr = sInfo->initDescription;
			memcpy(&oDescr,&srcDescr,sizeof(pObjectDescr));
		}
	}

	//----------------------------------------------------------------
	//
	// Try to copy it directly from the source objects attribute
	//

	if (dst->HasAttribute(attTypePBSetup))
	{
		//----------------------------------------------------------------
		//
		// fill object description
		//
		
		CKParameterOut *par =  dst->GetAttributeParameter(attTypePBSetup);
		if (!par)
			return 0;

		IParameter::Instance()->copyTo(&oDescr,par);
		oDescr.version  = pObjectDescr::E_OD_VERSION::OD_DECR_V1;

	}


	//----------------------------------------------------------------
	//
	// adjust data
	//


	//----------------------------------------------------------------
	//
	// find pivot settings 
	//
	if ( (copyFlags & PB_CF_PIVOT_SETTINGS ) )
	{
		//check in objects attribute
		int attPivot  = GetPMan()->getAttributeTypeByGuid(VTS_PHYSIC_PIVOT_OFFSET);
	}


	if (oDescr.flags & BF_SubShape)
	{
		if (oDescr.hullType == HT_Wheel)
		{
			VxVector loc;
			pWheel *wheel = createWheel(dstBodyReference,dst,oDescr.wheel,oDescr.convexCylinder,loc);
			if(wheel && wheel->castWheel2() )
				return wheel->castWheel2()->getWheelShape();
			else
				return NULL;

		}else if( oDescr.hullType !=  HT_Cloth)
		{

			srcBody->addSubShape(NULL,oDescr,dst);
			NxShape *result = srcBody->getSubShape(dst);
			if (result)
			{
				//----------------------------------------------------------------
				//
				// Adjust mass 
				//
				if ( ( copyFlags & PB_CF_MASS_SETTINGS) )
				{
				/*	srcBody->updateMassSettings(srcBody->getInitialDescription()->mass);
				
						result->updateMassSettings(oDescr.mass);
					else if(pFactory::Instance()->findSettings(oDescr.mass,referenceObject))
						result->updateMassSettings(oDescr.mass);
						*/
				
				}
				return result;
			}
		}
	}
	return NULL;

	/*
	int sType  = vtAgeia::getHullTypeFromShape(srcShape);

	switch(sType)
	{

		case HT_Sphere:
			{
				NxSphereShape *tShape = srcShape->isSphere();
				NxSphereShapeDesc old;
				tShape->saveToDesc(old);

			}
	}
	

	*/


}

NxShape *pFactory::createShape(CK3dEntity *bodyReference,pObjectDescr descr,CK3dEntity *srcReference,CKMesh *mesh,VxVector localPos,VxQuaternion localRotation)
{
	
	NxShape *result = NULL;
	if (!bodyReference || !mesh )
	{
		return result;
	}
	pRigidBody *body=GetPMan()->getBody(bodyReference);

	if (!body)
		return NULL;

	NxActor *actor = body->getActor();

	if (!actor)
		return NULL;

	int hType = descr.hullType;
	VxVector box_s = mesh->GetLocalBox().GetSize();
	float density  = descr.density;
	float skinWidth   = descr.skinWidth;
	float radius = mesh->GetRadius();
	NxQuat rot = pMath::getFrom(localRotation);
	NxVec3 pos = pMath::getFrom(localPos);
	CK_ID srcID = mesh->GetID();


	pWheel *wheel = NULL;
	pSubMeshInfo *sInfo  = NULL;

	
	switch(hType)
	{

		case HT_ConvexCylinder:
		{
			
			
			
			NxConvexShapeDesc shape;
			pConvexCylinderSettings &cSettings = descr.convexCylinder;

			iAssertW( ( descr.mask & OD_ConvexCylinder),
				pFactory::Instance()->findSettings(cSettings,srcReference),
				"Hull type has been set to convex cylinder but there is no descriptions \
				passed or activated in the pObjectDescr::mask.Trying object attributes....");

			if (cSettings.radius.reference)
				cSettings.radius.evaluate(cSettings.radius.reference);

			if (cSettings.height.reference)
				cSettings.height.evaluate(cSettings.height.reference);

			cSettings.radius.value = cSettings.radius.value > 0.0f ? (cSettings.radius.value*0.5) : (box_s.v[cSettings.radius.referenceAxis] * 0.5f);
			cSettings.height.value = cSettings.height.value > 0.0f ? cSettings.height.value : (box_s.v[cSettings.height.referenceAxis] * 0.5f);

			iAssertW( cSettings.isValid() , cSettings.setToDefault(),"");
			bool resultAssert = true;

			iAssertWR( pFactory::Instance()->_createConvexCylinderMesh(&shape,cSettings,srcReference),"",resultAssert);
			/*
			NxConvexShapeDesc shape;
			if (!_createConvexCylinder(&shape,srcReference))
				xLogger::xLog(XL_START,ELOGERROR,E_LI_AGEIA,"Couldn't create convex cylinder mesh");
*/
			shape.density  = density;
			shape.localPose.t = pos;
			shape.localPose.M = rot;
			shape.skinWidth = skinWidth;

			result = actor->createShape(shape);

			break;
		}

		//////////////////////////////////////////////////////////////////////////
		case HT_Box:
		{
			NxBoxShapeDesc shape;
			shape.dimensions = pMath::getFrom(box_s)*0.5f;
			shape.density  = density;
			shape.localPose.t = pos;
			shape.localPose.M = rot;
			shape.skinWidth = skinWidth;
			result = actor->createShape(shape);
			break;
		}
		//////////////////////////////////////////////////////////////////////////
		case HT_Sphere:
		{
			NxSphereShapeDesc shape;
			shape.radius = radius;
			shape.density  = density;
			shape.localPose.M = rot;
			shape.localPose.t = pMath::getFrom(localPos);
			if (skinWidth!=-1.0f)
				shape.skinWidth = skinWidth;
			result = actor->createShape(shape);
			break;
		}
		//////////////////////////////////////////////////////////////////////////
		case HT_Mesh:
		{
			NxTriangleMeshDesc myMesh;
			myMesh.setToDefault();

			pFactory::Instance()->createMesh(&actor->getScene(),mesh,myMesh);

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
			shape.meshData	= GetPMan()->getPhysicsSDK()->createTriangleMesh(MemoryReadBuffer(buf.data));
			shape.density  = density;
			shape.localPose.M = rot;
			shape.localPose.t = pMath::getFrom(localPos);
			if (skinWidth!=-1.0f)
				shape.skinWidth = skinWidth;
			result = actor->createShape(shape);
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
		
			if (mesh->GetVertexCount()>=256 )
			{
				xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Only 256 vertices for convex meshes allowed, by Ageia!");
				goto nothing;
			}

			NxConvexMeshDesc myMesh;
			myMesh.setToDefault();
			pFactory::Instance()->createConvexMesh(&actor->getScene(),mesh,myMesh);

			NxConvexShapeDesc shape;
			bool status = InitCooking();
			if (!status) 
			{
				xLogger::xLog(XL_START,ELOGERROR,E_LI_AGEIA,"Couldn't initiate cooking lib!");
				goto nothing;
			}
			MemoryWriteBuffer buf;

			status = CookConvexMesh(myMesh, buf);
			if (!status) {
				xLogger::xLog(XL_START,ELOGERROR,E_LI_AGEIA,"Couldn't cook convex mesh!");
				goto nothing;
			}
			shape.meshData	= GetPMan()->getPhysicsSDK()->createConvexMesh(MemoryReadBuffer(buf.data));
			shape.density  = density;
			
			shape.localPose.M = rot;
			shape.localPose.t = pMath::getFrom(localPos);
			if (skinWidth!=-1.0f)
				shape.skinWidth = skinWidth;
			result = actor->createShape(shape);
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
		case HT_Capsule:
		{
			NxCapsuleShapeDesc shape;

			pCapsuleSettingsEx &cSettings = descr.capsule;
			
			iAssertW( ( descr.mask & OD_Capsule),
				pFactory::Instance()->findSettings(cSettings,srcReference),
				"Hull type has been set to convex cylinder but there is no descriptions \
				passed or activated in the pObjectDescr::mask.Trying object attributes....");

			bool resultAssert = true;

			if (cSettings.radius.reference)
				cSettings.radius.evaluate(cSettings.radius.reference);

			if (cSettings.height.reference)
				cSettings.height.evaluate(cSettings.height.reference);

			iAssertWR(cSettings.isValid(),cSettings.setToDefault(),resultAssert);

			shape.radius = cSettings.radius.value > 0.0f ? (cSettings.radius.value*0.5) : (box_s.v[cSettings.radius.referenceAxis] * 0.5f);
			shape.height = cSettings.height.value > 0.0f ? (cSettings.height.value-( 2*shape.radius)) : (box_s.v[cSettings.height.referenceAxis]  -  ( 2*shape.radius)) ;

			shape.density  = density;
			shape.localPose.M = rot;
			shape.localPose.t = pos;
			shape.skinWidth = skinWidth;
			result = actor->createShape(shape);
			break;
		}
		//////////////////////////////////////////////////////////////////////////
		case HT_Wheel:
		{
			
			pWheelDescr &wheelDescr = descr.wheel;
			CKParameterOut *wheelParameter  = NULL;

			//----------------------------------------------------------------
			//
			// determine wheel settings 
			//
			if (!(descr.mask & OD_Wheel))
			{
				wheelParameter = pFactory::Instance()->findSettings(wheelDescr,srcReference);
			}

			bool resultAssert = true;
			iAssertWR(wheelDescr.isValid(),wheelDescr.setToDefault(),resultAssert);
			//----------------------------------------------------------------
			//
			// determine radius
			//
			if (wheelDescr.radius.reference == 0 && wheelDescr.radius.value == 0.0f )
			{
				float radiusBestFit = 0.0f;
				if (box_s[0] > radiusBestFit)
					radiusBestFit = box_s[0];

				if (box_s[1] > radiusBestFit)
					radiusBestFit = box_s[1];
				
				if (box_s[2] > radiusBestFit)
					radiusBestFit = box_s[2];
				
				wheelDescr.radius.value = radiusBestFit * 0.5f;

			}

			iAssertW(wheelDescr.radius.isValid(),wheelDescr.radius.evaluate(srcReference),"");
			if(!wheelDescr.radius.isValid())
				wheelDescr.radius.value = srcReference->GetRadius();

			
			VxVector box_s = mesh->GetLocalBox().GetSize();
			float radius = wheelDescr.radius.value > 0.0f ? wheelDescr.radius.value : box_s.v[wheelDescr.radius.referenceAxis] * 0.5f;

			NxWheelShapeDesc shape;
			shape.setToDefault();

			shape.radius = radius;
			shape.localPose.M = rot;
			shape.localPose.t = pos;

			float heightModifier = (wheelDescr.wheelSuspension + radius ) / wheelDescr.wheelSuspension;
			shape.suspension.spring = wheelDescr.springRestitution*heightModifier;
			shape.suspension.damper = wheelDescr.springDamping * heightModifier;
			shape.suspension.targetValue = wheelDescr.springBias * heightModifier;
			shape.suspensionTravel = wheelDescr.wheelSuspension;
			shape.inverseWheelMass = wheelDescr.inverseWheelMass;


			const pTireFunction& latFunc  = wheelDescr.latFunc;
			const pTireFunction& longFunc  = wheelDescr.longFunc;


			NxTireFunctionDesc lngTFD;
			lngTFD.extremumSlip = longFunc.extremumSlip ;
			lngTFD.extremumValue = longFunc.extremumValue;
			lngTFD.asymptoteSlip = longFunc.asymptoteSlip;
			lngTFD.asymptoteValue = longFunc.asymptoteValue;
			lngTFD.stiffnessFactor = longFunc.stiffnessFactor;

			NxTireFunctionDesc latTFD;
			latTFD.extremumSlip = latFunc.extremumSlip ;
			latTFD.extremumValue = latFunc.extremumValue;
			latTFD.asymptoteSlip = latFunc.asymptoteSlip;
			latTFD.asymptoteValue = latFunc.asymptoteValue;
			latTFD.stiffnessFactor = latFunc.stiffnessFactor;


			shape.lateralTireForceFunction =latTFD;
			shape.longitudalTireForceFunction =lngTFD;
			shape.wheelFlags  =wheelDescr.wheelShapeFlags;

			//----------------------------------------------------------------
			//
			// evaluate wheel settings into attribute parameters 
			//
			if (wheelParameter)
			{
				IParameter::Instance()->copyTo(wheelParameter,descr.wheel);

			}

			result = actor->createShape(shape);

		}
	}

	if(!result)
	{
		return NULL;
	}

	//----------------------------------------------------------------
	//
	// add sub mesh meta

	//
	sInfo  = new pSubMeshInfo();

	sInfo->meshID = srcID;
	sInfo->mesh = mesh;

	sInfo->entID = srcReference->GetID();
	sInfo->refObject  = srcReference;
	
	result->setName(srcReference->GetName());
	result->userData = (void*)sInfo;
	sInfo->initDescription = descr;

	//----------------------------------------------------------------
	//
	// wheel extra data
	//
	if ( descr.hullType == HT_Wheel )
	{
		sInfo->wheel = new pWheel2(body,&descr.wheel);
		int a  = (descr.wheel.wheelFlags & WF_VehicleControlled) ? true : false;
		int b  = (descr.wheel.wheelFlags & WF_Accelerated)  ? true : false;
		((pWheel2*)sInfo->wheel)->setWheelShape((NxWheelShape*)result->isWheel());
		sInfo->wheel->setEntID(srcReference->GetID());
		sInfo->wheel->mWheelFlags = descr.wheel.wheelFlags;

		
	}

	//----------------------------------------------------------------
	//
	// Material
	//
	if ((descr.mask & OD_Material))
		body->updateMaterialSettings(descr.material,srcReference);
	else if(pFactory::Instance()->findSettings(descr.material,srcReference))
		body->updateMaterialSettings(descr.material,srcReference);


	//----------------------------------------------------------------
	//
	// Collision
	//
	if ((descr.mask & OD_Collision))
		body->updateCollisionSettings(descr.collision,srcReference);
	else if(pFactory::Instance()->findSettings(descr.collision,srcReference))
		body->updateCollisionSettings(descr.collision,srcReference);


	//----------------------------------------------------------------
	//
	// Adjust pivot 
	//
	
	if ( (descr.mask & OD_Pivot) )
	{
		iAssertW1( descr.pivot.isValid(),descr.pivot.setToDefault());
		body->updatePivotSettings(descr.pivot,srcReference);
	}else if(pFactory::Instance()->findSettings(descr.pivot,srcReference))
		body->updatePivotSettings(descr.pivot,srcReference);


	//----------------------------------------------------------------
	//
	// post 
	//
	if (descr.flags & BF_TriggerShape )
	{
		result->setFlag(NX_TRIGGER_ENABLE,TRUE);
	}

	//----------------------------------------------------------------
	//
	// Mass
	//
	if ((descr.mask & OD_Mass))
		body->updateMassSettings(descr.mass);
	else if(pFactory::Instance()->findSettings(descr.mass,srcReference))
		body->updateMassSettings(descr.mass);

	return result;

	nothing : 
	return NULL;

}

bool pFactory::_createConvexCylinderMesh(NxConvexShapeDesc *dstShapeDescription,pConvexCylinderSettings& srcSettings,CK3dEntity*referenceObject)
{



#ifdef _DEBUG
	assert(referenceObject);	// <- should never happen !
#endif // _DEBUG

	bool result  = false;

	
/*	srcSettings.radius.value *=0.5f;
	srcSettings.height.value *=0.5f;
*/
	NxArray<NxVec3> points;
	NxVec3 center(0,0,0);

	NxVec3 frontAxis =  getFrom(srcSettings.forwardAxis);				// = wheelDesc->downAxis.cross(wheelDesc->wheelAxis);
	NxVec3 downAxis = getFrom(srcSettings.downAxis);//downAxis *=-1.0;	//  = wheelDesc->downAxis;
	NxVec3 wheelAxis  = getFrom(srcSettings.rightAxis);				// = wheelDesc->wheelAxis;


	//frontAxis.normalize();
	frontAxis *= srcSettings.radius.value;
	//downAxis.normalize();
	downAxis *= srcSettings.radius.value;
	//wheelAxis.normalize();
	wheelAxis *= srcSettings.height.value;

	NxReal step;

	if(srcSettings.buildLowerHalfOnly) 
	{
		if((srcSettings.approximation& 0x1) == 0)
			srcSettings.approximation++;

		step = (NxReal)(NxTwoPi) / (NxReal)(srcSettings.approximation*2);
	}
	else 
	{
		step = (NxReal)(NxTwoPi) / (NxReal)(srcSettings.approximation);
	}
	for(NxU32 i = 0; i < srcSettings.approximation; i++) 
	{
		NxReal iReal;
		if(srcSettings.buildLowerHalfOnly) 
		{
			iReal = (i > (srcSettings.approximation >> 1))?(NxReal)(i+srcSettings.approximation):(NxReal)i;
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


//	srcSettings.convexFlags |=NX_CF_COMPUTE_CONVEX;
//	convexDesc.flags				|= srcSettings.convexFlags;

	int cf = CF_ComputeConvex;
	cf |= srcSettings.convexFlags;
	
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

		dstShapeDescription->meshData = getPhysicSDK()->createConvexMesh(MemoryReadBuffer(buf.data));
		dstShapeDescription->localPose.t = center;
		dstShapeDescription->localPose.M.setColumn(0, NxVec3( 1, 0, 0));
		dstShapeDescription->localPose.M.setColumn(1, NxVec3( 0,-1, 0));
		dstShapeDescription->localPose.M.setColumn(2, NxVec3( 0, 0, -1));
		if(dstShapeDescription->meshData != NULL) 
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




NxShape * pFactory::_createConvexCylinder(NxActor *actor,int approximation,VxVector _forwardAxis,VxVector _downAxis,VxVector _rightAxis,float height,float radius,bool buildLowerHalf,int shapeFlags)
{

	if (!actor || approximation<1 )
		return NULL;

	NxConvexShape *result = NULL;
	NxArray<NxVec3> points;
	NxVec3 center(0,0,0);

	NxVec3 frontAxis =  getFrom(_forwardAxis);// = wheelDesc->downAxis.cross(wheelDesc->wheelAxis);
	NxVec3 downAxis = getFrom(_downAxis);//  = wheelDesc->downAxis;

	downAxis *=-1.0;

	NxVec3 wheelAxis  = getFrom(_rightAxis);// = wheelDesc->wheelAxis;
	//frontAxis.normalize();
	frontAxis *= radius;
	//downAxis.normalize();
	downAxis *= radius;
	//wheelAxis.normalize();
	wheelAxis *= height;

	NxReal step;

	if(buildLowerHalf) 
	{
		if((approximation & 0x1) == 0)
			approximation++;
		
		step = (NxReal)(NxTwoPi) / (NxReal)(approximation*2);
	}
	else 
	{
		step = (NxReal)(NxTwoPi) / (NxReal)(approximation);
	}
	for(NxU32 i = 0; i < approximation; i++) 
	{
		NxReal iReal;
		if(buildLowerHalf) 
		{
			iReal = (i > (approximation >> 1))?(NxReal)(i+approximation):(NxReal)i;
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
	convexDesc.flags				|= shapeFlags;
	//convexDesc.flags				|= NX_CF_COMPUTE_CONVEX;

	// Cooking from memory
	bool status = InitCooking();
	if (!status) {
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Couldn't initiate cooking lib!");
		return NULL;
	}
	MemoryWriteBuffer buf;
	if(CookConvexMesh(convexDesc, buf)) 
	{
		NxConvexShapeDesc convexShapeDesc;
		convexShapeDesc.meshData = actor->getScene().getPhysicsSDK().createConvexMesh(MemoryReadBuffer(buf.data));
		convexShapeDesc.localPose.t = center;
		convexShapeDesc.localPose.M.setColumn(0, NxVec3( 1, 0, 0));
		convexShapeDesc.localPose.M.setColumn(1, NxVec3( 0,-1, 0));
		convexShapeDesc.localPose.M.setColumn(2, NxVec3( 0, 0, -1));
		if(convexShapeDesc.meshData != NULL) 
		{
			NxU32 shapeNumber = actor->getNbShapes();
			result = actor->createShape(convexShapeDesc)->isConvexMesh();
			if (!result) {
				xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Couldn't create convex cylinder mesh");
			}
			//wheel->wheelConvex->userData = wheel;
		}
	}
	
	CloseCooking();

	return result;
	
}

NxShapeDesc& pFactory::createShape(int hullType,CK3dEntity*ent,float density)
{

	assert(ent);
	float radius = ent->GetRadius();
	if (ent->GetRadius() < 0.001f )
	{
		radius  = 1.0f;
	}

	VxVector box_s= BoxGetZero(ent);
	switch(hullType)
	{

	case HT_Box:
		{
			NxBoxShapeDesc result;
			//result.setToDefault();
			result.dimensions = pMath::getFrom(box_s);

			result.density  = density;
			return result;	
		}
	case HT_Sphere:
		{
			NxSphereShapeDesc result;
			//result.setToDefault();
			result.localPose.t  = NxVec3(0,radius,0);
			result.radius = radius;
			result.density  = density;
			return result;
		}
	}

	NxBoxShapeDesc result;
	result.setToDefault();
	result.dimensions = pMath::getFrom(box_s);
	return result;
}
