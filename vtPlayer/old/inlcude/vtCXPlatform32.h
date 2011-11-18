/********************************************************************
	created:	2007/12/12
	created:	12:12:2007   11:54
	filename: 	x:\junctions\ProjectRoot\current\vt_player\exKamPlayer\inlcude\vtCXPlatform32.h
	file path:	x:\junctions\ProjectRoot\current\vt_player\exKamPlayer\inlcude
	file base:	vtCXPlatform32
	file ext:	h
	author:		mc007
	
	purpose:	
*********************************************************************/

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



