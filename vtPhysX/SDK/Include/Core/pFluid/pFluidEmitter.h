#ifndef __P_FLUIDS_FLUID_EMITTER_H__
#define __P_FLUIDS_FLUID_EMITTER_H__


/** \addtogroup fluids
  @{
*/


#include "Nxp.h"
#include "NxPhysicsSDK.h"
#include "pFluidEmitterDesc.h"

class pFluid;
class pFluidEmitterDesc;
class NxShape;

/**
\brief The fluid emitter class. It represents an emitter (fluid source) which is associated with one fluid.

The emitter is an alternative to adding particles to the fluid via the pFluid::addParticles() method.

Emission always takes place in a plane given by the orientation and position of the emitter. The 
shape of the area of emission is either a rectangle or an ellipse. The direction of emission is usually 
perpendicular to the emission plane. However, this can be randomly modulated using the setRandomAngle() 
method. An emitter can have two types of operation:
<ol>
<li>
	<i>Constant pressure.</i>
		In this case the state of the surrounding fluid is taken into account. The emitter tries 
		to match the rest spacing of the particles. Nice rays of water can be generated this way.
<li>
	<i>Constant flow rate.</i>
		In this case the emitter keeps emitting the same number of particles each frame. The rate can 
		be adjusted dynamically.
</ol>
The emitter's pose can be animated directly or attached to a shape which belongs to a 
dynamic actor. This shape is called the frame shape. When attaching an emitter to a shape, one 
has the option of enabling impulse transfer from the emitter to the body of the shape. 
The impulse generated is dependent on the rate, density, 
and velocity of the emitted particles.

*/
class MODULE_API pFluidEmitter
{

	public:
		pFluidEmitter();
		~pFluidEmitter()	{}

	/**
	\brief Sets the position of the emitter in world space.

	\param[in] vec New position in world space.

	*/
	void		setGlobalPosition(const VxVector& vec);

	/**
	\brief Sets the orientation of the emitter in world space.

	\param[in] rot New orientation in world space.
	*/
	void		setGlobalOrientation(const VxQuaternion& rot);

	/**
	\brief Returns the position of the emitter in world space.

	\return The world space position.
	*/
	VxVector		getGlobalPosition()const;

	/**
	\brief Returns the orientation of the emitter in world space.

	\return The world space orientation.

	*/
	
	VxQuaternion		getGlobalOrientation()const;

	/**
	\brief Sets the position of the emitter relative to the frameShape. 

	The pose is relative to the shape frame.

	If the frameShape is NULL, world space is used.

	\param[in] vec The new local position of the emitter.

	@see pFluidEmitterDesc.relPose
	*/
	void		setLocalPosition(const VxVector& vec)	;

	/**
	\brief Sets the orientation of the emitter relative to the frameShape. 

	The pose is relative to the shape frame.

	If the frameShape is NULL, world space is used.

	\param[in] mat The new local orientation of the emitter.

	@see pFluidEmitterDesc.relPose
	*/
	void		setLocalOrientation(const VxQuaternion& mat);

	
	/**
	\brief Returns the position of the emitter relative to the frameShape. 

	The pose is relative to the shape frame.

	If the frameShape is NULL, world space is used.

	\return The local position of the emitter.
	@see pFluidEmitterDesc.relPose
	*/

	VxVector		getLocalPosition()const;

	/**
	\brief Returns the orientation of the emitter relative to the frameShape. 

	The pose is relative to the shape frame.

	If the frameShape is NULL, world space is used.

	\return The local orientation of the emitter.

	@see pFluidEmitterDesc.relPose
	*/
	VxQuaternion		getLocalOrientation()const;

	/**
	\brief Sets the frame shape. Can be set to NULL.

	\param[in] shape The frame shape.

	@see pFluidEmitterDesc.frameShape
	*/
	void 		setFrameShape(CK3dEntity* shape);

	/**
	\brief Returns the frame shape. May be NULL.

	\return The frame shape.

	@see pFluidEmitterDesc.frameShape
	*/
	CK3dEntity * 	getFrameShape()							const;

	/**
	\brief Returns the radius of the emitter along the x axis.

	\return Radius of emitter along the X axis.
	
	@see pFluidEmitterDesc.dimensionX
	*/
	float 			getDimensionX()						const;

	/**
	\brief Returns the radius of the emitter along the y axis.

	\return Radius of emitter along the Y axis.

	@see pFluidEmitterDesc.dimensionY
	*/
	float 			getDimensionY()const;

	/**
	\brief Sets the maximal random displacement in every dimension.

	\param[in] disp The maximal random displacement of particles.
	@see pFluidEmitterDesc.randomPos
	*/
	void 			setRandomPos(VxVector disp);

	/**
	\brief Returns the maximal random displacement in every dimension.

	\return The maximal random displacement of particles.

	@see pFluidEmitterDesc.randomPos
	*/
	VxVector 			getRandomPos()const;

	/**
	\brief Sets the maximal random angle offset (in radians). 

	<b>Unit:</b> Radians

	\param[in] angle Maximum random angle for emitted particles.

	@see pFluidEmitterDesc.randomAngle
	*/
	void 			setRandomAngle(float angle);

	/**
	\brief Returns the maximal random angle offset (in radians). 

	<b>Unit:</b> Radians

	\return Maximum random angle for emitted particles.

	@see pFluidEmitterDesc.randomAngle
	*/
	float 			getRandomAngle();

	/**
	\brief Sets the velocity magnitude of the emitted particles. 

	\param[in] vel New velocity magnitude of emitted particles.

	@see pFluidEmitterDesc.fluidVelocityMagnitude
	*/
	void 			setFluidVelocityMagnitude(float vel)	;

	/**
	\brief Returns the velocity magnitude of the emitted particles.

	\return Velocity magnitude of emitted particles.

	@see pFluidEmitterDesc.fluidVelocityMagnitude
	*/
	float 			getFluidVelocityMagnitude()			const;

	/**
	\brief Sets the emission rate (particles/second).
	
	Only used if the pEmitterType is PFET_ConstantFlowRate.

	\param[in] rate New emission rate.

	@see pFluidEmitterDesc.rate
	*/
	void 			setRate(float rate);

	/**
	\brief Returns the emission rate.

	\return Emission rate.
	@see pFluidEmitterDesc.rate
	*/
	float 			getRate()							const;

	/**
	\brief Sets the particle lifetime.

	\param[in] life Lifetime of emitted particles.

	@see pFluidEmitterDesc.particleLifetime
	*/
	void 			setParticleLifetime(float life)	;

	/**
	\brief Returns the particle lifetime.

	\return Lifetime of emitted particles.

	@see pFluidEmitterDesc.particleLifetime
	*/
	float 			getParticleLifetime()				const;

	/**
	\brief Sets the repulsion coefficient.

	\param[in] coefficient The repulsion coefficient in the range from 0 to inf.

	@see pFluidEmitterDesc.repulsionCoefficient getRepulsionCoefficient()
	*/
	void				setRepulsionCoefficient(float coefficient);

	/**
	\brief Retrieves the repulsion coefficient.

	\return The repulsion coefficient.
	@see pFluidEmitterDesc.repulsionCoefficient setRepulsionCoefficient()
	*/
	float				getRepulsionCoefficient() const;

	/**
	\brief Resets the particle reservoir. 

	\param[in] new maxParticles value.
	@see pFluidEmitterDesc.maxParticles
	*/
	void				resetEmission(int maxParticles);

	/**
	\brief Returns the maximal particle number to be emitted. 

	\return max particles.
	
	@see pFluidEmitterDesc.maxParticles
	*/
	int 				getMaxParticles()				const;

	/**
	\brief Returns the number of particles that have been emitted already. 

	\return number of particles already emitted.

	*/
	int				getNbParticlesEmitted()			const;

	/**
	\brief Sets the emitter flags. 

	\param[in] flag Member of #pFluidEmitterFlag.
	\param[in] val New flag value.

	@see pFluidEmitterFlag
	*/
	void			setFlag(pFluidEmitterFlag flag, bool val);

	/**
	\brief Returns the emitter flags.

	\param[in] flag Member of #pFluidEmitterFlag.
	\return The current flag value.

	@see pFluidEmitterFlag
	*/
	bool			getFlag(pFluidEmitterFlag flag)			const;

	/**
	\brief Get the emitter shape.

	\param[in] shape Member of #pEmitterShape.
	\return True if it is of type shape.

	@see pFluidEmitterDesc.shape
	*/
	bool			getShape(pEmitterShape shape)			const;

	/**
	\brief Get the emitter type.

	\param[in] type Member of #pEmitterType
	\return True if it is of type type.

	@see pEmitterType
	*/
	bool			getType(pEmitterType type) const;

	
	NxFluidEmitter* getEmitter() const { return mEmitter; }
	void setEmitter(NxFluidEmitter* val) { mEmitter = val; }


	/**
	\brief Returns the owner fluid.

	\return The fluid this emitter is associated with.

	*/
	pFluid * getFluid() const { return mFluid; }
	
	void setFluid(pFluid * val) { mFluid = val; }
	

	CK_ID getEntityReference() const { return mEntityReference; }
	void setEntityReference(CK_ID val) { mEntityReference = val; }
	
	pFluidRenderSettings * getRenderSettings() const { return mRenderSettings; }
	void setRenderSettings(pFluidRenderSettings * val) { mRenderSettings = val; }
	

private : 

	
	NxFluidEmitter* mEmitter;


	pFluid *mFluid;

	CK_ID mEntityReference;

	pFluidRenderSettings *mRenderSettings;
	
};

/** @} */

#endif
