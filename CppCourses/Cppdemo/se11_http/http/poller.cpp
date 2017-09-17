#include <common.h>
#include <poller.h>
#ifdef	USE_POLL
namespace bbt{
Poller::Poller()
{
	nufds = 0;
	ufds = NULL;
}

Poller::~Poller()
{
	if(ufds)
	{
		delete[] ufds;
		ufds = NULL;
	}
}

pollfd *Poller::getList(int cnt)
{
	if(nufds < cnt)
	{
		if(ufds)
			delete[] ufds;
		ufds = new pollfd[cnt];
		nufds = cnt;
	}
	return ufds;
}
}
#endif

