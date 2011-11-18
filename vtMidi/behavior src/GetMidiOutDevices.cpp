
#include "CKAll.h"

#include "../MidiManager.h"


CKObjectDeclaration	*FillBehaviorGetMidiOutDevicesDecl();
CKERROR CreateGetMidiOutDevicesProto(CKBehaviorPrototype **pproto);
int GetMidiOutDevices(const CKBehaviorContext& behcontext);


/************************************************************************/
/*																																			  */
/************************************************************************/

CKObjectDeclaration	*FillBehaviorGetMidiOutDevicesDecl(){
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Get MidiOut Devices");	
	od->SetDescription("Returns all Midi Out Devices");
	od->SetCategory("Controllers/Midi");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x388d1c1c,0xe3d4bce));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Guenter Baumgart");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateGetMidiOutDevicesProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	return od;
}
CKERROR CreateGetMidiOutDevicesProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Get MidiOut Devices");
	if(!proto) return CKERR_OUTOFMEMORY;
	
	proto->DeclareInput("Get Next");
	proto->DeclareInput("Get Prev");
	
	proto->DeclareOutput("Finish");
	proto->DeclareOutput("LoopOut");
	
	proto->DeclareOutParameter("Name",CKPGUID_STRING);
	proto->DeclareOutParameter("Index",CKPGUID_INT);
	proto->DeclareOutParameter("Count",CKPGUID_INT);
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);

	proto->SetFunction(GetMidiOutDevices);
	*pproto = proto;
	return CK_OK;
}

int indexDO = 0;
int countDO = 0;


int GetMidiOutDevices(const CKBehaviorContext& behcontext)
{

	CKBehavior* beh = behcontext.Behavior;
  	CKContext* ctx = behcontext.Context;


	CKPluginManager* ThePluginManager=CKGetPluginManager();
	CKRenderManager *rm = (CKRenderManager *)ctx->GetRenderManager();

	MIDIOUTCAPS tmp; // get midi devices infos

	

	//////////////////////////////////////////////////////////////////////////
	if( beh->IsInputActive(0) ){

		MidiManager *mm = (MidiManager *) behcontext.Context->GetManagerByGuid( MIDI_MANAGER_GUID );

		countDO=midiInGetNumDevs();
		if( !countDO){
			mm->m_Context->OutputToConsole("No Midi Out Devices !");
			return CK_OK;
		}
		for ( int i = 0 ; i < countDO ; i++ ){

			midiOutGetDevCaps(i, &tmp,sizeof(tmp) );
			mm->m_Context->OutputToConsole(tmp.szPname);

		}
		beh->SetOutputParameterValue(2,&countDO);
		beh->ActivateInput(0, FALSE);
		indexDO = 0;
		beh->ActivateInput(1, TRUE);

	}

	//////////////////////////////////////////////////////////////////////////
	if( beh->IsInputActive(1) ){
			beh->ActivateInput(1, FALSE);
		
			if (indexDO > (countDO-1)){
				indexDO = 0;
				beh->ActivateOutput(0,TRUE);
				return CKBR_OK;
			}
			beh->SetOutputParameterValue(1,&indexDO);
			
				
			midiOutGetDevCaps(indexDO, &tmp,sizeof(tmp) );

			indexDO++;

			

			CKParameterOut *pout2 = beh->GetOutputParameter(0);
			
			pout2->SetStringValue( tmp.szPname );
			beh->ActivateOutput(1);


	}

	


	
	return CKBR_OK;

}


