#include "StdAfx.h"
#include "vt_python_ext.h"
#include "initman.h"
extern vt_python_man* pym;

//////////////////////////////////////////////////////////////////////////
PyObject* log_CaptureStdout(PyObject* self, PyObject* pArgs)
{
	char* LogStr = NULL;
	if (!PyArg_ParseTuple(pArgs, "s", &LogStr)) return NULL;
	if (pym)
	{
		pym->m_stdOut << LogStr;
	}

	
	Py_INCREF(Py_None);
	return Py_None;
}
//////////////////////////////////////////////////////////////////////////
PyObject* log_CaptureStderr(PyObject* self, PyObject* pArgs)
{
		char* LogStr = NULL;
		if (!PyArg_ParseTuple(pArgs, "s", &LogStr)) return NULL;
		if (pym)
		{
			pym->m_stdOut << LogStr;
		}
		Py_INCREF(Py_None);
		return Py_None;
}
//////////////////////////////////////////////////////////////////////////
