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


/* ---------------------- */
/* Transferts de Fichiers */
/* ---------------------- */

#define  FTP4W_INCLUDES_AND_GENERAL +

#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


#include <tcp4w.h>      /* external header file */ 

#include "port32.h"    /* 16/32 bits           */
#include "ftp4w.h"     /* external header file */ 
#include "ftp4w_in.h"  /* internal header file */
#include "rfc959.h"    /* only for error codes */ 



extern LPProcData pFirstProcData;


/* ----------------------------------------------------------- */
/* FtpCloseFileTransfer: Ferme les ressources Winsockets       */
/*                       ouvertes pour un transfert de fichier */
/*                       le fichier est également fermé        */
/*                       Si bFlush est a TRUE, la socket de    */
/*                       données est vidée.                    */
/* ----------------------------------------------------------- */
int FtpCloseFileTransfer (LPProcData pProcData, BOOL bFlush)
{
int Rc;
                
    _lclose     (pProcData->File.hf);    /* close file */
     pProcData->File.hf = HFILE_ERROR;   
     /* close listen socket */
    /* flush received buffer */          
     if (bFlush)
       {
         do
            Rc = TcpRecv ( pProcData->ftp.data_socket, 
                           pProcData->File.szBuf, 
                           sizeof pProcData->File.szBuf, 
                           min (5, pProcData->ftp.nTimeOut), 
                           HFILE_ERROR );
         while (Rc > 0);
       } /* bFlush */
     /* close opened sockets */
return TcpClose(&pProcData->ftp.data_socket)==0 ? FTPERR_OK : FTPERR_CANTCLOSE;
}  /* FtpCloseFileTransfer */



/* ------------------------------------------------------------ */
/* Utilitaire GetFTPListenSocket                                */
/* ouvre une socket en réception                                */
/* based on WINTEL (ftp.c) and BSD (ftp.c)                      */
/*          Retourne un numéro de socket   						*/
/*          Si erreur de la pile TCP  : INVALID_SOCKET      	*/
/*          Si erreur de communication: INVALID_ANSWER         	*/
/* ------------------------------------------------------------ */
SOCKET GetFTPListenSocket(LPFtpData pFtpData)
{
SOCKET listen_skt;
int    iLength, Rc;
LPSTR  a, p;
char   szTmp2 [sizeof "PORT 255,255,255,255,255,255"]; 
unsigned short  NonPrivPort;   /* a non priviliged port chosen by system */
struct sockaddr_in saTmpAddr;

   NonPrivPort = 0;
   Rc = TcpGetListenSocket (& listen_skt, NULL, & NonPrivPort, 1);
   if (Rc != TCP4U_SUCCESS)  return INVALID_SOCKET;
   p = (LPSTR) & NonPrivPort;
   /* inform remote end about our port that we created. */
   iLength = sizeof (saTmpAddr);
   getsockname(pFtpData->ctrl_socket, (LPSOCKADDR) &saTmpAddr, &iLength);
   a = (LPSTR) & saTmpAddr.sin_addr;

#define  UC(b)  (((int)b)&0xff)
   wsprintf (szTmp2, "PORT %d,%d,%d,%d,%d,%d", 
             UC(a[0]), UC(a[1]), UC(a[2]), UC(a[3]), UC(p[1]), UC(p[0]) );
   /* on envoie la requete et on attend la réponse */
   if (   TnSend (pFtpData->ctrl_socket, szTmp2, FALSE, pFtpData->hLogFile) !=TN_SUCCESS
       || IntFtpGetAnswerCode (pFtpData) != 200 )
     {
        closesocket (listen_skt);
        return (SOCKET) INVALID_ANSWER;
     }
return listen_skt;
}  /* GetFTPListenSocket */


/* ------------------------------------------------------------ */
/* Utilitaire GetFTPConnectSocket                               */
/* Prépare une socket pour le connect                           */
/*          Retourne un numéro de socket   						*/
/*          Si erreur de la pile TCP  : INVALID_SOCKET      	*/
/*          Si erreur de communication: INVALID_ANSWER         	*/
/*          Si erreur dans passive mode:INVALID_MODE         	*/
/* ------------------------------------------------------------ */

SOCKET GetFTPConnectSocket(LPFtpData pFtpData)
{
int     nCount;
int     Rc;
SOCKET  connect_skt;
LPSTR   a, p1, p2;  /* a comme adress IP, p comme port */
unsigned short   nPort;

  if ( TnSend (pFtpData->ctrl_socket, "PASV", FALSE, pFtpData->hLogFile) !=TN_SUCCESS)
        return (SOCKET) INVALID_ANSWER;
  Rc = IntFtpGetAnswerCode (pFtpData);
  if (Rc != 227 )   /* 227 seul retour acceptable */
    {
      switch (Rc)
        {
          case 500 : 
          case 502 : return (SOCKET) INVALID_MODE;
          default :  return (SOCKET) INVALID_ANSWER;
        }
    } /* Retour command PASV incorrect */

  /* Réponse du type : 227 Entering Passive Mode (0,0,0,0,12,56) */
  a = strchr (& pFtpData->szInBuf [4], '(');
  if (a==NULL)  return (SOCKET) INVALID_ANSWER;

  /* transforme les 3 premiers ',' en '.' */
  for ( p1=strchr(++a,','), nCount=0; 
        p1!=NULL  &&  nCount<3; 
        nCount++) 
     { 
       *p1++='.';
        p1 = strchr(p1, ',');
      }  
        
  if (p1==NULL)  return (SOCKET) INVALID_ANSWER; /* mauvaise chaine */
  *p1++=0;  /* fin de chaine et p pointe sur la chaine Port*/
  if ( (p2=strchr(p1, ','))==NULL )  return (SOCKET) INVALID_ANSWER; 
  *p2++=0;  
  
  nPort = atoi (p1) * 256 + atoi(p2);
  Rc = TcpConnect (& connect_skt, a, NULL, & nPort);
return Rc==TCP4U_SUCCESS ? connect_skt : INVALID_SOCKET;
} /* GetFTPConnectSocket */



/* ------------------------------------------------------------ */
/* Tools Fonction     ToolsSetDataConnection ()                 */
/*                    etablit une connexion sur le port 20      */
/*                    A l'aide de GetFTPlistenScket             */
/*                   Modifie les éléments suivants :            */
/*                       - pProcData->File.hf                   */
/*                       - pProcData->ftp.saAcceptAddr          */
/*                       - pProcData->ftp.cType                 */
/*                       - pProcData->ftp.data_socket           */
/* ------------------------------------------------------------ */
int ToolsSetDataConnection (LPProcData pProcData, 
                            LPCSTR szLocalFile,
                            int   nMode, 
                            char  cBinAscMode,
                            long  lByteCount,
                            LPSTR szCmdString)
{
SOCKET tmp_socket;   /* socket d'aide à la création de la connexion data */
HFILE  hFile = HFILE_ERROR;
int    Rc, wErrMsg;


  /* ferme le fichier de la précédent connexion si besoin */
  if (pProcData->File.hf!=HFILE_ERROR)  _lclose (pProcData->File.hf);
  pProcData->File.hf = HFILE_ERROR;
  pProcData->File.bAborted = FALSE;

  /* changement de type (ASCII / binaire) */
  if (cBinAscMode==TYPE_I   ||  cBinAscMode==TYPE_A   ||  cBinAscMode==TYPE_L8)
     {
       Rc=FtpSetType (cBinAscMode);
       if (Rc!=FTPERR_OK)      return Rc;
     }

 /* Ouverture du fichier local */
  if (szLocalFile!=NULL)
    {
      switch (nMode)
        {
           case  FTPMODE_WRITE  :  hFile = _lcreat (szLocalFile, 0);    break;
           case  FTPMODE_APPEND :  hFile = _lopen (szLocalFile, OF_WRITE);  
                                   if (hFile==HFILE_ERROR)  
                                           hFile = _lcreat (szLocalFile, 0); 
                                   else    _llseek (hFile, 0, SEEK_END); 
                                   break;
           case  FTPMODE_READ  :   hFile = _lopen (szLocalFile, OF_READ);  break;
           default             :   hFile = HFILE_ERROR;                 break;
         }                              
      if (hFile == HFILE_ERROR)  return FTPERR_CANTOPENLOCALFILE;
    } /* szLocalFile non Nul */


  /* demande de la socket de connexion : distinction mode passif/mode classique */
  if (pProcData->ftp.bPassif)
        tmp_socket = GetFTPConnectSocket (& pProcData->ftp);
  else  tmp_socket = GetFTPListenSocket  (& pProcData->ftp);

  /* discussion sur les cas d'erreur */
  switch (tmp_socket)
  {
     case INVALID_SOCKET :	/* error in socket calls */
    	  _lclose (hFile);   
    	   return pProcData->ftp.bPassif ? 
    				FTPERR_CANTCREATESOCKET : 
    				FTPERR_DATACONNECTION ;
	 case INVALID_MODE :   /* Server does not support passive mode */
          _lclose (hFile);   
           return FTPERR_PASVCMDNOTIMPL ;
     case INVALID_ANSWER :  /* error in answer */
          _lclose (hFile);   
    	   return FTPERR_UNEXPECTEDANSWER;
  } /* tmp_socket en erreur */

  if (lByteCount>0)
    {
       Rc = FtpRestart (lByteCount);
       if (Rc != FTPERR_RESTARTOK)
         {  _lclose (hFile);   return Rc; }
    }  /* Commande Restart */

  /* envoi de la requete Get/Put/Dir */
  Rc = TnSend ( pProcData->ftp.ctrl_socket, szCmdString, FALSE, pProcData->ftp.hLogFile );
  if (Rc!=TN_SUCCESS)     
    { _lclose (hFile); TcpClose  (& tmp_socket); return FTPERR_SENDREFUSED; }
  Rc = IntFtpGetAnswerCode ( & pProcData->ftp );
  if (Rc!=150  &&  Rc!=125)   /* 150 et 125 seules réponses autorisées */
    {  
      switch (Rc)
        { 
           case -1  : wErrMsg = FTPERR_NOREPLY;         break;
           case 550 : wErrMsg = FTPERR_NOREMOTEFILE;    break;
           case 553 : wErrMsg = FTPERR_TRANSFERREFUSED; break;
           default  : wErrMsg = FTPERR_UNEXPECTEDANSWER;break;
         }
       _lclose  (hFile);
       if (pProcData->ftp.bPassif) WSACancelBlockingCall ();
       TcpClose  (& tmp_socket);
       return wErrMsg;
    } /* discussion des cas d'erreur */

  /* mode classique, il reste à attendre la connexion */
  if (! pProcData->ftp.bPassif)
    {
      Rc =  TcpAccept (& pProcData->ftp.data_socket,
                         tmp_socket,
                         pProcData->ftp.nTimeOut);
      /* fermeture de la socket d'attente de connexion */
      TcpClose (& tmp_socket);
      if (Rc != TCP4U_SUCCESS)
       {
         _lclose  (hFile); /* fermeture fichier */
         return HAS_ABORTED() ? FTPERR_CANCELBYUSER : FTPERR_DATACONNECTION;
       }
    } /* mode classique */
  else      pProcData->ftp.data_socket = tmp_socket;
  pProcData->File.hf = hFile;

return FTPERR_OK;
}  /* ToolsSetDataConnection */


/* ------------------------------------------------------------ */
/* Tools Fonction     AbortAction ()                            */
/*                    Traitement Asynchrone de l'abort  ou de   */
/*                    certains accidents comme plus d'espace    */
/*                    disque                                    */
/* ------------------------------------------------------------ */
int AbortAction (LPProcData pProcData, BOOL bFlush, BOOL bMsg, UINT nDbg)
{
int Rc;
    /* =====> Premier cas : FtpRelease a été appelé ->  */
#ifdef TRACE_ABORT
 char szTmp[50];
 wsprintf (szTmp, "Abort n° %d -> %d/%d", nDbg, pProcData->File.bAborted, IsBadWritePtr(pProcData, sizeof *pProcData) );
 MessageBox (pProcData->hParentWnd, szTmp, "DLL", MB_OK);
#endif

    if (pProcData->ftp.bVerbose)  Ftp4w_Trace (NULL, "entering AbortAction, Debug pos %d", nDbg); 

    if (IsBadWritePtr (pProcData, sizeof *pProcData)) return 0;
    /* reset flag */
    pProcData->File.bAborted = FALSE;
    /* =====> Soit un réél Abort, warning to server */
    do
      {  /* emission prioritaire de Abort */
         Rc = TnSend ( pProcData->ftp.ctrl_socket, "ABOR", TRUE, pProcData->ftp.hLogFile ); 
         Rc = IntFtpGetAnswerCode ( & pProcData->ftp );
      }
    while (Rc==500);
    Rc = IntFtpGetAnswerCode ( & pProcData->ftp );
    /* close sockets and files */
    FtpCloseFileTransfer (pProcData, bFlush);
    /* send Message if AsynchronousMode */
    if (bMsg && pProcData->File.bAsyncMode)
         PostMessage (pProcData->Msg.hParentWnd, 
                      pProcData->Msg.nCompletedMessage, 
                      TRUE,  (LPARAM) FTPERR_CANCELBYUSER);
    TcpFlush (pProcData->ftp.ctrl_socket);
return Rc;
} /* AbortAction */




/* ******************************************************************* */
/*                                                                     */
/* Partie VI : Utilisation de la fenêtre FTP                           */
/*                                                                     */
/* ******************************************************************* */


/* ------------------------------------------------------------- */
/* CALBACK                                                       */
/*          The Data transfer are not logged                     */
/* ------------------------------------------------------------- */

LRESULT _export PASCAL FAR DLLWndProc (HWND hWnd, UINT Msg, 
                                       WPARAM wParam, LPARAM lParam) 
{
LPProcData pProcData;
int    Rc, iRetCode=0;
int    nRead=0, nWritten=0;
LPSTR    p;


  pProcData = FtpDataPtr ();
  if (pProcData==NULL) return (LRESULT) DefWindowProc (hWnd,Msg,wParam,lParam);
  
   /* ------- */
   /* ABORT ? */
   /* ------- */
   if  ( HAS_ABORTED() && (Msg==WMFTP_RECV || Msg==WMFTP_SEND ||Msg==WMFTP_DIR) )
      {
        AbortAction (pProcData, Msg!=WMFTP_SEND, TRUE, 1);
        return 0l;
      }  /* Abort */


  switch (Msg)
    {
       /* Essentiel pour windows 3.1 */
       case WM_TIMER :
            KillTimer (hWnd, wParam);
            PostMessage  (hWnd, wParam, 0, 0l);
            break;
            
       /* ---------------- */
       /* Suite du fichier */
       /* ---------------- */
       case WMFTP_SEND :
            nRead = _lread (pProcData->File.hf, pProcData->File.szBuf, 
                            sizeof pProcData->File.szBuf );
            /* lecture OK */
            if (nRead > 0)
              {
                pProcData->File.lPos += (LONG) nRead;
                Rc = TcpSend (pProcData->ftp.data_socket, 
                                    pProcData->File.szBuf, nRead, 0, HFILE_ERROR);
                if (Rc == TCP4U_SUCCESS)  /* envoi OK */
                  {  
                     /* on envoie un message à la fenêtre FTP */
                     if ( ++pProcData->File.nCount % FTP_RATE(pProcData) != 0  
                          || ! SetTimer (pProcData->hFtpWnd, WMFTP_SEND, 
                                         pProcData->File.nDelay, NULL) )
                        PostMessage (pProcData->hFtpWnd, WMFTP_SEND, 0, (LPARAM) pProcData);
                     /* on envoie un message à l'utilisateur */
                     if (pProcData->File.bNotify)
                        PostMessage (pProcData->Msg.hParentWnd, 
                                     pProcData->Msg.nCompletedMessage, 
                                     FALSE, pProcData->File.lPos );
                  }  /* TcpSend OK */
                /* Fin du fichier */  
                else  /* problem */
                  {
                    /* Abort can happen during send */
                    if HAS_ABORTED()   AbortAction (pProcData, FALSE, TRUE, 2);
                    else    
                     {
                        FtpCloseFileTransfer (pProcData, FALSE);
                        PostMessage (pProcData->Msg.hParentWnd, 
                                     pProcData->Msg.nCompletedMessage, 
                                     TRUE, (LPARAM) FTPERR_SENDREFUSED);
                        break;
                     }
                  }
               }  /* lecture du fichier possible */ 
            else     /* fin de fichier */
               {
                 FtpCloseFileTransfer (pProcData, FALSE);
                 Rc = IntFtpGetAnswerCode ( & pProcData->ftp );
                 switch (Rc)
                   {
                     case 250 : 
                     case 226 : iRetCode = FTPERR_OK ; break;
                     case 452 :
                     case 552 : iRetCode = FTPERR_CANTWRITE ; break;
                     default  : iRetCode = FTPERR_UNEXPECTEDANSWER; break;
                   }
                 PostMessage (pProcData->Msg.hParentWnd, 
                              pProcData->Msg.nCompletedMessage, 
                              TRUE, 
                              (LPARAM) iRetCode);
               }
           break;



       case WMFTP_RECV :
              nRead = TcpRecv (pProcData->ftp.data_socket, 
                               pProcData->File.szBuf, 
                               sizeof pProcData->File.szBuf, 
                               pProcData->ftp.nTimeOut,
                               HFILE_ERROR);                               
            /* Abort during receive */
            if HAS_ABORTED()
              {
                AbortAction (pProcData, TRUE, TRUE, 3);
                break;  
              }  /* Abort */

            /* ---------------- */
            /* Suite du fichier */
            /* ---------------- */
            if (          nRead > 0   
                            &&
                (nWritten = _lwrite (pProcData->File.hf, 
                                     pProcData->File.szBuf, 
                                     nRead) )    == nRead )
                {
                   pProcData->File.lPos += (LONG) nWritten;
                   if ( ++pProcData->File.nCount % FTP_RATE(pProcData) != 0  
                        || ! SetTimer (pProcData->hFtpWnd, WMFTP_RECV, 
                                       pProcData->File.nDelay, NULL) )
                      PostMessage (pProcData->hFtpWnd, WMFTP_RECV, 0, (LPARAM) pProcData);
                   /* on envoie un message à l'utilisateur */
                   if (pProcData->File.bNotify)
                      PostMessage (pProcData->Msg.hParentWnd, 
                                   pProcData->Msg.nCompletedMessage, 
                                   FALSE,  pProcData->File.lPos );
                }  /* TcpRecv and write OK */
            else     /* fin de données */
               {
                 if (nRead>0  &&  nWritten!=nRead) /* erreur d'ecriture */
                   {
                     AbortAction (pProcData, TRUE, FALSE, 4);
                     iRetCode=FTPERR_CANTWRITE;
                   }
                 else
                   {  
                     FtpCloseFileTransfer (pProcData, FALSE);
                     Rc = IntFtpGetAnswerCode ( & pProcData->ftp );
                     if (nRead == 0)  /* fin de transfert */
                       switch (Rc)
                         {
                            case 226 :
                            case 250 :  iRetCode = FTPERR_OK ; break;
                            default  :  iRetCode = FTPERR_UNEXPECTEDANSWER;
                         }
                     else   
                        iRetCode = FTPERR_DATACONNECTION;
                    }
                 PostMessage (pProcData->Msg.hParentWnd, 
                              pProcData->Msg.nCompletedMessage, 
                              TRUE, 
                             (LPARAM) iRetCode);
               } /* end of data transfer or error */
           break;



       /* Dir : Mode trame (sinon receive classique) */
       case WMFTP_DIR :
            nRead = TnReadLine (pProcData->ftp.data_socket, 
                                pProcData->File.szBuf, 
                                sizeof pProcData->File.szBuf,
                                pProcData->ftp.nTimeOut, 
                                HFILE_ERROR );

            if HAS_ABORTED()
              {
                 AbortAction (pProcData, TRUE, TRUE, 5);
                 break;  
              }  /* Abort */

            /* --------------------------------- */
            /* Suite de la lecture du répertoire */
            /* --------------------------------- */
            p = strchr (pProcData->File.szBuf, '\r');
            if (p!=NULL)  *p = 0;
            if (nRead > 0)
              {
                 /* on envoie un message à l'utilisateur */
                 PostMessage (pProcData->Msg.hParentWnd, 
                              pProcData->Msg.nCompletedMessage, 
                              FALSE,  (LPARAM) pProcData->File.szBuf);
                 if (  ++pProcData->File.nCount % FTP_DIRRATE(pProcData) != 0  
                     || ! SetTimer (pProcData->hFtpWnd, WMFTP_DIR, 
                                    pProcData->File.nDelay, NULL) )
                        PostMessage (pProcData->hFtpWnd, WMFTP_DIR, 0, (LPARAM) pProcData);
              }  /* Receive OK */
            else     /* fin de données */
             {
                 FtpCloseFileTransfer (pProcData, FALSE);
                 if (nRead == 0)  /* fin de transfert */
                   {
                     Rc = IntFtpGetAnswerCode ( & pProcData->ftp );
                     switch (Rc)
                       {
                          case 226 :
                          case 250 :  iRetCode = FTPERR_OK ; break;
                          default  :  iRetCode = FTPERR_UNEXPECTEDANSWER;
                       }
                   }
                 else
                    iRetCode = FTPERR_DATACONNECTION;
                 PostMessage (pProcData->Msg.hParentWnd, 
                              pProcData->Msg.nCompletedMessage, 
                              TRUE, 
                              (LPARAM) iRetCode);
               }
           break;


    } /* switch Msg */
return (LRESULT) DefWindowProc (hWnd, Msg, wParam, lParam);
} /* DLLWndProc */




/* ******************************************************************* */
/*                                                                     */
/* Partie VII : Transfert de fichier synchrone                         */
/*                                                                     */
/* ******************************************************************* */


/* ------------------------------------------------------------    */
/* FtpSyncSend                                                     */
/* ------------------------------------------------------------    */
int FtpSyncSend (LPProcData pProcData)
{
int             nRead, Rc;
struct timeval  TO;

  for ( ; ; ) 
    {
       if HAS_ABORTED()
         {
            AbortAction (pProcData, FALSE, TRUE, 6);
            return FTPERR_CANCELBYUSER;
         }
         
       nRead = _lread  (pProcData->File.hf, 
                        pProcData->File.szBuf, 
                        sizeof pProcData->File.szBuf);
       if (nRead > 0)
          {
            pProcData->File.lPos += nRead;
            Rc = TcpSend (pProcData->ftp.data_socket, 
                                pProcData->File.szBuf, nRead, 0, HFILE_ERROR);
            if (Rc == TCP4U_SUCCESS)
               {  
                if (pProcData->File.bNotify)
                    PostMessage (pProcData->Msg.hParentWnd, 
                                 pProcData->Msg.nCompletedMessage, 
                                 FALSE,
                                 pProcData->File.lPos );
                 if (++pProcData->File.nCount % FTP_RATE(pProcData) == 0)  
                    {
                      TO.tv_sec  = pProcData->File.nDelay / 1000;
                      TO.tv_usec = (long) (pProcData->File.nDelay % 1000) * 1000l;
                      Rc = select (0, NULL, NULL, NULL, & TO);
                      if (Rc==-1 &&  HAS_ABORTED())
                       {
                          AbortAction (pProcData, FALSE, TRUE, 7);
                          return FTPERR_CANCELBYUSER;
                        }
                    }  /* pause */
               } /* nWritten <> nRead */             
             else
               {
                   FtpCloseFileTransfer (pProcData, FALSE);
                   return  FTPERR_SENDREFUSED;
               }  /* probleme reseau */ 
          } /* nRead > 0 */
        else     /* fin de données */
          {   
              FtpCloseFileTransfer (pProcData, FALSE);
              return FtpAutomate (_S_ENDFILETRANSFER, NULL);
          } /* can not read file */
    }  /* do forever */
} /* FtpSyncSend */


/* ------------------------------------------------------------    */
/* Fonction DLL    FtpSyncRecv                                     */
/* ------------------------------------------------------------    */
int FtpSyncRecv (LPProcData pProcData)
{
int             nRead=0, nWritten, iRetCode, Rc;
struct timeval  TO;

  for ( ; ; ) 
    {
       if ( ! HAS_ABORTED() )
            nRead = TcpRecv (pProcData->ftp.data_socket, 
                             pProcData->File.szBuf, 
                             sizeof pProcData->File.szBuf, 
                             pProcData->ftp.nTimeOut,
                             HFILE_ERROR);
                                 
       /* repeter le test car bAborted peut etre positionne entre temps */                   
       if HAS_ABORTED()
         {
            AbortAction (pProcData, TRUE, TRUE, 8);
            return FTPERR_CANCELBYUSER;
         }

       if (nRead > 0)
          {
             nWritten = _lwrite (pProcData->File.hf, 
                                 pProcData->File.szBuf, 
                                 nRead);
             if (nWritten!=nRead)
               {
                   /* envoi d'un ABORT au distant */ 
                   AbortAction (pProcData, TRUE, FALSE, 9);  /* receive data */
                   return  FTPERR_CANTWRITE;
               }  /* lecture TCP Non OK */ 
             pProcData->File.lPos += (unsigned) nWritten;
             if (pProcData->File.bNotify)
                 PostMessage (pProcData->Msg.hParentWnd, 
                              pProcData->Msg.nCompletedMessage, 
                              FALSE,
                              pProcData->File.lPos );
             if (++pProcData->File.nCount % FTP_RATE(pProcData) == 0)  
               {
                 TO.tv_sec  = pProcData->File.nDelay / 1000;
                 TO.tv_usec = (long) (pProcData->File.nDelay % 1000) * 1000l;
                 Rc = select (0, NULL, NULL, NULL, & TO);
               }  /* pause to be done */
          } /* nRead > 0 */
        else     /* fin de données */
          {
             if (nRead == TCP4U_SOCKETCLOSED)  /* fin de transfert normal */
                 {
                     FtpCloseFileTransfer (pProcData, FALSE);
                     iRetCode = FtpAutomate (_S_ENDFILETRANSFER, NULL);
                 }
              else /* erreur dans la connexion */
                 { 
                   AbortAction (pProcData, TRUE, FALSE, 10); 
                   iRetCode = FTPERR_DATACONNECTION;
                 }
               return iRetCode;
          } /* nRead <=0 */
    }  /* do forever */
} /* FtpSyncRecv */



/* ******************************************************************* */
/*                                                                     */
/* Partie VIII : Fonctions FTP exportées                               */
/*                                                                     */
/* ******************************************************************* */

/* ------------------------------------------------------------ */
/* Fonction DLL    FtpAbort                                     */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpAbort (void)
{
LPProcData pProcData;
  pProcData = FtpDataPtr ();
  if (pProcData == NULL)  return FALSE;
  if (pProcData->ftp.data_socket   != INVALID_SOCKET) 
    {
      WSACancelBlockingCall ();
      pProcData->File.bAborted = TRUE;
    }
  else if (pProcData->ftp.ctrl_socket!=INVALID_SOCKET)  WSACancelBlockingCall ();
return TRUE;
}       


/* ------------------------------------------------------------ */
/* Fonction DLL    FtpGetFileSize                               */
/* ------------------------------------------------------------ */
DWORD _export PASCAL FAR FtpGetFileSize (void)
{
LPProcData pProcData;
LPSTR      lp;
long       lFileSize=0;
  pProcData = FtpDataPtr ();
  if (pProcData == NULL)  return 0;
 
  /* code de retour serveur : 150, 125 -> opening data connection */
  if (   _fmemcmp (pProcData->ftp.szInBuf, "150", 3)!=0
      && _fmemcmp (pProcData->ftp.szInBuf, "125", 3)!=0)   return 0;
  AnsiLower (pProcData->ftp.szInBuf) ;
  lp=_fstrstr (pProcData->ftp.szInBuf, " bytes");
  if (lp==NULL)  return 0;
  while (isdigit (*--lp));
  for (lp++ ; isdigit (*lp) ; lp++ )
    { 
      lFileSize *= 10;
      lFileSize += (*lp-'0');
    }
return lFileSize;  
} /* FtpGetFileSize */


/* ------------------------------------------------------------ */
/* Fonction DLL    FtpOpenDataConnection                        */
/*                 Open the data connection on port 20          */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpOpenDataConnection (LPCSTR szRemote, int nAction,
                                              char cType)
{
char       szTmp [FTP_REPSTRLENGTH];
LPProcData pProcData;

  pProcData = FtpDataPtr ();
  wsprintf (szTmp, "%s %s", 
                nAction==FTP4W_STORE_ON_SERVER  ? (LPSTR) "STOR" :
                nAction==FTP4W_APPEND_ON_SERVER ? (LPSTR) "APPE" :  
                                                  (LPSTR) "RETR",
                szRemote);
return ToolsSetDataConnection (pProcData, NULL, FTPMODE_NOTHING,
                               cType, 0, szTmp);
} /* FtpOpenDataConnection */


/* ------------------------------------------------------------ */
/* Fonction DLL    FtpRecvThroughDataConnection                 */
/*                 recv on port port 20                         */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpRecvThroughDataConnection (LPSTR szBuf, 
                                                unsigned int far *lpBufSize)
{
int Rc= FTPERR_CANCELBYUSER;
LPProcData pProcData;

  pProcData = FtpDataPtr ();
  if ( ! HAS_ABORTED() )
       Rc = TcpRecv (pProcData->ftp.data_socket, szBuf, *lpBufSize, 
                          pProcData->ftp.nTimeOut, HFILE_ERROR);
  /* repeter le test car bAborted peut etre positionne entre temps */                   
  if HAS_ABORTED()
      {
         AbortAction (pProcData, TRUE, TRUE, 20);
         return FTPERR_CANCELBYUSER;
      }
  if (Rc>=TCP4U_SUCCESS)   
       { *lpBufSize = Rc;   return FTPERR_OK;  }
   if (Rc==TCP4U_SOCKETCLOSED) return FTPERR_ENDOFDATA ;
   else return FTPERR_DATACONNECTION;
} /* FtpRecvTroughDataConnection */


/* ------------------------------------------------------------ */
/* Fonction DLL    FtpSendThroughDataConnection                 */
/*                 send on port port 20                         */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpSendThroughDataConnection (LPCSTR szBuf, 
                                                    unsigned int uBufSize)
{
int Rc;
LPProcData pProcData;
   pProcData = FtpDataPtr ();
   Rc=TcpSend (pProcData->ftp.data_socket, szBuf, uBufSize, FALSE,HFILE_ERROR);
   if (Rc>=TCP4U_SUCCESS)      return FTPERR_OK;  
   else return FTPERR_DATACONNECTION;
} /* FtpSendTroughDataConnection */



/* ------------------------------------------------------------ */
/* Fonction DLL    FtpCloseDataConnection                       */
/*                 Closes the data connection on port 20        */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpCloseDataConnection (void)
{
LPProcData pProcData;
int        Rc;
  pProcData = FtpDataPtr ();
  Rc = TcpClose(&pProcData->ftp.data_socket)==TCP4U_SUCCESS ? 
                 FTPERR_OK : FTPERR_CANTCLOSE;
  if (Rc==FTPERR_OK)  
    {
       Rc = IntFtpGetAnswerCode ( & pProcData->ftp );
       switch (Rc)
        {
           case 250 : 
           case 226 : return FTPERR_OK ;
           case 452 :
           case 552 : return FTPERR_CANTWRITE ;
           default  : return FTPERR_UNEXPECTEDANSWER; 
        }
    }  /* transfert OK */
return FTPERR_CANTCLOSE;
} /* FtpCloseDataConnection */




/* ------------------------------------------------------------ */
/* Fonction DLL    FtpRecvFile                                  */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpRecvFile (LPCSTR szRemote, LPCSTR szLocal, 
                                    char cType, BOOL bNotify,
                                    HWND hParentWnd, UINT wMsg)
{
LPProcData pProcData;
int        Rc;
char       szTmp [FTP_REPSTRLENGTH];

  pProcData = FtpDataPtr ();
  if (pProcData == NULL)    return FTPERR_NOTINITIALIZED;
  if (pProcData->ftp.ctrl_socket==INVALID_SOCKET)   return FTPERR_NOTCONNECTED;

  pProcData->Msg.nCompletedMessage = wMsg;
  pProcData->Msg.hParentWnd = hParentWnd;

  wsprintf (szTmp, "RETR %s", szRemote);
  Rc = ToolsSetDataConnection (pProcData, 
                               szLocal, 
                               FTPMODE_WRITE,
                               cType,
                               0,           /* Skip restart command */
                               szTmp);
  if (Rc!=FTPERR_OK)     RETURN(pProcData,Rc); 
  
  pProcData->File.bNotify = bNotify;
  pProcData->File.lPos = 0;
  pProcData->File.lTotal = FtpGetFileSize ();
  
  if (pProcData->File.bAsyncMode)
   {
     PostMessage (pProcData->hFtpWnd, WMFTP_RECV, 0,(LPARAM)pProcData);
     return FTPERR_OK;
   }
  else return  FtpSyncRecv (pProcData);
} /* FtpRecvFile */



/* ------------------------------------------------------------ */
/* Fonction DLL    FtpSendFile                                  */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpSendFile (LPCSTR szLocal, LPCSTR szRemote, 
                                    char cType, BOOL bNotify,
                                    HWND hParentWnd, UINT wMsg)
{
LPProcData pProcData;
int        Rc;
char       szTmp [FTP_REPSTRLENGTH];

  pProcData = FtpDataPtr ();
  if (pProcData == NULL)    return FTPERR_NOTINITIALIZED;
  if (pProcData->ftp.ctrl_socket==INVALID_SOCKET)   return FTPERR_NOTCONNECTED;

  pProcData->Msg.nCompletedMessage = wMsg;
  pProcData->Msg.hParentWnd = hParentWnd;

  wsprintf ( szTmp, "STOR %s", szRemote);
  Rc = ToolsSetDataConnection (pProcData, 
                               szLocal, 
                               FTPMODE_READ,
                               cType, 
                               0,           /* Skip restart command */
                               szTmp);
  if (Rc!=FTPERR_OK)    RETURN(pProcData, Rc);

  pProcData->File.bNotify = bNotify;
  pProcData->File.lPos = 0;
  pProcData->File.lTotal = _llseek (pProcData->File.hf, 0, SEEK_END);
  _llseek (pProcData->File.hf, 0, SEEK_SET);

  if (pProcData->File.bAsyncMode)
   {
     PostMessage (pProcData->hFtpWnd, WMFTP_SEND, 0,(LPARAM)pProcData);
     return FTPERR_OK;
   }
  else return  FtpSyncSend (pProcData);
} /* FtpSendFile */


/* ------------------------------------------------------------ */
/* Fonction DLL    FtpAppendToLocalFile                         */
/*                 The same function than FtpRecvFile           */
/*                 Only a parameter has been changed :          */
/*                 ToolsSetDataConnection is called with        */
/*                 FTPMODE_APPEND instead of FTPMODE_READ       */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpAppendToLocalFile (LPCSTR szRemote, LPCSTR szLocal, 
                                            char cType, BOOL bNotify,
                                            HWND hParentWnd, UINT wMsg)
{
LPProcData pProcData;
int        Rc;
char       szTmp [FTP_REPSTRLENGTH];

  pProcData = FtpDataPtr ();
  if (pProcData == NULL)    return FTPERR_NOTINITIALIZED;
  if (pProcData->ftp.ctrl_socket==INVALID_SOCKET)   return FTPERR_NOTCONNECTED;

  pProcData->Msg.nCompletedMessage = wMsg;
  pProcData->Msg.hParentWnd = hParentWnd;

  wsprintf (szTmp, "RETR %s", szRemote);
  Rc = ToolsSetDataConnection (pProcData, 
                               szLocal, 
                               FTPMODE_APPEND,
                               cType,
                               0,           /* Skip restart command */
                               szTmp);  
  if (Rc!=FTPERR_OK)       RETURN(pProcData,Rc); 
  
  pProcData->File.bNotify = bNotify;
  pProcData->File.lPos = 0;
  pProcData->File.lTotal = FtpGetFileSize ();
  
  if (pProcData->File.bAsyncMode)
   {
     PostMessage (pProcData->hFtpWnd, WMFTP_RECV, 0,(LPARAM)pProcData);
     return FTPERR_OK;
   }
  else return  FtpSyncRecv (pProcData);
} /* FtpAppendToLocalFile */



/* ------------------------------------------------------------ */
/* Fonction DLL    FtpAppendToRemoteFile                        */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpAppendToRemoteFile (LPCSTR szLocal, LPCSTR szRemote,
                                    char cType, BOOL bNotify,
                                    HWND hParentWnd, UINT wMsg)
{
LPProcData pProcData;
int        Rc;
char       szTmp [FTP_REPSTRLENGTH];

  pProcData = FtpDataPtr ();
  if (pProcData == NULL)    return FTPERR_NOTINITIALIZED;
  if (pProcData->ftp.ctrl_socket==INVALID_SOCKET)   return FTPERR_NOTCONNECTED;

  pProcData->Msg.nCompletedMessage = wMsg;
  pProcData->Msg.hParentWnd = hParentWnd;

  wsprintf ( szTmp, "APPE %s", szRemote);
  Rc = ToolsSetDataConnection (pProcData, 
                               szLocal, 
                               FTPMODE_READ,
                               cType, 
                               0,           /* Skip restart command */
                               szTmp);
  if (Rc!=FTPERR_OK)    RETURN(pProcData, Rc);

  pProcData->File.bNotify = bNotify;
  pProcData->File.lPos = 0;
  pProcData->File.lTotal = _llseek (pProcData->File.hf, 0, SEEK_END);
  _llseek (pProcData->File.hf, 0, SEEK_SET);

  if (pProcData->File.bAsyncMode)
   {
     PostMessage (pProcData->hFtpWnd, WMFTP_SEND, 0,(LPARAM)pProcData);
     return FTPERR_OK;
   }
  else return  FtpSyncSend (pProcData);
} /* FtpAppendToRemoteFile */



/* ------------------------------------------------------------ */
/* Fonction DLL    FtpRestartRecvFile                           */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpRestartRecvFile (LPCSTR szRemote, HFILE hLocal, 
                                           char cType, BOOL bNotify,
                                           long lByteCount,
                                           HWND hParentWnd, UINT wMsg)
{
LPProcData pProcData;
int        Rc;
char       szTmp [FTP_REPSTRLENGTH];

  pProcData = FtpDataPtr ();
  if (pProcData == NULL)    return FTPERR_NOTINITIALIZED;
  if (pProcData->ftp.ctrl_socket==INVALID_SOCKET)   return FTPERR_NOTCONNECTED;

  pProcData->Msg.nCompletedMessage = wMsg;
  pProcData->Msg.hParentWnd = hParentWnd;

  wsprintf (szTmp, "RETR %s", szRemote);
  Rc = ToolsSetDataConnection (pProcData,
                               NULL,            /* no file to be opened */
                               FTPMODE_WRITE,   /* unused */
                               cType,
                               lByteCount,      /* Restart command */
                               szTmp);
  if (Rc!=FTPERR_OK)     RETURN(pProcData,Rc); 
  
  pProcData->File.hf = hLocal;
  pProcData->File.bNotify = bNotify;
  
  pProcData->File.lPos    = 0;
  pProcData->File.lTotal  = FtpGetFileSize () - lByteCount;
  
  if (pProcData->File.bAsyncMode)
   {
     PostMessage (pProcData->hFtpWnd, WMFTP_RECV, 0,(LPARAM)pProcData);
     return FTPERR_OK;
   }
  else return  FtpSyncRecv (pProcData);
} /* FtpRestartRecvFile */



/* ------------------------------------------------------------ */
/* Fonction DLL    FtpRestartSendFile                           */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpRestartSendFile (HFILE hLocal, LPCSTR szRemote, 
                                          char cType, BOOL bNotify,
                                          long lByteCount,
                                          HWND hParentWnd, UINT wMsg)
{
LPProcData pProcData;
int        Rc;
char       szTmp [FTP_REPSTRLENGTH];
long       lFilePos;

  pProcData = FtpDataPtr ();
  if (pProcData == NULL)    return FTPERR_NOTINITIALIZED;
  if (pProcData->ftp.ctrl_socket==INVALID_SOCKET)   return FTPERR_NOTCONNECTED;

  pProcData->Msg.nCompletedMessage = wMsg;
  pProcData->Msg.hParentWnd = hParentWnd;

  wsprintf ( szTmp, "STOR %s", szRemote);
  Rc = ToolsSetDataConnection (pProcData, 
                               NULL,            /* no file to be opened */       
                               FTPMODE_READ,    /* unused */
                               cType, 
                               lByteCount,      /* Restart command */
                               szTmp);
  if (Rc!=FTPERR_OK)    RETURN(pProcData, Rc);

  pProcData->File.hf = hLocal;
  pProcData->File.bNotify = bNotify;
  
  /* le nombre d'octets transférés doit partir de 0. En revanche, le nombre  */
  /* d'octets à transférer doit tenir compte de l'avancement dans le fichier */
  lFilePos = _llseek (pProcData->File.hf, 0, SEEK_CUR);
  pProcData->File.lTotal =_llseek (pProcData->File.hf, 0, SEEK_END) - lFilePos;
  _llseek (pProcData->File.hf, lFilePos, SEEK_SET);
  pProcData->File.lPos = 0;

  if (pProcData->File.bAsyncMode)
   {
     PostMessage (pProcData->hFtpWnd, WMFTP_SEND, 0,(LPARAM)pProcData);
     return FTPERR_OK;
   }
  else return  FtpSyncSend (pProcData);
} /* FtpRestartSendFile */


/* ------------------------------------------------------------ */
/* Fonction DLL    FtpDir                                       */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpDir (LPCSTR szDef, LPCSTR szLocalFile,
                               BOOL  bLongDir, 
                               HWND  hParentWnd, UINT wMsg)
{
LPProcData pProcData;
int        Rc;
char       szTmp [FTP_REPSTRLENGTH];

  pProcData = FtpDataPtr ();
  if (pProcData == NULL)  return FTPERR_NOTINITIALIZED;
  if (pProcData->ftp.ctrl_socket==INVALID_SOCKET)   return FTPERR_NOTCONNECTED;

  pProcData->File.bNotify  = FALSE;
  pProcData->Msg.nCompletedMessage = wMsg;
  pProcData->Msg.hParentWnd = hParentWnd;
  /* synchronous mode -> data must be sent into a file */
  if (! pProcData->File.bAsyncMode  && szLocalFile==NULL)
      return FTPERR_INVALIDPARAMETER;

  if (szDef!=NULL && szDef[0]!=0)
        wsprintf ( szTmp, "%s %s", (LPSTR) (bLongDir ? "LIST" : "NLST"), szDef);
  else  lstrcpy (szTmp, bLongDir ? "LIST" : "NLST");
  Rc = ToolsSetDataConnection (pProcData, 
                               szLocalFile, 
                               FTPMODE_WRITE,
                               TYPE_A, 
                               0,
                               szTmp);
  if (Rc!=FTPERR_OK)     RETURN (pProcData, Rc);
  /* On complete la structure */
  pProcData->File.lPos = 0;
  pProcData->File.lTotal = 0;
  if (pProcData->File.bAsyncMode)
    {
      PostMessage (pProcData->hFtpWnd, 
                   szLocalFile==NULL ? WMFTP_DIR : WMFTP_RECV,
                   0, (LPARAM) pProcData);  
      return FTPERR_OK;
    }
  else return FtpSyncRecv (pProcData);
} /* FtpDir */




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* FtpMGet                                                                 */
/* Receptionne tous les fichiers du repertoire courant                     */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int _export PASCAL FAR FtpMGet (LPCSTR szFilter, char cType, BOOL bNotify,
                     BOOL (CALLBACK *f) (LPCSTR szRemFile, LPCSTR szLocalFile, int Rc) ) 
{
char  szDir [FTP_REPSTRLENGTH];
char  szDirTo [FTP_REPSTRLENGTH];
int   Ark;
int   Rc;
char  szTempDirFile [144]; /* nom du fichier temporaire  */
HFILE hTmpFile;       /* handler fichier temporaire */
LPProcData pProcData;
LPSTR p;

   if (FtpIsAsynchronousMode ())  return FTPERR_ASYNCMODE;  

   pProcData = FtpDataPtr ();
   if (pProcData == NULL)  return FTPERR_NOACTIVESESSION;

   /* on conserve l'ancien mode */
   GetTempFileName (GetTempDrive(0), "4w", 0, szTempDirFile); 
   /* NLST sur le filtre propose */
   Rc = FtpDir (szFilter, szTempDirFile, FALSE, 0, 0);
   if (Rc != FTPERR_OK)
    {
      unlink (szTempDirFile);
      return  FTPERR_CANTOPENLOCALFILE;
    }  /* error in Dir command */
       
   /* dir command is OK: list of files to be received is in szTmpFile */   
   hTmpFile = _lopen (szTempDirFile, OF_READ);
   if (hTmpFile==HFILE_ERROR)  
    {
      unlink (szTempDirFile);
      return  FTPERR_CANTOPENLOCALFILE;
    }  /* can not open temporary file (?)  */
   do
     {
       memset (szDir, 0, sizeof szDir);
       Ark = _lread (hTmpFile, szDir, sizeof szDir - 1);
       if (Ark<=0)     break;
       p = strchr (szDir, '\n');
       if (p == NULL)  break;
       *p=0;   /* fin de chaine */
       if (*(p-1)=='\r')  *(p-1)=0;   /* precaution */  
       /* revenir sur le debut de la ligne suivante */
      _llseek (hTmpFile, 1 + (int) (p - szDir) - Ark, 1);
       lstrcpy (szDirTo, szDir);
       Rc = FTPERR_OK; 
       if (f == NULL    ||   (*f) (szDir, szDirTo, FTPERR_ENDOFDATA))
            Rc = FtpRecvFile (szDir, szDirTo, cType, bNotify, 0, 0);
     } /* reste des donnees a lire */
   while (  (Rc==FTPERR_OK && f == NULL) || (*f) (szDir, szDirTo, Rc) );
   _lclose (hTmpFile);
   unlink (szTempDirFile);
return Rc;
} /* FtpMGFet */                                
