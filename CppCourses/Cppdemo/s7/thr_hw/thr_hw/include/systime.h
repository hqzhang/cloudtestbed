/**
 * This class is used to access non-reentrant date and time functions in the
 * standard C library.
 *
 * The class has two purposes:
 * - 1 To be used internaly in CommonCpp's date and time classes to make them
 *     thread safe.
 * - 2 To be used by clients as thread safe replacements to the standard C
 *     functions, much like Thread::sleep() represents a thread safe version
 *     of the standard sleep() function.
 *
 * @note The class provides one function with the same name as its equivalent
 *       standard function and one with another, unique name. For new clients,
 *       the version with the unique name is recommended to make it easy to
 *       grep for accidental usage of the standard functions. The version with
 *       the standard name is provided for existing clients to sed replace their
 *       original version.
 *
 * @short Thread safe date and time functions.
 */
#ifndef BBT_SYSTIME_H_
#define BBT_SYSTIME_H_
#include <synchronization.h>
#include <ctime>
#include <sys/time.h>
namespace bbt{
class SysTime
{
private:
		static Mutex timeLock;

protected:
		inline static void lock(void)
			{timeLock.enterMutex();}

		inline static void unlock(void)
			{timeLock.leaveMutex();}

public:
        static time_t getTime(time_t *tloc = NULL);
        static time_t time(time_t *tloc) 
			{ return getTime(tloc); };

        static int getTimeOfDay(struct timeval *tp);
        static int gettimeofday(struct timeval *tp, struct timezone *)
			{ return getTimeOfDay(tp); };
        
};
} 
#endif


