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

#include <iostream>
#include <common.h>
#include <exception.h>
#include <bthread.h>
#include <address.h>
#include <socket.h>
#include "private.h"
// #include "nat.h"

#define socket_errno errno
# ifdef	IPPROTO_IP
#  ifndef  SOL_IP
#   define SOL_IP	IPPROTO_IP
#  endif // !SOL_IP
# endif	 // IPPROTO_IP

#ifndef	INADDR_LOOPBACK
#define	INADDR_LOOPBACK	(unsigned long)0x7f000001
#endif

namespace bbt {
using namespace std;

# define DUP_SOCK(s) dup(s)

Mutex Socket::mutex;

bool Socket::check(Family fam)
{
	SOCKET so = INVALID_SOCKET;

	switch(fam)
	{
	case IPV4:
		so = socket(fam, SOCK_DGRAM, 0);
		break;
#ifdef	BBT_IPV6
	case IPV6:
		so = socket(fam, SOCK_DGRAM, 0);
		break;
#endif
	}

	if(so == INVALID_SOCKET)
		return false;

	close(so);
	return true;
}

Socket::Socket()
{
	setSocket();
}

Socket::Socket(int domain, int type, int protocol)
{
	setSocket();
	so = socket(domain, type, protocol);
	if(so == INVALID_SOCKET)
	{
		error(errCreateFailed,"Could not create socket",socket_errno);
		return;
	}
#ifdef	SO_NOSIGPIPE
	int opt = 1;
	setsockopt(so, SOL_SOCKET, SO_NOSIGPIPE, (char *)&opt, sizeof(opt));
#endif
	state = AVAILABLE;
}

Socket::Socket(SOCKET fd)
{
	setSocket();
	if (fd == INVALID_SOCKET)
	{
		error(errCreateFailed,"Invalid socket handle passed",0);
		return;
	}
	so = fd;
	state = AVAILABLE;
}

Socket::Socket(const Socket &orig)
{
	setSocket();
	so = DUP_SOCK(orig.so);
	if(so == INVALID_SOCKET)
		error(errCopyFailed,"Could not duplicate socket handle",socket_errno);
	state = orig.state;
}

Socket::~Socket()
{
	endSocket();
}

void Socket::setSocket(void)
{
	flags.thrown    = false;
	flags.broadcast = false;
	flags.route     = true;
	flags.keepalive = false;
	flags.loopback  = true;
	flags.multicast = false;
	flags.linger	= false;
	flags.ttl	= 1;
	errid           = errSuccess;
	errstr          = NULL;
	syserr          = 0;
	state           = INITIAL;
	so              = INVALID_SOCKET;
}

Socket::Error Socket::sendLimit(int limit)
{
#ifdef	SO_SNDLOWAT
	if(setsockopt(so, SOL_SOCKET, SO_SNDLOWAT, (char *)&limit, sizeof(limit)))
		return errInvalidValue;

	return errSuccess;
	
#else
	return errServiceUnavailable;
#endif
}

Socket::Error Socket::receiveLimit(int limit)
{
#ifdef  SO_RCVLOWAT
        if(setsockopt(so, SOL_SOCKET, SO_RCVLOWAT, (char *)&limit, sizeof(limit)))
                return errInvalidValue;

        return errSuccess;

#else
        return errServiceUnavailable;
#endif
}

Socket::Error Socket::sendTimeout(timeout_t to)
{
#if 0 // linux don't support read and writen this value
#ifdef  SO_SNDTIMEO
        struct timeval tv;

        tv.tv_sec = to / 1000;
        tv.tv_usec = (to % 1000) * 1000;

        if(setsockopt(so, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv)))
                return errInvalidValue;

        return errSuccess;

#else
        return errServiceUnavailable;
#endif
#endif 
        return errServiceUnavailable;
}

Socket::Error Socket::receiveTimeout(timeout_t to)
{
#if 0 // linux don't support read and writen this value
#ifdef  SO_RCVTIMEO
        struct timeval tv;

        tv.tv_sec = to / 1000;
        tv.tv_usec = (to % 1000) * 1000;

        if(setsockopt(so, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv)))
                return errInvalidValue;

        return errSuccess;
#else
        return errServiceUnavailable;
#endif
#endif
        return errServiceUnavailable;
}	

Socket::Error Socket::sendBuffer(unsigned bufsize)
{
#ifdef	SO_SNDBUF
	if(setsockopt(so, SOL_SOCKET, SO_SNDBUF, (char *)&bufsize, sizeof(bufsize)))
		return errInvalidValue;
	return errSuccess;
#else
	return errServiceUnavailable;
#endif
}
		
Socket::Error Socket::bufferSize(unsigned bufsize)
{
	Error err = receiveBuffer(bufsize);
	if(err == errSuccess)
		err = sendBuffer(bufsize);

	return err;
}

Socket::Error Socket::receiveBuffer(unsigned bufsize)
{
#ifdef  SO_RCVBUF
        if(setsockopt(so, SOL_SOCKET, SO_RCVBUF, (char *)&bufsize, sizeof(bufsize)))
		return errInvalidValue;
        return errSuccess;
#else
        return errServiceUnavailable; 
#endif
}

ssize_t Socket::readLine(char *str, size_t request, timeout_t timeout)
{
	bool crlf = false;
	bool nl = false;
	size_t nleft = request - 1; // leave also space for terminator
	int nstat,c;

	if(request < 1)
		return 0;

	str[0] = 0;

	while(nleft && !nl)
	{
		if(timeout)
		{
			if(!isPending(pendingInput, timeout))
			{
				error(errTimeout,"Read timeout", 0);
				return -1;
			}
		}
		nstat = ::recv(so, str, _IOLEN64 nleft, MSG_PEEK);
		if(nstat <= 0)
		{
			error(errInput,"Could not read from socket", socket_errno);
			return -1;
		}

		// FIXME: if unique char in buffer is '\r' return "\r"
		//        if buffer end in \r try to read another char?
		//        and if timeout ??
		//        remember last \r

		for(c=0; c < nstat; ++c)
		{
			if(str[c] == '\n')
			{
				if (c > 0 && str[c-1] == '\r')
					crlf = true;
				++c;
				nl = true;
				break;
			}
		}
		nstat = ::recv(so, str, c, 0);
		// TODO: correct ???
		if(nstat < 0)
			break;

		// adjust ending \r\n in \n
		if(crlf)
		{
			--nstat;
			str[nstat - 1] = '\n';
		}

		str += nstat;
		nleft -= nstat;
	}
	*str = 0;
	return (ssize_t)(request - nleft - 1);
}

ssize_t Socket::readData(void *Target, size_t Size, char Separator, timeout_t timeout)
{
  if ((Separator == 0x0D) || (Separator == 0x0A))
    return (readLine ((char *) Target, Size, timeout));

  if (Size < 1)
    return (0);

  ssize_t nstat;

  if (Separator == 0)           // Flat-out read for a number of bytes.
    {
      if (timeout)
        if (!isPending (pendingInput, timeout))
          {
            error(errTimeout);
            return (-1);
          }
      nstat =::recv (so, (char *)Target, _IOLEN64 Size, 0);

      if (nstat < 0)
        {
          error (errInput);
          return (-1);
        }
      return (nstat);
    }
  /////////////////////////////////////////////////////////////
  // Otherwise, we have a special char separator to use
  /////////////////////////////////////////////////////////////
  bool found = false;
  size_t nleft = Size;
  int c;
  char *str = (char *) Target;

  memset (str, 0, Size);

  while (nleft && !found)
    {
      if (timeout)
        if (!isPending (pendingInput, timeout))
          {
            error(errTimeout);
            return (-1);
          }

      nstat =::recv (so, str, _IOLEN64 nleft, MSG_PEEK);
      if (nstat <= 0)
        {
          error (errInput);
          return (-1);
        }

      for (c = 0; (c < nstat) && !found; ++c)
        if (str[c] == Separator)
          found = true;

      memset (str, 0, nleft); 
      nstat =::recv (so, str, c, 0);
      if (nstat < 0)
        break;

      str += nstat;
      nleft -= nstat;
    }
  return (ssize_t)(Size - (ssize_t) nleft);
}

ssize_t Socket::writeData(const void *Source, size_t Size, timeout_t timeout)
{
  if (Size < 1)
    return (0);

  ssize_t nstat;
  const char *Slide = (const char *) Source;

  while (true)
    {
      if (timeout)
        if (!isPending (pendingOutput, timeout))
          {
            error(errOutput);
            return (-1);
          }

      nstat =::send (so, Slide, _IOLEN64 Size, 0);

      if (nstat <= 0)
        {
          error(errOutput);
          return (-1);
        }
      Size -= nstat;
      Slide += Size;

      if (Size <= 0)
        break;
    }
  return (nstat);
}

bool Socket::isConnected(void) const
{
	return (Socket::state == CONNECTED) ? true : false;
}

bool Socket::isActive(void) const
{
	return (state != INITIAL) ? true : false;
}

bool Socket::operator!() const
{
	return (Socket::state == INITIAL) ? true : false;
}

void Socket::endSocket(void)
{
	if(Socket::state == STREAM)
	{
		state = INITIAL;
		if(so > -1)
		{
			SOCKET sosave = so;
			so = INVALID_SOCKET;
			close(sosave);
		}
		return;
	}

	state = INITIAL;
	if(so == INVALID_SOCKET)
		return;

#ifdef	SO_LINGER
	struct linger linger;

	if(flags.linger)
	{
		linger.l_onoff = 1;
		linger.l_linger = 60;
	}
	else
		linger.l_onoff = linger.l_linger = 0;
	setsockopt(so, SOL_SOCKET, SO_LINGER, (char *)&linger,
		(socklen_t)sizeof(linger));
#endif
	close(so);
	so = INVALID_SOCKET;
}

Socket::Error Socket::connectError(void)
{
	char* str = "Could not connect to remote host";
	switch(errno)
	{
#ifdef	EHOSTUNREACH
	case EHOSTUNREACH:
		return error(errConnectNoRoute,str,socket_errno);
#endif
#ifdef	ENETUNREACH
	case ENETUNREACH:
		return error(errConnectNoRoute,str,socket_errno);
#endif
	case EINPROGRESS:
		return error(errConnectBusy,str,socket_errno);
#ifdef	EADDRNOTAVAIL
	case EADDRNOTAVAIL:
		return error(errConnectInvalid,str,socket_errno);
#endif
	case ECONNREFUSED:
		return error(errConnectRefused,str,socket_errno);
	case ETIMEDOUT:
		return error(errConnectTimeout,str,socket_errno);
	default:
		return error(errConnectFailed,str,socket_errno);
	}
}

Socket::Error Socket::error(Error err, char *errs, long systemError) const
{
	errid  = err;
	errstr = errs;
	syserr = systemError;
	if(!err)
		return err;

	if(flags.thrown)
		return err;

	// prevents recursive throws

	flags.thrown = true;
	switch(Thread::getException())
	{
	case Thread::throwObject:
		throw((Socket *)this);
	case Thread::throwException:
		{
			if(!errs)
				errs = "";
			throw SockException(std::string(errs), err, systemError);
		}
	case Thread::throwNothing:
		break;
	}
	return err;
}

const char *Socket::getSystemErrorString(void) const
{
	SockException e(errstr, errid, syserr);
	return e.getSystemErrorString();
}

Socket::Error Socket::setBroadcast(bool enable)
{
	int opt = (enable ? 1 : 0);
	if(setsockopt(so, SOL_SOCKET, SO_BROADCAST,
		      (char *)&opt, (socklen_t)sizeof(opt)))
		return error(errBroadcastDenied,"Could not set socket broadcast option",socket_errno);
	flags.broadcast = enable;
	return errSuccess;
}

Socket::Error Socket::setKeepAlive(bool enable)
{
	int opt = (enable ? ~0: 0);
#if (defined(SO_KEEPALIVE))
	if(setsockopt(so, SOL_SOCKET, SO_KEEPALIVE,
		      (char *)&opt, (socklen_t)sizeof(opt)))
		return error(errKeepaliveDenied,"Could not set socket keep-alive option",socket_errno);
#endif
	flags.keepalive = enable;
	return errSuccess;
}

Socket::Error Socket::setLinger(bool linger)
{
#ifdef	SO_LINGER
	flags.linger = linger;
	return errSuccess;
#else
	return error(errServiceUnavailable,"Socket lingering not supported");
#endif
}

Socket::Error Socket::setRouting(bool enable)
{
	int opt = (enable ? 1 : 0);

#ifdef	SO_DONTROUTE
	if(setsockopt(so, SOL_SOCKET, SO_DONTROUTE,
		      (char *)&opt, (socklen_t)sizeof(opt)))
		return error(errRoutingDenied,"Could not set dont-route socket option",socket_errno);
#endif
	flags.route = enable;
	return errSuccess;
}

Socket::Error Socket::setNoDelay(bool enable)
{
	int opt = (enable ? 1 : 0);

	if(setsockopt(so, IPPROTO_TCP, TCP_NODELAY,
		      (char *)&opt, (socklen_t)sizeof(opt)))
		return error(errNoDelay,"Could not set tcp-nodelay socket option",socket_errno);


	return errSuccess;
}

Socket::Error Socket::setTypeOfService(Tos service)
{
#ifdef	SOL_IP
	unsigned char tos;
	switch(service)
	{
#ifdef 	IPTOS_LOWDELAY
	case tosLowDelay:
		tos = IPTOS_LOWDELAY;
		break;
#endif
#ifdef 	IPTOS_THROUGHPUT
	case tosThroughput:
		tos = IPTOS_THROUGHPUT;
		break;
#endif
#ifdef	IPTOS_RELIABILITY
	case tosReliability:
		tos = IPTOS_RELIABILITY;
		break;
#endif
#ifdef	IPTOS_MINCOST
	case tosMinCost:
		tos = IPTOS_MINCOST;
		break;
#endif
	default:
		return error(errServiceUnavailable,"Unknown type-of-service");
	}
	if(setsockopt(so, SOL_IP, IP_TOS,(char *)&tos, (socklen_t)sizeof(tos)))
		return error(errServiceDenied,"Could not set type-of-service",socket_errno);
	return errSuccess;
#else
	return error(errServiceUnavailable,"Socket type-of-service not supported",socket_errno);
#endif
}

Socket::Error Socket::setTimeToLiveByFamily(unsigned char ttl, Family fam)
{
	if(!flags.multicast)
		return error(errMulticastDisabled,"Multicast not enabled on socket");

	switch(fam)
	{
#ifdef	BBT_IPV6
	case IPV6:
		flags.ttl = ttl;
		setsockopt(so, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, (char *)&ttl,	sizeof(ttl));
		return errSuccess;
#endif
	default:
#ifdef	IP_MULTICAST_TTL
		flags.ttl = ttl;
		setsockopt(so, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, sizeof(ttl));
		return errSuccess;
#else
		return error(errServiceUnavailable,"Multicast not supported");
#endif
	}
}

Socket::Error Socket::setLoopbackByFamily(bool enable, Family family)
{
	unsigned char loop;
	
	if(!flags.multicast)
		return error(errMulticastDisabled,"Multicast not enabled on socket");

	if(enable)
		loop = 1;
	else
		loop = 0;
	flags.loopback = enable;
	
	switch(family)
	{
#ifdef	BBT_IPV6
	case IPV6:
		setsockopt(so, IPPROTO_IPV6, IPV6_MULTICAST_LOOP, (char *)&loop, sizeof(loop));
		return errSuccess;
#endif
	default:	
#ifdef  IP_MULTICAST_LOOP
		setsockopt(so, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loop, sizeof(loop));
		return errSuccess;
#else
		return error(errServiceUnavailable,"Multicast not supported");
#endif
	}
}

bool Socket::isPending(Pending pending, timeout_t timeout)
{
	int status;
#ifdef USE_POLL
	struct pollfd pfd;

	pfd.fd = so;
	pfd.revents = 0;

	if(so == INVALID_SOCKET)
		return true;

	switch(pending)
	{
	case pendingInput:
		pfd.events = POLLIN;
		break;
	case pendingOutput:
		pfd.events = POLLOUT;
		break;
	case pendingError:
		pfd.events = POLLERR | POLLHUP;
		break;
	}

	status = 0;
	while(status < 1)
	{

		if(timeout == TIMEOUT_INF)
			status = poll(&pfd, 1, -1);
		else
			status = poll(&pfd, 1, timeout);

		if(status < 1)
		{
			// don't stop polling because of a simple
			// signal :)
			if(status == -1 && errno == EINTR)
				continue;

			return false;
		}
	}

	if(pfd.revents & pfd.events)
		return true;
	return false;
#else
	struct timeval tv;
	struct timeval *tvp = &tv;
	fd_set grp;

	if(timeout == TIMEOUT_INF)
		tvp = NULL;
	else
	{
		tv.tv_usec = (timeout % 1000) * 1000;
		tv.tv_sec = timeout / 1000;
	}

	FD_ZERO(&grp);
	SOCKET sosave = so;
	if(so == INVALID_SOCKET)
		return true;
	FD_SET(sosave, &grp);
	switch(pending)
	{
	case pendingInput:
		status = select((int)so + 1, &grp, NULL, NULL, tvp);
		break;
	case pendingOutput:
		status = select((int)so + 1, NULL, &grp, NULL, tvp);
		break;
	case pendingError:
		status = select((int)so + 1, NULL, NULL, &grp, tvp);
		break;
	}
	if(status < 1)
		return false;
	if(FD_ISSET(so, &grp))
		return true;
	return false;
#endif
}

Socket &Socket::operator=(const Socket &from)
{
	if(so == from.so)
		return *this;

	if(state != INITIAL)
		endSocket();

	so = DUP_SOCK(from.so);
	if(so == INVALID_SOCKET)
	{
		error(errCopyFailed,"Could not duplicate socket handle",socket_errno);
		state = INITIAL;
	}
	else
		state = from.state;

	return *this;
}

void Socket::setCompletion(bool immediate)
{
	flags.completion = immediate;
	int fflags = fcntl(so, F_GETFL);

	switch( immediate )
	{
	case false:
		fflags |= O_NONBLOCK;
		fcntl(so, F_SETFL, fflags);
		break;
	case true:
		fflags &=~ O_NONBLOCK;
		fcntl(so, F_SETFL, fflags);
		break;
	}
}

IPV4Host Socket::getIPV4Sender(tpport_t *port) const
{
	struct sockaddr_in from;
	char buf;
	socklen_t len = sizeof(from);
	int rc = ::recvfrom(so, &buf, 1, MSG_PEEK,
			    (struct sockaddr *)&from, &len);

	if(rc < 1)
	{
		if(port)
			*port = 0;

		memset((void*) &from, 0, sizeof(from));
		error(errInput,"Could not read from socket",socket_errno);
	}
	else
	{
		if(port)
			*port = ntohs(from.sin_port);
	}

	return IPV4Host(from.sin_addr);
}

#ifdef  BBT_IPV6
IPV6Host Socket::getIPV6Sender(tpport_t *port) const
{
	struct sockaddr_in6 from;
	char buf;
	socklen_t len = sizeof(from);
	int rc = ::recvfrom(so, &buf, 1, MSG_PEEK,
			    (struct sockaddr *)&from, &len);

	if(rc < 1)
	{
		if(port)
			*port = 0;

		memset((void*) &from, 0, sizeof(from));
		error(errInput,"Could not read from socket",socket_errno);
	}
	else
	{
		if(port)
			*port = ntohs(from.sin6_port);
	}

	return IPV6Host(from.sin6_addr);
}
#endif

IPV4Host Socket::getIPV4Local(tpport_t *port) const
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);

	if(getsockname(so, (struct sockaddr *)&addr, &len))
	{
		error(errResourceFailure,"Could not get socket address",socket_errno);
	      	if(port)
			*port = 0;
		memset(&addr.sin_addr, 0, sizeof(addr.sin_addr));
	}
	else
	{
		if(port)
		    	*port = ntohs(addr.sin_port);
	}
	return IPV4Host(addr.sin_addr);
}

#ifdef  BBT_IPV6
IPV6Host Socket::getIPV6Local(tpport_t *port) const
{
	struct sockaddr_in6 addr;
	socklen_t len = sizeof(addr);

	if(getsockname(so, (struct sockaddr *)&addr, &len))
	{
		error(errResourceFailure,"Could not get socket address",socket_errno);
	      	if(port)
			*port = 0;
		memset(&addr.sin6_addr, 0, sizeof(addr.sin6_addr));
	}
	else
	{
		if(port)
		    	*port = ntohs(addr.sin6_port);
	}
	return IPV6Host(addr.sin6_addr);
}

#endif
#if 0
IPV4Host Socket::getIPV4NAT(tpport_t *port) const
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	natResult res;

	if((res=natv4Lookup((int)so, &addr))!=natOK)
	{
	  	if(res==natNotSupported)
		  	error( errServiceUnavailable, natErrorString( res ) );
		else if(res==natSearchErr)
		  	error( errSearchErr, natErrorString( res ) );
		else
		  	error( errLookupFail, natErrorString( res ), socket_errno );
		if(port)
			*port = 0;
		memset(&addr.sin_addr, 0, sizeof(addr.sin_addr));
	}
	else
	{
		if(port)
	    		*port = ntohs(addr.sin_port);
	}
	return IPV4Host(addr.sin_addr);
}

#ifdef  BBT_IPV6
IPV6Host Socket::getIPV6NAT(tpport_t *port) const
{
	struct sockaddr_in6 addr;
	socklen_t len = sizeof(addr);
	natResult res;

	if((res=natv6Lookup(so, &addr))!=natOK)
	{
	  	if(res==natNotSupported)
		  	error( errServiceUnavailable, natErrorString( res ) );
		else if(res==natSearchErr)
		  	error( errSearchErr, natErrorString( res ) );
		else
		  	error( errLookupFail, natErrorString( res ), socket_errno );
		if(port)
			*port = 0;
		memset(&addr.sin6_addr, 0, sizeof(addr.sin6_addr));
	}
	else
	{
		if(port)
	    		*port = ntohs(addr.sin6_port);
	}
	return IPV6Host(addr.sin6_addr);
}
#endif

#endif
IPV4Host Socket::getIPV4Peer(tpport_t *port) const
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);

	if(getpeername(so, (struct sockaddr *)&addr, &len))
	{
		if(errno == ENOTCONN)
			error(errNotConnected,"Could not get peer address",socket_errno);
		else
			error(errResourceFailure,"Could not get peer address",socket_errno);
		if(port)
			*port = 0;
		memset(&addr.sin_addr, 0, sizeof(addr.sin_addr));
	}
	else
	{
		if(port)
	    		*port = ntohs(addr.sin_port);
	}
	return IPV4Host(addr.sin_addr);
}

#ifdef  BBT_IPV6
IPV6Host Socket::getIPV6Peer(tpport_t *port) const
{
	struct sockaddr_in6 addr;
	socklen_t len = sizeof(addr);

	if(getpeername(so, (struct sockaddr *)&addr, &len))
	{
		if(errno == ENOTCONN)
			error(errNotConnected,"Could not get peer address",socket_errno);
		else
			error(errResourceFailure,"Could not get peer address",socket_errno);
		if(port)
			*port = 0;
		memset(&addr.sin6_addr, 0, sizeof(addr.sin6_addr));
	}
	else
	{
		if(port)
	    		*port = ntohs(addr.sin6_port);
	}
	return IPV6Host(addr.sin6_addr);
}
#endif

Socket::Error Socket::setMulticastByFamily(bool enable, Family family)
{
	socklen_t len;

	switch(family)
	{
#ifdef	BBT_IPV6
	case IPV6:
		struct sockaddr_in6 addr;
		len = sizeof(addr);

		if(enable == flags.multicast)
			return errSuccess;

		flags.multicast = enable;
		if(enable)
			getsockname(so, (struct sockaddr *)&addr, &len);
		else
			memset(&addr.sin6_addr, 0, sizeof(addr.sin6_addr));

		setsockopt(so, IPPROTO_IPV6, IPV6_MULTICAST_IF, (char *)&addr.sin6_addr, sizeof(addr.sin6_addr));
		return errSuccess;
#endif
	default:
#ifdef	IP_MULTICAST_IF
		struct sockaddr_in addr4;
		len = sizeof(addr4);

		if(enable == flags.multicast)
			return errSuccess;

		flags.multicast = enable;
		if(enable)
			getsockname(so, (struct sockaddr *)&addr4, &len);
		else
			memset(&addr4.sin_addr, 0, sizeof(addr4.sin_addr));

		setsockopt(so, IPPROTO_IP, IP_MULTICAST_IF, (char *)&addr4.sin_addr, sizeof(addr4.sin_addr));
		return errSuccess;
#else
		return error(errServiceUnavailable,"Multicast not supported");
#endif
	}
}

Socket::Error Socket::join(const IPV4Multicast &ia)
{
#ifdef	IP_ADD_MEMBERSHIP
	struct ip_mreq group;
	struct sockaddr_in myaddr;
	socklen_t len = sizeof(myaddr);

	if(!flags.multicast)
		return error(errMulticastDisabled,"Multicast not enabled on socket");

	getsockname(so, (struct sockaddr *)&myaddr, &len);
	memcpy(&group.imr_interface, &myaddr.sin_addr, sizeof(&myaddr.sin_addr));
	group.imr_multiaddr = getaddress(ia);
	setsockopt(so, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group));
	return errSuccess;
#else
	return error(errServiceUnavailable,"Multicast not supported");
#endif
}

#ifdef  BBT_IPV6
Socket::Error Socket::join(const IPV6Multicast &ia)
{
#ifdef	IPV6_ADD_MEMBERSHIP
	struct ipv6_mreq group;
	struct sockaddr_in6 myaddr;
	socklen_t len = sizeof(myaddr);

	if(!flags.multicast)
		return error(errMulticastDisabled,"Multicast not enabled on socket");

	getsockname(so, (struct sockaddr *)&myaddr, &len);
	memcpy(&group.ipv6mr_interface, &myaddr.sin6_addr, sizeof(&myaddr.sin6_addr));
	group.ipv6mr_multiaddr = getaddress(ia);
	setsockopt(so, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, (char *)&group, sizeof(group));
	return errSuccess;
#else
	return error(errServiceUnavailable,"Multicast not supported");
#endif
}

#endif

Socket::Error Socket::drop(const IPV4Multicast &ia)
{
#ifdef	IP_DROP_MEMBERSHIP
	struct ip_mreq group;
	struct sockaddr_in myaddr;
	socklen_t len = sizeof(myaddr);

	if(!flags.multicast)
		return error(errMulticastDisabled,"Multicast not enabled on socket");

	getsockname(so, (struct sockaddr *)&myaddr, &len);
	memcpy(&group.imr_interface, &myaddr.sin_addr, sizeof(&myaddr.sin_addr));
	group.imr_multiaddr = getaddress(ia);
	setsockopt(so, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char *)&group, sizeof(group));
	return errSuccess;
#else
	return error(errServiceUnavailable,"Multicast not supported");
#endif
}

#ifdef  BBT_IPV6
Socket::Error Socket::drop(const IPV6Multicast &ia)
{
#ifdef	IPV6_DROP_MEMBERSHIP
	struct ipv6_mreq group;
	struct sockaddr_in6 myaddr;
	socklen_t len = sizeof(myaddr);

	if(!flags.multicast)
		return error(errMulticastDisabled,"Multicast not enabled on socket");

	getsockname(so, (struct sockaddr *)&myaddr, &len);
	memcpy(&group.ipv6mr_interface, &myaddr.sin6_addr, sizeof(&myaddr.sin6_addr));
	group.ipv6mr_multiaddr = getaddress(ia);
	setsockopt(so, IPPROTO_IPV6, IPV6_DROP_MEMBERSHIP, (char *)&group, sizeof(group));
	return errSuccess;
#else
	return error(errServiceUnavailable,"Multicast not supported");
#endif
}
#endif


UDPSocket::UDPSocket(const char *name, Family fam) :
Socket(fam, SOCK_DGRAM, 0)
{
	char namebuf[128], *cp;
	tpport_t port;
	struct servent *svc;
	socklen_t alen;
	struct sockaddr *addr;

	family = fam;
	
	snprintf(namebuf, sizeof(namebuf), "%s", name);
	cp = strrchr(namebuf, '/');
	if(!cp && family == IPV4)
		cp = strrchr(namebuf, ':');

	if(!cp)
	{
		cp = namebuf;
		name = "*";
	}
	else
	{
		name = namebuf;
		*(cp++) = 0;
	}
	
	if(isdigit(*cp))
		port = atoi(cp);
	else
	{
		mutex.enter();
		svc = getservbyname(cp, "udp");
		if(svc)
			port = ntohs(svc->s_port);
		mutex.leave();
		if(!svc)
		{
			error(errBindingFailed, "Could not find service", errno);
			endSocket();
			return;
		}
	}

	struct sockaddr_in addr4;
	IPV4Address ia4(name);
#ifdef	BBT_IPV6
	struct sockaddr_in6 addr6;
	IPV6Address ia6(name);
#endif
	
	switch(family)
	{
#ifdef	BBT_IPV6
	case IPV6:
		peer.ipv6.sin6_family = family;
		memset(&addr6, 0, sizeof(addr6));
		addr6.sin6_family = family;
		addr6.sin6_addr = getaddress(ia6);
		addr6.sin6_port = htons(port);
		alen = sizeof(addr6);
		addr = (struct sockaddr *)&addr6;
		break;
#endif
	case IPV4:
		peer.ipv4.sin_family = family;
		memset(&addr4, 0, sizeof(addr4));
		addr4.sin_family = family;
		addr4.sin_addr = getaddress(ia4);
		addr4.sin_port = htons(port);
		alen = sizeof(&addr4);
		addr = (struct sockaddr *)&addr4;
	}

#if defined(SO_REUSEADDR) 
        int opt = 1;
        setsockopt(so, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                (socklen_t)sizeof(opt));
#endif	

	if(!bind(so, addr, alen))
		state = BOUND;

	if(state != BOUND)
	{
		endSocket();
		error(errBindingFailed, "Count not bind socket", errno);
		return;
	}
}


UDPSocket::UDPSocket(Family fam) :
Socket(fam, SOCK_DGRAM, 0)
{
	family = fam;
	memset(&peer, 0, sizeof(peer));
	switch(fam)
	{
#ifdef	BBT_IPV6
	case IPV6:
		peer.ipv6.sin6_family = family;
		break;
#endif
	default:	
		peer.ipv4.sin_family = family;
	}
}

UDPSocket::UDPSocket(const IPV4Address &ia, tpport_t port) :
Socket(AF_INET, SOCK_DGRAM, 0)
{
	family = IPV4;
	memset(&peer.ipv4, 0, sizeof(peer));
	peer.ipv4.sin_family = AF_INET;
	peer.ipv4.sin_addr = getaddress(ia);
	peer.ipv4.sin_port = htons(port);
#if defined(SO_REUSEADDR) 
	int opt = 1;
	setsockopt(so, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, (socklen_t)sizeof(opt));
#endif
	if(bind(so, (struct sockaddr *)&peer.ipv4, sizeof(peer.ipv4)))
	{
		endSocket();
		error(errBindingFailed,"Could not bind socket",socket_errno);
		return;
	}
	state = BOUND;
}

#ifdef  BBT_IPV6
UDPSocket::UDPSocket(const IPV6Address &ia, tpport_t port) :
Socket(AF_INET6, SOCK_DGRAM, 0)
{
	family = IPV6;
	memset(&peer.ipv6, 0, sizeof(peer));
	peer.ipv6.sin6_family = AF_INET6;
	peer.ipv6.sin6_addr = getaddress(ia);
	peer.ipv6.sin6_port = htons(port);
#if defined(SO_REUSEADDR) 
	int opt = 1;
	setsockopt(so, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, (socklen_t)sizeof(opt));
#endif
	if(bind(so, (struct sockaddr *)&peer.ipv6, sizeof(peer.ipv6)))
	{
		endSocket();
		error(errBindingFailed,"Could not bind socket",socket_errno);
		return;
	}
	state = BOUND;
}
#endif

UDPSocket::~UDPSocket()
{
	endSocket();
}

ssize_t UDPSocket::send(const void *buf, size_t len)
{
	struct sockaddr *addr;
	socklen_t alen;

	switch(family)
	{
#ifdef	BBT_IPV6
	case IPV6:
		addr = (struct sockaddr *)&peer.ipv6;
		alen = sizeof(struct sockaddr_in6);
		break;
#endif
	case IPV4:
		addr = (struct sockaddr *)&peer.ipv4;
		alen = sizeof(struct sockaddr_in);
	}

	if(isConnected())
	{
		addr = NULL;
		alen = 0;
	}
	
	return _IORET64 ::sendto(so, (const char *)buf, _IOLEN64 len, 0, addr, alen);
}

ssize_t UDPSocket::receive(void *buf, size_t len, bool reply)
{
	struct sockaddr *addr;
	struct sockaddr_in senderAddress;  // DMC 2/7/05 ADD for use below.
	socklen_t alen;

	switch(family)
	{
#ifdef	BBT_IPV6
	case IPV6:
		addr = (struct sockaddr *)&peer.ipv6;
		alen = sizeof(struct sockaddr_in6);
		break;
#endif
	case IPV4:
		addr = (struct sockaddr *)&peer.ipv4;
		alen = sizeof(struct sockaddr_in);
	}

	if(isConnected() || !reply)
	{
		// DMC 2/7/05 MOD to use senderAddress instead of NULL, to prevent 10014 error
		// from recvfrom.
		//addr = NULL;
		//alen = 0;
		addr = (struct sockaddr*)(&senderAddress);
		alen = sizeof(struct sockaddr_in);
	}
	
	int bytes = ::recvfrom(so, (char *)buf, _IOLEN64 len, 0, addr, &alen);


	return _IORET64 bytes;
}

Socket::Error UDPSocket::join(const IPV4Multicast &ia,int InterfaceIndex)
{


#if defined(IP_ADD_MEMBERSHIP)
	// if no by index, use INADDR_ANY
	struct ip_mreq group;
	struct sockaddr_in myaddr;
	socklen_t len = sizeof(myaddr);

	if(!flags.multicast)
		return error(errMulticastDisabled);

	getsockname(so, (struct sockaddr *)&myaddr, &len);
	memset(&group, sizeof(group), 0);
	group.imr_multiaddr.s_addr = ia.getAddress().s_addr;
	group.imr_interface.s_addr = INADDR_ANY;
	setsockopt(so, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group));
	return errSuccess;
#else
        return error(errServiceUnavailable);
#endif
}


Socket::Error UDPSocket::getInterfaceIndex(const char *DeviceName,int& InterfaceIndex)
{
  return error(errServiceUnavailable);
}

#ifdef	AF_UNSPEC
Socket::Error UDPSocket::disconnect(void)
{
	struct sockaddr_in addr;
        int len = sizeof(addr);

	if(so == INVALID_SOCKET)
		return errSuccess;

	Socket::state = BOUND;

        memset(&addr, 0, len);
        addr.sin_family = AF_UNSPEC;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_NONE;
        if(::connect(so, (sockaddr *)&addr, len))
                return connectError();
        return errSuccess;
}
#else   
Socket::Error UDPSocket::disconnect(void)
{
	if(so == INVALID_SOCKET)
		return errSuccess;

	Socket::state = BOUND;
        return connect(getLocal());
}
#endif

void UDPSocket::setPeer(const IPV4Host &ia, tpport_t port)
{
	memset(&peer.ipv4, 0, sizeof(peer.ipv4));
	peer.ipv4.sin_family = AF_INET;
	peer.ipv4.sin_addr = getaddress(ia);
	peer.ipv4.sin_port = htons(port);
}

void UDPSocket::connect(const IPV4Host &ia, tpport_t port)
{
	setPeer(ia, port);
	if(so == INVALID_SOCKET)
		return;

	if(!::connect(so, (struct sockaddr *)&peer.ipv4, sizeof(struct sockaddr_in)))
		Socket::state = CONNECTED;
}

#ifdef	BBT_IPV6
void UDPSocket::setPeer(const IPV6Host &ia, tpport_t port)
{        
	memset(&peer.ipv6, 0, sizeof(peer.ipv6));
        peer.ipv6.sin6_family = AF_INET6;
        peer.ipv6.sin6_addr = getaddress(ia);
        peer.ipv6.sin6_port = htons(port);
}

void UDPSocket::connect(const IPV6Host &ia, tpport_t port)
{
	setPeer(ia, port);

	if(so == INVALID_SOCKET)
		return;

	if(!::connect(so, (struct sockaddr *)&peer.ipv6, sizeof(struct sockaddr_in6)))
		Socket::state = CONNECTED;

}

#endif


void UDPSocket::setPeer(const char *name)
{
	char namebuf[128], *cp;
	struct servent *svc;
	tpport_t port;
	struct sockaddr *addr;
	socklen_t alen;

        snprintf(namebuf, sizeof(namebuf), "%s", name);
        cp = strrchr(namebuf, '/');
        if(!cp)
                cp = strrchr(namebuf, ':');

	if(!cp)
		return;
		
	if(isdigit(*cp))
		port = atoi(cp);
	else
	{
		mutex.enter();
		svc = getservbyname(cp, "udp");
		if(svc)
			port = ntohs(svc->s_port);
		mutex.leave();
		if(!svc)
			return;
	}
	
	memset(&peer, 0, sizeof(peer));
	
	switch(family)
	{
#ifdef	BBT_IPV6
	case IPV6:
		setPeer(IPV6Host(namebuf), port);
		break;
#endif
	case IPV4:
		setPeer(IPV4Host(namebuf), port);
	}	
}


void UDPSocket::connect(const char *service)
{
	int rtn;

	setPeer(service);

	if(so == INVALID_SOCKET)
		return;

	switch(family)
	{
#ifdef	BBT_IPV6
	case IPV6:
		rtn = ::connect(so, (struct sockaddr *)&peer.ipv6, sizeof(struct sockaddr_in6));
		break;
#endif
	case IPV4:
		rtn = ::connect(so, (struct sockaddr *)&peer.ipv4, sizeof(struct sockaddr_in));
	}
	if(!rtn)
		Socket::state = CONNECTED;
}

IPV4Host UDPSocket::getIPV4Peer(tpport_t *port) const
{
	// FIXME: insufficient buffer
	//        how to retrieve peer ??
	char buf;
	socklen_t len = sizeof(peer.ipv4);
	int rtn = ::recvfrom(so, &buf, 1, MSG_PEEK, (struct sockaddr *)&peer.ipv4, &len);

	if(rtn < 1)
	{
		if(port)
			*port = 0;

		memset((void*) &peer.ipv4, 0, sizeof(peer.ipv4));
	}
	else
	{
		if(port)
			*port = ntohs(peer.ipv4.sin_port);
	}
	return IPV4Host(peer.ipv4.sin_addr);
}

#ifdef  BBT_IPV6
IPV6Host UDPSocket::getIPV6Peer(tpport_t *port) const
{
	// FIXME: insufficient buffer
	//        how to retrieve peer ??
	char buf;
	socklen_t len = sizeof(peer.ipv6);
	int rtn = ::recvfrom(so, &buf, 1, MSG_PEEK, (struct sockaddr *)&peer.ipv6, &len);

	if(rtn < 1)
	{
		if(port)
			*port = 0;

		memset((void*) &peer.ipv6, 0, sizeof(peer.ipv6));
	}
	else
	{
		if(port)
			*port = ntohs(peer.ipv6.sin6_port);
	}
	return IPV6Host(peer.ipv6.sin6_addr);
}
#endif

UDPBroadcast::UDPBroadcast(const IPV4Address &ia, tpport_t port) :
UDPSocket(ia, port)
{
	if(so != INVALID_SOCKET)
		setBroadcast(true);
}

void UDPBroadcast::setPeer(const IPV4Broadcast &ia, tpport_t port)
{
	memset(&peer.ipv4, 0, sizeof(peer.ipv4));
	peer.ipv4.sin_family = AF_INET;
	peer.ipv4.sin_addr = getaddress(ia);
	peer.ipv4.sin_port = htons(port);
}

void TCPSocket::setSegmentSize(unsigned mss)
{
#ifdef	TCP_MAXSEG
	if(mss > 1)
		setsockopt(so, IPPROTO_TCP, TCP_MAXSEG, (char *)&mss, sizeof(mss));
#endif
	segsize = mss;
}

TCPSocket::TCPSocket(const char *name, unsigned backlog, unsigned mss) :
Socket(AF_INET, SOCK_STREAM, 0)
{
	char namebuf[128], *cp;
        struct sockaddr_in addr;
	struct servent *svc;

	memset(&addr, 0, sizeof(addr));
        snprintf(namebuf, sizeof(namebuf), "%s", name);
        cp = strrchr(namebuf, '/');
        if(!cp)
                cp = strrchr(namebuf, ':');

        if(!cp)
        {
                cp = namebuf;
                name = "*";
        }
        else
        {
                name = namebuf;
                *(cp++) = 0;
        }

	addr.sin_family = AF_INET;
	if(isdigit(*cp))
		addr.sin_port = htons(atoi(cp));
	else
	{
		mutex.enter();
		svc = getservbyname(cp, "tcp");
		if(svc)
			addr.sin_port = svc->s_port;
		mutex.leave();
		if(!svc)
		{
                	endSocket();
                	error(errBindingFailed, "Could not find service", errno);
                	return;

		}
	}

	IPV4Address ia(name);
	addr.sin_addr = getaddress(ia);

#if defined(SO_REUSEADDR) 
        int opt = 1;    
        setsockopt(so, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
		(socklen_t)sizeof(opt));
#endif      

        if(bind(so, (struct sockaddr *)&addr, sizeof(addr)))
        {
                endSocket();
                error(errBindingFailed,"Could not bind socket",socket_errno);
                return;
        }

	setSegmentSize(mss);
        if(listen(so, backlog)) 
        {
                endSocket();
                error(errBindingFailed,"Could not listen on socket",
			socket_errno);
                return;
        }
        state = BOUND;
}


TCPSocket::TCPSocket(const IPV4Address &ia, tpport_t port, unsigned backlog, unsigned mss) :
Socket(AF_INET, SOCK_STREAM, 0)
{
	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr = getaddress(ia);
	addr.sin_port = htons(port);

#if defined(SO_REUSEADDR) 
	int opt = 1;
	setsockopt(so, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, (socklen_t)sizeof(opt));
#endif
	if(bind(so, (struct sockaddr *)&addr, sizeof(addr)))
	{
		endSocket();
		error(errBindingFailed,"Could not bind socket",socket_errno);
		return;
	}

	setSegmentSize(mss);

	if(listen(so, backlog))
	{
		endSocket();
		error(errBindingFailed,"Could not listen on socket",socket_errno);
		return;
	}
	state = BOUND;
}

bool TCPSocket::onAccept(const IPV4Host &ia, tpport_t port)
{
	return true;
}

TCPSocket::~TCPSocket()
{
	endSocket();
}

#ifdef  BBT_IPV6

void TCPV6Socket::setSegmentSize(unsigned mss)
{
#ifdef	TCP_MAXSEG
	if(mss > 1)
		setsockopt(so, IPPROTO_TCP, TCP_MAXSEG, (char *)&mss, sizeof(mss));
#endif
	segsize = mss;
}


TCPV6Socket::TCPV6Socket(const char *name, unsigned backlog, unsigned mss) :
Socket(AF_INET6, SOCK_STREAM, 0)
{
	char namebuf[128], *cp;
        struct sockaddr_in6 addr;
	struct servent *svc;

	memset(&addr, 0, sizeof(addr));
        snprintf(namebuf, sizeof(namebuf), "%s", name);
        cp = strrchr(namebuf, '/');

        if(!cp)
        {
                cp = namebuf;
                name = "*";
        }
        else
        {
                name = namebuf;
                *(cp++) = 0;
        }

	addr.sin6_family = AF_INET6;
	if(isdigit(*cp))
		addr.sin6_port = htons(atoi(cp));
	else
	{
		mutex.enter();
		svc = getservbyname(cp, "tcp");
		if(svc)
			addr.sin6_port = svc->s_port;
		mutex.leave();
		if(!svc)
		{
                	endSocket();
                	error(errBindingFailed, "Could not find service", errno);
                	return;

		}
	}

	IPV6Address ia(name);
	addr.sin6_addr = getaddress(ia);

#if defined(SO_REUSEADDR) 
        int opt = 1;    
        setsockopt(so, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
		(socklen_t)sizeof(opt));
#endif      

        if(bind(so, (struct sockaddr *)&addr, sizeof(addr)))
        {
                endSocket();
                error(errBindingFailed,"Could not bind socket",socket_errno);
                return;
        }

	setSegmentSize(mss);

        if(listen(so, backlog)) 
        {
                endSocket();
                error(errBindingFailed,"Could not listen on socket",
			socket_errno);
                return;
        }
        state = BOUND;
}


TCPV6Socket::TCPV6Socket(const IPV6Address &ia, tpport_t port, unsigned backlog, unsigned mss) :
Socket(AF_INET6, SOCK_STREAM, 0)
{
	struct sockaddr_in6 addr;

	memset(&addr, 0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	addr.sin6_addr = getaddress(ia);
	addr.sin6_port = htons(port);

#if defined(SO_REUSEADDR) 
	int opt = 1;
	setsockopt(so, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, (socklen_t)sizeof(opt));
#endif
	if(bind(so, (struct sockaddr *)&addr, sizeof(addr)))
	{
		endSocket();
		error(errBindingFailed,"Could not bind socket",socket_errno);
		return;
	}

	setSegmentSize(mss);

	if(listen(so, backlog))
	{
		endSocket();
		error(errBindingFailed,"Could not listen on socket",socket_errno);
		return;
	}
	state = BOUND;
}

bool TCPV6Socket::onAccept(const IPV6Host &ia, tpport_t port)
{
	return true;
}

TCPV6Socket::~TCPV6Socket()
{
	endSocket();
}

#endif

void TCPSocket::reject(void)
{
	SOCKET rej = accept(so, NULL, NULL);
	shutdown(rej, 2);
	close(rej);
}

#ifdef	BBT_IPV6
void TCPV6Socket::reject(void)
{
        SOCKET rej = accept(so, NULL, NULL);
        shutdown(rej, 2);
        close(rej);
}
#endif

TCPStream::TCPStream(TCPSocket &server, bool throwflag, timeout_t to) :
	streambuf(), Socket(accept(server.getSocket(), NULL, NULL)),
	iostream((streambuf *)this)
	,bufsize(0)
	,gbuf(NULL)
	,pbuf(NULL)
{
	tpport_t port;
	family = IPV4;

	timeout = to;
	setError(throwflag);
	IPV4Host host = getPeer(&port);
	if(!server.onAccept(host, port))
	{
		endSocket();
		error(errConnectRejected);
		clear(ios::failbit | rdstate());
		return;
	}

	segmentBuffering(server.getSegmentSize());
	Socket::state = CONNECTED;
}

#ifdef  BBT_IPV6
TCPStream::TCPStream(TCPV6Socket &server, bool throwflag, timeout_t to) :
	streambuf(), Socket(accept(server.getSocket(), NULL, NULL)),
	iostream((streambuf *)this)
	,bufsize(0)
	,gbuf(NULL)
	,pbuf(NULL)
{
	tpport_t port;

	family = IPV6;


	timeout = to;
	setError(throwflag);
	IPV6Host host = getIPV6Peer(&port);
	if(!server.onAccept(host, port))
	{
		endSocket();
		error(errConnectRejected);
		clear(ios::failbit | rdstate());
		return;
	}

	segmentBuffering(server.getSegmentSize());
	Socket::state = CONNECTED;
}
#endif

TCPStream::TCPStream(const IPV4Host &host, tpport_t port, unsigned size, bool throwflag, timeout_t to) :
	streambuf(), Socket(AF_INET, SOCK_STREAM, 0),
        iostream((streambuf *)this),
	bufsize(0),gbuf(NULL),pbuf(NULL)
{
	family = IPV4;
	timeout = to;
	setError(throwflag);
	connect(host, port, size);
}

#ifdef  BBT_IPV6
TCPStream::TCPStream(const IPV6Host &host, tpport_t port, unsigned size, bool throwflag, timeout_t to) :
	streambuf(), Socket(AF_INET6, SOCK_STREAM, 0),
        iostream((streambuf *)this),
	bufsize(0),gbuf(NULL),pbuf(NULL)
{
	family = IPV6;

	timeout = to;
	setError(throwflag);
	connect(host, port, size);
}
#endif

TCPStream::~TCPStream()
{
#ifdef  BBT_EXCEPTIONS
                try { endStream(); }
                catch( ... ) { if ( ! std::uncaught_exception()) throw;}; 
#else
                endStream();
#endif
}


void TCPStream::connect(const char *target, unsigned mss)
{
        char namebuf[128];
        char *cp;
        bool connected = false;
	struct servent *svc;
	tpport_t port;

        snprintf(namebuf, sizeof(namebuf), "%s", target);
        cp = strrchr(namebuf, '/');
        if(!cp)
                cp = strrchr(namebuf, ':');

        if(!cp)
        {
                endStream();
                connectError();
                return;
        }

	*(cp++) = 0;

	if(isdigit(*cp))
		port = atoi(cp);
	else
	{
		mutex.enter();
		svc = getservbyname(cp, "tcp");
		if(svc)
			port = ntohs(svc->s_port);
		mutex.leave();
		if(!svc)
		{
			endStream();
			connectError();
			return;
		}
	}
	
	switch(family)
	{
	case IPV4:
		connect(IPV4Host(namebuf), port, mss);
		break;
#ifdef	BBT_IPV6
	case IPV6:
		connect(IPV6Host(namebuf), port, mss);
		break;
#endif
	default:
		endStream();
		connectError();
	}
}


void TCPStream::connect(const IPV4Host &host, tpport_t port, unsigned mss)
{
	size_t i;
	fd_set fds;
	struct timeval to;
	bool connected = false;
	int rtn;
	long sockopt;
	socklen_t len = sizeof(sockopt);
	socklen_t alen = sizeof(mss);

#ifdef	TCP_MAXSEG
	if(mss)
		setsockopt(so, IPPROTO_TCP, TCP_MAXSEG, (char *)&mss, sizeof(mss));
#endif

	for(i = 0 ; i < host.getAddressCount(); i++)
	{
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr = host.getAddress(i);
		addr.sin_port = htons(port);
	
		if(timeout)
			setCompletion(false);

		// Win32 will crash if you try to connect to INADDR_ANY.
		if ( INADDR_ANY == addr.sin_addr.s_addr )
			addr.sin_addr.s_addr = INADDR_LOOPBACK;
		rtn = ::connect(so, (struct sockaddr *)&addr, (socklen_t)sizeof(addr));
		if(!rtn)
		{
			connected = true;
			break;
		}

		if(errno == EINPROGRESS)
		{
			FD_ZERO(&fds);
			FD_SET(so, &fds);
			to.tv_sec = timeout / 1000;
			to.tv_usec = timeout % 1000 * 1000;
			
			// timeout check for connect completion

			if(::select((int)so + 1, NULL, &fds, NULL, &to) < 1)
				continue;

			getsockopt(so, SOL_SOCKET, SO_ERROR, (char *)&sockopt, &len);
			if(!sockopt)
			{
				connected = true;
				break;
			}
			endSocket();
			so = socket(AF_INET, SOCK_STREAM, 0);
			if(so == INVALID_SOCKET)
				break;
		}
	}

	setCompletion(true);
	if(!connected)
	{
		rtn = errno;
		endStream();
		errno = rtn;
		connectError();
		return;
	}

	segmentBuffering(mss);
	Socket::state = CONNECTED;
}

#ifdef  BBT_IPV6
void TCPStream::connect(const IPV6Host &host, tpport_t port, unsigned mss)
{
	size_t i;
	fd_set fds;
	struct timeval to;
	bool connected = false;
	int rtn;
	long sockopt;
	socklen_t len = sizeof(sockopt);
	socklen_t alen = sizeof(mss);

#ifdef	TCP_MAXSEG
	if(mss)
		setsockopt(so, IPPROTO_TCP, TCP_MAXSEG, (char *)&mss, sizeof(mss));
#endif

	for(i = 0 ; i < host.getAddressCount(); i++)
	{
		struct sockaddr_in6 addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin6_family = AF_INET6;
		addr.sin6_addr = host.getAddress(i);
		addr.sin6_port = htons(port);
	
		if(timeout)
			setCompletion(false);

		// Win32 will crash if you try to connect to INADDR_ANY.
		if ( !memcmp(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any)))
			memcpy(&addr.sin6_addr, &in6addr_loopback, sizeof(in6addr_loopback));
		rtn = ::connect(so, (struct sockaddr *)&addr, (socklen_t)sizeof(addr));
		if(!rtn)
		{
			connected = true;
			break;
		}

		if(errno == EINPROGRESS)
		{
			FD_ZERO(&fds);
			FD_SET(so, &fds);
			to.tv_sec = timeout / 1000;
			to.tv_usec = timeout % 1000 * 1000;
			
			// timeout check for connect completion

			if(::select((int)so + 1, NULL, &fds, NULL, &to) < 1)
				continue;

			getsockopt(so, SOL_SOCKET, SO_ERROR, (char *)&sockopt, &len);
			if(!sockopt)
			{
				connected = true;
				break;
			}
			endSocket();
			so = socket(AF_INET6, SOCK_STREAM, 0);
			if(so == INVALID_SOCKET)
				break;
		}
	}

	setCompletion(true);
	if(!connected)
	{
		rtn = errno;
		endStream();
		errno = rtn;
		connectError();
		return;
	}

	segmentBuffering(mss);
	Socket::state = CONNECTED;
}
#endif

TCPStream::TCPStream(const char *target, Family fam, unsigned mss, bool throwflag, timeout_t to) :
        streambuf(), Socket(PF_INET, SOCK_STREAM, 0),
        iostream((streambuf *)this),
	timeout(to),
	bufsize(0),gbuf(NULL),pbuf(NULL)
{
	family = fam;
	setError(throwflag);
	connect(target, mss);
}

TCPStream::TCPStream(Family fam, bool throwflag, timeout_t to) :
	streambuf(), Socket(PF_INET, SOCK_STREAM, 0),
        iostream((streambuf *)this),
	timeout(to),
        bufsize(0),gbuf(NULL),pbuf(NULL)
{
	family = fam;
	setError(throwflag);
}

TCPStream::TCPStream(const TCPStream &source) :
streambuf(), Socket(DUP_SOCK(source.so)),
iostream((streambuf *)this) 
{
	family = source.family;
	bufsize = source.bufsize;
	allocate(bufsize);
}

void TCPStream::connect(TCPSocket &tcpip)
{
	tpport_t port;
	
	endStream();
	family = IPV4;
	so = accept(tcpip.getSocket(), NULL, NULL);
	if(so == INVALID_SOCKET)
		return;

	IPV4Host host = getPeer(&port);
	if(!tcpip.onAccept(host, port))
	{
		endSocket();
		clear(ios::failbit | rdstate());
		return;
	}

	segmentBuffering(tcpip.getSegmentSize());
	Socket::state = CONNECTED;
}	

#ifdef	BBT_IPV6

void TCPStream::connect(TCPV6Socket &tcpip)
{
        tpport_t port;

        endStream();
        family = IPV6;
        so = accept(tcpip.getSocket(), NULL, NULL);
        if(so == INVALID_SOCKET)
                return;

        IPV6Host host = getIPV6Peer(&port);
        if(!tcpip.onAccept(host, port))
        {
                endSocket();
                clear(ios::failbit | rdstate());
                return;
        }

	segmentBuffering(tcpip.getSegmentSize());
        Socket::state = CONNECTED;
}
#endif

void TCPStream::segmentBuffering(unsigned mss)
{
	unsigned max = 0;
	socklen_t alen = sizeof(max);

	if(mss == 1)	// special interactive
	{
		allocate(1);
		return;
	}

#ifdef	TCP_MAXSEG
	if(mss)
		setsockopt(so, IPPROTO_TCP, TCP_MAXSEG, (char *)&max, sizeof(max));
	getsockopt(so, IPPROTO_TCP, TCP_MAXSEG, (char *)&max, &alen);
#endif

	if(max && max < mss)
		mss = max;

	if(!mss)
	{
		if(max)
			mss = max;
		else
			mss = 536;
		allocate(mss);
		return;
	}

#ifdef	TCP_MAXSEG
	setsockopt(so, IPPROTO_TCP, TCP_MAXSEG, (char *)&mss, sizeof(mss));
#endif

	if(mss < 80)
		mss = 80;

	if(mss * 7 < 64000)
		bufferSize(mss * 7);
	else if(mss * 6 < 64000)
		bufferSize(mss * 6);
	else
		bufferSize(mss * 5);

	if(mss < 512)
		sendLimit(mss * 4);

	allocate(mss);
}	

int TCPStream::getSegmentSize(void)
{
	unsigned mss = 0;
#ifdef	TCP_MAXSEG
	socklen_t alen = sizeof(mss);
	
	getsockopt(so, IPPROTO_TCP, TCP_MAXSEG, (char *)&mss, &alen);
#endif
	if(!mss)
		return (int)bufsize;

	return mss;	
}

void TCPStream::disconnect(void)
{
	if(Socket::state == AVAILABLE)
		return;

	endStream();
	so = socket(family, SOCK_STREAM, 0);
	if(so != INVALID_SOCKET)
		Socket::state = AVAILABLE;
}

void TCPStream::endStream(void)
{
	if(bufsize) sync();
	if(gbuf)
		delete[] gbuf;
	if(pbuf)
		delete[] pbuf;
	gbuf = pbuf = NULL;
	bufsize = 0;
	clear();
	endSocket();
}

void TCPStream::allocate(size_t size)
{
	if(size < 2)
	{
		bufsize = 1;
		gbuf = pbuf = 0;
		return;
	}

	gbuf = new char[size];
	pbuf = new char[size];
	if(!pbuf || !gbuf)
	{
		error(errResourceFailure, "Could not allocate socket stream buffers");
		return;
	}
	bufsize = size;
	clear();

#if (defined(__GNUC__) && (__GNUC__ < 3)) && !defined(WIN32) && !defined(STLPORT)
	setb(gbuf, gbuf + size, 0);
#endif
	setg(gbuf, gbuf + size, gbuf + size);
	setp(pbuf, pbuf + size);
}

int TCPStream::doallocate()
{
	if(bufsize)
		return 0;

	allocate(1);
	return 1;
}

int TCPStream::uflow()
{
	int ret = underflow();

	if (ret == EOF)
		return EOF; 

	if (bufsize != 1)
		gbump(1); 

	return ret; 
}

int TCPStream::underflow()
{
	ssize_t rlen = 1;
	unsigned char ch;

	if(bufsize == 1)
	{
		if(Socket::state == STREAM)
			rlen = ::read((int)so, (char *)&ch, 1);
		else if(timeout && !Socket::isPending(pendingInput, timeout))
        {
            clear(ios::failbit | rdstate());
            error(errTimeout,"Socket read timed out",socket_errno);
            return EOF;
		}
		else
			rlen = ::recv(so, (char *)&ch, 1, 0);
		if(rlen < 1)
		{
			if(rlen < 0)
			{
				clear(ios::failbit | rdstate());
				error(errInput,"Could not read from socket",socket_errno);
			}
			return EOF;
		}
		return ch;
	}

	if(!gptr())
		return EOF;

	if(gptr() < egptr())
		return (unsigned char)*gptr();

	rlen = (ssize_t)((gbuf + bufsize) - eback());
	if(Socket::state == STREAM)
		rlen = ::read((int)so, (char *)eback(), _IOLEN64 rlen);
	else if(timeout && !Socket::isPending(pendingInput, timeout))
	{
		clear(ios::failbit | rdstate());
		error(errTimeout,"Socket read timed out",socket_errno);
		return EOF;
	}
	else
		rlen = ::recv(so, (char *)eback(), rlen, 0);
	if(rlen < 1)
	{
//		clear(ios::failbit | rdstate());
		if(rlen < 0)
                        error(errNotConnected,"Connection error",socket_errno);
		else
		{
			error(errInput,"Could not read from socket",socket_errno);
			clear(ios::failbit | rdstate());
		}
		return EOF;
	}
	error(errSuccess);

	setg(eback(), eback(), eback() + rlen);
	return (unsigned char) *gptr();
}

bool TCPStream::isPending(Pending pending, timeout_t timer)
{
	if(pending == pendingInput && in_avail())
		return true;
	else if(pending == pendingOutput)
		flush();

	return Socket::isPending(pending, timer);
}

int TCPStream::sync(void)
{
	overflow(EOF);
	setg(gbuf, gbuf + bufsize, gbuf + bufsize);
	return 0;
}

#ifdef	HAVE_SNPRINTF
size_t TCPStream::printf(const char *format, ...)
{
	va_list args;
	size_t len;
	char *buf;

	va_start(args, format);
	overflow(EOF);
	len = pptr() - pbase();
	buf = pptr();
	vsnprintf(buf, len, format, args);
	va_end(args);
	len = strlen(buf);
	if(Socket::state == STREAM)
		return ::write((int)so, buf, _IOLEN64 len);
	else
		return ::send(so, buf, _IOLEN64 len, 0);
}
#endif

int TCPStream::overflow(int c)
{
	unsigned char ch;
	ssize_t rlen, req;

	if(bufsize == 1)
	{
		if(c == EOF)
			return 0;

		ch = (unsigned char)(c);
		if(Socket::state == STREAM)
			rlen = ::write((int)so, (const char *)&ch, 1);
		else
			rlen = ::send(so, (const char *)&ch, 1, 0);
		if(rlen < 1)
		{
			if(rlen < 0)
			{
				clear(ios::failbit | rdstate());
				error(errOutput,"Could not write to socket",socket_errno);
			}
			return EOF;
		}
		else
			return c;
	}

	if(!pbase())
		return EOF;

	req = (ssize_t)(pptr() - pbase());
	if(req)
	{
		if(Socket::state == STREAM)
			rlen = ::write((int)so, (const char *)pbase(), req);
		else
			rlen = ::send(so, (const char *)pbase(), req, 0);
		if(rlen < 1)
		{
			if(rlen < 0)
			{
				clear(ios::failbit | rdstate());
				error(errOutput,"Could not write to socket",socket_errno);
			}
			return EOF;
		}
		req -= rlen;
	}

	// if write "partial", rebuffer remainder

	if(req)
//		memmove(pbuf, pptr() + rlen, req);
		memmove(pbuf, pbuf + rlen, req);
	setp(pbuf, pbuf + bufsize);
	pbump(req);

	if(c != EOF)
	{
		*pptr() = (unsigned char)c;
		pbump(1);
	}
	return c;
}

TCPSession::TCPSession(const IPV4Host &ia, 
		       tpport_t port, size_t size, int pri, size_t stack) :
Thread(pri, stack), TCPStream(IPV4)
{
	setCompletion(false);
	setError(false);
	allocate(size);
	
	size_t i;
	for(i = 0 ; i < ia.getAddressCount(); i++)
	{
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr = ia.getAddress(i);
		addr.sin_port = htons(port);

		// Win32 will crash if you try to connect to INADDR_ANY.
		if ( INADDR_ANY == addr.sin_addr.s_addr )
			addr.sin_addr.s_addr = INADDR_LOOPBACK;
		if(::connect(so, (struct sockaddr *)&addr, (socklen_t)sizeof(addr)) == 0)
			break;
		
		if(errno == EINPROGRESS)
		{
			Socket::state = CONNECTING;
			return;
		}
	}

	if(i == ia.getAddressCount())
	{
		endSocket();
		Socket::state = INITIAL;
		return;
	}

	setCompletion(true);
	Socket::state = CONNECTED;
}

#ifdef  BBT_IPV6
TCPSession::TCPSession(const IPV6Host &ia, 
		       tpport_t port, size_t size, int pri, size_t stack) :
Thread(pri, stack), TCPStream(IPV6)
{
	setCompletion(false);
	setError(false);
	allocate(size);
	
	size_t i;
	for(i = 0 ; i < ia.getAddressCount(); i++)
	{
		struct sockaddr_in6 addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin6_family = AF_INET6;
		addr.sin6_addr = ia.getAddress(i);
		addr.sin6_port = htons(port);

		// Win32 will crash if you try to connect to INADDR_ANY.
		if(!memcmp(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any)))
			memcpy(&addr.sin6_addr, &in6addr_loopback, sizeof(in6addr_loopback));
		if(::connect(so, (struct sockaddr *)&addr, (socklen_t)sizeof(addr)) == 0)
			break;
		
		if(errno == EINPROGRESS)
		{
			Socket::state = CONNECTING;
			return;
		}
	}

	if(i == ia.getAddressCount())
	{
		endSocket();
		Socket::state = INITIAL;
		return;
	}

	setCompletion(true);
	Socket::state = CONNECTED;
}
#endif


TCPSession::TCPSession(TCPSocket &s, int pri, size_t stack) :
Thread(pri, stack), TCPStream(s)
{
	setCompletion(true);
	setError(false);
}

#ifdef  BBT_IPV6
TCPSession::TCPSession(TCPV6Socket &s, int pri, size_t stack) :
Thread(pri, stack), TCPStream(s)
{
	setCompletion(true);
	setError(false);
}
#endif

TCPSession::~TCPSession()
{
	terminate();
	endStream();
}

int TCPSession::waitConnection(timeout_t timer)
{
	long sockopt = 0;
	socklen_t len = sizeof(sockopt);

	switch(Socket::state)
	{
	case INITIAL:
		return -1;
	case CONNECTED:
		break;
	case CONNECTING:
		if(!Socket::isPending(pendingOutput, timer))
		{
			endSocket();
			Socket::state = INITIAL;
			return -1;
		}

		getsockopt(so, SOL_SOCKET, SO_ERROR, (char *)&sockopt, &len);
		if(sockopt)
		{
			endSocket();
			Socket::state = INITIAL;
			return -1;
		}
	case AVAILABLE:
	case BOUND:
	case STREAM:
		break;
	}
	Socket::state = CONNECTED;
	return 0;
}

void TCPSession::initial(void)
{
	if(waitConnection(60000))
		exit();
}

ostream& operator<<(ostream &os, const IPV4Address &ia)
{
	os << inet_ntoa(getaddress(ia));
	return os;
}

}
