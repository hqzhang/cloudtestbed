#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	"ourhdr.h"

#define	FIFO	"temp.fifo"

int
main(void)
{
	int		fdread, fdwrite;
	char *outstr = "Output through FIFO\n";
	char str[255];

	unlink(FIFO);
	if (mkfifo(FIFO, FILE_MODE) < 0)
		err_sys("mkfifo error");

	if ( (fdread = open(FIFO, O_RDONLY | O_NONBLOCK)) < 0)
		err_sys("open error for reading");
	if ( (fdwrite = open(FIFO, O_WRONLY)) < 0)
		err_sys("open error for writing");

	clr_fl(fdread, O_NONBLOCK);
	write(fdwrite,outstr,strlen(outstr)+1);
	if(read(fdread,str,255)<0)
		err_sys("read error");
	printf("read from FIFO: %s",str);

	exit(0);
}
