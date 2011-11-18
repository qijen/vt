// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__51C3EE7E_892C_46F8_BD78_73E1459B08BA__INCLUDED_)
#define AFX_STDAFX_H__51C3EE7E_892C_46F8_BD78_73E1459B08BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <CKDefines.h>

#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <msacm.h>

#include <CKContext.h>
#include <CKSoundReader.h>

#include "stdio.h"
#include "./header/OGGreader.h"

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

//CKContext *g_pContext;

/*
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif
*/

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__51C3EE7E_892C_46F8_BD78_73E1459B08BA__INCLUDED_)
