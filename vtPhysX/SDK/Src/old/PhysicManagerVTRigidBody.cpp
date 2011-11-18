#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "VSLManagerSDK.h"

//#include "pVehicle.h"




PhysicManager *ourMan = NULL;

CKGUID GetPhysicManagerGUID() { 	return GUID_MODULE_MANAGER;}

typedef ForceMode PForceMode;
typedef D6MotionMode PJMotion;
typedef D6DriveType PDriveType;
typedef int BodyLockFlags;
//////////////////////////////////////////////////////////////////////////

#define TESTGUID CKGUID(0x2c5c47f6,0x1d0755d9)


void PhysicManager::_RegisterVSLRigidBody()
{

		
	
	STARTVSLBIND(m_Context)





	STOPVSLBIND
	

}


PhysicManager*GetPhysicManager()
{
	return GetPMan();
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