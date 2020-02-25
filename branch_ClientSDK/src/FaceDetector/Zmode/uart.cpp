/*************************************************************************
	> File Name: uart.c
	> Author: 
	> Mail: 
	> Created Time: 2015年09月11日 星期五 14时10分51秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <strings.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/time.h>

#include "uart.h"
#include "cfg.h"


static inline 
uint64_t timestamp(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}


/*
 *   1：有数据可读。
 *   0：超时。
 * < 0：出错。
 *
 * timeout_us为输入输出参数，输入要等待的超时时间，
 * 返回还剩余的超时时间。
 */
static 
int wait_for_data(int fd, int *p_timeout_us)
{
    uint64_t us_begin = timestamp();

    int timeout_us = *p_timeout_us;
    struct timeval tv;
    struct timeval *p_tv = NULL;
    if (timeout_us >= 0)
    {
        tv.tv_sec = timeout_us / 1000000;
        tv.tv_usec = timeout_us % 1000000;
        p_tv = &tv;
    }

    int ret;
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    do
    {
        ret = select(fd + 1, &rfds, NULL, NULL, p_tv);
    } while ((ret == -1) && (errno == EINTR));
    if (ret < 0)
    {
        // 这里不直接返回；但放到后面的话，怕gettimeofday()等调用万一影响
        // errno的值（理论上成功的话不应该有影响）。
        ret = -errno;
    }

    int us_elapsed = (int)(timestamp() - us_begin);
    timeout_us -= us_elapsed;
    *p_timeout_us = timeout_us > 0 ? timeout_us : 0;

    return ret;
}


int uart_selectable(int handle)
{
    int fd = handle;
    return fd;
}

int uart_open(const char *dev)
{
    // 如果用非阻塞式ZMODEM的lsz/lrz会出问题。另外非阻塞式在其它方面
    // 也会比较麻烦。
    // O_NOCTTY是从左工原来的代码中沿用过来的，目前先不去了解具体是
    // 做什么的。
    int fd = open(dev, O_RDWR | O_NOCTTY/* | O_NDELAY*/);
    if (fd < 0)
    {
        return -errno;
    }

    return fd;
}


int uart_close(int handle)
{
    int fd = handle;
    return close(fd);
}


int uart_read(int handle, uint8_t *buf, int count)
{
    int fd = handle;
    int ret = read(fd, buf, count);
    if (ret < 0)
    {
        return -errno;
    }

    return ret;
}


static 
int uart_read_once(int fd, uint8_t *buf, int count, int *p_timeout_us)
{
    int ret = wait_for_data(fd, p_timeout_us);
    if (ret <= 0)
    {
        return ret;
    }

    ret = read(fd, buf, count);
    if (ret < 0)
    {
        return -errno;
    }

    return ret;
}


int uart_read_with_timeout(int handle, uint8_t *buf, int count, int timeout_us)
{
    int fd = handle;
    return uart_read_once(fd, buf, count, &timeout_us);
}


/*
 * 当使用select时，这个实现不是最优的，因为多调用了一次select()。
 */
int uart_read_all(int handle, uint8_t *buf, int count, int timeout_us)
{
    int fd = handle;
    int remain = count;
    do
    {
        int pos = count - remain;
        int rlen = uart_read_once(fd, &buf[pos], remain, &timeout_us);
        // 读数据遇到错误。这种情况下可能也读取了一部分有效
        // 数据，不过就不返回了。
        if (rlen < 0)
        {
            return rlen;
        }

        // 这个接口也可以用于不确定有多少数据的情况下，可以给出一个比较
        // 大一些的超时时间，返回实际读到的个数。
        if (rlen == 0)
        {
            break;
        }

        remain -= rlen;
    }while (remain);

    return count - remain;
}

int uart_write(int handle, const uint8_t *buf, int count)
{
    int fd = handle;
    int ret = write(fd, buf, count);
    if (ret < 0)
    {
        return -errno;
    }

    return ret;
}

static speed_t convert_baudrate(int baudrate)
{
    switch(baudrate) {
        case 0: return B0;
        case 50: return B50;
        case 75: return B75;
        case 110: return B110;
        case 134: return B134;
        case 150: return B150;
        case 200: return B200;
        case 300: return B300;
        case 600: return B600;
        case 1200: return B1200;
        case 1800: return B1800;
        case 2400: return B2400;
        case 4800: return B4800;
        case 9600: return B9600;
        case 19200: return B19200;
        case 38400: return B38400;
        case 57600: return B57600;
        case 115200: return B115200;
        case 230400: return B230400;
        case 460800: return B460800;
        case 500000: return B500000;
        case 576000: return B576000;
        case 921600: return B921600;
        case 1000000: return B1000000;
        case 1152000: return B1152000;
        case 1500000: return B1500000;
        case 2000000: return B2000000;
        case 2500000: return B2500000;
        case 3000000: return B3000000;
        // 在Cygwin中头文件中没有下面两个定义，编译会失败。
#if !defined(__CYGWIN__)
        case 3500000: return B3500000;
        case 4000000: return B4000000;
#endif
        default:  return -1;
    }
}


static 
int set_baudrate(struct termios *tio, int ibaudrate)
{
    speed_t baudrate;
    
    baudrate = convert_baudrate(ibaudrate);
    if(baudrate < 0)
    {
        return -1;
    }
    cfsetispeed(tio, baudrate);
    cfsetospeed(tio, baudrate);
    return 0;
}

static 
int set_datawidth(struct termios *tio, int idatawidth)
{
    switch(idatawidth)
    {
        case 7: 
            tio->c_cflag |= CS7;
            break;
        case 8:
            tio->c_cflag |= CS8;
            break;
        default:
            return -1;
    }
    
    return 0;
}

static 
int set_parity(struct termios *tio, char cparity)
{
    switch(cparity)
    {
        case 'N':
            tio->c_cflag &= (~PARENB);
            break;
        case 'O':
            tio->c_cflag |= PARENB;
            tio->c_cflag |= PARODD;
            tio->c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E':
            tio->c_cflag |= PARENB;
            tio->c_cflag &= (~PARODD);
            tio->c_iflag |= (INPCK | ISTRIP);
            break;
        default:
            return -1;
    }

    return 0;
}

static int set_stopbit(struct termios *tio, int istopbit)
{
    switch(istopbit)
    {
        case 1:
            tio->c_cflag &= (~CSTOPB);
            break;
        case 2:
            tio->c_cflag |= CSTOPB;
            break;
        default:
            return -1;
    }

    return 0;
}

int uart_config(int handle, int baudrate, int datawidth, char parity, int stopbit)
{
    struct termios oldtio, newtio;
    int ret;
    int fd = handle;

    ret = tcgetattr(fd, &oldtio);
    if (ret < 0)
    {
        return -1;
    }

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = (CLOCAL | CREAD);

    if (set_baudrate(&newtio, baudrate) < 0)
    {
        return -1;
    }

    if (set_datawidth(&newtio, datawidth) < 0)
    {
        return -1;
    }
    
    if (set_parity(&newtio, parity) < 0)
    {
        return -1;
    }

    if (set_stopbit(&newtio, stopbit) < 0)
    {
        return -1;
    }

    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd, TCIFLUSH);

    ret = tcsetattr(fd, TCSANOW, &newtio);
    if(ret)
    {
        return -1;
    }

    return 0;
}


