#ifndef __IDistributedClasses_h_
#define __IDistributedClasses_h_

#include "xNetTypes.h"

class xNetInterface;


class IDistributedClasses
{

public:
	IDistributedClasses(xNetInterface *netInterface);
	virtual ~IDistributedClasses();

	
	xDistributedClassesArrayType* getDistrutedClassesPtr();
	xDistributedClass *createClass(const char* name,int templatetype);
	xDistributedClass *get(const char* name);
	xDistributedClass *get(const char* name,int entityType);
	xDistributedClass *getByIndex(int index);
	int getNumClasses();
	int destroyClass(xDistributedClass* _class);
	virtual void destroy();
	xNetInterface*getNetInterface();
	void setNetInterface(xNetInterface*netInterface);
	void deployClass(xDistributedClass*_class);
private:
	//TNL::SafePtr<xNetInterface>m_NetInterface;
	xNetInterface *m_NetInterface;
	xDistributedClassesArrayType *m_DistrutedClasses;
	
};

#endif
