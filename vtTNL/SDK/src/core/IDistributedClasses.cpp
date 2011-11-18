#include "IDistributedClasses.h"
#include "xDistributedBaseClass.h"
#include "xDistributed3DObjectClass.h"
#include "xDistributedSessionClass.h"
#include "xDistributedPropertyInfo.h"
#include "xNetInterface.h"
#include "vtConnection.h"
#include "xLogger.h"

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
IDistributedClasses::deployClass(xDistributedClass*_class)
{
	if (!_class)return;
	
	xNetInterface *nInterface   = getNetInterface();
	if (!nInterface)return;

	if (nInterface->IsServer())return;

	//////////////////////////////////////////////////////////////////////////
	//pickup some data :
    
	TNL::StringPtr className(_class->getClassName().getString());
	TNL::Int<16>entityType  = _class->getEnitityType();

	TNL::Vector<TNL::StringPtr>propertyNames;
	TNL::Vector<TNL::Int<16> >nativeTypes;
	TNL::Vector<TNL::Int<16> >valueTypes;
	TNL::Vector<TNL::Int<16> >predictionTypes;
	
	xDistributedPropertiesListType &props  = *_class->getDistributedProperties();
	for (unsigned int i  = 0 ; i < props.size() ; i ++ )
	{
		xDistributedPropertyInfo *dInfo  = props[i];
		propertyNames.push_back( TNL::StringPtr(dInfo->mName) );
		nativeTypes.push_back(dInfo->mNativeType);
		valueTypes.push_back(dInfo->mValueType);
		predictionTypes.push_back(dInfo->mPredictionType);
	}

	//////////////////////////////////////////////////////////////////////////
	//we  call this on the server : 
	if (nInterface->getConnection())
	{
		nInterface->getConnection()->c2sDeployDistributedClass(className,entityType,propertyNames,nativeTypes,valueTypes,predictionTypes);
	}

}

/*
*******************************************************************
* Function: IDistributedClasses::IDistributedClasses
*
* Description: 
*    Distributed class interface destructor. 
* Parameters: 
* Returns: 
*
*******************************************************************
*/
IDistributedClasses::~IDistributedClasses()
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
IDistributedClasses::IDistributedClasses(xNetInterface *netInterface) : m_NetInterface(netInterface)  , m_DistrutedClasses(new xDistributedClassesArrayType() )
{
	//m_DistrutedClasses  = new XDistributedClassesArrayType();
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
xDistributedClassesArrayType* IDistributedClasses::getDistrutedClassesPtr()
{
	return m_DistrutedClasses;
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
xDistributedClass*IDistributedClasses::createClass(const char* name,int templatetype)
{

	if (!strlen(name))		return NULL;

	xDistributedClassesArrayType *_classes = getDistrutedClassesPtr();

	xDistributedClass *result = get(name);

	if (result ==NULL )
	{
		switch(templatetype)
		{
		case E_DC_BTYPE_3D_ENTITY:
			{

				result  = new xDistributed3DObjectClass();
			}
			break;

		case E_DC_BTYPE_CLIENT:
			{
				result  = new xDistributedClass();
			}
			break;

		case E_DC_BTYPE_SESSION:
			{
				result  = new xDistributedSessionClass();
			}
			break;

		default:
			{
				result  = new xDistributedClass();
			}
			break;
		}

		result->setClassName(name);
		result->setEnitityType(templatetype);
		_classes->insert(std::make_pair(name,result));
		//xLogger::xLog(ELOGINFO,E_LI_DISTRIBUTED_CLASS_DESCRIPTORS,"Distributed class created  : %s : type : %d",name,templatetype);
		//xLogger::xLog(XL_START,ELOGINFO,E_LI_DISTRIBUTED_CLASS_DESCRIPTORS,"Distributed class created  : %s : type : %d",name,templatetype);
		return result;
	}else
	{
		return result;
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
xDistributedClass*
IDistributedClasses::getByIndex(int index)
{
	/*if (index > 0 && index < getDistrutedClassesPtr()->size() )
	{
		return *getDistrutedClassesPtr()->GetByIndex(index);
	}*/
	return NULL;
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
IDistributedClasses::destroyClass(xDistributedClass *_class)
{
	if (_class ==NULL )
	{
		xLogger::xLog(XL_START,ELOGERROR,E_LI_DISTRIBUTED_CLASS_DESCRIPTORS,"invalid object");
		return -1;
	}

	xDistributedClassesArrayType *_classes = getDistrutedClassesPtr();
	xDistClassIt begin = _classes->begin();
	xDistClassIt end = _classes->end();

	while (begin!=end)
	{
		xDistributedClass *_tclass = begin->second;
		if (_tclass == _class)
		{
			xNString cName = _class->getClassName().getString();
			_classes->erase(begin);
			delete _tclass;
			//xLogger::xLog(XL_START,ELOGINFO,E_LI_DISTRIBUTED_CLASS_DESCRIPTORS,"%s removed and destroyed! Classes left :%d ",cName.getString(),_classes->size());
			return 1;
		}
		begin++;
	}
	//xLogger::xLog(XL_START,ELOGERROR,E_LI_DISTRIBUTED_CLASS_DESCRIPTORS,"couldn't find object");
	return 0;
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
int IDistributedClasses::getNumClasses(){	return getDistrutedClassesPtr()->size();}

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
xDistributedClass*IDistributedClasses::get(const char* name)
{
	xDistributedClassesArrayType *_classes = getDistrutedClassesPtr();
	xDistClassIt begin = _classes->begin();
	xDistClassIt end = _classes->end();


	while (begin!=end)
	{
		xDistributedClass *_class = begin->second;
		if (_class)
		{
			if (!strcmp(_class->getClassName().getString(),name) )
			{
				return _class;
			}
		}
		begin++;
	}
	return NULL;
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
xDistributedClass*IDistributedClasses::get(const char* name,int entityType)
{
	xDistributedClassesArrayType *_classes = getDistrutedClassesPtr();
	xDistClassIt begin = _classes->begin();
	xDistClassIt end = _classes->end();


	while (begin!=end)
	{
		xDistributedClass *_class = begin->second;
		if (_class)
		{
			if (!strcmp(_class->getClassName().getString(),name) && _class->getEnitityType() == entityType)
			{
				return _class;
			}
		}
		begin++;
	}
	return NULL;
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
void IDistributedClasses::destroy()
{
	xDistributedClassesArrayType *_classes = getDistrutedClassesPtr();
	xDistClassIt begin = _classes->begin();
	xDistClassIt end = _classes->end();
	_classes->clear();
	return;
	while (begin!=end)
	{
		xDistributedClass *_class = begin->second;
		if (_class)
		{
			delete _class;
			_class = NULL;
			begin = _classes->begin();
		}
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
xNetInterface* IDistributedClasses::getNetInterface(){ return m_NetInterface; }
