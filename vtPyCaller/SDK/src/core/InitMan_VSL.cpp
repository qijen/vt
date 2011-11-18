#include "StdAfx.h"
#include "InitMan.h"



#include "Dll_Tools.h"
#include "vt_python_funcs.h"

static vt_python_man *_im;

#include "PythonModule.h"
#ifdef RegisterClassA
#undef RegisterClassA
#endif
#include "VSLManagerSDK.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
vt_python_man::vt_python_man(CKContext* context):CKBaseManager(context,INIT_MAN_GUID,"vt_python_man")//Name as used in profiler
{
	m_Context->RegisterNewManager(this);
	_im = this;
}

void
vt_python_man::RegisterVSL(){

		
		STARTVSLBIND(m_Context)

		DECLAREPOINTERTYPE(PythonModule)

		DECLARESTATIC_3(PythonModule,PythonModule*,CreatePythonModule,const char*,const char*,int)


        DECLAREFUN_C_0(void,vpCInit)
		DECLAREFUN_C_1(void,vpCPyRun_SimpleString,const char*)
		//DECLAREFUN_S_3(const char*,vpSimpleTest,const char*,const char*,const char*)
		DECLAREFUN_C_0(void,DestroyPython)
		DECLAREFUN_C_0(void,PythonLoad)
        
				
		/************************************************************************/
		/* Variable|Parameter Stuff                                                                     */
		/************************************************************************/

		
		//DECLAREFUN_C_3(BOOL,VT_SetVariableValue,const char*, int,bool )
		


		//DECLAREFUN_C_1(BOOL,ImportVars,const char*)
		/*DECLAREFUN_C_4(BOOL, WriteIniValue, const char* ,const char*,const char*,const char*)
		DECLAREFUN_C_1(BOOL, CreatePath,const char*)
		DECLAREFUN_C_0(void, testAll)

		DECLAREFUN_C_2(BOOL, ShowNodalDebug,CKGroup*,BOOL)*/


				
		
	STOPVSLBIND

		


}