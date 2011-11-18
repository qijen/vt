// vDevTestUnit.cpp : Defines the entry point for the DLL application.
//

//#include "stdafx.h"

//
#include "CKAll.h"
#include "Dll_Tools.h"

#include <windows.h>

#include "VSLManagerSDK.h"

#include <typeinfo>
#include <tchar.h>
#include <shellapi.h>



#define DECLAREFUN_6d(ret,func,callconv,dllName,arg1,arg2,arg3,arg4,arg5,arg6){\
	DllFunc< ret (*)(arg1,arg2,arg3,arg4,arg5,arg6) > Dllf(dllName,func);\
	VSLM->RegisterFunction(func, Dllf ,callconv,6,#ret,#arg1,#arg2,#arg3,#arg4,#arg5,#arg6);\
}
#define DECLAREFUN_C_6d(ret,func,dllName,arg1,arg2,arg3,arg4,arg5,arg6) DECLAREFUN_6d(ret,func,VCALLTYPE_CDECL,dllName,arg1,arg2,arg3,arg4,arg5,arg6)


/*

#define DECLAREFUN_0(ret,func,callconv) {\
typedef ret (*fptr)(); \
fptr rawPtr = (fptr) func; \
TPtrToRoutine ptr; \
ptr.CreateFunctionPtr((void**) &rawPtr); \
VSLM->RegisterFunction(#func,ptr,callconv,0,#ret);\
}
#define DECLAREFUN_C_0(ret,func) DECLAREFUN_0(ret,func,VCALLTYPE_CDECL)
#define DECLAREFUN_S_0(ret,func) DECLAREFUN_0(ret,func,VCALLTYPE_STDCALL)
*/

bool is=false;

template<class T>class Real{
    
};

template<class T>class tSocket{

public : 

	tSocket(const char*in){
        resultT = new Real<void*>();        
	}

	T resultT;
	operator T(){
		return resultT;
	}
};


#define DECLAREFUNS_0(ret,func,callconv,dllName) {\
	typedef ret (*fptr)(); \
	DllFunc<fptr>Dllf(_T(dllName),func);\
	fptr rawPtr = (fptr)Dllf; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	is=VSLM->RegisterFunction(func,ptr,callconv,0,#ret);\
}
#define DECLAREFUNSS_0(ret,func,dllName) DECLAREFUNS_0(ret,func,VCALLTYPE_CDECL,dllName)



CKERROR InitInstanc1e(CKContext* context)
{

	using namespace VSL; 

	VSLManager *VSLM = (VSLManager *)context->GetManagerByGuid(VSLMANAGER_GUID);

	DECLAREFUNSS_0(void,"testS","base.dll")



	typedef HINSTANCE(WINAPI *_ShellExec_proto)(HWND,const char *,const char*,const char*,const char *,int);

	DllFunc<_ShellExec_proto>_ShellExec(_T("shell32.dll"),"ShellExecuteA");


	HINSTANCE a = (*_ShellExec)(NULL,"open","www.gmx.net",NULL,NULL, SW_MAXIMIZE);


	//MessageBox(NULL,"","",0);

	//typedef void (*ptrF)();
	//tSocket<void*>("void");

	//DllFunc<ptrF>asd( _T("Base.dll"),"testS");
	//VSLM->RegisterFunction( "testS", asd ,VCALLTYPE_CDECL,0,"void");


    
	//(*asd)();






	//if (is)
		//MessageBox(NULL,"succed","",0);








	//(*_ShellExec)(NULL,"open","www.gmx.net",NULL,NULL, SW_MAXIMIZE); //-) simple and handy, or not ?



	//DECLAREFUN_C_6d(void,"ShellExecuteA","shell32.dll",int,const char*,const char*,const char*,const char*,int)



		//base::win32::shell::testS();


		//VSLM->RegisterSpace();

		//DECLAREFUN_C_0d(void,_ShellExec,"base.dll")







//		typedef void(*_ShellExec_proto)();

		//DllFunc<void(*)()>xas(("Base.dll"),"testS");


		return CK_OK;
}
