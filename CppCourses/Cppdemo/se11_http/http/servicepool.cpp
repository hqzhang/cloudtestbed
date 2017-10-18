#include <servicepool.h>
using namespace std;
namespace bbt{
ServicePool::ServicePool(int size)
{
	int i;
	cond.enterMutex();
	for(i=0;i<size;i++)
	{
		pool.push(new SocketService);
	}
	cond.leaveMutex();
}

SocketService* ServicePool::getService()
{
	cond.enterMutex();
	while(pool.empty())
		cond.wait();
	SocketService *svc=pool.front();
	pool.pop();
	cond.leaveMutex();
	return svc;
}

void ServicePool::freeService(SocketService *svc)
{
	cond.enterMutex();
	pool.push(svc);
	cond.signal(0);
	cond.leaveMutex();
}

ServicePool::~ServicePool()
{
	SocketService *svc;
	int size;
	cond.enterMutex();
	size=pool.size();
	for(int i=0;i<size;i++)
	{
		svc=pool.front();
		pool.pop();
		delete svc;
	}
	cond.leaveMutex();
}

	
} //namespace bbt
