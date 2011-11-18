#include "precomp.h"
#include "vtNetworkManager.h"
#include "VSLManagerSDK.h"

#include "xNetInterface.h"
#include "xNetworkFactory.h"
#include "tnlRandom.h"
#include "tnlSymmetricCipher.h"
#include "tnlAsymmetricKey.h"

#include "vtTools.h"
#include "xNetworkTypes.h"


CKObjectDeclaration	*FillBehaviorArrayNetMessageWaiterDecl();
CKERROR CreateArrayNetMessageWaiterProto(CKBehaviorPrototype **);
int ArrayNetMessageWaiter(const CKBehaviorContext& behcontext);
CKERROR ArrayNetMessageWaiterCB(const CKBehaviorContext& behcontext);



#define BEH_IN_INDEX_MIN_COUNT 2
#define BEH_OUT_MIN_COUNT 3
#define BEH_OUT_MAX_COUNT 1


//int msgCounter = 0;

CKObjectDeclaration	*FillBehaviorArrayNetMessageWaiterDecl()
{
	CKObjectDeclaration *od = CreateCKObjectDeclaration("Network Wait Array Message");	
	od->SetDescription("Waits for a Message from the Network.");
	
	od->SetCategory("TNL/Message");
	od->SetType( CKDLL_BEHAVIORPROTOTYPE);
	od->SetGuid(CKGUID(0x3bf873ad,0x5a2e47bd));
	od->SetAuthorGuid(VIRTOOLS_GUID);
	od->SetAuthorName("Virtools");
	od->SetVersion(0x00010000);
	od->SetCreationFunction(CreateArrayNetMessageWaiterProto);
	od->SetCompatibleClassId(CKCID_OBJECT);
	return od;
}


CKERROR CreateArrayNetMessageWaiterProto(CKBehaviorPrototype **pproto)
{
	CKBehaviorPrototype *proto = CreateCKBehaviorPrototype("Network Wait Array Message");
	if(!proto) return CKERR_OUTOFMEMORY;

	

	proto->DeclareInput("In");
	proto->DeclareInput("Stop");
	proto->DeclareInput("Next");

	proto->DeclareOutput("Out");
	proto->DeclareOutput("Message");
	proto->DeclareOutput("Error");

	

	proto->DeclareInParameter("Connection ID", CKPGUID_INT, "0");
	proto->DeclareInParameter("Message", CKPGUID_MESSAGE, "OnClick");
	proto->DeclareInParameter("Array", CKPGUID_DATAARRAY, "netArray");
	proto->DeclareInParameter("First Column", CKPGUID_INT, "0");
	proto->DeclareInParameter("Append", CKPGUID_BOOL, "false");



	proto->DeclareOutParameter("Sender ID", CKPGUID_INT, "0");
	proto->DeclareOutParameter("Private", CKPGUID_BOOL, "false");
	proto->DeclareOutParameter("Error", CKPGUID_INT, "0");


	

	proto->SetFlags(CK_BEHAVIORPROTOTYPE_NORMAL);
	proto->SetBehaviorFlags((CK_BEHAVIOR_FLAGS)( CKBEHAVIOR_VARIABLEPARAMETEROUTPUTS  ));
	proto->SetFunction(ArrayNetMessageWaiter);
	proto->SetBehaviorCallbackFct(ArrayNetMessageWaiterCB);

	*pproto = proto;
	return CK_OK;

}


int ArrayNetMessageWaiter(const CKBehaviorContext& behcontext)
{
	CKBehavior* beh = behcontext.Behavior;
	CKMessageManager* mm = behcontext.MessageManager;
	CKContext* ctx = behcontext.Context;
	CKParameterManager *pam = static_cast<CKParameterManager *>(ctx->GetParameterManager());

	
	beh->ActivateInput(0,FALSE);



	int connectionID = vtTools::BehaviorTools::GetInputParameterValue<int>(beh,0);
	
	int Msg=vtTools::BehaviorTools::GetInputParameterValue<int>(beh,1);
	XString msgName(mm->GetMessageTypeName(Msg));
	
	CKDataArray *array  = static_cast<CKDataArray*>(vtTools::BehaviorTools::GetInputParameterValue<CKObject*>(beh,2));

	int firstC = vtTools::BehaviorTools::GetInputParameterValue<int>(beh,3);

	bool append = vtTools::BehaviorTools::GetInputParameterValue<int>(beh,4);

	//////////////////////////////////////////////////////////////////////////
		

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

	//////////////////////////////////////////////////////////////////////////

	using namespace vtTools;
	using namespace vtTools::Enums;

	XArrayNetworkMessageTypeIterator begin  =  GetNM()->GetArrayNetworkMessages().Begin();
	XArrayNetworkMessageTypeIterator end  =  GetNM()->GetArrayNetworkMessages().End();

	while (begin != end)
	{
		vtArrayNetworkMessage*msg = *begin;
		if (msg)
		{	
			
			if ( msg->complete )
			{
				vtArrayNetworkMessage::ArrayTypeIterator parBegin  = msg->columns.begin();
				vtArrayNetworkMessage::ArrayTypeIterator parEnd  = msg->columns.end();
				
				int subIndex=0;

				int startR  = append   ? array->GetRowCount() : 0;
				while(parBegin!=parEnd)
				{
					vtArrayNetworkMessage::ColumnType column = parBegin->second;
					for (int i  = 0 ; i < column.size() ; i++ )
					{
						CKParameter* para = column[i];
						int value = 0 ;
						para->GetValue(&value);
						logprintf("msgarray value %d",value);
						
						if (append)
						{
							array->AddRow();
						}
						array->SetElementValueFromParameter(startR,firstC,para);

						startR++;
						subIndex++;
						CK_ID id  =  para->GetID();
						ctx()->DestroyObject(id);
					}
					parBegin++;
				}
				GetNM()->GetArrayNetworkMessages().Remove(begin);
				logprintf("removing array by bb message !");
				begin  = GetNM()->GetArrayNetworkMessages().Begin();
			}
		}
		begin++;
	}
	/*int startC = 0 ; 
	if (firstC < array->GetColumnCount())
	{
	}*/

	/*if (strcmp(msg->name.Str(),msgName.Str()))
	{
	continue;
	}*/

	/*vtArrayNetworkMessage::ArrayTypeIterator parBegin  = msg->columns.begin();
	vtArrayNetworkMessage::ArrayTypeIterator parEnd  = msg->columns.end();

	int subIndex=0;
	while(parBegin!=parEnd)
	{
	vtArrayNetworkMessage::ColumnType column = parBegin->second;

	for (int i  = 0 ; i < column.size() ; i++ )
	{
	CKParameter *pin = column[i];
	CK_ID id  =  pin->GetID();
	ctx()->DestroyObject(id);
	}
	parBegin++;
	}
	GetArrayNetworkMessages().Remove(begin);
	logprintf("removing array message !");
	begin  = GetArrayNetworkMessages().Begin();*/


	/*
	XNetworkMessagesTypeIterator begin  =  GetNM()->GetXNetworkMessages().Begin();
	XNetworkMessagesTypeIterator end  =  GetNM()->GetXNetworkMessages().End();
	
	if (!GetNM()->GetXNetworkMessages().Size())
	{
		return CKBR_ACTIVATENEXTFRAME;
	}

	while (begin != end)
	{
		vtNetworkMessage*msg = *begin;
		if (msg)
		{
			if (strcmp(msg->name.Str(),msgType.Str()))
			{
				continue;
			}
			if ( msg->complete )
			{
				
				//logprintf("broadcast message recieved:%a",msg->name.Str());
				vtNetworkMessage::MessageParameterArrayTypeIterator parBegin  = msg->m_RecievedParameters.begin();
				vtNetworkMessage::MessageParameterArrayTypeIterator parEnd  = msg->m_RecievedParameters.end();

				int subIndex=0;
				while(parBegin!=parEnd)
				{
					CKParameter *pin = parBegin->second;
					CKParameterType pType  = pin->GetType();
					SuperType sType = ParameterTools::GetVirtoolsSuperType(ctx,pam->ParameterTypeToGuid(pType));	
					const char *pname  = pam->ParameterTypeToName(pType);

					switch (sType)
					{
					case vtSTRING:
						{
							if ( (beh->GetOutputParameterCount() - BEH_OUT_MIN_COUNT ) + subIndex > 0 )
							{
								CKParameterOut *pout = beh->GetOutputParameter(BEH_OUT_MIN_COUNT + subIndex );
								if (pout)
								{
									if (pout->GetType() == pin->GetType() )
									{
										pout->CopyValue(pin);
									}
								}
								CK_ID id  =  pin->GetID();
								ctx->DestroyObject(id);
							}
							break;
						}
					case vtFLOAT:
						{
							if ( (beh->GetOutputParameterCount() - BEH_OUT_MIN_COUNT ) + subIndex > 0 )
							{
								CKParameterOut *pout = beh->GetOutputParameter(BEH_OUT_MIN_COUNT + subIndex );
								if (pout)
								{
									if (pout->GetType() == pin->GetType() )
									{
										pout->CopyValue(pin);
									}
								}
								CK_ID id  =  pin->GetID();
								ctx->DestroyObject(id);
							}
							break;
						}
					case vtINTEGER:
						{
							if ( (beh->GetOutputParameterCount() - BEH_OUT_MIN_COUNT ) + subIndex > 0 )
							{
								CKParameterOut *pout = beh->GetOutputParameter(BEH_OUT_MIN_COUNT + subIndex );
								if (pout)
								{
									if (pout->GetType() == pin->GetType() )
									{
										pout->CopyValue(pin);
									}
								}
								CK_ID id  =  pin->GetID();
								ctx->DestroyObject(id);
							}
							break;
						}

					default :
						XString err("wrong input parameter type: ");
					}

					subIndex++;
					parBegin++;
				}
				beh->ActivateOutput(1);
				beh->SetOutputParameterValue(0,&msg->srcUserID);
				GetNM()->GetXNetworkMessages().Remove(begin);
				begin  = GetNM()->GetXNetworkMessages().Begin();
			}
		}
		begin++;
	}*/
	return CKBR_ACTIVATENEXTFRAME;
}

CKERROR ArrayNetMessageWaiterCB(const CKBehaviorContext& behcontext)
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