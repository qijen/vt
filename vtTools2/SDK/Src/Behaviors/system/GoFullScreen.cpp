#include "pch.h"

#include "CKAll.h"

CKObjectDeclaration	*FillBehaviorGoFullScreenDecl();
CKERROR CreateGoFullScreenProto(CKBehaviorPrototype **pproto);
int GoFullScreen(const CKBehaviorContext& behcontext);


/************************************************************************/
/*																																			  */
/************************************************************************/

CKObjectDeclaration	*FillBehaviorGoFullScreenDecl(){
	CKObjectDeclaration *od = CreateCKObjectDeclaration("GoFullScreen");	
	
	od->SetCategory("Narratives/System");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x126046,0x718a4147));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("mw");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateGoFullScreenProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}
CKERROR CreateGoFullScreenProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("GoFullScreen");
	if(!proto) return CKERR_OUTOFMEMORY;
	proto->DeclareInput("Go");
	proto->DeclareOutput("Out");
    	
	proto->DeclareInParameter("driver index",CKPGUID_INT);
	proto->DeclareInParameter("width",CKPGUID_INT);
	proto->DeclareInParameter("height",CKPGUID_INT);
	proto->DeclareInParameter("bpp",CKPGUID_INT);
	proto->DeclareInParameter("refreshrate",CKPGUID_INT);
	
	proto->DeclareInParameter("SwitchToFS",CKPGUID_BOOL);
	
	proto->DeclareInParameter("mode_index",CKPGUID_INT);

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);

	proto->SetFunction(GoFullScreen);
	*pproto = proto;
	return CK_OK;
}

#include <windows.h>

 BOOL CALLBACK EnumProcessListFunc( HWND, LPARAM );
 BOOL CALLBACK
	 EnumProcessListFunc( HWND hWnd, LPARAM lParam )
 {
	 static DWORD dwCurrentProcessId;
	 static BOOL  fFirstTime = TRUE;
	 static LONG  MaxStrLen  = 0;
	 static TCHAR TextBuffer[256];

	 if( fFirstTime ) {
		 fFirstTime = FALSE;
		 dwCurrentProcessId = GetCurrentProcessId();
	 }

	 if( hWnd ) {
		 GetWindowText( hWnd, (LPTSTR) &TextBuffer, sizeof(TextBuffer)/sizeof(TCHAR) );
		 if( *TextBuffer ) {
			 DWORD  dwProcessId;

			 /*GetWindowThreadProcessId( hWnd, &dwProcessId );
			 if( dwProcessId != dwCurrentProcessId ) {
				 LRESULT Index;
				 HWND hWndListBox = (HWND) lParam;

				 //Index = ListBoxInsert( hWndListBox, &MaxStrLen, TextBuffer );
				 SendMessage( hWndListBox, LB_SETITEMDATA, (WPARAM) Index,
					 (LPARAM) dwProcessId );
			 }*/
		 }
	 }

	 return( TRUE );
 }

 BOOL CALLBACK EnumChildProc(HWND hwnd,LPARAM lParam)
 {

	 if( hwnd ) 
	 {
		  static TCHAR TextBuffer[256];
		 GetWindowText( hwnd, (LPTSTR) &TextBuffer, sizeof(TextBuffer)/sizeof(TCHAR) );
		 if( *TextBuffer ) 
		 {
			 return true;

		 }
	 }else
		 return FALSE;

 }
int GoFullScreen(const CKBehaviorContext& behcontext)
{

	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;


	CKPluginManager* ThePluginManager=CKGetPluginManager();
	CKRenderManager *rm = (CKRenderManager *)ctx->GetRenderManager();

	CKRenderContext *rctx = rm->GetRenderContext(0);
	int rcount  = rm->GetRenderContextCount();

	int DriverIndex=0;
	beh->GetInputParameterValue(0, &DriverIndex);

	int width = 0;
	beh->GetInputParameterValue(1, &width);
	int height= 0;
	beh->GetInputParameterValue(2, &height);
	int bpp = 0;
	beh->GetInputParameterValue(3, &bpp);
	int rr = 0;
	beh->GetInputParameterValue(4, &rr);
	bool gOFS = false;
	beh->GetInputParameterValue(5, &gOFS);

	int mode = 0;
	beh->GetInputParameterValue(6, &mode);





	HWND m_hWndParent = (HWND)ctx->GetMainWindow();
	//turn off

	if(rctx->IsFullScreen() && !gOFS ){
		ctx->Pause();
		rctx->StopFullScreen();
		//ShowWindow(m_hWndParent ,SW_RESTORE);
		
		RECT m_hWndParent_rect ;

		GetWindowRect(m_hWndParent,&m_hWndParent_rect);

		/*
		if (m_hWndParent_rect.bottom!=0 && !rctx->IsFullScreen() )  
		{
			//allow the window to be resized
			LONG st = GetWindowLong(m_hWndParent,GWL_STYLE);
			st|=WS_THICKFRAME;
			SetWindowLong(m_hWndParent,GWL_STYLE,st);
			
			int Xpos=(GetSystemMetrics(SM_CXSCREEN)-width)/2;
			int Ypos=(GetSystemMetrics(SM_CYSCREEN)-height)/2;

			
			//reposition the window
			::SetWindowPos(m_hWndParent,HWND_NOTOPMOST,Xpos,Ypos,m_hWndParent_rect.right - m_hWndParent_rect.left,m_hWndParent_rect.bottom - m_hWndParent_rect.top,NULL);
		}
*/


		//////////////////////////////////////////////////////////////////////////
		//restore the main window size (only in DirectX rasterizer->m_MainWindowRect.bottom not modified)
		if (m_hWndParent_rect.bottom!=0 && !rctx->IsFullScreen()) {
			//allow the window to be resized
			LONG st = GetWindowLong(m_hWndParent,GWL_STYLE);
			st|=WS_THICKFRAME;
			st&=~WS_SIZEBOX;
			SetWindowLong(m_hWndParent,GWL_STYLE,st);			
		}

		//reposition the window
		m_hWndParent_rect.left = (GetSystemMetrics(SM_CXSCREEN)-width)/2;
		m_hWndParent_rect.right = width+m_hWndParent_rect.left;
		m_hWndParent_rect.top = (GetSystemMetrics(SM_CYSCREEN)-height)/2;
		m_hWndParent_rect.bottom = height+m_hWndParent_rect.top;
		BOOL ret = AdjustWindowRect(&m_hWndParent_rect,WS_OVERLAPPEDWINDOW & ~(WS_SYSMENU|WS_SIZEBOX|WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_SIZEBOX),FALSE);
		::SetWindowPos(m_hWndParent,HWND_NOTOPMOST,m_hWndParent_rect.left,m_hWndParent_rect.top,m_hWndParent_rect.right - m_hWndParent_rect.left,m_hWndParent_rect.bottom - m_hWndParent_rect.top,NULL);

		// now we can show the main widnwos
		ShowWindow(m_hWndParent,SW_SHOW);
		SetFocus(m_hWndParent);

		rctx->Resize(0,0,width,height);


		HWND rWin = GetWindow(m_hWndParent,GW_CHILD);
		if( rWin ) 
		{
			//static TCHAR TextBuffer[256];
			//GetWindowText( rWin, (LPTSTR) &TextBuffer, sizeof(TextBuffer)/sizeof(TCHAR) );
			::SetWindowPos(rWin,NULL,0,0,width,height,SWP_NOMOVE|SWP_NOZORDER);
			//rctx->Resize(0,0,m_WindowedWidth,m_WindowedHeight);
			//MessageBox(NULL,TextBuffer,"",NULL);

		}
		//EnumChildWindows(m_hWndParent, (WNDENUMPROC) EnumProcessListFunc,NULL );
		
		// and set the position of the render window in the main window
		//::SetWindowPos(m_RenderWindow,NULL,0,0,m_WindowedWidth,m_WindowedHeight,SWP_NOMOVE|SWP_NOZORDER);

		// and give the focus to the render window
		SetFocus(m_hWndParent);

		
		ctx->Play();
		beh->ActivateOutput(0);

		return CKBR_OK;

	}
	if(!rctx->IsFullScreen() && gOFS ) {

		ctx->Pause();
		int w,h,FSdriver;		
		if (mode<0)
			rctx->GoFullScreen(w=width,h=height,bpp,FSdriver=DriverIndex,rr);
		else {
			VxDriverDesc *MainDesc=rm->GetRenderDriverDescription(DriverIndex);
			if (MainDesc)
				rctx->GoFullScreen(w=MainDesc->DisplayModes[mode].Width,
				h=MainDesc->DisplayModes[mode].Height,
				MainDesc->DisplayModes[mode].Bpp,
				FSdriver=DriverIndex,rr);
		}	
		ctx->Play();

		VxDriverDesc* Check_API_Desc=rm->GetRenderDriverDescription(DriverIndex);

		if (Check_API_Desc->Caps2D.Family==CKRST_DIRECTX && rctx->IsFullScreen()) {
			//store current size
			RECT g_mainwin_rect;

			GetWindowRect(m_hWndParent,&g_mainwin_rect);

			//Resize the window
			::SetWindowPos(m_hWndParent,HWND_TOPMOST,0,0,w,h,NULL);

			//Prevent the window from beeing resized
			LONG st = GetWindowLong(m_hWndParent,GWL_STYLE);
			st&=(~WS_THICKFRAME);
			SetWindowLong(m_hWndParent,GWL_STYLE,st);
			
			
			



		}







		//ShowWindow(m_hWndParent,SW_SHOW|SW_MAXIMIZE);

		beh->ActivateOutput(0);
	}
	return CKBR_OK;
}


