#ifndef __DLL_TOOLS_H__
#define __DLL_TOOLS_H__

#include <wtypes.h>

																																		*/
namespace DllTools
{

	/*************************************************************************/
	/* class : DllFunc																														*/
	/*  																																			
	
	this is a small helper to bind an exported dll-func to a prototyyp								  

	example, you want to replace a load/save function for custom parameter at run-time: 

	typedef void (*_CGBLCISAVELOADFUNC_proto)(CKParameter *,CKStateChunk **,CKBOOL);

	DllFunc<_CGBLCISAVELOADFUNC_proto>CGBLCISAVELOADFUNC_proto(_T("proto.dll"),"CGBLCISAVELOADFUNC");


	CGBLCISAVELOADFUNC_proto.Load();  // binds the func to the functor

	CGBLCISAVELOADFUNC_proto.Release(); // unbind, but keeps infos about dll-name/func.

	(*CGBLCISAVELOADFUNC_proto)(par,chunk,true); for executing .

	You can use this for dynamic dll to vsl mapping ! : see last comment.


	*/
	template<class T> class DllFunc 
	{
		public:

		DllFunc(	const char* _dllName, const char* _fnName ,const bool logging = TRUE ) :
		dllName(_dllName) , fn(0) , fnName(_fnName)
		{
			if (logging && !dllHandle)
			{ 
				printf("couldn't found DLL\n");
				return;
			}
		}
		operator T()
		{

			printf("executing func : %s ",fnName);

			return fn; 
		}
		void Release()
		{

			FreeLibrary(dllHandle);
			fn = 0;

		}
		void Load()
		{
			FreeLibrary(dllHandle);
			printf("loading lib : %s for function",dllName,fnName);
			dllHandle = ( LoadLibrary (dllName) );
			fn = ( T )GetProcAddress(dllHandle, fnName);
			if (!fn)
			{ 
				printf("couldn't attach fnc \n");
			}
			
		}


	public:
		T fn;
		HMODULE dllHandle;
		const char* dllName;
		const char* fnName;
	};

}/*end namespace */

#endif