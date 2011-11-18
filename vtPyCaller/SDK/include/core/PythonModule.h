#ifndef __PYTHON_MODULE_H_
#define __PYTHON_MODULE_H_
#endif

#include "pyembed.h"

class PythonModule
{

public:
	PythonModule(const char*file,const char*func,int bid);



	int m_BehaviourID;
	int GetBehaviourID() const { return m_BehaviourID; }
	void SetBehaviourID(int val) { m_BehaviourID = val; }
    
	PyObject *m_PObject;
    
	static PythonModule* CreatePythonModule(const char*file,const char*func,int bid=0);
	static PythonModule* GetPythonModule(int id);



	private:
	
		const char* m_File;
	
		const char* m_Func;
    
};