
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                     */
/*                                                                     */
/*          D L L    F T P                                             */
/*                                                                     */
/*                       W I N D O W S                                 */
/*                                                                     */
/*                               P o u r     A r t h i c               */
/*                                                                     */
/*                                       V e r s i o n     3 . 0  1    */
/*                                                                     */
/*                                                                     */
/*                                                                     */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/* Version 1.2 ------------------------------------------------------------- */
/*             23.02.95 : Correction d'un mauvais code de retour             */
/*             27.02.95 : Ajout du code de retour 250 pour les transferts de */
/*                        fichiers (retour de MVS)                           */
/*             02.03.95 : Ajout de la fonction Ftp4wVer                      */
/*             07.03.95 : FtpRelease : Destruction de la fenêtre avant       */
/*                        l'appel à UnregisterClass                          */

/* Version 2.0 ------------------------------------------------------------- */
/*             10.03.95 : Réorganisation du code                             */
/*             14.03.95 : Ajout de FtpPWD, FtpMKD, FtpRMD, FtpCDUP           */
/*                        Rajout des lignes                                  */
/*                             case 502,504: return FTPERR_CMDNOTIMPLEMENTED */
/*                        Ajout de FtpDelete                                 */
/*             15.03.95 : Ajout du mode Passif                               */
/*                        Reste peut-être une petite erreur : si le fichier  */
/*                        n'existe pas, la socket n'est pas forcément fermée */
/*             16.03.95 : Suppression des warnings de compilation            */
/*             27.03.95 : Ajout de la commande FtpSyst                       */
/*             27.03.95 : Ajout de la commande FtpRenameFile                 */
/*             29.03.95 : Modification du fonctionnement de FtpSyst          */
/*             29.03.95 : Ajout de la fonction FtpAppendToRemoteFile         */
/*             29.03.95 : Correction d'un bug dans la fonction Ftp4wVer      */
/*             31.03.95 : WSACleanUp est appele apres les close dans WEP     */
/* Version 2.1 ------------------------------------------------------------- */
/*              3.04.95 : Utilisation de la bibliotheque Tcp4w/Tn4w          */
/*              4.04.95 : Fonctionnement en Automate                         */
/* Version 2.2 ------------------------------------------------------------- */
/*              7.04.95 : Ajout de FtpAppendToLocalFile                      */
/*                        Ajout des constantes FTPMODE_APPEND, FTPMODE_READ  */
/*             12.04.95 : Elimination du delai sur reception synchrone       */ 
/*             21.04.95   Integration des primitives Telnet                  */
/*             28.04.95   Modification des parametres Slices                 */
/*              6.06.95   Modification pour le support de LAN WorkPlace      */
/*             12.06.95   Compilation pour Version 2.2j (CICA)               */ 
/*             17.07.95   Correction du bug SetDefaultPort Version 2.2k      */
/*             08.08.95   Bug: Changement de la macro RETURN                 */
/* Version 2.3 ------------------------------------------------------------- */
/*             29.08.95   Suppression du WEP                                 */ 
/*             29.08.95   Ajout de la fonction FtpSendAccount                */ 
/*             12.10.95   changement de fonctionnement du TcpClose           */
/*                        Ajout d'un type par defaut dans Recv/Send          */
/*              3.11.95   FtpOpenConnection retourne FTERR_CANCELBYUSER, si  */
/*                        il est interrompu par un FtpAbort                  */
/* Version 2.4 ------------------------------------------------------------- */
/*             16.11.95   Implementation des commandes FtpRestartSend/Recv   */
/*                        Ajout du parametre lByte ds ToolsSetDataConnection */
/*                        Implementation de la commande FtpRestart           */ 
/*                        Extension de l'utilisation de Tcp4w                */
/*             21.11.95   Correction dans le Calcul de la jauge (%) pour     */
/*                        FtpRestartSendFile                                 */
/*             25.11.95   Portage 32 bits: Ajout des fichiers Port32.h et    */
/*                        Port32.c                                           */
/* Version 2.5 ------------------------------------------------------------- */
/*              8.02.96   Ajout des commandes FtpOpenDataConnection,         */
/*                        FtpCloseDataConnection, FtpSendThroughDataConn...  */
/*             20.02.96   AJout de FtpFirewallLogin                          */ 
/*             14.03.96   Ajout de FtpErrorString, FtpMGet (module WFTP4W_E) */
/*              1.04.96   Ajout de FtpBufferPtr                              */
/*             10.04.96   Utilisation de Tcp4u Version 2.0                   */
/* Version 2.6 ------------------------------------------------------------- */
/*             22.05.96   Passage en environnement Gulliver                  */
/*             11.06.96   Externalisation de IntFtpGetAnswerCode             */
/*                        et création de IntTnSend (pour Firewall code)      */
/*             12.06.96   Elimination du htons devant FtpSetDefaultPort      */
/* Version 2.7 ------------------------------------------------------------- */
/*             05.08.96   Premier essai de MultiThread avec FtpMtInit        */
/*						  Correction d'une mauvaise detection de timeout     */
/*             27.11.96   Ajout du code retour 257 sur RMD (WAR FTP server)  */
/*			   06.12.96   Détails codes retour sur Get(Connect/Listen)Socket */

/* Version 3.0 ------------------------------------------------------------- */
/*			   31.01.97   Nouvelle annee, nouvelle version                   */
/*                        Fin du suppotr de la DLL 16 bits -> V2.7           */
/*						  Division du code en petits modules                 */
/*			   06.12.97   Ajout du code 230 Acct Unisys                      */
/*			   08.12.97   Utilisation du Debug de Tcp4u                      */


#define  FTP4W_INCLUDES_AND_GENERAL +

#include <windows.h>
#include <windowsx.h>

#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <winsock.h>

#include <tcp4w.h>      /* external header file */ 

#include "port32.h"    /* 16/32 bits           */
#include "ftp4w.h"     /* external header file */ 
#include "ftp4w_in.h"  /* internal header file */
#include "rfc959.h"    /* only for error codes */ 


#define FTP4W_CLASS_NAME  "Ftp4wClassName"


/* ------------------------------------------------------------------------- */
/* Variables                                                                 */
/* ------------------------------------------------------------------------- */

/* Pointeur sur l'index de la première connexion */
LPProcData pFirstProcData=NULL;
                      
int Protection (void);







/* ******************************************************************* */
/*                                                                     */
/* Partie IV : Allocations/libérations des ressources                  */
/*                                                                     */
/* ******************************************************************* */


/* --------------------------------------------------------------*/
/* Fonction DLL    FtpInit                                       */
/*                 Création d'une structure de travail           */
/*                 ouverture d'une fenêtre interne               */
/*                 Initialisation de la structure                */
/* ------------------------------------------------------------- */
int _export PASCAL FAR FtpInit (HWND hParentWnd)
{
LPProcData         pProcData, pLast;
HWND               hFtpWnd;
int                Rc;
    
   if (ToolsLocateProcData () != NULL)  return FTPERR_SESSIONUSED;

   Rc=Tcp4uInit ();
   if (Rc!=TCP4U_SUCCESS)      return FTPERR_WINSOCKNOTUSABLE;  

   if (pFirstProcData==NULL || IsBadWritePtr(pFirstProcData, sizeof *pFirstProcData))
     {                  
       pProcData = pFirstProcData = Calloc (1, sizeof *pProcData);
       if (pProcData==NULL)  return FTPERR_INSMEMORY;
       pProcData->Next = pProcData->Prev = NULL;
     }
   else 
     {
       for ( pLast=pFirstProcData ; pLast->Next!=NULL ; pLast= pLast->Next );
       pLast->Next = pProcData = Calloc (1, sizeof *pProcData);
       if (pProcData==NULL)  return FTPERR_INSMEMORY;
       pProcData->Prev = pLast;
       pProcData->Next = NULL;
     }

   /* Get task information */      
   pProcData->hInstance       = GetTaskInstance (hParentWnd); /* port32.c */
   /* fIdentifyThread callback sur l'appelant, nThread sa valeur */
   pProcData->fIdentifyThread = GetCurrentThreadId;
   pProcData->nThreadIdent    = GetCurrentThreadId();


   /* est-ce que la fenetre interne a deja ete creee */
   if (pProcData==pFirstProcData)
   {WNDCLASS           wndClass;
	  /* lstrcpy (szFTPDLL_CLASSNAME, FTP4W_CLASS_NAME); */
	  /* EnregistreClasse : Enregistre la classe de notre fenêtre     */
	  memset (& wndClass, 0, sizeof wndClass); 
	  wndClass.lpszClassName = FTP4W_CLASS_NAME;
	  wndClass.hInstance     = pProcData->hInstance;
	  wndClass.lpfnWndProc   = (WNDPROC) DLLWndProc;
      Rc = RegisterClass (&wndClass);
	  if (! Rc)   { FtpRelease();   return FTPERR_CANTCREATEWINDOW; } 
	}
#ifdef UNE_FENETRE   
   if (pProcData==pFirstProcData)
   {
#endif   
     /* création de la fenêtre */    
      hFtpWnd=CreateWindow (FTP4W_CLASS_NAME,/* window class name        */
                          "", 0,             /* window title, style      */
                          0, 0, 0, 0,        /* x, y, cx, xy             */
                          hParentWnd,        /* a parent for this window */
                          NULL,              /* use the class menu       */
                          pProcData->hInstance, /* who created this wndw */
                          NULL  );           /* no parms to pass on      */
	  if (hFtpWnd==NULL)   
      {  
         FtpRelease ();
         return FTPERR_CANTCREATEWINDOW;
      }
	  pProcData->hFtpWnd         = hFtpWnd;          
#ifdef UNE_FENETRE   
   } /* pas d'instance deja cree -> creation de la fenetre interne */
	   /* si la fenetre interne existe, on la reutilise simplement */ 
   else
	   pProcData->hFtpWnd = pFirstProcData->hFtpWnd;
#endif

   /* On remplit les infos relatives à la tâche */  
   pProcData->hParentWnd      = hParentWnd;
   /* renseignements FTP : Pas de session existante */
   pProcData->ftp.ctrl_socket     = INVALID_SOCKET;
   pProcData->ftp.data_socket     = INVALID_SOCKET;
   /* juste pour noyer l'appel */
   pProcData->ftp.bVerbose        = Protection (); /* Protection rend FALSE */
   pProcData->ftp.nTimeOut        = FTP_DEFTIMEOUT;
   pProcData->ftp.hLogFile        = HFILE_ERROR;
   pProcData->ftp.bPassif         = FALSE;
   pProcData->ftp.nPort           = FTP_DEFCTRLPORT;
   /* renseignement fichier */
   pProcData->File.bAborted   = FALSE;
   pProcData->File.bNotify    = FALSE;
   pProcData->File.bAsyncMode = TRUE;
   pProcData->File.hf         = HFILE_ERROR;
   pProcData->File.nAsyncAlone= FTP_DEFASYNCALONE;
   pProcData->File.nAsyncMulti= FTP_DEFASYNCMULTI;
   pProcData->File.nDelay     = FTP_DEFDELAY;
return FTPERR_OK;
};  /* FtpInit */



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* tentative de Multithread                                          */
/* L'utilisateur donne un callback retournant un DWORD (typiquement  */
/* GetCurrentThreadId() appele a chaque appel Ftp4w32				 */
/* Cet appel n'est pas fait dans la DLL car l'id retourne par Windows*/
/* serait identique d'uin thread a l'autre (pas de difference a      */
/* l'interieur d'une DLL											 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Note warning "voulu" en compil 16 bits -> THREADID -> HANDLE */
/* et déclaration en DWORD dans le .H                           */
int _export PASCAL FAR FtpMtInit ( HWND hParentWnd, THREADID (CALLBACK *f)(void) )
{
LPProcData pProcData;

  /* Init comme d'habitude */
  int Rc=FtpInit (hParentWnd);
  if (Rc==FTPERR_OK) 
    {
	  /* et ecrasement des identifiants de thread */
      pProcData = ToolsLocateProcData ();
      pProcData->fIdentifyThread = f;
      pProcData->nThreadIdent = (*f) ();
    }
return Rc;  
} /* FtpMtInit */



/* ------------------------------------------------------------ */
/* Fonction DLL    FtpRelease                                   */
/*                 Libère les ressources Windows (fenêtre et    */
/*                 structure) prises par FtpInit                */
/* ------------------------------------------------------------ */

int _export PASCAL FAR FtpRelease (void)
{
LPProcData pProcData;

   pProcData = ToolsLocateProcData ();
   if (pProcData == NULL)  return FTPERR_NOACTIVESESSION;

   if (Tcp4uCleanup ()!=TCP4U_SUCCESS)  return FTPERR_STILLCONNECTED;
   

   /* si une seule appli est enregistre detruire la fenetre */
#ifdef UNE_FENETRE
   if (pProcData==pFirstProcData  && pProcData->Next==NULL)
   {
	 DestroyWindow (pProcData->hFtpWnd);  /* Avant le UnregisterClass */
     UnregisterClass (FTP4W_CLASS_NAME, pProcData->hInstance);
   }
#else   
   DestroyWindow (pProcData->hFtpWnd);  /* Avant le UnregisterClass */
   if (pProcData==pFirstProcData  && pProcData->Next==NULL)
	     UnregisterClass (FTP4W_CLASS_NAME, pProcData->hInstance);
#endif	 
   if (pProcData->Next != NULL)  pProcData->Next->Prev = pProcData->Prev;
   if (pProcData->Prev != NULL)  pProcData->Prev->Next = pProcData->Next;
   else                          pFirstProcData = pProcData->Next;
   Free (pProcData);
return FTPERR_OK;
} /* FtpRelease */




/* ------------------------------------------------------------ */
/* Fonction DLL    FtpLocalClose                                */
/*                 Ferme les ressources Winsockets              */
/* ------------------------------------------------------------ */
int _export PASCAL FAR FtpLocalClose (void)
{
LPProcData pProcData;
   pProcData = ToolsLocateProcData ();
   if (pProcData == NULL)  return FTPERR_NOTINITIALIZED;
   FtpCloseFileTransfer (pProcData, FALSE);
   TcpClose (& pProcData->ftp.ctrl_socket);
return    pProcData->ftp.ctrl_socket==INVALID_SOCKET 
       && pProcData->ftp.data_socket==INVALID_SOCKET ? 
          FTPERR_OK : FTPERR_CANTCLOSE;
} /* FtpLocalClose */


     
/* ----------------------------------------------------------- */
/* Procédure de fin de vie de la DLL                           */
/*      Libération des ressources Windows en cours             */
/*      On essaie aussi de libérer les sockets utilisées       */
/*      La procédure doit rendre 1 pour que le module soit     */
/*      libéré                                                 */
/* ----------------------------------------------------------- */
int _export CALLBACK WEP (int nType)
{
 nType=1;  /* suppress warning */
return 1;
} /* WEP */




/* --------------------------------------------------------------*/
/* Fonction DLL    FtpNOOP                                       */
/*                 Vide le buffer de control                     */
/* ------------------------------------------------------------- */
int _export PASCAL FAR FtpFlush (void)
{
LPProcData pProcData;
   pProcData = ToolsLocateProcData ();
   if (pProcData == NULL)  return FTPERR_NOTINITIALIZED;
   TcpFlush (pProcData->ftp.ctrl_socket);
return FTPERR_OK;
} /* FtpFlush () */



