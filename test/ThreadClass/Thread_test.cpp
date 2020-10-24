#include "Thread.h"
#include <iostream>
#include <unistd.h>
using namespace std;

class ThreadTest:public Thread{
public:
	ThreadTest(int n):count(n){
		cout << "ThreadTest ..." << endl;
	}
	~ThreadTest(){
		cout << "~ThreadTest ..." << endl;
	}
	
private:
	virtual void run(){
		while(count--){
			cout << "ThreadTest run ..." << endl;
			sleep(1);
		}
	}
private:
	int count;
};

int main(){
	ThreadTest t(5);
	//t.setAutoDelete(true);//栈上的对象不能自动销毁
	t.start();
	t.join();

	ThreadTest *t2 = new ThreadTest(5);
	t2->setAutoDelete(true);
	t2->start();
	t2->join();
	
	pause();
	
	return 0;
}
