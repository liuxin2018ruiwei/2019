#include "mysem.h"

// 创建信号量和获取信号量公用函数
static int comm_sem ( int nums , int semflag)
{
    // 获取key
    key_t key = ftok(PATHNAME, PROJ_ID);
    if(key < 0)
    {
        perror("ftok");
        return -1;
    }

    int semid = semget(key,nums, semflag );
    if( semid < 0)
    {
        perror("semget");
        return -1;
    }
    return semid;
}


int create_sems(int nums)  // 创建含有nums个信号量的集合
{
    return comm_sem(nums, IPC_CREAT|IPC_EXCL|0666);
}



int get_sems()     // 获取信号量
{
    return comm_sem(0, IPC_CREAT);
}

union semun
{
    int val; // value for SETVAL
    struct semid_ds *buf; // buffer for IPC_STAT & IPC_SET
    unsigned short *array; // buffer for GETALL & SELALL
    struct seminfo * __buf; // buffer for IPC_INFO
};

// 初始化semid对应的信号量集中编号为which的信号量值为value
int init_sems(int semid , int which, int value)
{
    union semun _semun;
    _semun.val = value;//value仅能取值0或1
    int ret = semctl(semid, which, SETVAL,_semun);
    if(ret < 0)
    {
        perror("inin_sem");
        return -1;
    }
    return 0;
}

int destroy_sems(int semid) // 释放该信号量集
{
    int ret = semctl(semid, 0, IPC_RMID, NULL);
    if(ret < 0)
    {
        perror("rm_sem");
        return -1;
    }
    return 0;
}

static int comm_sem_op(int semid, int which, int op)
{
    struct sembuf _sembuf;
    _sembuf.sem_num = which;
    _sembuf.sem_op = op;
    _sembuf.sem_flg = 0; //  IPC_NOWAIT  SEM_UNDO
    return semop(semid, &_sembuf, 1);
}


int P(int semid, int which)    // 表示通过 信号量值-1
{
    return comm_sem_op(semid, which , -1);
}
int V(int semid, int which)    // 表示释放 信号量值+1
{
    return comm_sem_op(semid, which, 1);
}

