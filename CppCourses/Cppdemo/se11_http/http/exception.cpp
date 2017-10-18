#include <common.h>
#include <exception.h>

namespace bbt {

Exception::Exception(const std::string& what_arg) throw(): 
  _what(what_arg) 
{ }

Exception::~Exception() throw()
{}

const char *Exception::what() const throw()
{
	return _what.c_str();
}

const char *Exception::getString() const 
{
	return _what.c_str();
}

IOException::IOException(const std::string &what_arg, long systemError) throw() : 
	Exception(what_arg), _systemError(systemError), 
	_systemErrorString(NULL)
{
}

IOException::~IOException() throw()
{
	delete [] _systemErrorString;
}

long IOException::getSystemError() const throw()
{
	return _systemError;
}

const char* IOException::getSystemErrorString() const throw()
{
	const uint32 errStrSize = 2048;
	if ( !_systemErrorString )
		_systemErrorString = new char[errStrSize];
	strerror_r(_systemError, _systemErrorString, errStrSize);
	return _systemErrorString;

}

}

