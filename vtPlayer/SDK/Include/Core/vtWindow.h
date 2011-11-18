#ifndef __VTWINDOW_H_
#define __VTWINDOW_H_
#include <Windows.h>
#include "BaseMacros.h"
#include <BaseTyps.h>


class CCustomPlayer;

class vtWindow
{
public:

	vtWindow();
	int CreateAsChild(long *parent);
	int Run();
	int Init();
	void ParseCommandLine(const char*cmdLine);
	void Destroy();
	int Tick();
	int DoFrame();
	
	int Show(int ShowFlags);
	int GetWidth();
	int GetHeight();
	int UpdateRenderSize(int w,int h);


	//send a message to a certain object given by name or when not specified as broadcast
	int SendMessage(char *targetObject,char *message,int id0,int id1,int id2,int value);
	int SendMessage(char *targetObject,char *message,int id0,int id1,int id2,float value);
	int SendMessage(char *targetObject,char *message,int id0,int id1,int id2,char* value);

	//pseudo message sending to csharp : 
	int HasMessages();
	int GetNumMessages();
	char *GetMessageName(int messageID);
	int GetNumParameters(int messageID);
	int GetMessageParameterType(int messageID,int parameterSubID);
	
	int GetMessageValueInt(int messageID,int parameterSubID);
	float GetMessageValueFloat(int messageID,int parameterSubID);
	char* GetMessageValueStr(int messageID,int parameterSubID);

	void CleanMessages();
	void DeleteMessage(int messageID);

	void Pause();
	void Play();
	void LoadCompostion(char *file);
	void Terminate(int flags);
	int WndProc( long *hWnd, int uMsg, long * wParam, long* lParam );
    
protected:

	CCustomPlayer *m_Player;
	CONSOLE_SCREEN_BUFFER_INFO m_SavedConsoleInfo;


private:
};
////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////

#endif