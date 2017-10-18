AR=gar
CC=gcc
CFLAGS = -g -O -D_REENTRANT -Wall -D__EXTENSIONS__
LIBS= lib/libunp.a -lresolv -lsocket -lnsl -lpthread

export AR CC CFLAGS 

LIBDIRS=lib
TCPSERV = tcpserv01 tcpserv02 tcpserv03 tcpserv04
TCPCLI = tcpcli01 tcpcli04
SELECT = tcpcli06 tcpcli07 tcpcli08 tcpservselect01 tcpservpoll01

SO =	checkopts prdefaults rcvbuf rcvbufset sockopt

PROGS=daytimetcpcli daytimetcpsrv daytimetcpsrv1 $(TCPSERV) $(TCPCLI) $(SELECT) $(SO)
    
    
all:$(LIBDIRS)	$(PROGS)

.PHONY : $(LIBDIRS) clean distclean

$(LIBDIRS):
	$(MAKE) -C $@

clean:
	@rm -f *.o
	$(MAKE) -C $(LIBDIRS) clean

distclean: clean
	@rm -f $(PROGS)



daytimetcpcli:	daytimetcpcli.o
	${CC} ${CFLAGS} -o $@ daytimetcpcli.o ${LIBS}

daytimetcpsrv:	daytimetcpsrv.o
	${CC} ${CFLAGS} -o $@ daytimetcpsrv.o ${LIBS}

daytimetcpsrv1:	daytimetcpsrv1.o
	${CC} ${CFLAGS} -o $@ daytimetcpsrv1.o ${LIBS}

tcpserv01:	tcpserv01.o
	${CC} ${CFLAGS} -o $@ tcpserv01.o ${LIBS}
tcpcli01:	tcpcli01.o
	${CC} ${CFLAGS} -o $@ tcpcli01.o ${LIBS}

tcpcli04:	tcpcli04.o
	${CC} ${CFLAGS} -o $@ tcpcli04.o ${LIBS}

tcpserv02:	tcpserv02.o sigchldwait.o
		${CC} ${CFLAGS} -o $@ tcpserv02.o sigchldwait.o ${LIBS}

tcpserv03:	tcpserv03.o sigchldwait.o
		${CC} ${CFLAGS} -o $@ tcpserv03.o sigchldwait.o ${LIBS}

tcpserv04:	tcpserv04.o sigchldwaitpid.o
		${CC} ${CFLAGS} -o $@ tcpserv04.o sigchldwaitpid.o ${LIBS}


sockopt:	sockopt.o
		${CC} ${CFLAGS} -o $@ sockopt.o ${LIBS}

checkopts:	checkopts.o
		${CC} ${CFLAGS} -o $@ checkopts.o ${LIBS}

prdefaults:	prdefaults.o
		${CC} ${CFLAGS} -o $@ prdefaults.o ${LIBS}

rcvbuf:	rcvbuf.o
		${CC} ${CFLAGS} -o $@ rcvbuf.o ${LIBS}

rcvbufset:	rcvbufset.o
		${CC} ${CFLAGS} -o $@ rcvbufset.o ${LIBS}


tcpcli06:	tcpcli06.o strcliselect01.o
		${CC} ${CFLAGS} -o $@ tcpcli06.o strcliselect01.o ${LIBS}

tcpcli07:	tcpcli07.o strcliselect02.o
		${CC} ${CFLAGS} -o $@ tcpcli07.o strcliselect02.o ${LIBS}

tcpcli08:	tcpcli08.o
		${CC} ${CFLAGS} -o $@ tcpcli08.o ${LIBS}

tcpservselect01:	tcpservselect01.o
		${CC} ${CFLAGS} -o $@ tcpservselect01.o ${LIBS}

tcpservpoll01:	tcpservpoll01.o
		${CC} ${CFLAGS} -o $@ tcpservpoll01.o ${LIBS}

clean:
