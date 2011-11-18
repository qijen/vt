/********************************************************************
	created:	2006/01/18
	created:	18:1:2006   17:12
	filename: 	e:\public\vt midi\MidiManager.cpp
	file path:	e:\public\vt midi
	file base:	MidiManager
	file ext:	cpp
	
	purpose:	midi low-level-stuff
*********************************************************************/



#include "CKAll.h"
#include "MidiManager.h"

#include "MidiSound/MidiSound.h"

// Constructor
MidiManager::MidiManager(CKContext *ctx):CKMidiManager(ctx,"Midi Manager")
{
  memset(noteState,0,sizeof(noteState));
  ctx->RegisterNewManager(this);
}

// Destructor
MidiManager::~MidiManager()
{
}


//-----------------------------
// Midi Callback Function
//-----------------------------
void CALLBACK MidiInProc( HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2 ){

	MidiManager *mm = (MidiManager *) dwInstance;

	if (!mm->midiDeviceIsOpen)
		return;
	

	  midiMessage tmp;
	  tmp.channel = (BYTE)((dwParam1 & 0x0F));
	  tmp.command = (BYTE)((dwParam1 & 0xF0) >> 4);
	  tmp.note = (BYTE)((dwParam1 & 0xFF00) >> 8);
	  tmp.attack = (BYTE)((dwParam1 & 0xFF0000) >> 16);
	  tmp.time = dwParam2;

	  mm->listFromCallBack.PushBack(tmp);


	  if( tmp.command==9 && tmp.attack!=0 ){ // note activated
		mm->ActivateNote(tmp.note, tmp.channel, TRUE);
		return;
	  }
	  if( tmp.command==9 || tmp.command==8 ){ // note deactivated
		mm->ActivateNote(tmp.note, tmp.channel, FALSE);
		return;
	  }
}

void CALLBACK MidiOutProc( HMIDIIN hMidiIn, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2 ){

	MidiManager *mm = (MidiManager *) dwInstance;


}
//-------------------
// Midi Note State
//-------------------
void MidiManager::ActivateNote( int note, int channel, CKBOOL state){
  int notedec = note>>3;
  int dec = abs( (channel<<4) + notedec );
  
  if( dec>=MIDI_MAXNOTES ) dec = MIDI_MAXNOTES-1;

  if( state ){
    noteState[dec] |= 1 << (note-(notedec<<3));
  } else {
    noteState[dec] &= ~(1 << (note-(notedec<<3)));
  }
}

CKBOOL MidiManager::IsNoteActive(int note, int channel){
  int notedec = note>>3;
  int dec = abs( (channel<<4) + notedec );
  
  if( dec>=MIDI_MAXNOTES ) dec = MIDI_MAXNOTES-1;

  if( noteState[dec] & (1 << (note-(notedec<<3))) ) return TRUE;

  return FALSE;
}


//------------------
//------------------
// Manager Events
//------------------
//------------------
CKERROR MidiManager::OnCKPlay()
{
  if( m_Context->IsReseted() ){ // to be sure it's not a un-resume PLAY
		if (midiDeviceBBrefcount) {
			if (OpenMidiIn(DesiredmidiDevice)!=CK_OK || OpenMidiOut(DesiredmidiOutDevice)!=CK_OK ) {
				//m_Context->OutputToConsole("Trying to open default port (0)");
				if( DesiredmidiDevice!=0  ){
					if( OpenMidiIn(0)!=CK_OK) {
						//m_Context->OutputToConsole("Unable to open Default port (0)");
						return CKERR_INVALIDOPERATION;
					}
				}
			}
		}
  }

  return CK_OK;
}

CKERROR MidiManager::OnCKInit()
{

  midiDeviceHandle = 0;
  midiDeviceOutHandle = 0 ;
  midiDeviceIsOpen = FALSE;
  midiOutDeviceIsOpen = FALSE;
  midiCurrentDevice = 0;
  midiCurrentOutDevice = 0;
  DesiredmidiDevice = 0;
  DesiredmidiOutDevice= 0;
  midiDeviceBBrefcount = 0;
  

  int midiDeviceCount = midiInGetNumDevs();
  int midiOutDeviceCount = midiOutGetNumDevs();

  if( !midiDeviceCount ){
    m_Context->OutputToConsole("No Midi Device !");
    return CK_OK;
  }

  MIDIINCAPS tmp; // get midi devices infos
  for( int a=0 ; a< midiDeviceCount ; a++ ){
    midiInGetDevCaps(a, &tmp,sizeof(tmp) );
  }

  MIDIOUTCAPS tmpO; // get midi out devices infos
  for( int z=0 ; z< midiOutDeviceCount ; z++ ){
	  midiOutGetDevCaps(z, &tmpO,sizeof(tmpO) );
  }

  return CK_OK;
}

CKERROR MidiManager::OnCKEnd()
{
 return CK_OK;
}

CKERROR MidiManager::OnCKReset()
{
  if (midiDeviceIsOpen )
	return CloseMidiIn();
  return CK_OK;
}

CKERROR MidiManager::CloseMidiIn() {

	if( midiDeviceHandle && midiDeviceIsOpen ){
		if (midiInStop( midiDeviceHandle )== MMSYSERR_NOERROR) {

			if (midiInClose(midiDeviceHandle) == MMSYSERR_NOERROR) {

				midiDeviceIsOpen = FALSE;
				return CK_OK;
			}
		}
	}

  return CKERR_INVALIDOPERATION;
}

CKERROR MidiManager::CloseMidiOut() {

	if( midiDeviceOutHandle && midiOutDeviceIsOpen ){
			if (midiOutClose(midiDeviceOutHandle) == MMSYSERR_NOERROR) {
				midiOutDeviceIsOpen = FALSE;
				return CK_OK;
			}
		}

	return CKERR_INVALIDOPERATION;
}
CKERROR MidiManager::OpenMidiOut(int DesiredmidiDevice) {

	if(midiOutDeviceIsOpen) {
		m_Context->OutputToConsole("Midi Out Device Already open",false);
		return CKERR_INVALIDOPERATION;
	}


	MMRESULT mr;
	
	mr = midiOutOpen( &midiDeviceOutHandle, DesiredmidiOutDevice, 0, 0, CALLBACK_NULL);


	if(!midiDeviceOutHandle|| (mr!=MMSYSERR_NOERROR) )	{
		m_Context->OutputToConsole("Failed to open Desired Midi Out Device");
		return CKERR_INVALIDOPERATION;
	}

	midiOutDeviceIsOpen = TRUE;
	midiCurrentOutDevice = DesiredmidiOutDevice;

	if (mr!=MMSYSERR_NOERROR){
		m_Context->OutputToConsole("Failed to Start Desired Midi Out Device");
		return CKERR_INVALIDOPERATION;
	}

	return CK_OK;

}

CKERROR MidiManager::OpenMidiIn(int DesiredmidiDevice) {

	if(midiDeviceIsOpen) {
		m_Context->OutputToConsole("Midi Device Already open");
		return CKERR_INVALIDOPERATION;
	}

      
    MMRESULT mr;
	//midiDeviceHandle = NULL;
	mr = midiInOpen( &midiDeviceHandle, DesiredmidiDevice, (DWORD)MidiInProc, (unsigned long)this, CALLBACK_FUNCTION|MIDI_IO_STATUS );
	

    if(!midiDeviceHandle || (mr!=MMSYSERR_NOERROR) )	{
		m_Context->OutputToConsole("Failed to open Desired Midi IN Device");
		return CKERR_INVALIDOPERATION;
	}

	midiDeviceIsOpen = TRUE;
	midiCurrentDevice = DesiredmidiDevice;
    ZeroMemory(&noteState, MIDI_MAXNOTES);
    mr = midiInStart( midiDeviceHandle );
	if (mr!=MMSYSERR_NOERROR){
		m_Context->OutputToConsole("Failed to Start Desired Midi IN Device");
		return CKERR_INVALIDOPERATION;
	}

	return CK_OK;
    
}

CKERROR MidiManager::PostClearAll()
{
 midiDeviceBBrefcount = 0;
 return CK_OK;
}

CKERROR MidiManager::PreProcess()
{
  listFromCallBack.Swap(listForBehaviors);
  

  return CK_OK;
}

CKERROR MidiManager::PostProcess()
{


	//////////////////////////////////////////////////////////////////////////

	listForBehaviors.Clear();
	if (midiDeviceIsOpen && (midiCurrentDevice!=DesiredmidiDevice)) {
		MMRESULT mreset;
		mreset = midiInReset(midiDeviceHandle);
		if (mreset != MMSYSERR_NOERROR) {
		m_Context->OutputToConsole("Unable to Reset the current Midi Device, before opening a new Midi device",false);
		return CKERR_INVALIDOPERATION;
		}
		if (CloseMidiIn()!= CK_OK)
			return CKERR_INVALIDOPERATION;

		midiDeviceIsOpen = FALSE;

		if (OpenMidiIn(DesiredmidiDevice)!=CK_OK)
			return CKERR_INVALIDOPERATION;
	}

  //////////////////////////////////////////////////////////////////////////

  if (midiOutDeviceIsOpen && (midiCurrentOutDevice!=DesiredmidiOutDevice)) {
	  MMRESULT mreset;
	  mreset = midiOutReset(midiDeviceOutHandle);
	  if (mreset != MMSYSERR_NOERROR) {
		  m_Context->OutputToConsole("Unable to Reset the current Midi Out Device, before opening a new Midi Out device",false);
		  return CKERR_INVALIDOPERATION;
	  }
	  if (CloseMidiOut()!= CK_OK)
		  return CKERR_INVALIDOPERATION;

	  midiOutDeviceIsOpen = FALSE;
	  if (OpenMidiOut(DesiredmidiOutDevice)!=CK_OK){
          m_Context->OutputToConsole("couldn't attach new midi out device");		
		  return CKERR_INVALIDOPERATION;
	  }
  }

  return CK_OK;
}

// Midi Sound Functions
CKERROR  
MidiManager::SetSoundFileName(void* source,CKSTRING filename) 
{
	MidiSound* ms = (MidiSound*)source;
	if (!ms) return CKERR_INVALIDPARAMETER;

	return ms->SetSoundFileName(filename);
}

CKSTRING 
MidiManager::GetSoundFileName(void* source) 
{
	MidiSound* ms = (MidiSound*)source;
	if (!ms) return NULL;

	return (CKSTRING)ms->GetSoundFileName();
}

CKERROR 
MidiManager::Play(void* source) 
{
	MidiSound* ms = (MidiSound*)source;
	if (!ms) return CKERR_INVALIDPARAMETER;

	return ms->Start();
}

CKERROR 
MidiManager::Restart(void* source) 
{
	MidiSound* ms = (MidiSound*)source;
	if (!ms) return CKERR_INVALIDPARAMETER;

	return ms->Restart();
}

CKERROR 
MidiManager::Stop(void* source) 
{
	MidiSound* ms = (MidiSound*)source;
	if (!ms) return CKERR_INVALIDPARAMETER;

	return ms->Stop();
}

CKERROR 
MidiManager::Pause(void* source,CKBOOL pause) 
{
	MidiSound* ms = (MidiSound*)source;
	if (!ms) return CKERR_INVALIDPARAMETER;

	return ms->Pause();
}

CKBOOL 
MidiManager::IsPlaying(void* source) 
{
	MidiSound* ms = (MidiSound*)source;
	if (!ms) return CKERR_INVALIDPARAMETER;
	
	return ms->IsPlaying();
}

CKBOOL 
MidiManager::IsPaused(void* source) 
{
	MidiSound* ms = (MidiSound*)source;
	if (!ms) return CKERR_INVALIDPARAMETER;
	
	return ms->IsPaused();
}

void*   
MidiManager::Create(void* hwnd) 
{
	return new MidiSound(hwnd);
}

void	
MidiManager::Release(void* source) 
{
	CloseFile(source);
	delete ((MidiSound*)(source));
}

CKERROR 
MidiManager::OpenFile(void* source) 
{
	MidiSound* ms = (MidiSound*)source;
	if (!ms) return CKERR_INVALIDPARAMETER;

	return ms->OpenFile();
}

CKERROR 
MidiManager::CloseFile(void* source) 
{
	MidiSound* ms = (MidiSound*)source;
	if (!ms) return CKERR_INVALIDPARAMETER;

	return ms->CloseFile();
}

CKERROR 
MidiManager::Preroll(void* source) 
{
	MidiSound* ms = (MidiSound*)source;
	if (!ms) return CKERR_INVALIDPARAMETER;

	return ms->Preroll();
}

CKERROR 
MidiManager::Time(void* source,CKDWORD* pTicks) 
{
	MidiSound* ms = (MidiSound*)source;
	if (!ms) return CKERR_INVALIDPARAMETER;

	return ms->Time((DWORD*)pTicks);
}

CKDWORD 
MidiManager::MillisecsToTicks(void* source,CKDWORD msOffset) 
{
	MidiSound* ms = (MidiSound*)source;
	if (!ms) return CKERR_INVALIDPARAMETER;

	return ms->MillisecsToTicks(msOffset);
}

CKDWORD 
MidiManager::TicksToMillisecs(void* source,CKDWORD tkOffset) 
{
	MidiSound* ms = (MidiSound*)source;
	if (!ms) return CKERR_INVALIDPARAMETER;

	return ms->TicksToMillisecs(tkOffset);
}
