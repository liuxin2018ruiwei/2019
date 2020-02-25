/********************************************************************
* Name      :  ActiveCamreaQPSIImageUCmd.h                                  
* Describ   :  the class is used to process msg which's code is 0X0212
* Author    :  Zhang Botong                                          
* Date      :  2018-06-15                                                 
********************************************************************/

#ifndef ACTIVE_CAMERA_QPSI_IMAGE_UCMD_H
#define ACTIVE_CAMERA_QPSI_IMAGE_UCMD_H

#include "UartCommand.h"

#define UART_CAMERA_QPSI_IMAGE_FLAG_REC_BIT  	(0x80)
#define UART_CAMERA_QPSI_IMAGE_COMMAND			(0x0212)
#define UART_CAMERA_QPSI_IMAGE_COMMAND_LEN		(2)


class ActiveCamreaQPSIImageUCmd: public UartCommand
{
 public: 
  ActiveCamreaQPSIImageUCmd();
  virtual int processMsg(Message * pMsg);
  
  virtual ~ActiveCamreaQPSIImageUCmd();
 protected:
 private:
};
#endif
