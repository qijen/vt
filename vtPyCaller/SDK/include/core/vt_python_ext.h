#include <Python.h>

#ifndef __VT_PYTHON_EXT_H_
	#define  __VT_PYTHON_EXT_H_

	PyObject* log_CaptureStdout(PyObject* self, PyObject* pArgs);
	PyObject* log_CaptureStderr(PyObject* self, PyObject* pArgs);
#endif