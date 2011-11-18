#ifndef __CSMANAGER_H_
#define __CSMANAGER_H_

#include "vtTools.h"
#include "CKBaseManager.h"
#include <stdlib.h>
#include <map>
#include <vector>

#define INIT_MAN_GUID		CKGUID(0x35824c8a,0x4e320ac4)


class csMessage 
{
public :

	csMessage(){}
	
	virtual ~csMessage()	{	}
	
	XString name;
	char*GetName(){ return  name.Str() ; }
	void SetName(const char* _name)	{		name  = _name;	}


	typedef XArray<CKParameter*> MessageParameterArrayType;
	MessageParameterArrayType m_Parameters;
	int GetNumParameters(){ return m_Parameters.Size() ; }
	void AddParameter(CKParameter *pin){  m_Parameters.PushBack(pin);  }

};

typedef std::vector<csMessage*>csMessagesArrayType;

class CSManager : public CKBaseManager
{

	public:
		//Ctor
		CSManager(CKContext* ctx);
		//Dtor
		~CSManager();

		// Initialization	
		virtual CKERROR OnCKInit();
		virtual CKERROR OnCKReset();
		virtual CKERROR PreProcess();
		


	//////////////////////////////////////////////////////////////////////////
	//
	int HasMessages() const { return m_HasMessages; }
	void SetHasMessages(int val) { m_HasMessages = val; }



	//////////////////////////////////////////////////////////////////////////
	csMessagesArrayType m_CSMessages;
	csMessagesArrayType& GetCSMessages() { return m_CSMessages; }
	void AddMessage(csMessage*msg);

	__inline int GetNumMessages();
    __inline char *GetMessageName(int messageID);
	__inline int GetNumParameters(int messageID);
	__inline int GetMessageParameterType(int messageID,int parameterSubID);

	__inline int GetMessageValueInt(int messageID,int parameterSubID);
	__inline float GetMessageValueFloat(int messageID,int parameterSubID);
	__inline char* GetMessageValueStr(int messageID,int parameterSubID);
	__inline void CleanMessages();
	__inline void DeleteMessage(int messageID);
        
	virtual CKDWORD	GetValidFunctionsMask()		{ return CKMANAGER_FUNC_OnCKInit|		CKMANAGER_FUNC_OnCKReset; 	}

	private:

	protected:
	int m_HasMessages;
	
	
};
//////////////////////////////////////////////////////////////////////////
char*
CSManager::GetMessageName(int messageID)
{
	XString result;
	if (GetNumMessages() && messageID < GetNumMessages() )
	{
		csMessage* msg = GetCSMessages().at(messageID);
		return msg->GetName();
	}
	return result.Str();
}
//////////////////////////////////////////////////////////////////////////
int CSManager::GetNumMessages()
{ 	

	//MessageBox(NULL,"","",1);
	return GetCSMessages().size(); 
}
//////////////////////////////////////////////////////////////////////////
int CSManager::GetNumParameters(int messageID)
{
	int result  = -1;
	if (messageID < GetNumMessages())
	{
		csMessage* msg = GetCSMessages().at(messageID);
		return msg->GetNumParameters();
	}
	return result;
}
//////////////////////////////////////////////////////////////////////////
int CSManager::GetMessageParameterType(int messageID,int parameterSubID)
{
	int result  = -1;
	if (GetNumParameters(messageID))
	{

		if (parameterSubID < GetNumParameters(messageID))
		{
			csMessage* msg = GetCSMessages().at(messageID);
			CKParameter *par = *msg->m_Parameters.At(parameterSubID);
			if (par)
			{
				CKParameterManager *pam = static_cast<CKParameterManager *>(m_Context->GetParameterManager());
				CKParameterType pType  = par->GetType();
				using namespace vtTools;
				using namespace vtTools::Enums;
				SuperType sType = ParameterTools::GetVirtoolsSuperType(m_Context,pam->ParameterTypeToGuid(pType));	
				switch (sType)
				{
				case vtSTRING:
					result=1;
					break;
				case vtFLOAT:
					result=2;
					break;
				case vtINTEGER:
					result=3;
					break;

				default :
					return -1;
				}
			}
		}
	}
	return result;
}
//////////////////////////////////////////////////////////////////////////
int CSManager::GetMessageValueInt(int messageID,int parameterSubID)
{
	assert(GetMessageParameterType(messageID,parameterSubID)==3 );

	int result  = -1;
	if (GetNumParameters(messageID))
	{
		if (parameterSubID < GetNumParameters(messageID))
		{
			csMessage* msg = GetCSMessages().at(messageID);
			CKParameter *par = *msg->m_Parameters.At(parameterSubID);
			if (par)
			{
				par->GetValue(&result);
			}
		}
	}
	return result;
}
//////////////////////////////////////////////////////////////////////////
float CSManager::GetMessageValueFloat(int messageID,int parameterSubID)
{
	assert(GetMessageParameterType(messageID,parameterSubID)==2 );

	float result  = -1.0f;
	if (GetNumParameters(messageID))
	{
		if (parameterSubID < GetNumParameters(messageID))
		{
			csMessage* msg = GetCSMessages().at(messageID);
			CKParameter *par = *msg->m_Parameters.At(parameterSubID);
			if (par)
			{
				par->GetValue(&result);
			}
		}
	}
	return result;
}
//////////////////////////////////////////////////////////////////////////
char*CSManager::GetMessageValueStr(int messageID,int parameterSubID)
{
	assert(GetMessageParameterType(messageID,parameterSubID)==1 );

	char* result  = NULL;
	if (GetNumParameters(messageID))
	{
		if (parameterSubID < GetNumParameters(messageID))
		{
			csMessage* msg = GetCSMessages().at(messageID);
			CKParameter *par = *msg->m_Parameters.At(parameterSubID);
			if (par)
			{
				VxScratch sbuffer(par->GetDataSize() +1 );
				CKSTRING buffer = (CKSTRING)sbuffer.Mem();
				par->GetStringValue(buffer);
				return buffer;
			}
		}
	}
	return result;
}
//////////////////////////////////////////////////////////////////////////
void CSManager::CleanMessages()
{
	for (int i = 0 ; i < GetNumMessages() ; i++ )
	{
		csMessage* msg = GetCSMessages().at(i);
		for (int j  = 0 ; j < GetNumParameters(i) ; j++ )
		{
			msg->m_Parameters[j]=NULL;
		}
		msg->m_Parameters.Clear();
	}
	GetCSMessages().erase(GetCSMessages().begin(),GetCSMessages().end());
}
//////////////////////////////////////////////////////////////////////////
void CSManager::DeleteMessage(int messageID)
{
	if (messageID < GetNumMessages() )
	{
		csMessage* msg = GetCSMessages().at(messageID);
		for (int j  = 0 ; j < GetNumParameters(messageID) ; j++ )
		{
			msg->m_Parameters[j]=NULL;
		}
		msg->m_Parameters.Clear();
		GetCSMessages().erase(GetCSMessages().begin() + messageID);
	}
}


#endif
