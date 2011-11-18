////////////////////////////////////////////////////////////////////////////////
//
// ARTPlusPatternTransformation
// ----------------------------
//
// Description:
// Buildingblock that transforms a given object by the detected transformation
// matrix of the corrosponding pattern (single marker). If the pattern is not
// detected, the Object (and its hierachy) will be set as hidden.
//
// Input Parameter:
// IN_OBJECT			: The related virtools object
// IN_PATTERN_NUMBER	: The pattern number, which will
//						  be detected by the BB
// IN_PATTERN_WIDTH		: The width of the real pattern in mm
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
// Version 1.0	: First Release
//
// Known Bugs	: None
//
// Copyright Jörg Stöcklein <ozone@uni-paderborn.de>, University of Paderborn
////////////////////////////////////////////////////////////////////////////////

#include "CKAll.h"
#include <ARToolKitPlus/TrackerSingleMarker.h>

extern bool ARTPlusInitialized;
extern ARToolKitPlus::TrackerSingleMarker* tracker;
extern ARToolKitPlus::ARMarkerInfo* markerInfo;
extern int numMarkers;

#define IN_OBJECT			0
#define IN_PATTERN_NUMBER	1
#define IN_PATTERN_WIDTH	2

#define OUT_POSITION			0
#define OUT_QUATERNION			1
#define OUT_MARKERID			2
#define OUT_DETECTED			3
#define OUT_TRANSFORM_MATRIX	4

CKObjectDeclaration	*FillBehaviorARTPlusPatternTransformationDecl();
CKERROR CreateARTPlusPatternTransformationProto(CKBehaviorPrototype **);
int ARTPlusPatternTransformation(const CKBehaviorContext& BehContext);

CKERROR ARTPlusPatternTransformationCB(const CKBehaviorContext& BehContext);

int findMarker(int patternID);

CKObjectDeclaration	*FillBehaviorARTPlusPatternTransformationDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Single Marker Transformation");

	od->SetType(CKDLL_BEHAVIORPROTOTYPE);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateARTPlusPatternTransformationProto);
	od->SetDescription("Calculates Transformation for given Videoframe");
	od->SetCategory("ARToolKitPlus");
	od->SetGuid(CKGUID(0x6b733302,0x12d23c1f));
	od->SetAuthorGuid(CKGUID(0x653d3e01,0x631c3314));
	od->SetAuthorName("Jörg Stöcklein");
	od->SetCompatibleClassId(CKCID_BEOBJECT);

	return od;
}

CKERROR CreateARTPlusPatternTransformationProto(CKBehaviorPrototype** pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Single Marker Transformation");
	if(!proto) 	return CKERR_OUTOFMEMORY;

//---	Inputs declaration
	proto->DeclareInput("In");

//---	Outputs declaration
	proto->DeclareOutput("Out");

//----- Input Parameters Declaration
	proto->DeclareInParameter("Object", CKPGUID_3DENTITY);
	proto->DeclareInParameter("Pattern Number",CKPGUID_INT, "0");
	proto->DeclareInParameter("Pattern Width (cm)", CKPGUID_FLOAT, "8.0");

//---	Output Parameters Declaration
	proto->DeclareOutParameter("Position",CKPGUID_VECTOR);
	proto->DeclareOutParameter("Quaternion",CKPGUID_QUATERNION);
	proto->DeclareOutParameter("Marker ID",CKPGUID_INT, "-1");
	proto->DeclareOutParameter("Detected",CKPGUID_BOOL, FALSE);
	proto->DeclareOutParameter("Transformation Matrix",CKPGUID_MATRIX);

//----	Local Parameters Declaration

//----	Settings Declaration

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorCallbackFct(ARTPlusPatternTransformationCB,CKCB_BEHAVIORBASE|CKCB_BEHAVIOREDITIONS|CKCB_BEHAVIORPAUSE|CKCB_BEHAVIORREADSTATE|CKCB_BEHAVIORRESET|CKCB_BEHAVIORACTIVATESCRIPT|CKCB_BEHAVIORDEACTIVATESCRIPT|CKCB_BEHAVIORRESUME|CKCB_BEHAVIORLOAD|CKCB_BEHAVIORNEWSCENE,NULL);
	proto->SetFunction(ARTPlusPatternTransformation);

	*pproto = proto;
	return CK_OK;
}

int ARTPlusPatternTransformation(const CKBehaviorContext& BehContext)
{
	CKBehavior* beh = BehContext.Behavior;
	CKBOOL detected = FALSE;
	int patternID = -1;
	int markerId = -1;
	float buffer[16];
	float patternWidth = 8.0f;
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
		// get object
		Object = CK3dEntity::Cast(beh->GetInputParameterObject(IN_OBJECT));
		if(Object==NULL)  return CKBR_BEHAVIORERROR;

		if(markerInfo!=NULL && numMarkers>0)
		{
			// Get pattern id
			beh->GetInputParameterValue(IN_PATTERN_NUMBER, &patternID);

			// Get pattern width
			beh->GetInputParameterValue(IN_PATTERN_WIDTH, &patternWidth);

			// define size of the marker
			tracker->setPatternWidth(patternWidth);

			markerId = findMarker(patternID);
			if(	markerId>=0 && markerInfo[markerId].id==patternID)
			{
				ARFloat	patt_center[2] = {0.0f, 0.0f};
				tracker->calcOpenGLMatrixFromMarker(&markerInfo[markerId], patt_center, patternWidth, (float *)buffer);

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
		}

		VxMatrix mat = VxMatrix(gl_para);
		Vx3DDecomposeMatrix(mat, quat, pos, scale);

		// Set true, if marker is detected
		beh->SetOutputParameterValue(OUT_DETECTED, &detected, 0);

		// Set marker id
		beh->SetOutputParameterValue(OUT_MARKERID, &patternID, 0);

		// Set Position
		beh->SetOutputParameterValue(OUT_POSITION, &pos, 0);

		// Set quaternion
		beh->SetOutputParameterValue(OUT_QUATERNION, &quat, 0);

		// Set matrix
		beh->SetOutputParameterValue(OUT_TRANSFORM_MATRIX, &mat, 0);

		//Check object visibility
		if(detected==TRUE)
		{
			// Transform object
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

CKERROR ARTPlusPatternTransformationCB(const CKBehaviorContext& BehContext)
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

int findMarker(int patternID)
{
    // find best visible marker
    int j, k = -1;
    for(j = 0; j < numMarkers; j++)
	{
        if(markerInfo[j].id!=-1 && (patternID==-1 || patternID==markerInfo[j].id))
		{
            if(k == -1)
				k = j;
			else
			if(markerInfo[k].cf < markerInfo[j].cf)
				k = j;
        }
	}
	return k;
}