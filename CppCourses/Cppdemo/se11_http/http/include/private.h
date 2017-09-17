#ifndef	BBT_PRIVATE_H_
#define	BBT_PRIVATE_H_
#define RETSIGTYPE void
namespace bbt {

class ThreadImpl
{
friend class Thread;
friend class DummyThread;
friend class PosixThread;
friend class Slog;

	ThreadImpl(int type):
		_msgpos(0),
		_throw(Thread::throwObject),
		_tid(0),
		_suspendEnable(true),
		_type(type),
		_jtid(0)
	{ ; };

	// derived class copy constructor creates new instance, so base
	// copy constructor of ThreadImpl should do nothing...

	ThreadImpl(const ThreadImpl& copy)
		{;};

	ThreadImpl &operator=(const ThreadImpl& copy)
		{return *this;};

	pthread_attr_t _attr;
	AtomicCounter _suspendcount;
	static ThreadKey _self;
	// log information
	size_t _msgpos;
	char _msgbuf[128];
	Thread::Throw _throw;
	cctid_t _tid;

	friend Thread *getThread(void);
	volatile bool _suspendEnable:1;
	unsigned int _type:3;
	cctid_t _jtid;

public:
	// C binding functions
	static inline void ThreadExecHandler(Thread* th);
	static inline RETSIGTYPE ThreadSigSuspend(int);
	static inline void ThreadCleanup(Thread* arg);
	static inline void ThreadDestructor(Thread* arg);
	static inline void PosixThreadSigHandler(int signo);
};

}

#endif
