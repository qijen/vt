////////////////////////////////////////////////////////////////////////////////
//
// ARToolKitPlusManager
// --------------------
//
// Description:
// This manager is used for setting the right camera projection matrix if
// an ARToolKitPlus building block is used and the flag
// ARTPlus(Multi)CorrectCamera is set.
//
//
// Version 1.0	: First Release
//
// Known Bugs	: None
//
// Copyright Jörg Stöcklein <ozone@uni-paderborn.de>, University of Paderborn
////////////////////////////////////////////////////////////////////////////////

#include "CKAll.h"
#include "ARToolKitPlusManager.h"
#include <ARToolKitPlus/TrackerSingleMarker.h>
#include <ARToolKitPlus/TrackerMultiMarker.h>

extern bool ARTPlusInitialized;
extern bool ARTPlusCorrectCamera;

extern bool ARTPlusMultiInitialized;
extern bool ARTPlusMultiCorrectCamera;

extern ARToolKitPlus::TrackerSingleMarker *tracker;
extern ARToolKitPlus::TrackerMultiMarker *multiTracker;

extern void argConvGlpara( float para[4][4], float gl_para[16] );
extern void argConvGlparaTrans( float para[4][4], float gl_para[16] );

VxMatrix projectMat;

ARToolKitPlusManager::ARToolKitPlusManager(CKContext *Context) : 
CKBaseManager(Context, ARToolKitPlusManagerGUID, "ARToolKitPlusManager")
{
	Context->RegisterNewManager(this);
}

ARToolKitPlusManager::~ARToolKitPlusManager()
{
}

//---  Called before the composition is reset.
CKERROR ARToolKitPlusManager::OnCKReset()
{
	return CKERR_NOTIMPLEMENTED;
}

//---  Called when the process loop is started.
CKERROR ARToolKitPlusManager::OnCKPlay()
{
	return CKERR_NOTIMPLEMENTED;
}

//---  Called when the process loop is paused.
CKERROR ARToolKitPlusManager::OnCKPause()
{
	return CKERR_NOTIMPLEMENTED;
}

//---  Called before the rendering of the 3D objects.
CKERROR ARToolKitPlusManager::OnPreRender(CKRenderContext* dev)
{
	if( (ARTPlusInitialized && ARTPlusCorrectCamera) ||
		(ARTPlusMultiInitialized && ARTPlusMultiCorrectCamera) )
	{
		float* buffer = NULL;
		float gl_matirx[4][4];

		if(tracker!=NULL && ARTPlusInitialized)
		{
			buffer = (float *)tracker->getProjectionMatrix();
		}
		else if (multiTracker!=NULL && ARTPlusMultiInitialized)
		{
			buffer = (float *)multiTracker->getProjectionMatrix();
		}
		else
		{
			return CKERR_NOTINITIALIZED;
		}

		argConvGlparaTrans(gl_matirx, buffer);

		gl_matirx[1][1] = -gl_matirx[1][1];

		VxMatrix mat = VxMatrix(gl_matirx);
		projectMat = dev->GetProjectionTransformationMatrix();

		dev->SetProjectionTransformationMatrix(mat);
	}

	return CK_OK;
}

//---  Called after the rendering of the 3D objects.
CKERROR ARToolKitPlusManager::OnPostRender(CKRenderContext* dev)
{
	if(ARTPlusInitialized && ARTPlusCorrectCamera)
	{
		dev->SetProjectionTransformationMatrix(projectMat);
	}

	return CK_OK;
}




