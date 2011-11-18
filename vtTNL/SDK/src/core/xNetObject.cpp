#include "xNetObject.h"
#include <xNetInterface.h>
#include "vtConnection.h"


/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
xNetObject::xNetObject()
{

	mNetFlags.set(Ghostable);
	setNetInterface(NULL);
	mOwnerConnection  = NULL;

}
void xNetObject::Destroy()
{

	setNetInterface(NULL);
	SetName("");

	
}
xNetObject::~xNetObject()
{

	Destroy();

	/*
	xDistributedObject *distObject = static_cast<xDistributedObject *>(this);
	if(distObject)
	{
		if (GetNetInterface())
		{
			vtDistributedObjectsArrayType *distObjects =  GetNetInterface()->getDistributedObjects();
			if (distObjects)
			{
				distObjects->Remove((xDistributedObject*)this);
				//retrieve its class : 
				
				distObject->SetNetInterface(NULL);
				distObject->SetName("");
			}
		}
	}*/
}

/*
*******************************************************************
* Function: 
*
* Description: 
*    
* Parameters: 
*
* Returns: 
*
*******************************************************************
*/
xNetObject::xNetObject(TNL::StringPtr name) : m_Name(name)
{
	mNetFlags.set(Ghostable);
	m_NetInterface = NULL;
	m_UserID = 0;
	mOwnerConnection = NULL;
}