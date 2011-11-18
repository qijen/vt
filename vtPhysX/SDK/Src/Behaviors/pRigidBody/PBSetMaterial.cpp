#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPBSetMaterialDecl();
CKERROR CreatePBSetMaterialProto(CKBehaviorPrototype **pproto);
int PBSetMaterial(const CKBehaviorContext& behcontext);
CKERROR PBSetMaterialCB(const CKBehaviorContext& behcontext);

using namespace vtTools::BehaviorTools;
using namespace vtTools::ParameterTools;


enum bInputs
{
	I_XML,
	I_DFRICTION,
	I_SFRICTION,
	I_RES,
	I_DFRICTIONV,
	I_SFRICTIONV,
	I_ANIS,
	I_FCMODE,
	I_RCMODE,
	I_FLAGS,
};

CKERROR PBSetMaterialCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;
	CKContext *ctx = beh->GetCKContext();

	switch(behcontext.CallbackMessage) 
	{

		case CKM_BEHAVIORLOAD:
		case CKM_BEHAVIORATTACH:
		{

			DWORD outputResult = 0;
			beh->GetLocalParameterValue(1,&outputResult);
			beh->EnableOutputParameter(0,outputResult);

			break;

		}

		case CKM_BEHAVIORSETTINGSEDITED:
		{


			DWORD outputResult = 0;
			beh->GetLocalParameterValue(1,&outputResult);
			beh->EnableOutputParameter(0,outputResult);

			int fromParameter=0;
			beh->GetLocalParameterValue(0,&fromParameter);

			if (fromParameter)
			{
				if (beh->GetInputParameterCount() > 1 )
				{
					while ( beh->GetInputParameterCount())
					{
						CKDestroyObject(beh->RemoveInputParameter(0));
					}
					beh->CreateInputParameter("Material Settings",VTS_MATERIAL);
				}
			}

			if (!fromParameter)
			{
				if (beh->GetInputParameterCount() < 2 )
				{
					CKDestroyObject(beh->RemoveInputParameter(0));
					beh->CreateInputParameter("XML Link",VTE_XML_MATERIAL_TYPE);
					beh->CreateInputParameter("Dynamic Friction",CKPGUID_FLOAT);
					beh->CreateInputParameter("Static Friction",CKPGUID_FLOAT);
					beh->CreateInputParameter("Restitution",CKPGUID_FLOAT);
					beh->CreateInputParameter("Dynamic Friction V",CKPGUID_FLOAT);
					beh->CreateInputParameter("Static Friction V",CKPGUID_FLOAT);
					beh->CreateInputParameter("Direction of Anisotropy ",CKPGUID_VECTOR);
					beh->CreateInputParameter("Friction Combine Mode",VTE_MATERIAL_COMBINE_MODE);
					beh->CreateInputParameter("Restitution Combine Mode",VTE_MATERIAL_COMBINE_MODE);
					beh->CreateInputParameter("Flags",VTF_MATERIAL_FLAGS);
				}
			}
			break;
			
		}
		
	}
	return CKBR_OK;

}

//************************************
// Method:    FillBehaviorPBSetMaterialDecl
// FullName:  FillBehaviorPBSetMaterialDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration*FillBehaviorPBSetMaterialDecl()
{

	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBSetMaterial");	
	od->SetCategory("Physic/Body");
	od->SetDescription("Attaches and/or modifies the physic material of a rigid body or its sub shape");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x52523d82,0x5cb74a78));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBSetMaterialProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePBSetMaterialProto
// FullName:  CreatePBSetMaterialProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePBSetMaterialProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBSetMaterial");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");


	proto->DeclareInParameter("Material Settings",VTS_MATERIAL,"NONE");
	proto->DeclareSetting("From Material Parameter",CKPGUID_BOOL,"TRUE");
	proto->DeclareSetting("Output Result",CKPGUID_BOOL,"FALSE");
	proto->DeclareSetting("Add Attribute",CKPGUID_BOOL,"FALSE");

	proto->DeclareOutParameter("Result",VTS_MATERIAL);


	/*! \page PBSetMaterial  

	PBSetMaterial is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Modifies a rigid bodies physic material .<br>

	<h3>Technical Information</h3>

	\image html PBSetMaterial.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	
	<SPAN CLASS="pin">Target: </SPAN>The 3D Entity associated to the rigid body or its sub shape.
	<BR>
	

	<SPAN CLASS="pin">XML Link: </SPAN> The name of the material stored in the chosen xml file. If set to "None" the values from the parameter struct will be used. The list of avaiable materials will be updated from PhysicDefaults.xml on Dev reset.
	<BR>

	<SPAN CLASS="pin">Dynamic Friction: </SPAN> Coefficient of dynamic friction -- should be in [0, +inf]. If set to greater than staticFriction, the effective value of staticFriction will be increased to match.
	If in the flags "Anisotropic" is set, then this value is used for the primary direction of anisotropy (U axis).<br>

		<b>Range:</b> [0,inf]<br>
		<b>Default:</b> 0.0


	<SPAN CLASS="pin">Static Friction: </SPAN>Coefficient of static friction -- should be in [0, +inf]
	If in the flags "Anisotropic" is set, then this value is used for the primary direction of anisotropy (U axis)

		<b>Range:</b> [0,inf]<br>
		<b>Default:</b> 0.0

	<SPAN CLASS="pin">Restitution: </SPAN>Coefficient of restitution --  0 makes the object bounce as little as possible, higher values up to 1.0 result in more bounce.
	Note that values close to or above 1 may cause stability problems and/or increasing energy.

		<b>Range:</b> [0,1]<br>
		<b>Default:</b> 0.0

	<SPAN CLASS="pin">Dynamic Friction V: </SPAN>Anisotropic dynamic friction coefficient for along the secondary (V) axis of anisotropy. 
	This is only used if the flag Anisotropic is set.
	
		<b>Range:</b> [0,inf]<br>
		<b>Default:</b> 0.0

	<SPAN CLASS="pin">Static Friction V : </SPAN>Anisotropic static friction coefficient for along the secondary (V) axis of anisotropy. 
	This is only used if the flag Anisotropic is set.

		<b>Range:</b> [0,inf]<br>
		<b>Default:</b> 0.0


	<SPAN CLASS="pin">Dir Of Anisotropy: </SPAN>Shape space direction (unit vector) of anisotropy.
	This is only used if flags Anisotropic is set.

		<b>Range:</b> direction vector<br>
		<b>Default:</b> 1.0f,0.0f,0.0f

	<SPAN CLASS="pin">Flags: </SPAN>Flags, a combination of the bits defined by the enum ::MaterialFlags. 

	<SPAN CLASS="setting">From Material Parameter: </SPAN>Switches the input method between the custom parameter struct "pMaterial" and its enacpsulated input. The parameter "pMaterial" can be modified through parameter operations!
	<BR>

	<SPAN CLASS="setting">Output Result: </SPAN>Outputs a pMaterial parameter. This can be useful if a xml setup has be been chosen.
	<BR>

	<SPAN CLASS="setting">Add Attribute: </SPAN>Adds the resulting material as a parameter to the selected object.
	<BR>

	
	<h3>Warning</h3>
	-	By default, each body or sub shape retrieves the worlds default material(PhysicDefaults.xml - Default). This building block creates or modifies a new material.
	-	If the settings are invalid, the building block aborts and outputs a message in the console. 
	-	You can assign a material to a deformable or a cloth object.


	<br>
	<br>
	*/

	proto->SetBehaviorCallbackFct( PBSetMaterialCB );
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetFunction(PBSetMaterial);
	*pproto = proto;
	return CK_OK;
}


//************************************
// Method:    PBSetMaterial
// FullName:  PBSetMaterial
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************




int PBSetMaterial(const CKBehaviorContext& behcontext)
{
	
	CKBehavior* beh = behcontext.Behavior;

	CKContext* ctx = behcontext.Context;

	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	



	if( beh->IsInputActive(0) )
	{
		beh->ActivateInput(0,FALSE);



		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		if( !target )	bbSErrorME(E_PE_REF);

		pRigidBody *body = GetPMan()->getBody(target);
		if (!body)
			body = GetPMan()->getBody(target);

		if( !body )	bbSErrorME(E_PE_REF);

		int fromStruct=0;
		beh->GetLocalParameterValue(0,&fromStruct);

		int output=0;
		beh->GetLocalParameterValue(1,&output);
		
		int AddAsAttribute=0;
		beh->GetLocalParameterValue(2,&AddAsAttribute);



		pMaterial mat;
		mat.setToDefault();


		if (fromStruct)
		{
			CKParameter *mParameter = beh->GetInputParameter(0)->GetRealSource();
			if (mParameter)
			{
				pFactory::Instance()->copyTo(mat,mParameter);
			}
		}else
		{
			mat.xmlLinkID = GetInputParameterValue<int>(beh,I_XML);
			mat.dynamicFriction  = GetInputParameterValue<float>(beh,I_DFRICTION);
			mat.dynamicFrictionV  = GetInputParameterValue<float>(beh,I_DFRICTIONV);
			mat.staticFriction  = GetInputParameterValue<float>(beh,I_SFRICTION);
			mat.staticFrictionV  = GetInputParameterValue<float>(beh,I_SFRICTIONV);
			mat.restitution  = GetInputParameterValue<float>(beh,I_RES);
			mat.dirOfAnisotropy  = GetInputParameterValue<VxVector>(beh,I_ANIS);
			mat.restitutionCombineMode = (CombineMode)GetInputParameterValue<int>(beh,I_RCMODE);
			mat.frictionCombineMode = (CombineMode)GetInputParameterValue<int>(beh,I_FCMODE);
			mat.flags = GetInputParameterValue<int>(beh,I_FLAGS);
		}

		XString nodeName = vtAgeia::getEnumDescription(GetPMan()->GetContext()->GetParameterManager(),VTE_XML_MATERIAL_TYPE,mat.xmlLinkID);
		if ( mat.xmlLinkID !=0 )
		{
			
			bool err = pFactory::Instance()->loadFrom(mat,nodeName.Str(),pFactory::Instance()->getDefaultDocument());
			if (!err)
			{
				XString error; 
				error << "Couldn't load " << nodeName << " from XML!";
				bbErrorME(error.Str());
			}
			if (output)
			{
				pFactory::Instance()->copyTo(beh->GetOutputParameter(0),mat);
			}
		}

		if (!mat.isValid())
		{
			XString error; 
			error << "Material Settings " <<  nodeName << " are invalid!";
			bbErrorME(error.Str());
		}

		if (mat.isValid())
		{
			body->setShapeMaterial(mat,target);

			if (AddAsAttribute)
			{
				if (!target->HasAttribute(GetPMan()->att_surface_props))
					target->SetAttribute(GetPMan()->att_surface_props);
				pFactory::Instance()->copyTo(target->GetAttributeParameter(GetPMan()->att_surface_props),mat);
			}
		}
	}
	beh->ActivateOutput(0);
	return 0;
}
