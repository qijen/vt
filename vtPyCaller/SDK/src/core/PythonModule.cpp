#include "StdAfx.h"
#include <PythonModule.h>
#include "InitMan.h"

extern vt_python_man *pym;

int pModId = 0;


PythonModule*
PythonModule::GetPythonModule(int id)
{

		VSLPModulesIt  it  = pym->GetVSLPModules().find(id);
		if(it  != pym->GetVSLPModules().end() )
		{
			return it->second;
		}else
		{
			return NULL;
		}
	
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
PythonModule::PythonModule(const char*file,const char*func,int bid) : m_File(file) , m_Func(func) , m_BehaviourID(bid)
{

}
//////////////////////////////////////////////////////////////////////////
PythonModule*
PythonModule::CreatePythonModule(const char*file,const char*func,int bid)
{

	
	PythonModule *pmod = new PythonModule(file,func,bid);
	int targetID = -1;

           
	//////////////////////////////////////////////////////////////////////////
	//its used in schematic  ? 
	
	CKBehavior *script   = static_cast<CKBehavior*>(pym->m_Context->GetObject(bid));
	if (script)
	{
		targetID = bid;
		
		//PyObject* val = PyInt_FromLong(bid);
		//PyObject_SetAttrString( module , "bid", val);
	}else
	{

	}
	
	pym->InsertPVSLModule(pmod);
	return pmod;

}

