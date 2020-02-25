/********************************************************************
* Name      :  EncodeImgPthd.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-06-07                                                 
********************************************************************/

#ifndef ENCODEIMGPTHD_H
#define ENCODEIMGPTHD_H

#include<semaphore.h>
#include "BaseThread.h"

class EncodeImgPthd:public BaseThread
{
public: 
  typedef char InitInfo;
  //初始化函数，主线程在run前调用,成功返回0，失败返回<0
  virtual int init(void* priData);
   int  initWithInfo(InitInfo *pInfo );
   int  setFaceNodeSemPost();
  
  //初始化函数，主线程在run后调用
  virtual void uninit();
  EncodeImgPthd();
  void stop();
  virtual ~EncodeImgPthd();
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
  	sem_t m_faceNodeAvailable;
};
#endif
