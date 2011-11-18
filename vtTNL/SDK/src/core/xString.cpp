#include "pch.h"
#include "xString.h"

xString::xString()								// Default constructor, just creates an empty string.
{
	Text = NULL;
	Len = Size = 0;
}
xString::xString(const xString& input)	// Copy constructor, makes an instance an exact copy of
{											// another string.
	AssignFrom(input);
}
	
#ifdef _UNICODE

#include <wchar.h>

xString::xString(const TUnicodeChar *input)		// Constructor building an ANSI string from a regular C
{													// string in Unicode format (used for doing conversions).
	if (input == NULL)						// Early out if string is NULL
	{
		Text = NULL;
		Size = Len = 0;
		return;
	}
	Size = wcslen(input) + 1;				// Use Unicode wcslen function
	Len = Size-1;
	Text = AllocStr(Size);
	for (int i=0 ; i<Len ; i++)			// Convert Unicode->ANSI char-by-char
		Text[i] = (TAnsiChar)input[i];
	Text[Len] = __xA('\0');
}

#endif

int 
xString::GetLength()const	// Returns the length of the string currently
{										// held by an instance of the string class.
	return Len;
}
xString::~xString()						// Destructor. The use of virtual is recommended in C++
{												// for all destructors that can potentially be overloaded.
	if (Size && Text != NULL)			// Free the memory used by the string
		FreeStr(Text);
}
//=======================================================
//
// Accessors
//
TChar*
xString::GetString()		// Returns a pointer to the actual string data.
{
	return Text;
}

const TAnsiChar*
xString::CStr() const		// Returns a pointer to the actual string data.
{
	return Text  ? Text : NULL;
}


bool 
xString::IsNull()const		// Returns true if a NULL string is held by an instance.
{
	return (Text == NULL || !_tcslen(Text));
}
bool 
xString::IsValidIndex(const int Index)const	// Returns true if the character index
{																// specified is within the bounds of the string.
	return (Index >= 0 && Index < Len);
}

//=======================================================
//
// Regular transformation functions
//	
int 
xString::Compare(const xString&ins, bool IgnoreCase) const
{	

	if (IsNull() && !ins.IsNull())
		return 1;
	else if (!IsNull() && ins.IsNull())
		return -1;
	else if (IsNull() && ins.IsNull())
		return 0;
	if (IgnoreCase)
		return _tcsicmp(Text, ins.Text);
	else
		return _tcscmp(Text, ins.Text);

}
bool xString::Find(xString&Str, int& Pos, bool IgnoreCase)const	// Finds a substring within this string.
{																					// Returns true if found (position in Pos).
	if (IsNull() || Str.IsNull())
		return false;

	// Define a function pointer that will be used to call the appropriate
	// compare function based on the value of IgnoreCase.
	int (* cmpfn)(const TCHAR*, const TCHAR*, size_t) = (IgnoreCase) ? _tcsnicmp : _tcsncmp;
	
	for (Pos=0 ; Pos<=(Len-Str.Len) ; Pos++)
	{
		if (cmpfn(&Text[Pos], Str.Text, Str.Len) == 0)
			return true;
	}

	return false;
}
void xString::Delete(int Pos, int Count)						// Deletes the specified number of characters,
{																// starting at the specified position.
	if (Pos > Len || Count==0)							// Start is outside string or nothing to delete?
		return;
	if (Pos + Count > Len)								// Clamp Count to string length
		Count = (Len - Pos);

	// Characters are deleted by moving up the data following the region to delete.
	for (int i=Pos ; i<Len-Count ; i++)
		Text[i] = Text[i+Count];

	Len -= Count;
	Text[Len] = __xT('\0');
	Optimize();
}
void xString::Insert(int Pos, TChar c)			// Inserts a character at the given position in the string.
{
	if (Pos<0 || Pos>Len)
		return;

	Grow(1);									// Grow the string by one byte to be able to hold character

	// Move down rest of the string.
	// Copying overlapping memory blocks requires the use of memmove() instead of memcpy().
	memmove((void *)&Text[Pos+1], (const void *)&Text[Pos], Len-Pos);
	Text[Pos] = c;
	Text[++Len] = __xT('\0');
}
void xString::Insert(int Pos, const xString& input)		// Inserts a complete string at the given
{																// location.
	if (Pos<0 || Pos>Len || input.IsNull())
		return;

	TChar *New = AllocStr(input.Len + Len + 1);
	if (Pos > 0)									// Set the string portion before the inserted string
		_tcsncpy(New, Text, Pos);
	_tcsncpy(&New[Pos], input.Text, input.Len);			// Insert the string
	if (Len-Pos > 0)													// Insert rest of orignal string
		_tcsncpy(&New[Pos+input.Len], &Text[Pos], Len-Pos);

	AssignFrom(New);							// Copy new string back into stringobject
}
xString 
xString::GetSubString(int Start, int Count, xString& Dest)		// Crops out a substring.
{
	if (!IsValidIndex(Start) || Count <= 0)		// Valid operation?
	{
		Dest = __xT("");
		return Dest;
	}

	TChar *Temp = AllocStr(Count + 1);
	_tcsncpy(Temp, &Text[Start], Count);
	Temp[Count] = __xT('\0');

	Dest = Temp;
	FreeStr(Temp);
	return Dest;
}

//=======================================================
//
// Special transformation functions
//
void xString::VarArg(TChar *Format, ...)		// Allows you to fill a string object with data in the
{												// same way you use sprintf()

	TChar Buf[0x1000];      // Need lots of space
	va_list  argptr;

	va_start(argptr, Format);
#ifdef _UNICODE
	vswprintf(Buf, Format, argptr);
#else
	vsprintf(Buf, Format, argptr);
#endif
	va_end(argptr);

	Size = _tcslen(Buf) + 1;
	Len = _tcslen(Buf);
	FreeStr(Text);
	Text = AllocStr(Size);
	if (Len > 0)
		_tcscpy(Text, Buf);
	else
		Text[0] = __xT('\0');
	
}
void xString::EatLeadingWhitespace()		// Convenient function that removes all whitespace
{											// (tabs and spaces) at the beginning of a string.
	if (IsNull())
		return;

	int i=0;
	for (i=0 ; i<Len ; i++)
	{
		if (Text[i] != 0x20 && Text[i] != 0x09)
			break;
	}
	Delete(0, i);
	
}
void xString::EatTrailingWhitespace()		// Convenient function that removes all whitespace
{											// (tabs and spaces) from the end of a string.
	if (IsNull())
		return;

	int i=0;
	for (i=Len-1 ; i>=0 ; i--)
	{
		if (Text[i] != 0x20 && Text[i] != 0x09)
			break;
	}
	Delete(i+1, Len-i-1);
	
}

	
//=======================================================
//
// Conversion functions
//
TAnsiChar*
xString::ToAnsi(TAnsiChar *Buffer, int BufferLen) const	// Converts the string to an ANSI string.
{
	int i, ConvertLen;

	if (BufferLen <= 0)
	{
		Buffer = NULL;
		return Buffer;
	}

	if (BufferLen >= Len)
		ConvertLen = Len;
	else
		ConvertLen = BufferLen-1;

	for (i=0 ; i<ConvertLen ; i++)
	{
#ifdef _UNICODE
		if (Text[i] > 255)		// If character is a non-ANSI Unicode character, fill with
			Buffer[i] = 0x20;		// space instead.
		else
#endif
			Buffer[i] = (TAnsiChar)Text[i];
	}
	Buffer[i] = __xA('\0');
	return Buffer;
}
TUnicodeChar *
xString::ToUnicode(TUnicodeChar *Buffer, int BufferLen) const		// Converts the string
{																								// to a Unicode string.
	int i, ConvertLen;

	if (BufferLen <= 0)
	{
		Buffer = NULL;
		return Buffer;
	}

	if (BufferLen >= Len)
		ConvertLen = Len;
	else
		ConvertLen = BufferLen-1;

	for (i=0 ; i<ConvertLen ; i++)
		Buffer[i] = (TUnicodeChar)Text[i];
	Buffer[i] = __xU('\0');
	return Buffer;
}

xString::operator TUnicodeChar*()const
{
	TUnicodeChar*out  =new TUnicodeChar[Size];
	ToUnicode(out,Size);
	return out;
}
int 
xString::ToInt() const								// Converts the string to an integer.
{
	if (IsNull())
		return 0;
#ifdef _UNICODE
	return _wtoi(Text);						// Unicode version of atoi
#else
	return atoi(Text);						// ANSI version of atoi
#endif
}

//=======================================================
//
// Assignment operators
//

void
xString::_attachRight(const TCHAR*rValue)
{
	Optimize();
	xString rBuffer(rValue);
	rBuffer.Optimize();

	TChar *Temp = xString::AllocStr(Size + rBuffer.Size);		// Allocate memory for new string
	if (Len)																// Copy first string into dest
		_tcscpy(Temp, Text);
	if (rBuffer.Len)																// Copy second string into dest
		_tcscpy(&Temp[Len], rBuffer.Text);
	Text = Temp;
}


xString& 
xString::operator<<(const void* iValue)
{
	const TChar* rValue = static_cast<const TChar*>(iValue);
	if ( rValue )
	{
		_attachRight(rValue);

	}else
		return *this;

}
xString& 
xString::operator<<(const int iValue)
{
	xString buffer("");
#ifdef _UNICODE
	_itow(iValue,buffer.GetString(),10);  
#else
	_itot(iValue,buffer.GetString(),10);  
#endif
	_attachRight(buffer);
	return *this;    		
}

xString& 
xString::operator<<(const TChar* rValue)
{

	Optimize();
	xString rBuffer(rValue);
	rBuffer.Optimize();

	return operator +=(rBuffer);

	const TChar* buffer = static_cast<const TChar*>(rValue);
	if ( buffer )
	{
		_attachRight(buffer);
		delete buffer;
	}else
		return *this;

}
//=======================================================
//
// Concatenation operators
//
//friend xString operator +(const xString& Str1, const xString& Str2);	// Concatenates two strings (see text)

//friend xString operator +(const xString& Str1, const xString& Str2);	// Concatenates two strings (see text)





//=======================================================
//
// Access operators
//
TAnsiChar*
xString::Str()
{	
	return operator TAnsiChar*();
}
TChar& 
xString::operator [](int Pos)	// Returns a character reference at a
{													// specific location.
	if (Pos < 0)								// If underrun, just return first character
		return Text[0];
	else if (Pos >= Len)						// If overrun, expand string in accordance
	{
		Grow(Pos+2);
		return Text[Pos];
	}
	else											// Otherwise, just return character
		return Text[Pos];
}



//=======================================================
//
// Protected low-level functions
//
void xString::Optimize()			// Discards any unused space allocated for the string
{
	Size = Len + 1;
	TChar *Temp = AllocStr(Size);
	_tcscpy(Temp, Text);
	FreeStr(Text);
	Text = Temp;
}
void xString::Grow(int Num)		// Allocates some more memory so the string can hold an additional Num characters
{
	Size += Num;
	TChar *Temp = AllocStr(Size);
	_tcscpy(Temp, Text);
	FreeStr(Text);
	Text = Temp;
}
void xString::AssignFrom(const xString& Str)			// Does the hard work for all non-converting assignments
{
	Size = Str.Size;
	Len = Str.Len;
	if (Size && Len)						// No point copying an empty string
	{
		Text = AllocStr(Size);
		_tcscpy(Text, Str.Text);
	}
	else
	{
		Text = NULL;
	}
}


TChar *
xString::AllocStr(int Size)		// Allocates a new character array. You can modify this
{												// function if you for instance use a custom memory manager.
	//return new TChar[Size];
	return (TChar *)calloc(Size, sizeof(TChar));
}
void xString::FreeStr(TChar *Ptr)		// Ditto
{
	if (Ptr == NULL)
		return;
	//delete Ptr;
	//return;
    
	free(Ptr);
}
//=======================================================
//
// Concatenation function (must be global for reasons stated in the text)
//
/*

xString::operator +(const xString& Str1, const TChar& Str2)
{

TChar *Temp = xString::AllocStr(Str1.Size + Str2.Size);		// Allocate memory for new string
if (Str1.Len)																// Copy first string into dest
	_tcscpy(Temp, Str1.Text);
if (Str2.Len)																// Copy second string into dest
	_tcscpy(&Temp[Str1.Len], Str2.Text);

xString Result = Temp;
return Result;
}*/