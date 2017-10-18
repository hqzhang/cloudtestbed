#include <synchronization.h>
#include <systime.h>
#include <cstring>
namespace bbt{
Mutex SysTime::timeLock;

time_t SysTime::getTime(time_t *tloc) 
{
    time_t ret;
    lock();
    time_t temp;
    std::time(&temp);
    memcpy(&ret, &temp, sizeof(time_t));
    if (tloc != NULL) 
        memcpy(tloc, &ret, sizeof(time_t));
    unlock();
    return ret;   
}

int SysTime::getTimeOfDay(struct timeval *tp) 
{
	struct timeval temp;
    int ret(0);
    lock();

    ret = ::gettimeofday(&temp, NULL);
    if (ret == 0) 
        memcpy(tp, &temp, sizeof(struct timeval));
    unlock();
    return ret;
}
} //namespace bbt


