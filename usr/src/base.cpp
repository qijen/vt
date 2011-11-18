#include <base.h>

#include <sstream>
#include <iostream>




std::ostream& base::_Debug = std::cerr;
std::ostream& base::_Log = std::cout;
std::ostream& base::_Console = std::cout;



#ifdef DEBUG
bool _outputExceptionOnConstruction = true;
bool _abortOnAssertionFailure = false;
#endif


/*
String base::intToString(Int i)
{
  std::ostringstream oss;
  oss << i;
  return oss.str();
}


Int base::stringToInt(const String& s)
{
  std::istringstream iss(s);
  Int v;
  iss >> v;
  return v;
}

Real base::stringToReal(const String& s)
{
  std::istringstream iss(s);
  Real v;
  iss >> v;
  return v;
}

String base::realToString(Real r)
{
  std::ostringstream oss;
  oss << r;
  return oss.str();
}



*/
