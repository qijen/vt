////////////////////////////////////////////////////////////////////////////////
//
// ARTPlusDetectionAndTransformation
// ---------------------------------
//
// Description:
// Buildingblock that detects a pattern (single marker) and transforms
// the related Object. If the pattern is not detected, the Object will
// be hidden.
//
// Input Parameter:
// IN_OBJECT			: The related virtools object
// IN_VIDEO_TEXTURE		: The image, in with ARToolKitPlus
//						  perform the detection
// IN_PATTERN_NUMBER	: The pattern number, which will
//						  be detected by the BB
// IN_PATTERN_WIDTH		: The width of the real pattern in mm
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
// OUT_POSITION			: Position vector of the detected pattern
// OUT_QUATERNION		: Rotation, given in quaternions, of the detected
//						  pattern
// OUT_MARKERID			: The ID of the Pattern, which has been detected.
//						  Should be the same number as IN_PATTERN_NUMBER
// OUT_DETECTED			: Flag which indicates that the pattern has been
//						  detected in the image or not
// OUT_TRANSFORM_MATRIX	: The complete transformation matrix.
//
//
// Version 1.0	: First Release
//
// Known Bugs	: None
//
// Copyright J�rg St�cklein <ozone@uni-paderborn.de>, University of Paderborn
////////////////////////////////////////////////////////////////////////////////

#include "CKAll.h"
#include <ARToolKitPlus/TrackerSingleMarker.h>

extern bool ARTPlusInitialized;
extern ARToolKitPlus::TrackerSingleMarker *tracker;

#define IN_OBJECT			0
#define IN_VIDEO_TEXTURE	1
#define IN_PATTERN_NUMBER	2
#define IN_PATTERN_WIDTH	3
#define IN_USE_BCH			4
#define IN_THIN_BORDER		5
#define IN_AUTO_THRESHOLD	6
#define IN_THRESHOLD		7

#define OUT_POSITION			0
#define OUT_QUATERNION			1
#define OUT_MARKERID			2
#define OUT_DETECTED			3
#define OUT_TRANSFORM_MATRIX	4

CKObjectDeclaration	*FillBehaviorARTPlusDetectionAndTransformationDecl();
CKERROR CreateARTPlusDetectionAndTransformationProto(CKBehaviorPrototype **);
int ARTPlusDetectionAndTransformation(const CKBehaviorContext& BehContext);

CKERROR ARTPlusDetectionAndTransformationCB(const CKBehaviorContext& BehContext);

CKObjectDeclaration	*FillBehaviorARTPlusDetectionAndTransformationDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Single Marker Detection and Transformation");

	od->SetType(CKDLL_BEHAVIORPROTOTYPE);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateARTPlusDetectionAndTransformationProto);
	od->SetDescription("Detects Single Marker and Calculates Transformation for given Videoframe");
	od->SetCategory("ARToolKitPlus");
	od->SetGuid(CKGUID(0x2f9e2472,0x639e208c));
	od->SetAuthorGuid(CKGUID(0x653d3e01,0x631c3314));
	od->SetAuthorName("J�rg St�cklein");
	od->SetCompatibleClassId(CKCID_BEOBJECT);

	return od;
}

CKERROR CreateARTPlusDetectionAndTransformationProto(CKBehaviorPrototype** pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Single Marker Detection and Transformation");
	if(!proto) 	return CKERR_OUTOFMEMORY;

//---	Inputs declaration
	proto->DeclareInput("In");

//---	Outputs declaration
	proto->DeclareOutput("Out");

//----- Input Parameters Declaration
	proto->DeclareInParameter("Object", CKPGUID_3DENTITY);
	proto->DeclareInParameter("Video Texture",CKPGUID_TEXTURE);
	proto->DeclareInParameter("Pattern Number",CKPGUID_INT, "0");
	proto->DeclareInParameter("Pattern Width (cm)", CKPGUID_FLOAT, "8.0");
	proto->DeclareInParameter("Use BCH Pattern", CKPGUID_BOOL, "TRUE");
	proto->DeclareInParameter("Use Thin Border", CKPGUID_BOOL, "TRUE");
	proto->DeclareInParameter("Enable Auto Threshold", CKPGUID_BOOL, "FALSE");
	proto->DeclareInParameter("Threshold", CKPGUID_INT, "150");

//---	Output Parameters Declaration
	proto->DeclareOutParameter("Position",CKPGUID_VECTOR);
	proto->DeclareOutParameter("Quaternion",CKPGUID_QUATERNION);
	proto->DeclareOutParameter("Marker ID",CKPGUID_INT, "-1");
	proto->DeclareOutParameter("Detected",CKPGUID_BOOL, FALSE);
	proto->DeclareOutParameter("Transformation Matrix",CKPGUID_MATRIX);

//----	Local Parameters Declaration

//----	Settings Declaration
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorCallbackFct(ARTPlusDetectionAndTransformationCB,CKCB_BEHAVIORBASE|CKCB_BEHAVIOREDITIONS|CKCB_BEHAVIORPAUSE|CKCB_BEHAVIORREADSTATE|CKCB_BEHAVIORRESET|CKCB_BEHAVIORACTIVATESCRIPT|CKCB_BEHAVIORDEACTIVATESCRIPT|CKCB_BEHAVIORRESUME|CKCB_BEHAVIORLOAD|CKCB_BEHAVIORNEWSCENE,NULL);
	proto->SetFunction(ARTPlusDetectionAndTransformation);

	*pproto = proto;
	return CK_OK;
}

int ARTPlusDetectionAndTransformation(const CKBehaviorContext& BehContext)
{
	CKBehavior* beh = BehContext.Behavior;
	CKTexture* texture = NULL;
	CKBYTE *pixel = NULL;
	CKBOOL detected = FALSE;
	int patternID = -1;
	int markerId = -1;
	float* buffer = NULL;
	float patternWidth = 8.0f; 
	CKBOOL useBCH = TRUE;
	CKBOOL thinBorder = TRUE;
	CKBOOL autoThreshold = FALSE;
	int threshold = 150;
	CK3dEntity* Object = NULL;
	VxQuaternion quat = VxQuaternion();
	VxVector pos = VxVector();
	VxVector scale = VxVector();

	float gl_para[4][4] = { {1.0f, 0.0f, 0.0f, 0.0f},
							{0.0f, 1.0f, 0.0f, 0.0f},
							{0.0f, 0.0f, 1.0f, 0.0f},
							{0.0f, 0.0f, 0.0f, 1.0f} };

	float gl_tmp[4][4] =  { {0.0f, 0.0f, 0.0f, 0.0f},
							{0.0f, 0.0f, 0.0f, 0.0f},
							{0.0f, 0.0f, 0.0f, 0.0f},
							{0.0f, 0.0f, 0.0f, 0.0f} };

	float koordSys[4][4] = { {1.0f, 0.0f, 0.0f, 0.0f},
							 {0.0f, 0.0f, 1.0f, 0.0f},
							 {0.0f, 1.0f, 0.0f, 0.0f},
							 {0.0f, 0.0f, 0.0f, 1.0f} };
 
	float koordSys2[4][4] = { {1.0f, 0.0f, 0.0f, 0.0f},
							  {0.0f, -1.0f, 0.0f, 0.0f},
							  {0.0f, 0.0f, 1.0f, 0.0f},
							  {0.0f, 0.0f, 0.0f, 1.0f} };

	beh->ActivateInput(0, FALSE);
	beh->ActivateOutput(0);

	if(ARTPlusInitialized == true)
	{
		// PatterID
		beh->GetInputParameterValue(IN_PATTERN_NUMBER, &patternID);

		// Get object
		Object = CK3dEntity::Cast(beh->GetInputParameterObject(IN_OBJECT));
		if(Object==NULL)  return CKBR_BEHAVIORERROR;

		// Get texture
		texture = CKTexture::Cast(beh->GetInputParameterObject(IN_VIDEO_TEXTURE));
		if(texture == NULL)
			return CKBR_BEHAVIORERROR;

		// Get pattern width
		beh->GetInputParameterValue(IN_PATTERN_WIDTH, &patternWidth);

		// define size of the marker
		tracker->setPatternWidth(patternWidth);

		// Get the type of pattern
		beh->GetInputParameterValue(IN_USE_BCH, &useBCH);

		// Get value for thin border
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

		// Get auto threshold
		beh->GetInputParameterValue(IN_AUTO_THRESHOLD, &autoThreshold);
		// Set auto thresholding in ARToolKitPlus
		tracker->activateAutoThreshold(autoThreshold?true:false);

		pixel = texture->LockSurfacePtr();
		if(pixel == NULL)
			return CKBR_BEHAVIORERROR;

		// here we go, just one call to find the camera pose
		markerId = tracker->calc(pixel, patternID);
		//markerId = tracker->calc(pixel);
		//float conf = (float)tracker->getConfidence();

		if(texture->Restore() != TRUE)
			return CKBR_BEHAVIORERROR;

		if(markerId==patternID)
		{
			// use the result of calc() to setup the OpenGL transformation
			buffer = (float *)tracker->getModelViewMatrix();
			detected = TRUE;

			for( int j = 0; j < 4; j++ )
			{
				for( int i = 0; i < 4; i++ )
				{
					gl_para[j][i] = buffer[j*4+i];
				}
			}

			for( int j = 0; j < 4; j++ )
			{
				for( int i = 0; i < 4; i++ )
				{
					gl_tmp[j][i] = 0.0;
					for(int k=0 ; k<4 ; k++)
					{
						gl_tmp[j][i] += koordSys[j][k]*gl_para[k][i];
					}
				}
			}

			for( int j = 0; j < 4; j++ )
			{
				for( int i = 0; i < 4; i++ )
				{
					gl_para[j][i] = 0.0;
					for(int k=0 ; k<4 ; k++)
					{
						gl_para[j][i] += gl_tmp[j][k]*koordSys2[k][i];
					}
				}
			}
		}

		// Set boolean
		beh->SetOutputParameterValue(OUT_DETECTED, &detected, 0);

		// Set marker id
		beh->SetOutputParameterValue(OUT_MARKERID, &markerId, 0);

		VxMatrix mat = VxMatrix(gl_para);
		Vx3DDecomposeMatrix(mat, quat, pos, scale);

		// Set position
		beh->SetOutputParameterValue(OUT_POSITION, &pos, 0);

		// Set quaternion
		beh->SetOutputParameterValue(OUT_QUATERNION, &quat, 0);

		// Set matrix
		beh->SetOutputParameterValue(OUT_TRANSFORM_MATRIX, &mat, 0);

		//Set visibility if the Object
		if(detected==TRUE)
		{
			// transform object if marker is detected
			Object->SetPosition(&pos, NULL, FALSE);
			Object->SetQuaternion(&quat, NULL, FALSE, FALSE);
			Object->Show(CKSHOW);
		}
		else
		{
			Object->Show(CKHIERARCHICALHIDE);
		}
	}
	return CKBR_OK;
}

CKERROR ARTPlusDetectionAndTransformationCB(const CKBehaviorContext& BehContext)
{
	switch (BehContext.CallbackMessage)
	{
		case CKM_BEHAVIORATTACH:			break;
		case CKM_BEHAVIORDELETE:			break;
		case CKM_BEHAVIORDETACH:			break;
		case CKM_BEHAVIORRESUME:			break;
		case CKM_BEHAVIORPAUSE:				break;
		case CKM_BEHAVIORRESET:				break;
		case CKM_BEHAVIORSETTINGSEDITED:	break;
		case CKM_BEHAVIOREDITED:			break;
		case CKM_BEHAVIORLOAD:				break;
		case CKM_BEHAVIORACTIVATESCRIPT:	break;
		case CKM_BEHAVIORDEACTIVATESCRIPT:	break;
		case CKM_BEHAVIORNEWSCENE:			break;
		case CKM_BEHAVIORREADSTATE:			break;

		default:  return CKBR_OK;
	}
	return CKBR_OK;
}
