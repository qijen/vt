#ifndef __P_FLUID_EMITTER_DESC_H__
#define __P_FLUID_EMITTER_DESC_H__

/** \addtogroup fluids
  @{
*/

/**
\brief Flags which control the behavior of fluid emitters.

@see pFluidEmitter
*/
enum pFluidEmitterFlag
	{
	/**
	\brief Flags whether the emitter should be visualized for debugging or not.
	*/
	PFEF_Visualization		= (1<<0),

	/**
	\brief This flag specifies whether the emission should cause a force on 
	the shapes body that the emitter is attached to.
	*/
	PFEF_ForceOnBody		= (1<<2),
	
	/**
	\brief If set, the velocity of the shapes body is added to the emitted particle velocity.
	
	This is the default behavior                         .
	*/
	PFEF_AddBodyVelocity	= (1<<3),
	
	/**
	\brief Flag to start and stop the emission. On default the emission is enabled.
	*/
	PFEF_Enabled				= (1<<4),
};

/**
\brief Flags to specify the shape of the area of emission.

Exactly one flag should be set at any time.

*/
enum pEmitterShape
{
	PFES_Rectangular		= (1<<0),
	PFES_Ellipse			= (1<<1)
};

/**
\brief Flags to specify the emitter's type of operation.
Exactly one flag should be set at any time.

@see pFluidEmitter
*/
enum pEmitterType
{
	PFET_ConstantPressure		= (1<<0),
	PFET_ConstantFlowRate	= (1<<1)
};


/**
\brief Descriptor for pFluidEmitter class.  Used for saving and loading the emitter state.
*/
class MODULE_API pFluidEmitterDesc
	{
	public:



	CK_ID entityReference;
	/**
	\brief A pointer to the shape to which the emitter is attached to.
	
	If this pointer is set to NULL, the emitter is attached to the world frame. The shape 
	must be in the same scene as the emitter. 
	*/
	CK3dEntity*				frameShape;

	/**
	\brief The emitter's mode of operation.

	Either the simulation enforces constant pressure or constant flow rate at the emission site, 
	given the velocity of emitted particles.

	@see NxEmitterType
	*/
	pEmitterType					type;

	/**
	\brief The maximum number of particles which are emitted from this emitter.
	
	If the total number of particles in the fluid already hit the maxParticles parameter of the fluid, 
	this maximal values can't be reached.
	
	If set to 0, the number of emitted particles is unrestricted.
	*/
	int					maxParticles;

	/**
	\brief The emitter's shape can either be rectangular or elliptical.

	@see pEmitterShape
	*/
	pEmitterShape					shape;

	/**
	\brief The sizes of the emitter in the directions of the first and the second axis of its orientation 
	frame (relPose).

	The dimensions are actually the radii of the size.

	*/
	float					dimensionX;
	float					dimensionY;

	/**
	\brief Random vector with values for each axis direction of the emitter orientation.
	
	The values have to be positive and describe the maximal random particle displacement in each dimension.
	
	The z value describes the randomization in emission direction. The emission direction 
	is specified by the third orientation axis of relPose.

	*/
    VxVector					randomPos;

	/**
	\brief Random angle deviation from emission direction.
	
	The emission direction is specified by the third orientation axis of relPose.
	
	<b>Unit:</b> Radians

	*/
	float					randomAngle;

	/**
	\brief The velocity magnitude of the emitted fluid particles.

	*/
	float					fluidVelocityMagnitude;

	/**
	\brief The rate specifies how many particles are emitted per second.
	
	The rate is only considered in the simulation if the type is set to PFET_ConstantFlowRate.

	@see NxEmitterType
	*/
	float					rate;
	
	/**
	\brief This specifies the time in seconds an emitted particle lives.

	If set to 0, each particle will live until it collides with a drain.
	*/
	float					particleLifetime;

	/**
	\brief Defines a factor for the impulse transfer from attached emitter to body.

	Only has an effect if PFEF_ForceOnBody is set.

	<b>Default:</b> 1.0 <br>
	<b>Range:</b> [0,inf)

	@see PFEF_ForceOnBody NxFluidEmitter.setRepulsionCoefficient()
	*/
	float					repulsionCoefficient;

	/**
	\brief A combination of pFluidEmitterFlags.

	@see pFluidEmitterFlag
	*/
	pFluidEmitterFlag					flags;

	~pFluidEmitterDesc();
	/**
	\brief (Re)sets the structure to the default.	
	*/
	void setToDefault();
	/**
	\brief Returns true if the current settings are valid
	*/
	bool isValid() const;

	/**
	\brief Constructor sets to default.
	*/
	pFluidEmitterDesc();
	};



/** @} */

#endif
