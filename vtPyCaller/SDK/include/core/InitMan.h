#ifndef InitMAN_H
#define InitMAN_H

#include "CKBaseManager.h"

#define INIT_MAN_GUID		CKGUID(0x6add640f,0x1acc1484)

class Python;
class PythonModule;


#include <stdlib.h>
#include <map>

#include "pyembed.h"

typedef std::map<CK_ID,PyObject*>PModules;
typedef std::map<CK_ID,PyObject*>::iterator PModulesIt;


typedef std::map<CK_ID,PythonModule*>VSLPModules;
typedef std::map<CK_ID,PythonModule*>::iterator VSLPModulesIt;

class vt_python_man : public CKBaseManager
{

	public:
		//Ctor
		vt_python_man(CKContext* ctx);
		//Dtor
		~vt_python_man();
		
		Python *py;

		static vt_python_man*GetPyMan();

		
		PModules m_pModules;
		CK_ID m_CurrentId;
		
		CK_ID CurrentId() const { return m_CurrentId; }
		void CurrentId(CK_ID val) { m_CurrentId = val; }


		PModules& GetPModules(){return m_pModules;} 

		PyObject *InsertPModule(CK_ID id,XString name,bool reload);
		void RemovePModule(CK_ID id);
		PyObject *GetPModule(CK_ID id);
		void CallPyModule(CK_ID id,XString func);
		void CallPyModule(CK_ID id,Arg_mmap *args,XString func);
		void CallPyModule(CK_ID id,XString func,const Arg_mmap& args,std::string& ret);
		void CallPyModule(CK_ID id,XString func,const Arg_mmap& args,double& ret);
		void CallPyModule(CK_ID id,XString func,const Arg_mmap& args,long& ret);
		void ClearModules();


		VSLPModules m_PVSLModules;
		VSLPModules& GetVSLPModules(){ return m_PVSLModules;}

		PythonModule *CreatePythonModule(char*file,char*func,CK_ID bid);
		PythonModule *DestroyPythonModule(CK_ID bid);
		PythonModule *GetPythonModule(CK_ID bid);
		void ClearPythonModules();
		
		void InsertPVSLModule(PythonModule *pMod);
        
		XString m_stdOut;
		XString m_stdErr;

		int InitPythonModules();
	
		// Initialization	
		virtual CKERROR OnCKInit();
		virtual CKERROR OnCKEnd();
		virtual CKERROR OnCKReset();
		virtual CKERROR PreProcess();
		virtual CKERROR PostProcess();
		virtual CKERROR PostClearAll();
		virtual CKERROR OnCKPlay();


	virtual CKDWORD	GetValidFunctionsMask()	
	{ return CKMANAGER_FUNC_OnCKInit|
		CKMANAGER_FUNC_PostProcess|
		CKMANAGER_FUNC_OnCKEnd|
		CKMANAGER_FUNC_OnCKReset|
		CKMANAGER_FUNC_PreProcess|
		CKMANAGER_FUNC_PostClearAll|
		CKMANAGER_FUNC_OnCKPlay; 
	}

	
	/************************************************************************/
	/* Parameter Functions																	                                        */
	/************************************************************************/

	void RegisterVSL();
	bool pLoaded;
};




#endif
