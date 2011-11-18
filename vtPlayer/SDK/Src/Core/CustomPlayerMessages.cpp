/******************************************************************************
File : CustomPlayer.cpp

Description: This file contains the CCustomPlayer class which
is the "interface" with the Virtools SDK. All other files are Windows/MFC
specific code.

Virtools SDK
Copyright (c) Virtools 2005, All Rights Reserved.
******************************************************************************/

#include "CPStdAfx.h"
#include "CustomPlayer.h"
#include "CustomPlayerDefines.h"

CKMessageType GetMessageByString(const char *msg)
{
	CKMessageManager *mm  = GetPlayer().m_MessageManager;

	for (int i  = 0 ; i < mm->GetMessageTypeCount() ; i++)
	{
		if (!strcmp(mm->GetMessageTypeName(i),msg) )
		{
			return i;
		}
	}
	return -1;
}
//************************************
// Method:    SendMessage
// FullName:  CCustomPlayer::SendMessage
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: char *targetObject
// Parameter: char *message
// Parameter: int id0
// Parameter: int id1
// Parameter: int id2
// Parameter: int value
//************************************
int CCustomPlayer::SendMessage(char *targetObject,char *message,int id0,int id1,int id2,int value)
{

	CKContext *ctx  = m_CKContext;
	CKLevel *level = ctx->GetCurrentLevel();
	CKMessageManager *mm = GetPlayer().m_MessageManager;

	CKBeObject *obj = static_cast<CKBeObject*>(ctx->GetObjectByName(targetObject));
	CKMessageType mType  = GetMessageByString(message);


	if (level && mType)
	{
		if (mType)
		{
			CKMessage *msg  = NULL;
			
				
			//no target object specified, we send a broadcast message :
			if (!obj)
			{
				msg = mm->SendMessageBroadcast(mType,CKCID_BEOBJECT,level);
				if(!msg)
					return -1;
			}//target object ok, we send  a single message  : 
			else
			{
				msg = mm->SendMessageSingle(mType,obj,level);
				if(!msg)
					return -1;
			}

			//////////////////////////////////////////////////////////////////////////
			// we attach our arguments as parameters :  

			//id0
			CKParameter *parameter0  = ctx->CreateCKParameterLocal("msg0",CKPGUID_INT,FALSE);
			parameter0->SetValue(&id0);msg->AddParameter(parameter0,true);
			//id1
			CKParameter *parameter1  = ctx->CreateCKParameterLocal("msg1",CKPGUID_INT,FALSE);
			parameter1->SetValue(&id1);	msg->AddParameter(parameter1,true);
			//id2
			CKParameter *parameter2  = ctx->CreateCKParameterLocal("msg2",CKPGUID_INT,FALSE);
			parameter2->SetValue(&id2);msg->AddParameter(parameter2,true);
			//the value : 
			CKParameter *valuex= ctx->CreateCKParameterLocal("msgValue",CKPGUID_INT,FALSE);
			valuex->SetValue(&value);msg->AddParameter(valuex,true);
            return 1;

		}
	}
	return -1;
}

//************************************
// Method:    SendMessage
// FullName:  CCustomPlayer::SendMessage
// Access:    public 
// Returns:   int
// Qualifier:
// Parameter: char *targetObject
// Parameter: char *message
// Parameter: int id0
// Parameter: int id1
// Parameter: int id2
// Parameter: int value
//************************************
int CCustomPlayer::SendMessage(char *targetObject,char *message,int id0,int id1,int id2,float value)
{

	CKContext *ctx  = m_CKContext;
	CKLevel *level = ctx->GetCurrentLevel();
	CKMessageManager *mm = GetPlayer().m_MessageManager;

	CKBeObject *obj = static_cast<CKBeObject*>(ctx->GetObjectByName(targetObject));
	CKMessageType mType  = GetMessageByString(message);


	if (level && mType)
	{
		if (mType)
		{
			CKMessage *msg  = NULL;


			//no target object specified, we send a broadcast message :
			if (!obj)
			{
				msg = mm->SendMessageBroadcast(mType,CKCID_BEOBJECT,level);
				if(!msg)
					return -1;
			}//target object ok, we send  a single message  : 
			else
			{
				msg = mm->SendMessageSingle(mType,obj,level);
				if(!msg)
					return -1;
			}

			//////////////////////////////////////////////////////////////////////////
			// we attach our arguments as parameters :  

			//id0
			CKParameter *parameter0  = ctx->CreateCKParameterLocal("msg0",CKPGUID_INT,FALSE);
			parameter0->SetValue(&id0);msg->AddParameter(parameter0,true);
			//id1
			CKParameter *parameter1  = ctx->CreateCKParameterLocal("msg1",CKPGUID_INT,FALSE);
			parameter1->SetValue(&id1);	msg->AddParameter(parameter1,true);
			//id2
			CKParameter *parameter2  = ctx->CreateCKParameterLocal("msg2",CKPGUID_INT,FALSE);
			parameter2->SetValue(&id2);msg->AddParameter(parameter2,true);
			//the value : 
			CKParameter *valuex= ctx->CreateCKParameterLocal("msgValue",CKPGUID_FLOAT,FALSE);
			valuex->SetValue(&value);msg->AddParameter(valuex,true);
			return 1;
		}
	}
	return -1;
}

int CCustomPlayer::SendMessage(char *targetObject,char *message,int id0,int id1,int id2,char* value)
{

	CKContext *ctx  = m_CKContext;
	CKLevel *level = ctx->GetCurrentLevel();
	CKMessageManager *mm = GetPlayer().m_MessageManager;

	CKBeObject *obj = static_cast<CKBeObject*>(ctx->GetObjectByName(targetObject));
	CKMessageType mType  = GetMessageByString(message);


	if (level && mType)
	{
		if (mType)
		{
			CKMessage *msg  = NULL;


			
			//no target object specified, we send a broadcast message :
			if (!obj)
			{
				msg = mm->SendMessageBroadcast(mType,CKCID_BEOBJECT,level);
				if(!msg)
					return -1;
			}//target object ok, we send  a single message  : 
			else
			{
				msg = mm->SendMessageSingle(mType,obj,level);
				if(!msg)
					return -1;
			}

			//////////////////////////////////////////////////////////////////////////
			// we attach our arguments as parameters :  

			//id0
			CKParameter *parameter0  = ctx->CreateCKParameterLocal("msg0",CKPGUID_INT,FALSE);
			parameter0->SetValue(&id0);msg->AddParameter(parameter0,true);
			//id1
			CKParameter *parameter1  = ctx->CreateCKParameterLocal("msg1",CKPGUID_INT,FALSE);
			parameter1->SetValue(&id1);	msg->AddParameter(parameter1,true);
			//id2
			CKParameter *parameter2  = ctx->CreateCKParameterLocal("msg2",CKPGUID_INT,FALSE);
			parameter2->SetValue(&id2);msg->AddParameter(parameter2,true);
			//the value : 
			CKParameter *valuex= ctx->CreateCKParameterLocal("msgValue",CKPGUID_STRING,FALSE);
			valuex->SetStringValue(value);msg->AddParameter(valuex,true);
			return 1;

		}
	}
	return -1;
}


