/**
 * @file synchronization.h 
 * @short Synchronization.
 **/

#ifndef	_BBT_SYNCHRONIZATION_H_
#define	_BBT_SYNCHRONIZATION_H_

#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

namespace bbt {


typedef	unsigned long	timeout_t;


/**
 * The Mutex class is used to protect a section of code so that at any
 * given time only a single thread can perform the protected operation.
 * 
 * The Mutex can be used as a base class to protect access in a derived
 * class.  When used in this manner, the ENTER_CRITICAL and LEAVE_CRITICAL
 * macros can be used to specify when code written for the derived class
 * needs to be protected by the default Mutex of the derived class, and
 * hence is presumed to be 'thread safe' from multiple instance execution.
 * One of the most basic C++ synchronization object is the Mutex
 * class.  A Mutex only allows one thread to continue execution at a given
 * time over a specific section of code.  Mutex's have a enter and leave
 * method; only one thread can continue from the Enter until the Leave is
 * called.  The next thread waiting can then get through.  Mutex's are also
 * known as "CRITICAL SECTIONS" in win32-speak.
 * 
 * The Mutex is always recursive in that if the same thread invokes
 * the same mutex lock multiple times, it must release it multiple times.
 * This allows a function to call another function which also happens to
 * use the same mutex lock when called directly. This was
 * deemed essential because a mutex might be used to block individual file
 * requests in say, a database, but the same mutex might be needed to block a
 * whole series of database updates that compose a "transaction" for one
 * thread to complete together without having to write alternate non-locking
 * member functions to invoke for each part of a transaction.
 * 
 * @short Mutex lock for protected access.
 */
class Mutex
{
private:
	static bool _debug;
	const char *_name;
protected:
	/*
	 * Pthread mutex object.  This is protected rather than private
	 * because some mixed mode pthread operations require a mutex as
	 * well as their primary pthread object.  A good example of this
	 * is the Event class, as waiting on a conditional object must be
	 * associated with an accessable mutex.  
	 */
	pthread_mutex_t	_mutex;

public:
	/**
	 * The mutex is always initialized as a recursive entity.
	 */
	Mutex(const char *name = NULL);

	/**
	 * Destroying the mutex removes any system resources associated
	 * with it.  If a mutex lock is currently in place, it is presumed
	 * to terminate when the Mutex is destroyed.
	 */
	virtual ~Mutex();

	/**
	 * Enable or disable deadlock debugging.
	 *
	 * @param mode debug mode.
	 */
	static void setDebug(bool mode)
		{_debug = mode;};


	/**
	 * Entering a Mutex locks the mutex for the current thread.  This
	 * also can be done using the ENTER_CRITICAL macro or by using the
	 * ++ operator on a mutex.
	 * 
	 * @see #leaveMutex
	 */
	void enterMutex(void);

	/**
	 * Tries to lock the mutex for the current thread. Behaves like
	 * #enterMutex , except that it doesn't block the calling thread
	 * if the mutex is already locked by another thread.
	 *
	 * @return true if locking the mutex was succesful otherwise false
	 *
	 * @see enterMutex
	 * @see leaveMutex
	 */
	bool tryEnterMutex(void);

	/**
	 * Leaving a mutex frees that mutex for use by another thread.  If
	 * the mutex has been entered (invoked) multiple times (recursivily)
	 * by the same thread, then it will need to be exited the same number
	 * of instances before it is free for re-use.  This operation can
	 * also be done using the LEAVE_CRITICAL macro or by the -- operator
	 * on a mutex.
	 * 
	 * @see #enterMutex
	 */
	void leaveMutex(void);
};

/**
 * The MutexLock class is used to protect a section of code so that at any
 * given time only a single thread can perform the protected operation.
 * 
 * It use Mutex to protect operation. Using this class is usefull and 
 * exception safe.
 * 
 * A common use is
 * 
 * void func_to_protect()
 * {
 *   MutexLock lock(mutex);
 *   ... operation ...
 * }
 *
 * NOTE: do not declare variable as "MutexLock (mutex)", the mutex will be 
 * released at statement end.
 * 
 * @author Frediano Ziglio <freddy77@angelfire.com>
 * @short Mutex automatic locker for protected access.
 */
class MutexLock
{
private:
	Mutex& mutex;
public:
	/**
	 * Acquire the mutex
	 */
	MutexLock( Mutex& _mutex ) : mutex( _mutex ) 
		{ mutex.enterMutex(); }
	/**
	 * Release the mutex automatically
	 */
	// this should be not-virtual
	~MutexLock()
		{ mutex.leaveMutex(); }
};

/**
 * The Mutex Counter is a counter variable which can safely be incremented
 * or decremented by multiple threads.  A Mutex is used to protect access
 * to the counter variable (an integer).  An initial value can be specified 
 * for the counter, and it can be manipulated with the ++ and -- operators.
 * 
 */
class MutexCounter : public Mutex
{
private:
	int	counter;

public:
	MutexCounter(const char *id = NULL);
	MutexCounter(int initial, const char *id = NULL);

	friend int operator++(MutexCounter &mc);
	friend int operator--(MutexCounter &mc);
};

/**
 * The AtomicCounter class offers thread-safe manipulation of an integer
 * counter.  These are commonly used for building thread-safe "reference"
 * counters for C++ classes.  
 *
 */
class AtomicCounter
{
private:
	int counter;
	pthread_mutex_t _mutex;

public:
	/**
	 * Initialize an atomic counter to 0.
	 */
	AtomicCounter();

	/**
	 * Initialize an atomic counter to a known value.
	 *
	 * @param value initial value.
	 */
	AtomicCounter(int value);

	~AtomicCounter();

	int operator++(void);
	int operator--(void);
	int operator+=(int change);
	int operator-=(int change);
	int operator+(int change);
	int operator-(int change);
	int operator=(int value);
	bool operator!(void);
	operator int();
};


/**
 * A conditional variable synchcronization object for one to one and
 * one to many signal and control events between processes.
 * Conditional variables may wait for and receive signals to notify
 * when to resume or perform operations.  Multiple waiting threads may
 * be woken with a broadcast signal.
 *
 * @warning While this class inherits from Mutex, the methods of the
 * class Conditional just handle the system conditional variable, so
 * the user is responsible for calling enterMutex and leaveMutex so as
 * to avoid race conditions. Another thing to note is that if you have
 * several threads waiting on one condition, not uncommon in thread
 * pools, each thread must take care to manually unlock the mutex if
 * cancellation occurs. Otherwise the first thread cancelled will
 * deadlock the rest of the thread.
 *
 */
class Conditional 
{
private:
	pthread_cond_t _cond;
	pthread_mutex_t _mutex;

public:
	/**
	 * Create an instance of a conditional.
	 */
	Conditional(const char *id = NULL);

	/**
	 * Destroy the conditional.
	 */
	virtual ~Conditional();

	/**
	 * Signal a conditional object and a waiting threads.
	 *
	 * @param broadcast this signal to all waiting threads if true.
	 */
	void signal(bool broadcast);

	/**
 	 * Wait to be signaled from another thread.
	 *
	 * @param timer time period to wait.
	 * @param locked flag if already locked the mutex.
	 */
	bool wait(timeout_t timer = 0, bool locked = false);

        /**
         * Locks the conditional's mutex for this thread.  Remember
	 * that Conditional's mutex is NOT a recursive mutex!
         *
         * @see #leaveMutex
         */
        void enterMutex(void);

	/**
	 * In the future we will use lock in place of enterMutex since
	 * the conditional composite is not a recursive mutex, and hence
	 * using enterMutex may cause confusion in expectation with the
	 * behavior of the Mutex class.
	 *
	 * @see #enterMutex
	 */
	inline void lock(void)
		{enterMutex();};

        /**
         * Tries to lock the conditional for the current thread. 
	 * Behaves like #enterMutex , except that it doesn't block the 
	 * calling thread.
         *
         * @return true if locking the mutex was succesful otherwise false
         *
         * @see enterMutex
         * @see leaveMutex
         */
        bool tryEnterMutex(void);

	inline bool test(void)
		{return tryEnterMutex();};

        /**
         * Leaving a mutex frees that mutex for use by another thread.  
	 *
         * @see #enterMutex
         */
        void leaveMutex(void);

	inline void unlock(void)
		{return leaveMutex();};
};

/**
 * A semaphore is generally used as a synchronization object between multiple
 * threads or to protect a limited and finite resource such as a memory or
 * thread pool.  The semaphore has a counter which only permits access by
 * one or more threads when the value of the semaphore is non-zero.  Each
 * access reduces the current value of the semaphore by 1.  One or more
 * threads can wait on a semaphore until it is no longer 0, and hence the
 * semaphore can be used as a simple thread synchronization object to enable
 * one thread to pause others until the thread is ready or has provided data
 * for them.  Semaphores are typically used as a
 * counter for protecting or limiting concurrent access to a given
 * resource, such as to permitting at most "x" number of threads to use
 * resource "y", for example.   
 * 
 * @short Semaphore counter for thread synchronization. 
 */
class  Semaphore
{
private:
	sem_t _semaphore;
public:
	/**
	 * The initial value of the semaphore can be specified.  An initial
	 * value is often used When used to lock a finite resource or to 
	 * specify the maximum number of thread instances that can access a 
	 * specified resource.
	 * 
	 * @param resource specify initial resource count or 0 default.
	 */
	Semaphore(size_t resource = 0);
	

	/**
	 * Destroying a semaphore also removes any system resources
	 * associated with it.  If a semaphore has threads currently waiting
	 * on it, those threads will all continue when a semaphore is
	 * destroyed.
	 */
	virtual ~Semaphore();

	/**
	 * Wait is used to keep a thread held until the semaphore counter
	 * is greater than 0.  If the current thread is held, then another
	 * thread must increment the semaphore.  Once the thread is accepted, 
	 * the semaphore is automatically decremented, and the thread 
	 * continues execution.
	 * 
	 * The pthread semaphore object does not support a timed "wait", and
	 * hence to maintain consistancy, neither the posix nor win32 source
	 * trees support "timed" semaphore objects.
	 * 
	 * @see #post
	 */
	void wait(void);

	/**
	 * TryWait is a non-blocking variant of Wait. If the semaphore counter
	 * is greater than 0, then the thread is accepted and the semaphore
	 * counter is decreased. If the semaphore counter is 0 TryWait returns
	 * immediately with false.
	 *
	 * @return true if thread is accepted otherwise false
	 *
	 * @see #wait
	 * @see #post
	 */
	bool tryWait(void);

	/**
	 * Posting to a semaphore increments its current value and releases
	 * the first thread waiting for the semaphore if it is currently at
	 * 0.  Interestingly, there is no support to increment a semaphore by
	 * any value greater than 1 to release multiple waiting threads in
	 * either pthread or the win32 API.  Hence, if one wants to release
	 * a semaphore to enable multiple threads to execute, one must perform
	 * multiple post operations.
	 * 
	 * @see #wait
	 */
	void post(void);
	
	/**
	 * Get the current value of a semaphore.
	 *
	 * @return current value.
	 */
	int getValue(void);

};
} // namesapce bbt
#endif
