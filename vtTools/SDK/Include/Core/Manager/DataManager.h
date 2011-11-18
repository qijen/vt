#ifndef DataManager_H
#define DataManager_H "$Id:$"

#include "CKBaseManager.h"

// define whether we use the manager to relay data or the global variable
#define USE_MANAGER


//  [4/13/2009 macro willson] new headers for external access added

#include "gConfig.h"

//----------------------------------------------------------------
//
// External access 
//
#ifdef G_EXTERNAL_ACCESS



	#include <WTypes.h>	//! @todo : HANDLE type 

	#include "MemoryFileMappingTypes.h"	//! todo : use forwards

#endif // G_EXTERNAL_ACCESS



//----------------------------------------------------------------
//
// unique object identifiers 
//
#define DataManagerGUID CKGUID(0x5164ef93, 0x384edab9)

class DataManager : public CKBaseManager 
{
//##############################################################
//                Public Part                      
//##############################################################
public :
	DataManager(CKContext* Context);
	~DataManager();	
	
	VxVector _vPos;


#ifdef G_EXTERNAL_ACCESS
	
	//  [4/13/2009 macro willson]
	//----------------------------------------------------------------
	//
	// External access : data members 
	//
	HANDLE m_hMMFile;
	vtExternalEvent          *m_pData;

	//----------------------------------------------------------------
	//
	// External access : functions
	//


	/*!

	\brief	initiates shared memory helper objects. Must be called due CKInit
	*/
	int _initSharedMemory(int);

	/*
	\brief handles messages. Must be called in a PreProcess.
	*/
	int _SharedMemoryTick(int);


	/*
	\brief Might be silly. Just to clean up. Must be called in PostProcess.
	*/
	int _SharedMemoryTickPost(int);



	//----------------------------------------------------------------
#endif	


	//  [4/13/2009 macro willson] : enabled for external access
	/*!
	\brief Called at the end of each process loop.
	*/
	virtual CKERROR PostProcess();

	//---  Called at the beginning of each process loop.
	virtual CKERROR PreProcess();

	// -- Called once at CKPlay. Needed to initiate shared memory
	CKERROR OnCKInit();

	//-	set function mask for pre and post process callbacks
	virtual CKDWORD GetValidFunctionsMask()	
	{ 
		return CKMANAGER_FUNC_OnCKInit|CKMANAGER_FUNC_PostProcess|CKMANAGER_FUNC_PreProcess;
			
	}

	//  [4/14/2009 macro willson] : end external access	----------------------------------------------------------------



	
//--------------------------------------------------------------
// Unused methods
//--------------------------------------------------------------

/*



	

//---  Called after the composition has been restarted.
	virtual CKERROR OnCKPostReset();

//---  Called before the composition is reset.
	virtual CKERROR OnCKReset();

//---  Called when the process loop is started.
	virtual CKERROR OnCKPlay();

//---  Called when the process loop is paused.
	virtual CKERROR OnCKPause();

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

//---  Called before the rendering of the 3D objects.
	virtual CKERROR OnPreRender(CKRenderContext* dev);

//---  Called after the rendering of the 3D objects.
	virtual CKERROR OnPostRender(CKRenderContext* dev);

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

