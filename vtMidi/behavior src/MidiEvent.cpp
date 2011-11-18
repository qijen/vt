/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//		            MidiEvent
//
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

#include "CKAll.h"
#include "../MidiManager.h"


CKERROR CreateMidiEventBehaviorProto(CKBehaviorPrototype **);
int MidiEvent(const CKBehaviorContext& behcontext);

CKERROR MidiEventCallBack(const CKBehaviorContext& behcontext); // CallBack Functioon


/*******************************************************/
/*                     PROTO DECALRATION               */
/*******************************************************/
CKObjectDeclaration	*FillBehaviorMidiEventDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Midi Event");
	od->SetDescription("Gets a Midi event ON/OFF.");
	od->SetCategory("Controllers/Midi");
	od->SetType(CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x7c652f90,0x64404377));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateMidiEventBehaviorProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}


/*******************************************************/
/*                     PROTO CREATION                  */
/*******************************************************/
CKERROR CreateMidiEventBehaviorProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Midi Event");
	if(!proto) return CKERR_OUTOFMEMORY;
	
	proto->DeclareInput("On");
	proto->DeclareInput("Off");
	
	proto->DeclareOutput("Activated");
	proto->DeclareOutput("Deactivated");

	proto->DeclareInParameter("Channel", CKPGUID_INT, "0");
	proto->DeclareInParameter("Note 1", CKPGUID_INT, "0");
	
	proto->DeclareLocalParameter(NULL, CKPGUID_BOOL, "FALSE"); // Combination was ok

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(MidiEvent);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)(CKBEHAVIOR_VARIABLEPARAMETERINPUTS));
	
  proto->SetBehaviorCallbackFct( MidiEventCallBack );
  
	*pproto = proto;
	return CK_OK;
}


/*******************************************************/
/*                     MAIN FUNCTION                   */
/*******************************************************/
int MidiEvent(const CKBehaviorContext& behcontext)
{
  CKBehavior *beh = behcontext.Behavior;

  if( beh->IsInputActive(1) ){ // OFF
    beh->ActivateInput(1, FALSE);

    return CKBR_OK;
  }
  
  CKBOOL combiWasOK = FALSE;
  if( beh->IsInputActive(0) ){ // ON
    beh->ActivateInput(0, FALSE);
    beh->SetLocalParameterValue(0, &combiWasOK);
  } else {
    beh->GetLocalParameterValue(0, &combiWasOK);
  }

  MidiManager *mm = (MidiManager *) behcontext.Context->GetManagerByGuid( MIDI_MANAGER_GUID );

  // Channel
  int channel=0;
  beh->GetInputParameterValue(0, &channel);

  int note, count = beh->GetInputParameterCount();
  //--- test if all input notes are activated or not
  for( int a=1 ; a<count ; a++ ){
    beh->GetInputParameterValue(a, &note);
    if( !mm->IsNoteActive(note, channel) ) break;
  }

  if( a==count ){ // All notes are pressed
    if( !combiWasOK ){
      beh->ActivateOutput(0);
      
      combiWasOK = TRUE;
      beh->SetLocalParameterValue(0, &combiWasOK);
      
      return CKBR_ACTIVATENEXTFRAME;
    }
  } else { // Not all notes are pressed
    if( combiWasOK ){
      beh->ActivateOutput(1);
      
      combiWasOK = FALSE;
      beh->SetLocalParameterValue(0, &combiWasOK);
      
      return CKBR_ACTIVATENEXTFRAME;
    }
  }

  return CKBR_ACTIVATENEXTFRAME;
}


/*******************************************************/
/*                     CALLBACK                        */
/*******************************************************/
CKERROR MidiEventCallBack(const CKBehaviorContext& behcontext){
  CKBehavior *beh = behcontext.Behavior;
  MidiManager *mm = (MidiManager *) behcontext.Context->GetManagerByGuid( MIDI_MANAGER_GUID );

  switch( behcontext.CallbackMessage ){
    
  case CKM_BEHAVIOREDITED:
    {
      int c_pin = beh->GetInputParameterCount();
      
      char name[20];

      CKParameterIn *pin;
      for( int a=2 ; a<c_pin ; a++){
        pin = beh->GetInputParameter(a);
        sprintf( name, "Note %d", a);
        pin->SetName( name );
        pin->SetGUID( CKPGUID_INT );
      }
    } break;
    
  case CKM_BEHAVIORATTACH:
  case CKM_BEHAVIORLOAD:
		{
			mm->AddMidiBBref();
		} break;
  case CKM_BEHAVIORDETACH:
		{
			mm->RemoveMidiBBref();
		} break;
  }
  
  return CKBR_OK; 
}
