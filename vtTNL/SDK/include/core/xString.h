#ifndef _XSTRING_H__
#define _XSTRING_H__

#include <tchar.h>
#include <wchar.h>
#include <stdlib.h>
#include <string>


typedef char		TAnsiChar;

#ifdef _WIN32
	typedef wchar_t TUnicodeChar;
#else
	typedef unsigned short TUnicodeChar;
#endif


//
// Define an alias for the native character data type
//
#ifdef _UNICODE
	typedef TUnicodeChar		TChar;
#else
	typedef TAnsiChar		TChar;
#endif

//
// This macro creates an ASCII string (actually a no-op)
//
#define __xA(x) (x)

//
// This macro creates a Unicode string by adding the magic L before the string
//
#define __xU(x) (L##x)

//
// Define an alias for building a string in the native format
//
#ifdef _UNICODE
#define __xT __xU
#else
#define __xT __xA
#endif


class xString  
{
	
public:

//	typedef xString uniStringPtr;
	//=======================================================
	//
	// Construction and destruction
	//
	

	//=======================================================
	//
	// Accessors
	//
	TChar *GetString();
	const TAnsiChar *CStr() const;
	
	int GetLength()const;
	bool IsNull() const;
	bool IsValidIndex(const int Index)const ;

	//=======================================================
	//
	// Regular transformation functions
	//	
	int Compare(const xString& ins, bool IgnoreCase = false)const; 

	bool Find(xString& Str, int& Pos, bool IgnoreCase=false)	const	;
	void Delete(int Pos, int Count);
	void Insert(int Pos, TChar c);
	void Insert(int Pos, const xString& input);	
	xString GetSubString(int Start, int Count, xString& Dest);		// Crops out a substring.
	
	//=======================================================
	//
	// Special transformation functions
	//
	void VarArg(TChar *Format, ...);
	void EatLeadingWhitespace();
	void EatTrailingWhitespace()	;

	 
	//=======================================================
	//
	// Conversion functions
	//
	TAnsiChar *ToAnsi(TAnsiChar *Buffer, int BufferLen) const;
	TUnicodeChar *ToUnicode(TUnicodeChar *Buffer, int BufferLen) const;
	int ToInt() const;
	//=======================================================
	

	xString();
	xString(const xString& input);
	xString(const TUnicodeChar *input);
	xString(int size)
	{

		Size = size;
		Len = size -1;
		Text = AllocStr(Size);
	}

	virtual ~xString();
	//=======================================================
	//
	// Concatenation operators
	//
		// Concatenates two strings (see text)

	
	friend xString operator +(const xString& Str1, const xString& Str2);	// Concatenates two strings (see text)
	
	/*
	xString& operator +(const TChar& Str,const xString& Str1);
	xString& operator +(const char left[],const xString &right);
	xString& operator +(const xString &right,const char left[]);
	xString& operator +(const xString &right);
	*/
	
	/*
	xString& operator +=(const xString& String)		// Adds another string to the end of the
	{															// current one.
		if (String.Len > 0)
		{
			Grow(String.Len);
			_tcsncpy(&Text[Len], String.Text, String.Len);
			Len += String.Len;
		}
	}*/

	

	//xString& operator +=(const char input[]);

	void AssignFrom(const TAnsiChar *String)	// Constructor building a Unicode string from a regular C
	{											// string in ANSI format (used for doing conversions).
		if (String == NULL)						// Early out if string is NULL
		{
			Text = NULL;
			Size = Len = 0;
			return;
		}
		Size = strlen(String) + 1;				// Use ANSI strlen function
		Len = Size-1;
		Text = AllocStr(Size);
		for (int i=0 ; i<Len ; i++)			// Convert ANSI->Unicode char-by-char
			Text[i] = (TUnicodeChar)String[i];
		Text[Len] = __xA('\0');
	}

	xString& operator +=(const xString& String)		// Adds another string to the end of the
	{	
		EatLeadingWhitespace();
		EatTrailingWhitespace();
        
		//Insert(Len,String);
		

		int len2  = strlen(CStr()); 
		std::string a1;
		a1+=CStr();
		a1+= String.CStr();
		FreeStr(Text);
		AssignFrom(a1.c_str());

        return *this;

		// current one.
		if (String.Len > 0)
		{
			Grow(String.Len);
			strncpy(&Text[Len], String.Text, 2);
			Len += String.Len;
		}
		return *this;
	}
	
	/*xString& operator +=(const char* String)		// Adds another string to the end of the
	{
	
		return *this << xString(String);
	}*/
	
	//=======================================================
	//
	// Access operators
	//
	TAnsiChar*Str();
	operator TAnsiChar*()
	{
	
		/*TAnsiChar*out  =new TAnsiChar[Size];
		ToAnsi(out,Size);
		return out ?  out : NULL ;*/
	   return Text;

	}
	operator TUnicodeChar *() const;
	TChar& xString::operator [](int Pos);

	//
	// Assignment operators
	//
	void operator =(const xString& String)		// Sets this string to the contents of another string.
	{
		AssignFrom(String);
	}
	
#ifndef _UNICODE
	void operator =(const TAnsiChar *String)	// Sets this string to the contents of a character array
	{														// in ANSI format (only included in Unicode builds)
		if (String == NULL)
		{
			Text = NULL;
			Size = Len = 0;
			return;
		}
		Size = strlen(String) + 1;			// Use ANSI strlen function
		Len = Size-1;
		Text = AllocStr(Size);
		for (int i=0 ; i<Len ; i++)		// Convert ANSI->Unicode char-by-char
			Text[i] = (TUnicodeChar)String[i];
		Text[Len] = __xA('\0');
	}
	xString(const TAnsiChar *String)	// Constructor building a Unicode string from a regular C
	{											// string in ANSI format (used for doing conversions).
		if (String == NULL)						// Early out if string is NULL
		{
			Text = NULL;
			Size = Len = 0;
			return;
		}
		Size = strlen(String) + 1;				// Use ANSI strlen function
		Len = Size-1;
		Text = AllocStr(Size);
		for (int i=0 ; i<Len ; i++)			// Convert ANSI->Unicode char-by-char
			Text[i] = (TUnicodeChar)String[i];
		Text[Len] = __xA('\0');
	}
#else
	void operator =(const TUnicodeChar *String)	// Sets this string to the contents of a character array
	{															// in Unicode format (only included in ANSI builds)
		if (String == NULL)
		{
			Text = NULL;
			Size = Len = 0;
			return;
		}
		Size = wcslen(String) + 1;		// Use Unicode wcslen function
		Len = Size-1;
		Text = AllocStr(Size);
		for (int i=0 ; i<Len ; i++)		// Convert Unicode->ANSI char-by-char
			Text[i] = (TAnsiChar)String[i];
		Text[Len] = __xA('\0');
	}
	
#endif



	
	//=======================================================
	//
	// Comparison operators (operates through Compare()).
	// Functions exist for comparing both string objects and character arrays.
	//
	
	bool operator < (const xString& Str) const	{ return (bool)(Compare(Str) == -1);				}
	bool operator > (const xString& Str) const	{ return (bool)(Compare(Str) == 1);					}
	bool operator <=(const xString& Str) const	{ return (bool)(Compare(Str) != 1);					}
	bool operator >=(const xString& Str) const	{ return (bool)(Compare(Str) != -1);				}
	bool operator ==(const xString& Str) const	{ return (bool)(Compare(Str) == 0);					}
	bool operator !=(const xString& Str) const	{ return (bool)(Compare(Str) != 0);					}
	bool operator < (const TChar *Chr) const		{ return (bool)(Compare(xString(Chr)) == -1);	}
	bool operator > (const TChar *Chr) const		{ return (bool)(Compare(xString(Chr)) == 1);		}
	bool operator <=(const TChar *Chr) const		{ return (bool)(Compare(xString(Chr)) != 1);		}
	bool operator >=(const TChar *Chr) const		{ return (bool)(Compare(xString(Chr)) != -1);	}
	bool operator ==(const TChar *Chr) const		{ return (bool)(Compare(xString(Chr)) == 0);		}
	bool operator !=(const TChar *Chr) const		{ return (bool)(Compare(xString(Chr)) != 0);		}
	
	// Concatenation operator
	xString& operator << (const TChar* rValue);

	void _attachRight(const TCHAR*rValue);
	// Concatenation operator
	xString& operator << (const char iValue);

	// Concatenation operator
	xString& operator << (const int iValue);

	// Concatenation operator
	//xString& operator << (const signed iValue);

	// Concatenation operator
	xString& operator << (const unsigned int iValue);

	// Concatenation operator
	xString& operator << (const float iValue);

	// Concatenation operator
	xString& operator << (const void* iValue);
	//=======================================================
	//
	// Protected low-level functions
	//

	void AssignFrom(const xString& Str);
	void AssignFrom(const TUnicodeChar *Str);
	
	
	
protected:
	void Optimize();
	void Grow(int Num);
	

	static TChar *AllocStr(int Size);
	static void FreeStr(TChar *Ptr);
	//=======================================================
	//
	// Protected data members
	//
//protected:
	TChar						*Text;			// The actual character array
	int						Size;				// Number of bytes allocated for string
	int						Len;				// Number of characters in string

	

};


#endif