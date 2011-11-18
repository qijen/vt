#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "Stream.h"
#include "cooking.h"
#include "tinyxml.h"

#include <xDebugTools.h>

NxShape*pFactory::createWheelShape1(CK3dEntity *bodyReference,CK3dEntity*wheelReference,pWheelDescr wheelDescr)
{

	NxWheelShape *result = NULL;
	pRigidBody *body=GetPMan()->getBody(bodyReference);
	

	bool assertResult = true;
	iAssertWR(bodyReference && wheelReference && wheelDescr.isValid() && body && body->getActor(),"",assertResult);
	if (!assertResult)
		return NULL;

	NxActor *actor = body->getActor();


	//----------------------------------------------------------------
	//
	// prepare data : 
	//
	CKMesh *mesh = wheelReference->GetCurrentMesh();
	VxVector box_s = mesh->GetLocalBox().GetSize();
	
	float radius = wheelDescr.radius.value > 0.0f ? wheelDescr.radius.value : box_s.v[wheelDescr.radius.referenceAxis] * 0.5f;

	VxQuaternion quatOffset;
	VxVector posOffset;
	vtAgeia::calculateOffsets(bodyReference,wheelReference,quatOffset,posOffset);
	CK_ID srcID = mesh->GetID();
	
	//----------------------------------------------------------------
	//
	// create convex cylinder : 
	//
	NxConvexShapeDesc shape;
	bool resultAssert = true;
	iAssertW(wheelDescr.convexCylinder.isValid(),wheelDescr.convexCylinder.setToDefault(),"");
	iAssertW( pFactory::Instance()->_createConvexCylinderMesh(&shape,wheelDescr.convexCylinder,wheelReference),"");

	shape.localPose.M = pMath::getFrom(quatOffset);
	shape.localPose.t = pMath::getFrom(posOffset);
	
	actor->createShape(shape)->isConvexMesh();

/*	NxConvexShapeDesc shape;
	if (!_createConvexCylinder(&shape,wheelReference))
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,"Couldn't create convex cylinder mesh");

*/



	return NULL;
}