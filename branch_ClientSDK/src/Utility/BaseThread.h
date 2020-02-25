#ifndef BASE_THREAD_H__
#define BASE_THREAD_H__


#include <pthread.h>  


class BaseThread
{

public:  
    BaseThread();  
    virtual ~BaseThread();//虚析构函数  

	//初始化函数，主线程在run前调用,成功返回0，失败返回<0
	virtual int init(void* priData) = 0;

	//初始化函数，主线程在run后调用
	virtual void uninit() = 0;
	
    void start();//线程的启动方法  
    void join();//等待线程结束并且收回被等待线程的资源  
  	void stop();
    //void SetAutoDelete(bool autoDelete);  
    //Run 函数需要通过该接口判断外部是否停止线程，如果线程被外部停止，则结束Run函数的运行
    bool isRunning() const { return _isRunning;} 
    long getThreadId() const {return _threadId;}	
private:  
    static void* threadRoutine(void* arg);//线程入口函数  也是静态全局函数 

	/*
	*	线程执行体run,通过继承类实现不同的线程函数run 
	*	Run 函数需要通过该接口判断外部是否停止线程，如果线程被外部停止，则结束Run函数的运行
	*	example:
	*	void run()
	*	{
	*		while( isRunning())
	*		{
	*			do_something();
	*		}
	*	}
	*
	*/
    virtual void run() = 0;   
    
    pthread_t _threadId;//线程ID
    bool _isRunning;
  
	
};


#endif

