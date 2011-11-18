#include <CPStdAfx.h>
#include "CustomPlayerApp.h"
#include "CustomPlayer.h"
#include "CustomPlayerDefines.h"

#include "resourceplayer.h"
#include "CustomPlayerDialogGraphicPage.h"
#include "CustomPlayerDialog.h"

IMPLEMENT_DYNAMIC(CustomPlayerDialog, CPropertySheet)

BEGIN_MESSAGE_MAP(CustomPlayerDialog, CPropertySheet)
	//{{AFX_MSG_MAP(CModalShapePropSheet)
	ON_COMMAND(ID_APPLY_NOW, OnApplyNow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CustomPlayerDialogAboutPage*aboutPage=NULL;
CustomPlayerDialogGraphicPage *stylePage=NULL;
CustomPlayerDialogErrorPage *errorPage=NULL;
//************************************
// Method:    CModalShapePropSheet
// FullName:  CModalShapePropSheet::CModalShapePropSheet
// Access:    public 
// Returns:   // Qualifier: : CPropertySheet(AFX_IDS_APP_TITLE, pWndParent)
// Parameter: CWnd* pWndParent 
// Parameter: int displayErrorPage
// Parameter: CString errorText
//************************************
CustomPlayerDialog::CustomPlayerDialog(CWnd* pWndParent,CString errorText)
: CPropertySheet(AFX_IDS_APP_TITLE, pWndParent)
{
		

	if (GetPlayer().GetPAppStyle()->g_ShowConfigTab)
	{
		AddPage(&m_GraphicPage);
		stylePage = &m_GraphicPage;
	}
	if (strlen(errorText))
	{
		AddPage(&m_errorPage);
		errorPage = &m_errorPage;
		m_ErrorText = errorText;
		m_errorPage.errorText = errorText;
	}

	if (GetPlayer().GetPAppStyle()->g_ShowAboutTab)
	{
		AddPage(&m_aboutPage);
		aboutPage  =  &m_aboutPage;
	}
}

bool _checkOpenGLVersion(XString driverName)
{
	vtPlayer::Structs::xSEngineWindowInfo* ewinfo = GetPlayer().GetEngineWindowInfo();
	XArray<XString>::Iterator begin = ewinfo->g_OpenGLMask.Begin();
	XArray<XString>::Iterator end = ewinfo->g_OpenGLMask.End();

	while(begin!=end)
	{
		XString supportedOpenglVersionString = *begin;
		if (supportedOpenglVersionString.Length())
		{
			if (driverName.Contains(supportedOpenglVersionString))
			{
				return true;
			}

		}
		begin++;
	}
    return false;
}
//************************************
// Method:    _UpdateDriverList
// FullName:  _UpdateDriverList
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: int DriverId
//************************************
void _UpdateDriverList(int DriverId)
{

	vtPlayer::Structs::xSEnginePointers* ep = GetPlayer().GetEnginePointers();
	vtPlayer::Structs::xSEngineWindowInfo* ewinfo = GetPlayer().GetEngineWindowInfo();
	int count = GetPlayer().GetEnginePointers()->TheRenderManager->GetRenderDriverCount();
	//////////////////////////////////////////////////////////////////////////
	//we fill the driver list :
	for (int i=0;i<count;i++)
	{
		VxDriverDesc *desc=ep->TheRenderManager->GetRenderDriverDescription(i);

		XString driverName = desc->DriverName.CStr();

		//////////////////////////////////////////////////////////////////////////
		//we are using an OpenGL version restriction, the versions are specified in the player.ini seperated by a comma : 1.1,2.0
		if (driverName.Contains("OpenGL"))
		{
            if (!_checkOpenGLVersion(driverName))
            {
				continue;
            }
		}
		//////////////////////////////////////////////////////////////////////////
		//we add it to the drivers combo box : 
		stylePage->m_Driver.AddString(desc->DriverName.CStr());

		//we set the driver in the listbox to driver we found in the player.ini
		if (i==DriverId)
		{
			stylePage->m_Driver.SetCurSel(i);
		}
	}
}


void _UpdateResolutionLists(HWND hwndDlg,int DriverId)
{
	vtPlayer::Structs::xSEnginePointers* ep = GetPlayer().GetEnginePointers();
	vtPlayer::Structs::xSEngineWindowInfo* ewinfo = GetPlayer().GetEngineWindowInfo();
	int count = GetPlayer().GetEnginePointers()->TheRenderManager->GetRenderDriverCount();
	int i=0;
	char ChaineW[256];
	bool driverfound = false;

	int targetDriver = ewinfo->g_GoFullScreen ? ewinfo->g_FullScreenDriver : ewinfo->g_WindowedDriver ; 
	VxDriverDesc *MainDesc=ep->TheRenderManager->GetRenderDriverDescription(targetDriver);


	XArray<int>bpps;//temporary storage to avoid doubles in the list
	XArray<int>rrates;//temporary storage to avoid doubles in the list
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
	//we fill the bpp list 
	for (i=0;i<MainDesc->DisplayModes.Size();i++)
	{
		XArray<int>::Iterator it = bpps.Find(MainDesc->DisplayModes[i].Bpp);


		if( it == bpps.End() )
		{
			bpps.PushBack(MainDesc->DisplayModes[i].Bpp);
			sprintf(ChaineW,"%d",MainDesc->DisplayModes[i].Bpp);
			stylePage->m_Bpps.AddString(ChaineW);

			if (ewinfo->g_Bpp== MainDesc->DisplayModes[i].Bpp)
			{
				stylePage->m_Bpps.SetCurSel(stylePage->m_Bpps.GetCount()-1);
				
			}
		}
		//////////////////////////////////////////////////////////////////////////
		//	we fill the refresh  list : 
		it = rrates.Find(MainDesc->DisplayModes[i].RefreshRate);
		if( it == rrates.End() )
		{
			rrates.PushBack(MainDesc->DisplayModes[i].RefreshRate);
			sprintf(ChaineW,"%d",MainDesc->DisplayModes[i].RefreshRate);
			stylePage->m_RRates.AddString(ChaineW);
			if (ewinfo->g_RefreshRate == MainDesc->DisplayModes[i].RefreshRate)
			{
				stylePage->m_RRates.SetCurSel(stylePage->m_RRates.GetCount()-1);
			}
		}



		//////////////////////////////////////////////////////////////////////////
		//we fill the resolution list, pseudo code : 
		
		// create a string of the resolution given by the description "MainDesc", : "WidthHeight" or ie.: "800600"
		XString mode;mode<<MainDesc->DisplayModes[i].Width<<MainDesc->DisplayModes[i].Height;

		// have a look in temporary array whether it already exists : 
		it = modes.Find(mode.ToInt());

		//	the iterator couldn't find the resolution, so add it to the modes (full screen + windowed) combo box  
		if( it == modes.End() )
		{

			// create the final string for combo box item : 
			sprintf(ChaineW,"%d x %d",MainDesc->DisplayModes[i].Width,MainDesc->DisplayModes[i].Height);


			//////////////////////////////////////////////////////////////////////////	//////////////////////////////////////////////////////////////////////////
            //we have a resolution restriction  ? 
			if (ewinfo->g_HasResMask)
			{
				// in the player.ini file you have two entries : "XResolutions" and "YResolutions", ie.:  
				// XResolutions = 1024,800
				// YResolutions = 768,600
                // where you must have always an appropriate value for x or y !  
				
				// we check that the given width from the MainDescr is in the resolution mask "ewinfo->g_ResMaskArrayX"
				// which is filled during the startup in the function CCustomPlayer::PLoadEngineWindowProperties (CustomPlayerProfileXML.cpp)

				XArray<int>::Iterator itx = ewinfo->g_ResMaskArrayX.Find(  MainDesc->DisplayModes[i].Width);

				// if not, we just skip the current  MainDescr : 
				if(itx==ewinfo->g_ResMaskArrayX.End())
				{
					continue;
				}else
				{
					//Ok, the given width is in the mask, so we also check that height matches :

					// we take the appropriate y value from the mask y-array : 
					int widthsIndex  =  ewinfo->g_ResMaskArrayX.GetPosition(MainDesc->DisplayModes[i].Width) ;
					int appropriateYValue = *ewinfo->g_ResMaskArrayY.At(widthsIndex);

					if(appropriateYValue !=MainDesc->DisplayModes[i].Height )
					{
                        //No !, so we can skip the MainDescr.
						continue;
					}

				}
				////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////

				// add the modes string representation to our temp array : 
				modes.PushBack(mode.ToInt());

				// create the final string for combo box item : 
				//sprintf(ChaineW,"%d x %d",MainDesc->DisplayModes[i].Width,MainDesc->DisplayModes[i].Height);

				// now add it to full screen resolution combo box : 
				stylePage->m_FModes.AddString(ChaineW);

				//and to the windowed resolution combo box : 
				stylePage->m_windowMode.AddString(ChaineW);

				// the current resolution equals the player.ini full screen value?  so we set it as the selected one in the combo box :
				if (ewinfo->g_Height == MainDesc->DisplayModes[i].Height && ewinfo->g_Width == MainDesc->DisplayModes[i].Width )
				{
					stylePage->m_FModes.SetCurSel( stylePage->m_FModes.GetCount()-1 );

				}
				// the current resolution equals the player.ini windowed resolution value ?  so we set it as the selected one in the combo box :
				if (ewinfo->g_HeightW == MainDesc->DisplayModes[i].Height && ewinfo->g_WidthW == MainDesc->DisplayModes[i].Width )
				{
					stylePage->m_windowMode.SetCurSel( stylePage->m_windowMode.GetCount()-1 );
				}
			}else
			{
				// add the modes string representation to our temp array : 
				modes.PushBack(mode.ToInt());

				// create the final string for combo box item : 
				//sprintf(ChaineW,"%d x %d",MainDesc->DisplayModes[i].Width,MainDesc->DisplayModes[i].Height);

				// now add it to full screen resolution combo box : 
				stylePage->m_FModes.AddString(ChaineW);

				//and to the windowed resolution combo box : 
				stylePage->m_windowMode.AddString(ChaineW);

				// the current resolution equals the player.ini full screen value?  so we set it as the selected one in the combo box :
				if (ewinfo->g_Height == MainDesc->DisplayModes[i].Height && ewinfo->g_Width == MainDesc->DisplayModes[i].Width )
				{
					stylePage->m_FModes.SetCurSel( stylePage->m_FModes.GetCount()-1 );

				}
				// the current resolution equals the player.ini windowed resolution value ?  so we set it as the selected one in the combo box :
				if (ewinfo->g_HeightW == MainDesc->DisplayModes[i].Height && ewinfo->g_WidthW == MainDesc->DisplayModes[i].Width )
				{
					stylePage->m_windowMode.SetCurSel( stylePage->m_windowMode.GetCount()-1 );
				}

			}
		}
	}
}

BOOL CustomPlayerDialog::OnInitDialog()
{

	
	

	BOOL bResult = CPropertySheet::OnInitDialog();

	// add the preview window to the property sheet.
	CRect rectWnd;
	GetWindowRect(rectWnd);
	SetWindowPos(NULL, 0, 0,		rectWnd.Width() ,		rectWnd.Height(),		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	
	CenterWindow();


	if(GetPlayer().GetPAppStyle()->g_ShowConfigTab)
	{

		
		vtPlayer::Structs::xSEnginePointers* ep = GetPlayer().GetEnginePointers();
		vtPlayer::Structs::xSEngineWindowInfo* ewinfo = GetPlayer().GetEngineWindowInfo();

		if(ewinfo->g_GoFullScreen)
			_UpdateDriverList(ewinfo->g_FullScreenDriver);
		else
			_UpdateDriverList(ewinfo->g_WindowedDriver);


		if(ewinfo->g_GoFullScreen)
		{
			_UpdateResolutionLists(NULL,ewinfo->g_FullScreenDriver);
			
			if (ewinfo->g_FullScreenDriver < stylePage->m_Driver.GetCount() )
			{
				stylePage->m_Driver.SetCurSel( ewinfo->g_FullScreenDriver);
			}

		}
		else{

			_UpdateResolutionLists(NULL,ewinfo->g_WindowedDriver);

			
			if (ewinfo->g_WindowedDriver < stylePage->m_Driver.GetCount() )
			{
				stylePage->m_Driver.SetCurSel( ewinfo->g_WindowedDriver );
			}
			

		}





		m_errorPage.errorText = m_ErrorText;

		stylePage->m_CB_FSSA.AddString("0");
		stylePage->m_CB_FSSA.AddString("1");
		stylePage->m_CB_FSSA.AddString("2");
		stylePage->m_CB_FSSA.AddString("4");
		stylePage->m_CB_FSSA.AddString("8");

		stylePage->m_CB_FSSA.SetCurSel(ewinfo->FSSA/2);
		stylePage->m_CB_FSSA.UpdateData();

		stylePage->m_Bpps.SetCurSel(ewinfo->g_Bpp);
		stylePage->m_FullScreenBtn.SetCheck(ewinfo->g_GoFullScreen);


	}

	if(GetPlayer().GetPAppStyle()->g_ShowAboutTab)
	{

		this->SetActivePage((CPropertyPage*)&m_aboutPage);		

	}

	if ( m_ErrorText.GetLength() )
	{
		this->SetActivePage((CPropertyPage*)&m_errorPage);		
	}

	return bResult;
}
void CustomPlayerDialog::OnApplyNow()
{
	Default();

	vtPlayer::Structs::xSEnginePointers* ep = GetPlayer().GetEnginePointers();
	vtPlayer::Structs::xSEngineWindowInfo* ewinfo = GetPlayer().GetEngineWindowInfo();

	//////////////////////////////////////////////////////////////////////////
	CString bpp;
	stylePage->m_Bpps.GetLBText(stylePage->m_Bpps.GetCurSel(),bpp);
	XString oBpp(bpp);
	ewinfo->g_Bpp = oBpp.ToInt();

	//////////////////////////////////////////////////////////////////////////
	CString rr;
	stylePage->m_RRates.GetLBText(stylePage->m_RRates.GetCurSel(),rr);
	XString oRR(rr);
	ewinfo->g_RefreshRate= oRR.ToInt();
	//////////////////////////////////////////////////////////////////////////
	CString fssa;
	stylePage->m_CB_FSSA.GetLBText(stylePage->m_CB_FSSA.GetCurSel(),fssa);
	XString oFSSA(fssa);
	ewinfo->FSSA= oFSSA.ToInt();

	if (ewinfo->g_GoFullScreen)
		ewinfo->g_FullScreenDriver  = stylePage->m_Driver.GetCurSel();
	else
		ewinfo->g_WindowedDriver  = stylePage->m_Driver.GetCurSel();


	//////////////////////////////////////////////////////////////////////////
	ewinfo->g_GoFullScreen = stylePage->m_FullScreenBtn.GetCheck();

	//////////////////////////////////////////////////////////////////////////
	CString fmode;
	stylePage->m_FModes.GetLBText(stylePage->m_FModes.GetCurSel(),fmode);


	XStringTokenizer	tokizer(fmode, "x");
	const char*tok = NULL;
	tok = tokizer.NextToken(tok);
	XString width(tok);
	tok = tokizer.NextToken(tok);
	XString height(tok);
	ewinfo->g_Width = width.ToInt();
	ewinfo->g_Height = height.ToInt();

	{

		CString wmode;
		stylePage->m_windowMode.GetLBText(stylePage->m_windowMode.GetCurSel(),wmode);
		XStringTokenizer	tokizer(wmode, "x");
		const char*tok = NULL;
		tok = tokizer.NextToken(tok);
		XString width(tok);
		tok = tokizer.NextToken(tok);
		XString height(tok);
		ewinfo->g_WidthW = width.ToInt();
		ewinfo->g_HeightW = height.ToInt();
	}

	GetPlayer().PSaveEngineWindowProperties(CUSTOM_PLAYER_CONFIG_FILE,*GetPlayer().GetEngineWindowInfo());

}


