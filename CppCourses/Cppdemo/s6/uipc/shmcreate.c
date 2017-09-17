#include	"unpipc.h"

int
main(int argc, char **argv)
{
	int		c, fd, flags;
	char	*ptr;
	off_t	length;

	flags = O_RDWR | O_CREAT;
	while ( (c = getopt(argc, argv, "e")) != -1) {
		switch (c) {
		case 'e':
			flags |= O_EXCL;
			break;
		}
	}
	if (optind != argc - 2)
		err_quit("usage: shmcreate [ -e ] <name> <length>");
	length = atoi(argv[optind + 1]);

	fd = shm_open(argv[optind], flags, FILE_MODE);
	ftruncate(fd, length);

	ptr = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	exit(0);
}
