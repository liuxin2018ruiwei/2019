/********************************************************************
* Name      :  EncodeImgPthd.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-06-07                                                 
********************************************************************/

#include <stdio.h>
#include <unistd.h>
#include "EncodeImgPthd.h"
#include <FaceDataManage.h>
#include "EncodeModule.h"
#include <pthread.h>




//初始化函数，主线程在run前调用,成功返回0，失败返回<0
int EncodeImgPthd::init(void* priData)
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

int EncodeImgPthd:: initWithInfo(InitInfo *pInfo )
{
		
	if( nullptr == pInfo )
	{
		printf("ERR: %d %s pConfig=%P\n", __LINE__, __func__, pInfo );
		
		return -1;
	}

	sem_init(&m_faceNodeAvailable, 0, 0);

	return 0;
}

void EncodeImgPthd::stop()
{
	//printf("EncodeImgPthd stop/n");
	setFaceNodeSemPost();
	BaseThread::stop();
}

//初始化函数，主线程在run后调用
void EncodeImgPthd::uninit()
{
	if (true == isRunning())
	{
		stop();
	}
	sem_destroy( &m_faceNodeAvailable);
}

int EncodeImgPthd::setFaceNodeSemPost()
{
	return sem_post(&m_faceNodeAvailable);
}

EncodeImgPthd::EncodeImgPthd()
{
}

EncodeImgPthd::~EncodeImgPthd()
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
void EncodeImgPthd::run()
{
	while( isRunning() )
	{
		//usleep( 900*1000 );
		//printf("\nPthread EncodeImgPthd %d : %s;size = %d\n", __LINE__, __func__,EncodeModule::getInstance()->getFaceEncodeQue().size());
		//while (!EncodeModule::getInstance()->getFaceEncodeQue().empty())
		sem_wait(&m_faceNodeAvailable);
		FaceNode tempFaceNode = EncodeModule::getInstance()->getFaceEncodeQue().front();
		EncodeModule::getInstance()->processFaceInfo(&tempFaceNode);
		EncodeModule::getInstance()->getFaceEncodeQue().pop();
		//printf("run : size = %d \n",EncodeModule::getInstance()->getFaceEncodeQue().size());
	}

}
