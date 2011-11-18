  PhysicManager pm = GetPhysicManager();
  pRigidBody b = pm.getBody(body);
  if(b)
  {
	b.setPosition(myPosition);
	b.setRotation(myQuat);
	b.setLinearMomentum(theNewForce);
	b.setAngularMomentum(theNewTorque);
	b.setLinearVelocity(theNewVelocity);
	b.setAngularVelocity(theNewVelocity);
	
  }
