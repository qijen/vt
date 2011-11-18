
/*
*****************************************************************
*        Copyright © ITI Scotland 2006      
*----------------------------------------------------------------
* Module     : $File: //depot/ITITM005/Code/GBLCommon/include/GBLPlatformWIN32.h $       
*
* Programmer : $Author: gunther.baumgart $
* Date       : $DateTime: 2006/05/17 15:37:18 $ 
*
*----------------------------------------------------------------
*
* Module Summary : Platform specific Types and Functions.
*
*----------------------------------------------------------------
* $Revision: #5 $ 
* $Change: 21762 $
*****************************************************************
*/

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



