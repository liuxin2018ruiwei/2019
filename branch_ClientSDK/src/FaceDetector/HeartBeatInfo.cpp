/********************************************************************
* Name      :  HeartBeatInfo.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   : to recode heat beat                                                       
* Author    :  liuxin                                        
* Date      :  2018-06-25                                                 
********************************************************************/
#include <stdio.h>
#include "HeartBeatInfo.h"
#include <time.h>
#include <sys/time.h>

HeartBeatInfo* HeartBeatInfo::m_this =  nullptr;

UINT8 HeartBeatInfo:: getHeatBeatInterval()
{
	//printf("\n\n get _heatBeatInterval = %d \n\n", m_heatBeatInterval);
	return m_heatBeatInterval;
}
void HeartBeatInfo::setHeatBeatInterval(UINT8 value)
{
	 m_heatBeatInterval= value;
	 //printf("\n\n set _heatBeatInterval = %d \n\n", m_heatBeatInterval);
}


long long HeartBeatInfo:: getLastSendTime()
{
	return m_lastSendTime;
}
void HeartBeatInfo::setLastSendTime(long long value)
{
	 m_lastSendTime= value;
}

long long HeartBeatInfo::getReplyTime()
{
	return m_replyTime;
}

void HeartBeatInfo:: setReplyTime(long long value)
{
	m_replyTime = value;
}

int HeartBeatInfo::getHeatBeatLostCount()
{
	return m_heatBeatLostCount;
}

void HeartBeatInfo:: setHeatBeatLostCount(int  value)
{
	m_heatBeatLostCount= value;
}


void  HeartBeatInfo::getNowTimeSetLastConTime()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	//printf("[LX] set time %d:%d\n",tv.tv_sec,tv.tv_usec);
	setLastSendTime( (long long)tv.tv_sec*1000 +  (long long)(tv.tv_usec/1000) );//ms
}

void  HeartBeatInfo::getNowTimeSetLastReplyTime()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	//printf("[LX] set time %u:%u\n",tv.tv_sec,tv.tv_usec);
	setReplyTime( (long long)tv.tv_sec*1000 +  (long long)(tv.tv_usec/1000));
	
}
//
int HeartBeatInfo::reset(UINT8 timeInterval)
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	//printf("[LX] set time %d:%d\n",tv.tv_sec,tv.tv_usec);
	setLastSendTime( (long long)tv.tv_sec*1000 +  (long long)(tv.tv_usec/1000));
	setReplyTime( (long long)tv.tv_sec*1000 +  (long long)(tv.tv_usec/1000));
	setHeatBeatLostCount(0);
	setHeatBeatInterval(timeInterval);
	return 0;
}


HeartBeatInfo* HeartBeatInfo::getInstance()
{
	if( nullptr == m_this )
	{
		m_this = new HeartBeatInfo;
	}

	return m_this;
}

int HeartBeatInfo::initHeartBeatInfo(void *priData )
{
#ifdef DEBUG_LOG
	printf("%d : %s\n", __LINE__, __func__);
#endif	
	
/*	if( nullptr == priData )
	{
		printf("ERR: %d %s priData=%P\n", __LINE__, __func__, priData );
		
		return -1;
	}
	*/
	//UINT8 initHeatBeatInterval = (*(UINT8 *)priData);
//	printf("\n[LX] test initHeatBeatInterval = %d",initHeatBeatInterval);
	getNowTimeSetLastReplyTime();
	getNowTimeSetLastConTime();
	setHeatBeatLostCount((long long)0);
	setHeatBeatInterval(0);

	return 0;

}

int  HeartBeatInfo:: getUpState()
{
	return m_upgradeState;
}
void  HeartBeatInfo::setUpState(int value)
{
	m_upgradeState = value;
}


HeartBeatInfo::~HeartBeatInfo()
{
}


HeartBeatInfo::HeartBeatInfo()
{
}
