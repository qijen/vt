#include <StdAfx.h>
#include "vtPhysXAll.h"




#define PARAMETER_OP_TYPE_STRING_TO_ATT CKGUID(0x3678447e,0x30362a74)

#define PARAM_OP_TYPE_CIS_INI_COLLISION CKGUID(0x4ec2349b,0x5edf7dd8)
#define PARAM_OP_TYPE_CHAS_CONTACT CKGUID(0x3ed57b83,0x47ad145f)
#define PARAM_OP_TYPE_CRAY_COLLISION CKGUID(0x53425880,0x1b540c2b)

#define PARAM_OP_TYPE_CGROUP_COLLISION CKGUID(0x7b762a1a,0x702e471c)

#define PARAM_OP_RC_ANY_BOUNDS CKGUID(0x31e415e2,0x61e42210)
#define PARAM_OP_RC_ANY_SHAPE CKGUID(0x62943427,0x31877a8f)



void ParamOpRayCastAnyShape(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{

	using namespace vtTools::ParameterTools;
	using namespace vtTools::BehaviorTools;

	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	CK_ID targetID  = vtTools::ParameterTools::GetValueFromParameterStruct<CK_ID>(p1->GetRealSource(),E_RC_WORLD,false);
	CK_ID oriRef  = vtTools::ParameterTools::GetValueFromParameterStruct<CK_ID>(p1->GetRealSource(),E_RC_ORI_REF,false);
	CK_ID dirRef  = vtTools::ParameterTools::GetValueFromParameterStruct<CK_ID>(p1->GetRealSource(),E_RC_DIR_REF,false);

	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{

		//////////////////////////////////////////////////////////////////////////
		// the world :  
		pWorld *world=GetPMan()->getWorld(targetID);
		if (!world)
		{
			int result = false;
			res->SetValue(&result);
			return;
		}

		CK3dEntity *rayOriRef= (CK3dEntity *)GetPMan()->m_Context->GetObject(oriRef);
		CK3dEntity *rayDirRef= (CK3dEntity *) GetPMan()->m_Context->GetObject(dirRef);

		VxVector ori  = vtTools::ParameterTools::GetValueFromParameterStruct<VxVector>(p1->GetRealSource(),E_RC_ORI,false);
		VxVector dir = vtTools::ParameterTools::GetValueFromParameterStruct<VxVector>(p1->GetRealSource(),E_RC_DIR,false);

		VxVector oriOut  = ori;
		if (rayOriRef)
		{
			rayOriRef->Transform(&oriOut,&ori);
		}

		//dir : 
		VxVector dirOut  = dir;
		if (rayDirRef)
		{
			VxVector dir,up,right;
			rayDirRef->GetOrientation(&dir,&up,&right);
			rayDirRef->TransformVector(&dirOut,&up);
		}

		int groups  = vtTools::ParameterTools::GetValueFromParameterStruct<int>(p1->GetRealSource(),E_RC_GROUPS,false);
		float length = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),E_RC_LENGTH,false);
		int types  = vtTools::ParameterTools::GetValueFromParameterStruct<int>(p1->GetRealSource(),E_RC_SHAPES_TYPES,false);


		CK_ID* paramids = static_cast<CK_ID*>(p1->GetReadDataPtr());	
		CKParameterOut* maskP = static_cast<CKParameterOut*>(p1->GetCKContext()->GetObject(paramids[E_RC_GROUPS_MASK]));

		NxGroupsMask mask;
		mask.bits0 = GetValueFromParameterStruct<int>(maskP,0);
		mask.bits1 = GetValueFromParameterStruct<int>(maskP,1);
		mask.bits2 = GetValueFromParameterStruct<int>(maskP,2);
		mask.bits3 = GetValueFromParameterStruct<int>(maskP,3);

		NxRay ray;
		ray.orig = getFrom(oriOut);
		ray.dir = getFrom(dirOut);


		//NxShape **shapes = new NxShape*[2];

		NxU32 total = world->getScene()->getTotalNbShapes();//world->getScene()->getNbDynamicShapes() + world->getScene()->getNbStaticShapes();
		//NxShape** shapes = NULL ;//(NxShape**)NxAlloca(nbShapes*sizeof(NxShape*));

		NxShape** shapes = (NxShape**)NxAlloca(total*sizeof(NxShape*));
		for (NxU32 i = 0; i < total; i++)  shapes[i] = NULL;

		//NxShape **shapes = NULL;
		int result  = world->getScene()->raycastAnyShape(ray,(NxShapesType)types,groups,length,&mask,shapes);
		
			NxShape *s = shapes[0];
			if (s)
			{
				pSubMeshInfo *sInfo  = static_cast<pSubMeshInfo*>(s->userData);
				if (sInfo->entID)
				{
					CKObject *obj = (CKObject*)GetPMan()->m_Context->GetObject(sInfo->entID);
					if (obj)
					{
						CK_ID  id = obj->GetID();
						res->SetValue(&id);
						//beh->SetOutputParameterObject(bbO_Shape,obj);
					}

				}

			}
		

		//res->SetValue(&result);
		return;
	}


}


void ParamOpRayCastAnyBounds(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{

	using namespace vtTools::ParameterTools;
	using namespace vtTools::BehaviorTools;

	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	CK_ID targetID  = vtTools::ParameterTools::GetValueFromParameterStruct<CK_ID>(p1->GetRealSource(),E_RC_WORLD,false);
	CK_ID oriRef  = vtTools::ParameterTools::GetValueFromParameterStruct<CK_ID>(p1->GetRealSource(),E_RC_ORI_REF,false);
	CK_ID dirRef  = vtTools::ParameterTools::GetValueFromParameterStruct<CK_ID>(p1->GetRealSource(),E_RC_DIR_REF,false);

	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{

		//////////////////////////////////////////////////////////////////////////
		// the world :  
		pWorld *world=GetPMan()->getWorld(targetID);
		if (!world)
		{
			int result = false;
			res->SetValue(&result);
			return;
		}

		CK3dEntity *rayOriRef= (CK3dEntity *)GetPMan()->m_Context->GetObject(oriRef);
		CK3dEntity *rayDirRef= (CK3dEntity *) GetPMan()->m_Context->GetObject(dirRef);

		VxVector ori  = vtTools::ParameterTools::GetValueFromParameterStruct<VxVector>(p1->GetRealSource(),E_RC_ORI,false);
		VxVector dir = vtTools::ParameterTools::GetValueFromParameterStruct<VxVector>(p1->GetRealSource(),E_RC_DIR,false);

		VxVector oriOut  = ori;
		if (rayOriRef)
		{
			rayOriRef->Transform(&oriOut,&ori);
		}

		//dir : 
		VxVector dirOut  = dir;
		if (rayDirRef)
		{
			VxVector dir,up,right;
			rayDirRef->GetOrientation(&dir,&up,&right);
			rayDirRef->TransformVector(&dirOut,&up);
		}

		int groups  = vtTools::ParameterTools::GetValueFromParameterStruct<int>(p1->GetRealSource(),E_RC_GROUPS,false);
		float length = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),E_RC_LENGTH,false);
		int types  = vtTools::ParameterTools::GetValueFromParameterStruct<int>(p1->GetRealSource(),E_RC_SHAPES_TYPES,false);


		CK_ID* paramids = static_cast<CK_ID*>(p1->GetReadDataPtr());	
		CKParameterOut* maskP = static_cast<CKParameterOut*>(p1->GetCKContext()->GetObject(paramids[E_RC_GROUPS_MASK]));

		NxGroupsMask mask;
		mask.bits0 = GetValueFromParameterStruct<int>(maskP,0);
		mask.bits1 = GetValueFromParameterStruct<int>(maskP,1);
		mask.bits2 = GetValueFromParameterStruct<int>(maskP,2);
		mask.bits3 = GetValueFromParameterStruct<int>(maskP,3);

		NxRay ray;
		ray.orig = getFrom(oriOut);
		ray.dir = getFrom(dirOut);

		int result  = world->getScene()->raycastAnyBounds(ray,(NxShapesType)types,groups,length,&mask);

		res->SetValue(&result);
		return;




	}


}







#define PARAM_OP_TYPE_MGET_SFRICTION CKGUID(0x58566e7b,0x494f208a)
#define PARAM_OP_TYPE_MGET_SFRICTIONV CKGUID(0x7af723af,0x7e222884)
#define PARAM_OP_TYPE_MGET_SDFRICTION CKGUID(0x10733925,0x77c37dba)
#define PARAM_OP_TYPE_MGET_SDFRICTIONV CKGUID(0x29131ba,0x3b2a6f07)
#define PARAM_OP_TYPE_MGET_ANIS CKGUID(0x255256df,0x61fe2f77)
#define PARAM_OP_TYPE_MGET_FMODE CKGUID(0x321f0335,0x589576df)
#define PARAM_OP_TYPE_MGET_RMODE CKGUID(0x1cb07645,0x79ff1329)
#define PARAM_OP_TYPE_MGET_XML_TYPE CKGUID(0x6fea2100,0x6667545b)
#define PARAM_OP_TYPE_MGET_RES CKGUID(0x41702512,0x78c48ca)


#define PARAM_OP_TYPE_MSET_SFRICTION CKGUID(0x2f2e1071,0x2d4623ec)
#define PARAM_OP_TYPE_MSET_SFRICTIONV CKGUID(0x31940b2a,0x67f43440)
#define PARAM_OP_TYPE_MSET_SDFRICTION CKGUID(0x205b0164,0x39d626d0)
#define PARAM_OP_TYPE_MSET_SDFRICTIONV CKGUID(0x11f36b7a,0x7877377b)
#define PARAM_OP_TYPE_MSET_ANIS CKGUID(0x36565c47,0x46002830)
#define PARAM_OP_TYPE_MSET_FMODE CKGUID(0x4ddb7828,0x4d111b71)
#define PARAM_OP_TYPE_MSET_RMODE CKGUID(0x5d2315b7,0x2cb834f3)


void ParamOpMGetF(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	int index  = vtTools::ParameterTools::GetValueFromParameterStruct<int>(p1->GetRealSource(),0,false);
	float value = 0;
	if (index !=0 && GetPMan()->getCurrentFactory())
	{
		if (GetPMan()->getDefaultConfig())
		{
			XString mName = vtAgeia::getEnumDescription(pm,VTE_XML_MATERIAL_TYPE,index);
			NxMaterialDesc *mDesrc  = pFactory::Instance()->createMaterialFromXML(mName.Str(),GetPMan()->getDefaultConfig());
			if (mDesrc)
			{
				value = mDesrc->staticFriction;
				res->SetValue(&value);
				delete mDesrc;
				mDesrc = NULL;
				return;
			}
		}
	}
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),2,false);
		}
	}
	res->SetValue(&value);
}
void ParamOpMGetDF(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	int index  = vtTools::ParameterTools::GetValueFromParameterStruct<int>(p1->GetRealSource(),0,false);
	float value = 0;
	if (index !=0 && GetPMan()->getCurrentFactory())
	{
		if (GetPMan()->getDefaultConfig())
		{
			XString mName = vtAgeia::getEnumDescription(pm,VTE_XML_MATERIAL_TYPE,index);
			NxMaterialDesc *mDesrc  = pFactory::Instance()->createMaterialFromXML(mName.Str(),GetPMan()->getDefaultConfig());
			if (mDesrc)
			{
				value = mDesrc->dynamicFriction;
				res->SetValue(&value);
				delete mDesrc;
				mDesrc = NULL;
				return;
			}
		}
	}
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),1,false);
		}
	}
	res->SetValue(&value);
}
void ParamOpMGetR(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	int index  = vtTools::ParameterTools::GetValueFromParameterStruct<int>(p1->GetRealSource(),0,false);
	float value = 0;
	if (index !=0 && GetPMan()->getCurrentFactory())
	{
		if (GetPMan()->getDefaultConfig())
		{
			XString mName = vtAgeia::getEnumDescription(pm,VTE_XML_MATERIAL_TYPE,index);
			NxMaterialDesc *mDesrc  = pFactory::Instance()->createMaterialFromXML(mName.Str(),GetPMan()->getDefaultConfig());
			if (mDesrc)
			{
				value = mDesrc->restitution;
				res->SetValue(&value);
				delete mDesrc;
				mDesrc = NULL;
				return;
			}
		}
	}
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),3,false);
		}
	}
	res->SetValue(&value);
}
void ParamOpMGetDFV(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	int index  = vtTools::ParameterTools::GetValueFromParameterStruct<int>(p1->GetRealSource(),0,false);
	float value = 0;
	if (index !=0 && GetPMan()->getCurrentFactory())
	{
		if (GetPMan()->getDefaultConfig())
		{
			XString mName = vtAgeia::getEnumDescription(pm,VTE_XML_MATERIAL_TYPE,index);
			NxMaterialDesc *mDesrc  = pFactory::Instance()->createMaterialFromXML(mName.Str(),GetPMan()->getDefaultConfig());
			if (mDesrc)
			{
				value = mDesrc->dynamicFrictionV;
				res->SetValue(&value);
				delete mDesrc;
				mDesrc = NULL;
				return;
			}
		}
	}
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),4,false);
		}
	}
	res->SetValue(&value);
}

void ParamOpMGetFV(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	int index  = vtTools::ParameterTools::GetValueFromParameterStruct<int>(p1->GetRealSource(),0,false);
	float value = 0;
	if (index !=0 && GetPMan()->getCurrentFactory())
	{
		if (GetPMan()->getDefaultConfig())
		{
			XString mName = vtAgeia::getEnumDescription(pm,VTE_XML_MATERIAL_TYPE,index);
			NxMaterialDesc *mDesrc  = pFactory::Instance()->createMaterialFromXML(mName.Str(),GetPMan()->getDefaultConfig());
			if (mDesrc)
			{
				value = mDesrc->staticFrictionV;
				res->SetValue(&value);
				delete mDesrc;
				mDesrc = NULL;
				return;
			}
		}
	}
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<float>(p1->GetRealSource(),5,false);
		}
	}
	res->SetValue(&value);
}
void ParamOpMGetA(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	int index  = vtTools::ParameterTools::GetValueFromParameterStruct<int>(p1->GetRealSource(),0,false);
	VxVector value(0.0f);
	if (index !=0 && GetPMan()->getCurrentFactory())
	{
		if (GetPMan()->getDefaultConfig())
		{
			XString mName = vtAgeia::getEnumDescription(pm,VTE_XML_MATERIAL_TYPE,index);
			NxMaterialDesc *mDesrc  = pFactory::Instance()->createMaterialFromXML(mName.Str(),GetPMan()->getDefaultConfig());
			if (mDesrc)
			{
				value = getFrom(mDesrc->dirOfAnisotropy);
				res->SetValue(&value);
				delete mDesrc;
				mDesrc = NULL;
				return;
			}
		}
	}

	if (p1)
	{
		if (p1->GetRealSource())
		{
			value = vtTools::ParameterTools::GetValueFromParameterStruct<VxVector>(p1->GetRealSource(),6,false);
		}
	}
	res->SetValue(&value);
}
void ParamOpMGetFMode(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	int index  = vtTools::ParameterTools::GetValueFromParameterStruct<int>(p1->GetRealSource(),0,false);
	int value = 0;
	if (index !=0 && GetPMan()->getCurrentFactory())
	{
		if (GetPMan()->getDefaultConfig())
		{
			XString mName = vtAgeia::getEnumDescription(pm,VTE_XML_MATERIAL_TYPE,index);
			NxMaterialDesc *mDesrc  = pFactory::Instance()->createMaterialFromXML(mName.Str(),GetPMan()->getDefaultConfig());
			if (mDesrc)
			{
				value = mDesrc->frictionCombineMode;
				res->SetValue(&value);
				delete mDesrc;
				mDesrc = NULL;
				return;
			}
		}
	}
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<int>(p1->GetRealSource(),7,false);
		}
	}
	res->SetValue(&value);
}
//////////////////////////////////////////////////////////////////////////
void ParamOpMGetRMode(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	int index  = vtTools::ParameterTools::GetValueFromParameterStruct<int>(p1->GetRealSource(),0,false);
	int value = 0;
	if (index !=0 && GetPMan()->getCurrentFactory())
	{
		if (GetPMan()->getDefaultConfig())
		{
			XString mName = vtAgeia::getEnumDescription(pm,VTE_XML_MATERIAL_TYPE,index);
			NxMaterialDesc *mDesrc  = pFactory::Instance()->createMaterialFromXML(mName.Str(),GetPMan()->getDefaultConfig());
			if (mDesrc)
			{
				value = mDesrc->restitutionCombineMode;
				res->SetValue(&value);
				delete mDesrc;
				mDesrc = NULL;
				return;
			}
		}
	}
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<int>(p1->GetRealSource(),8,false);
		}
	}
	res->SetValue(&value);
}
void ParamOpMGetXMat(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	int value = 0;
	if (p1)
	{
		if (p1->GetRealSource())
		{
			value  = vtTools::ParameterTools::GetValueFromParameterStruct<int>(p1->GetRealSource(),0,false);
		}
	}
	res->SetValue(&value);
}


#define PARAM_OP_TYPE_BGET_RESTITUTION CKGUID(0x58566e7b,0x494f208a)


void ParamOpStringToAdd(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	CKAttributeManager *am = (CKAttributeManager*)context->GetAttributeManager();

	//CKSTRING  test = 
	CKParameter *p = p1->GetRealSource();

	if (!p)	{ return;	}

	CKSTRING test = vtTools::ParameterTools::GetParameterAsString(p);
	if (!strlen(test))return;


	XString aName;
	XString aCat;
	XString aType;

	XStringTokenizer	tokizer(test, "|");
	const char*tok = NULL;
	int nb = 0;
	while ((tok=tokizer.NextToken(tok)) && nb < 3)
	{
		XString tokx(tok);

		switch (nb)
		{
			case 0:
				aName = tokx.Str();
			break;

			case 1:
				aCat= tokx.Str();
			break;

			case 2:
				aType = tokx.Str();
			break;
		}
		nb++;
	}

	CKAttributeType aIType = am->GetAttributeTypeByName(aName.Str());
	if (aIType!=-1)
	{
		res->SetValue(&aIType);
		return;
	}


	CKGUID pType =  pm->ParameterNameToGuid(aType.Str());
	CKParameterType pt = pm->ParameterGuidToType(pType);
	if (pt==-1)
	{
		pType  = CKPGUID_NONE;
	}

	aIType = am->RegisterNewAttributeType(aName.Str(),pType,CKCID_OBJECT);

	if (aCat.Length())
	{
		am->AddCategory(aCat.Str());
		am->SetAttributeCategory(aIType,aCat.Str());
	}


	res->SetValue(&aIType);
	
}

void PhysicManager::_RegisterParameterOperations()
{

	CKParameterManager *pm = m_Context->GetParameterManager();

	_RegisterParameterOperationsBody();
	_RegisterParameterOperationsJoint();
	_RegisterParameterOperationsVehicle();


	pm->RegisterOperationType(PARAMETER_OP_TYPE_STRING_TO_ATT, "convert");
	pm->RegisterOperationFunction(PARAMETER_OP_TYPE_STRING_TO_ATT,CKPGUID_ATTRIBUTE,CKPGUID_STRING,CKPGUID_NONE,ParamOpStringToAdd);


	pm->RegisterOperationType(PARAM_OP_TYPE_MGET_RMODE, "pMgRMode");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_MGET_RMODE,VTE_MATERIAL_COMBINE_MODE,VTS_MATERIAL,CKPGUID_NONE,ParamOpMGetRMode);

	pm->RegisterOperationType(PARAM_OP_TYPE_MGET_FMODE, "pMgFMode");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_MGET_FMODE,VTE_MATERIAL_COMBINE_MODE,VTS_MATERIAL,CKPGUID_NONE,ParamOpMGetFMode);

	pm->RegisterOperationType(PARAM_OP_TYPE_MGET_RES, "pMgRestitution");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_MGET_RES,CKPGUID_FLOAT,VTS_MATERIAL,CKPGUID_NONE,ParamOpMGetR);


	pm->RegisterOperationType(PARAM_OP_TYPE_MGET_SFRICTION, "pMgSFriction");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_MGET_SFRICTION,CKPGUID_FLOAT,VTS_MATERIAL,CKPGUID_NONE,ParamOpMGetF);
	
	pm->RegisterOperationType(PARAM_OP_TYPE_MGET_SFRICTIONV, "pMgSVFriction");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_MGET_SFRICTIONV,CKPGUID_FLOAT,VTS_MATERIAL,CKPGUID_NONE,ParamOpMGetFV);

	pm->RegisterOperationType(PARAM_OP_TYPE_MGET_SDFRICTION, "pMgDFriction");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_MGET_SDFRICTION,CKPGUID_FLOAT,VTS_MATERIAL,CKPGUID_NONE,ParamOpMGetDF);

	pm->RegisterOperationType(PARAM_OP_TYPE_MGET_SDFRICTIONV, "pMgDVFriction");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_MGET_SDFRICTIONV,CKPGUID_FLOAT,VTS_MATERIAL,CKPGUID_NONE,ParamOpMGetDFV);

	pm->RegisterOperationType(PARAM_OP_TYPE_MGET_ANIS, "pMgAnis");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_MGET_ANIS,CKPGUID_VECTOR,VTS_MATERIAL,CKPGUID_NONE,ParamOpMGetA);

	pm->RegisterOperationType(PARAM_OP_TYPE_MGET_XML_TYPE, "pMgType");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_MGET_XML_TYPE,VTE_XML_MATERIAL_TYPE,VTS_MATERIAL,CKPGUID_NONE,ParamOpMGetXMat);



	/************************************************************************/
	/*                                                                      */
	/************************************************************************/




	
/*
	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_FRICTION, "GetFriction");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_FRICTION,CKPGUID_FLOAT,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBGetFriction);

	

	
	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_LDAMP, "GetLinDamping");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_LDAMP,CKPGUID_FLOAT,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBGetLDamp);
	
	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_LDAMPT, "GetLinDampingT");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_LDAMPT,CKPGUID_FLOAT,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBGetLDampT);

	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_ADAMP, "GetAngDamping");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_ADAMP,CKPGUID_FLOAT,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBGetADamp);

	pm->RegisterOperationType(PARAM_OP_TYPE_BGET_ADAMPT, "GetAngDampingT");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_BGET_ADAMPT,CKPGUID_FLOAT,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpBGetADampT);

	pm->RegisterOperationType(PARAM_OP_TYPE_CIS_INI_COLLISION, "IsInCollision");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_CIS_INI_COLLISION,CKPGUID_BOOL,CKPGUID_3DENTITY,CKPGUID_3DENTITY,ParamOpCIsInCollision);

	pm->RegisterOperationType(PARAM_OP_TYPE_CHAS_CONTACT, "HasContact");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_CHAS_CONTACT,CKPGUID_3DENTITY,CKPGUID_3DENTITY,CKPGUID_NONE,ParamOpCHasContact);

	pm->RegisterOperationType(PARAM_OP_TYPE_CRAY_COLLISION, "RayCollision");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_CRAY_COLLISION,CKPGUID_3DENTITY,CKPGUID_3DENTITY,CKPGUID_VECTOR4,ParamOpCRayCollision);

	pm->RegisterOperationType(PARAM_OP_TYPE_CGROUP_COLLISION, "IsCollision");
	pm->RegisterOperationFunction(PARAM_OP_TYPE_CGROUP_COLLISION,CKPGUID_3DENTITY,CKPGUID_3DENTITY,CKPGUID_GROUP,ParamOpCIsInCollisionWithGroup);
*/


	

	





	pm->RegisterOperationType(PARAM_OP_RC_ANY_BOUNDS, "raycastAnyBounds");
	pm->RegisterOperationFunction(PARAM_OP_RC_ANY_BOUNDS,CKPGUID_BOOL,VTS_RAYCAST,CKPGUID_NONE,ParamOpRayCastAnyBounds);
	
	pm->RegisterOperationType(PARAM_OP_RC_ANY_SHAPE, "raycastAnyShape");
	pm->RegisterOperationFunction(PARAM_OP_RC_ANY_SHAPE,CKPGUID_3DENTITY,VTS_RAYCAST,CKPGUID_NONE,ParamOpRayCastAnyShape);


	
	




	
    
}

/*
void ParamOpBGetLVelocity(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);
void ParamOpBGetAVelocity(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);
void ParamOpBGetForce(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);
void ParamOpBGetTorque(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);
void ParamOpBGetFriction(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);
void ParamOpBisFixed(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);
void ParamOpBGetHType(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);

void ParamOpBGetLDamp(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);
void ParamOpBGetLDampT(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);

void ParamOpBGetADamp(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);
void ParamOpBGetADampT(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);

void ParamOpCIsInCollision(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);

void ParamOpCHasContact(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);
void ParamOpCRayCollision(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);

void ParamOpCIsInCollisionWithGroup(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2);

*/


/*
void ParamOpCIsInCollisionWithGroup(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{

	//////////////////////////////////////////////////////////////////////////
	//retrieve the position ori : 
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));

	//////////////////////////////////////////////////////////////////////////
	//retrieve the group
	CK_ID targetIDG;
	p2->GetValue(&targetIDG);
	CKGroup *group = static_cast<CKGroup*>(context->GetObject(targetIDG));


	//////////////////////////////////////////////////////////////////////////
	//our result object : 
	CK3dEntity* result = NULL;
	CK_ID id = 0;

	//////////////////////////////////////////////////////////////////////////
	//check our input object : 
	if (!ent)
	{
		res->SetValue(&id);
		return;
	}

	if (ent )
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 

		if (world)
		{
			VxVector pos,normal;
			float depth;
			result = world->CIsInCollision(ent,group,pos,normal,depth);	
			if (result)
			{
				id = result->GetID();
			}

		}
	}
	res->SetValue(&id);
}


//////////////////////////////////////////////////////////////////////////
void ParamOpCRayCollision(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	
	//////////////////////////////////////////////////////////////////////////
	//retrieve the position ori : 
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));

	//////////////////////////////////////////////////////////////////////////
	//our result object : 
	CK3dEntity* result = NULL;
	CK_ID id = 0;

	//////////////////////////////////////////////////////////////////////////
	//check our input object : 
	if (!ent)
	{
		res->SetValue(&id);
		return;
	}

	
	//////////////////////////////////////////////////////////////////////////
	//we retrieve ori of the ray by a box : 
	VxVector4 inVec(0,0,0,0);
	p2->GetValue(&inVec);

	VxVector oriOut(0,0,0);
	
	//////////////////////////////////////////////////////////////////////////
	//direction of the ray : 
	VxVector dirIn (inVec.x,inVec.y,inVec.z);
		
	pWorld *world=GetPMan()->getWorldByBody(ent); 

	if (world)
	{
		VxVector pos,normal;
		float depth;
		result = world->CRayCollision(oriOut,ent,dirIn,ent,inVec.w,true,pos,normal);		
		if (result)
		{
			id = result->GetID();
		}

	}
	res->SetValue(&id);
}
//////////////////////////////////////////////////////////////////////////
void ParamOpCHasContact(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());

	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));

	CK3dEntity* result = NULL;
	CK_ID id = 0;

	if (ent )
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		
		if (world)
		{
			VxVector pos,normal;
			float depth;
			result = world->CIsInCollision(ent,pos,normal,depth);		
			if (result)
			{
				id = result->GetID();
			}
			
		}
	}
	res->SetValue(&id);
}
//////////////////////////////////////////////////////////////////////////
void ParamOpCIsInCollision(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));

	CK_ID targetID2;
	p2->GetValue(&targetID2);
	CK3dEntity *ent2  = static_cast<CK3dEntity*>(context->GetObject(targetID2));
	
	int result = 0;

	if (ent && ent2)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		pWorld *world2=GetPMan()->getWorldByBody(ent2); 
		if (world && world2 && world == world2)
		{
			VxVector pos,normal;
			float depth;
            result = world->CIsInCollision(ent,ent2,pos,normal,depth);		
		}
	}
	res->SetValue(&result);
}
//////////////////////////////////////////////////////////////////////////
void ParamOpBGetFriction(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				float vec   = target->GetFriction();
				res->SetValue(&vec);
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void ParamOpBGetLDamp(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				float vec   = target->GetLinearDamping();
				res->SetValue(&vec);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void ParamOpBGetLDampT(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				float vec   = target->GetLinearDampingThreshold();
				res->SetValue(&vec);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void ParamOpBGetADamp(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				float vec   = target->GetLinearDamping();
				res->SetValue(&vec);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void ParamOpBGetADampT(CKContext* context, CKParameterOut* res, CKParameterIn* p1, CKParameterIn* p2)
{
	CKParameterManager *pm = static_cast<CKParameterManager *>(context->GetParameterManager());
	CK_ID targetID;
	p1->GetValue(&targetID);
	CK3dEntity *ent  = static_cast<CK3dEntity*>(context->GetObject(targetID));
	if (ent)
	{
		pWorld *world=GetPMan()->getWorldByBody(ent); 
		if (world)
		{
			pRigidBody*target= world->getBody(ent);
			if (target)
			{
				float vec   = target->GetLinearDampingThreshold();
				res->SetValue(&vec);
			}
		}
	}
}
*/
//////////////////////////////////////////////////////////////////////////