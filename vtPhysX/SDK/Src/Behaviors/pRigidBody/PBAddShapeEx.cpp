#include <StdAfx.h>
#include "pCommon.h"
#include "IParameter.h"


CKObjectDeclaration	*FillBehaviorPBAddShapeExDecl();
CKERROR CreatePBAddShapeExProto(CKBehaviorPrototype **pproto);
int PBAddShapeEx(const CKBehaviorContext& behcontext);
CKERROR PBAddShapeExCB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_BodyRef,
	bbI_HullType,
	bbI_Flags,
	bbI_Pivot,
	bbI_Mass,
	bbI_Collision,
	bbI_CCD,
	bbI_Material,
	bbI_Capsule,
	bbI_CCylinder,
	bbI_Wheel,
};


#define BB_SSTART 3
#define gPIMAP pInMap2322
BBParameter pInMap2322[]  =
{
	BB_PIN(bbI_BodyRef,CKPGUID_3DENTITY,"Body Reference",""),
	BB_PIN(bbI_HullType,VTE_COLLIDER_TYPE,"Hull Type","Sphere"),
	BB_PIN(bbI_Flags,VTF_BODY_FLAGS,"Flags","Collision,Sub Shape"),
	BB_SPIN(bbI_Pivot,VTS_PHYSIC_PIVOT_OFFSET,"Pivot",""),
	BB_SPIN(bbI_Mass,VTS_PHYSIC_MASS_SETUP,"Mass",""),
	BB_SPIN(bbI_Collision,VTS_PHYSIC_COLLISIONS_SETTINGS,"Collision","All,0,0.025f"),
	BB_SPIN(bbI_CCD,VTS_PHYSIC_CCD_SETTINGS,"CCD",""),
	BB_SPIN(bbI_Material,VTS_MATERIAL,"Material",""),
	BB_SPIN(bbI_Capsule,VTS_CAPSULE_SETTINGS_EX,"Capsule Settings",""),
	BB_SPIN(bbI_CCylinder,VTS_PHYSIC_CONVEX_CYLDINDER_WHEEL_DESCR,"Convex Cylinder Settings",""),
	BB_SPIN(bbI_Wheel,VTS_PHYSIC_WHEEL_DESCR,"Wheel Settings",""),
};

CKObjectDeclaration	*FillBehaviorPBAddShapeExDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBAddShapeEx");	
	od->SetCategory("Physic/Body");
	od->SetDescription("Adds an entity to the physic engine.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0xd8e2970,0x1efe7f65));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBAddShapeExProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePBAddShapeExProto
// FullName:  CreatePBAddShapeExProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePBAddShapeExProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBAddShapeEx");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PBAddShapeEx  

	PBAddShapeEx is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Adds a sub shape to a registered rigid body.<br>
	See <A HREF="PBPhysicalizeEx.cmo">PBPhysicalizeExSamples.cmo</A> for example.

	<h3>Technical Information</h3>

	\image html PBAddShapeEx.png

	<SPAN CLASS="in">In: </SPAN>triggers the process <BR>

	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed <BR>

	<BR>

	<SPAN CLASS="pin">Target:</SPAN>The 3D Entity associated to the rigid body <BR>

	<SPAN CLASS="pin">Flags: </SPAN>Flags to determine common properties for the desired body.It is possible to alter certain flags after creation. See #BodyFlags for more information <br> 

	-	<b>Range:</b> [BodyFlags] 
	-	<b>Default:</b>Moving,Collision,World Gravity<br>

	-	Ways to alter flags :
		-	Using \ref PBSetPar
		-	Using VSL : #pRigidBody::updateFlags()

	<SPAN CLASS="pin">Hull Type: </SPAN>The desired shape type. The intial shape can NOT be changed after creation. See #HullType for more information <br>

	-	<b>Range:</b> [HullType]<br>
	-	<b>Default:</b>Sphere<br>


	<br>
	Is utilizing #pRigidBody #pWorld #PhysicManager #pFactory::createRigidBody().<br>




	<h3>Optional Parameters</h3>

	All more specific parameters such as material or pivot offset must be enabled by the building block settings<br>

	<SPAN CLASS="pin">Pivot: </SPAN>Specifies the rigid bodies local shape offset (#pPivotSettings) <br><br>
	\image html pBPivotParameter.jpg

	-	<b>Offset Linear:</b> \copydoc pPivotSettings::localPosition
	-	<b>Offset Angular:</b> \copydoc pPivotSettings::localOrientation
	-	<b>Offset Reference:</b> \copydoc pPivotSettings::pivotReference

	<h4>Notes</h4><br>

	-	Alter or set the shape offset : 
	-	Using the built-in building blocks with "Physics" settings enabled : 
		-	"Set Position" 
		-	"Translate"
		-	"Set Orientation"
		-	#pRigidBody::setPosition() or #pRigidBody::setRotation()
		-	Attach attribute "Physics\pBPivotSettings" to :	
			-	3D-Entity
			-	its mesh
			-	or to the meshes material<br>

	<hr>

	<SPAN CLASS="pin">Mass: </SPAN>Overrides mass setup (#pMassSettings) <br><br>
	\image html pBMassParameter.jpg

	-	<b>New Density:</b>	\copydoc pMassSettings::newDensity

	-	<b>Total Mass:</b>	\copydoc pMassSettings::totalDensity

	-	<b>Offset Linear:</b>	\copydoc pMassSettings::localPosition

	-	<b>Offset Angular:</b>	\copydoc pMassSettings::localOrientation

	-	<b>Offset Reference:</b>	\copydoc pMassSettings::massReference

	<h4>Notes</h4><br>
	-	Alter or set mass settings : 
	-	Attach attribute "Physics\pBOptimization" to the :	
	-	3D-Entity
	-	its mesh
	-	or to the meshes material<br>
	-	#pRigidBody::updateMassFromShapes()

	<hr>

	<SPAN CLASS="pin">Collision: </SPAN>Overrides collsion settings (#pCollisionSettings) <br><br>
	\image html pBCollisionParameter.jpg

	-	<b>Collision Group: </b>	\copydoc pCollisionSettings::collisionGroup

	-	<b>Group Mask:</b>	\copydoc pCollisionSettings::groupsMask

	-	<b>Skin Width:</b>\copydoc pCollisionSettings::skinWidth

	<h4>Notes</h4><br>
	-	Alter or set collisions settings : 
		-	\ref PBSetPar. Collisions group can be set per sub shape.
		-	pRigidBody::setCollisionsGroup()
		-	pRigidBody::setGroupsMask()
		-	Attach attribute "Physics\pBCollisionSettings" to :	
			-	3D-Entity
			-	its mesh
			-	or to the meshes material <br>
	-	Please create custom groups in the Virtools "Flags and Enum manager" : "pBCollisionsGroup". This enumeration is stored in the cmo <br>

	<hr>

	<SPAN CLASS="pin">CCD: </SPAN>Specifies a CCD mesh. This parameter is NOT being used in this release.<br><br>
	\image html pBCCSettingsParameter.jpg

	<hr>


	<SPAN CLASS="pin">Material: </SPAN>Specifies a physic material(#pMaterial)<br><br>
	\image html pBMaterial.jpg

	-	<b>XML Link :</b> \copydoc pMaterial::xmlLinkID
	-	<b>Dynamic Friction :</b> \copydoc pMaterial::dynamicFriction
	-	<b>Static Friction: </b> \copydoc pMaterial::staticFriction
	-	<b>Restitution: </b> \copydoc pMaterial::restitution
	-	<b>Dynamic Friction V: </b> \copydoc pMaterial::dynamicFrictionV
	-	<b>Static Friction V : </b> \copydoc pMaterial::staticFrictionV
	-	<b>Direction Of Anisotropy: </b> \copydoc pMaterial::dirOfAnisotropy
	-	<b>Friction Combine Mode: </b> \copydoc pMaterial::frictionCombineMode
	-	<b>Restitution Combine Mode: </b> \copydoc pMaterial::restitutionCombineMode
	-	<b>Flags: </b> \copydoc pMaterial::flags

	<h4>Notes</h4><br>

	-	Alter or set a physic material is also possible by :
	-	\ref PBSetMaterial
	-	#pRigidBody::updateMaterialSettings()
	-	Attach attribute "Physics\pBMaterial" to :	
	-	3D-Entity
	-	its mesh
	-	or to the meshes material
	-	Using VSL : 

	<SPAN CLASS="NiceCode">
	\include pBMaterialSetup.vsl
	</SPAN>

	-	The enumeration "XML Link"  is being populated by the file "PhysicDefaults.xml" and gets updated on every reset. 
	-	If using settings from XML, the parameter gets updated too<br>

	<hr>



	<SPAN CLASS="pin">Capsule Settings: </SPAN>Overrides capsule default dimensions(#pCapsuleSettingsEx)<br><br>
	\image html pBCapsuleSettings.jpg

	-	<b>Radius :</b> \copydoc pCapsuleSettingsEx::radius
	-	<b>Height :</b> \copydoc pCapsuleSettingsEx::height


	<h4>Notes</h4><br>

	-	Setting a rigid bodies capsule dimension is also possible by :
	-	Attach the attribute "Physics\pCapsule" to :	
	-	3D-Entity
	-	its mesh
	-	or to the meshes material

	-	VSL :

	<SPAN CLASS="NiceCode">
	\include pBCapsuleEx.vsl
	</SPAN>

	<hr>


	<SPAN CLASS="pin">Convex Cylinder Settings: </SPAN>Overrides default convex cylinder settings(#pConvexCylinderSettings)<br><br>
	\image html pBConvexCylinder.jpg

	-	<b>Approximation :</b> \copydoc pConvexCylinderSettings::approximation

	-	<b>Radius :</b> \copydoc pConvexCylinderSettings::radius
	-	<b>Height :</b> \copydoc pConvexCylinderSettings::height

	-	<b>Forward Axis :</b> \copydoc pConvexCylinderSettings::forwardAxis
	-	<b>Forward Axis Reference:</b> \copydoc pConvexCylinderSettings::forwardAxisRef

	-	<b>Down Axis :</b> \copydoc pConvexCylinderSettings::downAxis
	-	<b>Down Axis Reference:</b> \copydoc pConvexCylinderSettings::downAxisRef

	-	<b>Right :</b> \copydoc pConvexCylinderSettings::rightAxis
	-	<b>Right Axis Reference:</b> \copydoc pConvexCylinderSettings::rightAxisRef

	-	<b>Build Lower Half Only :</b> \copydoc pConvexCylinderSettings::buildLowerHalfOnly

	-	<b>Convex Flags :</b> \copydoc pConvexCylinderSettings::convexFlags


	<h4>Notes</h4><br>

	-	Set a rigid bodies convex cylinder parameters by :
		-	Attach the attribute "Physics\pConvexCylinder" to :	
		-	3D-Entity
		-	its mesh
		-	or to the meshes material
		-	VSL :
		<SPAN CLASS="NiceCode">
			\include pBConvexCylinder.vsl
		</SPAN>

	<hr>


	<SPAN CLASS="pin">Wheel: </SPAN>Overrides wheel settings(#pWheelDescr)<br><br>
	\image html pBWheelSettings.jpg

		-	@todo Documentation !

	*/

	
	BB_EVALUATE_PINS(gPIMAP)
	BB_EVALUATE_SETTINGS(gPIMAP)

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	proto->SetBehaviorCallbackFct( PBAddShapeExCB );
	proto->SetFunction(PBAddShapeEx);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PBAddShapeEx
// FullName:  PBAddShapeEx
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PBAddShapeEx(const CKBehaviorContext& behcontext)
{

	using namespace vtTools::BehaviorTools;
	using namespace vtTools;
	using namespace vtTools::AttributeTools;
	using namespace vtTools::ParameterTools;


	//////////////////////////////////////////////////////////////////////////	//////////////////////////////////////////////////////////////////////////
	//
	//	objects		
	//
	
	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;


	
	//the object :
	CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
	if( !target ) 	bbErrorME("No Reference Object specified");

	//the world reference, optional used
	CK3dEntity*worldRef = NULL;

	//the world object, only used when reference has been specified
	pWorld *world = NULL;

	//final object description 
	pObjectDescr oDesc;
	
	pRigidBody *body = NULL;
	NxShape *shape = NULL;
	XString errMesg;

	//----------------------------------------------------------------
	//
	// sanity checks 
	//


	CK3dEntity *bodyReference = (CK3dEntity *) beh->GetInputParameterObject(bbI_BodyRef);
	if( !bodyReference) 	
		bbErrorME("No body reference specified");
	body  = GetPMan()->getBody(bodyReference);
	if( !body){
		errMesg.Format("Object %s is not physicalized",bodyReference->GetName());
		bbErrorME(errMesg.Str());
	}

	if( !target->GetCurrentMesh() ){
		errMesg.Format("Object %s has no mesh",target->GetName());
		bbErrorME(errMesg.Str());
	}

	shape= GetPMan()->getSubShape(target);
	if (shape && !body->isSubShape(bodyReference) )
	{
		errMesg.Format("Object %s is not a sub shape of %s",target,bodyReference->GetName());
		bbErrorME(errMesg.Str());
	}

	//get the parameter array
	BB_DECLARE_PIMAP;


	//----------------------------------------------------------------
	//
	// general settings
	//
	oDesc.hullType = (HullType)GetInputParameterValue<int>(beh,bbI_HullType);
	oDesc.flags = (BodyFlags)GetInputParameterValue<int>(beh,bbI_Flags);

	oDesc.version = pObjectDescr::E_OD_VERSION::OD_DECR_V1;
	VxQuaternion refQuad;target->GetQuaternion(&refQuad,body->GetVT3DObject());
	VxVector relPos;target->GetPosition(&relPos,body->GetVT3DObject());


	//	optional
	//		Pivot
	//
	BBSParameterM(bbI_Pivot,BB_SSTART);
	if (sbbI_Pivot)
	{
		CKParameter*pivotParameter = beh->GetInputParameter(BB_IP_INDEX(bbI_Pivot))->GetRealSource();
		if (pivotParameter)
		{
			IParameter::Instance()->copyTo(oDesc.pivot,pivotParameter);
			oDesc.mask |= OD_Pivot;
		}
	}
	//----------------------------------------------------------------
	//	optional
	//		mass
	//
	BBSParameterM(bbI_Mass,BB_SSTART);
	if (sbbI_Mass)
	{

		CKParameter*massParameter = beh->GetInputParameter(BB_IP_INDEX(bbI_Mass))->GetRealSource();
		if (massParameter)
		{
			IParameter::Instance()->copyTo(oDesc.mass,massParameter);
			oDesc.mask |= OD_Mass;
		}
	
	}

	//----------------------------------------------------------------
	//	optional
	//		collision
	//
	BBSParameterM(bbI_Collision , BB_SSTART);
	if (sbbI_Collision)
	{

		CKParameter*par = beh->GetInputParameter(BB_IP_INDEX(bbI_Collision))->GetRealSource();
		if (par)
		{
			IParameter::Instance()->copyTo(oDesc.collision,par);
			oDesc.mask |= OD_Collision;
		}
	}

	//----------------------------------------------------------------
	//	optional
	//		collision : CCD 
	//
	BBSParameterM(bbI_CCD, BB_SSTART);
	if (sbbI_CCD)
	{
		CKParameter*par = beh->GetInputParameter(BB_IP_INDEX(bbI_CCD))->GetRealSource();
		if (par)
		{
			IParameter::Instance()->copyTo(oDesc.ccd,par);
			oDesc.mask |= OD_CCD;
		}
	}

	//----------------------------------------------------------------
	//	optional
	//		Material
	//
	BBSParameterM(bbI_Material, BB_SSTART);
	if (sbbI_Material)
	{
		CKParameter*par = beh->GetInputParameter(BB_IP_INDEX(bbI_Material))->GetRealSource();
		if (par)
		{
			pFactory::Instance()->copyTo(oDesc.material,par);
			oDesc.mask |= OD_Material;
		}
	}

	//----------------------------------------------------------------
	//	optional
	//		capsule
	//
	BBSParameterM(bbI_Capsule, BB_SSTART);
	if (sbbI_Capsule)
	{
		if (oDesc.hullType == HT_Capsule)
		{
			CKParameter*par = beh->GetInputParameter(BB_IP_INDEX(bbI_Capsule))->GetRealSource();
			if (par)
			{
				IParameter::Instance()->copyTo(oDesc.capsule,par);
				oDesc.mask |= OD_Capsule;
			}
		}else{
			errMesg.Format("You attached a capsule parameter but the hull type is not capsule");
			bbWarning(errMesg.Str());
		}
	}

	//----------------------------------------------------------------
	//	optional
	//		convex cylinder
	//
	BBSParameterM(bbI_CCylinder, BB_SSTART);
	if (sbbI_CCylinder)
	{
		if (oDesc.hullType == HT_ConvexCylinder)
		{
			CKParameter*par = beh->GetInputParameter(BB_IP_INDEX(bbI_CCylinder))->GetRealSource();
			if (par)
			{
				pFactory::Instance()->copyTo(oDesc.convexCylinder,par,true);
				oDesc.mask |= OD_ConvexCylinder;
			}
		}else{
			errMesg.Format("You attached a convex cylinder parameter but the hull type is not a convex cylinder");
			bbWarning(errMesg.Str());
		}
	}

	//----------------------------------------------------------------
	//	optional
	//		convex cylinder
	//
	BBSParameterM(bbI_Wheel, BB_SSTART);
	if (sbbI_Wheel)
	{
		if (oDesc.hullType == HT_Wheel)
		{
			CKParameter*par = beh->GetInputParameter(BB_IP_INDEX(bbI_Wheel))->GetRealSource();
			if (par)
			{
				IParameter::Instance()->copyTo(oDesc.wheel,(CKParameterOut*)par);
				oDesc.mask |= OD_Wheel;
			}
		}else{
			errMesg.Format("You attached a wheel parameter but the hull type is not a wheel");
			bbWarning(errMesg.Str());
		}
	}
	
	//----------------------------------------------------------------
	//
	// create sub shape : 
	//
	shape  = pFactory::Instance()->createShape(bodyReference,oDesc,target,target->GetCurrentMesh(),relPos,refQuad);

	//----------------------------------------------------------------
	//
	// update input parameters
	//
	if (sbbI_Material)
	{
		CKParameterOut *par = (CKParameterOut*)beh->GetInputParameter(BB_IP_INDEX(bbI_Material))->GetRealSource();
		if (par)
		{
			pFactory::Instance()->copyTo(par,oDesc.material);
		}
	}
	//----------------------------------------------------------------
	//
	// error out
	//
	errorFound:
	{
		beh->ActivateOutput(0);
		return CKBR_GENERICERROR;
	}
	//----------------------------------------------------------------
	//
	// All ok
	//
	allOk:
	{
		beh->ActivateOutput(0);
		return CKBR_OK;
	}

	return 0;
}

//************************************
// Method:    PBAddShapeExCB
// FullName:  PBAddShapeExCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PBAddShapeExCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	CKContext* ctx = behcontext.Context;

	BB_DECLARE_PMAP;

	switch(behcontext.CallbackMessage) 
	{

	case CKM_BEHAVIORCREATE:
	case CKM_BEHAVIORLOAD:
		{
			BB_LOAD_PIMAP(gPIMAP,BB_SSTART);
			break;
		}
	case CKM_BEHAVIORDETACH:
		{
			BB_DESTROY_PIMAP;
			break;
		}

	case CKM_BEHAVIORATTACH:
		{
			BB_INIT_PIMAP(gPIMAP,BB_SSTART);
			break;
		}
	case CKM_BEHAVIORSETTINGSEDITED:
		{
			BB_REMAP_PIMAP(gPIMAP,BB_SSTART);
			break;
		}
	}
	return CKBR_OK;
}

