/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//		            SetInMidiPort
//
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

#include "CKAll.h"
#include "../MidiManager.h"


CKERROR CreateSetInMidiPortProto(CKBehaviorPrototype **);
int SetInMidiPort(const CKBehaviorContext& behcontext);
CKERROR SetInMidiPortCallBack(const CKBehaviorContext& behcontext); // CallBack Functioon

/*******************************************************/
/*                     PROTO DECALRATION               */
/*******************************************************/
CKObjectDeclaration	*FillBehaviorSetInMidiPortDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Set Midi Input Port");	
	od->SetDescription("Sets the Midi Input Port (usally 0).");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE );
	od->SetGuid(CKGUID(0x42d96c11,0x64242546));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateSetInMidiPortProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->SetCategory("Controllers/Midi");
	return od;
}


/*******************************************************/
/*                     PROTO CREATION                  */
/*******************************************************/
CKERROR CreateSetInMidiPortProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Set Midi Input Port");
  if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	proto->DeclareInParameter("Midi Input Port", CKPGUID_INT, "0");

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(SetInMidiPort);

  proto->SetBehaviorCallbackFct( SetInMidiPortCallBack );

	*pproto = proto;
	return CK_OK;

}


/*******************************************************/
/*                     MAIN FUNCTION                   */
/*******************************************************/
int SetInMidiPort(const CKBehaviorContext& behcontext)
{
  CKBehavior *beh = behcontext.Behavior;
  MidiManager *mm = (MidiManager *) behcontext.Context->GetManagerByGuid( MIDI_MANAGER_GUID );

  beh->ActivateInput(0, FALSE);
  beh->ActivateOutput(0);

  mm->DesiredmidiDevice = 0;
  beh->GetInputParameterValue(0, &mm->DesiredmidiDevice);

  return CKBR_OK;
}


/*******************************************************/
/*                     CALLBACK                        */
/*******************************************************/
CKERROR SetInMidiPortCallBack(const CKBehaviorContext& behcontext){
  CKBehavior *beh = behcontext.Behavior;
  MidiManager *mm = (MidiManager *) behcontext.Context->GetManagerByGuid( MIDI_MANAGER_GUID );
  
  switch( behcontext.CallbackMessage ){

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
