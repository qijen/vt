#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "VSLManagerSDK.h"

//#include "pVehicle.h"








void PhysicManager::_RegisterVSLJoint()
{
	
		
	
	STARTVSLBIND(m_Context)



	STOPVSLBIND
	

}



/*
void __newvtWorldSettings(BYTE *iAdd) 
{
new (iAdd) pWorldSettings();
}
void __newvtSleepingSettings(BYTE *iAdd) 
{
new (iAdd) pSleepingSettings();
}
void __newvtJointSettings(BYTE *iAdd) 
{
new (iAdd) pJointSettings();
}


int TestWS(pWorldSettings pWS)
{
VxVector grav = pWS.Gravity();
return 2;
}

pFactory* GetPFactory();


pFactory* GetPFactory()
{
return pFactory::Instance();
}


extern pRigidBody*getBody(CK3dEntity*ent);
*/