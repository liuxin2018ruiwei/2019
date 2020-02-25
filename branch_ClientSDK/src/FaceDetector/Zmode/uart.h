/*
 * Copyright(c) 2016 Reconova Information Technologies Co., Ltd. 
 * All rights reserved.
 *
 * 在原来的防疲劳中用的左工给的串口访问的基础上优化一下。
 * 考虑到UART操作可能要做一些额外的东西，所以我们把handle和fd分开，
 * 不过目前其实还是一个，handle还是int类型。后面如果有需要考虑改为
 * void *类型，分配一个context。
 *
 * Created by Cyberman Wu on Mar 29th, 2016.
 *
 */

#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * 直接用串口设备名称来打开。这个是最灵活的，因为不同平台上串口设备
 * 文件取名的规则不同。
 * 串口以阻塞式打开，后面访问接口中，read可以指定超时时间，而write
 * 则直接用阻塞式来写，用非阻塞式写在目前应用中比较麻烦而且没有什么
 * 帮助。
 */
int uart_open(const char *dev);
/*
 * 这个就我们目前的应用来说，主要是配置波特波，而且原来也不全，还有
 * 波控配置这里没有参数。
 */
int uart_config(int handle, int baudrate, int datasize, char parity, int stopbit);

/*
 * 关闭uart接口。
 */
int uart_close(int handle);


/*
 * 阻塞式UART读数据。
 *
 * 参数说明：
 *   handle         - 串口句柄。
 *   buf            - 存放读到的数据的缓冲区地址。
 *   count          - 缓冲区大小。
 *
 * 返回值：
 *   系统调用read()的返回值，出错时返回-errno。注意读数据成功返回
 *   值有可能小于count。
 *
 * 这个通常是用于外部用select()判断串口可读之后。不过从目前测试
 * 情况看，不同的串口select()会有差异，有的反应快，select()返回
 * 经常只能读到一个字节；而有的则可以读到多个字节。
 */
int uart_read(int handle, uint8_t *buf, int count);

/*
 * 从UART读取数据，最后一个参数是最大等待时间（us）。这个接口成功
 * 读到一次数据就返回，因为UART实现为流式的，所以返回不一定就是
 * count。
 *
 * 参数说明：
 *   handle         - 串口句柄。
 *   buf            - 存放读到的数据的缓冲区地址。
 *   count          - 缓冲区大小。
 *   timeout_us     - 最大等待时间，us。0表示不等待，-1表示一直等待。
 *
 * 返回值：
 *   > 0：读到的数据长度（小于或等于count）。
 *   = 0：等待数据超时。
 *   < 0：-errno。
 */
int uart_read_with_timeout(int handle, uint8_t *buf, int count, int timeout_us);
/*
 * 这个接口和uart_read_with_timeout()最大的区别是它试图读满count个
 * 字节的数据，而不是只要成功读取一次数据就返回。
 * 当然真正超时之后，还是会返回实际读取到的个数，这样可用于有些不
 * 确定有多少数据的情况下。
 *
 * 当用于select()后再使用，这个接口中有一个问题，会多一次判断是否
 * 有数据的操作。这个可以增加一个参数来优化，不过目前我们不想过于
 * 复杂就先做了。
 */
int uart_read_all(int handle, uint8_t *buf, int count, int timeout_us);
/*
 * 通过UART发送数据。目前发送只提供阻塞式发送。
 *
 * 参数说明：
 *   handle         - 串口句柄。
 *   buf            - 要发送数据的缓冲区地址。
 *   count          - 要发送数据的长度。
 *
 * 返回值：
 *   实际写入的长度，对于阻塞式发送一般是count，失败返回-errno。
 */
int uart_write(int handle, const uint8_t *buf, int count);


/*
 * 返回一个可用于select()调用的句柄。
 */
int uart_selectable(int handle);


#ifdef __cplusplus
}
#endif

#endif  /* __UART_H__ */
