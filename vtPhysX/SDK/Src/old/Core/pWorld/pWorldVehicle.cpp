#include <StdAfx.h>
#include "vtPhysXAll.h"


void pWorld::updateVehicle(pVehicle *vehicle, float dt)
{
	if (vehicle)
	{ 
		vehicle->updateVehicle(dt);
	}
}

void pWorld::updateVehicles(float dt)
{

	int nbActors = getScene()->getNbActors();
	if (!nbActors)
	{
		return;
	}
	NxActor** actors = getScene()->getActors();
	while(nbActors--)
	{
		NxActor* actor = *actors++;
		pRigidBody *body = static_cast<pRigidBody*>(actor->userData);
		if (body)
		{
			if (!isVehicle(actor) && body->hasWheels())
			{
				body->updateWheels(dt);
			}
		}
		
		if (body && isVehicle(actor))
		{
			updateVehicle(body->getVehicle(),dt);
		}
	}
}
bool pWorld::isVehicle(NxActor* actor)
{

	pRigidBody *body = static_cast<pRigidBody*>(actor->userData);
	if (body)
	{
		return body->getVehicle() ? true : false;
	}
	return false;
}

pVehicle*pWorld::getVehicle(CK3dEntity* body)
{

	pRigidBody *rbody = getBody(body);
	if (body && rbody->getVehicle() )
	{
		return rbody->getVehicle();
	}
	return NULL;
}
