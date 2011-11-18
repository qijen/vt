#ifndef __vtDistributedObjectsInterface_h_
#define __vtDistributedObjectsInterface_h_

#include "xNetTypes.h"
class xDistributedObject;
class xNetInterface;


class IDistributedObjects
{
public:
	IDistributedObjects();
	IDistributedObjects(xNetInterface *netInterface);
	~IDistributedObjects();

	
	
	xDistributedObject *create(int objID,const char *templateClassName,const char* objectName="");

	xDistributedObject *getByUserID(int userID,int classType);
	xDistributedObject *get(int serverID);
	xDistributedObject *getByEntityID(int entityID);
	xDistributedObject *get(const char*name);
	xDistributedObject *get(const char*name,int classType);

	xNetInterface * getNetInterface(){return m_NetworkInterface;}
	void setNetInterface(xNetInterface * val){ m_NetworkInterface  = val; }

	void create(const char* objectName,const char*className);
	void cCreateDistObject(int objectID,int ownerID,const char* objectName,const char*className);
	void removeObject(xDistributedObject* object);
	void deleteObject(int serverID);
	void deleteObject(xDistributedObject*object);
	void Clean();
	virtual void Destroy();
	virtual void removeAll(xDistributedObjectsArrayType *distObjects);
	uxString print(TNL::BitSet32 flags);

	

protected:
//	TNL::SafePtr <xNetInterface> m_NetworkInterface;
	xNetInterface *m_NetworkInterface;
	
private:


};
#endif
