#ifndef __UTIL__H__
#define __UTIL__H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __USE_LOGCAT__
#include <android/log.h>
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO ,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#else
#define LOGI(...) {fprintf(stdout, __VA_ARGS__); fprintf(stdout, "\n");}
#define LOGE(...) {fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");}
#define LOGD(...) {fprintf(stdout, __VA_ARGS__); fprintf(stdout, "\n");}
#endif

// 这个是专门用于调试的打印，发布版本中要关掉。DEBUG()我们不自动输出回车，
// 更灵活一些；但Android log有可能是自动输出回车的，这个目前还不好控制。
// DEBUG()是否用logcat独立出来，以方便在开发过程中调试。
#ifdef __ENABLE_DEBUG_INFO
#  ifdef __DEBUG_USE_LOGCAT
#    warning "Debug using __android_log_print() ..."
#    define DEBUG(...) __android_log_print(ANDROID_LOG_INFO , LOG_TAG, __VA_ARGS__)
#  else
#    define DEBUG(...) {fprintf(stdout, __VA_ARGS__);}
#  endif
#else
#  define DEBUG(...)
#endif

int GetProfileString(const char *profile, const char *AppName, const char *KeyName, char *KeyVal);

int Abs(int n);

#ifdef __cplusplus
}
#endif

#endif // __UTIL__H__


