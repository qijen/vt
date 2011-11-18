#include "CKAll.h"



#define SNDINTERNAL
/*****************************************************************************
*
*  pSeq CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
*  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED
*  TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR
*  A PARTICULAR PURPOSE.
*
*  Copyright (C) 1993 - 1996 Microsoft Corporation. All Rights Reserved.
*
******************************************************************************
*
*MidiSound::uence.C
*
*MidiSound::uencer engine for MIDI player app
*
*****************************************************************************/

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <limits.h>

#include "MidiSound.h"

static void FAR PASCAL seqMIDICallback(HMIDISTRM hms, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
static MMRESULT  XlatSMFErr(SMFRESULT smfrc);


extern CRITICAL_SECTION  gMidiCS;

// Fichier de son associe
 MMRESULT MidiSound::SetSoundFileName(const char * name) {
	 if (name==NULL) {
		 return MCIERR_INVALID_FILE;
	 }

	pSeq->pstrFile=CKStrdup((char *)name);
	return OpenFile();
}

 const char* MidiSound::GetSoundFileName() {
	return pSeq->pstrFile;
}
	

BOOL MidiSound::IsPlaying()
{
	return (pSeq->uState==SEQ_S_PLAYING);
}

BOOL MidiSound::IsPaused()
{
	return (pSeq->uState==SEQ_S_PAUSED);
}

/***************************************************************************
*  
*MidiSound::AllocBuffers
*
* Allocate buffers for pSeq instance.
*
* pSeq                      - TheMidiSound::uencer instance to allocate buffers for.
*
* Returns
*   MMSYSERR_NOERROR If the operation was successful.
*
*   MCIERR_OUT_OF_MEMORY  If there is insufficient memory for
*     the requested number and size of buffers.
*
*MidiSound::AllocBuffers allocates playback buffers based on the
* cbBuffer and cBuffer fields of pSeq. cbBuffer specifies the
* number of bytes in each buffer, and cBuffer specifies the
* number of buffers to allocate.
*
*MidiSound::AllocBuffers must be called before any otherMidiSound::uencer call
* on a newly alloctedMidiSound::UENCE structure. It must be paired with
* a call toMidiSound::FreeBuffers, which should be the last call made
* before theMidiSound::UENCE structure is discarded.
*
***************************************************************************/
 MMRESULT MidiSound::AllocBuffers()
{
    DWORD                   dwEachBufferSize;
    DWORD                   dwAlloc;
    UINT                    i;
    LPBYTE                  lpbWork;

    //assert(pSeq != NULL);

    pSeq->uState    =SEQ_S_NOFILE;
    pSeq->lpmhFree  = NULL;
    pSeq->lpbAlloc  = NULL;
    pSeq->hSmf      = (HSMF)NULL;
    
    /* First make sure we can allocate the buffers they asked for
    */
    dwEachBufferSize = sizeof(MIDIHDR) + (DWORD)(pSeq->cbBuffer);
    dwAlloc          = dwEachBufferSize * (DWORD)(pSeq->cBuffer);
    
    pSeq->lpbAlloc = (unsigned char*) GlobalAllocPtr(GMEM_MOVEABLE|GMEM_SHARE, dwAlloc);
    if (NULL == pSeq->lpbAlloc)
        return MCIERR_OUT_OF_MEMORY;

    /* Initialize all MIDIHDR's and throw them into a free list
    */
    pSeq->lpmhFree = NULL;

    lpbWork = pSeq->lpbAlloc;
    for (i=0; i < pSeq->cBuffer; i++)
    {
        ((LPMIDIHDR)lpbWork)->lpNext            = pSeq->lpmhFree;

        ((LPMIDIHDR)lpbWork)->lpData            = (char*)lpbWork + sizeof(MIDIHDR);
        ((LPMIDIHDR)lpbWork)->dwBufferLength    = pSeq->cbBuffer;
        ((LPMIDIHDR)lpbWork)->dwBytesRecorded   = 0;
        ((LPMIDIHDR)lpbWork)->dwUser            = (DWORD)(UINT)pSeq;
        ((LPMIDIHDR)lpbWork)->dwFlags           = 0;

        pSeq->lpmhFree = (LPMIDIHDR)lpbWork;

        lpbWork += dwEachBufferSize;
    }

    return MMSYSERR_NOERROR;
}

/***************************************************************************
*  
*MidiSound::FreeBuffers
*
* Free buffers for pSeq instance.
*
* pSeq                      - TheMidiSound::uencer instance to free buffers for.
*   
*MidiSound::FreeBuffers frees all allocated memory belonging to the
* givenMidiSound::uencer instance pSeq. It must be the last call
* performed on the instance before it is destroyed.
*       
****************************************************************************/
 VOID MidiSound::FreeBuffers()
{
    LPMIDIHDR               lpmh;
    
    //assert(pSeq != NULL);

    if (NULL != pSeq->lpbAlloc)
    {
        lpmh = (LPMIDIHDR)pSeq->lpbAlloc;
        //assert(!(lpmh->dwFlags & MHDR_PREPARED));
        
        GlobalFreePtr(pSeq->lpbAlloc);
    }
}

/***************************************************************************
*  
*MidiSound::OpenFile
*
* Associates a MIDI file with the givenMidiSound::uencer instance.
*
* pSeq                      - TheMidiSound::uencer instance.
*
* Returns
*   MMSYSERR_NOERROR If the operation is successful.
*    
*   MCIERR_UNSUPPORTED_FUNCTION If there is already a file open
*     on pSeq instance.
*     
*   MCIERR_OUT_OF_MEMORY If there was insufficient memory to
*     allocate internal buffers on the file.
*
*   MCIERR_INVALID_FILE If initial attempts to parse the file
*     failed (such as the file is not a MIDI or RMI file).
*
*MidiSound::OpenFile may only be called if there is no currently open file
* on the instance. It must be paired with a call toMidiSound::CloseFile
* when operations on pSeq file are complete.
*
* The pstrFile field of pSeq contains the name of the file
* to open. pSeq name will be passed directly to mmioOpen; it may
* contain a specifcation for a custom MMIO file handler. The task
* context used for all I/O will be the task which callsMidiSound::OpenFile.
*
***************************************************************************/
 MMRESULT MidiSound::OpenFile()
{                            
    MMRESULT                rc      = MMSYSERR_NOERROR;
    SMFOPENFILESTRUCT       sofs;
    SMFFILEINFO             sfi;
    SMFRESULT               smfrc;
    DWORD                   cbBuffer;

    //assert(pSeq != NULL);

    if (pSeq->uState !=SEQ_S_NOFILE)
    {
        return MCIERR_UNSUPPORTED_FUNCTION;
    }

    //assert(pSeq->pstrFile != NULL);
    
    sofs.pstrName     = pSeq->pstrFile;

    smfrc = smfOpenFile(&sofs);
    if (SMF_SUCCESS != smfrc)
    {
        rc = XlatSMFErr(smfrc);
        goto Seq_Open_File_Cleanup;
    }

    pSeq->hSmf = sofs.hSmf;
    smfGetFileInfo(pSeq->hSmf, &sfi);
    
    pSeq->dwTimeDivision = sfi.dwTimeDivision;
    pSeq->tkLength       = sfi.tkLength;
    pSeq->cTrk           = sfi.dwTracks;
               
    /* Track buffers must be big enough to hold the state data returned
    ** by smfSeek()
    */
    cbBuffer = __min(pSeq->cbBuffer, smfGetStateMaxSize());
    
Seq_Open_File_Cleanup:    
    if (MMSYSERR_NOERROR != rc)
       CloseFile();
    else
        pSeq->uState =SEQ_S_OPENED;

    return rc;
}

/***************************************************************************
*  
*MidiSound::CloseFile
*
* Deassociates a MIDI file with the givenMidiSound::uencer instance.
*
* pSeq                      -  TheMidiSound::uencer instance.
*
* Returns
*   MMSYSERR_NOERROR If the operation is successful.
*    
*   MCIERR_UNSUPPORTED_FUNCTION If theMidiSound::uencer instance is not
*     stopped.
*     
* A call toMidiSound::CloseFile must be paired with a prior call to
*MidiSound::OpenFile. All buffers associated with the file will be
* freed and the file will be closed. TheMidiSound::uencer must be
* stopped before pSeq call will be accepted.
*
***************************************************************************/
 MMRESULT MidiSound::CloseFile()
{
    LPMIDIHDR               lpmh;
    
    //assert(pSeq != NULL);
    
    if (SEQ_S_OPENED != pSeq->uState)
        return MCIERR_UNSUPPORTED_FUNCTION;
    
    if ((HSMF)NULL != pSeq->hSmf)
    {
        smfCloseFile(pSeq->hSmf);
        pSeq->hSmf = (HSMF)NULL;
    }

    /* If we were prerolled, need to clean up -- have an open MIDI handle
    ** and buffers in the ready queue
    */
	//EnterCriticalSection(&gMidiCS);
	
    for (lpmh = pSeq->lpmhFree; lpmh; lpmh = lpmh->lpNext)
        midiOutUnprepareHeader(pSeq->hmidi, lpmh, sizeof(*lpmh));

    if (pSeq->lpmhPreroll)
	{
        midiOutUnprepareHeader(pSeq->hmidi, pSeq->lpmhPreroll, sizeof(*pSeq->lpmhPreroll));
		(void) GlobalFreePtr(pSeq->lpmhPreroll);
	}

    if (pSeq->hmidi != NULL)
    {
        // cast by aymeric
		//EnterCriticalSection(&gMidiCS);			
		midiStreamClose((HMIDISTRM)pSeq->hmidi);
        pSeq->hmidi = NULL;
		//LeaveCriticalSection(&gMidiCS);		
	}
    pSeq->uState =SEQ_S_NOFILE;
	
	//LeaveCriticalSection(&gMidiCS);

    return MMSYSERR_NOERROR;
}

/***************************************************************************
*  
*MidiSound::Preroll
*
* Prepares the file for playback at the given position.
*
* pSeq                      - TheMidiSound::uencer instance.
*
* lpPreroll                 - Specifies the starting and ending tick
*                             positions to play between.
*
* Returns
*   MMSYSERR_NOERROR If the operation is successful.
*    
*   MCIERR_UNSUPPORTED_FUNCTION If theMidiSound::uencer instance is not
*     opened or prerolled.
*
* Open the device so we can initialize channels.
*
* Loop through the tracks. For each track, seek to the given position and
* send the init data SMF gives us to the handle.
*
* Wait for all init buffers to finish.
*
* Unprepare the buffers (they're only ever sent here; theMidiSound::uencer
* engine merges them into a single stream during normal playback) and
* refill them with the first chunk of data from the track. 
*
*     
****************************************************************************/
 MMRESULT MidiSound::Preroll()
{
	pSeq->tkBase=0;
	pSeq->tkEnd=pSeq->tkLength;
	struct tag_preroll prerroll={0,pSeq->tkLength};
	LPPREROLL lpPreroll=&prerroll;
    SMFRESULT           smfrc;
    MMRESULT            mmrc        = MMSYSERR_NOERROR;
    MIDIPROPTIMEDIV     mptd;
    LPMIDIHDR           lpmh = NULL;
    LPMIDIHDR           lpmhPreroll = NULL;
    DWORD               cbPrerollBuffer;
    UINT                uDeviceID=MIDI_MAPPER ;

    //assert(pSeq != NULL);

    pSeq->mmrcLastErr = MMSYSERR_NOERROR;

    if (pSeq->uState !=SEQ_S_OPENED &&
        pSeq->uState !=SEQ_S_PREROLLED)
        return MCIERR_UNSUPPORTED_FUNCTION;

	pSeq->tkBase = lpPreroll->tkBase;
	pSeq->tkEnd = lpPreroll->tkEnd;

    if (pSeq->hmidi)
    {
        // Recollect buffers from MMSYSTEM back into free queue
        //
		//EnterCriticalSection(&gMidiCS);
        pSeq->uState =SEQ_S_RESET;
        midiOutReset(pSeq->hmidi);
		//LeaveCriticalSection(&gMidiCS);
		while (pSeq->uBuffersInMMSYSTEM)
			Sleep(0);
    }
    
    pSeq->uBuffersInMMSYSTEM = 0;
    pSeq->uState =SEQ_S_PREROLLING;
    
    //
    // We've successfully opened the file and all of the tracks; now
    // open the MIDI device and set the time division.
    //
    // NOTE:MidiSound::Preroll is equivalent to seek; device might already be open
    //
    if (NULL == pSeq->hmidi)
    {
        uDeviceID = pSeq->uDeviceID;
		uDeviceID = MIDI_MAPPER;
        // cast by aymeric
		if ((mmrc = midiStreamOpen((LPHMIDISTRM)&pSeq->hmidi,
                                   &uDeviceID,
                                   1,
                                   (DWORD)seqMIDICallback,
                                   0,
                                   CALLBACK_FUNCTION)) != MMSYSERR_NOERROR)
        {
            pSeq->hmidi = NULL;
            goto Preroll_Cleanup;
        }
        
        mptd.cbStruct  = sizeof(mptd);
        mptd.dwTimeDiv = pSeq->dwTimeDivision;
        // cast by aymeric
        if ((mmrc = midiStreamProperty(
                                       (HMIDISTRM)pSeq->hmidi,
                                       (LPBYTE)&mptd,
                                       MIDIPROP_SET|MIDIPROP_TIMEDIV)) != MMSYSERR_NOERROR)
        {
            //DPF(1, "midiStreamProperty() -> %04X", (WORD)mmrc);
			//EnterCriticalSection(&gMidiCS);
				midiStreamClose((HMIDISTRM)pSeq->hmidi);            
				pSeq->hmidi = NULL;
				mmrc = MCIERR_DEVICE_NOT_READY;
            //LeaveCriticalSection(&gMidiCS);
			goto Preroll_Cleanup;
        }
    }

    mmrc = MMSYSERR_NOERROR;

    //
    //  Allocate a preroll buffer.  Then if we don't have enough room for
    //  all the preroll info, we make the buffer larger.  
    //
    if (!pSeq->lpmhPreroll)
    {
        cbPrerollBuffer = 4096;
        lpmhPreroll = (LPMIDIHDR)GlobalAllocPtr(GMEM_MOVEABLE|GMEM_SHARE,
                                                            cbPrerollBuffer);
    }
    else
    {
        cbPrerollBuffer = pSeq->cbPreroll;
        lpmhPreroll = pSeq->lpmhPreroll;
    }

    lpmhPreroll->lpNext            = pSeq->lpmhFree;
    lpmhPreroll->lpData            = (char*)lpmhPreroll + sizeof(MIDIHDR);
    lpmhPreroll->dwBufferLength    = cbPrerollBuffer - sizeof(MIDIHDR);
    lpmhPreroll->dwBytesRecorded   = 0;
    lpmhPreroll->dwUser            = (DWORD)(UINT)pSeq;
    lpmhPreroll->dwFlags           = 0;

    do
    {
        smfrc = smfSeek(pSeq->hSmf, pSeq->tkBase, lpmhPreroll);
        if( SMF_SUCCESS != smfrc )
        {
            if( ( SMF_NO_MEMORY != smfrc )  ||
                ( cbPrerollBuffer >= 32768L ) )
            {
                //DPF(1, "smfSeek() returned %lu", (DWORD)smfrc);

                GlobalFreePtr(lpmhPreroll);
                pSeq->lpmhPreroll = NULL;

                mmrc = XlatSMFErr(smfrc);
                goto Preroll_Cleanup;
            }
            else   //  Try to grow buffer.
            {
                cbPrerollBuffer *= 2;
                lpmh = (LPMIDIHDR)GlobalReAllocPtr( lpmhPreroll, cbPrerollBuffer, 0 );
                if( NULL == lpmh )
                {
                    //DPF(2,"seqPreroll - realloc failed, aborting preroll.");
                    mmrc = MCIERR_OUT_OF_MEMORY;
                    goto Preroll_Cleanup;
                }

                lpmhPreroll = lpmh;
                lpmhPreroll->lpData = (char*)lpmhPreroll + sizeof(MIDIHDR);
                lpmhPreroll->dwBufferLength = cbPrerollBuffer - sizeof(MIDIHDR);

                pSeq->lpmhPreroll = lpmhPreroll;
                pSeq->cbPreroll = cbPrerollBuffer;
            }
        }
    } while( SMF_SUCCESS != smfrc );

    if (!pSeq->lpmhPreroll)
    {
		//to keep access to buffer and free memory before closing
		pSeq->lpmhPreroll = lpmhPreroll;
		pSeq->cbPreroll = cbPrerollBuffer;
	}

    if (MMSYSERR_NOERROR != (mmrc = midiOutPrepareHeader(pSeq->hmidi, lpmhPreroll, sizeof(MIDIHDR))))
    {
        //DPF(1, "midiOutPrepare(preroll) -> %lu!", (DWORD)mmrc);

        mmrc = MCIERR_DEVICE_NOT_READY;
        goto Preroll_Cleanup;
    }

    ++pSeq->uBuffersInMMSYSTEM;

    if (MMSYSERR_NOERROR != (mmrc = midiStreamOut((HMIDISTRM)pSeq->hmidi, lpmhPreroll, sizeof(MIDIHDR))))
    {
        //DPF(1, "midiStreamOut(preroll) -> %lu!", (DWORD)mmrc);

        mmrc = MCIERR_DEVICE_NOT_READY;
        --pSeq->uBuffersInMMSYSTEM;
        goto Preroll_Cleanup;
    }
    //DPF(3,"seqPreroll: midiStreamOut(0x%x,0x%lx,%u) returned %u.",pSeq->hmidi,lpmhPreroll,sizeof(MIDIHDR),mmrc);

    pSeq->fdwSeq &= ~SEQ_F_EOF;
    while (pSeq->lpmhFree)
    {
        lpmh = pSeq->lpmhFree;
        pSeq->lpmhFree = lpmh->lpNext;

        smfrc = smfReadEvents(pSeq->hSmf, lpmh, pSeq->tkEnd);
        if (SMF_SUCCESS != smfrc && SMF_END_OF_FILE != smfrc)
        {
            //DPF(1, "SFP: smfReadEvents() -> %u", (UINT)smfrc);
            mmrc = XlatSMFErr(smfrc);
            goto Preroll_Cleanup;
        }

        if (MMSYSERR_NOERROR != (mmrc = midiOutPrepareHeader(pSeq->hmidi, lpmh, sizeof(*lpmh))))
        {
            //DPF(1, "SFP: midiOutPrepareHeader failed");
            goto Preroll_Cleanup;
        }

        if (MMSYSERR_NOERROR != (mmrc = midiStreamOut((HMIDISTRM)pSeq->hmidi, lpmh, sizeof(*lpmh))))
        {
            //DPF(1, "SFP: midiStreamOut failed");
            goto Preroll_Cleanup;
        }

        ++pSeq->uBuffersInMMSYSTEM; 

        if (SMF_END_OF_FILE == smfrc)
        {
            pSeq->fdwSeq |=SEQ_F_EOF;
            break;
        }
    } 

Preroll_Cleanup:
    if (MMSYSERR_NOERROR != mmrc)
    {
        pSeq->uState =SEQ_S_OPENED;
        pSeq->fdwSeq &= ~SEQ_F_WAITING;
    }
    else
    {
        pSeq->uState =SEQ_S_PREROLLED;
    }

    return mmrc;
}

/***************************************************************************
*  
*MidiSound::Start
*
* Starts playback at the current position.
*
* pSeq                      - TheMidiSound::uencer instance.
*
* Returns
*   MMSYSERR_NOERROR If the operation is successful.
*    
*   MCIERR_UNSUPPORTED_FUNCTION If theMidiSound::uencer instance is not
*     stopped.
*
*   MCIERR_DEVICE_NOT_READY If the underlying MIDI device could
*     not be opened or fails any call.
* 
* TheMidiSound::uencer must be prerolled beforeMidiSound::Start may be called.
*
* Just feed everything in the ready queue to the device.
*       
***************************************************************************/
 MMRESULT MidiSound::Start()
{
    if (NULL == pSeq)
	{
        return MCIERR_UNSUPPORTED_FUNCTION;
    }

    if (SEQ_S_PREROLLED != pSeq->uState)
    {
	Preroll();
	}

    if (SEQ_S_PREROLLED != pSeq->uState)
    {
        //DPF(1, "seqStart(): State is wrong! [%u]", pSeq->uState);
        return MCIERR_UNSUPPORTED_FUNCTION;
    }

    pSeq->uState =SEQ_S_PLAYING;

    return midiStreamRestart((HMIDISTRM)pSeq->hmidi);
}

/***************************************************************************
*  
*MidiSound::Pause
*
* Pauses playback of the instance.
*
* pSeq                      - TheMidiSound::uencer instance.
*
* Returns
*   MMSYSERR_NOERROR If the operation is successful.
*    
*   MCIERR_UNSUPPORTED_FUNCTION If theMidiSound::uencer instance is not
*     playing.
*
* TheMidiSound::uencer must be playing beforeMidiSound::Pause may be called.
* Pausing theMidiSound::uencer will cause all currently on notes to be turned
* off. pSeq may cause playback to be slightly inaccurate on restart
* due to missing notes.
*       
***************************************************************************/
 MMRESULT MidiSound::Pause()
{
    //assert(NULL != pSeq);
    
    if (SEQ_S_PLAYING != pSeq->uState)
        return MCIERR_UNSUPPORTED_FUNCTION;

    pSeq->uState =SEQ_S_PAUSED;

	OutputDebugString("MidiSound::Pause\n");
	
	//EnterCriticalSection(&gMidiCS);        
    midiStreamPause((HMIDISTRM)pSeq->hmidi);
    //LeaveCriticalSection(&gMidiCS);
        
    return MMSYSERR_NOERROR;
}

/***************************************************************************
*  
*MidiSound::Restart
*
* Restarts playback of an instance after a pause.
*
* pSeq                      - TheMidiSound::uencer instance.
*
* Returns
*    MMSYSERR_NOERROR If the operation is successful.
*    
*    MCIERR_UNSUPPORTED_FUNCTION If theMidiSound::uencer instance is not
*     paused.
*
* TheMidiSound::uencer must be paused beforeMidiSound::Restart may be called.
*
***************************************************************************/
 MMRESULT MidiSound::Restart()
{
    //assert(NULL != pSeq);

	 switch(pSeq->uState){
		case SEQ_S_PAUSED: 
			pSeq->uState =SEQ_S_PLAYING;
			midiStreamRestart((HMIDISTRM)pSeq->hmidi);	 
			break;
	 
		case SEQ_S_OPENED:
			Start();
			break;
	 }
    return MMSYSERR_NOERROR;
}

/***************************************************************************
*  
*MidiSound::Stop
*
* Totally stops playback of an instance.
*
* pSeq                      - TheMidiSound::uencer instance.
*
* Returns
*   MMSYSERR_NOERROR If the operation is successful.
*    
*   MCIERR_UNSUPPORTED_FUNCTION If theMidiSound::uencer instance is not
*     paused or playing.
*
* TheMidiSound::uencer must be paused or playing beforeMidiSound::Stop may be called.
*
***************************************************************************/
 MMRESULT MidiSound::Stop()
{
    //assert(NULL != pSeq);

    /* Automatic success if we're already stopped
    */
    if (SEQ_S_PLAYING != pSeq->uState &&
       SEQ_S_PAUSED != pSeq->uState)
    {
        pSeq->fdwSeq &= ~SEQ_F_WAITING;
        return MMSYSERR_NOERROR;
    }

    pSeq->uState =SEQ_S_STOPPING;
    pSeq->fdwSeq |=SEQ_F_WAITING;
    //EnterCriticalSection(&gMidiCS);

	OutputDebugString("MidiSound::Stop\n");

	if(pSeq->hmidi){
		
#if 1 //--- modification by Francisco
		pSeq->mmrcLastErr = midiStreamClose((HMIDISTRM)pSeq->hmidi);
		pSeq->uState = SEQ_S_OPENED;
		pSeq->hmidi = 0;
		pSeq->fdwSeq &= ~SEQ_F_WAITING;
        return MMSYSERR_NOERROR;
#else //---		
		if (MMSYSERR_NOERROR != (pSeq->mmrcLastErr = midiStreamStop((HMIDISTRM)pSeq->hmidi)))
		{
			pSeq->fdwSeq &= ~SEQ_F_WAITING;
			//LeaveCriticalSection(&gMidiCS);
			return MCIERR_DEVICE_NOT_READY;
		}

		while (pSeq->uBuffersInMMSYSTEM)
			Sleep(0);

		if(pSeq->hmidi)			
			midiStreamClose((HMIDISTRM)pSeq->hmidi);
#endif
	}    
    //LeaveCriticalSection(&gMidiCS);
	return MMSYSERR_NOERROR;
}

/***************************************************************************
*  
*MidiSound::Time
*
* Determine the current position in playback of an instance.
*
* pSeq                      - TheMidiSound::uencer instance.
*
* pTicks                    - A pointer to a DWORD where the current position
*                             in ticks will be returned.
*
* Returns
*   MMSYSERR_NOERROR If the operation is successful.
*
*   MCIERR_DEVICE_NOT_READY If the underlying device fails to report
*     the position.
*    
*   MCIERR_UNSUPPORTED_FUNCTION If theMidiSound::uencer instance is not
*     paused or playing.
*
* TheMidiSound::uencer must be paused, playing or prerolled beforeMidiSound::Time
* may be called.
*
***************************************************************************/
 MMRESULT MidiSound::Time(PTICKS pTicks)
{
    MMRESULT                mmr;
    MMTIME                  mmt;
    
    //assert(pSeq != NULL);

    if (SEQ_S_PLAYING != pSeq->uState &&
       SEQ_S_PAUSED != pSeq->uState &&
       SEQ_S_PREROLLING != pSeq->uState &&
       SEQ_S_PREROLLED != pSeq->uState &&
       SEQ_S_OPENED != pSeq->uState)
    {
        //DPF(1, "seqTime(): State wrong! [is %u]", pSeq->uState);
        return MCIERR_UNSUPPORTED_FUNCTION;
    }

    *pTicks = 0;
    if (SEQ_S_OPENED != pSeq->uState)
    {
        *pTicks = pSeq->tkBase;
        if (SEQ_S_PREROLLED != pSeq->uState)
        {
            mmt.wType = TIME_TICKS;
            mmr = midiStreamPosition((HMIDISTRM)pSeq->hmidi, &mmt, sizeof(mmt));
            if (MMSYSERR_NOERROR != mmr)
            {
                //DPF(1, "midiStreamPosition() returned %lu", (DWORD)mmr);
                return MCIERR_DEVICE_NOT_READY;
            }

            *pTicks += mmt.u.ticks;
        }
    }

    return MMSYSERR_NOERROR;
}
                              
/***************************************************************************
*  
*MidiSound::MillisecsToTicks
*
* Given a millisecond offset in the output stream, returns the associated
* tick position.
*
* pSeq                      - TheMidiSound::uencer instance.
*
* msOffset                  - The millisecond offset into the stream.
*
* Returns the number of ticks into the stream.
*
***************************************************************************/
 TICKS MidiSound::MillisecsToTicks(DWORD msOffset)
{
    return smfMillisecsToTicks(pSeq->hSmf, msOffset);
}

/***************************************************************************
*  
*MidiSound::TicksToMillisecs
*
* Given a tick offset in the output stream, returns the associated
* millisecond position.
*
* pSeq                      - TheMidiSound::uencer instance.
*
* tkOffset                  - The tick offset into the stream.
*
* Returns the number of milliseconds into the stream.
*
***************************************************************************/
 DWORD MidiSound::TicksToMillisecs(TICKS tkOffset)
{
    return smfTicksToMillisecs(pSeq->hSmf, tkOffset);
}

/***************************************************************************
*  
*MidiSound::MIDICallback
*
* Called by the system when a buffer is done
*
* dw1                       - The buffer that has completed playback.
*
***************************************************************************/
static void FAR PASCAL seqMIDICallback(HMIDISTRM hms, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	LPMIDIHDR					lpmh		= (LPMIDIHDR)dw1;
    PSEQ                    pSeq;
    MMRESULT                mmrc;
    SMFRESULT               smfrc;

	if (uMsg != MOM_DONE)
		return;

	//assert(NULL != lpmh);

    pSeq = (PSEQ)(lpmh->dwUser);

    //assert(pSeq != NULL);

    --pSeq->uBuffersInMMSYSTEM;
    
    if (SEQ_S_RESET == pSeq->uState)
    {
        // We're recollecting buffers from MMSYSTEM
        //
		if (lpmh != pSeq->lpmhPreroll)
		{
        	lpmh->lpNext   = pSeq->lpmhFree;
        	pSeq->lpmhFree = lpmh;
		}

        return;
    }
    

    if ((SEQ_S_STOPPING == pSeq->uState) || (pSeq->fdwSeq &SEQ_F_EOF))
    {
        /*
        ** Reached EOF, just put the buffer back on the free
        ** list 
        */
		if (lpmh != pSeq->lpmhPreroll)
		{
        	lpmh->lpNext   = pSeq->lpmhFree;
        	pSeq->lpmhFree = lpmh;
		}

        if (MMSYSERR_NOERROR != (mmrc = midiOutUnprepareHeader(pSeq->hmidi, lpmh, sizeof(*lpmh))))
        {
            //DPF(1, "midiOutUnprepareHeader failed inMidiSound::BufferDone! (%lu)", (DWORD)mmrc);
        }

        if (0 == pSeq->uBuffersInMMSYSTEM)
        {
            //DPF(1, "seqBufferDone: normalMidiSound::uencer shutdown.");
            
            /* Totally done! Free device and notify.
            */
			//EnterCriticalSection(&gMidiCS);
			OutputDebugString("midiStreamClose\n");
/*
			if(pSeq->hmidi)			
				midiStreamClose((HMIDISTRM)pSeq->hmidi);
*/            			
            pSeq->hmidi = NULL;
            pSeq->uState =SEQ_S_OPENED;
            pSeq->mmrcLastErr = MMSYSERR_NOERROR;
            pSeq->fdwSeq &= ~SEQ_F_WAITING;

			//LeaveCriticalSection(&gMidiCS);

        	// lParam indicates whether or not to preroll again. Don't if we were explicitly
        	// stopped.
        	//    
            PostMessage(pSeq->hWnd, MMSG_DONE, (WPARAM)pSeq, (LPARAM)(SEQ_S_STOPPING != pSeq->uState));
        }
    }
    else
    {
        /*
        ** Not EOF yet; attempt to fill another buffer
        */
        smfrc = smfReadEvents(pSeq->hSmf, lpmh, pSeq->tkEnd);
        
        switch(smfrc)
        {
            case SMF_SUCCESS:
                break;

            case SMF_END_OF_FILE:
                pSeq->fdwSeq |=SEQ_F_EOF;
                smfrc = SMF_SUCCESS;
                break;

            default:
                //DPF(1, "smfReadEvents returned %lu in callback!", (DWORD)smfrc);
                pSeq->uState =SEQ_S_STOPPING;
                break;
        }

        if (SMF_SUCCESS == smfrc)
        {
            ++pSeq->uBuffersInMMSYSTEM;
            mmrc = midiStreamOut((HMIDISTRM)pSeq->hmidi, lpmh, sizeof(*lpmh));
            if (MMSYSERR_NOERROR != mmrc)
            {
                //DPF(1, "seqBufferDone(): midiStreamOut() returned %lu!", (DWORD)mmrc);
                
                --pSeq->uBuffersInMMSYSTEM;
                pSeq->uState =SEQ_S_STOPPING;
            }
        }
    }
}

/***************************************************************************
*  
* XlatSMFErr
*
* Translates an error from the SMF layer into an appropriate MCI error.
*
* smfrc                     - The return code from any SMF function.
*
* Returns
*   A parallel error from the MCI error codes.   
*
***************************************************************************/
static MMRESULT  XlatSMFErr(SMFRESULT smfrc)
{
    switch(smfrc)
    {
        case SMF_SUCCESS:
            return MMSYSERR_NOERROR;

        case SMF_NO_MEMORY:
            return MCIERR_OUT_OF_MEMORY;

        case SMF_INVALID_FILE:
        case SMF_OPEN_FAILED:
        case SMF_INVALID_TRACK:
            return MCIERR_INVALID_FILE;

        default:
            return MCIERR_UNSUPPORTED_FUNCTION;
    }
}


 MidiSound::MidiSound(void *hwnd) {
    if ((pSeq = (PSEQ)LocalAlloc(LPTR, sizeof(SEQ))) == NULL)
        return;

    pSeq->cBuffer  = 4;
    pSeq->cbBuffer = 1024;
	pSeq->pstrFile = NULL;

    if (AllocBuffers() != MMSYSERR_NOERROR)
        return;
    pSeq->hWnd = (HWND)hwnd;
}

MidiSound::~MidiSound() {
	Stop();
	if (pSeq->pstrFile)
		free(pSeq->pstrFile);
	pSeq->pstrFile=NULL;
	FreeBuffers();
	LocalFree(pSeq);
}



