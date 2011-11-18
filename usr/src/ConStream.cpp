#include <StdAfx.h>
//#include "stdafx.h"
#include <pch.h>
#include <io.h>
#include <fcntl.h>

#include "ConStream.h"
using namespace std;
//
// The ConStream constructor initializes the object to point to the
// NUL device. It does this by calling two consecutive constructors.
// First, the member variable m_Nul is initialized with a FILE object
// created by opening device "nul", the bit bucket. Second, the base
// class constructor is called with a reference to m_Nul, which is
// an ofstream object. This sets up ConStream so that it will direct
// its output to the given file.
//

ConStream::ConStream() : m_Nul( m_fNul = fopen( "nul", "w" ) ),
#ifdef _UNICODE
                         basic_ostream<wchar_t>( &m_Nul )
#else
                         basic_ostream<char>( &m_Nul )
#endif
{
	lastLine = 0 ;
	fp;
	buf = new  char[MAX_PATH];
    m_FileBuf = 0;
    m_hConsole = INVALID_HANDLE_VALUE;
}

//
// The ConStream destructor always has to close the m_fNul FILE object
// which was created in the constructor. Even if the Open() method has
// been called and the bit bucket isn't being used, the FILE object is
// still using memory and a system file handle.
//
// If the ConStream object has been opened with a call to member function
// Open(), we have to call the Win32 API function FreeConsole() to close
// the console window. If the console window was open, we also call the
// C fclose() function on the m_fConsole member.
//
ConStream::~ConStream()
{
    delete m_FileBuf;
    if ( m_hConsole != INVALID_HANDLE_VALUE ) {
        FreeConsole();
        fclose( fp );
    }
    fclose( fp );
}

//
// Opening the stream means doing these things:
//   1) Opening a Win32 console using the Win32 API
//   2) Getting an O/S handle to the console
//   3) Converting the O/S handle to a C stdio file handle
//   4) Converting the C stdio file handler to a C FILE object
//   5) Attaching the C FILE object to a C++ filebuf
//   6) Attaching the filebuf object to this
//   7) Disabling buffering so we see our output in real time.
//
void ConStream::Open()
{

	// allocate a console for this app

	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text

	// redirect unbuffered STDOUT to the console

	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);

	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen( hConHandle, "w+" );

	*stdout = *fp;

	//setvbuf( stdout, buf, _IOFBF , MAX_PATH );
	setvbuf( stdout, NULL, _IONBF , 0);

	// redirect unbuffered STDIN to the console

	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);

	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen( hConHandle, "r+" );

	*stdin = *fp;

//	setvbuf( stdin, buf, _IOFBF, MAX_PATH  );
	setvbuf( stdin, NULL, _IONBF , 0);


	// redirect unbuffered STDERR to the console

	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);

	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

	fp = _fdopen( hConHandle, "w+" );

	*stderr = *fp;

	//setvbuf( stderr, buf, _IOFBF, MAX_PATH );
	setvbuf( stderr, NULL, _IONBF, 0);
	/*SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_INTENSITY | FOREGROUND_RED |
		FOREGROUND_GREEN);*/
	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
	// point to console as well
	ios::sync_with_stdio();

};

//
// Closing the ConStream is considerably simpler. We just use the
// init() call to attach this to the NUL file stream, then close
// the console descriptors.
//
void ConStream::Close()
{
    if ( fp != INVALID_HANDLE_VALUE ) {
        init( &m_Nul );
        FreeConsole();
        fclose( fp );
//        fp = INVALID_HANDLE_VALUE;
    }
};
