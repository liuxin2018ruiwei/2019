#ifndef UART_PROTOCOL_H
#define UART_PROTOCOL_H

#include <termios.h>
#include"Observer.h"
#include"ObjectOfObserver.h"
#include"Protocol.h"
#include "DetectFaceIFCommon.h"

#define UART_FD_INIT -1
//#define RECEIVE_BUFF_SIZE 10240
#define RECEIVE_BUFF_SIZE 10300
//#define READ_HEADER_SIZE 14
#define UART_SELECT_TIME 200000

#define RET_OK 0
#define RET_NG -1

typedef unsigned short int UINT16;
#define MAX_PATH_NUM (50)
#define MAX_RECEIVE_UART_DATA_LEN (3072) //（8 + 4 + 4 + 1 + 17 *37）* 4 = 686 * 4 = 2584  留一部分数据预留设置成3k

class AppContexts;
class UartProtocol : public Protocol, public ObjectOfObserver
{
public:
  typedef struct _strPkgBuff
  {
	unsigned char *Buffer; //存的buff
	int BuffLen;//目前发过来的buff长度 ，固定的空间的长度
	int ReqLen;//消息协议里面的长度，需要填充的长度
	int Offset;//目前已经填充的长度
	long TimeOut;//刷新的时间
  }PkgBuff;
  
  typedef std::list< PkgBuff > PkgList;
  typedef PkgList::iterator PkgItor;
  
  typedef enum
  {
	// ||<--ReqLen = 0
	// ||<--OffSet = 0
	PkgBuffIdle, // 空白的
	// ||<--ReqLen = 0
	// |Header|<--Offset = sizeof(Header)
	PkgBuffWaitSize,//等待长度的
	// |Header|Size|Body|<--ReqLen = Size
	// |Header|Size|<--Offset = sizeof(Header/Size) ~ sizeof(Header/Size/Body)-1
	PkgBuffAppend,//需要添加的
	// |Header|Size|Body|<--ReqLen = Size
	// |Header|Size|Body|<==Offset = Size
	PkgBuffComplete,//完成的状态
	// |Header|X|<--ReqLen = x
	// |Header|X|<--Offset = x
	// wait timeout
	PkgBuffTimeOut,//超时状态
	UnknownState
  }PkgState;
  
  enum{ MaxPkgBufferLen = 150000 };
  enum{ SizePkgLen = 6};//5};
  enum{ WaitTimeOut = 1};//1000000};//500000 }; //ms
  
  // return the pointer of the class, it's forbiden to new class directly,must call getInstence()
	static UartProtocol* getInstance();
	// init the communication obj before start to use it
	// priData	(in) the pointer  of appcontext
	virtual int init(void* priData);
	// return (out) the length have received data 
	// receive data from uart port, and it will notify the object about the data
	virtual int receiveData();
	// return (out) the length have send data
	//virtual int sendData();
	// before closing the work, terminate must be call to free some resource
	virtual int terminate();
	int setBaudrate(int setBaudrateData);

	virtual ~UartProtocol();

	int reInitPort(SerialPortInfo* pPortInfoPtr);

	// the pointer itself
	static UartProtocol* m_Inst;
 protected:
	int addDataToIdleBuff( PkgItor &it, unsigned char *pBuffer, int nDataLen );
	int addDataToWaitSizeBuff( PkgItor &it, unsigned char *pBuffer, int nDataLen);
	int addDataToAppendBuff( PkgItor &it, unsigned char *pBuffer, int nDataLen );
	int addDataToTimeOutBuff( PkgItor &it, unsigned char *pBuffer, int nDataLen );

	int getAvaildPkgBuff( PkgItor &it, PkgItor diffItr );

	PkgState getPkgState( PkgItor &it );
	int receiveData( PkgItor &it, int nLen );

	int initBuff( int buffCnt, int buffLen );

	int uninitBuff();
	
	UartProtocol();

	void close_port(int fd);
	speed_t convert_baudrate(int baudrate);
	//int open_port(int portno);

	int open_port(char *devName, UINT16 devNameLen);
	int set_baudrate(struct termios *tio, int ibaudrate);
	int set_datawidth(struct termios *tio, int idatawidth);
	int set_parity(struct termios *tio, char cparity);
	int set_port(int fd, int ibaudrate, int idatawidth, char cparity, int istopbit);
	int set_stopbit(struct termios *tio, int istopbit);


	int initPort(SerialPortInfo* pPortInfoPtr);

	unsigned char *getBuff();
private:

#ifndef SETUP_MODEL_OPEN
	// the pointer of  app context, saved some globl info
	AppContexts* m_pAppContxt;
#endif
	// keep the pointer of the buffer
	unsigned char* m_pBuffer;

	PkgList m_pkgList;

};

#endif
