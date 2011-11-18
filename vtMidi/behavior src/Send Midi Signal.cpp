/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//		            SendMidiSignal
//
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

#include "CKAll.h"
#include "../MidiManager.h"


CKERROR CreateSendMidiSignalProto(CKBehaviorPrototype **);
int SendMidiSignal(const CKBehaviorContext& behcontext);

CKERROR SendMidiSignalCallBack(const CKBehaviorContext& behcontext); // CallBack Functioon


/*******************************************************/
/*                     PROTO DECALRATION               */
/*******************************************************/
CKObjectDeclaration	*FillBehaviorSendMidiSignalDecl()
{
  CKObjectDeclaration *od = CreateCKObjectDeclaration("Send Midi Data");	
  od->SetDescription("Sends a Midi signal.");
  od->SetCategory("Controllers/Midi");
  od->SetType( CKDLL_BEHAVIORPROTOTYPE);
  od->SetGuid(CKGUID(0x19c33646,0x22be5e86));
  od->SetAuthorGuid(VIRTOOLS_GUID);
  od->SetAuthorName("Guenter Baumgart");
  od->SetVersion(0x00020000);
  od->SetCreationFunction(CreateSendMidiSignalProto);
  od->SetCompatibleClassId(CKCID_BEOBJECT);
  return od;
}


/*******************************************************/
/*                     PROTO CREATION                  */
/*******************************************************/
CKERROR CreateSendMidiSignalProto(CKBehaviorPrototype **pproto)
{
  CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Send Midi Data");
  if(!proto) return CKERR_OUTOFMEMORY;
  
  proto->DeclareInput("On");
  
  proto->DeclareOutput("Activated");
  proto->DeclareOutput("Error");
  
  proto->DeclareInParameter("cmd", CKPGUID_INT, "144");
  proto->DeclareInParameter("var0", CKPGUID_INT, "10");
  proto->DeclareInParameter("var1", CKPGUID_INT, "0");
  proto->DeclareOutParameter("return code", CKPGUID_INT, "0");

  proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
  proto->SetFunction(SendMidiSignal);

  *pproto = proto;
  return CK_OK;
}

/*******************************************************/
/*                     MAIN FUNCTION                   */
/*******************************************************/
int SendMidiSignal(const CKBehaviorContext& behcontext)
{
	CKBehavior *beh = behcontext.Behavior;

	MidiManager *mm = (MidiManager *) behcontext.Context->GetManagerByGuid( MIDI_MANAGER_GUID );
	
	int return_code;
	
	union { unsigned long word; unsigned char data[4]; } message;

	beh->GetInputParameterValue(0, &message.data[0]);
	beh->GetInputParameterValue(1, &message.data[1]);
	beh->GetInputParameterValue(2, &message.data[2] );
	message.data[3] = 0;     // unused

	return_code = midiOutShortMsg(mm->midiDeviceOutHandle, message.word);

	if (return_code != MMSYSERR_NOERROR) {
		mm->m_Context->OutputToConsole("error sending midi-cmd");
		beh->SetOutputParameterValue(0,&return_code);
		beh->ActivateInput(0, FALSE);
		return CK_OK;
	}else
		beh->ActivateInput(1, FALSE);
    
  return CK_OK;

}

