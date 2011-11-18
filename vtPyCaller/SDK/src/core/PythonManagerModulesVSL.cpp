#include "StdAfx.h"
#include "InitMan.h"
#include "vt_python_funcs.h"
#include <iostream>
using std::cout;
#include <sstream>
#include "pyembed.h"



#include "PythonModule.h"
//************************************
// Method:    ClearModules
// FullName:  vt_python_man::ClearModules
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void
vt_python_man::ClearPythonModules()
{

	/*
	PModulesIt begin = GetPModules().begin();
	PModulesIt end = GetPModules().end();
	while(begin!=end)
	{

		if (begin->second)
		{
			Py_XDECREF(begin->second);
			begin->second  = NULL;
		}
		begin++;
	}
	GetPModules().clear();
	*/
}
/*
void
vt_python_man::CallPyModule(CK_ID id,XString func,const Arg_mmap& args,long& ret)
{
	CKObject* obj = m_Context->GetObject(id);
	if(obj)
	{
		PyObject *module = GetPModule(id);
		if (module)
		{
			this->py->call(module,func.CStr(),args,ret);
		}
	}

}
void
vt_python_man::CallPyModule(CK_ID id,XString func,const Arg_mmap& args,double& ret)
{
	CKObject* obj = m_Context->GetObject(id);
	if(obj)
	{
		PyObject *module = GetPModule(id);
		if (module)
		{
			this->py->call(module,func.CStr(),args,ret);
		}
	}
}

void
vt_python_man::CallPyModule(CK_ID id,XString func,const Arg_mmap& args,std::string& ret)
{
	CKObject* obj = m_Context->GetObject(id);
	if(obj)
	{
		PyObject *module = GetPModule(id);
		if (module)
		{
			this->py->call(module,func.CStr(),args,ret);
		}
	}
}
void
vt_python_man::CallPyModule(CK_ID id,Arg_mmap *args,XString func)
{

	CKObject* obj = m_Context->GetObject(id);
	if(obj)
	{
		PyObject *module = GetPModule(id);
		if (module)
		{
			this->py->call(module,func.CStr());
		}
	}
}

void 
vt_python_man::CallPyModule(CK_ID id,XString func)
{

	CKObject* obj = m_Context->GetObject(id);
	if(obj)
	{
		PyObject *module = GetPModule(id);
		if (module)
		{
			this->py->call(module,func.CStr());
		}
	}
}*/
//************************************
// Method:    RemovePModule
// FullName:  vt_python_man::RemovePModule
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CK_ID id
//************************************
/*
void
vt_python_man::DestroyPythonModule(CK_ID bid)
{
	CKObject* obj = m_Context->GetObject(id);
	if(obj)
	{
		PModulesIt  it  = GetPModules().find(id);
		if(it  != GetPModules().end() )
		{
            if (it->second)
            {
				Py_DECREF(it->second);
				it->second  = NULL;
				GetPModules().erase(it);
            }
		}
	}
	
}
*/
//************************************
// Method:    GetPModule
// FullName:  vt_python_man::GetPModule
// Access:    public 
// Returns:   PyObject*
// Qualifier:
// Parameter: CK_ID id
//************************************
PythonModule*
vt_python_man::GetPythonModule(CK_ID bid)
{
	/*CKObject* obj = m_Context->GetObject(id);
	if(obj)
	{
		PModulesIt  it  = GetPModules().find(id);
		if(it  != GetPModules().end() )
		{
			return it->second;
		}else
		{
			return NULL;
		}
	}*/
	return NULL;
}

//************************************
// Method:    InsertPModule
// FullName:  vt_python_man::InsertPModule
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: CK_ID id
// Parameter: XString name
// Parameter: bool reload
//************************************
PythonModule*
vt_python_man::CreatePythonModule(char*file,char*func,CK_ID bid)
{

    /*CKObject* obj = m_Context->GetObject(id);

	if(obj)
	{
		if (GetPModule(id))
		{
			RemovePModule(id);
		}
		//bit = BArray.insert(BArray.end(),std::make_pair(target,bodyI));
		PModulesIt  it  = GetPModules().find(id);
		if(it  == GetPModules().end() )
		{

			try 
			{

				PyObject* namep = PyString_FromString( name.CStr() );
				PyObject* module = PyImport_Import(namep);    

				if (reload)
					PyImport_ReloadModule(module);

				Py_DECREF(namep);
				GetPModules().insert( GetPModules().end(),std::make_pair(id,module ) );

				if (!module)
				{
					std::ostringstream oss;
					m_Context->OutputToConsoleEx("PyErr : \t Failed to load module" );
					return NULL;
					
				}

				return module;
			}	catch (Python_exception ex)
			{
				m_Context->OutputToConsoleEx("PyErr : \t %s",(CKSTRING)ex.what());
				std::cout << ex.what() << "pyexeption in beh : ";
				PyErr_Clear();
				//beh->ActivateOutput(1,TRUE);
			}
            

		
			//GetPModules().insert( GetPModules().end(),std::make_pair(id,module ) );
			//return module;
		}
	}
	*/
	return NULL;
}




