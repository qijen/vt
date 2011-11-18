#include <virtools/vtcxglobal.h>

#include ".\GBLAsyncBlock.h"


/*
 *******************************************************************
 * Function: CKObjectDeclaration *FillBehaviour( void )
 *
 * Description : As its name infers, this function describes each Building Block
 *               on a functional level : what it can be applied to, its GUID, its 
 *               creation function, etc.. 
 *		
 *
 * Paramters :
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
CKObjectDeclaration * ExeInThread::FillBehaviour( void )
{
    CKObjectDeclaration *objectDeclaration = CreateCKObjectDeclaration( "ExecuteBBInThread" );	
    
    objectDeclaration->SetType( CKDLL_BEHAVIORPROTOTYPE );
    objectDeclaration->SetCreationFunction( ExeInThread::CreatePrototype );
    objectDeclaration->SetDescription( "Executes a BB in a Thread" );
    objectDeclaration->SetCategory( "Narratives/Script Management" );
    objectDeclaration->SetGuid( CKGUID( 0x72387e71,0x89d786d) );
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
 * Paramters :
 *    behaviorPrototypePtr    w   Pointer to a CKBehaviorPrototype object that 
 *                                describes the behavior's internal structure 
 *                                and relationships with other objects.
 *
 * Returns : CKERROR
 *
 *******************************************************************
 */
CKERROR ExeInThread::CreatePrototype( CKBehaviorPrototype** behaviorPrototypePtr )
{
    CKBehaviorPrototype *behaviorPrototype = CreateCKBehaviorPrototype( "ExecuteBBInThread" );
    if ( !behaviorPrototype ) 
    {
        return CKERR_OUTOFMEMORY;
    }
    
	//---     Inputs declaration
	behaviorPrototype->DeclareInput( "In" );

	//---     Outputs declaration
	behaviorPrototype->DeclareOutput( "Out" );



    //----	Local Parameters Declaration

	behaviorPrototype->DeclareLocalParameter("thethread", CKPGUID_POINTER, "0");
	behaviorPrototype->DeclareLocalParameter("thestatus", CKPGUID_INT, "0");


    
    //----	Settings Declaration

	behaviorPrototype->SetBehaviorCallbackFct( ExeInThread::CallBack, CKCB_BEHAVIORATTACH|CKCB_BEHAVIORDETACH|CKCB_BEHAVIORDELETE|CKCB_BEHAVIOREDITED|CKCB_BEHAVIORSETTINGSEDITED|CKCB_BEHAVIORLOAD|CKCB_BEHAVIORPRESAVE|CKCB_BEHAVIORPOSTSAVE|CKCB_BEHAVIORRESUME|CKCB_BEHAVIORPAUSE|CKCB_BEHAVIORRESET|CKCB_BEHAVIORRESET|CKCB_BEHAVIORDEACTIVATESCRIPT|CKCB_BEHAVIORACTIVATESCRIPT|CKCB_BEHAVIORREADSTATE, NULL );
    behaviorPrototype->SetFunction( ExeInThread::BehaviourFunction );
    
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
 * Paramters :
 *    behaviourContext    r   Behavior context reference, which gives access to 
 *                            frequently used global objects ( context, level, manager, etc... )
 *
 * Returns : int, If it is done, it should return CKBR_OK. If it returns 
 *                CKBR_ACTIVATENEXTFRAME, the behavior will again be called 
 *                during the next process loop.
 *
 *******************************************************************
 */
int ExeInThread::BehaviourFunction( const CKBehaviorContext& behaviorContext )
{
	CKBehavior* beh = behaviorContext.Behavior;
	CKContext* ctx = beh->GetCKContext();

	if (beh->IsInputActive(0))
	{

		beh->ActivateInput(0,FALSE);

		VxThread *VXT; 

		int status = 0;
		beh->GetLocalParameterValue(1,&status);

		if (status == ThreadStatus::Requested)
		{	
			return CKBR_ACTIVATENEXTFRAME;
		}

		if (status == ThreadStatus::Active)
		{	
			status = ThreadStatus::Requested;
			beh->SetLocalParameterValue(1,&status);
			return CKBR_ACTIVATENEXTFRAME;
		}

		VXT = new VxThread();
		VXT->SetName("Thread");	
		VXT->SetPriority(VXTP_NORMAL);		
		AsyncThreadInfo * threadInfo;

		threadInfo = new AsyncThreadInfo;

		threadInfo->targetBeh = NULL;

		int count = beh->GetParent()->GetSubBehaviorLinkCount();

		for (int i=0; i<count; i++)
		{
			CKBehaviorLink *link =  beh->GetParent()->GetSubBehaviorLink(i);
			if (link->GetInBehaviorIO() == beh->GetOutput(0))
			{
				threadInfo->targetBeh =  link->GetOutBehaviorIO()->GetOwner();

				int targetInputs = threadInfo->targetBeh->GetInputCount();
				if (targetInputs == 1)
				{
					threadInfo->targetInputToActivate = 0;
				}
				else
				{
					for (int j=0; j<targetInputs; j++)
					{
						if (link->GetOutBehaviorIO() == threadInfo->targetBeh->GetInput(j))
						{
							threadInfo->targetInputToActivate = j;
							break;
						}
					}
				}
				break;
			}
		}

		if (threadInfo->targetBeh == NULL)
		{
			delete threadInfo;
			return CKBR_BEHAVIORERROR;
		}

		VXT->CreateThread(BlockingThreadFunction,threadInfo);
		beh->SetLocalParameterValue(0,&VXT,sizeof(VxThread *));

		status = ThreadStatus::Active;
		beh->SetLocalParameterValue(1,&status);

		return CKBR_ACTIVATENEXTFRAME;

	}
	else
	{
		VxThread *VXT; 
		beh->GetLocalParameterValue(0,&VXT);

		unsigned int status = -1;

		VXT->GetExitCode(status);

		if (status == VXT_OK)
		{
			VXT->Close();
			delete VXT;
			VXT=NULL;
			beh->SetLocalParameterValue(0,&VXT);

			

			int status;
			beh->GetLocalParameterValue(1,&status);
	
			if (status == ThreadStatus::Requested)
			{
				beh->ActivateInput(0);
				status = ThreadStatus::Idle;
				beh->SetLocalParameterValue(1,&status);
				return CKBR_ACTIVATENEXTFRAME;

			}

			status = ThreadStatus::Idle;
			beh->SetLocalParameterValue(1,&status);
			return CKBR_OK;
		}

	}

	return CKBR_ACTIVATENEXTFRAME;
}

/*
*******************************************************************
* Function: int Callback( const CKBehaviorContext& behaviorContext )
*
* Description : The Behavior Callback function is called by Virtools when various events happen 
*               in the life of a BuildingBlock. Exactly which events trigger a call to the 
*               Behavior Callback function is defined in the Behavior Prototype, along with the 
*               declaration of the function pointer 
*
* Parameters :
*    behaviourContext    r   Behavior context reference, which gives 
*                            access to frequently used global objects 
*                            ( context, level, manager, etc... )
*
* Returns : int, The return value of the callback function should be one of the CK_BEHAVIOR_RETURN values.
*
*******************************************************************
*/
int ExeInThread::CallBack( const CKBehaviorContext& behaviorContext )
{
	switch ( behaviorContext.CallbackMessage )
	{
		case CKM_BEHAVIORATTACH:
			break;
		case CKM_BEHAVIORDETACH:
			break;
		case CKM_BEHAVIORDELETE:
			break;
		case CKM_BEHAVIOREDITED:
			break;
		case CKM_BEHAVIORSETTINGSEDITED:
			break;
		case CKM_BEHAVIORLOAD:
			break;
		case CKM_BEHAVIORPRESAVE:
			break;
		case CKM_BEHAVIORPOSTSAVE:
			break;
		case CKM_BEHAVIORRESUME:
		{
			int status = ThreadStatus::Idle;
			behaviorContext.Behavior->SetLocalParameterValue(1,&status);
		}
		break;

		case CKM_BEHAVIORPAUSE:
			break;
		case CKM_BEHAVIORRESET:
			break;
		case CKM_BEHAVIORNEWSCENE:
			break;
		case CKM_BEHAVIORDEACTIVATESCRIPT:
			break;
		case CKM_BEHAVIORACTIVATESCRIPT:
			break;
		case CKM_BEHAVIORREADSTATE:
			break;

	}
	return CKBR_OK;
}

unsigned int BlockingThreadFunction(void *arg)
{
	ExeInThread::AsyncThreadInfo* threadInfo = (ExeInThread::AsyncThreadInfo*)arg;

	threadInfo->targetBeh->ActivateInput(threadInfo->targetInputToActivate);

	int res;

	do
	{
		res = threadInfo->targetBeh->Execute(0);
	}
	while (res == CKBR_ACTIVATENEXTFRAME);
	delete threadInfo;
	return VXT_OK;
}


