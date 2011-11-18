#include "xNetEnumerations.h"
#include "xDistTools.h"

namespace xDistTools
{

xNString ValueTypeToString(int valueType)
{

	switch(valueType)
	{
		case E_DC_PTYPE_3DVECTOR:
			return xNString("3D Vector");
		case E_DC_PTYPE_2DVECTOR:
			return xNString("2D Vector");
		case E_DC_PTYPE_INT:
			return xNString("Integer");
		case E_DC_PTYPE_FLOAT:
			return xNString("Float");
		case E_DC_PTYPE_STRING:
			return xNString("String");
	}

	return xNString("Unknown");
}

int SuperTypeToValueType(int superType)
{

	switch(superType)
	{
	case vtVECTOR2D:
		return E_DC_PTYPE_2DVECTOR;
	case vtVECTOR:
		return E_DC_PTYPE_3DVECTOR;
	case vtINTEGER:
		return E_DC_PTYPE_INT;
	case vtQUATERNION:
		return E_DC_PTYPE_QUATERNION;
	case vtSTRING:
		return E_DC_PTYPE_STRING;
	case vtBOOL:
		return E_DC_PTYPE_INT;
	case vtFLOAT:
		return E_DC_PTYPE_FLOAT;
	default :
		return E_DC_PTYPE_UNKNOWN;

	}
	return E_DC_PTYPE_UNKNOWN;
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
int ValueTypeToSuperType(int valueType)
{



	switch(valueType)
	{
		case E_DC_PTYPE_3DVECTOR:
			return vtVECTOR;
		case E_DC_PTYPE_2DVECTOR:
			return vtVECTOR2D;
		case E_DC_PTYPE_QUATERNION:
			return vtQUATERNION;
		case E_DC_PTYPE_INT:
			return vtINTEGER;
		case E_DC_PTYPE_FLOAT:
			return vtFLOAT;
		case E_DC_PTYPE_STRING:
			return vtSTRING;
	}
	return vtUNKNOWN;
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

int NativeTypeToValueType(int nativeType)
{
	int result = 0;
	switch(nativeType)
	{
		case E_DC_3D_NP_LOCAL_POSITION:
		case E_DC_3D_NP_LOCAL_SCALE:
		case E_DC_3D_NP_WORLD_POSITION:
		case E_DC_3D_NP_WORLD_SCALE:
			return E_DC_PTYPE_3DVECTOR;
		case E_DC_3D_NP_WORLD_ROTATION:
		case E_DC_3D_NP_LOCAL_ROTATION:
			return E_DC_PTYPE_QUATERNION;
		case E_DC_3D_NP_VISIBILITY:
			return E_DC_PTYPE_BOOL;
	    break;
	}
	return E_DC_PTYPE_UNKNOWN;
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
TNL::StringPtr NativeTypeToString(int nativeType)
{

	switch(nativeType)
	{
		case E_DC_3D_NP_LOCAL_POSITION:
			return TNL::StringPtr("Local Position");
			break;
		case E_DC_3D_NP_LOCAL_ROTATION:
			return TNL::StringPtr("Local Rotation");
			break;
		case E_DC_3D_NP_LOCAL_SCALE:
			return TNL::StringPtr("Local Scale");
		case E_DC_3D_NP_WORLD_POSITION:
			return TNL::StringPtr("World Position");
			break;
		case E_DC_3D_NP_WORLD_ROTATION:
			return TNL::StringPtr("World Rotation");
			break;
		case E_DC_3D_NP_WORLD_SCALE:
			return TNL::StringPtr("World Scale");
			break;
		case E_DC_3D_NP_VISIBILITY:
			return TNL::StringPtr("Visibility");
			break;
		default:
			return TNL::StringPtr("Unknown");
			break;
	}
	return TNL::StringPtr("null");
}

}
