
#ifndef	BBT_POSIXTHREAD_H_
#define	BBT_POSIXTHREAD_H_

#define	BBT_POSIX

#include <ctime>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include "thread.h"

namespace bbt {


class ThreadKey;


class PosixThread: public Thread
{
private:
	/** @internal */
	friend class ThreadImpl;
	friend class Thread;
	time_t	_alarm;
	static void signalThread(Thread* th,signo_t signo);
protected:
		
	/**
	 * In the Posix version of Common C++, this can be used to send a
	 * signal into the parent thread of the current object.
	 * 
	 * @param signo a posix signal id.
	 */
	inline void signalParent(signo_t signo)
		{ signalThread(_parent,signo); };
	
	/**
	 * In the Posix version of Common C++, this can be used to send a
	 * signal into the main application thread.
	 * 
	 * @param signo a posix signal id.
	 */
	inline void signalMain(signo_t signo)
		{ signalThread(_main,signo);};

	/**
	 * A derivable method to call when a SIGALRM is being delivered
	 * to a specific thread.
	 */
	virtual void onTimer(void);

	/**
	 * A derived method to handle hangup events being delivered
	 * to a specific thread.
	 */
	virtual void onHangup(void);

	/**
	 * A derived method to call when a SIGABRT is being delivered
	 * to a specific thread.
	 */
	virtual void onException(void);

	/**
	 * A derived method to call when a SIGPIPE is being delivered
	 * to a specific thread.
	 */
	virtual void onDisconnect(void);

	/**
	 * A derived method to handle asynchronous I/O requests delivered
	 * to the specified thread.
	 */
	virtual void onPolling(void);

	/**
	 * A derivable method to call for delivering a signal event to
	 * a specified thread.
	 *
	 * @param - posix signal id.
	 */
	virtual void onSignal(int);
	
	/**
	 * Used to specify a timeout event that can be delivered to the
	 * current thread via SIGALRM.  When the timer expires, the onTimer() 
	 * method is called for the thread.  At present, only one thread
	 * timer can be active at any given time.  On some operating
	 * systems (including Linux) a timer can be active on each thread.
	 * 
	 * @param timer timeout in milliseconds.
	 * @param periodic should the timer be periodic.
	 * @note currently, periodic timers are only available on
	 * systems with a working setitimer call.
	 */
	void setTimer(timeout_t timer, bool periodic = false);
	
	/**
	 * Gets the time remaining for the current threads timer before
	 * it expires.
	 * 
	 * @return time remaining before timer expires in milliseconds.
	 */
	timeout_t getTimer(void) const;
	
	/**
	 * Terminates the timer before the timeout period has expired.
	 * This prevents the timer from sending it's SIGALRM and makes
	 * the timer available to other threads.
	 */
	void endTimer(void);

	/**
	 * Used to wait on a Posix signal from another thread.  This can be
	 * used as a crude rondevious/synchronization method between threads.
	 * 
	 * @param signo a posix signal id.
	 */
	void waitSignal(signo_t signo);
	
	/**
	 * Used to enable or disable a signal within the current thread.
	 *
	 * @param signo posix signal id.
	 * @param active set to true to enable.
	 */
	void setSignal(int signo, bool active);

	/**
 	 * Access to pthread_attr structure
 	 *  this allows setting/modifying pthread attributes
 	 *  not covered in the platform independant Thread constructor,
 	 *  e.g. contention scope or scheduling policy
 	 */
	pthread_attr_t *getPthreadAttrPtr(void);

	/**
 	 * Get pthread_t of underlying posix thread (useful for 
	 * debugging/logging)
  	 */
	pthread_t getPthreadId(void);

public:

	PosixThread(int pri = 0, size_t stack = 0);
	
	/**
	 * Delivers a Posix signal to the current thread.
	 * 
	 * @param signo a posix signal id.
	 */
	inline void signalThread(int signo)
		{signalThread(this, signo);};

	/**
	 * Install a signal handler for use by threads and
	 * the OnSignal() event notification handler.
	 *
	 * @param signo posix signal id.
	 */
	static void sigInstall(int signo);
};


	
}

#endif
