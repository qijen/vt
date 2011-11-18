/*****************************************************************************
*
*  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
*  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
*  A PARTICULAR PURPOSE.
*
*  Copyright (C) 1993 - 1996 Microsoft Corporation. All Rights Reserved.
*
******************************************************************************
*
* SMF.H
*
* Public include file for Standard MIDI File access routines.
*
*****************************************************************************/

#ifndef _SMF_
#define _SMF_

//#include "global.h" 

typedef DWORD SMFRESULT;
typedef DWORD TICKS;
typedef TICKS  *PTICKS;
typedef BYTE  *HPBYTE;

#define MAX_TICKS           ((TICKS)0xFFFFFFFFL)

#define SMF_SUCCESS         (0L)
#define SMF_INVALID_FILE    (1L)
#define SMF_NO_MEMORY       (2L)
#define SMF_OPEN_FAILED     (3L)
#define SMF_INVALID_TRACK   (4L)
#define SMF_META_PENDING    (5L)
#define SMF_ALREADY_OPEN    (6L)
#define SMF_END_OF_TRACK    (7L)
#define SMF_NO_META         (8L)
#define SMF_INVALID_PARM    (9L)
#define SMF_INVALID_BUFFER  (10L)
#define SMF_END_OF_FILE     (11L)
#define SMF_REACHED_TKMAX   (12L)

DECLARE_HANDLE(HSMF);

typedef struct tag_smfopenstruct
{
    LPSTR               pstrName;
    DWORD               dwTimeDivision;
    HSMF                hSmf;
}   SMFOPENFILESTRUCT,
    *PSMFOPENFILESTRUCT;

extern SMFRESULT smfOpenFile(
    PSMFOPENFILESTRUCT  psofs);

extern SMFRESULT  smfCloseFile(
    HSMF                hsmf);

typedef struct tag_smffileinfo
{
    DWORD               dwTracks;
    DWORD               dwFormat;
    DWORD               dwTimeDivision;
    TICKS               tkLength;
}   SMFFILEINFO,
    *PSMFFILEINFO;

extern SMFRESULT  smfGetFileInfo(
    HSMF                hsmf,
    PSMFFILEINFO        psfi);

extern DWORD  smfTicksToMillisecs(
    HSMF                hsmf,
    TICKS               tkOffset);

extern DWORD  smfMillisecsToTicks(
    HSMF                hsmf,
    DWORD               msOffset);

extern SMFRESULT  smfReadEvents(
    HSMF                hsmf,
    LPMIDIHDR           lpmh,
    TICKS               tkMax);

extern SMFRESULT  smfSeek(
    HSMF                hsmf,
    TICKS               tkPosition,
    LPMIDIHDR           lpmh);

extern DWORD  smfGetStateMaxSize(
    void);

/* Buffer described by LPMIDIHDR is in polymsg format, except that it
** can contain meta-events (which will be ignored during playback by
** the current system). This means we can use the pack functions, etc.
*/
#define PMSG_META       ((BYTE)0xC0)

#endif
