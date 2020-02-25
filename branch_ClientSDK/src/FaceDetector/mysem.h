#ifndef _MYSEM_H_
#define _MYSEM_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>  // ftok
#include <sys/sem.h>
#include <sys//wait.h>

#define PATHNAME "."  // ftok 中生成key值 . 表示当前路径
#define PROJ_ID  56  // ftok 中配合PATHNAME 生成唯一key值

/*
union semun
{
    int val; // value for SETVAL
    struct semid_ds *buf; // buffer for IPC_STAT & IPC_SET
    unsigned short *array; // buffer for GETALL & SELALL
    struct seminfo * __buf; // buffer for IPC_INFO
};
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>  // ftok
#include <sys/sem.h>
#include <sys//wait.h>

#define PATHNAME "."  // ftok 中生成key值 . 表示当前路径
#define PROJ_ID  56  // ftok 中配合PATHNAME 生成唯一key值

int create_sems(int nums);  // 创建含有nums个信号量的集合

int get_sems();     // 获取信号量




// 初始化semid对应的信号量集中编号为which的信号量值为value
int init_sems(int semid , int which, int value);

int destroy_sems(int semid); // 释放该信号量集


int P(int semid, int which);    // 表示分配 信号量值-1
int V(int semid, int which);    // 表示释放 信号量值+1



#endif /* _MYSEM_H_ */
