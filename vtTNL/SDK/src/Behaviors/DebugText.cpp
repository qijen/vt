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
#include "xLogger.h"

#include "vtLogTools.h"

CKObjectDeclaration	*FillBehaviorDebugTextDecl();
CKERROR CreateDebugTextProto(CKBehaviorPrototype **pproto);
int DebugText(const CKBehaviorContext& behcontext);
CKERROR DebugTextCallBackObject(const CKBehaviorContext& behcontext);

CKObjectDeclaration	*FillBehaviorDebugTextDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("DebugText");	
	od->SetDescription("Displays a text on console");
	
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x37852d34,0x27a84a90));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName(VTCX_AUTHOR);
	od->SetVersion(0x00020000);
	od->SetCreationFunction(CreateDebugTextProto);
	od->SetCompatibleClassId(CKCID_BEOBJECT);
	od->SetCategory("TNL/Misc");
	return od;
}


CKERROR CreateDebugTextProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("DebugText");
	if(!proto) return CKERR_OUTOFMEMORY;
	
	proto->DeclareInput("On");
	
	proto->DeclareOutput("Exit On");
	
	
	
	proto->DeclareInParameter("Component", CKPGUID_INT);
	proto->DeclareInParameter("Level", CKPGUID_INT);
	proto->DeclareInParameter("Text", CKPGUID_STRING);
	proto->DeclareOutParameter("Text", CKPGUID_STRING);

	
	
	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetFunction(DebugText);

	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)(CKBEHAVIOR_VARIABLEPARAMETERINPUTS));
	
	proto->SetBehaviorCallbackFct(DebugTextCallBackObject);
	
	*pproto = proto;
	return CK_OK;
	
}


int DebugText(const CKBehaviorContext& behcontext)
{
  CKBehavior *beh = behcontext.Behavior;

	
  int component=-1;
  beh->GetInputParameterValue(0,&component);
  int level=-1;
  beh->GetInputParameterValue(1,&level);

  
  CKParameterIn* pin;
	CKParameter* pout;

	XString buffer;
	
	// we construct the string
	int pinc = beh->GetInputParameterCount();
	for(int i=2;i<pinc;i++) {
		pin = beh->GetInputParameter(i);
		pout = pin->GetRealSource();
		if(pout) {
			int paramsize = pout->GetStringValue(NULL);
			if (paramsize) {
				XAP<char> paramstring(new char[paramsize]);
				pout->GetStringValue(paramstring,FALSE);

				buffer << (char*)paramstring;
				buffer << " ";
			} 
		}
	}
	
	using namespace vtTools::BehaviorTools;
	SetOutputParameterValue<CKSTRING>(beh,0,buffer.Str());

	xLogger::xLog(level,component,"Dbg %s",buffer.CStr());
	beh->ActivateOutput(0);
    


	return 0;
}


CKERROR 
DebugTextCallBackObject(const CKBehaviorContext& behcontext)
{

	return 0;

}

