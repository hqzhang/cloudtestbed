/**
 * Timer ports are used to provide synchronized timing events when managed
 * under a "service thread" such as SocketService.  This is made into a
 * stand-alone base class since other derived libraries (such as the
 * serial handlers) may also use the pooled "service thread" model
 * and hence also require this code for managing timing.
 *
 * @author David Sugar <dyfet@ostel.com>
 * @short synchronized millisecond timing for service threads.
 */
#ifndef _BBT_TIMERPORT_H_
#define _BBT_TIMERPORT_H_
#include <common.h>
#include <bthread.h>
namespace bbt{
class TimerPort
{
	struct timeval timer;
	bool active;

public:
	/**
	 * Create a timer, mark it as inactive, and set the initial
	 * "start" time to the creation time of the timer object.  This
	 * allows "incTimer" to initially refer to time delays relative
	 * to the original start time of the object.
	 */
	TimerPort();

	/**
	 * Set a new start time for the object based on when this call is
	 * made and optionally activate the timer for a specified number
	 * of milliseconds.  This can be used to set the starting time
	 * of a realtime session.
	 *
	 * @param timeout delay in milliseconds from "now"
	 */
	void setTimer(timeout_t timeout = 0);

	/**
	 * Set a timeout based on the current time reference value either
	 * from object creation or the last setTimer().  This reference
	 * can be used to time synchronize realtime data over specified
	 * intervals and force expiration when a new frame should be
	 * released in a synchronized manner.  
	 *
	 * @param timeout delay in milliseconds from reference.
	 */
	void incTimer(timeout_t timeout);

	/**
	 * This is used to "disable" the service thread from expiring
	 * the timer object.  It does not effect the reference time from
	 * either creation or a setTimer().
	 */
	void endTimer(void);

	/**
	 * This is used by service threads to determine how much time
	 * remains before the timer expires based on a timeout specified
	 * in setTimer() or incTimer().  It can also be called after
	 * setting a timeout with incTimer() to see if the current timeout
	 * has already expired and hence that the application is already
	 * delayed and should skip frame(s).
	 *
	 * return time remaining in milliseconds, or TIMEOUT_INF if
	 * inactive.
	 */
	timeout_t getTimer(void) const;

	/**
	 * This is used to determine how much time has elapsed since a
	 * timer port setTimer benchmark time was initially set.  This
	 * allows one to use setTimer() to set the timer to the current
	 * time and then measure elapsed time from that point forward.
	 *
	 * return time elapsed in milliseconds, or TIMEOUT_INF if
	 * inactive.
	 */
	timeout_t getElapsed(void) const;
};
}
#endif
