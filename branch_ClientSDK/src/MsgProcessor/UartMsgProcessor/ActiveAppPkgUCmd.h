/********************************************************************
* Name      :  ActiveAppPkgUCmd.h                                  
* Describ   :  the class is used to process msg which's code is 0X0202
* Author    :  Zhang Botong                                          
* Date      :  2018-06-15                                                 
********************************************************************/

#ifndef ACTIVE_APP_PKG_UCMD_H
#define ACTIVE_APP_PKG_UCMD_H

#include "UartCommand.h"

#define UART_APP_PKG_FLAG_REC_BIT  		(0x80)
#define UART_APP_PKG_COMMAND			(0x0202)
#define UART_APP_PKG_COMMAND_LEN		(2)


class ActiveAppPkgUCmd: public UartCommand
{
 public: 
  ActiveAppPkgUCmd();
  virtual int processMsg(Message * pMsg );
  
  virtual ~ActiveAppPkgUCmd();
 protected:
 private:
};
#endif
