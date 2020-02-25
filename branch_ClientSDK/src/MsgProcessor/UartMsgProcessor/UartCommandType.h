/********************************************************************
* Name      :  UartCommandType.h                                  
* Describ   :  some common definaiton about uart command
* Author    :  Zhang Botong                                          
* Date      :  2018-06-14                                                 
********************************************************************/

#ifndef UARTCOMMANDTYPE_H
#define UARTCOMMANDTYPE_H

namespace UartCommandCode
{
  typedef unsigned short CodeType;
  static const CodeType QUERY_MODULE_INFO 		= 0x0101;
  static const CodeType QUERY_HEART_BEAT 		= 0x0102;
  static const CodeType QUERY_START_FACE_CHECK 	= 0x017F;
  static const CodeType QUERY_STOP_FACE_CHECK 	= 0x017E;
  static const CodeType QUERY_CAMERA_QSPI_IMAGE_UPGRADE = 0x0211;
  static const CodeType QUERY_APP_CFG_UPGRADE 	= 0x0231;
  static const CodeType QUERY_APP_PKG_UPGRADE 	= 0x0201;
  static const CodeType QUERY_FACE_COORDINATE 	= 0x1184; 
  static const CodeType QUERY_ACTIVE_APP_CFG 	= 0x0232;
  static const CodeType QUERY_ACTIVE_APP_PKG 	= 0x0202;
  static const CodeType QUERY_ACTIVE_APP_CAMERA_QPSI_IMAGE 	= 0x0212;
  static const CodeType QUERY_FACE_DETECTE_VERSION 	= 0x0105;

};
#endif
