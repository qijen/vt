#include <StdAfx.h>
#include "vtPhysXAll.h"

#include "pVTireFunction.h"

 pTireFunction::pTireFunction()
{
	setToDefault();
}

 void pTireFunction::setToDefault()
{
	extremumSlip = 1.0f;
	extremumValue = 0.02f;
	asymptoteSlip = 2.0f;
	asymptoteValue = 0.01f;	
	stiffnessFactor = 1000000.0f;	//quite stiff by default.
	xmlLink =0;

}

 bool pTireFunction::isValid() const
{
	if(!(0.0f < extremumSlip))			return false;
	if(!(extremumSlip < asymptoteSlip))	return false;
	if(!(0.0f < extremumValue))			return false;
	if(!(0.0f < asymptoteValue))		return false;
	if(!(0.0f <= stiffnessFactor))		return false;

	return true;
}


 float pTireFunction::hermiteEval(float t) const
{

	// This fix for TTP 3429 & 3675 is from Sega.
	// Assume blending functions (look these up in a graph):
	// H0(t) =  2ttt - 3tt + 1
	// H1(t) = -2ttt + 3tt
	// H2(t) =   ttt - 2tt + t
	// H3(t) =   ttt -  tt 

	float v = NxMath::abs(t);
	float s = (t>=0) ? 1.0f : -1.0f;

	float F;

	if(v<extremumSlip)
	{
		// For t in the interval 0 < t < extremumSlip
		// We normalize t:
		// a = t/extremumSlip;
		// and use H1 + H2 to compute F:
		// F = extremumValue * ( H1(a) + H2(a) )

		float a = v/extremumSlip;
		float a2 = a*a;
		float a3 = a*a2;

		F = extremumValue * (-a3 + a2 + a);
	}
	else
	{
		if(v<asymptoteSlip)
		{
			// For the interval extremumSlip <= t < asymtoteSlip
			// We normalize and remap t:
			// a = (t-extremumSlip)/(asymptoteSlip - extremumSlip)
			// and use H0 to compute F:
			// F = extremumValue + (extremumValue - asymtoteValue) * H0(a)
			// note that the above differs from the actual expression but this is how it looks with H0 factorized.

			float a = (v-extremumSlip)/(asymptoteSlip - extremumSlip);
			float a2 = a*a;
			float a3 = a*a2;

			float diff = asymptoteValue - extremumValue;
			F = -2.0f*diff*a3 + 3.0f*diff*a2 + extremumValue;
		}
		else
		{
			F = asymptoteValue;
		}
	}
	return s*F;
}

