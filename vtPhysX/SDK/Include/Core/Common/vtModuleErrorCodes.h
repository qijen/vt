#ifndef __VTMODULE_ERROR_CODES_H__
#define __VTMODULE_ERROR_CODES_H__


/*!
 * \brief
 * Error codes to identifier common errors in the SDK with automatic 
 * string conversion for building blocks
*/
typedef enum E_BB_ERRORS
{
	E_PE_NONE,
	E_PE_INTERN,
	E_PE_PAR,
	E_PE_REF,
	E_PE_XML,
	E_PE_FILE,
	E_PE_NoBody,
	E_PE_NoVeh,
	E_PE_NoWheel,
	E_PE_NoJoint,
	E_PE_NoCloth,
	E_PE_NoSDK,
};

#endif // __VTMODULEERRORCODES_H__