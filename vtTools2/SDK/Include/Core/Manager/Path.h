#ifndef PATH_H
#define PATH_H


#include "CKALL.H"
#include "windows.h"

#include <STDLIB.H>
#include <STRING.H>




#include "Shlwapi.h"
#pragma comment (lib,"SHLWAPI.LIB")



class XPath {

private:


	char path_separator_;
	char drive_separator_;
	char extension_separator_;
		
public:
		
	XString data_;
	
	//ctors
	
	inline XPath();
	XPath( const XPath& path );
	XPath( const XString& filepath );
	
    XPath( const XString& dirpath, const XString& filename );
    XPath
      (
      const XString& dirpath,
      const XString& base,
      const XString& extension
      );
	XPath
      (
      char drive,
      const XString& dirpath,
      const XString& filename
      );
    XPath
      (
      char drive,
      const XString& dirpath,
      const XString& base,
      const XString& extension
      );
	
	

	//components
	XString operator[]( int index ) const;

    // Testing.
    bool absolute() const;
    bool relative() const;

    bool directory_path() const;
    bool has_directory() const;
    bool has_extension() const;
    bool has_drive() const;
	bool is_valid();

		
	// Conversion.
    operator const char*() const;
    operator const XString&() const;
	
    // Comparison
    int operator==( const XPath& path ) const;
    int operator==( const XString& string ) const;
    int operator!=( const XPath& path ) const;
    int operator!=( const XString& string ) const;
    int operator<( const XPath& path ) const;
    int operator<( const XString& string ) const;

	XPath& operator=( const XString& filepath );
    XPath& operator=( const XPath& path );

	//used for the objectmode
	XPath& operator+=(CKBeObject *beo){
		
		if(data_.Length() == 0)data_ = VxGetTempPath().CStr();
		data_ << beo->GetName() << ".nmo";
		return *this;
	}

	
	// Accessors.
    char* GetFileName();
	char*  GetPath();

	
	
	char path_separator() const;
    char drive_separator() const;
    char extension_separator() const;
	
	int absolute_levels() const;
	
protected:

	void init_separators();
	// Common finds.
    size_t last_extension_separator() const;

};


/************************************************************************/
/* protected funtions										                                                                     */
/************************************************************************/
inline void
XPath::init_separators(){

	path_separator_ = '\\';
	drive_separator_ = ':';
	extension_separator_ = '.';
}

/************************************************************************/
/* ctors //ops // converts								                                                                     */
/************************************************************************/
inline
XPath::XPath(const XString& dirpath, const XString& filename ){
	init_separators();
	data_  << dirpath <<path_separator_<< filename;
}
inline
XPath::XPath(){
	init_separators();
}
inline
XPath::XPath( const XString& filepath ) :  data_( filepath ){
	init_separators();
}
inline
XPath::XPath( const XPath& path ) :  data_( path.data_ ){
	init_separators();
}
inline
XPath::operator const XString&() const{
  return data_;
}
inline
XPath::operator const char*() const{
  return data_.CStr();
}
inline XPath&
XPath::operator=( const XString& filepath ){
  data_ = filepath;
  return *this;
}
inline XPath&
XPath::operator=( const XPath& path ){
  data_ = path.data_;
  return *this;
}
inline int
XPath::operator==( const XPath& path ) const{
  return data_ == path.data_;
}
inline int
XPath::operator<( const XPath& path ) const{
  return data_ < path.data_;
}
inline int
XPath::operator<( const XString& string ) const{
  return data_ < string;
}
inline int
XPath::operator==( const XString& string ) const{
  return data_ == string;
}
inline int
operator==( const XString& string, const XPath& path ){
  return path == string;
}
inline int
XPath::operator!=( const XPath& path ) const{
  return data_ != path.data_;
}
inline int
XPath::operator!=( const XString& string ) const{
  return data_ != string;
}
inline int
operator!=( const XString& string, const XPath& path ){
  return path != string;
}
/************************************************************************/
/* Functions													                                                                        */
/************************************************************************/

inline char
XPath::extension_separator() const{
  return extension_separator_;
}
inline bool
XPath::has_drive() const{
  return data_.Find( drive_separator(), 0 ) == 1;
}
inline bool
XPath::has_extension() const{
  return data_.Find(extension_separator_ , 0 ) == 1;
}
inline bool
XPath::absolute() const{
  return has_drive() ||
          (data_.Find( path_separator(), 0 ) == 0);
}
inline bool
XPath::relative() const{
  return !absolute();
}
/************************************************************************/
/* Get Components                                                                     */
/************************************************************************/

/************************************************************************/
/* get components											                                                                     */
/************************************************************************/


#endif


