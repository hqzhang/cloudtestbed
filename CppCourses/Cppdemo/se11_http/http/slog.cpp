#include <slog.h>
#include <bthread.h>
#include <private.h>
#include <syslog.h>
#include <cstring>
#include <unistd.h>

using std::streambuf;
using std::ofstream;
using std::ostream;
using std::clog;
using std::endl;
using std::ios;
using std::strrchr;

#define MSGPOS thread->_msgpos 
// #define MSGPOS _msgpos 
#define MSGBUF thread->_msgbuf 
// #define MSGBUF _msgbuf 
namespace bbt {

Slog slog;

Slog::Slog(void) :
streambuf()
,ostream((streambuf *)this)
{
	_enable = true;
	_level = levelDebug;
        _clogEnable = true;
}

Slog::~Slog(void)
{
	closelog();
}

void Slog::close(void)
{
	closelog();
}

void Slog::open(const char *ident, Class grp)
{
	const char *cp;

	cp = strrchr(ident, '/');
	if(cp)
		ident = ++cp;

	int fac;

	switch(grp)
	{
	case classUser:
		fac = LOG_USER;
		break;

	case classDaemon:
		fac = LOG_DAEMON;
		break;

	case classSecurity:
		fac = LOG_AUTH;
		break;

	case classLocal0:
		fac = LOG_LOCAL0;
		break;

	case classLocal1:
		fac = LOG_LOCAL1;
		break;

	case classLocal2:
		fac = LOG_LOCAL2;
		break;

	case classLocal3:
		fac = LOG_LOCAL3;
		break;

	case classLocal4:
		fac = LOG_LOCAL4;
		break;

	case classLocal5:
		fac = LOG_LOCAL5;
		break;

	case classLocal6:
		fac = LOG_LOCAL6;
		break;

	case classLocal7:
		fac = LOG_LOCAL7;
		break;

	default:
		fac = LOG_USER;
		break;
	}
	openlog(ident, 0, fac);
}

int Slog::overflow(int c)
{
	ThreadImpl *thread = getThread()->priv;

	if(!thread)
		return c;

	if(c == '\n' || !c)
	{
		if(!MSGPOS)
			return c;

		MSGBUF[MSGPOS] = 0;
		if (_enable)
			syslog(priority, "%s", MSGBUF);
		MSGPOS = 0;

		if ( _enable && _clogEnable
                     && (getppid() > 1)
                     )
			clog << MSGBUF << endl;
		_enable = true;
		return c;
	}

	if (MSGPOS < (int)(sizeof(MSGBUF) - 1))
		MSGBUF[MSGPOS++] = c;

	return c;
}

Slog &Slog::operator()(const char *ident, Class grp, Level level)
{
	ThreadImpl *thread = getThread()->priv;

	if(!thread)
		return *this;

	MSGPOS = 0;
	_enable = true;
	open(ident, grp);
	return this->operator()(level, grp);
}

Slog &Slog::operator()(Level level, Class grp)
{
	ThreadImpl *thread = getThread()->priv;

	if(!thread)
		return *this;

	MSGPOS = 0;
	if(_level >= level)
		_enable = true;
	else
		_enable = false;

	switch(level)
	{
	case levelEmergency:
		priority = LOG_EMERG;
		break;
	case levelAlert:
		priority = LOG_ALERT;
		break;
	case levelCritical:
		priority = LOG_CRIT;
		break;
	case levelError:
		priority = LOG_ERR;
		break;
	case levelWarning:
		priority = LOG_WARNING;
		break;
	case levelNotice:
		priority = LOG_NOTICE;
		break;
	case levelInfo:
		priority = LOG_INFO;
		break;
	case levelDebug:
		priority = LOG_DEBUG;
		break;
	}
	switch(grp)
	{
	case classSecurity:
		priority |= LOG_AUTH;
		break;
	case classUser:
		priority |= LOG_USER;
		break;
	case classDaemon:
		priority |= LOG_DAEMON;
		break;
	case classDefault:
		priority |= LOG_USER;
		break;
	case classLocal0:
		priority |= LOG_LOCAL0;
		break;
	case classLocal1:
		priority |= LOG_LOCAL1;
		break;
	case classLocal2:
		priority |= LOG_LOCAL2;
		break;
	case classLocal3:
		priority |= LOG_LOCAL3;
		break;
	case classLocal4:
		priority |= LOG_LOCAL4;
		break;
	case classLocal5:
		priority |= LOG_LOCAL5;
		break;
	case classLocal6:
		priority |= LOG_LOCAL6;
		break;
	case classLocal7:
		priority |= LOG_LOCAL7;
		break;
	}
	priority = level;
	return *this;
}

Slog &Slog::operator()(void)
{
	return *this;
}	

ThreadImpl *Slog::getPriv(void)
{
	Thread *thread = Thread::get();

	if(!thread)
		return NULL;

	return thread->priv;
}

};

