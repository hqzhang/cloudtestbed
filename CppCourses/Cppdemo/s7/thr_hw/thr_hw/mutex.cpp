// Copyright (C) 1999-2005 Open Source Telecom Corporation.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
// 
// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however    
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.    
//
// This exception applies only to the code released under the name GNU
// Common C++.  If you copy code from other releases into a copy of GNU
// Common C++, as the General Public License permits, the exception does
// not apply to the code that you add in this way.  To avoid misleading
// anyone as to the status of such modified files, you must delete
// this exception notice from them.
//
// If you write modifications of your own for GNU Common C++, it is your choice
// whether to permit this exception to apply to your modifications.
// If you do not wish that, delete this exception notice.
//

#include <exception.h>
#include <bthread.h>
#include <private.h>
#include <slog.h>
#include <iostream>
#include <cerrno>


namespace bbt {
#ifndef PTHREAD_MUTEXTYPE_RECURSIVE
#ifdef PTHREAD_MUTEXTYPE_RECURSIVE_NP
#define PTHREAD_MUTEXTYPE_RECURSIVE PTHREAD_MUTEXTYPE_RECURSIVE_NP
#else 
#define PTHREAD_MUTEXTYPE_RECURSIVE 0
#endif
#endif
#if 0
ThreadLock::ThreadLock()
{
#ifdef HAVE_PTHREAD_RWLOCK
	pthread_rwlockattr_t attr;
	
	pthread_rwlockattr_init(&attr);
	if(pthread_rwlock_init(&_lock, &attr))
	{
#ifdef	CCXX_EXCEPTIONS
		if(Thread::getException() == Thread::throwObject)
			throw(this);
#ifdef	COMMON_STD_EXCEPTION
		else if(Thread::getException() == Thread::throwException)
			throw(SyncException("Mutex constructor failure"));
#endif
#endif
	}
#endif
}

ThreadLock::~ThreadLock()
{
#ifdef HAVE_PTHREAD_RWLOCK
	pthread_rwlock_destroy(&_lock);
#endif
}

void ThreadLock::readLock(void)
{
#ifdef HAVE_PTHREAD_RWLOCK
	pthread_rwlock_rdlock(&_lock);
#else
	mutex.enterMutex();
#endif
}

void ThreadLock::writeLock(void)
{
#ifdef HAVE_PTHREAD_RWLOCK
	pthread_rwlock_wrlock(&_lock);
#else
	mutex.enterMutex();
#endif
}

void ThreadLock::unlock(void)
{
#ifdef HAVE_PTHREAD_RWLOCK
	pthread_rwlock_unlock(&_lock);
#else
	mutex.leaveMutex();
#endif
}

bool ThreadLock::tryReadLock(void)
{
#ifdef	HAVE_PTHREAD_RWLOCK
	if(pthread_rwlock_tryrdlock(&_lock))
		return false;
	return true;
#else
	return mutex.tryEnterMutex();
#endif
}

bool ThreadLock::tryWriteLock(void)
{
#ifdef	HAVE_PTHREAD_RWLOCK
	if(pthread_rwlock_trywrlock(&_lock))
		return false;
	return true;
#else
	return mutex.tryEnterMutex();
#endif
}
#endif
Conditional::Conditional(const char *id) 
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutex_init(&_mutex, &attr);
	pthread_mutexattr_destroy(&attr);
	if(pthread_cond_init(&_cond, NULL) && Thread::getException() == Thread::throwObject)
		throw this;
}

Conditional::~Conditional()
{
	pthread_cond_destroy(&_cond);
	pthread_mutex_destroy(&_mutex);
}

bool Conditional::tryEnterMutex(void)
{
        if(pthread_mutex_trylock(&_mutex) != 0)
		return false;
	return true;
}

void Conditional::enterMutex(void)
{
        pthread_mutex_lock(&_mutex);
}

void Conditional::leaveMutex(void)
{
        pthread_mutex_unlock(&_mutex);
}

void Conditional::signal(bool broadcast)
{
	if(broadcast)
		pthread_cond_broadcast(&_cond);
	else
		pthread_cond_signal(&_cond);
}

bool Conditional::wait(timeout_t timeout, bool locked)
{
	struct timespec ts;
	int rc;

	if(!locked)
		enterMutex();
	if(!timeout)
	{
		pthread_cond_wait(&_cond, &_mutex);
		if(!locked)
			leaveMutex();
		return true;
	}
	getTimeout(&ts, timeout);
	rc = pthread_cond_timedwait(&_cond, &_mutex, &ts);
	if(!locked)
		leaveMutex();
	if(rc == ETIMEDOUT)
		return false;
	return true;
}

bool Mutex::_debug = false;
Mutex::Mutex(const char *name)
{
	pthread_mutexattr_t _attr;

	pthread_mutexattr_init(&_attr);
	pthread_mutexattr_settype(&_attr, PTHREAD_MUTEXTYPE_RECURSIVE);
	pthread_mutex_init(&_mutex, &_attr);
	pthread_mutexattr_destroy(&_attr);

	_name = name;
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&_mutex);
}


bool Mutex::tryEnterMutex(void)
{
	return (pthread_mutex_trylock(&_mutex) == 0) ? true : false;
}

void Mutex::enterMutex(void)
{
	if(_debug && _name)
		slog.debug() << Thread::get()->getName() 
			<< ": entering " << _name << std::endl;

	pthread_mutex_lock(&_mutex);
}

void Mutex::leaveMutex(void)
{
	pthread_mutex_unlock(&_mutex);
	if(_debug && _name)
		slog.debug() << Thread::get()->getName() 
			<< ": leaving" << _name << std::endl;

}


MutexCounter::MutexCounter(int initial, const char *id) : Mutex(id)
{
	counter = initial;
}

int operator++(MutexCounter &mc)
{
	int rtn;

	mc.enterMutex();
	rtn = mc.counter++;
	mc.leaveMutex();
	return rtn;
}

// ??? why cannot be < 0 ???
int operator--(MutexCounter &mc)
{
	int rtn = 0;

	mc.enterMutex();
	if(mc.counter)
	{
		rtn = --mc.counter;
		if(!rtn)
		{
			mc.leaveMutex();
			throw mc;
		}
	}
	mc.leaveMutex();
	return rtn;
}


AtomicCounter::AtomicCounter()
{
	counter = 0;

        pthread_mutexattr_t _attr;
        pthread_mutexattr_init(&_attr);
        pthread_mutex_init(&_mutex, &_attr);
        pthread_mutexattr_destroy(&_attr);
}

AtomicCounter::AtomicCounter(int value)
{
	counter = value;
	
        pthread_mutexattr_t _attr;
        pthread_mutexattr_init(&_attr);
        pthread_mutex_init(&_mutex, &_attr);
        pthread_mutexattr_destroy(&_attr);
}

AtomicCounter::~AtomicCounter()
{
        pthread_mutex_destroy(&_mutex);
}

int AtomicCounter::operator++(void)
{
	int value;

	pthread_mutex_lock(&_mutex);
	value = ++counter;
	pthread_mutex_unlock(&_mutex);
	return value;
}

int AtomicCounter::operator--(void)
{
	int value;
	pthread_mutex_lock(&_mutex);
	value = --counter;
	pthread_mutex_unlock(&_mutex);
	return value;
}

int AtomicCounter::operator+=(int change)
{
	int value;
	pthread_mutex_lock(&_mutex);
	counter += change;
	value = counter;
	pthread_mutex_unlock(&_mutex);
	return value;
}

int AtomicCounter::operator-=(int change)
{
	int value;
	pthread_mutex_lock(&_mutex);
	counter -= change;
	value = counter;
	pthread_mutex_unlock(&_mutex);
	return value;
}

int AtomicCounter::operator+(int change)
{
	int value;
	pthread_mutex_lock(&_mutex);
	value = counter + change;
	pthread_mutex_unlock(&_mutex);
	return value;
}

int AtomicCounter::operator-(int change)
{
	int value;
	pthread_mutex_lock(&_mutex);
	value = counter - change;
	pthread_mutex_unlock(&_mutex);
	return value;
}

AtomicCounter::operator int()
{
	int value;
	pthread_mutex_lock(&_mutex);
	value = counter;
	pthread_mutex_unlock(&_mutex);
	return value;
}

int AtomicCounter::operator=(int value)
{
	int ret;
	pthread_mutex_lock(&_mutex);
	ret = counter;
	counter = value;
	pthread_mutex_unlock(&_mutex);
	return ret;
}

bool AtomicCounter::operator!(void)
{
	int value;
	pthread_mutex_lock(&_mutex);
	value = counter;
	pthread_mutex_unlock(&_mutex);
	if(value)
		return false;
	return true;
}
} //namespace bbt

