////////////////////////////////////////////////////////////////////////////////
//
// ARTPlus
// -------
//
// Description:
// The ARTPlus building block initialize the ARToolKitPlus for Single marker
// detection. It should called once in your virtools project. Due to the fact,
// that the number of pattern which can be detected, is set on compile time,
// you can only detect 20 Pattern inside a video frame. If you want to detect
// more, than you have to change the number and recompile (See line 183).
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
#define IN_VIDEO_TEXTURE 0
#define IN_CAMERA_PARAM_FILE 1
#define IN_NEAR_CLIP_PLANE 2
#define IN_FAR_CLIP_PLANE 3
#define IN_ENABLE_CAMERA_CORRECTION 4

// Output Parameter
#define OUT_CAMERA_TRANSFORM_MATRIX 0
#define OUT_ERROR_STRING 1

// Local Parameters
#define LOCAL_TRACKER 0

// Output Signals
#define OUTPUT_OK 0
#define OUTPUT_ERROR 1

#include "CKAll.h"

#include "ARToolKitLogger.h"
#include <ARToolKitPlus/TrackerSingleMarkerImpl.h>

CKObjectDeclaration	*FillBehaviorARTPlusDecl();
CKERROR CreateARTPlusProto(CKBehaviorPrototype **);

void cleanUp();
int ARTPlus(const CKBehaviorContext& BehContext);
int ARTPlusCallBack(const CKBehaviorContext& BehContext);

bool ARTPlusInitialized = false;
bool ARTPlusCorrectCamera = true;

void argConvGlpara( float para[4][4], float gl_para[16] );
void argConvGlparaTrans( float para[4][4], float gl_para[16] );

ARToolKitPlus::TrackerSingleMarker *tracker = NULL;
ARToolKitLogger logger;

CKObjectDeclaration	*FillBehaviorARTPlusDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Single Marker Tracker");	
	
	od->SetType(CKDLL_BEHAVIORPROTOTYPE);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateARTPlusProto);
	od->SetDescription("Single Marker Tracker");
	od->SetCategory("ARToolKitPlus");
	od->SetGuid(CKGUID(0x6d8452a7, 0x94937df2));
	od->SetAuthorGuid(CKGUID(0x56495254,0x4f4f4c53));
	od->SetAuthorName("Jörg Stöcklein");
	od->SetCompatibleClassId(CKCID_BEOBJECT);

	return od;
}

CKERROR CreateARTPlusProto(CKBehaviorPrototype** pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Single Marker Tracker");
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
	proto->DeclareInParameter("Near Clip Plane", CKPGUID_FLOAT, "1.0");
	proto->DeclareInParameter("Far Clip Plane", CKPGUID_FLOAT, "1000.0");
	proto->DeclareInParameter("Enable Camera Correction", CKPGUID_BOOL, "TRUE");

//---     Output Parameters declaration
	proto->DeclareOutParameter("CameraMatrix", CKPGUID_MATRIX);
	proto->DeclareOutParameter("Error", CKPGUID_STRING, "Success");


//----	Local Parameters Declaration
	//proto->DeclareLocalParameter("ARTracker", CKPGUID_POINTER, NULL, 4);

//----	Settings Declaration

	proto->SetBehaviorCallbackFct(ARTPlusCallBack, CKCB_BEHAVIORATTACH|CKCB_BEHAVIORDETACH|CKCB_BEHAVIORDELETE|CKCB_BEHAVIOREDITED|CKCB_BEHAVIORSETTINGSEDITED|CKCB_BEHAVIORLOAD|CKCB_BEHAVIORPRESAVE|CKCB_BEHAVIORPOSTSAVE|CKCB_BEHAVIORRESUME|CKCB_BEHAVIORPAUSE|CKCB_BEHAVIORRESET|CKCB_BEHAVIORRESET|CKCB_BEHAVIORDEACTIVATESCRIPT|CKCB_BEHAVIORACTIVATESCRIPT|CKCB_BEHAVIORREADSTATE, NULL);
	proto->SetFunction(ARTPlus);

	*pproto = proto;
	return CK_OK;
}

int ARTPlus(const CKBehaviorContext& BehContext)
{
	CKBehavior* beh = BehContext.Behavior;
	float nearclip = 1.0f;
	float farclip = 1000.0f;
	float* buffer = NULL;
	float gl_matirx[4][4];

	beh->ActivateInput(0,FALSE);

	if(ARTPlusInitialized == false)
	{
		CKTexture* texture = NULL;
		CKSTRING camparam = NULL;
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

		// Patternname
		camparam = (CKSTRING)(beh->GetInputParameterReadDataPtr(IN_CAMERA_PARAM_FILE));

		// Fetch value for NearClip 
		beh->GetInputParameterValue(IN_NEAR_CLIP_PLANE, &nearclip);

		// Fetch value for FarClip
		beh->GetInputParameterValue(IN_FAR_CLIP_PLANE, &farclip);

		// create a tracker that does:
		//  - 12x12 sized marker images
		//  - samples at a maximum of 12x12
		//  - works with luminance (gray) images
		//  - can load a maximum of 10 pattern
		//  - can detect a maximum of 12 patterns in one image
		tracker = new ARToolKitPlus::TrackerSingleMarkerImpl<12,12,12,20,20>(width, height);

		// set a logger so we can output error messages
		//
		tracker->setLogger(&logger);
		tracker->setPixelFormat(ARToolKitPlus::PIXEL_FORMAT_ABGR);
		tracker->setLoadUndistLUT(true);

		tracker->setImageProcessingMode(ARToolKitPlus::IMAGE_FULL_RES);

		// load a camera file. two types of camera files are supported:
		//  - Std. ARToolKit
		//  - MATLAB Camera Calibration Toolbox
		if(!tracker->init(camparam, nearclip, farclip))            // load std. ARToolKit camera file
		//if(!tracker->init("data/PGR_M12x0.5_2.5mm.cal", 1.0f, 1000.0f))        // load MATLAB file
		{
			printf("ERROR: init() failed\n");
			delete tracker;
			tracker = NULL;

			beh->ActivateOutput(OUTPUT_ERROR);
			char string[] = "ERROR: init() failed";
			beh->SetOutputParameterValue(OUT_ERROR_STRING, string, strlen(string)+1);

			return CKERR_NOTINITIALIZED;
		}

		// Fetch value for Camera Correction
		beh->GetInputParameterValue(IN_ENABLE_CAMERA_CORRECTION, &enableCamCorr);
		ARTPlusCorrectCamera = enableCamCorr==TRUE?true:false;

		// let's use lookup-table undistortion for high-speed
		// note: LUT only works with images up to 1024x1024
		tracker->setUndistortionMode(ARToolKitPlus::UNDIST_LUT);

		// RPP is more robust than ARToolKit's standard pose estimator
		tracker->setPoseEstimator(ARToolKitPlus::POSE_ESTIMATOR_RPP);

//		beh->SetLocalParameterValue(LOCAL_TRACKER, (void *)tracker, 4);

		ARTPlusInitialized = true;
	}
	beh->ActivateOutput(OUTPUT_OK);

//	In development
//	ARToolKitPlus::TrackerSingleMarker *localTracker = (ARToolKitPlus::TrackerSingleMarker *)beh->GetLocalParameter(LOCAL_TRACKER);

	buffer = (float *)tracker->getProjectionMatrix();
	argConvGlpara(gl_matirx, buffer);
	VxMatrix mat = VxMatrix(gl_matirx);

	// Set matrix
	beh->SetOutputParameterValue(OUT_CAMERA_TRANSFORM_MATRIX, &mat, 0);

	// Set error string
	char string[] = "Success";
	beh->SetOutputParameterValue(OUT_ERROR_STRING, string, strlen(string)+1);

	return CKBR_OK;
}

int ARTPlusCallBack(const CKBehaviorContext& BehContext)
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
	if(ARTPlusInitialized)
	{
		tracker->cleanup();
		delete tracker;
		ARTPlusInitialized = false;
	}
	tracker = NULL;
}

void argConvGlpara( float para[4][4], float gl_para[16] )
{
    int     i, j;

    for( j = 0; j < 4; j++ )
	{
        for( i = 0; i < 4; i++ )
		{
            para[j][i] = (gl_para[i*4+j]);
        }
    }
}

void argConvGlparaTrans( float para[4][4], float gl_para[16] )
{
    int     i, j;

    for( j = 0; j < 4; j++ )
	{
        for( i = 0; i < 4; i++ )
		{
            para[i][j] = (gl_para[i*4+j]);
        }
    }
}

