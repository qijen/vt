#include <StdAfx.h>
#include "pCommon.h"
#include "IParameter.h"

#include "pCallbackSignature.h"
#include "vtBBHelper.h"
#include "xDebugTools.h"
#include "pCallbackSignature.h"


CKObjectDeclaration	*FillBehaviorPBSetCallbackDecl();
CKERROR CreatePBSetCallbackProto(CKBehaviorPrototype **pproto);
int PBSetCallback(const CKBehaviorContext& behcontext);
CKERROR PBSetCallbackCB(const CKBehaviorContext& behcontext);

enum bInputs
{
	bbI_FetchSignature,
	bbI_ContactTypeMask,
	bbI_TriggerTypeMask,
	bbI_Threshold,
	bbI_Reserved,
	bbI_Reserved2,
	bbI_Contact,
	bbI_ContactModification,
	bbI_Trigger,
	bbI_WheelContact,
	bbI_RayCastHit,
	bbI_PreProcess,
	bbI_PostProcess,
	bbI_Copy,
	bbI_Delete
//	bbI_JointBreak,
};


void creatInputParameters(BBParameter pArray[],int size,CKBehavior *beh)
{
	//----------------------------------------------------------------
	//
	// remove all 
	//
	while(beh->GetInputParameterCount())
	{
		CKParameterIn* pi = beh->RemoveInputParameter(0);
		CKDestroyObject(pi);
	}
	for(int i  = 0 ; i < size ; i ++ )
	{
		BBParameter *par = &pArray[i];
		beh->CreateInputParameter( par->name.Str(), par->guid);
	}
}

void creatOutputParameters(BBParameter pArray[],int size,CKBehavior *beh)
{
	//----------------------------------------------------------------
	//
	// remove all 
	//
	while(beh->GetOutputParameterCount())
	{
		CKParameterOut* pi = beh->RemoveOutputParameter(0);
		CKDestroyObject(pi);
	}
	for(int i  = 0 ; i < size ; i ++ )
	{
		BBParameter *par = &pArray[i];
		beh->CreateOutputParameter( par->name.Str(), par->guid);
	}
}

#define BB_SSTART 6

BBParameter pInMap221[]  =
{
	BB_PIN(bbI_FetchSignature,CKPGUID_BOOL,"Create Parameters Only",""),
	BB_PIN(bbI_ContactTypeMask,VTF_COLLISIONS_EVENT_MASK,"Contact Event Mask",""),
	BB_PIN(bbI_TriggerTypeMask,VTF_TRIGGER,"Trigger Event Mask",""),
	BB_PIN(bbI_Threshold,CKPGUID_FLOAT,"Contact Threshold",""),
	BB_PIN(bbI_Reserved,CKPGUID_FLOAT,"Stub",""),
	BB_PIN(bbI_Reserved2,CKPGUID_FLOAT,"Stub",""),
	BB_SPIN(bbI_Contact,CKPGUID_BEHAVIOR,"Contact Notification",""),
	BB_SPIN(bbI_ContactModification,CKPGUID_BEHAVIOR,"Contact Modification",""),
	BB_SPIN(bbI_Trigger,CKPGUID_BEHAVIOR,"Trigger Event",""),
	BB_SPIN(bbI_WheelContact,CKPGUID_BEHAVIOR,"Wheel Contact Modify",""),
	BB_SPIN(bbI_RayCastHit,CKPGUID_BEHAVIOR,"Raycast Hit",""),
	BB_SPIN(bbI_PreProcess,CKPGUID_BEHAVIOR,"Pre Process",""),
	BB_SPIN(bbI_PostProcess,CKPGUID_BEHAVIOR,"Post Process",""),
	BB_SPIN(bbI_Copy,CKPGUID_BEHAVIOR,"Copy",""),
	BB_SPIN(bbI_Delete,CKPGUID_BEHAVIOR,"Delete","")
	//BB_SPIN(bbI_JointBreak,CKPGUID_BEHAVIOR,"Joint Break",""),

};

#define gPIMAP pInMap221


CKObjectDeclaration	*FillBehaviorPBSetCallbackDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("PBSetCallback");
	od->SetCategory("Physic/Body");
	od->SetDescription("Sets a callback script for the manager post process.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x1db50304,0x371a786b));
	od->SetAuthorGuid(VTCX_AUTHOR_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreatePBSetCallbackProto);
	od->SetCompatibleClassId(CKCID_3DENTITY);
	od->NeedManager(GUID_MODULE_MANAGER);

	return od;
}

CKERROR CreatePBSetCallbackProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("PBSetCallback");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	/*! \page PBSetCallback

	PBSetCallback is categorized in \ref Body

	<h3>Description</h3>
	Apply to a <A HREF="Documentation.chm::/beobjects/1_ck3dentity.html">3DEntity</A><br>
	Modifies various physical parameters.<br>

	<h3>Technical Information</h3>

	\image html PBSetCallback.png

	<SPAN CLASS="in">In: </SPAN>triggers the process
	<BR>
	<SPAN CLASS="out">Out: </SPAN>is activated when the process is completed.
	<BR>
	<BR>
	<SPAN CLASS="pin">Target: </SPAN>The 3D Entity associated to the rigid body or its sub shape.
	<BR>

	<SPAN CLASS="NiceCode">
	\include PBSetEx.cpp
	</SPAN>
	*/

	proto->SetBehaviorCallbackFct( PBSetCallbackCB );

	BB_EVALUATE_PINS(gPIMAP)
	BB_EVALUATE_SETTINGS(pInMap221)
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);

	proto->SetFunction(PBSetCallback);
	*pproto = proto;
	return CK_OK;
}

//************************************
// Method:    PBSetCallback
// FullName:  PBSetCallback
// Access:    public
// Returns:   int
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
int PBSetCallback(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKContext* ctx = behcontext.Context;
	PhysicManager *pm = GetPMan();
	pFactory *pf = pFactory::Instance();
	using namespace vtTools::BehaviorTools;

	BB_DECLARE_PIMAP;
	int fetchSignature  = GetInputParameterValue<int>(beh,BB_IP_INDEX(bbI_FetchSignature));
	if( beh->IsInputActive(0) )
	{
		beh->ActivateInput(0,FALSE);

		//////////////////////////////////////////////////////////////////////////
		//the object :
		CK3dEntity *target = (CK3dEntity *) beh->GetTarget();
		if( !target && !fetchSignature ) 
			bbErrorME("No Reference Object specified");
		pRigidBody *body = NULL;


		//////////////////////////////////////////////////////////////////////////
		// the world :  
		pWorld *world=GetPMan()->getWorldByBody(target); 
		if (!world && !fetchSignature)
			bbErrorME("No valid world object found");

		body = GetPMan()->getBody(target);
		if (!body && !fetchSignature)	
			bbErrorME("Object not physicalized");

		

		int contactMask  = GetInputParameterValue<int>(beh,BB_IP_INDEX(bbI_ContactTypeMask));
		int triggerMask  = GetInputParameterValue<int>(beh,BB_IP_INDEX(bbI_TriggerTypeMask));


		BBSParameterM(bbI_Contact,BB_SSTART);
		BBSParameterM(bbI_Trigger,BB_SSTART);
		BBSParameterM(bbI_ContactModification,BB_SSTART);
		BBSParameterM(bbI_RayCastHit,BB_SSTART);
		
		BBSParameterM(bbI_Copy,BB_SSTART);
		BBSParameterM(bbI_Delete,BB_SSTART);
		BBSParameterM(bbI_WheelContact,BB_SSTART);
		BBSParameterM(bbI_PreProcess,BB_SSTART);
		BBSParameterM(bbI_PostProcess,BB_SSTART);
		//BBSParameterM(bbI_JointBreak,BB_SSTART);


		//----------------------------------------------------------------
		//
		// update contact threshold
		//
		float threshold = GetInputParameterValue<float>(beh,BB_IP_INDEX(bbI_Threshold));
		if (body && !fetchSignature && sbbI_Contact && (body->getFlags() & BF_Moving ))
			body->setContactReportThreshold(threshold);
		if (sbbI_Delete || sbbI_Copy || sbbI_PostProcess || sbbI_PostProcess || sbbI_RayCastHit)
		{
			bbErrorME("Not Implemented yet");
		}
		XString errMessg;
        
		//----------------------------------------------------------------
		//
		// collision notify
		//
		if (sbbI_Contact)
		{


			CK_ID contactScript = GetInputParameterValue<int>(beh,BB_IP_INDEX(bbI_Contact));
			CKBehavior * cScript  = (CKBehavior*)GetPMan()->GetContext()->GetObject(contactScript);
			if (!cScript)
			{
				errMessg.Format("Callback for contact notification enabled but input script invalid");
				bbWarning(errMessg.Str());
			}else
			{
				if (fetchSignature){
					creatInputParameters(pInMapContactCallback,BB_PMAP_SIZE(pInMapContactCallback),cScript);
				}
				else
					body->setContactScript(contactScript,contactMask);
			}
		}

		//----------------------------------------------------------------
		//
		// wheel contact modify
		//
		if (sbbI_WheelContact)
		{
			CK_ID wheelContactModifyScript = GetInputParameterValue<int>(beh,BB_IP_INDEX(bbI_WheelContact));
			CKBehavior * cScript  = (CKBehavior*)GetPMan()->GetContext()->GetObject(wheelContactModifyScript);
			if (!cScript)
			{
				errMessg.Format("Callback for wheel contact modification enabled but input script invalid");
				bbWarning(errMessg.Str());
			}else
			{
				if (fetchSignature){
					creatInputParameters(pInMapWheelContactModifyCallback,BB_PMAP_SIZE(pInMapWheelContactModifyCallback),cScript);
					creatOutputParameters(pOutMapWheelContactModifyCallback,BB_PMAP_SIZE(pOutMapWheelContactModifyCallback),cScript);
				}
				else{

					pWheel2 *w = body->getWheel2(target);
					w->setWheelContactScript(wheelContactModifyScript);
					

				}
			}
		}

		//----------------------------------------------------------------
		//
		// collision notify
		//
		if (sbbI_Trigger)
		{


			CK_ID triggerScript = GetInputParameterValue<int>(beh,BB_IP_INDEX(bbI_Trigger));
			CKBehavior * cScript  = (CKBehavior*)GetPMan()->GetContext()->GetObject(triggerScript);
			if (!cScript)
			{
				errMessg.Format("Callback for trigger notification enabled but input script invalid");
				bbWarning(errMessg.Str());
			}else
			{
				if (fetchSignature){
					creatInputParameters(pInMapTriggerCallback,BB_PMAP_SIZE(pInMapTriggerCallback),cScript);
				}
				else
					body->setTriggerScript(triggerScript,triggerMask,target);
			}
		}


		//----------------------------------------------------------------
		//
		// collision notify
		//
		if (sbbI_ContactModification)
		{
			CK_ID contactModifyScript = GetInputParameterValue<int>(beh,BB_IP_INDEX(bbI_ContactModification));
			CKBehavior * cScript  = (CKBehavior*)GetPMan()->GetContext()->GetObject(contactModifyScript);
			if (!cScript)
			{
				errMessg.Format("Callback for contact modification enabled but input script invalid");
				bbWarning(errMessg.Str());
			}else
			{
				if (fetchSignature)
				{
					creatInputParameters(pInMapContactModifyCallback,BB_PMAP_SIZE(pInMapContactModifyCallback),cScript);
					creatOutputParameters(pOutMapContactModifyCallback,BB_PMAP_SIZE(pOutMapContactModifyCallback),cScript);
				}
				else
					body->setContactModificationScript(contactModifyScript);
			}
		}
		//----------------------------------------------------------------
		//
		// raycast 
		//
		if (sbbI_RayCastHit)
		{
			bbErrorME("Not Implemented yet");

			CK_ID raycastScript = GetInputParameterValue<int>(beh,BB_IP_INDEX(bbI_RayCastHit));
			CKBehavior * cScript  = (CKBehavior*)GetPMan()->GetContext()->GetObject(raycastScript);
			if (!cScript)
			{
				errMessg.Format("Callback for raycast hit reports enabled but input script invalid");
				bbWarning(errMessg.Str());
			}else
			{
				if (fetchSignature)
				{
					creatInputParameters(pInMapRaycastHitCallback,BB_PMAP_SIZE(pInMapRaycastHitCallback),cScript);
				}
				else
					body->setRayCastScript(raycastScript);
			}
		}



		//----------------------------------------------------------------
		//
		// raycast 
		//
		/*if (sbbI_JointBreak)
		{
			CK_ID jointBreakScript = GetInputParameterValue<int>(beh,BB_IP_INDEX(bbI_JointBreak));
			CKBehavior * cScript  = (CKBehavior*)GetPMan()->GetContext()->GetObject(jointBreakScript);
			if (!cScript)
			{
				errMessg.Format("Callback for joint breaks enabled but input script invalid");
				bbWarning(errMessg.Str());
			}else
			{
				if (fetchSignature)
				{
					creatInputParameters(pInMapJointBreakCallback,BB_PMAP_SIZE(pInMapJointBreakCallback),cScript);
				}
				else
					body->setJointBreakScript(jointBreakScript);
			}
		}
		*/
	}
	beh->ActivateOutput(0);
	return 0;
}

//************************************
// Method:    PBSetCallbackCB
// FullName:  PBSetCallbackCB
// Access:    public
// Returns:   CKERROR
// Qualifier:
// Parameter: const CKBehaviorContext& behcontext
//************************************
CKERROR PBSetCallbackCB(const CKBehaviorContext& behcontext)
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

