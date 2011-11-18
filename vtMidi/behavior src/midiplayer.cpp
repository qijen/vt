/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//		            MidiPlayer
//
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
#include "CKALL.h"
#include "CKMidiSound.h"

CKERROR CreateMidiPlayerBehaviorProto(CKBehaviorPrototype **pproto);
int MidiPlayer(const CKBehaviorContext& behcontext);
CKERROR MidiPlayerCB(const CKBehaviorContext& behcontext);


/*******************************************************/
/*                     PROTO DECALRATION               */
/*******************************************************/
CKObjectDeclaration	*FillBehaviorMidiPlayerDecl()
{
	
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Midi Player");
	od->SetDescription("Plays/Stops a MIDI sound.");
	/* rem:
	<SPAN CLASS=in>Play: </SPAN>Starts playback.<BR>
	<SPAN CLASS=in>Stop: </SPAN>Stops playback.<BR>
	<BR>
	<SPAN CLASS=out>Out Play: </SPAN>is activated when the Play input is triggered.<BR>
	<SPAN CLASS=out>Out Stop: </SPAN>is activated when the Stop input is triggered.<BR>
	<BR>
	*/
	od->SetCategory("Sounds/Midi");
	od->SetType(CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x843cb43a, 0xa12dac48));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateMidiPlayerBehaviorProto);
	od->SetCompatibleClassId(CKCID_MIDISOUND);
	return od;
}


/*******************************************************/
/*                     PROTO CREATION                  */
/*******************************************************/
CKERROR CreateMidiPlayerBehaviorProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Midi Player");
	if(!proto) return CKERR_OUTOFMEMORY;
	
	proto->DeclareInput("Play");
	proto->DeclareInput("Stop");
  proto->DeclareInput("Pause/Resume");
	
	proto->DeclareOutput("Out Play");
	proto->DeclareOutput("Out Stop");
	proto->DeclareOutput("Out Pause/Resume");

	proto->SetBehaviorFlags(CKBEHAVIOR_NONE);
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	
	proto->SetFunction(MidiPlayer);
	proto->SetBehaviorFlags(CKBEHAVIOR_TARGETABLE);
	proto->SetBehaviorCallbackFct(MidiPlayerCB);
	
	*pproto = proto;
	return CK_OK;
}


/*******************************************************/
/*                     MAIN FUNCTION                   */
/*******************************************************/
int MidiPlayer(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;

  //____________________________/ PLAY
  if( beh->IsInputActive(0) ){

    CKMidiSound *midi=NULL; // back-compatibility
    midi = (CKMidiSound *)( beh->GetInputParameter(0) ? beh->GetInputParameterObject(0):beh->GetTarget() );

    if(midi){
      midi->Play();
      beh->ActivateInput(0,FALSE);
      beh->ActivateOutput(0);
    }
  }
  
  //____________________________/ STOP
  if( beh->IsInputActive(1) ){

    CKMidiSound *midi=NULL; // back-compatibility
    midi = (CKMidiSound *)( beh->GetInputParameter(0) ? beh->GetInputParameterObject(0):beh->GetTarget() );

    if(midi){
      midi->Stop();
      beh->ActivateInput(1,FALSE);
      beh->ActivateOutput(1);
    }
  }
  
  //____________________________/ PAUSE
  if( beh->IsInputActive(2) ){

    CKMidiSound *midi=NULL; // back-compatibility
    midi = (CKMidiSound *)( beh->GetInputParameter(0) ? beh->GetInputParameterObject(0):beh->GetTarget() );

    if(midi){
      midi->Pause( midi->IsPlaying() );
      beh->ActivateInput(2,FALSE);
      beh->ActivateOutput(2);
    }
  }

  
	return CKBR_OK;
}


/*******************************************************/
/*                     CALLBACK                        */
/*******************************************************/
CKERROR MidiPlayerCB(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;
  
  CKMidiSound *midi=NULL; // back-compatibility
  midi = (CKMidiSound *)( beh->GetInputParameter(0) ? beh->GetInputParameterObject(0):beh->GetTarget() );
  if( !midi ) return CKBR_OK;

  switch(behcontext.CallbackMessage)
	{
	  
  case CKM_BEHAVIORDETACH:
  case CKM_BEHAVIORRESET:
    {
      midi->Stop();

    } break;
  case CKM_BEHAVIORPAUSE:
    {
      midi->Pause();

    } break;
  
  case CKM_BEHAVIORRESUME:
    {
		if( midi->IsPaused() ){
			midi->Pause(FALSE);
		}
    } break;

	}

	return CKBR_OK;
}
