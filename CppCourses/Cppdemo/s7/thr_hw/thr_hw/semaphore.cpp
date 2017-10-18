#include <bthread.h>
#include <private.h>

namespace bbt {


#include <cerrno>

Semaphore::Semaphore(size_t resource) 
{
	
	if(sem_init(&_semaphore, 0, resource))
	{
		if(Thread::getException() == Thread::throwObject)
			throw(this);
	}
}

Semaphore::~Semaphore()
{
		sem_destroy(&_semaphore);
}

void Semaphore::wait(void)
{
	sem_wait(&_semaphore);
}

void Semaphore::post(void)
{
	sem_post(&_semaphore);
}

bool Semaphore::tryWait(void)
{
	return (sem_trywait(&_semaphore) == 0) ? true : false;
}

int Semaphore::getValue(void)
{
	int value;

	sem_getvalue(&_semaphore, &value);
	return value;
}
};
