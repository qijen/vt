/********************************************************************
	created:	2006/05/07
	created:	05:07:2006   8:14
	filename: 	x:\junctions\ProjectRoot\current\vt_plugins\vt_toolkit\Behaviors\Generic\BGInstancer.cpp
	file path:	x:\junctions\ProjectRoot\current\vt_plugins\vt_toolkit\Behaviors\Generic
	file base:	BGInstancer
	file ext:	cpp
	author:		mc007
	
	purpose:	
*********************************************************************/
#include <virtools/vtcxglobal.h> //for pch only,can be removed!

#include "BGInstancer.h"



/*
 *******************************************************************
 * Function: CKObjectDeclaration *FillBehaviour( void )
 *
 * Description : As its name infers, this function describes each Building Block
 *               on a functional level : what it can be applied to, its GUID, its 
 *               creation function, etc.. 
 *		
 *
 * Parameters :
 *    None
 *
 * Returns : CKObjectDeclaration *, containing:
 *               - The type of object declaration ( Must Be CKDLL_BEHAVIORPROTOTYPE )
 *               - The function that will create the CKBehaviorPrototype for this behavior.
 *               - A short description of what the behavior is supposed to do.
 *               - The category in which this behavior will appear in the Virtools interface.
 *               - A unique CKGUID
 *               - Author and Version info
 *               - The class identifier of objects to which the behavior can be applied to.
 *
 *******************************************************************
 */
CKObjectDeclaration * BGWrapper::FillBehaviour( void )
{
    CKObjectDeclaration *objectDeclaration = CreateCKObjectDeclaration( "BgInstancer" );	
    
    objectDeclaration->SetType( CKDLL_BEHAVIORPROTOTYPE );
    objectDeclaration->SetCreationFunction( BGWrapper::CreatePrototype );
    objectDeclaration->SetDescription( "Encapsulates the functionality provided by a Behaviour Graph and allows reuse while minimising maintenance overhead." );
    objectDeclaration->SetCategory( "Narratives/Script Management" );
    objectDeclaration->SetGuid( BGWRAPPER_GUID );
    objectDeclaration->SetVersion( 0x00000001 );
    objectDeclaration->SetAuthorGuid( VTCX_AUTHOR_GUID );
    objectDeclaration->SetAuthorName( VTCX_AUTHOR );
    objectDeclaration->SetCompatibleClassId( CKCID_BEOBJECT );
    
    return objectDeclaration;
}

/*
 *******************************************************************
 * Function: CKERROR CreatePrototype( CKBehaviorPrototype** behaviorPrototypePtr )
 *
 * Description : The prototype creation function will be called the first time 
 *               a behavior must be created to create the CKBehaviorPrototype 
 *               that contains the description of the behavior.
 *
 * Parameters :
 *    behaviorPrototypePtr        Pointer to a CKBehaviorPrototype object that 
 *                                describes the behavior's internal structure 
 *                                and relationships with other objects.
 *
 * Returns : CKERROR
 *
 *******************************************************************
 */
CKERROR BGWrapper::CreatePrototype( CKBehaviorPrototype** behaviorPrototypePtr )
	{

#if RUNTIME	
	// Not editable from Virtools Dev
	CKBehaviorPrototype *behaviorPrototype = CreateCKBehaviorPrototypeRunTime( "BGWrapper" );
#elif GAMEDEVELOPER 
	// Edition depend on the BB.
	CKBehaviorPrototype *behaviorPrototype = CreateCKBehaviorPrototype( "BGWrapper" );
#else					
	// Editable from Virtools Dev
	CKBehaviorPrototype *behaviorPrototype = CreateCKBehaviorPrototype( "BGWrapper" );
#endif 

	
	if ( !behaviorPrototype ) 
        return CKERR_OUTOFMEMORY;

    //----	Local Parameters Declaration
	behaviorPrototype->DeclareLocalParameter("BG Script", CKPGUID_BEHAVIOR );

    //----	Settings Declaration
	behaviorPrototype->DeclareSetting("BG filename", CKPGUID_STRING ,"path undefined.(BGWrapper Settings)");

	behaviorPrototype->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);

	behaviorPrototype->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)(CKBEHAVIOR_INTERNALLYCREATEDINPUTS |
												CKBEHAVIOR_INTERNALLYCREATEDINPUTPARAMS |
												CKBEHAVIOR_INTERNALLYCREATEDOUTPUTS |
												CKBEHAVIOR_INTERNALLYCREATEDOUTPUTPARAMS));

	behaviorPrototype->SetBehaviorCallbackFct(BGWrapperCB,
												CKCB_BEHAVIORLOAD |
												CKCB_BEHAVIORRESET |
												CKCB_BEHAVIORSETTINGSEDITED |
												CKCB_BEHAVIORDETACH |
												CKCB_BEHAVIORDEACTIVATESCRIPT
												, NULL);

    behaviorPrototype->SetFunction( BGWrapper::BehaviourFunction );
    
    *behaviorPrototypePtr = behaviorPrototype;
    return CK_OK;
}

/*
 *******************************************************************
 * Function: int BehaviourFunction( const CKBehaviorContext& behaviorContext )
 *
 * Description : The execution function is the function that will be called 
 *               during the process loop of the behavior engine, if the behavior 
 *               is defined as using an execution function. This function is not 
 *               called if the behavior is defined as a graph. This function is the 
 *               heart of the behavior: it should compute the essence of the behavior, 
 *               in an incremental way. The minimum amount of computing should be 
 *               done at each call, to leave time for the other behaviors to run. 
 *               The function receives the delay in milliseconds that has elapsed 
 *               since the last behavioral process, and should rely on this value to 
 *               manage the amount of effect it has on its computation, if the effect 
 *               of this computation relies on time.
 *
 * Parameters :
 *    behaviourContext	      Behavior context reference, which gives access to 
 *                            frequently used global objects ( context, level, manager, etc... )
 *
 * Returns : int, If it is done, it should return CKBR_OK. If it returns 
 *                CKBR_ACTIVATENEXTFRAME, the behavior will again be called 
 *                during the next process loop.
 *
 *******************************************************************
 */

int BGWrapper::BehaviourFunction( const CKBehaviorContext& behContext )
{
	CKBehavior	*behaviour = behContext.Behavior;
	CKContext	*context = behContext.Context;
	int			iPin, nbPin;

	CKBehavior* script = (CKBehavior*)behaviour->GetLocalParameterObject(EBGWRAPPERPARAM_PARAMETER_SCRIPT);

	if (script == NULL)
		return CKBR_GENERICERROR;

	// Activate the right inputs
	nbPin = behaviour->GetInputCount();
	for (iPin = 0; iPin < nbPin; iPin++)
		{
		if (behaviour->IsInputActive(iPin))
			{
			script->ActivateInput(iPin, TRUE);
			behaviour->ActivateInput(iPin, FALSE);
			}
		}
	
	// Deactivate all the outputs
	nbPin = script->GetOutputCount();
	for (iPin = 0; iPin < nbPin; iPin++)
		{
		behaviour->ActivateOutput(iPin, FALSE);
		}

	// Parameter In: Set Source
	int nbPinBB = behaviour->GetInputParameterCount();
	int nbPinBG = script->GetInputParameterCount();

	if (nbPinBB != nbPinBG)
		return CKBR_GENERICERROR;

	for (iPin = 0; iPin < nbPinBB; iPin++)
	{
		CKParameterIn *pBin = behaviour->GetInputParameter(iPin);
		CKParameter* pSource = pBin->GetDirectSource();

		CKParameterIn *pSin = script->GetInputParameter(iPin);
		pSin->SetDirectSource(pSource);
	}

	// Execute the contained script
	CKERROR result = script->Execute(behContext.DeltaTime);

	// The script loop on itself too much times
	if (result == CKBR_INFINITELOOP) 
		{
		context->OutputToConsoleExBeep("Execute Script : Script %s Executed too much times",script->GetName());
		script->Activate(FALSE,FALSE);
		return CKBR_OK;
		}

	// Activate the right outputs 
	nbPin = script->GetOutputCount();
	for (iPin = 0; iPin < nbPin; iPin++)
		{
		if (script->IsOutputActive(iPin))
			{
			behaviour->ActivateOutput(iPin);
			script->ActivateOutput(iPin, FALSE);
			}
		}

	// Update Parameters Out
	nbPin = behaviour->GetOutputParameterCount();
	for (iPin = 0; iPin < nbPin; iPin++)  
		{
		CKParameterOut *pBout = behaviour->GetOutputParameter(iPin);
		CKParameterOut *pSout = script->GetOutputParameter(iPin);
		pBout->CopyValue(pSout, TRUE);
		}

	
	// Test if there are any active sub-behaviors, restart the next frame if any
	BOOL bActivateNextFrame = FALSE;
	ActivateNextFrameSubBB(script,bActivateNextFrame);
	if (bActivateNextFrame)
		return CKBR_ACTIVATENEXTFRAME;

	// return the execute value
	return result;	
}

/*
 *******************************************************************
 * Function: CKERROR BGWrapperCB(const CKBehaviorContext& behContext)
 *
 * Description : The Behavior Callback function is called by Virtools 
 *               when various events happen in the life of a BuildingBlock.
 *
 * Parameters :
 *    behaviourContext	      Behavior context reference, which gives access to 
 *                            frequently used global objects ( context, level, manager, etc... )
 *
 * Returns : CKERROR
 *
 *******************************************************************
 */
CKERROR BGWrapper::BGWrapperCB(const CKBehaviorContext& behContext)
{
	CKERROR	result = CKBR_GENERICERROR; 

	// Initialize common pointers.	
	CKBehavior *behaviour   = behContext.Behavior;
	CKContext  *context   = behContext.Context;
	CKLevel    *level = behContext.CurrentLevel;
	CKBeObject *owner = behContext.Behavior->GetOwner();
	
	char*name = behaviour->GetName();
	
	if ( behaviour == NULL || context == NULL || level == NULL /*|| owner == NULL*/ )
		return CKBR_OK;
	
	//Get The BG Script Object if exists
	CKBehavior* newBG = NULL;
	CKBehavior* curBG = (CKBehavior*)behaviour->GetLocalParameterObject(EBGWRAPPERPARAM_PARAMETER_SCRIPT);

	// Get the BG nms file path. GetStringValue doesn't work with setting...
	char fileName[_MAX_PATH+1];
	memset(fileName,0,_MAX_PATH+1);
	CKParameter* scriptPath  = behaviour->GetLocalParameter(EBGWRAPPERPARAM_PARAMETER_NAME);
	if ( scriptPath == NULL )
		return CKBR_OK;
	
	int lenPath  = scriptPath->GetDataSize();
	if ( lenPath >= _MAX_PATH )
		return CKBR_OK;
	
	void*ptrPath = scriptPath->GetReadDataPtr(TRUE);
	if ( ptrPath == NULL )
		return CKBR_OK;
	
	memcpy(fileName,ptrPath,lenPath);

	CKDWORD message = behContext.CallbackMessage;

	switch (message)
		{
		case CKM_BEHAVIORLOAD :				// when the behavior is loaded
		case CKM_BEHAVIORRESET:				// when the behavior is reseted
		case CKM_BEHAVIORSETTINGSEDITED :	// when the settings are edited
			{
				if ( curBG != NULL )
				{
					DestroyCurrentBG(level,behaviour,curBG);
					curBG = NULL;
				}

				newBG = BGLoader( fileName, behContext );
				if ( newBG == NULL )
					return CKBR_OK;
				
				if ( message == CKM_BEHAVIORLOAD || message == CKM_BEHAVIORRESET )
				{
					//context->OutputToConsoleExBeep("%s : LOADED %s",behaviour->GetName(), fileName);
					if ( CheckIO(behaviour, newBG) == TRUE )
						result = CKBR_OK;
					else	
						context->OutputToConsoleExBeep("%s : Too many inputs/outputs changes in %s\r\nPlease reconstruct the wrapper.",behaviour->GetName(), fileName);
				}
				else if ( message == CKM_BEHAVIORSETTINGSEDITED )
				{
					if ( CheckIO(behaviour, newBG) == TRUE )
					{					
						result = CKBR_OK;
					}
					else if (DeleteIO(behaviour) == TRUE)
					{
						if ( CreateIO(behaviour, newBG ) == TRUE )
							result = CKBR_OK;
						else	
							context->OutputToConsoleExBeep("%s : Cannot Create Inputs/Outputs %s",behaviour->GetName(), fileName);
					}
					else
						context->OutputToConsoleExBeep("%s : Cannot Delete Inputs/Outputs %s",behaviour->GetName(), fileName);
				}

				if ( result == CKBR_OK && newBG != NULL )
					SetNewBG(behaviour,newBG);
			}
			break;
	
		case CKM_BEHAVIORDEACTIVATESCRIPT:
			{
				if ( curBG != NULL )
					DesactivateSubBB(curBG);
				result = CKBR_OK;
			}
			break;
		
		case CKM_BEHAVIORDETACH : // when the behavior is deleted
			{
				if (curBG != NULL)
				{
					DestroyCurrentBG(level,behaviour,curBG);
					curBG = NULL;
				}
				result = CKBR_OK;
			}
			break;				
		
		default: 
			{
				result = CKBR_OK;
			}
			break;

		}
		
	if (result != CKBR_OK)
		context->OutputToConsoleExBeep("%s : Problem while manipulating",behaviour->GetName());



	return result;
	}


 /*
 *******************************************************************
 * Function: CKBehavior* BGLoader(CKSTRING fileName,const CKBehaviorContext& behContext)
 *
 * Description : Load a virtools script.and add it to the current level.
 *
 * Parameters :
 *    fileName				  string containing	the script filename to be loaded.
 *    behaviourContext	      Behavior context reference, which gives access to 
 *                            frequently used global objects ( context, level, manager, etc... )
 *
 * Returns : CKBehavior* the loaded behaviour, NULL if failed.
 *
 *******************************************************************
 */
CKBehavior* BGWrapper::BGLoader(CKSTRING fileName,const CKBehaviorContext& behContext)
{
	CKERROR	result = CKBR_GENERICERROR;

	// Initialize common pointers.	
	CKBehavior *behaviour   = behContext.Behavior;
	CKContext  *context   = behContext.Context;
	CKLevel    *level = behContext.CurrentLevel;
	CKBeObject *owner = behContext.Behavior->GetOwner();

	if ( behaviour == NULL || context == NULL || level == NULL )
		return NULL;
	
	char fileToLoad[_MAX_PATH],nakedFileName[_MAX_PATH];
	char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	_splitpath(fileName, drive, dir, fname, ext);

	if ( ext[0] == 0 )
		strcpy(ext,".nms");

	strcpy(fileToLoad,drive);
	strcat(fileToLoad,dir);
	strcat(fileToLoad,fname);
	strcat(fileToLoad,ext);

	if ( strcmp(_strlwr(ext),".nms") != 0 ) 
	{
		context->OutputToConsoleExBeep("BGWrapper : Can only load .nms files %s",fileToLoad);
		return NULL;
	}
	
	CKObjectArray* array = CreateCKObjectArray();

	context->SetAutomaticLoadMode(CKLOAD_OK, CKLOAD_OK, CKLOAD_USECURRENT, CKLOAD_USECURRENT);

	// Virtools Load the BGs nms file.first try the absolute path, then the lastCmoloaded path, and then the PathManager manager data paths
	if (context->Load(fileToLoad, array, (CK_LOAD_FLAGS)(CK_LOAD_AUTOMATICMODE | CK_LOAD_AS_DYNAMIC_OBJECT )) != CK_OK)
	{
		strcpy(nakedFileName,fname);
		strcat(nakedFileName,ext);

		CKSTRING lastCmo = context->GetLastCmoLoaded();
		char driveCmo[_MAX_DRIVE], dirCmo[_MAX_DIR], fnameCmo[_MAX_FNAME], extCmo[_MAX_EXT];
		_splitpath(lastCmo, driveCmo, dirCmo, fnameCmo, extCmo);

		strcpy(fileToLoad,driveCmo);strcat(fileToLoad,dirCmo);strcat(fileToLoad,nakedFileName);

		if (context->Load(fileToLoad, array, (CK_LOAD_FLAGS)(CK_LOAD_AUTOMATICMODE | CK_LOAD_AS_DYNAMIC_OBJECT )) != CK_OK)
		{
			// failed then try to go thru the data path.
			CKPathManager* pathmanager = behContext.Context->GetPathManager();
			if (!pathmanager) 
			{
				context->OutputToConsoleExBeep("BGWrapper : Cannot find the Path Manager");
				return NULL;
			}

			XString resolved(nakedFileName);
			array->Clear();
			pathmanager->ResolveFileName(resolved,DATA_PATH_IDX,-1);
			if (context->Load(resolved.Str(), array, (CK_LOAD_FLAGS)(CK_LOAD_AUTOMATICMODE | CK_LOAD_AS_DYNAMIC_OBJECT )) != CK_OK)
			{
				context->OutputToConsoleExBeep("BGWrapper : Cannot Load %s", nakedFileName);
				return NULL;
			}
		}


	}

	// Check if only one Object is loaded (we should have only one BG here)
	if ( array->GetCount() != 1 )
	{
		context->OutputToConsoleExBeep("BGWrapper : To many objects inside the nms %s.It should contain one BG only.",fileToLoad);

		level->BeginRemoveSequence(TRUE);
		for (array->Reset(); !array->EndOfList(); array->Next()) 
		{
			CKObject* curObject = array->GetData(context);
			level->RemoveObject(curObject);
			CKDestroyObject(curObject);
		}
		level->BeginRemoveSequence(FALSE);		
		DeleteCKObjectArray(array);

		return NULL;
	}

	array->Reset();
	CKObject* curObject = array->GetData(context);
	
	if ( curObject == NULL )
	{
		context->OutputToConsoleExBeep("BGWrapper : Object NULL in loaded array.");
		return NULL;
	}

	// Make it not to be saved even if it is loaded as dynamic... (not needed ?)
	curObject->ModifyObjectFlags( CK_OBJECT_NOTTOBESAVED,0 );
	
	// Check if the object we've loaded is derivated from the BEHAVIOR
	if ( !CKIsChildClassOf(curObject, CKCID_BEHAVIOR) )
	{
		context->OutputToConsoleExBeep("BGWrapper : no behavior in the nms : %s",fileToLoad);

		level->BeginRemoveSequence(TRUE);
		level->RemoveObject(curObject);
		level->BeginRemoveSequence(FALSE);		
		CKDestroyObject(curObject);
		DeleteCKObjectArray(array);

		return NULL;
	}

	CKBehavior*pBG = (CKBehavior*)curObject;

	// Check if the behavior we've loaded is a BG and not a BB.
	if ( pBG->IsUsingFunction() )
	{
		context->OutputToConsoleExBeep("BGWrapper : BGWrapper accepts only a BG not a BB : %s",fileToLoad);

		level->BeginRemoveSequence(TRUE);
		level->RemoveObject(curObject);
		level->BeginRemoveSequence(FALSE);		
		CKDestroyObject(curObject);
		DeleteCKObjectArray(array);

		return NULL;
	}

	// Check if the BG can be applied to the <BGWrapper BB>'s BeObject owner.
	char*nameee = pBG->GetName();
	CK_CLASSID cid = pBG->GetCompatibleClassID();
	
	
	if (owner!=NULL)
		if ( !CKIsChildClassOf(owner, pBG->GetCompatibleClassID()) )
		{
			context->OutputToConsoleExBeep("BGWrapper : Incompatible Class, cannot add BG to script %s",fileToLoad);

			level->BeginRemoveSequence(TRUE);
			level->RemoveObject(curObject);
			level->BeginRemoveSequence(FALSE);		
			CKDestroyObject(curObject);
			DeleteCKObjectArray(array);

			return NULL;
		}

	// Add the BG to the <BGWrapper BB>'s level.
	level->BeginAddSequence(TRUE);
	level->AddObject(curObject);
	level->BeginAddSequence(FALSE);
	DeleteCKObjectArray(array);

	if (owner!=NULL)
		OwnerSubBB(pBG,owner);

	return pBG;
}


 /*
 *******************************************************************
 * Function: BOOL CheckIO(CKBehavior* behaviour, CKBehavior* script)
 *
 * Description : Check if all the Inputs,Outputs, pIns and pOuts of both behavior are matching
 *
 * Parameters :
 *    behaviour		  Behavior 1, usually the BG wrapper BB.
 *    script	      Behavior 2, usually the wrapped BG.
 *
 * Returns : BOOL
 *
 *******************************************************************
 */
BOOL BGWrapper::CheckIO(CKBehavior* behaviour, CKBehavior* script)
	{
	int		iParam;
	int		nbPin0, nbPin1;
	int		flag = 0;
	
	// Input
	nbPin0 = behaviour->GetInputCount();
	nbPin1 = script->GetInputCount();
	if (nbPin0 != nbPin1)
		return FALSE;

	// Ouput
	nbPin0 = behaviour->GetOutputCount();
	nbPin1 = script->GetOutputCount();
	if (nbPin0 != nbPin1)
		return FALSE;

	// Parameter In
	nbPin0 = behaviour->GetInputParameterCount();
	nbPin1 = script->GetInputParameterCount();
	if (nbPin0 != nbPin1)
		return FALSE;
	for (iParam = 0; iParam < nbPin0; iParam++)
		{
		CKParameterIn* pSin = script->GetInputParameter(iParam);
		CKParameterIn* pBin = behaviour->GetInputParameter(iParam);
		if (pSin == NULL || pBin == NULL)
			return FALSE;
		if (pSin->GetType() != pBin->GetType())
			return FALSE;
		}

	// Parameter Out
	nbPin0 = behaviour->GetOutputParameterCount();
	nbPin1 = script->GetOutputParameterCount();
	if (nbPin0 != nbPin1)
		return FALSE;
	for (iParam = 0; iParam < nbPin0; iParam++)
		{
		CKParameterOut* pSout = script->GetOutputParameter(iParam);
		CKParameterOut* pBout = behaviour->GetOutputParameter(iParam);
		if (pSout == NULL || pBout == NULL)
			return FALSE;
		if (pSout->GetType() != pBout->GetType())
			return FALSE;
		}

	return TRUE;
	}


 /*
 *******************************************************************
 * Function: BOOL	:CreateIO(CKBehavior* behaviour, CKBehavior* script)
 *
 * Description : Check if all the Inputs,Outputs, pIns and pOuts of both behavior are matching
 *
 * Parameters :
 *    behaviour		  Behavior 1, usually the BG wrapper BB.
 *    script	      Behavior 2, usually the wrapped BG.
 *
 * Returns : BOOL
 *
 *******************************************************************
 */
BOOL	BGWrapper::CreateIO(CKBehavior* behaviour, CKBehavior* script)
	{
	int		iIO;
	int		nbPin;
	
	// Input
	nbPin = script->GetInputCount();
	for (iIO = 0; iIO < nbPin; iIO++)
		{
		CKBehaviorIO* pPin = script->GetInput(iIO);
		if (pPin == NULL)
			return FALSE;
		
		if (behaviour->AddInput(pPin->GetName()) != iIO)
			return FALSE;
		}

	// Output
	nbPin = script->GetOutputCount();
	for (iIO = 0; iIO < nbPin; iIO++)
		{
		CKBehaviorIO* pPin = script->GetOutput(iIO);
		if (pPin == NULL)
			return FALSE;
		
		if (behaviour->AddOutput(pPin->GetName()) != iIO)
			return FALSE;
		}

	// Parameter In
	nbPin = script->GetInputParameterCount();
	for (iIO = 0; iIO < nbPin; iIO++)
		{
		CKParameterIn* pPin = script->GetInputParameter(iIO);
		if (pPin == NULL)
			return FALSE;
		
		CKParameterIn* pPin2;
		if ((pPin2 = behaviour->CreateInputParameter(pPin->GetName(), (CKParameterType)pPin->GetType())) == NULL)
			return FALSE;
		}

	// Parameter Out
	nbPin = script->GetOutputParameterCount();
	for (iIO = 0; iIO < nbPin; iIO++)
		{
		CKParameterOut* pPin = script->GetOutputParameter(iIO);
		if (pPin == NULL)
			return FALSE;
		
		if (behaviour->CreateOutputParameter(pPin->GetName(), (CKParameterType)pPin->GetType()) == NULL)
			return FALSE;
		}

	return TRUE;
	}

 /*
 *******************************************************************
 * Function: BOOL	DeleteIO(CKBehavior* behaviour)
 *
 * Description : Delete all kind of inputs/outputs on the given behavior
 *
 * Parameters :
 *    behaviour		  The behavior to be naked on.
 *
 * Returns : BOOL
 *
 *******************************************************************
 */
BOOL	BGWrapper::DeleteIO(CKBehavior* behaviour)
	{
	int		iIO;
	int		nbPin0;
	
	// Input
	nbPin0 = behaviour->GetInputCount();
	for (iIO = nbPin0-1; iIO >= 0; iIO--)
		{
		if (behaviour->DeleteInput(iIO) != CK_OK)
			return FALSE;
		}

	// Ouput
	nbPin0 = behaviour->GetOutputCount();
	for (iIO = nbPin0-1; iIO >= 0; iIO--)
		{
		if (behaviour->DeleteOutput(iIO) != CK_OK)
			return FALSE;
		}

	// Parameter In
	nbPin0 = behaviour->GetInputParameterCount();
	for (iIO = nbPin0-1; iIO >= 0; iIO--)
		{
		CKParameterIn* pParam;

		if ((pParam = behaviour->RemoveInputParameter(iIO)) == NULL)
			return FALSE;
		CKDestroyObject(pParam);
		}

	// Parameter Out
	nbPin0 = behaviour->GetOutputParameterCount();
	for (iIO = nbPin0-1; iIO >= 0; iIO--)
		{
		CKParameterOut* pParam;
		if ((pParam = behaviour->RemoveOutputParameter(iIO)) == NULL)
			return FALSE;
		CKDestroyObject(pParam);
		}

	return TRUE;
	}

 /*
 *******************************************************************
 * Function: BOOL	HasIO(CKBehavior* behaviour)
 *
 * Description : return TRUE if the behavior has almost one IN/OU/PIN/POUT
 *
 * Parameters :
 *    behaviour		  The behavior to be checked.
 *
 * Returns : BOOL
 *
 *******************************************************************
 */

BOOL	BGWrapper::HasIO(CKBehavior* behaviour)
	{
	// Input
	if (behaviour->GetInputCount())
		return TRUE;

	// Ouput
	if (behaviour->GetOutputCount())
		return TRUE;

	// Parameter In
	if (behaviour->GetInputParameterCount())
		return TRUE;

	// Parameter Out
	if (behaviour->GetOutputParameterCount())
		return TRUE;

	return FALSE;
	}

 /*
 *******************************************************************
 * Function: void ActivateNextFrameSubBB(CKBehavior* scriptObject,BOOL &bActivateNextFrame)
 *
 * Description : set the bActivateNextFrame to TRUE if one sub behavior is still active.
 *
 * Parameters :
 *    scriptObject	  The behavior to be checked.
 *
 * Returns : void
 *
 *******************************************************************
 */
void BGWrapper::ActivateNextFrameSubBB(CKBehavior* scriptObject,BOOL &bActivateNextFrame)
{
	if ( scriptObject == NULL )  
		return;
	
	if ( scriptObject->IsActive() )
	{
		bActivateNextFrame = TRUE;
		return;
	}

	for ( int i = 0; i < scriptObject->GetSubBehaviorCount(); i++)
	{
		CKBehavior*scriptSub = scriptObject->GetSubBehavior(i);
		ActivateNextFrameSubBB(scriptSub,bActivateNextFrame);
	}
}

 /*
 *******************************************************************
 * Function: void DesactivateSubBB(CKBehavior* scriptObject)
 *
 * Description : Desactivate all sub-Behavior
 *
 * Parameters :
 *    scriptObject	  The behavior to be totally desactivated.
 *
 * Returns : void
 *
 *******************************************************************
 */
void BGWrapper::DesactivateSubBB(CKBehavior* scriptObject)
{
	if ( scriptObject == NULL )  
		return;
	
	scriptObject->Activate(FALSE,TRUE);

	for ( int i = 0; i < scriptObject->GetSubBehaviorCount(); i++)
	{
		CKBehavior*scriptSub = scriptObject->GetSubBehavior(i);
		DesactivateSubBB(scriptSub);
	}
}

 /*
 *******************************************************************
 * Function: void OwnerSubBB(CKBehavior* scriptObject,CKBeObject*owner)
 *
 * Description : Set Owner ptr to all sub-Behavior.
 *
 * Parameters :
 *    scriptObject	  behavior to be parsed.
 *    owner           owner to be assigned.
 *
 * Returns : void
 *
 *******************************************************************
 */
void BGWrapper::OwnerSubBB(CKBehavior* scriptObject,CKBeObject*owner)
{
	if ( scriptObject == NULL )  
		return;
	
	scriptObject->SetSubBehaviorOwner(owner);
	scriptObject->SetOwner(owner);

	for ( int i = 0; i < scriptObject->GetSubBehaviorCount(); i++)
	{
		CKBehavior*scriptSub = scriptObject->GetSubBehavior(i);
		OwnerSubBB(scriptSub,owner);
	}
}

 /*
 *******************************************************************
 * Function: void	SetNewBG(CKBehavior *behaviour,CKBehavior *newBehavior)
 *
 * Description : Set new BB name and store the BG locally.
 *
 * Parameters :
 *    behaviour			BG wrapper BB
 *    newBehavior       BG assigned to the wrapper
 *
 * Returns : void
 *
 *******************************************************************
 */
void	BGWrapper::SetNewBG(CKBehavior *behaviour,CKBehavior *newBehavior)
{
	if ( behaviour == NULL || newBehavior == NULL )
		return;

	CKSTRING nameBG = newBehavior->GetName();
	size_t len = strlen(nameBG);
	char*buf = (char*)malloc(len+128);
	if ( buf != NULL )
	{
		sprintf(buf, "BI(%s)", newBehavior->GetName());
		behaviour->SetName(buf);
		free(buf);
	}
	else
		behaviour->SetName("BI(###ERROR###)");

	behaviour->SetLocalParameterObject(EBGWRAPPERPARAM_PARAMETER_SCRIPT,newBehavior);
}

/*
 *******************************************************************
 * Function: void	DestroyCurrentBG(CKLevel* level,CKBehavior *behaviour,CKBehavior *scriptObject)
 *
 * Description : Destroy the currently wrapped BG
 *
 * Parameters :
 *    level           level from witch the BG will be removed
 *    behaviour		  BG wrapper BB.
 *    scriptObject    BG assigned to the wrapper.
 *
 * Returns : void
 *
 *******************************************************************
 */
void	BGWrapper::DestroyCurrentBG(CKLevel* level,CKBehavior *behaviour,CKBehavior *scriptObject)
{
	if ( level == NULL || behaviour == NULL || scriptObject == NULL )
		return;

	behaviour->SetName("BI(###Failed###)");

	level->BeginRemoveSequence(TRUE);
	level->RemoveObject(scriptObject);
	level->BeginRemoveSequence(FALSE);		
	CKDestroyObject(scriptObject);
	
	behaviour->SetLocalParameterObject(EBGWRAPPERPARAM_PARAMETER_SCRIPT,NULL);

}
