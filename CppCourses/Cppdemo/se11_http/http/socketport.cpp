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

#include <common.h>
#include <bthread.h>
#include <address.h>
#include <socket.h>
#include <socketport.h>
#include <poller.h>
#include "private.h"
#include <cerrno>
#define socket_errno errno

#ifndef	INADDR_LOOPBACK
#define	INADDR_LOOPBACK	(unsigned long)0x7f000001
#endif

namespace bbt{

SocketPort::SocketPort(SocketService *svc, TCPSocket &tcp) :
Socket(accept(tcp.getSocket(), NULL, NULL))
{
	detect_pending = true;
	detect_output = false;
	detect_disconnect = true;

	next = prev = NULL;
	service = NULL;

	// FIXME: use macro here and in other files...
	if(so > -1)
	{
		setError(false);
    if( svc )
  		svc->attach(this);
	}
}

#ifdef  BBT_IPV6
SocketPort::SocketPort(SocketService *svc, TCPV6Socket &tcp) :
Socket(accept(tcp.getSocket(), NULL, NULL))
{
	detect_pending = true;
	detect_output = false;
	detect_disconnect = true;

	next = prev = NULL;
	service = NULL;

	// FIXME: use macro here and in other files...
	if(so > -1)
	{
		setError(false);
    if( svc )
  		svc->attach(this);
	}
}
#endif

SocketPort::SocketPort(SocketService *svc, const IPV4Address &ia, tpport_t port) :
Socket(AF_INET, SOCK_DGRAM, 0)
{
	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	next = prev = NULL;
	service = NULL;
	addr.sin_family = AF_INET;
	addr.sin_addr = getaddress(ia);
	addr.sin_port = htons(port);
	detect_pending = true;
	detect_output = false;
	detect_disconnect = true;

	if(bind(so, (struct sockaddr *)&addr, (socklen_t)sizeof(addr)))
	{
		endSocket();
		error(errBindingFailed,"Could not bind socket",socket_errno);
		return;
	}
	state = BOUND;
	setError(false);

  if(svc)
  	svc->attach(this);
}

#ifdef  BBT_IPV6
SocketPort::SocketPort(SocketService *svc, const IPV6Address &ia, tpport_t port) :
Socket(AF_INET6, SOCK_DGRAM, 0)
{
	struct sockaddr_in6 addr;

	memset(&addr, 0, sizeof(addr));
	next = prev = NULL;
	service = NULL;
	addr.sin6_family = AF_INET6;
	addr.sin6_addr = getaddress(ia);
	addr.sin6_port = htons(port);
	detect_pending = true;
	detect_output = false;
	detect_disconnect = true;

	if(bind(so, (struct sockaddr *)&addr, (socklen_t)sizeof(addr)))
	{
		endSocket();
		error(errBindingFailed,"Could not bind socket",socket_errno);
		return;
	}
	state = BOUND;
	setError(false);

  if(svc)
  	svc->attach(this);
}
#endif

SocketPort::SocketPort(SocketService *svc, const IPV4Host &ih, tpport_t port) :
Socket(AF_INET, SOCK_STREAM, 0)
{
	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	next = prev = NULL;
	service = NULL;
	addr.sin_family = AF_INET;
	addr.sin_addr = getaddress(ih);
	addr.sin_port = htons(port);
	detect_pending = true;
	detect_disconnect = true;

	long opts = fcntl(so, F_GETFL);
	fcntl(so, F_SETFL, opts | O_NDELAY);

	int rtn = ::connect(so, (struct sockaddr *)&addr, (socklen_t)sizeof(addr));

	if(!rtn)
	{
		state = CONNECTED;
	}
	else
	{
		if(errno == EINPROGRESS)
		{
			state = CONNECTING;
		}
		else
		{
			endSocket();
			connectError();
			return;
		}
	}

	fcntl(so, F_SETFL, opts);
	setError(false);
	detect_output = (state == CONNECTING);

	if(svc)
  		svc->attach(this);

//	if(state == CONNECTING)
//		setDetectOutput(true);
}

#ifdef  BBT_IPV6
SocketPort::SocketPort(SocketService *svc, const IPV6Host &ih, tpport_t port) :
Socket(AF_INET6, SOCK_STREAM, 0)
{
	struct sockaddr_in6 addr;

	memset(&addr, 0, sizeof(addr));
	next = prev = NULL;
	service = NULL;
	addr.sin6_family = AF_INET6;
	addr.sin6_addr = getaddress(ih);
	addr.sin6_port = htons(port);
	detect_pending = true;
	detect_disconnect = true;

	long opts = fcntl(so, F_GETFL);
	fcntl(so, F_SETFL, opts | O_NDELAY);

	int rtn = ::connect(so, (struct sockaddr *)&addr, (socklen_t)sizeof(addr));

	if(!rtn)
	{
		state = CONNECTED;
	}
	else
	{
		if(errno == EINPROGRESS)
		{
			state = CONNECTING;
		}
		else
		{
			endSocket();
			connectError();
			return;
		}
	}

	fcntl(so, F_SETFL, opts);
	setError(false);
	detect_output = (state == CONNECTING);

	if(svc)
  		svc->attach(this);

//	if(state == CONNECTING)
//		setDetectOutput(true);
}
#endif

SocketPort::~SocketPort()
{
	if(service) {
		service->detach(this);
	}
	endSocket();
}

void SocketPort::expired(void)
{
}

void SocketPort::pending(void)
{
}

void SocketPort::output(void)
{
}

void SocketPort::disconnect(void)
{
}

void SocketPort::attach( SocketService* svc )
{
	if(service)
		service->detach(this);
	service = svc;	
	if(svc)
		svc->attach(this);
}

Socket::Error SocketPort::connect(const IPV4Address &ia, tpport_t port)
{
	struct sockaddr_in addr;
	Error rtn = errSuccess;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr = getaddress(ia);
	addr.sin_port = htons(port);

	long opts = fcntl(so, F_GETFL);
	fcntl(so, F_SETFL, opts | O_NDELAY);
	// Win32 will crash if you try to connect to INADDR_ANY.
	if ( INADDR_ANY == addr.sin_addr.s_addr )
	        addr.sin_addr.s_addr = INADDR_LOOPBACK;
	if(::connect(so, (struct sockaddr *)&addr, (socklen_t)sizeof(addr)))
		rtn = connectError();
	fcntl(so, F_SETFL, opts);
	return rtn;
}

#ifdef  BBT_IPV6
Socket::Error SocketPort::connect(const IPV6Address &ia, tpport_t port)
{
	struct sockaddr_in6 addr;
	Error rtn = errSuccess;

	memset(&addr, 0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	addr.sin6_addr = getaddress(ia);
	addr.sin6_port = htons(port);

	long opts = fcntl(so, F_GETFL);
	fcntl(so, F_SETFL, opts | O_NDELAY);
	if(!memcmp(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any)))
		memcpy(&addr.sin6_addr, &in6addr_loopback, sizeof(in6addr_loopback));
	if(::connect(so, (struct sockaddr *)&addr, (socklen_t)sizeof(addr)))
		rtn = connectError();
	fcntl(so, F_SETFL, opts);
	return rtn;
}

#endif

void SocketPort::setTimer(timeout_t ptimer)
{
	TimerPort::setTimer(ptimer);
  if( service )
  	service->update();
}

void SocketPort::incTimer(timeout_t ptimer)
{
	TimerPort::incTimer(ptimer);
  if( service )
  	service->update();
}

void SocketPort::setDetectPending( bool val )
{
	if ( detect_pending != val ) {
		detect_pending = val;
#ifdef USE_POLL
		if ( ufd ) {
			if ( val ) {
				ufd->events |= POLLIN;
			} else {
				ufd->events &= ~POLLIN;
			}
		}
#endif
    if( service )
  		service->update();
	}
}

void SocketPort::setDetectOutput( bool val )
{
	if ( detect_output != val ) {
		detect_output = val;
#ifdef USE_POLL
		if ( ufd ) {
			if ( val ) {
				ufd->events |= POLLOUT;
			} else {
				ufd->events &= ~POLLOUT;
			}
		}
#endif
    if( service )
  		service->update();
	}
}


SocketService::SocketService(int pri, size_t stack, const char *id) :
Thread(pri, stack), Mutex(id)
{
	first = last = NULL;
	count = 0;
	FD_ZERO(&connect);
	long opt;
	::pipe(iosync);
	hiwater = iosync[0] + 1;
	FD_SET(iosync[0], &connect);

	opt = fcntl(iosync[0], F_GETFL);
	fcntl(iosync[0], F_SETFL, opt | O_NDELAY);	
}

SocketService::~SocketService()
{
	update(0);

	terminate();

	while(first)
		delete first;
}

void SocketService::onUpdate(unsigned char buf)
{
}

void SocketService::onEvent(void)
{
}

void SocketService::onCallback(SocketPort *port)
{
}

void SocketService::attach(SocketPort *port)
{
	enterMutex();
#ifdef	USE_POLL
	port->ufd = 0;
#endif
	if(last)
		last->next = port;

	port->prev = last;
	last = port;
	FD_SET(port->so, &connect);
	if(port->so >= hiwater)
		hiwater = port->so + 1;
	port->service = this;

	++count;
	if(!first) first = port;
	
	// start thread if necessary
	if (count == 1)
	{
		if (!isRunning())
		{
			leaveMutex();
			start();
			return;
		}
	}
	leaveMutex();
	update();
}

void SocketService::detach(SocketPort *port)
{
	enterMutex();
#if !defined(USE_POLL) 
	FD_CLR(port->so, &connect);
#endif
	if(port->prev) {
		port->prev->next = port->next;
	} else {
		first = port->next;
	}
	
	if(port->next) {
		port->next->prev = port->prev;
	} else {
		last = port->prev;
	}
	port->service = NULL;
	
	--count;
	leaveMutex();
	update();
}



void SocketService::update(unsigned char flag)
{
	::write(iosync[1], (char *)&flag, 1);
}

#define MUTEX_START { MutexLock _lock_(*this);
#define MUTEX_END }

void SocketService::run(void)
{
	timeout_t timer, expires;
	SocketPort *port;
	unsigned char buf;

#ifdef 	USE_POLL
	
	Poller			  mfd;
	pollfd			* p_ufd;
	int				  lastcount = 0;

	// initialize ufd in all attached ports :
	// probably don't need this but it can't hurt.
	enterMutex();
	port = first;
	while(port)
	{
		port->ufd = 0;
		port = port->next;
	}
	leaveMutex();
	
#else
	struct timeval timeout, *tvp;
	fd_set inp, out, err;
	FD_ZERO(&inp);
	FD_ZERO(&out);
	FD_ZERO(&err);
	int so;
#endif


	setCancel(cancelDeferred);
	for(;;)
	{
		timer = TIMEOUT_INF;
		while(1 == ::read(iosync[0], (char *)&buf, 1))
		{
			if(buf)
			{
				onUpdate(buf);
				continue;
			}

			setCancel(cancelImmediate);
			sleep(TIMEOUT_INF);
			exit();
		}

#ifdef	USE_POLL

		bool	reallocate = false;
		
		MUTEX_START
		onEvent();
		port = first;
		while(port)
		{
			onCallback(port);
			if ( ( p_ufd = port->ufd ) ) {

				if ( ( POLLHUP | POLLNVAL ) & p_ufd->revents ) {
					// Avoid infinite loop from disconnected sockets
					port->detect_disconnect = false;
					p_ufd->events &= ~POLLHUP;

					SocketPort* p = port;
					port = port->next;
					detach(p);
					reallocate = true;
					p->disconnect();
					continue;
				}
	
				if ( ( POLLIN | POLLPRI ) & p_ufd->revents )
					port->pending();
	
				if ( POLLOUT & p_ufd->revents )
					port->output();

			} else {
				reallocate = true;
			}

retry:
			expires = port->getTimer();

			if(expires > 0)
				if(expires < timer)
					timer = expires;

			if(!expires)
			{
				port->endTimer();
				port->expired();
				goto retry;
			}

			port = port->next;
		}

		//
		// reallocate things if we saw a ServerPort without
		// ufd set !
		if ( reallocate || ( ( count + 1 ) != lastcount ) ) {
			lastcount = count + 1;
			p_ufd = mfd.getList( count + 1 );
	
			// Set up iosync polling
			p_ufd->fd = iosync[0];
			p_ufd->events = POLLIN | POLLHUP;
			p_ufd ++;
			
			port = first;
			while(port)
			{
				p_ufd->fd = port->so;
				p_ufd->events =
					( port->detect_disconnect ? POLLHUP : 0 )
					| ( port->detect_output ? POLLOUT : 0 )
					| ( port->detect_pending ? POLLIN : 0 )
				;
				port->ufd = p_ufd;
				p_ufd ++;
				port = port->next;
			}
		}
		MUTEX_END
		poll( mfd.getList(), lastcount, timer );

#else
		MUTEX_START
		onEvent();
		port = first;
		while(port)
		{
			onCallback(port);
			so = port->so;
			if(FD_ISSET(so, &err)) {
				port->detect_disconnect = false;
				
				SocketPort* p = port;
				port = port->next;
				p->disconnect();
				continue;
			}

			if(FD_ISSET(so, &inp))
				port->pending();

			if(FD_ISSET(so, &out))
				port->output();

retry:
			expires = port->getTimer();
			if(expires > 0)
				if(expires < timer)
					timer = expires;

			// if we expire, get new scheduling now

			if(!expires)
			{
				port->endTimer();
				port->expired();
				goto retry;
			}

			port = port->next;
		}

		FD_ZERO(&inp);
		FD_ZERO(&out);
		FD_ZERO(&err);
		FD_SET(iosync[0],&inp);
		port = first;
		while(port)
		{
			so = port->so;
			if(port->detect_pending)
				FD_SET(so, &inp);

			if(port->detect_output)
				FD_SET(so, &out);

			if(port->detect_disconnect)
				FD_SET(so, &err);

			port = port->next;
		}
		
		MUTEX_END
		if(timer == TIMEOUT_INF)
			tvp = NULL;
		else
		{
			tvp = &timeout;
			timeout.tv_sec = timer / 1000;
			timeout.tv_usec = (timer % 1000) * 1000;
		}
		select(hiwater, &inp, &out, &err, tvp);		
#endif
	}
}						

}
