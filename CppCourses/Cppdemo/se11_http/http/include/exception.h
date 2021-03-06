/**
 * @file exception.h
 * exception model base classes.
 **/

#ifndef	BBT_EXCEPTION_H_
#define	BBT_EXCEPTION_H_



#include <exception>
#include <string>
namespace bbt {

/**
 * Mainline exception handler, this is the root for all  C++
 * @short Base exception class for all C++ exceptions.
 */
class Exception : public std::exception 
{
private:
	std::string _what;

public:
	Exception(const std::string& what_arg) throw();
	virtual ~Exception() throw();
	virtual const char *getString() const; 
	virtual const char *what() const throw(); 
};

class IOException : public Exception
{
private:
	long _systemError;
	mutable char* _systemErrorString;

public:
	IOException(const std::string &what_arg, long systemError = 0) throw();
	virtual ~IOException() throw();

	virtual long getSystemError() const throw();
	virtual const char* getSystemErrorString() const throw();
};

/**
 * A sub-hierarchy for thread exceptions.
 *
 * @author David Sugar <dyfet@ostel.com>
 * @short thread exception hierarchy.
 */
class ThrException : public Exception
{
	public:
		ThrException(const std::string &what_arg) : Exception(what_arg) 
		{};
};

/**
 * A sub-hierarchy for all task synchronizion related exceptions.
 *
 * @author David Sugar <dyfet@ostel.com>
 * @short Synchronization object exception hierarchy.
 */
class SyncException : public ThrException
{
	public:
		SyncException(const std::string &what_arg) : 
			ThrException(what_arg) {};
};

class InterruptException : public ThrException
{
public:
	InterruptException() : ThrException("interrupted") {};
};

}; //end of namespace 
#endif
