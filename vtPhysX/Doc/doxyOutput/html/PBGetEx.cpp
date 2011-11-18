  PhysicManager pm = GetPhysicManager();
  pRigidBody b = pm.getBody(body);
  if(b)
  {
	int collGroupMainShape = b.getCollisionsGroup();
	bool isKinematic = b.isKinematic();
	bool isGravityOn = b.isAffectedByGravity();
	bool is collisionOn = b.isCollisionEnabled();
   }
