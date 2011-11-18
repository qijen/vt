#include "xDistributed3DObjectClass.h"
#include "xDistributedPropertyInfo.h"
#include "xDistTools.h"



/*xDistributed3DObjectClass::~xDistributed3DObjectClass()
{

}*/

xDistributed3DObjectClass::xDistributed3DObjectClass() : xDistributedClass()
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
 int xDistributed3DObjectClass::getFirstUserField(){ return 8; }
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
xDistributed3DObjectClass::getUserFieldBitValue(int walkIndex)
{
	int userTypeCounter = 0;
	xDistributedPropertiesListType &props  = *getDistributedProperties();
	for (unsigned int i  = 0 ; i < props.size() ; i ++ )
	{
		xDistributedPropertyInfo *dInfo  = props[i];
		if (dInfo->mNativeType==E_DC_3D_NP_USER)
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
xDistributed3DObjectClass::getInternalUserFieldIndex(int inputIndex)
{
	int userTypeCounter = 0;
	xDistributedPropertiesListType &props  = *getDistributedProperties();
	for (unsigned int i  = 0 ; i < props.size() ; i ++ )
	{
		xDistributedPropertyInfo *dInfo  = props[i];
		if (dInfo->mNativeType==E_DC_3D_NP_USER)
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
int xDistributed3DObjectClass::getUserFieldCount()
{
	int userTypeCounter = 0;
	xDistributedPropertiesListType &props  = *getDistributedProperties();
	for (unsigned int i  = 0 ; i < props.size() ; i ++ )
	{
		if (props[i]->mNativeType==E_DC_3D_NP_USER)
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
xDistributed3DObjectClass::addProperty(int nativeType,int predictionType)
{
	xDistributedPropertyInfo *result  = exists(nativeType);
	if (!result)
	{
		TNL::StringPtr name  = xDistTools::NativeTypeToString(nativeType);
		int valueType = xDistTools::NativeTypeToValueType(nativeType);
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
xDistributed3DObjectClass::addProperty(const char*name,int type,int predictionType)
{
	xDistributedPropertyInfo *result  = exists(name);
	if (!result)
	{
		result  = new xDistributedPropertyInfo( name ,type,E_DC_3D_NP_USER ,predictionType );
		getDistributedProperties()->push_back( result );
	}
}
