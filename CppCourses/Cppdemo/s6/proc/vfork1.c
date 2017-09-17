#include	<sys/types.h>
#include	"ourhdr.h"

int		globvar = 6;		/* external variable in initialized data */

int
main(void)
{
	int		var;		/* automatic variable on the stack */
	pid_t	pid;

	var = 88;
	printf("before vfork\n");	/* we don't flush stdio */

	if ( (pid = vfork()) < 0)
		printf("vfork error");
	else if (pid == 0) {		/* child */
		globvar++;					/* modify parent's variables */
		var++;
		_exit(0);				/* child terminates */
	}

	/* parent */
	printf("pid = %d, globvar = %d, var = %d\n", getpid(), globvar, var);
	exit(0);
}
