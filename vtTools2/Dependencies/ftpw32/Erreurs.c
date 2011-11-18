/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*                                                                           */
/*     Ftp4w Version 3.01                                                     */
/*     FtpCheckError                                                         */
/*                                                                           */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  
#include  <windows.h>
#include  <string.h>
#include  <stdio.h>

#include "port32.h"    /* 16/32 bits           */
#include "ftp4w.h"
#include "ftp4w_in.h"



extern LPProcData pFirstProcData;

/* ******************************************************************* */
/*                                                                     */
/* Partie IX : C o p y r i g h t                                       */
/*                                                                     */
/* ******************************************************************* */

/* #define CRC  1 		     */
#define CRC_VALUE 51210
static int  nVER= 0x0301;
static char szVER [] = 
"@(#)------------------------------------------------------------------------\n\
FTP for windows Version 3.01 by Ph. Jounin (ph.jounin@computer.org)          \n\
-----------------------------------------------------------------------------\n";


int _export PASCAL FAR Ftp4wVer (LPSTR szVerStr, int nStrSize)
{
LPSTR p;
   if (szVerStr!=NULL  && nStrSize!=0)
    {
      lstrcpyn (szVerStr, strchr (szVER, '\n')+1, nStrSize);
      if ( (p=strchr (szVerStr, '\n'))!=NULL)  *p=0;
    }
return nVER;    
} /* Ftp4wVer */


int Protection ()
{
static char    sz2[] = "This is an illegal copy of FTP4W.DLL\nPlease contact the author at ph.jounin@computer.org\nin order to have an original version.";
unsigned       Ark, Evan;
unsigned short cRc;
  for (Ark=0, Evan= lstrlen(szVER) / sizeof cRc, cRc=216 ; Ark<Evan ; Ark++)
     cRc +=  ((unsigned short *) szVER ) [Ark] + szVER[Ark];
  for (Ark=0, Evan=lstrlen(sz2) / sizeof cRc             ; Ark<Evan ; Ark++)
     cRc +=  ((unsigned short *) sz2) [Ark] - Ark;
  if (cRc != CRC_VALUE)
    { 
      * (strrchr (szVER, ')')+1)=0;
      MessageBox (FtpDataPtr ()->hFtpWnd, sz2, strchr (szVER, 'F'), MB_OK);
    }
#ifdef CRC
    { 
    char szTmp [256];
      wsprintf (szTmp,"cRc Value %u", (unsigned) cRc);
      MessageBox (FtpDataPtr ()->hFtpWnd, szTmp,  strchr (szVER, 'F'), MB_OK);
    }
#endif
return FALSE;
} /* Protection */
  

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/* I *do* have a suggestion for you.  I wrote the following function (it     */
/* does not cover all errors) that I thought you might want to add to        */
/* your dll file.  Pardon the tab spacing...  I call this function after     */
/* any ftp4w function to check for errors and report them.  I pass a handle  */
/* to a parent window (for creation of message box, could use 0 for desktop) */
/* and then I pass the return code and the name of the function that I just  */
/* called.  Now all of this *could* be stored internal (except for the parent*/
/* window handle) to the .dll file... Have a global variable that stores     */
/* the last return code as well as the last function called so when          */
/* ftpcheckerror is called, it doesn't need any parameters.. Although you    */
/* would definitely want to allow a char* string to be passed so the end     */
/* programmer can pass something to it like a line # in the c file for       */
/* ease of debugging.  Anyways, here goes:                                   */
/*                                                                           */
/* Michael Douglass                                                          */
/* Texas Networking, Inc.                                                    */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define SizeOfTab(t)   ( sizeof (t) / sizeof (t[0]) )

struct S_Ftp4wErr
{
   int   Ftp4wErrCode;
   LPSTR Ftp4wErrString;
}; /* struct S_Ftp4wErr */

struct S_Ftp4wErr sFtp4wErr [] =
{
   { FTPERR_OK,                  "Successful function"                    },
   { FTPERR_ENTERPASSWORD,       "Enter a password"                       },
   { FTPERR_ENTERACCOUNT,        "Enter account"                          },
   { FTPERR_ACCOUNTNEEDED,       "Enter account"                          },
   { FTPERR_RESTARTOK,           "Restart command successful"             },
   { FTPERR_CANCELBYUSER,        "Transfer aborted by FtpAbort"           },
   { FTPERR_ENDOFDATA,           "Server has closed the data connection"  },
   { FTPERR_INVALIDPARAMETER,    "Error in parameters"                    },
   { FTPERR_SESSIONUSED,         "User has already a FTP session"         },
   { FTPERR_NOTINITIALIZED,      "FtpInit has not been called"            },
   { FTPERR_NOTCONNECTED,        "User is not connected to a server"      },
   { FTPERR_CANTOPENFILE,        "Can not open specified file"            },
   { FTPERR_CANTOPENLOCALFILE,   "Can not open specified file"            },
   { FTPERR_CANTWRITE,           "Can not write into file (disk full?)"   },
   { FTPERR_NOACTIVESESSION,     "FtpRelease without FtpInit"             },
   { FTPERR_STILLCONNECTED,      "FtpRelease without any Close"           },
   { FTPERR_SERVERCANTEXECUTE,   "File action not taken"                  },
   { FTPERR_LOGINREFUSED,        "Server rejects userid/passwd"           },
   { FTPERR_NOREMOTEFILE,        "Server can not open remote file"        },
   { FTPERR_TRANSFERREFUSED,     "Host refused the transfer"              },
   { FTPERR_WINSOCKNOTUSABLE,    "A winsock.DLL ver 1.1 is required"      },
   { FTPERR_CANTCLOSE,           "Close failed (cmd is in progress)"      },
   { FTPERR_FILELOCKED,          "Temporary error during FtpDelete"       },
   { FTPERR_FWLOGINREFUSED,      "Firewall rejects userid/passwd"         },
   { FTPERR_ASYNCMODE,           "Function available only in synchronous mode" },
   { FTPERR_UNKNOWNHOST,         "Can not resolve host adress"            },
   { FTPERR_NOREPLY,             "Host does not send an answer"           },
   { FTPERR_CANTCONNECT,         "Error during connection"                },
   { FTPERR_CONNECTREJECTED,     "Host has no FTP server"                 },
   { FTPERR_SENDREFUSED,         "Can not send data (network down)"       },
   { FTPERR_DATACONNECTION,      "Connection on data-port failed"         },
   { FTPERR_TIMEOUT,             "Timeout occured"                        },
   { FTPERR_FWCANTCONNECT,       "Error during connection with Firewall " },
   { FTPERR_FWCONNECTREJECTED,   "Firewall has no FTP server"             },
   { FTPERR_UNEXPECTEDANSWER,    "Answer was not expected"                },
   { FTPERR_CANNOTCHANGETYPE,    "Host rejects the TYPE command"          },
   { FTPERR_CMDNOTIMPLEMENTED,   "Command not implemented"                },
   { FTPERR_PWDBADFMT,           "PWD cmd OK, but answer has no directory"},
   { FTPERR_PASVCMDNOTIMPL,      "Server don't support passive mode"      },
   { FTPERR_CANTCREATEWINDOW,    "Insuffisent free resources"             },
   { FTPERR_INSMEMORY,           "Insuffisent Heap memory"                },
   { FTPERR_CANTCREATESOCKET,    "No socket available"                    },
   { FTPERR_CANTBINDSOCKET,      "Bind is not succesful"                  },
   { FTPERR_SYSTUNKNOWN,         "Host system not in the list"            },
}; /* sFtp4wErr */




LPSTR _export PASCAL FAR FtpErrorString (int Rc)
{
int Idx;
   for ( Idx=0 ;  
         Idx<SizeOfTab(sFtp4wErr) && sFtp4wErr[Idx].Ftp4wErrCode!=Rc;  
         Idx++ );
return Idx>= SizeOfTab(sFtp4wErr) ?   (LPSTR) "Not an Ftp4w return code" :
                                      sFtp4wErr[Idx].Ftp4wErrString;
} /* FtpErrorString */


#include <stdarg.h>
void _cdecl Ftp4w_Trace (const char *szFile, const char *fmt, ...)
{
HFILE hFile;
va_list marker;
char szBuf[512];

    va_start( marker, fmt );
	hFile = _lopen (szFile, OF_WRITE);

	sprintf (szBuf, "%ld: Proc %d\t", GetTickCount(), GetCurrentThreadId ());
	vsprintf (szBuf, fmt, marker);
	strcat (szBuf,  "\r\n");
	OutputDebugString (szBuf);
} /* Ftp4w_Trace */




