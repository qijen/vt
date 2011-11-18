#ifndef __bCCOError_H
#define __bCCOError_H

#include <CKAll.h>

/**
* Within the Virtools C/C++ SDK, this element a learning application error.
*/
class bCCOError
{

public:
	/**
	* Within the Virtools C/C++ SDK, this enumeration represents the type of  the
	* error which will be used to determine how the handling operation reacts to the
	* error.
	*/
	enum bECOErrorType
	{
		/**
		* Within the Virtools C/C++ SDK, this enumeration denotes a fatal GBL platform
		* error.  The GBL platform will take appropriate steps to shutdown the learning
		* application in such circumstances.
		*/
		bERROR_FATAL = 2,
		/**
		* Within the Virtools C/C++ SDK, this enumeration denotes a local GBL platform
		* error.  The GBL platform will not take additional steps to handle such an error,
		* instead, it is the responsibility of the operation receiving such an error to
		* take appropriate actions.
		*/
		bERROR_LOCAL = 1,
		/**
		* Within the Virtools C/C++ SDK, this enumeration denotes a successful status,
		* generally used by operations as a successful return value.
		*/
		bERROR_OK = 0
	};

	/**
	* The destructor of the class.
	**/
	bCCOError() {;}
	
	/**
	* This is the copy constructor for the GBL error class, ensuring that 
	* a deep copy of the information is performed, including the internal 
	* error description. 
	**/
	bCCOError(bCCOError&error)
		{
			errorType = error.GetType();
			errorDescription = error.GetDescription();
			errorCode = error.GetCode();
		}

        inline bCCOError& operator=( const bCCOError &rhs )
        {
			errorType = rhs.GetType();
			errorDescription = rhs.GetDescription();
			errorCode = rhs.GetCode();

            return *this;
        }

        inline bCCOError& operator=( const bECOErrorType &type )
        {
			errorType = type;

            return *this;
        }

        inline bCCOError& operator=( const int &code )
        {
			errorCode = code;

            return *this;
        }

        inline bCCOError& operator=( const XString &desc )
        {
			errorDescription = desc;

            return *this;
        }

	/**
	* A constructor for the class, which only requires the type to be specified.
	**/
	bCCOError(bCCOError::bECOErrorType type) : 
		errorType(type) 
		{;}
	
	/**
	* A standard constructor for the class, which will accept values being supplied 
	* for all values contained within the error. NOTE: This operation also specifies
	* default initial values for any of the parameters that are not provided.
	**/
	bCCOError(bCCOError::bECOErrorType type , XString description, int code) : 
		errorType(type) , errorDescription(description), errorCode(code) 
		{;}
	
	/**
	* The destructor of the class.
	**/
	~bCCOError() {;}

	/**
	* Implicit conversion to string.
	**/
	inline operator const char *( ) const 
	{
	     return errorDescription.CStr();    	
	}
    inline const char * GetDescription()const
    {
        return errorDescription.CStr();
    }
	
	/**
	* Implicit conversion to int. It will return the error code.
	**/
	inline operator int()const{
		return errorCode;
	}
    inline int GetCode()const
    {
        return errorCode;
    }
	
	/**
	* Implicit conversion to bECOErrorType.  It will return the error type.
	**/
	inline operator bECOErrorType()const
	{
		return errorType;
	}
    inline bECOErrorType GetType()const
    {
        return errorType;
    }

	/**
	* An accessor operation to set the value of the description for the GBL error.
	**/
	inline SetDescription(XString desc)
	{
		errorDescription = desc;
	}
	
	/**
	* An accessor operation to set the value of the code for the GBL error.
	**/
	inline SetCode(int code)
	{
		errorCode = code;
	}

	/**
	* An accessor operation to set the value of the type for the GBL error.
	**/
	inline SetType(bECOErrorType type)
	{
		errorType = type;
	}

private:
	/**
	* This element represents the unique code associated with the GBL platform error.
	*/
	int errorCode;
	/**
	* This element represents the human readable description associated with error,
	* that is suitable for being displayed in a dialog or log file.
	*/
	XString errorDescription;
	/**
	* This element is represents the severity associated with the GBL platform error.
	*/
	bCCOError::bECOErrorType errorType;

};
#endif