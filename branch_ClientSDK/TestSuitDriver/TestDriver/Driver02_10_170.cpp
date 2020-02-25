/********************************************************************
* Name      :  Driver02_10_170.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "DetectFaceIF.h"
#include "Driver02_10_170.h"
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




Driver02_10_170::Driver02_10_170(unsigned int nCaseId ): Driver( nCaseId )
{

}

int Driver02_10_170::init()
{
	Driver::init();
}

int Driver02_10_170::run()
{

	/*int a = 1;
	//uninitDetectFaceIF((void *)&a);
	//printf("test uninit ok \n");
	//Driver::init();
	//printf("Driver init ok \n");
	printf("------------------------------test Encode--------------------- \n");
	//-------------------test------
	char fileTempName[128] = {0};
	memcpy(&fileTempName,"/dev/Camera-QSPI-image.bin", sizeof("/dev/Camera-QSPI-image.bin"));
	printf("fileTempName = %s\n\n",fileTempName);
	//-------------------test------
	testZmode(fileTempName);
	printf("------------------------------Encode test ok-----------------------\n");

	uninitDetectFaceIF((void *)&a);
	printf("test uninit ok \n");
	Driver::init();
	printf("Driver init ok \n");

	printf("------------------------------test Encode2--------------------- \n");
	testZmode(fileTempName);
	printf("------------------------------Encode2 test ok-----------------------\n");
	*/
	return 0;
}

int Driver02_10_170::uninit()
{
	Driver::uninit();
}

Driver02_10_170::~Driver02_10_170()
{
	
}




	 
	

