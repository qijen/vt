#include <StdAfx.h>
#include "vtPhysXAll.h"
#include <xDebugTools.h>


class PhysicVariableWatcher : public CKVariableManager::Watcher  
{
public:
	PhysicVariableWatcher(CKContext* context, XString variableName, XString currentValue);

	virtual void PostWrite(const char* iName);
private:
	PhysicVariableWatcher();

	CKContext* context;
	CKBOOL manualSetInProgress;
	XString variableName;
	XString currentValue;
};

PhysicVariableWatcher::PhysicVariableWatcher(CKContext* context, XString variableName, XString currentValue)
{
	this->context = context;
	this->manualSetInProgress = FALSE;
	this->variableName = variableName;
	this->currentValue = currentValue;
}

static PhysicVariableWatcher *watcherConsole=NULL;


void PhysicVariableWatcher::PostWrite(const char* iName)
{

	
	XString newValue;
	XString msg;

	// Check if we are currently manually setting the value
	if (this->manualSetInProgress)
	return;

	if( !context || !context->GetVariableManager())
		return;

	if(!GetPMan())
		return;

	// Get new value
	int a=0;
	context->GetVariableManager()->GetValue(iName, &GetPMan()->_LogToConsole);
	xLogger::GetInstance()->enableConsoleOutput(GetPMan()->_LogToConsole);


	return ;


	// Validate value

	if (0 == newValue.Length())
	{
		// This is valid, it means disable the GBL platform
	}
	else
	{

		int LogConsole = newValue.ToInt();
		if ( LogConsole ==1 )
		{
			// Restore back to previous value
	//		this->manualSetInProgress = TRUE;
	//		context->GetVariableManager()->SetValue(this->variableName.CStr(), this->currentValue.CStr());
	//		this->manualSetInProgress = FALSE;
		}
		else
		{
			// Remember current value
			//this->currentValue = newValue;

			// Write back in preferred format
			//newValue.Format("(0x%08x,0x%08x)", laid.guid.d1, laid.guid.d2);

			//this->manualSetInProgress = TRUE;
			//context->GetVariableManager()->SetValue(this->variableName.CStr(), newValue.CStr());
			//this->manualSetInProgress = FALSE;
		}
	}
}


void
PhysicManager::_registerWatchers(CKContext*context)
{
	const char* nameConsoleLogger = "Physic Console Logger/Console";
	watcherConsole = new PhysicVariableWatcher(context, nameConsoleLogger, "0");
	context->GetVariableManager()->RegisterWatcher(nameConsoleLogger, watcherConsole);

}