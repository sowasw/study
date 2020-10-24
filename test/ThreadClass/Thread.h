#ifndef _THREAD_H_
#define _THREAD_H_

#include <pthread.h>
#include <functional>

///1
//面向对象的线程类：继承
class Thread{
public:
	Thread();
	virtual ~Thread();
	
	void start();
	void join();
	void setAutoDelete(bool autoDelete);
	
private:
	virtual void run() = 0;//c++的调用规则，隐含参数this指针
	static void* threadMain(void *arg);//c语言的调用规则
	pthread_t threadId;
	bool autoDelete;
};

///2
//基于对象，bind/function
class Thread2{
public:
	typedef std::function<void ()> ThreadFunc;
	explicit Thread2(ThreadFunc threadFunc);
	~Thread2();
	
	void start();
	void join();
	void setAutoDelete(bool autoDelete);
	
private:
	void run();
	static void* threadMain(void *arg);
	pthread_t threadId;
	bool autoDelete;
	ThreadFunc threadCallBack;
};



#endif
