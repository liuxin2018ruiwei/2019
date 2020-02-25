/********************************************************************
* Name      :  ZmodeUpgrade.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                     
* Author    :  liuxin                                        
* Date      :  2018-06-25                                                 
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <strings.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "ZmodeUpgrade.h"
#include "UartProtocol.h"
#include "uart.h"
#include "cfg.h"
#include "Communication.h"


static int term_need_reset = 0;
static struct termios orig_termios;
static int sg_baudrate = 115200;
static int c_tty_max_baudrate = 921600;


ZmodeUpgrade* ZmodeUpgrade::m_this =  nullptr;

ZmodeUpgrade* ZmodeUpgrade::getInstance()
{
	if( nullptr == m_this )
	{
		m_this = new ZmodeUpgrade;
	}

	return m_this;
}




int ZmodeUpgrade::initZmodeUpgrade(void *priData )
{
	int s32Ret;
#ifndef X86COMPILE
	return s32Ret;
#endif

}

int ZmodeUpgrade::uninitZmodeUpgrade( )
{
	int ret32 = 0;
#ifndef X86COMPILE
	
#endif
	return ret32;

}


void reset_terminal_mode()
{
    if (!term_need_reset)
    {
        return;
    }
    tcsetattr(0, TCSANOW, &orig_termios);
    term_need_reset = 0;
}

void ZmodeUpgrade::set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
	//tcgetattr函数用于获取与终端相关的参数。
	//参数fd为终端的文件描述符，返回的结果保存在termios 结构体中
	//
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    term_need_reset = 1;
    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);//注册终止函数(即main执行结束后调用的函数),在atexit(f)调用之前被静态分配的对象的析构函数将在f()的调用之后被调用
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}


int ZmodeUpgrade::test_UART(int uart_handle)
{
#define MAX_BUF (64)
    char buf[MAX_BUF];
    int cmd_len = snprintf(buf, sizeof(buf), "\r");
	for(int i = 0; i<MAX_BUF; i++ )
	{
		buf[i] = 0xAA;
	}	
    int wlen = uart_write(uart_handle, (uint8_t *)buf, MAX_BUF);
    if (wlen != MAX_BUF)
    {
        ERR("uart_write() return %d!\n\r", wlen);
        return -1;
    }

	// 错误了，过不了，返回是0
    int timeout_us = 1000000;
    int rlen = uart_read_with_timeout(uart_handle, (uint8_t *)buf, sizeof(buf), timeout_us);
    if (rlen <= 8) 
    {
        ERR("uart_read_with_timeout() failed with %d!\n\r", rlen);
        return -2;
    }

    return 0;
}

//uart_hand 传出参数
int ZmodeUpgrade::initUart(const char *uart_dev, int * return_handle)
{
	int err = 0;
	int uart_handle = uart_open(uart_dev);
	if (uart_handle < 0)
	{
		ERR("Failed to open UART device '%s' with '%s'!\n", 
			uart_dev, strerror(-uart_handle));
		return 102;
	}
	
	*return_handle = uart_handle;
	//printf("*return_handle = %d, uart_handle = %d\n",*return_handle, uart_handle);
	
	err = uart_config(uart_handle, sg_baudrate, 8, 'N', 1);
	if (err)
	{
		ERR("Failed to setting up serial port.\n");
		return 103;
	}

	set_conio_terminal_mode();
	err = test_UART(uart_handle);
	if (err)
	{
		INFO("Connecting Zynq console failed!\n\r");
		return 104;
	}

	//printf("init UART ok!");
	return 0;
}

 
int ZmodeUpgrade::send_file(int uart_handle, char *fname)
{
    int uart_fd = uart_selectable(uart_handle);
	//sleep(1);
	usleep(100 * 1000);

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork()");
        fprintf(stderr, "Failed to fork new process for file sending!\n");
        return -1;
    }

    int status = 0;
    if (pid == 0)//子进程
    {
        //printf("File for sending: %s\n", fname);
        char target_cmd[] = "/home/lsz -vv -b\r";
        write(uart_fd, target_cmd, sizeof(target_cmd) - 1);
		//将uart_fd重定向到标准输入输出，这样升级的文件数据导入到uart_fd对应的文件里面
       
        dup2(uart_fd, 0);
        dup2(uart_fd, 1);

		//ok
        char *argv[5];
        argv[0] = (char *)"/home/lsz";
        argv[1] = (char *)"-vv";
        argv[2] = (char *)"-b";
        argv[3] = fname;
        argv[4] = NULL;


        execvp(argv[0], argv);
        perror("execvp()");

        status = -2;
		exit(0);
    }
    else //父进程回收
    {
    
    	//printf("father wait....\n");
        waitpid(pid, &status, 0);
        //status = WEXITSTATUS(status);
		//printf("wait ok! status = %d \n",status);
		//WEXITSTATUS(status) 当WIFEXITED返回非零值时，
		//我们可以用这个宏来提取子进程的返回值，如果子进程调用exit(5)退出，
		//WEXITSTATUS(status)就会返回5；如果子进程调用exit(7)，WEXITSTATUS(status)就会返回7。
		//请注意，如果进程不是正常退出的，也就是说，WIFEXITED返回0，这个值就毫无意义。

		
		//Communication
		Communication::getInstances()->start();
    }

    return status;
}
/*
int ZmodeUpgrade::send_file_ZMODEM(int uart_fd, const char *fname)
{
    //sleep(1);
    usleep(100 * 1000);
	printf("send test start!");
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork()");
        ERR("Failed to fork new process for file sending!\n");
        return -1;
    }
    

    int status;
    if (pid == 0)
    {
        // 用-S同步时间测试没有成功。
        INFO("File for sending: %s\n", fname);

        char *argv[6];
        dup2(uart_fd, 0);
        dup2(uart_fd, 1);
        argv[0] = (char *)"/home/lsz";
        argv[1] = (char *)"-vv";
        argv[2] = (char *)"-b";
        argv[3] = (char *)fname;
        argv[4] = NULL;
        execvp(argv[0], argv);
        perror("execvp()");
        status = -2;
    }
    else
    {
       waitpid(pid, &status, 0);
    }
//	printf("send test start ok !");

    return status;
}
*/


const char * ZmodeUpgrade::fname_skip_path(const char *fname)
{
    const char *pf = fname;
    const char *p = fname;
    for ( ; *p != '\0'; ++p)
    {
        if (*p == '/')
        {
            pf = p + 1;
        }
    }

    return pf;
}

int ZmodeUpgrade::start_install_QSPI(int uart_handle, char *fname)
{
    int uart_fd = uart_selectable(uart_handle);

    int cmd_len;
    char cmd_buf[128];
    cmd_len = snprintf(cmd_buf, sizeof(cmd_buf), 
                       "time flashcp -v %s /dev/mtd9\r", fname_skip_path(fname));
    write(uart_fd, cmd_buf, cmd_len);

    return 0;
}

int term_getch()
{
    unsigned char c;
    int r = read(0, &c, sizeof(c));
    return (r < 0) ? r : c;
}


int ZmodeUpgrade::startZmod( char *pathAndFileName )
{
	if (NULL == pathAndFileName)
	{
		printf("ERR : pathAndFileName = NULL\n");
		return -1;
	}
	int ret32 = 0;
	int err = 0;


	//用我们的串口初始化的数据
	int uart_handle = UartProtocol::getInstance()->getFD();
	//printf("uart_handle = %d \n ",uart_handle);

	
	char *fileName;
	fileName = pathAndFileName;


	//printf("send_file get-->uart_handle = %d, fileName =%s \n ",uart_handle,fileName);

	
	if (true == Communication::getInstances()->isRunning())
	{
		//printf("Communication --> stop\n");
		Communication::getInstances()->stop();
	}
	
	err = send_file(uart_handle, fileName);//发送文件通过串口升级
	//err =send_file_ZMODEM(uart_handle, fileName);
	if (err)
	{
	    ERR("Sending '%s' failed with %d!\n", fileName, err);
	    return 105;
	}
	
    return 0;
}





ZmodeUpgrade::~ZmodeUpgrade()
{
}


ZmodeUpgrade::ZmodeUpgrade()
{
}

