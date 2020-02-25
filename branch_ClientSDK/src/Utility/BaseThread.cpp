#include "BaseThread.h"
#include <stdio.h>



BaseThread::BaseThread()
	: _isRunning(false)
{  

}  
  
BaseThread::~BaseThread()
{  

}  
  
void BaseThread::start()//启动线程  
{  
	if( _isRunning )
		return ;
	_isRunning = true;
    int ret = pthread_create(&_threadId, NULL, threadRoutine, this);  
    //第三个参数这里并不能直接放上执行函数run  
    //因为run函数式普通的成员函数，隐含的第一个参数是Thread*(就是this)  
    //调用的时候是thiscall约定，也就是说他不能做为入口函数  
}


void BaseThread::stop()
{
	_isRunning = false;
	join();
}

  
void BaseThread::join() //等待线程执行结束并且收回被等待线程的资源  
{  
	//printf(">join pthread_join _threadId = %x\n",_threadId);
	if(0 != _threadId)
	{
		pthread_join(_threadId, NULL);
	}
	//printf("now <join pthread_join _threadId = %x,_isRunning = %d\n",_threadId,_isRunning);
}  
  
void* BaseThread::threadRoutine(void* arg)//  
{  
    BaseThread* thread = static_cast<BaseThread*>(arg);//派生类指针转换成基类指针  
    thread->run();  
    pthread_exit(NULL);
    return NULL;  
}




  


