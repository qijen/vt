#ifndef __PCLOTHTYPES_H__
#define __PCLOTHTYPES_H__


#include "pClothFlags.h"


/** \addtogroup Cloth
@{
*/

/**
\brief Descriptor class for pCloth.
*/
class MODULE_API pClothDesc
{
public:

	/**
	\brief Thickness of the cloth.

	The thickness is usually a fraction of the overall extent of the cloth and
	should not be set to a value greater than that.	A good value is the maximal
	distance between two adjacent cloth particles in their rest pose. Visual
	artifacts or collision problems may appear if the thickness is too small.

	<b>Default:</b> 0.01 <br>
	<b>Range:</b> [0,inf)

	@see pCloth.setThickness()
	*/
	float thickness;

	/**
	\brief Density of the cloth (mass per area).

	<b>Default:</b> 1.0 <br>
	<b>Range:</b> (0,inf)
	*/
	float density;

	/**
	\brief Bending stiffness of the cloth in the range 0 to 1.

	Only has an effect if the flag PCF_Bending is set.

	<b>Default:</b> 1.0 <br>
	<b>Range:</b> [0,1]

	@see PCF_Bending pCloth.setBendingStiffness()
	*/
	float bendingStiffness;

	/**
	\brief Stretching stiffness of the cloth in the range 0 to 1.

	Note: stretching stiffness must be larger than 0.

	<b>Default:</b> 1.0 <br>
	<b>Range:</b> (0,1]

	@see pCloth.setStretchingStiffness()
	*/
	float stretchingStiffness;

	/**
	\brief Spring damping of the cloth in the range 0 to 1.

	Only has an effect if the flag PCF_Damping is set.

	<b>Default:</b> 0.5 <br>
	<b>Range:</b> [0,1]

	@see PCF_Damping pCloth.setDampingCoefficient()
	*/
	float dampingCoefficient;

	/**
	\brief Friction coefficient in the range 0 to 1. 

	Defines the damping of the velocities of cloth particles that are in contact.

	<b>Default:</b> 0.5 <br>
	<b>Range:</b> [0,1]

	@see pCloth.setFriction()
	*/
	float friction;

	/**
	\brief If the flag PCF_Pressure is set, this variable defines the volume
	of air inside the mesh as volume = pressure * restVolume. 

	For pressure < 1 the mesh contracts w.r.t. the rest shape
	For pressure > 1 the mesh expands w.r.t. the rest shape

	<b>Default:</b> 1.0 <br>
	<b>Range:</b> [0,inf)

	@see PCF_pressure pCloth.setPressure()
	*/
	float pressure;

	/**
	\brief If the flag PCF_Tearable is set, this variable defines the 
	elongation factor that causes the cloth to tear. 

	Must be larger than 1.
	Make sure meshData.maxVertices and the corresponding buffers
	in meshData are substantially larger (e.g. 2x) than the number 
	of original vertices since tearing will generate new vertices.

	When the buffer cannot hold the new vertices anymore, tearing stops.

	<b>Default:</b> 1.5 <br>
	<b>Range:</b> (1,inf)

	@see pCloth.setTearFactor()
	*/
	float tearFactor;

	/**
	\brief Defines a factor for the impulse transfer from cloth to colliding rigid bodies.

	Only has an effect if PCF_CollisionTwoWay is set.

	<b>Default:</b> 0.2 <br>
	<b>Range:</b> [0,inf)

	@see PCF_CollisionTwoWay pCloth.setCollisionResponseCoefficient()
	*/
	float collisionResponseCoefficient;

	/**
	\brief Defines a factor for the impulse transfer from cloth to attached rigid bodies.

	Only has an effect if the mode of the attachment is set to nx_cloth_attachment_twoway.

	<b>Default:</b> 0.2 <br>
	<b>Range:</b> [0,1]

	@see pCloth.attachToShape pCloth.attachToCollidingShapes pCloth.attachVertexToShape pCloth.setAttachmentResponseCoefficient()
	*/
	float attachmentResponseCoefficient;

	/**
	\brief If the flag nx_cloth_attachment_tearable is set in the attachment method of pCloth, 
	this variable defines the elongation factor that causes the attachment to tear. 

	Must be larger than 1.

	<b>Default:</b> 1.5 <br>
	<b>Range:</b> (1,inf)

	@see pCloth.setAttachmentTearFactor() pCloth.attachToShape pCloth.attachToCollidingShapes pCloth.attachVertexToShape

	*/
	float attachmentTearFactor;

	/**
	\brief Defines a factor for the impulse transfer from this cloth to colliding fluids.

	Only has an effect if the PCF_FluidCollision flag is set.

	<b>Default:</b> 1.0 <br>
	<b>Range:</b> [0,inf)

	Note: Large values can cause instabilities

	@see pClothDesc.flags pClothDesc.fromFluidResponseCoefficient
	*/
	float toFluidResponseCoefficient;

	/**
	\brief Defines a factor for the impulse transfer from colliding fluids to this cloth.

	Only has an effect if the PCF_FluidCollision flag is set.

	<b>Default:</b> 1.0 <br>
	<b>Range:</b> [0,inf)

	Note: Large values can cause instabilities

	@see pClothDesc.flags pClothDesc.ToFluidResponseCoefficient
	*/
	float fromFluidResponseCoefficient;

	/**
	\brief If the PCF_Adhere flag is set the cloth moves partially in the frame 
	of the attached actor. 

	This feature is useful when the cloth is attached to a fast moving character.
	In that case the cloth adheres to the shape it is attached to while only 
	velocities below the parameter minAdhereVelocity are used for secondary effects.

	<b>Default:</b> 1.0
	<b>Range:</b> [0,inf)

	@see PCF_ADHERE
	*/ 

	float minAdhereVelocity;

	/**
	\brief Number of solver iterations. 

	Note: Small numbers make the simulation faster while the cloth gets less stiff.

	<b>Default:</b> 5
	<b>Range:</b> [1,inf)

	@see pCloth.setSolverIterations()
	*/ 

	unsigned int  solverIterations;

	/**
	\brief External acceleration which affects all non attached particles of the cloth. 

	<b>Default:</b> (0,0,0)

	@see pCloth.setExternalAcceleration()
	*/ 
	VxVector externalAcceleration;

	/**
	\brief Acceleration which acts normal to the cloth surface at each vertex.

	<b>Default:</b> (0,0,0)

	@see pCloth.setWindAcceleration()
	*/ 
	VxVector windAcceleration;

	/**
	\brief The cloth wake up counter.

	<b>Range:</b> [0,inf)<br>
	<b>Default:</b> 20.0f*0.02f

	@see pCloth.wakeUp() pCloth.putToSleep()
	*/
	float wakeUpCounter;

	/**
	\brief Maximum linear velocity at which cloth can go to sleep.

	If negative, the global default will be used.

	<b>Range:</b> [0,inf)<br>
	<b>Default:</b> -1.0

	@see pCloth.setSleepLinearVelocity() pCloth.getSleepLinearVelocity()
	*/
	float sleepLinearVelocity;

	/**
	\brief Sets which collision group this cloth is part of.

	<b>Range:</b> [0, 31]
	<b>Default:</b> 0

	pCloth.setCollisionGroup()
	*/
	xU16 collisionGroup;

	/**
	\brief Sets the 128-bit mask used for collision filtering.

	<b>Default:</b> 0

	@see NxGroupsMask pCloth.setGroupsMask() pCloth.getGroupsMask()
	*/
	pGroupsMask groupsMask;

	/**
	\brief Force Field Material Index, index != 0 has to be created.

	<b>Default:</b> 0
	*/
	xU16 forceFieldMaterial;

	/**
	\brief If the flag PCF_ValidBounds is set, this variable defines the volume
	outside of which cloth particle are automatically removed from the simulation. 

	@see PCF_ValidBounds pCloth.setValidBounds()
	*/
	VxBbox validBounds;

	/**
	\brief This parameter defines the size of grid cells for collision detection.

	The cloth is represented by a set of world aligned cubical cells in broad phase.
	The size of these cells is determined by multiplying the length of the diagonal
	of the AABB of the initial cloth size with this constant.

	<b>Range:</b> [0.01,inf)<br>
	<b>Default:</b> 0.25
	*/
	float relativeGridSpacing;

	/**
	\brief Flag bits.

	<b>Default:</b> PCF_GRAVITY

	@see NxClothFlag pCloth.setFlags()
	*/ 
	unsigned int flags;

	/**
	\brief Possible debug name. The string is not copied by the SDK, only the pointer is stored.

	<b>Default:</b> NULL

	@see pCloth.setName() pCloth.getName()
	*/
	const char* name;


	/**
	\brief Vertex color to identify vertieces as tearable.

	<b>Default:</b> 255,255,254
	*/
	VxColor tearVertexColor;

	/**
	\brief Reference object to identify the world.

	<b>Default:</b> pDefaultWorld
	*/
	CK_ID worldReference;

	/**
	\brief Attachment flags.

	<b>Default:</b> PCAF_ClothAttachmentTwoway
	
	@see #pClothAttachmentFlag
	*/
	pClothAttachmentFlag attachmentFlags;

	/**
	\brief Constructor sets to default.
	*/
	pClothDesc();


	/**
	\brief (Re)sets the structure to the default.	
	*/
	void setToDefault();

	/**
	\brief Returns true if the current settings are valid
	*/
	bool isValid() const;



};

/** @} */
#endif 
// __PCLOTHTYPES_H__