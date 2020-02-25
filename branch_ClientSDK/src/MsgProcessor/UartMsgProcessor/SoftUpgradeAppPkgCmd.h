/********************************************************************
* Name      :  SoftUpgradeAppPkgCmd.h                                  
* Describ   :  the class is used to process msg which's code is 0x0201
* Author    :  Zhang Botong                                          
* Date      :  2018-06-15                                                 
********************************************************************/

#ifndef SOFT_UPGRADE_APP_PKG_CMD_H
#define SOFT_UPGRADE_APP_PKG_CMD_H

#include "UartCommand.h"

#define UART_SOFT_UPGRADE_COMMAND_LEN			(2)
#define UART_SOFT_UPGRADE_REC_BIT				(0x80)
#define UART_SOFT_UPGRADE_REC_CONTINUE_BIT		(0xC0)

#define  UART_ACTIVE_APP_PKG_COMMAND (0x0202) 
#define  UART_ACTIVE_APP_CFG_COMMAND (0x0232)
#define  UART_ACTIVE_APP_CAMERA_QPSI_IAMGE_COMMAND (0x0212)

#define  UART_SEND_FLAG_LOCATION (2)
#define  UART_SEND_BEAT_FLAG_SEND_BIT (0x00)
#define  UART_SEND_SEQUENCE_NUMBER_LOCATION (3)
#define  UART_SEND_LEN			(4)


#define UPGREAD_START 	(1)
#define UPGREAD_STOP	(0)

class SoftUpgradeAppPkgCmd: public UartCommand
{
 public: 
  SoftUpgradeAppPkgCmd();
  virtual int processMsg(Message * pMsg);
  
  virtual ~SoftUpgradeAppPkgCmd();
 protected:
 private:
};
#endif
