/********************************************************************
* Name      :  Version.h                                  
* Describ   :  control the version of the SDK
* Author    :  Zhang Botong                                          
* Date      :  2018-06-05                                                 
********************************************************************/

#ifndef VERSION_H
#define VERSION_H

/*
Templet:

#define VERSION_RELEASE_XXXX
//#define VERSION_RELEASE_YYYY
//#define VERSION_DEBUG_AAA


#ifdef VERSION_RELEASE_XXXX

#define XXXXXXXX xxxx
#define YYYYYYYY yyyy

#endif

#ifdef VERSION_RELEASE_YYYY

#define XXXXXXXX xxxx
#define YYYYYYYY yyyy

#endif

 */

//***********************************************************
// !!!!! Just Only one MARCRO can be defined

#define VERSION_DEBUG_V100R002
// R001 means the sdk has both of the function : detect face and transfer face
//#define VERSION_SDK_V100R001
//R002 means the sdk just has the function of transfer face
//#define VERSION_SDK_V100R002
//R003 means the sdk just has the function of detect face
//#define VERSION_SDK_V100R003 

//***********************************************************

#ifdef VERSION_DEBUG_V100R001

//#define PRINTF_SEND_RECEIVE
#define DEBUG_LOG
//#define FUNCTION_TEST
#define SDK_VERSION_INFO "V100R001BD01"
// open uart for face detect module
#define __UART_TRANSPORT_
#endif

#ifdef VERSION_DEBUG_V100R002
//#define PRINTF_SEND_RECEIVE
#define HAISI_ENCODE_BY_USER
#define SDK_VERSION_INFO "V100R002BD01"
#define __UART_TRANSPORT_
#endif

#endif
