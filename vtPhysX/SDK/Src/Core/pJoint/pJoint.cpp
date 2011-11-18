#include <StdAfx.h>
#include "vtPhysXAll.h"


using namespace vtAgeia;

VxVector pJoint::getGlobalAxis(){
	if (!getJoint())
		return VxVector(-1,-1,-1);

	return getFrom(getJoint()->getGlobalAxis());
	
}

void pJoint::setGlobalAxis(VxVector axis)
{
	if (getJoint())
		getJoint()->setGlobalAxis(getFrom(axis));
}


VxVector pJoint::getGlobalAnchor(){
	if (!getJoint())
		return VxVector(-1,-1,-1);

	return getFrom(getJoint()->getGlobalAnchor());

}

void pJoint::setGlobalAnchor(VxVector anchor)
{
	if (getJoint())
		getJoint()->setGlobalAnchor(getFrom(anchor));
}


int pJoint::getNbLimitPlanes()
{
	if (!getJoint())
		return -1;

	NxJoint *j = getJoint();
	j->resetLimitPlaneIterator();
	int numLimitPlanes = 0;
	NxVec3 limitPoint;
	if ( j->hasMoreLimitPlanes() )
	{
		NxVec3 planeNormal;
		NxReal planeD;
		NxReal restitution;

		bool ok = true;
		while ( ok )
		{
			j->getNextLimitPlane( planeNormal, planeD, &restitution );
			++numLimitPlanes;
			ok = j->hasMoreLimitPlanes();
		}
	}
    
	return numLimitPlanes;

}

//////////////////////////////////////////////////////////////////////////
pJointFixed::pJointFixed(pRigidBody* _a,pRigidBody* _b) : pJoint(_a,_b,JT_Fixed)
{
}

pJoint::pJoint(pRigidBody* _a,pRigidBody* _b,int _type ) : m_SolidA(_a) , m_SolidB(_b) ,m_type((JType)_type)
{
	m_vtObjectA = _a ? _a->GetVT3DObject() : NULL;
	m_vtObjectB = _b ? _b->GetVT3DObject() : NULL;

	mAID = _a ? _a->GetVT3DObject()->GetID() : -1 ;
	mBID = _b ? _b->GetVT3DObject()->GetID() : -1 ;

	mJoint = NULL;
	m_pWorld = NULL;
}
pJointD6*pJoint::castD6Joint()
{

	if (getJoint())
	{
		if (getJoint()->isD6Joint())
		{
			return static_cast<pJointD6*>(this);
		}
	}
	return NULL;
}


pJointPulley* pJoint::castPulley()
{

	
	if (getJoint())
	{
		if (getJoint()->isPulleyJoint())
		{
			return static_cast<pJointPulley*>(this);
		}
	}
	return NULL;
}
pJointDistance* pJoint::castDistanceJoint()
{

	
	if (getJoint())
	{
		if (getJoint()->isDistanceJoint())
		{
			return static_cast<pJointDistance*>(this);
		}
	}
	return NULL;
}

pJointBall* pJoint::castBall()
{

	
	if (getJoint())
	{
		if (getJoint()->isSphericalJoint())
		{
			return static_cast<pJointBall*>(this);
		}
	}
	return NULL;
}

pJointRevolute* pJoint::castRevolute()
{


	if (getJoint())
	{
		if (getJoint()->isRevoluteJoint())
		{
			return static_cast<pJointRevolute*>(this);
		}
	}
	return NULL;
}

pJointPrismatic* pJoint::castPrismatic()
{


	if (getJoint())
	{
		if (getJoint()->isPrismaticJoint())
		{
			return static_cast<pJointPrismatic*>(this);
		}
	}
	return NULL;
}
pJointCylindrical* pJoint::castCylindrical()
{


	if (getJoint())
	{
		if (getJoint()->isCylindricalJoint())
		{
			return static_cast<pJointCylindrical*>(this);
		}
	}
	return NULL;
}

pJointPointInPlane* pJoint::castPointInPlane()
{
	if (getJoint())
	{
		if (getJoint()->isPointInPlaneJoint())
		{
			return static_cast<pJointPointInPlane*>(this);
		}
	}
	return NULL;
}

pJointPointOnLine* pJoint::castPointOnLine()
{
	if (getJoint())
	{
		if (getJoint()->isPointOnLineJoint())
		{
			return static_cast<pJointPointOnLine*>(this);
		}
	}
	return NULL;
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
int pJoint::addLimitPlane(const VxVector normal, VxVector pointInPlane, float restitution/* =0.0f */)
{	
	int res = -1;
	if (getJoint()){		
	
		res  = getJoint()->addLimitPlane(pMath::getFrom(normal),pMath::getFrom(pointInPlane),restitution);

	}
	return res;
}
void pJoint::setLimitPoint( VxVector point,bool pointIsOnActor2/*=true*/ )
{	
	if (getJoint()){		
		return getJoint()->setLimitPoint(getFrom(point),pointIsOnActor2);
	}
}
void pJoint::purgeLimitPlanes(){ 	if (getJoint())		getJoint()->purgeLimitPlanes();}
void pJoint::resetLimitPlaneIterator(){ 	if (getJoint())		getJoint()->resetLimitPlaneIterator();}
int pJoint::hasMoreLimitPlanes(){ 	if (getJoint())		return getJoint()->hasMoreLimitPlanes(); return false;}
int pJoint::getNextLimitPlane (VxVector &planeNormal, float &planeD,float *restitution)
{
	if (getJoint())
	{
		NxVec3 n;
		
		int k =getJoint()->getNextLimitPlane(n,planeD,restitution);
		planeNormal = pMath::getFrom(n);
		return k;
	}
	return 0;
}



void pJoint::getBreakForces( float& maxForce,float& maxTorque )
{
	if (getJoint())
		getJoint()->getBreakable(maxForce,maxTorque);    
}

void pJoint::setBreakForces( float maxForce,float maxTorque )
{
	if (!getJoint())
	return;

	if ( maxTorque !=0.0f && maxForce != 0.0f)
		getJoint()->setBreakable(maxForce,maxTorque);    

	//if ( maxTorque =0.0f && maxForce = 0.0f)
	//	getJoint()->setBreakable(,maxTorque);    


}

void pJoint::setLocalAnchor0(VxVector anchor0)
{
	if (getJoint())
	{

		if(getJoint()->isRevoluteJoint())
		{
			NxRevoluteJointDesc descr;	
			NxRevoluteJoint *joint  = static_cast<NxRevoluteJoint*>(getJoint());
			if (!joint)return;
			joint->saveToDesc(descr);
			descr.localAnchor[0]  = getFrom(anchor0);
			joint->loadFromDesc(descr);

		}
	}
}






/************************************************************************/
/*                                                                      */
/************************************************************************/

