CXXFLAGS = -D_REENTRANT -g -I./include
CXX = g++
vpath %h ./include

.PHONY: clean all
all:semaphore.o slog.o thread.o systime.o threadkey.o mutex.o friends.o
semaphore.o:semaphore.cpp synchronization.h bthread.h
slog.o:slog.cpp private.h bthread.h
thread.o:thread.cpp private.h bthread.h 
systime.o:systime.cpp systime.h
threadkey.o:threadkey.cpp bthread.h 
mutex.o:mutex.cpp synchronization.h
friends.o:friends.cpp
thread1:thread1.cpp all
	g++  -o$@ $(CXXFLAGS) $<  slog.o semaphore.o systime.o thread.o mutex.o friends.o threadkey.o -lpthread 

thread2:thread2.cpp all
	g++  -o$@ $(CXXFLAGS) $<  slog.o semaphore.o systime.o thread.o mutex.o friends.o threadkey.o -lpthread 

thread3:thread3.cpp all
	g++  -o$@ $(CXXFLAGS) $<  slog.o semaphore.o systime.o thread.o mutex.o friends.o threadkey.o -lpthread 

clean:
	rm -f *.o
	
