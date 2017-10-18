/**
 * @file slog.h
 * @short System logging facilities abstraction.
 **/

#ifndef	_SLOG_H_
#define	_SLOG_H_

#include <thread.h>
#include <synchronization.h>
#include <fstream>
#include <iostream>

namespace bbt {

/**
 * The slog class is used to stream messages to the system's logging facility (syslogd).
 * A default <code>slog</code> object is used to avoid confusion with the native syslog
 * facility and to imply a logical relationship to the C++ <code>clog()</code>.
 *
 * The key difference is that the <code>slog</code> object sends it's output to the
 * system logging daemon (typically syslogd) rather than through stderr.
 * <code>slog</code> can be streamed with the <code><<</code> operator just
 * like <code>clog</code>; a default slog object is pre-initialized, and you stream
 * character data to it.
 *
 * The <code>slog</code> allows one to specify logging levels and other properties through the <code>()</code> operators.
 * Hence, once can do:
 *
 * <code><pre>
 * slog("mydaemon", SLOG_DAEMON, SLOG_EMERGENCY) << I just died << endl; </pre></code>
 *
 * or things like: 
 *
 * <code><pre>
 * slog("mydaemon", SLOG_DAEMON); 
 * slog(SLOG_INFO) << "daemon initalized" << endl; </pre></code>
 *
 * The intent is to be as common-place and as convenient to use as the stderr based clog facility
 * found in C++, and this is especially useful for C++ daemons.
 *
 * The <code>std::flush</code> manipulator doesn't work.  Either the
 * <code>std::endl</code> or <code>std::ends</code> manipulators
 * must be used to cause the output to be sent to the daemon.
 * 
 * When this class is used on a system that doesn't have the syslog headers
 * (i.e. a non-posix win32 box), the output goes to the a file with the same name
 * as the syslog identifier string with '.log' appended to it.  If the identifier string ends in
 * '.exe', the '.exe' is removed before the '.log' is appened. (e.g. the identifier foo.exe will
 * generate a log file named foo.log)
 * 
 * @author David Sugar <dyfet@ostel.com>
 * <br>Minor docs & hacks by Jon Little <littlej@arlut.utexas.edu>
 *
 * @short system logging facility class.
 */
class Slog : protected std::streambuf, public std::ostream
{
public:
	typedef enum Class
	{
		classSecurity,
		classDaemon,
		classUser,
		classDefault,
		classLocal0,
		classLocal1,
		classLocal2,
		classLocal3,
		classLocal4,
		classLocal5,
		classLocal6,
		classLocal7
	} Class;

	typedef enum Level
	{
		levelEmergency,
		levelAlert,
		levelCritical,
		levelError,
		levelWarning,
		levelNotice,
		levelInfo,
		levelDebug
	} Level;

private:
	int priority;
	Level  _level;
	bool _enable;
        bool _clogEnable;

	ThreadImpl *getPriv(void);

protected:
	/**
         * This is the streambuf function that actually outputs the data
         * to the device.  Since all output should be done with the standard
         * ostream operators, this function should never be called directly.
	 */
	int overflow(int c);

public:
	/**
         * Default (and only) constructor.  The default log level is set to
         * SLOG_DEBUG.  There is no default log facility set.  One should be
         * set before attempting any output.  This is done by the <code>open()</code> or the
         * <code>operator()(const char*, Class, Level)</code>
         * functions.
	 */
	Slog(void);

	virtual ~Slog(void);

	void close(void);

	/**
         * (re)opens the output stream.
         * @param ident The identifier portion of the message sent to the syslog daemon.
         * @param grp The log facility the message is sent to
	 */
	void open(const char *ident, Class grp = classUser);

	/**
         * Sets the log identifier, level, and class to use for subsequent output
         * @param ident The identifier portion of the message
         * @param grp The log facility the message is sent to
         * @param level The log level of the message
	 */
	Slog &operator()(const char *ident, Class grp = classUser,
			 Level level = levelError);

	/**
         * Changes the log level and class to use for subsequent output
         * @param level The log level of the message
         * @param grp The log facility the message is sent to
	 */
	Slog &operator()(Level level, Class grp = classDefault);

        /**
         * Does nothing except return *this.
         */
	Slog &operator()(void);

	/**
         * Sets the logging level.
         * @param enable is the logging level to use for further output
	 */
	inline void level(Level enable)
		{_level = enable;};

	/**
         * Enables or disables the echoing of the messages to clog in addition
         * to the syslog daemon.  This is enabled by the default class constructor.
         * @param f true to enable, false to disable clog output
	 */
        inline void clogEnable(bool f=true)
                {_clogEnable = f;};

	inline Slog &warn(void)
		{return operator()(Slog::levelWarning);};

	inline Slog &error(void)
		{return operator()(Slog::levelError);};

	inline Slog &debug(void)
		{return operator()(Slog::levelDebug);};

	inline Slog &emerg(void)
		{return operator()(Slog::levelEmergency);};

	inline Slog &alert(void)
		{return operator()(Slog::levelAlert);};

	inline Slog &critical(void)
		{return operator()(Slog::levelCritical);};

	inline Slog &notice(void)
		{return operator()(Slog::levelNotice);};

	inline Slog &info(void)
		{return operator()(Slog::levelInfo);};

};

extern Slog slog;

}; //namespace bbt

#endif

