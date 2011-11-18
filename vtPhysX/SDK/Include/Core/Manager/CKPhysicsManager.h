#ifndef CKPhysicManager_H
	#define CKPhysicManager_H "$Id:$"


#include "vtPhysXBase.h"
#include "CKBaseManager.h"

class MODULE_API CKPhysicManager :public CKBaseManager {
public:
  
  
#ifdef DOCJETDUMMY // Docjet secret macro
#else


	virtual CKERROR OnCKInit()=0; 
	virtual CKERROR PostClearAll()=0;
	virtual CKERROR PreSave()=0;
	virtual CKERROR OnCKReset()=0;
	virtual CKERROR PreProcess()=0;

	virtual CKDWORD GetValidFunctionsMask()	{ return CKMANAGER_FUNC_PostClearAll|
													 CKMANAGER_FUNC_OnCKInit|
													 CKMANAGER_FUNC_PreSave|
													 CKMANAGER_FUNC_PostLoad|
													 CKMANAGER_FUNC_OnCKReset|
													 CKMANAGER_FUNC_PreProcess;
													                      }
	virtual void _RegisterParameters()=0;
	virtual void _RegisterVSL()=0;
	 
	CKPhysicManager(CKContext *Context,CKGUID guid,char* name);
	virtual ~CKPhysicManager() {}


	
#endif // Docjet secret macro
};

// CK2 VERSION ...
#endif
