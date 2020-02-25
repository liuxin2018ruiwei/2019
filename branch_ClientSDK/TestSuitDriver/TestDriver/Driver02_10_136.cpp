/********************************************************************
* Name      :  Driver02_10_136.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "Driver02_10_136.h"
#include <unistd.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>




// to_string example
#include <iostream>   // std::cout
#include <string>     // std::string, std::to_string

using namespace std;




Driver02_10_136::Driver02_10_136(unsigned int nCaseId ): Driver( nCaseId )
{

}

int Driver02_10_136::init()
{
	Driver::init();
}

int Driver02_10_136::run()
{

	lxTest();
	
	return 0;
}

int Driver02_10_136::uninit()
{
	Driver::uninit();
}

Driver02_10_136::~Driver02_10_136()
{
	
}

/*****************************************
**********************************************************/

 
UINT64 Driver02_10_136::getNowTimeMS()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	UINT64 retTime =  (UINT64)((long long)tv.tv_sec*1000 +  (long long)(tv.tv_usec/1000) );//ms
	printf("getNowTimeMS %lu; ms = %ld\n", tv.tv_sec, tv.tv_usec/1000);
	return retTime;
}

/****==================================================***/
void Driver02_10_136::lxTest()
{
	//sleep(3);
	printf("Driver02_10_136 \n");

	char pVersion[64] ={0};
	
	char modulewareVersion[64] = {0};
	for(int i = 0; i<64; i++)
	{
		pVersion[i] = 0;
		//printf("%x ",i,modulewareVersion[i]);
		modulewareVersion[i] = i+3;
	}
/*	
	int nLen = 64;
	int ret32 = setModuleProtocolVersion( modulewareVersion, nLen );
	ret32 = getModuleProtocolVersion( pVersion,  nLen );
	printf("pVersion[0] = %x, pVersion[1] = %x \n", pVersion[0],pVersion[1]);
	for(int i = 0; i<nLen; i++)
	{
		printf("%x ",pVersion[i]);
	}
	printf("\n\n");

	nLen = 64;
	char  softwareVersion[64] = {0};
	for(int i = 0; i<nLen; i++)
	{
		softwareVersion[i] = i;
	}
	char  getSoftwareVersion[64];
	ret32 =  setModuleSoftwareVersion( softwareVersion, nLen );
	ret32 =  getModuleSoftwareVersion( getSoftwareVersion,  nLen );
	for(int i = 0; i<nLen; i++)
	{
		printf("%x ",getSoftwareVersion[i]);
	}

	printf("\n\n");
	nLen = 64;
	char setFirmwareVersion[64] ={0};
	for(int i = 0; i<nLen; i++)
	{
		setFirmwareVersion[i] = nLen -i;
	}
	char getFirmwareVersion[64];
	ret32 =  setModuleFirmwareVersion( setFirmwareVersion,  nLen );
	ret32 =  getModuleFirmwareVersion( getFirmwareVersion,  nLen );
	for(int i = 0; i<nLen; i++)
	{
		printf("%x ",getFirmwareVersion[i]);
	}
	printf("\nDriver02_10_136  ok\n\n");

*/
/*	printf("\nDriver02_10_140  \n\n");

	FaceNode myNode;
	myNode.InsertTime = getNowTimeMS();
	printf("myNode.InsertTime: %lu ; s = %ld \n",myNode.InsertTime,myNode.InsertTime/1000);
	
	time_t t_of_day = myNode.InsertTime/1000;
	struct tm* timeStruct;
	timeStruct  =  localtime(&t_of_day);

	int year=  timeStruct->tm_year+1900;
	int mon =  timeStruct->tm_mon+1;
	int day =  timeStruct->tm_mday;
	int hou =  timeStruct->tm_hour;
	int min =  timeStruct->tm_min;
	int sec =  timeStruct->tm_sec;
	
	printf("year = %d\n",year);
	printf("mon = %d \n",mon);
	printf("day = %d \n",day);
	printf("h : %d;min :%d;s :%d \n",hou,min,sec);
	printf("ms : %d \n",myNode.InsertTime%1000);

	char outName[114] ={0};
	int inputLen = sizeof(outName);
	UINT64 trackNo = 1234;
	myNode.TrackNo = trackNo;
	int outLen = 0;
	testFaceImgName( outName, &outLen,inputLen,&myNode);
	printf("testFaceImgName outLen = %d;%s \n",outLen,outName);
	//char * outName;

	sleep(2);
	trackNo = 4321;
	myNode.TrackNo = trackNo;
	testBackImgName( outName, &outLen,  inputLen, &myNode);
	printf("testBackImgName outLen = %d;%s \n",outLen,outName);

	printf("\nDriver02_10_140  ok\n\n");
	*/
}

void Driver02_10_136::writeInt32(UINT8 * data, int value)
{
	data[0] =  (char)(value & 0xff);
	data[1] =  (char)((value >> 8) & 0xff);
	data[2] =  (char)((value >> 16) & 0xff);
	data[3] =  (char)((value >> 24) & 0xff);
	
}

	 
	

