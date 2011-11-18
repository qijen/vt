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


#include <tcp4w.h>      /* external header file */ 

#include "port32.h"    /* 16/32 bits           */
#include "ftp4w.h"     /* external header file */ 
#include "ftp4w_in.h"  /* internal header file */
#include "rfc959.h"    /* only for error codes */ 

extern LPProcData pFirstProcData;


/* ******************************************************************* */
/*                                                                     */
/* Partie V : Utilitaires Applicatifs                                  */
/*                                                                     */
/* ******************************************************************* */


/* ----------------------------------------------------------- */
/* ToolsLocateProcData :  Retrouve la structure de données de  */
/*                        la session                           */
/* ----------------------------------------------------------- */
LPProcData ToolsLocateProcData (void)
{
LPProcData pProcData;
  for ( pProcData = pFirstProcData ;
            pProcData != NULL    
        && (*pProcData->fIdentifyThread)()!= pProcData->nThreadIdent ;
        pProcData = pProcData->Next );
return pProcData;
} /* ToolsLocateProcData */



/* ------------------------------------------------------------ */
/* Fonction DLL     FtpDataPtr                                  */
/*                  Renvoie un pointeur sur la structure        */
/* ------------------------------------------------------------ */
LPProcData _export PASCAL FAR FtpDataPtr (void)
{
return ToolsLocateProcData ();
} /* FtpDataPtr */


/* ------------------------------------------------------------ */
/* Fonction DLL     FtpBufferPtr                                */
/*                  Renvoie un pointeur sur le buffer de donnees*/
/* ------------------------------------------------------------ */
LPSTR _export PASCAL FAR FtpBufferPtr (void)
{
return ToolsLocateProcData()->ftp.szInBuf;
} /* FtpDataPtr */



/* -------------------------------------------------------------- */
/* Fonctions DLL  de  parametrage                                 */
/* -------------------------------------------------------------- */
/* Remplace les defines suivants                                  */
/* - FtpBytesTransferred()     FtpDataPtr()->File.lPos            */
/* - FtpBytesToBeTransferred() FtpDataPtr()->File.lTotal          */
/* - FtpSetDefaultTimeOut(x)   FtpDataPtr()->ftp.nTimeOut=x       */
/* - FtpSetDefaultPort(x)      FtpDataPtr()->ftp.nPort=x          */
/* - FtpSetAsynchronousMode()  FtpDataPtr()->File.bAsyncMode=TRUE */
/* - FtpSetSynchronousMode()   FtpDataPtr()->File.bAsyncMode=FALSE*/
/* - FtpIsAsynchronousMode()   FtpDataPtr()->File.bAsyncMode      */
/* - FtpSetNewDelay(x)         FtpDataPtr()->File.nDelay=x        */
/* - FtpSetNewSlices(x,y)      FtpDataPtr()->File.nAsyncAlone=x,\ */
/*                             FtpDataPtr()->File.nAsyncAlone=y   */
/* -------------------------------------------------------------- */

/* redefinition of bad spelled functions */
long _export PASCAL FAR FtpBytesTransfered (void)
{ return FtpBytesTransferred(); }
long _export PASCAL FAR FtpBytesToBeTransfered(void)   
{ return FtpBytesToBeTransferred(); }


/* the new functions */
long _export PASCAL FAR FtpBytesTransferred (void) 
{LPProcData p = ToolsLocateProcData();
 return  p==NULL ? 0 :p->File.lPos; }

long _export PASCAL FAR FtpBytesToBeTransferred(void)   
{LPProcData p = ToolsLocateProcData();
 return  p==NULL ? 0 :p->File.lTotal; }

void _export PASCAL FAR FtpSetDefaultTimeOut(int x)  
{  ToolsLocateProcData()->ftp.nTimeOut=x; /* x seconds */ }

void _export PASCAL FAR FtpSetDefaultPort(int x) 
{ ToolsLocateProcData()->ftp.nPort=(short) x; }

void _export PASCAL FAR FtpSetAsynchronousMode(void) 
{  ToolsLocateProcData()->File.bAsyncMode=TRUE; }

void _export PASCAL FAR FtpSetSynchronousMode(void)    
{ ToolsLocateProcData()->File.bAsyncMode=FALSE; }

BOOL _export PASCAL FAR FtpIsAsynchronousMode(void)   
{ return ToolsLocateProcData()->File.bAsyncMode; }

void _export PASCAL FAR FtpSetNewDelay(int x) 
{ ToolsLocateProcData()->File.nDelay=x; /* x millisec */ }

void _export PASCAL FAR FtpSetNewSlices(int x,int y) 
{ 
LPProcData p = ToolsLocateProcData();
 p->File.nAsyncAlone=x, p->File.nAsyncAlone=y;
}

void _export PASCAL FAR FtpSetPassiveMode (BOOL bPassif) 
{ ToolsLocateProcData()->ftp.bPassif = bPassif; }

void _export PASCAL FAR FtpLogTo (HFILE hLogFile)
{ ToolsLocateProcData()->ftp.hLogFile = hLogFile; }

void _export PASCAL FAR FtpEnableDebugging (void)
{ Tcp4uEnableLog (0xFFFF); }

/* ------------------------------------------------------------ */
/* Fonction DLL     FtpSetVerboseMode                           */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpSetVerboseMode (BOOL bMode, HWND hWnd, UINT wMsg)
{
LPProcData pProcData;

  pProcData = ToolsLocateProcData ();
  if (pProcData == NULL)  return FTPERR_NOTINITIALIZED;
  pProcData->ftp.bVerbose = bMode;
  pProcData->VMsg.hVerboseWnd = hWnd;
  pProcData->VMsg.nVerboseMsg = wMsg;
return FTPERR_OK; 
} /* SetVerboseMode */


