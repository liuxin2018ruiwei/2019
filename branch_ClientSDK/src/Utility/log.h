#ifndef __UTIL__H__
#define __UTIL__H__

#include<stdio.h>
#include "file_util.h"

#ifdef __cplusplus
extern "C" {
#endif


#define ESC_START     "\033["
#define ESC_END       "\033[0m"
#define COLOR_FATAL   "31;40;5m"
#define COLOR_ALERT   "31;40;1m"
#define COLOR_CRIT    "31;40;1m"
#define COLOR_ERROR   "31;40;1m"
#define COLOR_WARN    "33;40;1m"
#define COLOR_NOTICE  "34;40;1m"
#define COLOR_INFO    "32;40;1m"
#define COLOR_DEBUG   "36;40;1m"
#define COLOR_TRACE   "37;40;1m" 
#define LOGD(format, args...) (printf( ESC_START COLOR_INFO "[Time :%lld ms]-[%s]-[%d]:"      	format ESC_END, file_util::getMilliSecond(), __FUNCTION__ , __LINE__, ##args)) 
#define LX_DEBUG(format, args...) (printf( ESC_START COLOR_DEBUG "[Time :%lld ms]-[%s]-[%d]:" 	format ESC_END, file_util::getMilliSecond(), __FUNCTION__ , __LINE__, ##args)) 
#define LX_WARN(format, args...) (printf( ESC_START COLOR_WARN "[Time :%lld ms]-[%s]-[%d]:"   	format ESC_END, file_util::getMilliSecond(), __FUNCTION__ , __LINE__, ##args)) 
#define LX_ERR(format, args...) (printf( ESC_START COLOR_ERROR "[Time :%lld ms]-[%s]-[%d]:"   	format ESC_END, file_util::getMilliSecond(), __FUNCTION__ , __LINE__, ##args)) 



#ifdef __cplusplus
}
#endif

#endif // __UTIL__H__


