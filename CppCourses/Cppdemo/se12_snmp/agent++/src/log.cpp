/*_############################################################################
  _## 
  _##  log.cpp  
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


#ifndef WIN32
#include <unistd.h>
#else
#include <process.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <string.h>

#include <agent_pp/log.h>

#ifdef WIN32
#ifdef __BCPLUSPLUS__
#define _getpid getpid
#endif
#endif

#ifdef AGENTPP_NAMESPACE
using namespace Agentpp;
#endif

// default log filter: logs with level less or equal filter value are logged
// error, warning, event, info, debug:
static unsigned char default_logfilter[] = { 9, 9, 4, 6, 7 };

#undef   LOG_INDENT


/*--------------------------- class LogEntry --------------------------*/

 /**
  * Initialize a log entry, showing timestamp, class, and level.
  * 
  */  
void LogEntry::init(void)
{
#ifdef WIN32
	int pid = _getpid();
#else
	int pid = getpid();
#endif

	add_timestamp();
	add_string(": ");
	add_integer(pid);
	add_string(": ");

	char buf[20];
	sprintf(buf, "(%X)", get_level());
	add_string(buf);

	switch (type & 0xF0) {
	case DEBUG_LOG:   add_string("DEBUG  : "); break;
	case INFO_LOG:	  add_string("INFO   : "); break;
	case WARNING_LOG: add_string("WARNING: "); break;
	case ERROR_LOG:	  add_string("ERROR  : "); break;
	case EVENT_LOG:	  add_string("EVENT  : "); break;
	}

#ifdef LOG_INDENT
	// indent log by level
	for (int i=0; i<(type & 0x0F); i++) 
		add_string(" ");
#endif
}

/**
 * Add a string value to the log entry.
 *
 * @param l - A numeric value.
 */
LogEntry& LogEntry::operator+=(const char* s)
{
	// The convention for Agent++ log messages is that the
	// timestamp, etc. is followed by the class and method name,
	// then by the list of arguments.
	if (count == 0) 
		add_string(s);
	else {
	  if (count == 1) 
		add_string(": ");
	  else 
		add_string(", ");

	  add_string("(");
	  add_string(s);
	  add_string(")");
	}
	count++;
	return *this;
}

/**
 * Add a numeric value to the log entry.
 *
 * @param l - A numeric value.
 */
LogEntry& LogEntry::operator+=(const long l)
{
	if (count == 1) 
		add_string(": ");
	else 
		add_string(", ");

	count++;
	add_string("(");
	add_integer(l);
	add_string(")");
	return *this;
}

/**
 * Add an integer to the log.
 *
 * @param s - An integer value.
 * @return TRUE if the value has been added and FALSE if the log
 *         entry is full.
 */
boolean LogEntry::add_integer(long l)
{
	char buf[40];
	sprintf(buf, "%ld", l);
	return add_string(buf);
}

/**
 * Add the current time to the log entry.
 */
boolean LogEntry::add_timestamp(void)
{
	return add_string(DefaultLog::log()->now());
}


/*------------------------- class LogEntryImpl ------------------------*/

/**
 * Constructor for the standard log entry implementation.
 */  
LogEntryImpl::LogEntryImpl(unsigned char t) : LogEntry(t)
{
	value = new char[MAX_LOG_SIZE];
        value[0] = '\0';
	ptr = value;
	output_stopped = FALSE;
}

/**
 * Destructor for the standard log entry implementation.
 */  
LogEntryImpl::~LogEntryImpl()
{
	delete [] value;
}

/**
 * Add a string to the log.
 *
 * @param s - A string value.
 * @return TRUE if the value has been added and FALSE if the log
 *         entry is full.
 */
boolean LogEntryImpl::add_string(const char* s)
{
	if (output_stopped)
		return FALSE;

	int len = strlen(s);
	if (len <= bytes_left()) {
		strcat(ptr, s);
		ptr += len;
		return TRUE;
	}

	if (bytes_left() >= 3) {
		strcat(ptr, "...");
		ptr += 3;
	}
	output_stopped = TRUE;
	return FALSE;
}	


/*-------------------------- class AgentLog ---------------------------*/

/**
 * Default constructor.
 */
AgentLog::AgentLog()
{
	for (int i=0; i<LOG_TYPES; i++)
		logfilter[i] = default_logfilter[i];
}

void AgentLog::set_filter(int logclass, unsigned char filter)
{ 
	int idx = (logclass/16)-1;
	if ((idx >=0) && (idx <= LOG_TYPES) && (filter<16)) 
		logfilter[idx] = filter; 
}

unsigned char AgentLog::get_filter(int logclass) const
{
	int idx = (logclass/16)-1;	
	if ((idx >= 0) && (idx < LOG_TYPES)) { 
		return logfilter[idx]; 
	}
	return 0;
}

const char* AgentLog::now(char* buf)
{
        if (buf == NULL) buf = static_buf;

	time_t t;
	time(&t);
	strftime(buf, 18, "%Y%m%d.%H:%M:%S", localtime(&t));
	return buf;
}	

/*static*/ const char* AgentLog::get_current_time() 
{
	char* buf = new char[18];
        strcpy(buf, DefaultLog::log()->now());
	return buf;
}	


/*------------------------ class AgentLogImpl -------------------------*/

/**
 * Default constructor. Log is directed to stdout.
 */
AgentLogImpl::AgentLogImpl(FILE* fp) : AgentLog()
{
	set_dest(fp);
}

/**
 * Constructor with file name of a log file. Log is directed
 * to the given file.
 *
 * @param fname - The file name of a log file.
 */ 
AgentLogImpl::AgentLogImpl(const char* fname) : AgentLog()
{
	set_dest(fname);
}

/**
 * Destructor.
 */
AgentLogImpl::~AgentLogImpl()
{
	if (close_needed) fclose(logfile);
}

/**
 * Set destination of logs to a given file.
 * 
 * @param fname - A file name. "" directs logs to stdout.
 */
void AgentLogImpl::set_dest(const char* fname)
{
	close_needed = FALSE;
	if ((!fname) || (strlen(fname) == 0)) 
		logfile = stdout;
	else {
		logfile = fopen(fname, "a");
		if (logfile == NULL)
			logfile = stdout;
		else
			close_needed = TRUE;
	}
}

/**
 * Set destination of logs to a given file.
 * 
 * @param fname - A pointer to an open log file. 0 directs logs to stdout.
 */
void AgentLogImpl::set_dest(FILE* fp)
{
	logfile = fp ? fp : stdout;
	close_needed = FALSE;
}

/**
 * Create a new LogEntry.
 *
 * @param t - The type of the log entry.
 * @return A new instance of LogEntry (or of a derived class).
 */
LogEntry* AgentLogImpl::create_log_entry(unsigned char t) const
{
	return new LogEntryImpl(t);
}

/**
 * Add a LogEntry to the receiver Log.
 *
 * @param log - A log entry.
 * @return The receiver log itself.
 */
AgentLog& AgentLogImpl::operator+=(const LogEntry* log)
{
	fprintf(logfile, "%s\n", log->get_value());

	// check if critical error
	if ((log->get_class() == ERROR_LOG) && (log->get_level() == 0))
		raise(SIGTERM);

	return *this;
}


// define the default logs

#ifdef _THREADS
#ifndef _WIN32THREADS
pthread_mutex_t logmutex = PTHREAD_MUTEX_INITIALIZER;
#endif
#endif

AgentLog* DefaultLog::instance = 0;
LogEntry* DefaultLog::entry = 0;

