////////////////////////////////////////////////////////////////////////////////
//
// ARToolKitPlusDetect
// -------------------
//
// Description:
// Buildingblock that detects pattern (single marker) in the given video
// image. If you want to use more than one pattern, that it is wise to use
// the ARToolKitPlusDetect-BB once and for each pattern the
// ARTPlusPatternTransformation-BB instead of using the
// ARTPlusDetectionAndTransformation-BB for each pattern. The cause is, that
// the ARTPlusDetectionAndTransformation-BB will run a complete detection
// on the video image eache time you call it.
//
// Input Parameter:
// IN_VIDEO_TEXTURE		: The image, in with ARToolKitPlus
//						  perform the detection
// IN_USE_BCH			: Flag which indicates to use BCH-pattern
//						  (look into ARToolKitPlus for description)
// IN_THIN_BORDER		: Flag which indicates to use pattern with
//						  a thin boarder)
//						  (look into ARToolKitPlus for description)
// IN_AUTO_THRESHOLD	: Flag which indicates to use the auto-threshold
//						  function from the ARToolKitPlus
//						  (look into ARToolKitPlus for description)
// IN_THRESHOLD			: The threshold value used, if IN_AUTO_THRESHOLD
//						  is set to false
//
// Output Parameter:
// OUT_ERROR_STRING		: String which describe the error, if one occurs. If
//						  there was no error the string will contain the word
//						  "Success" (without the marks)
//
// Version 1.0	: First Release
//
// Known Bugs	: None
//
// Copyright Jörg Stöcklein <ozone@uni-paderborn.de>, University of Paderborn
////////////////////////////////////////////////////////////////////////////////


// Input Parameter
#define IN_VIDEO_TEXTURE	0
#define IN_USE_BCH			1
#define IN_THIN_BORDER		2
#define IN_AUTO_THRESHOLD	3
#define IN_THRESHOLD		4

// Output Parameter
#define OUT_ERROR_STRING 0

// Output Signals
#define OUTPUT_OK 0
#define OUTPUT_ERROR 1

#include "CKAll.h"
#include <ARToolKitPlus/ar.h>
#include <ARToolKitPlus/TrackerSingleMarker.h>

CKObjectDeclaration	*FillBehaviorARToolKitPlusDetectDecl();
CKERROR CreateARToolKitPlusDetectProto(CKBehaviorPrototype **);

int ARToolKitPlusDetect(const CKBehaviorContext& BehContext);
int ARToolKitPlusDetectCallBack(const CKBehaviorContext& BehContext);

extern bool ARTPlusInitialized;

ARToolKitPlus::ARMarkerInfo* markerInfo = NULL;
int numMarkers = 0;

extern ARToolKitPlus::TrackerSingleMarker *tracker;

CKObjectDeclaration	*FillBehaviorARToolKitPlusDetectDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Single Marker Detection");	
	
	od->SetType(CKDLL_BEHAVIORPROTOTYPE);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateARToolKitPlusDetectProto);
	od->SetDescription("Single Marker Detection");
	od->SetCategory("ARToolKitPlus");
	od->SetGuid(CKGUID(0x1ff46552,0x6c31e58));
	od->SetAuthorGuid(CKGUID(0x56495254,0x4f4f4c53));
	od->SetAuthorName("Jörg Stöcklein");
	od->SetCompatibleClassId(CKCID_BEOBJECT);

	return od;
}

CKERROR CreateARToolKitPlusDetectProto(CKBehaviorPrototype** pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Single Marker Detection");
	if (!proto) {
		return CKERR_OUTOFMEMORY;
	}

//---     Inputs declaration
	proto->DeclareInput("In");

//---     Outputs declaration
	proto->DeclareOutput("Out");
	proto->DeclareOutput("Error");

//---     Input Parameters declaration
	proto->DeclareInParameter("VideoImage", CKPGUID_TEXTURE);
	proto->DeclareInParameter("Use BCH Pattern", CKPGUID_BOOL, "TRUE");
	proto->DeclareInParameter("Use Thin Border", CKPGUID_BOOL, "TRUE");
	proto->DeclareInParameter("Enable Auto Threshold", CKPGUID_BOOL, "FALSE");
	proto->DeclareInParameter("Threshold", CKPGUID_INT, "150");

//---     Output Parameters declaration
	proto->DeclareOutParameter("Error", CKPGUID_STRING, "Success");

//----	Local Parameters Declaration

//----	Settings Declaration

	proto->SetBehaviorCallbackFct(ARToolKitPlusDetectCallBack, CKCB_BEHAVIORATTACH|CKCB_BEHAVIORDETACH|CKCB_BEHAVIORDELETE|CKCB_BEHAVIOREDITED|CKCB_BEHAVIORSETTINGSEDITED|CKCB_BEHAVIORLOAD|CKCB_BEHAVIORPRESAVE|CKCB_BEHAVIORPOSTSAVE|CKCB_BEHAVIORRESUME|CKCB_BEHAVIORPAUSE|CKCB_BEHAVIORRESET|CKCB_BEHAVIORRESET|CKCB_BEHAVIORDEACTIVATESCRIPT|CKCB_BEHAVIORACTIVATESCRIPT|CKCB_BEHAVIORREADSTATE, NULL);
	proto->SetFunction(ARToolKitPlusDetect);

	*pproto = proto;
	return CK_OK;
}

int ARToolKitPlusDetect(const CKBehaviorContext& BehContext)
{
	CKBehavior* beh = BehContext.Behavior;
	beh->ActivateInput(0,FALSE);

	if(ARTPlusInitialized == true)
	{
		CKTexture* texture = NULL;
		CKBYTE *pixel = NULL;
		CKBOOL autoThreshold = FALSE;
		int threshold = 150;
		CKBOOL useBCH = TRUE;
		CKBOOL thinBorder = TRUE;

		// Texture (Wichtig, Object holen, nicht Value oder Ptr!!!)
		texture = CKTexture::Cast(beh->GetInputParameterObject(IN_VIDEO_TEXTURE));

		if(texture == NULL)
		{
			beh->ActivateOutput(OUTPUT_ERROR);
			char string[] = "ERROR: No Texture Present";
			beh->SetOutputParameterValue(OUT_ERROR_STRING, string, strlen(string)+1);
			return CKERR_NOTINITIALIZED;
		}

		pixel = texture->LockSurfacePtr();
		if(pixel == NULL)
		{
			beh->ActivateOutput(OUTPUT_ERROR);
			char string[] = "ERROR: Can't lock texture surface";
			beh->SetOutputParameterValue(OUT_ERROR_STRING, string, strlen(string)+1);
			return CKBR_BEHAVIORERROR;
		}

		// Auto Threshold holen
		beh->GetInputParameterValue(IN_AUTO_THRESHOLD, &autoThreshold);
		// set Auto Thresholding
		tracker->activateAutoThreshold(autoThreshold?true:false);

		// Threshold holen
		beh->GetInputParameterValue(IN_THRESHOLD, &threshold);
		// set a threshold. alternatively we could also activate automatic thresholding
		tracker->setThreshold(threshold);

		// Pattern Typ holen
		beh->GetInputParameterValue(IN_USE_BCH, &useBCH);

		// Thin Border holen
		beh->GetInputParameterValue(IN_THIN_BORDER, &thinBorder);

		if(useBCH)
		{
			// the marker in the BCH test image has a thin border...
			tracker->setBorderWidth(0.125f);
		}
		else
		{
			tracker->setBorderWidth(thinBorder ? 0.125f : 0.250f);
		}

		// switch to simple ID based markers
		// use the tool in tools/IdPatGen to generate markers
		tracker->setMarkerMode(useBCH ? ARToolKitPlus::MARKER_ID_BCH : ARToolKitPlus::MARKER_ID_SIMPLE);

		// here we go, just one call to find the camera pose
		numMarkers = 0;
		markerInfo = NULL;
		tracker->calc(pixel, -1, true, &markerInfo, &numMarkers);

		if(texture->Restore() != TRUE)
		{
			beh->ActivateOutput(OUTPUT_ERROR);
			char string[] = "ERROR: Can't restore texture surface";
			beh->SetOutputParameterValue(OUT_ERROR_STRING, string, strlen(string)+1);
			return CKBR_BEHAVIORERROR;
		}
	}
	beh->ActivateOutput(OUTPUT_OK);

	char string[] = "Success";
	beh->SetOutputParameterValue(OUT_ERROR_STRING, string, strlen(string)+1);

	return CKBR_OK;
}

int ARToolKitPlusDetectCallBack(const CKBehaviorContext& BehContext)
{
	switch (BehContext.CallbackMessage)
	{
		case CKM_BEHAVIORATTACH:
			break;
		case CKM_BEHAVIORDETACH:
			break;
		case CKM_BEHAVIORDELETE:
			break;
		case CKM_BEHAVIOREDITED:
			break;
		case CKM_BEHAVIORSETTINGSEDITED:
			break;
		case CKM_BEHAVIORLOAD:
			break;
		case CKM_BEHAVIORPRESAVE:
			break;
		case CKM_BEHAVIORPOSTSAVE:
			break;
		case CKM_BEHAVIORRESUME:
			break;
		case CKM_BEHAVIORPAUSE:
			break;
		case CKM_BEHAVIORRESET:
			break;
		case CKM_BEHAVIORNEWSCENE:
			break;
		case CKM_BEHAVIORDEACTIVATESCRIPT:
			break;
		case CKM_BEHAVIORACTIVATESCRIPT:
			break;
		case CKM_BEHAVIORREADSTATE:
			break;
	}
	return CKBR_OK;
}
