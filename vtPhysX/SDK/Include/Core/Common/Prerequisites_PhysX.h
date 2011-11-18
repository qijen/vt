#ifndef __PREREQUISITES_PHYS_X_H__
#define __PREREQUISITES_PHYS_X_H__

class NxCompartment;
class NxActor;
class NxActorDesc;
class NxScene;
class NxSceneDescr;
class NxUserNotify;
class NxFluidUserNotify;
class NxCloth;
class NxClothDesc;
class NxClothUserNotify;
class NxClothMeshDesc;

class NxSoftBodyUserNotify;
class NxUserContactModify;
class NxUserTriggerReport;
class NxUserContactReport;
class NxUserActorPairFiltering;
class NxBounds3;
class NxUserScheduler;
class NxSceneDesc;
class NxBodyDesc;
class NxShapeDesc;
class NxMaterial;
class NxMaterialDesc;
class NxUserDebugRenderer;
class NxTriangleMesh;
class NxTriangleMeshDesc;
class NxConvexMesh;
class NxConvexMeshDesc;
class NxUserOutputStream;
class NxUserAllocator;
class NxJoint;
class NxD6Joint;

class NxStream;
class NxFoundationSDK;
class NxCCDSkeleton;			//this class doesn't actually exist.
class NxSimpleTriangleMesh;
class NxHeightField;
class NxHeightFieldDesc;

class NxPhysicsSDKDesc;
class NxPhysicsSDK;
class NxMeshData;
class NxClothMesh;
class NxControllerManager;
class NxBoxController;
class NxBoxControllerDesc;
class NxCapsuleController;
class NxCapsuleControllerDesc;
class UserAllocator;
class NxRemoteDebugger;
class NxShape;
class NxContactPair;
class NxConvexShapeDesc;
class NxWheelShape;
class NxConvexShape;
class NxCapsuleShape;
class NxRaycastHit;
class NxWheelContactData;


#if NX_USE_CLOTH_API
	class NxClothMesh;
#endif
#if NX_USE_SOFTBODY_API
	class NxSoftBodyMesh;
#endif


//----------------------------------------------------------------
//
//! \brief NxU Stream
//
namespace NXU
{

	class NxActorDesc;
	class NxuPhysicsCollection;


}




#endif