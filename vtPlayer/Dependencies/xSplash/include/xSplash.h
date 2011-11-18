#ifndef __X_SPLASH_H_
#define __X_SPLASH_H_

#include <BaseMacros.h>

class CSplashScreenEx;

namespace xSplash
{
	

	MODULE_API CSplashScreenEx *GetSplash();
	MODULE_API void CreateSplash(HINSTANCE hinst,int w,int h);
	MODULE_API void CreateSplashEx(CWnd *parent,int w,int h);
	MODULE_API void HideSplash();
	MODULE_API void ShowSplash();
	MODULE_API void SetText(const char* text);

}//end of namespace xSplash

#endif