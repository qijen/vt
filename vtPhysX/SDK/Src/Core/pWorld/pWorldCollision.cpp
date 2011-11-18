#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "pWorldCallbacks.h"

bool pContactModify::onContactConstraint(NxU32& changeFlags, const NxShape* shape0, const NxShape* shape1, const NxU32 featureIndex0, const NxU32 featureIndex1, NxContactCallbackData& data)
{

	bool result = true;
	
	NxActor *actor0 = &shape0->getActor();
	NxActor *actor1 = &shape1->getActor();
	
	pRigidBody *body0  = NULL;
	pRigidBody *body1  = NULL;

	if (actor0)
	{
		body0  = static_cast<pRigidBody*>(actor0->userData);
		
	}
	if (actor1)
	{
		body1  = static_cast<pRigidBody*>(actor1->userData);
	}

	pContactModifyData cData;
	cData.dynamicFriction0 = data.dynamicFriction0;
	cData.dynamicFriction1 = data.dynamicFriction1;
	cData.staticFriction0 = data.staticFriction0;
	cData.staticFriction1 = data.staticFriction1;
	cData.localorientation0 = getFrom(data.localorientation0);
	cData.localorientation1 = getFrom(data.localorientation1);
	cData.localpos0 = getFrom(data.localpos0);
	cData.localpos1 = getFrom(data.localpos1);
	cData.restitution = data.restitution;
	cData.minImpulse  = data.minImpulse;
	cData.maxImpulse = data.maxImpulse;
	cData.target = getFrom(data.target);
	cData.error = getFrom(data.error);


	//----------------------------------------------------------------
	//
	// execute callback scripts
	//
	if (body0 )
	{
		if (body0->getFlags() & BF_ContactModify )
		{
			result  = body0->onContactConstraint((int&)changeFlags,body0->GetVT3DObject(),body1->GetVT3DObject(),&cData);
			if (!result)
				return false;
			
			if (changeFlags == CMM_None )
			{
				return true;
			}
		}
	}
	if (body1)
	{
		if (body1->getFlags() & BF_ContactModify)
		{
			result  = body1->onContactConstraint((int&)changeFlags,body1->GetVT3DObject(),body0->GetVT3DObject(),&cData);
			if (!result)
				return false;

			if (changeFlags == CMM_None )
			{
				return true;
			}
		}
	}

	//----------------------------------------------------------------
	//
	// copy result back to sdk
	//
	if (changeFlags!=CMM_None)
	{
		data.dynamicFriction0 = cData.dynamicFriction0;
		data.dynamicFriction1 = cData.dynamicFriction1;
		data.staticFriction0 = cData.staticFriction0;
		data.staticFriction1 = cData.staticFriction1;
		data.localorientation0 = getFrom(cData.localorientation0);
		data.localorientation1 = getFrom(cData.localorientation1);
		data.localpos0 = getFrom(cData.localpos0);
		data.localpos1 = getFrom(cData.localpos1);
		data.restitution = cData.restitution;
		data.minImpulse  = cData.minImpulse;
		data.maxImpulse = cData.maxImpulse;
		data.target = getFrom(cData.target);
		data.error = getFrom(cData.error);
	}
	return true;
}

void pContactReport::onContactNotify(NxContactPair& pair, NxU32 events)
{

	pRigidBody *bodyA = NULL;
	pRigidBody *bodyB  = NULL;

	if (pair.actors[0])
	{
		bodyA  = static_cast<pRigidBody*>(pair.actors[0]->userData);
	}
	if (pair.actors[1])
	{
		bodyB  = static_cast<pRigidBody*>(pair.actors[1]->userData);
	}

	if (bodyA) 
	{
		/*
		if (bodyA->hasWheels())
		{
			bodyA->handleContactPair(&pair,0);
			return ;
		}
		*/

		/*
		if (bodyA->getVehicle())
		{
			pVehicle* v = bodyA->getVehicle();
			v->handleContactPair(&pair, 0);
			return ;
		}
		*/
	}
	if ( bodyB ) 
	{
		/*
		if (bodyB->hasWheels())
		{
			bodyB->handleContactPair(&pair,1);
			return ;
		}

		if (bodyB->getVehicle())
		{
			pVehicle* v = bodyB->getVehicle();
			v->handleContactPair(&pair, 1);
			return ;
		}
		*/

		
	}

	// Iterate through contact points
	NxContactStreamIterator i(pair.stream);
	//user can call getNumPairs() here
	while(i.goNextPair())
	{
		//user can also call getShape() and getNumPatches() here
		while(i.goNextPatch())
		{
			//user can also call getPatchNormal() and getNumPoints() here
			const NxVec3& contactNormal = i.getPatchNormal();
			while(i.goNextPoint())
			{
				//user can also call getPoint() and getSeparation() here
				const NxVec3& contactPoint = i.getPoint();
				
				pCollisionsEntry entry;
				entry.point = pMath::getFrom(contactPoint);
				NxU32 faceIndex = i.getFeatureIndex0();
				if(faceIndex==0xffffffff)faceIndex = i.getFeatureIndex1();
				if(faceIndex!=0xffffffff)entry.faceIndex = faceIndex;

				entry.actors[0] = pair.actors[0];
				entry.actors[1] = pair.actors[1];
				entry.faceIndex = faceIndex;
				entry.sumNormalForce = pMath::getFrom(pair.sumNormalForce);
				entry.sumFrictionForce = pMath::getFrom(pair.sumFrictionForce);
				entry.faceNormal = pMath::getFrom(contactNormal);
				entry.eventType = events;
				entry.pointNormalForce=i.getPointNormalForce();
				entry.distance = i.getSeparation();


				////////////////////////////////////////////////////////////////////////////
				//store shapes


				NxShape *shapeA = i.getShape(0);
				if (shapeA)
				{
					pSubMeshInfo *shapeInfo=  static_cast<pSubMeshInfo*>(shapeA->userData);
					if (shapeInfo)
					{
						entry.shapeEntityA = shapeInfo->entID;
					}
				}

				shapeA = i.getShape(1);
				if (shapeA)
				{
					pSubMeshInfo *shapeInfo=  static_cast<pSubMeshInfo*>(shapeA->userData);
					if (shapeInfo)
					{
						entry.shapeEntityB = shapeInfo->entID;
					}
				}

				
				pRigidBody *bodyA = NULL;
				pRigidBody *bodyB  = NULL;
				if (pair.actors[0])
				{
					bodyA  = static_cast<pRigidBody*>(pair.actors[0]->userData);
				}
				if (pair.actors[1])
				{
					bodyB  = static_cast<pRigidBody*>(pair.actors[1]->userData);
				}

				
				if (bodyA && bodyA->getActor() /*&& (bodyA->getActor()->getContactReportFlags() & NX_NOTIFY_ON_TOUCH ) */)
				{
					entry.bodyA = bodyA;
					entry.bodyB = bodyB;
					bodyA->getCollisions().Clear();
					bodyA->getCollisions().PushBack(entry);
					bodyA->onContactNotify(&entry);
				}

				if (bodyB && bodyB->getActor()/* && (bodyB->getActor()->getContactReportFlags() & NX_NOTIFY_ON_TOUCH ) */)
				{
					entry.bodyA = bodyA;
					entry.bodyB = bodyB;
					bodyB->getCollisions().Clear();
					bodyB->getCollisions().PushBack(entry);
					bodyB->onContactNotify(&entry);
				}

				//				result->getActor()->setContactReportFlags(NX_NOTIFY_ON_TOUCH);
				/*
				{
				entry.bodyA = body;
				body->getCollisions().Clear();
				body->getCollisions().PushBack(entry);
				}

				if (pair.actors[1])
				{
				pRigidBody *body = static_cast<pRigidBody*>(pair.actors[1]->userData);
				if (body)
				{
				entry.bodyB = body;
				body->getCollisions().Clear();
				body->getCollisions().PushBack(entry);

				}
				}*/
				/*getWorld()->getCollisions().PushBack(entry);*/
			}
		}
	}
}

int pWorld::overlapOBBShapes(const VxBbox& worldBounds, CK3dEntity*shapeReference, pShapesType shapeType,CKGroup *shapes,int activeGroups/* =0xffffffff */, const pGroupsMask* groupsMask/* =NULL */, bool accurateCollision/* =false */)
{

	int result=0;

	NxBox box;

	if (shapeReference)
	{

		NxShape *shape  = getShapeByEntityID(shapeReference->GetID());
		if (shape)
		{
			//shape->checkOverlapAABB()

			NxBoxShape*boxShape  = static_cast<NxBoxShape*>(shape->isBox());
			if (boxShape)
			{
				boxShape->getWorldOBB(box);
			}
		}

	}else{

		box.center = getFrom(worldBounds.GetCenter());
		box.extents = getFrom(worldBounds.GetSize());
	}

	int total = 0;
	if (shapeType & ST_Dynamic )
	{
		total+=getScene()->getNbDynamicShapes();
	}

	if (shapeType & ST_Static)
	{
		total+=getScene()->getNbStaticShapes();
	}

	NxShape** _shapes = (NxShape**)NxAlloca(total*sizeof(NxShape*));
	for (NxU32 i = 0; i < total; i++)  _shapes[i] = NULL;



	NxGroupsMask mask;
	if (groupsMask)
	{
		mask.bits0 = groupsMask->bits0;
		mask.bits1 = groupsMask->bits1;
		mask.bits2 = groupsMask->bits2;
		mask.bits3 = groupsMask->bits3;
	}else{

		mask.bits0 = 0;
		mask.bits1 = 0;
		mask.bits2 = 0;
		mask.bits3 = 0;

	}
	result = getScene()->overlapOBBShapes(box,(NxShapesType)shapeType,total,_shapes,NULL,activeGroups,&mask,accurateCollision);

	if (_shapes && shapes )
	{
		for (int i  = 0 ; i < result ; i++)
		{
			NxShape *s = _shapes[i];
			if (s)
			{
				const char* name =s->getName();
				pSubMeshInfo *sInfo  = static_cast<pSubMeshInfo*>(s->userData);
				if (sInfo->entID)
				{
					CKObject *obj = (CKObject*)GetPMan()->m_Context->GetObject(sInfo->entID);
					if (obj)
					{

						shapes->AddObject((CKBeObject*)obj);
					}
				}
			}
		}
	}

	int op=2;




	return result;

}

int pWorld::overlapSphereShapes(const VxSphere& worldSphere,CK3dEntity*shapeReference,pShapesType shapeType,CKGroup*shapes,int activeGroups/* =0xffffffff */, const pGroupsMask* groupsMask/* =NULL */, bool accurateCollision/* =false */)
{

	int result=0;

	NxSphere sphere;
	
	if (shapeReference)
	{
		
			NxShape *shape  = getShapeByEntityID(shapeReference->GetID());
			if (shape)
			{
				//shape->checkOverlapAABB()

				NxSphereShape *sphereShape  = static_cast<NxSphereShape*>(shape->isSphere());
				if (sphereShape)
				{
					sphere.radius = sphereShape->getRadius() + worldSphere.Radius();

					//ori : 
					VxVector ori  = worldSphere.Center();
					VxVector oriOut  = ori;
					if (shapeReference)
					{
						shapeReference->Transform(&oriOut,&ori);
					}

					sphere.center = getFrom(oriOut);

				}
			}

	}else{

		sphere.center = getFrom(worldSphere.Center());
		sphere.radius = worldSphere.Radius();

	}

	int total = 0;
	if (shapeType & ST_Dynamic )
	{
		total+=getScene()->getNbDynamicShapes();
	}
	
	if (shapeType & ST_Static)
	{
		total+=getScene()->getNbStaticShapes();
	}

	NxShape** _shapes = (NxShape**)NxAlloca(total*sizeof(NxShape*));
	for (NxU32 i = 0; i < total; i++)  _shapes[i] = NULL;



	NxGroupsMask mask;
	if (groupsMask)
	{
		mask.bits0 = groupsMask->bits0;
		mask.bits1 = groupsMask->bits1;
		mask.bits2 = groupsMask->bits2;
		mask.bits3 = groupsMask->bits3;
	}else{

		mask.bits0 = 0;
		mask.bits1 = 0;
		mask.bits2 = 0;
		mask.bits3 = 0;

	}
	



	result = getScene()->overlapSphereShapes(sphere,(NxShapesType)shapeType,total,_shapes,NULL,activeGroups,&mask,accurateCollision);

	if (_shapes && shapes )
	{
		for (int i  = 0 ; i < result ; i++)
		{
			NxShape *s = _shapes[i];
			if (s)
			{
				const char* name =s->getName();
				pSubMeshInfo *sInfo  = static_cast<pSubMeshInfo*>(s->userData);
				if (sInfo->entID)
				{
					CKObject *obj = (CKObject*)GetPMan()->m_Context->GetObject(sInfo->entID);
					if (obj)
					{
						
						shapes->AddObject((CKBeObject*)obj);
					}
				}
			}
		}
	}

	int op=2;




	return result;

}

int pWorld::raycastAllShapes(const VxRay& worldRay, pShapesType shapesType, int groups, float maxDist, pRaycastBit hintFlags, const pGroupsMask* groupsMask)
{

	int result = 0;

	NxRay rayx;
	rayx.dir = getFrom(worldRay.m_Direction);
	rayx.orig = getFrom(worldRay.m_Origin);

	pRayCastReport &report = *getRaycastReport();

	NxGroupsMask *mask  = NULL;
	if (groupsMask)
	{
		mask = new NxGroupsMask();
		mask->bits0 = groupsMask->bits0;
		mask->bits1 = groupsMask->bits1;
		mask->bits2 = groupsMask->bits2;
		mask->bits3 = groupsMask->bits3;

	}
	
	result  = getScene()->raycastAllShapes(rayx,report,(NxShapesType)shapesType,groups,maxDist,hintFlags,mask);
	return result;


}
bool pWorld::raycastAnyBounds(const VxRay& worldRay, pShapesType shapesType, pGroupsMask *groupsMask/* =NULL */,int groups/* =0xffffffff */, float maxDist/* =NX_MAX_F32 */)
{

	if (!getScene())
	{
		return false;
	}

	NxRay _worldRay;
	_worldRay.dir = getFrom(worldRay.m_Direction);
	_worldRay.orig = getFrom(worldRay.m_Origin);
	
	NxShapesType _shapesType = (NxShapesType)shapesType;
	
	NxReal _maxDist=maxDist;
	
	NxGroupsMask *mask  = NULL;
	if (groupsMask)
	{
		mask = new NxGroupsMask();
		mask->bits0 = groupsMask->bits0;
		mask->bits1 = groupsMask->bits1;
		mask->bits2 = groupsMask->bits2;
		mask->bits3 = groupsMask->bits3;

	}
	bool result  = getScene()->raycastAnyBounds(_worldRay,_shapesType,groups,_maxDist,mask);
	return result;

}

void pWorld::cIgnorePair(CK3dEntity *_a, CK3dEntity *_b, int ignore)
{

	pRigidBody *a  = GetPMan()->getBody(_a);
	pRigidBody *b  = GetPMan()->getBody(_b);
	if (a&&b && a->getActor() && b->getActor() )
	{
		if (getScene())
		{
			getScene()->setActorPairFlags(*a->getActor(),*b->getActor(),ignore ? NX_IGNORE_PAIR : NX_NOTIFY_ALL );
		}
	}
}

void pWorld::cSetGroupCollisionFlag(int g1 , int g2,int enabled)
{

	if (getScene())
	{
		if (g1 >=0 && g1 <= 31 && g2 >=0 && g2 <= 31)
		{
			if (enabled==0 || enabled ==1)
			{
				getScene()->setGroupCollisionFlag(g1,g2,enabled);
			}
		}
	}
}

void pTriggerReport::onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status)
{

	NxActor *triggerActor = &triggerShape.getActor();
	NxActor *otherActor = &otherShape.getActor();
	pRigidBody *triggerBody  = NULL;
	pRigidBody *otherBody  = NULL;

	if (triggerActor)
	{
		triggerBody  = static_cast<pRigidBody*>(triggerActor->userData);
		//triggerBody->getTriggers().Clear();
	}

	if (otherActor)
	{
		otherBody  = static_cast<pRigidBody*>(otherActor->userData);
		//otherBody->getTriggers().Clear();
	}
	

	pTriggerEntry entry;
	entry.shapeA  = &triggerShape;
	entry.shapeB = &otherShape;
	entry.triggerEvent = status;

	entry.triggerBodyB = triggerBody;
	entry.otherBodyB = otherBody;


	entry.triggerBody = triggerBody->GetVT3DObject();
	pSubMeshInfo *sInfo = (pSubMeshInfo*)otherShape.userData;
	if (sInfo)
	{
		entry.otherObject = (CK3dEntity*)GetPMan()->GetContext()->GetObject(sInfo->entID);
	}

	pSubMeshInfo *sInfoOri = (pSubMeshInfo*)triggerShape.userData;
	if (sInfo)
	{
		entry.triggerShapeEnt = (CK3dEntity*)GetPMan()->GetContext()->GetObject(sInfoOri->entID);
	}

	if (triggerBody)
	{
		//triggerBody->getTriggers().PushBack(entry);
        triggerBody->onTrigger(&entry);
		GetPMan()->getTriggers().PushBack(entry);
	}
}


void pWorld::initUserReports()
{
	
	contactReport = new pContactReport();
	contactReport->setWorld(this);

	triggerReport  = new pTriggerReport();
	triggerReport->setWorld(this);

	raycastReport = new pRayCastReport();
	raycastReport->setWorld(this);

	contactModify = new pContactModify();
	contactModify->setWorld(this);


	//getScene()->raycastAllShapes()
}
bool pRayCastReport::onHit(const NxRaycastHit& hit)
{

	CKBehavior *beh =(CKBehavior*)GetPMan()->m_Context->GetObject(mCurrentBehavior);
	if ( beh )
	{
		pRayCastHits *carray = NULL;
		beh->GetLocalParameterValue(0,&carray);
		if (carray)
		{
			//carray->clear();
		}else
		{
			carray =  new pRayCastHits();
		}

		//carray->push_back(const_cast<NxRaycastHit&>(&hit));

		NxRaycastHit *_hit = new NxRaycastHit();

		_hit->distance = hit.distance;
		_hit->faceID = hit.faceID;
		_hit->flags = hit.flags;
		_hit->internalFaceID = hit.internalFaceID;
		_hit->materialIndex = hit.materialIndex;
		_hit->shape = hit.shape;
		_hit->u =  hit.u;
		_hit->v = hit.v;
		_hit->worldImpact = hit.worldImpact;
		_hit->worldNormal = hit.worldNormal;
		
		const char *name = hit.shape->getName();
		
		carray->push_back(_hit);
		beh->SetLocalParameterValue(0,&carray);


	}
	return true;
}

void pWorld::setFilterOps(pFilterOp op0,pFilterOp op1, pFilterOp op2)
{
	getScene()->setFilterOps((NxFilterOp)op0,(NxFilterOp)op1,(NxFilterOp)op2);
}

void pWorld::setFilterBool(bool flag){	getScene()->setFilterBool(flag);}
void pWorld::setFilterConstant0(const pGroupsMask& mask)
{
	NxGroupsMask _mask;
	_mask.bits0=mask.bits0;
	_mask.bits1=mask.bits1;
	_mask.bits2=mask.bits2;
	_mask.bits3=mask.bits3;
	
	getScene()->setFilterConstant0(_mask);
}

void pWorld::setFilterConstant1(const pGroupsMask& mask)
{
	NxGroupsMask _mask;
	_mask.bits0=mask.bits0;
	_mask.bits1=mask.bits1;
	_mask.bits2=mask.bits2;
	_mask.bits3=mask.bits3;

	getScene()->setFilterConstant1(_mask);
}

bool pWorld::getFilterBool()const{	return getScene()->getFilterBool();}
pGroupsMask pWorld::getFilterConstant0()const
{
	NxGroupsMask mask = getScene()->getFilterConstant0();
	pGroupsMask _mask;
	
	_mask.bits0=mask.bits0;
	_mask.bits1=mask.bits1;
	_mask.bits2=mask.bits2;
	_mask.bits3=mask.bits3;

	return _mask;

}

pGroupsMask pWorld::getFilterConstant1()const
{
	NxGroupsMask mask = getScene()->getFilterConstant1();
	pGroupsMask _mask;
	_mask.bits0=mask.bits0;
	_mask.bits1=mask.bits1;
	_mask.bits2=mask.bits2;
	_mask.bits3=mask.bits3;
	return _mask;
}



