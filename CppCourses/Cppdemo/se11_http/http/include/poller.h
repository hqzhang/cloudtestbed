#ifndef _BBT_POLLER_H_
#define _BBT_POLLER_H_

#ifdef USE_POLL
namespace bbt{
/**
 * The poller class is used to help manage pollfd structs for use in the
 * updated serial and socket "port" code.
 *
 * @author Gianni Mariani <gianni@mariani.ws>
 * @short pollfd assistance class for port classes.
 */
class Poller 
{
private:
	int nufds;
	pollfd *ufds;

public:
	Poller();

	virtual ~Poller();

	/**
	 * reserve a specified number of poll descriptors.  If additional
	 * descriptors are needed, they are allocated.
	 *
	 * @return new array of descriptors.
	 * @param cnt number of desctiptors to reserve
	 */
	pollfd *getList(int cnt);

	/**
	 * Retreive the current array of poll descriptors.
	 *
	 * @return array of descriptors.
	 */
	inline	pollfd *getList(void)
		{return ufds;};
};
}
#endif

#endif
