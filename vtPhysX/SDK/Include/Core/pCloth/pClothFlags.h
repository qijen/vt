#include "pTypes.h"


/** \addtogroup Cloth
@{
*/
//! Collection of flags describing the behavior of a cloth object.
enum pClothFlag
{
	/**
	\brief Enable/disable pressure simulation. 
		Note: Pressure simulation only produces meaningful results for closed meshes.
	*/
	PCF_Pressure			  = (1<<0),

	/**
	\brief Makes the cloth static. 
	*/
	PCF_Static = (1<<1),
	/**
	\brief 
	PCF_Static			  = (1<<1),/*!<Makes the cloth static. */
	/**
	\brief Disable collision handling with the rigid body scene. 
	@see pClothDesc.collisionResponseCoefficient
	*/
	PCF_DisableCollision  = (1<<2),

	/**
	\brief Enable/disable self-collision handling within a single piece of cloth.


	@see PCF_triangle_collision
	*/
	PCF_SelfCollision	  = (1<<3),
	/**
	\brief Enable/disable gravity. If off, the cloth is not subject to the gravitational force
	of the rigid body scene.
	*/
	PCF_Gravity            = (1<<5),

	/**
	\brief Enable/disable bending resistance. Select the bending resistance through 
	pClothDesc.bendingStiffness. Two bending modes can be selected via the PCF_BendingOrtho flag.


	@see pClothDesc.bendingStiffness PCF_BendingOrtho
	*/
	PCF_Bending            = (1<<6),

	/**
	\brief Enable/disable orthogonal bending resistance. This flag has an effect only if
	PCF_Bending is set. If PCF_BendingOrtho is not set, bending is modeled via an
	additional distance constraint. This mode is fast but not independent of stretching
	resistance. If PCF_BendingOrtho is set, bending is modeled via an angular spring
	between adjacent triangles. This mode is slower but independent of stretching resistance.


	@see pClothDesc.bendingStiffness PCF_Bending
	*/
	PCF_BendingOrtho      = (1<<7),

	/**
	\brief Enable/disable damping of internal velocities. Use pClothDesc.dampingCoefficient
	to control damping.


	@see pClothDesc.dampingCoefficient
	*/
	PCF_Damping            = (1<<8),

	/**
	\brief Enable/disable two way collision of cloth with the rigid body scene.
	
	In either case, cloth is influenced by colliding rigid bodies.
	If PCF_CollisionTwoway is not set, rigid bodies are not influenced by 
	colliding pieces of cloth. Use pClothDesc.collisionResponseCoefficient to
	control the strength of the feedback force on rigid bodies.

	When using two way interaction care should be taken when setting the density of the attached objects.
	For example if an object with a very low or high density is attached to a cloth then the simulation 
	may behave poorly. This is because impulses are only transfered between the cloth and rigid body solver
	outside the solvers.

	Two way interaction works best when NX_SF_SEQUENTIAL_PRIMARY is set in the primary scene. If not set,
	collision and attachment artifacts may happen.


	@see pClothDesc.collisionResponseCoefficient
	*/
	PCF_CollisionTwoway   = (1<<9),

	/**
	Not supported in current release.
	\brief Enable/disable collision detection of cloth triangles against the scene.
	If PCF_TriangleCollision is not set, only collisions of cloth particles are detected.
	If PCF_TriangleCollision is set, collisions of cloth triangles are detected as well.

	*/
	PCF_TriangleCollision = (1<<11),

	/**
	\brief Defines whether the cloth is tearable. 
	
	Note: Make sure meshData.maxVertices and the corresponding buffers
	in meshData are substantially larger (e.g. 2x) then the number 
	of original vertices since tearing will generate new vertices.
	When the buffer cannot hold the new vertices anymore, tearing stops.

	Note: For tearing, make sure you cook the mesh with	the flag
	NX_CLOTH_MESH_TEARABLE set in the NxClothMeshDesc.flags.


	@see pClothDesc.tearFactor 
	*/
	PCF_Tearable           = (1<<12),

	/**
	\brief Defines whether this cloth is simulated on the PPU.
	
	To simulate a piece of cloth on the PPU
	set this flag and create the cloth in a regular software scene.
	Note: only use this flag during creation, do not change it using NxCloth.setFlags().
	*/
	PCF_Hardware           = (1<<13),

	/**
	\brief Enable/disable center of mass damping of internal velocities.

	This flag only has an effect if the flag PCF_Damping is set. If set, 
	the global rigid body modes (translation and rotation) are extracted from damping. 
	This way, the cloth can freely move and rotate even under high damping. 
	Use pClothDesc.dampingCoefficient to control damping. 


	@see pClothDesc.dampingCoefficient
	*/
	PCF_ComDamping		  = (1<<14),

	/**
	\brief If the flag PCF_ValidBounds is set, cloth particles outside the volume
	defined by pClothDesc.validBounds are automatically removed from the simulation. 


	@see pClothDesc.validBounds
	*/
	PCF_ValidBounds		  = (1<<15),

	/**
	\brief Enable/disable collision handling between this cloth and fluids.

	Note: With the current implementation, do not switch on fluid collision for
	many cloths. Create scenes with a few large pieces because the memory usage
	increases linearly with the number of cloths.
	The performance of the collision detection is dependent on both, the thickness
	and the particle radius of the fluid so tuning these parameters might improve
	the performance significantly.

	Note: The current implementation does not obey the pWorld::setGroupCollisionFlag
	settings. If PCF_FluidCollision is set, collisions will take place even if
	collisions between the groups that the corresponding cloth and fluid belong to are
	disabled.


	@see pClothDesc.toFluidResponseCoefficient pClothDesc.fromFluidResponseCoefficient
	*/
	PCF_FluidCollision    = (1<<16),

	/**
	\brief Disable continuous collision detection with dynamic actors. 
	Dynamic actors are handled as static ones.

	*/
	PCF_DisableDynamicCCD  = (1<<17),

	/**
	\brief Moves cloth partially in the frame of the attached actor. 

	This feature is useful when the cloth is attached to a fast moving character.
	In that case the cloth adheres to the shape it is attached to while only 
	velocities below the parameter minAdhereVelocity are used for secondary effects.


	@see pClothDesc.minAdhereVelocity
	*/
	PCF_AddHere  = (1<<18),

	/**
	\brief Invokes #attachToShape() automatically during the clothes creation procedure.

	This is only to be used when the entity has a physic attribute attached whereas it has to have 
	the BF_SubShape flags enabled and the entity must be in the hierarchy of the rigid body.
	*/
	PCF_AttachToParentMainShape  = (1<<19),

	/**
	\brief Invokes #attachToCollidingShapes() automatically during the clothes creation procedure.
	*/
	PCF_AttachToCollidingShapes  = (1<<20),
	/**
	\brief Invokes #attachToCore() automatically during the clothes creation procedure.
	*/
	PCF_AttachToCore  = (1<<21),
	/**
	\brief Attaches a cloth attribute after the clothes creation.
	*/
	PCF_AttachAttributes  = (1<<22),

};

/**
\brief Cloth attachment flags.

@see pCloth.attachToShape() pCloth.attachToCollidingShapes() pCloth.attachVertexToShape()
*/

enum pClothAttachmentFlag
{
	/**
	\brief The default is only object->cloth interaction (one way).

	With this flag set, cloth->object interaction is turned on as well.
	*/
	PCAF_ClothAttachmentTwoway			  = (1<<0),

	/**
	\brief When this flag is set, the attachment is tearable.

	\note If the cloth is attached to a dynamic shape using two way interaction
	half way torn attachments can generate unexpected impluses on the shape.
	To prevent this, only attach one row of cloth vertices to the shape.
	@see pClothDesc.attachmentTearFactor
	*/
	PCAF_ClothAttachmentTearable		  = (1<<1),
};

/** @} */