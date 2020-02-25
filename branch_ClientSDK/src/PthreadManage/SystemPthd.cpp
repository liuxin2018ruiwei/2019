
/********************************************************************
* Name      :  SystemPthd.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  liuxin                                         
* Date      :  2018-06-23                                                 
********************************************************************/

#include <stdio.h>
#include <unistd.h>
#include "SystemPthd.h"
#include "HeartBeatInfo.h"
#include <sys/time.h>
#include "SendHeartBeatReq.h"
#include "Requestion.h"
#include "CommunicationAgent.h"

#include <time.h>
#include "ProcessReq.h"
#include "FaceDetector.h"
#include "Communication.h"
#include "UserCommonRequestion.h"
#include "ConfigManage.h"



//初始化函数，主线程在run前调用,成功返回0，失败返回<0
int SystemPthd::init(void* priData)
{
	
	if( nullptr == priData )
	{
		printf("ERR: %d %s pConfig=%P\n", __LINE__, __func__, priData );
		
		return -1;
	}
	//heatBeatLostCount = 0;
	
	return 0;

}
int SystemPthd::initWithInfo(InitInfo *pInfo )
{
	if( nullptr == pInfo )
	{
		printf("ERR: %d %s pConfig=%P\n", __LINE__, __func__, pInfo );
		
		return -1;
	}
	
	return 0;
}



//初始化函数，主线程在run后调用
void SystemPthd::uninit()
{

}

SystemPthd::SystemPthd()
{
}

SystemPthd::~SystemPthd()
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


int SystemPthd::sendHeatBeat()
{

	struct timeval tv;
	gettimeofday(&tv,NULL);
	long long newTime = (long long)tv.tv_sec*1000 +  (long long)(tv.tv_usec/1000);//ms

	long long getSendTime  = HeartBeatInfo::getInstance()->getLastSendTime();
	long long getReplyTime = HeartBeatInfo::getInstance()->getReplyTime();
	long long  getIntervalTime = HeartBeatInfo::getInstance()->getHeatBeatInterval();//获取到的是s
	getIntervalTime = getIntervalTime * 1000;//change to s

	

	if((getSendTime -getReplyTime) > (LOST_TIME_COEFFICIENT * getIntervalTime))
	{
		//printf("getSendTime = %lld ,getRepalyTime = %lld \n",getSendTime, getReplyTime);
		int heatBeatCount = HeartBeatInfo::getInstance()->getHeatBeatLostCount();
		heatBeatCount++;
		HeartBeatInfo::getInstance()->setHeatBeatLostCount(heatBeatCount);
		if(heatBeatCount >= MAX_LOST_TIME)
		{
			//notif 
			heatBeatCount = 0;
			HeartBeatInfo::getInstance()->setHeatBeatLostCount(heatBeatCount);
		 
			CallbackNotifyFunc pFun= ProcessReq::getInstance()->getClientNotifyFun();

			if(*pFun == NULL)
			{
				printf("CallbackNotifyFunc pFun is NULL \n");
				return -1;
			}
			RWNotifyType myRWNotifyType = NotifyState;
			UINT8 pExtData = Unusual;
			UINT32 pDataLen = 1;
			(*pFun)(&myRWNotifyType,&pExtData,&pDataLen);
		}
	}

	if( (newTime - getReplyTime ) >  getIntervalTime )
	{
		Requestion *rq = new Requestion(Requestion::SendHeartBeatConReq);
		return CommunicationAgent::getInstance()->sendReq( rq );
	}

}

void SystemPthd::judgeRecoverBarudrate()
{
	if(SEND_GET_SOFET_VERSION_AFTER_CHANGE_FPGA == FaceDetector::getInstance()->getFlagRecoverBarudrate())
	{
		struct timeval tv;
		gettimeofday(&tv,NULL);
		long long getNewTime = (long long)tv.tv_sec*1000 +  (long long)(tv.tv_usec/1000);
		//printf("getnewtime = %lld\n",getNewTime);

		long long getRecoverBarudTime = FaceDetector::getInstance()->getRecoverBarudTime();
		//printf("getNewTime = %lld, getTime = %lld,  newTime -  getRecoverBarudTime = %lld\n", getNewTime, getRecoverBarudTime,
		//																		getNewTime -  getRecoverBarudTime);
		if(getNewTime -  getRecoverBarudTime > 1000)
		{
			//printf("-----> recove to 115200 \n\n\n\n");
			
			Communication::getInstances()->setBautrate(115200);
			usleep(1000 * 1000);
				
			FaceDetector::getInstance()->setflagRecoverBarudrate(FLAG_NO_USE);

		}
		
	}

	if(GET_DATA_AFTER_SEND_GET_VERION == FaceDetector::getInstance()->getFlagRecoverBarudrate())
	{
		FaceDetector::getInstance()->setflagRecoverBarudrate(FLAG_NO_USE);
	}

	
	
}

void SystemPthd::run()
{
	while( isRunning() )
	{
		usleep( 25*1000 );
		//printf("\nPthread systempthd run sendHeatbeat\n");
		judgeRecoverBarudrate();
		if( ( (HeartBeatInfo::getInstance()->getHeatBeatInterval()) != 0)
		   && ( (HeartBeatInfo::getInstance()->getUpState()) ==0) )//heart beat test is close
		{
			sendHeatBeat();
		}
		
	}

}
