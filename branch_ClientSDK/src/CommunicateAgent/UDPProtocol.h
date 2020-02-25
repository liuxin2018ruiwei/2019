#ifndef UDP_PROTOCOL_H
#define UDP_PROTOCOL_H

#include <netinet/in.h>
#include <sys/socket.h>
#include "Communication.h"
#include "Observer.h"
#include "ObjectOfObserver.h"
#include "Version.h"

#ifdef SAVE_IMAGES
#define OFFSET 7000 // ��Щ�汾���˿���ռ�ã�ʹ��OFFSET ������
#else
#define OFFSET 0
#endif

#define UDP_SERVER_PORT 15658 + OFFSET
#define UDP_SELECT_TIME 200000
#define UDP_READ_BUFFER_SIZE 10240
#define UDP_CLIENT_PORT 15656 + OFFSET

class UDPProtocol : public Protocol,public ObjectOfObserver
{
public:
	// return the UDPProtocol of the class
	static UDPProtocol* getInstance();
	// clean up the buffer hold by m_fd
	// return  (out) the result of the last read
	virtual int cleanupFD();
	// init the protocol obj before start to use it
	virtual int init();
	// return (out) the length have received data 
	// receive data from uart port, and it will notify the object about the data
	virtual int receiveData();
	virtual int receiveData( unsigned char* pBuffer, int nLen );
	// pData (in) the pointer of the data to send
	// nLen (in) the length of the data
	// return (out) the data have send
	virtual int sendData( unsigned char* pData, int nLen);
	// return (out) the length have send data
	//virtual int sendData();
	// before closing the work, terminate must be call to free some resource
	virtual int terminate();
	virtual ~UDPProtocol();
	// pObj (in) after received data ,call pObj->notify() to use the data
	//static int receiverUDPData(ObjectOfObserver* pObj);
protected:
	// pBuffer (out) the buffer to save the data read
	// nBuffLen (in) the length want to read
	// nReadLen (out) the length actually read
	// return (out)RET_NG time out or error happened
	//				RET_OK read ok
	virtual int readFD(unsigned char* pBuffer,\
					   int nBufferLen, \
					   int& nReadLen);
	// pBuffer (out) the buffer to save the data read
	// nBuffLen (in) the length want to read
	// nReadLen (out) the length actually read
	// return (out)RET_NG time out or error happened
	//				RET_OK read ok
	virtual int read_socket(unsigned char* pBuffer,\
					   int nBufferLen, \
					   int& nReadLen,\
					   unsigned int flag);
private:
	void close_port(int fd);
	// return (out) the fd have created; 
	//when failed , return <= 0
	// when success, return > 0
	int createSocketPort();
	UDPProtocol();

	struct sockaddr_in m_CAddr;

	// the pointer itself
	static UDPProtocol* m_Inst;
	// the pointer of buffer
	unsigned char* m_pBuffer;

};


#endif // !UDP_COMMUNICATION_H

