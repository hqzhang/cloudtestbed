#include	"unpipc.h"

#define	SEM_NAME	"mysem"

/* include diff */
int
main(int argc, char **argv)
{
	int		i, nloop;
	int		*ptr;
	sem_t	*mutex;

	if (argc != 2)
		err_quit("usage: incr_map_anon <#loops>");
	nloop = atoi(argv[1]);

		/* 4map into memory */
	ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
			   MAP_SHARED | MAP_ANON, -1, 0);
/* end diff */

		/* 4create, initialize, and unlink semaphore */
	mutex = sem_open(SEM_NAME, O_CREAT | O_EXCL, FILE_MODE, 1);
	sem_unlink(SEM_NAME);

	setbuf(stdout, NULL);	/* stdout is unbuffered */
	if (fork() == 0) {		/* child */
		for (i = 0; i < nloop; i++) {
			sem_wait(mutex);
			printf("child: %d\n", (*ptr)++);
			sem_post(mutex);
		}
		exit(0);
	}

		/* 4parent */
	for (i = 0; i < nloop; i++) {
		sem_wait(mutex);
		printf("parent: %d\n", (*ptr)++);
		sem_post(mutex);
	}
	exit(0);
}
