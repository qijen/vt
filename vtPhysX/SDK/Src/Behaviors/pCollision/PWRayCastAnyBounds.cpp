#include <StdAfx.h>
#include "pCommon.h"

#include "pWorldCallbacks.h"


CKObjectDeclaration	*FillBehaviorPWRayCastAnyBoundsDecl();
CKERROR CreatePWRayCastAnyBoundsProto(CKBehaviorPrototype **pproto);
int PWRayCastAnyBounds(const CKBehaviorContext& behcontext);
CKERROR PWRayCastAnyBoundsCB(const CKBehaviorContext& behcontext);

CKObjectDeclaration	*FillBehaviorPWRayCastAnyBoundsDecl()
{

	CKObjectDeclaration *od = CreateCKObjectDeclaration("PWRayCastAnyBounds");	
	od->SetCategory("Physic/Collision");
	od->SetDescription("Performs a ray cast test");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x6e155f2f,0x6d634931));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePWRayCastAnyBoundsProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePWRayCastAnyBoundsProto
// FullName:  CreatePWRayCastAnyBoundsProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
enum bInput
{
	bbI_RayOri,
	bbI_RayOriRef,
	bbI_RayDir,
	bbI_RayDirRef,
	bbI_Length,
	bbI_ShapesType,
	bbI_Groups,
	bbI_Mask,
};

enum bbS
{
	bbS_Groups=0,
	bbS_Mask=1
};


enum bbOT
{
	bbOT_Yes,
	bbOT_No,

};
CKERROR CreatePWRayCastAnyBoundsProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PWRayCastAnyBounds");
	if(!proto) return CKERR_OUTOFMEMORY;

	/*! \page PWRayCastAnyBounds  

	PWRayCastAnyBounds is categorized in \ref Collision

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Performs a ray cast test.<br>
	See <A HREF="PWRayCasts.cmo">PWRayCasts.cmo</A> for example.


	<h3>Technical Information</h3>

	\image html PWRayCastAnyBounds.png

	<SPAN CLASS="in">In: </SPAN>Triggers the process.
	<BR>
	


	<SPAN CLASS="out">Yes: </SPAN>Hit occured.
	<BR>
	<SPAN CLASS="out">No: </SPAN>No hits.
	<BR>
	





	<SPAN CLASS="pin">Target: </SPAN>World Reference. pDefaultWorld!
	<BR>
	<SPAN CLASS="pin">Ray Origin: </SPAN>Start of the ray.
	<BR>
	<SPAN CLASS="pin">Ray Origin Reference: </SPAN>Reference object to determine the start of the ray. Ray Origin becomes transformed if an object is given.
	<BR>
	<SPAN CLASS="pin">Ray Direction: </SPAN>Direction of the ray.
	<BR>
	<SPAN CLASS="pin">Ray Direction Reference: </SPAN>Reference object to determine the direction of the ray. Ray Direction becomes transformed if an object is given. Up axis will be used then.
	<BR>
	<SPAN CLASS="Length">Length: </SPAN>Lenght of the ray.
	<BR>
	<SPAN CLASS="pin">Shapes Types: </SPAN>Adds static and/or dynamic shapes to the test.
	<BR>

	<SPAN CLASS="pin">Groups: </SPAN>Includes specific groups to the test. 
	<BR>
	<SPAN CLASS="pin">Groups Mask: </SPAN>Alternative mask used to filter shapes. See #pRigidBody::setGroupsMask
	<BR>

	<h3>Note</h3>
	This is as parameter opertion avaiable. See \ref CollisionsOps !
	<br>
	<br>

	Is utilizing  #pWorld::raycastAnyBounds().<br>
	
	*/
	
	proto->DeclareInput("In");
	
	proto->DeclareOutput("Yes");
	proto->DeclareOutput("No");
	


	proto->DeclareInParameter("Ray Origin",CKPGUID_VECTOR);
	proto->DeclareInParameter("Ray Origin Reference",CKPGUID_3DENTITY);
	proto->DeclareInParameter("Ray Direction",CKPGUID_VECTOR);
	proto->DeclareInParameter("Ray Direction Reference",CKPGUID_3DENTITY);
	proto->DeclareInParameter("Length",CKPGUID_FLOAT);
	proto->DeclareInParameter("Shapes Type",VTF_SHAPES_TYPE);

	proto->DeclareInParameter("Groups",CKPGUID_INT);
	proto->DeclareInParameter("Filter Mask",VTS_FILTER_GROUPS);

	
	proto->DeclareSetting("Groups",CKPGUID_BOOL,"false");
	proto->DeclareSetting("Groups Mask",CKPGUID_BOOL,"false");


	
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetBehaviorCallbackFct( PWRayCastAnyBoundsCB );
	proto->SetFunction(PWRayCastAnyBounds);
	*pproto = proto;
	return CK_OK;
}
enum bOutputs
{
	bbO_None,
	bbO_Enter,
	bbO_Stay,
	bbO_Leave,
};

//************************************
// Method:    PWRayCastAnyBounds
// FullName:  PWRayCastAnyBounds
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PWRayCastAnyBounds(const CKBehaviorContext& behcontext)
{

	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	
	pFactory *pf = pFactory::Instance();

	using namespace vtTools::BehaviorTools;
	using namespace vtTools::ParameterTools;

	

	//////////////////////////////////////////////////////////////////////////
	//the object : 
	CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
	if( !target ) return CKBR_OWNERERROR;


	//////////////////////////////////////////////////////////////////////////
	// the world :  
	pWorld *world=GetPMan()->getWorld(target->GetID());
	if (!world)
	{
		beh->ActivateOutput(bbOT_No);
		return 0;
	}

	NxScene *scene =  world->getScene();
	if (!scene)
	{
		beh->ActivateOutput(bbOT_No);
		return 0;
	}

	if( beh->IsInputActive(0) )
	{

		beh->ActivateInput(0,FALSE);

		//////////////////////////////////////////////////////////////////////////
		CK3dEntity *rayOriRef= (CK3dEntity *) beh->GetInputParameterObject(bbI_RayOriRef);
		CK3dEntity *rayDirRef= (CK3dEntity *) beh->GetInputParameterObject(bbI_RayDirRef);

		//ori : 
		VxVector ori  = GetInputParameterValue<VxVector>(beh,bbI_RayOri);
		VxVector oriOut  = ori;
		if (rayOriRef)
		{
			rayOriRef->Transform(&oriOut,&ori);
		}

		//dir : 
		VxVector dir = GetInputParameterValue<VxVector>(beh,bbI_RayDir);
		VxVector dirOut  = dir;
		if (rayDirRef)
		{
			VxVector dir,up,right;
			rayDirRef->GetOrientation(&dir,&up,&right);
			rayDirRef->TransformVector(&dirOut,&up);
		}

		float lenght = GetInputParameterValue<float>(beh,bbI_Length);
		int types  = GetInputParameterValue<int>(beh,bbI_ShapesType);



		VxRay ray;
		ray.m_Direction = dirOut;
		ray.m_Origin = oriOut;

		pRayCastReport &report = *world->getRaycastReport();
		report.setCurrentBehavior(beh->GetID());

		int groupsEnabled;
		DWORD groups = 0xffffffff;
		beh->GetLocalParameterValue(bbS_Groups,&groupsEnabled);
		if (groupsEnabled)
		{
			groups = GetInputParameterValue<int>(beh,bbI_Groups);
		}

		pGroupsMask *gmask = NULL;
		DWORD mask;
		beh->GetLocalParameterValue(bbS_Mask,&mask);
		if (mask)
		{
			CKParameter *maskP = beh->GetInputParameter(bbI_Mask)->GetRealSource();
			gmask->bits0  = GetValueFromParameterStruct<int>(maskP,0);
			gmask->bits1  = GetValueFromParameterStruct<int>(maskP,1);
			gmask->bits2  = GetValueFromParameterStruct<int>(maskP,2);
			gmask->bits3  = GetValueFromParameterStruct<int>(maskP,3);

		}

		int nbShapes = world->raycastAnyBounds(ray,(pShapesType)types,gmask,groups,lenght);

		

		if (nbShapes)
		{
			beh->ActivateOutput(bbOT_Yes);

		}else{
			beh->ActivateOutput(bbOT_No);
		}
	}

	return 0;
}

//************************************
// Method:    PWRayCastAnyBoundsCB
// FullName:  PWRayCastAnyBoundsCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PWRayCastAnyBoundsCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	
	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{



			DWORD groups;
			beh->GetLocalParameterValue(bbS_Groups,&groups);
			beh->EnableInputParameter(bbI_Groups,groups);

			DWORD mask;
			beh->GetLocalParameterValue(bbS_Mask,&mask);
			beh->EnableInputParameter(bbI_Mask,mask);


			
		
		}
		break;
	}
	return CKBR_OK;

}

