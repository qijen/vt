/********************************************************************
	created:	2004/11/06
	created:	6:11:2004   16:53
	filename: 	D:\projects new\vt tools\Manager\typedefs.h
	file path:	D:\projects new\vt tools\Manager
	file base:	typedefs
	file ext:	h
	author:		
	
	purpose:	gBaumgart
*********************************************************************/


#ifndef TYPEDEFS_H
	#define TYPEDEFS_H
	
	#include "CKAll.h"



	#include <STDLIB.H>
	#include <LIST>
	#include <MAP>
/*
	The following template "Hashfunc"  only meets the requests of std::map. the reason for it is the standard compare.(result : int, but bool is used)

	from XString.h:
	
	  int operator == (const XBaseString& iStr) const
	  { return !Compare(iStr); }

	if i don´t use this help class i´ll get the following warning:

  "
  D:\SDK\VC98\INCLUDE\functional(86) : warning C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
  D:\SDK\VC98\INCLUDE\functional(86) : while compiling class-template member function 'bool __thiscall std::less<class XString>::operator ()(const class XString &,const class XString &) const'
  "
  by call ZipJobList zili.find(XXX)


  or use simply :
  struct XStringCMP{	
		bool operator() (const XString& p, const XString& q) const
				{	return strcmp(p.CStr(),q.CStr())<0; }
	  };
 */

template <class T> struct HashFunc{
	bool operator ()(const T& x,const T& y)const;
};


//the XString special:
template <> struct HashFunc<XString>{
	bool operator ()(const XString& In1,const XString& In2)const{
			
		return strcmp(In1.CStr(),In2.CStr())<0;
		/* or : 
		size_t out  = 0;
		const char* key = In1.CStr();
		while (*key) out = (out <<1)^*key++;//an integer rep of a c-string
		*/
	}
};

/*	
	ZipJobList
	+Keystring		|		Filelist
	+......		
*/

typedef std::list<XString>XFileList;
typedef std::list<XString>::iterator XFileListIterator;

typedef std::map<XString,XFileList*,HashFunc<XString> >ZipJobList; 
typedef std::map<XString,XFileList*,HashFunc<XString> >::iterator ZiJoIt;


/************************************************************************/
/*							none zip stuff														                                                                     */
/************************************************************************/

/************************************************************************/
/*	 a workin  array container											                                                                     */
/************************************************************************/
/*
  template<class T, int amount> class fake_container{

public:
	
	 //standard stuff:
	typedef T value_type;

	typedef T* iterator;

	typedef const T*const_iterator;
	typedef T& reference;
	typedef const T& const_reference;
	

	typedef const T* const_iterator;
	T v[amount];
	operator T*() {return v;}
	
	//ops: 
	iterator& operator++(){return v+1;}
	iterator& operator--(){return v-1;}
	//..........................................................

	//nice:
	reference operator[] (ptrdiff_t i) { return v[i] ; }
	const_reference operator[] (ptrdiff_t i) const { return v[i] ; }
	
	iterator begin() {return v;}
	const_iterator begin() const {return v;}
	iterator end() {return v+amount;}
	const_iterator end() const {return v + amount;}
	size_t size () const { return amount;}
		
	protected:
	};
*/


/************************************************************************/
/* tests																																									*/
/************************************************************************/


//this should become pointer array, but very strange
/*
template<class T> class Vector{
	T*v;
	int sz;
	
	public:
	Vector();
//		explicit Vector(int);

//		Vector();
//		operator T*() {return v;}

//T& elem(int i){return v[i];}
//T& operator[](int i);
};
*/
//thats becomes a vector - void - pointer - special template
/*
template<> class Vector<void*>{
	void **p;
	void *&operator[](int i);
};
*/

/************************************************************************/
/* the void* specialist:                                                                     */
/************************************************************************/
/*
template<class T> class Vector<T*> : private Vector<void*>{

	public:
 	typedef Vector<void*> Base;

	Vector() : Base(){}
	puplicit Vector()(int i) : Base(i){}
	T*elem(int i){ return static_cast<T*&>(Base::elem(i));}
	T*& operator[](int i){return static_cast<T*&>(Base::operator [](i));}
};

*/

/*
template<class T> class Vector{
	T*v;
	int sz;
	
	public:
	Vector();
	explicit Vector(int);
	operator T*() {return v;}
	T& elem(int i){return v[i];}
	T& operator[](int i);
};
*/
#endif
