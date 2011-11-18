#ifndef __PMANAGERTYPES_H__
#define __PMANAGERTYPES_H__

#include "pNxSDKParameter.h"


typedef enum pManagerFlags
{	
	PMF_DONT_DELETE_SCENES=1<<1,
	PMF_DONT_USE_HARDWARE=1<<2,
};

struct pTriggerEntry
{

	NxShape *shapeA;
	NxShape *shapeB;
	int triggerEvent;
	bool triggered;
	CK3dEntity *triggerBody;
	CK3dEntity *otherObject;
	CK3dEntity *triggerShapeEnt;

	pRigidBody *triggerBodyB;
	pRigidBody *otherBodyB;


	pTriggerEntry()
	{
		shapeA = shapeB  = NULL;
		triggerShapeEnt =otherObject = triggerBody = NULL;
		 triggerBodyB = otherBodyB = NULL;
		triggerEvent;
		triggered  = false;
	}
};

typedef XArray<pTriggerEntry>pTriggerArray;


//################################################################
//
// Help Structures, used by the manager only 
//

//----------------------------------------------------------------
//
//! \brief Container to maintain multiple worlds
//
typedef XHashTable<pWorld*,CK3dEntity*> pWorldMap;
typedef XHashTable<pWorld*,CK3dEntity*>::Iterator pWorldMapIt;

typedef XArray<CK_ID>pBodyList;
typedef XArray<CK_ID>::Iterator pBodyListIterator;


typedef XHashTable<pRigidBodyRestoreInfo*,CK_ID> pRestoreMap;
typedef XHashTable<pRigidBodyRestoreInfo*,CK_ID>::Iterator pRestoreMapIt;



//----------------------------------------------------------------
//
//! \brief Function pointer. Used in custom parameter structures to populate
//	found xml types
//
typedef XString (pFactory::*PFEnumStringFunction)(const TiXmlDocument * doc);

//----------------------------------------------------------------
//
//! \brief Function pointer to link object registrations per attribute type 
//
typedef int (*ObjectRegisterFunction)(CK3dEntity*,int,bool,bool);

//----------------------------------------------------------------
//
//! \brief Meta data for ObjectRegisterFunction. Used for attribute callbacks.
//
struct ObjectRegistration
{
	CKGUID guid;
	ObjectRegisterFunction rFunc;
	ObjectRegistration(CKGUID _guid,ObjectRegisterFunction _func) : 
	guid(_guid) , 
		rFunc(_func)
	{

	}
};

//----------------------------------------------------------------
//
//! \brief	Meta data for attributes callbacks when the scene is playing. <br>
//!			Virtools is not initializing the attributes parameter correctly.
//			Ths structur is used to track data to the next frame.  
//
struct pAttributePostObject
{

	CK_ID objectId;
	ObjectRegisterFunction func;
	int attributeID;

	pAttributePostObject() :
	objectId(-1) , func(NULL) , attributeID(-1){}

	pAttributePostObject(CK_ID _id,ObjectRegisterFunction _func,int _attributeID) :
	objectId(_id) , 
		func(_func) , 
		attributeID(_attributeID)
	{

	}

};
//----------------------------------------------------------------
//
//! \brief Container type to store objects with uninitialized attribute parameters.
//
typedef XArray<pAttributePostObject>PostRegistrationArrayType;
typedef XArray<pAttributePostObject>::Iterator PostRegistrationArrayIteratorType;


//----------------------------------------------------------------
//
//! \brief Container to store custom function pointer per attribute type
//
typedef XHashTable<ObjectRegisterFunction,int>AttributeFunctionArrayType;
typedef XHashTable<ObjectRegisterFunction,int>::Iterator AttributeFunctionArrayIteratorType;

//----------------------------------------------------------------
//
//! \brief CustomParametersArrayType is responsible to track custom structures and its default value.
//!
typedef XHashTable<vtTools::ParameterTools::CustomStructure*,CKGUID>CustomParametersArrayType;
typedef XHashTable<vtTools::ParameterTools::CustomStructure*,CKGUID>::Iterator CustomParametersArrayIteratorType;





struct pSDKParameters
{
	float SkinWidth;
	float DefaultSleepLinVelSquared ;
	float DefaultSleepAngVel_squared; 
	float BounceThreshold  ; 
	float DynFrictScaling  ; 
	float StaFrictionScaling; 
	float MaxAngularVelocity  ; 
	float ContinuousCD  ; 
	float AdaptiveForce  ; 
	float CollVetoJointed   ; 
	float TriggerTriggerCallback  ; 
	float CCDEpsilon  ; 
	float SolverConvergenceThreshold ; 
	float BBoxNoiseLevel  ; 
	float ImplicitSweepCacheSize  ; 
	float DefaultSleepEnergy  ; 
	float ConstantFluidMaxPackets  ; 
	float ConstantFluidMaxParticlesPerStep ; 
	float AsynchronousMeshCreation  ; 
	float ForceFieldCustomKernelEpsilon  ; 
	float ImprovedSpringSolver ; 
	int disablePhysics;

	pSDKParameters()
	{

		SkinWidth  = 0.25f;
		DefaultSleepLinVelSquared = 0.15f*0.15f;
		DefaultSleepAngVel_squared  = 0.15f*0.15f;  
		BounceThreshold  = -2.0f ; 
		DynFrictScaling =1.0f; 
		StaFrictionScaling  = 1.0f; 
		MaxAngularVelocity = 7.0f ; 
		ContinuousCD  = 0.0f; 
		AdaptiveForce  = 1.0f; 
		CollVetoJointed   = 1.0f; 
		TriggerTriggerCallback= 1.0f ; 
		CCDEpsilon  = 0.01f; 
		SolverConvergenceThreshold  = 0.0f ; 
		BBoxNoiseLevel =0.001f ; 
		ImplicitSweepCacheSize = 5.0f  ; 
		DefaultSleepEnergy  = 0.005f; 
		ConstantFluidMaxPackets  = 925.0f ; 
		ConstantFluidMaxParticlesPerStep = 4096 ; 
		ImprovedSpringSolver = 1.0f; 
		disablePhysics = 0;
	}
};


class pRemoteDebuggerSettings
{

public :
	XString mHost;
	int port;
	int enabled;

	pRemoteDebuggerSettings()
	{
		mHost= "localhost";
		port  = 5425;
		enabled = 0;
	}
};



#endif // __PMANAGERTYPES_H__