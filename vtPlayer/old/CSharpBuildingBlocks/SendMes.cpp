#include "precomp.h"
#include "Manager/Manager.h"

#include "virtools/vt_tools.h"


CKObjectDeclaration	*FillBehaviorSendCSMessageDecl();
CKERROR CreateSendCSMessageProto(CKBehaviorPrototype **);
int SendMessage(const CKBehaviorContext& behcontext);
CKERROR SendMessageCB(const CKBehaviorContext& behcontext);


#define BEH_IN_INDEX_MIN_COUNT 1
#define BEH_OUT_MIN_COUNT 0
#define BEH_OUT_MAX_COUNT 1


int msgCounter = 0;

CKObjectDeclaration	*FillBehaviorSendCSMessageDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Send CSMessage");	
	od->SetDescription("Stores a message in the CSManager ");
	
	od->SetCategory("CSharp/Message");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x2f0b513c,0xa4b1eb9));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateSendCSMessageProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


CKERROR CreateSendCSMessageProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Send CSMessage");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	
	proto->DeclareInParameter("Message", CKPGUID_STRING, "msg");
	//proto->DeclareInParameter("Target User ID", CKPGUID_INT, "0");

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETERINPUTS  ));
	proto->SetFunction(SendMessage);
	proto->SetBehaviorCallbackFct(SendMessageCB);

	*pproto = proto;
	return CK_OK;

}


int SendMessage(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());
	CSManager* cman =(CSManager*)ctx->GetManagerByGuid(INIT_MAN_GUID);

	
	XString msg((CKSTRING) beh->GetInputParameterReadDataPtr(0));
	
	csMessage *msgout  = new csMessage();
	msgout->SetName(msg.CStr());
	beh->ActivateInput(0,FALSE);

	using namespace vtTools;
	using namespace vtTools::Enums;
	int bcount = beh->GetInputParameterCount();


	for (int i = BEH_IN_INDEX_MIN_COUNT  ; i < beh->GetInputParameterCount() ; i++ )
	{

		CKParameterIn *ciIn = beh->GetInputParameter(i);
		CKParameterType pType  = ciIn->GetType();
		SuperType sType = ParameterTools::GetVirtoolsSuperType(ctx,pam->ParameterTypeToGuid(pType));	
		const char *pname  = pam->ParameterTypeToName(pType);

		switch (sType)
		{
			case vtSTRING:
			case vtFLOAT:
			case vtINTEGER:
			{
				msgout->AddParameter(ciIn->GetRealSource());
				break;
			}
			
			default :
				XString err("wrong input parameter type: ");
				err << ciIn->GetName() << "Only Types derivated from Interger,Float or String are acceptable";
				ctx->OutputToConsole(err.Str(),FALSE );
				return CKBR_BEHAVIORERROR;
			}
	}
	cman->AddMessage(msgout);
	beh->ActivateOutput(0);
	return CKBR_OK;
}

CKERROR SendMessageCB(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKBeObject *beo = (CKBeObject *)beh->GetTarget();
	if (!beo) {
		return CKBR_OWNERERROR;
	}
  
	if (behcontext.CallbackMessage==CKM_BEHAVIOREDITED) 
	{
	
	}

	return CKBR_OK;
}