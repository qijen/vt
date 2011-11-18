#ifndef __PWORLDCALLBACKS_H__
#define __PWORLDCALLBACKS_H__

#include "vtPhysXAll.h"



class pRayCastReport : public NxUserRaycastReport
{

public:

	bool onHit(const NxRaycastHit& hit);
	pWorld *mWorld;
	pWorld * getWorld() const { return mWorld; }
	void setWorld(pWorld * val) { mWorld = val; }
	int mCurrentBehavior;
	int getCurrentBehavior() const { return mCurrentBehavior; }
	void setCurrentBehavior(int val) { mCurrentBehavior = val; }
	pRayCastReport()
	{
		mWorld = NULL;
		mCurrentBehavior=-1;
	}
} ;

class pContactReport : public NxUserContactReport
{
public:
	virtual void onContactNotify(NxContactPair& pair, NxU32 events);
	pWorld *mWorld;
	pWorld * getWorld() const { return mWorld; }
	void setWorld(pWorld * val) { mWorld = val; }

} ;

class pContactModify : public NxUserContactModify
{
public:
	virtual bool onContactConstraint(
		NxU32& changeFlags, 
		const NxShape* shape0, 
		const NxShape* shape1, 
		const NxU32 featureIndex0, 
		const NxU32 featureIndex1,
		NxContactCallbackData& data);

	pWorld *mWorld;
	pWorld * getWorld() const { return mWorld; }
	void setWorld(pWorld * val) { mWorld = val; }

} ;


class pTriggerReport : public NxUserTriggerReport    
{        
public:
	void onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status);        
	pWorld *mWorld;
	pWorld * getWorld() const { return mWorld; }
	void setWorld(pWorld * val) { mWorld = val; }
};


#endif // __PWORLDCALLBACKS_H__