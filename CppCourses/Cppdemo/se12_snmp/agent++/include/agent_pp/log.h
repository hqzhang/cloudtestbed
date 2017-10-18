/*_############################################################################
  _## 
  _##  log.h  
  _## 
  _##
  _##  AGENT++ API Version 3.5.22c
  _##  -----------------------------------------------
  _##  Copyright (C) 2000-2004 Frank Fock, Jochen Katz
  _##  
  _##  LICENSE AGREEMENT
  _##
  _##  WHEREAS,  Frank  Fock  and  Jochen  Katz  are  the  owners of valuable
  _##  intellectual  property rights relating to  the AGENT++ API and wish to
  _##  license AGENT++ subject to the  terms and conditions set forth  below;
  _##  and
  _##
  _##  WHEREAS, you ("Licensee") acknowledge  that Frank Fock and Jochen Katz
  _##  have the right  to grant licenses  to the intellectual property rights
  _##  relating to  AGENT++, and that you desire  to obtain a license  to use
  _##  AGENT++ subject to the terms and conditions set forth below;
  _##
  _##  Frank  Fock    and Jochen   Katz   grants  Licensee  a  non-exclusive,
  _##  non-transferable, royalty-free  license  to use   AGENT++ and  related
  _##  materials without  charge provided the Licensee  adheres to all of the
  _##  terms and conditions of this Agreement.
  _##
  _##  By downloading, using, or  copying  AGENT++  or any  portion  thereof,
  _##  Licensee  agrees to abide  by  the intellectual property  laws and all
  _##  other   applicable laws  of  Germany,  and  to all of   the  terms and
  _##  conditions  of this Agreement, and agrees  to take all necessary steps
  _##  to  ensure that the  terms and  conditions of  this Agreement are  not
  _##  violated  by any person  or entity under the  Licensee's control or in
  _##  the Licensee's service.
  _##
  _##  Licensee shall maintain  the  copyright and trademark  notices  on the
  _##  materials  within or otherwise  related   to AGENT++, and  not  alter,
  _##  erase, deface or overprint any such notice.
  _##
  _##  Except  as specifically   provided in  this  Agreement,   Licensee  is
  _##  expressly   prohibited  from  copying,   merging,  selling,   leasing,
  _##  assigning,  or  transferring  in  any manner,  AGENT++ or  any portion
  _##  thereof.
  _##
  _##  Licensee may copy materials   within or otherwise related   to AGENT++
  _##  that bear the author's copyright only  as required for backup purposes
  _##  or for use solely by the Licensee.
  _##
  _##  Licensee may  not distribute  in any  form  of electronic  or  printed
  _##  communication the  materials  within or  otherwise  related to AGENT++
  _##  that  bear the author's  copyright, including  but  not limited to the
  _##  source   code, documentation,  help  files, examples,  and benchmarks,
  _##  without prior written consent from the authors.  Send any requests for
  _##  limited distribution rights to fock@agentpp.com.
  _##
  _##  Licensee  hereby  grants  a  royalty-free  license  to  any  and   all 
  _##  derivatives  based  upon this software  code base,  that  may  be used
  _##  as a SNMP  agent development  environment or a  SNMP agent development 
  _##  tool.
  _##
  _##  Licensee may  modify  the sources  of AGENT++ for  the Licensee's  own
  _##  purposes.  Thus, Licensee  may  not  distribute  modified  sources  of
  _##  AGENT++ without prior written consent from the authors. 
  _##
  _##  The Licensee may distribute  binaries derived from or contained within
  _##  AGENT++ provided that:
  _##
  _##  1) The Binaries are  not integrated,  bundled,  combined, or otherwise
  _##     associated with a SNMP agent development environment or  SNMP agent
  _##     development tool; and
  _##
  _##  2) The Binaries are not a documented part of any distribution material. 
  _##
  _##
  _##  THIS  SOFTWARE  IS  PROVIDED ``AS  IS''  AND  ANY  EXPRESS OR  IMPLIED
  _##  WARRANTIES, INCLUDING, BUT NOT LIMITED  TO, THE IMPLIED WARRANTIES  OF
  _##  MERCHANTABILITY AND FITNESS FOR  A PARTICULAR PURPOSE  ARE DISCLAIMED.
  _##  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  _##  INDIRECT,   INCIDENTAL,  SPECIAL, EXEMPLARY,  OR CONSEQUENTIAL DAMAGES
  _##  (INCLUDING,  BUT NOT LIMITED  TO,  PROCUREMENT OF SUBSTITUTE  GOODS OR
  _##  SERVICES; LOSS OF  USE,  DATA, OR PROFITS; OR  BUSINESS  INTERRUPTION)
  _##  HOWEVER CAUSED  AND ON ANY THEORY  OF  LIABILITY, WHETHER IN CONTRACT,
  _##  STRICT LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
  _##  IN  ANY WAY OUT OF  THE USE OF THIS  SOFTWARE,  EVEN IF ADVISED OF THE
  _##  POSSIBILITY OF SUCH DAMAGE. 
  _##
  _##
  _##  Stuttgart, Germany, Thu Jan 13 00:12:31 CET 2005 
  _##  
  _##########################################################################*/



#ifndef _log_h_
#define _log_h_

#include <agent_pp/agent++.h>
#include <agent_pp/threads.h>

#ifndef WIN32
#include <sys/types.h>
#endif
#include <stdio.h>
#include <string.h>


#ifdef AGENTPP_NAMESPACE
namespace Agentpp {
#endif

// Log entry class
#define ERROR_LOG	0x10
#define WARNING_LOG	0x20
#define EVENT_LOG	0x30
#define INFO_LOG	0x40
#define DEBUG_LOG	0x50

#define LOG_TYPES       5

#ifdef _NO_LOGGING
 
#define LOG_BEGIN(x)	
#define LOG(x)		
#define LOG_END		

#else

#define LOG_BEGIN(x)						\
{								\
	if (DefaultLog::log()->log_needed(x))			\
	{							\
		DefaultLog::log()->lock();			\
		DefaultLog::create_log_entry(x);	

#define LOG(x)		*DefaultLog::log_entry() += x

#define LOG_END							\
		*DefaultLog::log() += DefaultLog::log_entry();	\
		DefaultLog::delete_log_entry();			\
		DefaultLog::log()->unlock();			\
	}							\
}

#endif


/*--------------------------- class LogEntry --------------------------*/

/**
 * The LogEntry class represents log entries. An instance of LogEntry can
 * be added to a Log. Each LogEntry can be classified into the log classes
 * ERROR_LOG, WARNING_LOG, EVENT_LOG, INFO_LOG, and DEBUG_LOG with up to
 * 16 severity levels. A log entry consists of a descriptor string and
 * optional several string or numeric values. 
 * @see Log
 * 
 * @author Frank Fock
 * @author Marty Janzen
 * @version 3.5f
 */

class AGENTPP_DECL LogEntry {
public:
	/**
	 * Constructor with log class and severity level
	 * 
	 * @param t - The type of the log entry. The type is composed 
	 *            by a logical OR of the log entry class with a level
	 *            of 0 up to 15. 
	 * @note A error log of level 0 will stop program execution!
	 */  
	LogEntry(unsigned char t) : type(t), count(0) {}

	/**
	 * Virtual destructor.
	 */  
	virtual ~LogEntry() {}

	/**
	 * Initialize a new log entry, showing timestamp, class, and level.
	 */ 
	virtual void init(void);

	/**
	 * Add a numeric value to the log entry.
	 *
	 * @param l - A numeric value.
	 */
	virtual LogEntry& operator+=(const long);

	/**
	 * Add a string value to the log entry.
	 *
	 * @param l - A numeric value.
	 */
	virtual LogEntry& operator+=(const char*);

	/**
	 * Get the contents of this log entry.
         *
	 * @return Current contents of this log entry.
	 */ 
	virtual const char* get_value(void) const { return ""; }

	/**
	 * Get the class of this log entry.
         *
	 * @return Log entry class.
	 */ 
	unsigned char get_class(void) const { return type & 0xF0; }

	/**
	 * Get the level of this log entry.
         *
	 * @return Log entry level.
	 */ 
	unsigned char get_level(void) const { return type & 0x0F; }

protected:
	/**
	 * Add a string to the log.
	 *
	 * @param s - A string value.
	 * @return TRUE if the value has been added and FALSE if the log
	 *         entry is full.
	 */
	virtual boolean	add_string(const char*) = 0;

	/**
	 * Add an integer to the log.
	 *
	 * @param s - An integer value.
	 * @return TRUE if the value has been added and FALSE if the log
	 *         entry is full.
	 */
	virtual boolean	add_integer(long);

	/**
	 * Add the current time to the log.
	 */
	virtual boolean add_timestamp(void);

protected:
	unsigned char  	type;
	int		count;
};


/*------------------------- class LogEntryImpl ------------------------*/

/**
 * The LogEntryImpl class implements a log entry using a dynamically
 * allocated, but fixed-size buffer.
 * @see Log
 * 
 * @author Marty Janzen
 * @version 3.5f
 */
#define MAX_LOG_SIZE	255

class AGENTPP_DECL LogEntryImpl : public LogEntry {
public:
	/**
	 * Constructor with log class and severity level
	 * 
	 * @param t - The type of the log entry. The type is composed 
	 *            by logical or the log entry class with a level
	 *            of 0 up to 15. 
	 * @note A error log of level 0 will stop program execution!
	 */  
	LogEntryImpl(unsigned char);

	/**
	 * Destructor.
	 */  
	~LogEntryImpl();

	/**
	 * Get the contents of this log entry.
         *
	 * @return Current contents of this log entry.
	 */ 
	virtual const char* get_value(void) const { return value; }

protected:
	/**
	 * Count the bytes left for additional values.
	 *
	 * @return The number of bytes left in this log entry.
	 */  
	int		bytes_left() 
	    { return (int)(MAX_LOG_SIZE-(ptr-value)-1); }

	/**
	 * Add a string to the log.
	 *
	 * @param s - A string value.
	 * @return TRUE if the value has been added and FALSE if the log
	 *         entry is full.
	 */
	boolean		add_string(const char*);

private:
        char*		value;
	char*		ptr;
	boolean		output_stopped;
};


/*--------------------------- class AgentLog --------------------------*/

/**
 * The AgentLog class is an abstract base class representing a log for
 * information that is generated during the run time of an AGENT++
 * SNMP agent.  A derived class only needs to provide appropriate
 * create_log_entry() and operator+= methods.
 * @see LogEntry
 *
 * @author Frank Fock
 * @version 3.5.14
 */
 
class AGENTPP_DECL AgentLog {
public:
	/**
	 * Default constructor.
	 */ 
	AgentLog();

	/**
	 * Virtual destructor.
	 */
	virtual ~AgentLog() {}

	/**
	 * Lock the receiver.  Default action is to perform no locking.
	 */
	virtual void	lock() {}

	/**
	 * Unlock the receiver.  Default action is to perform no locking.
	 */
	virtual void	unlock() {}

	/**
	 * Set a filter on a specified log class. Only log entries with
	 * a level less or equal than the specified level will be logged.
	 *
	 * @param logclass - A log entry class. @see LogEntry
	 * @param filter - A value between 0 and 15.
	 */ 
	virtual void	set_filter(int logclass, unsigned char filter);

	/**
	 * Gets the log level for the given log class.
	 * @return
	 *    a unsigned char value between 0 and 15 
	 */
	virtual unsigned char get_filter(int logclass) const; 

	/**
	 * Create a new LogEntry.
	 *
	 * @param t - The type of the log entry.
	 * @return A new instance of LogEntry (or of a derived class).
	 */
	virtual LogEntry* create_log_entry(unsigned char) const = 0;

	/**
	 * Add a LogEntry to the receiver Log.
	 *
	 * @param log - A log entry.
	 * @return The receiver log itself.
	 */
	virtual AgentLog& operator+=(const LogEntry*) = 0;

	/**
	 * Check whether a logging for the given type of LogEntry
	 * has to be done or not.
	 *
	 * @param type
	 *    the type of the log entry. The type is composed 
	 *    by logical or the log entry class with a level
	 *    of 0 up to 15.
	 * @return
	 *    TRUE if logging is needed, FALSE otherwise.
	 */
	virtual boolean	log_needed(unsigned char t) 
	  { return ((t & 0x0F) <= logfilter[(t / 16) - 1]); }

	/**
	 * Return the current time as a string.
	 * 
	 * @param
	 *    a buffer (of at least 18 characters, for the default method)
         *    into which to place a string containg the current time.
         *    If no buffer is supplied, a static area is used.
	 * @return
	 *    a string containing the current time. Either the supplied
	 *    buffer or the static area.
	 */
	virtual const char*	now(char* = 0);

	/**
	 * Return the current time as a string, using the current
         * default log object.  (For backward compatibility.)
	 * @note that the user is responsible for deleting the returned
	 * string, using delete [].
	 * 
	 * @return
	 *    a string containg the current time.
	 */
	static const char*	get_current_time();

protected:
	unsigned char		logfilter[LOG_TYPES];
	char			static_buf[18];
};


/*------------------------- class AgentLogImpl ------------------------*/

/**
 * The AgentLogImpl class is an implementation of AgentLog which writes
 * log messages to a file, or to stdout or stderr.
 * @see LogEntry 
 *
 * @author Frank Fock
 * @version 3.5f
 */
 
class AGENTPP_DECL AgentLogImpl : public AgentLog {
public:
	/**
	 * Default constructor, with optional pointer to an open log file.
         * Log is directed to the file if given, otherwise to stdout
	 *
	 * @param fp - An open log file.  0 implies stdout.
	 */ 
	AgentLogImpl(FILE* = stdout);

	/**
	 * Constructor with file name of a log file. Log is directed
	 * to the given file.
	 *
	 * @param fname - The file name of a log file.
	 */ 
	AgentLogImpl(const char*);

	/**
	 * Destructor.
	 */
	~AgentLogImpl();

	/**
	 * Set destination of logs to a given file.
	 * 
	 * @param fname - A file name. "" directs logs to stdout.
	 */
	void		set_dest(const char*);

	/**
	 * Set destination of logs to a given file.
	 * 
	 * @param fp - A pointer to an open file.  0 directs logs to stdout.
	 */
	void		set_dest(FILE*);

	/**
	 * Lock the receiver.
	 */
	void lock()
	{
#ifdef _THREADS
		logLock.start_synch();
#endif
	}

	/**
	 * Unlock the receiver.
	 */
	 void unlock()
	{
#ifdef _THREADS
		logLock.end_synch();
#endif
	}

	/**
	 * Create a new LogEntry.
	 *
	 * @param t - The type of the log entry.
	 * @return A new instance of LogEntry (or of a derived class).
	 */
	virtual LogEntry* create_log_entry(unsigned char) const;

	/**
	 * Add a LogEntry to the receiver Log.
	 *
	 * @param log - A log entry.
	 * @return The receiver log itself.
	 */
	virtual AgentLog& operator+=(const LogEntry*);

protected:
	ThreadManager		logLock;
	FILE*			logfile;
	boolean			close_needed;
};


/*--------------------------- class DefaultLog --------------------------*/

/**
 * The DefaultLog class has a static Log member, that is used by the
 * AGENT++ API for logging.
 *
 * @version 3.5.4
 * @author Frank Fock (singlton pattern -> Philippe Roger)
 */  

class AGENTPP_DECL DefaultLog {
public:
	DefaultLog() { }
	~DefaultLog() { }

	/** 
	 * Initialize the default logger with the given loggin implementation.
	 *
	 * @param logger
	 *    an AgentLog instance to be used as default logger. A previously
	 *    set logger will be deleted.
	 */
	static void init(AgentLog* logger) 
	  { if (instance) delete instance; instance = logger; }

	/**
	 * Return the default logger. 
	 *
	 * @return
	 *    a pointer to an AgentLog instance.
	 */
	static AgentLog* log() 
	  { if (!instance) init(new AgentLogImpl()); return instance; }

	/**
	 * Create a new log entry or reuse an existing one.
	 *
	 * @param type
	 *    the type of the log entry as bitwise or of log class and level. 
	 */
	static void create_log_entry(unsigned char t)
	  { if (!entry) { entry = log()->create_log_entry(t); entry->init();} }

	/**
	 * Return the current log entry. If there is none, an ERROR_LOG entry
	 * with level 1 will be created.
	 *
	 * @return
	 *    a pointer to a LogEntry instance.
	 */
	static LogEntry* log_entry() 
	  { if (!entry) create_log_entry(ERROR_LOG | 1); return entry; } 

	/**
	 * Delete current log entry.
	 */
	static void delete_log_entry() 
	  { if (entry) delete entry; entry = 0; }

protected:

	static AgentLog* instance;
	static LogEntry* entry;
};


#ifdef AGENTPP_NAMESPACE
}
#endif
#endif // _log_h_
