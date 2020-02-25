/********************************************************************
* Name      :  Driver02_10_115.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "Driver02_10_115.h"
#include <unistd.h>
#include <string>
#include<cstring>

//#include "MD5.h"
#include "md5.h"


/*
#include<iostream>
#include<string>
using namespace std;
#define shift(x, n) (((x) << (n)) | ((x) >> (32-(n))))//ÓÒÒÆµÄÊ±ºò£¬¸ßÎ»Ò»¶¨Òª²¹Áã£¬¶ø²»ÊÇ²¹³ä·ûºÅÎ»
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))    
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))
#define A 0x67452301
#define B 0xefcdab89
#define C 0x98badcfe
#define D 0x10325476
//strBayeµÄ³¤¶È
unsigned int strlength;
//A,B,C,DµÄÁÙÊ±±äÁ¿
unsigned int atemp;
unsigned int btemp;
unsigned int ctemp;
unsigned int dtemp;
//³£Á¿ti unsigned int(abs(sin(i+1))*(2pow32))
const unsigned int k[]={
        0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
        0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,0x698098d8,
        0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,
        0xa679438e,0x49b40821,0xf61e2562,0xc040b340,0x265e5a51,
        0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
        0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,
        0xfcefa3f8,0x676f02d9,0x8d2a4c8a,0xfffa3942,0x8771f681,
        0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,
        0xbebfbc70,0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
        0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,0xf4292244,
        0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,
        0xffeff47d,0x85845dd1,0x6fa87e4f,0xfe2ce6e0,0xa3014314,
        0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391};
//Ïò×óÎ»ÒÆÊý
const unsigned int s[]={7,12,17,22,7,12,17,22,7,12,17,22,7,
        12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
        4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,
        15,21,6,10,15,21,6,10,15,21,6,10,15,21};
const char str16[]="0123456789abcdef";
void mainLoop(unsigned int M[])
{
    unsigned int f,g;
    unsigned int a=atemp;
    unsigned int b=btemp;
    unsigned int c=ctemp;
    unsigned int d=dtemp;
    for (unsigned int i = 0; i < 64; i++)
    {
        if(i<16){
            f=F(b,c,d);
            g=i;
        }else if (i<32)
        {
            f=G(b,c,d);
            g=(5*i+1)%16;
        }else if(i<48){
            f=H(b,c,d);
            g=(3*i+5)%16;
        }else{
            f=I(b,c,d);
            g=(7*i)%16;
        }
        unsigned int tmp=d;
        d=c;
        c=b;
        b=b+shift((a+f+k[i]+M[g]),s[i]);
        a=tmp;
    }
    atemp=a+atemp;
    btemp=b+btemp;
    ctemp=c+ctemp;
    dtemp=d+dtemp;
}

unsigned int* add(string str)
{
    unsigned int num=((str.length()+8)/64)+1;//ÒÔ512Î»,64¸ö×Ö½ÚÎªÒ»×é
    unsigned int *strByte=new unsigned int[num*16];    //64/4=16,ËùÒÔÓÐ16¸öÕûÊý
    strlength=num*16;
    for (unsigned int i = 0; i < num*16; i++)
        strByte[i]=0;
    for (unsigned int i=0; i <str.length(); i++)
    {
        strByte[i>>2]|=(str[i])<<((i%4)*8);//Ò»¸öÕûÊý´æ´¢ËÄ¸ö×Ö½Ú£¬i>>2±íÊ¾i/4 Ò»¸öunsigned int¶ÔÓ¦4¸ö×Ö½Ú£¬±£´æ4¸ö×Ö·ûÐÅÏ¢
    }
    strByte[str.length()>>2]|=0x80<<(((str.length()%4))*8);//Î²²¿Ìí¼Ó1 Ò»¸öunsigned int±£´æ4¸ö×Ö·ûÐÅÏ¢,ËùÒÔÓÃ128×óÒÆ
    
    strByte[num*16-2]=str.length()*8;
    return strByte;
}
string changeHex(int a)
{
    int b;
    string str1;
    string str="";
    for(int i=0;i<4;i++)
    {
        str1="";
        b=((a>>i*8)%(1<<8))&0xff;   //ÄæÐò´¦ÀíÃ¿¸ö×Ö½Ú
        for (int j = 0; j < 2; j++)
        {
            str1.insert(0,1,str16[b%16]);
            b=b/16;
        }
        str+=str1;
    }
    return str;
}
string getMD5(string source)
{
    atemp=A;    //³õÊ¼»¯
    btemp=B;
    ctemp=C;
    dtemp=D;
    unsigned int *strByte=add(source);
    for(unsigned int i=0;i<strlength/16;i++)
    {
        unsigned int num[16];
        for(unsigned int j=0;j<16;j++)
            num[j]=strByte[i*16+j];
        mainLoop(num);
    }
    return changeHex(atemp).append(changeHex(btemp)).append(changeHex(ctemp)).append(changeHex(dtemp));
}
*/
	/*void PrintMD5(const string &str, MD5 &md5) {
		cout << "MD5(\"" << str << "\") = " << md5.toString() << endl;
	}
	
	string FileDigest(const string &file) {
	
		ifstream in(file.c_str(), ios::binary);
		if (!in)
			return "";
	
		MD5 md5;
		std::streamsize length;
		char buffer[1024];
		while (!in.eof()) {
			in.read(buffer, 1024);
			length = in.gcount();
			if (length > 0)
				md5.update(buffer, length);
		}
		in.close();
		return md5.toString();
	}
	
	*/

	

/////////////////////////////////////////////////////////////////////////////////////////////////////



Driver02_10_115::Driver02_10_115(unsigned int nCaseId ): Driver( nCaseId )
{

}

int Driver02_10_115::init()
{
	Driver::init();
}

int Driver02_10_115::run()
{

	lxTest();
	
	return 0;
}

int Driver02_10_115::uninit()
{
	Driver::uninit();
}

Driver02_10_115::~Driver02_10_115()
{
	
}
void testTest(UINT64 *pReqID, UINT8 *pRet, UINT8 *pReplyData,UINT32 *pDataLen )
{
	printf("for fate!\n\n");
}
/*****************************************
**********************************************************/

  

/****==================================================***/
void Driver02_10_115::lxTest()
{
	int ret32 = 0;

	printf("lx115\n");
	printf("---start---------------------------------------------test----WBS_02_10_115-------------------------\n");
	int a = 1;
	UINT64 pReqID = 56;
		
	/*
	  UINT8 *FileNamPtr;//æ–‡ä»¶åç§°å­—ç¬¦ä¸²åœ°å€
	  UINT8 NameLen;//å­—ç¬¦ä¸²é•¿åº¦ï¼ˆç›®å‰æœ€å¤§64ï¼‰
	  UINT8 Algin[3];//å­—èŠ‚å¯¹é½ã€‚ä¸ä½¿ç”¨
	  UINT8 MD5[16];//å¿ƒè·³é—´éš”æ—¶é•¿ï¼Œ0ï¼šå…³é—­å¯¹æ£€æµ‹æ¨¡å—çš„å¿ƒè·³æ£€æµ‹ï¼Œæ­£å€¼ï¼šå¿ƒè·³æ£€æµ‹é—´éš”ï¼ˆé»˜è®¤ä¸º10ç§’ï¼‰
	  UINT32 FileSize;
	*/
	UpgradeDataInfo DataInfo ;
		
	UpgradeDataInfo *pDataInfo = &DataInfo;

	const char* pFileName =  "Camera-QSPI-image.bin";//"test.txt";
	pDataInfo->FileNamPtr = (UINT8*)pFileName;
	pDataInfo->NameLen = sizeof("Camera-QSPI-image.bin");
	const char* pFilePath = "/home/ruiwei/Desktop"; // "/mnt/hgfs/code/work/2 source/2 Software/branch_ClientSDK";
	pDataInfo->FilePathPtr = (UINT8*)pFilePath;
	pDataInfo->PathLen = sizeof("/home/ruiwei/Desktop");//("/mnt/hgfs/code/work/2 source/2 Software/branch_ClientSDK");
	UINT8 tempMD5[16] = {	0x30,0x55,0x1c,0x58,
							0x4d,0xf5,0x82,0x09,
							0xf1,0x2f,0xbb,0x37,
							0xb9,0xf0,0x98,0xef
					};

	

	printf("FilePathPtr = %s;name FileNamPtr = %s\n",pDataInfo->FileNamPtr,pDataInfo->FilePathPtr);
	int i;
	memcpy(pDataInfo->MD5, tempMD5,16);
	/*for( i = 0 ; i < 16; i++)
	{
		printf("set %x\n",pDataInfo->MD5[i]);
	}
	printf("\n");
	*/
	//pDataInfo->FileSize = 37585;
	
	//callbackInfo.RegisterReplyFunc = usrCallbackReplyRegisterClient;
	CallbackReplyFunc pFunc = testTest; 
	printf("\n%x\n",pFunc);
	printf("WBS_02_10_115 pDataInfo = %x,pDataInfo->FileSize = %u,  pDataInfo->F; \n",pDataInfo,pDataInfo->FileSize);
	startUpgrade((void *)&a, &pReqID, pDataInfo,pFunc);
	
	printf("---end---------------------------------------------test----WBS_02_10_115-------------------------\n");
}

void Driver02_10_115::writeInt32(UINT8 * data, int value)
{
	data[0] =  (char)(value & 0xff);
	data[1] =  (char)((value >> 8) & 0xff);
	data[2] =  (char)((value >> 16) & 0xff);
	data[3] =  (char)((value >> 24) & 0xff);
	
}

	 
	

