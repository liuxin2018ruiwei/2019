/********************************************************************
* Name      :  QueryInfoUCmd.h                                  
* Describ   :  the class is used to process msg which's code is 0x0101
* Author    :  Zhang Botong                                          
* Date      :  2018-06-15                                                 
********************************************************************/

#ifndef QUERYINFOUCMD_H
#define QUERYINFOUCMD_H

#include "UartCommand.h"

#define UART_GET_FACE_VERSION_COMMAND_LEN (2)
#define PROTOCOL_VERSION_LEN (2)
#define UART_GET_MODULE_INFO_COMMAND (0x0101)
#define UART_GET_MODULE_INFO_FLAG_REC_BIT (0x80)



class QueryInfoUCmd: public UartCommand
{
 public: 
  QueryInfoUCmd();
  virtual int processMsg(Message * pMsg);
  
  virtual ~QueryInfoUCmd();
 protected:
 private:
};
#endif
