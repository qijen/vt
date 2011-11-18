#ifndef _XNET_ENUMERATIONS_H_
#define _XNET_ENUMERATIONS_H_

#define xBIT(x) (1 << (x))                       ///< Returns value with bit x set (2^x)


typedef enum E_MESSAGE_FLAGS
{

	E_MF_NEW,
	E_MF_SENDING,
	E_MF_SENT,
	E_MF_TIMEOUT,
	E_MF_FINISH,
	E_MF_OUTGOING,
	E_MF_INCOMING,
	E_MF_SEND_TO_ALL,
	E_MF_DELETED,
};

typedef enum E_MESSAGE_WRITE_FLAGS
{

	E_MWF_UPDATE_SERVER,
	E_MWF_SEND_SRC_USER_ID,
	E_MWF_SEND_TARGET_USER_ID,
	E_MWF_UPDATE_GHOST,
	E_MWF_SERVER_UPDATE,
};

typedef enum E_MESSAGE_READ_FLAGS
{
	E_MRF_UPDATE_BY_GHOST,
	E_MRF_READ_SRC_USER_ID,
	E_MRF_READ_TARGET_USER_ID,
	E_MRF_UPDATE_GHOST,
	E_MRF_SERVER_UPDATE,
};
typedef enum E_OBJECT_TYPE
{

	E_OT_DIST_OBJECT,
	E_OT_DIST_PROPERTY,
	E_OT_CLASS,
	E_OT_MESSAGE,
};

typedef enum E_LOG_ITEMS
{

	E_LI_CLIENT,
	E_LI_SESSION,
	E_LI_3DOBJECT,
	E_LI_2DOBJECT,
	E_LI_DISTRIBUTED_BASE_OBJECT,
	E_LI_DISTRIBUTED_CLASS_DESCRIPTORS,
	E_LI_MESSAGES,
	E_LI_ARRAY_MESSAGES,
	E_LI_CONNECTION,
	E_LI_NET_INTERFACE,
	E_LI_GHOSTING,
	E_LI_STATISTICS,
	E_LI_BUILDINGBLOCKS,
	E_LI_VSL,
	E_LI_CPP,
	E_LI_ASSERTS,
	E_LI_PREDICTION,
	E_LI_SERVER_MESSAGES
};

typedef enum E_LOG_FLAGS
{
	E_LF_CPP_FUNCS,
	E_LF_CPP_FUNC_SIGNATURE,
	E_LF_BB_NAME,
	E_LF_BB_OWNER,
	E_LF_TIMESTAMPS,


};

typedef enum E_DV_STATE
{

	E_DV_OK,
	E_DV_UPDATED
};

enum SuperType
{
	vtSTRING = 1,
	vtFLOAT = 2,
	vtINTEGER = 3,
	vtVECTOR = 4,
	vtVECTOR2D = 5,
	vtCOLOUR = 6,
	vtMATRIX = 7,
	vtQUATERNION = 8,
	vtRECTANGLE		= 9,
	vtBOX		= 10,
	vtBOOL		= 11,
	vtENUMERATION = 12,
	vtFLAGS = 13,
	vtFile = 14,
	vtOBJECT = 16,
	vtUNKNOWN = 17
};

typedef enum E_DO_FLAGS
{
	E_DO_OK=0x0000,
	E_DO_NEEDS_SEND=0x0001,
};


typedef enum E_DP_FLAGS
{
	E_DP_OK=0x0000,
	E_DP_NEEDS_SEND=0x0001,
};

typedef enum E_DO_UPDATE_FLAGS
{
	E_DO_NONE=0x0000,
	E_DO_POSITION=0x0001,
	E_DO_ROTATION=0x0002,
};



typedef enum E_DO_INTERFACE_FLAGS
{
	E_DO_CREATED=0x0001,
	E_DO_PROCESSED=0x0002,
	E_DO_BINDED=0x0004
};



typedef enum E_XNETWORK_INTERFACE_STATUS
{

	E_NI_CREATED,
	E_NI_CONNECTING,
	E_NI_CONNECTED,
	E_NI_DESTROYED_BY_SERVER,
	E_NI_ERROR
};

enum E_DO_DELETE_STATE
{
	E_DO_DS_DELETED,
	E_DO_DS_DELETE_NOTIFIED,

};

enum USER_FLAG
{
	USER_NEW,
	USER_OK,
	USER_DELETED,
	USERNAME_CHANGED,
	USERNAME_OK
};

typedef enum E_DO_OWNERSHIP_STATUS
{
	E_DO_OS_OWNER,
	E_DO_OS_OWNERCHANGED,
	E_DO_OS_REQUEST,
	E_DO_OS_OTHER,
	E_DO_OS_NONE,
	E_DO_OS_BIND,
	E_DO_OS_RELEASED
};

typedef enum E_DO_OWNERSHIP_REQUEST
{

	E_DO_OSR_PROGRESS,
	E_DO_OSR_FAILED,
	E_DO_OSR_ACCEPTED
};

typedef enum E_DO_STATE_FLAGS
{
	E_DOSF_UNPACKED,
	E_DOSF_SHOWN,
	E_DOSF_SESSION_LEFT,
	E_DOSF_DELETED,
	E_DOSF_INITIAL_STATE_SEND
};
typedef enum E_DO_CREATION_FLAGS
{

	E_DO_CREATION_NONE,
	E_DO_CREATION_INCOMPLETE,
	E_DO_PENDING_ID_REQUESTED,
	E_DO_CREATION_CREATED,
	E_DO_CREATION_COMPLETE,
	E_DO_CREATION_TIMEOUT,
	E_DO_CREATION_ERROR
};


typedef enum E_PREDICTION_TYPE
{

	E_PTYPE_PREDICTED,
	E_PTYPE_RELIABLE,
	E_PTYPE_NON_RELIABLE
};

typedef enum E_DC_BASE_TYPE
{

	E_DC_BTYPE_3D_ENTITY,
	E_DC_BTYPE_2D_ENTITY,
	E_DC_BTYPE_CLIENT,
	E_DC_BTYPE_SESSION
};
typedef enum E_DC_PROPERTY_TYPE
{

	E_DC_PTYPE_3DVECTOR,
	E_DC_PTYPE_QUATERNION,
	E_DC_PTYPE_2DVECTOR,
	E_DC_PTYPE_FLOAT,
	E_DC_PTYPE_INT,
	E_DC_PTYPE_BOOL,
	E_DC_PTYPE_STRING,
	E_DC_PTYPE_UNKNOWN,
};


typedef enum E_DCF_3D_NATIVE_PROPERTIES
{
	E_DCF_3D_NP_LOCAL_POSITION=0x0001,
	E_DCF_3D_NP_LOCAL_ROTATION=0x0002,
	E_DCF_3D_NP_LOCAL_SCALE=0x0004,
	E_DCF_3D_NP_WORLD_POSITION=0x0008,
	E_DCF_3D_NP_WORLD_ROTATION=0x0010,
	E_DCF_3D_NP_WORLD_SCALE=0x0020,
	E_DCF_3D_NP_VISIBILITY=0x0040,
	E_DCF_3D_NP_USER_0=0x0080,
	E_DCF_3D_NP_USER_1=0x0100,
	E_DCF_3D_NP_USER_2=0x0200,
	E_DCF_3D_NP_USER_3=0x0400,
	E_DCF_3D_NP_USER_4=0x0800,

};


typedef enum E_DC_3D_NATIVE_PROPERTIES
{
	E_DC_3D_NP_LOCAL_POSITION=1,
	E_DC_3D_NP_LOCAL_ROTATION=2,
	E_DC_3D_NP_LOCAL_SCALE=3,
	E_DC_3D_NP_WORLD_POSITION=4,
	E_DC_3D_NP_WORLD_ROTATION=5,
	E_DC_3D_NP_WORLD_SCALE=6,
	E_DC_3D_NP_VISIBILITY=7,
	E_DC_3D_NP_USER=8
};

typedef enum E_DC_SESSION_NATIVE_PROPERTIES
{
	E_DC_S_NP_MAX_USERS=1,
	E_DC_S_NP_PASSWORD=2,
	E_DC_S_NP_TYPE=3,
	E_DC_S_NP_LOCKED=4,
	E_DC_S_NP_NUM_USERS=5,
	E_DC_S_NP_USER=6,
};

typedef enum E_DO_UPDATE_STATE
{
	E_DO_US_OK,
	E_DO_US_PENDING,
	E_DO_US_SEND,
};
typedef enum E_C_PACKET
{
	E_C_CLIENT_NONE=0x000,
	E_C_CLIENT_OBJECT_UPDATE=0x0001
};


typedef enum E_CLIENT_FLAGS
{
	E_CF_CONNECTED,
	E_CF_CONNECTION_LOST,
	E_CF_SESSION_JOINED,
	E_CF_SESSION_DESTROYED,
	E_CF_REMOVED,
	E_CF_ADDING,
	E_CF_ADDED,
	E_CF_DELETING,
	E_CF_DELETED,
	E_CF_NM_FREE,
	E_CF_NM_SENDING,
	E_CF_NM_SENT,

};

typedef enum E_SESSION_FLAGS
{
	E_SF_INCOMPLETE,
	E_SF_PARAMETER_ATTACHED,
	E_SF_POPULATE_PARAMETERS,
	E_SF_COMPLETE,
	E_SF_LOCKED
};

typedef enum E_NETWORK_ERROR
{
	E_NWE_OK,
	E_NWE_INTERN,
	E_NWE_NO_CONNECTION,
	E_NWE_NO_SESSION,
	E_NWE_SESSION_WRONG_PASSWORD,
	E_NWE_SESSION_LOCKED,
	E_NWE_SESSION_EXISTS,
	E_NWE_SESSION_FULL,
	E_NWE_NOT_SESSION_MASTER,
	E_NWE_INVALID_PARAMETER,
	E_NWE_NO_SUCH_USER,
	E_NWE_DIST_CLASS_EXISTS,
	E_NWE_NO_SERVER,
	E_NWE_SESSION_ALREADY_JOINED,
	E_NWE_NO_SUCH_SESSION
};

typedef enum E_OBJECT_PRINT_FLAGS
{

	E_OPF_NAME,
	E_OPF_GHOST_ID,
	E_OPF_USER_ID,
	E_OPF_SESSION_ID,
	E_OPF_CLASS,
	E_OPF_NUM_PROPS,
	E_OPF_PROPS,
	E_OPF_NATIVE_PROPERTIES,
	E_OPF_USER_PROPERTIES,
	E_OPF_SESSION_LOCKED,
	E_OPF_SESSION_USERS,
	E_OPF_SESSION_NAME,
	E_OPF_SESSION_PRIVATE,
	E_OPF_SESSION_MAXUSERS,
	E_OPF_SESSION_MASTER,
	E_OPF_CLIENT_USERNAME,
	E_OPF_CLIENT_LOCAL_ADDRESS,
	E_OPF_CLIENT_LOCAL_GHOST_IDS,
	E_OPF_CLIENT_CONNECTION_SETUP,

};

typedef enum E_PROPERTY_PRINT_FLAGS
{
	E_PPF_NAME,
	E_PPF_VALUE_TYPE,
	E_PPF_NATIVE_TYPE,
	E_PPF_PREDICTION_TYPE,
	E_PPF_SERVER_VALUE,
	E_PPF_LAST_VALUE,
	E_PPF_CURRENT_VALUE,
	E_PPF_DIFFERENCE,
	E_PPF_MIN_TIME,
	E_PPF_MIN_THRESHOLD
};

#endif
