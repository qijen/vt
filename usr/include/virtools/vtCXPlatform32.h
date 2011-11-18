#include <WTypes.h>

//////////////////////////////////////////////////////////////////////////
//Macros :
/*
	From winnt.h :
*/
#ifndef MAKEWORD
	#define MAKEWORD(a, b)      ((WORD)(((BYTE)((DWORD_PTR)(a) & 0xff)) | ((WORD)((BYTE)((DWORD_PTR)(b) & 0xff))) << 8))
#endif

#ifndef LOWORD
	#define LOWORD(l)           ((WORD)((DWORD_PTR)(l) & 0xffff))
#endif

#ifndef HIWORD
	#define HIWORD(l)           ((WORD)((DWORD_PTR)(l) >> 16))
#endif
//////////////////////////////////////////////////////////////////////////



