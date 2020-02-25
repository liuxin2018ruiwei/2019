/********************************************************************
* Name      :  MsgReqProcPthd.cpp                                  
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-06-07                                                 
********************************************************************/

#include <stdio.h>
#include <unistd.h>
#include "MsgReqProcPthd.h"
#include "RequestProcessor.h"


//初始化函数，主线程在run前调用,成功返回0，失败返回<0
int MsgReqProcPthd::init(void* priData)
{
#ifdef DEBUG_LOG
	printf("%d : %s\n", __LINE__, __func__);
#endif	
	
	if( nullptr == priData )
	{
		printf("ERR: %d %s pConfig=%P\n", __LINE__, __func__, priData );
		
		return -1;
	}

	return 0;
}

int MsgReqProcPthd:: initWithInfo(InitInfo *pInfo )
{
		
		if( nullptr == pInfo )
		{
			printf("ERR: %d %s pConfig=%P\n", __LINE__, __func__, pInfo );
			
			return -1;
		}
		sem_init( &m_reqAvailable, 0, 0);
		//pthread_detach(pthread_self());
		return 0;
}

void MsgReqProcPthd::stop()
{
	setReqSemPost();
	BaseThread::stop();
}

//初始化函数，主线程在run后调用
void MsgReqProcPthd::uninit()
{
	if (true == isRunning())
	{
		stop();
	}

	sem_destroy( &m_reqAvailable);
}

int MsgReqProcPthd::setReqSemPost()
{
	return sem_post(&m_reqAvailable);
}


MsgReqProcPthd::MsgReqProcPthd()
{
}

MsgReqProcPthd::~MsgReqProcPthd()
{
}

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
 //发送请求的线程， 请求和数据回复都在该线程，便于控制以及防止发送和接收的不一致
void MsgReqProcPthd::run()
{
#ifdef DEBUG_LOG
	printf("%d : %s\n", __LINE__, __func__);
#endif
	RequestProcessor* pProcor = RequestProcessor::getInstance();
	while( isRunning() )
	{
		//usleep( 700 *1000 );
		//printf("\n[LX] uart send recive -->PthreadMsgReqProcPthd::run()\n");
		sem_wait(&m_reqAvailable);
		pProcor->processReq();

	}

}
