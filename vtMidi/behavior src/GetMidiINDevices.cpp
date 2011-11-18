#include "CKAll.h"
#include "../MidiManager.h"

CKObjectDeclaration	*FillBehaviorGetMidiINDevicesDecl();
CKERROR CreateGetMidiINDevicesProto(CKBehaviorPrototype **pproto);
int GetMidiINDevices(const CKBehaviorContext& behcontext);


/************************************************************************/
/*																																			  */
/************************************************************************/

CKObjectDeclaration	*FillBehaviorGetMidiINDevicesDecl(){
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Get MidiIN Devices");	
	od->SetDescription("Returns all Midi IN Devices");
	od->SetCategory("Controllers/Midi");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x5f521403,0x3bef5561));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Guenter Baumgart");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateGetMidiINDevicesProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}
CKERROR CreateGetMidiINDevicesProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Get MidiIN Devices");
	if(!proto) return CKERR_OUTOFMEMORY;
	
	proto->DeclareInput("Get Next");
	proto->DeclareInput("Get Prev");
	
	proto->DeclareOutput("Finish");
	proto->DeclareOutput("LoopOut");
	
	proto->DeclareOutParameter("Name",CKPGUID_STRING);
	proto->DeclareOutParameter("Index",CKPGUID_INT);
	proto->DeclareOutParameter("Count",CKPGUID_INT);
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);

	proto->SetFunction(GetMidiINDevices);
	*pproto = proto;
	return CK_OK;
}

int indexD = 0;
int countD = 0;


int GetMidiINDevices(const CKBehaviorContext& behcontext)
{

	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;


	CKPluginManager* ThePluginManager=CKGetPluginManager();
	CKRenderManager *rm = (CKRenderManager *)ctx->GetRenderManager();
	//////////////////////////////////////////////////////////////////////////
	if( beh->IsInputActive(0) ){

		MidiManager *mm = (MidiManager *) behcontext.Context->GetManagerByGuid( MIDI_MANAGER_GUID );

		countD=midiInGetNumDevs();
		if( !countD){
			mm->m_Context->OutputToConsole("No Midi IN Devices !");
			return CK_OK;
		}
		beh->ActivateInput(0, FALSE);
		indexD = 0;
		beh->ActivateInput(1, TRUE);

	}
	//////////////////////////////////////////////////////////////////////////
	if( beh->IsInputActive(1) ){
			beh->ActivateInput(1, FALSE);
		
			if (indexD > (countD-1)){
				indexD = 0;
				beh->ActivateOutput(0,TRUE);
				return CKBR_OK;
			}
			beh->SetOutputParameterValue(1,&indexD);
			
			MIDIINCAPS tmp;
			midiInGetDevCaps(indexD, &tmp,sizeof(tmp) );
			indexD++;
			CKParameterOut *pout2 = beh->GetOutputParameter(0);
			pout2->SetStringValue( tmp.szPname );
			beh->ActivateOutput(1);


	}
	return CKBR_OK;

}


