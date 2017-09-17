#ifndef	BBT_COMMON_H_
#define	BBT_COMMON_H_

#ifndef	_REENTRANT
#define _REENTRANT 1
#endif

#ifndef _THREAD_SAFE
#define _THREAD_SAFE 1
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <unistd.h>
#include <features.h>
#include <sys/types.h>
#include <sys/time.h>

typedef u_int8_t uint8;
typedef int16_t int16;
typedef u_int16_t uint16;
typedef int32_t int32;
typedef u_int32_t uint32;
#define HAVE_64_BITS
typedef int64_t int64;
typedef u_int64_t uint64;

#include <poll.h>
#include <sys/poll.h>
#define	USE_POLL

#include <sys/socket.h>
#include <sys/select.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>


#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <sys/un.h>

#ifndef SUN_LEN
#define SUN_LEN(ptr) sizeof(sockaddr_un.sun_len) + sizeof(sockaddr_un.sun_family) + sizeof(sockaddr_un.sun_path) + 1
#endif

#define	BBT_IPV6

#define	BBT_MULTIFAMILY_IP


#define THROW(x) throw x
#define THROWS(x) throw(x)
#define	NEW_THROWS throw()
#define	THROWS_EMPTY throw()


#include <sys/fcntl.h>
#include <endian.h>

#include <signal.h>
#ifndef SA_ONESHOT
#define SA_ONESHOT SA_RESETHAND
#endif

#include <string.h>
#include <strings.h>
#include <alloca.h>

#include <bits/atomicity.h>
#define HAVE_ATOMIC

#include <pthread.h>

#include <semaphore.h>
#include <sched.h>

#define __PTHREAD_H__

#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN 32768
#endif


#endif



