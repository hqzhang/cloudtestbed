#include	"unpipc.h"

int
main(int argc, char **argv)
{
	int		fd, i, n, val, *ptr;
	pid_t	childpid;

	if (argc != 2)
		err_quit("usage: test3 <pathname>");

	unlink(argv[1]);		/* error OK */
	fd = open(argv[1], O_RDWR | O_CREAT | O_EXCL, FILE_MODE);
	val = 9999;
	write(fd, &val, sizeof(int));	/* sets file size too */

	ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	close(fd);
	*ptr = 1;

	if ( (childpid = fork()) == 0) {
		fd = open(argv[1], O_RDONLY);		/* child */
		for (i = 0; i < 10; i++) {
			lseek(fd, 0, SEEK_SET);
			if ( (n = Read(fd, &val, sizeof(int))) != sizeof(int))
				err_quit("read returned %d", n);
			printf("val = %d\n", val);
			sleep(1);
		}
		exit(0);
	}
	waitpid(childpid, NULL, 0);

	exit(0);
}
