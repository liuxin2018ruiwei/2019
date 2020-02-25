#ifndef STUBUART_PROTOCOL_H
#define STUBUART_PROTOCOL_H

#include "UartProtocol.h"
class StubUartProtocol 
{
public:
  typedef struct _strPkgBuff
  {
	unsigned char *Buffer;
	int BuffLen;
	int ReqLen;
	int Offset;
	long TimeOut;
  }PkgBuff;
  
  typedef std::list< PkgBuff > PkgList;
  typedef PkgList::iterator PkgItor;
  
  typedef enum
  {
	// ||<--ReqLen = 0
	// ||<--OffSet = 0
	PkgBuffIdle,
	// ||<--ReqLen = 0
	// |Header|<--Offset = sizeof(Header)
	PkgBuffWaitSize,
	// |Header|Size|Body|<--ReqLen = Size
	// |Header|Size|<--Offset = sizeof(Header/Size) ~ sizeof(Header/Size/Body)-1
	PkgBuffAppend,
	// |Header|Size|Body|<--ReqLen = Size
	// |Header|Size|Body|<==Offset = Size
	PkgBuffComplete,
	// |Header|X|<--ReqLen = x
	// |Header|X|<--Offset = x
	// wait timeout
	PkgBuffTimeOut,
	UnknownState
  }PkgState;
  
  enum{ MaxPkgBufferLen = 150000 };
  enum{ SizePkgLen = 6};
  enum{ WaitTimeOut = 1};//1000000};//500000 }; //ms
  
  // return the pointer of the class, it's forbiden to new class directly,must call getInstence()
	static StubUartProtocol* getInstance();
#if 0
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
#endif
	virtual ~StubUartProtocol();

	virtual int selectRead( unsigned char * pData, int nLen );
	// the pointer itself
	static StubUartProtocol* m_Inst;
	int initPort();
 protected:
	int fillDummyData_default( unsigned char* pData, int nLen, unsigned int &seq );
	
#if 0
	int addDataToIdleBuff( PkgItor &it, unsigned char *pBuffer, int nDataLen );
	int addDataToWaitSizeBuff( PkgItor &it, unsigned char *pBuffer, int nDataLen);
	int addDataToAppendBuff( PkgItor &it, unsigned char *pBuffer, int nDataLen );
	int addDataToTimeOutBuff( PkgItor &it, unsigned char *pBuffer, int nDataLen );

	int getAvaildPkgBuff( PkgItor &it, PkgItor diffItr );

	PkgState getPkgState( PkgItor &it );
	int receiveData( PkgItor &it, int nLen );

	int initBuff( int buffCnt, int buffLen );

	int uninitBuff();
#endif
	StubUartProtocol();

#if 0
	void close_port(int fd);
	speed_t convert_baudrate(int baudrate);
	int open_port(int portno);
	int set_baudrate(struct termios *tio, int ibaudrate);
	int set_datawidth(struct termios *tio, int idatawidth);
	int set_parity(struct termios *tio, char cparity);
	int set_port(int fd, int ibaudrate, int idatawidth, char cparity, int istopbit);
	int set_stopbit(struct termios *tio, int istopbit);


	unsigned char *getBuff();
#endif
 private:

	unsigned int m_seq;
};

#endif
