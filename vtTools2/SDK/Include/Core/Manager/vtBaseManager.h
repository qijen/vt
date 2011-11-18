#ifndef VT_BASE_MANAGER_H__
#define VT_BASE_MANAGER_H__ "$Id:$"


#include "BaseMacros.h"
#include "CKBaseManager.h"


class MODULE_API vtBaseManager : public CKBaseManager 
{

public:

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
	virtual void RegisterParameters()=0;
	virtual void RegisterVSL()=0;

	
	vtBaseManager(CKContext *context,CKGUID guid,char* name) : CKBaseManager(context,guid,name) {};

	~vtBaseManager(){};

};

// CK2 VERSION ...
#endif
