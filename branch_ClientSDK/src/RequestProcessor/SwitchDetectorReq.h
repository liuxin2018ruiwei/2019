/********************************************************************
* Name      :  SwitchDetectorReq.h                                  
* Describ   :  This function is used to start or stop face check request 
* Author    :  liuxin                                          
* Date      :  2018-06-28                                                 
********************************************************************/

#ifndef SEND_SWITCH_DETECOR_REQUESTION_H
#define SEND_SWITCH_DETECOR_REQUESTION_H

#include "Requestion.h"

#include "BaseReq.h"

#define UART_FACE_CHECK_FLAG_SEND_BIT		(0x00)
#define UART_FACE_CHECK_FLAG_REC_BIT  		(0x80)
#define UART_FACE_CHECK_FLAG_REPORT_BIT		(0x10)
#define UART_FACE_CHECK_FLAG_UPGRADE_BIT	(0xC0)

#define UART_START_FACE_CHECK_COMMAND				(0x017F)
#define UART_STOP_FACE_CHECK_COMMAND				(0x017E)

#define UART_FACE_CHECK_COMMAND_LEN			(2)
#define UART_FACE_CHECK_SEND_LEN			(4)
#define UART_FACE_CHECK_COMMAND_LOCATION 	(0)
#define UART_FACE_CHECK_FLAG_LOCATION		(2)
#define UART_FACE_CHECK_SEQUENCE_NUMBER_LOCATION (3)


class SwitchDetectorReq:public BaseReq
{
 public: 
  SwitchDetectorReq();
  virtual int processReq( Requestion *pReq );
  virtual ~SwitchDetectorReq();
 protected:
 private:
	int sendStartFaceCheckByUart();
	int sendStopFaceCheckByUart();
};

#endif
