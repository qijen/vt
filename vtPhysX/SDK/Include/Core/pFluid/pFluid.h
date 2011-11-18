#ifndef __P_FLUID_H__
#define __P_FLUID_H__

#include "pTypes.h"
#include "NxPlane.h"
#include "pFluidFlags.h"

#include "pTypes.h"

class CK3dPointCloud;


/** \addtogroup Fluid
@{
*/

/**
\brief2 Describes an pFluidDesc.
*/
class MODULE_API pFluidDesc
{
public:

	/**
	\brief Sets the maximal number of particles for the fluid used in the simulation.

	If more particles are added directly, or more particles are emitted into the
	fluid after this limit is reached, they are simply ignored.

	*/
	int						maxParticles;

	/**
	\brief Defines the number of particles which are reserved for creation at runtime.

	If pFluidDesc.flags.PFF_PriorityMode is set the oldest particles are removed until 
	there are no more than (maxParticles - numReserveParticles) particles left. This removal
	is carried out for each simulation step, on particles which have a finite life time 
	(i.e. > 0.0). The deletion guarantees a reserve of numReserveParticles particles which 
	can be added for each simulaiton step. Note that particles which have equal lifetime can 
	get deleted at the same time. In order to avoid this, the particle lifetimes 
	can be varied randomly.

	This parameter must be smaller than pFluidDesc.maxParticles.
	*/
	int						numReserveParticles;

	/**
	\brief The particle resolution given as particles per linear meter measured when the fluid is
	in its rest state (relaxed).

	Even if the particle system is simulated without particle interactions, this parameter defines the 
	emission density of the emitters.

	*/
	float						restParticlesPerMeter;

	/**
	\brief Target density for the fluid (water is about 1000).

	Even if the particle system is simulated without particle interactions, this parameter defines 
	indirectly in combination with restParticlesPerMeter the mass of one particle 
	( mass = restDensity/(restParticlesPerMeter^3) ).

	The particle mass has an impact on the repulsion effect on emitters and actors.

	*/
	float						restDensity;

	/**
	\brief Radius of sphere of influence for particle interaction.

	This parameter is relative to the rest spacing of the particles, which is defined by the parameter
	restParticlesPerMeter

	( radius = kernelRadiusMultiplier/restParticlesPerMeter ).

	This parameter should be set around 2.0 and definitely below 2.5 for optimal performance and simulation quality.

	@see restParticlesPerMeter
	*/
	float						kernelRadiusMultiplier;

	/**
	\brief Maximal distance a particle is allowed to travel within one timestep.

	This parameter is relative to the rest spacing of the particles, which is defined by the parameter
	restParticlesPerMeter:

	( maximal travel distance = motionLimitMultiplier/restParticlesPerMeter ).

	Default value is 3.6 (i.e., 3.0 * kernelRadiusMultiplier).

	The value must not be higher than the product of packetSizeMultiplier and kernelRadiusMultiplier.

	@see restParticlesPerMeter
	*/
	float						motionLimitMultiplier;

	/**
	\brief Defines the distance between particles and collision geometry, which is maintained during simulation.

	For the actual distance, this parameter is divided by the rest spacing of the particles, which is defined by the parameter
	restParticlesPerMeter:

	( distance = collisionDistanceMultiplier/restParticlesPerMeter ).

	It has to be positive and not higher than packetSizeMultiplier*kernelRadiusMultiplier.
	Default value is 0.12 (i.e., 0.1 * kernelRadiusMultiplier).

	@see restParticlesPerMeter, kernelRadiusMultiplier
	*/
	float						collisionDistanceMultiplier;

	/**
	\brief This parameter controls the parallelization of the fluid.

	The spatial domain is divided into so called packets, equal sized cubes, aligned in a grid.

	The parameter given defines the edge length of such a packet. This parameter is relative to the interaction 
	radius of the particles, given as kernelRadiusMultiplier/restParticlesPerMeter.

	It has to be a power of two, no less than 4.

	*/
	int						packetSizeMultiplier;

	/**
	\brief The stiffness of the particle interaction related to the pressure.

	This factor linearly scales the force which acts on particles which are closer to each other than 
	the rest spacing.

	Setting	this parameter appropriately is crucial for the simulation.  The right value depends on many factors
	such as viscosity, damping, and kernelRadiusMultiplier.  Values which are too high will result in an
	unstable simulation, whereas too low values will make the fluid appear "springy" (the fluid
	acts more compressible).

	Must be positive.

	*/
	float						stiffness;

	/**
	\brief 	The viscosity of the fluid defines its viscous behavior.

	Higher values will result in a honey-like behavior.  Viscosity is an effect which depends on the 
	relative velocity of neighboring particles; it reduces the magnitude of the relative velocity.

	Must be positive.

	*/
	float						viscosity;

	/**
	\brief 	The surfaceTension of the fluid defines an attractive force between particles

	Higher values will result in smoother surfaces.
	Must be nonnegative.

	*/
	float						surfaceTension;

	/**
	\brief Velocity damping constant, which is globally applied to each particle.

	It generally reduces the velocity of the particles. Setting the damping to 0 will leave the 
	particles unaffected.

	Must be nonnegative.

	*/
	float						damping;

	/**
	\brief Defines a timespan for the particle "fade-in".

	This feature is experimental. When a particle is created it has no influence on the simulation. 
	It takes fadeInTime until the particle has full influence. If set to zero, the particle has full 
	influence on the simulation from start.

	<b>Default:</b> 0.0 <br>
	<b>Range:</b> [0,inf)
	*/
	float						fadeInTime;

	/**
	\brief Acceleration (m/s^2) applied to all particles at all time steps.

	Useful to simulate smoke or fire.
	This acceleration is additive to the scene gravity. The scene gravity can be turned off 
	for the fluid, using the flag PFF_DisableGravity.

	@see pFluid.getExternalAcceleration() pFluid.setExternalAcceleration()
	*/
	VxVector						externalAcceleration;

	/**
	\brief Defines the plane the fluid particles are projected to. This parameter is only used if
	PFF_ProjectToPlane is set.

	<b>Default:</b> XY plane

	@see PFF_ProjectToPlane pFluid.getProjectionPlane() pFluid.setProjectionPlane()
	*/
	NxPlane						projectionPlane;

	/**
	\brief Defines the restitution coefficient used for collisions of the fluid particles with static shapes.

	Must be between 0 and 1.

	A value of 0 causes the colliding particle to get a zero velocity component in the
	direction of the surface normal of the static shape at the collision location; i.e.
	it will not bounce.

	A value of 1 causes a particle's velocity component in the direction of the surface normal to invert;
	i.e. the particle bounces off the surface with the same velocity magnitude as it had before collision. 
	(Caution: values near 1 may have a negative impact on stability)

	*/
	float						restitutionForStaticShapes;

	/**
	\brief Defines the dynamic friction of the fluid regarding the surface of a static shape.

	Must be between 0 and 1.

	A value of 1 will cause the particle to lose its velocity tangential to
	the surface normal of the shape at the collision location; i.e. it will not slide
	along the surface.

	A value of 0 will preserve the particle's velocity in the tangential surface
	direction; i.e. it will slide without resistance on the surface.

	*/
	float						dynamicFrictionForStaticShapes;

	/**
	\brief Defines the static friction of the fluid regarding the surface of a static shape.

	This feature is currently unimplemented! 

	*/
	float						staticFrictionForStaticShapes;

	/**
	\brief Defines the strength of attraction between the particles and static rigid bodies on collision. 

	This feature is currently unimplemented! 

	*/
	float						attractionForStaticShapes;

	/**
	\brief Defines the restitution coefficient used for collisions of the fluid particles with dynamic shapes.

	Must be between 0 and 1.

	(Caution: values near 1 may have a negative impact on stability)

	@see restitutionForStaticShapes
	*/
	float						restitutionForDynamicShapes;

	/**
	\brief Defines the dynamic friction of the fluid regarding the surface of a dynamic shape.

	Must be between 0 and 1.

	@see dynamicFrictionForStaticShapes
	*/
	float						dynamicFrictionForDynamicShapes;

	/**
	\brief Defines the static friction of the fluid regarding the surface of a dynamic shape.

	This feature is currently unimplemented! 

	*/
	float						staticFrictionForDynamicShapes;

	/**
	\brief Defines the strength of attraction between the particles and the dynamic rigid bodies on collision. 

	This feature is currently unimplemented! 

	*/
	float						attractionForDynamicShapes;

	/**
	\brief Defines a factor for the impulse transfer from fluid to colliding rigid bodies.

	Only has an effect if PFF_CollisionTwoway is set.

	<b>Default:</b> 0.2 <br>
	<b>Range:</b> [0,inf)

	@see PFF_CollisionTwoway pFluid.setCollisionResponseCoefficient()
	*/
	float						collisionResponseCoefficient;

	/**
	\brief pFluidSimulationMethod flags. Defines whether or not particle interactions are considered 
	in the simulation.

	@see pFluidSimulationMethod
	*/
	pFluidSimulationMethod						simulationMethod;

	/**
	\brief pFluidCollisionMethod flags. Selects whether static collision and/or dynamic collision 
	with the environment is performed.

	@see pFluidCollisionMethod
	*/
	pFluidCollisionMethod						collisionMethod;

	/**
	\brief Sets which collision group this fluid is part of.

	<b>Default:</b> 0

	pFluid.setCollisionGroup()
	*/
	int collisionGroup;


	/**
	\brief Sets the 128-bit mask used for collision filtering.

	<b>Default:</b> 0

	@see NxGroupsMask pFluid.setGroupsMask() pFluid.getGroupsMask()
	*/
	NxGroupsMask groupsMask;

	/**
	\brief Flags defining certain properties of the fluid.

	@see pFluidFlag
	*/
	unsigned int						flags;

	void*						userData;	//!< Will be copied to NxFluid::userData
	const char*					name;		//!< Possible debug name. The string is not copied by the SDK, only the pointer is stored.

	/**
	\brief Constructor sets to default.

	*/
	pFluidDesc();	
	/**
	\brief (Re)sets the structure to the default.	

	*/
	void setToDefault();
	/**
	\brief Returns true if the current settings are valid

	*/
	bool isValid() const;


	/**
	\brief Retrieve the fluid desc type.

	\return The fluid desc type. See #pFluidDescType
	*/
	xU16 getType() const;

	CK_ID worldReference;

protected:

	NxFluidDescType			type;

};


struct pParticle
{
	NxVec3	position;
	NxVec3  velocity;
	float	density;
	float  lifetime;
	unsigned int	id;
	NxVec3	collisionNormal;
};

/**
\brief The fluid class represents the main module for the particle based fluid simulation.
SPH (Smoothed Particle Hydrodynamics) is used to animate the particles.

There are two kinds of particle interaction forces which govern the behavior of the fluid:
<ol>
<li>
	Pressure forces: These forces result from particle densities higher than the
	"rest density" of the fluid.  The rest density is given by specifying the inter-particle
	distance at which the fluid is in its relaxed state.  Particles which are closer than
	the rest spacing are pushed away from each other.
<li>
	Viscosity forces:  These forces act on neighboring particles depending on the difference
	of their velocities.  Particles drag other particles with them which is used to simulate the
	viscous behaviour of the fluid.
</ol>
	The fluid class manages a set of particles.
	Particles can be created in two ways:
<ol>
<li>
	Particles can be added by the user directly.
<li>
	The user can add emitters to the fluid and configure the parameters of the emission.
	(See pFluidEmitter)
</ol>
	Particles can be removed in two ways as well:
<ol>
<li>
	The user can specify a lifetime for the particles.  When its lifetime expires, a particle is deleted.
<li>
	Shapes can be configured to act as drains.  When a particle intersects with a drain, the particle is deleted.
	(See pShapeFlag)
</ol>

Particles collide with static and dynamic shapes. Particles are also affected by the scene gravity and a user force, 
as well as global velocity damping.  In order to render a fluid, the user can supply the fluid instance with a 
user buffer into which the particle state is written after each simuation step.

For a good introduction to SPH fluid simulation,
see http://graphics.ethz.ch/~mattmuel/publications/sca03.pdf

@see pFluidDesc, pFluidEmitter, pFluidEmitterDesc, pShapeFlag
*/
class pFluid
{
	public:
		pFluid(NxFluidDesc &desc, bool trackUserData, bool provideCollisionNormals, const VxVector& color,  float particleSize);
		~pFluid();

		NxFluid*				getNxFluid() { return mFluid; }

		pParticle*		getParticles() { return mParticleBuffer; }
		unsigned				getParticlesNum() { return mParticleBufferNum; }

		const unsigned*			getCreatedIds() { return mCreatedParticleIds; }
		unsigned				getCreatedIdsNum() { return mCreatedParticleIdsNum; }

		const unsigned*			getDeletedIds() { return mDeletedParticleIds; }
		unsigned				getDeletedIdsNum() { return mDeletedParticleIdsNum; }

		void					setParticleSize(float size) { mParticleSize = size; }
		float					getParticleSize() { return mParticleSize; }

		void draw();
		
		NxFluid* getFluid() const { return mFluid; }
		void setFluid(NxFluid* val) { mFluid = val; }
		
		CK_ID getEntityID() const { return entityID; }
		void setEntityID(CK_ID val) { entityID = val; }
		void updateVirtoolsMesh();

		CK3dEntity *getParticleObject();

		pParticle*		mParticleBuffer;


		/************************************************************************/
		/* emitter operations :																												*/
		/************************************************************************/

		/**
		\brief Creates an emitter for this fluid.

		pFluidEmitterDesc::isValid() must return true.

		\param desc The fluid emitter descriptor. See #pFluidEmitterDesc.
		\return The new fluid.

		@see pFluidEmitter
		*/
		pFluidEmitter*		createEmitter(const pFluidEmitterDesc& desc);

		/**
		\brief Deletes the specified emitter.

		The emitter must belong to this fluid. Do not keep a reference to the deleted instance.
		Avoid release calls while the scene is simulating (in between simulate() and fetchResults() calls).

		\param emitter The emitter to release.

		*/
		void				releaseEmitter(pFluidEmitter& emitter);

		/**
		\brief Returns the number of emitters.

		\return The number of emitters.
		*/
		int getNbEmitters()const;



		CK3dPointCloud* mPointCloud;
		
		CK3dPointCloud* getPointCloud() const { return mPointCloud; }
		void setPointCloud(CK3dPointCloud* val) { mPointCloud = val; }

		void updateCloud();





private:

	unsigned			mParticleBufferNum;
	

	NxFluid*			mFluid;
	
	VxVector				mParticleColor;
	float				mParticleSize;
	unsigned int				mMaxParticles;
	
	CK_ID	entityID;
	

	/**
	These fields are only relevant for tracking user particle data (MyParticle)
	*/
	bool				mTrackUserData;
	pFluid*			mMyParticleBuffer;
	unsigned int				mCreatedParticleIdsNum;
	unsigned int*				mCreatedParticleIds;
	unsigned int				mDeletedParticleIdsNum;
	unsigned int*				mDeletedParticleIds;

	//rendering
	float*				mRenderBuffer;
	float*				mRenderBufferUserData;

	protected:
	private:
};


/** @} */

#endif
