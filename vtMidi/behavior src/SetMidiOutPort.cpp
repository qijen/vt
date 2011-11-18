/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//
//		            SetMidiOutPort
//
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

#include "CKAll.h"
#include "../MidiManager.h"


CKERROR CreateSetMidiOutPortProto(CKBehaviorPrototype **);
int SetMidiOutPort(const CKBehaviorContext& behcontext);

/*******************************************************/
/*                     PROTO DECALRATION               */
/*******************************************************/
CKObjectDeclaration	*FillBehaviorSetMidiOutPortDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Set Midi Output Port");	
	od->SetDescription("Sets the Midi Out Port (usally 0).");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE );
	od->SetGuid(CKGUID(0x149d28c7,0x29947940));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Guenter Baumgart");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateSetMidiOutPortProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->SetCategory("Controllers/Midi");
	return od;
}


/*******************************************************/
/*                     PROTO CREATION                  */
/*******************************************************/
CKERROR CreateSetMidiOutPortProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Set Midi Out Port");
  if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	proto->DeclareInParameter("Midi Output Port", CKPGUID_INT, "0");

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(SetMidiOutPort);
	*pproto = proto;
	return CK_OK;

}


/*******************************************************/
/*                     MAIN FUNCTION                   */
/*******************************************************/
int SetMidiOutPort(const CKBehaviorContext& behcontext)
{
  CKBehavior *beh = behcontext.Behavior;
  MidiManager *mm = (MidiManager *) behcontext.Context->GetManagerByGuid( MIDI_MANAGER_GUID );

  beh->ActivateInput(0, FALSE);
  beh->ActivateOutput(0);
  
  //see @MidiManager::PostProcess();
  beh->GetInputParameterValue(0, &mm->DesiredmidiOutDevice);

  return CKBR_OK;
}
