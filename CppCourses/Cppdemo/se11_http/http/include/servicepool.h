#ifndef _SERVICEPOOL_H_
#define _SERVICEPOOL_H_
#include <socket.h>
#include <synchronization.h>
#include <queue>
#include <socketport.h>
namespace bbt{
class ServicePool
{
	private:
		Conditional cond;
		std::queue<SocketService *> pool;
	public:
		ServicePool(int size=1);
		SocketService* getService();
		void freeService(SocketService*);
		virtual ~ServicePool();	
};
}
#endif
