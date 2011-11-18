#ifndef __P_FLUID_FLAGS_
#define __P_FLUID_FLAGS_



/** \addtogroup Fluid
@{
*/

/**
\brief Fluid flags
*/
enum pFluidFlag
{
	/**
	\brief Enables debug visualization for the NxFluid.
	*/
	PFF_VISUALIZATION							= (1<<0),

	/**
	\brief Disables scene gravity for the NxFluid.
	*/
	PFF_DisableGravity						= (1<<1),

	/**
	\brief Enable/disable two way collision of fluid with the rigid body scene.
	In either case, fluid is influenced by colliding rigid bodies.
	If PFF_CollisionTwoway is not set, rigid bodies are not influenced by 
	colliding pieces of fluid. Use pFluidDesc.collisionResponseCoefficient to
	control the strength of the feedback force on rigid bodies.

	@see NxFluidDesc.collisionResponseCoefficient
	*/
	PFF_CollisionTwoway						= (1<<2),


	/**
	\brief Enable/disable execution of fluid simulation.
	*/
	PFF_Enabled								= (1<<3),

	/**
	\brief Defines whether this fluid is simulated on the PPU.
	*/
	PFF_Hardware								= (1<<4),

	/**
	\brief Enable/disable particle priority mode. 
	If enabled, the oldest particles are deleted to keep a certain budget for 
	new particles. Note that particles which have equal lifetime can get deleted 
	at the same time. In order to avoid this, the particle lifetimes 
	can be varied randomly.

	@see pFluidDesc.numReserveParticles
	*/
	PFF_PriorityMode							= (1<<5),

	/**
	\brief Defines whether the particles of this fluid should be projected to a plane.
	This can be used to build 2D fluid applications, for instance. The projection
	plane is defined by the parameter pFluidDesc.projectionPlane.

	@see pFluidDesc.projectionPlane
	*/
	PFF_ProjectToPlane						= (1<<6),

	/**
	\brief Forces fluid static mesh cooking format to parameters given by the fluid descriptor.

	Currently not implemented!
	*/
	PFF_ForceStrictCookingFormat			= (1<<7),

};

/**
\brief Describes the particle simulation method

Particles can be treated in two ways: either they are simulated considering
interparticular forces (SPH), or they are simulated independently.
In the latter case (with the simulation method set to PFS_NO_PARTICLE_INTERACTION),
you still get collision between particles and static/dynamic shapes, damping,
acceleration due to gravity, and the user force.
*/
enum pFluidSimulationMethod
{
	/**
	\brief Enable simulation of inter particle forces.
	*/
	PFS_SPH						= (1<<0),	

	/**
	\brief Do not simulate inter particle forces.
	*/
	PFS_NoParticleInteraction	= (1<<1),

	/**
	\brief Alternate between SPH and simple particles
	*/
	PFS_MixedMode					= (1<<2),
};

/**
\brief The fluid collision method
*/
enum pFluidCollisionMethod
{
	/**
	\brief collide with static objects
	*/
	PFCM_Static                    = (1<<0),
	/**
	\brief collide with dynamic objects
	*/
	PFCM_Dynamic                   = (1<<1),
};

/** @} */


#endif