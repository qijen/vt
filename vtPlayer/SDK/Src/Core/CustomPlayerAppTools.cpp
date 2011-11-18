/********************************************************************
	created:	2007/11/28
	created:	28:11:2007   16:25
	filename: 	f:\ProjectRoot\current\vt_player\exKamPlayer\src\Dialogs\CustomPlayerConfigurationDialog.cpp
	file path:	f:\ProjectRoot\current\vt_player\exKamPlayer\src\Dialogs
	file base:	CustomPlayerConfigurationDialog
	file ext:	cpp
	author:		mc007
	
	purpose:	
*********************************************************************/
#include <CPStdAfx.h>
#include "CustomPlayerApp.h"
#include "CustomPlayer.h"
#include "CustomPlayerDefines.h"


//////////////////////////////////////////////////////////////////////////
//
//
//
//
//////////////////////////////////////////////////////////////////////////


extern CCustomPlayer*	thePlayer;




vtPlayer::Structs::xEApplicationMode
CCustomPlayer::PGetApplicationMode(const char* pstrCommandLine)
{

	using namespace vtPlayer::Structs;
	
	// Skip the first part of the command line, which is the full path 
	// to the exe.  If it contains spaces, it will be contained in quotes.
	if (*pstrCommandLine == TEXT('\"'))
	{
		pstrCommandLine++;
		while (*pstrCommandLine != TEXT('\0') && *pstrCommandLine != TEXT('\"'))
			pstrCommandLine++;
		if( *pstrCommandLine == TEXT('\"') )
			pstrCommandLine++;
	}
	else
	{
		while (*pstrCommandLine != TEXT('\0') && *pstrCommandLine != TEXT(' '))
			pstrCommandLine++;
		if( *pstrCommandLine == TEXT(' ') )
			pstrCommandLine++;
	}

	// Skip along to the first option delimiter "/" or "-"
	while ( *pstrCommandLine != TEXT('\0') && *pstrCommandLine != TEXT('/') && *pstrCommandLine != TEXT('-') )
		pstrCommandLine++;

	// If there wasn't one, then must be config mode
	if ( *pstrCommandLine == TEXT('\0') )
		return  normal;

	// Otherwise see what the option was
	switch ( *(++pstrCommandLine) )
	{
	case 'p':
	case 'P':
		pstrCommandLine++;
		while ( *pstrCommandLine && !isdigit(*pstrCommandLine) )
			pstrCommandLine++;
		if ( isdigit(*pstrCommandLine) )
		{
#ifdef _WIN64
			CHAR strCommandLine[2048];
			DXUtil_ConvertGenericStringToAnsiCb(strCommandLine, pstrCommandLine, sizeof(strCommandLine));
			m_hWndParent = (HWND)(_atoi64(strCommandLine));
#else
			m_hWndParent = (HWND)LongToHandle(_ttol(pstrCommandLine));

#endif
		}
		return preview;

		//  call config dialog : 
		case 'C':
		case 'c':
			return  config;
		case 'D':
		case 'd':
			return  popup;
		break;
	default:
		// All other options => run the screensaver (typically this is "/s")
		return  normal;
	}
	return  normal;
}
