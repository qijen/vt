#ifndef __P_CLOTH_H__
#define __P_CLOTH_H__

#include "vtPhysXBase.h"




/** \addtogroup Cloth
@{
*/



/**
brief2 Class for clothes.
*/
class MODULE_API pCloth
{
public:


	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	void releaseReceiveBuffers();
	void allocateClothReceiveBuffers(int numVertices, int numTriangles);
	
	CK_ID getEntityID() const { return mEntityID; }
	void setEntityID(CK_ID val) { mEntityID = val; }

	NxCloth * getCloth() const { return mCloth; }
	void setCloth(NxCloth * val) { mCloth = val; }

	pCloth();
	virtual ~pCloth();

	NxMeshData * getReceiveBuffers() const { return mReceiveBuffers; }
	void setReceiveBuffers(NxMeshData * val) { mReceiveBuffers = val; }

	bool cookMesh(NxClothMeshDesc* desc);

	void releaseMeshDescBuffers(const NxClothMeshDesc* desc);
	bool generateMeshDesc(pClothDesc cDesc,NxClothMeshDesc *desc, CKMesh*mesh);

	pWorld * getWorld() const { return mWorld; }
	void setWorld(pWorld * val) { mWorld = val; }

	NxClothMesh * getClothMesh() const { return mClothMesh; }
	void setClothMesh(NxClothMesh * val) { mClothMesh = val; }

	void updateVirtoolsMesh();



	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	/**
	\brief Sets the cloth bending stiffness in the range from 0 to 1.

	\param[in] stiffness The stiffness of this cloth.
	@see pClothDesc.bendingStiffness getBendingStiffness()
	*/
	void setBendingStiffness(float stiffness);

	/**
	\brief Retrieves the cloth bending stiffness.

	\return Bending stiffness of cloth.
	@see pClothDesc.bendingStiffness setBendingStiffness()
	*/
	float getBendingStiffness() const;

	/**
	\brief Sets the cloth stretching stiffness in the range from 0 to 1.

	Note: The stretching stiffness must be larger than 0.

	\param[in] stiffness Stiffness of cloth.
	@see pClothDesc.stretchingStiffness getStretchingStiffness()
	*/
	void setStretchingStiffness(float stiffness);

	/**
	\brief Retrieves the cloth stretching stiffness.

	\return stretching stiffness of cloth.

	@see pClothDesc.stretchingStiffness setStretchingStiffness()
	*/
	float getStretchingStiffness() const;

	/**
	\brief Sets the damping coefficient in the range from 0 to 1.

	\param[in] dampingCoefficient damping coefficient of cloth.
	@see pClothDesc.dampingCoefficient getDampingCoefficient()
	*/
	void setDampingCoefficient(float dampingCoefficient);

	/**
	\brief Retrieves the damping coefficient.

	\return damping coefficient of cloth.
	@see pClothDesc.dampingCoefficient setDampingCoefficient()
	*/
	float getDampingCoefficient() const;

	/**
	\brief Sets the cloth friction coefficient in the range from 0 to 1.

	\param[in] friction The friction of the cloth.
	@see pClothDesc.friction getFriction()
	*/
	void setFriction(float friction);

	/**
	\brief Retrieves the cloth friction coefficient.

	\return Friction coefficient of cloth.
	@see pClothDesc.friction setFriction()
	*/
	float getFriction() const;

	/**
	\brief Sets the cloth pressure coefficient (must be non negative).

	\param[in] pressure The pressure applied to the cloth.

	@see pClothDesc.pressure getPressure()
	*/
	void setPressure(float pressure);

	/**
	\brief Retrieves the cloth pressure coefficient.

	\return Pressure of cloth.
	@see pClothDesc.pressure setPressure()
	*/
	float getPressure() const;

	/**
	\brief Sets the cloth tear factor (must be larger than one).

	\param[in] factor The tear factor for the cloth

	@see pClothDesc.tearFactor getTearFactor()
	*/
	void setTearFactor(float factor);

	/**
	\brief Retrieves the cloth tear factor.

	\return tear factor of cloth.
	@see pClothDesc.tearFactor setTearFactor()
	*/
	float getTearFactor() const;

	/**
	\brief Sets the cloth attachment tear factor (must be larger than one).

	\param[in] factor The attachment tear factor for the cloth
	@see pClothDesc.attachmentTearFactor getAttachmentTearFactor()
	*/
	void setAttachmentTearFactor(float factor);

	/**
	\brief Retrieves the attachment cloth tear factor.

	\return tear attachment factor of cloth.
	@see pClothDesc.attachmentTearFactor setAttachmentTearFactor()
	*/
	float getAttachmentTearFactor() const;

	/**
	\brief Sets the cloth thickness (must be positive).

	\param[in] thickness The thickness of the cloth.
	@see pClothDesc.thickness getThickness()
	*/
	void setThickness(float thickness);

	/**
	\brief Gets the cloth thickness.

	\return thickness of cloth.
	@see pClothDesc.thickness setThickness()
	*/
	float getThickness() const;

	/**
	\brief Gets the cloth density.

	\return density of cloth.
	@see pClothDesc.density
	*/
	float getDensity() const;

	/**
	\brief Gets the relative grid spacing for the broad phase.
	The cloth is represented by a set of world aligned cubical cells in broad phase.
	The size of these cells is determined by multiplying the length of the diagonal
	of the AABB of the initial cloth size with this constant.

	\return relative grid spacing.
	@see pClothDesc.relativeGridSpacing
	*/
	float getRelativeGridSpacing() const;

	/**
	\brief Retrieves the cloth solver iterations.

	\return solver iterations of cloth.
	@see pClothDesc.solverIterations setSolverIterations()
	*/
	int getSolverIterations() const;

	/**
	\brief Sets the cloth solver iterations.

	\param[in] iterations The new solver iteration count for the cloth.
	@see pClothDesc.solverIterations getSolverIterations()
	*/
	void setSolverIterations(int iterations);

	/**
	\brief Returns a world space AABB enclosing all cloth points.

	\param[out] bounds Retrieves the world space bounds.
	@see NxBounds3
	*/
	void getWorldBounds(VxBbox & bounds) const;

	/**
	\brief Attaches the cloth to a shape. All cloth points currently inside the shape are attached.

	\note This method only works with primitive and convex shapes. Since the inside of a general 
	triangle mesh is not clearly defined.

	\param[in] shape Shape to which the cloth should be attached to.
	\param[in] attachmentFlags One or two way interaction, tearable or non-tearable
	@see NxClothAttachmentFlag freeVertex() attachToCollidingShapes()
	*/
	void attachToShape(CKBeObject *shape, int attachmentFlags);

	/**
	\brief Attaches the cloth to all shapes, currently colliding. 

	\note This method only works with primitive and convex shapes. Since the inside of a general 
	triangle mesh is not clearly defined.

	\param[in] attachmentFlags One or two way interaction, tearable or non-tearable
	@see NxClothAttachmentFlag pClothDesc.attachmentTearFactor pClothDesc.attachmentResponseCoefficient freeVertex()
	*/
	void attachToCollidingShapes(int attachmentFlags);

	/**
	\brief Detaches the cloth from a shape it has been attached to before. 

	If the cloth has not been attached to the shape before, the call has no effect.

	\param[in] shape Shape from which the cloth should be detached.
	@see NxClothAttachmentFlag pClothDesc.attachmentTearFactor pClothDesc.attachmentResponseCoefficient freeVertex() attachToShape()
	*/
	void detachFromShape(CKBeObject *shape);

	/**
	\brief Attaches a cloth vertex to a local position within a shape.

	\param[in] vertexId Index of the vertex to attach.
	\param[in] shape Shape to attach the vertex to.
	\param[in] localPos The position relative to the pose of the shape.
	\param[in] attachmentFlags One or two way interaction, tearable or non-tearable

	<b>Platform:</b>
	\li PC SW: Yes
	\li PPU  : Yes
	\li PS3  : Yes
	\li XB360: Yes

	@see NxShape freeVertex() NxClothAttachmentFlag attachToShape()
	*/
	void attachVertexToShape(int vertexId, CKBeObject *shape, const VxVector &localPos, int attachmentFlags);

	/**
	\brief Attaches a cloth vertex to a position in world space.

	\param[in] vertexId Index of the vertex to attach.
	\param[in] pos The position in world space.
	@see pClothAttachmentFlag pClothDesc.attachmentTearFactor pClothDesc.attachmentResponseCoefficient freeVertex() attachToShape()
	*/
	void attachVertexToGlobalPosition(const int vertexId, const VxVector &pos);

	/**
	\brief Frees a previously attached cloth point.

	\param[in] vertexId Index of the vertex to free.
	@see attachVertexToGlobalPosition() attachVertexToShape() detachFromShape()
	*/
	void freeVertex(const int vertexId);

	/**
	\brief Changes the weight of a vertex in the cloth solver for a period of time.

	If this method is called for some vertex, the cloth solver will, during a time
	period of length expirationTime, assign a different weight to the vertex
	while internal cloth constraints (i.e. bending & stretching) are being resolved.

	With a high dominanceWeight, the modified vertex will force neighboring vertices
	to strongly accommodate their positions while its own is kept fairly constant. 
	The reverse holds for smaller dominanceWeights. 

	Using a dominanceWeight of +infinity has a similar effect as temporarily attaching 
	the vertex to a global position. However, unlike using attachments, the velocity 
	of the vertex is kept intact when using this method.

	\note The current implementation will not support the full range of dominanceWeights.
	All dominanceWeights > 0.0 are treated equally as being +infinity.

	\note An expiration time of 0.0 is legal and will result in dominance being
	applied throughout one substep before being discarded immediately.

	\note Having a large number of vertices dominant at once may result in a performance penalty.

	\param[in] vertexId Index of the vertex.
	\param[in] expirationTime Time period where dominance will be active for this vertex.
	\param[in] dominanceWeight Dominance weight for this vertex.

	@see attachVertexToGlobalPosition()
	*/
	void dominateVertex(int vertexId, float expirationTime, float dominanceWeight);

	/**
	\brief Return the attachment status of the given vertex.

	\param[in] vertexId Index of the vertex.

	@see getVertexAttachmentShape() getVertexAttachmentPosition()
	*/
	xU16 getVertexAttachmentStatus(int vertexId) const;

	/**
	\brief Returns the pointer to an attached shape pointer of the given vertex.

	If the vertex is not attached or attached to a global position, NULL is returned.

	\param[in] vertexId Index of the vertex.

	@see getVertexAttachmentStatus() getVertexAttachmentPosition()
	*/
	NxShape* getVertexAttachmentShape(int vertexId) const;

	/**
	\brief Returns the attachment position of the given vertex.

	If the vertex is attached to shape, the position local to the shape's pose is returned.
	If the vertex is not attached, the return value is undefined.

	\param[in] vertexId Index of the vertex.
	@see getVertexAttachmentStatus() getVertexAttachmentShape()
	*/
	VxVector getVertexAttachmentPosition(int vertexId) const;

	/**
	\brief Attaches the cloth to an actor. 

	\note Call this function only once right after the cloth is created.
	Turning cloth into metal and vice versa during the simulation is not recommended.

	\note This feature is well suited for volumetric objects like barrels. 
	It cannot handle two dimensional flat pieces well.

	After this call, the cloth is infinitely stiff between collisions and simply 
	moves with the actor. At impacts with an impact impulse greater than impulseThreshold, 
	the cloth is plastically deformed. Thus, a cloth with a core behaves like a piece of metal.

	The core actor's geometry is adjusted automatically. Its size also depends on the 
	cloth thickness. Thus, it is recommended to choose small values for the thickness.
	At impacts, colliding objects are moved closer to the cloth by the value provided in 
	penetrationDepth which causes a more dramatic collision result.

	The core actor must have at least one shape, and currently supported shapes are 
	spheres, capsules, boxes and compounds of spheres.
	It is recommended to specify the density rather than the mass of the core body. 
	This way the mass and inertia tensor are updated when the core deforms.

	The maximal deviation of cloth particles from their initial positions
	(modulo the global rigid body transforms translation and rotation) can be
	specified via the parameter maxDeformationDistance. Setting this parameter to
	zero means that the deformation is not limited.  

	\param actor The core actor to attach the cloth to.
	\param impulseThreshold Threshold for when deformation is allowed.
	\param penetrationDepth Amount by which colliding objects are brought closer to the cloth.
	\param maxDeformationDistance Maximum deviation of cloth particles from initial position.

	*/
	void attachToCore(CK3dEntity *body, float impulseThreshold, float penetrationDepth, float maxDeformationDistance);

	/**
	\brief Tears the cloth at a given vertex. 

	First the vertex is duplicated. The triangles on one side of the split plane keep 
	the original vertex. For all triangles on the opposite side the original vertex is 
	replaced by the new one. The split plane is defined by the world location of the 
	vertex and the normal provided by the user.

	Note: TearVertex performs a user defined vertex split in contrast to an automatic split
	that is performed when the flag NX_CLF_TEARABLE is set. Therefore, tearVertex works 
	even if NX_CLF_TEARABLE is not set in pClothDesc.flags.

	Note: For tearVertex to work, the clothMesh has to be cooked with the flag
	NX_CLOTH_MESH_TEARABLE set in NxClothMeshDesc.flags.

	\param[in] vertexId Index of the vertex to tear.
	\param[in] normal The normal of the split plane.
	\return true if the split had an effect (i.e. there were triangles on both sides of the split plane)

	@see NxClothFlag, NxClothMeshFlags, pClothDesc.flags NxSimpleTriangleMesh.flags

	<b>Platform:</b>
	\li PC SW: Yes
	\li PPU  : Yes
	\li PS3  : Yes
	\li XB360: Yes
	*/
	bool tearVertex(const int vertexId, const VxVector &normal);

	/**
	\brief Executes a raycast against the cloth.

	\param[in] worldRay The ray in world space.
	\param[out] hit The hit position.
	\param[out] vertexId Index to the nearest vertex hit by the raycast.

	\return true if the ray hits the cloth.
	*/
	//BOOL raycast(const NxRay& worldRay, VxVector &hit, int &vertexId);

	/**
	\brief Sets which collision group this cloth is part of.

	\param[in] collisionGroup The collision group for this cloth.

	@see NxCollisionGroup
	*/
	void setGroup(int collisionGroup);

	/**
	\brief Retrieves the value set with #setGroup().

	\return The collision group this cloth belongs to.

	@see NxCollisionGroup
	*/
	int getGroup() const;

	/**
	\brief Sets 128-bit mask used for collision filtering.

	\param[in] groupsMask The group mask to set for the cloth.
	@see getGroupsMask() NxGroupsMask
	*/
	void setGroupsMask(const pGroupsMask& groupsMask);

	/**
	\brief Sets 128-bit mask used for collision filtering.

	\return The group mask for the cloth.

	@see setGroupsMask() NxGroupsMask
	*/
	const pGroupsMask getGroupsMask() const;


	/**
	\brief Sets the valid bounds of the cloth in world space.

	If the flag NX_CLF_VALIDBOUNDS is set, these bounds defines the volume
	outside of which cloth particle are automatically removed from the simulation. 

	\param[in] validBounds The valid bounds.

	@see pClothDesc.validBounds getValidBounds() NxBounds3
	*/
	void setValidBounds(const VxBbox& validBounds);

	/**
	\brief Returns the valid bounds of the cloth in world space.

	\param[out] validBounds The valid bounds.


	@see pClothDesc.validBounds setValidBounds() NxBounds3
	*/
	void getValidBounds(NxBounds3& validBounds) const;

	/**
	\brief Sets the position of a particular vertex of the cloth.

	\param[in] position New position of the vertex.
	\param[in] vertexId Index of the vertex.

	@see getPosition() setPositions() getPositions() setVelocity() getVelocity() getNumberOfParticles()
	*/
	void setPosition(const VxVector& position, int vertexId);

	/**
	\brief Sets the positions of the cloth.

	The user must supply a buffer containing all positions (i.e same number of elements as number of particles).

	\param[in] buffer The user supplied buffer containing all positions for the cloth.
	\param[in] byteStride The stride in bytes between the position vectors in the buffer. Default is size of VxVector.
	@see getPositions() setVelocities() getVelocities() getNumberOfParticles()
	*/
	void setPositions(void* buffer, int byteStride = sizeof(VxVector));

	/**
	\brief Gets the position of a particular vertex of the cloth.

	\param[in] vertexId Index of the vertex.
	@see setPosition() setPositions() getPositions() setVelocity() getVelocity() getNumberOfParticles()
	*/
	VxVector getPosition(int vertexId) const;

	/**
	\brief Gets the positions of the cloth.

	The user must supply a buffer large enough to hold all positions (i.e same number of elements as number of particles).

	\param[in] buffer The user supplied buffer to hold all positions of the cloth.
	\param[in] byteStride The stride in bytes between the position vectors in the buffer. Default is size of VxVector.
	@see setPositions() setVelocities() getVelocities() getNumberOfParticles()
	*/
	void getPositions(void* buffer, int byteStride = sizeof(VxVector));

	/**
	\brief Sets the velocity of a particular vertex of the cloth.

	\param[in] position New velocity of the vertex.
	\param[in] vertexId Index of the vertex.
	@see setPosition() getPosition() getVelocity() setVelocities() getVelocities()  getNumberOfParticles()
	*/
	void setVelocity(const VxVector& velocity, int vertexId);

	/**
	\brief Sets the velocities of the cloth.

	The user must supply a buffer containing all velocities (i.e same number of elements as number of particles).

	\param[in] buffer The user supplied buffer containing all velocities for the cloth.
	\param[in] byteStride The stride in bytes between the velocity vectors in the buffer. Default is size of VxVector.
	@see getVelocities() setPositions() getPositions() getNumberOfParticles()
	*/
	void setVelocities(void* buffer, int byteStride = sizeof(VxVector));

	/**
	\brief Gets the velocity of a particular vertex of the cloth.

	\param[in] vertexId Index of the vertex.
	@see setPosition() getPosition() setVelocity() setVelocities() getVelocities()  getNumberOfParticles()
	*/
	VxVector getVelocity(int vertexId) const;


	/**
	\brief Sets the collision response coefficient.

	\param[in] coefficient The collision response coefficient (0 or greater).
	@see pClothDesc.collisionResponseCoefficient getCollisionResponseCoefficient()
	*/
	void setCollisionResponseCoefficient(float coefficient);

	/**
	\brief Retrieves the collision response coefficient.

	\return The collision response coefficient.

	@see pClothDesc.collisionResponseCoefficient setCollisionResponseCoefficient()
	*/
	float getCollisionResponseCoefficient() const;

	/**
	\brief Sets the attachment response coefficient

	\param[in] coefficient The attachment response coefficient in the range from 0 to 1.
	@see pClothDesc.attachmentResponseCoefficient getAttachmentResponseCoefficient()
	*/
	void setAttachmentResponseCoefficient(float coefficient);

	/**
	\brief Retrieves the attachment response coefficient

	\return The attachment response coefficient.

	@see pClothDesc.attachmentResponseCoefficient setAttachmentResponseCoefficient()
	*/
	float getAttachmentResponseCoefficient() const;

	/**
	\brief Sets the response coefficient for collisions from fluids to this cloth

	\param[in] coefficient The response coefficient 
	@see pClothDesc.fromFluidResponseCoefficient getFromFluidResponseCoefficient()
	*/
	void setFromFluidResponseCoefficient(float coefficient);

	/**
	\brief Retrieves response coefficient for collisions from fluids to this cloth

	\return The response coefficient.
	@see pClothDesc.fromFluidResponseCoefficient setFromFluidResponseCoefficient()
	*/
	float getFromFluidResponseCoefficient() const;

	/**
	\brief Sets the response coefficient for collisions from this cloth to fluids

	\param[in] coefficient The response coefficient 

	@see pClothDesc.toFluidResponseCoefficient getToFluidResponseCoefficient()
	*/
	void setToFluidResponseCoefficient(float coefficient);

	/**
	\brief Retrieves response coefficient for collisions from this cloth to fluids

	\return The response coefficient.

	@see pClothDesc.toFluidResponseCoefficient setToFluidResponseCoefficient()
	*/
	float getToFluidResponseCoefficient() const;

	/**
	\brief Sets an external acceleration which affects all non attached particles of the cloth

	\param[in] acceleration The acceleration vector (unit length / s^2)
	@see pClothDesc.externalAcceleration getExternalAcceleration()
	*/
	void setExternalAcceleration(VxVector acceleration);

	/**
	\brief Retrieves the external acceleration which affects all non attached particles of the cloth

	\return The acceleration vector (unit length / s^2)

	@see pClothDesc.externalAcceleration setExternalAcceleration()
	*/
	VxVector getExternalAcceleration() const;

	/**
	\brief If the NX_CLF_ADHERE flag is set the cloth moves partially in the frame 
	of the attached actor. 

	This feature is useful when the cloth is attached to a fast moving character.
	In that case the cloth adheres to the shape it is attached to while only 
	velocities below the parameter minAdhereVelocity are used for secondary effects.

	\param[in] velocity The minimal velocity for cloth to adhere (unit length / s)

	@see pClothDesc.minAdhereVelocity getMinAdhereVelocity()
	*/
	void setMinAdhereVelocity(float velocity);

	/**
	\brief If the NX_CLF_ADHERE flag is set the cloth moves partially in the frame 
	of the attached actor. 

	This feature is useful when the cloth is attached to a fast moving character.
	In that case the cloth adheres to the shape it is attached to while only 
	velocities below the parameter minAdhereVelocity are used for secondary effects.

	\return Returns the minimal velocity for cloth to adhere (unit length / s)
	@see pClothDesc.minAdhereVelocity setMinAdhereVelocity()
	*/
	float getMinAdhereVelocity() const;

	/**
	\brief Sets an acceleration acting normal to the cloth surface at each vertex.

	\param[in] acceleration The acceleration vector (unit length / s^2)
	@see pClothDesc.windAcceleration getWindAcceleration()
	*/
	void setWindAcceleration(VxVector acceleration);

	/**
	\brief Retrieves the acceleration acting normal to the cloth surface at each vertex.

	\return The acceleration vector (unit length / s^2)
	@see pClothDesc.windAcceleration setWindAcceleration()
	*/
	VxVector getWindAcceleration() const;

	/**
	\brief Returns true if this cloth is sleeping.

	When a cloth does not move for a period of time, it is no longer simulated in order to save time. This state
	is called sleeping. However, because the object automatically wakes up when it is either touched by an awake object,
	or one of its properties is changed by the user, the entire sleep mechanism should be transparent to the user.

	If a cloth is asleep after the call to NxScene::fetchResults() returns, it is guaranteed that the position of the cloth 
	vertices was not changed. You can use this information to avoid updating dependent objects.

	\return True if the cloth is sleeping.
	@see isSleeping() getSleepLinearVelocity() wakeUp() putToSleep()
	*/
	bool isSleeping() const;

	/**
	\brief Returns the linear velocity below which a cloth may go to sleep.

	A cloth whose linear velocity is above this threshold will not be put to sleep.

	@see isSleeping

	\return The threshold linear velocity for sleeping.
	@see isSleeping() getSleepLinearVelocity() wakeUp() putToSleep() setSleepLinearVelocity()
	*/
	float getSleepLinearVelocity() const;

	/**
	\brief Sets the linear velocity below which a cloth may go to sleep.

	A cloth whose linear velocity is above this threshold will not be put to sleep.

	If the threshold value is negative,	the velocity threshold is set using the NxPhysicsSDK's 
	NX_DEFAULT_SLEEP_LIN_VEL_SQUARED parameter.

	\param[in] threshold Linear velocity below which a cloth may sleep. <b>Range:</b> (0,inf]
	@see isSleeping() getSleepLinearVelocity() wakeUp() putToSleep()
	*/
	void setSleepLinearVelocity(float threshold);

	/**
	\brief Wakes up the cloth if it is sleeping.  

	The wakeCounterValue determines how long until the cloth is put to sleep, a value of zero means 
	that the cloth is sleeping. wakeUp(0) is equivalent to NxCloth::putToSleep().

	\param[in] wakeCounterValue New sleep counter value. <b>Range:</b> [0,inf]
	@see isSleeping() getSleepLinearVelocity() putToSleep()
	*/
	void wakeUp(float wakeCounterValue = pSLEEP_INTERVAL);

	/**
	\brief Forces the cloth to sleep. 

	The cloth will fall asleep.
	@see isSleeping() getSleepLinearVelocity() wakeUp()
	*/
	void putToSleep();

	/**
	\brief Sets the flags, a combination of the bits defined by the enum ::NxClothFlag.

	\param[in] flags #NxClothFlag combination.
	@see pClothDesc.flags NxClothFlag getFlags()
	*/
	void setFlags(int flags);

	/**
	\brief Retrieves the flags.

	\return The cloth flags.
	@see pClothDesc.flags NxClothFlag setFlags()
	*/
	int getFlags() const;

	/**
	\brief Sets a name string for the object that can be retrieved with getName().

	This is for debugging and is not used by the SDK. The string is not copied by 
	the SDK, only the pointer is stored.

	\param[in] name String to set the objects name to.
	@see getName()
	*/
	void setName(const char* name);

	/**
	\brief Applies a force (or impulse) defined in the global coordinate frame, to a particular 
	vertex of the cloth. 

	Because forces are reset at the end of every timestep, 
	you can maintain a total external force on an object by calling this once every frame.

	::NxForceMode determines if the force is to be conventional or impulsive.

	\param[in] force Force/impulse to add, defined in the global frame. <b>Range:</b> force vector
	\param[in] vertexId Number of the vertex to add the force at. <b>Range:</b> position vector
	\param[in] mode The mode to use when applying the force/impulse 
	(see #ForceMode, supported modes are FM_Force, FM_Impulse, FM_Acceleration, FM_VelocityChange)
	@see ForceMode 
	*/
	void addForceAtVertex(const VxVector& force, int vertexId, ForceMode mode = FM_Force);

	/**
	\brief Applies a radial force (or impulse) at a particular position. All vertices
	within radius will be affected with a quadratic drop-off. 

	Because forces are reset at the end of every timestep, 
	you can maintain a total external force on an object by calling this once every frame.

	::NxForceMode determines if the force is to be conventional or impulsive.

	\param[in] position Position to apply force at.
	\param[in] magnitude Magnitude of the force/impulse to apply.
	\param[in] radius The sphere radius in which particles will be affected. <b>Range:</b> position vector
	\param[in] mode The mode to use when applying the force/impulse 
	(see #ForceMode, supported modes are FM_Force, FM_Impulse, FM_Acceleration, FM_VelocityChange).
	@see ForceMode 
	*/
	void addForceAtPos(const VxVector& position, float magnitude, float radius, ForceMode mode = FM_Force);

	/**
	\brief Applies a directed force (or impulse) at a particular position. All vertices
	within radius will be affected with a quadratic drop-off. 

	Because forces are reset at the end of every timestep, 
	you can maintain a total external force on an object by calling this once every frame.

	::NxForceMode determines if the force is to be conventional or impulsive.

	\param[in] position Position to apply force at.
	\param[in] force Force to apply.
	\param[in] radius The sphere radius in which particles will be affected. <b>Range:</b> position vector
	\param[in] mode The mode to use when applying the force/impulse 
	(see #ForceMode, supported modes are FM_Force, FM_Impulse, FM_Acceleration, FM_VelocityChange).
	@see ForceMode 
	*/
	void addDirectedForceAtPos(const VxVector& position, const VxVector& force, float radius, ForceMode mode = FM_Force);

	/**
	\brief Finds triangles touching the input bounds.

	\warning This method returns a pointer to an internal structure using the indices member. Hence the
	user should use or copy the indices before calling any other API function.

	\param[in] bounds Bounds to test against in world space. <b>Range:</b> See #NxBounds3
	\param[out] nb Retrieves the number of triangle indices touching the AABB.
	\param[out] indices Returns an array of touching triangle indices. 
	The triangle indices correspond to the triangles referenced to by pClothDesc.meshdata (#NxMeshData).
	Triangle i has the vertices 3i, 3i+1 and 3i+2 in the array NxMeshData.indicesBegin.
	\return True if there is an overlap.

	@see NxBounds3 pClothDesc NxMeshData
	*/
	bool overlapAABBTriangles(const NxBounds3& bounds, int& nb, const int*& indices) const;



protected:
private:
	NxMeshData *mReceiveBuffers;
	
	CK_ID mEntityID;
	NxCloth *mCloth;
	NxClothMesh *mClothMesh;
	
	pWorld *mWorld;
};

/** @} */


#endif