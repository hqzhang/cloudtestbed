#include	<sys/types.h>
#include	"ourhdr.h" 
int		globvar = 6;		/* external variable in initialized data */
char	buf[] = "a write to stdout\n";

int
main(void)
{
	int		var;		/* automatic variable on the stack */
	pid_t	pid;

	var = 88;
	if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1)
		printf("write error\n");
	printf("before fork\n");	/* we don't flush stdout */

	if ( (pid = fork()) < 0)
		printf("fork error\n");
	else if (pid == 0) {		/* child */
		globvar++;					/* modify variables */
		var++;
	} else
		sleep(2);

	printf("pid = %d, globvar = %d, var = %d\n", getpid(), globvar, var);
	exit(0);
}
