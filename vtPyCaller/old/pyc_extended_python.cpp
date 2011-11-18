#include "BaseMacros.hpp"
#include "pch.h"
#include "CKAll.h"

#ifdef __cplusplus
extern "C" {
#endif

	API_EXPORT int API_cDECL InitPyVTModule(CKContext** _ctx);

#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

#include <boost/python.hpp>
namespace python = boost::python;

class Player
{
public:

	Player() {}

	Player( std::string name ) 
		: m_name(name) {}

	Player( std::string name, int score ) 
		: m_name(name),
		  m_score(score) {}

	void setName( std::string name ) { m_name = name; }
	std::string getName() { return m_name; }

	void setScore( int score ) { m_score = score; }
	int getScore() { return m_score; }

private:

	std::string m_name;
	int m_score;
};


BOOST_PYTHON_MODULE( PyVT )
{	

	python::class_<Player>("Player")
		.def( python::init<std::string>() )        // Overloaded constructor version #1
		.def( python::init<std::string, int>() )   // Overloaded constructor version #2
		.def( "setName", &Player::setName )
		.def( "getName", &Player::getName )
		.def( "setScore", &Player::setScore )
		.def( "getScore", &Player::getScore )
		;
}



BOOL APIENTRY DllMain( HANDLE hModule, 
					  DWORD  ul_reason_for_call, 
					  LPVOID lpReserved
					  )
{




	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:	 
		{
		//DisableThreadLibraryCalls(NULL);
		//MessageBox(NULL,"","attach",0);

		//	int y = 0;


		}

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#include "pch.h"
#include "CKAll.h"
CKContext *mctx=NULL;

int InitPyVTModule(CKContext ** _ctx)
{
	//MessageBox(NULL,"","asd",0);
	
	DebugBreak();
	if( PyImport_AppendInittab( "PyVT", initPyVT ) == -1 )
		throw runtime_error( "Failed to add \"testDerived\" to the "
		"interpreter's built-in modules" );


	if(*_ctx)
	{
		
		mctx = *_ctx;
		mctx->OutputToConsole("ASDASD");
		printf(("asdasdasd"));
		return 1;
	}
	return 0;
}	