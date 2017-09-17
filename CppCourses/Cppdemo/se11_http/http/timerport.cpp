#include <common.h>
#include <bthread.h>
#include <systime.h>
#include "private.h"
#include <timerport.h>

namespace bbt {

TimerPort::TimerPort()
{
	active = false;
       SysTime::getTimeOfDay(&timer);
}

void TimerPort::setTimer(timeout_t timeout)
{
       SysTime::getTimeOfDay(&timer);
	active = false;
	if(timeout)
		incTimer(timeout);
}

void TimerPort::incTimer(timeout_t timeout)
{
	int secs = timeout / 1000;
	int usecs = (timeout % 1000) * 1000;

	timer.tv_usec += usecs;
	if(timer.tv_usec > 1000000l)
	{
		++timer.tv_sec;
		timer.tv_usec %= 1000000l;
	}
	timer.tv_sec += secs;
	active = true;
}

void TimerPort::endTimer(void)
{
	active = false;
}

timeout_t TimerPort::getTimer(void) const
{
	struct timeval now;
	long diff;

	if(!active)
		return TIMEOUT_INF;

       SysTime::getTimeOfDay(&now);
	diff = (timer.tv_sec - now.tv_sec) * 1000l;
	diff += (timer.tv_usec - now.tv_usec) / 1000l;

	if(diff < 0)
		return 0l;

	return diff;
}

timeout_t TimerPort::getElapsed(void) const
{
	struct timeval now;
	long diff;

	if(!active)
		return TIMEOUT_INF;

       SysTime::getTimeOfDay(&now);
	diff = (now.tv_sec -timer.tv_sec) * 1000l;
	diff += (now.tv_usec - timer.tv_usec) / 1000l;
	if(diff < 0)
		return 0;
	return diff;
}
}
