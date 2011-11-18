/********************************************************************
	created:	2009/02/14
	created:	14:2:2009   15:50
	filename: 	SDK\Include\Core\vtParameterGuids.h
	file path:	SDK\Include\Core
	file base:	vtParameterGuids
	file ext:	h
	author:		Günter Baumgart
	
	purpose:	Unique identifiers for the entire component  
*********************************************************************/
#ifndef __VT_PARAMETER_GUIDS_H__
	#define __VT_PARAMETER_GUIDS_H__


//################################################################
//
// Common Parameter, used by joints, bodies, -or world objects
//
#define VTS_AXIS_REFERENCED_LENGTH	CKGUID(0x19d6054d,0x4c2a2c99)
#define VTE_PHYSIC_DOMINANCE_GROUP		CKGUID(0x2ae53cee,0x57ca74d0)

#define VTS_SLEEPING_SETTINGS		CKGUID(0x28d13431,0x24186938)

#define VTF_TRIGGER CKGUID(0xe9c412e,0x68025071)
#define VTE_FILTER_OPS CKGUID(0x58340fe5,0x67892b1f)
#define VTE_FILTER_MASK CKGUID(0x30ff289e,0x7e57707c)
#define VTS_FILTER_GROUPS CKGUID(0x14443c3a,0x7c886162)
#define VTF_SHAPES_TYPE CKGUID(0x2ff80c77,0x7ab71a8)
#define VTF_RAY_HINTS CKGUID(0x7f5552d,0x70632e9a)
#define VTS_RAYCAST CKGUID(0x3842035f,0x1bc81c7f)
#define VTF_COLLISIONS_EVENT_MASK CKGUID(0x1beb409d,0x5028494f)

#define VTF_WHEEL_CONTACT_MODIFY_FLAGS CKGUID(0x40495482,0x4ab3283e)



//################################################################
//
// Shape overrides 
//
#define VTS_CAPSULE_SETTINGS		CKGUID(0x9a441c3,0x1e1d25ce)
#define VTS_CAPSULE_SETTINGS_EX		CKGUID(0x16f102dc,0x7cb97e54)
//	Wheel type using a convex cylinder, a capsule and a joint spring
#define  VTS_PHYSIC_CONVEX_CYLDINDER_WHEEL_DESCR CKGUID(0x65e30713,0x55ca1048)


//################################################################
//
// World Related 
//
#define VTS_PHYSIC_DOMINANCE_SCENE_SETTINGS		CKGUID(0x636c3e44,0x658213ea)
	#define VTS_PHYSIC_DOMINANCE_ITEM	CKGUID(0x7bee51cc,0xb676809)
		#define VTS_PHYSIC_DOMINANCE_CONSTRAINT		CKGUID(0x6f44420b,0x14b7435d)

#define VTS_WORLD_SETTINGS		CKGUID(0x5a0b56eb,0x50fc04d2)
#define VTS_PHYSIC_WORLD_PARAMETER		CKGUID(0x27f223a1,0x365777f0)

//################################################################
//
// Rigid Body Related
//
#define VTS_PHYSIC_PARAMETER		CKGUID(0x90e519f,0x7ec5345d)
#define VTS_PHYSIC_ACTOR		CKGUID(0x381d7e69,0x456458fb)

#define VTF_PHYSIC_SUBSHAPE_INHERITANCE_FLAGS		CKGUID(0x55c60b24,0x4ddc754e)

#define VTS_PHYSIC_TRANSFORMATIONS_LIMIT_PARAMETER		CKGUID(0x413f3fb4,0x4f545c24)

#define VTF_PHYSIC_BODY_COMMON_SETTINGS CKGUID(0x44bc4e8d,0x16cb3288)
#define VTE_BODY_FORCE_MODE	CKGUID(0x28c8214c,0x1ab04db8)
#define VTF_PHYSIC_BODY_UPDATE_FLAGS		CKGUID(0x7f824fb3,0x5496b62)
#define VTF_PHYSIC_WORLD_UPDATE_FLAGS		CKGUID(0x7f824fb3,0x5496b62)
#define VTF_BODY_FLAGS CKGUID(0x2c173381,0x10f66ab3)
#define VTF_BODY_TRANS_FLAGS CKGUID(0x41242761,0x72e70c27)
#define VTE_COLLIDER_TYPE		CKGUID(0x1c415d41,0x5c534d7a)
#define VTF_CONVEX_FLAGS		CKGUID(0x2d9d5c3e,0x468c0266)
#define VTF_CONTACT_MODIFY_FLAGS		CKGUID(0x7d5c0e7c,0x144d2596)

//----------------------------------------------------------------
//
// XML Setup 
//

#define VTS_PHYSIC_ACTOR_XML_SETTINGS_INTERN		CKGUID(0x35977af4,0x5b430c0c)
#define VTS_PHYSIC_ACTOR_XML_SETTINGS_EXTERN		CKGUID(0x57aa4cab,0x7e173b06)
#define VTS_PHYSIC_ACTOR_XML_IMPORT_FLAGS			CKGUID(0x2a2c3ee5,0x33cc3c2f)
#define VTS_PHYSIC_ACTOR_XML_SETUP					CKGUID(0x5e916f6,0x6e7a44ed)
#define VTF_PHYSIC_ACTOR_COPY_FLAGS					CKGUID(0x7013615f,0x9aa642e)

//----------------------------------------------------------------
//
// Geometry 
//
#define VTS_PHYSIC_PIVOT_OFFSET					CKGUID(0x19e432af,0x571f5bf7)

//----------------------------------------------------------------
//
// Collision 
//
#define VTF_PHYSIC_COLLISION_MASK		CKGUID(0x7bf86391,0x1ac73b1)

#define VTS_PHYSIC_CCD_SETTINGS		CKGUID(0x37537e0c,0x55b668d6)
	#define VTF_PHYSIC_CCD_FLAGS		CKGUID(0x25ee18c2,0x3419342d)

#define VTS_PHYSIC_COLLISIONS_SETTINGS		CKGUID(0x8027e35,0x7c940e70)
	#define VTE_PHYSIC_BODY_COLL_GROUP		CKGUID(0xc1e1e0a,0x36fd0de4)

#define  VTS_PHYSIC_COLLISIONS_SETUP	CKGUID(0x64043794,0x4d9f454b)


//----------------------------------------------------------------
//
// Mass Configuration
//
#define VTS_PHYSIC_MASS_SETUP		CKGUID(0x36855c80,0x81a0a4e)
	#define VTE_PHYSIC_MASS_TYPE		CKGUID(0x71921e8a,0x8e22f63)

//----------------------------------------------------------------
//
// Optimization
//
#define VTS_PHYSIC_SLEEP_SETTINGS		CKGUID(0x41450454,0x3b4a65c2)
#define VTS_PHYSIC_DAMPING_PARAMETER		CKGUID(0x17ad0411,0x3ccd0dd7)
#define VTS_PHYSIC_ACTOR_OPTIMIZATION		CKGUID(0x2fca03db,0x25644fd4)

//----------------------------------------------------------------
//
// Material 
//
#define VTS_MATERIAL CKGUID(0x4785249d,0x57af4457)
#define VTF_MATERIAL_FLAGS	CKGUID(0x14ce161f,0x27cc5b83)
#define VTE_MATERIAL_COMBINE_MODE		CKGUID(0x6dbf7c19,0x3dfb0e12)
#define VTE_XML_MATERIAL_TYPE		CKGUID(0x57430496,0x29193343)






//################################################################
//
// Joints : 
//

//----------------------------------------------------------------
//
// Common shared types 
//


#define VTE_JOINT_TYPE CKGUID(0x5d9c0413,0xcb96c02)
#define VTS_PHYSIC_JAMOTOR_AXIS_TYPE		CKGUID(0x21352808,0x1e932c41)

#define VTE_JOINT_MOTION_MODE		CKGUID(0x6ec04e81,0xfd537e)
#define VTS_JOINT_DRIVE		CKGUID(0x563c20ca,0x581e0e4b)
#define VTS_JOINT_SPRING		CKGUID(0x495d0920,0x189674ba)
#define VTS_JLIMIT		CKGUID(0x8d61654,0x1fd01503)
#define VTS_JOINT_SLIMIT	CKGUID(0xd997313,0x28523dc0)
#define VTS_JOINT_MOTOR CKGUID(0x50ab7cc2,0x37ce0071)

#define VTS_JOINT_D6 CKGUID(0x215a2fa1,0x7cc02701)
#define VTF_JOINT_D6_AXIS_MASK	CKGUID(0x461c1af3,0x4b194a84)
#define VTS_JOINT_D6_AXIS_ITEM	CKGUID(0x37d01b38,0x10a03ef)
#define VTE_JOINT_MOTION_MODE_AXIS	CKGUID(0x5adb450c,0x5798057d)
#define VTE_JOINT_LIMIT_AXIS CKGUID(0x16d654a4,0x276a048f)
#define VTE_JOINT_DRIVE_AXIS CKGUID(0x1c73456a,0x7d846d2a)
#define VTE_JOINT_PROJECTION_MODE CKGUID(0x2cce3d0b,0x60603c02)
#define VTE_JOINT_TYPE CKGUID(0x16f21041,0x7a6479f3)

//----------------------------------------------------------------
//
// Complete Joint Setups 
//

#define VTS_JOINT_FIXED CKGUID(0x3a5163bf,0x3c315528)
#define VTS_JOINT_DISTANCE CKGUID(0x1edf6510,0xdea68b2)
#define VTS_JOINT_BALL CKGUID(0x20271cdc,0x645c4212)
#define VTS_JOINT_REVOLUTE CKGUID(0x7d45030c,0x4f6216ef)
#define VTS_JOINT_PRISMATIC CKGUID(0x37fd735c,0x6b83447d)
#define VTS_JOINT_CYLINDRICAL CKGUID(0x45e07719,0xba2297)

#define VTS_JOINT_POINT_IN_PLANE CKGUID(0xed23f6,0x2ba449a3)
#define VTS_JOINT_POINT_ON_LINE CKGUID(0x4429006a,0x34345b66)

#define VTS_JOINT_D6 CKGUID(0x52467f8a,0x3adc012b)
#define VTS_JOINT_D6_DRIVES CKGUID(0x46067fc4,0x56cf693e)

#define VTS_JOINT_BREAKABLE CKGUID(0xe3d7a63,0x2cc61e4a)
#define VTS_JOINT_D6 CKGUID(0x11bd2119,0x3843102b)

#define VTS_PHYSIC_JBALL_PARAMETER		CKGUID(0x2c47770d,0x1b7173ad)
#define VTS_PHYSIC_JFIXED_PARAMETER		CKGUID(0x780650ae,0x7e6406c5)
#define VTS_PHYSIC_JHINGE_PARAMETER		CKGUID(0x5a805563,0x292021ce)
#define VTS_PHYSIC_JHINGE2_PARAMETER		CKGUID(0x281717e5,0x353b61f2)
#define VTS_PHYSIC_JUNIVERSAL_PARAMETER		CKGUID(0x7a283845,0x53144e6a)
#define VTS_PHYSIC_JSLIDER_PARAMETER		CKGUID(0x67837673,0x20c04330)
#define VTS_PHYSIC_JMOTOR_PARAMETER		CKGUID(0x677e7eba,0x6505310c)
#define VTS_PHYSIC_JLIMIT_PARAMETER		CKGUID(0x17e57c53,0x43585c91)
#define VTE_PHYSIC_JDRIVE_TYPE CKGUID(0x75a51b10,0xe00025c)

//----------------------------------------------------------------
//
// Joint Misc Structures
//

#define VTS_PHYSIC_JLIMIT_PLANE CKGUID(0x5abe5f0b,0x7ca6657e)

//################################################################
//
// Wheel Parameters
//
#define VTF_VSTATE_FLAGS	CKGUID(0x49ce782d,0x7566828)
#define VTF_VFLAGS			CKGUID(0x5cf964cf,0xd382f37)


#define VTF_VWSHAPE_FLAGS CKGUID(0x7da158eb,0x16e921a1)
#define VTF_VWTIRE_SETTINGS CKGUID(0x2690c24,0xde55e2b)

#define VTE_BRAKE_XML_LINK CKGUID(0x77806807,0x4eac2b27)
#define VTE_BRAKE_LEVEL CKGUID(0x161f2b7d,0x2f657a2a)
#define VTF_BRAKE_FLAGS CKGUID(0x46535a8f,0x11815172)
#define VTS_BRAKE_TABLE CKGUID(0x59052709,0xa555846)
#define VTF_WHEEL_CONEX_SHAPE CKGUID(0x3f913a00,0x372f4a50)
#define VTS_WHEEL_CONTACT CKGUID(0x33f24aa8,0x34a57460)

#define  VTS_PHYSIC_WHEEL_DESCR CKGUID(0x5dcd09ae,0x73f72b97)
#define  VTS_PHYSIC_WHEEL_FLAGS CKGUID(0x72b70c7d,0x3b60239d)

//################################################################
//
// Vehicle
//
#define  VTS_PHYSIC_VEHICLE_DESCR CKGUID(0x54562468,0xd1a6de6)
#define  VTS_PHYSIC_VEHICLE_MOTOR_DESCR CKGUID(0x19317402,0x2f4b65a1)
#define  VTS_PHYSIC_GEAR_DESCR CKGUID(0x308e5c88,0x433871f9)
#define VTE_XML_VEHICLE_SETTINGS		CKGUID(0x67ca76e9,0x452f7ceb)
#define VTE_XML_VMOTOR_SETTINGS		CKGUID(0x6af977a6,0x11084c45)
#define VTF_VEHICLE_ENGINE_FLAGS		CKGUID(0x23823b40,0x694f152c)
#define VTE_XML_VGEAR_SETTINGS		CKGUID(0x9bc7981,0x4a6f7245)
#define VTE_XML_WHEEL_SETTINGS		CKGUID(0x1ed80439,0x1f9825c4)
#define VTE_XML_TIRE_SETTINGS		CKGUID(0x4cb47505,0x7b022333)
#define VTS_VMOTOR_ENTRY CKGUID(0x12dd3a77,0x5db358f8)
#define VTS_VMOTOR_TVALUES CKGUID(0x34af3aa2,0x23aa6422)
#define VTS_VGEAR_GRAPH_SETTINGS	CKGUID(0x25016106,0x3a0024a0)
#define VTS_VGEARBOX_FLAGS	CKGUID(0x47d632a5,0x50057698)
#define VTS_VGEAR_RATIO_ENTRY		CKGUID(0x36b93b1c,0x79f804c2)
#define VTS_VGEAR_RATIOS		CKGUID(0x5a0230db,0x441f5c18)
#define VTS_VGEAR_CURVE			CKGUID(0x6352317b,0x41fe3769)
#define VTS_VGEAR_SETTINGS	CKGUID(0x7dde30fb,0x701915ea)
#define VTE_VEHICLE_XML_LINK CKGUID(0x4c43611,0x736078b9)

#define VTF_VEHICLE_PROCESS_OPTIONS CKGUID(0x24fa465f,0x1c2f5b88)


//################################################################
//
// Cloth
//
#define VTE_CLOTH_FLAGS CKGUID(0x2c7d5bb6,0x6a9d7c41)
#define VTS_CLOTH_DESCR CKGUID(0x722a5c01,0x5c8d413d)
#define VTS_CLOTH_METAL_DESCR CKGUID(0x1ecb0821,0x709e7bdf)
#define VTE_CLOTH_ATTACH_FLAGS CKGUID(0x428b755b,0x36d60122)

//################################################################
//
// UNknow :
//
#define VTS_PHYSIC_HEIGHTFIELD_PARAMETERS		CKGUID(0x37430de4,0x54d06445)


#endif