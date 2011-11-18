#ifndef __P_CALLBACK_SIGNATURE_H__
#define __P_CALLBACK_SIGNATURE_H__

#include "pCommon.h"
#include "IParameter.h"
#include "vtBBHelper.h"
#include "xDebugTools.h"

//----------------------------------------------------------------
//
// wheel contact modify callback inputs
//
typedef enum bInputsWheelContactModifyCallback
{
	bbIWC_SrcObject,
	bbIWC_Point,
	bbIWC_Normal,
	bbIWC_Position,
	bbIWC_NormalForce,
	bbIWC_OtherMaterialIndex,
	bbIWC_Stub0,
	bbIWC_Stub1,
};
static BBParameter pInMapWheelContactModifyCallback[]=
{
	BB_PIN(bbIWC_SrcObject,CKPGUID_3DENTITY,"sourceObject",""),
	BB_PIN(bbIWC_Point,CKPGUID_VECTOR,"point",""),
	BB_PIN(bbIWC_Normal,CKPGUID_VECTOR,"normal",""),
	BB_PIN(bbIWC_Position,CKPGUID_FLOAT,"position",""),
	BB_PIN(bbIWC_NormalForce,CKPGUID_FLOAT,"normalForce",""),
	BB_PIN(bbIWC_OtherMaterialIndex,CKPGUID_INT,"otherMaterialIndex",""),
	BB_PIN(bbIWC_Stub0,CKPGUID_INT,"stub0",""),
	BB_PIN(bbIWC_Stub1,CKPGUID_INT,"stub1",""),
};
//----------------------------------------------------------------
//
// wheel contact modify callback outputs
//
typedef enum bOutputsWheelContactModifyCallback
{
	bbOWC_CreateContact,
	bbOWC_ModificationFlags,
	bbOWC_Point,
	bbOWC_Normal,
	bbOWC_Position,
	bbOWC_NormalForce,
	bbOWC_OtherMaterialIndex,
	bbOWC_Stub0,
	bbOWC_Stub1,
};
static BBParameter pOutMapWheelContactModifyCallback[]=
{
	BB_PIN(bbOWC_CreateContact,CKPGUID_BOOL,"createContact",""),
	BB_PIN(bbOWC_ModificationFlags,VTF_WHEEL_CONTACT_MODIFY_FLAGS,"modificationFlags",""),
	BB_PIN(bbOWC_Point,CKPGUID_VECTOR,"_point",""),
	BB_PIN(bbOWC_Normal,CKPGUID_VECTOR,"_normal",""),
	BB_PIN(bbOWC_Position,CKPGUID_FLOAT,"_position",""),
	BB_PIN(bbOWC_NormalForce,CKPGUID_FLOAT,"_normalForce",""),
	BB_PIN(bbOWC_OtherMaterialIndex,CKPGUID_INT,"_otherMaterialIndex",""),
	BB_PIN(bbOWC_Stub0,CKPGUID_INT,"_stub0",""),
	BB_PIN(bbOWC_Stub1,CKPGUID_INT,"_stub1",""),
};

//----------------------------------------------------------------
//
// contact modify callback ( has input and output )
//
typedef enum bInputsContactModifyCallback
{
	bbICM_SrcObject,
	bbICM_OtherObject,
	bbICM_MinImpulse,
	bbICM_MaxImpulse,
	bbICM_Error,
	bbICM_Target,
	bbICM_LP0,
	bbICM_LP1,
	bbICM_LO0,
	bbICM_LO1,
	bbICM_SF0,
	bbICM_SF1,
	bbICM_DF0,
	bbICM_DF1,
	bbICM_Restitution,
};
static BBParameter pInMapContactModifyCallback[]=
{
	BB_PIN(bbICM_SrcObject,CKPGUID_3DENTITY,"sourceObject",""),
	BB_PIN(bbICM_OtherObject,CKPGUID_3DENTITY,"otherObject",""),
	BB_PIN(bbICM_MinImpulse,CKPGUID_FLOAT,"minImpulse",""),
	BB_PIN(bbICM_MaxImpulse,CKPGUID_FLOAT,"maxImpulse",""),
	BB_PIN(bbICM_Error,CKPGUID_VECTOR,"error",""),
	BB_PIN(bbICM_Target,CKPGUID_VECTOR,"target",""),
	BB_PIN(bbICM_LP0,CKPGUID_VECTOR,"localPosition0",""),
	BB_PIN(bbICM_LP1,CKPGUID_VECTOR,"localPosition1",""),
	BB_PIN(bbICM_LO0,CKPGUID_QUATERNION,"localOrientation0",""),
	BB_PIN(bbICM_LO0,CKPGUID_QUATERNION,"localOrientation1",""),
	BB_PIN(bbICM_SF0,CKPGUID_FLOAT,"staticFriction0",""),
	BB_PIN(bbICM_SF1,CKPGUID_FLOAT,"staticFriction1",""),
	BB_PIN(bbICM_DF0,CKPGUID_FLOAT,"dynamicFriction0",""),
	BB_PIN(bbICM_DF0,CKPGUID_FLOAT,"dynamicFriction1",""),
	BB_PIN(bbICM_DF0,CKPGUID_FLOAT,"restitution",""),
};


//----------------------------------------------------------------
//
// contact modify callback ( has input and output )
//
typedef enum bOutputsContactModifyCallback
{
	bbOCM_ModifyFlags,
	bbOCM_CreateContact,
	bbOCM_MinImpulse,
	bbOCM_MaxImpulse,
	bbOCM_Error,
	bbOCM_Target,
	bbOCM_LP0,
	bbOCM_LP1,
	bbOCM_LO0,
	bbOCM_LO1,
	bbOCM_SF0,
	bbOCM_SF1,
	bbOCM_DF0,
	bbOCM_DF1,
	bbOCM_Restitution,
};

static BBParameter pOutMapContactModifyCallback[]=
{
	BB_PIN(bbOCM_ModifyFlags,VTF_CONTACT_MODIFY_FLAGS,"_modifyFlags",""),
	BB_PIN(bbOCM_CreateContact,CKPGUID_BOOL,"createContact",""),
	BB_PIN(bbOCM_MinImpulse,CKPGUID_FLOAT,"_minImpulse",""),
	BB_PIN(bbOCM_MaxImpulse,CKPGUID_FLOAT,"_maxImpulse",""),
	BB_PIN(bbOCM_Error,CKPGUID_VECTOR,"_error",""),
	BB_PIN(bbOCM_Target,CKPGUID_VECTOR,"_target",""),
	BB_PIN(bbOCM_LP0,CKPGUID_VECTOR,"_localPosition0",""),
	BB_PIN(bbOCM_LP1,CKPGUID_VECTOR,"_localPosition1",""),
	BB_PIN(bbOCM_LO0,CKPGUID_QUATERNION,"_localOrientation0",""),
	BB_PIN(bbOCM_LO0,CKPGUID_QUATERNION,"_localOrientation1",""),
	BB_PIN(bbOCM_SF0,CKPGUID_FLOAT,"_staticFriction0",""),
	BB_PIN(bbOCM_SF1,CKPGUID_FLOAT,"_staticFriction1",""),
	BB_PIN(bbOCM_DF0,CKPGUID_FLOAT,"_dynamicFriction0",""),
	BB_PIN(bbOCM_DF0,CKPGUID_FLOAT,"_dynamicFriction1",""),
	BB_PIN(bbOCM_DF0,CKPGUID_FLOAT,"_restitution",""),
};

//----------------------------------------------------------------
//
// contact notify callback
//
typedef enum bInputsContactCallback
{
	bbI_SrcObject,
	bbI_EventType,
	bbI_NormalForce,
	bbI_FForce,
	bbI_Point,
	bbI_PointNormalForce,
	bbI_FaceNormal,
	bbI_FaceIndex,
	bbI_Distance,
	bbI_OtherObject,
};
static BBParameter pInMapContactCallback[]=
{
	BB_PIN(bbI_SrcObject,CKPGUID_3DENTITY,"sourceObject",""),
	BB_PIN(bbI_EventType,VTF_COLLISIONS_EVENT_MASK,"eventType",""),
	BB_PIN(bbI_FaceNormal,CKPGUID_VECTOR,"sumNormalForce",""),
	BB_PIN(bbI_FForce,CKPGUID_VECTOR,"sumFrictionForce",""),
	BB_PIN(bbI_Point,CKPGUID_VECTOR,"point",""),
	BB_PIN(bbI_PointNormalForce,CKPGUID_FLOAT,"pointNormalForce",""),
	BB_PIN(bbI_FaceNormal,CKPGUID_VECTOR,"faceNormal",""),
	BB_PIN(bbI_FaceIndex,CKPGUID_INT,"faceIndex",""),
	BB_PIN(bbI_Distance,CKPGUID_FLOAT,"distance",""),
	BB_PIN(bbI_OtherObject,CKPGUID_3DENTITY,"otherObject",""),
};
//----------------------------------------------------------------
//
// trigger
//
typedef enum bInputsTriggerCallback
{
	bbIT_SrcObject,
	bbIT_EventType,
	bbIT_OtherObject,
};
static BBParameter pInMapTriggerCallback[]=
{
	BB_PIN(bbIT_SrcObject,CKPGUID_3DENTITY,"sourceObject",""),
	BB_PIN(bbIT_EventType,VTF_TRIGGER,"eventType",""),
	BB_PIN(bbIT_OtherObject,CKPGUID_3DENTITY,"otherObject",""),
};

//----------------------------------------------------------------
//
// trigger
//
typedef enum bInputsRaycastHitCallback
{
	bbRH_SrcObject,
	bbRH_OtherObject,
	bbRH_WorldImpact,
	bbRH_WorldNormal,
	bbRH_FIndex,
	bbRH_FInternalIdex,
	bbRH_Distance,
	bbRH_UV,
	bbRH_Material,
	bbRH_Flags,
};

typedef enum bInputsJointBreakCallback
{
	bbJB_SrcObject,
	bbJB_OtherObject,
	bbJB_Force,

};

static BBParameter pInMapRaycastHitCallback[]=
{
	BB_PIN(bbRH_SrcObject,CKPGUID_3DENTITY,"sourceObject",""),
	BB_PIN(bbRH_OtherObject,CKPGUID_3DENTITY,"otherObject",""),
	BB_PIN(bbRH_WorldImpact,CKPGUID_VECTOR,"impact",""),
	BB_PIN(bbRH_WorldNormal,CKPGUID_VECTOR,"normal",""),
	BB_PIN(bbRH_FIndex,CKPGUID_INT,"faceIndex",""),
	BB_PIN(bbRH_FInternalIdex,CKPGUID_INT,"faceInternalIndex",""),
	BB_PIN(bbRH_Distance,CKPGUID_FLOAT,"distance",""),
	BB_PIN(bbRH_UV,CKPGUID_2DVECTOR,"uv",""),
	BB_PIN(bbRH_Material,CKPGUID_INT,"material",""),
	BB_PIN(bbRH_Flags,VTF_RAY_HINTS,"flags",""),

};


static BBParameter pInMapJointBreakCallback[]=
{
	BB_PIN(bbJB_SrcObject,CKPGUID_3DENTITY,"sourceObject",""),
	BB_PIN(bbJB_OtherObject,CKPGUID_3DENTITY,"otherObject",""),
	BB_PIN(bbJB_Force,CKPGUID_FLOAT,"Break Force",""),
};

#endif