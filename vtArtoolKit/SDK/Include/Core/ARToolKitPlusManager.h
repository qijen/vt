#ifndef ARToolKitPlusManager_H
#define ARToolKitPlusManager_H "$Id:$"

#include "CKBaseManager.h"

#define ARToolKitPlusManagerGUID CKGUID(0x3d130848, 0x7ee54234)

class ARToolKitPlusManager : public CKBaseManager 
{
//##############################################################
//                Public Part                      
//##############################################################
public :
	ARToolKitPlusManager(CKContext* Context);
	~ARToolKitPlusManager();	
	
//--------------------------------------------------------------
// Methods to implement
//--------------------------------------------------------------

//---  Called before the composition is reset.
	virtual CKERROR OnCKReset();

//---  Called when the process loop is started.
	virtual CKERROR OnCKPlay();

//---  Called when the process loop is paused.
	virtual CKERROR OnCKPause();

//---  Called before the rendering of the 3D objects.
	virtual CKERROR OnPreRender(CKRenderContext* dev);

//---  Called after the rendering of the 3D objects.
	virtual CKERROR OnPostRender(CKRenderContext* dev);

//---  Returns list of functions implemented by the manager.
	 virtual CKDWORD GetValidFunctionsMask() {
		return
		CKMANAGER_FUNC_OnCKReset|
		CKMANAGER_FUNC_OnCKPlay|
		CKMANAGER_FUNC_OnCKPause|
		CKMANAGER_FUNC_OnPreRender|
		CKMANAGER_FUNC_OnPostRender;
	}

//--------------------------------------------------------------
// Unused methods
//--------------------------------------------------------------

/*
//---  Called at the beginning of each process loop.
	virtual CKERROR PreProcess();

//---  Called at the end of each process loop.
	virtual CKERROR PostProcess();

//---  Called after the composition has been restarted.
	virtual CKERROR OnCKPostReset();

//---  Called before a scene becomes active.
	virtual CKERROR PreLaunchScene(CKScene* OldScene, CKScene* NewScene);

//---  Called after a scene became active.
	virtual CKERROR PostLaunchScene(CKScene* OldScene, CKScene* NewScene);

//---  Called at the beginning of a copy.
	virtual CKERROR OnPreCopy(CKDependenciesContext& context);

//---  Called at the end of a copy.
	virtual CKERROR OnPostCopy(CKDependenciesContext& context);

//---  Called when objects are added to a scene.
	virtual CKERROR SequenceAddedToScene(CKScene* scn, CK_ID* objids, int count);

//---  Called when objects are removed from a scene.
	virtual CKERROR SequenceRemovedFromScene(CKScene* scn, CK_ID* objids, int count);

//---  Called just before objects are deleted.
	virtual CKERROR SequenceToBeDeleted(CK_ID* objids, int count);

//---  Called after objects have been deleted.
	virtual CKERROR SequenceDeleted(CK_ID* objids, int count);

//---  Called after the rendering of 2D entities.
	virtual CKERROR OnPostSpriteRender(CKRenderContext* dev);

//---  Called before the backbuffer is presented.
	virtual CKERROR OnPreBackToFront(CKRenderContext* dev);

//---  Called after the backbuffer is presented.
	virtual CKERROR OnPostBackToFront(CKRenderContext* dev);

//---  Called before switching to/from fullscreen.
	virtual CKERROR OnPreFullScreen(BOOL Going2Fullscreen, CKRenderContext* dev);

//---  Called after switching to/from fullscreen.
	virtual CKERROR OnPostFullScreen(BOOL Going2Fullscreen, CKRenderContext* dev);

//---  Called at the end of the creation of a CKContext.
	virtual CKERROR OnCKInit();

//---  Called at deletion of a CKContext.
	virtual CKERROR OnCKEnd();

//---  Called at the beginning of a load operation.
	virtual CKERROR PreLoad();

//---  Called to load manager data.
	virtual CKERROR LoadData(CKStateChunk* chunk, CKFile* LoadedFile) { return CK_OK; }

//---  Called at the end of a load operation.
	virtual CKERROR PostLoad();

//---  Called at the beginning of a save operation.
	virtual CKERROR PreSave();

//---   Called to save manager data. return NULL if nothing to save.
	virtual CKStateChunk* SaveData(CKFile* SavedFile) { return NULL; }

//---  Called at the end of a save operation.
	virtual CKERROR PostSave();

//---  Called at the beginning of a CKContext::ClearAll operation.
	virtual CKERROR PreClearAll();

//---  Called at the end of a CKContext::ClearAll operation.
	virtual CKERROR PostClearAll();


*/
//##############################################################
//                Protected Part                      
//##############################################################
protected :
	
};

#endif

