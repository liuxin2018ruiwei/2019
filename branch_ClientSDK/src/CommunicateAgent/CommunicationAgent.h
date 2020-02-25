#ifndef COMMUNICATION_AGENT_H__
#define COMMUNICATION_AGENT_H__

#include "NonCopyable.h"
#include "Communication.h"
#include "MessageQueue.h"
#include "Requestion.h"

//com : 通信对象代理，负责消息的收发
class CommunicationAgent : public NonCopyable
{
public:
  static const unsigned int RECEIVER_MESSAGE_BUFF_SIZE = 128;
  static const unsigned int RECEIVE_MESSAGE_QUEUE_SIZE = 20; //每秒接收的个数
  typedef enum{ SendToOuter,SendToInner }SentToType;
	/*******************************
	创建实例,需要调用destroyInstance释放
	com 					: 通信类指针
	receiveQueueSize		: 接收的消息队列长度
	responseDataLength		: 发送消息缓冲，设置为需要发送的消息的最大数据长度
	成功返回实例指针，失败返回0
	*********************************/
	static CommunicationAgent* createInstance(Communication* com,
								unsigned int receiveQueueSize,
								unsigned int responseDataLength);
	static void destroyInstance(CommunicationAgent** agent);
	
	static CommunicationAgent * getInstance();
	//
	//接收消息，需要调用releaseMsg释放。
	//
	Message* receiveMsg(void);
	int setupUsbProtocolDynamic();

	//释放receiveMsg返回的消息
	void releaseMsg(Message* msg);

	void setCurrentMsgProtocol(ProtocolType proto){_currentMsgProtocol = proto;};
	ProtocolType getCurrentMsgProtocol(){ return _currentMsgProtocol;};

	int sendMsg(unsigned char cmdflag,short int cmdCode, short int ret,
				char* data, int dataLen, SentToType protocol=SendToOuter);

	// send requestion to requestion manager
	int sendReq( Requestion * pReq );

	//添加过滤器，被过滤器过滤掉的消息不接收
	void addFilter(MessageFilterItf* filter);

	void removeFilter(MessageFilterItf* filter);

	int init(unsigned int receiveQueueSize, unsigned int responseDataLength);
	void uninit(void);
	

protected:
	CommunicationAgent(Communication* com);
	
private:	
	
	
	//消息队列，用于接收通信模块的notify，并保存消息列表
	MessageQueue	_msgQueue;

	Communication* 	_com;//通信接口，负责向外部收发数据
	
	static CommunicationAgent * _instance;
	Message*		_sendCache;
	ProtocolType _currentMsgProtocol;
};

#endif
