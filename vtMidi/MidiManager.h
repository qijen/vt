/*************************************************************************/
/*	File : MidiManager.h
/*
/*	Author : 
/*	Last Modification : 
/*
/*************************************************************************/

#include "CKBaseManager.h"
#include "XList.h"
#ifdef _XBOX
	#include "Xtl.h"
#else
	#include "windows.h"
#endif

//_________________Struct midiMessage
struct midiMessage {
  unsigned char channel;
  unsigned char command;
  unsigned char note;
  unsigned char attack;
  UINT time;
};

typedef XList<midiMessage> midiMessageList;


class MidiManager :public CKMidiManager{
	////////////////////////////////////////////////////////
	//                Public Part                       ////
	////////////////////////////////////////////////////////
public :

	// Midi Sound Playing Functions
	virtual void*   Create(void* hwnd) ;
	virtual void	Release(void* source) ;
	virtual CKERROR  SetSoundFileName(void* source,CKSTRING filename) ;
	virtual CKSTRING GetSoundFileName(void* source) ;
	virtual CKERROR Play(void* source) ;
	virtual CKERROR Restart(void* source) ;
	virtual CKERROR Stop(void* source) ;
	virtual CKERROR Pause(void* source,CKBOOL pause=TRUE) ;
	virtual CKBOOL IsPlaying(void* source) ;
	virtual CKBOOL IsPaused(void* source) ;
	virtual CKERROR OpenFile(void* source) ;
	virtual CKERROR CloseFile(void* source) ;
	virtual CKERROR Preroll(void* source) ;
	virtual CKERROR Time(void* source,CKDWORD* pTicks) ;
	virtual CKDWORD MillisecsToTicks(void* source,CKDWORD msOffset) ;
	virtual CKDWORD TicksToMillisecs(void* source,CKDWORD tkOffset) ;
	
	// Initialization	
	virtual CKERROR OnCKInit();
	virtual CKERROR OnCKEnd();
	virtual CKERROR OnCKReset();
	virtual CKERROR PreProcess();
	virtual CKERROR PostProcess();
	virtual CKERROR PostClearAll();
	virtual CKERROR OnCKPlay();
	
	virtual CKDWORD	GetValidFunctionsMask()	{ return CKMANAGER_FUNC_OnCKInit|
                                                               CKMANAGER_FUNC_OnCKEnd|
                                                               CKMANAGER_FUNC_OnCKReset|
                                                               CKMANAGER_FUNC_PreProcess|
                                                               CKMANAGER_FUNC_PostProcess|
                                                               CKMANAGER_FUNC_PostClearAll|
                                                               CKMANAGER_FUNC_OnCKPlay; }
	//{secret}	
	MidiManager(CKContext *ctx);
	//{secret}
	~MidiManager();

  //----- Midi Methodes
  void ActivateNote( int note, int channel, CKBOOL state=TRUE);
  CKBOOL IsNoteActive( int note, int channel );
  CKERROR OpenMidiIn(int);	//Try to open the midi device passed as paramter
  CKERROR CloseMidiIn();

  CKERROR OpenMidiOut(int);	//Try to open the midi device passed as paramter
  CKERROR CloseMidiOut();


	
	inline void AddMidiBBref(){ ++midiDeviceBBrefcount; }
	inline void RemoveMidiBBref(){ --midiDeviceBBrefcount; }

    

public:
  HMIDIIN midiDeviceHandle; // Device Handle
  HMIDIOUT midiDeviceOutHandle;

  int midiCurrentDevice; // current used Device ID
  int midiCurrentOutDevice; // current used Device ID

  
  int DesiredmidiDevice; //midi device to which the user would like to change during runtime
  int DesiredmidiOutDevice; //midi device to which the user would like to change during runtime


  CKBOOL midiDeviceIsOpen; // tells whether the current Device is open or not
  CKBOOL midiOutDeviceIsOpen; // tells whether the current Device is open or not

  int midiDeviceBBrefcount;

  
  midiMessageList listFromCallBack;
  midiMessageList listForBehaviors;

#define MIDI_MAXNOTES 256
  unsigned char noteState[MIDI_MAXNOTES]; // nb of channels * nb of note = 16 * 128 = 2048 bits = 256 bytes

};



