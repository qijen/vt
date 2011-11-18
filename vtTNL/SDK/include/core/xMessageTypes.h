#ifndef __XMESSAGE_TYPES_H_
#define __XMESSAGE_TYPES_H_

////////////////////////////////////////////////////////////////////////// //////////////////////////////////////////////////////////////////////////
#include "xPoint.h"
#include "xMessage.h"

class xMessageType
{


	public :
		xMessageType() 
		{
			type = -1;
			name ="";
			sessionID =-1;
			numParameters =-1;
		}

	int getType() const { return type; }
	void setType(int val) { type = val; }
	xNString getName() const { return name; }
	void setName(xNString val) { name = val; }
	int getNumParameters() const { return numParameters; }
	void setNumParameters(int val) { numParameters = val; }
	int getSessionID() const { return sessionID; }
	void setSessionID(int val) { sessionID = val; }

	protected :
		int type;
		
		xNString name;
		
		int numParameters;
		
		int sessionID;
		
};


class xNetMessage
{

public :
	xNetMessage(){}
	virtual ~xNetMessage(){}
	int type;
	int getType() const { return type; }
	void setType(int val) { type = val; }

};
//////////////////////////////////////////////////////////////////////////
template<class T>class xMessageData : public xNetMessage
{
public : 
	typedef T*  ValueRef;
	T value;


	T getValue(){return value;}
	void setValue(const T& _value )
	{
		value  = _value;
	}
	virtual ~xMessageData(){}

};
//////////////////////////////////////////////////////////////////////////
class xMessageFloat : public xMessageData<TNL::F32>
{
public :
	xMessageFloat(){}
};
//////////////////////////////////////////////////////////////////////////
class xMessageInt : public xMessageData<TNL::S32>
{
public :
	xMessageInt(){}
};
//////////////////////////////////////////////////////////////////////////
class xMessageString : public xMessageData<TNL::StringPtr>
{
public :
	xMessageString(){}
};
//////////////////////////////////////////////////////////////////////////
class xMessagePoint3F : public xMessageData<Point3F>
{
public :
	xMessagePoint3F(){}
};
//////////////////////////////////////////////////////////////////////////
class xMessagePoint4F : public xMessageData<Point4F>
{
public :
	xMessagePoint4F(){}
};
//////////////////////////////////////////////////////////////////////////

#endif