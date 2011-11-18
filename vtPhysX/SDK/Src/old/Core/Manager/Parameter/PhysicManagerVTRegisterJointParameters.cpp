#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "vtStructHelper.h"

#define PHYSIC_JOINT_CAT "Physic Constraints"

using namespace vtTools::AttributeTools;
using namespace vtTools::ParameterTools;




StructurMember myStructJBall[]  =
{
		STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Body B","BodyB"),

		STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Anchor","0,0,0"),
		STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Anchor Reference",""),

		STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Limit Swing Axis","0,0,0"),
		
		STRUCT_ATTRIBUTE(VTE_JOINT_PROJECTION_MODE,"Projection Mode","None"),
		STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Projection Distance","0.0"),
		STRUCT_ATTRIBUTE(CKPGUID_BOOL,"Collision","TRUE"),
		
		STRUCT_ATTRIBUTE(VTS_JLIMIT,"Swing Limit","0.0,0.0,0.0"),
		STRUCT_ATTRIBUTE(VTS_JLIMIT,"Twist High Limit ","0.0,0.0,0.0"),
		STRUCT_ATTRIBUTE(VTS_JLIMIT,"Twist Low Limit ","0.0,0.0,0.0"),

		STRUCT_ATTRIBUTE(VTS_JOINT_SPRING,"Swing Spring","0.0,0.0,0.0"),
		STRUCT_ATTRIBUTE(VTS_JOINT_SPRING,"Twist Spring","0.0,0.0,0.0"),
		STRUCT_ATTRIBUTE(VTS_JOINT_SPRING,"Joint Spring","0.0,0.0,0.0"),

		STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Maximum Force","0.0"),
		STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Maximum Torque","0.0"),

};


StructurMember myStructJDistance[]  =
{
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Body B","BodyB"),
	
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Local 0 Anchor","0,0,0"),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Local 0 Reference",""),
	
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Local 1 Anchor","0,0,0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Local 1 Reference","pDefaultWorld"),

	STRUCT_ATTRIBUTE(CKPGUID_BOOL,"Collision","TRUE"),
	
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Minimum Distance","0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Maximum Distance","0.0"),
	STRUCT_ATTRIBUTE(VTS_JOINT_SPRING,"Spring","0.0f,0.0f,0.0f"),

	
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Maximum Force","0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Maximum Torque","0.0"),

};


StructurMember myStructJFixed[]  =
{
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Body B","BodyB"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Maximum Force","0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Maximum Torque","0.0"),

};

StructurMember JPrismaticMemberTable[]  =
{
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Body B","BodyB"),
	
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Anchor","0,0,0"),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Anchor Reference",""),
	
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Axis","0,0,0"),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Axis Reference",""),

	STRUCT_ATTRIBUTE(CKPGUID_BOOL,"Collision",""),

	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Maximum Force","0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Maximum Torque","0.0"),
	
};

StructurMember JCylindricalMemberTable[]  =
{
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Body B","BodyB"),

	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Anchor","0,0,0"),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Anchor Reference",""),

	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Axis","0,0,0"),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Axis Reference",""),

	STRUCT_ATTRIBUTE(CKPGUID_BOOL,"Collision",""),

	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Maximum Force","0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Maximum Torque","0.0"),
};

StructurMember JPointInPlaneMemberTable[]  =
{
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Body B","BodyB"),

		STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Anchor","0,0,0"),
		STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Anchor Reference",""),

		STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Axis","0,0,0"),
		STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Axis Reference",""),

		STRUCT_ATTRIBUTE(CKPGUID_BOOL,"Collision",""),

		STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Maximum Force","0.0"),
		STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Maximum Torque","0.0"),
};

StructurMember JPointOnLineMemberTable[]  =
{
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Body B","BodyB"),

		STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Anchor","0,0,0"),
		STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Anchor Reference",""),

		STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Axis","0,0,0"),
		STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Axis Reference",""),

		STRUCT_ATTRIBUTE(CKPGUID_BOOL,"Collision",""),

		STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Maximum Force","0.0"),
		STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Maximum Torque","0.0"),
};

StructurMember JRevoluteMemberTable[]  =
{
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Body B","BodyB"),
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Anchor","0,0,0"),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Anchor Reference",""),

	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Axis","0,0,0"),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Axis Reference",""),

	STRUCT_ATTRIBUTE(CKPGUID_BOOL,"Collision","TRUE"),
	
	STRUCT_ATTRIBUTE(VTE_JOINT_PROJECTION_MODE,"Projection Mode","None"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Projection Distance","0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Projection Angle","0.3"),

	STRUCT_ATTRIBUTE(VTS_JOINT_SPRING,"Spring","0.0,0.0,0.0"),
	STRUCT_ATTRIBUTE(VTS_JLIMIT,"Limit High","0.0,0.0,0.0"),
	STRUCT_ATTRIBUTE(VTS_JLIMIT,"Limit Low","0.0,0.0,0.0"),
	STRUCT_ATTRIBUTE(VTS_JOINT_MOTOR,"Motor","0.0,0.0,0.0"),

	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Maximum Force","0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Maximum Torque","0.0"),


};



StructurMember JLimitPlaneMemberTable[]  =
{
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Body B","BodyB"),
	STRUCT_ATTRIBUTE(VTE_JOINT_TYPE,"Target Joint Type","JT_Any"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Restitution","0.0f"),
	STRUCT_ATTRIBUTE(CKPGUID_BOOL,"Point is on Body","False"),
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Limit Point",""),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Limit Point Reference",""),
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Normal",""),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Normal Up Reference",""),
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Point in Plane",""),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Point in Plane Reference",""),

};

StructurMember JD6Members[]  =
{
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Body B","BodyB"),
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Anchor","0,0,0"),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Anchor Reference",""),

	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Axis","0,0,0"),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Axis Reference",""),
	STRUCT_ATTRIBUTE(VTF_JOINT_D6_AXIS_MASK,"Axis Mask",""),
	STRUCT_ATTRIBUTE(VTS_JOINT_D6_AXIS_ITEM,"X",""),
	STRUCT_ATTRIBUTE(VTS_JOINT_D6_AXIS_ITEM,"Y",""),
	STRUCT_ATTRIBUTE(VTS_JOINT_D6_AXIS_ITEM,"Z",""),
	STRUCT_ATTRIBUTE(VTS_JOINT_D6_AXIS_ITEM,"Swing 1",""),
	STRUCT_ATTRIBUTE(VTS_JOINT_D6_AXIS_ITEM,"Swing 2",""),
	STRUCT_ATTRIBUTE(VTS_JOINT_D6_AXIS_ITEM,"Twist Low",""),
	STRUCT_ATTRIBUTE(VTS_JOINT_D6_AXIS_ITEM,"Twist High",""),
};

StructurMember JD6AxisItem[]  =
{
	STRUCT_ATTRIBUTE(VTE_JOINT_MOTION_MODE_AXIS,"Axis","X"),
	STRUCT_ATTRIBUTE(VTS_JOINT_SLIMIT,"Limit",""),
};

#define gSMapJDistance myStructJDistance
#define gSMapJFixed myStructJFixed
#define gSMapJBall myStructJBall
#define gSMapJPrismatic JPrismaticMemberTable
#define gSMapJRevolute JRevoluteMemberTable
#define gSMapJCylindrical JCylindricalMemberTable
#define gSMapJPointInPlane JPointInPlaneMemberTable
#define gSMapJPointOnLine JPointOnLineMemberTable
#define gSMapJLimitPlane JLimitPlaneMemberTable

extern void	PObjectAttributeCallbackFunc(int AttribType,CKBOOL Set,CKBeObject *obj,void *arg);
void PhysicManager::_RegisterJointParameters()
{

	
	CKParameterManager *pm = m_Context->GetParameterManager();
	CKAttributeManager* attman = m_Context->GetAttributeManager();


	//----------------------------------------------------------------
	//
	// D6 Help Structures
	//

	pm->RegisterNewEnum(VTE_JOINT_MOTION_MODE,"pJMotionMode","Locked=0,Limited=1,Free=2");
	pm->RegisterNewStructure(VTS_JOINT_SLIMIT,"pJD6SLimit","Damping,Spring,Value,Restitution",CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_ANGLE,CKPGUID_FLOAT);

	//----------------------------------------------------------------
	//
	// Types for D6 - Joint - Attribute only  :
	//

	
	//SetWindowLong(m_hWnd,GWL_USERDATA,(LONG)this_mod); // set our user data to a "this" pointer
	/*
	pm->RegisterNewEnum(VTF_JOINT_D6_AXIS_MASK,"pD6AxisMask","X=1,Y=2,Z=4,Swing1=8,Swing2=16,Twist Low=32,Twist High=64");
	REGISTER_CUSTOM_STRUCT("pJD6AxisItem",PS_D6_AXIS_ITEM,VTS_JOINT_D6_AXIS_ITEM,JD6AxisItem,TRUE);
	REGISTER_CUSTOM_STRUCT("pJD6",PS_D6,VTS_JOINT_D6,JD6Members,TRUE);
	REGISTER_STRUCT_AS_ATTRIBUTE("pJD6",PS_D6,PHYSIC_JOINT_CAT,VTS_JOINT_D6,CKCID_BEOBJECT,JD6Members,true,attRef);

	*/

	int attRef=0;
	pm->RegisterNewEnum(VTE_JOINT_TYPE,"pJointType","None=-1,Prismatic=0,Revolute=1,Cylindrical=2,Spherical=3,Point On Line=4,Point In Plane=5,Distance=6,Pulley=7,Fixed=8,D6=9");
	
	pm->RegisterNewEnum(VTE_PHYSIC_JDRIVE_TYPE,"pJD6DriveType","Disabled=0,Position=1,Velocity=2");
	pm->RegisterNewStructure(VTS_JOINT_DRIVE,"pJD6Drive","Damping,Spring,Force Limit,Drive Type",CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_FLOAT,VTE_PHYSIC_JDRIVE_TYPE);

	pm->RegisterNewStructure(VTS_JOINT_SPRING,"pJSpring","Damper,Spring,Target Value",CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_FLOAT);
	pm->RegisterNewStructure(VTS_JLIMIT,"pJLimit","Value,Restitution,Hardness",CKPGUID_ANGLE,CKPGUID_FLOAT,CKPGUID_FLOAT);


	pm->RegisterNewEnum(VTE_JOINT_MOTION_MODE_AXIS,"pJD6Axis","Twist=0,Swing1=1,Swing2=2,X=3,Y=4,Z=5");
	pm->RegisterNewEnum(VTE_JOINT_DRIVE_AXIS,"pJD6DriveAxis","Twist=0,Swing=1,Slerp=2,X=3,Y=4,Z=5");
	pm->RegisterNewEnum(VTE_JOINT_LIMIT_AXIS,"pJD6LimitAxis","Linear=0,Swing1=1,Swing2,Twist High,Twist Low");

	pm->RegisterNewEnum(VTE_JOINT_PROJECTION_MODE,"pJProjectionMode","None=0,Point MinDist=1,Linear MindDist");
	pm->RegisterNewStructure(VTS_JOINT_MOTOR,"pJMotor","Target Velocity,Maximum Force,Free Spin",CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_BOOL);

	REGISTER_CUSTOM_STRUCT("pJDistance",PS_JDISTANCE_MEMBERS,VTS_JOINT_DISTANCE,gSMapJDistance,FALSE);
	REGISTER_STRUCT_AS_ATTRIBUTE("pJDistance",PS_JDISTANCE_MEMBERS,PHYSIC_JOINT_CAT,VTS_JOINT_DISTANCE,CKCID_3DOBJECT,gSMapJDistance,true,attRef);

	REGISTER_CUSTOM_STRUCT("pJFixed",PS_JFIXED_MEMBERS,VTS_JOINT_FIXED,gSMapJFixed,FALSE);
	REGISTER_STRUCT_AS_ATTRIBUTE("pJFixed",PS_JFIXED_MEMBERS,PHYSIC_JOINT_CAT,VTS_JOINT_FIXED,CKCID_3DOBJECT,gSMapJFixed,true,attRef);

	REGISTER_CUSTOM_STRUCT("pJBall",PS_JBALL_MEMBERS,VTS_JOINT_BALL,gSMapJBall,FALSE);
	REGISTER_STRUCT_AS_ATTRIBUTE("pJBall",PS_JBALL_MEMBERS,PHYSIC_JOINT_CAT,VTS_JOINT_BALL,CKCID_3DOBJECT,gSMapJBall,true,attRef);

	REGISTER_CUSTOM_STRUCT("pJPrismatic",PS_JPRISMATIC_MEMBERS,VTS_JOINT_PRISMATIC ,gSMapJPrismatic,FALSE);
	REGISTER_STRUCT_AS_ATTRIBUTE("pJPrismatic",PS_JPRISMATIC_MEMBERS,PHYSIC_JOINT_CAT,VTS_JOINT_PRISMATIC ,CKCID_BEOBJECT,gSMapJPrismatic,true,attRef);

	REGISTER_CUSTOM_STRUCT("pJCylindrical",PS_JCYLINDRICAL_MEMBERS,VTS_JOINT_CYLINDRICAL,gSMapJCylindrical,FALSE);
	REGISTER_STRUCT_AS_ATTRIBUTE("pJCylindrical",PS_JCYLINDRICAL_MEMBERS,PHYSIC_JOINT_CAT,VTS_JOINT_CYLINDRICAL,CKCID_BEOBJECT,gSMapJCylindrical,true,attRef);

	REGISTER_CUSTOM_STRUCT("pJRevolute",PS_JREVOLUTE,VTS_JOINT_REVOLUTE,gSMapJRevolute,FALSE);
	REGISTER_STRUCT_AS_ATTRIBUTE("pJRevolute",PS_JREVOLUTE,PHYSIC_JOINT_CAT,VTS_JOINT_REVOLUTE ,CKCID_BEOBJECT,gSMapJRevolute,true,attRef);

	REGISTER_CUSTOM_STRUCT("pJPointInPlane",PS_JPOINT_IN_PLANE_MEMBERS,VTS_JOINT_POINT_IN_PLANE,gSMapJPointInPlane,FALSE);
	REGISTER_STRUCT_AS_ATTRIBUTE("pJPointInPlane",PS_JPOINT_IN_PLANE_MEMBERS,PHYSIC_JOINT_CAT,VTS_JOINT_POINT_IN_PLANE,CKCID_BEOBJECT,gSMapJPointInPlane,true,attRef);

	REGISTER_CUSTOM_STRUCT("pJPointOnLine",PS_JPOINT_ON_LINE_MEMBERS,VTS_JOINT_POINT_ON_LINE,gSMapJPointOnLine,FALSE);
	REGISTER_STRUCT_AS_ATTRIBUTE("pJPointOnLine",PS_JPOINT_ON_LINE_MEMBERS,PHYSIC_JOINT_CAT,VTS_JOINT_POINT_ON_LINE,CKCID_BEOBJECT,gSMapJPointOnLine,true,attRef);

	REGISTER_CUSTOM_STRUCT("pJLimitPlane",PS_JLIMIT_PLANE_MEMBERS,VTS_PHYSIC_JLIMIT_PLANE,gSMapJLimitPlane,FALSE);
	REGISTER_STRUCT_AS_ATTRIBUTE("pJLimitPlane",PS_JLIMIT_PLANE_MEMBERS,PHYSIC_JOINT_CAT,VTS_PHYSIC_JLIMIT_PLANE,CKCID_BEOBJECT,gSMapJLimitPlane,true,attRef);





	populateAttributeFunctions();
	_RegisterAttributeCallbacks();
    
}
