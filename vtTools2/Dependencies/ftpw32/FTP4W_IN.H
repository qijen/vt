
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                     */
/*                                                                     */
/*          D L L    F T P                                             */
/*                                                                     */
/*                       W I N D O W S                                 */
/*                                                                     */
/*                               P o u r     A r t h i c               */
/*                                                                     */
/*                                       V e r s i o n     3 . 0       */
/*                                                                     */
/*                                                                     */
/*                                                                     */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* -------------------------- */
/*     internal header file   */
/* -------------------------- */


#ifdef PAS_TCP4W
#  define  FTP_DEFCTRLPORT (htons (21))     /* port FTP commande              */
#else
#  define  FTP_DEFCTRLPORT 21     /* port FTP commande */
#endif
#define  FTP_DEFTIMEOUT       30          /* timeout en secondes            */
#define  FTP_TMPSTRLENGTH     50          /* taille d'une chaine temporaire */
#define  FTP_REPSTRLENGTH    256          /* taille d'une chaine repertoire */


/* messages internes */
#define  WMFTP_RELEASE (WM_USER+92)      /* msg interne demande  FtpRelease  */
#define  WMFTP_RECV    (WM_USER+95)      /* msg interne de réception fichier */
#define  WMFTP_DIR     (WM_USER+96)      /* msg interne de réception dir     */
#define  WMFTP_SEND    (WM_USER+98)      /* msg interne d'envoi de fichier   */

/* Modes d'ouverture de fichier */
#define  FTPMODE_WRITE    32
#define  FTPMODE_READ    216
#define  FTPMODE_APPEND  467
#define  FTPMODE_NOTHING   0

/* pause dans l'envoi/réception */
#define FTP_DEFASYNCALONE   10
#define FTP_DEFASYNCMULTI    3
#define FTP_DEFDELAY       100          /* millisecondes */
#define FTP_DIRRATE(pX)  (pX->Next==NULL && pX->Prev==NULL ?  10  : 4)
#define FTP_RATE(pX)     (pX->Next==NULL && pX->Prev==NULL ?  \
                          pX->File.nAsyncAlone : pX->File.nAsyncMulti)

/* return codes for GetFtpConnectSocket/GetFtpListeSocket */
#define  INVALID_MODE        ((SOCKET) -2)
#define  INVALID_ANSWER      ((SOCKET) -3)


/* Arret pendant un transfert */
#define HAS_ABORTED()   (      IsBadWritePtr (pProcData,sizeof *pProcData) \
                          ||   pProcData->File.bAborted)

/* Fin de fonction asynchrone/synchrone */                          
#define RETURN(pD,x)                                                       \
    { if(  (! IsBadWritePtr(pD,sizeof *pD))   &&   pD->File.bAsyncMode )   \
       { PostMessage(pD->Msg.hParentWnd,pD->Msg.nCompletedMessage,TRUE,x); \
         return FTPERR_OK; }                                               \
      else   { return x; }  }




/* ------------------------------------------------- */
/* structures imbriquees                             */
/* ------------------------------------------------- */
struct S_FtpData
{
   SOCKET   ctrl_socket;    /* control stream       init INVALID_SOCKET */
   SOCKET   data_socket;    /* data stream          init INVALID_SOCKET */
   char     cType;          /* type (ASCII/binary)  init TYPE_A         */
   BOOL     bVerbose;       /* verbose mode         init FALSE          */ 
   BOOL     bPassif;        /* VRAI -> mode passif                      */ 
   unsigned short nPort;    /* connexion Port       init FTP_DEFPORT    */
   unsigned nTimeOut;       /* TimeOut in seconds   init FTP_DEFTIMEOUT */
   HFILE    hLogFile;       /* Log file                                 */ 
   char     szInBuf [2048]; /* incoming Buffer                          */
   struct sockaddr_in saSockAddr;   /* not used anymore */
   struct sockaddr_in saAcceptAddr; /* not used anymore */
}; /* struct S_FtpData */


struct S_FileTrf 
{
   HFILE    hf;         /* handle of the file which is being transfered */
   unsigned nCount;     /* number of writes/reads made on a file        */
   unsigned nAsyncAlone;/* pause each N frame in Async mode  (Def 40)   */
   unsigned nAsyncMulti;/* Idem but more than one FTP sssion (Def 10)   */
   unsigned nDelay;     /* time of the pause in milliseconds            */
   BOOL     bAborted;   /* data transfer has been canceled              */
   char     szBuf[FTP_DATABUFFER]; /* Data buffer                       */
   BOOL     bNotify;    /* application receives a msg each data packet  */
   BOOL     bAsyncMode; /* synchronous or asynchronous Mode             */
   LONG     lPos;       /* Bytes transfered                             */
   LONG     lTotal;     /* bytes to be transfered                       */
}; /* struct S_FileTrf */

struct S_Msg
{
   HWND          hParentWnd;        /* window which the msg is to be passed   */
   UINT          nCompletedMessage; /* msg to be sent at end of the function  */
};  /* struct S_Msg */


struct S_Verbose
{
   HWND          hVerboseWnd;  /* window which the message is to be passed    */
   UINT          nVerboseMsg;  /* msg to be sent each time a line is received */
};

/* ------------------------------------------------- */
/* global structure                                  */
/* ------------------------------------------------- */
struct S_ProcData
{  
   /* task data */
   THREADID   (CALLBACK * fIdentifyThread)(void);   /* function d'identification thread */
   THREADID   nThreadIdent;       /* Identifiant du Thread                */
   /*HTASK      hTask;              * Task Id                              */
   HWND       hFtpWnd;            /* Handle of the internal window        */
   HWND       hParentWnd;         /* handle given to the FtpInit function */
   HINSTANCE  hInstance;          /* Task Instance                        */
   BOOL       bRelease;           /* FtpRelease has been called           */

   /* Mesasge information */
   struct S_Msg      Msg;      
   struct S_Verbose  VMsg;      
                                                               
   /* File information */
   struct S_FileTrf  File;  

   /* Ftp information */
   struct S_FtpData   ftp;

   /* Linked list */
   struct S_ProcData far *Next;
   struct S_ProcData far *Prev;
}; /* struct S_ProcData */

typedef struct S_ProcData far * LPProcData;
typedef struct S_FtpData far *  LPFtpData;


/* ------------------------------------------------- */
/* donnees automate                                  */
/* ------------------------------------------------- */
#define   _S_END      -2

enum FtpCmds  {  _S_CONNECT = 0, _S_USER, _S_PASS, _S_ACCOUNT, _S_QUIT, _S_HELP, 
                 _S_HELPCMD, _S_DELE, _S_CWD, _S_CDUP, _S_MKD, _S_RMD, 
                 _S_PWD, _S_TYPE, _S_RNFR, _S_RNTO, _S_SYST, _S_NOOP, _S_REST,
                 _S_ENDFILETRANSFER };


/* ------------------------------------------------- */
/* primitives internes                               */
/* ------------------------------------------------- */
LPProcData  _export PASCAL FAR FtpDataPtr(void);

int Protection (void);
int PASCAL FAR IntFtpGetAnswerCode (LPFtpData pFtpData); 
int PASCAL FAR IntTnSend (SOCKET s, LPSTR szString, 
                          BOOL bHighPriority, HFILE hf);


/* etage III : fonctions FTP */   
int FtpCloseFileTransfer (LPProcData pProcData, BOOL bFlush);

/* utilitaires */
LPProcData ToolsLocateProcData (void);
SOCKET GetFTPListenSocket(LPFtpData pFtpData);
int AbortAction (LPProcData pProcData, BOOL bFlush, BOOL bMsg, UINT nDbg);

/* Synchrones */
int FtpSyncSend (LPProcData pProcData);
int FtpSyncRecv (LPProcData pProcData);

/* Asynchrone */
LRESULT _export PASCAL FAR DLLWndProc (HWND hWnd, UINT Msg, 
                                       WPARAM wParam, LPARAM lParam);

void _cdecl Ftp4w_Trace (const char *szFile, const char *fmt, ...);
