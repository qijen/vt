#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "VSLManagerSDK.h"

//#include "pVehicle.h"


#include "Bind.h"

PhysicManager *ourMan = NULL;

typedef ForceMode PForceMode;
typedef D6MotionMode PJMotion;
typedef D6DriveType PDriveType;

//	a	{d1=35549095 d2=1877427728 d=0x0012fc9c }	CKGUID

#define BEH_GUID_VSL CKGUID(35549095,1877427728)


CKGUID getGuidByID(int op)
{

	CKBeObject *obj = (CKBeObject*)GetPMan()->GetContext()->GetObject(op);

	CKGUID a;
	if (obj)
	{
		CKBehavior *beh = (CKBehavior*)(obj);
		if (beh)
		{
			a = beh->GetPrototypeGuid();
			
		}
	}

return CKGUID();

}


int doVSLScript(int vslBehID)
{

	//----------------------------------------------------------------
	//
	// our script, simple addition of two inputs
	//
	XString vslScript;
	vslScript << "void main(){ c  = a + b; }" ;
	
	
	int result = 0;



	CKBehaviorIO *scriptIn;
	//sprintf(tmpName,"%s-beh",NameChar);

	/*
	CKBehavior *MyScript = (CKBehavior *) ctx()->CreateObject(CKCID_BEHAVIOR,"MyScript");

	MyScript->SetType(CKBEHAVIORTYPE_SCRIPT);
	scriptIn=MyScript->CreateInput("Start");
	MyScript->UseGraph();
	MyScript->SetCompatibleClassID(CKCID_BEOBJECT);
	*/

	


	//----------------------------------------------------------------
	//
	// create script
	//
	//CK_OBJECTCREATION_OPTIONS crOptions = CK_OBJECTCREATION_DYNAMIC;
	//CKBehavior *script = (CKBehavior *)ctx()->CreateObject(CKCID_BEHAVIOR,NULL,CK_OBJECTCREATION_DYNAMIC);
	CKBehavior *script = (CKBehavior *)ctx()->GetObject(vslBehID);
	//CKERROR error	=	script->InitFromGuid(BEH_GUID_VSL);
	
	// create ins/outs
	//
	script->CreateInputParameter("a",CKPGUID_INT);
	script->CreateInputParameter("b",CKPGUID_INT);

	script->CreateOutputParameter("c",CKPGUID_INT);
	
	script->SetName("Created_Rotate");

	//script->SetAsTargetable();
	//script->UseTarget();

	//error= MyScript->AddSubBehavior(script);
	//ctx()->GetCurrentLevel()->AddObject(MyScript);


	//----------------------------------------------------------------
	//
	// activate RunVSL Settings "Run-Time Script Change"
	//
	bool dynaFlag=true;
	
	//script->SetLocalParameterValue(0,&dynaFlag);
	//script->SetLocalParameterValue(1,&dynaFlag);

	int count = script->GetInputParameterCount();
	int lcount = script->GetLocalParameterCount();

	//----------------------------------------------------------------
	//
	// Pass the script content
	//
	vtTools::BehaviorTools::SetInputParameterValue<CKSTRING>(script,0,vslScript.Str());
	//----------------------------------------------------------------
	//
	// Pass arguments
	//
	vtTools::BehaviorTools::SetInputParameterValue<int>(script,1,10);
	vtTools::BehaviorTools::SetInputParameterValue<int>(script,2,11);

	//----------------------------------------------------------------
	//
	// execute
	//
	script->Execute(0.5f);
	result = vtTools::BehaviorTools::GetOutputParameterValue<int>(script,0);

	/*
	CKFile* file = ctx()->CreateCKFile();
	
	file->StartSave("c:\\1.nms");
	file->SaveObject((CKObject *) MyScript);
	file->EndSave();
	ctx()->DeleteCKFile(file);
	*/


	return result;
    
}



pRigidBody *getBody(CK3dEntity*ent){
	pRigidBody *body  = GetPMan()->getBody(ent);
	if (body)
	{
		return body;
	}else{
		return NULL;
	}
}

void __newpSpring(BYTE *iAdd) 
{
	new (iAdd) pSpring();
}
void __newpSoftLimit(BYTE *iAdd) 
{
	new (iAdd) pJD6SoftLimit();
}

void __newpDrive(BYTE *iAdd) 
{
	new (iAdd) pJD6Drive();
}
void __newpJointLimit(BYTE *iAdd) 
{
	new (iAdd) pJointLimit();
}

void __newpMotor(BYTE *iAdd) 
{
	new (iAdd) pMotor();
}


pSerializer *GetSerializer()
{
	return pSerializer::Instance();
}



void __newpClothDescr(BYTE *iAdd)
{
	new(iAdd)pClothDesc();
}

#define TESTGUID CKGUID(0x2c5c47f6,0x1d0755d9)


void PhysicManager::_RegisterVSL()
{
	ourMan = GetPMan();

	
	_RegisterVSLCommon();
	_RegisterVSLVehicle();

	STARTVSLBIND(m_Context)


		DECLAREFUN_C_1(CKGUID,getGuidByID,int)

		DECLAREFUN_C_1(int,doVSLScript,int)


		
		//////////////////////////////////////////////////////////////////////////
		//
		//		Bindings for pVehicle related classes : 
		//
		//

		DECLAREENUM("pClothAttachmentFlag")
		DECLAREENUMVALUE("pClothAttachmentFlag", "PCAF_ClothAttachmentTwoway" ,1 )
		DECLAREENUMVALUE("pClothAttachmentFlag", "PCAF_ClothAttachmentTearable" ,2 )

		DECLAREENUM("pClothFlag")
		DECLAREENUMVALUE("pClothFlag", "PCF_Pressure" ,1 )

		DECLAREENUMVALUE("pClothFlag", "PCF_Static",2)
		DECLAREENUMVALUE("pClothFlag", "PCF_DisableCollision",4)
		DECLAREENUMVALUE("pClothFlag", "PCF_SelfCollision",8)
		DECLAREENUMVALUE("pClothFlag", "PCF_Gravity",32)
		DECLAREENUMVALUE("pClothFlag", "PCF_Bending",64)
		DECLAREENUMVALUE("pClothFlag", "PCF_BendingOrtho",128)
		DECLAREENUMVALUE("pClothFlag", "PCF_Damping",256)
		DECLAREENUMVALUE("pClothFlag", "PCF_CollisionTwoway",512)
		DECLAREENUMVALUE("pClothFlag", "PCF_TriangleCollision",2048)
		DECLAREENUMVALUE("pClothFlag", "PCF_Tearable",4096)
		DECLAREENUMVALUE("pClothFlag", "PCF_Hardware",8192)
		DECLAREENUMVALUE("pClothFlag", "PCF_ComDamping",16384)
		DECLAREENUMVALUE("pClothFlag", "PCF_ValidBounds",32768)
		DECLAREENUMVALUE("pClothFlag", "PCF_FluidCollision",65536)
		DECLAREENUMVALUE("pClothFlag", "PCF_DisableDynamicCCD",131072)
		DECLAREENUMVALUE("pClothFlag", "PCF_AddHere",262144)
		DECLAREENUMVALUE("pClothFlag", "PCF_AttachToParentMainShape",524288)
		DECLAREENUMVALUE("pClothFlag", "PCF_AttachToCollidingShapes",1048576)
		DECLAREENUMVALUE("pClothFlag", "PCF_AttachToCore",2097152)
		DECLAREENUMVALUE("pClothFlag", "PCF_AttachAttributes",4194304)


	
				


		DECLAREOBJECTTYPE(pClothDesc)
		
		DECLAREMEMBER(pClothDesc,float,thickness)
		DECLAREMEMBER(pClothDesc,float,density)
		DECLAREMEMBER(pClothDesc,float,bendingStiffness)

		DECLAREMEMBER(pClothDesc,float,stretchingStiffness)
		DECLAREMEMBER(pClothDesc,float,dampingCoefficient)
		DECLAREMEMBER(pClothDesc,float,friction)
		DECLAREMEMBER(pClothDesc,float,pressure)
		DECLAREMEMBER(pClothDesc,float,tearFactor)
		DECLAREMEMBER(pClothDesc,float,collisionResponseCoefficient)
		DECLAREMEMBER(pClothDesc,float,attachmentResponseCoefficient)
		DECLAREMEMBER(pClothDesc,float,attachmentTearFactor)
		DECLAREMEMBER(pClothDesc,float,toFluidResponseCoefficient)
		DECLAREMEMBER(pClothDesc,float,fromFluidResponseCoefficient)
		DECLAREMEMBER(pClothDesc,float,minAdhereVelocity)
		DECLAREMEMBER(pClothDesc,int,solverIterations)
		DECLAREMEMBER(pClothDesc,VxVector,externalAcceleration)
		DECLAREMEMBER(pClothDesc,VxVector,windAcceleration)
		DECLAREMEMBER(pClothDesc,float,wakeUpCounter)
		DECLAREMEMBER(pClothDesc,float,sleepLinearVelocity)
		DECLAREMEMBER(pClothDesc,int,collisionGroup)
		DECLAREMEMBER(pClothDesc,VxBbox,validBounds)
		DECLAREMEMBER(pClothDesc,float,relativeGridSpacing)
		DECLAREMEMBER(pClothDesc,pClothFlag,flags)
		DECLAREMEMBER(pClothDesc,pClothAttachmentFlag,attachmentFlags)
		DECLAREMEMBER(pClothDesc,VxColor,tearVertexColor)
		DECLAREMEMBER(pClothDesc,CK_ID,worldReference)


		DECLAREMETHOD_0(pClothDesc,void,setToDefault)
		DECLARECTOR_0(__newpClothDescr)



	
		//////////////////////////////////////////////////////////////////////////
	//
	//					Vehicle : 
	//

	DECLAREMETHOD_1(PhysicManager,pVehicle*,getVehicle,CK3dEntity*)
	DECLAREMETHOD_1(PhysicManager,pWheel2*,getWheel,CK3dEntity*)


	



	//////////////////////////////////////////////////////////////////////////
	//
	//		Bindings for pVehicle related classes : 
	//
	//


	
	
	


		

		
		//////////////////////////////////////////////////////////////////////////
		//
		//		Bindings for pVehicle related classes : 
		//
		//
		DECLAREOBJECTTYPE(pJointLimit)
		DECLARECTOR_0(__newpJointLimit)
		DECLAREMEMBER(pJointLimit,float,hardness)
		DECLAREMEMBER(pJointLimit,float,restitution)
		DECLAREMEMBER(pJointLimit,float,value)

		DECLAREOBJECTTYPE(pJD6Drive)
		DECLARECTOR_0(__newpDrive)
		DECLAREMEMBER(pJD6Drive,float,damping)
		DECLAREMEMBER(pJD6Drive,float,spring)
		DECLAREMEMBER(pJD6Drive,float,forceLimit)
		DECLAREMEMBER(pJD6Drive,int,driveType)

		DECLAREOBJECTTYPE(pJD6SoftLimit)
		DECLARECTOR_0(__newpSoftLimit)
		DECLAREMEMBER(pJD6SoftLimit,float,damping)
		DECLAREMEMBER(pJD6SoftLimit,float,spring)
		DECLAREMEMBER(pJD6SoftLimit,float,value)
		DECLAREMEMBER(pJD6SoftLimit,float,restitution)


		DECLAREOBJECTTYPE(pSpring)
		DECLARECTOR_0(__newpSpring)
		//DECLARECTOR_3(__newpSpringSettings3,float,float,float)
		DECLAREMEMBER(pSpring,float,damper)
		DECLAREMEMBER(pSpring,float,spring)
		DECLAREMEMBER(pSpring,float,targetValue)

		DECLAREOBJECTTYPE(pMotor)
		DECLARECTOR_0(__newpMotor)
		DECLAREMEMBER(pMotor,float,maximumForce)
		DECLAREMEMBER(pMotor,float,targetVelocity)
		DECLAREMEMBER(pMotor,float,freeSpin)

		//////////////////////////////////////////////////////////////////////////
		//
		//					Serializer : 
		//
		DECLAREPOINTERTYPE(pSerializer)
		DECLAREFUN_C_0(pSerializer*, GetSerializer)
		DECLAREMETHOD_2(pSerializer,void,overrideBody,pRigidBody*,int)
		DECLAREMETHOD_2(pSerializer,int,loadCollection,const char*,int)
		DECLAREMETHOD_1(pSerializer,int,saveCollection,const char*)
		DECLAREMETHOD_2(pSerializer,void,parseFile,const char*,int)


		


		
		//////////////////////////////////////////////////////////////////////////
		//
		//					Factory
		//

		
		


		//////////////////////////////////////////////////////////////////////////
		//
		//					ENUMERATION 
		//

		DECLAREENUM("D6DriveType")

		DECLAREENUMVALUE("D6DriveType", "D6DT_Position" ,1 )
		DECLAREENUMVALUE("D6DriveType", "D6DT_Velocity" ,2 )


		DECLAREENUM("PForceMode")

		DECLAREENUMVALUE("PForceMode", "PFM_Force" , 0)
		DECLAREENUMVALUE("PForceMode", "PFM_Impulse" , 1)
		DECLAREENUMVALUE("PForceMode", "PFM_VelocityChange" , 2)
		DECLAREENUMVALUE("PForceMode", "PFM_SmoothImpulse" , 3)
		DECLAREENUMVALUE("PForceMode", "PFM_SmoothVelocityChange" , 4)
		DECLAREENUMVALUE("PForceMode", "PFM_Acceleration" , 5)
		
		DECLAREENUM("D6MotionMode")

		DECLAREENUMVALUE("D6MotionMode", "D6MM_Locked" , 0)
		DECLAREENUMVALUE("D6MotionMode", "D6MM_Limited" , 1)
		DECLAREENUMVALUE("D6MotionMode", "D6MM_Free" , 2)

		DECLAREENUM("JType")
		DECLAREENUMVALUE("JType", "JT_Any" , -1)
		DECLAREENUMVALUE("JType", "JT_Prismatic" , 0)
		DECLAREENUMVALUE("JType", "JT_Revolute" , 1)
		DECLAREENUMVALUE("JType", "JT_Cylindrical" , 2)
		DECLAREENUMVALUE("JType", "JT_Spherical" , 3)
		DECLAREENUMVALUE("JType", "JT_PointOnLine" , 4)
		DECLAREENUMVALUE("JType", "JT_PointInPlane" , 5)
		DECLAREENUMVALUE("JType", "JT_Distance" , 6)
		DECLAREENUMVALUE("JType", "JT_Pulley" , 7)
		DECLAREENUMVALUE("JType", "JT_Fixed" ,8 )
		DECLAREENUMVALUE("JType", "JT_D6" ,9 )

		
		//////////////////////////////////////////////////////////////////////////
		//
		//		Bindings for pVehicle related classes : 
		//
		//
		DECLAREMETHOD_2(pFactory,pVehicle*,createVehicle,CK3dEntity*,pVehicleDesc)
		

		//DECLAREMETHOD_0(pVehicle,float,getWheelRollAngle)
		//DECLAREMETHOD_0(pVehicle,float,getRpm)



		
		

		//////////////////////////////////////////////////////////////////////////
		//
		//					JOINT CREATION
		//
		DECLAREMETHOD_11_WITH_DEF_VALS(pFactory,pJointDistance*,createDistanceJoint,CK3dEntity*,NODEFAULT,CK3dEntity*,NODEFAULT,VxVector,VxVector(),VxVector,VxVector(),float,0.0f,float,0.0f,pSpring,pSpring(),BOOL,"TRUE",float,"0.0",float,"0.0",const char *,"pJDistance")
		DECLAREMETHOD_5(pFactory,pJointD6*,createD6Joint,CK3dEntity*,CK3dEntity*,VxVector,VxVector,bool)
		
		DECLAREMETHOD_5_WITH_DEF_VALS(pFactory,pJointFixed*,createFixedJoint,CK3dEntity*,NODEFAULT,CK3dEntity*,NODEFAULT,float,"0.0",float,"0,0",const char*,"PJFixed")

		DECLAREMETHOD_3_WITH_DEF_VALS(pFactory,pRigidBody*,createBody,CK3dEntity*,NODEFAULT,pObjectDescr,NODEFAULT,CK3dEntity*,NULL)
		
		DECLAREMETHOD_2(pFactory,pRigidBody*,createRigidBody,CK3dEntity*,pObjectDescr&)

		DECLAREMETHOD_2(pFactory,bool,loadMaterial,pMaterial&,const char*)
		DECLAREMETHOD_2(pFactory,pMaterial,loadMaterial,const char*,int&)

		DECLAREMETHOD_2(pFactory,bool,loadFrom,pWheelDescr&,const char*)
		DECLAREMETHOD_5(pFactory,pWheel*,createWheel,CK3dEntity *,CK3dEntity*,pWheelDescr,pConvexCylinderSettings,VxVector)

		DECLAREMETHOD_9_WITH_DEF_VALS(pFactory,pJointPulley*,createPulleyJoint,CK3dEntity*,NODEFAULT,CK3dEntity*,NODEFAULT,VxVector,NODEFAULT,VxVector,NODEFAULT,VxVector,NODEFAULT,VxVector,NODEFAULT,BOOL,"TRUE",float,"0.0",float,"0.0")
		DECLAREMETHOD_9_WITH_DEF_VALS(pFactory,pJointBall*,createBallJoint,CK3dEntity*,NODEFAULT,CK3dEntity*,NODEFAULT,VxVector,NODEFAULT,VxVector,NODEFAULT,VxVector,"0,0,1",bool,"TRUE",float,"0.0",float,"0.0",const char*,"pJBall")
		DECLAREMETHOD_7_WITH_DEF_VALS(pFactory,pJointRevolute*,createRevoluteJoint,CK3dEntity*,NODEFAULT,CK3dEntity*,NODEFAULT,VxVector,NODEFAULT,VxVector,NODEFAULT,bool,"TRUE",float,"0.0",float,"0.0",const char*,"pJRevolute")
		DECLAREMETHOD_7_WITH_DEF_VALS(pFactory,pJointPrismatic*,createPrismaticJoint,CK3dEntity*,NODEFAULT,CK3dEntity*,NODEFAULT,VxVector,NODEFAULT,VxVector,NODEFAULT,bool,TRUE,float,"0.0",float,"0.0",const char*,"pJPrismatic")
		DECLAREMETHOD_7_WITH_DEF_VALS(pFactory,pJointCylindrical*,createCylindricalJoint,CK3dEntity*,NODEFAULT,CK3dEntity*,NODEFAULT,VxVector,NODEFAULT,VxVector,NODEFAULT,bool,TRUE,float,"0.0",float,"0.0",,const char*,"pJCylindrical")
		DECLAREMETHOD_7_WITH_DEF_VALS(pFactory,pJointPointInPlane*,createPointInPlaneJoint,CK3dEntity*,NODEFAULT,CK3dEntity*,NODEFAULT,VxVector,NODEFAULT,VxVector,NODEFAULT,bool,TRUE,float,"0.0",float,"0.0",const char*,"pJPointInPlane")
		DECLAREMETHOD_7_WITH_DEF_VALS(pFactory,pJointPointOnLine*,createPointOnLineJoint,CK3dEntity*,NODEFAULT,CK3dEntity*,NODEFAULT,VxVector,NODEFAULT,VxVector,NODEFAULT,bool,TRUE,float,"0.0",float,"0.0",const char*,"pJPointOnLine")

		DECLAREMETHOD_2(pFactory,pCloth*,createCloth,CK3dEntity*,pClothDesc)



		//////////////////////////////////////////////////////////////////////////
		//
		//					Cloth
		//
		DECLAREMETHOD_4(pCloth,void,attachToCore,CK3dEntity*,float,float,float)
		DECLAREMETHOD_2(pCloth,void,attachToShape,CK3dEntity*,pClothAttachmentFlag)


		//////////////////////////////////////////////////////////////////////////
		//
		//					MANAGER
		//
		
		
		DECLAREMETHOD_0(PhysicManager,pWorld*,getDefaultWorld)
		DECLAREMETHOD_2_WITH_DEF_VALS(PhysicManager,pRigidBody*,getBody,CK3dEntity*,NODEFAULT,bool,FALSE)
		DECLAREMETHOD_1(PhysicManager,pWorld*,getWorldByBody,CK3dEntity*)
		DECLAREMETHOD_1(PhysicManager,pWorld*,getWorld,CK_ID)
		DECLAREMETHOD_1(PhysicManager,int,getAttributeTypeByGuid,CKGUID)
		DECLAREMETHOD_2(PhysicManager,void,copyToAttributes,pObjectDescr,CK3dEntity*)
		DECLAREMETHOD_1(CK3dEntity,CKBOOL,HasAttribute,int)

		DECLAREMETHOD_3_WITH_DEF_VALS(PhysicManager,pJoint*,getJoint,CK3dEntity*,NODEFAULT,CK3dEntity*,NULL,JType,JT_Any)

//		DECLAREMETHOD_0(PhysicManager,void,makeDongleTest)


		//////////////////////////////////////////////////////////////////////////
		//
		//					World
		//
		
		DECLAREMETHOD_1(pWorld,pRigidBody*,getBody,CK3dEntity*)
		DECLAREMETHOD_1(pWorld,pVehicle*,getVehicle,CK3dEntity*)
		DECLAREMETHOD_3(pWorld,pJoint*,getJoint,CK3dEntity*,CK3dEntity*,JType)

		DECLAREMETHOD_3(pWorld,void,setFilterOps,pFilterOp,pFilterOp,pFilterOp)
		DECLAREMETHOD_1(pWorld,void,setFilterBool,bool)
		DECLAREMETHOD_1(pWorld,void,setFilterConstant0,const pGroupsMask&)
		DECLAREMETHOD_1(pWorld,void,setFilterConstant1,const pGroupsMask&)
		
		DECLAREMETHOD_1(pWorld,void,setGravity,VxVector)
		DECLAREMETHOD_0(pWorld,VxVector,getGravity)






		//
		DECLAREMETHOD_5_WITH_DEF_VALS(pWorld,bool,raycastAnyBounds,const VxRay&,NODEFAULT,pShapesType,NODEFAULT,pGroupsMask,NODEFAULT,int,0xffffffff,float,NX_MAX_F32)
		DECLAREMETHOD_8(pWorld,bool,overlapSphereShapes,CK3dEntity*,const VxSphere&,CK3dEntity*,pShapesType,CKGroup*,int,const pGroupsMask*,BOOL)

		//(const VxRay& worldRay, pShapesType shapesType, pGroupsMask groupsMask,unsigned int groups=0xffffffff, float maxDist=NX_MAX_F32);



		DECLAREMETHOD_1(pJoint,void,setGlobalAxis,VxVector)
		DECLAREMETHOD_1(pJoint,void,setGlobalAnchor,VxVector)
		DECLAREMETHOD_0(pJoint,VxVector,getGlobalAxis)
		DECLAREMETHOD_0(pJoint,VxVector,getGlobalAnchor)



		//////////////////////////////////////////////////////////////////////////
		//
		//				JOINT :: Revolute
		//
		DECLAREMETHOD_0(pJoint,pJointRevolute*,castRevolute)
		DECLAREMETHOD_1(pJointRevolute,void,setGlobalAnchor,const VxVector&)
		DECLAREMETHOD_1(pJointRevolute,void,setGlobalAxis,const VxVector&)

		
		DECLAREMETHOD_1(pJointRevolute,void,setSpring,pSpring)
		DECLAREMETHOD_1(pJointRevolute,void,setHighLimit,pJointLimit)
		DECLAREMETHOD_1(pJointRevolute,void,setLowLimit,pJointLimit)
		DECLAREMETHOD_1(pJointRevolute,void,setMotor,pMotor)

		DECLAREMETHOD_0(pJointRevolute,pSpring,getSpring)
		DECLAREMETHOD_0(pJointRevolute,pJointLimit,getLowLimit)
		DECLAREMETHOD_0(pJointRevolute,pJointLimit,getHighLimit)
		DECLAREMETHOD_0(pJointRevolute,pMotor,getMotor)
		DECLAREMETHOD_1(pJointRevolute,void,enableCollision,bool)

		//////////////////////////////////////////////////////////////////////////
		//
		//					JOINT :: Ball
		//
		DECLAREMETHOD_0(pJoint,pJointBall*,castBall)

		DECLAREMETHOD_1(pJointBall,void,setAnchor,VxVector)
		
		DECLAREMETHOD_1(pJointBall,void,setSwingLimitAxis,VxVector)
		DECLAREMETHOD_1(pJointBall,bool,setSwingLimit,pJointLimit)
		DECLAREMETHOD_1(pJointBall,bool,setTwistHighLimit,pJointLimit)
		DECLAREMETHOD_1(pJointBall,bool,setTwistLowLimit,pJointLimit)

		
		DECLAREMETHOD_0(pJointBall,pJointLimit,getSwingLimit)
		DECLAREMETHOD_0(pJointBall,pJointLimit,getTwistHighLimit)
		DECLAREMETHOD_0(pJointBall,pJointLimit,getTwistLowLimit)
		
		DECLAREMETHOD_1(pJointBall,bool,setSwingSpring,pSpring)
		DECLAREMETHOD_1(pJointBall,bool,setTwistSpring,pSpring)
		DECLAREMETHOD_1(pJointBall,void,setJointSpring,pSpring)

		DECLAREMETHOD_0(pJointBall,pSpring,getSwingSpring)
		DECLAREMETHOD_0(pJointBall,pSpring,getTwistSpring)
		DECLAREMETHOD_0(pJointBall,pSpring,getJointSpring)
		
		DECLAREMETHOD_1(pJointBall,void,enableCollision,bool)
		//////////////////////////////////////////////////////////////////////////
		//
		//				JOINT Prismatic
		//
		//
		DECLAREMETHOD_0(pJoint,pJointPrismatic*,castPrismatic)

		DECLAREMETHOD_1(pJointPrismatic,void,setGlobalAnchor,VxVector)
		DECLAREMETHOD_1(pJointPrismatic,void,setGlobalAxis,VxVector)
		DECLAREMETHOD_1(pJointPrismatic,void,enableCollision,bool)
		//////////////////////////////////////////////////////////////////////////
		//
		//				JOINT Cylindrical
		//
		//
		DECLAREMETHOD_0(pJoint,pJointCylindrical*,castCylindrical)

		DECLAREMETHOD_1(pJointCylindrical,void,setGlobalAnchor,VxVector)
		DECLAREMETHOD_1(pJointCylindrical,void,setGlobalAxis,VxVector)
		DECLAREMETHOD_1(pJointCylindrical,void,enableCollision,int)

		//////////////////////////////////////////////////////////////////////////
		//
		//				JOINT Point In Plane
		//
		//
		DECLAREMETHOD_0(pJoint,pJointPointInPlane*,castPointInPlane)

		DECLAREMETHOD_1(pJointPointInPlane,void,setGlobalAnchor,VxVector)
		DECLAREMETHOD_1(pJointPointInPlane,void,setGlobalAxis,VxVector)
		DECLAREMETHOD_1(pJointPointInPlane,void,enableCollision,bool)

		//////////////////////////////////////////////////////////////////////////
		//
		//				JOINT Point In Plane
		//
		//
		DECLAREMETHOD_0(pJoint,pJointPointOnLine*,castPointOnLine)

		DECLAREMETHOD_1(pJointPointOnLine,void,setGlobalAnchor,VxVector)
		DECLAREMETHOD_1(pJointPointOnLine,void,setGlobalAxis,VxVector)
		DECLAREMETHOD_1(pJointPointOnLine,void,enableCollision,bool)

		//////////////////////////////////////////////////////////////////////////
		//
		//				JOINT BASE 
		//
		//
		DECLAREMETHOD_1(pJoint,void,setLocalAnchor0,VxVector)

		DECLAREMETHOD_2(pJoint,void,setBreakForces,float,float)
		DECLAREMETHOD_2(pJoint,void,getBreakForces,float&,float&)
		DECLAREMETHOD_3(pJoint,int,addLimitPlane,VxVector,VxVector,float)
		DECLAREMETHOD_2_WITH_DEF_VALS(pJoint,void,setLimitPoint,VxVector,NODEFAULT,bool,true)
		DECLAREMETHOD_0(pJoint,void,purgeLimitPlanes)
		DECLAREMETHOD_0(pJoint,void,resetLimitPlaneIterator)
		DECLAREMETHOD_0(pJoint,int,hasMoreLimitPlanes)
		DECLAREMETHOD_3(pJoint,int,getNextLimitPlane,VxVector&,float&,float&)
		DECLAREMETHOD_0(pJoint,int,getType)
		//////////////////////////////////////////////////////////////////////////
		//
		//					JOINT :: DISTANCE
		//
		DECLAREMETHOD_0(pJoint,pJointDistance*,castDistanceJoint)

		DECLAREMETHOD_1(pJointDistance,void,setMinDistance,float)
		DECLAREMETHOD_1(pJointDistance,void,setMaxDistance,float)
		DECLAREMETHOD_1(pJointDistance,void,setLocalAnchor0,VxVector)
		DECLAREMETHOD_1(pJointDistance,void,setLocalAnchor1,VxVector)
		DECLAREMETHOD_1(pJointDistance,void,setSpring,pSpring)
		DECLAREMETHOD_0(pJointDistance,float,getMinDistance)
		DECLAREMETHOD_0(pJointDistance,float,getMaxDistance)
		DECLAREMETHOD_0(pJointDistance,float,getLocalAnchor0)
		DECLAREMETHOD_0(pJointDistance,float,getLocalAnchor1)
		DECLAREMETHOD_0(pJointDistance,pSpring,getSpring)
		DECLAREMETHOD_1(pJointDistance,void,enableCollision,bool)
		//////////////////////////////////////////////////////////////////////////
		//
		//					JOINT PULLEY
		//

		DECLAREMETHOD_0(pJoint,pJointPulley*,castPulley)

		DECLAREMETHOD_1(pJointPulley,void,setLocalAnchorA,VxVector)
		DECLAREMETHOD_1(pJointPulley,void,setLocalAnchorB,VxVector)
		DECLAREMETHOD_1(pJointPulley,void,setPulleyA,VxVector)
		DECLAREMETHOD_1(pJointPulley,void,setPulleyB,VxVector)
		DECLAREMETHOD_1(pJointPulley,void,setStiffness,float)
		DECLAREMETHOD_1(pJointPulley,void,setRatio,float)
		DECLAREMETHOD_1(pJointPulley,void,setRigid,int)
		DECLAREMETHOD_1(pJointPulley,void,setDistance,float)
		DECLAREMETHOD_1(pJointPulley,void,setMotor,pMotor)
		DECLAREMETHOD_0(pJointPulley,VxVector,getLocalAnchorA)
		DECLAREMETHOD_0(pJointPulley,VxVector,getLocalAnchorB)
		DECLAREMETHOD_0(pJointPulley,VxVector,getPulleyA)
		DECLAREMETHOD_0(pJointPulley,VxVector,getPulleyB)
		DECLAREMETHOD_0(pJointPulley,float,getStiffness)

		DECLAREMETHOD_0(pJointPulley,float,getRatio)
		DECLAREMETHOD_0(pJointPulley,float,getDistance)
		DECLAREMETHOD_1(pJointPulley,void,enableCollision,bool)
		DECLAREMETHOD_0(pJointPulley,pMotor,getMotor)

		//////////////////////////////////////////////////////////////////////////
		//
		//			JOINT D6
		//

		DECLAREMETHOD_0(pJoint,pJointD6*,castD6Joint)

		DECLAREMETHOD_1(pJointD6,void,setTwistMotionMode,D6MotionMode)
		DECLAREMETHOD_1(pJointD6,void,setSwing1MotionMode,D6MotionMode)
		DECLAREMETHOD_1(pJointD6,void,setSwing2MotionMode,D6MotionMode)
		
		DECLAREMETHOD_0(pJointD6,D6MotionMode,getXMotion)
		DECLAREMETHOD_0(pJointD6,D6MotionMode,getYMotion)
		DECLAREMETHOD_0(pJointD6,D6MotionMode,getZMotion)


		DECLAREMETHOD_1(pJointD6,void,setXMotionMode,D6MotionMode)
		DECLAREMETHOD_1(pJointD6,void,setYMotionMode,D6MotionMode)
		DECLAREMETHOD_1(pJointD6,void,setZMotionMode,D6MotionMode)

		DECLAREMETHOD_0(pJointD6,D6MotionMode,getXMotion)
		DECLAREMETHOD_0(pJointD6,D6MotionMode,getYMotion)
		DECLAREMETHOD_0(pJointD6,D6MotionMode,getZMotion)

		//////////////////////////////////////////////////////////////////////////
		//softwLimits
		DECLAREMETHOD_0(pJointD6,pJD6SoftLimit,getLinearLimit)
		DECLAREMETHOD_1(pJointD6,int,setLinearLimit,pJD6SoftLimit)

		DECLAREMETHOD_0(pJointD6,pJD6SoftLimit,getSwing1Limit)
		DECLAREMETHOD_1(pJointD6,int,setSwing1Limit,pJD6SoftLimit)

		DECLAREMETHOD_0(pJointD6,pJD6SoftLimit,getSwing2Limit)
		DECLAREMETHOD_1(pJointD6,int,setSwing2Limit,pJD6SoftLimit)

		DECLAREMETHOD_0(pJointD6,pJD6SoftLimit,getTwistHighLimit)
		DECLAREMETHOD_1(pJointD6,int,setTwistHighLimit,pJD6SoftLimit)
		
		DECLAREMETHOD_0(pJointD6,pJD6SoftLimit,getTwistLowLimit)
		DECLAREMETHOD_1(pJointD6,int,setTwistLowLimit,pJD6SoftLimit)

		DECLAREMETHOD_0(pJointD6,pJD6Drive,getXDrive)
		DECLAREMETHOD_1(pJointD6,int,setXDrive,pJD6Drive)
		
		DECLAREMETHOD_0(pJointD6,pJD6Drive,getYDrive)
		DECLAREMETHOD_1(pJointD6,int,setYDrive,pJD6Drive)

		DECLAREMETHOD_0(pJointD6,pJD6Drive,getZDrive)
		DECLAREMETHOD_1(pJointD6,int,setZDrive,pJD6Drive)

		DECLAREMETHOD_0(pJointD6,pJD6Drive,getSwingDrive)
		DECLAREMETHOD_1(pJointD6,int,setSwingDrive,pJD6Drive)

		DECLAREMETHOD_0(pJointD6,pJD6Drive,getTwistDrive)
		DECLAREMETHOD_1(pJointD6,int,setTwistDrive,pJD6Drive)

		DECLAREMETHOD_0(pJointD6,pJD6Drive,getSlerpDrive)
		DECLAREMETHOD_1(pJointD6,int,setSlerpDrive,pJD6Drive)

		DECLAREMETHOD_1(pJointD6,void,setDrivePosition,VxVector)
		DECLAREMETHOD_1(pJointD6,void,setDriveRotation,VxQuaternion)
		
		DECLAREMETHOD_1(pJointD6,void,setDriveLinearVelocity,VxVector)
		DECLAREMETHOD_1(pJointD6,void,setDriveAngularVelocity,VxVector)
		
		DECLAREMETHOD_1(pJointD6,void,enableCollision,bool)




		//////////////////////////////////////////////////////////////////////////
		//
		//				Rigid Body Exports 
		//
		//


		/************************************************************************/
		/* Forces                                                                     */
		/************************************************************************/
		DECLAREMETHOD_3_WITH_DEF_VALS(pRigidBody,void,addForce,const VxVector&,NODEFAULT, PForceMode, 0,bool,true)
		DECLAREMETHOD_3_WITH_DEF_VALS(pRigidBody,void,addTorque,const VxVector&,NODEFAULT,PForceMode,0,bool,true)
		DECLAREMETHOD_3_WITH_DEF_VALS(pRigidBody,void,addLocalForce,const VxVector&,NODEFAULT,PForceMode,0,bool,true)
		DECLAREMETHOD_3_WITH_DEF_VALS(pRigidBody,void,addLocalTorque,const VxVector&,NODEFAULT, PForceMode,0,bool,true)
		DECLAREMETHOD_4_WITH_DEF_VALS(pRigidBody,void,addForceAtPos,  const VxVector&,NODEFAULT,const VxVector&,NODEFAULT,PForceMode,0,bool,true)
		DECLAREMETHOD_4_WITH_DEF_VALS(pRigidBody,void,addForceAtLocalPos,const VxVector,NODEFAULT, const VxVector&, NODEFAULT,PForceMode,0,bool,true)
		DECLAREMETHOD_4_WITH_DEF_VALS(pRigidBody,void,addLocalForceAtPos, const VxVector&, NODEFAULT,const VxVector&,NODEFAULT, PForceMode,0,bool,true)
		DECLAREMETHOD_4_WITH_DEF_VALS(pRigidBody,void,addLocalForceAtLocalPos, const VxVector&, NODEFAULT,const VxVector&, NODEFAULT,PForceMode,0,bool,true)


		/************************************************************************/
		/* Momentum                                                                     */
		/************************************************************************/
		DECLAREMETHOD_1(pRigidBody,void,setAngularMomentum,const VxVector&)
		DECLAREMETHOD_1(pRigidBody,void,setLinearMomentum,const VxVector&)
		DECLAREMETHOD_0(pRigidBody,VxVector,getAngularMomentum)
		DECLAREMETHOD_0(pRigidBody,VxVector,getLinearMomentum)
		
		/************************************************************************/
		/* collision + callbacks													*/
		/************************************************************************/
		DECLAREMETHOD_1(pRigidBody,void,setContactReportThreshold,float)
		DECLAREMETHOD_0(pRigidBody,float,setContactReportThreshold)

		DECLAREMETHOD_1(pRigidBody,void,setContactReportFlags,pContactPairFlags)
		DECLAREMETHOD_0(pRigidBody,int,getContactReportFlags)


		DECLAREMETHOD_2(pRigidBody,void,setContactScript,int,int)
		DECLAREMETHOD_2_WITH_DEF_VALS(pRigidBody,void,setTriggerScript,int,NODEFAULT,int,NODEFAULT,CK3dEntity*,NULL)
		DECLAREMETHOD_1(pRigidBody,void,setContactModificationScript,int)

		DECLAREMETHOD_2_WITH_DEF_VALS(pRigidBody,void,setSkinWidth,const float,NODEFAULT,CK3dEntity*,NULL)

		/************************************************************************/
		/* Pose :                                                                     */
		/************************************************************************/
		DECLAREMETHOD_1(pRigidBody,void,setPosition,const VxVector&)
		DECLAREMETHOD_1(pRigidBody,void,setRotation,const VxQuaternion&)
		DECLAREMETHOD_1(pRigidBody,void,translateLocalShapePosition,VxVector)

		/************************************************************************/
		/* Velocity :                                                                     */
		/************************************************************************/

		DECLAREMETHOD_1(pRigidBody,void,setLinearVelocity,const VxVector&)
		DECLAREMETHOD_1(pRigidBody,void,setAngularVelocity,const VxVector&)
		DECLAREMETHOD_0(pRigidBody,float,getMaxAngularSpeed)
		DECLAREMETHOD_0(pRigidBody,VxVector,getLinearVelocity)
		DECLAREMETHOD_0(pRigidBody,VxVector,getAngularVelocity)
		
		/************************************************************************/
		/* Mass                                                                     */
		/************************************************************************/
		

		DECLAREMETHOD_1(pRigidBody,void,setCMassOffsetGlobalPosition,VxVector)
		DECLAREMETHOD_1(pRigidBody,void,setCMassGlobalPosition,VxVector)
		DECLAREMETHOD_0(pRigidBody,VxVector,getCMassLocalPosition)
		DECLAREMETHOD_0(pRigidBody,VxVector,getCMassGlobalPosition)
		//DECLAREMETHOD_1(pRigidBody,void,setMass,float)
		DECLAREMETHOD_0(pRigidBody,float,getMass)
		DECLAREMETHOD_1(pRigidBody,void,setMassSpaceInertiaTensor,VxVector)
		DECLAREMETHOD_0(pRigidBody,VxVector,getMassSpaceInertiaTensor)
		DECLAREMETHOD_1(pRigidBody,void,setCMassOffsetLocalPosition,VxVector)


		DECLAREMETHOD_1(pRigidBody,void,setMassOffset,VxVector)
		DECLAREMETHOD_1(pRigidBody,void,setMaxAngularSpeed,float)

		/************************************************************************/
		/* Hull  */
		/************************************************************************/
		DECLAREMETHOD_2_WITH_DEF_VALS(pRigidBody,void,setBoxDimensions,const VxVector&,NODEFAULT,CKBeObject*,NULL)


		DECLAREMETHOD_0(pRigidBody,pWorld*,getWorld)
		DECLAREMETHOD_1(pRigidBody,pJoint*,isConnected,CK3dEntity*)
		DECLAREMETHOD_2(pRigidBody,pJoint*,isConnected,CK3dEntity*,int)
		//DECLAREMETHOD_2_WITH_DEF_VALS(pRigidBody,void,setBoxDimensions,const VxVector&,NODEFAULT,CKBeObject*,NULL)
		//DECLAREMETHOD_1_WITH_DEF_VALS(pRigidBody,float,getMass,CK3dEntity*,NULL)
		DECLAREMETHOD_0(pRigidBody,int,getHullType)

		DECLAREMETHOD_2(pRigidBody,void,setGroupsMask,CK3dEntity*,const pGroupsMask&)

		DECLAREMETHOD_0(pRigidBody,int,getFlags)
		DECLAREMETHOD_1(pRigidBody,VxVector,getPointVelocity,const VxVector&)
		DECLAREMETHOD_1(pRigidBody,VxVector,getLocalPointVelocity,const VxVector&)

		
		DECLAREMETHOD_0(pRigidBody,bool,isCollisionEnabled)

		DECLAREMETHOD_1(pRigidBody,void,setKinematic,int)
		DECLAREMETHOD_0(pRigidBody,int,isKinematic)

		DECLAREMETHOD_1(pRigidBody,void,enableGravity,int)
		DECLAREMETHOD_0(pRigidBody,bool,isAffectedByGravity)

		DECLAREMETHOD_1(pRigidBody,void,setSleeping,int)
		DECLAREMETHOD_0(pRigidBody,int,isSleeping)
		DECLAREMETHOD_1(pRigidBody,void,setLinearDamping,float)
		DECLAREMETHOD_1(pRigidBody,void,setAngularDamping,float)
		DECLAREMETHOD_1(pRigidBody,void,lockTransformation,BodyLockFlags)
		DECLAREMETHOD_1(pRigidBody,int,isBodyFlagOn,int)
		
		DECLAREMETHOD_1_WITH_DEF_VALS(pRigidBody,void,wakeUp,float,NX_SLEEP_INTERVAL)
		DECLAREMETHOD_1(pRigidBody,void,setSleepEnergyThreshold,float)
		DECLAREMETHOD_1(pRigidBody,void,setSolverIterationCount,int)
		DECLAREMETHOD_5_WITH_DEF_VALS(pRigidBody,bool,onSubShapeTransformation,bool,TRUE,bool,TRUE,bool,TRUE,CK3dEntity*,NULL,bool,true)


		DECLAREMETHOD_2_WITH_DEF_VALS(pRigidBody,void,setCollisionsGroup,int,NODEFAULT,CK3dEntity*,)
		DECLAREMETHOD_2_WITH_DEF_VALS(pRigidBody,void,enableCollision,bool,NODEFAULT,CK3dEntity*,NULL)

		DECLAREMETHOD_0(pRigidBody,bool,hasWheels);
		DECLAREMETHOD_0(pRigidBody,int,getNbWheels);
		
		DECLAREMETHOD_0(pRigidBody,int,getNbSubShapes);
		DECLAREMETHOD_0(pRigidBody,int,getNbSubBodies);





		DECLAREMETHOD_0(pRigidBody,int,getCollisionsGroup)
		DECLAREMETHOD_2(pRigidBody,int,updateMassFromShapes,float,float)
		DECLAREMETHOD_5_WITH_DEF_VALS(pRigidBody,int,addSubShape,CKMesh*,NULL,pObjectDescr,NODEFAULT,CK3dEntity*,NULL,VxVector,VxVector(),VxQuaternion,VxQuaternion())
		DECLAREMETHOD_3_WITH_DEF_VALS(pRigidBody,int,removeSubShape,CKMesh*,NODEFAULT,float,0.0,float,0.0)



		/************************************************************************/
		/* Joint																														 */
		/************************************************************************/
		DECLAREMETHOD_0(pRigidBody,int,getNbJoints);
		//DECLAREMETHOD_3_WITH_DEF_VALS(pRigidBody,pJoint*,getJointAtIndex,int,NODEFAULT,int&,NODEFAULT,CK3dEntity**,NULL)
		DECLAREMETHOD_1(pRigidBody,pJoint*,getJointAtIndex,int)
		DECLAREMETHOD_2(pRigidBody,pJoint*,getJoint,CK3dEntity*,JType)
		DECLAREMETHOD_1(pRigidBody,void,deleteJoint,pJoint*)
		DECLAREMETHOD_2(pRigidBody,void,deleteJoint,CK3dEntity*,JType)

	STOPVSLBIND
}
