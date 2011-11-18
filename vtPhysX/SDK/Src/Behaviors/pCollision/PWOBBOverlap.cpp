#include <StdAfx.h>
#include "pCommon.h"


CKObjectDeclaration	*FillBehaviorPWOBBOverlapDecl();
CKERROR CreatePWOBBOverlapProto(CKBehaviorPrototype **pproto);
int PWOBBOverlap(const CKBehaviorContext& behcontext);
CKERROR PWOBBOverlapCB(const CKBehaviorContext& behcontext);

CKObjectDeclaration	*FillBehaviorPWOBBOverlapDecl()
{

	CKObjectDeclaration *od = CreateCKObjectDeclaration("PWOBBOverlap");
	od->SetCategory("Physic/Collision");
	od->SetDescription("Performs an overlap test against masked shape groups.Outputs an object only!");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x16a46d79,0xa951a4a));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePWOBBOverlapProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePWOBBOverlapProto
// FullName:  CreatePWOBBOverlapProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
enum bInput
{

	//bbI_WorldRef,
	bbI_SIZE,
	bbI_Center,
	bbI_Ref,
	bbI_ShapesType,
	bbI_Accurate,
	bbI_Groups,
	bbI_Mask,


};

enum bbS
{
	bbS_Result=0,
	bbS_Index,
	bbS_Size,
	bbS_Groups=3,
	bbS_Mask=4
};

enum bbOT
{
	bbOT_Yes,
	bbOT_No,
	bbOT_Finish,
	bbOT_Next,
};
CKERROR CreatePWOBBOverlapProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PWOBBOverlap");
	if(!proto) return CKERR_OUTOFMEMORY;

	/*! \page PWOBBOverlap  


	PWOBBOverlap is categorized in \ref Collision

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Performs an overlap test against masked shape groups.<br>
	See <A HREF="PWOverlaps.cmo">PWOverlaps.cmo</A> for example.

	<h3>Technical Information</h3>

	\image html PWOBBOverlap.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="in">Next: </SPAN>Iterate through next hit.
	<BR>
	<BR>

	<SPAN CLASS="out">Yes: </SPAN>Hit occured.
	<BR>
	<SPAN CLASS="out">No: </SPAN>No hits.
	<BR>
	<SPAN CLASS="out">Finish: </SPAN>Last hit.
	<BR>
	<SPAN CLASS="out">Next: </SPAN>Loop out.
	<BR>
	<BR>


	<SPAN CLASS="pin">Target: </SPAN>World Reference. pDefaultWorld!
	<BR>
	<SPAN CLASS="pin">Size: </SPAN>The box dimension.
	<BR>
	<SPAN CLASS="pin">Center: </SPAN>The box center in world space.If shape reference is given, then its transforming this value in its local space.
	<BR>
	<SPAN CLASS="pin">Shapes Types: </SPAN>Adds static and/or dynamic shapes to the test.
	<BR>
	<SPAN CLASS="pin">Accurate: </SPAN>True to test the sphere against the actual shapes, false to test against the AABBs only.
	<BR>
	<SPAN CLASS="pin">Groups: </SPAN>Includes specific groups to the test. 
	<BR>
	<SPAN CLASS="pin">Groups Mask: </SPAN>Alternative mask used to filter shapes. See #pRigidBody::setGroupsMask
	<BR>


	<br>
	<h3>Note</h3><br>


	<br>
	<br>

	Is utilizing #pWorld::overlapOBBShapes()<br>

	*/
	
	proto->DeclareInput("In");
	proto->DeclareInput("Next");



	
	proto->DeclareOutput("Yes");
	proto->DeclareOutput("No");
	proto->DeclareOutput("Finish");
	proto->DeclareOutput("Next");

	proto->DeclareInParameter("Size",CKPGUID_VECTOR);
	proto->DeclareInParameter("Center",CKPGUID_VECTOR);
	proto->DeclareInParameter("Shape Reference",CKPGUID_3DENTITY);
	proto->DeclareInParameter("Shapes Type",VTF_SHAPES_TYPE);
	proto->DeclareInParameter("Accurate",CKPGUID_BOOL);
	proto->DeclareInParameter("Groups",CKPGUID_INT);
	proto->DeclareInParameter("Filter Mask",VTS_FILTER_GROUPS);


	proto->DeclareLocalParameter("result", CKPGUID_GROUP);
	proto->DeclareLocalParameter("index", CKPGUID_INT);
	proto->DeclareLocalParameter("size", CKPGUID_INT);

	proto->DeclareSetting("Groups",CKPGUID_BOOL,"false");
	proto->DeclareSetting("Groups Mask",CKPGUID_BOOL,"false");


	proto->DeclareOutParameter("Touched Body",CKPGUID_3DENTITY);
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetBehaviorCallbackFct( PWOBBOverlapCB );
	proto->SetFunction(PWOBBOverlap);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PWOBBOverlap
// FullName:  PWOBBOverlap
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PWOBBOverlap(const CKBehaviorContext& behcontext)
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


		CKGroup *carray = (CKGroup*)beh->GetLocalParameterObject(bbS_Result);
		if (carray)
		{
			//carray->clear();
			carray->Clear();
		}else
		{
			
			CK_OBJECTCREATION_OPTIONS creaoptions = (CK_OBJECTCREATION_OPTIONS)(CK_OBJECTCREATION_NONAMECHECK|CK_OBJECTCREATION_DYNAMIC);
			carray = (CKGroup*)ctx()->CreateObject(CKCID_GROUP,"asdasd",creaoptions);

		}
		
		beh->SetLocalParameterObject(0,carray);

		int hitIndex = 0;
		beh->SetLocalParameterValue(bbS_Index,&hitIndex);
		int hitSize = 0;
		beh->SetLocalParameterValue(bbS_Size,&hitSize);




		//////////////////////////////////////////////////////////////////////////
	
		int types  = GetInputParameterValue<int>(beh,bbI_ShapesType);
		int accurate  = GetInputParameterValue<int>(beh,bbI_Accurate);



		DWORD groupsEnabled;
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

		VxVector size = GetInputParameterValue<VxVector>(beh,bbI_SIZE);
		VxVector center = GetInputParameterValue<VxVector>(beh,bbI_Center);

		VxBbox box;
		box.SetCenter(center,size/2);
		box.SetDimension(center,size);



		CK3dEntity *shape = (CK3dEntity*)beh->GetInputParameterObject(bbI_Ref);

		int nbShapes = world->overlapOBBShapes(box,shape,(pShapesType)types,carray,groups,gmask,accurate);
		if (nbShapes)
		{
			beh->ActivateOutput(bbOT_Yes);
			beh->ActivateInput(1,TRUE);

		}else{
			beh->ActivateOutput(bbOT_No);
		}
	}

	if( beh->IsInputActive(1) )
	{

		beh->ActivateInput(1,FALSE);
		CKGroup *carray = (CKGroup*)beh->GetLocalParameterObject(bbS_Result);
		
		//////////////////////////////////////////////////////////////////////////
		if (carray)
		{
			if (carray->GetObjectCount())
			{
				CKBeObject *hit = carray->GetObject(carray->GetObjectCount()-1);
				if (hit)
				{


					beh->SetOutputParameterObject(0,hit);
					carray->RemoveObject(hit);
					
					
					

					if (carray->GetObjectCount())
					{
						beh->ActivateOutput(bbOT_Next);
					}else
					{
						beh->ActivateOutput(bbOT_Finish);
						CKDestroyObject(carray);
					}

				}
			}else{
				beh->ActivateOutput(bbOT_Finish);
				CKDestroyObject(carray);
			}
		}else
		{
			beh->ActivateOutput(bbOT_Finish);
			CKDestroyObject(carray);
		}

	}

	return 0;
}

//************************************
// Method:    PWOBBOverlapCB
// FullName:  PWOBBOverlapCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PWOBBOverlapCB(const CKBehaviorContext& behcontext)
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

