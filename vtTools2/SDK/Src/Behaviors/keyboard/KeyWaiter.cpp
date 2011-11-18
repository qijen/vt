#include <CKALL.h>

CKERROR CreateKeyWaiter2BehaviorProto(CKBehaviorPrototype **pproto);

int KeyWaiter2(const CKBehaviorContext& behcontext);
int ReadKey2(const CKBehaviorContext& behcontext);
CKERROR KeyWaiter2CallBack(const CKBehaviorContext& behcontext);

CKObjectDeclaration	*FillBehaviorKeyWaiter2Decl()
{
	
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Key Waiter2");
	od->SetDescription("Waits for a key to be pressed.");
	/* rem: 
	<SPAN CLASS=in>In: </SPAN>triggers the process.<BR>
	<SPAN CLASS=out>Out: </SPAN>is activated when the process is completed.<BR>
	<BR>
	<SPAN CLASS=pin>Key: </SPAN>key to wait for. This is chosen in the parameter dialog box by first 
	clicking "Select Key" and then pressing the key to wait for. The desired key will then appear to 
	the right of "Select Key".<BR> 
	<BR>
  <SPAN CLASS=setting>Wait For Any Key: </SPAN>if TRUE, the building block will wait any key to be pressed.
  The input parameter is deleted, and a output parameter is created; and in this case the output parameter retrieves the pressed key.<BR>
  Useful for "Press any key to continue..." capabilities.<BR>
  <BR>
  Waits for a key to be pressed. If the setting parameter 'Wait Any Key' is set to FALSE, this building block will 
  wait for a specific given key (default), if set to TRUE, then it will wait for anykey to be pressed, retrieving this key as an output parameter.<BR>
  <BR>
  See Also: Switch On Key, Key Event.<BR>
	*/
	od->SetCategory("Controllers/Keyboard");
	od->SetType(CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x60280837,0x38976869));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("mw");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateKeyWaiter2BehaviorProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->NeedManager(INPUT_MANAGER_GUID);
	return od;
}


CKERROR CreateKeyWaiter2BehaviorProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Key Waiter2");
	if(!proto) return CKERR_OUTOFMEMORY;
	
	proto->DeclareInput("In");
	proto->DeclareInput("StopIt");
	proto->DeclareOutput("Out");
	
	proto->DeclareInParameter("Key", CKPGUID_KEY);

  proto->DeclareSetting("Wait For Any Key", CKPGUID_BOOL, "FALSE");
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(KeyWaiter2);
	proto->SetBehaviorCallbackFct(KeyWaiter2CallBack);
  proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)(CKBEHAVIOR_INTERNALLYCREATEDINPUTPARAMS|CKBEHAVIOR_INTERNALLYCREATEDOUTPUTPARAMS));
	
	*pproto = proto;
	return CK_OK;
}


/*******************************************************/
/*                     CALLBACK                        */
/*******************************************************/
CKERROR KeyWaiter2CallBack(const CKBehaviorContext& behcontext){
	
	CKBehavior* beh = behcontext.Behavior;

	switch( behcontext.CallbackMessage ){
	case CKM_BEHAVIORLOAD:
	case CKM_BEHAVIORSETTINGSEDITED:
		{
      CKBOOL anyKey=FALSE;
      beh->GetLocalParameterValue(0, &anyKey);

      CKParameterIn *pin = beh->GetInputParameter(0);
      if( anyKey ){ // wait any Key
        beh->SetFunction(ReadKey2);
        if( pin ){
          CKDestroyObject( beh->RemoveInputParameter(0) );
          beh->CreateOutputParameter("Key", CKPGUID_KEY);
        }
      } else { // wait specific Key
        beh->SetFunction(KeyWaiter2);
        if( !pin ){
          CKDestroyObject( beh->RemoveOutputParameter(0) );
          beh->CreateInputParameter("Key", CKPGUID_KEY);
        }
      }
    } break;
	}
	
	return CKBR_OK; 
}


/*******************************************************/
/*               Main Function 1                       */
/*******************************************************/
int KeyWaiter2(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;

	CKInputManager* input = (CKInputManager*)behcontext.Context->GetManagerByGuid(INPUT_MANAGER_GUID);
	if (!input){
		behcontext.Context->OutputToConsoleEx("Can't get the Input Manager");
		return CKBR_GENERICERROR;
	}
	
	if ( beh->IsInputActive( 1 ) )
	{
		beh->ActivateInput( 0 , FALSE);
		beh->ActivateInput( 1 , FALSE);

		
		beh->ActivateOutput(0,FALSE);
		return CKBR_OK;
	}

  int key = 0;
  beh->GetInputParameterValue(0,&key);
  if( input->IsKeyDown(key) ){
    // Set IO states
    beh->ActivateInput(0,FALSE);
    beh->ActivateOutput(0,TRUE);
    return CKBR_OK;
  }
  
	return CKBR_ACTIVATENEXTFRAME;
}


/*******************************************************/
/*               Main Function 2                       */
/*******************************************************/
int ReadKey2(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;

	CKInputManager* input = (CKInputManager*)behcontext.Context->GetManagerByGuid(INPUT_MANAGER_GUID);
	if (!input){
		behcontext.Context->OutputToConsoleEx("Can't get the Input Manager");
		return CKBR_GENERICERROR;
	}

	if ( beh->IsInputActive( 1 ) )
	{
		beh->ActivateInput( 0 , FALSE);
		beh->ActivateInput( 1 , FALSE);


		beh->ActivateOutput(0,FALSE);
		return CKBR_OK;
	}

	CKDWORD key = 0;
	if(input->GetKeyFromBuffer(0,key) == KEY_PRESSED) {
		// Set IO states
		beh->ActivateInput(0,FALSE);
		beh->ActivateOutput(0,TRUE);
		
		// write the key
		beh->SetOutputParameterValue(0,&key);
		
		return CKBR_OK;
	}
	
	return CKBR_ACTIVATENEXTFRAME;
}


