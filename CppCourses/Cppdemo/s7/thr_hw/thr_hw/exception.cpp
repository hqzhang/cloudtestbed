#include <exception.h>

namespace bbt{

Exception::Exception(const std::string& what_arg) throw(): 
  _what(what_arg) 
{ }

Exception::~Exception() throw()
{}

const char *Exception::getString() const 
{
	return _what.c_str();
}



IOException::IOException(const std::string &what_arg) throw() : 
  Exception(what_arg)
{ }

IOException::~IOException() throw()
{ }

}; //end of namepsace


