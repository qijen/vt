#include <StdAfx.h>
#include "vtPhysXAll.h"

#define PARAMETER_OP_TYPE_WHEEL_GETCONTACT CKGUID(0x74654a40,0x74ba3b5b)
#define PARAMETER_OP_TYPE_WDATA_GET_COLLIDER CKGUID(0x4dae6732,0x37740a24)
#define PARAMETER_OP_TYPE_WDATA_GET_MATERIAL CKGUID(0xa45301e,0x73e41d8f)
#define PARAMETER_OP_TYPE_WDATA_GET_CPOINT CKGUID(0x2f731ff8,0xa792311)


void ParamOpWheelContactGetCollider(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	if (p1)
	{
		if (p1->GetRealSource())
		{
			CK_ID value  = vtTools::ParameterTools::GetValueFromParameterStruct<CK_ID>(p1->GetRealSource(),E_WCD_CONTACT_ENTITY,false);
			res->SetValue(&value);
		}
	}
}

void ParamOpWheelContactGetMaterial(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	if (p1)
	{
		if (p1->GetRealSource())
		{
			CKParameterOut *materialParameter  = vtTools::ParameterTools::GetParameterFromStruct(p1->GetRealSource(),E_WCD_OTHER_MATERIAL_INDEX,false);
			if (materialParameter)
			{
				res->CopyValue(materialParameter);
			}
		}
	}
}

void ParamOpWheelGetContact(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{

	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	pRigidBody *body = NULL;

	//user comes usually with entity which is associated with the wheel and not with the body reference : 
	body = GetPMan()->getBody(pFactory::Instance()->getMostTopParent(ent));
	if (!body)
		return;

	pWheel2 *wheel =(pWheel2*)body->getWheel(ent);
	if (!wheel)return;

	pWheelContactData cData = *wheel->getContact();

	//copy result in the parameter operations result parameter : 
	pFactory::Instance()->copyTo(res,cData);

}



void PhysicManager::_RegisterParameterOperationsVehicle()
{

	CKParameterManager *pm = m_Context->GetParameterManager();
	

	pm->RegisterOperationType(PARAMETER_OP_TYPE_WHEEL_GETCONTACT, "pwGContact");
	pm->RegisterOperationFunction(PARAMETER_OP_TYPE_WHEEL_GETCONTACT,VTS_WHEEL_CONTACT,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpWheelGetContact);

	//////////////////////////////////////////////////////////////////////////
	// member retrieve of the type pWheelContactData : 

	//other entity : 
	pm->RegisterOperationType(PARAMETER_OP_TYPE_WDATA_GET_COLLIDER, "wcdGEntity");
	pm->RegisterOperationFunction(PARAMETER_OP_TYPE_WDATA_GET_COLLIDER,CKPGUID_3DENTITY,VTS_WHEEL_CONTACT,CKPGUID_NONE,ParamOpWheelContactGetCollider);

	//material
	pm->RegisterOperationType(PARAMETER_OP_TYPE_WDATA_GET_MATERIAL, "wcdGMaterial");
	pm->RegisterOperationFunction(PARAMETER_OP_TYPE_WDATA_GET_MATERIAL,VTS_MATERIAL,VTS_WHEEL_CONTACT,CKPGUID_NONE,ParamOpWheelContactGetMaterial);



}
