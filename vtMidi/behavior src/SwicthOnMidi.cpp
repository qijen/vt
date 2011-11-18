/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//		            SwitchOnMidi
//
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

#include "CKAll.h"
#include "../MidiManager.h"


CKERROR CreateSwitchOnMidiProto(CKBehaviorPrototype **);
int SwitchOnMidi(const CKBehaviorContext& behcontext);

CKERROR SwitchOnMidiCallBack(const CKBehaviorContext& behcontext); // CallBack Functioon


/*******************************************************/
/*                     PROTO DECALRATION               */
/*******************************************************/
CKObjectDeclaration	*FillBehaviorSwitchOnMidiDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Switch On Midi");	
	od->SetDescription("Activates the appropriate output when receiving a Midi event.");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE );
	od->SetGuid(CKGUID(0x624b1bec,0x509400a8));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateSwitchOnMidiProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->SetCategory("Controllers/Midi");
	return od;
}


/*******************************************************/
/*                     PROTO CREATION                  */
/*******************************************************/
CKERROR CreateSwitchOnMidiProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Switch On Midi");
  if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("On");
	proto->DeclareInput("Off");

	proto->DeclareOutput("Out 1");

	proto->DeclareInParameter("Channel", CKPGUID_INT, "0");
	proto->DeclareInParameter("Note 1", CKPGUID_INT, "0");

  proto->DeclareOutParameter("Nop", CKPGUID_NONE);
  proto->DeclareOutParameter("Attack 1", CKPGUID_INT, "0");

	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEOUTPUTS| CKBEHAVIOR_INTERNALLYCREATEDINPUTPARAMS| CKBEHAVIOR_INTERNALLYCREATEDOUTPUTPARAMS));
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(SwitchOnMidi);

	proto->SetBehaviorCallbackFct( SwitchOnMidiCallBack );

	*pproto = proto;
	return CK_OK;

}


/*******************************************************/
/*                     MAIN FUNCTION                   */
/*******************************************************/
int SwitchOnMidi(const CKBehaviorContext& behcontext)
{
  CKBehavior *beh = behcontext.Behavior;

  if( beh->IsInputActive(1) ){ // OFF
    beh->ActivateInput(1, FALSE);

    return CKBR_OK;
  }
  
  if( beh->IsInputActive(0) ){ // ON
    beh->ActivateInput(0, FALSE);
  }

  int channel=0; // Channel
  beh->GetInputParameterValue(0, &channel);

  MidiManager *mm = (MidiManager *) behcontext.Context->GetManagerByGuid( MIDI_MANAGER_GUID );

  int note, attack, count = beh->GetInputParameterCount();

  for( int a=1 ; a<count ; a++ ){
    beh->GetInputParameterValue(a, &note);
    if( mm->IsNoteActive(note, channel) ){
      beh->ActivateOutput(a-1);
    }

    //--- Attack part
    XListIt<midiMessage> it;
    for( it = mm->listForBehaviors.Begin() ; it!=mm->listForBehaviors.End() ; it++ ){
      if( (*it).channel==channel ){
        if( (*it).command==9 || (*it).command==8 ){ // start a note
          if( note == (*it).note ){
            attack = (*it).attack;
            beh->SetOutputParameterValue(a, &attack);
          }
        }
      }
    }
    //---
  }

  
  return CKBR_ACTIVATENEXTFRAME;
}


/*******************************************************/
/*                     CALLBACK                        */
/*******************************************************/
CKERROR SwitchOnMidiCallBack(const CKBehaviorContext& behcontext){
  CKBehavior *beh = behcontext.Behavior;
  MidiManager *mm = (MidiManager *) behcontext.Context->GetManagerByGuid( MIDI_MANAGER_GUID );

  switch( behcontext.CallbackMessage ){

  case CKM_BEHAVIOREDITED:
		{
			int c_out = beh->GetOutputCount();
			int c_pin = beh->GetInputParameterCount()-1;
			
			char str[16];

      while( c_pin < c_out ){ // we must add Input Params / Ouput Params
				sprintf( str, "Note %d", c_pin+1);
				beh->CreateInputParameter(str, CKPGUID_INT);

        sprintf( str, "Attack %d", c_pin+1);
        beh->CreateOutputParameter(str, CKPGUID_INT);
        
        ++c_pin;
			}

      while( c_pin > c_out ){ // we must remove Input Params / Ouput Params
        CKDestroyObject(beh->RemoveInputParameter(c_pin));
        CKDestroyObject(beh->RemoveOutputParameter(c_pin));
        --c_pin;
      }

      CKBehaviorIO *out;
      while( c_out ){
        --c_out;
        out = beh->GetOutput(c_out);
        sprintf( str, "Out %d", c_out+1);
        out->SetName(str);
      }
    }
    break;
    
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
