#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "pWorldCallbacks.h"

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
		if (bodyA->hasWheels())
		{
			bodyA->handleContactPair(&pair,0);
			return ;
		}

		if (bodyA->getVehicle())
		{
			pVehicle* v = bodyA->getVehicle();
			v->handleContactPair(&pair, 0);
			return ;
		}
	}
	if ( bodyB ) 
	{
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
				if(faceIndex==0xffffffff)	
					faceIndex = i.getFeatureIndex1();
				if(faceIndex!=0xffffffff)
				{
					entry.faceIndex = faceIndex;
				}

				entry.actors[0] = pair.actors[0];
				entry.actors[1] = pair.actors[1];
				entry.faceIndex = faceIndex;
				entry.sumNormalForce = pMath::getFrom(pair.sumNormalForce);
				entry.sumFrictionForce = pMath::getFrom(pair.sumFrictionForce);
				entry.faceNormal = pMath::getFrom(contactNormal);
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

				if (bodyA && bodyA->getActor() && (bodyA->getActor()->getContactReportFlags() & NX_NOTIFY_ON_TOUCH ) )
				{

					entry.bodyA = bodyA;
					entry.bodyB = bodyB;
					bodyA->getCollisions().Clear();
					bodyA->getCollisions().PushBack(entry);
				}

				if (bodyB && bodyB->getActor() && (bodyB->getActor()->getContactReportFlags() & NX_NOTIFY_ON_TOUCH ) )
				{

					entry.bodyA = bodyA;
					entry.bodyB = bodyB;
					bodyB->getCollisions().Clear();
					bodyB->getCollisions().PushBack(entry);
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
		triggerBody->getTriggers().Clear();
	}

	if (otherActor)
	{
		otherBody  = static_cast<pRigidBody*>(otherActor->userData);
		otherBody->getTriggers().Clear();
	}
	

	pTriggerEntry entry;
	entry.shapeA  = &triggerShape;
	entry.shapeB = &otherShape;
	entry.triggerEvent = status;

	if (triggerBody)
	{
		triggerBody->getTriggers().PushBack(entry);

	}
	/*if(status & NX_TRIGGER_ON_ENTER)
	{


	}
	if(status & NX_TRIGGER_ON_LEAVE)
	{
				
	}
	if(status & NX_TRIGGER_ON_STAY)
	{
		// A body entered the trigger area for the first time

	}*/

	
	

}


void pWorld::initUserReports()
{
	
	contactReport = new pContactReport();
	contactReport->setWorld(this);

	triggerReport  = new pTriggerReport();
	triggerReport->setWorld(this);

	raycastReport = new pRayCastReport();
	raycastReport->setWorld(this);

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

void pWorld::setFilterBool(bool flag)
{
	getScene()->setFilterBool(flag);
}
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

bool pWorld::getFilterBool()const
{
	return getScene()->getFilterBool();
}

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



/*
int
pWorld::CIsInCollision(CK3dEntity*_a,CK3dEntity*_b)
{

	pWorld *world=GetPMan()->getWorldByBody(_a); 
	pWorld *world2=GetPMan()->getWorldByBody(_b); 


	if (!world || !world2 || world!=world2 )
		return false;

	pRigidBody*bodyA= world->getBody(_a);
	pRigidBody*bodyB= world->getBody(_b);

	dSurfaceParameters *surface = NULL;
	bool worldHasSurface = ( world->GetDataFlags() & WDF_HAS_SURFACE_PARAMETER ) ;


	bool useSurface  = false;

	if (worldHasSurface)
	{
		surface = &GetDefaultSurface();
		useSurface = true;
	}

	if ( bodyA->GetDataFlags() & BDF_HAS_SURFACE_PARAMETER )
	{
		surface  = &bodyA->GetDefaultSurface();
		useSurface = true;
	}

	if ( bodyB->GetDataFlags() & BDF_HAS_SURFACE_PARAMETER )
	{
		surface  = &bodyB->GetDefaultSurface();
		useSurface = true;
	}


	pRigidBody *surfaceMaterialBody=NULL;

	if (bodyA->GetDataFlags() & BDF_HAS_MATERIAL_SURFACE_PARAMETER)
	{

		surfaceMaterialBody = bodyA;
		useSurface = true;
	}

	if (bodyB->GetDataFlags() & BDF_HAS_MATERIAL_SURFACE_PARAMETER)
	{

		surfaceMaterialBody = bodyB;
		useSurface = true;
	}

	if (bodyA && bodyB && bodyA !=bodyB)
	{
		dGeomID gid1  = bodyA->GetOdeGeom();
		dGeomID gid2  = bodyB->GetOdeGeom();
		if (gid1 && gid2 && gid1!=gid2)
		{
			dContact *contact = new dContact[1];
			int nbContacts  = dCollide (gid1,gid2,1,&contact[0].geom,sizeof(dContactGeom));
			if (nbContacts)
			{
				for (int i=0; i < nbContacts; i++)
				{
					if (surfaceMaterialBody)
					{

						CK3dEntity *colliderB = NULL;
						if (surfaceMaterialBody->GetVT3DObject() == _a )
						{
							colliderB = _b;
						}else
							colliderB = _a;

						if (surfaceMaterialBody->GetVT3DObject()  == colliderB )
						{
							int op  = 3;
						}

						VxVector cPos(contact[i].geom.pos[0],contact[i].geom.pos[1],contact[i].geom.pos[2] );

						int faceIndex = vtAgeia::math::GetNearestFace(surfaceMaterialBody->GetVT3DObject(),cPos);
				

						CKMesh * mesh  =  surfaceMaterialBody->GetVT3DObject()->GetCurrentMesh();
						if (mesh)
						{

			
							VxVector pos1,pos2;
							surfaceMaterialBody->GetVT3DObject()->GetBaryCenter(&pos2);
							surfaceMaterialBody->GetVT3DObject()->Transform(&pos2,&pos2);
							
							VxIntersectionDesc desc;
							surfaceMaterialBody->GetVT3DObject()->RayIntersection(&cPos,&pos2,&desc,NULL);
                            CKMaterial *material =  mesh->GetFaceMaterial(desc.FaceIndex);
							if (material)
							{

								PhysicMaterialList& mList = surfaceMaterialBody->GetPhysicMaterials();
								int mSize = mList.Size();
								PhysicMaterialList::Iterator it  = mList.Find(material->GetID() );
								if( it != mList.End() )
								{
									dSurfaceParameters *sPar = *it;
									if (sPar)
									{
										surface = sPar;

									}
								}
							}
						}
					}
				}
			}
		}
	}

	return 0;
}
void collisionCallback(void *data,dGeomID o1,dGeomID o2)
{

	pWorld *world  = static_cast<pWorld*>(data);
	if(!world)
		return;


	if( !o1 || !o2)
		return;
	if (!o1->body && !o2->body) return;

	if( o1 == o2 )
		return;

	if (dGeomIsSpace(o1) || dGeomIsSpace(o2))
	{
		// colliding a space with something
		dSpaceCollide2(o1,o2,data,&collisionCallback);
		// Note we do not want to test intersections within a space,
		// only between spaces.
		return;
	}

	dBodyID b1,b2;

	b1 = dGeomgetBody(o1);
	b2 = dGeomgetBody(o2);

	CK_ID e1 = (CK_ID)(dGeomGetData(o1));
	CK_ID e2 = (CK_ID)(dGeomGetData(o2));


	CK3dEntity *_e1 = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(e1));
	CK3dEntity* _e2 = static_cast<CK3dEntity*>(GetPMan()->m_Context->GetObject(e2));

	// exit without doing anything if the two bodies are connected by a joint
	if (b1 && b2 && dAreConnected (b1,b2)) return;

	pRigidBody *solid1 = world->getBody(_e1);
	pRigidBody *solid2 = world->getBody(_e2);

	if (!solid1 || !solid2)
	{
		return;
	}
	float friction=solid1->GetFriction();
	float restitution = solid1->GetRestitution();
	//for friction, take minimum

	friction=(friction < solid2->GetFriction() ?  	friction :solid2->GetFriction());

	//restitution:take minimum
	restitution = restitution < solid2->GetRestitution()	?	restitution : solid2->GetRestitution();

	

	dSurfaceParameters *surface = NULL;
	bool worldHasSurface = ( world->GetDataFlags() & WDF_HAS_SURFACE_PARAMETER ) ;
	

	bool useSurface  = false;

	if (worldHasSurface)
	{
		surface = &world->GetDefaultSurface();
		useSurface = true;
	}

	if ( solid1->GetDataFlags() & BDF_HAS_SURFACE_PARAMETER )
	{
		surface  = &solid1->GetDefaultSurface();
		useSurface = true;
	}

	if ( solid2->GetDataFlags() & BDF_HAS_SURFACE_PARAMETER )
	{
		surface  = &solid2->GetDefaultSurface();
		useSurface = true;
	}


	pRigidBody *surfaceMaterialBody=NULL;
	
	if (solid1->GetDataFlags() & BDF_HAS_MATERIAL_SURFACE_PARAMETER)
	{
    
		surfaceMaterialBody = solid1;
		useSurface = true;
	}

	if (solid2->GetDataFlags() & BDF_HAS_MATERIAL_SURFACE_PARAMETER)
	{

		surfaceMaterialBody = solid2;
		useSurface = true;
	}


	dContact contact[10];
	
	if (int numc = dCollide (o1, o2, 10,&contact[0].geom, sizeof(dContact)))
	{
		for (int i=0; i < numc; i++)
		{

			if (contact[i].geom.depth > 0.0f && contact[i].geom.depth <= 0.0001f)
			{
				contact[i].geom.depth = 0.001f;
			}
			if (contact[i].geom.depth == 0.0f)
			{
				continue;
			}

			if (surfaceMaterialBody)
			{

				
				CK3dEntity *colliderB = NULL;
				if (surfaceMaterialBody->GetVT3DObject() == _e1 )
				{
					colliderB = _e2;
				}else
					colliderB = _e1;
				
				VxVector cPos(contact[i].geom.pos[0],contact[i].geom.pos[1],contact[i].geom.pos[2] );
				//int faceIndex = vtODE::math::GetNearestFace(surfaceMaterialBody->GetVT3DObject(),cPos);
				VxVector pos1,pos2;
				surfaceMaterialBody->GetVT3DObject()->GetBaryCenter(&pos2);
				surfaceMaterialBody->GetVT3DObject()->Transform(&pos2,&pos2);

				VxIntersectionDesc desc;
				surfaceMaterialBody->GetVT3DObject()->RayIntersection(&cPos,&pos2,&desc,NULL);
				int faceIndex  = desc.FaceIndex;
				CKMesh * mesh  =  surfaceMaterialBody->GetVT3DObject()->GetCurrentMesh();
				if (mesh)
				{
				
					CKMaterial *material =  mesh->GetFaceMaterial(faceIndex);
					if (material)
					{
                        
						PhysicMaterialList& mList = surfaceMaterialBody->GetPhysicMaterials();
						int mSize = mList.Size();
						PhysicMaterialList::Iterator it  = mList.Find(material->GetID() );
						if( it != mList.End() )
						{
							dSurfaceParameters *sPar = *it;
							if (sPar)
							{
								surface = sPar;
								
							}
						}
					}
				}
			}

			if ( useSurface)
			{
				
			
				contact[i].surface.mode = surface->mode; 
				contact[i].surface.mu =  surface->mu;
				contact[i].surface.mu2 =  friction;
				contact[i].surface.slip1 = surface->slip1;
				contact[i].surface.slip2 = surface->slip2;
				contact[i].surface.motion1 = surface->motion1;
				contact[i].surface.motion2 = surface->motion2;
				contact[i].surface.motionN = surface->motionN;
				contact[i].surface.soft_erp = surface->soft_erp;
				contact[i].surface.soft_cfm = surface->soft_cfm;
				contact[i].surface.bounce = restitution;//0.5;
				contact[i].surface.bounce_vel = surface->bounce_vel;
			}else
			{

				//contact[i].surface.mode = dContactSlip1 | dContactSlip2 |dContactBounce|dContactSoftERP | dContactSoftCFM | dContactApprox1;
				//contact[i].surface.mode = dContactSoftERP | dContactApprox1| dContactApprox0 | dContactSoftCFM|dContactBounce ;
				//contact[i].surface.mode = dContactSlip1 | dContactSlip2 | dContactSoftERP | dContactApprox1  |dContactBounce;
				contact[i].surface.mode = dContactSoftERP | dContactSoftCFM | dContactSlip1 | dContactSlip2;
				//contact[i].surface.mode = dContactSoftERP | dContactSoftCFM | dContactBounce| dContactApprox0| dContactSlip1 | dContactSlip2;
				//contact[i].surface.mode = dContactSlip1 | dContactSlip2 | dContactSoftERP | dContactApprox1 ;
				contact[i].surface.mu =  dInfinity;
				contact[i].surface.mu2 =  friction;
				contact[i].surface.slip1 = 0.000001;
				contact[i].surface.slip2 = 0.000001;
				contact[i].surface.soft_erp = 0.4;
				contact[i].surface.soft_cfm = 0.000000005;
				contact[i].surface.bounce = restitution;//0.5;
				contact[i].surface.bounce_vel = 0.0000001f;
			}

			dJointID c = dJointCreateContact (world->World(),world->ContactGroup(),contact+i);
			dJointAttach (c,dGeomgetBody(o1),dGeomgetBody(o2));
		}
	}
}
//////////////////////////////////////////////////////////////////////////
bool pWorld::collide()
{

	SpaceID()->collide(this,&collisionCallback);

	return true;
}
//////////////////////////////////////////////////////////////////////////
vt3DObjectType pWorld::CIsInCollision(vt3DObjectType a, CKGroup *group,VxVector& pos, VxVector& normal,float &length)
{

	pRigidBody *bodyA = getBody(a);

	dGeomID gid1  = NULL;
	if (bodyA)
	{
		gid1 = bodyA->GetOdeGeom();
	}

	if (!a || !group || !bodyA || !gid1)
	{
		return NULL;
	}

	for (int i  = 0 ; i < group->GetObjectCount() ; i ++)
	{

		pRigidBody*bodyB= getBody((CK3dEntity*)group->GetObject(i));
		if (bodyB)
		{

			if (bodyB==bodyA)
			{
				continue;
			}

			if (bodyB)
			{
				dGeomID gid2 = bodyB->GetOdeGeom();
				if (gid1==gid2)
				{
					continue;
				}

				if (gid2)
				{
					dContact *contact = new dContact[1];

					int nbContacts  = dCollide (gid1,gid2,1,&contact[0].geom,sizeof(dContactGeom));
					if (nbContacts)
					{
						for (int i = 0 ; i < nbContacts ; i++ )
						{
							pos.x = contact[0].geom.pos[0];
							pos.y  = contact[0].geom.pos[1];
							pos.z  = contact[0].geom.pos[2];

							normal.x =contact[0].geom.normal[0];
							normal.y =contact[0].geom.normal[1];
							normal.z = contact[0].geom.normal[2];

							return bodyB->GetVT3DObject();
						}
					}
				}
			}

		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
vt3DObjectType pWorld::CRayCollision(VxVector point,CK3dEntity*pointRef,VxVector dir,CK3dEntity*dirRef,float depth,bool skipPosRef,VxVector& pos, VxVector& normal)
{

	//////////////////////////////////////////////////////////////////////////
	//origin of the ray : 
	VxVector origin = point;
	VxVector originOut  = origin;
	if (pointRef)
	{
		pointRef->Transform(&originOut,&origin);
	}

	//////////////////////////////////////////////////////////////////////////
	//direction of the ray : 
	VxVector dirIn = dir;
	VxVector dirOut  = dirIn;
	if (dirRef)
	{
		VxVector dirl,up,right;
		dirRef->GetOrientation(&dirl,&up,&right);
		dirRef->TransformVector(&dirOut,&dirIn);
	}

	//dirOut*=dirIn;

	//////////////////////////////////////////////////////////////////////////
	pRigidBody *bodyA  = NULL;
	dGeomID gid1  = NULL;
	//if position reference is given : 
	if (pointRef)
	{
		bodyA  = getBody(pointRef);
		if (bodyA)
		{
			gid1 =  bodyA->GetOdeGeom();
		}
	}


	//////////////////////////////////////////////////////////////////////////
	//our ray : 
	dGeomID raygeom = dCreateRay(SpaceID(),depth);
	if (raygeom)
	{
		dGeomRaySet(raygeom,originOut.x,originOut.y,originOut.z,dirOut.x,dirOut.y,dirOut.z);
	}

	if (!raygeom)
	{
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	//we check through the world  :

	dxBody *bb;
	for (bb=World()->firstbody; bb; bb=(dxBody*)bb->next) 
	{
		pRigidBody *bodyB  = static_cast<pRigidBody*>(dBodyGetData(bb));
		if (bodyB)
		{
			dGeomID gid2 = bodyB->GetOdeGeom();
			CK3dEntity *ent2 = bodyB->GetVT3DObject();

			if (gid2)
			{
				dContact *contact = new dContact[1];

				int nbContacts  = dCollide (raygeom,gid2,1,&contact[0].geom,sizeof(dContactGeom));
				if (nbContacts)
				{
					if (gid1 &&  gid2 == gid1 && skipPosRef )
					{
						continue;
					}

					pos.x = contact[0].geom.pos[0];
					pos.y  = contact[0].geom.pos[1];
					pos.z  = contact[0].geom.pos[2];

					normal.x =contact[0].geom.normal[0];
					normal.y =contact[0].geom.normal[1];
					normal.z = contact[0].geom.normal[2];

					return bodyB->GetVT3DObject();

				}
			}
		}
	}

	return NULL;

}

//////////////////////////////////////////////////////////////////////////
vt3DObjectType pWorld::CIsInCollision(vt3DObjectType a, VxVector& pos, VxVector& normal,float &length)
{
	pWorld *world=GetPMan()->getWorldByBody(a); 
	if (world)
	{

		pRigidBody*bodyA= world->getBody(a);
		if (bodyA)
		{
			dGeomID gid1  = bodyA->GetOdeGeom();
			if (gid1)
			{
				dxBody *bb;
				for (bb=world->World()->firstbody; bb; bb=(dxBody*)bb->next) 
				{
					pRigidBody *bodyB  = static_cast<pRigidBody*>(dBodyGetData(bb));
					if (bodyB==bodyA)
					{
						continue;
					}
					if (bodyB)
					{

						dGeomID gid2 = bodyB->GetOdeGeom();
						if (gid1==gid2)
						{
							continue;
						}
						if (gid2)
						{
							dContact *contact = new dContact[1];

							int nbContacts  = dCollide (gid1,gid2,1,&contact[0].geom,sizeof(dContactGeom));
							if (nbContacts)
							{

								pos.x = contact[0].geom.pos[0];
								pos.y  = contact[0].geom.pos[1];
								pos.z  = contact[0].geom.pos[2];

								normal.x =contact[0].geom.normal[0];
								normal.y =contact[0].geom.normal[1];
								normal.z = contact[0].geom.normal[2];

								length = contact[0].geom.depth;
								return bodyB->GetVT3DObject();                                
							}
						}
					}
				}
			}
		}
	}
	return NULL;
}



//////////////////////////////////////////////////////////////////////////
bool pWorld::CIsInCollision(vt3DObjectType a, vt3DObjectType b,VxVector& pos, VxVector& normal,float &length)
{

	bool result = false;

	pWorld *world=GetPMan()->getWorldByBody(a); 
	pWorld *world2=GetPMan()->getWorldByBody(b); 


	if (!world || !world2 || world!=world2 )
		return false;

	pRigidBody*bodyA= world->getBody(a);
	pRigidBody*bodyB= world->getBody(b);

	if (bodyA && bodyB && bodyA !=bodyB)
	{
		dGeomID gid1  = bodyA->GetOdeGeom();
		dGeomID gid2  = bodyB->GetOdeGeom();
		if (gid1 && gid2 && gid1!=gid2)
		{
			dContact *contact = new dContact[1];
			int nbContacts  = dCollide (gid1,gid2,1,&contact[0].geom,sizeof(dContactGeom));
			if (nbContacts)
			{
				pos.x = contact[0].geom.pos[0];
				pos.y  = contact[0].geom.pos[1];
				pos.z  = contact[0].geom.pos[2];

				normal.x =contact[0].geom.normal[0];
				normal.y =contact[0].geom.normal[1];
				normal.z = contact[0].geom.normal[2];
				length = contact[0].geom.depth;
				return true;
			}
		}
	}
	return result;
}
*/