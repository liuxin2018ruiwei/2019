/********************************************************************
* Name      :  SendHeartBeatReq.h                                  
* Describ   :  This function is used to send a heartbeat connection request 
* Author    :  liuxin                                          
* Date      :  2018-06-25                                                 
********************************************************************/

#ifndef SEND_HEART_BEAT_REQUESTION_H
#define SEND_HEART_BEAT_REQUESTION_H

#include "Requestion.h"

#include "BaseReq.h"

#define UART_HEART_BEAT_FLAG_SEND_BIT		(0x00)
#define UART_HEART_BEAT_FLAG_REC_BIT  		(0x80)
#define UART_HEART_BEAT_FLAG_REPORT_BIT		(0x10)
#define UART_HEART_BEAT_FLAG_UPGRADE_BIT	(0xC0)
#define UART_HEART_BEAT_COMMAND				(0x0102)
#define UART_HEART_BEAT_COMMAND_LEN			(2)
#define UART_HEART_BEAT_SEND_LEN			(4)
#define UART_HEART_BEAT_COMMAND_LOCATION 	(0)
#define UART_HEART_BEAT_FLAG_LOCATION		(2)
#define UART_HEART_BEAT_SEQUENCE_NUMBER_LOCATION (3)


class SendHeartBeatReq:public BaseReq
{
 public: 
  SendHeartBeatReq();
  virtual int processReq( Requestion *pReq );
  virtual ~SendHeartBeatReq();
 protected:
 private:
 	//static int m_sequenceNumber;
	int sendHeatBeatByUart();
};

#endif
