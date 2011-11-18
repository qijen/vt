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
#include "resource.h"



//////////////////////////////////////////////////////////////////////////
//
//
//
//
//////////////////////////////////////////////////////////////////////////

extern CCustomPlayerApp theApp;
extern CCustomPlayer*	thePlayer;

INT_PTR CALLBACK ConfigureDialogProcHelper( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    return GetApp()->ConfigureDialogProc( hwndDlg, uMsg, wParam, lParam );
}
//////////////////////////////////////////////////////////////////////////
//
void CCustomPlayerApp::DoConfig()
{
	DialogBox( m_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), m_MainWindow , (DLGPROC)ConfigureDialogProcHelper );
}
//////////////////////////////////////////////////////////////////////////
//

struct _dinfo
{
	int _w,_h,_bpp,_hz;
	_dinfo():_w(0), _h(0),_bpp(0),_hz(0){}
	_dinfo(int w,int h,int bpp,int hz) : _w(w), _h(h),_bpp(bpp),_hz(hz){}
};

#include <stdlib.h>
#include <vector>
std::vector<_dinfo>_modes;

_dinfo _current;
_dinfo *_currentW=NULL;

int _currentDriver=-1;
int _currentWMode=-1;
int currentRes=-1;
int currentBpp=-1;
int currentRRate = -1;
int currentFSSA = -1;


void _UpdateLists(HWND hwndDlg,int DriverId)
{
	vtPlayer::Structs::xSEnginePointers* ep = GetPlayer().GetEnginePointers();
	vtPlayer::Structs::xSEngineWindowInfo* ewinfo = GetPlayer().GetEngineWindowInfo();
	int count = GetPlayer().GetEnginePointers()->TheRenderManager->GetRenderDriverCount();
	int i,index;
	char ChaineW[256];
	bool driverfound = false;

	SendDlgItemMessage(hwndDlg,IDC_LISTMODE,LB_RESETCONTENT,0,0);
	SendDlgItemMessage(hwndDlg,IDC_LISTDRIVER,LB_RESETCONTENT,0,0);
	
	SendDlgItemMessage(hwndDlg,IDCB_BPP,CB_RESETCONTENT,0,0);
	SendDlgItemMessage(hwndDlg,IDCB_RRATE,CB_RESETCONTENT,0,0);
	SendDlgItemMessage(hwndDlg,IDCB_RRATE,CB_RESETCONTENT,0,0);
	SendDlgItemMessage(hwndDlg,IDC_WINSIZE,CB_RESETCONTENT,0,0);


	//////////////////////////////////////////////////////////////////////////
	//we fill the driver list :
	for (i=0;i<count;i++)
	{
		VxDriverDesc *desc=ep->TheRenderManager->GetRenderDriverDescription(i);
		index=SendDlgItemMessage(hwndDlg,IDC_LISTDRIVER,LB_ADDSTRING,0,(LPARAM)desc->DriverName.CStr());
		SendDlgItemMessage(hwndDlg,IDC_LISTDRIVER,LB_SETITEMDATA,index,i);

		//////////////////////////////////////////////////////////////////////////
		//we set the driver in the listbox to driver we found in the player.ini
		if (i==DriverId)
		{
			SendDlgItemMessage(hwndDlg,IDC_LISTDRIVER,LB_SETCURSEL,index,0);
			driverfound = true;
		}

	}
	//FullScreen :
	SendDlgItemMessage(hwndDlg,IDC_CHB_FULLSCREEN,BM_SETCHECK,ewinfo->g_GoFullScreen,0);

	VxDriverDesc *MainDesc=ep->TheRenderManager->GetRenderDriverDescription(ewinfo->g_FullScreenDriver);
	
	XArray<int> bpps;//temporary storage to avoid doubles in the list
	XArray<int> rrates;//temporary storage to avoid doubles in the list
	XArray<int>modes;//temporary storage to avoid doubles in the list
	//////////////////////////////////////////////////////////////////////////
	//we only display window resolutions according to the current display settings:
	HDC         hdc;
	PAINTSTRUCT ps ;
	hdc = BeginPaint (hwndDlg, &ps) ;
	int currentDesktopBpp  = GetDeviceCaps (hdc, BITSPIXEL) ;
	int currentDesktopRRate  = GetDeviceCaps (hdc, VREFRESH) ;
	EndPaint(hwndDlg,&ps);

	//////////////////////////////////////////////////////////////////////////
	//we fill the bpp and the refresh rate combo boxes : 

	for (i=0;i<MainDesc->DisplayModes.Size();i++)
	{
		XArray<int>::Iterator it = bpps.Find(MainDesc->DisplayModes[i].Bpp);
		if( it == bpps.End() )
		{
			bpps.PushBack(MainDesc->DisplayModes[i].Bpp);
			sprintf(ChaineW,"%d",MainDesc->DisplayModes[i].Bpp);
			index = SendDlgItemMessage(hwndDlg,IDCB_BPP,CB_ADDSTRING,0,(LPARAM)ChaineW);
			SendDlgItemMessage(hwndDlg,IDCB_BPP,CB_SETITEMDATA,index,i);

			if (ewinfo->g_Bpp== MainDesc->DisplayModes[i].Bpp)
			{
				SendDlgItemMessage(hwndDlg,IDCB_BPP,CB_SETCURSEL,index,0);
			}
		}
		it = rrates.Find(MainDesc->DisplayModes[i].RefreshRate);
		if( it == rrates.End() )
		{
			rrates.PushBack(MainDesc->DisplayModes[i].RefreshRate);
			sprintf(ChaineW,"%d",MainDesc->DisplayModes[i].RefreshRate);
			index = SendDlgItemMessage(hwndDlg,IDCB_RRATE,CB_ADDSTRING,0,(LPARAM)ChaineW);
			SendDlgItemMessage(hwndDlg,IDCB_RRATE,CB_SETITEMDATA,index,i);
			if (ewinfo->g_RefreshRate == MainDesc->DisplayModes[i].RefreshRate)
			{
				SendDlgItemMessage(hwndDlg,IDCB_RRATE,CB_SETCURSEL,index,0);
			}
		}

		

		//////////////////////////////////////////////////////////////////////////
		XString mode;mode<<MainDesc->DisplayModes[i].Width<<MainDesc->DisplayModes[i].Height;
		it = modes.Find(mode.ToInt());
		if( it == modes.End() )
		{
			modes.PushBack(mode.ToInt());
			sprintf(ChaineW,"%d x %d",MainDesc->DisplayModes[i].Width,MainDesc->DisplayModes[i].Height);
			
			index=SendDlgItemMessage(hwndDlg,IDC_LISTMODE,LB_ADDSTRING,0,(LPARAM)ChaineW);
			SendDlgItemMessage(hwndDlg,IDC_LISTMODE,LB_SETITEMDATA,index,i);

			if (ewinfo->g_Height == MainDesc->DisplayModes[i].Height && ewinfo->g_Width == MainDesc->DisplayModes[i].Width )
			{
				SendDlgItemMessage(hwndDlg,IDC_LISTMODE,LB_SETCURSEL,index,0);
				SendDlgItemMessage(hwndDlg,IDC_LISTMODE,LB_SETTOPINDEX,index,0);
			}

            int iRRate = MainDesc->DisplayModes[i].RefreshRate;
			int iBpp  = MainDesc->DisplayModes[i].Bpp;
			int h= iRRate;
			if (MainDesc->DisplayModes[i].Bpp == currentDesktopBpp && MainDesc->DisplayModes[i].RefreshRate  == currentDesktopRRate )
			{
				index=SendDlgItemMessage(hwndDlg,IDC_WINSIZE,CB_ADDSTRING,0,(LPARAM)ChaineW);
				SendDlgItemMessage(hwndDlg,IDC_WINSIZE,CB_SETITEMDATA,index,i);

				if (ewinfo->g_HeightW == MainDesc->DisplayModes[i].Height && ewinfo->g_WidthW == MainDesc->DisplayModes[i].Width )
				{
					SendDlgItemMessage(hwndDlg,IDC_WINSIZE,CB_SETCURSEL,index,0);
					SendDlgItemMessage(hwndDlg,IDC_WINSIZE,CB_SETTOPINDEX,index,0);
				}
			}
		}
	}
}

INT_PTR CALLBACK CCustomPlayerApp::ConfigureDialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

	
	WORD wNotifyCode = HIWORD(wParam); 
	int wID = LOWORD(wParam);         
	


	vtPlayer::Structs::xSEnginePointers* ep = GetPlayer().GetEnginePointers();
	vtPlayer::Structs::xSEngineWindowInfo* ewinfo = GetPlayer().GetEngineWindowInfo();

	switch (uMsg)
	{
		case WM_INITDIALOG:
		{
			//bpp,rrate,res,resW,driver:
			_UpdateLists(hwndDlg,ewinfo->g_FullScreenDriver);
			
			//FSSA :
			switch(ewinfo->FSSA)
			{
				case 0:
					SendDlgItemMessage(hwndDlg,IDC_FSSA_0,BM_SETCHECK,1,0);
					break;
				case 1:
					SendDlgItemMessage(hwndDlg,IDC_FSSA_1,BM_SETCHECK,1,0);
					break;
				case 2:
					SendDlgItemMessage(hwndDlg,IDC_FSSA_2,BM_SETCHECK,1,0);
					break;
				default:
					SendDlgItemMessage(hwndDlg,IDC_FSSA_0,BM_SETCHECK,1,0);
					break;
			}

		}
		return FALSE;
		
		case WM_COMMAND:
			int prevWidth=0,prevHeight=0,prevBpp=0;

			if (wNotifyCode==LBN_SELCHANGE)
			{
				
				
				if (wID==IDC_LISTDRIVER)
				{
					
					int index=SendDlgItemMessage(hwndDlg,IDC_LISTDRIVER,LB_GETCURSEL,0,0);
					_UpdateLists(hwndDlg,index);

					return FALSE;
				
				}

				if (wID==IDC_LISTMODE)
				{
					currentRes = SendDlgItemMessage(hwndDlg,IDC_LISTMODE,LB_GETCURSEL,0,0);
					//VxDriverDesc *MainDesc=ep->TheRenderManager->GetRenderDriverDescription(ewinfo->g_FullScreenDriver);
					return FALSE;
				}
			}
	
			if(wNotifyCode==BN_CLICKED)
			{
				if (wID==IDC_FSSA_0)
				{
					currentFSSA = 0; 
					return FALSE;
				}

				if (wID==IDC_FSSA_1)
				{
					currentFSSA = 1; 
					return FALSE;
				}
				if (wID==IDC_FSSA_2)
				{
					currentFSSA = 2; 
					return FALSE;
				}
			}

			if (wNotifyCode==CBN_SELCHANGE )
			{
				if (wID==IDC_WINSIZE)
				{
					_currentWMode =SendDlgItemMessage(hwndDlg,IDC_WINSIZE,CB_GETCURSEL,0,0);
					return FALSE;
				}
				if (wID==IDCB_BPP)
				{
					currentBpp =SendDlgItemMessage(hwndDlg,IDCB_BPP,CB_GETCURSEL,0,0);
					return FALSE;
				}
				if (wID==IDCB_RRATE)
				{
					currentRRate =SendDlgItemMessage(hwndDlg,IDCB_RRATE,CB_GETCURSEL,0,0);
					return FALSE;
				}
			}


			//////////////////////////////////////////////////////////////////////////	//////////////////////////////////////////////////////////////////////////
			if (LOWORD(wParam) == IDC_CANCEL2){
				
				EndDialog(hwndDlg, LOWORD(wParam));
				SendNotifyMessageW(m_MainWindow,WM_CLOSE,0,0);

				return TRUE;

			}


			if (LOWORD(wParam) == IDOK) 
			{

					//fssa :  
					if(currentFSSA>=0)
						ewinfo->FSSA = currentFSSA;

					//fullscreen  : 
					ewinfo->g_GoFullScreen = IsDlgButtonChecked(hwndDlg, IDC_CHB_FULLSCREEN ) == BST_CHECKED;
					
					//bpp
					if(currentBpp>=0){
						TCHAR szText[_MAX_PATH + 1];
						SendDlgItemMessage(hwndDlg,IDCB_BPP, CB_GETLBTEXT, (WPARAM)currentBpp, (LPARAM)szText);
						XString o(szText);
						ewinfo->g_Bpp = o.ToInt();
					}
					
					//refresh rate
					if (currentRRate>=0)
					{
						TCHAR szText[_MAX_PATH + 1];
						SendDlgItemMessage(hwndDlg,IDCB_RRATE, CB_GETLBTEXT, (WPARAM)currentRRate, (LPARAM)szText);
						XString o(szText);
						ewinfo->g_RefreshRate = o.ToInt();
					}

					//full screen settings
					if (currentRes>=0)
					{
						int index=SendDlgItemMessage(hwndDlg,IDC_LISTMODE,LB_GETCURSEL,0,0);
						TCHAR szText[MAX_PATH];
						SendDlgItemMessage(hwndDlg,IDC_LISTMODE, LB_GETTEXT, index, ((LPARAM)szText));						
						XStringTokenizer	tokizer(szText, " x ");
						const char*tok = NULL;
						tok = tokizer.NextToken(tok);
						XString width(tok);
						tok = tokizer.NextToken(tok);
						XString height(tok);
						ewinfo->g_Width = width.ToInt();
						ewinfo->g_Height = height.ToInt();
					}

					if (_currentWMode>=0)
					{
						
						TCHAR szText[_MAX_PATH + 1];
						SendDlgItemMessage(hwndDlg,IDC_WINSIZE, CB_GETLBTEXT, (WPARAM)_currentWMode, (LPARAM)szText);
						MessageBox(NULL,szText,"",1);
						XStringTokenizer	tokizer(szText, "x");
						const char*tok = NULL;
						tok = tokizer.NextToken(tok);
						XString width(tok);
						tok = tokizer.NextToken(tok);
						XString height(tok);
						ewinfo->g_WidthW = width.ToInt();
						ewinfo->g_HeightW = height.ToInt();

					}

					EndDialog(hwndDlg, LOWORD(wParam));
					PSaveEngineWindowProperties(CUSTOM_PLAYER_CONFIG_FILE,*GetPlayer().GetEngineWindowInfo());
					SendNotifyMessageW(m_MainWindow,WM_CLOSE,0,0);
			
				return TRUE;
			}
	}
    return FALSE;
}

