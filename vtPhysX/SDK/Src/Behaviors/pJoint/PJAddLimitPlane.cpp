#include <StdAfx.h>
#include "pCommon.h"

CKObjectDeclaration	*FillBehaviorPJAddLimitPlaneDecl();
CKERROR CreatePJAddLimitPlaneProto(CKBehaviorPrototype **pproto);
int PJAddLimitPlane(const CKBehaviorContext& behcontext);
CKERROR PJAddLimitPlaneCB(const CKBehaviorContext& behcontext);

enum bbInputs
{

	bI_ObjectB,
	bI_Type,
	bI_Res,
	bI_LPoint,
	bI_LPointRef,
	bI_IsOnBodyB,
	bI_Anchor,
	bI_AnchorRef,
	bI_Axis,
	bI_AxisRef,
	bI_Coll
};

//************************************
// Method:    FillBehaviorPJAddLimitPlaneDecl
// FullName:  FillBehaviorPJAddLimitPlaneDecl
// Access:    public 
// Returns:   CKObjectDeclaration	*
// Qualifier:
//************************************
CKObjectDeclaration	*FillBehaviorPJAddLimitPlaneDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PJAddLimitPlane");	
	od->SetCategory("Physic/Joints");
	od->SetDescription("Adds a limit plane to a joint.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x150a1e5a,0xebc0c00));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePJAddLimitPlaneProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);

	return od;
}

//************************************
// Method:    CreatePJAddLimitPlaneProto
// FullName:  CreatePJAddLimitPlaneProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
CKERROR CreatePJAddLimitPlaneProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PJAddLimitPlane");
	if(!proto) return CKERR_OUTOFMEMORY;



	/*! \page PJAddLimitPlane  

	<br>
	PJAddLimitPlane is categorized in \ref Joints
	<br>

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Adds a limit plane. 
	<br>
	<br>See <A HREF="PJPointInPlane.cmo">PJPointInPlane.cmo</A>.


	<h3>Technical Information</h3>

	\image html PJAddLimitPlane.png

	This building blocks invokes #pJoint::setLimitPoint() and afterwards #pJoint::addLimitPlane() internally.
    

	<SPAN CLASS="in">In: </SPAN>triggers the process.
	<BR>
	<BR>
	
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	
	
	<SPAN CLASS="pin">Body B: </SPAN>The second body. Leave blank to create a joint constraint with the world. 
	<BR>

	<SPAN CLASS="pin">Joint Type:</SPAN> The joint type. This helps the building block to identify a joint constraint. As usual there can be only one joint of the type x between two bodies.<BR>


	<SPAN CLASS="pin">Restitution:</SPAN> Restitution of the limit plane. </br>
	-	<b>Range:</b> [0,1.0f)
	-	<b>Default:</b> 0.0 


	<SPAN CLASS="pin">Limit Point:</SPAN>The point specified in global coordinate frame.</br>
	-	<b>Range:</b> [Position Vector)

	<SPAN CLASS="pin">Limit Point Reference:</SPAN>Reference to transform limit point (local) into world space.</br>

	<SPAN CLASS="pin">Point is Body B:</SPAN>Set to true if the point is attached to the second actor.Otherwise it is attached to the first.</br>

	<SPAN CLASS="pin">Point in Plane:</SPAN>Point in the limit plane in global coordinates.</br>
	-	<b>Range:</b> [Position Vector)

	<SPAN CLASS="pin">Point in Plane Reference:</SPAN>Reference to transform plane "Point in Plane"(local then) into global coordinates.</br>

	<SPAN CLASS="pin">Normal:</SPAN>Normal for the limit plane in global coordinates.</br>
	-	<b>Range:</b> [Position Vector)

	<SPAN CLASS="pin">Normal Reference:</SPAN>Reference to transform plane "Normal"(local up then) into global coordinates.</br>

	


	<h3>VSL : Creation </h3><br>
	<SPAN CLASS="NiceCode">
		\include pJAddLimitPlane.vsl
	</SPAN>

	

	Is utilizing #pRigidBody #pWorld #PhysicManager #joint::setLimitPoint() #joint::addLimitPlane()

	*/

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	proto->SetBehaviorCallbackFct( PJAddLimitPlaneCB );
	proto->DeclareInParameter("Body B",CKPGUID_3DENTITY,"ball2");
	proto->DeclareInParameter("Type",VTE_JOINT_TYPE,"");
	proto->DeclareInParameter("Restitution",CKPGUID_FLOAT,"0.0f");
	
	proto->DeclareInParameter("Limit Point",CKPGUID_VECTOR);
	proto->DeclareInParameter("Limit Point Reference",CKPGUID_3DENTITY);
	proto->DeclareInParameter("Point Is On Body B",CKPGUID_BOOL,"TRUE");


	proto->DeclareInParameter("Point In Plane",CKPGUID_VECTOR);
	proto->DeclareInParameter("Point In Plane Reference",CKPGUID_3DENTITY);

	proto->DeclareInParameter("Normal",CKPGUID_VECTOR);
	proto->DeclareInParameter("Normal Reference",CKPGUID_3DENTITY);


	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);

	proto->SetFunction(PJAddLimitPlane);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PJAddLimitPlane
// FullName:  PJAddLimitPlane
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PJAddLimitPlane(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();

	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;

	if( beh->IsInputActive(0) )
	{
		

	
		beh->ActivateInput(0,FALSE);

		//////////////////////////////////////////////////////////////////////////
		//the object A: 
		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		CK3dEntity *targetB = (CK3dEntity *) beh->GetInputParameterObject(bI_ObjectB);
		
		if (!pFactory::Instance()->jointCheckPreRequisites(target,targetB,JT_Revolute))
		{

			return CK_OK;
		}
		
		// the world :  
		pWorld *worldA=GetPMan()->getWorldByBody(target); 
		pWorld *worldB=GetPMan()->getWorldByBody(targetB); 
		if (!worldA && ! worldB )
		{
			return 0;
		}
		if (!worldA)
		{
			worldA = worldB;
		}
				
		if (!worldA)
		{
			beh->ActivateOutput(0);
			return 0;
		}
		
		// the physic object A : 
		pRigidBody*bodyA= worldA->getBody(target);
		pRigidBody*bodyB= worldA->getBody(targetB);


		//anchor : 
		VxVector anchor0  = GetInputParameterValue<VxVector>(beh,bI_LPoint);
		VxVector anchorOut0  = anchor0;
		CK3dEntity*anchorReference0  =  (CK3dEntity *) beh->GetInputParameterObject(bI_LPointRef);
		if (anchorReference0)
		{
			anchorReference0->Transform(&anchorOut0,&anchor0);
		}


		//anchor : 
		VxVector anchor  = GetInputParameterValue<VxVector>(beh,bI_Anchor);
		VxVector anchorOut  = anchor;
		CK3dEntity*anchorReference  =  (CK3dEntity *) beh->GetInputParameterObject(bI_AnchorRef);
		if (anchorReference)
		{
			anchorReference->Transform(&anchorOut,&anchor);
		}
		//swing axis
		VxVector Axis  = GetInputParameterValue<VxVector>(beh,bI_Axis);
		VxVector axisOut  = Axis;
		CK3dEntity*axisReference  =  (CK3dEntity *) beh->GetInputParameterObject(bI_AxisRef);

		if (axisReference)
		{
			VxVector dir,up,right;
			axisReference->GetOrientation(&dir,&up,&right);
			axisReference->TransformVector(&axisOut,&up);
		}

		//////////////////////////////////////////////////////////////////////////
		int type = GetInputParameterValue<int>(beh,bI_Type);
		int isOnBodyB = GetInputParameterValue<int>(beh,bI_IsOnBodyB);
		float res = GetInputParameterValue<float>(beh,bI_Res);
		pJoint*joint = (worldA->getJoint(target,targetB,(JType)type));
		if(bodyA || bodyB)
		{
			if (joint)
			{
				if (XAbs(anchorOut0.SquareMagnitude()) >=0.01f)
					joint->setLimitPoint(anchorOut0,isOnBodyB);
	
				VxVector b = anchorOut;
				joint->addLimitPlane(axisOut,anchorOut,res);
			}
		}
		
		beh->ActivateOutput(0);
	}
	return 0;
}
//************************************
// Method:    PJAddLimitPlaneCB
// FullName:  PJAddLimitPlaneCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PJAddLimitPlaneCB(const CKBehaviorContext& behcontext)
{
	

	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	
	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
		
/*		
			DWORD twistLimit;
			beh->GetLocalParameterValue(1,&twistLimit);
			beh->EnableInputParameter(bbI_HighLimit,twistLimit);
			beh->EnableInputParameter(bbI_LowLimit,twistLimit);

			DWORD springSwing;
			beh->GetLocalParameterValue(0,&springSwing);
			beh->EnableInputParameter(bbI_Spring,springSwing);

			DWORD motor;
			beh->GetLocalParameterValue(2,&motor);
			beh->EnableInputParameter(bbI_Motor,motor);
*/
		break;
	}
	}
	return CKBR_OK;
}
