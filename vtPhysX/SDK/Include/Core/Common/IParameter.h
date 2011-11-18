#ifndef __IPARAMETER_H__
#define __IPARAMETER_H__


#include "vtPhysXBase.h"

/*!
	\if internal2
		\brief	Interface for parameters, implemented as singleton. 
		Helps to convert from PhysX to Virtools and vice versa. 

		\Note This class has been introduced to collect parameter exchange related functionality. In the initial implementation of 
				this SDK, pFactory was responsible to perform these tasks. 
				
		\Warning We are migrating all parameter related functions from pFactory to here.

	\endif 
				
 */
class MODULE_API IParameter 
{

public:

	/*!
	 * \brief	Default Constructor. Not been used. This is a singleton class and needs to
					be instanced only the PhysicManager
	 */
	IParameter();

	IParameter(PhysicManager*_pManager);

	/************************************************************************************************/
	/** @name RigidBody
	*/
	//@{


	/**
	\brief Conversion for the custom structure #pBSetup. 
	\param[in] pObjectDescr * dst, w: target object
	\param[in] CKParameter * src, r : source parameter. 

	\return int
	
	@see 
	*/
	int copyTo(pObjectDescr*dst,CKParameter*src);

	/**
	\brief Conversion for the custom structure #pBSetup. 
	\param[in] CKParameter * src, r : source parameter. 
	\param[in] pObjectDescr * dst, w: target object

	\return int

	@see 
	*/
	int copyTo(CKParameter*dst,pObjectDescr*src);

	/**
	\brief Conversion for the custom structure #pBPivotSettings. 
	\param[in] CKParameter * dst, w : target parameter. 
	\param[in] pObjectDescr * src, w: src object

	\return int
	@see 

	*/

	int copyTo(pOptimization& dst,CKParameter*src);
	int copyTo(CKParameter*dst,pOptimization src);

	int copyTo(pCCDSettings& dst,CKParameter*src);
	int copyTo(CKParameter*dst,pCCDSettings src);


	int copyTo(pAxisReferencedLength&dst,CKParameter*src,bool evaluate=true);
	int copyTo(CKParameter*dst,pAxisReferencedLength&src,bool evaluate=true);



	int copyTo(pCapsuleSettingsEx& dst,CKParameter*src);
	int copyTo(CKParameter*dst,pCapsuleSettingsEx src);



	int copyTo(pConvexCylinderSettings& dst,CKParameter*src);
	int copyTo(CKParameter*dst,pConvexCylinderSettings& src);

	int copyTo(pObjectDescr&dst,CK3dEntity*src,int copyFlags);
	int copyTo(pObjectDescr&dst,const pObjectDescr&src);


	int copyTo(pMassSettings& dst,CKParameter*src);
	int copyTo(CKParameter*dst,pMassSettings src);

	int copyTo(pPivotSettings& dst,CKParameter*src);
	int copyTo(CKParameter*dst,pPivotSettings src);

	int copyTo(pCollisionSettings& dst,CKParameter*src);
	int copyTo(CKParameter*dst,pCollisionSettings src);

	int copyTo(pWheelDescr& dst,CKParameter*src);
	int copyTo(CKParameter*dst,pWheelDescr src);

	int copyTo(pVehicleBrakeTable& dst,CKParameter*src);

	int copyTo(pLinearInterpolation&dst,CKParameter*src);

	int copyTo(pGearBox *dst,CKParameter*src);
	int copyTo(CKParameter*dst,pGearBox *src);

	int copyTo(pLinearInterpolation&dst,CKParameter*src,int size,float maxValue,float minValue);



	//@}



	static IParameter* Instance();
	
	private:
		PhysicManager* mManager;

};


#define GetIPar() IParameter::Instance()

#endif // __IPARAMETER_H__