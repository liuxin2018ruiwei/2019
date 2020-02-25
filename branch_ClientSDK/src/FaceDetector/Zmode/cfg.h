/*
 * Created by Cyberman Wu on Jan 19th, 2016.
 *
 */

#ifndef __CFG_H__
#define __CFG_H__

#if defined(__GNUC__)

#define ERR(fmt, args...) \
    fprintf(stderr, fmt, ##args)
#define INFO(fmt, args...) \
    printf(fmt, ##args)

#ifdef ENABLE_DEBUG_INFO
#define DEBUG(fmt, args...) \
    printf(fmt, ##args)
#else
#define DEBUG(fmt, args...) 
#endif  /* ENABLE_DEBUG_INFO */

#else

#define ERR         printf
#define INFO        printf

#ifdef ENABLE_DEBUG_INFO
#define DEBUG       printf
#else
#define DEBUG 
#endif  /* ENABLE_DEBUG_INFO */

#endif

#endif  /* __CFG_H__ */

