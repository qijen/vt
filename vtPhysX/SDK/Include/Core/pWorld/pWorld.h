#ifndef __P_WORLD_H__
#define __P_WORLD_H__

#include "pTypes.h"

/**
\brief Class to maintain physical objects.
*/
	class MODULE_API pWorld
	{

	public:
		pWorld();
		
		bool m_bCompletedLastFrame; // in class definition
		float m_fTimeSinceLastCallToSimulate; // in class def

		pWorld(CK3dEntity* _o);
		virtual ~pWorld();

		void _construct();

		int hadBrokenJoint();
		void cleanBrokenJoints();


		CK3dEntity* getReference() const { return m_vtReference; }
		void setReference(CK3dEntity* val) { m_vtReference = val; }

		pWorldSettings * getWorldSettings() const { return m_worldSettings; }
		void setWorldSettings(pWorldSettings * val) { m_worldSettings = val; }

		pSleepingSettings * getSleepingSettings() const { return m_SleepingSettings; }
		void setSleepingSettings(pSleepingSettings * val) { m_SleepingSettings = val; }

		NxScene* getScene() const { return mScene; }
		void setScene(NxScene* val) { mScene = val; }
		NxMaterial* getDefaultMaterial() const { return mDefaultMaterial; }
		void setDefaultMaterial(NxMaterial* val) { mDefaultMaterial = val; }


		int UpdateProperties(DWORD mode,DWORD flags);

		int& getDataFlags() { return m_DataFlags; }
		void setDataFlags(int val) { m_DataFlags = val; }
		void destroy();


		pRigidBody *getBody(CK3dEntity*ent);
		pRigidBody *getBodyFromSubEntity(CK3dEntity *sub);

		//pJoint*getJoint(CK3dEntity*,CK3dEntity*);
		pJoint*getJoint(CK3dEntity*_a,CK3dEntity*_b,JType type);
		void deleteJoint(pJoint*joint);

		void _checkForDominanceConstraints();
		NxCompartment * compartment;
		NxCompartment * getCompartment() const { return compartment; }
		void setCompartment(NxCompartment * val) { compartment = val; }

        
		/** @name Collision Filtering 
		*/
		//@{

		/**
		\brief Setups filtering operations. See comments for ::pGroupsMask

		<b>Sleeping:</b> Does <b>NOT</b> wake the actors up automatically.

		\param[in] op0 Filter op 0.
		\param[in] op1 Filter op 1.
		\param[in] op2 Filter op 2.

		@see setFilterBool() setFilterConstant0() setFilterConstant1()
		*/
		void setFilterOps(pFilterOp op0,pFilterOp op1, pFilterOp op2);


		/**
		\brief Setups filtering's boolean value. See comments for ::pGroupsMask

		<b>Sleeping:</b> Does <b>NOT</b> wake the actors up automatically.

		\param[in] flag Boolean value for filter.

		@see setFilterOps() setFilterConstant0() setFilterConstant1()
		*/
		void	setFilterBool(bool flag);		

		/**
		\brief Setups filtering's K0 value. See comments for ::pGroupsMask

		<b>Sleeping:</b> Does <b>NOT</b> wake the actors up automatically.

		\param[in] mask The new group mask. See #pGroupsMask.

		@see setFilterOps() setFilterBool() setFilterConstant1()
		*/
		void	setFilterConstant0(const pGroupsMask& mask);

		/**
		\brief Setups filtering's K1 value. See comments for ::pGroupsMask

		<b>Sleeping:</b> Does <b>NOT</b> wake the actors up automatically.

		\param[in] mask The new group mask. See #pGroupsMask.


		@see setFilterOps() setFilterBool() setFilterConstant0()
		*/
		void	setFilterConstant1(const pGroupsMask& mask);

		/**
		\brief Retrieves filtering operation. See comments for ::pGroupsMask

		\param[out] op0 First filter operator.
		\param[out] op1 Second filter operator.
		\param[out] op2 Third filter operator.

		See the user guide page "Contact Filtering" for more details.	

		\return the filter operation requested

		@see setFilterOps() setFilterBool() setFilterConstant0() setFilterConstant1()
		*/
		void	getFilterOps(pFilterOp& op0, pFilterOp& op1, pFilterOp& op2)const;

		/**
		\brief Retrieves filtering's boolean value. See comments for ::pGroupsMask

		\return flag Boolean value for filter.

		@see setFilterBool() setFilterConstant0() setFilterConstant1()
		*/
		bool 						getFilterBool() const;

		/**
		\brief Gets filtering constant K0. See comments for ::pGroupsMask

		\return the filtering constant, as a mask. See #pGroupsMask.

		@see setFilterOps() setFilterBool() setFilterConstant0() setFilterConstant1() getFilterConstant1()
		*/
		pGroupsMask				getFilterConstant0() const;

		/**
		\brief Gets filtering constant K1. See comments for ::NxGroupsMask

		\return the filtering constant, as a mask. See #pGroupsMask.

		@see setFilterOps() setFilterBool() setFilterConstant0() setFilterConstant1() getFilterConstant0()
		*/
		pGroupsMask				getFilterConstant1() const;







		//@}



		/** @name Raycasting
		*/
		//@{

		/**
		\brief Returns true if any axis aligned bounding box enclosing a shape of type shapeType is intersected by the ray.

		\note Make certain that the direction vector of NxRay is normalized.

		\note Because the SDK double buffers shape state, a shape will not be updated until a simulation step is 
		taken. For example the result of setting the global pose is not immediately visible.

		\param[in] worldRay The ray to cast in the global frame. <b>Range:</b> See #NxRay
		\param[in] shapesType Choose if to raycast against static, dynamic or both types of shape. See #pShapesType.
		\param[in] groups Mask used to filter shape objects. See #pRigidBody::setGroup
		\param[in] maxDist Max distance to check along the ray for intersecting bounds. <b>Range:</b> (0,inf)
		\param[in] groupsMask Alternative mask used to filter shapes. See #pRigidBody::setGroupsMask

		\return true if any axis aligned bounding box enclosing a shape of type shapeType is intersected by the ray

		@see pShapesType VxRay pRigidBody.setGroup()  raycastAnyShape()
		*/
		bool						raycastAnyBounds		(const VxRay& worldRay, pShapesType shapesType, pGroupsMask *groupsMask=NULL,int groups=0xffffffff, float maxDist=pFLOAT_MAX);

		/**
		\brief Returns true if any shape of type ShapeType is intersected by the ray.

		\note Make certain that the direction vector of NxRay is normalized.

		\note Because the SDK double buffers shape state, a shape will not be updated until a simulation step is 
		taken. For example the result of setting the global pose is not immediately visible.

		\param[in] worldRay The ray to cast in the global frame. <b>Range:</b> See #VxRay
		\param[in] shapesType Choose if to raycast against static, dynamic or both types of shape. See #pShapesType.
		\param[in] groups Mask used to filter shape objects. See #pRigidBody::setGroup
		\param[in] maxDist Max distance to check along the ray for intersecting objects. <b>Range:</b> (0,inf) 
		\param[in] groupsMask Alternative mask used to filter shapes. See #pRigidBody::setGroupsMask
		\param[in] cache Possible cache for persistent raycasts, filled out by the SDK.

		\return Returns true if any shape of type ShapeType is intersected by the ray.

		@see pShapesType VxRay pRigidBody.setGroup()  raycastAnyBounds()
		*/
		bool						raycastAnyShape			(const VxRay& worldRay, pShapesType shapesType, pGroupsMask groupsMask,CKDataArray* cache, int groups=0xffffffff, float maxDist=pFLOAT_MAX);

		/**
		\brief Calls the report's hitCallback() method for all the shapes of type ShapeType intersected by the ray.

		hintFlags is a combination of ::NxRaycastBit flags.
		Returns the number of shapes hit. The point of impact is provided as a parameter to hitCallback().

		\note Make certain that the direction vector of NxRay is normalized.

		\note Because the SDK double buffers shape state, a shape will not be updated until a simulation step is 
		taken. For example the result of setting the global pose is not immediatly visible.

		<h3>Example</h3>

		\include NxUserRaycastReport_Usage.cpp


		\param[in] worldRay The ray to cast in the global frame. <b>Range:</b> See #NxRay
		\param[in] report User callback, to be called when an intersection is encountered.
		\param[in] shapesType Choose if to raycast against static, dynamic or both types of shape. See #NxShapesType.
		\param[in] groups Mask used to filter shape objects. See #NxShape::setGroup
		\param[in] maxDist Max distance to check along the ray for intersecting objects. <b>Range:</b> (0,inf) 
		\param[in] hintFlags Allows the user to specify which field of #NxRaycastHit they are interested in. See #NxRaycastBit
		\param[in] groupsMask Alternative mask used to filter shapes. See #NxShape::setGroupsMask

		\return the number of shapes hit

		<b>Platform:</b>
		\li PC SW: Yes
		\li PPU  : Yes
		\li PS3  : Yes
		\li XB360: Yes

		@see raycastAnyShape() raycastAllBounds() NxRay NxUserRaycastReport NxShapesType NxShape.setGroup() NxRaycastHit
		*/
		int raycastAllShapes		(const VxRay& worldRay, pShapesType shapesType, int groups=0xffffffff, float maxDist=pFLOAT_MAX, pRaycastBit hintFlags=(pRaycastBit)0xffffffff, const pGroupsMask* groupsMask=NULL);


		//@}









		/** @name Overlap Testing
		*/
		//@{

		/**
		\brief Returns the set of shapes overlapped by the world-space sphere.

		You can test against static and/or dynamic objects by adjusting 'shapeType'. 
		Shapes are written to the static array 'shapes', which should be big enough to hold 'nbShapes'.

		The function returns the total number of collided shapes.

		\note Because the SDK double buffers shape state, a shape will not be updated until a simulation step is 
		taken. For example the result of setting the global pose is not immediatly visible.

		\param[in] worldSphere Sphere description in world space. <b>Range:</b> See #VxSphere
		\param[in] shapeType Choose if to intersect with static, dynamic or both types of shape. See #pShapesType.
		\param[out] shapes Buffer to store intersecting shapes. 
		\param[in] activeGroups Mask used to filter shape objects. See #pRigidBody::setGroup
		\param[in] groupsMask Alternative mask used to filter shapes. See #pRigidBody::setGroupsMask
		\param[in] accurateCollision True to test the sphere against the actual shapes, false to test against the AABBs only.

		\return the total number of collided shapes.


		@see pShapesType overlapAABBShapes
		*/
		int	overlapSphereShapes		(const VxSphere& worldSphere,CK3dEntity*shapeReference,pShapesType shapeType,CKGroup *shapes,int activeGroups=0xffffffff, const pGroupsMask* groupsMask=NULL, bool accurateCollision=false);

		/**
		\brief Returns the set of shapes overlapped by the world-space AABB.

		You can test against static and/or dynamic objects by adjusting 'shapeType'. 
		Shapes are written to the static array 'shapes', which should be big enough to hold 'nbShapes'.

		The function returns the total number of collided shapes.

		\note Because the SDK double buffers shape state, a shape will not be updated until a simulation step is 
		taken. For example the result of setting the global pose is not immediatly visible.

		\param[in] worldBounds Axis Aligned Bounding Box in world space. <b>Range:</b> 
		\param[in] shapeType Choose if to intersect with static, dynamic or both types of shape. See pShapesType.
		\param[out] shapes Buffer to store intersecting shapes. 
		\param[in] activeGroups Mask used to filter shape objects. See #pRigidBody::setGroup
		\param[in] groupsMask Alternative mask used to filter shapes. See #pRigidBody::setGroupsMask
		\param[in] accurateCollision True to test the AABB against the actual shapes, false to test against the AABBs only.

		\return the total number of collided shapes.

		@see pxShapesType overlapAABBShapes
		*/
		int overlapAABBShapes		(const VxBbox& worldBounds, CK3dEntity*shapeReference, pShapesType shapeType,CKGroup *shapes,int activeGroups=0xffffffff, const pGroupsMask* groupsMask=NULL, bool accurateCollision=false);

		/**
		\brief Returns the set of shapes overlapped by the world-space OBB.

		You can test against static and/or dynamic objects by adjusting 'shapeType'. 

		The function returns the total number of collided shapes.

		\note Because the SDK double buffers shape state, a shape will not be updated until a simulation step is 
		taken. For example the result of setting the global pose is not immediatly visible.

		\param[in] worldBox Oriented Bounding Box in world space. <b>Range:</b> See #NxBox
		\param[in] shapeType Choose if to intersect with static, dynamic or both types of shape. See #NxShapesType.
		\param[out] shapes Buffer to store intersecting shapes. Should be at least sizeof(NxShape *) * nbShapes.
		\param[in] activeGroups Mask used to filter shape objects. See #NxShape::setGroup
		\param[in] groupsMask Alternative mask used to filter shapes. See #NxShape::setGroupsMask
		\param[in] accurateCollision True to test the OBB against the actual shapes, false to test against the AABBs only.

		\return the total number of collided shapes.

		@see pShapesType overlapOBBShapes 
		*/
		int						overlapOBBShapes		(const VxBbox& worldBox,CK3dEntity*shapeReference, pShapesType shapeType,CKGroup *shapes, int activeGroups=0xffffffff, const pGroupsMask* groupsMask=NULL, bool accurateCollision=false);

		/**
		\brief Returns the set of shapes overlapped by the world-space capsule.

		You can test against static and/or dynamic objects by adjusting 'shapeType'. 
		Shapes are written to the static array 'shapes', which should be big enough to hold 'nbShapes'.
		An alternative is to use the ::NxUserEntityReport callback mechanism.

		The function returns the total number of collided shapes.

		\note Because the SDK double buffers shape state, a shape will not be updated until a simulation step is 
		taken. For example the result of setting the global pose is not immediatly visible.

		\param[in] worldCapsule capsule in world space. <b>Range:</b> See #NxCapsule
		\param[in] shapeType Choose if to intersect with static, dynamic or both types of shape. See #NxShapesType.
		\param[out] shapes Buffer to store intersecting shapes. Should be at least sizeof(NxShape *) * nbShapes.
		\param[in] activeGroups Mask used to filter shape objects. See #NxShape::setGroup
		\param[in] groupsMask Alternative mask used to filter shapes. See #NxShape::setGroupsMask
		\param[in] accurateCollision True to test the capsule against the actual shapes, false to test against the AABBs only.

		\return the total number of collided shapes.

		<b>Platform:</b>
		\li PC SW: Yes
		\li PPU  : Yes
		\li PS3  : Yes
		\li XB360: Yes

		@see pShapesType overlapCapsuleShapes 
		*/
		//virtual	NxU32						overlapCapsuleShapes		(const NxCapsule& worldCapsule, NxShapesType shapeType, NxU32 nbShapes, NxShape** shapes, NxUserEntityReport<NxShape*>* callback, NxU32 activeGroups=0xffffffff, const NxGroupsMask* groupsMask=NULL, bool accurateCollision=false) = 0;

		//@}





		void step(float stepsize);
		bool isValid();

		JointFeedbackListType m_JointFeedbackList;

		JointFeedbackListType& getJointFeedbackList()  { return m_JointFeedbackList; }
		void setJointFeedbackList(JointFeedbackListType val) { m_JointFeedbackList = val; }
		void inspectJoints();

		void deleteBody(pRigidBody*body);
		int getNbBodies();
		int getNbJoints();
		void checkList();


		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		void cSetGroupCollisionFlag(int g1 , int g2,int enabled);
		void cIgnorePair(CK3dEntity* _a,CK3dEntity *_b,int ignore);


		void initUserReports();
		pContactReport *contactReport;
		pContactReport*getContactReportPtr(){ return contactReport;}

		pContactModify *contactModify;
		pContactModify*getContactModifyPtr(){ return contactModify;}

		pTriggerReport *triggerReport;
		pTriggerReport *getTriggerReportPtr(){ return triggerReport;}

		pRayCastReport *raycastReport;
		pRayCastReport * getRaycastReport(){ return raycastReport; }
		void setRaycastReport(pRayCastReport * val) { raycastReport = val; }
		
		UserAllocator*	mAllocator;
		
		UserAllocator* setAllocator() const { return mAllocator; }
		void setAllocator(UserAllocator* val) { mAllocator= val; }

		int getNbOfVehicles();
		int getNbOfWhees();


	public  :

		CK3dEntity* m_vtReference;

		pSleepingSettings *m_SleepingSettings;
		pWorldSettings *m_worldSettings;

		NxScene*			mScene;
		NxMaterial* mDefaultMaterial;
		NxControllerManager *mCManager;
		
		

		void updateVehicles(float dt);
		void updateVehicle(pVehicle *vehicle,float dt);
		pVehicle *getVehicle(CK3dEntity* body);
		void deleteVehicle(CK3dEntity*body,int flags  = 0);
		bool isVehicle(NxActor* actor);
		pWheel1 *getWheel1(CK3dEntity *reference);
		pWheel2 *getWheel2(CK3dEntity *reference);

		/************************************************************************/
		/*                                                                      */
		/************************************************************************/
		void updateClothes();
		void updateFluids();



		pCloth *getCloth(CK3dEntity*reference);
		pCloth *getCloth(CK_ID id);
		void destroyCloth(CK_ID id);


		pFluid *getFluid(CK_ID id);
		pFluid *getFluid(CK3dEntity* entity);


		NxShape *getShapeByEntityID(CK_ID id);
		NxShape *getShapeByMeshID(CK_ID id);

		NxControllerManager * getCManager() const { return mCManager; }
		void setCManager(NxControllerManager * val) { mCManager = val; }


		void advanceTime(float lastDeltaMS);
		int onPreProcess();
		int onPostProcess();

		int callMask;
		int& getCallMask() { return callMask; }
		void setCallMask(int val) { callMask = val; }

		int overrideMask;
		int& getOverrideMask() { return overrideMask; }
		void setOverrideMask(int val) { overrideMask = val; }



		//----------------------------------------------------------------
		//
		// material 
		//
		bool getMaterialByIndex(int index,pMaterial&dst);
		bool updateMaterialByIndex(int index,pMaterial src);


		//---common settings 

		void setGravity(VxVector gravity);
		VxVector getGravity();

		int m_DataFlags;
	};

/** @} */

#endif // !defined(EA_0976AF4F_8E4B_4e7b_B4CD_B7588D3D4FF8__INCLUDED_)
