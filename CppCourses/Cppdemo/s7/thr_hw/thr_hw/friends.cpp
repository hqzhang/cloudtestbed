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

#include <bthread.h>
#include <private.h>
#include <systime.h>
#include <cstdlib>

#include <sys/time.h>
#ifdef	SIGTSTP
#include <sys/file.h>
#include <sys/ioctl.h>
#endif

#ifndef	_PATH_TTY
#define	_PATH_TTY "/dev/tty"
#endif


namespace bbt {


timespec *getTimeout(struct timespec *spec, timeout_t timer)
{
	static	struct timespec myspec;

	if(spec == NULL)
		spec = &myspec;

#ifdef	PTHREAD_GET_EXPIRATION_NP
	struct timespec offset;

	offset.tv_sec = timer / 1000;
	offset.tv_nsec = (timer % 1000) * 1000000;
	pthread_get_expiration_np(&offset, sec);
#else
	struct timeval current;

	SysTime::getTimeOfDay(&current);
	spec->tv_sec = current.tv_sec + ((timer + current.tv_usec / 1000) / 1000);
	spec->tv_nsec = ((current.tv_usec / 1000 + timer) % 1000) * 1000000;

#endif
	return spec;
}

void	wait(signo_t signo)
{
	sigset_t mask;

	sigemptyset(&mask);
	sigaddset(&mask, signo);
	sigwait(&mask, &signo);
}




void	Thread::sleep(timeout_t timeout)
{
	struct timespec timer;
	timer.tv_sec = timeout / 1000;
	timer.tv_nsec = (timeout % 1000) * 1000000;
	nanosleep(&timer,NULL);
}




}

