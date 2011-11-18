#include <StdAfx.h>
#include "pCommon.h"

#include "pWorldCallbacks.h"



CKObjectDeclaration	*FillBehaviorPWRayCastAllShapeDecl();
CKERROR CreatePWRayCastAllShapeProto(CKBehaviorPrototype **pproto);
int PWRayCastAllShape(const CKBehaviorContext& behcontext);
CKERROR PWRayCastAllShapeCB(const CKBehaviorContext& behcontext);

CKObjectDeclaration	*FillBehaviorPWRayCastAllShapeDecl()
{

	CKObjectDeclaration *od = CreateCKObjectDeclaration("PWRayCastAllShapes");	
	od->SetCategory("Physic/Collision");
	od->SetDescription("Performs a ray cast test");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x7a2308ad,0x7a777c6e));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePWRayCastAllShapeProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);
	
	return od;
}

//************************************
// Method:    CreatePWRayCastAllShapeProto
// FullName:  CreatePWRayCastAllShapeProto
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: CKBehaviorPrototype **pproto
//************************************
enum bInput
{

	//bbI_WorldRef,
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
	bbS_Hints=2,
	bbS_Groups=3,
	bbS_Mask=4
};

enum bbO
{

	bbO_Shape,
	bbO_Impact,
	bbO_Normal,
	bbO_FaceIndex,
	bbO_Distance,
	bbO_UV,
	bbO_Material

};
enum bbOT
{
	bbOT_Yes,
	bbOT_No,
	bbOT_Finish,
	bbOT_Next,

};
CKERROR CreatePWRayCastAllShapeProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PWRayCastAllShapes");
	if(!proto) return CKERR_OUTOFMEMORY;

	/*! \page PWRayCastAllShapes  


	PWRayCastAllShapes is categorized in \ref Collision

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Performs a ray cast test. Outputs hit informations.<br>
	See <A HREF="PWRayCasts.cmo">PWRayCasts.cmo</A> for example.

	<h3>Technical Information</h3>

	\image html PWRayCastAllShapes.png

	<SPAN CLASS="in">In: </SPAN>Triggers the process.
	<BR>
	<SPAN CLASS="in">Next: </SPAN>Next Hit.
	<BR>
	
	
	<SPAN CLASS="out">Yes: </SPAN>Hit occured.
	<BR>
	<SPAN CLASS="out">No: </SPAN>No hits.
	<BR>
	<SPAN CLASS="out">Finish: </SPAN>Last hit.
	<BR>
	<SPAN CLASS="out">Next: </SPAN>Loop out.
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

	<SPAN CLASS="pout">Touched Body: </SPAN>The touched body.
	<BR>
	<SPAN CLASS="pout">Impact Position: </SPAN>Hit point in world space.
	<BR>
	<SPAN CLASS="pout">Face Normal: </SPAN>Normal of the hit.
	<BR>
	<SPAN CLASS="pout">Face Index: </SPAN>
	<BR>
	<SPAN CLASS="pout">Distance: </SPAN>Distance between ray start and hit.
	<BR>
	<SPAN CLASS="pout">UV: </SPAN> not used yet !
	<BR>
	<SPAN CLASS="pout">Material Index: </SPAN> Index of the internal physic material.
	<BR>
	
	
	
	<br>
	<br>

	Is utilizing #pWorld::raycastAllShapes() <br>
	
	*/
	
	proto->DeclareInput("In");
	proto->DeclareInput("Next");



	
	proto->DeclareOutput("Yes");
	proto->DeclareOutput("No");
	proto->DeclareOutput("Finish");
	proto->DeclareOutput("Next");


	proto->DeclareInParameter("Ray Origin",CKPGUID_VECTOR);
	proto->DeclareInParameter("Ray Origin Reference",CKPGUID_3DENTITY);
	proto->DeclareInParameter("Ray Direction",CKPGUID_VECTOR);
	proto->DeclareInParameter("Ray Direction Reference",CKPGUID_3DENTITY);
	proto->DeclareInParameter("Length",CKPGUID_FLOAT);
	proto->DeclareInParameter("Shapes Type",VTF_SHAPES_TYPE);

	proto->DeclareInParameter("Groups",CKPGUID_INT);
	proto->DeclareInParameter("Filter Mask",VTS_FILTER_GROUPS);


	proto->DeclareLocalParameter("result", CKPGUID_POINTER);
	proto->DeclareLocalParameter("index", CKPGUID_INT);

	proto->DeclareSetting("RayCast Hints",VTF_RAY_HINTS,"0");
	proto->DeclareSetting("Groups",CKPGUID_BOOL,"false");
	proto->DeclareSetting("Groups Mask",CKPGUID_BOOL,"false");


	proto->DeclareOutParameter("Touched Body",CKPGUID_3DENTITY);
	proto->DeclareOutParameter("Impact Position",CKPGUID_VECTOR);
	proto->DeclareOutParameter("Face Normal",CKPGUID_VECTOR);
	proto->DeclareOutParameter("Face Index",CKPGUID_INT);
	proto->DeclareOutParameter("Distance",CKPGUID_FLOAT);
	proto->DeclareOutParameter("UV",CKPGUID_2DVECTOR);
	proto->DeclareOutParameter("Material Index",CKPGUID_INT);

	//proto->DeclareSetting("Trigger on Enter",CKPGUID_BOOL,"FALSE");
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	
	proto->SetBehaviorCallbackFct( PWRayCastAllShapeCB );
	proto->SetFunction(PWRayCastAllShape);
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
// Method:    PWRayCastAllShape
// FullName:  PWRayCastAllShape
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PWRayCastAllShape(const CKBehaviorContext& behcontext)
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


		pRayCastHits *carray = NULL;
		beh->GetLocalParameterValue(0,&carray);
		if (carray)
		{
			carray->clear();
		}else
		{
			carray =  new pRayCastHits();
		}
		
		beh->SetLocalParameterValue(0,&carray);

		int hitIndex = 0;
		beh->SetLocalParameterValue(1,&hitIndex);



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

		

		pRaycastBit hints;
		beh->GetLocalParameterValue(bbS_Hints,&hints);


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

		int nbShapes = world->raycastAllShapes(ray,(pShapesType)types,groups,lenght,hints,gmask);

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
		pRayCastHits *carray = NULL;
		beh->GetLocalParameterValue(0,&carray);

		//////////////////////////////////////////////////////////////////////////
		if (carray)
		{
			if (carray->size())
			{
				NxRaycastHit *hit = carray->at(carray->size()-1);
				if (hit)
				{

					pRaycastBit hints;
					beh->GetLocalParameterValue(bbS_Hints,&hints);

					//////////////////////////////////////////////////////////////////////////
					//
					//
					if (hints & RCH_Shape)
					{
						NxShape *shape = hit->shape;
						if (shape)
						{

							pSubMeshInfo *sInfo  = static_cast<pSubMeshInfo*>(shape->userData);
							if (sInfo->entID)
							{
								CKObject *obj = (CKObject*)GetPMan()->m_Context->GetObject(sInfo->entID);
								if (obj)
								{
									beh->SetOutputParameterObject(bbO_Shape,obj);
								}
								
							}
							
						}
					}

					//////////////////////////////////////////////////////////////////////////
					//
					//
					if (hints & RCH_Distance)
					{
						beh->SetOutputParameterValue(bbO_Distance,&hit->distance);
					}
					if (hints & RCH_FaceIndex)
					{
						beh->SetOutputParameterValue(bbO_FaceIndex,&hit->internalFaceID);
					}
					
					if (hints & RCH_Impact)
					{
						VxVector vec = getFrom(hit->worldImpact);
						beh->SetOutputParameterValue(bbO_Impact,&vec);
					}
					
					if (hints & RCH_FaceNormal)
					{
						VxVector vec = getFrom(hit->worldNormal);
						beh->SetOutputParameterValue(bbO_Normal,&vec);
					}

					if (hints & RCH_Material)
					{
						int vec = hit->materialIndex;
						beh->SetOutputParameterValue(bbO_Material,&vec);

					}
					if (hints & RCH_FaceIndex)
					{
						int vec = hit->faceID;
						beh->SetOutputParameterValue(bbO_FaceIndex,&vec);
					}

					
					carray->pop_back();

					if (carray->size())
					{
						beh->ActivateOutput(bbOT_Next);
					}else
					{
						beh->ActivateOutput(bbOT_Finish);
					}


				}
			}
		}

	}

	return 0;
}

//************************************
// Method:    PWRayCastAllShapeCB
// FullName:  PWRayCastAllShapeCB
// Access:    public 
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PWRayCastAllShapeCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	switch(behcontext.CallbackMessage) {
	
	case CKM_BEHAVIORATTACH:
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{

			
			DWORD flags;
			beh->GetLocalParameterValue(bbS_Hints,&flags);

			beh->EnableOutputParameter(bbO_Shape,flags & RCH_Shape);
			beh->EnableOutputParameter(bbO_Impact,flags & RCH_Impact);
			beh->EnableOutputParameter(bbO_Normal,flags & RCH_Normal);
			beh->EnableOutputParameter(bbO_FaceIndex,flags & RCH_FaceIndex);
			beh->EnableOutputParameter(bbO_Distance,flags & RCH_Distance);
			beh->EnableOutputParameter(bbO_UV,flags & RCH_UV);
			beh->EnableOutputParameter(bbO_Material,flags & RCH_Material);


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

