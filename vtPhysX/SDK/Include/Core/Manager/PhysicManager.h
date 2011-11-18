#ifndef _PhysicManager_H
#define _PhysicManager_H


#include "vtPhysXBase.h"
#include "CKPhysicsManager.h"
#include "pManagerTypes.h"

#include "pWorldTypes.h"

class xTime;

/**
\brief PhysicManager is a Virtools manager and acts as singleton object. 

Its responsible for :

-	maintaining of multiple worlds and all other physic objects
-	providing quick access to physic objects beyond the world boundaries
-	holding default parameters

*/
class MODULE_API PhysicManager : public CKPhysicManager
{

public:

	int sceneWasChanged;
	void _removeObjectsFromOldScene(CKScene *lastScene);
	CKScene* _isSceneObject(CK3dEntity *object);
	void _registerNewScene(CKScene*newScene);

	JointFeedbackListType m_JointFeedbackList;
	JointFeedbackListType& getJointFeedbackList()  { return m_JointFeedbackList; }
	void setJointFeedbackList(JointFeedbackListType val) { m_JointFeedbackList = val; }

	void _cleanOrphanedJoints();

	int physicFlags;
	int getPhysicFlags() const { return physicFlags; }
	void setPhysicFlags(int val) { physicFlags = val; }

	pTriggerArray mTriggers;
	pTriggerArray& getTriggers() { return mTriggers; }

	void _cleanTriggers();

	pBodyList bodyListRemove;
	int _checkRemovalList();
	pBodyList &getRemovalList(){ return bodyListRemove;}

	pBodyList bodyListCheck;
	int _checkListCheck();
	pBodyList &getCheckList(){ return bodyListCheck;}


	void copyToAttributes(pObjectDescr src,CK3dEntity *dst);
	xTime *time;
	bool disablePhysics;
	bool checkPhysics;

	bool m_IsSimulating;

	pBodyList resetList;
	pBodyList& _getResetList(){return resetList;}

	pRestoreMap *restoreMap;
	pRestoreMap* _getRestoreMap();


	//################################################################
	//
	// Friends
	//
	bool checkCallbackSignature(CKBehavior *beh,int type,XString& errMessage);


	//################################################################
	//
	// Constructors,initialization,registration
	//

	XString m_LastLogEntry;
	XString GetLastLogEntry() const { return m_LastLogEntry; }
	void SetLastLogEntry(XString val) { m_LastLogEntry = val; }

	//----------------------------------------------------------------
	//
	//! \brief Constructors
	//
	PhysicManager(CKContext* ctx);
	void _construct(xBitSet flags = 0 );
	//----------------------------------------------------------------
	//
	//! \brief Initialization
	//
	int initPhysicEngine(int flags = 0);
	int initManager(int flags=0);
	int performInitialization();

	void _initResources(int flags);
	
	//################################################################
	//
	//	Registration of attributes, custom structures, custom 
	//	enumeration and related
	//
	void _registerWatchers(CKContext*context);//not used
	void _unregisterWatchers();// not used





	//----------------------------------------------------------------
	//
	//! \brief Attaches functions per object type. Most 
	//	types are registered by attributes.
	//
	void _RegisterAttributeCallbacks();
	
	//----------------------------------------------------------------
	//
	//! \brief Registration of attribute types
	//
	void _RegisterAttributes();

	//----------------------------------------------------------------
	//
	//! \brief Attribute help functions
	//
	int getAttributeTypeByGuid(CKGUID guid);
	int GetPAttribute(){ return att_physic_object;}
	int GetSSAttribute(){ return att_sleep_settings;}
    
	//----------------------------------------------------------------
	//
	//! \brief Help function, not been used yet.
	//	Stub
	void registerCustomAttribute(XString attributeName,int attributeType);


	typedef int (*ObjectConstructorFunction)(PhysicManager*, CK3dEntity*,int);
	void registerObjectByAttributeFunction(CK3dEntity *ent,int attributeID,ObjectRegisterFunction cFunc);

	AttributeFunctionArrayType& getAttributeFunctions() { return mAttributeFunctions; }
	void setAttributeFunctions(AttributeFunctionArrayType val) { mAttributeFunctions = val; }

	ObjectRegisterFunction getAttributeFunction(CKGUID attributeParameterGuid);

	void populateAttributeFunctions();
	void cleanAttributePostObjects();
		
	AttributeFunctionArrayType mAttributeFunctions;
	PostRegistrationArrayType mPostCleanAttributeObjects;
	CustomParametersArrayType mCustomStructures;
	
	CustomParametersArrayType& _getCustomStructures() { return mCustomStructures; }
	void setCustomStructures(CustomParametersArrayType val) { mCustomStructures = val; }



	PostRegistrationArrayType& getAttributePostObjects() { return mPostCleanAttributeObjects; }
	void setAttributePostObjects(PostRegistrationArrayType val) { mPostCleanAttributeObjects = val; }


	ObjectRegistration* getRegistrationTable();
	
	//----------------------------------------------------------------
	//
	//! \brief Registers all custom parameter, enumeration.
	//
	void _RegisterParameters();
		void _RegisterJointParameters();
		void _RegisterBodyParameters();
		void _RegisterBodyParameterFunctions();
		void _RegisterVehicleParameters();
		void _RegisterWorldParameters();

	//----------------------------------------------------------------
	//
	//! \brief Help functions to register customer enumerations which
	//	are changed by xml files. OnCKReset for instance, the default 
	//	xml file will be re-parsed and the enumerations become update.
	//
	//
	void _RegisterDynamicParameters();
	void _RegisterDynamicEnumeration(XString file,XString enumerationName,CKGUID enumerationGuid,PFEnumStringFunction enumFunc,BOOL hidden);


	//################################################################
	//
	// Parameter operations 
	//

	//----------------------------------------------------------------
	//
	//! \brief Registers all parameter operations at once.
	//
	void _RegisterParameterOperations();
		void _RegisterParameterOperationsBody();
		void _RegisterParameterOperationsJoint();
		void _RegisterParameterOperationsMisc();
		void _RegisterParameterOperationsVehicle();
		void _RegisterParameterOperationsCollision();
	
	//################################################################
	//
	//	VSL Support
	//
	void _RegisterVSL();
	void _RegisterVSLCommon();
	void _RegisterVSLJoint();
	void _RegisterVSLRigidBody();
	void _RegisterVSLVehicle();
	void _RegisterVSLCloth();



	//----------------------------------------------------------------
	//
	//! \brief The fluid SDK is not finished and is intended as 
	//	additional component. 
	
#ifdef HAS_FLUID
	void _RegisterFluid_VSL();
#endif 
	
	
	
	
	//----------------------------------------------------------------
	//
	//! \brief Hooking building blocks, adds settings to built-in building blocks.
	//
	CKERROR _Hook3DBBs();
	CKERROR _HookGenericBBs();
	CKERROR _UnHookGenericBBs();
	CKERROR _UnHook3DBBs();


	//----------------------------------------------------------------
	//
	//! \brief Destruction
	//
	~PhysicManager();	
	void _destruct(xBitSet flags = 0);
	void _UnRegister(xBitSet flags=0);
	void _UnRegisterAttributeCallbacks(xBitSet flags=0);
	void _UnRegisterParameters(xBitSet flags=0);
	void cleanAll();
	void destroyWorlds();
	


	//################################################################
	//
	// Settings
	//

	//----------------------------------------------------------------
	//
	//! \brief XML related help functions
	//
	void reloadXMLDefaultFile(const char*fName);


	//----------------------------------------------------------------
	//
	//! \brief Flags to track the state of the manager. 
	//

	
	xBitSet& _getManagerFlags()  { return mManagerFlags; }
	void _setManagerFlags(xBitSet val) { mManagerFlags = val; }
	bool isValid();

	//----------------------------------------------------------------
	//
	//! \brief Common parameters for a world (NXScene) 
	//
	pWorldSettings * getDefaultWorldSettings(){ return mDefaultWorldSettings; }
	void setDefaultWorldSettings(pWorldSettings * val) { mDefaultWorldSettings = val; }

	

	//----------------------------------------------------------------
	//
	//! \brief Dongle related functions and members
	//
	XString _getConfigPath();
	int DongleHasBasicVersion;
	int DongleHasAdvancedVersion;

	static void makeDongleTest();


	//################################################################
	//
	// Periodic functions, called by frame. 
	//


	void checkWorldsByType(CKGUID attributeParameterType);
	
	
	//################################################################
	//
	// Misc functions
	//
	

	//----------------------------------------------------------------
	//
	//! \brief Logging
	//
	pLogger* getLogger(){ return mLogger; }
	void setLogger(pLogger* val) { mLogger = val; }
	void enableLogLevel(int type,int verbosity,int value);


	
	//################################################################
	//
	// PhysX related helpers 
	//
	
	NxPhysicsSDK* getPhysicsSDK(){ return mPhysicsSDK; }
	void setPhysicsSDK(NxPhysicsSDK* val) { mPhysicsSDK = val; }

	int getHWVersion();
	int getNbPPUs();
	int getInternalVersion(int& apiRev, int& descRev, int& branchId);


	//################################################################
	//
	// Object helpers
	//

	int getNbObjects(int flags =0);


	//----------------------------------------------------------------
	//
	//! \brief World related 


	pWorldMap* getWorlds(){ return m_Worlds; }
	pWorld *getWorld(CK_ID _o);
	pWorld *getWorld(CK3dEntity *_o,CK3dEntity *body=NULL);
	pWorld *getWorld(int index =0);

	pVehicle* getVehicle(CK3dEntity*bodyReference);
	pWheel2* getWheel(CK3dEntity*wheelReference);


	pWorld *getWorldByBody(CK3dEntity*ent);
	pWorld *getWorldByShapeReference(CK3dEntity *shapeReference);

	int getNbWorlds();
	void deleteWorld(CK_ID _o);
	

	void setWorlds(pWorldMap *val) { m_Worlds = val; }

	float timer;
	void advanceTime(float time);

	pWorld * getDefaultWorld() { return m_DefaultWorld; }
	void setDefaultWorld(pWorld * val) { m_DefaultWorld = val; }


	
	/************************************************************************************************/
	/** @name Joints
	*/
	//@{

	/**
	\brief Finds a joint object within all existing worlds, identified by two references and its type.
	
	\param[in] CK3dEntity * referenceA , r: the first object participating in the constraint. This argument must be non-zero.<br>
		-	<b>Range:</b> [object range) <br>
		-	<b>Default:</b> NULL <br>

	\param[in] CK3dEntity * referenceB , r:  the second object participating in the constraint. This can be NULL because joint constraints can be created between a rigid body and the global world frame.<br>
		-	<b>Range:</b> [object range) <br>
		-	<b>Default:</b> NULL <br>
	
	\param[in] JType type, r:	an additional hint to find the right joint object. Notice that one rigid body can have 1:n joints so it makes sense to pass the type to this function. By default its returning the first found joint on the given reference objects. <br>
		-	<b>Range:</b> [joint type range)  <br>
		-	<b>Default:</b> #JT_Any <br>

	\return pJoint*
	
	@see deleteJoint()
	*/
	pJoint*getJoint(CK3dEntity*referenceA,CK3dEntity*referenceB=NULL,JType type=JT_Any);


	/**
	\if internal2
		
		\brief Checks 3D entities for attached joint attributes. If so, its registering the new constraint. 

		\return void

	\endif 
	*/
	void _checkObjectsByAttribute(CKScene *newScene = NULL);
	int _checkResetList();

	//@}

	void checkWorlds();
	void checkClothes();
	
	void checkBodies();

	void _RegisterObjectsByAttribute();


	
	void createWorlds(int flags);
	void destroy();
	BOOL  checkDemo(CK3dEntity*);



	pFactory * getCurrentFactory(){ return m_currentFactory; }
	void setCurrentFactory(pFactory * val) { m_currentFactory = val; }
	

	/************************************************************************/
	/* xml document access                                                                      */
	/************************************************************************/

	TiXmlDocument* loadDefaults(XString filename);
	TiXmlDocument* getDefaultConfig() const { return m_DefaultDocument; }
	void setDefaultConfig(TiXmlDocument* val) { m_DefaultDocument = val; }

	/************************************************************************/
	/* RigidBody                                                                     */
	/************************************************************************/
	
	pRigidBody *getBody(CK3dEntity*ent,bool lookInSubshapes=false);
	pRigidBody *getBody(CK3dEntity*ent,pWorld* world);
	pRigidBody *getBody(const char*name,int flags=0);
	NxShape *getSubShape(CK3dEntity*referenceObject);

	/**
	\brief Returns the givens entity's rigid body object to which the sub shape belongs to.

	\param[in] CK3dEntity * ent ,the entity r: This argument must be non-zero.<br>
	-	<b>Range:</b> [object range) <br>
	-	<b>Default:</b> NULL <br>

	\return pRigidBody*

	@see deleteJoint()
	*/
	pRigidBody* isSubShape(CK3dEntity*ent);


	NxCCDSkeleton *getCCDSkeleton(CKBeObject *shapeReference);
	void doInit();

	/************************************************************************/
	/* Clothes :                                                                      */
	/************************************************************************/
	pCloth *getCloth(CK_ID entityID);


	/************************************************************************/
	/* Fluids :                                                                     */
	/************************************************************************/
	pFluid *getFluid(CK3dEntity *entityReference);
	pFluidEmitter *getFluidEmitter(CK3dEntity *entityReferene);


	/**
	\brief Function that lets you set global simulation parameters.

	Returns false if the value passed is out of range for usage specified by the enum.

	<b>Sleeping:</b> Does <b>NOT</b> wake any actors which may be affected.

	See #pSDKParameter for a description of parameters support by hardware.

	\param[in] paramEnum Parameter to set. See #pSDKParameter
	\param[in] paramValue The value to set, see #pSDKParameter for allowable values.
	\return False if the parameter is out of range.

	\note All parameters are available in the variable manager too.

	@see pSDKParameter getParameter
	*/
	void setParameter(pSDKParameter parm,float value);

	/**
	\brief Function that lets you query global simulation parameters.

	See #pSDKParameter for a description of parameters support by hardware.

	\param[in] paramEnum The Parameter to retrieve.
	\return The value of the parameter.
	@see setParameter pSDKParameter
	*/
	float getParameter(pSDKParameter parm,float value);

	void bindVariables();
	void unBindVariables();

	void setPSDKParameters(pSDKParameters&param);

	
	//----------------------------------------------------------------
	//
	// character controller 
	//

	UserAllocator * getUserAllocator() const { return mUserAllocator; }
	void setUserAllocator(UserAllocator * val) { mUserAllocator = val; }
	

	NxControllerManager* getControllerManager() const { return mControllerManager; }
	void setControllerManager(NxControllerManager* val) { mControllerManager = val; }

	void _createControllerManager();
	void _releaseControllerManager();

private : 
	
	
	UserAllocator *mUserAllocator;
	NxControllerManager* mControllerManager;
	TiXmlDocument* m_DefaultDocument;
	pWorld *m_DefaultWorld;
	pFactory *m_currentFactory;
	pWorldMap* m_Worlds;
	

	
	NxPhysicsSDK*	mPhysicsSDK;
	pLogger*mLogger;
	pWorldSettings *mDefaultWorldSettings;
	xBitSet mManagerFlags;
	NxRemoteDebugger* mRemoteDebugger;

	pSDKParameters m_SDKParameters;
	pRemoteDebuggerSettings mRemoteDebuggerSettings;
	
	pRemoteDebuggerSettings& getRemoteDebuggerSettings()  { return mRemoteDebuggerSettings; }
	void setRemoteDebuggerSettings(pRemoteDebuggerSettings val) { mRemoteDebuggerSettings = val; }


	IParameter *mIParameter;


	
public:
	int att_physic_object;	int att_hull_type;	int att_mass;	int att_surface_props;int att_physic_limit;	BOOL m_DelOnReset;
	int att_world_object;int att_sleep_settings;int att_damping;int att_collMask;int att_update_world_flags;int att_update_body_flags;
	int att_heightField;int att_JBall;int att_JFixed;int att_JHinge;int att_JHinge2;int att_JSlider;int att_JUniversal;int att_JMotor;
	int att_wheelDescr;
	int att_clothDescr;
	int att_trigger;
	int att_deformable;
	int att_capsule;
	
	float mLastStepTime;


	int _LogInfo;
	int _LogTrace;
	int _LogWarnings;
	int _LogErrors;
	int _LogToConsole;

public : 


	
	
	float getLastTimeStep(int flags);


	pSDKParameters& getSDKParameters() { return m_SDKParameters; }
	void setSDKParameters(pSDKParameters val) { m_SDKParameters = val; }


	NxRemoteDebugger* getRemoteDebugger();
    
	

	void Update();
	void update(float stepsize);


	static PhysicManager *GetInstance();
	static CKContext *GetContext();

	

	static PhysicManager * Cast(CKBaseManager* iM) { return GetInstance();}
	CKERROR OnCKInit(); 
	CKERROR PostClearAll();
	CKERROR PreSave();
	CKERROR OnCKReset();
	CKERROR OnCKEnd();
	CKERROR PreProcess();
	CKERROR SequenceToBeDeleted(CK_ID *objids,int count);
	CKERROR OnCKPlay();
	CKERROR OnCKPause();
	CKERROR PostProcess();
	CKERROR SequenceAddedToScene(CKScene *scn,CK_ID *objids,int count);
	CKERROR SequenceRemovedFromScene(CKScene *scn,CK_ID *objids,int count);
	CKERROR PreLaunchScene(CKScene* OldScene,CKScene* NewScene);
	CKERROR PostLaunchScene(CKScene* OldScene,CKScene* NewScene);
	CKERROR SequenceDeleted(CK_ID *objids,int count);
	CKERROR PreClearAll();
	CKERROR OnPostCopy(CKDependenciesContext& context);
	
	CKERROR PostLoad();

	int _migrateOldCustomStructures(CKScene *scnene);


	//---   Called to save manager data. return NULL if nothing to save...
	virtual CKStateChunk* SaveData(CKFile* SavedFile);
	void _saveUserEnumeration(CKStateChunk *chunk,CKFile* SavedFile);
	
	virtual CKERROR LoadData(CKStateChunk *chunk,CKFile* LoadedFile);
	void _loadUserEnumeration(CKStateChunk *chunk,CKFile* LoadedFile);

	//---  Called at the end of a save operation.
	CKERROR PostSave();
	float getLastDeltaTime(){return mLastStepTime;}

	
	virtual CKDWORD GetValidFunctionsMask()	
	{ 
		return 
		CKMANAGER_FUNC_PreLaunchScene|
		CKMANAGER_FUNC_PostLaunchScene|
		CKMANAGER_FUNC_OnSequenceRemovedFromScene|
		CKMANAGER_FUNC_OnSequenceDeleted|
		CKMANAGER_FUNC_OnSequenceAddedToScene|
		CKMANAGER_FUNC_PreClearAll|
		CKMANAGER_FUNC_PostClearAll|
		CKMANAGER_FUNC_OnSequenceToBeDeleted|
		CKMANAGER_FUNC_OnCKPlay|
		CKMANAGER_FUNC_OnCKPause|
		CKMANAGER_FUNC_OnCKInit|
		CKMANAGER_FUNC_PreSave|
		CKMANAGER_FUNC_PostLoad|
		CKMANAGER_FUNC_OnPostCopy|
		CKMANAGER_FUNC_PostSave|
		CKMANAGER_FUNC_OnCKReset|
		CKMANAGER_FUNC_PostProcess|
		CKMANAGER_FUNC_PreProcess|
		CKMANAGER_FUNC_OnCKEnd;
	}
	

	int processOptions;
	virtual int& getProcessOptions() { return processOptions; }
	virtual void setProcessOptions(int val) { processOptions = val; }



};

#define GetPMan() PhysicManager::GetInstance()
#define ctx() PhysicManager::GetInstance()->GetContext()
#define lastStepTimeSec GetPMan()->GetContext()->GetTimeManager()->GetLastDeltaTimeFree() * 0.001f
#define lastStepTimeMS GetPMan()->GetContext()->GetTimeManager()->GetLastDeltaTimeFree()



#endif

