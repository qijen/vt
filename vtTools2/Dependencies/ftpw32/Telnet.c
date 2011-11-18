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



/* -------------------------------------- */
/* Acces a TnSend depuis Ftp4w            */
/* -------------------------------------- */
int _export PASCAL FAR IntTnSend (SOCKET s, LPSTR szString, 
                                  BOOL bHighPriority, HFILE hf)
{
int  Rc;
LPProcData p = FtpDataPtr();

	Rc = TnSend (s, szString, bHighPriority, hf);
    if (p != NULL  && p->ftp.bVerbose)
    {
  		OutputDebugString (szString);
  		OutputDebugString ("\r\n");
  	}
return Rc;
}



/* ******************************************************************* */
/*                                                                     */
/* Partie III : Etage Telnet avec utilisation de la structure FtpData  */
/*              Traitement du mode Verbose                             */
/*                                                                     */
/* ******************************************************************* */

/* ------------------------------------------------------------ */
/* FtpGetAnswerCode : Attend une réponse du serveur et rend le  */
/*                    code numérique (les 3 premiers octets)    */
/*                    renvoyé par le serveur.                   */
/*                  - Le mode verbose consiste à envoyer        */
/*                    l'adresse du buffer à l'utilisateur       */
/*                  - Retourne un nombre entre 100 et 999 si la */
/*                    fonction s'est bien passée, sinon un code */
/*                    d'erreur TN_ERROR, TN_TIMEOUT...          */
/* ------------------------------------------------------------ */
int _export PASCAL FAR IntFtpGetAnswerCode (LPFtpData pFtpData)
{
int      Rc;

   memset (pFtpData->szInBuf, 0, sizeof pFtpData->szInBuf);
   Rc = TnGetAnswerCode (pFtpData->ctrl_socket, pFtpData->szInBuf, 
                         sizeof pFtpData->szInBuf, pFtpData->nTimeOut,
                         pFtpData->hLogFile);

   if (Rc>0 && pFtpData->bVerbose)
   { 
         LPProcData p = FtpDataPtr();
           if (p != NULL)
              SendMessage (p->VMsg.hVerboseWnd, p->VMsg.nVerboseMsg,
                           TRUE, (LPARAM) pFtpData->szInBuf);
  		OutputDebugString (pFtpData->szInBuf);
  		OutputDebugString ("\r\n");
   }
return Rc;
} /* IntFtpGetAnswerCode */


