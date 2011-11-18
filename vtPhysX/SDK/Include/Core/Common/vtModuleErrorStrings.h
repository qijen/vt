#ifndef __VT_MODULE_ERROR_STRINGS_H__
#define __VT_MODULE_ERROR_STRINGS_H__

/*!
 * \brief
 * String to describe the error's source
 */
static char* sLogItems[]=
{
	"AGEIA",
	"MANAGER",
	"VSL",
};

/*!
 * \brief
 * String to describe generic return codes
 */
static char* sErrorStrings[]=
{
	"OK",
	"Ageia error",
	"Invalid parameter",
	"Invalid operation",
};

/*!
 * \brief
 * String to describe component specific return codes
 * 
 */
static char* sBBErrorStrings[]=
{
	"OK",
	"\t Intern :",
	"\t Parameter invalid :",
	"\t Reference object invalid:",
	"\t XML error:",
	"\t Invalid File:",
	"\t Reference object is not physicalized:",
	"\t Reference object doesn't contains a vehicle controller:",
	"\t Reference object is not a wheel:",
	"\t Reference object is not a joint:",
	"\t Reference object is not a cloth:",
	"\t Couldn't initialize PhysX :",

};


#endif // __VTMODULEERRORSTRINGS_H__