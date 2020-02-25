/********************************************************************
* Name      :  NotifyFaceInfoUCmd.h                                  
* Describ   :  the class is used to process msg which's code is 0x0101
* Author    :  Zhang Botong                                          
* Date      :  2018-06-15                                                 
********************************************************************/

#ifndef NOTIFY_FACE_INFO_U_CMD_H
#define NOTIFY_FACE_INFO_U_CMD_H

#include "UartCommand.h"

#define COUNT_PEOPLE_TIME (5)


#define UART_COMMAND_LEN			(2)
#define UART_FACE_COORDINATE_COMMAND				(0x1184)
#define UART_FACE_COORDINATE_REC_BIT				(0x80)
#define MAX_SEND_LEN 	(40)


class NotifyFaceInfoUCmd: public UartCommand
{
 public: 
  NotifyFaceInfoUCmd();
  virtual int processMsg(Message * pMsg );
  
  virtual ~NotifyFaceInfoUCmd();
 protected:
 private:
 	int everyIntervalNotifyUse();
 	int lastTime;
};
#endif
