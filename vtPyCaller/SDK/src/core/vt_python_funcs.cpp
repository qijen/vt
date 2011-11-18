#include "virtools/vtcxglobal.h"
#include "windows.h"
#include "vt_python_funcs.h"
#include <iostream>
#include "pyembed.h"
#include "initman.h"
#include "vtTools.h"
#include "vt_python_ext.h"
extern vt_python_man* pym;


//************************************
// Method:    syspath_append
// FullName:  syspath_append
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: char *dirname
//************************************
int syspath_append( char *dirname )
{
	PyObject *mod_sys, *dict, *path, *dir;

	PyErr_Clear(  );

	dir = Py_BuildValue( "s", dirname );

	mod_sys = PyImport_ImportModule( "sys" );	/* new ref */
	dict = PyModule_GetDict( mod_sys );	/* borrowed ref */
	path = PyDict_GetItemString( dict, "path" );	/* borrowed ref */

	if( !PyList_Check( path ) )
		return 0;

	PyList_Append( path, dir );

	if( PyErr_Occurred(  ) )
		Py_FatalError( "could not build sys.path" );

	Py_DECREF( mod_sys );


	return 1;
}

//************************************
// Method:    vt_ActivateOut
// FullName:  vt_ActivateOut
// Access:    public static 
// Returns:   PyObject *
// Qualifier:
// Parameter: PyObject * self
// Parameter: PyObject * args
//************************************
static PyObject *vt_ActivateOut( PyObject * self, PyObject * args )
{
	int size = PyTuple_Size(args);
	int bid, index, value;
	PyArg_ParseTuple(args, "iii", &bid, &index, &value); 

    CK_ID cid = bid; 
	CKBehavior *beh = static_cast<CKBehavior*>(pym->m_Context->GetObject(cid)); 
	if (size!=3)
	{
		
		pym->m_Context->OutputToConsoleEx("PyError:%s : This function only accepts 3 arguments : \n\t bid,index,value ",beh->GetName());
		Py_RETURN_NONE;
	}

	if(beh && index < beh->GetOutputCount())
	{
		beh->ActivateOutput(index,value);
	}
    		

	Py_RETURN_NONE;

}
//************************************
// Method:    vt_SetOutVal
// FullName:  vt_SetOutVal
// Access:    public static 
// Returns:   PyObject *
// Qualifier:
// Parameter: PyObject * self
// Parameter: PyObject * args
//************************************
static PyObject *vt_SetOutVal( PyObject * self, PyObject * args )
{
	
	int size = PyTuple_Size(args);
	PyObject *val;
	int bid, index;
	PyArg_ParseTuple(args, "iiO", &bid, &index,&val); 

	CK_ID cid = bid; 
	CKBehavior *beh = static_cast<CKBehavior*>(pym->m_Context->GetObject(cid)); 
	if (size!=3)
	{

		pym->m_Context->OutputToConsole("PyError : This function only accepts 3 arguments : \n\t bid,index,value ");
		Py_RETURN_NONE;
	}
	
	using namespace vtTools;
	
	if (beh && val &&  index < beh->GetOutputParameterCount() )
	{
		if (PyInt_Check(val))
		{
			long ret = PyInt_AsLong(val);
			int retx = static_cast<int>(ret);
			beh->SetOutputParameterValue(index,&retx);
			Py_RETURN_NONE;
		}
		if (PyFloat_Check(val))
		{
			double ret = PyFloat_AsDouble(val);
			float retx  = static_cast<float>(ret);
			beh->SetOutputParameterValue(index,&retx);
			Py_RETURN_NONE;
		}
		if (PyString_Check(val))
		{
			std::string ret;
			CKParameterOut * pout = beh->GetOutputParameter(index);
			XString retx(ret.c_str());
			pout->SetStringValue(retx.Str());
			Py_RETURN_NONE;
		}
		if (PyTuple_Check(val))
		{
			std::string outList = to_string(val);
			CKParameterOut * pout = beh->GetOutputParameter(index);
			XString retx(outList.c_str());
			pout->SetStringValue(retx.Str());
			Py_RETURN_NONE;
		}

		
	}
	Py_RETURN_NONE;
}




//************************************
// Method:    vt_GetInVal
// FullName:  vt_GetInVal
// Access:    public static 
// Returns:   PyObject *
// Qualifier:
// Parameter: PyObject * self
// Parameter: PyObject * args
//************************************
static PyObject *vt_GetInVal( PyObject * self, PyObject * args )
{

	int size = PyTuple_Size(args);
	int bid, index;
	PyArg_ParseTuple(args, "ii", &bid, &index); 

	CK_ID cid = bid; 
	CKBehavior *beh = static_cast<CKBehavior*>(pym->m_Context->GetObject(cid)); 
	if (size!=2)
	{
		pym->m_Context->OutputToConsole("PyError : This function needs 2 arguments : \n\t bid,index");
		Py_RETURN_NONE;
	}

	using namespace vtTools;
	using namespace vtTools::Enums;

	CKParameterManager *pam = static_cast<CKParameterManager *>(pym->m_Context->GetParameterManager());

	if (index < beh->GetInputParameterCount() )
	{
		CKParameterIn *ciIn = beh->GetInputParameter(index);

		CKParameterType pType  = ciIn->GetType();
		vtTools::Enums::SuperType sType = ParameterTools::GetVirtoolsSuperType(pym->m_Context,pam->ParameterTypeToGuid(pType));	
	
		PyObject *val;

		switch (sType)
		{
		case vtSTRING:
			{
				val = PyString_FromString( vtTools::BehaviorTools::GetInputParameterValue<CKSTRING>(beh,index ));
				break;
			}
		case vtFLOAT:
			{
				val = PyFloat_FromDouble(static_cast<float>(vtTools::BehaviorTools::GetInputParameterValue<float>(beh,index )));
				break;
			}
		case vtINTEGER:
			{
				val = PyInt_FromLong( static_cast<long>(vtTools::BehaviorTools::GetInputParameterValue<int>(beh,index )));
				break;
			}

		default :
			XString err("wrong input parameter type: ");
			err << ciIn->GetName() << "Only Types derivated from Interger,Float or String are acceptable";
			pym->m_Context->OutputToConsole(err.Str(),FALSE );
			Py_RETURN_NONE;
		}

		if (!val)
		{
			Py_DECREF(val);	  
		}
		return val;
	}
	Py_RETURN_NONE;
}


PyObject *vt_IsInputActive( PyObject * self, PyObject * args )
{
		
	PyObject *arg;

	int size = PyTuple_Size(args);

	int bid, index, value;
	PyArg_ParseTuple(args, "ii", &bid, &index); 

	CK_ID cid = bid; 

	if (size!=2)
	{
		pym->m_Context->OutputToConsole("PyError : This function only accepts 2 arguments : \n\t bid,index ");
		Py_RETURN_NONE;
	}

	CKBehavior *beh = static_cast<CKBehavior*>(pym->m_Context->GetObject(cid)); 
	if(beh && index < beh->GetInputCount())
	{
		return Py_BuildValue( "i", beh->IsInputActive(index) ? 1:0  );
	}
	Py_RETURN_NONE;
}

static struct PyMethodDef vt_methods[] = 
{
	{"ActivateOut", vt_ActivateOut, METH_VARARGS, NULL},
	{"SetOutVal", vt_SetOutVal, METH_VARARGS, NULL},
	{"GetInVal", vt_GetInVal, METH_VARARGS, NULL},
	{"IsInputActive", vt_IsInputActive, METH_VARARGS, NULL},
	{NULL, NULL, 0, NULL}
};

void VTPy_Init(void);

static struct _inittab VTy_Inittab_Modules[] = 
{
	{"vt", VTPy_Init},
	{NULL, NULL}
};

void VTPy_Init(void)
{

	PyObject *module;
	PyObject *dict, *smode, *SpaceHandlers, *UnpackModes;
	module = Py_InitModule3("vt", vt_methods,"The main vt module");

}



int
vt_python_man::InitPythonModules()
{

	return vpInitModules();
}

int vpInitModules()
{
	
	return PyImport_ExtendInittab(VTy_Inittab_Modules);

}

//************************************
// Method:    PythonLoad
// FullName:  PythonLoad
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
bool PythonLoad()
{
	bool result  = false; 
	if (!pym->pLoaded)
	{
		int argc = 1;
		char *cmd =GetCommandLineA();
		char *c1[1];
		c1[0]=new char[256];
		strcpy(c1[0],cmd);
		result = !pym->InitPythonModules();
		pym->py = new Python(argc,c1);
		pym->pLoaded = true;
		PySetupStdRedirect();
	}
	return result;
}

static PyMethodDef logMethods[] = {
	{
		"CaptureStdout", log_CaptureStdout, METH_VARARGS, "Logs stdout"	},
		{	"CaptureStderr", log_CaptureStderr, METH_VARARGS, "Logs stderr"	},
		{	NULL, NULL, 0, NULL	}
};


void PySetupStdRedirect()
{
	Py_InitModule("log", logMethods);
	PyRun_SimpleString("import log\n"
		"import sys\n"
		"class StdoutCatcher:\n"
		"\tdef write(self, str):\n"
		"\t\tlog.CaptureStdout(str)\n"
		"class StderrCatcher:\n"
		"\tdef write(self, str):\n"
		"\t\tlog.CaptureStderr(str)\n"
		"sys.stdout = StdoutCatcher()\n"
		"sys.stderr = StderrCatcher()\n"
		);
}
//************************************
// Method:    vpCInit
// FullName:  vpCInit
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
int vpCInit()
{
	 Py_Initialize();
	 return true;
}
//************************************
// Method:    vpCPyRun_SimpleString
// FullName:  vpCPyRun_SimpleString
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: const char*cmd
//************************************
void vpCPyRun_SimpleString(const char*cmd)
{
	PyRun_SimpleString(cmd);
}

//************************************
// Method:    vpCPy_Finalize
// FullName:  vpCPy_Finalize
// Access:    public 
// Returns:   void
// Qualifier:
//************************************
void DestroyPython()
{
	if (pym->pLoaded)
	{
		if (pym->py)
		{

		}
	}
	if ( pym->py && pym->py->GetModule())
	{
		Py_XDECREF(pym->py->module);   
		pym->py->module = NULL;
	}
	
	pym->pLoaded = false;
	Py_Finalize();
}
