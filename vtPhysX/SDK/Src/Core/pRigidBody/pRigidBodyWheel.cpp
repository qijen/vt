#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "NXU_helper.h"  // NxuStream helper functions.
#include "NXU_PhysicsInstantiator.h"


void pRigidBody::updateWheels(float step)
{

	

	NxVec3 _localVelocity;

	bool _breaking=false;


	/*
	_computeMostTouchedActor();
	NxVec3 relativeVelocity;
	if (_mostTouchedActor == NULL || !_mostTouchedActor->isDynamic())
	{
		relativeVelocity = getActor()->getLinearVelocity();
	} else {
		relativeVelocity = getActor()->getLinearVelocity() - _mostTouchedActor->getLinearVelocity();
	}
	NxQuat rotation = getActor()->getGlobalOrientationQuat();
	NxQuat global2Local;
	_localVelocity = relativeVelocity;
	rotation.inverseRotate(_localVelocity);
	char master[512];
	*/
	int nbShapes = getActor()->getNbShapes();
	NxShape ** slist = (NxShape **)getActor()->getShapes();
	for (NxU32 j=0; j<nbShapes; j++)
	{
		NxShape *s = slist[j];
		if (s)
		{
			pSubMeshInfo *sinfo = static_cast<pSubMeshInfo*>(s->userData);
			if (sinfo && sinfo->wheel !=NULL)
			{
				pWheel *wheel = sinfo->wheel;
				pWheel2* wheel2 = dynamic_cast<pWheel2*>(wheel);
				
				if (wheel->getWheelFlag(WF_VehicleControlled))
				{
					continue;
				}
					
				wheel->_tick(step / GetPMan()->GetContext()->GetTimeManager()->GetTimeScaleFactor());

				/*

				NxWheelShape *wShape = wheel2->getWheelShape();
				if (!wShape) continue;



				//////////////////////////////////////////////////////////////////////////
				//
				//
				//
				NxWheelContactData wcd; 
				NxShape* contactShape = wShape->getContact(wcd);
				
				if (contactShape)
				{
					
					NxVec3 relativeVelocity;
					if ( !contactShape->getActor().isDynamic())
					{
						relativeVelocity = getActor()->getLinearVelocity();
					} else {
						relativeVelocity = getActor()->getLinearVelocity() - contactShape->getActor().getLinearVelocity();
					}
					NxQuat rotation = getActor()->getGlobalOrientationQuat();

					_localVelocity = relativeVelocity;
					rotation.inverseRotate(_localVelocity);
					_breaking = NxMath::abs(_localVelocity.z) < ( 0.1 );
//					wShape->setAxleSpeed()
				}

		
				float rollAngle = wheel2->getWheelRollAngle();
				rollAngle+=wShape->getAxleSpeed() * (step* 0.01f);
				while (rollAngle > NxTwoPi)	//normally just 1x
					rollAngle-= NxTwoPi;
				while (rollAngle< -NxTwoPi)	//normally just 1x
					rollAngle+= NxTwoPi;
				
				wheel2->setWheelRollAngle(rollAngle);

				NxMat34& wheelPose = wShape->getGlobalPose();

				
				NxReal  stravel = wShape->getSuspensionTravel();
				NxReal radius = wShape->getRadius();


				//have ground contact?
				if( contactShape && wcd.contactPosition <=  (stravel + radius) ) {
					wheelPose.t = NxVec3( wheelPose.t.x, wcd.contactPoint.y + wheel2->getRadius(), wheelPose.t.z );
				}
				else {
					wheelPose.t = NxVec3( wheelPose.t.x, wheelPose.t.y - wheel2->getSuspensionTravel(), wheelPose.t.z );
				}

				float rAngle = wheel2->getWheelRollAngle();
				float steer = wShape->getSteerAngle();

				NxMat33 rot, axisRot, rollRot;
				rot.rotY( wShape->getSteerAngle() );
				axisRot.rotY(0);
				rollRot.rotX(rAngle);
				wheelPose.M = rot * wheelPose.M * axisRot * rollRot;
				wheel2->setWheelPose(wheelPose);

				*/
				
			}
		}
	}
/*	//motorTorque *= 0.1f;
	brakeTorque *= 500.0f;
	if(handBrake && getWheelFlag(E_WF_AFFECTED_BY_HANDBRAKE))
		brakeTorque = 1000.0f;
	if(getWheelFlag(E_WF_ACCELERATED)) 
		mWheelShape->setMotorTorque(motorTorque);
	mWheelShape->setBrakeTorque(brakeTorque);


	


	mWheelPose = mWheelShape->getGlobalPose();

	NxWheelContactData wcd;
	NxShape* s = mWheelShape->getContact(wcd);	
	NxReal  stravel = mWheelShape->getSuspensionTravel();
	NxReal radius = mWheelShape->getRadius();


	//have ground contact?
	if( s && wcd.contactPosition <=  (stravel + radius) ) {
		mWheelPose.t = NxVec3( mWheelPose.t.x, wcd.contactPoint.y + radius, mWheelPose.t.z );
	}
	else {
		mWheelPose.t = NxVec3( mWheelPose.t.x, mWheelPose.t.y - stravel, mWheelPose.t.z );
	}


	NxMat33 rot, axisRot, rollRot;
	rot.rotY( mWheelShape->getSteerAngle() );
	//rot.rotX(0);
	axisRot.rotY(0);
	rollRot.rotX(_wheelRollAngle);
	mWheelPose.M = rot * mWheelPose.M * axisRot * rollRot;

	*/

}
bool pRigidBody::hasWheels()
{

	bool result = false;

	if (!getActor())
	{
		return false;
	}
	int nbShapes = getActor()->getNbShapes();
	NxShape ** slist = (NxShape **)getActor()->getShapes();
	for (NxU32 j=0; j<nbShapes; j++)
	{
		NxShape *s = slist[j];
		if (s)
		{
			pSubMeshInfo *sinfo = static_cast<pSubMeshInfo*>(s->userData);
			if (sinfo && sinfo->wheel !=NULL)
			{
				return true;
			}
		}
	}

	return result;

}

int pRigidBody::getNbWheels()
{

	int result = 0;

	if (!getActor())
	{
		return NULL;
	}
	int nbShapes = getActor()->getNbShapes();
	NxShape ** slist = (NxShape **)getActor()->getShapes();
	for (NxU32 j=0; j<nbShapes; j++)
	{
		NxShape *s = slist[j];
		if (s)
		{
			pSubMeshInfo *sinfo = static_cast<pSubMeshInfo*>(s->userData);
			if (sinfo && sinfo->wheel !=NULL)
			{
				result ++;
			}
		}
	}
	return result;
}

pWheel2 *pRigidBody::getWheel2(CK3dEntity* subShapeReference)
{
	return static_cast<pWheel2*>(getWheel(subShapeReference));
}
pWheel *pRigidBody::getWheel(CK3dEntity* subShapeReference)
{

	pWheel *result = NULL;
	if (!subShapeReference)
	{
		return result;
	}

	NxShape *subShape = _getSubShapeByEntityID(subShapeReference->GetID());
	if (!subShape) 
		return result;

	pSubMeshInfo *sInfo = static_cast<pSubMeshInfo*>(subShape->userData);

	if (sInfo && sInfo->wheel)
	{
		return sInfo->wheel;
	}
	/*

	NxWheelShape *wheel = static_cast<NxWheelShape*>(subShape->isWheel());
	if (wheel)
	{
	pSubMeshInfo *sInfo = static_cast<pSubMeshInfo*>(subShape->userData);

	if (sInfo && sInfo->wheel)
	{
	return sInfo->wheel;
	}
	}
	*/
	return result;

}