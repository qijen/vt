
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
#include "xUtils.h"

int
CCustomPlayer::DoSystemCheck(XString&errorText)
{

	/************************************************************************/
	/*	System Check :                                                                      */
	/************************************************************************/
	int foundError = 0;
	//////////////////////////////////////////////////////////////////////////
	//		Check DirectX Version :																							

	bool DirectXVer_OK  = true;
	if (CPR_CHECK_DIRECTX)
	{
		DWORD dxVerMinor=0;
		char *strVersion = NULL;
		xUtils::system::GetDirectVersion(strVersion,dxVerMinor);
        XString dxVer(strVersion);
		XString dxVer3 = dxVer.Crop(0,3);
		int dxVerWeNeedMin = GetPAppStyle()->g_MinimumDirectXVersion;
		int dxVerInt = dxVer3.ToInt();
		if(dxVerInt < dxVerWeNeedMin  )
		{
			errorText << "\n\n" << "Wrong DirectX Version Detected !";
			errorText << "\n\n" << "This application needs at least version : " << GetPAppStyle()->g_MinimumDirectXVersion;
			errorText << "\n\n" << "You have version : " << strVersion;
			errorText << "\n\n" << "You can download it from here :" << CPR_MINIMUM_DIRECTX_VERSION_FAIL_URL ;
			errorText << "\n";
			foundError =1;
			DirectXVer_OK =false;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//		Check System Ram 																								
	
	if (CPR_CHECK_RAM && DirectXVer_OK )
	{
		int ram = xUtils::system::GetPhysicalMemoryInMB();
		if( ram < GetPAppStyle()->g_MiniumumRAM)
		{
			errorText << "\n This application needs at least : " << GetPAppStyle()->g_MiniumumRAM  <<  " of System Memory.";
			errorText << "\n\n\t You have only : " << ram ; 
			foundError = 1;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//		Add the last line : support eMail

	if(foundError)
	{
		errorText << "\n\n You can contact us via EMail : mailto:"<<CP_SUPPORT_EMAIL ;
	}

	m_LastErrorText  = errorText ;
	return foundError;

}
