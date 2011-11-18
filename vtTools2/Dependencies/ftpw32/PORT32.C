/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                     */
/*                                                                     */
/*    Fichier de portage   Applis 16 bits -->  Applis 32 bits          */
/*                                                                     */
/*                                                                     */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Version 1.0 ------------------------------------------------------- */
/*             25.11.95   Creation du source pour portage 32 bits      */


#include <windows.h>
#include <windowsx.h>
#include "port32.h"

/* --------------------------------- */
/* 32 bits code                      */
/* --------------------------------- */      

#ifdef _WIN32
HINSTANCE GetTaskInstance (HWND hParentWnd)
{
return (HINSTANCE) GetWindowLong (hParentWnd,GWL_HINSTANCE); 
} /* GetTaskInstance */


LPSTR GetTempDrive(int nDrive)
{
static char szTempPath[144];
   GetTempPath (sizeof szTempPath, szTempPath);
return (LPSTR) szTempPath;
}
#endif
      

/* --------------------------------- */
/* 16 bits code                      */
/* --------------------------------- */      
#ifndef _WIN32
HINSTANCE GetTaskInstance (HWND hParentWnd)
{
TASKENTRY          TaskEntry;
  if (hParentWnd==NULL)
    {
       TaskEntry.dwSize = sizeof TaskEntry;
       TaskFindHandle ( & TaskEntry, GetCurrentTask() );
       return TaskEntry.hInst;
    }
   else 
       return (HINSTANCE) GetWindowWord (hParentWnd,GWW_HINSTANCE);
} /* GetTaskInstance */
#endif /* 16 bits code */


   
