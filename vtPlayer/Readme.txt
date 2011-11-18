

FAQ : 


Where to set fade in/out time, splash bitmap, bitmap trancparency, text position for loading text, or the font type  ? 

 -> go in the project xSplash -> xSplash.cpp -> function CreateSplashEx(...)
 
 : 

#define CSS_FADEIN		0x0001
#define CSS_FADEOUT		0x0002
#define CSS_FADE		CSS_FADEIN | CSS_FADEOUT
#define CSS_SHADOW		0x0004
#define CSS_CENTERSCREEN	0x0008
#define CSS_CENTERAPP		0x0010
#define CSS_HIDEONCLICK		0x0020

#define CSS_TEXT_NORMAL		0x0000
#define CSS_TEXT_BOLD		0x0001
#define CSS_TEXT_ITALIC		0x0002
#define CSS_TEXT_UNDERLINE	0x0004
   

Where to change the "Load ...90%" Text ? 

->CustomPlayer.cpp -> LoadCallback() (last function)

  if (loaddata.NbObjetsLoaded % 10  == 0) : means, update text each 10th object !
  

Where to change the about tab text ?

  -     goto \res\about.txt !
        you can use mailto:playgen@playgen.com or www.playgen.com for links.


Where to change global settings ? 

in CustomPlayerDefines.h ! : 

#define CPR_CHECK_DIRECTX	1		//	this leads to a directx check, is using xUtils.dll
#define CPR_MINIMUM_DIRECTX_VERSION	8		// this is our minimum version, always write at least 3 letters like 10.0
#define CPR_MINIMUM_DIRECTX_VERSION_FAIL_URL	"www.microsoft.com"
#define CPR_OFFER_INTERNAL_DIRECTX_INSTALLER 0		// not implemented 

#define CPR_CHECK_RAM	1	//performes a memory check, is using xUtils.dll and it needs a directx !!!
#define CPR_MINIMUM_RAM	256	//our minimum pRam
#define CPR_MINIMUM_RAM_FAIL_ABORT 0	// this leads to a application exit !


#define CP_SUPPORT_EMAIL "support@playgen.com" // 

//  [12/16/2007 mc007]	//////////////////////////////////////////////////////////////////////////
//
//	Application Features : 
#define CPF_HAS_SPLASH	1	//	displays a splash, is using xsplash.dll ! it also adds a loading callback
#define CPF_SPLASH_FILE	"splash.bmp" //not used !
#define CPF_SPLASH_TEXT_TYPE	"MicrogrammaDBolExt" 
#define CPF_SPLASH_TEXT_FORMAT (DT_SINGLELINE | DT_RIGHT | DT_BOTTOM)   


//  [12/17/2007 mc007]
#define CPA_SHOW_ABOUT_TAB 1	//adds an about tab, is using about.txt from \project source folder \res
#define CPA_SHOW_LICENSE_TAB 0	//adds an about tab, is using license.txt from \project source folder \res
#define CPA_SHOW_ERROR_TAB 1
#define CPA_ABORT_ON_ERROR 1	// aborts when the requirements are failing


        
