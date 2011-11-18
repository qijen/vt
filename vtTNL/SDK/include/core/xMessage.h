#ifndef __X_MESSAGE_H_
#define __X_MESSAGE_H_

#include "xNetTypes.h"

class xMessage
{
public :



	xMessage()
	{

		messageID = -1;
		messageType = -1;
		mFlags = 0 ; 
		lifeTime = 0.0f;
		mSendCount =0;
		srcUser = -1;
		dstUser = -1;
		mNumUsers = 0;


	}

	float getLifeTime() const { return lifeTime; }
	void setLifeTime(float val) { lifeTime = val; }

	bool IsComplete() const { return complete; }
	void SetComplete(bool val) { complete = val; }

	int getDstUser() const { return dstUser; }
	void setDstUser(int val) { dstUser = val; }


	int getSrcUser() const { return srcUser; }
	void setSrcUser(int val) { srcUser = val; }
	
	int getClientSideSrcUser() const { return clientSideSrcUser; }
	void setClientSideSrcUser(int val) { clientSideSrcUser = val; }

	int getNumParameters()  { return mParameters.size(); }

	int getMessageID() const { return messageID; }
	void setMessageID(int val) { messageID = val; }
	int getMessageType() const { return messageType; }
	void setMessageType(int val) { messageType = val; }


	xNString getName() const { return name; }
	void setName(xNString val) { name = val; }

	TNL::BitSet32& getFlags() { return mFlags; }
	void setFlags(TNL::BitSet32 val) { mFlags = val; }

	int getSendCount() { return mSendCount; }
	void setSendCount(int val) { mSendCount = val; }

	~xMessage();

	xDistributedPropertyArrayType mParameters;
	xDistributedPropertyArrayType&  getParameters() { return mParameters; }

	xDistributedClient * getClientSource() { return mClientSource; }
	void setClientSource(xDistributedClient * val) { mClientSource = val; }
	bool getIgnoreSessionMaster() const { return ignoreSessionMaster; }
	void setIgnoreSessionMaster(bool val) { ignoreSessionMaster = val; }
	
	int& getNumUsers()  { return mNumUsers; }
	void setNumUsers(int val) { mNumUsers = val; }

protected : 
	float lifeTime;
	bool complete;
	int dstUser;
	int srcUser;
	int clientSideSrcUser;
	
	
	int messageID;
	int messageType;
	xNString name;
	TNL::BitSet32 mFlags;
	int mSendCount;
	int mNumUsers;
	
	bool ignoreSessionMaster;
	
	xDistributedClient *mClientSource;
	


};

#endif