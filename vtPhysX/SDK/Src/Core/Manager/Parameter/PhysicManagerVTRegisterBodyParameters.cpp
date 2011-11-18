#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "vtStructHelper.h"
#include "vtAttributeHelper.h"

#include "gConfig.h"


#define PHYSIC_JOINT_CAT "Physic Constraints"

using namespace vtTools::AttributeTools;
using namespace vtTools::ParameterTools;


StructurMember bodyDamping[]  =
{
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Linear Damping","0.1"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Angular Damping","0.1"),
};

StructurMember bodySleeping[]  =
{
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Linear Sleep Velocity","0.1"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Angular Sleep Velocity","0.1"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Sleep Energy Threshold","0.1"),
};

StructurMember bodyXMLSetup[]  =
{
	STRUCT_ATTRIBUTE(VTS_PHYSIC_ACTOR_XML_SETTINGS_INTERN,"Internal","None"),
	STRUCT_ATTRIBUTE(VTS_PHYSIC_ACTOR_XML_SETTINGS_EXTERN,"External","None"),
	STRUCT_ATTRIBUTE(VTS_PHYSIC_ACTOR_XML_IMPORT_FLAGS,"Import Flags","Stub"),
};

StructurMember bodyCCDSettings[]  =
{
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Motion Threshold","None"),
	STRUCT_ATTRIBUTE(VTF_PHYSIC_CCD_FLAGS,"Flags","Stub"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Mesh Scale","1.0f"),
	STRUCT_ATTRIBUTE(CKPGUID_BEOBJECT,"Shape Reference","None"),
};

StructurMember bodyCollisionsSettings[]  =
{
	STRUCT_ATTRIBUTE(VTE_PHYSIC_BODY_COLL_GROUP,"Collisions Group","All"),
	STRUCT_ATTRIBUTE(VTS_FILTER_GROUPS,"Group Mask","None"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Skin Width","-1.0"),
	//STRUCT_ATTRIBUTE(VTS_PHYSIC_CCD_SETTINGS,"CCD Settings","None"),
};

StructurMember bodyCollisionsSetup[]  =
{
	STRUCT_ATTRIBUTE(VTS_PHYSIC_COLLISIONS_SETTINGS,"Collisions Settings","None"),
	//STRUCT_ATTRIBUTE(VTS_PHYSIC_CCD_SETTINGS,"CCD Settings","0"),
};

StructurMember bodyOptimistationSettings[]  =
{
	STRUCT_ATTRIBUTE(VTF_BODY_TRANS_FLAGS,"Transformation Locks","None"),
	STRUCT_ATTRIBUTE(VTS_PHYSIC_DAMPING_PARAMETER,"Damping Settings","None"),
	STRUCT_ATTRIBUTE(VTS_PHYSIC_SLEEP_SETTINGS,"Sleeping Settings","None"),
	STRUCT_ATTRIBUTE(CKPGUID_INT,"Solver Iterations",""),
	STRUCT_ATTRIBUTE(VTE_PHYSIC_DOMINANCE_GROUP,"Dominance Group",""),
	STRUCT_ATTRIBUTE(CKPGUID_INT,"Compartment Id",""),
};

StructurMember bodyCommonSettings[]  =
{
	STRUCT_ATTRIBUTE(VTE_COLLIDER_TYPE,"Hull Type","1"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Density","1"),
	STRUCT_ATTRIBUTE(VTF_BODY_FLAGS,"Flags","Moving Object,World Gravity,Enabled,Collision"),
/*	STRUCT_ATTRIBUTE(VTF_BODY_TRANS_FLAGS,"Transformation Locks",""),*/
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"World","pWorldDefault"),
};

StructurMember bodyGeomtryOverride[]  =
{
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Offset Linear","0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_EULERANGLES,"Offset Angular","0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Offset Reference",""),
};

StructurMember bodyMassSetup[]  =
{
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"New Density","0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Total Mass","0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Offset Linear","0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_EULERANGLES,"Offset Angular","0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Mass Offset Reference","0.0"),
};

StructurMember bodySetup[]  =
{
	STRUCT_ATTRIBUTE(VTS_PHYSIC_ACTOR_XML_SETUP,"XML Links","0"),
	STRUCT_ATTRIBUTE(VTF_PHYSIC_BODY_COMMON_SETTINGS,"Common Settings","0"),
	/*STRUCT_ATTRIBUTE(VTS_PHYSIC_PIVOT_OFFSET,"Pivot","0"),
	STRUCT_ATTRIBUTE(VTS_PHYSIC_MASS_SETUP,"Mass Setup","0"),*/
	STRUCT_ATTRIBUTE(VTS_PHYSIC_COLLISIONS_SETTINGS,"Collisions Setup","0"),
	/*STRUCT_ATTRIBUTE(VTS_PHYSIC_ACTOR_OPTIMIZATION,"Optimization","0"),*/
};

StructurMember axisReferencedLength[]  =
{
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Value","0"),
	STRUCT_ATTRIBUTE(CKPGUID_BEOBJECT,"Reference Object","0"),
	STRUCT_ATTRIBUTE(CKPGUID_AXIS,"Local Axis","0"),
};



StructurMember customCapsule[]  =
{
	STRUCT_ATTRIBUTE(VTS_AXIS_REFERENCED_LENGTH,"Radius",""),
	STRUCT_ATTRIBUTE(VTS_AXIS_REFERENCED_LENGTH,"Width","0"),
};
StructurMember customConvexCylinder[]  =
{
	STRUCT_ATTRIBUTE(CKPGUID_INT,"Approximation","10"),
	STRUCT_ATTRIBUTE(VTS_AXIS_REFERENCED_LENGTH,"Radius",""),
	STRUCT_ATTRIBUTE(VTS_AXIS_REFERENCED_LENGTH,"Width","0"),
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Forward Axis","0.0,0.0,-1.0"),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Forward Axis Reference","0"),
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Down Axis","0.0,-1.0,0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Down Axis Reference","0"),
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Right Axis","1.0,0.0,0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Right Axis Reference","0"),
	STRUCT_ATTRIBUTE(CKPGUID_BOOL,"Build Lower Half Only","0"),
	STRUCT_ATTRIBUTE(VTF_CONVEX_FLAGS,"Convex Flags","0"),
};

//#define gSMapJDistance myStructJDistance
extern void	PObjectAttributeCallbackFunc(int AttribType,CKBOOL Set,CKBeObject *obj,void *arg);

typedef CKERROR (*bodyParameterDefaultFunction)(CKParameter*);

bodyParameterDefaultFunction bodyCreateFuncOld = NULL;
/*
#define REGISTER_CUSTOM_STRUCT(NAME,ENUM_TYPE,GUID,MEMBER_ARRAY,HIDDEN) DECLARE_STRUCT(ENUM_TYPE,NAME,GUID,MEMBER_ARRAY,STRUCT_SIZE(MEMBER_ARRAY)); \
	XArray<CKGUID> ListGuid##ENUM_TYPE = STRUCT_MEMBER_GUIDS(ENUM_TYPE);\
	pm->RegisterNewStructure(GUID,NAME,STRUCT_MEMBER_NAMES(ENUM_TYPE).Str(),ListGuid##ENUM_TYPE);\
	CKParameterTypeDesc* param_type##ENUM_TYPE=pm->GetParameterTypeDescription(GUID);\
	if (param_type##ENUM_TYPE && HIDDEN) param_type##ENUM_TYPE->dwFlags|=CKPARAMETERTYPE_HIDDEN;\
	_getCustomStructures().Insert(GUID,(CustomStructure*)&MEMBER_ARRAY)

*/

void bodyDefaultFunctionMerged(CKParameter*in)
{
	
	CKStructHelper sHelper(in);
	//if ( ==0 )	//happens when dev is being opened and loads a cmo with physic objects.
		

	XString msg;

	msg.Format("parameter members : %d",sHelper.GetMemberCount());
	if(bodyCreateFuncOld!=0 )
	{
		bodyCreateFuncOld(in);
		xLogger::xLog(XL_START,ELOGERROR,E_LI_MANAGER,msg.Str());
	}
	
	return;

//	CKParameter
	//CKAttributeManager::SetAttributeDefaultValue()


}
void PhysicManager::_RegisterBodyParameterFunctions()
{
	return;

	CKContext* ctx = GetContext();

	CKParameterManager *pm = ctx->GetParameterManager();
	CKParameterTypeDesc *param_desc = pm->GetParameterTypeDescription(VTF_PHYSIC_BODY_COMMON_SETTINGS);
	if( !param_desc ) return;
	if (param_desc->CreateDefaultFunction!=0)
	{
		xLogger::xLog(ELOGERROR,E_LI_AGEIA,"has function");
		bodyCreateFuncOld = param_desc->CreateDefaultFunction;
		param_desc->CreateDefaultFunction = (CK_PARAMETERCREATEDEFAULTFUNCTION)bodyDefaultFunctionMerged;
	}
	
	//param_desc->UICreatorFunction = CKActorUIFunc;
		//param_desc->UICreatorFunction = CKDoubleUIFunc;


}
void PhysicManager::_RegisterBodyParameters()
{


	CKParameterManager *pm = m_Context->GetParameterManager();
	CKAttributeManager* attman = m_Context->GetAttributeManager();


	int attRef=0;
	//################################################################
	//
	// Geometry Related 
	//

	//	Object and Axis Related Length :
	REGISTER_CUSTOM_STRUCT("pAxisReferencedLength",PS_AXIS_REFERENCED_LENGTH,VTS_AXIS_REFERENCED_LENGTH,axisReferencedLength,false);
	REGISTER_CUSTOM_STRUCT("pCustomConvexCylinder",PS_CUSTOM_CONVEX_CYLINDER_DESCR,VTS_PHYSIC_CONVEX_CYLDINDER_WHEEL_DESCR,customConvexCylinder,false);
	REGISTER_STRUCT_AS_ATTRIBUTE("pCustomConvexCylinder",PS_CUSTOM_CONVEX_CYLINDER_DESCR,PHYSIC_BODY_CAT,VTS_PHYSIC_CONVEX_CYLDINDER_WHEEL_DESCR,CKCID_3DOBJECT,customConvexCylinder,true);

	
	REGISTER_CUSTOM_STRUCT("pCapsule",PS_CAPSULE,VTS_CAPSULE_SETTINGS_EX,customCapsule,false);
	REGISTER_STRUCT_AS_ATTRIBUTE("pCapsule",PS_CAPSULE,PHYSIC_BODY_CAT,VTS_CAPSULE_SETTINGS_EX,CKCID_3DOBJECT,customCapsule,true);
    	

	//////////////////////////////////////////////////////////////////////////
	//
	//		Collision Common Structs : 
	//
	
	pm->RegisterNewFlags(VTF_COLLISIONS_EVENT_MASK,"pCollisionEventMask","Ignore=1,Start Touch=2,End Touch=4,Touch=8,Impact=16,Roll=32,Slide=64,Forces=128,Start Touch Force Threshold=256,End Touch Force Threshold=512,Touch Force Threshold=1024,Contact Modification=65536");
	
	pm->RegisterNewFlags(VTF_WHEEL_CONTACT_MODIFY_FLAGS,"pWheelContactModifyFlags","Point=1,Normal=2,Position=4,Force=8,Material=16");


	
	pm->RegisterNewFlags(VTF_CONTACT_MODIFY_FLAGS,"pContactModifyFlags","None=0,Min Impulse=1,Max Impulse=2,Error=4,Target=8,Local Position0=16,Local Position1=32,Local Orientation0=64,Local Orientation1=128,Static Friction0=256,Static Friction1=512,Dynamic Friction0=1024,Dynamic Friction1=2048,Restitution=4096,Force32=2147483648");
	pm->RegisterNewFlags(VTF_CONVEX_FLAGS,"pConvexFlags","Flip Normals=1,16 Bit Indices=2,Compute Convex=4,Inflate Convex=8,Uncompressed Normals=64");
	pm->RegisterNewFlags(VTF_TRIGGER,"pTriggerFlags","Disable=8,OnEnter=1,OnLeave=2,OnStay=4");
	pm->RegisterNewEnum(VTE_FILTER_OPS,"pFilterOp","And=0,Or=1,Xor=2,Nand=3,Nor=4,NXor=5");
	pm->RegisterNewFlags(VTE_FILTER_MASK,"pFilterMask","0,1,2,3");
	pm->RegisterNewStructure(VTS_FILTER_GROUPS,"pFilterGroups","bits0,bits1,bits2,bits3",VTE_FILTER_MASK,VTE_FILTER_MASK,VTE_FILTER_MASK,VTE_FILTER_MASK);
	pm->RegisterNewFlags(VTF_SHAPES_TYPE,"pShapesTypes","Static=1,Dynamic=2");

	//////////////////////////////////////////////////////////////////////////
	//
	//		Body Sub Structs : 
	//

	pm->RegisterNewFlags(VTF_BODY_FLAGS,"pBFlags","Moving Object=1,World Gravity=2,Collision=4,Kinematic Object=8,Sub Shape=16,Hierarchy=32,Add Attributes=64,Trigger Shape=128,Deformable=256,Collision Notify=512,Collisions Force=1024,Contact Modify=2048,Sleep=4096");
	pm->RegisterNewFlags(VTF_BODY_TRANS_FLAGS,"pBTFlags","FrozenPositionX=2,FrozenPositionY=4,FrozenPositionZ=8,FrozenRotationX=16,FrozenRotationY=32,FrozenRotationZ=64");
	pm->RegisterNewEnum(VTE_COLLIDER_TYPE,"pBHullType","Sphere=0,Box=1,Capsule=2,Plane=3,Mesh=4,Convex Mesh=5,Height Field=6,Wheel=7,Cloth=8,Convex Cylinder");

	pm->RegisterNewStructure(VTS_PHYSIC_PARAMETER,"pObject", "Geometry,Physic Flags,Density,Skin Width,Mass Offset,Pivot Offset,Hierarchy,World,New Density,Total Mass,Collision Group",VTE_COLLIDER_TYPE,VTF_BODY_FLAGS,CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_VECTOR,CKPGUID_VECTOR,CKPGUID_BOOL,CKPGUID_3DENTITY,CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_INT);

	int currentAttributeType = -1;

	att_physic_object= attman->RegisterNewAttributeType("Object",VTS_PHYSIC_PARAMETER,CKCID_3DOBJECT);
	attman->SetAttributeDefaultValue(att_physic_object,"1;Moving Object,World Gravity,Enabled,Collision;1;-1;0,0,0;0,0,0;FALSE,pDefaultWorld");
	
	attman->SetAttributeCategory(att_physic_object,"Physic");


	pm->RegisterNewEnum(VTE_BODY_FORCE_MODE,"pBForceMode","Force=0,Impulse=1,Velocity Change=2,Smooth Impulse=3,Smooth Velocity Change=4,Acceleration=5");
	attman->SetAttributeCategory(att_physic_limit,"Physic");

	//////////////////////////////////////////////////////////////////////////
	//
	//	Capsule : 
	//
	pm->RegisterNewStructure(VTS_CAPSULE_SETTINGS,"Capsule", "Local Length Axis,Local Radius Axis,Length,Radius",CKPGUID_AXIS,CKPGUID_AXIS,CKPGUID_FLOAT,CKPGUID_FLOAT);

	CKParameterTypeDesc* param_type=pm->GetParameterTypeDescription(VTS_CAPSULE_SETTINGS);
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_HIDDEN;


	/*
	att_capsule = attman->RegisterNewAttributeType("Capsule",VTS_CAPSULE_SETTINGS,CKCID_BEOBJECT);
	attman->SetAttributeDefaultValue(att_capsule,"1;0;-1.0;-1.0f");
	attman->SetAttributeCategory(att_capsule,"Physic");
	*/

	//----------------------------------------------------------------
	//
	// copy flags
	//
	pm->RegisterNewFlags(VTF_PHYSIC_ACTOR_COPY_FLAGS,"pBCopyFlags","Physics=1,Shared=2,Pivot=4,Mass=8,Collision=16,CCD=32,Material=64,Optimization=128,Capsule=256,Convex Cylinder=512,Force=1024,Velocities=2048,Joints=4096,Limit Planes=8192,Swap Joint References=16384,Override Body Flags=32768,Copy IC=65536,Restore IC=131072");
	/*
	param_type=pm->GetParameterTypeDescription(VTF_PHYSIC_ACTOR_COPY_FLAGS);
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_HIDDEN;
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_TOSAVE;
	*/

    
	//////////////////////////////////////////////////////////////////////////
	//
	//		Body Collision Setup 
	//


	//////////////////////////////////////////////////////////////////////////	//////////////////////////////////////////////////////////////////////////
	//
	//		CCD Settings : 
	//	
	//  Custom Enumeration to setup ccd flags
	pm->RegisterNewFlags(VTF_PHYSIC_CCD_FLAGS,"pBCCDFlags","None=1,Shared=2,DynamicDynamic=4");
	REGISTER_CUSTOM_STRUCT("pBCCDSettings",PS_B_CCD,VTS_PHYSIC_CCD_SETTINGS,bodyCCDSettings,GC_SHOWPARAMETER);
	REGISTER_STRUCT_AS_ATTRIBUTE("pBCCDSettings",PS_B_CCD,PHYSIC_BODY_CAT,VTS_PHYSIC_CCD_SETTINGS,CKCID_3DOBJECT,bodyCCDSettings,true,attRef);


	//////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
	//
	// Collisions Settings
	//
	pm->RegisterNewEnum(VTE_PHYSIC_BODY_COLL_GROUP,"pBCollisionsGroup","All=0,MyObstacles=1,MyWheels=2");
	param_type=pm->GetParameterTypeDescription(VTE_PHYSIC_BODY_COLL_GROUP);
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_USER;
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_TOSAVE;


	REGISTER_CUSTOM_STRUCT("pBCollisionSettings",PS_B_COLLISON,VTS_PHYSIC_COLLISIONS_SETTINGS,bodyCollisionsSettings,GC_SHOWPARAMETER);
	REGISTER_STRUCT_AS_ATTRIBUTE("pBCollisionSettings",PS_B_COLLISON,PHYSIC_BODY_CAT,VTS_PHYSIC_COLLISIONS_SETTINGS,CKCID_3DOBJECT,bodyCollisionsSettings,true,attRef);


	/*		Merged */
	REGISTER_CUSTOM_STRUCT("pBCSetup",PS_B_COLLISION_SETUP,VTS_PHYSIC_COLLISIONS_SETUP,bodyCollisionsSetup,GC_SHOWPARAMETER );

	//////////////////////////////////////////////////////////////////////////
	//
	// XML Setup 
	pm->RegisterNewFlags(VTS_PHYSIC_ACTOR_XML_IMPORT_FLAGS,"pBXMLFlags","None=0,Stub=1");
	param_type=pm->GetParameterTypeDescription(VTS_PHYSIC_ACTOR_XML_IMPORT_FLAGS);
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_HIDDEN;

	REGISTER_CUSTOM_STRUCT("pBXMLSetup",PS_BODY_XML_SETUP,VTS_PHYSIC_ACTOR_XML_SETUP,bodyXMLSetup,false);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Common
	REGISTER_CUSTOM_STRUCT("pBCommon",PS_BODY_COMMON,VTF_PHYSIC_BODY_COMMON_SETTINGS,bodyCommonSettings,false);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Sleep
	REGISTER_CUSTOM_STRUCT("pBSleepSettings",PS_B_SLEEPING,VTS_PHYSIC_SLEEP_SETTINGS,bodySleeping,GC_SHOWPARAMETER);


	//////////////////////////////////////////////////////////////////////////
	//
	//	Damping
	REGISTER_CUSTOM_STRUCT("pBDamping",PS_B_DAMPING,VTS_PHYSIC_DAMPING_PARAMETER,bodyDamping,GC_SHOWPARAMETER);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Optimization
	REGISTER_CUSTOM_STRUCT("pBOptimisation",PS_B_OPTIMISATION,VTS_PHYSIC_ACTOR_OPTIMIZATION,bodyOptimistationSettings,GC_SHOWPARAMETER);
	REGISTER_STRUCT_AS_ATTRIBUTE("pBOptimisation",PS_B_OPTIMISATION,PHYSIC_BODY_CAT,VTS_PHYSIC_ACTOR_OPTIMIZATION,CKCID_3DOBJECT,bodyOptimistationSettings,true,attRef);

	

	//////////////////////////////////////////////////////////////////////////
	//
	//	Geometry
	REGISTER_CUSTOM_STRUCT("pBPivotSettings",PS_B_PIVOT,VTS_PHYSIC_PIVOT_OFFSET,bodyGeomtryOverride,GC_SHOWPARAMETER);
	REGISTER_STRUCT_AS_ATTRIBUTE("pBPivotSettings",PS_B_PIVOT,PHYSIC_BODY_CAT,VTS_PHYSIC_PIVOT_OFFSET,CKCID_3DOBJECT,bodyGeomtryOverride,true,attRef);


	//////////////////////////////////////////////////////////////////////////
	//
	//	Mass Override
	REGISTER_CUSTOM_STRUCT("pBMassSettings",PS_B_MASS,VTS_PHYSIC_MASS_SETUP,bodyMassSetup,false);
	param_type=pm->GetParameterTypeDescription(VTS_PHYSIC_MASS_SETUP);
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_USER;
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_TOSAVE;
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_HIDDEN;

	REGISTER_STRUCT_AS_ATTRIBUTE("pBMassSettings",PS_B_MASS,PHYSIC_BODY_CAT,VTS_PHYSIC_MASS_SETUP,CKCID_3DOBJECT,bodyMassSetup,true,attRef);






	//////////////////////////////////////////////////////////////////////////
	//
	// this is the new replacement for the "Object" attribute.	
	//	
	REGISTER_CUSTOM_STRUCT("pBSetup",PS_BODY_SETUP,VTS_PHYSIC_ACTOR,bodySetup,FALSE);
	REGISTER_STRUCT_AS_ATTRIBUTE("pBSetup",PS_BODY_SETUP,PHYSIC_BODY_CAT,VTS_PHYSIC_ACTOR,CKCID_3DOBJECT,bodySetup,attRef);


}
