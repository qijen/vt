//
// 3DTrans.cpp : Defines the initialization routines for the DLL.
//
#include "CKAll.h"
#include "MidiManager.h"

#ifdef macintosh
#include "BehaviorExport.h"
#endif


#ifdef CK_LIB
	#define RegisterBehaviorDeclarations	Register_MidiBehaviors_BehaviorDeclarations
	#define InitInstance					_MidiBehaviors_InitInstance
	#define ExitInstance					_MidiBehaviors_ExitInstance
	#define CKGetPluginInfoCount			CKGet_MidiBehaviors_PluginInfoCount
	#define CKGetPluginInfo					CKGet_MidiBehaviors_PluginInfo
	#define g_PluginInfo					g_MidiBehaviors_PluginInfo
#else
	#define RegisterBehaviorDeclarations	RegisterBehaviorDeclarations
	#define InitInstance					InitInstance
	#define ExitInstance					ExitInstance
	#define CKGetPluginInfoCount			CKGetPluginInfoCount
	#define CKGetPluginInfo					CKGetPluginInfo
	#define g_PluginInfo					g_PluginInfo
#endif


CKERROR InitInstance(CKContext* context);
CKERROR ExitInstance(CKContext* context);
void RegisterBehaviorDeclarations(XObjectDeclarationArray *reg);

#define MIDI_BEHAVIOR	CKGUID(0x58176f2d,0x6b80544c)
#define CKOGUID_ISNOTEACTIVE    CKDEFINEGUID(0x25d70112,0x683835b8)

CKPluginInfo g_PluginInfo[2];

int CKGetPluginInfoCount() { return 2; }

CKPluginInfo* CKGetPluginInfo(int Index)
{
	g_PluginInfo[0].m_Author			= "Virtools";
	g_PluginInfo[0].m_Description		= "Midi Building Blocks";
	g_PluginInfo[0].m_Extension			= "";
	g_PluginInfo[0].m_Type				= CKPLUGIN_BEHAVIOR_DLL;
	g_PluginInfo[0].m_Version			= 0x000001;
	g_PluginInfo[0].m_InitInstanceFct	= NULL;
	g_PluginInfo[0].m_ExitInstanceFct	= NULL;
	g_PluginInfo[0].m_GUID				= MIDI_BEHAVIOR;
	g_PluginInfo[0].m_Summary			= "Midi Building Blocks";

	g_PluginInfo[1].m_Author			= "Virtools";
	g_PluginInfo[1].m_Description		= "Midi Manager";
	g_PluginInfo[1].m_Extension			= "";
	g_PluginInfo[1].m_Type				= CKPLUGIN_MANAGER_DLL;
	g_PluginInfo[1].m_Version			= 0x000001;
	g_PluginInfo[1].m_InitInstanceFct	= InitInstance;
	g_PluginInfo[1].m_ExitInstanceFct	= ExitInstance;
	g_PluginInfo[1].m_GUID				= MIDI_MANAGER_GUID;
	g_PluginInfo[1].m_Summary			= "Midi Manager";

	return &g_PluginInfo[Index];
}
/**********************************************************************************/
/**********************************************************************************/

///////////////////////
///   Param Op      ///
///////////////////////
void BoolIsNoteActiveIntInt (CKContext *ctx, CKParameterOut *res, CKParameterIn *p1, CKParameterIn *p2){
  int note;
  p1->GetValue(&note);

  int channel;
  p2->GetValue(&channel);

  MidiManager *mm = (MidiManager *) ctx->GetManagerByGuid( MIDI_MANAGER_GUID );
  CKBOOL isNoteActive=mm->IsNoteActive(note, channel);

  res->SetValue(&isNoteActive);
  
}

////////////////////////
//  Initializations   //
////////////////////////
CKERROR InitInstance(CKContext* ctx)
{
  CKParameterManager *pm = ctx->GetParameterManager();

  //--- register a new Operation Type
  pm->RegisterOperationType(CKOGUID_ISNOTEACTIVE, "Is Note Active");

  //--- register a new Parameter Operation
  pm->RegisterOperationFunction(CKOGUID_ISNOTEACTIVE ,CKPGUID_BOOL, CKPGUID_INT, CKPGUID_INT, BoolIsNoteActiveIntInt);

  new MidiManager(ctx);

	return CK_OK;
}

CKERROR ExitInstance(CKContext* ctx)
{
  CKParameterManager *pm = ctx->GetParameterManager();

  //--- register a new Operation Type
  pm->UnRegisterOperationType(CKOGUID_ISNOTEACTIVE);
  
  MidiManager* man=(MidiManager*)ctx->GetManagerByGuid(MIDI_MANAGER_GUID);
  delete man;

  return CK_OK;
}


////////////////////////////////
//  Behaviors Registrations   //
////////////////////////////////
void RegisterBehaviorDeclarations(XObjectDeclarationArray *reg)
{
  RegisterBehavior(reg, FillBehaviorReadMidiSignalDecl);
  RegisterBehavior(reg, FillBehaviorMidiEventDecl);
  RegisterBehavior(reg, FillBehaviorSwitchOnMidiDecl);
  RegisterBehavior(reg, FillBehaviorMidiPlayerDecl);
  
  RegisterBehavior(reg, FillBehaviorSetMidiOutPortDecl);
  RegisterBehavior(reg, FillBehaviorGetMidiINDevicesDecl);
  RegisterBehavior(reg, FillBehaviorGetMidiOutDevicesDecl );
  RegisterBehavior(reg, FillBehaviorSetInMidiPortDecl);


  RegisterBehavior(reg, FillBehaviorSendMidiSignalDecl );

}



CRITICAL_SECTION  gMidiCS;

#if !defined(CK_LIB)
BOOL WINAPI DllMain(HINSTANCE hinstDLL,  // handle to the DLL module
  DWORD fdwReason,     // reason for calling function
  LPVOID lpvReserved   // reserved
){

	switch(fdwReason){
		case DLL_PROCESS_ATTACH:
			InitializeCriticalSection(&gMidiCS); 
			break;
	
		case DLL_PROCESS_DETACH:
			DeleteCriticalSection(&gMidiCS); 			
			break;

	}
	
	return TRUE;
}
#endif