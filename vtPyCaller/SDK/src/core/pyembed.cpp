#include "StdAfx.h"
#include <iostream>
using std::cout;
#include <sstream>
#include "pyembed.h"
#include "InitMan.h"
extern vt_python_man* pym;
// a generic function to raise an exception when a python
// function call fails.
static void raise_func_failed_exception(const std::string& func_name)  
  throw (Python_exception);

// functions to convert complex python data structures to their C++ 
// counterparts
static void make_list_from_tuple(PyObject* tuple, String_list& out);
static void make_list_from_list(PyObject* list, String_list& out);
static void make_map_from_dict(PyObject* dict, String_map& out);

// class Python_env
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
Python_env::Python_env(int argc, char** argv)
{
  Py_Initialize();    
  PySys_SetArgv(argc, argv);
}
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
Python_env::~Python_env()
{
  Py_Finalize();
}

// class Python_exception : public std::exception
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
Python_exception::Python_exception(const std::string& m)
{
  message = m;
}
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
Python_exception::~Python_exception() throw()
{
}
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
const char* 
Python_exception::what() throw()
{
  return message.c_str();
}

// class Python
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
Python::Python(int argc, char** argv) 
{
  env = new Python_env(argc, argv);
  module = 0;
}
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
Python::~Python()
{
  if (module)
    Py_DECREF(module);
  if (env)
    delete env;
}
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
void 
Python::run_string(const std::string& s)
  throw (Python_exception)
{
  int ret = PyRun_SimpleString(s.c_str());
  if (ret)
    throw Python_exception("There was a error in the script.");
}
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
void 
Python::run_file(const std::string& f)
  throw (Python_exception)
{
  FILE* file = fopen(f.c_str(), "r");
  if (!file)
    {
      std::ostringstream oss;
      oss << "Failed to open file <" << f << '>';
      throw Python_exception(oss.str());
    }
  int ret = PyRun_SimpleFile(file, f.c_str());
  fclose(file);
  if (ret)
    throw Python_exception("There was a error in the script.");
}

////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
void 
Python::load(const std::string& module_name,bool reload)
  throw (Python_exception)
{
	try 
	{
	PyObject* name = PyString_FromString(module_name.c_str());
	module = PyImport_Import(name);    

	if (reload)
		PyImport_ReloadModule(module);

	Py_DECREF(name);
	if (!module)
	{
		//std::ostringstream oss;
		//oss << "Failed to load module <" << module_name << ">";
		//throw Python_exception(oss.str());
	}

	}
	catch (Python_exception ex)
	{
		pym->m_Context->OutputToConsoleEx("PyErr : \t %s",(CKSTRING)ex.what());
		std::cout << ex.what() << "pyexeption";
		PyErr_Clear();

	}
}
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
void 
Python::call(PyObject *imodule,const std::string& func_name)			       
throw (Python_exception)
{
	Arg_map args;
	PyObject* ret_val = make_call(imodule,func_name, args);
	if (ret_val)
		Py_DECREF(ret_val);
}
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
void 
Python::call(const std::string& func_name)			       
  throw (Python_exception)
{
  Arg_map args;
  PyObject* ret_val = make_call(func_name, args);
  if (ret_val)
    Py_DECREF(ret_val);
}

////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
void Python::call(const std::string& func_name,const Arg_map& args)
  throw (Python_exception)
{
  PyObject* ret_val = make_call(func_name, args);
  if (ret_val)
    Py_DECREF(ret_val);
}

////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////


void 
Python::call(const std::string& func_name,const Arg_map& args,long& ret)
  throw (Python_exception)
{
  PyObject* ret_val = make_call(func_name, args);
  if (ret_val)
    {
      if (!PyInt_Check(ret_val))
	{
	  Py_DECREF(ret_val);
	  throw Python_exception("Not an integer value in return.");
	}
      ret = PyInt_AsLong(ret_val);
      Py_DECREF(ret_val);
    }
  else
    raise_func_failed_exception(func_name);
}
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////


void 
Python::call(PyObject *imodule,const std::string& func_name,const Arg_map& args,long& ret)
throw (Python_exception)
{
	PyObject* ret_val = make_call(imodule,func_name, args);
	if (ret_val)
	{
		if (!PyInt_Check(ret_val))
		{
			Py_DECREF(ret_val);
			throw Python_exception("Not an integer value in return.");
		}
		ret = PyInt_AsLong(ret_val);
		Py_DECREF(ret_val);
	}
	else
		raise_func_failed_exception(func_name);
}
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
void 
Python::call(const std::string& func_name,const Arg_map& args,double& ret)
  throw (Python_exception)
{
  PyObject* ret_val = make_call(func_name, args);
  if (ret_val)
    {
      if (!PyFloat_Check(ret_val))
	{
	  Py_DECREF(ret_val);
	  throw Python_exception("Not a real value in return.");
	}
      ret = PyFloat_AsDouble(ret_val);
      Py_DECREF(ret_val);
    }
  else
    raise_func_failed_exception(func_name);
}
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
void 
Python::call(PyObject *imodule,const std::string& func_name,const Arg_map& args,double& ret)
throw (Python_exception)
{
	PyObject* ret_val = make_call(imodule,func_name, args);
	if (ret_val)
	{
		if (!PyFloat_Check(ret_val))
		{
			Py_DECREF(ret_val);
			throw Python_exception("Not a real value in return.");
		}
		ret = PyFloat_AsDouble(ret_val);
		Py_DECREF(ret_val);
	}
	else
		raise_func_failed_exception(func_name);
}
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
void 
Python::call(const std::string& func_name,const Arg_map& args,std::string& ret)
  throw (Python_exception)
{
  PyObject* ret_val = make_call(func_name, args);
  if (ret_val)
    {
      if (!PyString_Check(ret_val))
	{
	  Py_DECREF(ret_val);            
	  throw Python_exception("Not a string value in return.");
	}
      ret = PyString_AsString(ret_val);
      Py_DECREF(ret_val);            
    }
  else
    raise_func_failed_exception(func_name);
}
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
void 
Python::call(PyObject *imodule,const std::string& func_name,const Arg_map& args,std::string& ret)
throw (Python_exception)
{
	PyObject* ret_val = make_call(imodule,func_name, args);
	if (ret_val)
	{
		if (!PyString_Check(ret_val))
		{
			Py_DECREF(ret_val);            
			throw Python_exception("Not a string value in return.");
		}
		ret = PyString_AsString(ret_val);
		Py_DECREF(ret_val);            
	}
	else
		raise_func_failed_exception(func_name);
}
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
void 
Python::call(const std::string& func_name,const Arg_map& args,String_list& ret)
  throw (Python_exception)
{
  PyObject* ret_val = make_call(func_name, args);
  if (ret_val)
    {
      if (PyTuple_Check(ret_val))
	make_list_from_tuple(ret_val, ret);
      else if (PyList_Check(ret_val))
	make_list_from_list(ret_val, ret);
      else
	{
	  Py_DECREF(ret_val);
	  throw Python_exception("Not a tuple or list in return.");
	}
      Py_DECREF(ret_val);            
    }
  else
    raise_func_failed_exception(func_name);
}
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
void 
Python::call(const std::string& func_name,const Arg_map& args,String_map& ret)
  throw (Python_exception)
{
  PyObject* ret_val = make_call(func_name, args);
  if (ret_val)
    {
      if (!PyDict_Check(ret_val))
	{
	  Py_DECREF(ret_val);            
	  throw Python_exception("Not a dictionary object in return.");
	}
      make_map_from_dict(ret_val, ret);
      Py_DECREF(ret_val);            
    }
  else
    raise_func_failed_exception(func_name);
}
PyObject* 
Python::get_function_object(const std::string& func_name)
  throw (Python_exception)
{
  if (!module)
    throw Python_exception("No module loaded.");
  PyObject* func = PyObject_GetAttrString(module, (char*)func_name.c_str());
  if (!func || !PyCallable_Check(func))
    {
      std::ostringstream oss;
      oss << '<' << func_name << "> is not a function or is not callable.";
      throw Python_exception(oss.str());
    }
  return func;
}
//////////////////////////////////////////////////////////////////////////

//************************************
// Method:    get_function_object
// FullName:  Python::get_function_object
// Access:    public 
// Returns:   PyObject*
// Qualifier: throw (Python_exception)
// Parameter: PyObject *imodule
// Parameter: const std::string& func_name
//************************************
PyObject* 
Python::get_function_object(PyObject *imodule,const std::string& func_name)
throw (Python_exception)
{
	if (!imodule)
		throw Python_exception("No module loaded.");
	PyObject* func = PyObject_GetAttrString(imodule, (char*)func_name.c_str());
	if (!func || !PyCallable_Check(func))
	{
		std::ostringstream oss;
		oss << '<' << func_name << "> is not a function or is not callable.";
		throw Python_exception(oss.str());
	}
	return func;
}

PyObject* Python::create_args(const Arg_map& args)
throw (Python_exception)
	{
	if (!module)
		throw Python_exception("No module loaded.");
	size_t sz = args.size();  
	if (!sz)
		return 0;
	PyObject* tuple = PyTuple_New(sz);
	size_t i = 0;
	Arg_map::const_iterator it;
	for (it = args.begin(); it != args.end(); it++)
	{
		PyObject* val = 0;
		switch (it->second)
			{
			case Py_long:
				{
				//long l = strtol(it->first.c_str(), 0, 10);
				long L1 = 1000;
				val = PyInt_FromLong(L1);
				break;
				}
			case Py_real:
				{
					double d = strtod(it->first.c_str(), 0);	    
					val = PyFloat_FromDouble(d);
					break;
				}
			case Py_string:
				{
					val = PyString_FromString(it->first.c_str());
					break;
				}
			default:
				{
					Py_DECREF(tuple);	    
					throw Python_exception("Not a supported type for argument passing.");
				}
			}
		if (!val)
			{
				Py_DECREF(tuple);	  
				std::ostringstream oss;
				oss << "Failed to convert <" << it->second << "> to PyObject.";
				throw Python_exception(oss.str());
			}      
		PyTuple_SetItem(tuple, i++, val);
		//Py_DECREF(val);      
		}
	return tuple;
	}

PyObject* Python::create_args(const Arg_mmap& args,PyObject* val)
throw (Python_exception)
{
	//if (!module)
	//	throw Python_exception("No module loaded.");
	size_t sz = args.size();  
	if (!sz)
		return 0;
	PyObject* tuple = PyTuple_New(sz);
	size_t i = 0;
	Arg_mmap::const_iterator it;
	for (it = args.begin(); it != args.end(); it++)
	{
		switch (it->second)
		{
		case Py_long:
			{
				long l = strtol(it->first.c_str(), 0, 10);
				//long L1 = 1000;
				val = PyInt_FromLong(l);
				break;
			}
		case Py_real:
			{
				double d = strtod(it->first.c_str(), 0);	    
				val = PyFloat_FromDouble(d);
				break;
			}
		case Py_string:
			{
				val = PyString_FromString(it->first.c_str());
				break;
			}
		default:
			{
				Py_DECREF(tuple);	    
				throw Python_exception("Not a supported type for argument passing.");
			}
		}
		if (!val)
		{
			Py_DECREF(tuple);	  
			std::ostringstream oss;
			oss << "Failed to convert <" << it->second << "> to PyObject.";
			throw Python_exception(oss.str());
		}      
		PyTuple_SetItem(tuple, i++, val);
		//Py_DECREF(val);      
	}
	return tuple;
}

static bool cleared = false;
PyObject*Python::make_call(const std::string& func_name,const Arg_mmap& args)
				  throw (Python_exception)
{
	PyObject* _temp = 0;
	PyObject* func = get_function_object(func_name);
	PyObject* args_tuple = create_args(args,_temp);
	Py_XINCREF(args_tuple);
	PyObject* ret = PyObject_CallObject(func, args_tuple);
	Py_XDECREF(args_tuple);
	Py_DECREF(func);  
	Py_XDECREF(_temp);   
	return ret;
}
PyObject*Python::make_call(PyObject* imodule,const std::string& func_name,const Arg_mmap& args)
				  throw (Python_exception)
{
	PyObject* _temp = 0;
	PyObject* func = get_function_object(imodule,func_name);
	PyObject* args_tuple = create_args(args,_temp);
	Py_XINCREF(args_tuple);
	PyObject* ret = PyObject_CallObject(func, args_tuple);
	Py_XDECREF(args_tuple);
	Py_DECREF(func);  
	Py_XDECREF(_temp);   
	return ret;
}

// local functions

void
raise_func_failed_exception(const std::string& func_name)
  throw (Python_exception)
{
  std::ostringstream oss;
  oss << "Call to function <" << func_name << "> failed.";
  throw Python_exception(oss.str());
}

void 
make_list_from_tuple(PyObject* tuple, String_list& out)
{
  if (tuple)
    {
      int size = PyTuple_Size(tuple);
      for (int i=0; i<size; i++)
	{
	  PyObject* val = PyTuple_GetItem(tuple, i);
	  out.push_back(to_string(val));
	}
    }
}

void 
make_list_from_list(PyObject* list, String_list& out)
{
  if (list)
    {
      int size = PyList_Size(list);
      for (int i=0; i<size; i++)
	{
	  PyObject* val = PyList_GetItem(list, i);
	  out.push_back(to_string(val));
	}
    }
}

void 
make_map_from_dict(PyObject* dict, String_map& out)
{
  if (dict)
    {
      PyObject* key;
      PyObject* value;
      int pos = 0;
      while (PyDict_Next(dict, &pos, &key, &value)) 
	out[to_string(key)] = to_string(value);
    }
}

std::string 
to_string(PyObject* val)
{
  if (val)
    {
      std::ostringstream oss;
      if (PyInt_Check(val))
	oss << PyInt_AsLong(val);
      else if (PyLong_Check(val))
	oss << PyLong_AsLong(val);
      else if (PyFloat_Check(val))
	oss << PyFloat_AsDouble(val);
      else if (PyString_Check(val))
	oss << '\"' << PyString_AsString(val) << '\"';
      else if (PyTuple_Check(val))
	{
	  //oss << '(';
	  String_list ret;
	  make_list_from_tuple(val, ret);
	  size_t sz = ret.size();
	  for (size_t i=0; i<sz; i++)
	    {
	      oss << ret[i];
	      if (i != (sz - 1))
		oss << ',';
	    }
	  //oss << ')';
	}
      else if (PyList_Check(val))
	{
	  oss << '[';
	  String_list ret;
	  make_list_from_list(val, ret);
	  size_t sz = ret.size();
	  for (size_t i=0; i<sz; i++)
	    {
	      oss << ret[i];
	      if (i != (sz - 1))
		oss << ',';
	    }
	  oss << ']';
	}
      else if (PyDict_Check(val))
	{
	  oss << '{';
	  String_map ret;
	  make_map_from_dict(val, ret);
	  size_t sz = ret.size();
	  size_t i = 0;
	  String_map::const_iterator it;
	  for (it=ret.begin(); it != ret.end(); it++)
	    {
	      oss << it->first << ':' << it->second;
	      if (i != (sz - 1))
		oss << ',';
	      i++;
	    }
	  oss << '}';
	}
      else
	return "";
      return oss.str();
    }
  else
    return "";
}