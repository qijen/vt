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




#define  FTP4W_INCLUDES_AND_GENERAL +

#include <windows.h>
#include <windowsx.h>
#include <string.h>

#include <tcp4w.h>      /* external header file */ 

#include "port32.h"    /* 16/32 bits           */
#include "ftp4w.h"     /* external header file */ 
#include "ftp4w_in.h"  /* internal header file */
#include "rfc959.h"    /* only for error codes */ 

extern LPProcData pFirstProcData;



/* ************************************************************ */
/*                                                              */
/* Donnees  Automates                                           */
/*                                                              */
/* ************************************************************ */



struct S_AnswerTranslation
{
  int nFtpAns;
  int n4wAns;
}; /* struct S_AnswerTranslation */

struct S_OrdreSmtp 
{
   LPSTR                        szOrdre;
   struct S_AnswerTranslation   s [10];    /* _S_END last answer */
}  
static sOrdre []= 
{
  { NULL,  /* Trame de connexion */
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 110, FTPERR_OK,                },
        { 120, FTPERR_OK,                },
        { 220, FTPERR_OK,                },
        { 421, FTPERR_SERVERCANTEXECUTE, },
        { _S_END,  0                     } }   },
  { "USER %s",  /* Trame authentification */
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 220, FTPERR_OK,                },
        { 230, FTPERR_OK,                },
        { 331, FTPERR_ENTERPASSWORD,     },
        { 421, FTPERR_SERVERCANTEXECUTE, },
        { _S_END,  0                     } }   },
  { "PASS %s",  /* Trame authentification */
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 220, FTPERR_OK,                },
        { 230, FTPERR_OK,                },
        { 332, FTPERR_ACCOUNTNEEDED,     },
        { 501, FTPERR_CMDNOTIMPLEMENTED, },
        { 503, FTPERR_CMDNOTIMPLEMENTED, },
        { 504, FTPERR_CMDNOTIMPLEMENTED, },
        { 530, FTPERR_LOGINREFUSED,      }, 
        { _S_END,  0                     } }   },
  { "ACCT %s",  /* Trame authentification */
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 200, FTPERR_OK,                },
        { 202, FTPERR_OK,                },
        { 220, FTPERR_OK,                },
        { 230, FTPERR_OK,                },	/* serveur Unisys */
        { 500, FTPERR_CMDNOTIMPLEMENTED, },
        { 503, FTPERR_CMDNOTIMPLEMENTED, },
        { 530, FTPERR_LOGINREFUSED,      }, 
        { _S_END,  0                     } }   },
  { "QUIT",
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 200, FTPERR_OK,                },
        { 220, FTPERR_OK,                },
        { 221, FTPERR_OK,                },
        { _S_END,  0                     } }   },
  { "HELP",
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 211, FTPERR_OK,                },
        { 214, FTPERR_OK,                },
        { 220, FTPERR_OK,                },
        { 504, FTPERR_CMDNOTIMPLEMENTED, },
        { _S_END,  0                     } }   },
  { "HELP %s",
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 211, FTPERR_OK,                },
        { 214, FTPERR_OK,                },
        { 220, FTPERR_OK,                },
        { 504, FTPERR_CMDNOTIMPLEMENTED, },
        { _S_END,  0                     } }   },
  { "DELE %s",
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 200, FTPERR_OK,                },
        { 250, FTPERR_OK,                },
        { 450, FTPERR_FILELOCKED,        },
        { 502, FTPERR_CMDNOTIMPLEMENTED, },
        { 504, FTPERR_CMDNOTIMPLEMENTED, },
        { 550, FTPERR_SERVERCANTEXECUTE, },
        { _S_END,  0                     } }   },
  { "CWD %s",
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 200, FTPERR_OK,                },
        { 250, FTPERR_OK,                },
        { 450, FTPERR_FILELOCKED,        },
        { 502, FTPERR_CMDNOTIMPLEMENTED, },
        { 504, FTPERR_CMDNOTIMPLEMENTED, },
        { 550, FTPERR_SERVERCANTEXECUTE, },
        { _S_END,  0                     } }   },
  { "CDUP",
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 200, FTPERR_OK,                },
        { 250, FTPERR_OK,                },
        { 450, FTPERR_FILELOCKED,        },
        { 502, FTPERR_CMDNOTIMPLEMENTED, },
        { 504, FTPERR_CMDNOTIMPLEMENTED, },
        { 550, FTPERR_SERVERCANTEXECUTE, },
        { _S_END,  0                     } }   },
  { "MKD %s",
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 200, FTPERR_OK,                },
        { 227, FTPERR_OK,                },
        { 257, FTPERR_OK,                },
        { 502, FTPERR_CMDNOTIMPLEMENTED, },
        { 504, FTPERR_CMDNOTIMPLEMENTED, },
        { 521, FTPERR_SERVERCANTEXECUTE, },
        { 550, FTPERR_SERVERCANTEXECUTE, },
        { _S_END,  0                     } }   },
  { "RMD %s",
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 200, FTPERR_OK,                },
        { 250, FTPERR_OK,                },
        { 257, FTPERR_OK,                },
        { 450, FTPERR_FILELOCKED,        },
        { 502, FTPERR_CMDNOTIMPLEMENTED, },
        { 504, FTPERR_CMDNOTIMPLEMENTED, },
        { 550, FTPERR_SERVERCANTEXECUTE, },
        { _S_END,  0                     } }   },
  { "PWD",
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 200, FTPERR_OK,                },
        { 257, FTPERR_OK,                },
        { 502, FTPERR_CMDNOTIMPLEMENTED, },
        { 504, FTPERR_CMDNOTIMPLEMENTED, },
        { 550, FTPERR_SERVERCANTEXECUTE, },
        { _S_END,  0                     } }   },
  { "TYPE %s",
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 200, FTPERR_OK,                },
        { 502, FTPERR_CMDNOTIMPLEMENTED, },
        { 504, FTPERR_CMDNOTIMPLEMENTED, },
        { 530, FTPERR_CANNOTCHANGETYPE,  },
        { 550, FTPERR_CANNOTCHANGETYPE,  },
        { _S_END,  0                     } }   },
  { "RNFR %s",
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 350, FTPERR_OK,                },
        { 450, FTPERR_FILELOCKED,        },
        { 502, FTPERR_CMDNOTIMPLEMENTED, },
        { 504, FTPERR_CMDNOTIMPLEMENTED, },
        { 550, FTPERR_NOREMOTEFILE,      },
        { _S_END,  0                     } }   },
  { "RNTO %s",
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 250, FTPERR_OK,                },
        { 502, FTPERR_CMDNOTIMPLEMENTED, },
        { 504, FTPERR_CMDNOTIMPLEMENTED, },
        { 550, FTPERR_SERVERCANTEXECUTE, },
        { 553, FTPERR_SERVERCANTEXECUTE, },
        { _S_END,  0                     } }   },
  { "SYST",
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 200, FTPERR_OK,                },
        { 215, FTPERR_OK,                },
        { 220, FTPERR_OK,                },
        { 502, FTPERR_CMDNOTIMPLEMENTED, },
        { 504, FTPERR_CMDNOTIMPLEMENTED, },
        { _S_END,  0                     } }   },
  { "NOOP",
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 200, FTPERR_OK,                },
        { 500, FTPERR_CMDNOTIMPLEMENTED, },
        { _S_END,  0                     } }   },
  { "REST %s",
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 350, FTPERR_RESTARTOK,         },
        { 500, FTPERR_CMDNOTIMPLEMENTED, },
        { 502, FTPERR_CMDNOTIMPLEMENTED, },
        { 504, FTPERR_CMDNOTIMPLEMENTED, },
        { _S_END,  0                     } }   },
  { NULL,  /* EndFileTransfer */
     {  {  -1, FTPERR_NOREPLY            },
        {   0, FTPERR_UNEXPECTEDANSWER,  },
        { 226, FTPERR_OK,                },
        { 250, FTPERR_OK,                },
        { 421, FTPERR_SERVERCANTEXECUTE, },
        { 552, FTPERR_CANTWRITE ,        },
        { _S_END,  0                     } }   },

}; /* sOrdre */



/* ------------------------------------------------------------ */
/* FtpAutomate                                                  */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpAutomate (int nIdx, LPCSTR szParam)
{
LPProcData pProcData;
int        Rc=TN_SUCCESS;
char       szBuf [FTP_REPSTRLENGTH];
int        Ark;
  
  /* ------- retrouve la bonne session */
  pProcData = ToolsLocateProcData ();
  if (pProcData == NULL)  return FTPERR_NOTINITIALIZED;
  if (pProcData->ftp.ctrl_socket==INVALID_SOCKET)   return FTPERR_NOTCONNECTED;
  /* ------- Nécessite d'envoyer un ordre */
  if (sOrdre [nIdx].szOrdre != NULL) 
    {
      /* ------- traite un parametre */
      if (szParam!=NULL)   wsprintf (szBuf, sOrdre [nIdx].szOrdre, szParam);
      else                 lstrcpy ( szBuf, sOrdre [nIdx].szOrdre );
     /* ------- Envoie la commande */
      Rc = IntTnSend ( pProcData->ftp.ctrl_socket, szBuf, FALSE, pProcData->ftp.hLogFile);
      if (Rc != TN_SUCCESS)       return FTPERR_SENDREFUSED;
    }
  /* ------- Recupere la reponse */
  Rc = IntFtpGetAnswerCode (& pProcData->ftp);
  /* ------- Translation de la reponse */
  for (Ark=0 ;  sOrdre [nIdx].s[Ark].nFtpAns != _S_END ;  Ark++ )
    {
      if (Rc==sOrdre [nIdx].s[Ark].nFtpAns)  return sOrdre [nIdx].s[Ark].n4wAns;
    }  /* code de  retour */
return FTPERR_UNEXPECTEDANSWER;
}   /* FtpAutomate */





/* ------------------------------------------------------------*/
/* Fonction DLL    FtpOpenConnection                           */
/* ----------------------------------------------------------- */
int _export PASCAL FAR FtpOpenConnection (LPCSTR szHost)
{
LPProcData             pProcData;
int                    Rc;

  pProcData = ToolsLocateProcData ();
  if (pProcData == NULL)  return FTPERR_NOTINITIALIZED;

  Rc=TcpConnect(& pProcData->ftp.ctrl_socket, 
                  szHost, 
                pProcData->ftp.nPort==FTP_DEFCTRLPORT?(LPSTR)"ftp":(LPSTR) NULL,
                & pProcData->ftp.nPort);
  switch (Rc)
    {
       case TCP4U_SUCCESS :       pProcData->ftp.cType       = TYPE_A;
                               return FtpAutomate (_S_CONNECT, NULL);
       case TCP4U_HOSTUNKNOWN :   return FTPERR_UNKNOWNHOST;
       case TCP4U_TIMEOUT :       return FTPERR_TIMEOUT;
       case TCP4U_CONNECTFAILED : return FTPERR_CONNECTREJECTED;
       case TCP4U_NOMORESOCKET :  return FTPERR_CANTCREATESOCKET;
       case TCP4U_CANCELLED    :  return FTPERR_CANCELBYUSER;
       default              :  return FTPERR_CANTCONNECT;
    } /* return FTP4W error codes */              
} /* FtpOpenConnection */


/* ------------------------------------------------------------ */
/* Fonction DLL    FtpLogin                                     */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpLogin (LPCSTR szHost, LPCSTR szUser, LPCSTR szPasswd, 
                                 HWND hParentWnd, UINT wMsg)
{
int Rc;
LPProcData pProcData;
   pProcData = ToolsLocateProcData ();
   if (pProcData == NULL)  return FTPERR_NOTINITIALIZED;
   
   /* enregistre le message à envoyer */
   pProcData->Msg.nCompletedMessage = wMsg;
   pProcData->Msg.hParentWnd = hParentWnd;
   /* demande d'ouverture de la connexion */
   if (szHost==NULL)    return FTPERR_INVALIDPARAMETER;
   Rc = FtpOpenConnection (szHost);
   if (Rc==FTPERR_OK  &&  szUser!=NULL)    Rc = FtpSendUserName (szUser);
   if (Rc==FTPERR_ENTERPASSWORD  &&  szPasswd!=NULL)  
            Rc = FtpSendPasswd (szPasswd);
   RETURN (pProcData, Rc); 
} /* FtpLogin */



/* ------------------------------------------------------------ */
/* Fonction DLL    FtpSendUserName                              */
/* ------------------------------------------------------------ */
int _export PASCAL FAR  FtpSendUserName (LPCSTR szUserName)
{
return FtpAutomate (_S_USER, szUserName);
} /* FtpSendUserName */



/* ------------------------------------------------------------ */
/* Fonction DLL    FtpSendPasswd                                */
/* ------------------------------------------------------------ */
int _export PASCAL FAR  FtpSendPasswd (LPCSTR szPasswd) 
{
return FtpAutomate (_S_PASS, szPasswd);
} /* FtpSendPasswd */
                           

/* ------------------------------------------------------------ */
/* Fonction DLL    FtpSendAccount                               */
/* ------------------------------------------------------------ */
int _export PASCAL FAR  FtpSendAccount (LPCSTR szAccount) 
{
return FtpAutomate (_S_ACCOUNT, szAccount);
} /* FtpSendAccount */
                           
/* ------------------------------------------------------------ */
/* Fonction DLL    FtpCloseConnection                           */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpCloseConnection (void)
{
int Rc;
  Rc = FtpAutomate (_S_QUIT, NULL);
  if (Rc==FTPERR_OK)  FtpLocalClose ();
return Rc;
} /* FtpCloseConnection */



/* ------------------------------------------------------------ */
/* Fonction DLL    FtpHelp                                      */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpHelp (LPCSTR szArg, LPSTR szBuf, UINT uBufSize)
{
int Rc;
LPSTR p;
LPProcData pProcData; 

  Rc = FtpAutomate (szArg==NULL ?   _S_HELP : _S_HELPCMD, szArg);
  if (Rc==FTPERR_OK)
    {
       pProcData = ToolsLocateProcData ();
      /* on élimine la dernière ligne (code de retour) */
       p = strrchr (pProcData->ftp.szInBuf, '\r');
       if (p!=NULL) *p= 0;
       lstrcpyn (szBuf, & pProcData->ftp.szInBuf[4], uBufSize);
     }
return Rc;     
} /* FtpHELP */


/* ------------------------------------------------------------ */
/* Fonction DLL    FtpDeleteFile                                */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpDeleteFile (LPCSTR szRemoteFile)
{
return FtpAutomate (_S_DELE, szRemoteFile);
} /* FtpDelete */


/* ------------------------------------------------------------ */
/* Fonction DLL    FtpCWD                                       */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpCWD (LPCSTR szPath)
{
return FtpAutomate (_S_CWD, szPath);
} /* FtpCWD */


/* ------------------------------------------------------------ */
/* Fonction DLL    FtpCDUP                                      */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpCDUP (void)
{
return FtpAutomate (_S_CDUP, NULL);
} /* FtpCDUP */



/* ------------------------------------------------------------ */
/* Fonction DLL    FtpMKD                                       */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpMKD (LPCSTR szPath, LPSTR szFullDir, UINT uBufSize)
{
int        Rc;
LPSTR      p;
LPProcData pProcData; 

  Rc = FtpAutomate (_S_MKD, szPath);
  if (Rc==FTPERR_OK)
    {
      /* Voir explications RFC 959 appendix II et fonction FtpPWD */
      if (szFullDir==NULL || uBufSize==0)  return FTPERR_OK;
     /* sinon on tente de rendre dans szFullDir le nom du */
     /* repertoire qu'on vient de créer                   */
      pProcData = ToolsLocateProcData ();
      if (    pProcData->ftp.szInBuf[4]=='"' 
            &&  (p=strchr (& pProcData->ftp.szInBuf[5], '"'))!=NULL )
          {
            *p=0;  /* on remplace le 2ème " par un fin de chaine */
             lstrcpyn (szFullDir, & pProcData->ftp.szInBuf[5], uBufSize);
          }
       else  szFullDir [0] =0;  /* illisible -> on rend une chaine vide */
   }
return Rc;
} /* FtpMKD */


/* ------------------------------------------------------------ */
/* Fonction DLL    FtpRMD                                       */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpRMD (LPCSTR szPath)
{
return FtpAutomate (_S_RMD, szPath);
} /* FtpRMD */


/* ------------------------------------------------------------ */
/* Fonction DLL    FtpPWD                                       */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpPWD (LPSTR szBuf, UINT uBufSize)
{
LPProcData pProcData;
int        Rc; 
LPSTR      p;

  Rc = FtpAutomate (_S_PWD, NULL);
  if (Rc==FTPERR_OK)
    {
      pProcData = ToolsLocateProcData ();
      /* en principe pas la peine */
      /* Réponse standard de PWD : 257 "full dir name" .... */
      /* si reponse OK, on renvoie FTPERR_OK, si 257 mais   */
      /* format different, on renvoie FTPERR_PWDBADFMT      */
      
      if (    pProcData->ftp.szInBuf[4]=='"' 
          &&  (p=strchr (& pProcData->ftp.szInBuf[5], '"'))!=NULL )
               {
                 *p=0;  /* on remplace le " par un fin de chaine */
                 lstrcpyn (szBuf, & pProcData->ftp.szInBuf[5], uBufSize);
                 return FTPERR_OK;
               }
      else  
               {
                 lstrcpyn (szBuf, & pProcData->ftp.szInBuf[4], uBufSize);
                 return FTPERR_PWDBADFMT;
              }
     }
return Rc;
} /* FtpPWD */


/* ------------------------------------------------------------    */
/* Fonction DLL    FtpSetType                                         */
/* change the type of the data transfer (eithr ASCII or BINARY)    */
/* ------------------------------------------------------------    */
int _export PASCAL FAR FtpSetType (char cType)
{
char       szTmp [5];
LPProcData pProcData;
int        Rc;

  pProcData = ToolsLocateProcData ();
  if (pProcData==NULL)   return FTPERR_NOTINITIALIZED;
  if ( pProcData->ftp.cType == cType )  Rc = FTPERR_OK;
  else
    {
       szTmp[0] = cType;   
       if (cType!=TYPE_L8)   szTmp[1] = 0;  /* transfo en chaine */
       else                { szTmp[1] = ' ', szTmp[2] = '8', szTmp[3]=0; }
       Rc =FtpAutomate (_S_TYPE, szTmp);
       if (Rc==FTPERR_OK)    pProcData->ftp.cType = cType;
     } /* if not the current type */
return Rc;
} /* FtpSetType */


/* ------------------------------------------------------------    */
/* Fonction DLL    FtpRestart                                      */
/* Ask to the server to send a file from Nth byte                  */
/* ------------------------------------------------------------    */
int _export PASCAL FAR FtpRestart (long lByteCount)
{
char szByteCount [sizeof "4294967295"];
  if (lByteCount<=0)  return FTPERR_RESTARTOK; /* should not happen */
  wsprintf (szByteCount, "%ld", lByteCount);
return FtpAutomate (_S_REST, szByteCount);
} /* FtpRestart */


/* ------------------------------------------------------------    */
/* Fonction DLL    FtpRenameFile                                   */
/* Change the name of a remote file                                */
/* ------------------------------------------------------------    */
int _export PASCAL FAR FtpRenameFile (LPCSTR szFrom, LPCSTR szTo)
{
int        Rc;
  Rc = FtpAutomate (_S_RNFR, szFrom);
return  Rc==FTPERR_OK ?  FtpAutomate (_S_RNTO, szTo) : Rc;
} /* FtprenameFile */



/* ------------------------------------------------------------    */
/* Fonction DLL    FtpQuote                                        */
/* returns either a FTP return code (ie 200), or a WFTP error code */
/* ------------------------------------------------------------    */
int _export PASCAL FAR FtpQuote (LPCSTR szCmd, LPSTR szReplyBuf, UINT uBufSize)
{
LPProcData pProcData;
int        Rc;

  pProcData = ToolsLocateProcData ();
  if (pProcData==NULL  ||  szCmd==NULL)  return FTPERR_NOTINITIALIZED;
  if (pProcData->ftp.ctrl_socket==INVALID_SOCKET)   return FTPERR_NOTCONNECTED;

  Rc = TnSend ( pProcData->ftp.ctrl_socket, szCmd, FALSE, pProcData->ftp.hLogFile);
  if (Rc != TN_SUCCESS)          return FTPERR_SENDREFUSED;
  Rc = IntFtpGetAnswerCode ( & pProcData->ftp );
  if (Rc==-1)     return FTPERR_NOREPLY;
  if (szReplyBuf!=NULL) 
     lstrcpyn ( szReplyBuf, pProcData->ftp.szInBuf, uBufSize );
return Rc;
} /* FtpQuote */


/* ------------------------------------------------------------    */
/* Fonction DLL    FtpSyst                                         */
/* returns the index in the string or an error code                */
/* ------------------------------------------------------------    */
int _export PASCAL FAR FtpSyst (LPCSTR FAR * szSystStr)
{
LPProcData pProcData;
int        Rc;
char       szBuffer[128];  /* chaine retournee par le serveur */

  Rc= FtpAutomate (_S_SYST, NULL);
  if (Rc!=FTPERR_OK)  return Rc;
  pProcData = ToolsLocateProcData ();
  AnsiUpper (pProcData->ftp.szInBuf);
  for ( Rc=0 ; szSystStr[Rc]!=NULL ; Rc++)
   {
     /* copier la chaine pour la comparaison en majuscules */
     lstrcpyn (szBuffer, szSystStr[Rc], sizeof szBuffer);
     AnsiUpper (szBuffer);
     if (strstr (pProcData->ftp.szInBuf, szBuffer)!=NULL) return Rc;
   }
return FTPERR_SYSTUNKNOWN;  /* chaine non trouvee */
} /* FtpSyst */


