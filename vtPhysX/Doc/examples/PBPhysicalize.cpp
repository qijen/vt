pFactory pf  = getPFactory();

pObjectDescr descr;

descr.flags = BF_Moving + BF_Gravity + BF_Collision;
descr.density = 1.0f;
descr.hullType = HT_Box;
descr.shapeOffset = pivotOffset;

 pRigidBody body = pf.createBody(entity,descr);
