/********************************************************************
* Name      :  DriverTestCommunicate.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/

#include <stdio.h>
#include <unistd.h>
#include "DetectFaceIF.h"
#include "DriverTestCommunicate.h"

DriverTestCommunicate::DriverTestCommunicate(unsigned int nCaseId ): Driver( nCaseId )
{

}

int DriverTestCommunicate::init()
{
	Driver::init();
}

int DriverTestCommunicate::run()
{
	void *TFHandle = getHandle();

	DetectCfg detectcfg;
	detectcfg.ConfigMask = 0x01;
	detectcfg.DetectType = 4;
	setDetectConfig( TFHandle, &detectcfg);

	usleep( 5* 1000 *1000 );
//	LXTestWBS_02_10_01();
	return 0;
}

int DriverTestCommunicate::uninit()
{
	Driver::uninit();
}

DriverTestCommunicate::~DriverTestCommunicate()
{
	
}

