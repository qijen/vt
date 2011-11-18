/********************************************************************
	created:	2008/04/10
	created:	10:4:2008   12:49
	filename: 	x:\junctions\ProjectRoot\current\vt_plugins\vt_tnl\Manager\xDistributedBaseClass.h
	file path:	x:\junctions\ProjectRoot\current\vt_plugins\vt_tnl\Manager
	file base:	xDistributedBaseClass
	file ext:	h
	author:		mc007
	
	purpose:	
*********************************************************************/
#ifndef _XDISTRIBUTED_BASE_CLASS_H_
#define _XDISTRIBUTED_BASE_CLASS_H_
#include "xNetTypes.h"

//typedef XHashTable<xDistributedPropertyInfo*,XString,XHashFunXStringI>xDistributedPropertyInfoArrayType;
//typedef XHashTable<xDistributedPropertyInfo*,XString,XHashFunXStringI>::Iterator xDistributedPropertyInfoArrayIterator;

class xDistributed3DObjectClass;

/*
#ifdef GetClassNameA
	#undef  GetClassNameA
#endif
*/
class xDistributedClass
{
	public:

		xDistributedClass();
		virtual ~xDistributedClass();

		int getEnitityType() { return m_EnitityType; }
		void setEnitityType(int val) { m_EnitityType = val; }

		xNString getClassName();
		void setClassName(xNString name);

		xDistributedPropertiesListType* getDistributedProperties() { return m_DistributedProperties; }		
		int getNativeFlags()	{ return m_NativeFlags; }
		void setNativeFlags( int val ){ m_NativeFlags = val; }

		virtual void addProperty(const char*name,int type,int predictionType){}
		virtual void addProperty(int nativeType,int predictionType){}

		xDistributed3DObjectClass *cast(xDistributedClass *_in);

		virtual int getFirstUserField(){ return 0; }
		virtual int getUserFieldBitValue(int walkIndex){ return 0; }
		virtual int getInternalUserFieldIndex(int inputIndex){ return 0;} 
		virtual int getUserFieldCount(){return 0;}

		virtual xNString NativeTypeToString(int nativeType){ return xNString("");}
		virtual int NativeTypeToValueType(int nativeType){ return -1;}
        
	public : 
		xDistributedPropertyInfo *exists(const char*name);
		virtual xDistributedPropertyInfo *exists(int nativeType);

	protected:

		int m_NativeFlags;
		
		int m_EnitityType;
		xNString m_ClassName;
		int m_NativeProperties;
		
		xDistributedPropertiesListType *m_DistributedProperties;
		
	private:
};

#endif
