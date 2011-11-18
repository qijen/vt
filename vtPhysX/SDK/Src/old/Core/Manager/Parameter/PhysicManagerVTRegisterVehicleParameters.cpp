#include <StdAfx.h>
#include "vtPhysXAll.h"
#include "vtAttributeHelper.h"
#include <vtStructHelper.h>


using namespace vtTools::AttributeTools;
using namespace vtTools::ParameterTools;

vtTools::ParameterTools::StructurMember breakTable[]  =
{
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"0","250"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"1","250"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"2","300"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"3","350"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"4","450"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"5","575"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"6","625"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"7","700"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"8","1000"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"9","1000"),
};

vtTools::ParameterTools::StructurMember myStructWheelContactData[]  =
{
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Contact Point",""),
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Contact Normal",""),
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Longitudes Direction",""),
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Lateral Direction",""),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Contact Force",""),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Longitudes Slip",""),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Lateral Slip",""),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Longitudes Impulse",""),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Lateral Impulse",""),
	STRUCT_ATTRIBUTE(VTS_MATERIAL,"Other Material",""),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Contact Pos",""),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"Colliding Entity",""),
};

void PhysicManager::_RegisterVehicleParameters()
{
	CKParameterManager *pm = m_Context->GetParameterManager();
	CKAttributeManager* attman = m_Context->GetAttributeManager();

	CKParameterTypeDesc* param_type = NULL;

	//################################################################
	//
	// Tire Function 
	//
	pm->RegisterNewStructure(VTF_VWTIRE_SETTINGS,"pTireFunction",
		"XML Link,Extremum Slip,Extremum Value,Asymptote Slip,Asymptote Value,Stiffness Factor",
		VTE_XML_TIRE_SETTINGS,CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_FLOAT);	


	//################################################################
	//
	// Motor related 
	//
	pm->RegisterNewStructure(VTS_VMOTOR_ENTRY,"RPM / Newton Meter","RPM,Newton Meter",CKPGUID_FLOAT,CKPGUID_FLOAT);
	param_type=pm->GetParameterTypeDescription(VTS_VMOTOR_ENTRY);
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_HIDDEN;

	pm->RegisterNewStructure(VTS_VMOTOR_TVALUES,"pVehicleMotor Torques","1,2,3,4,5,6,7",		VTS_VMOTOR_ENTRY,VTS_VMOTOR_ENTRY,		VTS_VMOTOR_ENTRY,VTS_VMOTOR_ENTRY,		VTS_VMOTOR_ENTRY,VTS_VMOTOR_ENTRY,		VTS_VMOTOR_ENTRY);


	pm->RegisterNewStructure(VTS_VGEAR_RATIO_ENTRY,"Gear / Ratio","Gear,Ratio",CKPGUID_INT,CKPGUID_FLOAT);
	param_type=pm->GetParameterTypeDescription(VTS_VGEAR_RATIO_ENTRY);
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_HIDDEN;
	pm->RegisterNewStructure(VTS_VGEAR_RATIOS,"pVehicleGear List","1,2,3,4,5,6,7",		VTS_VGEAR_RATIO_ENTRY,VTS_VGEAR_RATIO_ENTRY,		VTS_VGEAR_RATIO_ENTRY,VTS_VGEAR_RATIO_ENTRY,		VTS_VGEAR_RATIO_ENTRY,VTS_VGEAR_RATIO_ENTRY,		VTS_VGEAR_RATIO_ENTRY);

	//################################################################
	//
	// Vehicle Common
	//
	pm->RegisterNewFlags(VTF_VSTATE_FLAGS,"pVehicleStateFlags","Is Moving=1,Is Accelerated=2,Is Accelerated Forward=4,Is Accelerated Backward=8,All Wheels On Ground=16,Is Falling=32,Handbrake=64,Is Braking=128,Is Steering=256,Has Gearbox=512,Has Motor=1024");
	pm->RegisterNewEnum(VTE_BRAKE_LEVEL,"pVehicleBreakLevel","No Break=-1,Small=0,Medium=1,High=2");
	pm->RegisterNewEnum(VTE_BRAKE_XML_LINK,"pVehicleXMLBrakeSettings","Stub=1");
	pm->RegisterNewEnum(VTE_VEHICLE_XML_LINK,"pVehicleXMLLink","Stub=1");

	pm->RegisterNewFlags(VTF_BRAKE_FLAGS,"pVehicleBreakFlags","Use Table=1,Auto Break=2");

	REGISTER_CUSTOM_STRUCT("pVehicleBreakTable",E_VBT_STRUCT,VTS_BRAKE_TABLE,breakTable,FALSE);



	//################################################################
	//
	// Wheel 
	//

	REGISTER_CUSTOM_STRUCT("pWheelContactData",E_WCD_STRUCT,VTS_WHEEL_CONTACT,myStructWheelContactData,FALSE);

	pm->RegisterNewFlags(VTS_PHYSIC_WHEEL_FLAGS,"pWheelFlags","Steerable Input=1,Steerable Auto=2,Affected By Handbrake=4,Accelerated=8,Controlled by Vehicle=16,Affected by Differential=32,Ignore Tire Function=64");
	//pm->RegisterNewFlags(VTS_PHYSIC_WHEEL_FLAGS,"pWheelFlags","Steerable Input=1,Steerable Auto=2,Affected By Handbrake=4,Accelerated=8,Build Lower Half=256,Use Wheel Shape=512,Controlled by Vehicle");
	pm->RegisterNewFlags(VTF_VWSHAPE_FLAGS,"pWheelShapeFlags","AxisContactNormal=1,InputLateralSlip=2,InputLongitudinal=4,UnscaledSpringBehavior=8,EmulateLegacyWheel=32,ClampedFriction=64");

	pm->RegisterNewStructure(VTS_PHYSIC_WHEEL_DESCR,"pWheelDescr",
		"XML Link,Suspension,Spring Restitution,Spring Bias,Spring Damping,Maximum Brake Force,Friction To Side,Friction To Front,Inverse Wheel Mass,Wheel Flags,Wheel Shape Flags,Lateral Force Settings,Longitudinal Force Settings",
		VTE_XML_WHEEL_SETTINGS,
		CKPGUID_FLOAT,
		CKPGUID_FLOAT,
		CKPGUID_FLOAT,
		CKPGUID_FLOAT,
		CKPGUID_FLOAT,
		CKPGUID_FLOAT,
		CKPGUID_FLOAT,
		CKPGUID_FLOAT,
		VTS_PHYSIC_WHEEL_FLAGS,
		VTF_VWSHAPE_FLAGS,
		VTF_VWTIRE_SETTINGS,
		VTF_VWTIRE_SETTINGS);	

	att_wheelDescr = attman->RegisterNewAttributeType("pWheel",VTS_PHYSIC_WHEEL_DESCR,CKCID_BEOBJECT);
	attman->SetAttributeCategory(att_wheelDescr ,"Physic");

}
