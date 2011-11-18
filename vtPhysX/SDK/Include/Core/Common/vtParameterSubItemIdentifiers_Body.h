#ifndef __VT_BODY_STRUCTS_H__
#define __VT_BODY_STRUCTS_H__

//////////////////////////////////////////////////////////////////////////
//
//		@todo : old custom structures, classical registered 
//
typedef enum E_PHYSIC_PARAMETER_STRUCT
{
	E_PPS_HULLTYPE,
	E_PPS_BODY_FLAGS,
	E_PPS_DENSITY,
	E_PPS_SKIN_WIDTH,
	E_PPS_MASS_OFFSET,
	E_PPS_SHAPE_OFFSET,
	E_PPS_HIRARCHY,
	E_PPS_WORLD,
	E_PPS_NEW_DENSITY,
	E_PPS_TOTAL_MASS,
	E_PPS_COLL_GROUP
};

typedef enum E_MATERIAL_STRUCT
{
	E_MS_XML_TYPE,
	E_MS_DFRICTION,
	E_MS_SFRICTION,
	E_MS_RESTITUTION,
	E_MS_DFRICTIONV,
	E_MS_SFRICTIONV,
	E_MS_ANIS,
	E_MS_FCMODE,
	E_MS_RCMODE,
	E_MS_FLAGS,
};

typedef enum E_RAY_CAST_STRUCT
{
	E_RC_WORLD,
	E_RC_ORI,
	E_RC_ORI_REF,
	E_RC_DIR,
	E_RC_DIR_REF,
	E_RC_LENGTH,
	E_RC_GROUPS,
	E_RC_GROUPS_MASK,
	E_RC_SHAPES_TYPES
};

typedef enum E_CLOTH_STRUCT
{
	E_CS_THICKNESS,
	E_CS_DENSITY,
	E_CS_BENDING_STIFFNESS,
	E_CS_STRETCHING_STIFFNESS,
	E_CS_DAMPING_COEFFICIENT,
	E_CS_FRICTION,
	E_CS_PRESSURE,
	E_CS_TEAR_FACTOR,
	E_CS_COLLISIONRESPONSE_COEFFICIENT,
	E_CS_ATTACHMENTRESPONSE_COEFFICIENT,
	E_CS_ATTACHMENT_TEAR_FACTOR,
	E_CS_TO_FLUID_RESPONSE_COEFFICIENT,
	E_CS_FROM_FLUIDRESPONSE_COEFFICIENT,
	E_CS_MIN_ADHERE_VELOCITY,
	E_CS_SOLVER_ITERATIONS,
	E_CS_EXTERN_ALACCELERATION,
	E_CS_WIND_ACCELERATION,
	E_CS_WAKE_UP_COUNTER,
	E_CS_SLEEP_LINEAR_VELOCITY,
	E_CS_COLLISIONG_ROUP,
	E_CS_VALID_BOUNDS,
	E_CS_RELATIVE_GRID_SPACING,
	E_CS_FLAGS,
	E_CS_TEAR_VERTEX_COLOR,
	E_CS_WORLD_REFERENCE,
	E_CS_ATTACHMENT_FLAGS,
};

typedef enum E_WCD_STRUCT
{
	E_WCD_CPOINT,
	E_WCD_CNORMAL,
	E_WCD_LONG_DIR,
	E_WCD_LAT_DIR,
	E_WCD_CONTACT_FORCE,
	E_WCD_LONG_SLIP,
	E_WCD_LAT_SLIP,
	E_WCD_LONG_IMPULSE,
	E_WCD_LAT_IMPULSE,
	E_WCD_OTHER_MATERIAL_INDEX,
	E_WCD_C_POS,
	E_WCD_CONTACT_ENTITY,
};

typedef enum E_VBT_STRUCT
{
	E_VBT_0,
	E_VBT_1,
	E_VBT_2,
	E_VBT_3,
	E_VBT_4,
	E_VBT_5,
	E_VBT_6,
	E_VBT_7,
	E_VBT_8,
	E_VBT_9
};

typedef enum WHEEL_DESCR_STRUCT
{
	E_WD_XML,
	E_WD_SUSPENSION,
	E_WD_SPRING_RES,
	E_WD_DAMP,
	E_WD_SPRING_BIAS,
	E_WD_MAX_BFORCE,
	E_WD_FSIDE,
	E_WD_FFRONT,
	E_WD_INVERSE_WHEEL_MASS,
	E_WD_FLAGS,
	E_WD_SFLAGS,
	E_WD_LAT_FUNC,
	E_WD_LONG_FUNC,
};

typedef enum E_CAPSULE_STRUCT
{
	E_CS_LENGTH_AXIS,
	E_CS_RADIUS_AXIS,
	E_CS_LENGTH,
	E_CS_RADIUS,
};


//////////////////////////////////////////////////////////////////////////
//
//		New Structs
//
/**
\brief	Data mask to determine which parts of a rigid bodies description have to
		be evolved.
*/
enum pObjectDescrMask
{
	/**
	\brief Description has XML settings
	*/
	OD_XML = (1 << 0),
	/**
	\brief Description has pivot override
	*/
	OD_Pivot = (1 << 1),
	/**
	\brief Description has mass override
	*/
	OD_Mass= (1 << 2),
	/**
	\brief Description has collisions settings
	*/
	OD_Collision = (1 << 3),
	/**
	\brief Description has CCD settings
	*/
	OD_CCD = (1 << 4),
	/**
	\brief Description has material settings
	*/
	OD_Material = (1 << 5),
	/**
	\brief Description has optimization settings
	*/
	OD_Optimization = (1 << 6),
	/**
	\brief Description has capsule override
	*/
	OD_Capsule = (1 << 7),
	/**
	\brief Description has convex cylinder override
	*/
	OD_ConvexCylinder = (1 << 8),
	/**
	\brief Description has wheel settings
	*/
	OD_Wheel = (1 << 9)
};

enum PB_COPY_FLAGS
{
	PB_CF_PHYSICS=1,
	PB_CF_SHARE_MESHES=(1<<1),
	PB_CF_PIVOT_SETTINGS=(1<<2),
	PB_CF_MASS_SETTINGS=(1<<3),
	PB_CF_COLLISION=(1<<4),
	PB_CF_CCD=(1<<5),
	PB_CF_MATERIAL=(1<<6),
	PB_CF_OPTIMIZATION=(1<<7),
	PB_CF_CAPSULE=(1<<8),
	PB_CF_CONVEX_CYLINDER=(1<<9),
	PB_CF_FORCE=(1<<10),
	PB_CF_VELOCITIES=(1<<11),
	PB_CF_JOINTS=(1<<12),
	PB_CF_LIMIT_PLANES=(1<<13),
	PB_CF_SWAP_JOINTS_REFERENCES=(1<<14),
	PB_CF_OVRRIDE_BODY_FLAGS=(1<<15),
	PB_CF_COPY_IC=(1<<16),
	PB_CF_RESTORE_IC=(1<<17),

};

enum PS_B_COLLISON
{
	PS_BC_GROUP,
	PS_BC_GROUPSMASK,
	PS_BC_SKINWITDH,
	PS_BC_CCD_SETUP
};

enum PS_B_CCD
{
	PS_B_CCD_MOTION_THRESHOLD,
	PS_B_CCD_FLAGS,
	PS_B_CCD_SCALE,
	PS_B_CCD_MESH_REFERENCE,
};

enum PS_B_COLLISION_SETUP
{
	PS_BCS_COLLISION_COMMON,
	PS_BCS_CCD,
};


enum PS_B_DAMPING
{
	PS_BD_LINEAR,
	PS_BD_ANGULAR,
};


enum PS_B_SLEEPING
{
	PS_BS_LINEAR_SLEEP,
	PS_BS_ANGULAR_SLEEP,
	PS_BS_THRESHOLD,
};

enum PS_B_OPTIMISATION
{
	PS_BO_LOCKS,
	PS_BO_DAMPING,
	PS_BO_SLEEPING,
	PS_BO_SOLVER_ITERATIONS,
	PS_BO_DOMINANCE_GROUP,
	PS_BO_COMPARTMENT_ID,

};
enum PS_B_PIVOT
{
	PS_BP_LINEAR,
	PS_BP_ANGULAR,
	PS_BP_REFERENCE,
};

enum PS_B_MASS
{
	PS_BM_DENSITY,
	PS_BM_TOTAL_MASS,
	PS_BM_PIVOT_POS,
	PS_BM_PIVOT_ROTATION,
	PS_BM_PIVOT_REFERENCE,
};

enum PS_BODY_FULL
{
	PS_BODY_XML,
	PS_BODY_HULL_TYPE,
	PS_BODY_FLAGS,

};

enum PS_BODY_XML_SETUP
{
	PS_INTERN_LINK,
	PS_EXTERN_LINK,
	PS_XML_MPORT_FLAGS,
};

enum PS_BODY_COMMON
{
	PS_BC_HULL_TYPE,
	PS_BC_DENSITY,
	PS_BC_FLAGS,
/*	PS_BC_TFLAGS,*/
	PS_BC_WORLD
};

enum PS_BODY_SETUP
{
	PS_XML_SETUP,
	PS_COMMON_SETTINGS,
	/*PS_PIVOT,
	PS_MASS,*/
	PS_COLLISION_SETTINGS,

};
enum PS_AXIS_REFERENCED_LENGTH
{
	PS_ARL_VALUE,
	PS_ARL_REF_OBJECT,
	PS_ARL_REF_OBJECT_AXIS,
};

enum PS_CAPSULE
{
	PS_BCAPSULE_RADIUS_REFERENCED_VALUE,
	PS_PCAPSULE_HEIGHT_REFERENCED_VALUE,
};
enum PS_CUSTOM_CONVEX_CYLINDER_DESCR
{
	PS_CC_APPROXIMATION,
	PS_CC_RADIUS_REFERENCED_VALUE,
	PS_CC_HEIGHT_REFERENCED_VALUE,
	PS_CC_FORWARD_AXIS,
	PS_CC_FORWARD_AXIS_REF,
	PS_CC_DOWN_AXIS,
	PS_CC_DOWN_AXIS_REF,
	PS_CC_RIGHT_AXIS,
	PS_CC_RIGHT_AXIS_REF,
	PS_CC_BUILD_LOWER_HALF_ONLY,
	PS_CC_EXTRA_SHAPE_FLAGS
};



#endif
