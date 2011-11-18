#ifndef __X_PLATFORM__
	#define __X_PLATFORM__

	enum E_ASSERTION_FAILURE_SEVERITY
	{
		AFS__MIN,
		
		AFS_ASSERT=AFS__MIN,
		AFS_CHECK,

		AFS__MAX,
	};

	//----------------------------------------------------------------
	//
	// host identifier
	//

	#define xTARGET_OS_GENUNIX	1
	#define xTARGET_OS_WINDOWS	2
	#define xTARGET_OS_QNX		3
	#define xTARGET_OS_MAC		4

	//----------------------------------------------------------------
	//
	// base types
	//
	#if !defined(__LINE__)
		extern const unsigned int __LINE__;
	#endif 


	#if !defined(__FILE__)
		extern const char *const __FILE__;
	#endif 


	//----------------------------------------------------------------
	//
	// Determine platform
	//

	#if defined(_WINDOWS) || defined(_WIN32)

		#define xTARGET_OS			xTARGET_OS_WINDOWS

	#elif defined(__QNX__)

		#define xTARGET_OS			xTARGET_OS_QNX

	#elif defined(__APPLE__)

		#define xTARGET_OS			xTARGET_OS_MAC

	#endif

	//----------------------------------------------------------------
	//
	// determine call convention
	//
	#if xTARGET_OS == xTARGET_OS_WINDOWS

		#define xCONVENTION_METHOD
		#define xCONVENTION_API __stdcall
		#define xCONVENTION_CALLBACK __stdcall


	#else // #if xTARGET_OS != xTARGET_OS_WINDOWS

		#define xCONVENTION_METHOD
		#define xCONVENTION_API
		#define xCONVENTION_CALLBACK

	#endif


	//----------------------------------------------------------------
	//
	// cpu bandwidth 
	//
	#if !defined(xTARGET_BITS)
	
		#if defined(_LP64) || defined(_WIN64)
			#define xTARGET_BITS			xTARGET_BITS_64
		#else 
			#define xTARGET_BITS			xTARGET_BITS_32
		#endif 

	#else // #if defined(xTARGET_BITS)

		#if xTARGET_BITS <= 0 || xTARGET_BITS >= xTARGET_BITS__MAX

			#error Please define a valid value for xTARGET_BITS

		#endif 

	#endif 

	//----------------------------------------------------------------
	//
	// determine compiler
	//

	#define xCOMPILER__OTHER	1
	#define xCOMPILER_GCC	2
	#define xCOMPILER_MSVC	3

	#define xCOMPILER__MAX	4


	#define xCOMPILER_VERSION__OTHER		1
	#define xCOMPILER_VERSION_MSVC1998	2
	#define xCOMPILER_VERSION_GCCLT4		3

	#define xCOMPILER_VERSION__MAX		4


	//////////////////////////////////////////////////////////////////////////

	#if !defined(xCOMPILER)

		#if defined(__GNUC__)

			#define xCOMPILER		xCOMPILER_GCC

			#if __GNUC__ < 4
			
				#define xCOMPILER_VERSION	xCOMPILER_VERSION_GCCLT4
			
			#endif 
	
		
		#elif defined(_MSC_VER)

			#define xCOMPILER		xCOMPILER_MSVC

				#if _MSC_VER <= 1200

					#define xCOMPILER_VERSION	xCOMPILER_VERSION_MSVC1998

				#endif 

		#else 

			#define xCOMPILER		xCOMPILER__OTHER

		#endif 


	#else 

		#if xCOMPILER <= 0 || xCOMPILER >= xCOMPILER__MAX

			#error Please define a valid value for xCOMPILER

		#endif 

	#endif 

	
	#if !defined(xCOMPILER_VERSION)

		#define xCOMPILER_VERSION xCOMPILER_VERSION__OTHER

	#endif 


	
	#if xCOMPILER_VERSION <= 0 || xCOMPILER_VERSION >= xCOMPILER_VERSION__MAX

		#error Please define a valid value for xCOMPILER_VERSION

	#endif 


	//----------------------------------------------------------------
	//
	// inline , always inline
	//
	#if !defined(_xINLINES_DEFINED)

		#define _xINLINES_DEFINED


		#if xCOMPILER == xCOMPILER_GCC

			#define xALWAYSINLINE_PRE__DEFINITION inline
			#define xALWAYSINLINE_IN__DEFINITION __attribute__((always_inline))

		#elif xCOMPILER == xCOMPILER_MSVC

			#define xALWAYSINLINE_PRE__DEFINITION __forceinline
			#define xALWAYSINLINE_IN__DEFINITION

		#else 

			#define xALWAYSINLINE_PRE__DEFINITION inline
			#define xALWAYSINLINE_IN__DEFINITION

		#endif 



		#if defined(_DEBUG)

			#define xALWAYSINLINE_PRE inline
			#define xALWAYSINLINE_IN
			#define xINLINE inline
		
		#else

			#define xALWAYSINLINE_PRE xALWAYSINLINE_PRE__DEFINITION
			#define xALWAYSINLINE_IN xALWAYSINLINE_IN__DEFINITION
			#define xINLINE inline
		#endif 

	#endif // #if !defined(_xINLINES_DEFINED)


#endif