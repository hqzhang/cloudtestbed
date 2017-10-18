#include <slog.h>
#include <pthread.h>

#include <iostream>
using namespace std;
using namespace bbt;
class MyThread:public Thread
{
protected:
void run(){
	bbt::slog.info()<<"My thread running...!"<<endl;
	sleep(10000);
	bbt::slog.info()<<"My thread end"<<endl;
	};
};
int main()
{

	/*
	   bbt::Mutex::setDebug(true);
	   Mutex mt("testMutex");

	   mt.enterMutex();
	   mt.leaveMutex();
	 */
	char c;
	MyThread my;
	Thread *main=getThread();
	my.start();
	if(my.isRunning()){
		my.suspend();
		cout <<"suspend thread"<<endl;
		cout <<"put any ch to resume"<<endl;
		cin.get(c);
		my.resume();
		cin.get(c);
	}
	bbt::slog("MYTEST");
	bbt::slog.alert()<<"this is slog test!"<<endl;
	bbt::slog.error()<<"error message"<<endl;
}
