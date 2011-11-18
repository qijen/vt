
/* ----------------------------------- */
/* 16 bits declarations                */
/* ----------------------------------- */
#ifndef _WIN32
#  include <toolhelp.h>
#  define PostMessage(hWnd,wMsg,wParam,lParam)  while (!PostMessage (hWnd,wMsg,wParam,lParam))  Yield ();
#  define GetCurrentThreadId    GetCurrentTask
#  define Calloc(n,s)  (void far *)GlobalAllocPtr(GMEM_SHARE | GMEM_ZEROINIT,n*s)
#  define Free(p)                 GlobalFreePtr (p)
#  define THREADID    HTASK
/*
#  define OF_WRITE       WRITE
#  define OF_READ        READ 
  */
#endif

/* ----------------------------------- */
/* 32 bits redeclarations              */
/* ----------------------------------- */
#ifdef _WIN32
#  define IsTask(x)   (   GetThreadPriority(x)!= THREAD_PRIORITY_ERROR_RETURN \
                       || GetLastError() != ERROR_INVALID_HANDLE)
                    
#  define _export
#  define unlink _unlink
#  define Calloc(n,s)    calloc (n,s)
#  define Free(p)        free (p)         
#  define THREADID		  DWORD
LPSTR GetTempDrive(int nDrive);
#endif


/* ----------------------------------- */
/* functions defined for compatibility */
/* ----------------------------------- */

HINSTANCE GetTaskInstance (HWND hParentWindow);
