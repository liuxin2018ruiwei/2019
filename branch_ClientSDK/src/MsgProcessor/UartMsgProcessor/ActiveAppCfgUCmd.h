/********************************************************************
* Name      :  ActiveAppCfgUCmd.h                                  
* Describ   :  the class is used to process msg which's code is 0x0232
* Author    :  Zhang Botong                                          
* Date      :  2018-06-15                                                 
********************************************************************/

#ifndef ACTIVE_APP_CFG_U_CMD_H
#define ACTIVE_APP_CFG_U_CMD_H

#include "UartCommand.h"


#define UART_APP_CFG_FLAG_REC_BIT  		(0x80)
#define UART_APP_CFG_COMMAND			(0x0232)
#define UART_APP_CFG_COMMAND_LEN		(2)


class ActiveAppCfgUCmd: public UartCommand
{
 public: 
  ActiveAppCfgUCmd();
  virtual int processMsg(Message * pMsg);
  
  virtual ~ActiveAppCfgUCmd();
 protected:
 private:
};
#endif
