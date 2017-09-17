#include	<stdlib.h>
#include <stdio.h>
static void	my_exit1(void), my_exit2(void);
void err_sys(const char *);
int
main(void)
{
	if (atexit(my_exit2) != 0)
		err_sys("can't register my_exit2");
	if (atexit(my_exit1) != 0)
		err_sys("can't register my_exit1");
	if (atexit(my_exit1) != 0)
		err_sys("can't register my_exit1");
	printf("main is done\n");
	return(0);
}
void err_sys(const char *s)
{
	printf("%s\n",s);
}

static void
my_exit1(void)
{
	printf("first exit handler\n");
}
static void
my_exit2(void)
{
	printf("second exit handler\n");
}
