#ifndef __P_REFERENCED_OBJECT_H__
#define __P_REFERENCED_OBJECT_H__

template<class EngineObject,class ImplementationObject>class MODULE_API pStoredObjectAssociation
{
	public:

		//xEngineObjectAssociation() : mInternalId(-1) , mClassId(-1) {};
		//xEngineObjectAssociation(EngineObject _Object,ImplementationObject _ImplObject,int _EngineObjectId) : mEngineObject(_Object) , mImplementationObject(_ImplObject) ,   mInternalId(_EngineObjectId) {  } 
		//xEngineObjectAssociation< (EngineObject _Object,ImplementationObject _ImplObject,int _EngineObjectId){}
		int getInternalId() const { return mInternalId; }
		void setInternalId(int val) { mInternalId = val; }

		int getClassId() const { return mClassId; }
		void setClassId(int val) { mClassId = val; }


		bool existsInCore();

		virtual void onCreate(){};
		virtual void onCopy(int oldId){};
		virtual void onRemove(){};
		virtual void onDelete(){};
		virtual void onCheck(){};

		//operator T(){	return mObject; }


	protected:

	private:
		int mClassId;
		int mInternalId;
		EngineObject mEngineObject;
		ImplementationObject mImplementationObject;

};


template<class T>class MODULE_API xImplementationObject
{
public:

		//xLinkedObjectStorage() : mInternalId(-1) , mClassId(-1) {};
		typedef void* xImplementationObject<T>::*StoragePtr;
		xImplementationObject(){}
		xImplementationObject(StoragePtr storage)
		{ 

		} 

		T getImpl() { return mObject; }


	protected:
	private:
		T mObject;
		StoragePtr mStorage;

};


template<class T>class MODULE_API xEngineObjectAssociation
{
	public:

		xEngineObjectAssociation() :
			mInternalId(-1) , mClassId(-1) {};
		xEngineObjectAssociation(T _Object,int _InternalId) : 
			mObject(_Object) , mInternalId(_InternalId) {  } 
		
		int getInternalId() const { return mInternalId; }
		void setInternalId(int val) { mInternalId = val; }

		int getClassId() const { return mClassId; }
		void setClassId(int val) { mClassId = val; }

		
		bool existsInCore();

		virtual void onCreate(){};
		virtual void onCopy(int oldId){};
		virtual void onRemove(){};
		virtual void onDelete(){};
		virtual void onCheck(){};

		//operator T(){	return mObject; }

	protected:
	private:
		int mClassId;
		int mInternalId;
		T mObject;

};
#endif