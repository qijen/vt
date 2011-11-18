
/* Some code about firewall: This code does not work */


#include <windows.h>
#include <ftp4w.h>


/* Fin de fonction asynchrone/synchrone */                          
#define RETURN(pD,x)                                                       \
    { if(  (! IsBadWritePtr(pD,sizeof *pD))   &&   pD->File.bAsyncMode )   \
       { PostMessage(pD->Msg.hParentWnd,pD->Msg.nCompletedMessage,TRUE,x); \
         return FTPERR_OK; }                                               \
      else   { return x; }  }
#define SizeOfTab(t)   ( sizeof (t) / sizeof (t[0]) )
#define TN_SUCCESS    1

/* -------------------------------------- */
/* debugging info                         */
/* -------------------------------------- */
#define DBG_FIREWALL +
#ifdef DBG_FIREWALL
static void wMsgBox (char *szStr, char *szArg, int nArg)
{
char szDbg[256];
LPProcData pProcData;

  pProcData = FtpDataPtr();
  if (szArg==NULL)       wsprintf (szDbg, szStr, nArg);
  else if (nArg==11111)  wsprintf (szDbg, szStr, szArg);
  else                   wsprintf (szDbg, szStr, szArg, nArg);
  MessageBox (pProcData->hParentWnd, szDbg, "Ftp4w", MB_OK);
} /* wMsgBox */
#else
#  define wMsgBox(a,b,c)
#endif /* DBG_FIREWALL */



/* Note: two undocumented functions are used in this code:     */
/* - IntTnSend which sends a message to the remote host on     */
/*   socket S                                                  */
/* - IntFtpGetAnswerCode which waits for an answer from the    */
/* server and returns the 3 digits code sent by the server     */

int PASCAL FAR IntFtpGetAnswerCode (LPFtpData pFtpData); 
int PASCAL FAR IntTnSend (SOCKET s, LPSTR szString, 
                          BOOL bHighPriority, HFILE hf);


/* ------------------------------------------------------------ */
/* Fonction DLL    FtpFirewallLogin                             */
/* ------------------------------------------------------------ */

struct S_Firewall
{ 
  int   nType;
  BOOL  bFWUserPwd;
  LPSTR szCmd;
  LPSTR szUserStr;
}; /* struct S_Firewall */

static struct S_Firewall sFirewall[] =
{  
   {  FTP4W_FWSITE,          TRUE,  "SITE %s", "USER %s"    },
   {  FTP4W_FWPROXY,         FALSE, "OPEN %s", "USER %s"    },
   {  FTP4W_FWUSERWITHLOGON, TRUE,   NULL,     "USER %s@%s" },
   {  FTP4W_FWUSERNOLOGON,   FALSE,  NULL,     "USER %s@%s" },
}; /* struct S_Firewall sFirewall */


int PASCAL FAR FtpFirewallLogin(
                       LPSTR szFWHost, LPSTR szFWUser, LPSTR szFWPasswd,
                       LPSTR szRemHost,LPSTR szRemUser,LPSTR szRemPasswd,
                       int nFirewallType,
                       HWND hParentWnd, UINT wMsg)
{
int        Idx;
int        Rc;
LPProcData pProcData;
char       szTnS[256];

   /* Reads data structure */
   pProcData = FtpDataPtr ();
   if (pProcData == NULL)  return FTPERR_NOTINITIALIZED;
   
   /* The message to be sent in async mode */
   pProcData->Msg.nCompletedMessage = wMsg;
   pProcData->Msg.hParentWnd = hParentWnd;
   /* ùcheck parameters */
   if (szFWHost==NULL  ||  szRemHost==NULL)    return FTPERR_INVALIDPARAMETER;
   /* Get FW structure */
   for (Idx=0 ;  
        Idx<SizeOfTab(sFirewall) && sFirewall[Idx].nType!=nFirewallType; 
        Idx++ );
   if (Idx >= SizeOfTab(sFirewall))   return FTPERR_INVALIDPARAMETER;
   wMsgBox ("Index DB sFirewall: %d", NULL, Idx);

   /* open connection with firewall */
   Rc = FtpOpenConnection (szFWHost);
wMsgBox ("Retour FtpOpenConnection: %d", NULL, Rc);
   if (Rc==FTPERR_CANTCONNECT)      Rc=FTPERR_FWCANTCONNECT;
   if (Rc==FTPERR_CONNECTREJECTED)  Rc=FTPERR_FWCONNECTREJECTED;
   if (Rc!=FTPERR_OK)    RETURN (pProcData, Rc);
   if (sFirewall[Idx].bFWUserPwd)
     {
       if (Rc==FTPERR_OK  &&  szFWUser!=NULL)    Rc =FtpSendUserName(szFWUser);
       if (Rc==FTPERR_ENTERPASSWORD  &&  szFWPasswd!=NULL)  
            Rc = FtpSendPasswd (szFWPasswd);
       if (Rc==FTPERR_LOGINREFUSED)  Rc=FTPERR_FWLOGINREFUSED;
       if (Rc!=FTPERR_OK)  RETURN (pProcData, Rc);
wMsgBox ("Retour Firewall LOGON: %d", NULL, Rc);
     }  /* Log on fiirewall */
     
   /* Connection with the remote destination */
   if (sFirewall[Idx].szCmd!=NULL)
     {
       wsprintf (szTnS, sFirewall[Idx].szCmd, szRemHost);
       Rc = IntTnSend (pProcData->ftp.ctrl_socket, szTnS, 
                    FALSE, pProcData->ftp.hLogFile);
       if (Rc!=TN_SUCCESS)   RETURN (pProcData, FTPERR_SENDREFUSED)
       if ( IntFtpGetAnswerCode (& pProcData->ftp)/100 != 2 )
           RETURN (pProcData, FTPERR_CANTCONNECT);
     }  /* szCmd pas NULL -> SITE ou PROXY */
   
   /* Authentification */
   wsprintf (szTnS, sFirewall[Idx].szUserStr, szRemUser, szRemHost);
   Rc = IntTnSend (pProcData->ftp.ctrl_socket, szTnS, 
                FALSE, pProcData->ftp.hLogFile);
   if (Rc!=TN_SUCCESS)   RETURN (pProcData, FTPERR_SENDREFUSED)
   Rc = IntFtpGetAnswerCode (& pProcData->ftp);
wMsgBox ("Retour UserStr FWUSER: %d", NULL, Rc);
   /* analyses returned message */
   switch (Rc)
    {
      case 230 :  RETURN (pProcData, FTPERR_OK); break;
      case 331 :  Rc = FtpSendPasswd (szRemPasswd);  
wMsgBox ("Retour UserStr PASSWD: %d", NULL, Rc);
                  RETURN (pProcData, Rc);        break;
      default :   RETURN (pProcData, FTPERR_CANTCONNECT);
                  break;
    }  /* User */
} /* FtpFirewallLogin */
