/********************************************************************
 * Name      :  TransportFaceIFCommon.h                                  
 * Describ   :  the defination of the common data type 
* Author    :  Zhang Botong                                          
 * Date      :  2018-06-04                                                 
 ********************************************************************/

#ifndef DETECTFACEIFCOMMON_H
#define DETECTFACEIFCOMMON_H

#ifdef __cplusplus
extern "C"
{
#endif
  
  typedef unsigned char UINT8;
  typedef unsigned short int UINT16;
  typedef unsigned int UINT32;
  typedef unsigned long long int UINT64;

  //1)用户登录信息接口		
  enum{
	UserNameLen= 64,
	UserPWDLen = 64};

  typedef struct _strUserInfo	
  {	
	UINT8 EncryptType; //指定用户信息在传输时加密的方式，取值：0：不加密，1：RSA
	UINT8 Authorization;  //指定用户要求的权限。 暂不使用
	UINT8 Align[2];  //对齐，保留，暂不使用
	UINT8 UserName[UserNameLen]; //用户名称，ASCII码表示，不足64位时以\0结束.
	UINT8 UserPWD[UserPWDLen];  //密码，ASCII表示，不足64位时以\0结束。
  }RWUserInfo;	

  //2)客户端/服务端描述接口		
  typedef struct _strRWIFInfo	
  {	
	UINT8 IPType; //说明IP的类型，取值 0：IPV4，1：IPV6
	UINT8 Align[3]; //对齐，保留，暂不使用
	UINT8 IPArray[16]; //IP地址，可能是IPV4或IPV6
	UINT16 CtrlPort; //IP地址所指设备开放的控制端口
	UINT16 DataPort; //IP地址所指设备开放的数据端口
	UINT64 CameraID; //相机的唯一ID。
  }RWIFInfo;	

  //3 客户端状态类型		
  typedef enum	
	{	
	  UnRegistered, //注销状态,即未注册
	  Registered, //注册状态,已经认证通过后注册到服务端。
	  OffLine, //离线状态,已经注册后长时间没有通信
	  OnLine, //在线状态,并且近期有通信
	  Unusual, //异常状态。已经注册的设备,但处于异常运行状态。
	  StateMax //状态最大值
	}RWClientState;

  //4	异常状态信息接口	
  typedef struct _strRWErrorInfo	
  {	
	UINT32 ErrorCode;
	UINT32 DataLen;
  }RWErrorInfo;	

  //5 图片信息接口		
  enum{ImgTextLen=256};
  typedef struct _strRWImgInfo	
  {	
	UINT64 ImgID; // 图片唯一性ID,用于标识不同的图片;
	UINT32 Width; //图片的宽
	UINT32 Height; //图片的高
	UINT32 Step; //图片的步长
	UINT8 ImgType; // 图片的格式,取值 0：灰度图; 1：RGB888;2：NV21;3：Jpg(此时Step为0);4：Bmp;5:BGR888;6:YUV402P
	UINT8 Align[3];// 对齐,保留,暂不使用
	UINT8 ImgText[ImgTextLen];//
	UINT64 TimeStamp;//时间戳
	UINT32 ImgLen;// 图片数据的长度
	UINT32 *ImgPtr;// 图片数据的指针
  }RWImgInfo;	

  //6)矩形框信息接口		
  typedef struct _strRWFaceInfo	
  {	
	UINT64 ImgID; //关联的图片ID
	UINT64 TrackNo; //跟踪号
	UINT32 RectX; // 人脸框左上角的横坐标
	UINT32 RectY; //人脸框左上角的纵坐标
	UINT32 RectWidth; // 人脸框的宽
	UINT32 RectHeight; // 人脸框的高
	UINT8 RectType; //人脸框类型：0：背景图, 1：头肩框, 2：半身照框,3：全身照框, 4：人脸框
	UINT8 Align[3]; // 对齐,保留,暂不使用
	float QScore;//图片质量分
  }RWFaceInfo;	

  //7 图片通知接口		
  typedef struct _strRWPicNotify	
  {	
	UINT64 CameraID; //相机唯一ID
	UINT8 NotifyType; //通知类型 
	UINT8 Align[3]; // 对齐,保留,暂不使用
	UINT32 ImgSum; //图片数量
  }RWPicNotify;	

  //8)客户端通知类型		
  typedef enum	
	{	
	  NotifyState,
	  NotifyImgInfo,
	  NotifyFaceInfo,
	  NotifyImgData,
	  NotifyVersion,
	  NotifyCountFace,
	  OptMax
	}RWNotifyType;	

  //9)应答回调函数类型		
  typedef void (*CallbackReplyFunc)(UINT64 *pReqID, UINT8 *pRet, UINT8 *pReplyData,
									UINT32 *pDataLen );

  //10 通知回调函数类型		
  typedef void (*CallbackNotifyFunc)( RWNotifyType *pNotifyType, UINT8 *pExtData, UINT32 *pDataLen );	

  //11 回调函数信息接口		
  typedef struct _strCallBackInfo	
  {	
	CallbackReplyFunc RegisterReplyFunc; //在服务端反馈登录的结果时调用
	CallbackReplyFunc CancelRelpyFunc; //在服务端反馈注销的结果时调用
	CallbackNotifyFunc NotifyFaceFunc;//在服务端通知图像信息时调用
	CallbackNotifyFunc NotifyClientFunc;//在服务端通知客户端状态时调用
  }CallBackInfo;	

 //CallbackFaceEncodeFunc 用户人脸编码回调函数定义，
 //调用该回调函数后，不会再调用用户的回调函数通知用户编译好的图片数据
 /****
 	UINT64 backgroundID 传入要编码的背景图ID
 	RWFaceInfo RectInfo 传入截图的坐标、长宽信息
 	int compressLevel 	传入要截图的压缩等级
 	void * userReserVer 预留指针
  ****/
  typedef int (*CallbackFaceEncodeFunc)( UINT64 backgroundID, RWFaceInfo rectInfo, int compressLevel, void * userReserVer);


  /****
 	UINT64 backgroundID 传入要编码的背景图指针
 	int compressLevel 	传入要截图的压缩等级
 	void * userReserVer 预留指针
  ****/
  //调用该回调函数后，不会再调用用户的回调函数通知用户编译好的图片数据
  typedef int (*CallbackBackGroundEncodeFunc)( UINT64 backgroundID, int compressLevel, void * userReserVer);

  typedef struct _strEncodeCallBackInfo	
  {	
	CallbackFaceEncodeFunc RegisterEncodeFunc; //在客户端注册人脸编码函数
	CallbackBackGroundEncodeFunc RegisterBackgroundEncodeFunc;//背景图的客户端编码注册函数
  }EncodeCallBackInfo;

  //12 串口信息类型		
  typedef struct _strSerialPortInfo	
  {	
	UINT8 *DevPathPtr;//字符串指针,DetectType=0x01时有效。指明检测模块使用的串口的设备路径
	UINT16 PathLen;//指明DevPathPtr的长度。
	UINT32 Baudrate;//串口的波特率
  }SerialPortInfo;

  //13)配置信息类型		
  typedef struct _strConfigInfo	
  {	
	UINT8 DetectType;  //指定是否加载检测功能：0x00 不加载;0x01 加载(默认值)
	UINT8 TransferType;  //指定传输中加载的协议,取值：0x00：不加载传输功能,0x01 瑞为私有SDK(默认值);0x02：厦门Http协议。
	UINT8 BuffCount;  //初始化内存池时内存块的总个数
	UINT8 Algin;  //字节对齐,暂不使用
	UINT64 CameraID;//相机ID。唯一标识
	UINT32 BuffSize; //初始化内存池时,每个内存块的大小
	SerialPortInfo *SerialPortInfoPtr;//串口信息的指针,DetectType=0x01时有效。指明检测模块使用的串口的配置信息
	RWUserInfo *SrcUserInfo;// 本设备的用户信息。 TransferType 非0时有效
	RWIFInfo *SrcIFInfo;// 本设备的接口信息,TransferType 非0时有效
	RWImgInfo * FrameInfo; //说明输入的原始帧的图像信息：图像格式，图像长，宽，图像步长
  }DetectConfigInfo;	

  //  14 人脸检测配置类型	
  typedef struct _strDetectCfg	
  {	
	UINT32 ConfigMask;//掩码，只有标志位为1的对应项有效。标识为0的项目忽略
	UINT8 DetectType;//抓拍类型，0：关闭抓拍，1：离开后抓拍（默认值），2：实时抓拍，3：间隔抓拍， 4：离开后+实时抓拍。
	UINT8 DetectHeartbeat;//心跳间隔时长，0：关闭对检测模块的心跳检测，正值：心跳检测间隔（默认为10秒）
	UINT8 DetectThreshold;//抓拍阈值，大于抓拍阈值的人脸才会考虑。范围0-100.默认值 1; 0表示检测到的头肩结果也考虑
	UINT8 BackgroundCtrl;//背景图上传控制;高4位控制人脸框叠加，低4位控制背景压缩率，0x00：不上传背景图，0x01：压缩率25%的背景图;0x02：压缩率60%的背景图，0x04：压缩率90%的背景图;0x00（高4位0）：背景图不叠加人脸;0x10：背景图叠加人脸框
	UINT8 FaceCtrl;//人脸图控制;（高4位控制人脸图类型，低4位控制人脸图压缩率）0x10：人脸图;0x20 头肩照;0x40 半身照; 0x80 全身照;0x01压缩率25&的人脸图，0x02压缩率60%的人脸图;0x04压缩率90%的人脸图
	UINT8 DetectTime;////在实时抓拍，表示人脸出现X秒后抓拍；间隔抓拍中，表示每隔X秒抓拍一张。
	UINT16 FaceSize; //最小人脸;小于最小人脸的数据会别忽略。取值0-512. 默认值0
  }DetectCfg;	
  
  //  15 升级文件信息类型	
  typedef struct _strUpgradeDataInfo	
  {	
	UINT8 *FileNamPtr;//文件名称字符串地址
	UINT8 NameLen;//文件名字符串长度（目前最大64）
	UINT8 *FilePathPtr;//文件路径字符串地址
	UINT8 PathLen;//路径字符串长度（目前最大64）
	UINT8 UpgradeType;//升级类型,0:app.cfg, 1:app.pkg, 2:Camera-QSPI-image.bin
	UINT8 Algin[3];//字节对齐。不使用
	UINT8 MD5[16];//心跳间隔时长，0：关闭对检测模块的心跳检测，正值：心跳检测间隔（默认为10秒）
	UINT32 FileSize;//整个文件的大小
  }UpgradeDataInfo;	
  
  //  16 升级请求类型	
  typedef enum	
	{	
	  ReqStartUpgrade,
	  ReqSendUpgradeData,
	  ReqStopUpgrade,
	  ReqMax
	}UpgradeReqType;	


  typedef struct _strPoint
  {
	int X;
	int Y;
  }Point;

  typedef struct _strLine
  {
	Point Head;
	Point End;
  }Line;


  enum{ VERSION_SIZE = 64 };

  typedef struct _strVersionInfo
  {
	char ProtocolVersion[VERSION_SIZE];
	char SoftwareVersion[VERSION_SIZE];
	char FirmwareVersion[VERSION_SIZE];
	char HardwareVersion[VERSION_SIZE];
  }VersionInfo;

#ifdef __cplusplus
}
#endif

#endif
