/********************************************************************
* Name      :  SystemPthd.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  liuxin                                         
* Date      :  2018-06-23                                                
********************************************************************/

#ifndef SYSTEMPTHD_H
#define SYSTEMPTHD_H

#include "BaseThread.h"
//typedef unsigned long int UINT64;
typedef long long unsigned int UINT64;


class SystemPthd:public BaseThread
{
public: 
#define MAX_LOST_TIME	(3)
#define LOST_TIME_COEFFICIENT 	(1.5)
#define RESTRAT_TIME_MAX (20)

  typedef char InitInfo;
  //初始化函数，主线程在run前调用,成功返回0，失败返回<0
  virtual int init(void* priData );
  int initWithInfo(InitInfo *pInfo );
  
  //初始化函数，主线程在run后调用
  virtual void uninit();
  SystemPthd();

  int sendHeatBeat();
  virtual ~SystemPthd();
 protected:
 	void judgeRecoverBarudrate();
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
};
#endif
