#include "precomp.h"
#include "vtNetworkManager.h"
#include "VSLManagerSDK.h"

#include "xNetInterface.h"
#include "xNetworkFactory.h"
#include "tnlRandom.h"
#include "tnlSymmetricCipher.h"
#include "tnlAsymmetricKey.h"

#include "vtTools.h"


CKObjectDeclaration	*FillBehaviorSendArrayMessageDecl();
CKERROR CreateSendArrayMessageProto(CKBehaviorPrototype **);
int SendArrayMessage(const CKBehaviorContext& behcontext);
CKERROR SendArrayMessageCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 0
#define BEH_OUT_MAX_COUNT 1




CKObjectDeclaration	*FillBehaviorSendArrayMessageDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Network Send Array Message");	
	od->SetDescription("Sends the contents of an array as message to a Network.");
	
	od->SetCategory("TNL/Message");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x63db76e8,0x7b6549d3));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateSendArrayMessageProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


CKERROR CreateSendArrayMessageProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Send Network Message");
	if(!proto) return CKERR_OUTOFMEMORY;

	proto->DeclareInput("In");
	proto->DeclareOutput("Out");
	proto->DeclareOutput("Error");

	
	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	proto->DeclareInParameter("Message", CKPGUID_MESSAGE, "OnClick");
	proto->DeclareInParameter("Dest (User ID)", CKPGUID_INT, "0");

	proto->DeclareInParameter("Array", CKPGUID_DATAARRAY, "0");
	
	proto->DeclareInParameter("Starting Row", CKPGUID_INT, "0");
	proto->DeclareInParameter("Ending Row", CKPGUID_INT, "-1");

	proto->DeclareInParameter("Starting Column", CKPGUID_INT, "0");
	proto->DeclareInParameter("Ending Column", CKPGUID_INT, "-1");

	proto->DeclareSetting("Reliable", CKPGUID_BOOL, "true");
	proto->DeclareSetting("Single", CKPGUID_BOOL, "false");
	proto->DeclareSetting("Exclude Master", CKPGUID_BOOL, "false");





	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETERINPUTS  ));
	proto->SetFunction(SendArrayMessage);
	proto->SetBehaviorCallbackFct(SendArrayMessageCB);

	*pproto = proto;
	return CK_OK;

}


int msgCounter3 = 0;

int SendArrayMessage(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());

	using namespace vtTools::BehaviorTools;

	int connectionID = vtTools::BehaviorTools::GetInputParameterValue<int>(beh,0);
	
	int msgType = vtTools::BehaviorTools::GetInputParameterValue<int>(beh,1);
	XString msgTypeStr(mm->GetMessageTypeName(msgType));

	int dstID = vtTools::BehaviorTools::GetInputParameterValue<int>(beh,2);
	CKDataArray *array  = static_cast<CKDataArray*>(vtTools::BehaviorTools::GetInputParameterValue<CKObject*>(beh,3));
	
	int startRow = GetInputParameterValue<int>(beh,4);
	int endRow  = GetInputParameterValue<int>(beh,5);

	int startCol = GetInputParameterValue<int>(beh,6);
	int endCol  = GetInputParameterValue<int>(beh,7);

	//////////////////////////////////////////////////////////////////////////
	//sanity checks :
	if (!array)
	{
		GetNM()->m_Context->OutputToConsoleEx("You must specify an array");
		return FALSE;
	}

	if (endRow ==-1)	{		endRow = array->GetRowCount();	}
	if (endCol ==-1)	{		endCol = array->GetColumnCount();	}
	
	if (startRow < 0)	{		startRow = 0;	}
	//if (startRow > endRow )	{		startRow = 0;	}
	
	if (startCol <0  )	{		startCol  = 0;	}
	//if (startCol > endCol   )	{		startCol  = 0;	}

	xNetInterface *cin  = GetNM()->GetClientNetInterface();
	if (!cin)
	{
		return FALSE;
	}

	if(cin->GetConnection())
	{
		if (!cin->isValid())
		{
			return FALSE;
		}
	}

	TNL::Vector<TNL::Int<16> > dstIDs;
	dstIDs.push_back(dstID);

	CKGUID cGuid = array->GetColumnParameterGuid(0);
	CKParameterManager *pm = ctx()->GetParameterManager();
	int cType  = pm->ParameterGuidToType(cGuid);


	
	
	for (int j  = startCol ; j < endCol ; j++)
	{
		TNL::Vector<TNL::Int<16> >outVec;
		for (int i  = startRow ; i < endRow ; i++)
		{
			int val = 0;
			array->GetElementValue(i,0,&val);
			outVec.push_back(val);
		}
		cin->GetConnection()->c2sArrayMsgInt( cin->GetConnection()->GetUserID(),dstIDs,cType,startRow,endRow,startCol,endCol,msgTypeStr.CStr(),msgCounter3,j,outVec );
	}
	//////////////////////////////////////////////////////////////////////////
	/*

	//logprintf("sending broadcast message :","" );


	using namespace vtTools;
	using namespace vtTools::Enums;
	int bcount = beh->GetInputParameterCount();
	int srcId = cin->GetConnection()->GetUserID();

	for (int i = BEH_IN_INDEX_MIN_COUNT  ; i < beh->GetInputParameterCount() ; i++ )
	{

		CKParameterIn *ciIn = beh->GetInputParameter(i);
		CKParameterType pType  = ciIn->GetType();
		SuperType sType = ParameterTools::GetVirtoolsSuperType(ctx,pam->ParameterTypeToGuid(pType));	
		const char *pname  = pam->ParameterTypeToName(pType);

		

	}*/
	

	return CKBR_OK;
}

CKERROR SendArrayMessageCB(const CKBehaviorContext& behcontext)
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