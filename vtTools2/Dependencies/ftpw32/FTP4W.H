
/* *************************************************************** */
/*                                                                 */
/*                                                                 */
/*                                                                 */
/*    FTP4W.DLL  (Version 3.0)                                     */
/*                                                                 */
/*                                                                 */
/*                                   By Ph. Jounin (SNCF 30.12.98) */
/*                                     Internet jounin-ph@magic.fr */
/*                                            Copyright SNCF 94-97 */
/* *************************************************************** */


#ifndef _FTP4W_API_

#include <winsock.h>

#ifdef __cplusplus  
extern "C" {            /* Assume C declarations for C++ */   
#endif  /* __cplusplus */   

#define  FTP_DATABUFFER  4096 /* a good value for X25/Ethernet/Token Ring */



/* ----------------------------------------------------------- */

/* ASCII or binary transfer */
#define   TYPE_A        'A'
#define   TYPE_I        'I'
#define   TYPE_L8       'L'
#define   TYPE_DEFAULT   0
                          
/* actions requested by user */
#define   FTP4W_STORE_ON_SERVER   65
#define   FTP4W_APPEND_ON_SERVER  87
#define   FTP4W_GET_FROM_SERVER   223

/* Firewall Types */
#define   FTP4W_FWSITE             100
#define   FTP4W_FWPROXY            103
#define   FTP4W_FWUSERWITHLOGON    106
#define   FTP4W_FWUSERNOLOGON      109

                          
/* ----------------------------------------------------------- */
/*              Return codes of FTP functions                  */
/* ----------------------------------------------------------- */
/* success */
#define  FTPERR_OK                    0 /* succesful function                */
/* OK but waits for further parameters */
#define  FTPERR_ENTERPASSWORD         1 /* userid need a password            */
#define  FTPERR_ENTERACCOUNT          2 /* user/pass OK but account required */
#define  FTPERR_ACCOUNTNEEDED         2 /* user/pass OK but account required */
#define  FTPERR_RESTARTOK             3 /* Restart command successful        */
#define  FTPERR_ENDOFDATA             4  /* server has closed the data-conn   */
#define  FTPERR_CANCELBYUSER         -1 /* Transfer aborted by user FtpAbort */
/* user's or programmer's Errors  */
#define  FTPERR_INVALIDPARAMETER   1000 /* Error in parameters               */
#define  FTPERR_SESSIONUSED        1001 /* User has already a FTP session    */
#define  FTPERR_NOTINITIALIZED     1002 /* FtpInit has not been call         */
#define  FTPERR_NOTCONNECTED       1003 /* User is not connected to a server */
#define  FTPERR_CANTOPENFILE       1004 /* can not open specified file       */
#define  FTPERR_CANTOPENLOCALFILE  FTPERR_CANTOPENFILE
#define  FTPERR_CANTWRITE          1005 /* can't write into file (disk full?)*/
#define  FTPERR_NOACTIVESESSION    1006 /* FtpRelease without FtpInit        */
#define  FTPERR_STILLCONNECTED     1007 /* FtpRelease without any Close      */
#define  FTPERR_SERVERCANTEXECUTE  1008 /* file action not taken             */
#define  FTPERR_LOGINREFUSED       1009 /* Server rejects usrid/passwd       */
#define  FTPERR_NOREMOTEFILE       1010 /* server can not open file          */
#define  FTPERR_TRANSFERREFUSED    1011 /* Host refused the transfer         */
#define  FTPERR_WINSOCKNOTUSABLE   1012 /* A winsock.DLL ver 1.1 is required */
#define  FTPERR_CANTCLOSE          1013 /* Close failed (cmd is in progress) */
#define  FTPERR_FILELOCKED         1014 /* temporary error during FtpDelete  */
#define  FTPERR_FWLOGINREFUSED     1015 /* Firewallrejects usrid/passwd      */
#define  FTPERR_ASYNCMODE          1016 /* FtpMGet only in synchronous mode  */
/* TCP errors */
#define  FTPERR_UNKNOWNHOST        2001 /* can not resolve host adress       */
#define  FTPERR_NOREPLY            2002 /* host does not send an answer      */
#define  FTPERR_CANTCONNECT        2003 /* Error during connection           */
#define  FTPERR_CONNECTREJECTED    2004 /* host has no FTP server            */
#define  FTPERR_SENDREFUSED        2005 /* can't send data (network down)    */
#define  FTPERR_DATACONNECTION     2006 /* connection on data-port failed    */
#define  FTPERR_TIMEOUT            2007 /* timeout occured                   */
#define  FTPERR_FWCANTCONNECT      2008 /* Error during connection with FW   */
#define  FTPERR_FWCONNECTREJECTED  2009 /* Firewall has no FTP server        */
/* FTP server errors */
#define  FTPERR_UNEXPECTEDANSWER   3001  /* answer was not expected          */
#define  FTPERR_CANNOTCHANGETYPE   3002  /* host rejects the TYPE command    */
#define  FTPERR_CMDNOTIMPLEMENTED  3003  /* host recognize but can't exec cmd*/
#define  FTPERR_PWDBADFMT          3004  /* PWD cmd OK, but answer has no "  */
#define  FTPERR_PASVCMDNOTIMPL     3005  /* Server don't support passive mode*/
/* Resource errors */
#define  FTPERR_CANTCREATEWINDOW   5002  /* Insufficent free resources       */
#define  FTPERR_INSMEMORY          5003  /* Insuffisent Heap memory          */
#define  FTPERR_CANTCREATESOCKET   5004  /* no more socket                   */
#define  FTPERR_CANTBINDSOCKET     5005  /* bind is not succesful            */
#define  FTPERR_SYSTUNKNOWN        5006  /* host system not in the list      */



/* **************************************************************** */
/*                                                                  */
/*                    P R O T O T Y P E S                           */
/*                                                                  */
/* **************************************************************** */

LPSTR PASCAL FAR  FtpBufferPtr (void);
LPSTR PASCAL FAR  FtpErrorString (int Rc);
int PASCAL FAR WEP (int nType);
int PASCAL FAR Ftp4wVer (LPSTR szVerStr, int nStrSize);

/* change default parameters */
int  PASCAL FAR FtpSetVerboseMode (BOOL bVerboseMode, HWND hVerboseWnd, UINT wMsg);
long PASCAL FAR FtpBytesTransferred (void);
long PASCAL FAR FtpBytesToBeTransferred(void);
void PASCAL FAR FtpSetDefaultTimeOut (int nTo_in_sec);
void PASCAL FAR FtpSetDefaultPort(int nDefPort);
void PASCAL FAR FtpSetAsynchronousMode(void);
void PASCAL FAR FtpSetSynchronousMode(void);
BOOL PASCAL FAR FtpIsAsynchronousMode(void);   
void PASCAL FAR FtpSetNewDelay(int x); 
void PASCAL FAR FtpSetNewSlices(int x,int y) ;
void PASCAL FAR FtpSetPassiveMode (BOOL bPassive);
void PASCAL FAR FtpLogTo (HFILE hLogFile);
/* mispelled functions - only for backwards compatibilty */
long PASCAL FAR FtpBytesTransfered (void);
long PASCAL FAR FtpBytesToBeTransfered(void);

/* Init functions */
int PASCAL FAR FtpRelease (void);
int PASCAL FAR FtpInit (HWND hParentWnd);
int PASCAL FAR FtpMtInit ( HWND hParentWnd, DWORD (FAR *f)(void) );
int PASCAL FAR FtpFlush (void);

/* Connection */
int PASCAL FAR FtpLogin (LPCSTR szHost, LPCSTR szUser, LPCSTR szPasswd,
                         HWND hParentWnd, UINT wMsg);
int PASCAL FAR FtpOpenConnection (LPCSTR szHost);
int PASCAL FAR FtpCloseConnection (void);
int PASCAL FAR FtpLocalClose (void);

/* authentification */
int PASCAL FAR  FtpSendUserName (LPCSTR szUserName);
int PASCAL FAR  FtpSendPasswd (LPCSTR szPasswd);
int PASCAL FAR  FtpSendAccount (LPCSTR szAccount);

/* commands */
int PASCAL FAR FtpHelp   (LPCSTR szArg, LPSTR szBuf, UINT uBufSize);
int PASCAL FAR FtpDeleteFile (LPCSTR szRemoteFile);
int PASCAL FAR FtpRenameFile (LPCSTR szFrom, LPCSTR szTo);
int PASCAL FAR FtpQuote  (LPCSTR szCmd, LPSTR szReplyBuf, UINT uBufSize);
int PASCAL FAR FtpSyst   (LPCSTR FAR *szSystemStr);
int PASCAL FAR FtpSetType(char cType);
int PASCAL FAR FtpCWD    (LPCSTR szPath);
int PASCAL FAR FtpCDUP   (void);
int PASCAL FAR FtpPWD    (LPSTR szBuf, UINT uBufSize);
int PASCAL FAR FtpMKD    (LPCSTR szPath, LPSTR szFullDir, UINT uBufSize);
int PASCAL FAR FtpRMD    (LPCSTR szPath);

/* file transfer */
int PASCAL FAR FtpAbort (void);
int PASCAL FAR FtpSendFile (LPCSTR szLocal, LPCSTR szRemote, char cType, 
                            BOOL bNotify, HWND hParentWnd, UINT wMsg);
int PASCAL FAR FtpAppendToRemoteFile (LPCSTR szLocal, LPCSTR szRemote,char cType,
                            BOOL bNotify, HWND hParentWnd, UINT wMsg);
int PASCAL FAR FtpRecvFile (LPCSTR szRemote, LPCSTR szLocal, char cType, 
                            BOOL bNotify, HWND hParentWnd, UINT wMsg);
int PASCAL FAR FtpAppendToLocalFile (LPCSTR szLocal, LPCSTR szRemote,char cType,
                            BOOL bNotify, HWND hParentWnd, UINT wMsg);
DWORD PASCAL FAR FtpGetFileSize (void);
int PASCAL FAR FtpMGet (LPCSTR szFilter, char cType, BOOL bNotify,
                 BOOL (CALLBACK *f) (LPCSTR szRemFile, LPCSTR szLocalFile, int Rc) );

int PASCAL FAR FtpRestart (long lByteCount);
int PASCAL FAR FtpRestartSendFile (HFILE hLocal, LPCSTR szRemote, char cType, 
                            BOOL bNotify, long lByteCount,
                            HWND hParentWnd, UINT wMsg);
int PASCAL FAR FtpRestartRecvFile (LPCSTR szRemote, HFILE hLocal, char cType, 
                            BOOL bNotify, long lByteCount,
                            HWND hParentWnd, UINT wMsg);


/* Directory */
int PASCAL FAR FtpDir (LPCSTR szDef, LPCSTR szLocalFile,
                       BOOL  bLongDir, HWND  hParentWnd, UINT wMsg);

/* Advanced function */
int PASCAL FAR FtpOpenDataConnection (LPCSTR szRemote,int nAction,char cType);
int PASCAL FAR FtpRecvThroughDataConnection (LPSTR szBuf,
                                            unsigned int far *lpBufSize);
int PASCAL FAR FtpSendThroughDataConnection(LPCSTR szBuf,unsigned int uBufSize);
int PASCAL FAR FtpCloseDataConnection (void);

/* Firewall function: Not Available */
int PASCAL FAR FtpFirewallLogin(LPCSTR szFWHost, LPCSTR szFWUser, LPCSTR szFWPass,
                                LPCSTR szRemHost,LPCSTR szRemUser,LPCSTR szRemPass,
                                int nFirewallType,
                                HWND hParentWnd, UINT wMsg);
/* hidden exported functions */
int PASCAL FAR IntFtpGetAnswerCode ();
int PASCAL FAR FtpAutomate (int nIdx, LPCSTR szParam);


/* _______________________________________________________________ */

#ifdef __cplusplus     
}  /* End of extern "C" */   
#endif /* ifdef __cplusplus */  

#define _FTP4W_API_ loaded
#endif /* ifndef FTP4W_API */

