#include <StdAfx.h>
#include "vtPhysXAll.h"

#define PARAMETER_OP_TYPE_IS_CONNECTED CKGUID(0x2fe947dd,0x783224e9)

#define PARAM_OP_TYPE_JGET_LIMIT1 CKGUID(0x3678447e,0x30362a74)
#define PARAM_OP_TYPE_JGET_LIMIT2 CKGUID(0xc21ab2,0x465f7f69)
#define PARAM_OP_TYPE_JGET_LIMIT3 CKGUID(0x3ed57b83,0x47ad145f)

//pMotor : 
#define PARAM_OP_TYPE_JMOTOR_SET_TVEL CKGUID(0xa872a4,0x4e8921a4)
#define PARAM_OP_TYPE_JMOTOR_SET_MAXF CKGUID(0x2026057d,0x372684a)
#define PARAM_OP_TYPE_JMOTOR_SET_SPIN_FREE CKGUID(0x4aa2636b,0x734a6d4c)

#define PARAM_OP_TYPE_JMOTOR_GET_TVEL CKGUID(0x6f91728a,0x29d13cda)
#define PARAM_OP_TYPE_JMOTOR_GET_MAXF CKGUID(0x1e583ea9,0x4305055)
#define PARAM_OP_TYPE_JMOTOR_GET_SPIN_FREE CKGUID(0x50f145b,0x45df2205)

/************************************************************************/
/* joint structures :                                                                      */
/************************************************************************/

void ParamOpJMotorSetTVel(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);
void ParamOpJMotorSetFMAX(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);
void ParamOpJMotorSetSpinFree(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);

void ParamOpJMotorGetTVel(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);
void ParamOpJMotorGetFMAX(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);
void ParamOpJMotorGetSpinFree(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);


void ParamOpJMotorSetTVel(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	if (!GetPMan()->getCurrentFactory())
	{
		return;
	}
	float value = 0.0f;
	p2->GetValue(&value);
	res->CopyValue(p1->GetRealSource(),false);
	vtTools::ParameterTools::SetParameterStructureValue<float>(res,0,value,false);
}

void ParamOpJMotorGetTVel(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	float value = 0.0f;
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),0,false);
		}
	}
	res->SetValue(&value);
}
void ParamOpJMotorSetFMAX(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	if (!GetPMan()->getCurrentFactory())
	{
		return;
	}
	float value = 0.0f;
	p2->GetValue(&value);
	res->CopyValue(p1->GetRealSource(),false);
	vtTools::ParameterTools::SetParameterStructureValue<float>(res,1,value,false);
}


void ParamOpJMotorGetFMAX(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	float value = 0.0f;
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),1,false);
		}
	}
	res->SetValue(&value);
}
void ParamOpJMotorSetSpinFree(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	if (!GetPMan()->getCurrentFactory())
	{
		return;
	}
	int value = 0;
	p2->GetValue(&value);
	res->CopyValue(p1->GetRealSource(),false);
	vtTools::ParameterTools::SetParameterStructureValue<int>(res,2,value,false);
}

void ParamOpJMotorGetSpinFree(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	int value = 0;
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<int>(p1->GetRealSource(),2,false);
		}
	}
	res->SetValue(&value);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
#define PARAM_OP_TYPE_JLIMIT_SET_VALUE CKGUID(0x38a829f0,0x47851486)
#define PARAM_OP_TYPE_JLIMIT_SET_RES CKGUID(0x3ce77eb1,0x2e921a87)
#define PARAM_OP_TYPE_JLIMIT_SET_HARD CKGUID(0x111a4a9f,0x54094430)

#define PARAM_OP_TYPE_JLIMIT_GET_VALUE CKGUID(0xc203321,0x4ca77bd)
#define PARAM_OP_TYPE_JLIMIT_GET_RES CKGUID(0x19f812e7,0x5fb3cfc)
#define PARAM_OP_TYPE_JLIMIT_GET_HARD CKGUID(0x6b1b44cd,0x5efc7f51)

void ParamOpJLimitSetValue(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	if (!GetPMan()->getCurrentFactory())
	{
		return;
	}
	float value = 0.0f;
	p2->GetValue(&value);
	res->CopyValue(p1->GetRealSource(),false);
	vtTools::ParameterTools::SetParameterStructureValue<float>(res,0,value,false);
}

void ParamOpJLimitGetValue(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	float value = 0.0f;
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),0,false);
		}
	}
	res->SetValue(&value);
}
void ParamOpJLimitSetRes(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	if (!GetPMan()->getCurrentFactory())
	{
		return;
	}
	float value = 0.0f;
	p2->GetValue(&value);
	res->CopyValue(p1->GetRealSource(),false);
	vtTools::ParameterTools::SetParameterStructureValue<float>(res,1,value,false);
}


void ParamOpJLimitGetRes(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	float value = 0.0f;
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),1,false);
		}
	}
	res->SetValue(&value);
}
void ParamOpJLimitSetHard(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	if (!GetPMan()->getCurrentFactory())
	{
		return;
	}
	float value = 0.0f;
	p2->GetValue(&value);
	res->CopyValue(p1->GetRealSource(),false);
	vtTools::ParameterTools::SetParameterStructureValue<float>(res,2,value,false);
}

void ParamOpJLimitGetHard(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	float value = 0.0f;
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),2,false);
		}
	}
	res->SetValue(&value);
}

/************************************************************************/
/* spring :                                                                      */
/************************************************************************/
#define PARAM_OP_TYPE_JSPRING_SET_SPRING CKGUID(0x2e0f1602,0x7f9d30fe)
#define PARAM_OP_TYPE_JSPRING_SET_DAMPER CKGUID(0x7392369,0x168f33a1)
#define PARAM_OP_TYPE_JSPRING_SET_VALUE CKGUID(0x70026320,0x35b41a38)

#define PARAM_OP_TYPE_JSPRING_GET_SPRING CKGUID(0x3dde73ff,0x550c16ff)
#define PARAM_OP_TYPE_JSPRING_GET_DAMPER CKGUID(0x1f793582,0x11f96df9)
#define PARAM_OP_TYPE_JSPRING_GET_VALUE CKGUID(0x76226303,0x67ba262f)

void ParamOpJSpringSetSpring(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	if (!GetPMan()->getCurrentFactory())
	{
		return;
	}
	float value = 0.0f;
	p2->GetValue(&value);
	res->CopyValue(p1->GetRealSource(),false);
	vtTools::ParameterTools::SetParameterStructureValue<float>(res,0,value,false);
}

void ParamOpJSpringGetSpring(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	float value = 0.0f;
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),0,false);
		}
	}
	res->SetValue(&value);
}
void ParamOpJSpringSetDamper(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	if (!GetPMan()->getCurrentFactory())
	{
		return;
	}
	float value = 0.0f;
	p2->GetValue(&value);
	res->CopyValue(p1->GetRealSource(),false);
	vtTools::ParameterTools::SetParameterStructureValue<float>(res,1,value,false);
}


void ParamOpJSpringGetDamper(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	float value = 0.0f;
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),1,false);
		}
	}
	res->SetValue(&value);
}
void ParamOpJSpringSetValue(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	if (!GetPMan()->getCurrentFactory())
	{
		return;
	}
	float value = 0.0f;
	p2->GetValue(&value);
	res->CopyValue(p1->GetRealSource(),false);
	vtTools::ParameterTools::SetParameterStructureValue<float>(res,2,value,false);
}

void ParamOpJSpringGetVAlue(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	float value = 0.0f;
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),2,false);
		}
	}
	res->SetValue(&value);
}




/************************************************************************/
/* spring :                                                                      */
/************************************************************************/
#define PARAM_OP_TYPE_JSLIMIT_SET_DAMPING CKGUID(0x24e53be6,0x43bf6178)
#define PARAM_OP_TYPE_JSLIMIT_SET_SPRING CKGUID(0x19ea18da,0x4a8f7902)
#define PARAM_OP_TYPE_JSLIMIT_SET_VALUE CKGUID(0x7abb085e,0x464b16b4)
#define PARAM_OP_TYPE_JSLIMIT_SET_RES CKGUID(0x8eb56f2,0x44a40a2)

#define PARAM_OP_TYPE_JSLIMIT_GET_DAMPING CKGUID(0x74b33ddd,0x6faa11f1)
#define PARAM_OP_TYPE_JSLIMIT_GET_SPRING CKGUID(0x4440614,0x134514de)
#define PARAM_OP_TYPE_JSLIMIT_GET_VALUE CKGUID(0x455a525d,0x77e17e01)
#define PARAM_OP_TYPE_JSLIMIT_GET_RES CKGUID(0x7d1554b4,0x32a72cd3)


void ParamOpJSLimitSetDamping(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	if (!GetPMan()->getCurrentFactory())
	{
		return;
	}
	float value = 0.0f;
	p2->GetValue(&value);
	res->CopyValue(p1->GetRealSource(),false);
	vtTools::ParameterTools::SetParameterStructureValue<float>(res,0,value,false);
}

void ParamOpJSLimitGetDamping(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	float value = 0.0f;
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),0,false);
		}
	}
	res->SetValue(&value);
}
void ParamOpJSLimitSetSpring(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	if (!GetPMan()->getCurrentFactory())
	{
		return;
	}
	float value = 0.0f;
	p2->GetValue(&value);
	res->CopyValue(p1->GetRealSource(),false);
	vtTools::ParameterTools::SetParameterStructureValue<float>(res,1,value,false);
}


void ParamOpJSLimitGetSpring(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	float value = 0.0f;
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),1,false);
		}
	}
	res->SetValue(&value);
}
void ParamOpJSLimitSetValue(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	if (!GetPMan()->getCurrentFactory())
	{
		return;
	}
	float value = 0.0f;
	p2->GetValue(&value);
	res->CopyValue(p1->GetRealSource(),false);
	vtTools::ParameterTools::SetParameterStructureValue<float>(res,2,value,false);
}

void ParamOpJSLimitGetValue(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	float value = 0.0f;
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),2,false);
		}
	}
	res->SetValue(&value);
}







void ParamOpJSLimitSetRes(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	if (!GetPMan()->getCurrentFactory())
	{
		return;
	}
	float value = 0.0f;
	p2->GetValue(&value);
	res->CopyValue(p1->GetRealSource(),false);
	vtTools::ParameterTools::SetParameterStructureValue<float>(res,3,value,false);
}
void ParamOpJSLimitGetRes(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	float value = 0.0f;
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),3,false);
		}
	}
	res->SetValue(&value);
}






/************************************************************************/
/* spring :                                                                      */
/************************************************************************/
#define PARAM_OP_TYPE_JDRIVE_SET_DAMPING CKGUID(0x2ef2554b,0x50681945)
#define PARAM_OP_TYPE_JDRIVE_SET_SPRING CKGUID(0x657274d7,0x5c23079c)
#define PARAM_OP_TYPE_JDRIVE_SET_FORCE CKGUID(0x54d75463,0x2e343c56)
#define PARAM_OP_TYPE_JDRIVE_SET_TYPE CKGUID(0x37ff7a9d,0x1f1c3013)

#define PARAM_OP_TYPE_JDRIVE_GET_DAMPING CKGUID(0x3d4b76b7,0xf059e8)
#define PARAM_OP_TYPE_JDRIVE_GET_SPRING CKGUID(0x4abe6b69,0x56615834)
#define PARAM_OP_TYPE_JDRIVE_GET_FORCE CKGUID(0x4ff912df,0x40d1429)
#define PARAM_OP_TYPE_JDRIVE_GET_TYPE CKGUID(0x54a63237,0x1f8a3347)

void ParamOpJDriveSetDamping(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	if (!GetPMan()->getCurrentFactory())
	{
		return;
	}
	float value = 0.0f;
	p2->GetValue(&value);
	res->CopyValue(p1->GetRealSource(),false);
	vtTools::ParameterTools::SetParameterStructureValue<float>(res,0,value,false);
}

void ParamOpJDriveGetDamping(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	float value = 0.0f;
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),0,false);
		}
	}
	res->SetValue(&value);
}
void ParamOpJDriveSetSpring(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	if (!GetPMan()->getCurrentFactory())
	{
		return;
	}
	float value = 0.0f;
	p2->GetValue(&value);
	res->CopyValue(p1->GetRealSource(),false);
	vtTools::ParameterTools::SetParameterStructureValue<float>(res,1,value,false);
}


void ParamOpJDriveGetSpring(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	float value = 0.0f;
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),1,false);
		}
	}
	res->SetValue(&value);
}
void ParamOpJDriveSetForce(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	if (!GetPMan()->getCurrentFactory())
	{
		return;
	}
	float value = 0.0f;
	p2->GetValue(&value);
	res->CopyValue(p1->GetRealSource(),false);
	vtTools::ParameterTools::SetParameterStructureValue<float>(res,2,value,false);
}

void ParamOpJDriveGetForce(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	float value = 0.0f;
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),2,false);
		}
	}
	res->SetValue(&value);
}

void ParamOpJDriveSetType(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	if (!GetPMan()->getCurrentFactory())
	{
		return;
	}
	int value = 0;
	p2->GetValue(&value);
	res->CopyValue(p1->GetRealSource(),false);
	vtTools::ParameterTools::SetParameterStructureValue<int>(res,3,value,false);
}
void ParamOpJDriveGetType(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	int value = 0;
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<int>(p1->GetRealSource(),3,false);
		}
	}
	res->SetValue(&value);
}







void ParamOpJIsConnected(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{

	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));

	CK_ID targetID2;
	p2->GetValue(&targetID2);
	CK3dEntity *ent2  = static_cast<CK3dEntity*>(context->GetObject(targetID2));


	if (!pFactory::Instance()->jointCheckPreRequisites(ent,ent2,JT_Distance))
	{
		int result = 0;
		res->SetValue(&result);
		return;
	}


	// the world :  
	pWorld *worldA=GetPMan()->getWorldByBody(ent); 
	pWorld *worldB=GetPMan()->getWorldByBody(ent2); 
	if (!worldA)
	{
		worldA = worldB;
	}
	if (!worldA)
	{
		int result = 0;

		res->SetValue(&result);
		return;

	}

	pJoint*joint = static_cast<pJoint*>(worldA->getJoint(ent,ent2,JT_Any));
	int result = joint ?  1 : 0;
	res->SetValue(&result);
	return;

}


void PhysicManager::_RegisterParameterOperationsJoint()
{

	CKParameterManager *pm = m_Context->GetParameterManager();

	pm->RegisterOperationType(PARAMETER_OP_TYPE_IS_CONNECTED, "connected");
	pm->RegisterOperationFunction(PARAMETER_OP_TYPE_IS_CONNECTED,CKPGUID_BOOL,CKPGUID_3DENTITY,CKPGUID_3DENTITY,ParamOpJIsConnected);


	/************************************************************************/
	/* Drive                                                                      */
	/************************************************************************/

	pm->RegisterOperationType(PARAM_OP_TYPE_JDRIVE_SET_DAMPING, "jDsDamping");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JDRIVE_SET_DAMPING,VTS_JOINT_DRIVE,VTS_JOINT_DRIVE,CKPGUID_FLOAT,ParamOpJDriveSetDamping);

	pm->RegisterOperationType(PARAM_OP_TYPE_JDRIVE_GET_DAMPING, "jDgDamping");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JDRIVE_GET_DAMPING,CKPGUID_FLOAT,VTS_JOINT_DRIVE,CKPGUID_NONE,ParamOpJDriveGetDamping);

	pm->RegisterOperationType(PARAM_OP_TYPE_JDRIVE_SET_SPRING, "jDsSpring");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JDRIVE_SET_SPRING,VTS_JOINT_DRIVE,VTS_JOINT_DRIVE,CKPGUID_FLOAT,ParamOpJDriveSetSpring);

	pm->RegisterOperationType(PARAM_OP_TYPE_JDRIVE_GET_SPRING, "jDgSpring");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JDRIVE_GET_SPRING,CKPGUID_FLOAT,VTS_JOINT_DRIVE,CKPGUID_NONE,ParamOpJDriveGetSpring);


	pm->RegisterOperationType(PARAM_OP_TYPE_JDRIVE_SET_FORCE, "jDsForce");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JDRIVE_SET_FORCE,VTS_JOINT_DRIVE,VTS_JOINT_DRIVE,CKPGUID_FLOAT,ParamOpJDriveSetForce);

	pm->RegisterOperationType(PARAM_OP_TYPE_JDRIVE_GET_FORCE, "jDgForce");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JDRIVE_GET_FORCE,CKPGUID_FLOAT,VTS_JOINT_DRIVE,CKPGUID_NONE,ParamOpJDriveGetForce);


	pm->RegisterOperationType(PARAM_OP_TYPE_JDRIVE_SET_TYPE, "jDsType");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JDRIVE_SET_TYPE,VTS_JOINT_DRIVE,VTS_JOINT_DRIVE,VTE_PHYSIC_JDRIVE_TYPE,ParamOpJDriveSetType);

	pm->RegisterOperationType(PARAM_OP_TYPE_JDRIVE_GET_TYPE, "jDgType");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JDRIVE_GET_TYPE,VTE_PHYSIC_JDRIVE_TYPE,VTS_JOINT_DRIVE,CKPGUID_NONE,ParamOpJDriveGetType);

	/************************************************************************/
	/* Soft Limit                                                                      */
	/************************************************************************/

	pm->RegisterOperationType(PARAM_OP_TYPE_JSLIMIT_SET_DAMPING, "jSLsDamping");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JSLIMIT_SET_DAMPING,VTS_JOINT_SLIMIT,VTS_JOINT_SLIMIT,CKPGUID_FLOAT,ParamOpJSLimitSetDamping);

	pm->RegisterOperationType(PARAM_OP_TYPE_JSLIMIT_GET_DAMPING, "jSLgDamping");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JSLIMIT_GET_DAMPING,CKPGUID_FLOAT,VTS_JOINT_SLIMIT,CKPGUID_NONE,ParamOpJSLimitGetDamping);


	pm->RegisterOperationType(PARAM_OP_TYPE_JSLIMIT_SET_SPRING, "jSLsSpring");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JSLIMIT_SET_SPRING,VTS_JOINT_SLIMIT,VTS_JOINT_SLIMIT,CKPGUID_FLOAT,ParamOpJSLimitSetSpring);

	pm->RegisterOperationType(PARAM_OP_TYPE_JSLIMIT_GET_SPRING, "jSLgSpring");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JSLIMIT_GET_SPRING,CKPGUID_FLOAT,VTS_JOINT_SLIMIT,CKPGUID_NONE,ParamOpJSLimitGetSpring);


	pm->RegisterOperationType(PARAM_OP_TYPE_JSLIMIT_SET_VALUE, "jSLsValue");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JSLIMIT_SET_VALUE,VTS_JOINT_SLIMIT,VTS_JOINT_SLIMIT,CKPGUID_FLOAT,ParamOpJSLimitSetValue);

	pm->RegisterOperationType(PARAM_OP_TYPE_JSLIMIT_GET_VALUE, "jSLgValue");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JSLIMIT_GET_VALUE,CKPGUID_FLOAT,VTS_JOINT_SLIMIT,CKPGUID_NONE,ParamOpJSLimitGetValue);

	pm->RegisterOperationType(PARAM_OP_TYPE_JSLIMIT_SET_RES, "jSLsRes");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JSLIMIT_SET_RES,VTS_JOINT_SLIMIT,VTS_JOINT_SLIMIT,CKPGUID_FLOAT,ParamOpJSLimitSetRes);

	pm->RegisterOperationType(PARAM_OP_TYPE_JSLIMIT_GET_RES, "jSLgRes");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JSLIMIT_GET_RES,CKPGUID_FLOAT,VTS_JOINT_SLIMIT,CKPGUID_NONE,ParamOpJSLimitGetRes);

	/************************************************************************/
	/* spring :                                                                       */
	/************************************************************************/

	pm->RegisterOperationType(PARAM_OP_TYPE_JSPRING_SET_SPRING, "jSsSpring");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JSPRING_SET_SPRING,VTS_JOINT_SPRING,VTS_JOINT_SPRING,CKPGUID_FLOAT,ParamOpJSpringSetSpring);

	pm->RegisterOperationType(PARAM_OP_TYPE_JSPRING_GET_SPRING, "jSgSpring");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JSPRING_GET_SPRING,CKPGUID_FLOAT,VTS_JOINT_SPRING,CKPGUID_NONE,ParamOpJSpringGetSpring);


	pm->RegisterOperationType(PARAM_OP_TYPE_JSPRING_SET_DAMPER, "jSsDamper");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JSPRING_SET_DAMPER,VTS_JOINT_SPRING,VTS_JOINT_SPRING,CKPGUID_FLOAT,ParamOpJSpringSetDamper);

	pm->RegisterOperationType(PARAM_OP_TYPE_JSPRING_GET_DAMPER, "jSgDamper");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JSPRING_GET_DAMPER,CKPGUID_FLOAT,VTS_JOINT_SPRING,CKPGUID_NONE,ParamOpJSpringGetDamper);

	pm->RegisterOperationType(PARAM_OP_TYPE_JSPRING_SET_VALUE, "jSsValue");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JSPRING_SET_VALUE,VTS_JOINT_SPRING,VTS_JOINT_SPRING,CKPGUID_FLOAT,ParamOpJSpringSetValue);

	pm->RegisterOperationType(PARAM_OP_TYPE_JSPRING_GET_VALUE, "jSgValue");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JSPRING_GET_VALUE,CKPGUID_FLOAT,VTS_JOINT_SPRING,CKPGUID_NONE,ParamOpJSpringGetVAlue);


	/************************************************************************/
	/* pJLimit Structure Access :                                                                      */
	/************************************************************************/


	pm->RegisterOperationType(PARAM_OP_TYPE_JLIMIT_SET_VALUE, "jLsValue");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JLIMIT_SET_VALUE,VTS_JLIMIT,VTS_JLIMIT,CKPGUID_FLOAT,ParamOpJLimitSetValue);

	pm->RegisterOperationType(PARAM_OP_TYPE_JLIMIT_GET_VALUE, "jLgValue");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JLIMIT_GET_VALUE,CKPGUID_FLOAT,VTS_JLIMIT,CKPGUID_NONE,ParamOpJLimitGetValue);

	pm->RegisterOperationType(PARAM_OP_TYPE_JLIMIT_SET_RES, "jLsRestitution");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JLIMIT_SET_RES,VTS_JLIMIT,VTS_JLIMIT,CKPGUID_FLOAT,ParamOpJLimitSetRes);

	pm->RegisterOperationType(PARAM_OP_TYPE_JLIMIT_GET_RES, "jLgRestitution");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JLIMIT_GET_RES,CKPGUID_FLOAT,VTS_JLIMIT,CKPGUID_NONE,ParamOpJLimitGetRes);


	pm->RegisterOperationType(PARAM_OP_TYPE_JLIMIT_SET_HARD, "jLsHardness");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JLIMIT_SET_HARD,VTS_JLIMIT,VTS_JLIMIT,CKPGUID_FLOAT,ParamOpJLimitSetHard);

	pm->RegisterOperationType(PARAM_OP_TYPE_JLIMIT_GET_RES, "jLgHardness");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JLIMIT_GET_HARD,CKPGUID_FLOAT,VTS_JLIMIT,CKPGUID_NONE,ParamOpJLimitGetHard);


	/************************************************************************/
	/* pMotor Structure Acess :                                                                      */
	/************************************************************************/
	pm->RegisterOperationType(PARAM_OP_TYPE_JMOTOR_SET_TVEL, "jMsVel");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JMOTOR_SET_TVEL,VTS_JOINT_MOTOR,VTS_JOINT_MOTOR,CKPGUID_FLOAT,ParamOpJMotorSetTVel);

	pm->RegisterOperationType(PARAM_OP_TYPE_JMOTOR_GET_TVEL, "jMgVel");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JMOTOR_GET_TVEL,CKPGUID_FLOAT,VTS_JOINT_MOTOR,CKPGUID_NONE,ParamOpJMotorGetTVel);

	pm->RegisterOperationType(PARAM_OP_TYPE_JMOTOR_SET_MAXF, "jMsFMax");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JMOTOR_SET_MAXF,VTS_JOINT_MOTOR,VTS_JOINT_MOTOR,CKPGUID_FLOAT,ParamOpJMotorSetFMAX);

	pm->RegisterOperationType(PARAM_OP_TYPE_JMOTOR_GET_MAXF, "jMgFMax");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JMOTOR_GET_MAXF,CKPGUID_FLOAT,VTS_JOINT_MOTOR,CKPGUID_NONE,ParamOpJMotorGetFMAX);

	pm->RegisterOperationType(PARAM_OP_TYPE_JMOTOR_SET_SPIN_FREE, "jMsSpinFree");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JMOTOR_SET_SPIN_FREE,VTS_JOINT_MOTOR,VTS_JOINT_MOTOR,CKPGUID_BOOL,ParamOpJMotorSetSpinFree);

	pm->RegisterOperationType(PARAM_OP_TYPE_JMOTOR_GET_SPIN_FREE, "jMgSpingFree");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_JMOTOR_GET_SPIN_FREE,CKPGUID_BOOL,VTS_JOINT_MOTOR,CKPGUID_NONE,ParamOpJMotorGetSpinFree);






}

