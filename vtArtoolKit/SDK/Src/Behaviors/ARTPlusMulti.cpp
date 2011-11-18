////////////////////////////////////////////////////////////////////////////////
//
// ARTPlusMulti
// ------------
//
// Description:
// The ARTPlus building block initialize the ARToolKitPlus for multi marker
// detection. It should called once in your virtools project. Due to the fact,
// that the number of pattern which can be detected, is set on compile time,
// you can only detect 30 Pattern inside a video frame. If you want to detect
// more, than you have to change the number and recompile (See line 185).
//
// Input Parameter:
// IN_VIDEO_TEXTURE		: The image, in with ARToolKitPlus
//						  perform the detection
// IN_CAMERA_PARAM_FILE : The filename of the camera parameter file
//						  (look into ARToolKitPlus for description)
// IN_NEAR_CLIP_PLANE	: Near clip plane used by the camera
// IN_FAR_CLIP_PLANE	: Far clip plane used by the camera
//						  (look into ARToolKitPlus for description)
// IN_ENABLE_CAMERA_CORRECTION :
//						  Flag which indicates that the ARToolKitPlusManager
//						  should use the camera transformation matrix as
//						  projection matrix
//
// Output Parameter:
// OUT_CAMERA_TRANSFORM_MATRIX :
//						  The camera transformation matrix.
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
#define IN_VIDEO_TEXTURE			0
#define IN_CAMERA_PARAM_FILE		1
#define IN_MULTIMARKER_CONFIG_FILE	2
#define IN_NEAR_CLIP_PLANE			3
#define IN_FAR_CLIP_PLANE			4
#define IN_ENABLE_CAMERA_CORRECTION	5

// Output Parameter
#define OUT_CAMERA_TRANSFORM_MATRIX 0
#define OUT_ERROR_STRING 1

// Output Signals
#define OUTPUT_OK 0
#define OUTPUT_ERROR 1

#include "CKAll.h"

#include "ARToolKitLogger.h"
#include <ARToolKitPlus/TrackerMultiMarkerImpl.h>

CKObjectDeclaration	*FillBehaviorARTPlusMultiDecl();
CKERROR CreateARTPlusMultiProto(CKBehaviorPrototype **);

void cleanUp();
int ARTPlusMulti(const CKBehaviorContext& BehContext);
int ARTPlusMultiCallBack(const CKBehaviorContext& BehContext);

bool ARTPlusMultiInitialized = false;
bool ARTPlusMultiCorrectCamera = true;

extern void argConvGlpara( float para[4][4], float gl_para[16] );
extern void argConvGlparaTrans( float para[4][4], float gl_para[16] );

ARToolKitPlus::TrackerMultiMarker *multiTracker = NULL;
ARToolKitLogger multiLogger;

CKObjectDeclaration	*FillBehaviorARTPlusMultiDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Multi Marker Tracker");	
	
	od->SetType(CKDLL_BEHAVIORPROTOTYPE);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateARTPlusMultiProto);
	od->SetDescription("Multi Marker Tracker");
	od->SetCategory("ARToolKitPlus");
	od->SetGuid(CKGUID(0xa59f1,0x3d486f81));
	od->SetAuthorGuid(CKGUID(0x56495254,0x4f4f4c53));
	od->SetAuthorName("Jörg Stöcklein");
	od->SetCompatibleClassId(CKCID_BEOBJECT);

	return od;
}

CKERROR CreateARTPlusMultiProto(CKBehaviorPrototype** pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Multi Marker Tracker");
	if (!proto) {
		return CKERR_OUTOFMEMORY;
	}

//---     Inputs declaration
	proto->DeclareInput("In");

//---     Outputs declaration
	proto->DeclareOutput("Out");
	proto->DeclareOutput("Error");

//---     Input Parameters declaration
	proto->DeclareInParameter("Video Image", CKPGUID_TEXTURE);
	proto->DeclareInParameter("Camera-Param File", CKPGUID_STRING, "c:\\LogitechPro4000.dat");
	proto->DeclareInParameter("Multimarker Config File", CKPGUID_STRING, "c:\\markerboard_480-499.cfg");
	proto->DeclareInParameter("Near Clip Plane", CKPGUID_FLOAT, "1.0");
	proto->DeclareInParameter("Far Clip Plane", CKPGUID_FLOAT, "1000.0");
	proto->DeclareInParameter("Enable Camera Correction", CKPGUID_BOOL, "TRUE");

//---     Output Parameters declaration
	proto->DeclareOutParameter("CameraMatrix", CKPGUID_MATRIX);
	proto->DeclareOutParameter("Error", CKPGUID_STRING, "Success");

//----	Local Parameters Declaration

//----	Settings Declaration
	proto->SetBehaviorCallbackFct(ARTPlusMultiCallBack, CKCB_BEHAVIORATTACH|CKCB_BEHAVIORDETACH|CKCB_BEHAVIORDELETE|CKCB_BEHAVIOREDITED|CKCB_BEHAVIORSETTINGSEDITED|CKCB_BEHAVIORLOAD|CKCB_BEHAVIORPRESAVE|CKCB_BEHAVIORPOSTSAVE|CKCB_BEHAVIORRESUME|CKCB_BEHAVIORPAUSE|CKCB_BEHAVIORRESET|CKCB_BEHAVIORRESET|CKCB_BEHAVIORDEACTIVATESCRIPT|CKCB_BEHAVIORACTIVATESCRIPT|CKCB_BEHAVIORREADSTATE, NULL);
	proto->SetFunction(ARTPlusMulti);

	*pproto = proto;
	return CK_OK;
}

int ARTPlusMulti(const CKBehaviorContext& BehContext)
{
	CKBehavior* beh = BehContext.Behavior;
	float nearclip = 1.0f;
	float farclip = 1000.0f;
	float* buffer = NULL;
	float gl_matirx[4][4];

	beh->ActivateInput(0,FALSE);

	if(ARTPlusMultiInitialized == false)
	{
		CKTexture* texture = NULL;
		CKSTRING camparam = NULL;
		CKSTRING multiConfig = NULL;
		float nearclip = 1.0f;
		float farclip = 1000.0f;
		CKBOOL enableCamCorr = TRUE;

		// Texture (Important, request the Object not the Value or the Ptr!!!)
		texture = CKTexture::Cast(beh->GetInputParameterObject(IN_VIDEO_TEXTURE));

		if(texture == NULL)
		{
			beh->ActivateOutput(OUTPUT_ERROR);
			char string[] = "ERROR: No Texture Present";
			beh->SetOutputParameterValue(OUT_ERROR_STRING, string, strlen(string)+1);
			return CKERR_NOTINITIALIZED;
		}

		int width = texture->GetWidth();
		int height = texture->GetHeight();
		int windowwidth = BehContext.CurrentRenderContext->GetWidth();
		int windowheight = BehContext.CurrentRenderContext->GetHeight();

		// Get camera parameter
		camparam = (CKSTRING)(beh->GetInputParameterReadDataPtr(IN_CAMERA_PARAM_FILE));

		// Get multi marker config
		multiConfig = (CKSTRING)(beh->GetInputParameterReadDataPtr(IN_MULTIMARKER_CONFIG_FILE));

		// Fetch value for NearClip 
		beh->GetInputParameterValue(IN_NEAR_CLIP_PLANE, &nearclip);

		// Fetch value for FarClip
		beh->GetInputParameterValue(IN_FAR_CLIP_PLANE, &farclip);

		// create a tracker that does:
		//  - 12x12 sized marker images
		//  - samples at a maximum of 12x12
		//  - works with luminance (gray) images
		//  - can load a maximum of 1 pattern
		//  - can detect a maximum of 30 patterns in one image
		multiTracker = new ARToolKitPlus::TrackerMultiMarkerImpl<12,12,12,1,30>(width, height);

		// set a logger so we can output error messages
		multiTracker->setLogger(&multiLogger);
		multiTracker->setPixelFormat(ARToolKitPlus::PIXEL_FORMAT_ABGR);
		multiTracker->setLoadUndistLUT(true);

		multiTracker->setImageProcessingMode(ARToolKitPlus::IMAGE_FULL_RES);
		multiTracker->setUseDetectLite(false);

		if(!multiTracker->init(camparam, multiConfig, nearclip, farclip))
		{
			printf("ERROR: init() failed\n");
			delete multiTracker;
			multiTracker = NULL;

			beh->ActivateOutput(OUTPUT_ERROR);
			char string[] = "ERROR: init() failed";
			beh->SetOutputParameterValue(OUT_ERROR_STRING, string, strlen(string)+1);

			return CKERR_NOTINITIALIZED;
		}

		// Get value for camera correction enable 
		beh->GetInputParameterValue(IN_ENABLE_CAMERA_CORRECTION, &enableCamCorr);
		ARTPlusMultiCorrectCamera = enableCamCorr==TRUE?true:false;

		// let's use lookup-table undistortion for high-speed
		// note: LUT only works with images up to 1024x1024
		multiTracker->setUndistortionMode(ARToolKitPlus::UNDIST_LUT);

		// RPP is more robust than ARToolKit's standard pose estimator
		multiTracker->setPoseEstimator(ARToolKitPlus::POSE_ESTIMATOR_RPP);

		ARTPlusMultiInitialized = true;
	}
	beh->ActivateOutput(OUTPUT_OK);

	buffer = (float *)multiTracker->getProjectionMatrix();
	argConvGlpara(gl_matirx, buffer);
	VxMatrix mat = VxMatrix(gl_matirx);

	// set matrix
	beh->SetOutputParameterValue(OUT_CAMERA_TRANSFORM_MATRIX, &mat, 0);

	// set error string
	char string[] = "Success";
	beh->SetOutputParameterValue(OUT_ERROR_STRING, string, strlen(string)+1);

	return CKBR_OK;
}

int ARTPlusMultiCallBack(const CKBehaviorContext& BehContext)
{
	switch (BehContext.CallbackMessage)
	{
		case CKM_BEHAVIORATTACH:
			break;
		case CKM_BEHAVIORDETACH:
			break;
		case CKM_BEHAVIORDELETE:
		{
			cleanUp();
			break;
		}
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
		{
			cleanUp();
			break;
		}
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

void cleanUp()
{
	if(ARTPlusMultiInitialized)
	{
		multiTracker->cleanup();
		delete multiTracker;
		ARTPlusMultiInitialized = false;
	}
	multiTracker = NULL;
}
