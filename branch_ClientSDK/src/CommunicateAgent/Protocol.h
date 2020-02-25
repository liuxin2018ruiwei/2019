#ifndef PROTOCOL_H
#define PROTOCOL_H

#include<string>
#include<list>
#include<pthread.h>
#include<semaphore.h>

#define RET_OK 0
#define RET_NG -1
#define FD_INIT -1
#define SEND_MESSAGE_BUFF_MAX 3
#define SELECT_WAIT_TIME 20000
//#define SELECT_WAIT_TIME 100000
#define READ_HEADER_SIZE  8//12
// the position of the number which is the total length
#define READ_HEADER_LENGTH_INDEX 4
// the size of the number which is the total length 
#define READ_HEADER_LENGTH_SIZE 2
#define READ_HEADER_TYPE_INDEX 9
#define READ_HEADER_TYPE_MASK 0x10
#define READ_HEADER_CODE_INDEX 10
#define READ_HEADER_CODE_SIZE 2
#define READ_HEADER_REPLY_CODE_INDEX 12
#define READ_HEADER_REPLY_CODE_SIZE 2
#define REPLY_HEADER_TYPE_MASK 0x12
//#define SEND_MESSAGE_BUFF_MAX 3
// the time wait for the fd get avaialbe
#define RECEIVE_DATA_WAIT_TIME 10 * 1000
#define READ_DATA_CHECK_OK 0
#define READ_DATA_CHECK_HEADER_NG -1
#define READ_DATA_CHECK_SUM_NG -2
#define READ_DATA_CHECK_OTHER -3
typedef std::list< std::string > MsgList;
typedef MsgList::iterator MsgItor;

typedef enum enProtocol
{
	UART_Protocol,
	SDK_Protocol,
	Http_Protocol,
	UDP_Protocol,
	USB_Protocol,
	RDT_RS232_Protocol,
	BROAD_Protocol,
	ProtocolMax
}ProtocolType;

typedef struct _protocol_msg{
	unsigned char * msg_data;
	ProtocolType msg_protocol;
}ProtocolMsg;

//这个0xFF是判断需要判断的位
const unsigned char PROTOCOL_HEADER_MASK[READ_HEADER_SIZE] = \
{0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//这个0xFB是运算后结果要是0xFB
const unsigned char PROTOCOL_HEADER_FORMATE[READ_HEADER_SIZE] = \
{0xFB, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

class SendMessageQueue;

class Protocol
{
public:
	Protocol();
	// check the data if the data is corrent
	// pData	(in) the data want to check
	// nLen		(in) the length of the data
	// return	(out) 
	//			READ_DATA_CHECK_OK 
	//			READ_DATA_cHECK_HEADER_NG -1
	//			READ_DATA_CHECK_SUM_NG -2
	int Check(unsigned char* pData, int nLen);
	// check the SUM to make sure the data is correct
	// return(out) RET_OK / RET_NG
	//int CheckSum(unsigned char* pData, int nLen);
	// return (out) the fd of the file
	virtual int getFD(); 
	// sData(out) get the data to send which will save in sData
	//virtual int getDataToSend( std::string& sData);
	virtual int init( void * priData );
	// init the protocol obj before start to use it
	virtual int init();
	// return (out) the length have received data 
	virtual int receiveData()=0;
	// pBuffer (out) the pointer of the buffer, if received data ,save into it
	// nLen (in) the length of the buffer
	// return (out) the length of the data be saved
	virtual int receiveData( unsigned char* pBuffer, int nLen );
	// clean up the buffer hold by m_fd
	// return  (out) the result of the last read
	virtual int cleanupFD();
	// return (out) the length have send data
	//virtual int sendData();
	// pData (in) the pointer of the data to send
	// nLen (in) the length of the data
	// return (out) the data have send
	virtual int sendData( unsigned char* pData, int nLen);
	// fd (in) the fd of the file
	virtual int setFD(int fd);
	// pData (in) the pointer of the data  to send ;
	// nDataLen (in) the length of the data
	virtual int saveData(unsigned char* pData, int nDataLen);

	virtual int setSendQueue(SendMessageQueue * queue);
	// before closing the work, terminate must be call to free some resource
	virtual int terminate()=0;

	virtual ~Protocol();

protected:
	// pBuffer (out) the buffer to save the data read
	// nBuffLen (in) the length want to read
	// nReadLen (out) the length actually read
	// return (out)RET_NG time out or error happened
	//				RET_OK read ok
	virtual int readFD(unsigned char* pBuffer, int nBufferLen, int& nReadLen);
	// set the check sum into the data ,and save at the end of it
	// pData	(in/out) the source data to get check sum and the destination to set checksum
	// nLen		(in) the length of the data
	int setCheckSum(unsigned char* pData, int nLen);
	// reply the message to notify it with error code
	// ErrorCode	(in) the code to reply the message
	// pData		(in) the pointer of the data which have some mistake, and have to reply
	// nLen			(in) the length of the data
	int replyError(short ErrorCode, unsigned char* pData, int nLen);
	SendMessageQueue *m_sendQueue;
		
private:
	int m_fd;
#if 0
	MsgList m_Msgs;
	pthread_mutex_t m_SendMutex;
	sem_t m_MsgAvailable;
	sem_t m_ListAvailable;
#endif
	//unsigned char* m_pReply;

};

#endif
