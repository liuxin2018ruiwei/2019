/********************************************************************
* Name      :  MsgReqProcPthd.h                                  
* Describ   :  the class is used to manage a pthread for process message and requestion
* Author    :  Zhang Botong                                          
* Date      :  2018-06-07                                                 
********************************************************************/

#ifndef MSGREQPROCPTHD_H
#define MSGREQPROCPTHD_H

#include<semaphore.h>
#include "BaseThread.h"

class MsgReqProcPthd:public BaseThread
{
public: 
  typedef char InitInfo;
  //初始化函数，主线程在run前调用,成功返回0，失败返回<0
  virtual int init(void* priData);
   int  initWithInfo(InitInfo *pInfo );
  int setReqSemPost();
  
  //初始化函数，主线程在run后调用
  virtual void uninit();
  void stop();
  MsgReqProcPthd();
  virtual ~MsgReqProcPthd();
 protected:
 private:
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
    virtual void run();   
	sem_t m_reqAvailable;
	
};
#endif
