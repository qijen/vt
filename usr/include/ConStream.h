#if !defined( _CONSTREAM_H )
#define _CONSTREAM_H

//
// Note that ConStream uses the standard C++ libraries that ship with
// Visual C++ 5.0 and later, not the older libraries with the .h suffix.
// The library would require some modifications to work with the older
// libraries.
//
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <WTypes.h>
#include <fstream>

//using namespace std;

//
// The ConStream class is derived from what we normally think of as ostream,
// which means you can use standard insertion operators to write to it. Of
// course, this includes insertion operators for user defined classes. At
// all times, a ConStream object is either writing out to to a FILE
// object attached to the NUL device, or a FILE object attached to a console
// created using the Win32 API. Which of the two is in use depends on whether
// or not the ConStream object has had its Open() or Close() method called.
//
#define  MAX_BUFFER 4096

class ConStream   : public std::basic_ostream<char>
{
public :
    ConStream();
    virtual ~ConStream();
    void Open();
    void Close();
protected :
    HANDLE m_hConsole;

	std::basic_filebuf<char> *m_FileBuf;
	std::basic_filebuf<char> m_Nul;

	FILE *m_fNul;
    FILE *m_fConsole;

public:
	int lastLine;
	char* buf;
	int hConHandle;
	long lStdHandle;
	FILE *fp;

};

#endif // !defined( _CONSTREAM_H )
