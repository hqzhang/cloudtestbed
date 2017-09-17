
#include <bthread.h>
#include <sched.h>
#include <exception.h>
#include <cstdio>
#include <private.h>
#include <systime.h>
#include <new>


namespace bbt {

#define BBT_SIG_THREAD_ALARM
// NOTE: Comment this line to test Resume/Signal using one signal method
// #define BBT_SIG_THREAD_STOPCONT
typedef	void	*(*exec_t)(void *);
typedef	RETSIGTYPE (*signalexec_t)(int);
extern "C"
{

static void bbt_exec_handler(Thread *th);
static void bbt_thread_cleanup(void* arg);
static void bbt_thread_destructor(void* arg);
static void bbt_sig_handler(int signo);
}



/*
 * Start Suspend/Resume stuff
 */

// method to suspend are
// - system suspend/resume not recursive
// - one signal only, not recursive
#define BBT_SUSPEND_MODE_STOPCONT 2
#define BBT_SUSPEND_MODE_ONE_SIGNAL 3

#if defined(BBT_SIG_THREAD_STOPCONT)
#  define BBT_SUSPEND_MODE BBT_SUSPEND_MODE_STOPCONT
#   define BBT_SIG_THREAD_SUSPEND SIGSTOP
#   define BBT_SIG_THREAD_RESUME  SIGCONT
    static inline void bbt_resume(cctid_t tid) { 
	    pthread_kill(tid, BBT_SIG_THREAD_RESUME); 
    }
    static inline void bbt_suspend(cctid_t tid) { 
	    pthread_kill(tid, BBT_SIG_THREAD_SUSPEND); 
    }
# else
#  define BBT_SUSPEND_MODE BBT_SUSPEND_MODE_ONE_SIGNAL
#  ifndef SIGUSR3
#  ifdef  SIGWINCH
#  define SIGUSR3 SIGWINCH
#  else
#  define SIGUSR3 SIGINT
#  endif
#  endif
#  define BBT_SIG_THREAD_SUSPEND SIGUSR3
#  define BBT_SIG_THREAD_RESUME SIGUSR3
   static inline void bbt_resume(cctid_t tid) { 
	   pthread_kill(tid, BBT_SIG_THREAD_RESUME); 
   }
   static inline void bbt_suspend(cctid_t tid) { 
	   pthread_kill(tid, BBT_SIG_THREAD_SUSPEND); 
   }
# endif

Thread::Cancel Thread::enterCancel(void)
{
	Thread *th = getThread();

	if(!th)
		return cancelInitial;

	Cancel old = th->_cancel;	
	if(old != cancelDisabled && old != cancelImmediate)
	{
		th->setCancel(cancelImmediate);
		pthread_testcancel();
	}

	return old;
}

void Thread::exitCancel(Cancel old)
{
	Thread *th = getThread();

	if(!th)
		return;

	if(old != th->_cancel)
	{
		pthread_testcancel();
		th->setCancel(old);
	}
}

void Thread::suspend(void)
{
	if(!priv)
		return;


	if (!priv->_suspendEnable) return;
	if (++priv->_suspendcount != 1) return;
	bbt_suspend(priv->_tid);

}

#ifndef	AUTOSTACK
#define	AUTOSTACK	0x100000
#endif

size_t Thread::_autostack = AUTOSTACK;

void Thread::resume(void)
{
	if(!priv)
		return;

	if (!priv->_suspendEnable) return;
	int c;
	if ( (c = --priv->_suspendcount) > 0) return;
	if ( c < 0 ) 
	{
		++priv->_suspendcount;
		return;
	}
	bbt_resume(priv->_tid);

}

void Thread::join(void)
{
	bool detached = isDetached();
	joinSem.wait();
	if(detached)
	{
		joinSem.post();
		return;
	}

	if(priv->_jtid)
	{
		pthread_join(priv->_jtid, NULL);
	}

	priv->_jtid = 0;
	joinSem.post();	// enable next waiting thread after cleanup
}

#if BBT_SUSPEND_MODE == BBT_SUSPEND_MODE_ONE_SIGNAL
// NOTE: Do not modify _suspendcount here, one program can call
// Suspend 2 or more time but this function can be called only once
inline RETSIGTYPE ThreadImpl::ThreadSigSuspend(int)
{
//to do wait single to suspend
}

static RETSIGTYPE bbt_sigsuspend(int signo)
{
	return ThreadImpl::ThreadSigSuspend(signo);
}
#endif
void	Thread::setSuspend(Suspend mode)
{
	if(!priv)
		return;

	priv->_suspendEnable = (mode == suspendEnable);
#ifdef  BBT_SIG_THREAD_SUSPEND
	sigset_t mask;

	sigemptyset(&mask);
	sigaddset(&mask, BBT_SIG_THREAD_SUSPEND);

	switch(mode)
	{
	case suspendEnable:
		pthread_sigmask(SIG_UNBLOCK, &mask, NULL);
		return;
	case suspendDisable:
		pthread_sigmask(SIG_BLOCK, &mask, NULL);
	}
#endif
}

/*
 * End Suspend/Resume stuff
 */

static	sigset_t *blocked_signals(sigset_t *sig)
{
	sigemptyset(sig);
	sigaddset(sig, SIGINT);
	sigaddset(sig, SIGKILL);
	sigaddset(sig, SIGHUP);
	sigaddset(sig, SIGABRT);
	sigaddset(sig, SIGALRM);
	sigaddset(sig, SIGPIPE);
#if BBT_SUSPEND_MODE == BBT_SUSPEND_MODE_ONE_SIGNAL
	sigaddset(sig, SIGUSR3);
#endif
	return sig;
}

typedef enum ThreadType { 
	threadTypeNormal=0, 
	threadTypeMain, 
	threadTypePosix,
	threadTypeDummy
} ThreadType;

class MainThread : public Thread
{
protected:
	void run(void) {return;};
	void onSignal(int signo) { std::exit(signo);};

public:
	MainThread() : Thread(true) {};
};

// mantain info on thread creation
class DummyThread : public Thread
{
protected:
	void run() {};
public:
	DummyThread() : Thread(false) { priv->_type = threadTypeDummy; }
};

// NOTE: _self instantiation MUST appear before _mainthread !!
ThreadKey ThreadImpl::_self(bbt_thread_destructor);


static	MainThread _mainthread;
Thread	*Thread::_main = NULL;

// invalid pointer to thread used to test deleted thread
// point in the middle of mainthread...
#define DUMMY_INVALID_THREAD ((Thread*)(((char*)((Thread*)&_mainthread))+1))


//void PosixThread::sigInstall(int);

Thread::Thread(bool isMain):
_cancel(cancelDefault), _start(NULL), priv(new ThreadImpl(threadTypeDummy))
{
	priv->_suspendEnable = false;
	priv->_tid = pthread_self();
	_parent = NULL;
	struct sigaction act;
	
	// NOTE: for race condition (signal handler can use getThread)
	// you should initialize _main and _self before registering signals
	ThreadImpl::_self.setKey(this);
	if(isMain == true)
	{
		_main = this;
		priv->_type = threadTypeMain;
		PosixThread::sigInstall(SIGHUP);
		PosixThread::sigInstall(SIGALRM);
		PosixThread::sigInstall(SIGPIPE);
		PosixThread::sigInstall(SIGABRT);

		memset(&act, sizeof(act), 0);
		act.sa_handler = (signalexec_t)&bbt_sig_handler;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGIO, &act, NULL);

# if BBT_SUSPEND_MODE == BBT_SUSPEND_MODE_ONE_SIGNAL
		act.sa_handler = bbt_sigsuspend;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGUSR3, &act, NULL);
# endif

    	}

}	

Thread::Thread(int pri, size_t stack):
_cancel(cancelDefault), _start(NULL), priv(new ThreadImpl(threadTypeNormal))
{
	int	salign;

	pthread_attr_init(&priv->_attr);
	pthread_attr_setdetachstate(&priv->_attr, PTHREAD_CREATE_JOINABLE);

#ifdef	PTHREAD_STACK_MIN
	if(stack && stack <=  _autostack)
		pthread_attr_setstacksize(&priv->_attr, _autostack);
	else if(stack > _autostack)
	{
		if(stack < PTHREAD_STACK_MIN)
			stack = PTHREAD_STACK_MIN;
		else	// align to nearest min boundry
		{	
			salign = stack / PTHREAD_STACK_MIN;
			if(stack % PTHREAD_STACK_MIN)
				++salign;
			stack = salign * PTHREAD_STACK_MIN;
		}
		if(stack && pthread_attr_setstacksize(&priv->_attr, stack))
		{
#ifdef	BBT_EXCEPTIONS
			switch(Thread::getException())
			{
			case throwObject:
				throw(this);
				return;
#ifdef	COMMON_STD_EXCEPTION
			case throwException:
				throw(ThrException("no stack space"));
				return;
#endif
			default:
				return;
			}
#else
			return;
#endif
		}
	}
#endif

	if(pri)
	{
		struct sched_param sched;
		int policy;

		policy = sched_getscheduler(0);
		if(policy < 0)
		{
#ifdef	BBT_EXCEPTIONS
			switch(Thread::getException())
			{
			case throwObject:
				throw(this);
				return;
#ifdef	COMMON_STD_EXCEPTION
			case throwException:
				throw(ThrException("invalid scheduler"));
				return;
#endif
			default:
				return;
			}
#else
			return;
#endif
		}

		sched_getparam(0, &sched);

		pri = sched.sched_priority - pri;
		if(pri 	> sched_get_priority_max(policy))
			pri = sched_get_priority_max(policy);

		if(pri < sched_get_priority_min(policy))
			pri = sched_get_priority_min(policy);

		sched.sched_priority = pri;
		pthread_attr_setschedpolicy(&priv->_attr, policy);
		pthread_attr_setschedparam(&priv->_attr, &sched);
	}	

	if(!pri)
	        pthread_attr_setinheritsched(&priv->_attr, PTHREAD_INHERIT_SCHED);             

	_parent = getThread();
	priv->_throw = _parent->priv->_throw;

	_cancel = cancelInitial;

}

Thread::Thread(const Thread &th)
{
	priv = new ThreadImpl(threadTypeNormal);
	_parent = th._parent;
	priv->_attr = th.priv->_attr;
	_cancel = cancelInitial;
	_start = NULL;
	priv->_throw = th.priv->_throw;
	priv->_suspendEnable = false;

	setName(NULL);
	
}

Thread::~Thread()
{
	if(!priv)
		return;

	if(this == &_mainthread)
		return;

	if(priv->_type == threadTypeDummy)
	{
		delete priv;
		priv = NULL;
		return;
	}

	terminate();
}

void Thread::setName(const char *text)
{
	if(text)
		snprintf(_name, sizeof(_name), "%s", text);
	else
		snprintf(_name, sizeof(_name), "%ld", (long)getId());
}	

void Thread::initial(void)
{
}

void Thread::final(void)
{
}

void *Thread::getExtended(void)
{
	return NULL;
}

void Thread::notify(Thread *)
{
}

bool Thread::isThread(void)
{
	if(!priv)
		return false;

	return (priv->_tid == pthread_self()) ? true : false;
}

bool Thread::isDetached(void)
{
        if(!priv)
                return false;

        int state;

        pthread_attr_getdetachstate(&priv->_attr, &state);
        if(state == PTHREAD_CREATE_DETACHED)
                return true;
        return false;
}

cctid_t Thread::getId(void) const
{
	return priv->_tid;
}

bool Thread::isRunning(void)
{
	if(!priv)
		return false;
	return (priv->_tid != 0) ? true : false;
}

int Thread::start(Semaphore *st)
{
// to do ...
}

int Thread::detach(Semaphore *st)
{
//to do ...
}

void Thread::terminate(void)
{
	if(!priv)
		return;

        cctid_t jtid = priv->_jtid, tid = priv->_tid;

	if(jtid && (pthread_self() != jtid))
	{
		pthread_join(jtid, NULL);
		priv->_jtid = 0;
	}
	else if((pthread_self() != tid) && tid)
	{
		// in suspend thread cannot be cancelled or signaled
		// ??? rigth
		// bbt_resume(priv->_tid);
		

		// assure thread has ran before we try to cancel...
		if(_start)
			_start->post();

		pthread_cancel(tid);
		if(!isDetached())
		{
			pthread_join(tid,NULL);
			priv->_tid = 0;
		}
	}

        pthread_attr_destroy(&priv->_attr);
	delete priv;
	priv = NULL;
}

void Thread::sync(void)
{
#if defined(__GNU__)
	Thread::exit();
#else
	Thread::sleep(TIMEOUT_INF);
#endif
}

void Thread::exit(void)
{
	if (isThread())
	{
		setCancel(cancelDisabled);
		pthread_exit(NULL);
	}
	
}
		
void Thread::close()
{
	bool detached = isDetached();

	setCancel(cancelDisabled);
	if(_parent)
		_parent->notify(this);
      
	// final can call destructor (that call Terminate)
	final();

	// test if this class is self-exiting thread

	if (ThreadImpl::_self.getKey() == this)
	{
		if(priv)
		{
			priv->_jtid = priv->_tid;
		}
		joinSem.post();
	}

	// see if detached, and hence self deleting

	if(detached)
		delete this;
}

inline void ThreadImpl::ThreadCleanup(Thread* th)
{
	// close thread
	// (freddy77) Originally I thougth to throw an exception for deferred
	// for capture it and cleanup using C++ destructor
	// this doesn't work out!! 
	// Throwing exception here (in cleanup) core dump app
	th->close();
}
static void bbt_thread_cleanup(void* arg)
{
	ThreadImpl::ThreadCleanup( (Thread*)arg );
}

inline void ThreadImpl::ThreadExecHandler(Thread *th)
{
// to do ...
}

// delete Thread class created for no CommonC++ thread
inline void ThreadImpl::ThreadDestructor(Thread* th)
{
	if (!th || th == DUMMY_INVALID_THREAD || !th->priv)
		return;
	if(!th->priv)
		return;
	if (th->priv->_type == threadTypeDummy)
		delete th;
}

static void bbt_thread_destructor(void* arg)
{
	ThreadImpl::ThreadDestructor( (Thread*)arg );
}

static void bbt_exec_handler(Thread *th)
{
	ThreadImpl::ThreadExecHandler(th);
}

void	Thread::setCancel(Cancel mode)
{
	int old;

	switch(mode)
	{
	case cancelImmediate:
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old);
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old);
		break;
	case cancelDeferred:
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old);
		pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old);
		break;
	case cancelInitial:
	case cancelDisabled:
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old);
		break;
	default:
		return;
	}
	_cancel = mode;
}


void	Thread::yield(void)
{

	pthread_testcancel();
	sched_yield();

}

void Thread::setException(Thread::Throw mode)
{
	Thread *thread = getThread();
	thread->priv->_throw = mode;
}

Thread::Throw Thread::getException(void)
{
	Thread *thread = getThread();
	return thread->priv->_throw;
}

Cancellation::Cancellation(Thread::Cancel cancel)
{
	Thread *thread = getThread();
	if(!thread)
		return;

	prior = thread->getCancel();
	thread->setCancel(cancel);
}

Cancellation::~Cancellation()
{
	Thread *thread = getThread();
	if(!thread)
		return;

	thread->setCancel(prior);
}

bool Thread::testCancel(void)
{

        pthread_testcancel();
	return false;

} 


Thread *Thread::get(void)
{
	Thread *thread;
	
	// fix strange no-init on Solaris
        if(!Thread::_main)
	{
		new (&_mainthread) MainThread();
		return &_mainthread;
	}

	thread = (Thread *)ThreadImpl::_self.getKey();

	// class have been deleted, return NULL
	if (thread == DUMMY_INVALID_THREAD)
		return NULL;

	if(!thread)
	{
		// this Thread will be deleted by bbt_thread_destruct
		thread = new DummyThread;
		ThreadImpl::_self.setKey(thread);
	}
	return thread;
}


/*
 * PosixThread implementation
 */
inline void ThreadImpl::PosixThreadSigHandler(int signo)
{
	Thread	*t = getThread();
	PosixThread *th = NULL;

#ifdef	BBT_EXCEPTIONS
	if (t) th = dynamic_cast<PosixThread*>(t);
#else
        if (t) th = (PosixThread*)(t);
#endif

	if (!th) return;

	switch(signo)
	{
	case SIGHUP:
		if(th)
			th->onHangup();
		break;
	case SIGABRT:
		if(th)
			th->onException();
		break;
	case SIGPIPE:
		if(th)
			th->onDisconnect();
		break;
	case SIGALRM:
		    if(th)
			th->onTimer();
		break;
	case SIGIO:
		if(th)
			th->onPolling();
		break;
	default:
		if(th)
			th->onSignal(signo);
	}
}
static void bbt_sig_handler(int signo)
{
	ThreadImpl::PosixThreadSigHandler(signo);
}

PosixThread::PosixThread(int pri, size_t stack):
	Thread(pri,stack)
{
       SysTime::getTime(&_alarm);
}

void PosixThread::onTimer(void)
{
}

void PosixThread::onHangup(void)
{
}

void PosixThread::onException(void)
{
}

void PosixThread::onDisconnect(void)
{
}

void PosixThread::onPolling(void)
{
}

void PosixThread::onSignal(int sig)
{
}


void PosixThread::setTimer(timeout_t timer, bool periodic)
{
	sigset_t sigs;

	struct itimerval itimer;
	
	memset(&itimer, 0, sizeof(itimer));
	itimer.it_value.tv_usec = (timer * 1000) % 1000000;
	itimer.it_value.tv_sec = timer / 1000;
	if (periodic) {
		itimer.it_interval.tv_usec = itimer.it_value.tv_usec;
		itimer.it_interval.tv_sec = itimer.it_value.tv_sec;
	}

       SysTime::getTime(&_alarm);
	sigemptyset(&sigs);
	sigaddset(&sigs, SIGALRM);
	pthread_sigmask(SIG_UNBLOCK, &sigs, NULL);

	setitimer(ITIMER_REAL, &itimer, NULL);
}	

timeout_t PosixThread::getTimer(void) const
{
	struct itimerval itimer;
	if(!_alarm)
		return 0;

	getitimer(ITIMER_REAL, &itimer);
	return (timeout_t)(itimer.it_value.tv_sec * 1000 +
		itimer.it_value.tv_usec / 1000);
}

void PosixThread::endTimer(void)
{
	static const struct itimerval itimer = {{0, 0},{0,0}};

	sigset_t sigs;

	setitimer(ITIMER_REAL, (struct itimerval *)&itimer, NULL);
	sigemptyset(&sigs);
	sigaddset(&sigs, SIGALRM);
	pthread_sigmask(SIG_BLOCK, &sigs, NULL);
}

void	PosixThread::waitSignal(signo_t signo)
{
	sigset_t	mask;

	sigemptyset(&mask);
	sigaddset(&mask, signo);
	signo = sigwait(&mask,&signo);
}

void	PosixThread::setSignal(int signo, bool mode)
{
	sigset_t sigs;

	sigemptyset(&sigs);
	sigaddset(&sigs, signo);

	if(mode)
		pthread_sigmask(SIG_UNBLOCK, &sigs, NULL);
	else
		pthread_sigmask(SIG_BLOCK, &sigs, NULL);
}	

void	PosixThread::signalThread(Thread* th,signo_t signo)
{
	pthread_kill(th->priv->_tid, signo);
}

pthread_attr_t * PosixThread::getPthreadAttrPtr(void)
{
 	return &priv->_attr;
}

pthread_t PosixThread::getPthreadId(void)
{
	return priv->_tid;
}

void PosixThread::sigInstall(int signo)
{
	struct sigaction act;
	
	act.sa_handler = (signalexec_t)&bbt_sig_handler;
	sigemptyset(&act.sa_mask);

	act.sa_flags = 0;
	sigaction(signo, &act, NULL);
}



}
 

