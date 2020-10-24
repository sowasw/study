#include "Thread.h"
#include <iostream>
using namespace std;

///1
Thread::Thread():autoDelete(false){
	cout << "Thread ..." << endl;
}
Thread::~Thread(){
	cout << "~Thread ..." << endl;
}
	
void Thread::start(){
	pthread_create(&threadId, NULL, &threadMain, this);
	//this指针通过参数传过去后要静态转换（static_cast），需要编译时确定
	//c++规则：类中的this指针是不确定的，因为有可能是指向派生类的指针，
	//所以要把this指针转换为让c语言调用规则能正确调用的指针（这里是当前类为基类的指针）
	
	//pthread_create不能调用非静态成员函数，threadMain必须为静态成员函数或者全局函数
	//因为非静态成员函数是c++规则
	//静态成员函数不能使用非静态成员函数及非静态成员变量，不同对象的成员不一致，需要通过当前对象的指针（this指针作为参数传递，并转换成当前类型指针）调用
	
	//pthread_create，join可以直接使用非静态的成员变量，不同对象的成员函数指针指向的地址可能不同，但是其成员变量的地址固定
	
	//静态函数、全局函数为c语言调用规则
    //非静态成员函数是c++规则，隐含参数this指针，而this指针是类型不确定的，动态的指向其派生类
    
    //下面的threadMain函数就是例子
}

void Thread::join(){
	pthread_join(threadId, NULL);
}
	
void Thread::setAutoDelete(bool autoDelete){
	this->autoDelete = autoDelete;
}

void* Thread::threadMain(void *arg){
	Thread *thread = static_cast<Thread*>(arg);//静态成员函数不能使用非静态成员函数及非静态成员变量
	thread->run();//必须通过Thread *型指针来调用，直接使用run函数是不行的
	
	if(thread->autoDelete)//线程结束，自动销毁线程对象
		delete thread;
		
	return NULL;
}


///2
Thread2::Thread2(ThreadFunc threadFunc)
			:autoDelete(false), threadCallBack(threadFunc)
{
	cout << "Thread2 ..." << endl;
}
Thread2::~Thread2(){
	cout << "~Thread2 ..." << endl;
}
	
void Thread2::start(){
	pthread_create(&threadId, NULL, &threadMain, this);
}

void Thread2::join(){
	pthread_join(threadId, NULL);
}
	
void Thread2::setAutoDelete(bool autoDelete){
	this->autoDelete = autoDelete;
}

void* Thread2::threadMain(void *arg){
	Thread2 *thread = static_cast<Thread2 *>(arg);//静态成员函数不能使用非静态成员函数
	thread->run();
	
	if(thread->autoDelete)//线程结束，自动销毁线程对象
		delete thread;
		
	return NULL;
}

void Thread2::run(){
	threadCallBack();
}
