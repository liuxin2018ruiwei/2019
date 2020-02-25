/********************************************************************
* Name      :  SoftUpgradeReq.h                                  
* Describ   :  This function is used to send a heartbeat connection request 
* Author    :  liuxin                                          
* Date      :  2018-06-25                                                 
********************************************************************/

#ifndef SOFET_UPGRADE_REQ_H
#define SOFET_UPGRADE_REQ_H

#include "Requestion.h"

#include "BaseReq.h"

#define UART_SOFT_UPGRADE_APP_CFG_COMMAND				(0x0231)
#define UART_SOFT_UPGRADE_APP_PKG_COMMAND				(0x0201)
#define UART_SOFT_UPGRADE_CAMREA_QSPI_IMAGE_COMMAND		(0x0211)

#define UART_SOFT_UPGRADE_SEND_LEN			(92)

#define UART_SOFT_UPGRADE_COMMAND_LOCATION 			(0)
#define UART_SOFT_UPGRADE_FLAG_LOCATION				(2)
#define UART_SOFT_UPGRADE_SEQUENCE_NUMBER_LOCATION 	(3)
#define UART_SOFT_UPGRADE_FILE_NAME_LOCATION		(4)
#define UART_MID5_LOCATION							(68)
#define UART_TRANSFER_METHOD_LOCATION				(84)
#define UART_REVERSED_LOCATION						(85)
#define UART_FILE_SIZE_LOCATION						(88)

#define UART_FILE_NAME_LEN 							(64)
#define UART_MID5_CHECK_SUM_LEN 					(16)
#define UART_SEND_BEGIN_OFFSET (4) //发送报文从12开始偏移，发送从SQU_NUM后面开始，而我发送重COMMAND开始的，要偏移到SQU_NUM



#define UART_SOFT_UPGRADE_FLAG_SEND_BIT		(0x00)



class SoftUpgradeReq:public BaseReq
{
 public: 
  SoftUpgradeReq();
  virtual int processReq( Requestion *pReq );
  virtual ~SoftUpgradeReq();
 protected:
 private:
	//static UINT8 m_sequenceNumber;
};

#endif
