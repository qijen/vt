// --------------------------------------------------------------------------
//					www.UnitedBusinessTechnologies.com
//			  Copyright (c) 1998 - 2002  All Rights Reserved.
//
// Source in this file is released to the public under the following license:
// --------------------------------------------------------------------------
// This toolkit may be used free of charge for any purpose including corporate
// and academic use.  For profit, and Non-Profit uses are permitted.
//
// This source code and any work derived from this source code must retain 
// this copyright at the top of each source file.
// 
// UBT welcomes any suggestions, improvements or new platform ports.
// email to: XMLFoundation@UnitedBusinessTechnologies.com
// --------------------------------------------------------------------------
#include "pch.h"
#include "GString.h"
#include "GException.h"

#include <string.h> // for: memcpy(), strlen(), strcmp(), memmove(), strchr(), 
					//      strstr(), strncmp()
#include <ctype.h>  // for: isdigit()
#include <stdlib.h> // for: atof(), atoi(), atol(), abs(), and strtol()
#include <stdio.h>  // for: sprintf(),  vsprintf(), FILE, fopen(), fwrite(), 
					//      fclose(), fseek(), fread()
#include <stdarg.h> // for: va_start(), va_end */


// Global default format specifiers that affect all GStrings
GString	GString::g_FloatFmt("%.6g");
GString	GString::g_DoubleFmt("%.6g");		
GString	GString::g_LongDoubleFmt("%.6Lg");



#ifndef _WIN32
	#ifndef __int64
		#define	__int64 long long
	#endif
#endif



static void x64toa (      
        unsigned __int64 val,
        char *buf,
        unsigned radix,
        int is_neg
        )
{
        char *p;                /* pointer to traverse string */
        char *firstdig;         /* pointer to first digit */
        char temp;              /* temp char */
        unsigned digval;        /* value of digit */

        p = buf;

        if ( is_neg )
        {
            *p++ = '-';         /* negative, so output '-' and negate */
            val = (unsigned __int64)(-(__int64)val);
        }

        firstdig = p;           /* save pointer to first digit */

        do {
            digval = (unsigned) (val % radix);
            val /= radix;       /* get next digit */

            /* convert to ascii and store */
            if (digval > 9)
                *p++ = (char) (digval - 10 + 'a');  /* a letter */
            else
                *p++ = (char) (digval + '0');       /* a digit */
        } while (val > 0);

        /* We now have the digit of the number in the buffer, but in reverse
           order.  Thus we reverse them now. */

        *p-- = '\0';            /* terminate string; p points to last digit */

        do {
            temp = *p;
            *p = *firstdig;
            *firstdig = temp;   /* swap *p and *firstdig */
            --p;
            ++firstdig;         /* advance to next two digits */
        } while (firstdig < p); /* repeat until halfway */
}


// long long to ascii
char * lltoa ( __int64 val,char *buf,int radix)
{
        x64toa((unsigned __int64)val, buf, radix, (radix == 10 && val < 0));
        return buf;
}





#define CommonConstruct(nInitialSize)				\
	_str = 0;										\
	_len = 0;										\
	_strIsOnHeap = 0;								\
	_max = nInitialSize;							\
	if (nInitialSize == 256)						\
		_str = _initialbuf;							\
	else											\
	{												\
		_initialbuf[0] = 0;							\
		_str = new char[nInitialSize];				\
		_strIsOnHeap = 1;							\
	}												\
	if (!_str)										\
		throw GenericException("String", 0);		\
	_str[_len] = 0;


void GString::AppendEscapeXMLReserved(const char *Src, int nLen/* = -1*/)
{
	const unsigned char *src = (const unsigned char *)Src;
	while (*src && (nLen != 0))
	{
		if (nLen != -1) // -1 means to append until a null in src
			nLen--;

		switch (*src)
		{
			case '<' :
			case '>' :
			case '&' :
			case '"' :
			case '\'' :
				this->write("&#",2);
				(*this) += (unsigned int)(unsigned char)*src;
				(*this) += ';';
				break;
			default :
				// IE can't deal with special chars(йвезкли....) so escape them too
				if ((*src > 127) || (*src < 32))
				{
					this->write("&#",2);
					(*this) += (unsigned int)(unsigned char)*src;
					(*this) += ';';
					break;

				}
				else // most cases, append normal char to this GString
				{
					// inline operator += code
					if (_len >= _max)
						resize();
					_str[_len] = *src;
					_len++;

					if (_len >= _max)
						resize();
					_str[_len] = 0;
				}
				break;
		}
		src++;
	}
}

void GString::AppendXMLTabs( int nTabs )
{
	static const char* TABS =															
	"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"	
	"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
	const int MAX_TABS = 80;


	if (_len)
#ifdef _WIN32
		(*this) << "\r\n";
#else
		(*this) << "\n";
#endif


	write( TABS, (nTabs > MAX_TABS) ? MAX_TABS : nTabs  );
}


void GString::EscapeXMLReserved()
{
	GString str_xml( Length() + 1024 );
	const unsigned char *src = (const unsigned char *)(const char *)(*this);

	while (*src)
	{
		switch (*src)
		{
			case '<' :
			case '>' :
			case '&' :
			case '"' :
			case '\'' :
				str_xml << "&#" << (unsigned int)*src << ';';				
				break;
			default :
				if ((*src > 127) || (*src < 32))
				{
					str_xml << "&#" << (unsigned int)((unsigned char)*src) << ';';				
				}
				else
				{
					str_xml << *src;
				}
				break;
		}
		src++;
	}

	(*this) = str_xml;
}


void GString::resize()
{
	// double the size of the buffer
	_max <<= 1;
	char *nstr = new char[_max];
	
	// not enough memory for resize
	if (!nstr)
		throw GenericException("String", 0);

	nstr[0] = 0;
	
	if (_str)
	{
		memcpy(nstr, _str, _len);
		if (_strIsOnHeap)
			delete [] _str;
	}

	_str = nstr;
	_strIsOnHeap = 1;
	_initialbuf[0] = 0;
}

// constructs an empty string
GString::GString(long nInitialSize)
{
	CommonConstruct(nInitialSize);
}

// constructs a copy of the source string 
GString::GString(const GString &src )
{
	long l = (src._len > 256) ? src._len + 256 : 256;
	CommonConstruct( l );

	_len = ___min(_max, src._len);
	memcpy(_str, src._str, _len);
	_str[_len] = 0;
}

GString::GString(const GString &src, int nCnt)
{
	long l = (src._len > 256) ? src._len + 256 : 256;
	CommonConstruct(l);

	_len = ___min(_max, ___min(src._len, nCnt));
	memcpy(_str, src._str, _len);
	_str[_len] = 0;
}

// constructs a copy of the character string
GString::GString(const char *src)
{
	long srcLen = (src) ? strlen(src) : 256;
	long nInitialSize = (srcLen > 256) ? srcLen + 256 : 256;

	CommonConstruct(nInitialSize);

	while (src && *src)
	{
		_str[_len] = *src;
		_len++;
		src++;
		if (_len >= _max)
			resize();
	}
	if (_len >= _max)
		resize();
	_str[_len] = 0;
}



GString::GString(const char *src, int nCnt)
{
	long nInitialSize = (nCnt > 256) ? nCnt + 256 : 256;

	CommonConstruct(nInitialSize);

	_len = 0;
	if (src)
	{
		int i;
		_len = ___min(_max - 1, nCnt);
		for (i = 0; i < _len && src[i]; i++)
			_str[i] = src[i];
		_len = i;
	}

	_str[_len] = 0;
}

// constructs a string with ch repeated nCnt times
GString::GString(char ch, short nCnt)
{
	long nInitialSize = (nCnt > 256) ? nCnt + 256 : 256;

	CommonConstruct(nInitialSize);

	_len = ___min(_max - 1, nCnt);
	int i;
	for (i = 0; i < _len; i++)
		_str[i] = ch;

	_len = i;
	_str[_len] = 0;
}

GString::~GString()
{
	if (_strIsOnHeap)
		delete [] _str;
}
/*
.h declaration: GString & operator=(const strstreambuf &);

GString & GString::operator=(const strstreambuf &buf)
{
	_max = ((strstreambuf &)buf).seekoff(0, ios::cur, ios::in | ios::out) + 1;
	char *nstr = new char[_max];
	
	// not enough memory for resize
	if (!nstr)
		throw GenericException("String", 0);

	nstr[0] = 0;
	
	if (_str)
	{
		if (_strIsOnHeap)
			delete [] _str;
	}

	_str = nstr;
	_strIsOnHeap = 1;
	_initialbuf[0] = 0;

	_len = _max - 1;
	strncpy(_str, ((strstreambuf &)buf).str(), _len);
	_str[_len] = 0;
	((strstreambuf &)buf).freeze(0);

	return *this;
}*/

GString & GString::operator=(char _p)
{
	if (!_max)
	{
		_max = 2;
		resize();
	}

	_len = 0;
	_str[_len] = _p;
	_len++;
	_str[_len] = 0;

	return *this;
}

GString & GString::operator=(__int64 _p)
{
	char  szBuffer[256];
//	sprintf(szBuffer, "%I64d", _p);
	lltoa ( _p,szBuffer,10);


	int nLen = strlen(szBuffer);
	for (_len = 0; _len < nLen; _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[_len];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}

GString & GString::operator=(const char * _p)
{
	_len = 0;
	while (_p && *_p)
	{
		if (_len >= _max)
			resize();
		_str[_len] = *_p;
		_p++;
		_len++;
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}

GString & GString::operator=(const signed char * _p)
{
	*this = (const char *)_p;

	return *this;
}

GString & GString::operator=(unsigned char _p)
{
	*this = (char)_p;

	return *this;
}

GString & GString::operator=(signed char _p)
{
	*this = (char)_p;

	return *this;
}

GString & GString::operator=(short _p)
{
	char  szBuffer[25];
	sprintf(szBuffer, "%hi", _p);

	int nLen = strlen(szBuffer);
	for (_len = 0; _len < nLen; _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[_len];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}

GString & GString::operator=(unsigned short _p)
{
	char  szBuffer[25];
	sprintf(szBuffer, "%hu", _p);

	int nLen = strlen(szBuffer);
	for (_len = 0; _len < nLen; _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[_len];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}

GString & GString::operator=(int _p)
{
	char  szBuffer[25];
	sprintf(szBuffer, "%li", _p);

	int nLen = strlen(szBuffer);
	for (_len = 0; _len < nLen; _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[_len];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}

GString & GString::operator=(unsigned int _p)
{
	char  szBuffer[25];
	sprintf(szBuffer, "%lu", _p);

	int nLen = strlen(szBuffer);
	for (_len = 0; _len < nLen; _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[_len];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}

GString & GString::operator=(long _p)
{
	char  szBuffer[25];
	sprintf(szBuffer, "%li", _p);

	int nLen = strlen(szBuffer);
	for (_len = 0; _len < nLen; _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[_len];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}

GString & GString::operator=(unsigned long _p)
{
	char  szBuffer[25];
	sprintf(szBuffer, "%lu", _p);

	int nLen = strlen(szBuffer);
	for (_len = 0; _len < nLen; _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[_len];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}

GString & GString::operator=(float _p)
{
	char  szBuffer[50];
	sprintf(szBuffer, g_FloatFmt._str, _p);
	int nLen = strlen(szBuffer);

	for (_len = 0; _len < nLen; _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[_len];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}



GString & GString::operator=(double _p)
{
	char  szBuffer[50];
	sprintf(szBuffer, "%.6g", _p);
	int nLen = strlen(szBuffer);

	for (_len = 0; _len < nLen; _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[_len];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}

GString & GString::operator=(long double _p)
{
	char  szBuffer[50];
	sprintf(szBuffer, "%.6Lg", _p);
	int nLen = strlen(szBuffer);

	for (_len = 0; _len < nLen; _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[_len];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}

GString & GString::operator=(const GString & _p)
{
	for (_len = 0; _len < _p._len; _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = _p._str[_len];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}


GString & GString::operator+=(__int64 _p)
{
	char  szBuffer[25];
//	sprintf(szBuffer, "%I64d", _p);
	lltoa ( _p,szBuffer,10);

	int nLen = strlen(szBuffer);
	for (int i = 0; i < nLen; i++, _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[i];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}



GString & GString::operator+=(const signed char * _p)
{
	while (_p && *_p)
	{
		if (_len >= _max)
			resize();
		_str[_len] = *_p;
		_p++;
		_len++;
	}


	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}

GString & GString::operator+=(const char * _p)
{
	while (_p && *_p)
	{
		if (_len >= _max)
			resize();
		_str[_len] = *_p;
		_p++;
		_len++;
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}


void GString::write(const char *p,int nSize)
{
	while (_len + nSize + 1 >= _max)
		resize();
	memcpy(&_str[_len],p,nSize);
	_len += nSize;
	_str[_len] = 0;
}

GString & GString::operator+=(char _p)
{
	if (_p)
	{
		if (_len >= _max)
			resize();
		_str[_len] = _p;
		_len++;

		if (_len >= _max)
			resize();
		_str[_len] = 0;
	}

	return *this;
}

GString & GString::operator+=(unsigned char _p)
{
	if (_p)
	{
		if (_len >= _max)
			resize();
		_str[_len] = _p;
		_len++;

		if (_len >= _max)
			resize();
		_str[_len] = 0;
	}

	return *this;
}

GString & GString::operator+=(signed char _p)
{
	if (_p)
	{
		if (_len >= _max)
			resize();
		_str[_len] = _p;
		_len++;

		if (_len >= _max)
			resize();
		_str[_len] = 0;
	}

	return *this;
}

GString & GString::operator+=(short _p)
{
	char  szBuffer[25];
	sprintf(szBuffer, "%hi", _p);

	int nLen = strlen(szBuffer);
	for (int i = 0; i < nLen; i++, _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[i];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}

GString & GString::operator+=(unsigned short _p)
{
	char  szBuffer[25];
	sprintf(szBuffer, "%hu", _p);

	int nLen = strlen(szBuffer);
	for (int i = 0; i < nLen; i++, _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[i];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}

GString & GString::operator+=(int _p)
{
	char  szBuffer[25];
	sprintf(szBuffer, "%li", _p);

	int nLen = strlen(szBuffer);
	for (int i = 0; i < nLen; i++, _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[i];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}

GString & GString::operator+=(unsigned int _p)
{
	char  szBuffer[25];
	sprintf(szBuffer, "%lu", _p);

	int nLen = strlen(szBuffer);
	for (int i = 0; i < nLen; i++, _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[i];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}

GString & GString::operator+=(long _p)
{
	char  szBuffer[25];
	sprintf(szBuffer, "%ld", _p);

	int nLen = strlen(szBuffer);
	for (int i = 0; i < nLen; i++, _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[i];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}

GString & GString::operator+=(unsigned long _p)
{
	char  szBuffer[25];
	sprintf(szBuffer, "%lu", _p);
	int nLen = strlen(szBuffer);

	for (int i = 0; i < nLen; i++, _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[i];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;



	return *this;
}

GString & GString::operator+=(float _p)
{
	char  szBuffer[50];
	sprintf(szBuffer, g_FloatFmt._str, _p);
	int nLen = strlen(szBuffer);


	for (int i = 0; i < nLen; i++, _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[i];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;


	return *this;
}

GString & GString::operator+=(double _p)
{
	char  szBuffer[50];
	sprintf(szBuffer, "%.6g", _p);
	int nLen = strlen(szBuffer);


	for (int i = 0; i < nLen; i++, _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[i];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;


	return *this;
}

GString & GString::operator+=(long double _p)
{
	char  szBuffer[50];
	sprintf(szBuffer, "%.6Lg", _p);
	int nLen = strlen(szBuffer);


	for (int i = 0; i < nLen; i++, _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = szBuffer[i];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;


	return *this;
}

GString & GString::operator+=(const GString & _p)
{
	while (_len + _p._len + 1 >= _max)
		resize();
	memcpy(&_str[_len],_p._str,_p._len);
	_len += _p._len;
	_str[_len] = 0;
	return *this;
/*
	for (int i = 0; i < _p._len; i++, _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = _p._str[i];
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
*/
}

GString & GString::operator<<(__int64 _p)
{
	return *this += _p;
}

GString & GString::operator<<(const char * _p)
{
	while (_p && *_p)
	{
		if (_len >= _max)
			resize();
		_str[_len] = *_p;
		_p++;
		_len++;
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;

	return *this;
}

GString & GString::operator<<(const signed char * _p)
{
	return *this += _p;
}

GString & GString::operator<<(char _p)
{
	if (_p)
	{
		if (_len >= _max)
			resize();
		_str[_len] = _p;
		_len++;

		if (_len >= _max)
			resize();
		_str[_len] = 0;
	}
	return *this;
}

GString & GString::operator<<(unsigned char _p)
{
	return *this += _p;
}

GString & GString::operator<<(signed char _p)
{
	return *this += _p;
}

GString & GString::operator<<(short _p)
{
	return *this += _p;
}

GString & GString::operator<<(unsigned short _p)
{
	return *this += _p;
}

GString & GString::operator<<(int _p)
{
	return *this += _p;
}

GString & GString::operator<<(unsigned int _p)
{
	return *this += _p;
}

GString & GString::operator<<(long _p)
{
	return *this += _p;
}

GString & GString::operator<<(unsigned long _p)
{
	return *this += _p;
}

GString & GString::operator<<(float _p)
{
	return *this += _p;
}

GString & GString::operator<<(double _p)
{
	return *this += _p;
}

GString & GString::operator<<(long double _p)
{
	return *this += _p;
}


GString & GString::operator<<(const GString & _p)
{
//	return *this += _p;
	while (_len + _p._len + 1 >= _max)
		resize();
	memcpy(&_str[_len],_p._str,_p._len);
	_len += _p._len;
	_str[_len] = 0;
	return *this;
}

GString operator+(GString &_p1, GString &_p2)
{
	GString strRet(_p1._len + _p2._len + 1);
	int i;
	for (i = 0; i < _p1._len; strRet._len++, i++)
		strRet._str[strRet._len] = _p1._str[i];

	for (i = 0; i < _p2._len; strRet._len++, i++)
		strRet._str[strRet._len] = _p2._str[i];

	strRet._str[strRet._len] = 0;

	return strRet;
}

GString operator+(GString &_p1, const char *_p2)
{
	GString strRet;

	for (int i = 0; i < _p1._len; strRet._len++, i++)
	{
		if (strRet._len >= strRet._max)
			strRet.resize();
		strRet._str[strRet._len] = _p1._str[i];
	}

	while (_p2 && *_p2)
	{
		if (strRet._len >= strRet._max)
			strRet.resize();
		strRet._str[strRet._len] = *_p2;
		_p2++;
		strRet._len++;
	}

	if (strRet._len >= strRet._max)
		strRet.resize();
	strRet._str[strRet._len] = 0;

	return strRet;
}

GString operator+(const char *_p1, GString &_p2)
{
	GString strRet;

	while (_p1 && *_p1)
	{
		if (strRet._len >= strRet._max)
			strRet.resize();
		strRet._str[strRet._len] = *_p1;
		_p1++;
		strRet._len++;
	}

	for (int i = 0; i < _p2._len; strRet._len++, i++)
	{
		if (strRet._len >= strRet._max)
			strRet.resize();
		strRet._str[strRet._len] = _p2._str[i];
	}

	if (strRet._len >= strRet._max)
		strRet.resize();
	strRet._str[strRet._len] = 0;

	return strRet;
}

GString operator+(GString &_p1, const signed char *_p2)
{
	GString strRet;

	for (int i = 0; i < _p1._len; strRet._len++, i++)
	{
		if (strRet._len >= strRet._max)
			strRet.resize();
		strRet._str[strRet._len] = _p1._str[i];
	}

	while (_p2 && *_p2)
	{
		if (strRet._len >= strRet._max)
			strRet.resize();
		strRet._str[strRet._len] = *_p2;
		_p2++;
		strRet._len++;
	}

	if (strRet._len >= strRet._max)
		strRet.resize();
	strRet._str[strRet._len] = 0;

	return strRet;
}

GString operator+(const signed char *_p1, GString &_p2)
{
	GString strRet;

	while (_p1 && *_p1)
	{
		if (strRet._len >= strRet._max)
			strRet.resize();
		strRet._str[strRet._len] = *_p1;
		_p1++;
		strRet._len++;
	}

	for (int i = 0; i < _p2._len; strRet._len++, i++)
	{
		if (strRet._len >= strRet._max)
			strRet.resize();
		strRet._str[strRet._len] = _p2._str[i];
	}

	if (strRet._len >= strRet._max)
		strRet.resize();
	strRet._str[strRet._len] = 0;

	return strRet;
}

GString operator+(GString &_p1, char _p2)
{
	GString strRet(_p1._len + 2);

	for (int i = 0; i < _p1._len; strRet._len++, i++)
		strRet._str[strRet._len] = _p1._str[i];

	strRet._str[strRet._len] = _p2;
	strRet._len++;
	strRet._str[strRet._len] = 0;

	return strRet;
}

GString operator+(char _p1, GString &_p2)
{
	GString strRet(_p2._len + 2);

	strRet._str[strRet._len] = _p1;
	strRet._len++;

	for (int i = 0; i < _p2._len; strRet._len++, i++)
		strRet._str[strRet._len] = _p2._str[i];

	strRet._str[strRet._len] = 0;

	return strRet;
}

GString operator+(GString &_p1, unsigned char _p2)
{
	GString strRet(_p1._len + 2);

	for (int i = 0; i < _p1._len; strRet._len++, i++)
		strRet._str[strRet._len] = _p1._str[i];

	strRet._str[strRet._len] = _p2;
	strRet._len++;
	strRet._str[strRet._len] = 0;

	return strRet;
}

GString operator+(unsigned char _p1, GString &_p2)
{
	GString strRet(_p2._len + 2);

	strRet._str[strRet._len] = _p1;
	strRet._len++;

	for (int i = 0; i < _p2._len; strRet._len++, i++)
		strRet._str[strRet._len] = _p2._str[i];

	strRet._str[strRet._len] = 0;

	return strRet;
}

GString operator+(GString &_p1, signed char _p2)
{
	GString strRet(_p1._len + 2);

	for (int i = 0; i < _p1._len; strRet._len++, i++)
		strRet._str[strRet._len] = _p1._str[i];

	strRet._str[strRet._len] = _p2;
	strRet._len++;
	strRet._str[strRet._len] = 0;

	return strRet;
}

GString operator+(signed char _p1, GString &_p2)
{
	GString strRet(_p2._len + 2);

	strRet._str[strRet._len] = _p1;
	strRet._len++;

	for (int i = 0; i < _p2._len; strRet._len++, i++)
		strRet._str[strRet._len] = _p2._str[i];

	strRet._str[strRet._len] = 0;

	return strRet;
}

//ostream& operator<<(ostream &os, GString &s)
//{
//	os << s._str;
//	return os;
//}

int GString::operator >  (const GString &s) const
{
	return strcmp(_str, s._str) > 0;
}

int GString::operator >= (const GString &s) const
{
	return strcmp(_str, s._str) >= 0;
}

int GString::operator == (const GString &s) const
{
	return strcmp(_str, s._str) == 0;
}

int GString::operator <  (const GString &s) const
{
	return strcmp(_str, s._str) < 0;
}

int GString::operator <= (const GString &s) const
{
	return strcmp(_str, s._str) <= 0;
}

int GString::operator != (const GString &s) const
{
	return strcmp(_str, s._str) != 0;
}

int GString::operator >  (const char *s) const
{
	return strcmp(_str, s) > 0;
}

int GString::operator >= (const char *s) const
{
	return strcmp(_str, s) >= 0;
}

int GString::operator == (const char *s) const
{
	return strcmp(_str, s) == 0;
}

int GString::operator <  (const char *s) const
{
	return strcmp(_str, s) < 0;
}

int GString::operator <= (const char *s) const
{
	return strcmp(_str, s) <= 0;
}

int GString::operator != (const char *s) const
{
	return strcmp(_str, s) != 0;
}

int GString::Compare(const char *s) const
{
	return strcmp(_str, s);
}

int GString::Compare(GString &s) const
{
	return strcmp(_str, s._str);
}

int GString::CompareNoCase(const char *s) const
{
	int i = 0;  // start at the beginning

	// loop through the two strings comparing case insensitively
	while ((toupper(_str[i]) == toupper(s[i])) &&  // the two strings are equal
		   (_str[i] != '\0'))                      // the first hasn't ended
		i++;

	// the difference between the characters that ended it is
	// indicative of the direction of the comparison.  if this
	// is negative, the first was before the second.  if it is
	// positive, the first was after the second.  if it is zero,
	// the two are equal (and the != '\0' condition stopped the
	// loop such that the two were of equal length).
	return (short)toupper(_str[i]) - (short)toupper(s[i]);
}

int GString::CompareNoCase(const GString &s) const
{
	int i = 0;  // start at the beginning

	// loop through the two strings comparing case insensitively
	while ((toupper(_str[i]) == toupper(s._str[i])) &&  // the two strings are equal
		   (_str[i] != '\0'))							// the first hasn't ended
		i++;

	// the difference between the characters that ended it is
	// indicative of the direction of the comparison.  if this
	// is negative, the first was before the second.  if it is
	// positive, the first was after the second.  if it is zero,
	// the two are equal (and the != '\0' condition stopped the
	// loop such that the two were of equal length).
	return (short)toupper(_str[i]) - (short)toupper(s._str[i]);
}

char& GString::operator[](int nIdx)
{
	// check for subscript out of range
	if (nIdx > _len)
		throw GenericException("String", 1);

	return _str[nIdx];
}

char GString::GetAt(int nIdx) const
{
	// check for subscript out of range
	if (nIdx > _len)
		throw GenericException("String", 1);

	return _str[nIdx];
}

void GString::SetAt(int nIdx, char ch)
{
	// check for subscript out of range
	if (nIdx >= _len)
		throw GenericException("String", 1);

	_str[nIdx] = ch;
}

void GString::PadLeft(int nCnt, char ch /* = ' ' */)
{
	if (nCnt > _len)
		Prepend(nCnt - _len, ch);
}

void GString::Prepend(int nCnt, char ch /* = ' ' */)
{
	while (nCnt + _len + 1 >= _max)
		resize();

	memmove(&_str[nCnt], _str, _len);
	_len += nCnt;

	for (int i = 0; i < nCnt; i++)
		_str[i] = ch;

	_str[_len] = 0;
}

void GString::TrimLeft(char ch /* = ' ' */, short nCnt /* = -1 */)
{
	int i = 0;
	while ((i < _len) && (_str[i] == ch) && (nCnt != 0))
	{
		i++;
		nCnt--;
	}

	if (i != 0)
	{
		_len -= i;
		memmove(_str, &_str[i], _len + 1);
	}
}

#define isWS(ch) ( ch == 0x20 || ch == 0x09 || ch == 0x0D || ch == 0x0A)
void GString::TrimLeftWS()
{
	int i = 0;
	while ((i < _len) && (isWS(_str[i])))
		i++;

	if (i != 0)
	{
		_len -= i;
		memmove(_str, &_str[i], _len + 1);
	}
}

void GString::PadRight(int nCnt, char ch /* = ' ' */)
{
	if (nCnt > _len)
		Append(nCnt - _len, ch);
}

void GString::Append(int nCnt, char ch /* = ' ' */)
{
	for (int i = 0; i < nCnt; i++, _len++)
	{
		if (_len >= _max)
			resize();
		_str[_len] = ch;
	}

	if (_len >= _max)
		resize();
	_str[_len] = 0;
}

void GString::TrimRightBytes(short nCnt)
{
	if (_len >= nCnt)
		_len -= nCnt;
	_str[_len] = 0;
}

void GString::TrimRight(char ch /* = ' ' */, short nCnt /* = -1 */)
{
	while ((_len) && (_str[_len - 1] == ch) && (nCnt != 0))
	{
		_len--;
		nCnt--;
	}

	_str[_len] = 0;
}

GString GString::Left (int nCnt) const
{
	if (nCnt > _len)
		nCnt = _len;

	return GString(_str, nCnt);
}

GString GString::Mid  (int nFirst) const
{
	if (nFirst > _len)
		nFirst = _len;

	return GString(&_str[nFirst]);
}

GString GString::Mid  (int nFirst, int nCnt) const
{
	if (nFirst > _len)
		nFirst = _len;

	return GString(&_str[nFirst], nCnt);
}

GString GString::Right(int nCnt) const
{
	if (nCnt > _len)
		nCnt = _len + 1;

	int nFirst = _len - nCnt;
	return GString(&_str[nFirst]);
}

void GString::TrimRightWS()
{
	while ((_len) && (isWS(_str[_len - 1])))
		_len--;

	_str[_len] = 0;
}

void GString::MakeUpper()
{
	for (int i = 0; i < _len; i++)
		_str[i] = toupper(_str[i]);
}

void GString::MakeLower()
{
	for (int i = 0; i < _len; i++)
		_str[i] = tolower(_str[i]);
}

void GString::MakeReverse()
{
	for (int a = 0, b = _len - 1; a < b; a++, b--)
	{
		char c = _str[b];
		_str[b] = _str[a];
		_str[a] = c;
	}
}

char * stristr(const char * str1, const char * str2)
{
	char *cp = (char *) str1;
	char *s1, *s2;

	if ( !*str2 )
		return((char *)str1);

	while (*cp)
	{
		s1 = cp;
		s2 = (char *) str2;

		while ( *s1 && *s2 && !(tolower(*s1)-tolower(*s2)) )
			s1++, s2++;

		if (!*s2)
			return(cp);

		cp++;
	}
	return 0;
}

int GString::FindSubStringCaseInsensitive( const char * lpszSub, int nStart/* = 0*/ ) const
{
	char *p = stristr(&_str[nStart], lpszSub);
	if (p)
		return p - _str;
	return -1;
}

int GString::Find( char ch ) const
{
	return Find(ch, 0);
}

int GString::Find( const char * lpszSub ) const
{
	return Find(lpszSub, 0);
}

int GString::Find( char ch, int nStart ) const
{
	if (nStart >= _len)
		return -1;

	// find first single character
	const char *lpsz = strchr(_str + nStart, (int)ch);

	// return -1 if not found and index otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - _str);
}

int GString::Find( const char * lpszSub, int nStart ) const
{
	if (nStart > _len)
		return -1;

	// find first matching substring
	const char *lpsz = strstr(_str + nStart, lpszSub);

	// return -1 for not found, distance from beginning otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - _str);
}

void GString::Insert( int nIndex, char ch )
{
	// subscript out of range
	if (nIndex > _len)
		throw GenericException("String", 1);

	if (_len + 2 >= _max)
		resize();

	_len++;
	memmove(&_str[nIndex + 1], &_str[nIndex], _len - nIndex);
	_str[nIndex] = ch;
	_str[_len] = 0;
}

void GString::Insert( int nIndex, const char * pstr )
{
	if (!pstr)
		return;

	// subscript out of range
	if (nIndex > _len)
		throw GenericException("String", 1);

	int nCnt = strlen(pstr);
	while (_len + nCnt + 1 >= _max)
		resize();

	_len += nCnt;
	memmove(&_str[nIndex + nCnt], &_str[nIndex], _len - nIndex);

	while (*pstr)
	{
		_str[nIndex] = *pstr;
		pstr++;
		nIndex++;
	}

	_str[_len] = 0;
}

void GString::MergeMask(const char *szSrc, const char *szMask)
{
	Empty();

	if ((szSrc != 0) && (*szSrc != 0))
	{
		while ((*szSrc != 0) || (*szMask != 0))
		{
			switch (*szMask)
			{
				// copy a single character from the 
				case '_' :
					if (*szSrc)
					{
						*this += *szSrc;
						szSrc++;
					}
					szMask++;
					break;
				// copy the remaining characters from szSrc
				case '*' :
					*this += szSrc;
					while (*szSrc)
						szSrc++;
					szMask++;
					break;
				// all other characters are mask chars
				// with the exception of '\' which is
				// used to escape '_', '*' and '\'
				default  :
					if (*szMask != 0)
					{
						if (*szMask == '\\')
							// skip the escape character
							szMask++;
						if (*szMask != 0)
						{
							*this += *szMask;
							szMask++;
						}
					}
					break;
			}

			// throw away the remaining characters in the source string
			if (*szMask == 0)
			break;
		}
	}
}

bool IsNaN(const char *szN, 
		   char decimal_separator,
		   char grouping_separator,
		   char minus_sign)
{
	bool bRet = false;

	bool bDot = false;
	bool bNeg = false;
	int i = 0;
	while ((*szN) && (!bRet))
	{
		bRet = !(((*szN >= '0') && (*szN <= '9')) || 
				   *szN == grouping_separator || 
				   *szN == decimal_separator ||
				   *szN == minus_sign);

		if (*szN == decimal_separator)
		{
			if (bDot)
				bRet = true; // NaN
			bDot = true;
		}

		if (*szN == minus_sign)
		{
			if (((i) && szN[1]) || bNeg)
				bRet = true; // NaN
			bNeg = true;
		}

		szN++;
		i++;
	}

	return bRet;
}

short CountOf(const char *szN, char zero_digit)
{
	int nRet = 0;

	while (*szN)
	{
		if (*szN == zero_digit)
			nRet++;

		szN++;
	}

	return nRet;
}

long round(const char *value)
{
	long dres = (long)atof(value);
	double drem = atof(value);
	drem -= dres;
	short nAdd = 1;
	if (drem < 0) drem *= -1, nAdd *= -1;
	if (drem >= .5) dres += nAdd;

	return dres;
}

void GString::FormatNumber(const char *szFormat, 
						   char decimal_separator,
						   char grouping_separator,
						   char minus_sign,
						   const char *NaN,
						   char zero_digit,
						   char digit,
						   char pattern_separator,
						   char percent,
						   char permille)
{
	if (szFormat && *szFormat)
	{
		// make sure that the string is a number {0..9, '.', '-'}
		// if the string contains a character not in the number
		// subset then set the value of the string to NaN.
		const char *szValue = _str;
		if (IsNaN(szValue, '.', ',', '-'))
			*this = NaN;
		else
		{
			// it's a number, get the whole part and the fraction part
			int nIdx = Find('.');
			GString strWhole;
			strWhole = (nIdx == -1) ? _str : (const char *)Left(nIdx);
			GString strFraction('0', (short)1);
			nIdx = Find('.') + 1;
			if (nIdx > 0)
				strFraction = Mid(nIdx);
			bool bIsNeg = (Find(minus_sign) != -1);

			long nWhole = abs(atol((const char *)strWhole));
			long nFract = abs(atol((const char *)strFraction));

			// check for % and ?
			if (percent == szFormat[0])
			{
				double d = atof(_str);
				d *= 100;
				GString strTemp;
				strTemp.Format("%f", d);
				nIdx = strTemp.Find('.');
				strFraction = (nIdx == -1) ? strTemp._str : (const char *)strTemp.Left(nIdx);
				nWhole = atol((const char *)strFraction);
				nFract = 0;
			}
			if (permille == szFormat[0])
			{
				double d = atof(_str);
				d *= 1000;
				GString strTemp;
				strTemp.Format("%f", d);
				nIdx = strTemp.Find('.');
				strFraction = (nIdx == -1) ? strTemp._str : (const char *)strTemp.Left(nIdx);
				nWhole = atol((const char *)strFraction);
				nFract = 0;
			}

			// if the number is negative, get the negative pattern out of the format.
			// if a negative pattern doesn't exist, the minus_sign will be prepended
			// to the positive pattern.
			GString strFormat(szFormat);
			nIdx = strFormat.Find(pattern_separator);
			if (bIsNeg)
			{
				if (nIdx != -1)
					strFormat = strFormat.Mid(nIdx + 1);
				else
					strFormat.Format("%c%s", minus_sign, (const char *)strFormat);
			}
			else
			{
				if (nIdx != -1)
					strFormat = strFormat.Left(nIdx);
			}

			GString strFormatWhole(strFormat);
			GString strFormatDecimal('#', (short)1);

			// determine the number of digits per group
			int nGroup = 0;
			nIdx = strFormat.Find(',');
			if (nIdx != -1)
			{
				nIdx++;
				int nNext = strFormat.Find(',', nIdx);
				if (nNext == -1)
					nNext = strFormat.Find('.', nIdx);
				if (nNext == -1)
					nNext = strFormat.Length();
				nGroup = (nNext - nIdx);
			}

			// determine the number of decimals to display
			int nDecimals = 0;
			nIdx = strFormat.Find('.');
			if ((nIdx != -1) && 
				(percent != szFormat[0]) &&
				(permille != szFormat[0]))
			{
				if (nGroup)
					strFormatWhole = strFormat.Mid(nIdx - nGroup, nGroup);
				else
					strFormatWhole = strFormat.Left(nIdx);
				nIdx++;
				strFormatDecimal = strFormat.Mid(nIdx);
				nDecimals = (strFormat.Length() - nIdx);
			}

			// Format the whole part of the number
			int nCount = CountOf((const char *)strFormatWhole, zero_digit);
			strWhole.Format("%0ld", nWhole);
			if (strWhole.Length() < nCount)
			{
				GString temp(zero_digit, (short)(nCount - (short)strWhole.Length()));
				strWhole.Format("%s%s", (const char *)temp, (const char *)strWhole);
			}

			Empty();

			// add all prefix characters
			nIdx = 0;
			const char *szP = (const char *)strFormat;
			while (*szP)
			{
				if (*szP == digit ||
					*szP == zero_digit ||
					*szP == decimal_separator ||
					*szP == grouping_separator ||
					*szP == percent ||
					*szP == permille)
					break;

				szP++;
				nIdx++;
			}
			strFormat = strFormat.Left(nIdx);

			strFormat.MakeReverse();

			int i, j;
			for (i = 0, j = strWhole.Length() - 1; j >= 0; j--, i++)
			{
				if ((nGroup) && (i == nGroup))
				{
					*this += grouping_separator;
					i = 0;
				}

				*this += strWhole[j];
			}
			*this += strFormat;

			MakeReverse();

			if (nDecimals)
			{
				*this += decimal_separator;

				strFraction.Format("%ld", nFract);
				const char *szF1 = (const char *)strFormatDecimal;
				const char *szF2 = (const char *)strFraction;
				i = 0;
				while (*szF1)
				{
					if (*szF2)
						*this += *szF2;
					else if (*szF1 == zero_digit)
						*this += zero_digit;
					else if (*szF1 != digit) // add all sufix characters
						*this += *szF1;
					if (*szF2)
						szF2++;
					if (*szF1)
						szF1++;
				}
			}

			if (percent == szFormat[0])
				*this += percent;
			if (permille == szFormat[0])
				*this += permille;
		}
	}
}

#define FORCE_ANSI      0x10000
#define FORCE_UNICODE   0x20000
#define FORCE_INT64     0x40000

void GString::FormatV(const char *lpszFormat, va_list argList)
{
	va_list argListSave = argList;

	// make a guess at the maximum length of the resulting string
	int nMaxLen = 0;
	for (const char *lpsz = lpszFormat; *lpsz != '\0'; lpsz++)
	{
		// handle '%' character, but watch out for '%%'
		if (*lpsz != '%' || *(lpsz = lpsz + 1) == '%')
		{
			nMaxLen += strlen(lpsz);
			continue;
		}

		int nItemLen = 0;

		// handle '%' character with format
		int nWidth = 0;
		for (; *lpsz != '\0'; lpsz++)
		{
			// check for valid flags
			if (*lpsz == '#')
				nMaxLen += 2;   // for '0x'
			else if (*lpsz == '*')
				nWidth = va_arg(argList, int);
			else if (*lpsz == '-' || *lpsz == '+' || *lpsz == '0' ||
				*lpsz == ' ')
				;
			else // hit non-flag character
				break;
		}
		// get width and skip it
		if (nWidth == 0)
		{
			// width indicated by
			nWidth = atoi(lpsz);
			for (; *lpsz != '\0' && isdigit(*lpsz); lpsz++)
				;
		}
		(nWidth >= 0);

		int nPrecision = 0;
		if (*lpsz == '.')
		{
			// skip past '.' separator (width.precision)
			lpsz++;

			// get precision and skip it
			if (*lpsz == '*')
			{
				nPrecision = va_arg(argList, int);
				lpsz++;
			}
			else
			{
				nPrecision = atoi(lpsz);
				for (; *lpsz != '\0' && isdigit(*lpsz); lpsz++)
					;
			}
			(nPrecision >= 0);
		}

		// should be on type modifier or specifier
		int nModifier = 0;
		if (strncmp(lpsz, "I64", 3) == 0)
		{
			lpsz += 3;
			nModifier = FORCE_INT64;
		}
		else
		{
			switch (*lpsz)
			{
			// modifiers that affect size
			case 'h':
				nModifier = FORCE_ANSI;
				lpsz++;
				break;
			case 'l':
				nModifier = FORCE_UNICODE;
				lpsz++;
				break;

			// modifiers that do not affect size
			case 'F':
			case 'N':
			case 'L':
				lpsz++;
				break;
			}
		}

		// now should be on specifier
		switch (*lpsz | nModifier)
		{
		// single characters
		case 'c':
		case 'C':
			nItemLen = 2;
#ifdef WIN32 
			va_arg(argList, char);
#else
			// gcc says: "`char' is promoted to `int' when passed through ...
			// so we pass 'int' not 'char' to va_arg"
			va_arg(argList, int);
#endif
			break;

		// strings
		case 's':
			{
				const char *pstrNextArg = va_arg(argList, const char *);
				if (pstrNextArg == NULL)
				   nItemLen = 6;  // "(null)"
				else
				{
				   nItemLen = strlen(pstrNextArg);
				   nItemLen = ___max(1, nItemLen);
				}
			}
			break;

		case 's'|FORCE_ANSI:
		case 'S'|FORCE_ANSI:
			{
				const char * pstrNextArg = va_arg(argList, const char *);
				if (pstrNextArg == NULL)
				   nItemLen = 6; // "(null)"
				else
				{
				   nItemLen = strlen(pstrNextArg);
				   nItemLen = ___max(1, nItemLen);
				}
			}
			break;
		}
		// adjust nItemLen for strings
		if (nItemLen != 0)
		{
			if (nPrecision != 0)
				nItemLen = ___min(nItemLen, nPrecision);
			nItemLen = ___max(nItemLen, nWidth);
		}
		else
		{
			switch (*lpsz)
			{
			// integers
			case 'd':
			case 'i':
			case 'u':
			case 'x':
			case 'X':
			case 'o':
				if (nModifier & FORCE_INT64)
					va_arg(argList, __int64);
				else
					va_arg(argList, int);
				nItemLen = 32;
				nItemLen = ___max(nItemLen, nWidth+nPrecision);
				break;

			case 'e':
			case 'g':
			case 'G':
				va_arg(argList, double);
				nItemLen = 128;
				nItemLen = ___max(nItemLen, nWidth+nPrecision);
				break;

			case 'f':
				{
					double f;
					// 312 == strlen("-1+(309 zeroes).")
					// 309 zeroes == max precision of a double
					// 6 == adjustment in case precision is not specified,
					//   which means that the precision defaults to 6
					char *pszTemp = new char[___max(nWidth, 312+nPrecision+6)];

					f = va_arg(argList, double);
					sprintf( pszTemp,  "%*.*f", nWidth, nPrecision+6, f );
					nItemLen = strlen(pszTemp);

					delete [] pszTemp;
				}
				break;

			case 'p':
				va_arg(argList, void*);
				nItemLen = 32;
				nItemLen = ___max(nItemLen, nWidth+nPrecision);
				break;

			// no output
			case 'n':
				va_arg(argList, int*);
				break;
			}
		}

		// adjust nMaxLen for output nItemLen
		nMaxLen += nItemLen;
	}

	if (nMaxLen + 1 > _max)
	{
		if (_strIsOnHeap)
			delete _str;
		_max = nMaxLen + 1;
		_str = new char[_max];
		_strIsOnHeap = 1;
		_initialbuf[0] = 0;
	}

	_len = vsprintf(_str, lpszFormat, argListSave);

	va_end(argListSave);

	if (_len < 0)
	{
		_len = 0;
		_str[_len] = 0;
	}
}

// Load an error from the active ErrorProfile() owned by GException.cpp
void GString::LoadResource(const char* szSystem, int error, ...)
{
	int nerror = error;

	GProfile &ErrorProfile = GetErrorProfile();
	int nsubSystem = atoi(ErrorProfile.GetString(szSystem, "SubSystem"));

	GString strKey;
	strKey.Format("%ld", error);
	strKey = ErrorProfile.GetString(szSystem, (const char *)strKey);

	va_list argList;
	va_start(argList, error);
	FormatV((const char *)strKey, argList);
	va_end(argList);

}


void GString::Format(const char *lpszFormat, ...)
{
	va_list argList;
	va_start(argList, lpszFormat);
	FormatV(lpszFormat, argList);
	va_end(argList);
}

int GString::ToFileAppend(const char* pzFileName, bool bThrowOnFail /*= 1*/)
{
	FILE *fp = fopen(pzFileName,"a");
	if (fp)
	{
		fwrite(_str,1,_len,fp);
		fclose(fp);
	}
	else
	{
		// the file could not be opened for writing
		if (bThrowOnFail)
			throw GenericException("String", 3, pzFileName);
		
		// fail
		return 0;
	}
	// success
	return 1;
}

int GString::ToFile(const char* pzFileName, bool bThrowOnFail/* = 1*/)
{
	FILE *fp = fopen(pzFileName,"wb");
	if (fp)
	{
		fwrite(_str,1,_len,fp);
		fclose(fp);
	}
	else
	{
		// the file could not be opened for writing
		if (bThrowOnFail)
			throw GenericException("String", 3, pzFileName);
		
		// fail
		return 0;
	}
	// success
	return 1;
}

int GString::FromFile(const char* pzFileName, bool bThrowOnFail/* = 1*/)
{
	FILE *fp = fopen(pzFileName,"rb");
	if (fp)
	{
		// get the size of the file
		fseek(fp,0,SEEK_END);
		long lBytes = ftell(fp);
		fseek(fp,0,SEEK_SET);
		
		if (_strIsOnHeap)
			delete [] _str;

		// pre-alloc the GString
		_str = new char[lBytes + 1];
		_strIsOnHeap = 1;
		_initialbuf[0] = 0;

		int n = fread(_str,sizeof(char),lBytes,fp);
		_len = lBytes;
		_str[_len] = 0;
		fclose(fp);
	}
	else  
	{
		// the file could not be opened.
		if (bThrowOnFail)
			throw GenericException("String", 2, pzFileName);
		
		// fail
		return 0;
	}
	// success
	return 1;
}

/*
int GString::GStringFromFile(GString &strDest, 
							 const char* pzFileName, 
							 int nMode)
{
	fstream fs(pzFileName, nMode);
 	if (fs.good())
	{
		fs.seekg( 0, ios::end );
		long lFileBytes = fs.tellg();
		fs.seekg( 0, ios::beg );

		if (strDest._strIsOnHeap)
			delete [] strDest._str;
		strDest._str = new char[lFileBytes + 1];
		strDest._strIsOnHeap = 1;
		strDest._initialbuf[0] = 0;

		fs.read(strDest._str, lFileBytes);
		strDest._len = lFileBytes;
		strDest._str[strDest._len] = 0;
	}
	else
	{
		return 0; // Failed to open input file
	}
	return 1; // success
}

int GString::GStringFromFile(ostream &strDest, 
							 const char* pzFileName,
							 int nMode)
{
	fstream fs(pzFileName, nMode);
 	if (fs.good())
	{
		fs.seekg( 0, ios::end );
		long lFileBytes = fs.tellg();
		fs.seekg( 0, ios::beg );

		char *buf = new char[lFileBytes];
		fs.read(buf,lFileBytes);

		if (strDest.tellp())
			strDest.seekp(strDest.tellp() - 1);
	
		strDest.write(buf,lFileBytes);
		delete buf;

		strDest << ends;
	}
	else
	{
		return 0; // Failed to open input file
	}
	return 1; // success
}
*/

void GString::ReplaceCaseInsensitive( const char * szWhat, const char *szRep, int nStart/* = 0*/  )
{
	if ((!szWhat) || (!szRep))
		return;

	int nPos = FindSubStringCaseInsensitive(szWhat, nStart);
	int nLen = strlen(szRep);

	while (nPos != -1)
	{
		Remove(nPos, strlen(szWhat));
		if (nLen)
			Insert(nPos, szRep);

		nPos += nLen;

		nPos = FindSubStringCaseInsensitive(szWhat, nPos);
	}
}


void GString::Replace( const char * szWhat, const char *szRep )
{
	if ((!szWhat) || (!szRep))
		return;

	int nPos = Find(szWhat);
	int nLen = strlen(szRep);

	while (nPos != -1)
	{
		Remove(nPos, strlen(szWhat));
		if (nLen)
			Insert(nPos, szRep);

		nPos += nLen;

		nPos = Find(szWhat, nPos);
	}
}

void GString::Replace( char chWhat, char chRep )
{
	char szWhat[2];
	szWhat[0] = chWhat;
	szWhat[1] = 0;

	char szRep[2];
	szRep[0] = chRep;
	szRep[1] = 0;

	Replace( (const char *)szWhat, (const char *)szRep );
}

void GString::Replace( const char * szWhat, char chRep )
{
	char szRep[2];
	szRep[0] = chRep;
	szRep[1] = 0;

	Replace( (const char *)szWhat, (const char *)szRep );
}

void GString::Replace( char chWhat, const char *szRep )
{
	char szWhat[2];
	szWhat[0] = chWhat;
	szWhat[1] = 0;

	Replace( (const char *)szWhat, (const char *)szRep );
}

void GString::Remove( int nStart, int nLen )
{
	// subscript out of range
	if (nStart + nLen > _len)
		throw GenericException("String", 1);

	memmove(&_str[nStart], &_str[nStart + nLen], _len - (nStart + nLen));
	_len -= nLen;
	_str[_len] = 0;
}

void GString::StripQuotes()
{
	if ((_len >= 2) && (_str[0] == _str[_len - 1]))
	{
		if ((_str[0] == '\'') || (_str[0] == '"'))
		{
			memmove(&_str[0], &_str[1], _len - 2);
			_len -= 2;
			_str[_len] = 0;
		}
	}
}


void GString::NormalizeSpace()
{
	int nBegWS = -1;
	int nEndWS = -1;

	for (int i = 0; i < _len; i++)
	{
		if (isWS(_str[i]))
		{
			nEndWS = i;
			if (nBegWS == -1)
				nBegWS = i;
		}
		else if ((nBegWS != -1) && 
				 (nEndWS != -1))
		{
			nEndWS++;
			Remove(nBegWS, nEndWS - nBegWS);
			Insert(nBegWS, ' ');
			i = nBegWS;
			nBegWS = -1;
			nEndWS = -1;
		}
		else
		{
			nBegWS = -1;
			nEndWS = -1;
		}
	}

	if ((nBegWS != -1) && 
		(nEndWS != -1))
	{
			nEndWS++;
			Remove(nBegWS, nEndWS - nBegWS);
			Insert(nBegWS, ' ');
	}

	TrimLeftWS();
	TrimRightWS();
}

void GString::Translate(const char *szConvert, const char *szTo)
{
	if ((szConvert) && (szTo))
	{
		int nTranslate = ___min(strlen(szConvert), strlen(szTo));
		for (int i = 0; i < _len; i++)
		{
			for (int j = 0; j < nTranslate; j++)
			{
				if (_str[i] == szConvert[j])
					_str[i] =  szTo[j];
			}
		}
	}
}


// replace each char in pzReplaceChars with %nn where
// nn is a two byte hex value of the byte that was replaced.
// example: GString s("ABC");  
//			s.EscapeWithHex("XYZB");
//			s == "A%42C"
//	42 is hex for 66 that is the ASCII of a B
//
// example: GString s("A\nC");  
//			s.EscapeWithHex("\r\n\t");
//			s == "A%0AC"
//
const char *GString::EscapeWithHex(const char *pzEscapeChars)
{
	GString strDestrination;
	GString strEscapeChars(pzEscapeChars);
	char *pSrc = (char *)(const char *)(*this);
	int nSourceBytes = strlen(pSrc);
	
	for(int i=0;i < nSourceBytes; i++)
	{
		// escape special chars
		if ( strEscapeChars.Find(pSrc[i]) > -1 )
		{
			char buf[20];
			sprintf(buf,"%%%02X",pSrc[i]);
			strDestrination += buf;
		}
		else 
		{
			strDestrination += pSrc[i];
		}
	}
	*this = strDestrination;
	return *this;
}

// reverse of GString::EscapeWithHex()
void GString::UnEscapeHexed()
{
	const char *pSource = *this;
	char *pDest = new char [strlen(pSource) + 1];

	char *pszQuery = (char *)pSource;
	char *t = pDest;
	while (*pszQuery)
	{
		switch (*pszQuery)
		{
		case '%' :
			{
				pszQuery++;
				char chTemp = pszQuery[2];
				pszQuery[2] = 0;
				char *pStart = pszQuery;
				char *pEnd;
				*t = (char)strtol(pStart, &pEnd, 16);
				pszQuery[2] = chTemp;
				pszQuery = pEnd;
				t++;
				continue;
			}
			break;
		default :
			*t = *pszQuery;
			break;
		}
		pszQuery++;
		t++;
	}
	*t = 0;
	*this = pDest;
	delete pDest;
}
