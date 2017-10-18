#include	"ourhdr.h"

int
main(void)
{
	pid_t	pid;

	if ( (pid = fork()) < 0)
		printf("fork error");
	else if (pid == 0)		/* child */
		
		exit(0);

	/* parent */
	sleep(4);

	system("ps -la");

	exit(0);
}
