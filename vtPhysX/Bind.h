#pragma once

/////////////////////////////////////////////////////////////
//  Externs binding must use these macro

// This macro is duplicated in VSL projet,
// because we don't want to provide VSL src in the SDK
#define NODEFAULT								nodefault

#define STARTVSLBIND(context)					using namespace VSL; \
												VSLManager *VSLM = (VSLManager *)context->GetManagerByGuid(VSLMANAGER_GUID); \
												if (VSLM) { VSLM->RegisterSpace();
#define STOPVSLBIND								}
#define REGISTERVSLGUID(iGUID, iClassName)		VSLM->RegisterGUID(iGUID, iClassName);

/////////////////////////////////////////////////////////////
// Type Declaration
// TODO : 3 param = alias type::RawName()

// Special class option for CKObjects
#ifndef VCLASSOPTION_USERDATA
	#define VCLASSOPTION_USERDATA(x)	((x << 24) & 0xFF000000)
#endif
#define VCLASSOPTION_CKOBJECT		VCLASSOPTION_USERDATA(1)
#define VCLASSOPTION_PINBYPOINTER	VCLASSOPTION_USERDATA(2)

#define DECLARETYPE(type)								VSLM->RegisterClass(#type, sizeof(type));
#define DECLARETYPEALIAS(type,alias)					VSLM->RegisterClass(#type, sizeof(type), alias);
#define DECLARETYPEWITHREF(type,ref)					VSLM->RegisterClass(#type, ref);
#define DECLARETYPEALIASWITHREF(type,ref,alias)			VSLM->RegisterClass(#type, ref, alias);

#if defined(_DEBUG) && !defined(VIRTOOLS_USER_SDK)
	#define DECLAREDOCSDKLINK(type,link)				VSLM->RegisterClassDocSDKLink(#type, link);
#else
	#define DECLAREDOCSDKLINK(type,link)
#endif

#define DECLAREPOINTERTYPE(type)						VSLM->RegisterClass(#type, sizeof(type), NULL, VCLASSOPTION_POINTERCOPY);
#define DECLAREPOINTERTYPEALIAS(type,alias)				VSLM->RegisterClass(#type, sizeof(type), alias, VCLASSOPTION_POINTERCOPY);
#define DECLARECKOBJECTTYPEALIAS(type,alias)			VSLM->RegisterClass(#type, sizeof(type), alias, VCLASSOPTION_POINTERCOPY|VCLASSOPTION_CKOBJECT);

#define DECLAREOBJECTTYPE(type)							VSLM->RegisterClass(#type, sizeof(type), NULL, VCLASSOPTION_MEMCOPY);
#define DECLAREOBJECTTYPEALIAS(type,alias)				VSLM->RegisterClass(#type, sizeof(type), alias, VCLASSOPTION_MEMCOPY);
#define DECLAREOBJECTTYPEALIAS_PINBYPOINTER(type,alias)	VSLM->RegisterClass(#type, sizeof(type), alias, VCLASSOPTION_MEMCOPY|VCLASSOPTION_PINBYPOINTER);

#define DECLAREINHERITANCESIMPLE(a,b)	VSLM->RegisterHeritage(a,b,0);
#define DECLAREINHERITANCEDOUBLE(a,b,c)	VSLM->RegisterHeritage(a,b,c,0);

#define REGISTERCONST(name,type,data)	VSLM->RegisterConstant(name,type,data);

///////////////////////////////////////////////////////////////////
// Enum

#define DECLAREENUM(enumName)						VSLM->RegisterEnum(enumName);
#define DECLAREENUMVALUE(enumName,valueName,value)	VSLM->RegisterEnumMember(enumName,valueName,value);

///////////////////////////////////////////////////////////////////
// Enum

enum BindedConstType
{
	BIND_BOOL,
	BIND_CHAR,
	BIND_INT,
	BIND_FLOAT
};

#define DECLARECONST(constName,constType,value)		VSLM->RegisterConstant(constName,constType,value);

///////////////////////////////////////////////////////////////////
// Member Declaration

#if defined(macintosh) || defined(PSX2) || defined(PSP) || (_XBOX_VER>=200)

#define DECLAREMEMBER(classe,mtype,member) { \
	classe* instance = NULL; \
	int offset = (int) &(instance->member); \
	XASSERT((offset + sizeof(mtype)) <= sizeof(classe)); \
	VSLM->RegisterMember(#classe,#member,#mtype, offset);\
}

#else

#define DECLAREMEMBER(classe,mtype,member) { \
	typedef mtype (classe::*mptr); \
	mptr var = (mptr)(&classe::member); \
	void* ptr = (void*)*(DWORD*)&var; \
	VSLM->RegisterMember(#classe,#member,#mtype,(int)ptr);\
}

#endif


/////////////////////////////////////////////////////////////
// function Declaration

#define DECLAREFUN_0(ret,func,callconv) {\
	typedef ret (*fptr)(); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,0,#ret);\
}
#define DECLAREFUN_C_0(ret,func) DECLAREFUN_0(ret,func,VCALLTYPE_CDECL)
#define DECLAREFUN_S_0(ret,func) DECLAREFUN_0(ret,func,VCALLTYPE_STDCALL)

#define DECLAREFUN_1(ret,func,arg1,callconv) {\
	typedef ret (*fptr)(arg1); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,1,#ret,#arg1);\
}

#define DECLAREFUN_1_WITH_DEF_VALS(ret,func,arg1,defVal1,callconv) {\
	typedef ret (*fptr)(arg1); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,1,#ret,#arg1" = "#defVal1);\
}

#define DECLAREFUN_C_1(ret,func,arg1) DECLAREFUN_1(ret,func,arg1,VCALLTYPE_CDECL)
#define DECLAREFUN_S_1(ret,func,arg1) DECLAREFUN_1(ret,func,arg1,VCALLTYPE_STDCALL)
#define DECLAREFUN_C_1_WITH_DEF_VALS(ret,func,arg1,defVal1) DECLAREFUN_1_WITH_DEF_VALS(ret,func,arg1,defVal1,VCALLTYPE_CDECL)
#define DECLAREFUN_S_1_WITH_DEF_VALS(ret,func,arg1,defVal1) DECLAREFUN_1_WITH_DEF_VALS(ret,func,arg1,defVal1,VCALLTYPE_STDCALL)

#define DECLAREFUN_2(ret,func,arg1,arg2,callconv) { \
	typedef ret (*fptr)(arg1,arg2); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,2,#ret,#arg1,#arg2); \
}

#define DECLAREFUN_2_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,callconv) { \
	typedef ret (*fptr)(arg1,arg2); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,2,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2); \
}

#define DECLAREFUN_C_2(ret,func,arg1,arg2) DECLAREFUN_2(ret,func,arg1,arg2,VCALLTYPE_CDECL)
#define DECLAREFUN_S_2(ret,func,arg1,arg2) DECLAREFUN_2(ret,func,arg1,arg2,VCALLTYPE_STDCALL)
#define DECLAREFUN_C_2_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2) DECLAREFUN_2_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,VCALLTYPE_CDECL)
#define DECLAREFUN_S_2_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2) DECLAREFUN_2_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,VCALLTYPE_STDCALL)

#define DECLAREFUN_3(ret,func,arg1,arg2,arg3,callconv) {\
	typedef ret (*fptr)(arg1,arg2,arg3); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,3,#ret,#arg1,#arg2,#arg3);\
}

#define DECLAREFUN_3_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,callconv) {\
	typedef ret (*fptr)(arg1,arg2,arg3); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,3,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3);\
}

#define DECLAREFUN_C_3(ret,func,arg1,arg2,arg3) DECLAREFUN_3(ret,func,arg1,arg2,arg3,VCALLTYPE_CDECL)
#define DECLAREFUN_S_3(ret,func,arg1,arg2,arg3) DECLAREFUN_3(ret,func,arg1,arg2,arg3,VCALLTYPE_STDCALL)
#define DECLAREFUN_C_3_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3) DECLAREFUN_3_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,VCALLTYPE_CDECL)
#define DECLAREFUN_S_3_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3) DECLAREFUN_3_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,VCALLTYPE_STDCALL)

#define DECLAREFUN_4(ret,func,arg1,arg2,arg3,arg4,callconv) {\
	typedef ret (*fptr)(arg1,arg2,arg3,arg4); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,4,#ret,#arg1,#arg2,#arg3,#arg4);\
}

#define DECLAREFUN_4_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,callconv) {\
	typedef ret (*fptr)(arg1,arg2,arg3,arg4); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,4,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4);\
}

#define DECLAREFUN_C_4(ret,func,arg1,arg2,arg3,arg4) DECLAREFUN_4(ret,func,arg1,arg2,arg3,arg4,VCALLTYPE_CDECL)
#define DECLAREFUN_S_4(ret,func,arg1,arg2,arg3,arg4) DECLAREFUN_4(ret,func,arg1,arg2,arg3,arg4,VCALLTYPE_STDCALL)
#define DECLAREFUN_C_4_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4) DECLAREFUN_4_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,VCALLTYPE_CDECL)
#define DECLAREFUN_S_4_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4) DECLAREFUN_4_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,VCALLTYPE_STDCALL)

#define DECLAREFUN_5(ret,func,arg1,arg2,arg3,arg4,arg5,callconv) {\
	typedef ret (*fptr)(arg1,arg2,arg3,arg4,arg5); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,5,#ret,#arg1,#arg2,#arg3,#arg4,#arg5);\
}

#define DECLAREFUN_5_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,callconv) {\
	typedef ret (*fptr)(arg1,arg2,arg3,arg4,arg5); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,5,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5);\
}

#define DECLAREFUN_C_5(ret,func,arg1,arg2,arg3,arg4,arg5) DECLAREFUN_5(ret,func,arg1,arg2,arg3,arg4,arg5,VCALLTYPE_CDECL)
#define DECLAREFUN_S_5(ret,func,arg1,arg2,arg3,arg4,arg5) DECLAREFUN_5(ret,func,arg1,arg2,arg3,arg4,arg5,VCALLTYPE_STDCALL)
#define DECLAREFUN_C_5_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5) DECLAREFUN_5_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,VCALLTYPE_CDECL)
#define DECLAREFUN_S_5_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5) DECLAREFUN_5_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,VCALLTYPE_STDCALL)

#define DECLAREFUN_6(ret,func,arg1,arg2,arg3,arg4,arg5,arg6,callconv) {\
	typedef ret (*fptr)(arg1,arg2,arg3,arg4,arg5,arg6); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,6,#ret,#arg1,#arg2,#arg3,#arg4,#arg5,#arg6);\
}

#define DECLAREFUN_6_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,callconv) {\
	typedef ret (*fptr)(arg1,arg2,arg3,arg4,arg5,arg6); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,6,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5,#arg6" = "#defVal6);\
}

#define DECLAREFUN_C_6(ret,func,arg1,arg2,arg3,arg4,arg5,arg6) DECLAREFUN_6(ret,func,arg1,arg2,arg3,arg4,arg5,arg6,VCALLTYPE_CDECL)
#define DECLAREFUN_S_6(ret,func,arg1,arg2,arg3,arg4,arg5,arg6) DECLAREFUN_6(ret,func,arg1,arg2,arg3,arg4,arg5,arg6,VCALLTYPE_STDCALL)
#define DECLAREFUN_C_6_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6) DECLAREFUN_6_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,VCALLTYPE_CDECL)
#define DECLAREFUN_S_6_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6) DECLAREFUN_6_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,VCALLTYPE_STDCALL)

///////////////////////////////////////////////////////////////////
// Function Declaration with a different name

#define DECLAREFUNALIAS_0(name,ret,func,callconv) {\
	typedef ret (*fptr)(); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(name,ptr,callconv,0,#ret); \
}

#define DECLAREFUNALIAS_C_0(name,ret,func) DECLAREFUNALIAS_0(name,ret,func,(VSL::VCallType)(VCALLTYPE_CDECL|VCALLTYPE_ALIAS))
#define DECLAREFUNALIAS_S_0(name,ret,func) DECLAREFUNALIAS_0(name,ret,func,(VSL::VCallType)(VCALLTYPE_STDCALL|VCALLTYPE_ALIAS))

#define DECLAREFUNALIAS_1(name,ret,func,arg1,callconv) {\
	typedef ret (*fptr)(arg1); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(name,ptr,callconv,1,#ret,#arg1);\
}
#define DECLAREFUNALIAS_C_1(name,ret,func,arg1) DECLAREFUNALIAS_1(name,ret,func,arg1,(VSL::VCallType)(VCALLTYPE_CDECL|VCALLTYPE_ALIAS))
#define DECLAREFUNALIAS_S_1(name,ret,func,arg1) DECLAREFUNALIAS_1(name,ret,func,arg1,(VSL::VCallType)(VCALLTYPE_STDCALL|VCALLTYPE_ALIAS))

#define DECLAREFUNALIAS_2(name,ret,func,arg1,arg2,callconv) {\
	typedef ret (*fptr)(arg1,arg2); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(name,ptr,callconv,2,#ret,#arg1,#arg2);\
}
#define DECLAREFUNALIAS_C_2(name,ret,func,arg1,arg2) DECLAREFUNALIAS_2(name,ret,func,arg1,arg2,(VSL::VCallType)(VCALLTYPE_CDECL|VCALLTYPE_ALIAS))
#define DECLAREFUNALIAS_S_2(name,ret,func,arg1,arg2) DECLAREFUNALIAS_2(name,ret,func,arg1,arg2,(VSL::VCallType)(VCALLTYPE_STDCALL|VCALLTYPE_ALIAS))

#define DECLAREFUNALIAS_3(name,ret,func,arg1,arg2,arg3,callconv) {\
	typedef ret (*fptr)(arg1,arg2,arg3); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(name,ptr,callconv,3,#ret,#arg1,#arg2,#arg3);\
}
#define DECLAREFUNALIAS_C_3(name,ret,func,arg1,arg2,arg3) DECLAREFUNALIAS_3(name,ret,func,arg1,arg2,arg3,(VSL::VCallType)(VCALLTYPE_CDECL|VCALLTYPE_ALIAS))
#define DECLAREFUNALIAS_S_3(name,ret,func,arg1,arg2,arg3) DECLAREFUNALIAS_3(name,ret,func,arg1,arg2,arg3,(VSL::VCallType)(VCALLTYPE_STDCALL|VCALLTYPE_ALIAS))

#define DECLAREFUNALIAS_4(name,ret,func,arg1,arg2,arg3,arg4,callconv) {\
	typedef ret (*fptr)(arg1,arg2,arg3,arg4); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(name,ptr,callconv,4,#ret,#arg1,#arg2,#arg3,#arg4);\
}
#define DECLAREFUNALIAS_C_4(name,ret,func,arg1,arg2,arg3,arg4) DECLAREFUNALIAS_4(name,ret,func,arg1,arg2,arg3,arg4,(VSL::VCallType)(VCALLTYPE_CDECL|VCALLTYPE_ALIAS))
#define DECLAREFUNALIAS_S_4(name,ret,func,arg1,arg2,arg3,arg4) DECLAREFUNALIAS_4(name,ret,func,arg1,arg2,arg3,arg4,(VSL::VCallType)(VCALLTYPE_STDCALL|VCALLTYPE_ALIAS))

#define DECLAREFUNALIAS_5(name,ret,func,arg1,arg2,arg3,arg4,arg5,callconv) {\
	typedef ret (*fptr)(arg1,arg2,arg3,arg4,arg5); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(name,(fptr)ptr,callconv,4,#ret,#arg1,#arg2,#arg3,#arg4,arg5);\
}
#define DECLAREFUNALIAS_C_5(name,ret,func,arg1,arg2,arg3,arg4,arg5) DECLAREFUNALIAS_5(name,ret,func,arg1,arg2,arg3,arg4,arg5,(VSL::VCallType)(VCALLTYPE_CDECL|VCALLTYPE_ALIAS))
#define DECLAREFUNALIAS_S_5(name,ret,func,arg1,arg2,arg3,arg4,arg5) DECLAREFUNALIAS_5(name,ret,func,arg1,arg2,arg3,arg4,arg5,(VSL::VCallType)(VCALLTYPE_STDCALL|VCALLTYPE_ALIAS))

#define DECLAREFUNALIAS_6(name,ret,func,arg1,arg2,arg3,arg4,arg5,arg6,callconv) {\
	typedef ret (*fptr)(arg1,arg2,arg3,arg4,arg5,arg6); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(name,ptr,callconv,4,#ret,#arg1,#arg2,#arg3,#arg4,arg5,arg6);\
}
#define DECLAREFUNALIAS_C_6(name,ret,func,arg1,arg2,arg3,arg4,arg5,arg6) DECLAREFUNALIAS_6(name,ret,func,arg1,arg2,arg3,arg4,arg5,arg6,(VSL::VCallType)(VCALLTYPE_CDECL|VCALLTYPE_ALIAS))
#define DECLAREFUNALIAS_S_6(name,ret,func,arg1,arg2,arg3,arg4,arg5,arg6) DECLAREFUNALIAS_6(name,ret,func,arg1,arg2,arg3,arg4,arg5,arg6,(VSL::VCallType)(VCALLTYPE_STDCALL|VCALLTYPE_ALIAS))

///////////////////////////////////////////////////////////////////
// constructor Declaration

#define DECLARENEW_0(ret,func,callconv) {\
	typedef ret (*fptr)(BYTE*); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,0,#ret);\
}
#define DECLARECTOR_0(func) DECLARENEW_0(void,func,VCALLTYPE_CDECL)

#define DECLARENEW_1(ret,func,arg1,callconv) {\
	typedef ret (*fptr)(BYTE*,arg1); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,1,#ret,#arg1);\
}
#define DECLARECTOR_1(func,arg1) DECLARENEW_1(void,func,arg1,VCALLTYPE_CDECL)

#define DECLARENEW_2(ret,func,arg1,arg2,callconv) {\
	typedef ret (*fptr)(BYTE*,arg1,arg2); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,2,#ret,#arg1,#arg2);\
}
#define DECLARECTOR_2(func,arg1,arg2) DECLARENEW_2(void,func,arg1,arg2,VCALLTYPE_CDECL)

#define DECLARENEW_3(ret,func,arg1,arg2,arg3,callconv) {\
	typedef ret (*fptr)(BYTE*,arg1,arg2,arg3); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,3,#ret,#arg1,#arg2,#arg3);\
}


#define DECLARECTOR_3(func,arg1,arg2,arg3) DECLARENEW_3(void,func,arg1,arg2,arg3,VCALLTYPE_CDECL)

#define DECLARENEW_4(ret,func,arg1,arg2,arg3,arg4,callconv) {\
	typedef ret (*fptr)(BYTE*,arg1,arg2,arg3,arg4); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,4,#ret,#arg1,#arg2,#arg3,#arg4);\
	}
#define DECLARECTOR_4(func,arg1,arg2,arg3,arg4) DECLARENEW_4(void,func,arg1,arg2,arg3,arg4,VCALLTYPE_CDECL)

#define DECLARENEW_5(ret,func,arg1,arg2,arg3,arg4,arg5,callconv) {\
	typedef ret (*fptr)(BYTE*,arg1,arg2,arg3,arg4,arg5); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,4,#ret,#arg1,#arg2,#arg3,#arg4,arg5);\
	}
#define DECLARECTOR_5(func,arg1,arg2,arg3,arg4,arg5) DECLARENEW_5(void,func,arg1,arg2,arg3,arg4,arg5,VCALLTYPE_CDECL)

#define DECLARENEW_6(ret,func,arg1,arg2,arg3,arg4,arg5,arg6,callconv) {\
	typedef ret (*fptr)(BYTE*,arg1,arg2,arg3,arg4,arg5,arg6); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,4,#ret,#arg1,#arg2,#arg3,#arg4,arg5,arg6);\
	}
#define DECLARECTOR_6(func,arg1,arg2,arg3,arg4,arg5,arg6) DECLARENEW_6(void,func,arg1,arg2,arg3,arg4,arg5,arg6,VCALLTYPE_CDECL)

#define DECLARENEW_7(ret,func,arg1,arg2,arg3,arg4,arg5,arg6,arg7,callconv) {\
	typedef ret (*fptr)(BYTE*,arg1,arg2,arg3,arg4,arg5,arg6,arg7); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,4,#ret,#arg1,#arg2,#arg3,#arg4,arg5,arg6,arg7);\
	}
#define DECLARECTOR_7(func,arg1,arg2,arg3,arg4,arg5,arg6,arg7) DECLARENEW_7(void,func,arg1,arg2,arg3,arg4,arg5,arg6,arg7,VCALLTYPE_CDECL)

#define DECLARENEW_8(ret,func,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,callconv) {\
	typedef ret (*fptr)(BYTE*,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,8,#ret,#arg1,#arg2,#arg3,#arg4,#arg5,#arg6,#arg7,#arg8);\
	}
#define DECLARECTOR_8(func,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8) DECLARENEW_8(void,func,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,VCALLTYPE_CDECL)

// With Def Values

#define DECLARENEW_1_WITH_DEF_VALS(ret,func,arg1,defVal1,callconv) {\
	typedef ret (*fptr)(BYTE*,arg1); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,1,#ret,#arg1" = "#defVal1);\
}
#define DECLARECTOR_1_WITH_DEF_VALS(func,arg1) DECLARENEW_1_WITH_DEF_VALS(void,func,arg1,defVal1,VCALLTYPE_CDECL)

#define DECLARENEW_2_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,callconv) {\
	typedef ret (*fptr)(BYTE*,arg1,arg2); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,2,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2);\
}
#define DECLARECTOR_2_WITH_DEF_VALS(func,arg1,defVal1,arg2,defVal2) DECLARENEW_2_WITH_DEF_VALS(void,func,arg1,defVal1,arg2,defVal2,VCALLTYPE_CDECL)

#define DECLARENEW_3_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,callconv) {\
	typedef ret (*fptr)(BYTE*,arg1,arg2,arg3); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,3,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3);\
}
#define DECLARECTOR_3_WITH_DEF_VALS(func,arg1,defVal1,arg2,defVal2,arg3,defVal3) DECLARENEW_3_WITH_DEF_VALS(void,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,VCALLTYPE_CDECL)

#define DECLARENEW_4_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,callconv) {\
	typedef ret (*fptr)(BYTE*,arg1,arg2,arg3,arg4); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,4,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4);\
	}
#define DECLARECTOR_4_WITH_DEF_VALS(func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4) DECLARENEW_4_WITH_DEF_VALS(void,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,VCALLTYPE_CDECL)

#define DECLARENEW_5_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,callconv) {\
	typedef ret (*fptr)(BYTE*,arg1,arg2,arg3,arg4,arg5); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,5,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5);\
	}
#define DECLARECTOR_5_WITH_DEF_VALS(func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5) DECLARENEW_5_WITH_DEF_VALS(void,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,VCALLTYPE_CDECL)

#define DECLARENEW_6_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,callconv) {\
	typedef ret (*fptr)(BYTE*,arg1,arg2,arg3,arg4,arg5,arg6); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,6,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5,#arg6" = "#defVal6);\
	}
#define DECLARECTOR_6_WITH_DEF_VALS(func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6) DECLARENEW_6_WITH_DEF_VALS(void,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,VCALLTYPE_CDECL)

#define DECLARENEW_7_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,arg7,defVal7,callconv) {\
	typedef ret (*fptr)(BYTE*,arg1,arg2,arg3,arg4,arg5,arg6,arg7); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,7,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5,#arg6" = "#defVal6,#arg7" = "#defVal7);\
	}
#define DECLARECTOR_7_WITH_DEF_VALS(func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,arg7,defVal7) DECLARENEW_7_WITH_DEF_VALS(void,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,arg7,defVal7,VCALLTYPE_CDECL)

#define DECLARENEW_8_WITH_DEF_VALS(ret,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,arg7,defVal7,arg8,defVal8,callconv) {\
	typedef ret (*fptr)(BYTE*,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,callconv,8,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5,#arg6" = "#defVal6,#arg7" = "#defVal7,#arg8" = "#defVal8);\
	}
#define DECLARECTOR_8_WITH_DEF_VALS(func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,arg7,defVal7,arg8,defVal8) DECLARENEW_8_WITH_DEF_VALS(void,func,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,arg7,defVal7,arg8,defVal8,VCALLTYPE_CDECL)

///////////////////////////////////////////////////////////////////
// destructor Declaration


#ifdef __GNUC__
	#define DECLAREDESTRUCTORFUNCTION(classe) void __dest##classe(classe *obj){obj->~classe();}
	#define DECLAREDESTRUCTORFUNCTIONALIAS(classe, alias) void __dest##alias(classe *obj){obj->~classe();}
#else
	#define DECLAREDESTRUCTORFUNCTION(classe) void __dest##classe(classe *obj){obj->~##classe();}
	#define DECLAREDESTRUCTORFUNCTIONALIAS(classe, alias) void __dest##alias(classe *obj){obj->~##classe();}
#endif

#define DECLAREDESTRUCTOR(func) {\
	typedef void (*fptr)(BYTE*); \
	fptr rawPtr = (fptr) func; \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &rawPtr); \
	VSLM->RegisterFunction(#func,ptr,VCALLTYPE_CDECL,0,"void");\
}
///////////////////////////////////////////////////////////////////
// class method Declaration

#define FPTR(a) (fptr) a

#define DECLAREMETHOD_0(classe,ret,method) { \
	typedef ret (classe::*fptr)(); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,0,#ret);\
}

#define DECLAREMETHODC_0(classe,ret,method) { \
	typedef ret (classe::*fptr)() const; \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,TRUE,ptr,VCALLTYPE_STDCALL,0,#ret);\
}

#define DECLAREMETHOD_1(classe,ret,method,arg1) { \
	typedef ret (classe::*fptr)(arg1); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,1,#ret,#arg1);\
}

#define DECLAREMETHOD_1_WITH_DEF_VALS(classe,ret,method,arg1,defVal1) { \
	typedef ret (classe::*fptr)(arg1); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,1,#ret,#arg1" = "#defVal1);\
}

#define DECLAREMETHODC_1(classe,ret,method,arg1) { \
	typedef ret (classe::*fptr)(arg1) const; \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,TRUE,ptr,VCALLTYPE_STDCALL,1,#ret,#arg1);\
}

#define DECLAREMETHODC_1_WITH_DEF_VALS(classe,ret,method,arg1,defVal1) { \
	typedef ret (classe::*fptr)(arg1) const; \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,TRUE,ptr,VCALLTYPE_STDCALL,1,#ret,#arg1" = "#defVal1);\
}

#define DECLAREMETHOD_2(classe,ret,method,arg1,arg2) { \
	typedef ret (classe::*fptr)(arg1,arg2); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,2,#ret,#arg1,#arg2);\
}

#define DECLAREMETHOD_2_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2) { \
	typedef ret (classe::*fptr)(arg1,arg2); \
	fptr var = FPTR(fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,2,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2);\
}


#define DECLAREMETHODC_2(classe,ret,method,arg1,arg2) { \
	typedef ret (classe::*fptr)(arg1,arg2) const; \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,TRUE,ptr,VCALLTYPE_STDCALL,2,#ret,#arg1,#arg2);\
}

#define DECLAREMETHODC_2_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2) { \
	typedef ret (classe::*fptr)(arg1,arg2) const; \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,TRUE,ptr,VCALLTYPE_STDCALL,2,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2);\
}

#define DECLAREMETHOD_3(classe,ret,method,arg1,arg2,arg3) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,3,#ret,#arg1,#arg2,#arg3);\
}

#define DECLAREMETHOD_3_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,3,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3);\
}

#define DECLAREMETHODC_3(classe,ret,method,arg1,arg2,arg3) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3) const; \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,TRUE,ptr,VCALLTYPE_STDCALL,3,#ret,#arg1,#arg2,#arg3);\
}

#define DECLAREMETHODC_3_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3) const; \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,TRUE,ptr,VCALLTYPE_STDCALL,3,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3);\
}

#define DECLAREMETHOD_4(classe,ret,method,arg1,arg2,arg3,arg4) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,4,#ret,#arg1,#arg2,#arg3,#arg4);\
}

#define DECLAREMETHODC_4(classe,ret,method,arg1,arg2,arg3,arg4) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4) const; \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,TRUE,ptr,VCALLTYPE_STDCALL,4,#ret,#arg1,#arg2,#arg3,#arg4);\
}

#define DECLAREMETHOD_4_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,4,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4);\
}

#define DECLAREMETHODC_4_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4) const; \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,TRUE,ptr,VCALLTYPE_STDCALL,4,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4);\
}

#define DECLAREMETHOD_5(classe,ret,method,arg1,arg2,arg3,arg4,arg5) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,5,#ret,#arg1,#arg2,#arg3,#arg4,#arg5);\
}

#define DECLAREMETHODC_5(classe,ret,method,arg1,arg2,arg3,arg4,arg5) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5) const; \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,TRUE,ptr,VCALLTYPE_STDCALL,5,#ret,#arg1,#arg2,#arg3,#arg4,#arg5);\
}

#define DECLAREMETHOD_5_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,5,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5);\
}

#define DECLAREMETHODC_5_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5) const; \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,TRUE,ptr,VCALLTYPE_STDCALL,5,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5);\
}

#define DECLAREMETHOD_6(classe,ret,method,arg1,arg2,arg3,arg4,arg5,arg6) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5,arg6); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,6,#ret,#arg1,#arg2,#arg3,#arg4,#arg5,#arg6);\
}

#define DECLAREMETHODC_6(classe,ret,method,arg1,arg2,arg3,arg4,arg5,arg6) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5,arg6) const; \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,TRUE,ptr,VCALLTYPE_STDCALL,6,#ret,#arg1,#arg2,#arg3,#arg4,#arg5,#arg6);\
}

#define DECLAREMETHOD_6_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5,arg6); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,6,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5,#arg6" = "#defVal6);\
}

#define DECLAREMETHODC_6_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5,arg6) const; \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,TRUE,ptr,VCALLTYPE_STDCALL,6,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5,#arg6" = "#defVal6);\
}

#define DECLAREMETHOD_7(classe,ret,method,arg1,arg2,arg3,arg4,arg5,arg6,arg7) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,7,#ret,#arg1,#arg2,#arg3,#arg4,#arg5,#arg6,#arg7);\
}

#define DECLAREMETHODC_7(classe,ret,method,arg1,arg2,arg3,arg4,arg5,arg6,arg7) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7) const; \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,TRUE,ptr,VCALLTYPE_STDCALL,7,#ret,#arg1,#arg2,#arg3,#arg4,#arg5,#arg6,#arg7);\
}


#define DECLAREMETHOD_7_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,arg7,defVal7) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,7,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5,#arg6" = "#defVal6,#arg7" = "#defVal7);\
}

#define DECLAREMETHODC_7_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,arg7,defVal7) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7) const; \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,TRUE,ptr,VCALLTYPE_STDCALL,7,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5,#arg6" = "#defVal6,#arg7" = "#defVal7);\
}

#define DECLAREMETHOD_8(classe,ret,method,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,8,#ret,#arg1,#arg2,#arg3,#arg4,#arg5,#arg6,#arg7,#arg8);\
}

#define DECLAREMETHODC_8(classe,ret,method,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8) const; \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,TRUE,ptr,VCALLTYPE_STDCALL,8,#ret,#arg1,#arg2,#arg3,#arg4,#arg5,#arg6,#arg7,#arg8);\
}

#define DECLAREMETHOD_8_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,arg7,defVal7,arg8,defVal8) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,8,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5,#arg6" = "#defVal6,#arg7" = "#defVal7,#arg8" = "#defVal8);\
}

#define DECLAREMETHODC_8_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,arg7,defVal7,arg8,defVal8) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8) const; \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,TRUE,ptr,VCALLTYPE_STDCALL,8,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5,#arg6" = "#defVal6,#arg7" = "#defVal7,#arg8" = "#defVal8);\
}
#define DECLAREMETHOD_9_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,arg7,defVal7,arg8,defVal8,arg9,defVal9) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,9,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5,#arg6" = "#defVal6,#arg7" = "#defVal7,#arg8" = "#defVal8,#arg9" = "#defVal9);\
}

#define DECLAREMETHOD_10_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,arg7,defVal7,arg8,defVal8,arg9,defVal9,arg10,defVal10) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,10,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5,#arg6" = "#defVal6,#arg7" = "#defVal7,#arg8" = "#defVal8,#arg9" = "#defVal9,#arg10" = "#defVal10);\
}

#define DECLAREMETHOD_11_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,arg7,defVal7,arg8,defVal8,arg9,defVal9,arg10,defVal10,arg11,defVal11) { \
	typedef ret (classe::*fptr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9,arg10,arg11); \
	fptr var = FPTR(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,FALSE,FALSE,ptr,VCALLTYPE_STDCALL,11,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5,#arg6" = "#defVal6,#arg7" = "#defVal7,#arg8" = "#defVal8,#arg9" = "#defVal9,#arg10" = "#defVal10,#arg11" = "#defVal11);\
}




///////////////////////////////////////////////////////////////////
// class operator Declaration

inline const char* extractOperator(const char* opname)
{
	const char* s = opname+8;
	while (*s == ' ') ++s;
	return s;
}

#define DECLAREOP_0(classe,ret,op) { \
	typedef ret (classe::*fptr)(); \
	fptr var = (fptr)(&classe::op); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,extractOperator(#op),FALSE,FALSE,ptr,VCALLTYPE_STDCALL,0,#ret);\
}

#define DECLAREOP_1(classe,ret,op,arg1) { \
	typedef ret (classe::*fptr)(arg1); \
	fptr var = (fptr)(&classe::op); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,extractOperator(#op),FALSE,FALSE,ptr,VCALLTYPE_STDCALL,1,#ret,#arg1);\
}

#define DECLAREOP_2(classe,ret,op,arg1,arg2) { \
	typedef ret (classe::*fptr)(arg1,arg2); \
	fptr var = (fptr)(&classe::op); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,extractOperator(#op),FALSE,FALSE,ptr,VCALLTYPE_STDCALL,2,#ret,#arg1,#arg2);\
}

// constant versions

#define DECLAREOPC_0(classe,ret,op) { \
	typedef ret (classe::*fptr)() const; \
	fptr var = (fptr)(&classe::op); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,extractOperator(#op),FALSE,TRUE,ptr,VCALLTYPE_STDCALL,0,#ret);\
}

#define DECLAREOPC_1(classe,ret,op,arg1) { \
	typedef ret (classe::*fptr)(arg1) const; \
	fptr var = (fptr)(&classe::op); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,extractOperator(#op),FALSE,TRUE,ptr,VCALLTYPE_STDCALL,1,#ret,#arg1);\
}

#define DECLAREOPC_2(classe,ret,op,arg1,arg2) { \
	typedef ret (classe::*fptr)(arg1,arg2) const; \
	fptr var = (fptr)(&classe::op); \
	TPtrToRoutine ptr; \
	ptr.CreateMethodPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,extractOperator(#op),FALSE,TRUE,ptr,VCALLTYPE_STDCALL,2,#ret,#arg1,#arg2);\
}

///////////////////////////////////////////////////////////////////
// friend method Declaration

#define DECLAREFRIEND_0(ret,method) { \
	typedef ret (*fptr)(); \
	fptr var = (fptr)(&method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterFunction(#method,ptr,VCALLTYPE_CDECL,0,#ret);\
}

#define DECLAREFRIEND_1(ret,method,arg1) { \
	typedef ret (*fptr)(arg1); \
	fptr var = (fptr)(&method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterFunction(#method,ptr,VCALLTYPE_CDECL,1,#ret,#arg1);\
}

#define DECLAREFRIEND_2(ret,method,arg1,arg2) { \
	typedef ret (*fptr)(arg1,arg2); \
	fptr var = (fptr)(&method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterFunction(#method,ptr,VCALLTYPE_CDECL,2,#ret,#arg1,#arg2);\
}

#define DECLAREFRIEND_3(ret,method,arg1,arg2,arg3) { \
	typedef ret (*fptr)(arg1,arg2,arg3); \
	fptr var = (fptr)(&method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterFunction(#method,ptr,VCALLTYPE_CDECL,3,#ret,#arg1,#arg2,#arg3);\
}

#define DECLAREFRIEND_4(ret,method,arg1,arg2,arg3,arg4) { \
	typedef ret (*fptr)(arg1,arg2,arg3,arg4); \
	fptr var = (fptr)(&method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterFunction(#method,ptr,VCALLTYPE_CDECL,4,#ret,#arg1,#arg2,#arg3,#arg4);\
}

///////////////////////////////////////////////////////////////////
// friend operator

#define DECLAREFRIENDOP_0(ret,op) { \
	typedef ret (*fptr)(); \
	fptr var = (fptr)(&op); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterFunction(extractOperator(#op),ptr,VCALLTYPE_CDECL,0,#ret);\
}

#define DECLAREFRIENDOP_1(ret,op,arg1) { \
	typedef ret (*fptr)(arg1); \
	fptr var = (fptr)(&op); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterFunction(extractOperator(#op),ptr,VCALLTYPE_CDECL,1,#ret,#arg1);\
}

#define DECLAREFRIENDOP_2(ret,op,arg1,arg2) { \
	typedef ret (*fptr)(arg1,arg2); \
	fptr var = (fptr)(&op); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterFunction(extractOperator(#op),ptr,VCALLTYPE_CDECL,2,#ret,#arg1,#arg2);\
}

///////////////////////////////////////////////////////////////////
// static method Declaration

#define DECLARESTATIC_0(classe,ret,method) { \
	typedef ret (*fptr)(); \
	fptr var = (fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,TRUE,FALSE,ptr,VCALLTYPE_CDECL,0,#ret);\
}

#define DECLARESTATIC_1(classe,ret,method,arg1) { \
	typedef ret (*fptr)(arg1); \
	fptr var = (fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,TRUE,FALSE,ptr,VCALLTYPE_CDECL,1,#ret,#arg1);\
}

#define DECLARESTATIC_2(classe,ret,method,arg1,arg2) { \
	typedef ret (*fptr)(arg1,arg2); \
	fptr var = (fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,TRUE,FALSE,ptr,VCALLTYPE_CDECL,2,#ret,#arg1,#arg2);\
}

#define DECLARESTATIC_3(classe,ret,method,arg1,arg2,arg3) { \
	typedef ret (*fptr)(arg1,arg2,arg3); \
	fptr var = (fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,TRUE,FALSE,ptr,VCALLTYPE_CDECL,3,#ret,#arg1,#arg2,#arg3);\
}

#define DECLARESTATIC_4(classe,ret,method,arg1,arg2,arg3,arg4) { \
	typedef ret (*fptr)(arg1,arg2,arg3,arg4); \
	fptr var = (fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,TRUE,FALSE,ptr,VCALLTYPE_CDECL,4,#ret,#arg1,#arg2,#arg3,#arg4);\
}

#define DECLARESTATIC_5(classe,ret,method,arg1,arg2,arg3,arg4,arg5) { \
	typedef ret (*fptr)(arg1,arg2,arg3,arg4,arg5); \
	fptr var = (fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,TRUE,FALSE,ptr,VCALLTYPE_CDECL,5,#ret,#arg1,#arg2,#arg3,#arg4,#arg5);\
}

#define DECLARESTATIC_6(classe,ret,method,arg1,arg2,arg3,arg4,arg5,arg6) { \
	typedef ret (*fptr)(arg1,arg2,arg3,arg4,arg5,arg6); \
	fptr var = (fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,TRUE,FALSE,ptr,VCALLTYPE_CDECL,6,#ret,#arg1,#arg2,#arg3,#arg4,#arg5,#arg6);\
}

#define DECLARESTATIC_7(classe,ret,method,arg1,arg2,arg3,arg4,arg5,arg6,arg7) { \
	typedef ret (*fptr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7); \
	fptr var = (fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,TRUE,FALSE,ptr,VCALLTYPE_CDECL,7,#ret,#arg1,#arg2,#arg3,#arg4,#arg5,#arg6,#arg7);\
}

#define DECLARESTATIC_8(classe,ret,method,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8) { \
	typedef ret (*fptr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8); \
	fptr var = (fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,TRUE,FALSE,ptr,VCALLTYPE_CDECL,8,#ret,#arg1,#arg2,#arg3,#arg4,#arg5,#arg6,#arg7,#arg8);\
}

#define DECLARESTATIC_9(classe,ret,method,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8, arg9) { \
	typedef ret (*fptr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9); \
	fptr var = (fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,TRUE,FALSE,ptr,VCALLTYPE_CDECL,9,#ret,#arg1,#arg2,#arg3,#arg4,#arg5,#arg6,#arg7,#arg8,#arg9);\
}

// With Default Values

#define DECLARESTATIC_1_WITH_DEF_VALS(classe,ret,method,arg1,defVal1) { \
	typedef ret (*fptr)(arg1); \
	fptr var = (fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,TRUE,FALSE,ptr,VCALLTYPE_CDECL,1,#ret,#arg1#defVal1);\
}

#define DECLARESTATIC_2_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2) { \
	typedef ret (*fptr)(arg1,arg2); \
	fptr var = (fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,TRUE,FALSE,ptr,VCALLTYPE_CDECL,2,#ret,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2);\
}

#define DECLARESTATIC_3_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3) { \
	typedef ret (*fptr)(arg1,arg2,arg3); \
	fptr var = (fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,TRUE,FALSE,ptr,VCALLTYPE_CDECL,3,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3);\
}

#define DECLARESTATIC_4_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4) { \
	typedef ret (*fptr)(arg1,arg2,arg3,arg4); \
	fptr var = (fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,TRUE,FALSE,ptr,VCALLTYPE_CDECL,4,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4);\
}

#define DECLARESTATIC_5_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5) { \
	typedef ret (*fptr)(arg1,arg2,arg3,arg4,arg5); \
	fptr var = (fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,TRUE,FALSE,ptr,VCALLTYPE_CDECL,5,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5);\
}

#define DECLARESTATIC_6_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6) { \
	typedef ret (*fptr)(arg1,arg2,arg3,arg4,arg5,arg6); \
	fptr var = (fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,TRUE,FALSE,ptr,VCALLTYPE_CDECL,6,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5,#arg6" = "#defVal6);\
}

#define DECLARESTATIC_7_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,arg7,defVal7) { \
	typedef ret (*fptr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7); \
	fptr var = (fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,TRUE,FALSE,ptr,VCALLTYPE_CDECL,7,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5,#arg6" = "#defVal6,#arg7" = "#defVal7);\
}

#define DECLARESTATIC_8_WITH_DEF_VALS(classe,ret,method,arg1,defVal1,arg2,defVal2,arg3,defVal3,arg4,defVal4,arg5,defVal5,arg6,defVal6,arg7,defVal7,arg8,defVal8) { \
	typedef ret (*fptr)(arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8); \
	fptr var = (fptr)(&classe::method); \
	TPtrToRoutine ptr; \
	ptr.CreateFunctionPtr((void**) &var); \
	VSLM->RegisterMethod(#classe,#method,TRUE,FALSE,ptr,VCALLTYPE_CDECL,8,#ret,#arg1" = "#defVal1,#arg2" = "#defVal2,#arg3" = "#defVal3,#arg4" = "#defVal4,#arg5" = "#defVal5,#arg6" = "#defVal6,#arg7" = "#defVal7,#arg8" = "#defVal8);\
}


///////////////////////////////////////////////////////////////////
// Enum for array of GUID used by binded function GetGUID

enum PGUID
{
	PGUID_NONE = 0,
	PGUID_VOIDBUF,
	PGUID_FLOAT,
	PGUID_ANGLE,
	PGUID_PERCENTAGE,
	PGUID_INT,
	PGUID_KEY,
	PGUID_BOOL,
	PGUID_STRING,
	PGUID_RECT,
	PGUID_VECTOR,
	PGUID_2DVECTOR,
	PGUID_QUATERNION,
	PGUID_EULERANGLES,
	PGUID_MATRIX,
	PGUID_COLOR,
	PGUID_BOX,
	PGUID_OBJECTARRAY,
	PGUID_OBJECT,
	PGUID_BEOBJECT,
	PGUID_MESH,
	PGUID_MATERIAL,
	PGUID_TEXTURE,
	PGUID_SPRITE,
	PGUID_3DENTITY,
	PGUID_CURVEPOINT,
	PGUID_LIGHT,
	PGUID_TARGETLIGHT,
	PGUID_ID,
	PGUID_CAMERA,
	PGUID_TARGETCAMERA,
	PGUID_SPRITE3D,
	PGUID_OBJECT3D,
	PGUID_BODYPART,
	PGUID_CHARACTER,
	PGUID_CURVE,
	PGUID_2DCURVE,
	PGUID_LEVEL,
	PGUID_PLACE,
	PGUID_GROUP,
	PGUID_2DENTITY,
	PGUID_RENDEROBJECT,
	PGUID_SPRITETEXT,
	PGUID_SOUND,
	PGUID_WAVESOUND,
	PGUID_MIDISOUND,
	PGUID_OBJECTANIMATION,
	PGUID_ANIMATION,
	PGUID_KINEMATICCHAIN,
	PGUID_SCENE,
	PGUID_BEHAVIOR,
	PGUID_MESSAGE,
	PGUID_SYNCHRO,
	PGUID_CRITICALSECTION,
	PGUID_STATE,
	PGUID_ATTRIBUTE,
	PGUID_CLASSID,
	PGUID_DIRECTION,
	PGUID_BLENDMODE,
	PGUID_FILTERMODE,
	PGUID_BLENDFACTOR,
	PGUID_FILLMODE,
	PGUID_LITMODE,
	PGUID_SHADEMODE,
	PGUID_GLOBALEXMODE,
	PGUID_ZFUNC,
	PGUID_ADDRESSMODE,
	PGUID_WRAPMODE,
	PGUID_3DSPRITEMODE,
	PGUID_FOGMODE,
	PGUID_LIGHTTYPE,
	PGUID_SPRITEALIGN,
	PGUID_SCRIPT,
	PGUID_LAYERTYPE,
	PGUID_STATECHUNK,
	PGUID_DATAARRAY,
	PGUID_COMPOPERATOR,
	PGUID_BINARYOPERATOR,
	PGUID_SETOPERATOR,
	PGUID_SPRITETEXTALIGNMENT,
	PGUID_OBSTACLEPRECISION,
	PGUID_OBSTACLEPRECISIONBEH,
	PGUID_OBSTACLE,
	PGUID_PATCHMESH,
	PGUID_POINTER,
	PGUID_ENUMS,
	PGUID_STRUCTS,
	PGUID_FLAGS,
	PGUID_FILTER,
	PGUID_TIME,
	PGUID_OLDTIME,
	PGUID_COPYDEPENDENCIES,
	PGUID_DELETEDEPENDENCIES,
	PGUID_SAVEDEPENDENCIES,
	PGUID_REPLACEDEPENDENCIES,
	PGUID_SCENEACTIVITYFLAGS,
	PGUID_SCENEOBJECT,
	PGUID_SCENERESETFLAGS,
	PGUID_ARRAYTYPE,
	PGUID_RENDEROPTIONS,
	PGUID_PARAMETERTYPE,
	PGUID_MATERIALEFFECT,
	PGUID_TEXGENEFFECT,
	PGUID_TEXGENREFEFFECT,
	PGUID_COMBINE2TEX,
	PGUID_COMBINE3TEX,
	PGUID_BUMPMAPPARAM,
	PGUID_TEXCOMBINE,
	PGUID_PIXELFORMAT,
	PGUID_AXIS,
	PGUID_SUPPORT,
	PGUID_BITMAP_SYSTEMCACHING,
	PGUID_OLDMESSAGE,
	PGUID_OLDATTRIBUTE,
	PGUID_3DPOINTCLOUD,
	PGUID_VIDEO,
	PGUID_RTVIEW,

	PGUID_LASTGUID // must stay in last position
};
