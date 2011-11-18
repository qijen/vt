#include <StdAfx.h>
#include "pCommon.h"
#include "IParameter.h"


CKObjectDeclaration	*FillBehaviorPBPhysicalizeExDecl();
CKERROR CreatePBPhysicalizeExProto(CKBehaviorPrototype **pproto);
int PBPhysicalizeEx(const CKBehaviorContext& behcontext);
CKERROR PBPhysicalizeExCB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_HullType,
	bbI_Flags,
	bbI_Density,
	bbI_XML,//	from here optional parameters
	bbI_World,	
	bbI_Pivot,
	bbI_Mass,
	bbI_Collision,
	bbI_CCD,
	bbI_Material,
	bbI_Optimization,
	bbI_Capsule,
	bbI_CCylinder,
};


#define BB_SSTART 3
#define gPIMAP pInMap232
BBParameter pInMap232[]  =
{

	BB_PIN(bbI_HullType,VTE_COLLIDER_TYPE,"Hull Type","Sphere"),
	BB_PIN(bbI_Flags,VTF_BODY_FLAGS,"Flags","Moving Object,World Gravity,Collision"),
	BB_PIN(bbI_Density,CKPGUID_FLOAT,"Density","1.0"),
	BB_SPIN(bbI_XML,VTS_PHYSIC_ACTOR_XML_SETUP,"XML Setup",""),// from here optional parameters
	BB_SPIN(bbI_World,CKPGUID_3DENTITY,"World Reference","pDefaultWorld"),
	BB_SPIN(bbI_Pivot,VTS_PHYSIC_PIVOT_OFFSET,"Pivot",""),
	BB_SPIN(bbI_Mass,VTS_PHYSIC_MASS_SETUP,"Mass",""),
	BB_SPIN(bbI_Collision,VTS_PHYSIC_COLLISIONS_SETTINGS,"Collision","All,0,0.025f"),
	BB_SPIN(bbI_CCD,VTS_PHYSIC_CCD_SETTINGS,"CCD",""),
	BB_SPIN(bbI_Material,VTS_MATERIAL,"Material",""),
	BB_SPIN(bbI_Optimization,VTS_PHYSIC_ACTOR_OPTIMIZATION,"Optimization",""),
	BB_SPIN(bbI_Capsule,VTS_CAPSULE_SETTINGS_EX,"Capsule Settings",""),
	BB_SPIN(bbI_CCylinder,VTS_PHYSIC_CONVEX_CYLDINDER_WHEEL_DESCR,"Convex Cylinder Settings",""),
};
CKObjectDeclaration	*FillBehaviorPBPhysicalizeExDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBPhysicalizeEx");	
	od->SetCategory("Physic/Body");
	od->SetDescription("Adds an entity to the physic engine.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x16b33c81,0x8a96d3e));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBPhysicalizeExProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePBPhysicalizeExProto
// FullName:  CreatePBPhysicalizeExProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePBPhysicalizeExProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBPhysicalizeEx");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PBPhysicalizeEx  

	PBPhysicalizeEx is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Registers an object in the physic engine.<br>
	See <A HREF="PBPhysicalizeExSamples.cmo">PBPhysicalizeEx.cmo</A> for example.
	
	<h3>Technical Information</h3>

	\image html PBPhysicalizeEx.png

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

	<SPAN CLASS="pin">Density: </SPAN>Density of the initial shape <br>
	
		-	<b>Range:</b> [0,inf]<br>
		-	<b>Default:</b>1.0f<br>
	
		-	Ways to change the mass : 
			-	Enable "Mass" in building block settings and set "New Density" or "Total Mass" non-zero. 
			-	Using VSL #pRigidBody::updateMassFromShapes()

	<br>
		Is utilizing #pRigidBody #pWorld #PhysicManager #pFactory::createRigidBody().<br>

	


	<h3>Optional Parameters</h3>

	All more specific parameters such as material or pivot offset must be enabled by the building block settings<br>

	PBPhysicalizeEx all settings enabled : <br>
	
	\image html PBPhysicalizeExCollapsed.jpg


	<SPAN CLASS="pin">XML Setup: </SPAN>This parameter is not being used in this release<br>
	
	<hr>
	<SPAN CLASS="pin">World Reference: </SPAN>World associated with this rigid body. Leave blank to use default world (3D - Frame "pDefaultWorld" with world attribute) <br>

		-	<b>Range:</b> [object range]<br>
		-	<b>Default:</b>NULL<br>

<hr>


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


	<SPAN CLASS="pin">Optimization: </SPAN>Specifies various optimizations(#pOptimization)<br><br>
	\image html pBOptimization.jpg

	-	<b>Transformation Locks :</b> Flags to lock a rigid body in certain degree of freedom. See also #pRigidBody::lockTransformation
	-	<b>Damping:</b> 
		
			-	<b>Linear Damping :</b> \copydoc  pRigidBody::setLinearDamping

			-	<b>Angular Damping :</b> \copydoc  pRigidBody::setAngularDamping

	-	<b>Sleeping:</b> 
		
			-	<b>Linear Sleep Velocity :</b> \copybrief  pRigidBody::setSleepLinearVelocity

			-	<b>Angular Sleep Velocity :</b> \copydoc  pRigidBody::setSleepAngularVelocity

			-	<b>Sleep Energy Threshold :</b> \copydoc  pRigidBody::setSleepEnergyThreshold
	
	-	<b>Solver Iteration :</b> \copydoc  pRigidBody::setSolverIterationCount

	-	<b>Dominance Group :</b> \copydoc  pRigidBody::setDominanceGroup

	-	<b>Compartment Id :</b> Not Implemented

	<h4>Notes</h4><br>

	-	Alter or set a rigid bodies optimization is also possible by :
		-	\ref PBSetPar
		-	#pRigidBody::updateOptimizationSettings()
		-	Attach the attribute "Physics\pBOptimization" to :	
			-	3D-Entity
			-	its mesh
			-	or to the meshes material<br>
		-	Using VSL before creation :

		
		\include pBOptimizationCreation.vsl
		

		-	Using VSL after creation :

		<SPAN CLASS="NiceCode">
			\include pBOptimizationAfterCreation.vsl
		</SPAN>

		

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
	
*/


	BB_EVALUATE_PINS(gPIMAP)
	BB_EVALUATE_SETTINGS(gPIMAP)



	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	proto->SetBehaviorCallbackFct( PBPhysicalizeExCB );
	proto->SetFunction(PBPhysicalizeEx);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PBPhysicalizeEx
// FullName:  PBPhysicalizeEx
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PBPhysicalizeEx(const CKBehaviorContext& behcontext)
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
	PhysicManager *pm = GetPMan();
	pFactory *pf = pFactory::Instance();

	//the object :
	CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
	if( !target ) 	bbErrorME("No Reference Object specified");

	//the world reference, optional used
	CK3dEntity*worldRef = NULL;

	//the world object, only used when reference has been specified
	pWorld *world = NULL;

	//final object description 
	pObjectDescr *oDesc = new pObjectDescr();

	pRigidBody *body = NULL;
	XString errMesg;

	//----------------------------------------------------------------
	//
	// sanity checks 
	//


	// rigid body 
	 GetPMan()->getBody(target);
	if( body){
		errMesg.Format("Object %s already registered.",target->GetName());
		bbErrorME(errMesg.Str());

	}
    
	//----------------------------------------------------------------
	//
	// 
	//
	if (!GetPMan()->isValid())
		GetPMan()->performInitialization();

	//////////////////////////////////////////////////////////////////////////	//////////////////////////////////////////////////////////////////////////
	//
	//		Collecting data. Stores all settings in a pObjectDescr.
	//

	//get the parameter array
	BB_DECLARE_PIMAP;

	//----------------------------------------------------------------
	//
	// generic settings
	//
	oDesc->hullType = (HullType)GetInputParameterValue<int>(beh,bbI_HullType);
	oDesc->flags = (BodyFlags)GetInputParameterValue<int>(beh,bbI_Flags);
	oDesc->density  = GetInputParameterValue<float>(beh,bbI_Density);

	//----------------------------------------------------------------
	//	optional
	//		world		  
	//
	BBSParameterM(bbI_World,BB_SSTART);
	if (sbbI_World)
	{
		worldRef = (CK3dEntity *) beh->GetInputParameterObject(BB_IP_INDEX(bbI_World));
		if (worldRef)
		{
			world  =  GetPMan()->getWorld(worldRef,target); 
			if (!world)
			{
				xLogger::xLog(ELOGERROR,E_LI_MANAGER,"World reference has been specified but no valid world object found. Switching to default world");
				goto errorFound;
			}
		}
	}
    
	//----------------------------------------------------------------
	//	optional
	//		Pivot
	//
	BBSParameterM(bbI_Pivot,BB_SSTART);
	if (sbbI_Pivot)
	{
		CKParameter*pivotParameter = beh->GetInputParameter(BB_IP_INDEX(bbI_Pivot))->GetRealSource();
		if (pivotParameter)
		{
	
			IParameter::Instance()->copyTo(oDesc->pivot,pivotParameter);
			oDesc->mask |= OD_Pivot;

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
		
			IParameter::Instance()->copyTo(oDesc->mass,massParameter);
			oDesc->mask |= OD_Mass;
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
			oDesc->collisionGroup = GetValueFromParameterStruct<int>(par,PS_BC_GROUP,false);

			CKParameterOut* maskPar = GetParameterFromStruct(par,PS_BC_GROUPSMASK);
			if (maskPar)
			{
				oDesc->groupsMask.bits0  = GetValueFromParameterStruct<int>(maskPar,0);
				oDesc->groupsMask.bits1  = GetValueFromParameterStruct<int>(maskPar,1);
				oDesc->groupsMask.bits2  = GetValueFromParameterStruct<int>(maskPar,2);
				oDesc->groupsMask.bits3  = GetValueFromParameterStruct<int>(maskPar,3);
			}
			
			oDesc->skinWidth = GetValueFromParameterStruct<float>(par,PS_BC_SKINWITDH,false);

			IParameter::Instance()->copyTo(oDesc->collision,par);
			oDesc->mask |= OD_Collision;
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
			IParameter::Instance()->copyTo(oDesc->ccd,par);
			oDesc->mask |= OD_CCD;
		}
	}
	//----------------------------------------------------------------
	//	optional
	//		optimization
	//
	BBSParameterM(bbI_Optimization, BB_SSTART);
	if (sbbI_Optimization)
	{

		CKParameter*par = beh->GetInputParameter(BB_IP_INDEX(bbI_Optimization))->GetRealSource();
		if (par)
		{
			IParameter::Instance()->copyTo(oDesc->optimization,par);
			oDesc->mask |= OD_Optimization;
			
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
			pFactory::Instance()->copyTo(oDesc->material,par);
			oDesc->mask |= OD_Material;
		}
	}

	//----------------------------------------------------------------
	//	optional
	//		capsule
	//
	BBSParameterM(bbI_Capsule, BB_SSTART);
	if (sbbI_Capsule)
	{
		if (oDesc->hullType == HT_Capsule)
		{
			CKParameter*par = beh->GetInputParameter(BB_IP_INDEX(bbI_Capsule))->GetRealSource();
			if (par)
			{
				IParameter::Instance()->copyTo(oDesc->capsule,par);
				oDesc->mask |= OD_Capsule;
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
		if (oDesc->hullType == HT_ConvexCylinder)
		{
			CKParameter*par = beh->GetInputParameter(BB_IP_INDEX(bbI_CCylinder))->GetRealSource();
			if (par)
			{
				pFactory::Instance()->copyTo(oDesc->convexCylinder,par,true);
				oDesc->mask |= OD_ConvexCylinder;
			}
		}else{
			errMesg.Format("You attached a convex cylinder parameter but the hull type is not a convex cylinder");
			bbWarning(errMesg.Str());
		}
	}

	
	oDesc->version = pObjectDescr::E_OD_VERSION::OD_DECR_V1;

	//////////////////////////////////////////////////////////////////////////	//////////////////////////////////////////////////////////////////////////
	//
	//	body creation
	//
	if (!body)
	{

		if(! (oDesc->flags & BF_SubShape) )
		{
			body  = pFactory::Instance()->createRigidBody(target,*oDesc);
		}
	}

	if (!body)
	{
		SAFE_DELETE(oDesc);
		bbErrorME("No Reference Object specified");
	}
	//////////////////////////////////////////////////////////////////////////	//////////////////////////////////////////////////////////////////////////
	//
	//	attribute creation, attach settings 		
	//
	if (oDesc->flags & BF_AddAttributes )
	{
		//body->saveToAttributes(oDesc);
		GetPMan()->copyToAttributes(*oDesc,target);
	}

	//----------------------------------------------------------------
	//
	// update input parameters
	//
	if (sbbI_Material)
	{
		CKParameterOut *par = (CKParameterOut*)beh->GetInputParameter(BB_IP_INDEX(bbI_Material))->GetRealSource();
		if (par)
		{
			pFactory::Instance()->copyTo(par,oDesc->material);
		}
	}
	//----------------------------------------------------------------
	//
	// cleanup 
	//
	//SAFE_DELETE(oDesc);

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
// Method:    PBPhysicalizeExCB
// FullName:  PBPhysicalizeExCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PBPhysicalizeExCB(const CKBehaviorContext& behcontext)
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

