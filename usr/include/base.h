#ifndef _BASE_
#define _BASE_

#include <iostream>
#include <string>
#include <list>
#include <stdexcept>
#include <algorithm>
#include <typeinfo>
#include <functional>


extern "C" {
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
//#include <GL/gl.h>
}

// instantiate unit tests
#ifdef DEBUG
#define INSTANTIATE_TEST_SUITES2
extern bool _abortOnAssertionFailure;
#endif


namespace base {
  typedef unsigned char     Byte;
  typedef int               SInt;
  typedef unsigned int      Int;
  typedef long unsigned int LInt;
  typedef std::string       String;
  typedef double            Real;   // Real & GLreal must be the same built-in type

  /// throw a std::runtime_error(errorstring).  If DEBUG, output an assertion failure message
  ///  to the Log and abort() if abortOnAssertionFailure mode set
  void assertionFailure(const String& errorstring);

  template<typename A> inline void tassert(A assertion, const String& errorstring)
  {
    if (!assertion)
      assertionFailure(errorstring);
  }

  template<typename T> void clearMemory(T* start, Int length)
  { memset(start, 0, size_t(length*sizeof(T))); }

  template<typename T> void copyMemory(const T* src, T* dest, Int length)
  { memcpy(dest, src, length*sizeof(T)); }

  // convenience: subscripting & indexing for list ( O(n) )
  template<typename T> const T& elementAt(const std::list<T>& l, typename std::list<T>::size_type i)
  {
    typename std::list<T>::const_iterator it = l.begin();
    while ((it != l.end()) && (i > 0)) {
      ++it; --i;
    }
    if (it == l.end()) throw std::out_of_range("elementAt - specified index not present");
    return *it;
  }

  template<typename T> T& elementAt(std::list<T>& l, typename std::list<T>::size_type i)
  {
    typename std::list<T>::iterator it = l.begin();
    while ((it != l.end()) && (i > 0)) {
      ++it; --i;
    }
    if (it == l.end()) throw std::out_of_range("elementAt - specified index not present");
    return *it;
  }



  class DeleteObject {
  public:
    template<typename T> void operator()(const T* ptr) const { delete ptr; }
  };

  template<typename C> void delete_all(C& c)
  {
    for_all(c, DeleteObject());
  }


  template<typename InputIterator, typename OutputIterator, typename Predicate>
  OutputIterator copy_if(InputIterator begin, InputIterator end,
			 OutputIterator destBegin, Predicate p)
  {
    while(begin!=end) {
      if (p(*begin)) *destBegin++=*begin;
      ++begin;
    }
    return destBegin;
  }

  String intToString(Int i);
  String realToString(Real r);
  Int stringToInt(const String& s);
  Real stringToReal(const String& s);

  template<typename T> String toString(const T& t)
  {
    std::ostringstream oss;
    oss << t;
    return oss.str();
  }

  template<typename T> T fromString(const String& s)
  {
    std::istringstream iss(s);
    T t;
    iss >> t;
    return t;
  }



  class Cloneable;
  template<typename C> C& clone(const C& c)
  {
    return dynamic_cast<C&>( static_cast<const Cloneable&>(c).clone());
  }

#ifndef VIRTOOLS

  extern std::ostream& _Debug;
  extern std::ostream& _Log;
  extern std::ostream& _Console;
#else

  extern std::ostream& _Debug;
  extern std::ostream& _Log;
  extern std::ostream& _Console;

#endif

  /// convert typeid().name() string into demangled form (e.g. "base::Object")
  String demangleTypeidName(const String& typeidName);

  /// convert type_info into qualified class name (calls demangleTypeidName() )
  String className(const std::type_info& ti);

  extern int _currentDebugVerbosity;

  // make narrow casting explicit for readability
  template <
    typename Sub,
    typename Super>
  //inline Sub* narrow_cast(Select< SUPERSUBCLASS_STRICT(Super,Sub),Super*,NullType>::Result p) { return static_cast<Sub*>(p); }
  inline Sub* narrow_cast(Super* p) { return static_cast<Sub*>(p); }

  template <
    typename Sub,
    typename Super>
  //inline Sub& narrow_cast(Select< SUPERSUBCLASS_STRICT(Super,Sub),Super&,NullType>::Result p) { return static_cast<Sub&>(p); }
  inline Sub& narrow_cast(Super& p) { return static_cast<Sub&>(p); }

} // base





// global names
using std::for_each;
using std::mem_fun;
using base::narrow_cast;




// Use Log() funcions to output to the log file.  This will remain in
//  release code.  Use Debug() if you want output that will dissapear
//  in release code.  Use Console() to write on the graphical console
//  (e.g. for user status messages etc.)
// NB: see debugtools header for Debug() usage.

//#include <bDebugtools.h>

#ifdef __mips
#define __func__ String("unknown")
#endif

#ifdef __GNUC_
#define _LOG_CALLER_NAME __PRETTY_FUNCTION__ << " -- "
#else
#define _LOG_CALLER_NAME base::className(typeid(*this)) << "::" << __FUNCTION__ << " -- "
#endif

#define Log(o) { base::_Log << _LOG_CALLER_NAME << o; }
#define Logln(o) { base::_Log << _LOG_CALLER_NAME << o << "\n"; }
#define Logc(o) { base::_Log << o; }
#define Logcln(o) { base::_Log << o << "\n"; }

#define Logf(o) { base::_Log << __func__ << " -- " << o; }
#define Logfln(o) { base::_Log << __func__ << " -- " << o << "\n"; }
#define Logfc(o) Logc(o)
#define Logfcln(o) Logcln(o)


#define Console(o) { base::_Console << o; }
#define Consoleln(o) { base::_Console << o << std::endl; }

// Often, when an Assert() fails, it is not clear where the exception
//  was raised from the message alone.  Enabling this flag will cause
//  the program to abort() from inside Assert() so that a debugger
//  stack trace can show the point of failure.
#ifdef DEBUG
  extern bool _abortOnAssertionFailure;
  #define abortOnAssertionFailureEnabled(e) _abortOnAssertionFailure=(e)
#else
  #define abortOnAssertionFailureEnabled(e)
#endif


// During development it is not unusual for exceptions to be thrown in unexpected places. For example,
//  from a function declared with throw().  It also happens when an exception tries to propagate
//  through C code in the call stack.  For example, this is common if the main loop of the app
//  is being executed from a C library callback (e.g. from GLUT).
//  This typically results in a call to abort() before the exception is caught.  The only way to trace where
//  the exception was thrown in that case is via the debugger.  To make life a little easier, in DEBUG mode
//  the Exception() macro is defined to print the exception text upon construction - so it can be seen even if
//  the exception is not caught.  However, this can be annoying in circumstances when exceptions are
//  expected (for example, in test cases that test for correct exception throwing).  Consequently, it may
//  be disabled.
#ifdef DEBUG
  extern bool _outputExceptionOnConstruction;
  #define exceptionOutputEnabled(e) _outputExceptionOnConstruction=(e)
#else
  #define exceptionOutputEnabled(e)
#endif



#ifdef __GNUC__

  #ifdef DEBUG
    #define Exception(o) (( (_outputExceptionOnConstruction?(printf("constructing exception: %s\n", \
      (String(__PRETTY_FUNCTION__)+" (line "+base::intToString(__LINE__)+") - "+String(o)).c_str())):(0)) ), \
                                    String(String("exception thrown: ")+__PRETTY_FUNCTION__+":\n - "+String(o)))


  #else   // ndef DEBUG
    #define Exception(o) String(String("exception thrown: ")+__PRETTY_FUNCTION__+" - "+String(o))
  #endif  // DEBUG

  #define Exceptionf(o) String(String("exception thrown: ")+__PRETTY_FUNCTION__+":\n - "+String(o))
  #define Assertion(o) String(String("assertion failed: ")+__PRETTY_FUNCTION__+" (line "+base::intToString(__LINE__)+") - "+String(o))
  #define Assertionf(o) String(String("assertion failed: ")+__PRETTY_FUNCTION__+" (line "+base::intToString(__LINE__)+") - "+String(o))


#else // ndef __GNUC__

  #ifdef DEBUG
    #define Exception(o) (( (_outputExceptionOnConstruction?(printf("constructing exception: %s\n", \
        (String(base::className(typeid(*this)))+"::"+String(__func__)+" - "+String(o)).c_str())):(0)  )), \
                             String(String("exception thrown: ")+String(base::className(typeid(*this)))+"::"+String(__func__)+":\n - "+String(o)))
  #else
    #define Exception(o) String(String("exception thrown: ")+String(base::className(typeid(*this)))+"::"+String(__func__)+":\n - "+String(o))
  #endif
  #define Exceptionf(o) String(String("exception thrown: ")+String(__func__)+":\n - "+String(o))
  #define Assertion(o) String(String("assertion failed: ")+String(base::className(typeid(*this)))+"::"+String(__func__)+" - "+String(o))
  #define Assertionf(o) String(String("assertion failed: ")+String(__func__)+" - "+String(o))

#endif // __GNUC__

#ifdef DEBUG

  #define Assert(a) { if (!(a)) base::assertionFailure(Assertion(#a)); }
  #define Assertf(a) { if (!(a)) base::assertionFailure(Assertionf(#a)); }
  #define Assertm(a,s) { if (!(a)) base::assertionFailure(Assertion(s)); }
  #define Assertmf(a,s) { if (!(a)) base::assertionFailure(Assertionf(s)); }
#else // ndef DEBUG
  #define Assert(a)
  #define Assertf(a)
  #define Assertm(a,s)
  #define Assertmf(a,s)
#endif // DEBUG

#define Assertifm(f,a,s) Assertm(!f || a,s)
#define Assertifmf(f,a,s) Assertmf(!f || a,s)

#define instanceof(var,type) (dynamic_cast<type*>(&var) != 0)


#endif

