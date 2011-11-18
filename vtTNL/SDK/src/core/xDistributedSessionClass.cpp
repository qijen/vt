#include "xDistributedSessionClass.h"
#include "xDistributedPropertyInfo.h"
#include "xDistTools.h"

xDistributedSessionClass::xDistributedSessionClass() : xDistributedClass()
{

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
 int xDistributedSessionClass::getFirstUserField(){ return 6; }
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
 int
xDistributedSessionClass::getUserFieldBitValue(int walkIndex)
{
	int userTypeCounter = 0;
	xDistributedPropertiesListType &props  = *getDistributedProperties();
	for (unsigned int i  = 0 ; i < props.size() ; i ++ )
	{
		xDistributedPropertyInfo *dInfo  = props[i];
		if (dInfo->mNativeType==E_DC_S_NP_USER )
		{
			if (i ==walkIndex)
			{
				break;
			}
			userTypeCounter++;
		}
	}
	int result = getFirstUserField();
    result +=userTypeCounter;
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
int
xDistributedSessionClass::getInternalUserFieldIndex(int inputIndex)
{
	int userTypeCounter = 0;
	xDistributedPropertiesListType &props  = *getDistributedProperties();
	for (unsigned int i  = 0 ; i < props.size() ; i ++ )
	{
		xDistributedPropertyInfo *dInfo  = props[i];
		if (dInfo->mNativeType==E_DC_S_NP_USER)
		{
			if (userTypeCounter == inputIndex)
			{
				return i;
			}
			userTypeCounter++;
		}
	}
	return -1;
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
int xDistributedSessionClass::getUserFieldCount()
{
	int userTypeCounter = 0;
	xDistributedPropertiesListType &props  = *getDistributedProperties();
	for (unsigned int i  = 0 ; i < props.size() ; i ++ )
	{
		if (props[i]->mNativeType==E_DC_S_NP_USER)
			userTypeCounter++;
	}
	return userTypeCounter;
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
void
xDistributedSessionClass::addProperty(int nativeType,int predictionType)
{
	xDistributedPropertyInfo *result  = exists(nativeType);
	if (!result)
	{
		TNL::StringPtr name  = NativeTypeToString(nativeType);
		int valueType = NativeTypeToValueType(nativeType);
		result  = new xDistributedPropertyInfo( name ,valueType , nativeType ,predictionType );
		getDistributedProperties()->push_back( result );
	}
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
void
xDistributedSessionClass::addProperty(const char*name,int type,int predictionType)
{
	xDistributedPropertyInfo *result  = exists(name);
	if (!result)
	{
		result  = new xDistributedPropertyInfo( name ,type,E_DC_S_NP_USER ,predictionType );
		getDistributedProperties()->push_back( result );
	}
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
xNString xDistributedSessionClass::NativeTypeToString(int nativeType)
{

	switch(nativeType)
	{
	case E_DC_S_NP_MAX_USERS:
		return TNL::StringPtr("Max Users");
		break;
	case E_DC_S_NP_PASSWORD:
		return TNL::StringPtr("Password");
		break;
	case E_DC_S_NP_LOCKED:
		return TNL::StringPtr("Is Locked");
	case E_DC_S_NP_NUM_USERS:
		return TNL::StringPtr("Num Users");
	case E_DC_S_NP_TYPE:
		return TNL::StringPtr("Session Type");
	default:
		return TNL::StringPtr("Unknown");
		break;
	}
	return TNL::StringPtr("null");
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
int xDistributedSessionClass::NativeTypeToValueType(int nativeType)
{
	int result = 0;
	switch(nativeType)
	{
		case E_DC_S_NP_TYPE:
		case E_DC_S_NP_LOCKED:
		case E_DC_S_NP_NUM_USERS:
		case E_DC_S_NP_MAX_USERS:
			return E_DC_PTYPE_INT;
		case E_DC_S_NP_PASSWORD:
			return E_DC_PTYPE_STRING;
	}
	return E_DC_PTYPE_UNKNOWN;
}