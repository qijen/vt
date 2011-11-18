#include <StdAfx.h>

#include "pCrossTypes.h"
#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "vtStructHelper.h"
#include "vtAttributeHelper.h"


using namespace vtTools::AttributeTools;
using namespace vtTools::ParameterTools;



/************************************************************************/
/*  Dominance Setup                                                                      */
/************************************************************************/


//////////////////////////////////////////////////////////////////////////
//
//		Dominance help member : 
//
StructurMember dominanceConstraint[]  =
{
	
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Dominance 0","0.0"),
	STRUCT_ATTRIBUTE(CKPGUID_FLOAT,"Dominance 1","1.0"),
};

//////////////////////////////////////////////////////////////////////////
//
//		Dominance help member : 
//
StructurMember dominanceItem[]  =
{

	STRUCT_ATTRIBUTE(VTE_PHYSIC_DOMINANCE_GROUP,"Dominance Group A","1"),
	STRUCT_ATTRIBUTE(VTE_PHYSIC_DOMINANCE_GROUP,"Dominance Group B","2"),
	STRUCT_ATTRIBUTE(VTS_PHYSIC_DOMINANCE_CONSTRAINT,"Dominance Constraint","1.0,0.0"),

};

StructurMember dominanceSetup[]  =
{

	STRUCT_ATTRIBUTE(VTS_PHYSIC_DOMINANCE_ITEM,"Settings 1","0"),
	STRUCT_ATTRIBUTE(VTS_PHYSIC_DOMINANCE_ITEM,"Settings 2","0"),
	STRUCT_ATTRIBUTE(VTS_PHYSIC_DOMINANCE_ITEM,"Settings 3","0"),
	STRUCT_ATTRIBUTE(VTS_PHYSIC_DOMINANCE_ITEM,"Settings 4","0"),
	STRUCT_ATTRIBUTE(VTS_PHYSIC_DOMINANCE_ITEM,"Settings 5","0"),
};
//STRUCT_ATTRIBUTE(CKPGUID_2DCURVE,"Settings 5","0"),

void PhysicManager::_RegisterWorldParameters()
{
	CKParameterManager *pm = m_Context->GetParameterManager();
	CKAttributeManager* attman = m_Context->GetAttributeManager();

	CKParameterTypeDesc* param_type = NULL;



	/************************************************************************/
	/* Dominance Structs										                                                                       */
	/************************************************************************/

	REGISTER_CUSTOM_STRUCT("pWDominanceConstraint",PS_W_DOMINANCE_CONSTRAINT,VTS_PHYSIC_DOMINANCE_CONSTRAINT,dominanceConstraint,true);

	// Dominance group as user friendly enumeration 
	pm->RegisterNewEnum(VTE_PHYSIC_DOMINANCE_GROUP,"pDominanceGroup","None=0,First=1,Second=2");
	param_type=pm->GetParameterTypeDescription(VTE_PHYSIC_DOMINANCE_GROUP);
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_USER;
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_TOSAVE;


	//if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_HIDDEN;


	REGISTER_CUSTOM_STRUCT("pDominanceItem",PS_W_DOIMINANCE,VTS_PHYSIC_DOMINANCE_ITEM,dominanceItem,true);

	REGISTER_CUSTOM_STRUCT("pDominanceSetup",PS_W_DOIMINANCE_SETUP,VTS_PHYSIC_DOMINANCE_SCENE_SETTINGS,dominanceSetup,true);

	REGISTER_STRUCT_AS_ATTRIBUTE("Dominance Group Setup",PS_W_DOIMINANCE_SETUP,PHYSIC_BODY_CAT,VTS_PHYSIC_DOMINANCE_SCENE_SETTINGS,CKCID_3DENTITY,dominanceSetup,true);


	/************************************************************************/
	/* world                                                                     */
	/************************************************************************/

	/*pm->RegisterNewEnum(VTE_PHYSIC_BODY_COLL_GROUP,"pBCollisionsGroup","All=0,MyObstacles=1,MyWheels=2");
	param_type=pm->GetParameterTypeDescription(VTE_PHYSIC_BODY_COLL_GROUP);
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_USER;
	if (param_type) param_type->dwFlags|=CKPARAMETERTYPE_HIDDEN;
	*/


	pm->RegisterNewStructure(VTS_PHYSIC_WORLD_PARAMETER,		"pWorldSettings", "Gravity,SkinWith",		CKPGUID_VECTOR,		CKPGUID_FLOAT		);

	att_world_object = attman->RegisterNewAttributeType("World",VTS_PHYSIC_WORLD_PARAMETER,CKCID_BEOBJECT);
	attman->SetAttributeCategory(att_world_object,"Physic");





}
