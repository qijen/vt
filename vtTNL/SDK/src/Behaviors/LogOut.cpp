#include "CKAll.h"
#include "vtNetworkManager.h"
#include <vtTools.h>
#include "xDistTools.h"
#include "xDistTypesAll.h"
#include "xMathTools.h"
#include "xDistributed3DObject.h"
#include "xDistributedProperty.h"
#include "xDistributedPropertyInfo.h"

#include "xNetInterface.h"
#include "vtConnection.h"

#include "xDistributedBaseClass.h"
#include "xDistributed3DObjectClass.h"
#include "IDistributedClasses.h"
#include "xDOStructs.h"

#include "IDistributedObjectsInterface.h"

#include "xDistributedPoint4F.h"
#include "xDistributedPoint3F.h"
#include "xDistributedPoint2F.h"
#include "xDistributedPoint1F.h"
#include "xDistributedString.h"
#include "xDistributedInteger.h"


CKObjectDeclaration	*FillBehaviorLogEntryDecl();
CKERROR CreateLogEntryProto(CKBehaviorPrototype **);
int LogEntry(const CKBehaviorContext& behcontext);
CKERROR LogEntryCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 0
#define BEH_OUT_MAX_COUNT 1



CKObjectDeclaration	*FillBehaviorLogEntryDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Log Event");	
	od->SetDescription("Displays Internal Log Entries");
	
	od->SetCategory("TNL");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x512542dc,0x1b836fd9));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateLogEntryProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


CKERROR CreateLogEntryProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Log Event");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");

	proto->DeclareOutParameter("Entry",CKPGUID_STRING);

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETERINPUTS  ));
	proto->SetFunction(LogEntry);
	proto->SetBehaviorCallbackFct(LogEntryCB);


	*pproto = proto;


	return CK_OK;

}


int LogEntry(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());


	XString File((CKSTRING) beh->GetInputParameterReadDataPtr(0));
	
	if (GetNM()->GetLastLogEntry().Length())
	{
		CKParameterOut *pout  = beh->GetOutputParameter(0);
		pout->SetStringValue(GetNM()->GetLastLogEntry().Str());
		GetNM()->SetLastLogEntry("");
		beh->ActivateInput(0,FALSE);
		beh->ActivateOutput(0);
	}

	

	return CKBR_ACTIVATENEXTFRAME;
}

CKERROR LogEntryCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKBeObject *beo = (CKBeObject *)beh->GetTarget();
	if (!beo) {
		return CKBR_OWNERERROR;
	}
  
	if (behcontext.CallbackMessage==CKM_BEHAVIOREDITED) 
	{
	/*	CKParameterIn* pin = beh->GetInputParameter(0);
		if (!pin) {
			CKBehavior* pbeh = beh;
			XString name;
			while ((pbeh=pbeh->GetParent())) {
				name << pbeh->GetName() << "->";
			}
			name << beh->GetName();

			XString str;
			str << '[' << name << "] : failed to retrieve first input parameter";
			behcontext.Context->OutputToConsole(str.Str(),TRUE);
			return CKBR_PARAMETERERROR;
		}
		
		if (pin->GetGUID()!=CKPGUID_MESSAGE) {
			pin->SetGUID(CKPGUID_MESSAGE,FALSE);

			CKBehavior* pbeh = beh;
			XString name;
			while ((pbeh=pbeh->GetParent())) {
				name << pbeh->GetName() << "->";
			}
			name << beh->GetName();

			XString str;
			str << '[' << name << "] : first input parameter type must be \"Message\"!";
			behcontext.Context->OutputToConsole(str.Str(),TRUE);
		}

		pin = beh->GetInputParameter(1);
		if (!pin) {
			CKBehavior* pbeh = beh;
			XString name;
			while ((pbeh=pbeh->GetParent())) {
				name << pbeh->GetName() << "->";
			}
			name << beh->GetName();

			XString str;
			str << '[' << name << "] : failed to retrieve second input parameter";
			behcontext.Context->OutputToConsole(str.Str(),TRUE);
			return CKBR_PARAMETERERROR;
		}
	
		if (!behcontext.ParameterManager->IsDerivedFrom(pin->GetGUID(),CKPGUID_OBJECT)) {
			pin->SetGUID(CKPGUID_BEOBJECT,FALSE);

			CKBehavior* pbeh = beh;
			XString name;
			while ((pbeh=pbeh->GetParent())) {
				name << pbeh->GetName() << "->";
			}
			name << beh->GetName();

			XString str;
			str << '[' << name << "] : second input parameter type must derived from \"BeObject\"!";
			behcontext.Context->OutputToConsole(str.Str(),TRUE);
		}*/
	}

	return CKBR_OK;
}