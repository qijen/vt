#ifndef __IMessages_h
#define __IMessages_h

#include "xNetTypes.h"

class Point3F;
class Point4F;
class xNetInterface;
class xNetworkMessage;


class IMessages
{
	public:

		IMessages(xNetInterface *ninterface);

		

		xMessageArrayType *getMessages();

		xNetInterface * getNetInterface() const { return mNetInterface; }
		void setNetInterface(xNetInterface * val) { mNetInterface = val; }

		void addMessage(xMessage*msg);
		xMessage*getNextOutMessage();
		xMessage*getNextInMessage();
		xMessage*getNextInMessage(xNString mName);
		void removeMessage(xMessage*msg);
		void deleteMessage(xMessage*msg);
		void deleteAllOldMessages();
		int getNumMessagesOfType(int type);
		int getNumOutMessages();




		xMessage*readFromStream(TNL::BitStream *bstream,TNL::BitSet32 flags);
		void writeToStream(xMessage *msg,TNL::BitStream *bstream,TNL::BitSet32 flags);
		
		void advanceTime(float deltaTime);
		float getMessageTimeout() const { return mMessageTimeout; }
		void setMessageTimeout(float val) { mMessageTimeout = val; }

		void checkMessages();

		xMessageType *getMessageType(xMessage*message);
		xMessageType *createMessageType(xMessage*message);
		void registerMessageType(xNString name,int type,int numParameters);
		
		float getThresoldTicker() const { return mThresoldTicker; }
		void setThresoldTicker(float val) { mThresoldTicker = val; }
		float getMinSendTime() const { return mMinSendTime; }
		void setMinSendTime(float val) { mMinSendTime = val; }

	protected:
	private:

		xNetInterface *mNetInterface;
		float mMessageTimeout;
		float mThresoldTicker;
		float mMinSendTime;
		
};

#endif
