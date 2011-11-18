#ifndef __VT_PYTHON_FUNCS_H_
#define  __VT_PYTHON_FUNCS_H_

//class PythonModule;

	int vpInitModules();
	int vpCInit();
	void vpCPyRun_SimpleString(const char*);
	void DestroyPython();
	const char* vpSimpleTest(const char*,const char*,const char*);
	bool PythonLoad();
	void PySetupStdRedirect(void);

	int syspath_append( char *dirname );

	//PythonModule* CreatePythonModule(const char*file,const char*func,int bid);

	
#endif