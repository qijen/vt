#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "NXU_helper.h"  // NxuStream helper functions.
#include "NXU_PhysicsInstantiator.h"

#include "Stream.h"
#include "cooking.h"

#include "IParameter.h"

void pRigidBody::updateMassSettings(pMassSettings massSettings)
{

	// Referential
	CK3dEntity* massRef = (CK3dEntity*)GetPMan()->GetContext()->GetObject(massSettings.massReference);

	VxVector massLocalOut = massSettings.localPosition;
	//----------------------------------------------------------------
	//
	// position
	//
	if (XAbs(massSettings.localPosition.SquareMagnitude()) >0.0f )
	{
		if (massRef)
		{
			VxVector tOut = massLocalOut;
			massRef->Transform(&massLocalOut,&massLocalOut);
			massRef->TransformVector(&tOut,&tOut,GetVT3DObject());
			getActor()->setCMassOffsetLocalPosition(getFrom(tOut));
		}else
		{
			getActor()->setCMassOffsetLocalPosition(getFrom(massLocalOut));
		}
	}
	//----------------------------------------------------------------
	//
	// rotational offset  : todo
	//
	VxMatrix mat;
	Vx3DMatrixFromEulerAngles(mat,massSettings.localOrientation.x,massSettings.localOrientation.y,massSettings.localOrientation.z);

	VxQuaternion outQuat;
	outQuat.FromMatrix(mat);

	VxQuaternion referenceQuat=outQuat;
	if (XAbs(massSettings.localOrientation.SquareMagnitude()) >0.0f )
	{
		if (massRef)
		{
			massRef->GetQuaternion(&referenceQuat,NULL);
			getActor()->setCMassOffsetGlobalOrientation(getFrom(referenceQuat));
		}else{
			getActor()->setCMassOffsetGlobalOrientation(getFrom(outQuat));
		}
	}

	//----------------------------------------------------------------
	//
	// recompute mass
	//

	float newDensity=massSettings.newDensity;
	float totalMass =massSettings.totalMass;

	int bMassResult = 0 ; 
	if (newDensity!=0.0f || totalMass!=0.0f )
	{
		bMassResult  = getActor()->updateMassFromShapes(newDensity,totalMass);
	}

	int op  = bMassResult;


}

int pRigidBody::_initMainShape(const pObjectDescr oDescr,NxActorDesc *actorDesc)
{

	//----------------------------------------------------------------
	//
	// Sanity Checks 
	//
	#ifdef _DEBUG
		assert(GetVT3DObject());	// has to been set before
	#endif // _DEBUG

	//----------------------------------------------------------------
	//
	// Collect some data 
	//
	
	VxVector box_s= BoxGetZero(GetVT3DObject());
	float density = oDescr.density;
	float radius = GetVT3DObject()->GetRadius();


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
			//shape.localPose.t = pMath::getFrom(shapeOffset);
			if (oDescr.skinWidth!=-1.0f)
				shape.skinWidth = oDescr.skinWidth;

			actorDesc->shapes.pushBack(&shape);


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
			//shape.localPose.t = pMath::getFrom(shapeOffset);
			if (oDescr.skinWidth!=-1.0f)
				shape.skinWidth = oDescr.skinWidth;
			
			actorDesc->shapes.pushBack(&shape);

			break;
		}
		//////////////////////////////////////////////////////////////////////////
	case HT_Mesh:
		{

			if (! (oDescr.flags & BF_Deformable) )
			{

				//xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Can not use a mesh as de");
				//return NULL;

			}

			NxTriangleMeshDesc myMesh;
			myMesh.setToDefault();

			pFactory::Instance()->createMesh(getWorld()->getScene(),GetVT3DObject()->GetCurrentMesh(),myMesh);

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

			actorDesc->shapes.pushBack(&shape);
			
			//shape.localPose.t = pMath::getFrom(shapeOffset);
			if (oDescr.skinWidth!=-1.0f)
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
			if (GetVT3DObject()->GetCurrentMesh())
			{
				if (GetVT3DObject()->GetCurrentMesh()->GetVertexCount()>=256 )
				{
					xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Only 256 vertices for convex meshs allowed, by Ageia!");
					return false;
				}
			}else
			{
				xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Object has no mesh!");
				return false;


			}

			NxConvexMeshDesc myMesh;
			myMesh.setToDefault();
			pFactory::Instance()->createConvexMesh(getWorld()->getScene(),GetVT3DObject()->GetCurrentMesh(),myMesh);

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
			
			actorDesc->shapes.pushBack(&shape);

			if (oDescr.skinWidth!=-1.0f)
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
	case HT_ConvexCylinder:
		{
			NxConvexShapeDesc shape;
			if (!pFactory::Instance()->_createConvexCylinder(&shape,GetVT3DObject())){
				xLogger::xLog(XL_START,ELOGERROR,E_LI_AGEIA,"Couldn't create convex cylinder mesh");
				return false;
			}

			shape.density  = density;
			if (oDescr.skinWidth!=-1.0f)
				shape.skinWidth = oDescr.skinWidth;

			actorDesc->shapes.pushBack(&shape);
			break;
		}
		//////////////////////////////////////////////////////////////////////////
	case HT_Capsule:
		{

			NxCapsuleShapeDesc shape;

			if (! (oDescr.flags & BF_Deformable) )
			{

				pCapsuleSettings cSettings;

				pFactory::Instance()->findSettings(cSettings,GetVT3DObject());

				shape.radius = cSettings.radius > 0.0f ? cSettings.radius : box_s.v[cSettings.localRadiusAxis] * 0.5f;
				shape.height = cSettings.height > 0.0f ? cSettings.height : box_s.v[cSettings.localLengthAxis]  -  ( 2*shape.radius) ;
			}
			shape.density  = density;
			//	shape.materialIndex = result->getMaterial()->getMaterialIndex();
			//	shape.localPose.t = pMath::getFrom(shapeOffset);
			if (oDescr.skinWidth!=-1.0f)
				shape.skinWidth = oDescr.skinWidth;
			actorDesc->shapes.pushBack(&shape);
			break;
		}

	case HT_Wheel:
		{

			xLogger::xLog(XL_START,ELOGERROR,E_LI_AGEIA,"Wheel shape can be sub shape only!");
				return false;
		}
	}

	return true;

}


/*int pRigidBody::_initMainShape(const pObjectDescr oDescr,NxActorDesc&actorDesc)
{

	//----------------------------------------------------------------
	//
	// Sanity Checks 
	//
	#ifdef _DEBUG
		assert(GetVT3DObject());	// has to been set before
	#endif // _DEBUG

	//----------------------------------------------------------------
	//
	// Collect some data 
	//
	
	VxVector box_s= BoxGetZero(GetVT3DObject());
	float density = oDescr.density;
	float radius = GetVT3DObject()->GetRadius();


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
			//shape.localPose.t = pMath::getFrom(shapeOffset);
			if (oDescr.skinWidth!=-1.0f)
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
			//shape.localPose.t = pMath::getFrom(shapeOffset);
			if (oDescr.skinWidth!=-1.0f)
				shape.skinWidth = oDescr.skinWidth;

			int oG = oDescr.collisionGroup;
			shape.group = oDescr.collisionGroup;

			int k  = shape.isValid();

			actorDesc.shapes.pushBack(&shape);

			break;
		}
		//////////////////////////////////////////////////////////////////////////
	case HT_Mesh:
		{

			if (! (oDescr.flags & BF_Deformable) )
			{

				//xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Can not use a mesh as de");
				//return NULL;

			}

			NxTriangleMeshDesc myMesh;
			myMesh.setToDefault();

			pFactory::Instance()->createMesh(getWorld()->getScene(),GetVT3DObject()->GetCurrentMesh(),myMesh);

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

			
			//shape.localPose.t = pMath::getFrom(shapeOffset);
			if (oDescr.skinWidth!=-1.0f)
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
			if (GetVT3DObject()->GetCurrentMesh())
			{
				if (GetVT3DObject()->GetCurrentMesh()->GetVertexCount()>=256 )
				{
					xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Only 256 vertices for convex meshs allowed, by Ageia!");
					return false;
				}
			}else
			{
				xLogger::xLog(ELOGERROR,E_LI_AGEIA,"Object has no mesh!");
				return false;


			}

			NxConvexMeshDesc myMesh;
			myMesh.setToDefault();
			pFactory::Instance()->createConvexMesh(getWorld()->getScene(),GetVT3DObject()->GetCurrentMesh(),myMesh);

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
			//	shape.materialIndex = result->getMaterial()->getMaterialIndex();
			actorDesc.shapes.pushBack(&shape);
			//shape.localPose.t = pMath::getFrom(shapeOffset);
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
			if (!pFactory::Instance()->_createConvexCylinder(&shape,GetVT3DObject())){
				xLogger::xLog(XL_START,ELOGERROR,E_LI_AGEIA,"Couldn't create convex cylinder mesh");
				return false;
			}

			shape.density  = density;
			if (oDescr.skinWidth!=-1.0f)
				shape.skinWidth = oDescr.skinWidth;

			actorDesc.shapes.pushBack(&shape);
			break;
		}
		//////////////////////////////////////////////////////////////////////////
	case HT_Capsule:
		{

			NxCapsuleShapeDesc shape;

			if (! (oDescr.flags & BF_Deformable) )
			{

				pCapsuleSettings cSettings;

				pFactory::Instance()->findSettings(cSettings,GetVT3DObject());

				shape.radius = cSettings.radius > 0.0f ? cSettings.radius : box_s.v[cSettings.localRadiusAxis] * 0.5f;
				shape.height = cSettings.height > 0.0f ? cSettings.height : box_s.v[cSettings.localLengthAxis]  -  ( 2*shape.radius) ;
			}
			shape.density  = density;
			//	shape.materialIndex = result->getMaterial()->getMaterialIndex();
			//	shape.localPose.t = pMath::getFrom(shapeOffset);
			if (oDescr.skinWidth!=-1.0f)
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

	return true;

}
*/

int pRigidBody::getNbJoints()
{

	pWorld *wA =getWorld();
	
	int result   = 0 ;

	if (!wA)
	{
		return result;
	}


	NxU32	jointCount = wA->getScene()->getNbJoints();
	if (jointCount)
	{
		NxArray< NxJoint * > joints;

		wA->getScene()->resetJointIterator();

		for	(NxU32 i = 0;	i	<	jointCount;	++i)
		{
			NxJoint	*j = wA->getScene()->getNextJoint();
			pJoint *mJoint = static_cast<pJoint*>( j->userData );

			if ( mJoint )
			{
				if (mJoint->GetVTEntA() == GetVT3DObject() || mJoint->GetVTEntB() == GetVT3DObject() )
				{
					result++;
				}
			}
		}
	}

	return result;
}

void pRigidBody::test()
{

	pRigidBody *body  = this;
	CK3dEntity* test = (CK3dEntity*)body;

}
void pRigidBody::readFrom(NXU::NxActorDesc *desc,int flags)
{

	if ( desc->mHasBody ) // only for dynamic actors
	{
		for (NxU32 k=0; k<desc->mShapes.size(); k++)
		{
			NXU::NxShapeDesc *shape = desc->mShapes[k];
			NxVec3 locPos = shape->localPose.t;
			NxQuat localQuad = shape->localPose.M;
		}
	}

}

float pRigidBody::getMass(CK3dEntity *shapeReference/* =NULL */)
{
	float result = 0 ; 
	if (getActor())
	{
		result = getActor()->getMass();
	}
	return result;
}
VxVector pRigidBody::getLocalPointVelocity( const VxVector& point ) const
{
		if(isValid())
			return pMath::getFrom(getActor()->getLocalPointVelocity(getFrom(point)));
	return VxVector();
}

VxVector pRigidBody::getPointVelocity( const VxVector& point ) const
{
		if(isValid())
			return pMath::getFrom(getActor()->getPointVelocity(getFrom(point)));
	return VxVector();
}
pJoint* pRigidBody::isConnected(CK3dEntity*ent)
{
	
	
	pJoint *result  =  NULL;
	pWorld *wA =getWorld();
	pWorld *wB =NULL;
	pRigidBody *b = NULL;
	if (!wA)
	{
		return NULL;
	}

	if (ent)
	{
		b = GetPMan()->getBody(ent);
	}
	if (b)
	{
		wB = b->getWorld();
	}

	NxU32	jointCount = wA->getScene()->getNbJoints();
	if (jointCount)
	{
		NxArray< NxJoint * > joints;

		wA->getScene()->resetJointIterator();
		for	(NxU32 i = 0;	i	<	jointCount;	++i)
		{
			NxJoint	*j = wA->getScene()->getNextJoint();
			pJoint *mJoint = static_cast<pJoint*>( j->userData );

			if ( mJoint )
			{
				if (mJoint->GetVTEntA() == GetVT3DObject() && mJoint->GetVTEntB() == ent )
				{
					return mJoint;
				}
				if (mJoint->GetVTEntB() == GetVT3DObject() && mJoint->GetVTEntA() == ent )
				{
					return mJoint;
				}
			}
		}
	}
	return NULL;
}

int pRigidBody::JGetNumJoints()
{
	int result  =  0  ;   
	/*
	OdeBodyType bid = GetOdeBody();
	if (bid)
	{
		dxJoint *j =  NULL;
		for (j=World()->World()->firstjoint; j; j=(dxJoint*)j->next) 
		{
			pJoint *pJ  =  static_cast<pJoint*>(dJointGetData(j));
			if (pJ)
			{
				if (pJ->GetOdeBodyA()   == bid  || pJ->GetOdeBodyA()   == bid   )
				{
					result ++;
				}
			}
		}
	}
	*/
	return 0;
}
void pRigidBody::setLocalShapePosition(VxVector relative,CK3dEntity*shapeReference/* =NULL */)
{

	
	if(!getActor())
		return;

	NxShape *subShape = getSubShape(shapeReference);
	if (subShape )
	{
		subShape->setLocalPosition(getFrom(relative));
	}

	/*NxU32 nbShapes = getActor()->getNbShapes();
   	if ( nbShapes )
   	{
   		NxShape ** slist = (NxShape **)getActor()->getShapes();
   		for (NxU32 j=0; j<nbShapes; j++)
   		{
   			NxShape *s = slist[j];
			if (s)
			{
				s->setLocalPosition(pMath::getFrom(relative));
				int op=2;
				return;

				//getActor()->updateMassFromShapes()
			}
   			//addShapeMesh(c,s);
    	}
	}*/

}






pRigidBody::pRigidBody(CK3dEntity* _e,pWorld *world) : xEngineObjectAssociation<CK3dEntity*>(_e,_e->GetID()) , mVTObject(_e) , m_pWorld(world)
{

	using namespace vtTools::AttributeTools;
	mActor = NULL;
	
	context = NULL;
	if (_e)
	{
		mEntID  = _e->GetID();
		mVTObject = _e;
	}
	mMaterial  = NULL;
	mDataFlags = 0;
	mMainShape = NULL;
	mMaterial  = NULL;
	mVehicle = NULL;
	mMainShape = NULL;
	mActor =NULL;
	mCloth = NULL;

	//mClothRecieveBuffer=NULL;

}


pRigidBody::pRigidBody(CK3dEntity* _e)
{
	SetVT3DObject(_e);
	if (_e){
		mEntID  = _e->GetID();
		mVTObject = _e;

	}
	mMaterial  = NULL;
	mDataFlags = 0;
	mVehicle = NULL;
	mMainShape = NULL;
	mActor =NULL;
	mVTObject = NULL;
	m_pWorld = NULL;
	mCloth = NULL;
	mInitialDescription = NULL;


       	
}


void pRigidBody::addLocalForceAtLocalPos(const VxVector& force, const VxVector& point,ForceMode mode,bool wakeUp)
{
	if(isValid())		
		getActor()->addLocalForceAtLocalPos(pMath::getFrom(force),pMath::getFrom(point),(NxForceMode)mode,wakeUp);
}
void pRigidBody::addLocalForceAtPos(const VxVector& force, const VxVector& point,ForceMode mode,bool wakeUp)
{			
	if(isValid())		
		getActor()->addLocalForceAtPos(pMath::getFrom(force),pMath::getFrom(point),(NxForceMode)mode,wakeUp);
}
void pRigidBody::addForceAtLocalPos(const VxVector& force, const VxVector& point,ForceMode mode,bool wakeUp)
{
	if(isValid())		
		getActor()->addForceAtLocalPos(pMath::getFrom(force),pMath::getFrom(point),(NxForceMode)mode,wakeUp);
}
void pRigidBody::addForceAtPos(const VxVector& force, const VxVector& point,ForceMode mode,bool wakeUp)
{			
	if(isValid())		
		getActor()->addForceAtPos(pMath::getFrom(force),pMath::getFrom(point),(NxForceMode)mode);
}
void pRigidBody::addTorque(const VxVector& torque,ForceMode mode,bool wakeUp)
{				
	if(isValid())	
		getActor()->addTorque(pMath::getFrom(torque),(NxForceMode)mode,wakeUp);
}
void pRigidBody::addLocalTorque(const VxVector& torque,ForceMode mode,bool wakeUp)
{									
	if(isValid())	
		getActor()->addLocalTorque(pMath::getFrom(torque),(NxForceMode)mode);
}

void pRigidBody::addLocalForce(const VxVector& force,ForceMode mode,bool wakeUp)
{			
	if(isValid())		
		getActor()->addLocalForce(pMath::getFrom(force),(NxForceMode)mode,wakeUp);	
}

void pRigidBody::addForce( const VxVector& force,ForceMode mode/*=E_FM_FORCE*/,bool wakeUp/*=true*/ )
{											
	if(isValid())	
		getActor()->addForce(pMath::getFrom(force),(NxForceMode)mode,wakeUp);	
}


void pRigidBody::setLinearMomentum( const VxVector& linMoment )
{	if(isValid())		getActor()->setLinearMomentum(pMath::getFrom(linMoment));	}
void pRigidBody::setAngularMomentum( const VxVector& angMoment )
{	
	if(isValid())	
		getActor()->setAngularMomentum(pMath::getFrom(angMoment));
}
VxVector pRigidBody::getAngularMomentum() const
{	
	if(isValid())
		return pMath::getFrom(getActor()->getAngularMomentum());
	return VxVector();
}

float pRigidBody::getAngularDamping() const
{	
	if(isValid())
		return getActor()->getAngularDamping();
	return 0.0f;
}
float pRigidBody::getLinearDamping() const
{	
	if(isValid())
		return getActor()->getLinearDamping();
	return 0.0f;
}
VxVector pRigidBody::getLinearMomentum()const{	
	if(isValid())
		return pMath::getFrom(getActor()->getLinearMomentum());
	return VxVector();
}
void pRigidBody::setPosition(const VxVector& pos,CK3dEntity *subShapeReference)
{		
	if(isValid()){

		
		NxShape *subShape = getSubShape(subShapeReference);
		if (subShape == getMainShape() )
		{
			if (!isKinematic())
			{
				getActor()->setGlobalPosition(pMath::getFrom(pos));

			}else{
				getActor()->moveGlobalPosition(pMath::getFrom(pos));

			}
		}else{
			if (subShape)
			{
				subShape->setGlobalPosition(getFrom(pos));
			}
		}
	}
}

void pRigidBody::setRotation(const VxQuaternion& rot,CK3dEntity *subShapeReference)
{
	if(isValid())		
	{
		NxShape *subShape = getSubShape(subShapeReference);
		if (subShape == getMainShape() )
		{
			getActor()->setGlobalOrientation(pMath::getFrom(rot));
		}else 
		{
            if (subShape)
            {
				subShape->setGlobalOrientation(getFrom(rot));
            }
		}
	}
}
float pRigidBody::getMaxAngularSpeed() const
{	
	if(isValid())return getActor()->getMaxAngularVelocity();
	return 0.0f;
}
void pRigidBody::setMaxAngularSpeed(float val) {	if(isValid())getActor()->setMaxAngularVelocity(val);	}
VxVector pRigidBody::getLinearVelocity() const{		return pMath::getFrom(getActor()->getLinearVelocity());}
VxVector pRigidBody::getAngularVelocity() const{	return pMath::getFrom(getActor()->getAngularVelocity());}
void pRigidBody::setLinearVelocity( const VxVector& linVel )
{		if(isValid())						
			getActor()->setLinearVelocity(pMath::getFrom(linVel));
}
void pRigidBody::setAngularVelocity( const VxVector& angVel )
{
	if(isValid())			
		getActor()->setAngularVelocity(pMath::getFrom(angVel));
}

void pRigidBody::setAngularDamping(float scale)
{

	if (getActor() && getActor()->isDynamic())
	{
		getActor()->setAngularDamping(scale);
	}
}
void pRigidBody::setLinearDamping(float scale)
{

	if (getActor() && getActor()->isDynamic())
	{
		getActor()->setLinearDamping(scale);
	}

}


void pRigidBody::setCMassOffsetLocalPosition(VxVector offset)
{
	if (getActor())
	{
		getActor()->setCMassOffsetLocalPosition( getFrom(offset) );
	}
}

void pRigidBody::translateLocalShapePosition(VxVector vec)
{

	if (getActor())
	{

		if (getMainShape())
		{
			NxVec3 currentPos = getMainShape()->getLocalPosition();
			getMainShape()->setLocalPosition( currentPos + getFrom(vec));
		}
	}
}

pJoint* pRigidBody::isConnected(CK3dEntity*ent,int type)
{
	/*
	pJoint *result  =  NULL;

	if ( GetVT3DObject()!=ent)
	{
	pRigidBody *b = GetPMan()->getBody(ent);

	OdeBodyType abid = GetOdeBody();
	OdeBodyType bbid = NULL;

	if (b)
	{
	bbid  = b->GetOdeBody();
	}

	dxJoint *j =  NULL;
	for (j=World()->World()->firstjoint; j; j=(dxJoint*)j->next) 
	{
	pJoint *pJ  =  static_cast<pJoint*>(dJointGetData(j));
	if (pJ)
	{

	if (!bbid)
	{
	if (dJointGetType(j) ==dJointTypeFixed )
	{
	if (pJ->GetOdeBodyA()==abid)
	{
	return pJ;
	}
	}
	}

	if ( pJ->GetOdeBodyA()==abid && dJointGetType(j) == type)
	{
	if (pJ->GetOdeBodyB()==bbid )
	{
	return pJ;
	}
	}

	if ( pJ->GetOdeBodyA()==bbid && dJointGetType(j) == type)
	{
	if (pJ->GetOdeBodyB()==abid )
	{
	return pJ;
	}
	}
	}
	}
	}
	return result;
	*/
	return NULL;
}

void pRigidBody::SetVT3DObject(CK3dEntity* _obj)
{ mEntID = _obj->GetID(); mVTObject=_obj;} 

CK3dEntity*pRigidBody::GetVT3DObject()
{	return mVTObject;	}