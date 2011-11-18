#ifndef _ISESSION_H_
#define _ISESSION_H_

#include "xNetTypes.h"

class xNetInterface;
class xDistributedSession;
class xDistributedClient;


class ISession
{
	public:
		ISession(xNetInterface *nInterface) :mNetInterface(nInterface) {}
		xNetInterface * getNetInterface() const { return mNetInterface; }

		int create(xNString name,int maxUsers,xNString password,int type);
		xDistributedSession *get(xNString name);
		xDistributedSession *getByUserID(int userID);
		xDistributedSession *get(int sessionID);
		void deleteSession(int sessionID);
		int getNumSessions();
		int joinClient(xDistributedClient*client, int sessionID,xNString password);
		int removeClient(xDistributedClient*client,int sessionID,bool destroy=false);
		void removeClient(int clientID);
		void lockSession(int userID,int sessionID);
		void unlockSession(int userID,int sessionID);
		uxString print(TNL::BitSet32 flags);

	protected:
	private:
		xNetInterface *mNetInterface;
		

};
#endif