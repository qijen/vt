#ifndef __P_CALLBACK_OBJECT_H__
#define __P_CALLBACK_OBJECT_H__

#include <xBitSet.h>
#include "vtInterfaceEnumeration.h"

class pWheelContactModify;
class pCollisionsEntry;
class pTriggerEntry;
class pContactModifyData;
class pWheelContactModifyData;

class MODULE_API pCallbackObject
{
	public:

		pCallbackObject() 
		{
			preScript  = -1;
			postScript = -1;
			callMask = 0;
			overrideMask = -1;
			contactScript = -1;
			rayCastScript = -1;
			wheelContactScript = -1;
			triggerScript = -1;

			triggerEventMask = 0 ; 
			collisionEventMask = 0 ; 
		}

		int contactScript;
		int rayCastScript;
		int wheelContactScript;
		int triggerScript;
		int jointBreakScript;

		int collisionEventMask;
		int contactModificationScript;
		
		int& getCollisionEventMask() { return collisionEventMask; }
		void setCollisionEventMask(int val) { collisionEventMask = val; }

		int triggerEventMask;
		
		int& getTriggerEventMask() { return triggerEventMask; }
		void setTriggerEventMask(int val) { triggerEventMask = val; }


//		virtual void advanceTime(float lastDeltaMS);

		

		//----------------------------------------------------------------
		//
		// generics 
		//
		int overrideMask;
		int& getOverrideMask() { return overrideMask; }
		void setOverrideMask(int val) { overrideMask = val; }

		xBitSet callMask;
		xBitSet& getCallMask() { return callMask; }
		void setCallMask(int val) { callMask = val; }

		int preScript;
		int& getPreScript() { return preScript; }
		void setPreScript(int val) { preScript = val; }

		
		int postScript;
		int& getPostScript() { return postScript; }
		void setPostScript(int val) { postScript = val; }

		virtual void processPostScript(){};
		virtual void processPreScript(){};
		
		virtual int onPreProcess(){ return -1;};
		virtual int onPostProcess(){return -1;};
		
		//----------------------------------------------------------------
		//
		// generic contact call
		//
		int getContactScript() const { return contactScript; }
		virtual void setContactScript(int behaviorID,int eventMask)
		{ 
			contactScript = behaviorID; 
			collisionEventMask = eventMask;
			setFlag(getCallMask(),CB_OnContactNotify,behaviorID);
		}
		virtual int onContact(pCollisionsEntry *report){ return -1;};

		
		//----------------------------------------------------------------
		//
		// raycast, unused ! 
		//
		int getRayCastScript() const { return rayCastScript; }
		virtual void setRayCastScript(int val) 
		{ 
			rayCastScript = val; 
			setFlag(getCallMask(),CB_OnRayCastHit,val);
		}
		virtual bool onRayCastHit(NxRaycastHit *report){ return false;};

		//----------------------------------------------------------------
		//
		// trigger
		//
		int getTriggerScript() const { return triggerScript; }
		virtual void setTriggerScript(int behaviorID,int eventMask,CK3dEntity *shapeReference = NULL) 
		{ 
			triggerScript = behaviorID; 
			triggerEventMask = eventMask;
			setFlag(getCallMask(),CB_OnTrigger,behaviorID);

		}
		virtual int onTrigger(pTriggerEntry *report){ return -1;};


		//----------------------------------------------------------------
		//
		// trigger
		//
		int getJointBreakScript() const { return jointBreakScript; }
		virtual void setJointBreakScript(int behaviorID,CK3dEntity *shapeReference = NULL) 
		{ 
			jointBreakScript = behaviorID; 
			setFlag(getCallMask(),CB_OnJointBreak,behaviorID);
		}
		virtual int onJointBreak(pBrokenJointEntry *entry){ return -1;};

		//----------------------------------------------------------------
		//
		// wheel related
		//
		int getWheelContactScript() const { return wheelContactScript; }
		virtual void setWheelContactScript(int val) { wheelContactScript = val; }

		virtual bool onWheelContact(CK3dEntity* wheelShapeReference, VxVector& contactPoint, VxVector& contactNormal, float& contactPosition, float& normalForce, CK3dEntity* otherShapeReference, int& otherShapeMaterialIndex){return true;}
		virtual bool onWheelContactModify(int& changeFlags,pWheelContactModifyData* contact){ return -1;}

		//----------------------------------------------------------------
		//
		// contact modification
		//
		int getContactModificationScript() const { return contactModificationScript; }
		virtual void setContactModificationScript(int val) 
		{ 
			contactModificationScript = val; 
			setFlag(getCallMask(),CB_OnContactModify,val);
		}
		
		virtual bool onContactConstraint(int& changeFlags,CK3dEntity *sourceObject,CK3dEntity *otherObject,pContactModifyData *data){ changeFlags = CMM_None; return true;		};


		int processOptions;
		virtual int& getProcessOptions() { return processOptions; }
		virtual void setProcessOptions(int val) { processOptions = val; }


	protected:
	private:


};
#endif