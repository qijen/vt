#ifndef __P_SDKP_SDK_PARAMETER_H__
#define __P_SDKP_SDK_PARAMETER_H__




/** \addtogroup PhysicSDK
@{
*/

/**
\brief Parameters enums to be used as the 1st arg to setParameter or getParameter.

@see PhysicManager.setParameter() PhysicManager.getParameter()
*/
typedef enum pSDKParameter
{
	
	/* RigidBody-related parameters  */


	/**
	\brief Default value for pShape::skinWidth.

	<b>Range:</b> [0, inf)<br>
	<b>Default:</b> 0.025<br>
	<b>Unit:</b> distance.

	@see pShape.setSkinWidth
	*/
	pSDKP_SkinWidth = 1,


	/**
	\brief The default linear velocity, squared, below which objects start going to sleep. 
	Note: Only makes sense when the pSDKP_BF_ENERGY_SLEEP_TEST is not set.

	<b>Range:</b> [0, inf)<br>
	<b>Default:</b> (0.15*0.15)

	*/
	pSDKP_DefaultSleepLinVelSquared = 2,

	/**
	\brief The default angular velocity, squared, below which objects start going to sleep. 
	Note: Only makes sense when the pSDKP_BF_ENERGY_SLEEP_TEST is not set.

	<b>Range:</b> [0, inf)<br>
	<b>Default:</b> (0.14*0.14)
	
	*/
	pSDKP_DefaultSleepAngVelSquared = 3,
	/**
	\brief A contact with a relative velocity below this will not bounce.	

	<b>Range:</b> (-inf, 0]<br>
	<b>Default:</b> -2

	@see pMaterial
	*/
	pSDKP_BounceThreshold = 4,

	/**
	\brief This lets the user scale the magnitude of the dynamic friction applied to all objects.	

	<b>Range:</b> [0, inf)<br>
	<b>Default:</b> 1

	@see pMaterial
	*/
	pSDKP_DynFrictScaling = 5,

	/**
	\brief This lets the user scale the magnitude of the static friction applied to all objects.

	<b>Range:</b> [0, inf)<br>
	<b>Default:</b> 1
	

	@see pMaterial
	*/
	pSDKP_StaFrictionScaling = 6,

	/**
	\brief See the comment for pRigidBody::setMaxAngularVelocity() for details.

	<b>Range:</b> [0, inf)<br>
	<b>Default:</b> 7

	@see pRigidBody.setMaxAngularVelocity()
	*/
	pSDKP_MaxAngularVelocity = 7,

	/* Collision-related parameters:  */

	/**
	\brief Enable/disable continuous collision detection (0.0f to disable)

	<b>Range:</b> [0, inf)<br>
	<b>Default:</b> 0.0

	@see NxPhysicsSDK.createCCDSkeleton()
	*/
	pSDKP_ContinuousCD = 8,

	

	/* General parameters and new parameters */

	/**
	\brief Used to enable adaptive forces to accelerate convergence of the solver.

	<b>Range:</b> [0, inf)<br>
	<b>Default:</b> 1.0
	*/
	pSDKP_AdaptiveForce = 68,

	/**
	\brief Controls default filtering for jointed bodies. True means collision is disabled.

	<b>Range:</b> {true, false}<br>
	<b>Default:</b> true

	@see pSDKP_JF_CollisionEnabled
	
	*/
	pSDKP_CollVetoJointed = 69,

	/**
	\brief Controls whether two touching triggers generate a callback or not.

	<b>Range:</b> {true, false}<br>
	<b>Default:</b> true

	@see NxUserTriggerReport
	*/
	pSDKP_TriggerTriggerCallback = 70,
	pSDKP_SelectHW_Algo = 71,

	/**
	\brief Distance epsilon for the CCD algorithm.  

	<b>Range:</b> [0, inf)<br>
	<b>Default:</b> 0.01

	*/
	pSDKP_CCDEpsilon = 73,

	/**
	\brief Used to accelerate solver.

	<b>Range:</b> [0, inf)<br>
	<b>Default:</b> 0
	
	*/
	pSDKP_SolverConvergenceThreshold = 74,

	/**
	\brief Used to accelerate HW Broad Phase.

	<b>Range:</b> [0, inf)<br>
	<b>Default:</b> 0.001

	*/
	pSDKP_BBoxNoiseLevel = 75,

	/**
	\brief Used to set the sweep cache size.

	<b>Range:</b> [0, inf)<br>
	<b>Default:</b> 5.0

	*/
	pSDKP_ImplicitSweepCacheSize = 76,

	/**
	\brief The default sleep energy threshold. Objects with an energy below this threshold are allowed
	to go to sleep. 
	Note: Only used when the pSDKP_BF_ENERGY_SLEEP_TEST flag is set.

	<b>Range:</b> [0, inf)<br>
	<b>Default:</b> 0.005

	*/
	pSDKP_DefaultSleepEnergy = 77,

	/**
	\brief Constant for the maximum number of packets per fluid. Used to compute the fluid packet buffer size in NxFluidPacketData.

	<b>Range:</b> [925, 925]<br>
	<b>Default:</b> 925

	@see NxFluidPacketData
	*/
	pSDKP_ConstantFluidMaxPackets = 78,

	/**
	\brief Constant for the maximum number of new fluid particles per frame.
	<b>Range:</b> [4096, 4096]<br>
	<b>Default:</b> 4096

	*/
	pSDKP_ConstantFluidMaxParticlesPerStep = 79,

	/**
	\brief [Experimental] Disables scene locks when creating/releasing meshes.

	Prevents the SDK from locking all scenes when creating and releasing triangle meshes, convex meshes, height field 
	meshes, softbody meshes and cloth meshes, which is the default behavior. Can be used to improve parallelism but beware
	of possible side effects.

	\warning Experimental feature.
	*/
	pSDKP_AsynchronousMeshCreation = 96,

	/**
	\brief Epsilon for custom force field kernels.

	This epsilon is used in custom force field kernels (NxSwTarget). Methods like recip()
	or recipSqrt() evaluate to zero if their input is smaller than this epsilon.

	<br>
	*/
	pSDKP_ForceFieldCustomKernelEpsilon = 97,

	/**
	\brief Enable/disable improved spring solver for joints and wheelshapes

	This parameter allows to enable/disable an improved version of the spring solver for joints and wheelshapes.

	\warning 
	The parameter is introduced for legacy purposes only and will be removed in future versions (such that
	the improved spring solver will always be used).

	\note 
	Changing the parameter will only affect newly created scenes but not existing ones.

	<b>Range:</b> {0: disabled, 1: enabled}<br>
	<b>Default:</b> 1

	*/
	pSDKP_ImprovedSpringSolver = 98,

	/**
	\brief This is not a parameter, it just records the current number of parameters (as max(NxParameter)+1) for use in loops.
	When a new parameter is added this number should be assigned to it and then incremented.
	*/
	pSDKP_PARAMS_NUM_VALUES = 99,

	pSDKP_PARAMS_FORCE_DWORD = 0x7fffffff
};
/** @} */

#endif