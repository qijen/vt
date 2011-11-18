#include "CKAll.h"

#ifdef WebPack





#define CAMERA_BEHAVIOR	CKGUID(0x12d94eba,0x47057415)
#define CKPGUID_PROJECTIONTYPE		CKDEFINEGUID(0x1ee22148, 0x602c1ca1)
#define CKPGUID_MOUSEBUTTON CKGUID(0x1ff24d5a,0x122f2c1f)

CKERROR InitInstanceCamera(CKContext* context)
{

	CKParameterManager* pm = context->GetParameterManager();
	pm->RegisterNewEnum( CKPGUID_PROJECTIONTYPE,"Projection Mode","Perspective=1,Orthographic=2" );
	// Mouse Camera Orbit
	pm->RegisterNewEnum(CKPGUID_MOUSEBUTTON,"Mouse Button","Left=0,Middle=2,Right=1" );

	return CK_OK;

}

CKERROR ExitInstanceCamera(CKContext* context)
{
	CKParameterManager* pm = context->GetParameterManager();
	pm->UnRegisterParameterType(CKPGUID_PROJECTIONTYPE);
	pm->UnRegisterParameterType(CKPGUID_MOUSEBUTTON);
	return CK_OK;

}



int RegisterCameraBeh(XObjectDeclarationArray *reg)
{

	// Cameras/Basic
	RegisterBehavior(reg, FillBehaviorDollyDecl);
	RegisterBehavior(reg, FillBehaviorSetOrthographicZoomDecl);
	RegisterBehavior(reg, FillBehaviorSetCameraTargetDecl);
	RegisterBehavior(reg, FillBehaviorSetClippingDecl);
	RegisterBehavior(reg, FillBehaviorSetFOVDecl);
	RegisterBehavior(reg, FillBehaviorSetProjectionDecl);
	RegisterBehavior(reg, FillBehaviorSetZoomDecl);

	// Cameras/FX
	RegisterBehavior(reg, FillBehaviorCameraColorFilterDecl);
	RegisterBehavior(reg, FillBehaviorVertigoDecl);

	// Cameras/Montage
	RegisterBehavior(reg, FillBehaviorGetCurrentCameraDecl);
	RegisterBehavior(reg, FillBehaviorSetAsActiveCameraDecl);

	// Cameras/Movement
	RegisterBehavior(reg, FillBehaviorOrbitDecl);
	RegisterBehavior(reg, FillBehaviorKeyboardCameraOrbitDecl);
	RegisterBehavior(reg, FillBehaviorMouseCameraOrbitDecl);
	RegisterBehavior(reg, FillBehaviorJoystickCameraOrbitDecl);
	RegisterBehavior(reg, FillBehaviorGenericCameraOrbitDecl);


	return 0;
}

#endif