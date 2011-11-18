#ifndef __P_V_TIRE_FUNCTION_H__
#define __P_V_TIRE_FUNCTION_H__

#include "vtPhysXBase.h"

/** \addtogroup Vehicle
@{
*/


/**
\brief cubic hermit spline coefficients describing the longitudinal tire force curve.

Force
^		extrema
|    _*_
|   ~   \     asymptote
|  /     \~__*______________
| /
|/
---------------------------> Slip
*/
class MODULE_API pTireFunction
	{
	public:

	virtual ~pTireFunction(){};
	
	/**
	\brief extremal point of curve.  Both values must be positive.

	<b>Range:</b> (0,inf)<br>
	<b>Default:</b> 1.0
	*/
	float extremumSlip;

	/**
	\brief extremal point of curve.  Both values must be positive.

	<b>Range:</b> (0,inf)<br>
	<b>Default:</b> 0.02
	*/
	float extremumValue;

	/**
	\brief point on curve at which for all x > minumumX, function equals minimumY.  Both values must be positive.

	<b>Range:</b> (0,inf)<br>
	<b>Default:</b> 2.0
	*/
	float asymptoteSlip;
	
	/**
	\brief point on curve at which for all x > minumumX, function equals minimumY.  Both values must be positive.

	<b>Range:</b> (0,inf)<br>
	<b>Default:</b> 0.01
	*/
	float asymptoteValue;


	/**
	\brief Scaling factor for tire force.
	
	This is an additional overall positive scaling that gets applied to the tire forces before passing 
	them to the solver.  Higher values make for better grip.  If you raise the *Values above, you may 
	need to lower this. A setting of zero will disable all friction in this direction.

	<b>Range:</b> (0,inf)<br>
	<b>Default:</b> 1000000.0 (quite stiff by default)
	*/
	float stiffnessFactor;


	/**
	\brief Scaling factor for tire force.
	
	This is an additional overall positive scaling that gets applied to the tire forces before passing 
	them to the solver.  Higher values make for better grip.  If you raise the *Values above, you may 
	need to lower this. A setting of zero will disable all friction in this direction.

	<b>Range:</b> (0,inf)<br>
	<b>Default:</b> 1000000.0 (quite stiff by default)
	*/
//	float stiffnessFactor;


	/**
	constructor sets to default.
	*/
	pTireFunction();	
	/**
	(re)sets the structure to the default.	
	*/
	virtual	void	setToDefault();
	/**
	returns true if the current settings are valid
	*/
	virtual	bool	isValid() const;

	/**
	evaluates the Force(Slip) function
	*/
	float hermiteEval(float t) const;


	int xmlLink;
	
};

/** @} */

#endif