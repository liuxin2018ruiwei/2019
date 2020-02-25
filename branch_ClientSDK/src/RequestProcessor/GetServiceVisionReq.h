/********************************************************************
* Name      :  GetServiceVisionReq.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-06-08                                                 
********************************************************************/

#ifndef GETSERVICEVISIONREQ_H
#define GETSERVICEVISIONREQ_H

#include "BaseReq.h"

#define UART_GET_MODULE_INFO_SEND_LEN (4)
#define UART_GET_MODULE_INFO_COMMAND (0x0101)
#define UART_GET_MODLUE_INFO_FLAG_LOCATION (2)
#define UART_GET_FLAG_SEND_BIT (0x00)
#define UART_SEQUENCE_NUMBER_LOCATION (3)


class GetServiceVisionReq:public BaseReq
{
 public: 
  GetServiceVisionReq();
  virtual int processReq( Requestion *pReq );
  virtual ~GetServiceVisionReq();
 protected:
 private:
};
#endif
