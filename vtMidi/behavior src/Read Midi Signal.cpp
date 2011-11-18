/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//		            ReadMidiSignal
//
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

#include "CKAll.h"
#include "../MidiManager.h"


CKERROR CreateReadMidiSignalProto(CKBehaviorPrototype **);
int ReadMidiSignal(const CKBehaviorContext& behcontext);

CKERROR ReadMidiSignalCallBack(const CKBehaviorContext& behcontext); // CallBack Functioon


/*******************************************************/
/*                     PROTO DECALRATION               */
/*******************************************************/
CKObjectDeclaration	*FillBehaviorReadMidiSignalDecl()
{
  CKObjectDeclaration *od = CreateCKObjectDeclaration("Read Midi");	
  od->SetDescription("Reads a Midi signal.");
  od->SetCategory("Controllers/Midi");
  od->SetType( CKDLL_BEHAVIORPROTOTYPE);
  od->SetGuid(CKGUID(0x408c50ed,0x68ba5988));
  od->SetAuthorGuid(VIRTOOLS_GUID);
  od->SetAuthorName("Virtools");
  od->SetVersion(0x00020000);
  od->SetCreationFunction(CreateReadMidiSignalProto);
  od->SetCompatibleClassId(CKCID_BEOBJECT);
  return od;
}


/*******************************************************/
/*                     PROTO CREATION                  */
/*******************************************************/
CKERROR CreateReadMidiSignalProto(CKBehaviorPrototype **pproto)
{
  CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Read Midi");
  if(!proto) return CKERR_OUTOFMEMORY;
  
  proto->DeclareInput("On");
  proto->DeclareInput("Off");
  
  proto->DeclareOutput("Activated");
  proto->DeclareOutput("Deactivated");
  
  proto->DeclareInParameter("Channel", CKPGUID_INT, "0");
  
  proto->DeclareOutParameter("Note", CKPGUID_INT, "-1");
  proto->DeclareOutParameter("Attack", CKPGUID_INT, "-1");

  proto->DeclareSetting("Ask For All Commands", CKPGUID_BOOL, "FALSE");
  
  proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
  proto->SetFunction(ReadMidiSignal);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)(CKBEHAVIOR_INTERNALLYCREATEDOUTPUTS|CKBEHAVIOR_INTERNALLYCREATEDOUTPUTPARAMS));

  proto->SetBehaviorCallbackFct( ReadMidiSignalCallBack );
  
  *pproto = proto;
  return CK_OK;
}

/*******************************************************/
/*                     MAIN FUNCTION                   */
/*******************************************************/
int ReadMidiSignal(const CKBehaviorContext& behcontext)
{
  CKBehavior *beh = behcontext.Behavior;

  if( beh->IsInputActive(1) ){ // OFF
    beh->ActivateInput(1, FALSE);

    return CKBR_OK;
  }
  
  if( beh->IsInputActive(0) ){ // ON
    beh->ActivateInput(0, FALSE);
  }
  
  MidiManager *mm = (MidiManager *) behcontext.Context->GetManagerByGuid( MIDI_MANAGER_GUID );

  // Channel
  int channel=0;
  beh->GetInputParameterValue(0, &channel);
  
  int command, note, attack;

  CKBOOL allCommands=FALSE;
  beh->GetLocalParameterValue(0, &allCommands);

  //________________________________________________________
  if( !allCommands ){ // use 9 and 8 as default commands
    XListIt<midiMessage> it;
    for( it = mm->listForBehaviors.Begin() ; it!=mm->listForBehaviors.End() ; it++ ){

      if( (*it).channel==channel ){
        if( (*it).command==9 && (*it).attack!=0 ){ // start a note
          note = (*it).note;
          attack = (*it).attack;
          beh->SetOutputParameterValue(0, &note);
          beh->SetOutputParameterValue(1, &attack);
          beh->ActivateOutput(0);
          return CKBR_ACTIVATENEXTFRAME;
        }
        if( (*it).command==9 || (*it).command==8 ){ // stop a note
          note = (*it).note;
          attack = (*it).attack;
          beh->SetOutputParameterValue(0, &note);
          beh->SetOutputParameterValue(1, &attack);
          beh->ActivateOutput(1);
          return CKBR_ACTIVATENEXTFRAME;
        }
      }

    }
  //_____________________________________________
  } else { // we are interested by every commands
    XListIt<midiMessage> it;
    for( it = mm->listForBehaviors.Begin() ; it!=mm->listForBehaviors.End() ; it++ ){
		if (beh->GetVersion()<0x00020000) {
			if ( command = (*it).command ){
				note = (*it).note;
				attack = (*it).attack;
				beh->SetOutputParameterValue(0, &note);
				beh->SetOutputParameterValue(1, &attack);
				beh->SetOutputParameterValue(2, &command);
				beh->ActivateOutput(0);
				return CKBR_ACTIVATENEXTFRAME;
			}
		}
		else {
			if ( command = (*it).command && (*it).channel == channel){
				note = (*it).note;
				attack = (*it).attack;
				beh->SetOutputParameterValue(0, &note);
				beh->SetOutputParameterValue(1, &attack);
				beh->SetOutputParameterValue(2, &command);
				beh->ActivateOutput(0);
				return CKBR_ACTIVATENEXTFRAME;
			}
		}
      
    }
    
  }
  
  return CKBR_ACTIVATENEXTFRAME;
}


/*******************************************************/
/*                     CALLBACK                        */
/*******************************************************/
CKERROR ReadMidiSignalCallBack(const CKBehaviorContext& behcontext){
  CKBehavior *beh = behcontext.Behavior;
  MidiManager *mm = (MidiManager *) behcontext.Context->GetManagerByGuid( MIDI_MANAGER_GUID );
  
  switch( behcontext.CallbackMessage ){
    
  case CKM_BEHAVIORSETTINGSEDITED:
    {
      int c_out = beh->GetOutputCount();
      
      CKBOOL allCommands=FALSE;
      beh->GetLocalParameterValue(0, &allCommands);
      
      CKBehaviorIO *out = beh->GetOutput(0);

      if( c_out==2 && allCommands ){ // All Commands
        beh->DeleteOutput(--c_out);
  			beh->CreateOutputParameter("Command", CKPGUID_INT);
        out->SetName("Message Received");
      }

      if( c_out==1 && !allCommands ){ // just 8 and 9
				CKDestroyObject( beh->RemoveOutputParameter(2) );
        beh->CreateOutput("Deactivate");
        out->SetName("Activate");
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
