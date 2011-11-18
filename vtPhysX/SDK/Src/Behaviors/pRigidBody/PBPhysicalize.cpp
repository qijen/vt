#include <StdAfx.h>
#include "pCommon.h"

CKObjectDeclaration	*FillBehaviorPBPhysicalizeDecl();
CKERROR CreatePBPhysicalizeProto(CKBehaviorPrototype **pproto);
int PBPhysicalize(const CKBehaviorContext& behcontext);
CKERROR PBPhysicalizeCB(const CKBehaviorContext& behcontext);

enum bInputs
{
/*	bbI_TargetObject=0,*/
	bbI_Flags=0,
	bbI_HullType,
	bbI_Density,
	bbI_SkinWidth,
	bbI_MassShift,
	bbI_ShapeShift,
	bbI_TargetWorld,
	bbI_Hierachy,
	bbI_NewDensity,
	bbI_TotalMass,
    bbI_Material

};

enum bSettings
{
	bbS_USE_DEFAULT_WORLD,
	//bbS_USE_WORLD_SLEEP_SETTINGS,
	//bbS_USE_WORLD_DAMPING_SETTINGS,
	bbS_USE_WORLD_MATERIAL,
	bbS_ADD_ATTRIBUTES
};

//************************************
// Method:    FillBehaviorPBPhysicalizeDecl
// FullName:  FillBehaviorPBPhysicalizeDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPBPhysicalizeDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBPhysicalize");	
	od->SetCategory("Physic/Body");
	od->SetDescription("Adds an entity to the physic engine.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x186f7d29,0xe8901dc));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBPhysicalizeProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePBPhysicalizeProto
// FullName:  CreatePBPhysicalizeProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePBPhysicalizeProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBPhysicalize");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/* \page TestBlock
	PBPhysicalize  

	PBPhysicalize is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Applies an impulsive torque defined in the actor local coordinate frame to the actor.<br>
	See <A HREF="pBPhysicalize.cmo">pBPhysicalize.cmo</A> for example.
	<h3>Technical Information</h3>

	\image html PBPhysicalize.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	
	<SPAN CLASS="pin">Target: </SPAN>The 3D Entity associated to the rigid body.
	<BR>
	
	<SPAN CLASS="pin">Flags: </SPAN>Flags to determine common properties for the desired body. See #BodyFlags.
	<BR>
	<SPAN CLASS="pin">Hulltype: </SPAN>The desired shape type. See #HullType.
	<br>
		<SPAN CLASS="framedOrange">You can add or remove sub shapes on the fly by pRigidBody::addSubShape() or #PBAddShape.<br>
			The intial shape can NOT changed after the bodies registration.</SPAN>
		<BR>
	<SPAN CLASS="pin">Density: </SPAN>Density of the initial shape. This will be only used if <br> 
		"Hierarchy" =false and <br> 
		"New Density" !=0.0f or  "Total Mass" != 0.0f. #pRigidBody::updateMassFromShapes() can update the mass dynamically.
	<br>
	<br>
	<SPAN CLASS="pin">Skin Width: </SPAN>Specifies by how much shapes can interpenetrate. See #pRigidBody::setSkinWidth().
	<br>
	<br>
	<SPAN CLASS="pin">Mass Offset: </SPAN>Moves the mass center in the local bodies space.
	<br>
	<br>
	<SPAN CLASS="pin">Pivot Offset: </SPAN>Position of the initial shape in the bodies space.
	<br>
	<br>
	<SPAN CLASS="pin">Target World: </SPAN>Multiple worlds are provided. If not specified, it belongs to an automatically created default world(pDefaultWorld).This parameter must be enabled through the settings of this building block.
	<br>
	<br>
	<SPAN CLASS="pin">Hierarchy: </SPAN>If enabled, this function will parse the entities hierarchy and attaches children as additional collisions shapes. Those children must have the physic attribute attached whereby the sub shape flag must be 
			enabled(#BodyFlags). Sub shapes can be attached by #addSubShape() or #PBAddShape afterwards.
	<br>
	<br>
	<SPAN CLASS="pin">New Density: </SPAN>Density scale factor of the shapes belonging to the body.If you supply a non-zero total mass, 
	the bodies mass and inertia will first be computed as	above and then scaled to fit this total mass. See #pRigidBody::updateMassFromShapes().
	<br>
	<br>
	<SPAN CLASS="pin">Total Mass: </SPAN>Total mass if it has sub shapes.If you supply a non-zero density, 
	the bodies mass and inertia will first be computed as above and then scaled by this factor.See #pRigidBody::updateMassFromShapes().
	<br>
	<br>
	<SPAN CLASS="pin">Material: </SPAN>The material of this body. This parameter must be enabled through the settings of this building block.By default it is using the worlds material. The material of a body can be specified explicity by attaching
	a physic material attribute on the entity or its mesh or the meshs material. This is the lookup order : <br>
	- building block input parameter  <br>
	- entity mesh <br>
	- entities mesh material 
	<br>
	<br>
	<br>
	<SPAN CLASS="setting">Use Default World: </SPAN>Enables input for the world reference.
	<BR>
	<SPAN CLASS="setting">Use Worlds Material: </SPAN>Enables input for a physic material.
	<BR>
	<SPAN CLASS="setting">Add Attributes: </SPAN>Attaches the physic attribute to the entity.
	<BR>
	

	<BR>
	<h3>Warning</h3>

	<h3>Note</h3><br>
	<br>
	<br>
		Is utilizing #pRigidBody #pWorld #PhysicManager #pFactory::createBody().<br>
	
	<h3>VSL</h3><br>
		<SPAN CLASS="NiceCode">
		\include PBPhysicalize.cpp
		</SPAN>

	*/

	
	proto->DeclareInParameter("Flags",VTF_BODY_FLAGS,"Moving Object,Updates in Virtools,Auto Disable,World Gravity,Enabled,Collision");
	proto->DeclareInParameter("Hull type",VTE_COLLIDER_TYPE,"Sphere");
	proto->DeclareInParameter("Density",CKPGUID_FLOAT,"1.0f");
	proto->DeclareInParameter("Skin Width",CKPGUID_FLOAT,"-1.0f");
	
	proto->DeclareInParameter("Mass Offset",CKPGUID_VECTOR,"0.0f,0.0f,0.0f");
	proto->DeclareInParameter("Pivot Offset",CKPGUID_VECTOR,"0.0f,0.0f,0.0f");
	proto->DeclareInParameter("Target World Reference",CKPGUID_3DENTITY,"pDefaultWorld");
	proto->DeclareInParameter("Hierarchy",CKPGUID_BOOL,"FALSE");
	proto->DeclareInParameter("New Density",CKPGUID_FLOAT,"0.0f");
	proto->DeclareInParameter("Total Mass",CKPGUID_FLOAT,"0.0f");
	proto->DeclareInParameter("Material",VTS_MATERIAL);
		
	proto->DeclareSetting("Use Default World",CKPGUID_BOOL,"TRUE");
	proto->DeclareSetting("Use Worlds Material",CKPGUID_BOOL,"TRUE");
	proto->DeclareSetting("Add Attributes",CKPGUID_BOOL,"TRUE");

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	proto->SetBehaviorCallbackFct( PBPhysicalizeCB );
	proto->SetFunction(PBPhysicalize);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PBPhysicalize
// FullName:  PBPhysicalize
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PBPhysicalize(const CKBehaviorContext& behcontext)
{
	
	using namespace vtTools::BehaviorTools;
	using namespace vtTools;
	using namespace vtTools::AttributeTools;

	//////////////////////////////////////////////////////////////////////////
	//basic parameters
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	pFactory *pf = pFactory::Instance();

	

	//////////////////////////////////////////////////////////////////////////
	//settings
	int useDWorld;	beh->GetLocalParameterValue(bbS_USE_DEFAULT_WORLD,&useDWorld);
//	int useWorldSS;	beh->GetLocalParameterValue(bbS_USE_WORLD_SLEEP_SETTINGS,&useWorldSS);
//	int useWorldDS;	beh->GetLocalParameterValue(bbS_USE_WORLD_DAMPING_SETTINGS,&useWorldDS);
	int useWorldM;	beh->GetLocalParameterValue(bbS_USE_WORLD_MATERIAL,&useWorldM);
	int addAttributes;	beh->GetLocalParameterValue(bbS_ADD_ATTRIBUTES,&addAttributes);


	
	
	//////////////////////////////////////////////////////////////////////////
	//the object : 
	CK3dEntity *referenceObject = (CK3dEntity *) beh->GetTarget();
	if( !referenceObject ) return CKBR_OWNERERROR;

	

	//////////////////////////////////////////////////////////////////////////
	//the world  :  
	CK3dEntity*worldRef = NULL;
	if (!useDWorld)
	{
		worldRef = (CK3dEntity *) beh->GetInputParameterObject(bbI_TargetWorld);
	}

	// the world :  
	pWorld *world=GetPMan()->getWorld(worldRef,referenceObject); 
	if (!world)
	{
		beh->ActivateOutput(0);
		return 0;

	}

	pRigidBody*result = world->getBody(referenceObject);
	if (result)
	{
		beh->ActivateOutput(0);
		return 0;
	}


	//////////////////////////////////////////////////////////////////////////
	//pick up some parameters :  
	int flags  = GetInputParameterValue<int>(beh,bbI_Flags);
	int hType = GetInputParameterValue<int>(beh,bbI_HullType);
	float density  = GetInputParameterValue<float>(beh,bbI_Density);
	float skinWidth = GetInputParameterValue<float>(beh,bbI_SkinWidth);
	int hierarchy  = GetInputParameterValue<int>(beh,bbI_Hierachy);
	float newDensity = GetInputParameterValue<float>(beh,bbI_NewDensity);
	float totalMass = GetInputParameterValue<float>(beh,bbI_TotalMass);




	VxVector massOffset  = GetInputParameterValue<VxVector>(beh,bbI_MassShift);
	VxVector shapeOffset  = GetInputParameterValue<VxVector>(beh,bbI_ShapeShift);


	//////////////////////////////////////////////////////////////////////////
	// we remove the old physic attribute : 
	if (referenceObject->HasAttribute(GetPMan()->GetPAttribute()))
	{
		referenceObject->RemoveAttribute(GetPMan()->GetPAttribute());
	}
	referenceObject->SetAttribute(GetPMan()->GetPAttribute());
	
	SetAttributeValue<int>(referenceObject,GetPMan()->GetPAttribute(),E_PPS_HIRARCHY,&hierarchy);
	SetAttributeValue<int>(referenceObject,GetPMan()->GetPAttribute(),E_PPS_HULLTYPE,&hType);
	SetAttributeValue<int>(referenceObject,GetPMan()->GetPAttribute(),E_PPS_BODY_FLAGS,&flags);
	SetAttributeValue<float>(referenceObject,GetPMan()->GetPAttribute(),E_PPS_DENSITY,&density);
	SetAttributeValue<float>(referenceObject,GetPMan()->GetPAttribute(),E_PPS_NEW_DENSITY,&newDensity);
	SetAttributeValue<float>(referenceObject,GetPMan()->GetPAttribute(),E_PPS_TOTAL_MASS,&totalMass);
	SetAttributeValue<VxVector>(referenceObject,GetPMan()->GetPAttribute(),E_PPS_MASS_OFFSET,&massOffset);
	//SetAttributeValue<VxVector>(referenceObject,GetPMan()->GetPAttribute(),E_PPS_MASS_OFFSET,&massOffset);

	CK_ID wID  = world->getReference()->GetID();
	AttributeTools::SetAttributeValue<CK_ID>(referenceObject,GetPMan()->GetPAttribute(),E_PPS_WORLD,&wID);
	


	//////////////////////////////////////////////////////////////////////////
	//Material : 
	if (!useWorldM)
	{
		if (referenceObject->HasAttribute(GetPMan()->att_surface_props))
		{
			referenceObject->RemoveAttribute(GetPMan()->att_surface_props);
		}
		
		referenceObject->SetAttribute(GetPMan()->att_surface_props);

		CKParameterIn *pMatIn = beh->GetInputParameter(bbI_Material);
		CKParameter *pMat  = pMatIn->GetRealSource();
		CKParameterOut* pout = referenceObject->GetAttributeParameter(GetPMan()->att_surface_props);
		int error  = pout->CopyValue(pMat);
		pout->Update();
	}

	pRigidBody *body = pFactory::Instance()->createRigidBodyFull(referenceObject,worldRef);
	if (body)
	{
		body->translateLocalShapePosition(shapeOffset);
	}
	//GetPMan()->checkWorlds();

	if (!addAttributes)
	{
		referenceObject->RemoveAttribute(GetPMan()->GetPAttribute());
		referenceObject->RemoveAttribute(GetPMan()->att_surface_props);
	}

	beh->ActivateOutput(0);

	return 0;
}

//************************************
// Method:    PBPhysicalizeCB
// FullName:  PBPhysicalizeCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PBPhysicalizeCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	
	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
			
			DWORD useDWorld;
			beh->GetLocalParameterValue(bbS_USE_DEFAULT_WORLD,&useDWorld);
   			beh->EnableInputParameter(bbI_TargetWorld,!useDWorld);

/*
			DWORD useDWorldSS;
			beh->GetLocalParameterValue(bbS_USE_WORLD_SLEEP_SETTINGS,&useDWorldSS);
			beh->EnableInputParameter(bbI_SleepSettings,!useDWorldSS);

			DWORD useDWorldDS;
			beh->GetLocalParameterValue(bbS_USE_WORLD_DAMPING_SETTINGS,&useDWorldDS);
			beh->EnableInputParameter(bbI_DampingSettings,!useDWorldDS);
*/
			DWORD useDWorldM;
			beh->GetLocalParameterValue(bbS_USE_WORLD_MATERIAL,&useDWorldM);
			beh->EnableInputParameter(bbI_Material,!useDWorldM);


		}
		break;
	}
	return CKBR_OK;

}

