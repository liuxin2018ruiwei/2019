/********************************************************************
* Name      :  HeartBeatInfo.h                                  
* Describ   :  the class is used to  some valuse of heat beat
* Author    :  liuxin                                       
* Date      :  2018-06-25                                                 
********************************************************************/

#ifndef HEARTBEATINFO_H
#define HEARTBEATINFO_H

#include "Version.h"


typedef unsigned char UINT8;

enum
{
  MinHeatBeat = 0,
  MaxHeatBeat = 255
};


class HeartBeatInfo
{
 public: 
  UINT8 getHeatBeatInterval();	
  void setHeatBeatInterval(UINT8 value);

  
  long long  getLastSendTime();
  void setLastSendTime(long long value);
  
  long long getReplyTime();
  void setReplyTime(long long value);
  
  int getHeatBeatLostCount();
  void setHeatBeatLostCount(int value);

  void getNowTimeSetLastConTime();
  void getNowTimeSetLastReplyTime();

  int reset(UINT8 timeInterval);
  
  static HeartBeatInfo* getInstance();
	
  int initHeartBeatInfo(void *priData );
  virtual ~HeartBeatInfo();

  int  getUpState();
  void setUpState(int value);
	

 protected:
  HeartBeatInfo();
  
 private:
  static HeartBeatInfo* m_this;
  UINT8 		m_heatBeatInterval;
  long long   	m_lastSendTime;
  long long  	m_replyTime;
  int  	m_heatBeatLostCount;
  int 	m_upgradeState;
  
};
#endif
