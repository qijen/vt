#ifndef __PWHEELTYPES_H__
#define __PWHEELTYPES_H__

#include "NxVec3.h"
#include "NxMat34.h"
#include "NxUserContactReport.h"

//#include "pRigidBodyTypes.h"
//#include "pVTireFunction.h"




/** \addtogroup Vehicle
@{
*/

class pWheelContactModifyData
{
public :
	CK3dEntity *object;
	VxVector contactPoint;
	VxVector contactNormal;
	float contactPosition;
	float normalForce;
	int otherMaterialIndex;
	pWheelContactModify()
	{
		object = NULL;
		contactPosition = 0.0f;
		normalForce = 0.0f;
		otherMaterialIndex = 0;
	}
};




struct ContactInfo 
{
	ContactInfo() { reset(); }
	void reset() { otherActor = NULL; relativeVelocity = 0; }
	bool isTouching() const { return otherActor != NULL; }
	NxActor*				otherActor;
	NxVec3					contactPosition;
	NxVec3					contactPositionLocal;
	NxVec3					contactNormal;
	NxReal					relativeVelocity;
	NxReal					relativeVelocitySide;
};


/**
\brief Contact information used by pWheel


@see pWheel pWheel.getContact()
*/
class pWheelContactData
{
public:
	/**
	\brief The point of contact between the wheel shape and the ground.

	*/
	VxVector contactPoint;

	/**
	\brief The normal at the point of contact.

	*/
	VxVector contactNormal;

	/**
	\brief The direction the wheel is pointing in.
	*/
	VxVector longitudalDirection;

	/**
	\brief The sideways direction for the wheel(at right angles to the longitudinal direction).
	*/
	VxVector lateralDirection;

	/**
	\brief The magnitude of the force being applied for the contact.
	*/
	float contactForce;

	/**
	\brief What these exactly are depend on NX_WF_INPUT_LAT_SLIPVELOCITY and NX_WF_INPUT_LNG_SLIPVELOCITY flags for the wheel.
	*/
	float longitudalSlip, lateralSlip;

	/**
	\brief the clipped impulses applied at the wheel.
	*/
	float longitudalImpulse, lateralImpulse;

	/**
	\brief The material index of the shape in contact with the wheel.

	@see pMaterial 
	*/
	int otherShapeMaterialIndex;

	/**
	\brief The distance on the spring travel distance where the wheel would end up if it was resting on the contact point.
	*/
	float contactPosition;

	/**
	\brief The distance on the spring travel distance where the wheel would end up if it was resting on the contact point.
	*/
	CK3dEntity* contactEntity;


	/**
	\brief The material of the colliding shape 
	*/
	pMaterial otherMaterial;

};

/**
\brief An interface class that the user can implement in order to modify the contact point on which the 
WheelShape base its simulation constraints.

<b>Threading:</b> It is <b>necessary</b> to make this class thread safe as it will be called in the context of the
simulation thread. It might also be necessary to make it reentrant, since some calls can be made by multi-threaded
parts of the physics engine.

You enable the use of this callback by specifying a callback function in NxWheelShapeDesc.wheelContactModify 
or by setting a callback function through NxWheelShape.setUserWheelContactModify().

Please note: 
+ There will only be callbacks if the WheelShape finds a contact point. Increasing the suspensionTravel value
gives a longer raycast and increases the chance of finding a contact point (but also gives a potentially slower 
simulation).

@see NxWheelShapeDesc.wheelContactModify NxWheelShape.setUserWheelContactModify() NxWheelShape.getUserWheelContactModify()
*/
class MODULE_API pWheelContactModify :  NxUserWheelContactModify
{
	public:

		pWheelContactModify() :
		  wheel(NULL) , lastContactModifyData()
		  {

		  }
		/**
		\brief This callback is called once for each wheel and sub step before the wheel constraints are setup
		and fed to the SDK. The values passed in the parameters can be adjusted to affect the vehicle simulation.
		The most interesting values are contactPosition, contactPoint, and contactNormal. The contactPosition value
		specifies how far on the travel distance the contactPoint was found. If you want to simulate a bumpy road,
		then this is the main parameter to change. It is also good to adjust the contactPoint variable, so that the
		wheel forces are applied in the correct position. 

		\param wheelShape The WheelShape that is being processed.
		\param contactPoint The contact point (in world coordinates) that is being used for the wheel.
		\param contactNormal The normal of the geometry at the contact point.
		\param contactPosition The distance on the spring travel distance where the wheel would end up if it was resting on the contact point.
		\param normalForce The normal force on the wheel from the last simulation step.
		\param otherShape The shape with which the wheel is in contact.
		\param otherShapeMaterialIndex The material on the other shape in the position where the wheel is in contact. Currently has no effect on the simulation.
		\param otherShapeFeatureIndex The feature on the other shape in the position where the wheel is in contact.

		\return Return true to keep the contact (with the possibly edited values) or false to drop the contact.
		*/
		virtual bool onWheelContact(NxWheelShape* wheelShape, NxVec3& contactPoint, NxVec3& contactNormal, NxReal& contactPosition, NxReal& normalForce, NxShape* otherShape, NxMaterialIndex& otherShapeMaterialIndex, NxU32 otherShapeFeatureIndex);

		pWheelContactModifyData* lastContactModifyData;


		pWheelContactModifyData* getLastContactModifyData() const { return lastContactModifyData; }
		void setLastContactModifyData(pWheelContactModifyData* val) { lastContactModifyData = val; }


		pWheel2* getWheel() const { return wheel; }
		void setWheel(pWheel2* val) { wheel = val; }
		
		pWheelContactModifyData& getLastData(){ return lastData; }
		void setLastData(pWheelContactModifyData val) { lastData = val; }


private:

	pWheel2* wheel;
	pWheelContactModifyData lastData;
	

};




/** @} */

#endif // __PWHEELTYPES_H__