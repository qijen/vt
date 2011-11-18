#ifndef __P_FACTORY_H__
	#define __P_FACTORY_H__

#include "vtPhysXBase.h"
/**
\brief Singleton class to create physic objects. 
*/
class MODULE_API pFactory
{

	
public:
	//friend class PhysicManager;
	virtual ~pFactory();

	void findAttributeIdentifiersByGuid(CKGUID guid,std::vector<int>&targetList);

	pFactory();
	pFactory(PhysicManager* prm1,TiXmlDocument*prm2);

	int reloadConfig(const char *fName);
	TiXmlDocument* loadConfig(const char* filename);
	
	pWorldSettings *createWorldSettings(const XString nodeName="Default",const TiXmlDocument * doc = NULL);
	pWorldSettings *createWorldSettings(const char* nodeName,const char *filename);
	pWorldSettings*getWorldSettings(CK3dEntity*ent);
	pWorld* createDefaultWorld(XString name);
	const TiXmlElement*getFirstDocElement(const TiXmlElement *root);
	TiXmlDocument* getDefaultDocument() const { return m_DefaultDocument; }

	TiXmlDocument* getDocument(XString filename);
	NxPhysicsSDK* getPhysicSDK() { return mPhysicSDK; }
	void setPhysicSDK(NxPhysicsSDK* val) { mPhysicSDK = val; }
	PhysicManager * getManager() const { return mManager; }
	void setManager(PhysicManager * val) { mManager = val; }
	void setDefaultDocument(TiXmlDocument* val) { m_DefaultDocument = val; }
	void clean();
	void destroy();


	//////////////////////////////////////////////////////////////////////////


	int copyTo(CKParameter *src,pDominanceSetupItem&dst);



	pSleepingSettings *CreateSleepingSettings(XString nodeName="Default", TiXmlDocument * doc = NULL);
	pSleepingSettings*CreateSleepingSettings(const char* nodeName,const char *filename);
	pSleepingSettings*getSleepingSettings(CK3dEntity*ent);

	pWorld *createWorld(CK3dEntity *referenceObject,pWorldSettings*worldSettings=NULL,pSleepingSettings*sleepSettings=NULL);

	pRemoteDebuggerSettings createDebuggerSettings(const TiXmlDocument * doc);


	/************************************************************************************************/
	/** @name RigidBody
	*/
	//@{
	
	
		
	pRigidBody *cloneRigidBody(CK3dEntity *src,CK3dEntity *dst,CKDependencies *deps,int copyFlags,int bodyFlags=0);
	NxShape *cloneShape(CK3dEntity *src,CK3dEntity *dst,CK3dEntity*dstBodyReference,int copyFlags,int bodyFlags=0);
	pJoint* cloneJoint(pJoint *src,CK3dEntity *srcReference,CK3dEntity *dstReference,int copyFlags);
	int cloneLimitPlanes(pJoint *src,pJoint *dst,CK3dEntity *srcReference,CK3dEntity *dstReference);



	void cloneJoints(CK3dEntity *src,CK3dEntity *dst,int copyFlags);


	pRigidBody *createRigidBodyFull(CK3dEntity *referenceObject,CK3dEntity *worldReferenceObject=NULL);
	pRigidBody *createBody(CK3dEntity *referenceObject,pObjectDescr descriction,CK3dEntity *worldReferenceObject=NULL);
	pRigidBody *createBody(CK3dEntity *referenceObject,CK3dEntity *worldReferenceObject=NULL);
	pRigidBody *createBody(CK3dEntity *referenceObject,CK3dEntity *worldReferenceObject,NXU::NxActorDesc *desc,int flags);
	
	pRigidBody *createRigidBody(CK3dEntity *referenceObject,pObjectDescr& oDescr);
	pRigidBody *createBox(CK3dEntity *referenceObject,CK3dEntity *worldReferenceObject,pObjectDescr*descr,int creationFlags);
	pRigidBody *createCapsule(CK3dEntity *referenceObject,CK3dEntity *worldReferenceObject,pObjectDescr*descr,int creationFlags);
	
	int createSubShape(CK3dEntity*target,CK3dEntity*child);
	NxCCDSkeleton *createCCDSkeleton(CKBeObject *meshReference,int flags);



	


	//@}
	
	/************************************************************************************************/
	/** @name Joints
	*/
	//@{
	
	pJoint *createJoint(CK3dEntity*,CK3dEntity*,int);
	pJointDistance*createDistanceJoint(CK3dEntity*a,CK3dEntity*b,VxVector anchor0=VxVector(0,0,0),VxVector anchor1=VxVector(0,0,0),float minDistance=0.0f,float maxDistance=0.0f,pSpring sSettings=pSpring(),bool collision=true,float maxForce = 0.0f, float maxTorque=0.0f,const char* attributeName="pJDistance");
	pJointD6* createD6Joint(CK3dEntity*a,CK3dEntity*b,VxVector globalAnchor=VxVector(0,0,0),VxVector globalAxis=VxVector(0,-1,0),bool collision=true,float maxForce = 0.0f, float maxTorque=0.0f);
	pJointFixed* createFixedJoint(CK3dEntity*a,CK3dEntity*b,float maxForce = 0.0f, float maxTorque=0.0f,const char* attributeName="pJFixed");
	pJointPulley*createPulleyJoint(CK3dEntity*a,CK3dEntity*b,VxVector pulley0,VxVector pulley1, VxVector anchor0, VxVector anchor1,bool collision=true,float maxForce = 0.0f, float maxTorque=0.0f);		
	pJointBall *createBallJoint(CK3dEntity*a,CK3dEntity*b,VxVector anchor,VxVector swingAxis,VxVector globalAxis=VxVector(0,1,0),bool collision=true,float maxForce = 0.0f, float maxTorque=0.0f,const char* attributeName="pJBall");
	pJointPrismatic*createPrismaticJoint(CK3dEntity*a,CK3dEntity*b,VxVector anchor,VxVector axis,bool collision=true,float maxForce = 0.0f, float maxTorque=0.0f,const char* attributeName="pJPrismatic");
	pJointCylindrical*createCylindricalJoint(CK3dEntity*a,CK3dEntity*b,VxVector anchor,VxVector axis,bool collision=true,float maxForce = 0.0f, float maxTorque=0.0f,const char* attributeName="pJCylindrical");
	pJointRevolute *createRevoluteJoint(CK3dEntity*a,CK3dEntity*b,VxVector anchor,VxVector axis,bool collision=true,float maxForce = 0.0f, float maxTorque=0.0f,const char* attributeName="pJRevolute");
	pJointPointInPlane *createPointInPlaneJoint(CK3dEntity*a,CK3dEntity*b,VxVector anchor,VxVector axis,bool collision=true,float maxForce = 0.0f, float maxTorque=0.0f,const char* attributeName="pJPointInPlane");
	pJointPointOnLine *createPointOnLineJoint(CK3dEntity*a,CK3dEntity*b,VxVector anchor,VxVector axis,bool collision=true,float maxForce = 0.0f, float maxTorque=0.0f,const char* attributeName="pJPointOnLine");




	pJoint *GetJoint(CK3dEntity*,CK3dEntity*);
	pJoint *GetJoint(CK3dEntity*,int);
	pJointSettings *CreateJointSettings(const XString nodeName="Default",const TiXmlDocument * doc = NULL);
	pJointSettings *CreateJointSettings(const char* nodeName,const char *filename);

	int jointCheckPreRequisites(CK3dEntity*,CK3dEntity*,int);

	//@}

	//////////////////////////////////////////////////////////////////////////
	//nx
	NxShapeDesc& createShape(int hullType,CK3dEntity*ent,float density = 1.0f);
	NxMaterialDesc* createMaterialFromXML(const char* nodeName="Default",const TiXmlDocument * doc = NULL);
	NxMaterialDesc* createMaterialFromEntity(CKBeObject*object);


	CKParameterOut* findSettings(pWheelDescr&dst,CKBeObject*src);
	bool findSettings(pMaterial&dst,CKBeObject*src);
	bool copyTo(pMaterial& dst,CKParameter*src);
	bool copyTo(pMaterial& dst,NxMaterial*src);
	bool copyTo(NxMaterial&dst,pMaterial*src);
	bool copyTo(CKParameterOut*dst,const pMaterial&src);
	bool copyTo(NxMaterialDesc&dst,const pMaterial&src);
	bool loadFrom(pMaterial& dst,const char* nodeName="Default",const TiXmlDocument * doc = NULL);
	bool loadMaterial(pMaterial&dst,const char* nodeName);
	pMaterial loadMaterial(const char* nodeName,int& error);

	//////////////////////////////////////////////////////////////////////////
	//nx  mesh : 
	void createMesh(NxScene *scene,CKMesh *mesh,NxTriangleMeshDesc&descr,bool hardware=false);
	void createConvexMesh(NxScene *scene,CKMesh *mesh,NxConvexMeshDesc&descr,bool hardware=false);
	NxShape *createShape(CK3dEntity *bodyReference,pObjectDescr descr,CK3dEntity *srcReference,CKMesh *mesh,VxVector localPos,VxQuaternion localRotation);


	static pFactory* Instance();

	pSpring createSpringFromParameter(CKParameter *par);
	pJointLimit createLimitFromParameter(CKParameter *par);
	pMotor createMotorFromParameter(CKParameter *par);
	pTireFunction createTireFuncFromParameter(CKParameter *par);
	pObjectDescr *createPObjectDescrFromParameter(CKParameter *par);
	
	
	/************************************************************************/
	/* clothes :                                                                      */
	/************************************************************************/
	pClothDesc *createClothDescrFromParameter(CKParameter *par);
	pCloth *createCloth(CK3dEntity *srcReference,pClothDesc descr);
	void copyToClothDescr(NxClothDesc* dst,pClothDesc src );
	void copyTo(pDeformableSettings &dst,CKParameter*par);
	
	/************************************************************************/
	/*Special Case Capsule	:                                                                       */
	/************************************************************************/
	bool copyTo(pCapsuleSettings&dst,CKParameter*src);
	bool findSettings(pCapsuleSettings&dst,CKBeObject *src);
	
	bool findSettings(pCapsuleSettingsEx&dst,CKBeObject *src);
	bool findSettings(pConvexCylinderSettings&dst,CKBeObject *src);
	bool findSettings(pCCDSettings&dst,CKBeObject *src);
	bool findSettings(pMassSettings&dst,CKBeObject *src);
	bool findSettings(pPivotSettings&dst,CKBeObject *src);
	bool findSettings(pOptimization&dst,CKBeObject *src);
	bool findSettings(pCollisionSettings&dst,CKBeObject*src);


	CKParameterOut* findSettingsParameter(CKBeObject *src,CKGUID guid);

	/************************************************************************/
	/* fluids :                                                                      */
	/************************************************************************/
	//pFluid*createFluid(CK3dEntity *srcReference ,pFluidDesc desc);
	//void copyToFluidDescr(NxFluidDesc &dst , pFluidDesc src );
	//CK3dEntity *createFluidEntity();
	//void initParticles(pFluidDesc &desc,NxParticleData&dst,CK3dEntity*srcReference,CK3dEntity*dstEntity);
	//void copyToEmitterDesc(NxFluidEmitterDesc&dst,pFluidEmitterDesc src);
	//CK3dPointCloud* createPointCloud(const pFluidDesc&descr);

	/************************************************************************/
	/* Vehicle                                                             */
	/************************************************************************/
	pVehicle *createVehicle(CK3dEntity *body,pVehicleDesc descr);
	pVehicleMotor *createVehicleMotor(pVehicleMotorDesc descr);
	pVehicleGears *createVehicleGears(pVehicleGearDesc descr);
	
	
	
	pWheel *createWheelSubShape(pRigidBody *body,CK3dEntity* subEntity,CKMesh *mesh,pObjectDescr *descr,VxVector localPos, VxQuaternion localRotation,NxShape*dstShape);
	

	XString _getMaterialsAsEnumeration(const TiXmlDocument * doc);
	XString _getBodyXMLInternalEnumeration(const TiXmlDocument * doc);
	XString _getBodyXMLExternalEnumeration(const TiXmlDocument * doc);
	
	XString _getVehicleTireFunctionAsEnumeration(const TiXmlDocument * doc);
	XString _getVehicleWheelAsEnumeration(const TiXmlDocument * doc);
	XString _getVehicleSettingsAsEnumeration(const TiXmlDocument * doc);
	XString _getVehicleMotorSettingsAsEnumeration(const TiXmlDocument * doc);
	XString _getVehicleGearSettingsAsEnumeration(const TiXmlDocument * doc);

	XString _getEnumDescription(const TiXmlDocument * doc,XString identifier);
	int _getEnumIdentifier(const TiXmlDocument * doc,XString type);
	int _getEnumIndex(XString enumerationFull,XString enumValue);

	
	NxShape *createWheelShape(NxActor *actor,pObjectDescr *descr,pWheelDescr *wheelDescr,CK3dEntity*srcReference,CKMesh *mesh,VxVector localPos,VxQuaternion localRotation);
	NxShape *_createWheelShape1(NxActor *actor,pWheel1 *dstWheel,pObjectDescr *descr,pWheelDescr *wheelDescr,CK3dEntity*srcReference,CKMesh *mesh,VxVector localPos,VxQuaternion localRotation);
	NxShape *_createWheelShape2(NxActor *actor,pObjectDescr *descr,pWheelDescr *wheelDescr,CK3dEntity*srcReference,CKMesh *mesh,VxVector localPos,VxQuaternion localRotation);
	
	NxShape *createWheelShape(CK3dEntity*bodyReference,pWheelDescr wheelDescr,CK3dEntity*wheelReference);


	pWheel *createWheel(pRigidBody *body,pWheelDescr descr,CK3dEntity *wheelShapeReference);
	pWheel* createWheel(CK3dEntity *bodyReference,CK3dEntity*srcReference,pWheelDescr wheelDescr,pConvexCylinderSettings convexCylinder,VxVector localPositionOffset);
		NxShape *createWheelShape2(CK3dEntity *bodyReference,CK3dEntity*wheelReference,pWheelDescr wheelDescr);
		NxShape *createWheelShape1(CK3dEntity *bodyReference,CK3dEntity*wheelReference,pWheelDescr wheelDescr);

	NxShape *_createConvexCylinder(NxActor *actor,int approximation,VxVector _forwardAxis,VxVector _downAxis,VxVector _rightAxis,float height,float radius,bool buildLowerHalf,int shapeFlags);
	bool _createConvexCylinder(NxConvexShapeDesc* shape,CK3dEntity*dstBodyReference,pObjectDescr *oDescr=NULL);
	
	bool _createConvexCylinderMesh(NxConvexShapeDesc *dstShapeDescription,pConvexCylinderSettings& srcSettings,CK3dEntity*referenceObject);

	NxShape *_createConvexCylinder(const pConvexCylinderSettings& cylinderDescr,CK3dEntity *srcReference);


	int loadVehicleDescrFromXML(pVehicleDesc& dst,const char* nodeName/* =NULL */,const TiXmlDocument * doc /* = NULL */ );


	//////////////////////////////////////////////////////////////////////////
	//
	//	Geometry related structures : 
	//
	void copyTo(pAxisReferencedLength&dst,CKParameter*src,bool evaluate=true);
	bool copyTo(pConvexCylinderSettings&dst,CKParameter*src,bool evaluate=true);

	//////////////////////////////////////////////////////////////////////////
	//
	//		wheels 
	int copyTo(pWheelDescr *dst,CKParameter *src);
	int loadFrom(pTireFunction& dst,const char* nodeName/* = */,const TiXmlDocument * doc /* = NULL */);
	int loadWheelDescrFromXML(pWheelDescr& dst,const char* nodeName/* =NULL */,const TiXmlDocument * doc /* = NULL */ );
	
	bool loadFrom(pWheelDescr& dst,const char* nodeName/* =NULL */);

	int copyTo(CKParameterOut *dst,pWheelDescr *src);
	int copyTo(pWheelDescr &dst,CKParameterOut *src);

	int copyTo(CKParameterOut *dst,const pTireFunction& src);


	//contact data  : 
	int copyTo(CKParameterOut *dst,const pWheelContactData& src);




	//////////////////////////////////////////////////////////////////////////
	//
	//
	//		collision structures 
	int copyTo(pGroupsMask &dst,CKParameter*src);
	int copyTo(CKParameterOut*dst,pGroupsMask src);


	CK3dEntity *getMostTopParent(CK3dEntity*ent);

protected:
	PhysicManager *mManager;
	

	TiXmlDocument* m_DefaultDocument;
	NxPhysicsSDK* mPhysicSDK;

	VxVector _str2Vec(XString _in);
	int _str2MaterialFlag(XString _in);
	int _str2WheelFlag(XString _in);
	int _str2SceneFlags(XString _in);
	int _str2PhysicFlags(XString _in);
	int _str2WheelShapeFlag(XString _in);
	XString ResolveFileName(const char *input);
	int _str2CombineMode(const char*input);
	
	CK3dEntity *createFrame(const char* name);
	
	
	

};


#endif // !defined(EA_10DFC1E8_486F_4770_9451_7898DBDAD59F__INCLUDED_)
