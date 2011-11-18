#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "vtStructHelper.h"
#include "vtAttributeHelper.h"

using namespace vtTools::AttributeTools;

int PhysicManager::getAttributeTypeByGuid(CKGUID guid)
{


	CKContext *ctx = GetPMan()->GetContext();

	CKAttributeManager *attMan = ctx->GetAttributeManager();
	CKParameterManager *parMan = ctx->GetParameterManager();

	int cCount = attMan->GetAttributeCount();
	for(int i  = 0 ; i < cCount ; i++)
	{
		CKSTRING name = attMan->GetAttributeNameByType(i);
		if ( parMan->ParameterTypeToGuid(attMan->GetAttributeParameterType(i)) == guid )
		{
			return i;
		}
	}

	return -1;
}

void PhysicManager::_RegisterDynamicEnumeration(XString file,XString enumerationName,CKGUID enumerationGuid,PFEnumStringFunction enumFunc,BOOL hidden)
{


	TiXmlDocument * defaultDoc  = loadDefaults(file.Str());
	if (!defaultDoc)
	{
		xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"Loading default config  :PhysicDefaults.xml: failed");
	}

	CKParameterManager *pm = m_Context->GetParameterManager();
	CKAttributeManager* attman = m_Context->GetAttributeManager();

	if (!getCurrentFactory())
	{
		pFactory *factory = new pFactory(this,getDefaultConfig());
		setCurrentFactory(factory);
	}

	pFactory *factory = pFactory::Instance();

	XString outList;
	if (defaultDoc)
		outList  = (factory->*enumFunc)(getDefaultConfig());

	if (!outList.Length())
		outList<< "None=0";

	CKParameterType pType = pm->ParameterGuidToType(enumerationGuid);
	if (pType==-1)
		pm->RegisterNewEnum(enumerationGuid,enumerationName.Str(),outList.Str());
	else{
		pm->ChangeEnumDeclaration(enumerationGuid,outList.Str());
	}

	CKParameterTypeDesc* param_type = pm->GetParameterTypeDescription(enumerationGuid);
	if (param_type && hidden)
		param_type->dwFlags|=CKPARAMETERTYPE_HIDDEN;

	xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"%s settings from xml detected : %s",enumerationName.Str(),outList.Str());



}

void PhysicManager::_RegisterDynamicParameters()
{

	
	TiXmlDocument * defaultDoc  = loadDefaults("PhysicDefaults.xml");
	if (!defaultDoc)
	{
		xLogger::xLog(XL_START,ELOGWARNING,E_LI_MANAGER,"Loading default config  :PhysicDefaults.xml: failed");
	}
	
	CKParameterManager *pm = m_Context->GetParameterManager();
	CKAttributeManager* attman = m_Context->GetAttributeManager();

	if (!getCurrentFactory())
	{
		pFactory *factory = new pFactory(this,getDefaultConfig());
		setCurrentFactory(factory);
	}
	

	int pType = 0;

	//////////////////////////////////////////////////////////////////////////
	//
	//		Material !
	//
/*
	XString materialList;
	if (defaultDoc)
	{
		materialList = pFactory::Instance()->_getMaterialsAsEnumeration(getDefaultConfig());
	}else{

		materialList << "None=0";
	}

	int pType = pm->ParameterGuidToType(VTE_XML_MATERIAL_TYPE);

	if (pType==-1)
		pm->RegisterNewEnum(VTE_XML_MATERIAL_TYPE,"pMaterialType",materialList.Str());
	else{
		pm->ChangeEnumDeclaration(VTE_XML_MATERIAL_TYPE,materialList.Str());
	}

	xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"materials : %s",materialList.Str());
*/

	_RegisterDynamicEnumeration("PhysicDefaults.xml","pMaterialType",VTE_XML_MATERIAL_TYPE,&pFactory::_getMaterialsAsEnumeration,false);

	_RegisterDynamicEnumeration("PhysicDefaults.xml","pBodyXMLInternalLink",VTS_PHYSIC_ACTOR_XML_SETTINGS_INTERN,&pFactory::_getBodyXMLInternalEnumeration,false);
	_RegisterDynamicEnumeration("PhysicDefaults.xml","pBodyXMLExternalLink",VTS_PHYSIC_ACTOR_XML_SETTINGS_EXTERN,&pFactory::_getBodyXMLExternalEnumeration,false);



	

	//////////////////////////////////////////////////////////////////////////
	//
	//	Vehicle Settings
	//
	XString vSListStr;
	if (defaultDoc)
		vSListStr= pFactory::Instance()->_getVehicleSettingsAsEnumeration(getDefaultConfig());

	if (!vSListStr.Length())
		vSListStr << "None=0";

	pType = pm->ParameterGuidToType(VTE_XML_VEHICLE_SETTINGS);
	if (pType==-1)
		pm->RegisterNewEnum(VTE_XML_VEHICLE_SETTINGS,"pVehicleSettingsLink",vSListStr.Str());
	else{
		pm->ChangeEnumDeclaration(VTE_XML_VEHICLE_SETTINGS,vSListStr.Str());
	}

	xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"vehicle settings : %s",vSListStr.Str());


	//////////////////////////////////////////////////////////////////////////
	//
	//	Wheel Settings
	//
	XString wSListStr;
	if (defaultDoc)
		wSListStr = pFactory::Instance()->_getVehicleWheelAsEnumeration(getDefaultConfig());

	if (!wSListStr.Length())
		wSListStr << "None=0";


	pType = pm->ParameterGuidToType(VTE_XML_WHEEL_SETTINGS);
	if (pType==-1)
		pm->RegisterNewEnum(VTE_XML_WHEEL_SETTINGS,"pWheelSettingsLink",wSListStr.Str());
	else{
		pm->ChangeEnumDeclaration(VTE_XML_WHEEL_SETTINGS,wSListStr.Str());
	}

	xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"wheel vehicle settings : %s",wSListStr.Str());


	CKParameterTypeDesc* param_type;
	param_type=pm->GetParameterTypeDescription(VTE_XML_WHEEL_SETTINGS);
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_HIDDEN;
	

	//////////////////////////////////////////////////////////////////////////
	//
	//		Wheel Tire Function
	//
	XString wTListStr;
	if (defaultDoc)
		wTListStr = pFactory::Instance()->_getVehicleTireFunctionAsEnumeration(getDefaultConfig());

	if (!wTListStr.Length())
		wTListStr << "None=0";

	pType = pm->ParameterGuidToType(VTE_XML_TIRE_SETTINGS);
	if (pType==-1)
		pm->RegisterNewEnum(VTE_XML_TIRE_SETTINGS,"pWheelSettingsLink",wTListStr.Str());
	else{
		pm->ChangeEnumDeclaration(VTE_XML_TIRE_SETTINGS,wTListStr.Str());
	}

	xLogger::xLog(XL_START,ELOGTRACE,E_LI_MANAGER,"wheel tire force settings : %s",wTListStr.Str());

	param_type=pm->GetParameterTypeDescription(VTE_XML_TIRE_SETTINGS);
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_HIDDEN;




	//_getVehicleTireFunctionAsEnumeration
	/*param_type=pm->GetParameterTypeDescription(CKPGUID_EVOLUTIONS);
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_HIDDEN;
*/


	_RegisterDynamicEnumeration("PhysicDefaults.xml","pMaterialType",VTE_XML_MATERIAL_TYPE,&pFactory::_getMaterialsAsEnumeration,false);

	

}


void	recheckWorldsFunc(int AttribType,CKBOOL Set,CKBeObject *obj,void *arg)
{

	int s = GetPMan()->getNbObjects();
	if (AttribType == GetPMan()->GetPAttribute())
	{
		pRigidBody *body = body = GetPMan()->getBody(pFactory::Instance()->getMostTopParent((CK3dEntity*)obj));
		if (body)
		{
			if (Set)
				if (!body->isSubShape(obj))
					body->_checkForNewSubShapes();
			else
				body->_checkForRemovedSubShapes();
		}
	}
	//CKParameterOut *pout = obj->GetAttributeParameter(GetPMan()->att_physic_object);

}

using namespace vtTools::ParameterTools;


vtTools::ParameterTools::StructurMember PBRigidBodyMemberMap[] =
{

	STRUCT_ATTRIBUTE(VTE_COLLIDER_TYPE,"Geometry","Sphere"),
	STRUCT_ATTRIBUTE(VTF_BODY_FLAGS,"Physic Flags","Moving Object,World Gravity,Enabled,Collision"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Density","1.0"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Skin Width","-1.0"),
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Mass Offset","0.0,0.0,0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_VECTOR,"Pivot Offset","0.0,0.0,0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_BOOL,"Hierarchy","FALSE"),
	STRUCT_ATTRIBUTE(CKPGUID_3DENTITY,"World","pDefaultWorld"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Total Mass","-1.0"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"New Density","-1.0"),
	STRUCT_ATTRIBUTE(CKPGUID_INT,"Collision Group","0"),
};

//pm->RegisterNewStructure(VTS_PHYSIC_PARAMETER,"pObject", ",Mass Offset,Pivot Offset,Hierarchy,World,New Density,Total Mass,Collision Group",
//VTE_COLLIDER_TYPE,VTF_BODY_FLAGS,CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_VECTOR,CKPGUID_VECTOR,CKPGUID_BOOL,CKPGUID_3DENTITY,CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_INT);


void PhysicManager::_RegisterParameters()
{

	CKParameterManager *pm = m_Context->GetParameterManager();
	CKAttributeManager* attman = m_Context->GetAttributeManager();

	

	attman->AddCategory("Physic");

	_RegisterWorldParameters();
	_RegisterBodyParameters();
	_RegisterBodyParameterFunctions();


	
	/************************************************************************/
	/* clothes :                                                                      */
	/************************************************************************/
	pm->RegisterNewFlags(VTF_VEHICLE_PROCESS_OPTIONS,"pVehicleProcessOptions","Lateral Damping=1,Longitudinal Damping=2,Slip Angle Damping=4,Slip Angle Delay=8,Slip Vector Tansa=16,SA Settle Down=32,Low Speed Check=64,Wheel LockAdjust=128,PhysXLoad=256,PhysXContacts=512,DoGregor=1024,DampVerticalVelocityReversal=2048,IntegrateImplicitVertical=4096,Diff Direct=8192,No Lateral=16384");

	pm->RegisterNewFlags(VTE_CLOTH_FLAGS,"pClothFlags","Pressure=1,Static=2,DisableCollision=4,SelfCollision=8,Gravity=32,Bending=64,BendingOrtho=128,Damping=256,CollisionTwoway=512,TriangleCollision=2048,Tearable=4096,Hardware=8192,ComDamping=16384,ValidBounds=32768,FluidCollision=65536,DisableCCD=131072,AddHere=262144,AttachToParentMainShape=524288,AttachToCollidingShapes=1048576,AttachToCore=2097152,AttachAttributes=4194304");
	pm->RegisterNewFlags(VTE_CLOTH_ATTACH_FLAGS,"pClothAttachFlags","Twoway=1,Tearable=2");

	pm->RegisterNewStructure(VTS_CLOTH_DESCR,"pClothDesc","Thickness,Density,Bending Stiffness,Stretching Stiffness,Damping Coefficient,Friction,Pressure,Tear Factor,Collision Response Coefficient,Attachment Response Coefficient,Attachment Tear Factor,To Fluid Response Coefficient,From Fluid Response Coefficient,Min Adhere Velocity,Solver Iterations,External Acceleration,Wind Acceleration,Wake Up Counter,Sleep Linear Velocity,Collision Group,Valid Bounds,Relative Grid Spacing,Flags,Tear Vertex Color,World Reference,Attachment Flags",CKPGUID_FLOAT,	CKPGUID_FLOAT,	CKPGUID_FLOAT,	CKPGUID_FLOAT,	CKPGUID_FLOAT,	CKPGUID_FLOAT,	CKPGUID_FLOAT,	CKPGUID_FLOAT,	CKPGUID_FLOAT,	CKPGUID_FLOAT,	CKPGUID_FLOAT,	CKPGUID_FLOAT,	CKPGUID_FLOAT,	CKPGUID_FLOAT,	CKPGUID_INT,	CKPGUID_VECTOR,	CKPGUID_VECTOR,	CKPGUID_FLOAT,	CKPGUID_FLOAT,	CKPGUID_INT,	CKPGUID_BOX,	CKPGUID_FLOAT,	VTE_CLOTH_FLAGS,	CKPGUID_COLOR,	CKPGUID_3DENTITY,	VTE_CLOTH_ATTACH_FLAGS);
	att_clothDescr= attman->RegisterNewAttributeType("Cloth",VTS_CLOTH_DESCR,CKCID_BEOBJECT);
	attman->SetAttributeCategory(att_clothDescr,"Physic");
	attman->SetAttributeDefaultValue(att_clothDescr,"0.01f;1.0f;1.0f;1.0f;0.5f;0.5f;1.0f;1.5f;0.2f;	0.2f;1.5f;1.0f;1.0f;1.0f;5;0.0f,0.0f,0.0f;0.0f,0.0f,0.0f;0.4f;-1.0f;0;0.0f,0.0f,0.0f,0.0f,0.0f,0.0f;0.25;Gravity;255,255,255,255;pDefaultWorld;Twoway");

	pm->RegisterNewStructure(VTS_CLOTH_METAL_DESCR,"pDeformableSettings","Impulse Threshold,Penetration Depth,Maximal Deformation Distance",CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_FLOAT);

	CKParameterTypeDesc* param_type=pm->GetParameterTypeDescription(VTS_CLOTH_METAL_DESCR);
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_HIDDEN;

	att_deformable = attman->RegisterNewAttributeType("Deformable Settings",VTS_CLOTH_METAL_DESCR,CKCID_BEOBJECT);
	attman->SetAttributeCategory(att_deformable,"Physic");
	
	//attman->SetAttributeCallbackFunction(att_clothDescr,recheckWorldsFunc,NULL);

	
	/************************************************************************/
	/* material                                                                     */
	/************************************************************************/
	pm->RegisterNewFlags(VTF_MATERIAL_FLAGS,"pMaterialFlags","Anisotropic=1,Disable Friction=16,Disable Strong Friction=32");
	pm->RegisterNewEnum(VTE_MATERIAL_COMBINE_MODE,"pFrictionCombineMode","Average=0,Min=1,Multiply=2,Max=3");
	pm->RegisterNewStructure(VTS_MATERIAL,"pMaterial","XML Link,DynamicFriction,Static Friction,Restitution,Dynamic Friction V,Static Friction V,Direction of Anisotropy,Friction Combine Mode,Restitution Combine Mode,Flags",VTE_XML_MATERIAL_TYPE,CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_VECTOR,VTE_MATERIAL_COMBINE_MODE,VTE_MATERIAL_COMBINE_MODE,VTF_MATERIAL_FLAGS);

	att_surface_props= attman->RegisterNewAttributeType("Material",VTS_MATERIAL,CKCID_BEOBJECT);
	attman->SetAttributeCategory(att_surface_props,"Physic");

	attman->SetAttributeCallbackFunction(att_surface_props,recheckWorldsFunc,NULL);


	/************************************************************************/
	/* Collision                                                                     */
	/************************************************************************/


	att_trigger= attman->RegisterNewAttributeType("Trigger",VTF_TRIGGER,CKCID_BEOBJECT);
	attman->SetAttributeCategory(att_trigger,"Physic");

	pm->RegisterNewFlags(VTF_RAY_HINTS,"pRayCastHints","Shape=1,Impact=2,Normal=4,Face Index=8,Distance=16,UV=32,Face Normal=64,Material=128");
	pm->RegisterNewStructure(VTS_RAYCAST,"pRayCast","World Reference,Ray Origin,Ray Origin Reference,Ray Direction,Ray Direction Reference,Length,Groups,Groups Mask,Shape Types",CKPGUID_3DENTITY,CKPGUID_VECTOR,CKPGUID_3DENTITY,CKPGUID_VECTOR,CKPGUID_3DENTITY,CKPGUID_FLOAT,CKPGUID_INT,VTS_FILTER_GROUPS,VTF_SHAPES_TYPE);



	/************************************************************************/
	/* Vehicle :                                                                      */
	/************************************************************************/
	//
	
	//pm->RegisterNewStructure(VTS_WHEEL_CONTACT,"pWheelContactData","Contact Point,Contact Normal,Longitudes Direction,Lateral Direction,Contact Force,Longitudes Slip,Lateral Slip,	Longitudes Impulse,Lateral Impulse,Other Material,Contact Position,	Contact Entity",CKPGUID_VECTOR,CKPGUID_VECTOR,CKPGUID_VECTOR,CKPGUID_VECTOR,	CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_FLOAT,CKPGUID_FLOAT,VTS_MATERIAL,CKPGUID_FLOAT,CKPGUID_3DENTITY);
	//REGISTER_CUSTOM_STRUCT("pJDistance",PS_JDISTANCE_MEMBERS,VTS_JOINT_DISTANCE,gSMapJDistance,FALSE);
	
	_RegisterVehicleParameters();
	_RegisterJointParameters();


	//registerWatchers();
}

XString getEnumDescription(CKParameterManager* pm,CKBeObject *object,int index)
{

	XString result="None";
	int pType = pm->ParameterGuidToType(VTE_XML_MATERIAL_TYPE);
	CKEnumStruct *enumStruct = pm->GetEnumDescByType(pType);
	if ( enumStruct )
	{
		for (int i = 0 ; i < enumStruct->GetNumEnums() ; i ++ )
		{
			if(i == index)
			{
				result = enumStruct->GetEnumDescription(i);
			}
		}
	}
	return result;
}

int getEnumIndexByDescription(CKParameterManager* pm,XString descr)
{

	int result =0;
	int pType = pm->ParameterGuidToType(VTE_XML_MATERIAL_TYPE);
	CKEnumStruct *enumStruct = pm->GetEnumDescByType(pType);
	if ( enumStruct )
	{
		for (int i = 0 ; i < enumStruct->GetNumEnums() ; i ++ )
		{

			if (!strcmp(enumStruct->GetEnumDescription(i),descr.CStr()))
			{
				return i;
			}
		}
	}
	return result;
}
