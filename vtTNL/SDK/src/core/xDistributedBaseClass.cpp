#include "xDistributedBaseClass.h"
#include "xDistributed3DObjectClass.h"
#include "xDistributedPropertyInfo.h"
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
xDistributedPropertyInfo* xDistributedClass::exists(int nativeType)
{
	xDistributedPropertyInfo *result = NULL;
	if (getDistributedProperties() ==NULL ) return NULL;
	int size = getDistributedProperties()->size();
	for (unsigned int i  = 0  ; i <  getDistributedProperties()->size() ; i ++ )
	{
		xDistributedPropertyInfo * current  =  m_DistributedProperties->at(i);
		int nType = current->mNativeType ;
		if ( current->mNativeType == nativeType)
		{
			return current;
		}
	}
	return result;
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
xDistributedPropertyInfo*	xDistributedClass::exists(const char*name)
{
	xDistributedPropertyInfo *result = NULL;

	if (!strlen(name))return result;
	
	for (unsigned int i  = 0  ; i <  getDistributedProperties()->size() ; i ++ )
	{
		xDistributedPropertyInfo * current  =  m_DistributedProperties->at(i);
		if (!strcmp(current->mName.getString(),name))
		{
			return current;
		}
	}
	return result;
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
xDistributed3DObjectClass*xDistributedClass::cast(xDistributedClass *_in){	return static_cast<xDistributed3DObjectClass*>(_in);	}


xNString 
xDistributedClass::getClassName()
{

	return m_ClassName;
}
xDistributedClass::~xDistributedClass()
{

	m_DistributedProperties->clear();
	//delete m_DistributedProperties;
}
//////////////////////////////////////////////////////////////////////////
void xDistributedClass::setClassName(xNString name)
{
	m_ClassName  = name;
}

xDistributedClass::xDistributedClass()
{

	m_DistributedProperties  =  new xDistributedPropertiesListType();
	m_EnitityType = 0;
	m_NativeFlags = 0;

}

