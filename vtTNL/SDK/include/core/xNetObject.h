/********************************************************************
	created:	2008/05/29
	created:	29:5:2008   13:06
	filename: 	x:\junctions\ProjectRoot\current\vt_plugins\svnLocal\vtTnlMaster\Manager\xNetObject.h
	file path:	x:\junctions\ProjectRoot\current\vt_plugins\svnLocal\vtTnlMaster\Manager
	file base:	xNetObject
	file ext:	h
	author:		mc007
	
	purpose:	
*********************************************************************/
#ifndef _XNETOBJECT_H_
#define _XNETOBJECT_H_

#include "tnl.h"
#include "tnlNetObject.h"
#include "xNetTypes.h"

class xNetInterface;
class vtConnection;

class xNetObject :  public TNL::NetObject
{

	
public:
	xNetObject();
	xNetObject(xNString name);
	virtual ~xNetObject();
	typedef NetObject Parent;

protected:
	xNString m_Name;
	xNetInterface *m_NetInterface;
	int m_UserID;
	vtConnection *mOwnerConnection;
public :
	xNString GetName() { return m_Name; }
	void SetName(xNString val) { m_Name = val; }

	

	xNetInterface * getNetInterface() { return m_NetInterface; }
	void setNetInterface(xNetInterface * val) { m_NetInterface = val; }

	
	int getUserID() const { return m_UserID; }
	void setUserID(int val) { m_UserID = val; }

	virtual void Destroy();
	
	vtConnection * getOwnerConnection() const { return mOwnerConnection; }
	void setOwnerConnection(vtConnection * val) { mOwnerConnection = val; }

};

#endif
