/********************************************************************
	created:	2003/12/01
	filename: 	H:\XLANG PROJECTS\BASE\INCLUDES\Dll_Tools.h
	file path:	 H:\XLANG PROJECTS\BASE\INCLUDES
	file base:	 Dll_Tools
	file ext:	  h
	author:		Günter Baumgart
	purpose:	rum DLL´eln
*********************************************************************/



#ifndef __Dll_Tools_h_
#define __Dll_Tools_h_ "$Id:$"




#include <stdlib.h>
#include <string>
#include <windows.h>


			//////////////////////////////////////////////////////////////////////////
			//ie:
			//	the fnc prototyp :	typedef HINSTANCE(WINAPI *_ShellExec_proto)(HWND,const char *,const char*,const char*,const char *,int);
			//  the fill :	DLL::DllFunc<_ShellExec_proto>_ShellExec(_T("shell32.dll"),"ShellExecute");

			template<class T> class DllFunc 
			{
			public:
				
					DllFunc(const char* dllName, const char* fnName ,const bool logging = TRUE ) :
					
												dllHandle( LoadLibrary (dllName) ) , fn(0)
					{
						  if (!dllHandle && logging)
						  { 
							  //loggin @:
							  return;
						  }
						  fn = ( T )GetProcAddress(dllHandle, fnName);
				
						  if (!fn)
						  {
							  char modName[_MAX_PATH],logmsg [400],_path[_MAX_PATH];
							  GetModuleFileName(  GetModuleHandle(NULL) ,modName,_MAX_PATH );
							  // loggin @:
							  sprintf ( logmsg , "%s %s %s" ,  modName , "couldn´t get function with prototyp : ", typeid(fn).name()  ) ;
							  sprintf( _path , "%s%s", modName ,".log" )	 ;
							  //loggin :
							  return;
						  }
						  
					 }
					operator T(){
						return fn; 
					}
			public:
					T fn;
					HMODULE dllHandle;
					void *ret;
			};




#endif //__win32Tools_h_
					 /*
DLL::DynamicFn<_ShellExec_proto>_ShellExec(_T("shell32.dll"),"ShellExecuteA");
		
	//	DLL::DynamicFn<ptrM>_ShellExec(_T("shell32.dll"),"ShellExecuteA");
		 
		return (*_ShellExec)(NULL,"open","www.gmx.net",NULL,NULL, SW_MAXIMIZE);*/


			/*
			/********************************************************************
			created:	2003/12/01
			filename: 	H:\XLANG PROJECTS\BASE\INCLUDES\Dll_Tools.h
			file path:	 H:\XLANG PROJECTS\BASE\INCLUDES
			file base:	 Dll_Tools
			file ext:	  h
			author:		Günter Baumgart
			purpose:	rum DLL´eln
			*********************************************************************/



#ifndef __Dll_Tools_h_
#define __Dll_Tools_h_ "$Id:$"




#include <stdlib.h>
#include <string>
#include <windows.h>


			//////////////////////////////////////////////////////////////////////////
			//ie:
			//	the fnc prototyp :	typedef HINSTANCE(WINAPI *_ShellExec_proto)(HWND,const char *,const char*,const char*,const char *,int);
			//  the fill :	DLL::DllFunc<_ShellExec_proto>_ShellExec(_T("shell32.dll"),"ShellExecute");

			template<class T> class DllFunc 
				{
				public:

					DllFunc(const char* dllName, const char* fnName ,const bool logging = TRUE ) :

					  dllHandle( LoadLibrary (dllName) ) , fn(0)
						  {
						  if (!dllHandle && logging)
							  { 
							  char modName[_MAX_PATH],logmsg [400],_path[_MAX_PATH];
							  GetModuleFileName(  GetModuleHandle(NULL) ,modName,_MAX_PATH );
							  sprintf ( logmsg , "%s %s %s" ,  modName , "couldn´t find ", dllName  ) ;
							  sprintf( _path , "%s%s", modName ,".log" )	 ;

							  return;
							  }
						  fn = ( T )GetProcAddress(dllHandle, fnName);

						  if (!fn)
							  {
							  char modName[_MAX_PATH],logmsg [400],_path[_MAX_PATH];
							  GetModuleFileName(  GetModuleHandle(NULL) ,modName,_MAX_PATH );
							  // loggin
							  sprintf ( logmsg , "%s %s %s" ,  modName , "couldn´t get function with prototyp : ", typeid(fn).name()  ) ;
							  sprintf( _path , "%s%s", modName ,".log" )	 ;
							  return;
							  }

						  }
					  operator T(){
						  return fn; 
						  }
				public:
					T fn;
					HMODULE dllHandle;
					void *ret;
				};




#endif //__win32Tools_h_
			