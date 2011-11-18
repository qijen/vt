#include "CPStdAfx.h"
//#include "CustomPlayer.h"
#include "vtWindow.h"


vtWindow *myWin=NULL;

int _tmain(int argc, _TCHAR* argv[])
{

	myWin  = new vtWindow();
	if (myWin->Init())
	{
//		CKInitCustomPlayer(false);
		CKStartUp();

		//		myWin->getPlayer()->m_hInstance
		myWin->DoFrame();
		myWin->Destroy();
	}

}
