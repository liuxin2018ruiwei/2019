/********************************************************************
* Name      :  TestDriverFactory.cpp                                  
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/

#include <stdio.h>
#include "TestDriverFactory.h"
#include "Driver02_10_01.h"
#include "Driver02_10_05.h"
#include "Driver02_10_16.h"
#include "Driver02_10_17.h"
#include "Driver02_10_18.h"
#include "Driver02_10_32.h"
#include "Driver02_10_33.h"
#include "Driver02_10_37.h"
#include "Driver02_10_39.h"
#include "Driver02_10_59.h"
#include "Driver02_10_62.h"
#include "Driver02_10_79.h"
#include "Driver02_10_115.h"
#include "Driver02_10_136.h"
#include "Driver02_10_156.h"
#include "Driver02_10_160.h"
#include "Driver02_10_170.h"
#include "Driver02_10_180.h"
#include "DriverTestCommunicate.h"


#include "ST_1_initDetectFaceIF.h"
#include "ST_2_registerCallback.h"
#include "ST_3_startSDK.h"
#include "ST_4_stopSDK.h"
#include "ST_5_getClientSDKVersion.h"

#include "ST_6_getFaceDetectVersionInfo.h"
#include "ST_8_uninitDetectFaceIF.h"
#include "ST_9_addFrame.h"
#include "ST_10_getDetectConfig.h"
#include "ST_11_setDetectConfig.h"
#include "ST_12_addDetectRegion.h"
#include "ST_13_deletetDetectRegion.h"
#include "ST_14_addShieldRegion.h"
#include "ST_15_deletetShieldRegion.h"
#include "ST_16_startUpgrade.h"

#include "ST_18_setLine.h"
#include "ST_19_resetCounter.h"

#include "IT_01_heatBeat_01.h"
#include "IT_01_heatBeat_02.h"
#include "IT_01_heatBeat_03.h"
#include "IT_01_heatBeat_04.h"
#include "IT_01_heatBeat_05.h"
#include "IT_01_heatBeat_06.h"
#include "IT_01_heatBeat_07.h"
#include "IT_01_heatBeat_08.h"

#include "IT_02_detectFace_01.h"
#include "IT_02_detectFace_02.h"
#include "IT_02_detectFace_03.h"
#include "IT_02_detectFace_04.h"
#include "IT_02_detectFace_05.h"


#include "IT_03_faceRegion_01.h"
#include "IT_03_faceRegion_02.h"
#include "IT_03_faceRegion_03.h"
#include "IT_03_faceRegion_04.h"
#include "IT_03_faceRegion_05.h"
#include "IT_03_faceRegion_06.h"
#include "IT_03_faceRegion_07.h"
#include "IT_03_faceRegion_08.h"
#include "IT_03_faceRegion_09.h"

#include "IT_04_faceSize_01.h"
#include "IT_04_faceSize_02.h"
#include "IT_04_faceSize_03.h"
#include "IT_04_faceSize_04.h"
#include "IT_04_faceSize_05.h"
#include "IT_04_faceSize_06.h"
#include "IT_04_faceSize_07.h"

#include "IT_05_faceDetectCapture_01.h"
#include "IT_05_faceDetectCapture_02.h"
#include "IT_05_faceDetectCapture_03.h"
#include "IT_05_faceDetectCapture_04.h"
#include "IT_05_faceDetectCapture_05.h"
#include "IT_05_faceDetectCapture_06.h"
#include "IT_05_faceDetectCapture_07.h"
#include "IT_05_faceDetectCapture_08.h"
#include "IT_05_faceDetectCapture_09.h"

#include "IT_06_CaptureDetectThreshold_01.h"
#include "IT_06_CaptureDetectThreshold_02.h"
#include "IT_06_CaptureDetectThreshold_03.h"
#include "IT_06_CaptureDetectThreshold_04.h"
#include "IT_06_CaptureDetectThreshold_05.h"
#include "IT_06_CaptureDetectThreshold_06.h"
#include "IT_06_CaptureDetectThreshold_07.h"
#include "IT_06_CaptureDetectThreshold_08.h"
#include "IT_06_CaptureDetectThreshold_09.h"
#include "IT_06_CaptureDetectThreshold_10.h"

#include "IT_07_BackgroundSwitchUpload_01.h"
#include "IT_07_BackgroundSwitchUpload_02.h"
#include "IT_07_BackgroundSwitchUpload_03.h"
#include "IT_07_BackgroundSwitchUpload_04.h"
#include "IT_07_BackgroundSwitchUpload_05.h"
#include "IT_07_BackgroundSwitchUpload_06.h"
#include "IT_07_BackgroundSwitchUpload_07.h"
#include "IT_07_BackgroundSwitchUpload_08.h"
#include "IT_07_BackgroundSwitchUpload_09.h"
#include "IT_07_BackgroundSwitchUpload_10.h"

#include "IT_08_FaceRectDraw_01.h"
#include "IT_08_FaceRectDraw_02.h"
#include "IT_08_FaceRectDraw_03.h"
#include "IT_08_FaceRectDraw_04.h"
#include "IT_08_FaceRectDraw_05.h"
#include "IT_08_FaceRectDraw_06.h"
#include "IT_08_FaceRectDraw_07.h"
#include "IT_08_FaceRectDraw_08.h"
#include "IT_08_FaceRectDraw_09.h"
#include "IT_08_FaceRectDraw_10.h"


#include "IT_09_BackPicCompress_01.h"
#include "IT_09_BackPicCompress_02.h"
#include "IT_09_BackPicCompress_03.h"
#include "IT_09_BackPicCompress_04.h"
#include "IT_09_BackPicCompress_05.h"
#include "IT_09_BackPicCompress_06.h"
#include "IT_09_BackPicCompress_07.h"
#include "IT_09_BackPicCompress_08.h"
#include "IT_09_BackPicCompress_09.h"
#include "IT_09_BackPicCompress_10.h"
#include "IT_09_BackPicCompress_11.h"
#include "IT_09_BackPicCompress_12.h"




#include "IT_10_faceCompress_01.h"
#include "IT_10_faceCompress_02.h"
#include "IT_10_faceCompress_03.h"
#include "IT_10_faceCompress_04.h"
#include "IT_10_faceCompress_05.h"
#include "IT_10_faceCompress_06.h"
#include "IT_10_faceCompress_07.h"
#include "IT_10_faceCompress_08.h"
#include "IT_10_faceCompress_09.h"
#include "IT_10_faceCompress_10.h"
#include "IT_10_faceCompress_11.h"
#include "IT_10_faceCompress_12.h"
#include "IT_10_faceCompress_13.h"



#include "IT_11_faceSizeCtrl_01.h"
#include "IT_11_faceSizeCtrl_02.h"
#include "IT_11_faceSizeCtrl_03.h"
#include "IT_11_faceSizeCtrl_04.h"
#include "IT_11_faceSizeCtrl_05.h"
#include "IT_11_faceSizeCtrl_06.h"
#include "IT_11_faceSizeCtrl_07.h"
#include "IT_11_faceSizeCtrl_08.h"
#include "IT_11_faceSizeCtrl_09.h"
#include "IT_11_faceSizeCtrl_10.h"
#include "IT_11_faceSizeCtrl_11.h"
#include "IT_11_faceSizeCtrl_12.h"
#include "IT_11_faceSizeCtrl_13.h"




#include "IT_12_ModleShow_01.h"
#include "IT_12_ModleShow_02.h"
#include "IT_12_ModleShow_03.h"
#include "IT_12_ModleShow_04.h"
#include "IT_12_ModleShow_05.h"
#include "IT_12_ModleShow_06.h"
#include "IT_12_ModleShow_07.h"
#include "IT_12_ModleShow_08.h"
#include "IT_12_ModleShow_09.h"
#include "IT_12_ModleShow_10.h"
#include "IT_12_ModleShow_11.h"

#include "IT_13_Upgrade_01.h"
#include "IT_13_Upgrade_02.h"
#include "IT_13_Upgrade_03.h"



#include "IT_14_CountPeople_01.h"
#include "IT_14_CountPeople_02.h"
#include "IT_14_CountPeople_03.h"
#include "IT_14_CountPeople_04.h"
#include "IT_14_CountPeople_05.h"
#include "IT_14_CountPeople_06.h"
#include "IT_14_CountPeople_07.h"
#include "IT_14_CountPeople_08.h"
#include "IT_14_CountPeople_09.h"
#include "IT_14_CountPeople_10.h"
#include "IT_14_CountPeople_11.h"


#include "IT_15_UpgradeFunction_01.h"
#include "IT_15_UpgradeFunction_02.h"
#include "IT_15_UpgradeFunction_03.h"
#include "IT_15_UpgradeFunction_04.h"
#include "IT_15_UpgradeFunction_05.h"
#include "IT_15_UpgradeFunction_06.h"
#include "IT_15_UpgradeFunction_07.h"
#include "IT_15_UpgradeFunction_08.h"
#include "IT_15_UpgradeFunction_09.h"
#include "IT_15_UpgradeFunction_10.h"
#include "IT_15_UpgradeFunction_11.h"
#include "IT_15_UpgradeFunction_12.h"
#include "IT_15_UpgradeFunction_13.h"






TestDriverFactory::TestDriverFactory()
{
}

Driver * TestDriverFactory::createDriver( CaseID nCaseId )
{
	Driver * pDriver = nullptr;
	switch( nCaseId )
	{
		case Case_Sample:
			pDriver = new Driver( Case_Sample );
			break;
		case Case02_10_01:
			pDriver = new Driver02_10_01(Case02_10_01 );
			break;
		case Case02_10_05:
			pDriver = new Driver02_10_05( Case02_10_05 );
			break;
		case Case02_10_16:
			pDriver = new Driver02_10_16( Case02_10_16 );
			break;
		case Case02_10_17:
			pDriver = new Driver02_10_17( Case02_10_17 );
			break;
		case Case02_10_18:
			pDriver = new Driver02_10_18( Case02_10_18 );
			break;
		case Case02_10_32:
			pDriver = new Driver02_10_32( Case02_10_32);
			break;
		case Case02_10_33:
			pDriver = new Driver02_10_33( Case02_10_33 );
			break;
		case Case02_10_37 :
			pDriver = new Driver02_10_37( Case02_10_37 );
			break;
		case Case02_10_39 :
			pDriver = new Driver02_10_39( Case02_10_39 );
			break;
		case Case02_10_59 :
			pDriver = new Driver02_10_59( Case02_10_59 );
			break;
		case Case02_10_62 :
			pDriver = new Driver02_10_62 (Case02_10_62 );
			break;
		case Case02_10_79 :
			pDriver = new Driver02_10_79 ( Case02_10_79 );
			break;
		case Case02_10_115 :
			pDriver = new Driver02_10_115 ( Case02_10_115 );
			break;
		case Case02_10_136 :
			pDriver = new Driver02_10_136 ( Case02_10_136 );
			break;
		case Case02_10_156 :
			pDriver = new Driver02_10_156 ( Case02_10_156 );
			break;
		case Case02_10_160 :
			pDriver = new Driver02_10_160 ( Case02_10_160 );
			break;
		case Case02_10_170 :
			pDriver = new Driver02_10_170 ( Case02_10_170 );
			break;
		case Case02_10_180 :
			pDriver = new Driver02_10_180 ( Case02_10_180 );
			break;
		case CaseTestCommunicate:
			pDriver = new DriverTestCommunicate( CaseTestCommunicate );
			break;
		case T_NUM_1_initDetectFaceIF :
			pDriver = new ST_1_initDetectFaceIF ( T_NUM_1_initDetectFaceIF );
			break;
		case T_NUM_2_registerCallback :
			pDriver = new ST_2_registerCallback ( T_NUM_2_registerCallback );
			break;
		case T_NUM_3_startSDK :
			pDriver = new ST_3_startSDK ( T_NUM_3_startSDK );
			break;
		case T_NUM_4_stoptSDK :
			pDriver = new ST_4_stopSDK ( T_NUM_4_stoptSDK );
			break;
		case T_NUM_5_getClientSDKVersion:
			pDriver = new ST_5_getClientSDKVersion( T_NUM_5_getClientSDKVersion );
			break;
		case T_NUM_6_getFaceDetectVersionInfo : 
			pDriver = new  ST_6_getFaceDetectVersionInfo( T_NUM_6_getFaceDetectVersionInfo );
			break;
		case T_NUM_8_uninitDetectFaceIF : 
			pDriver = new  ST_8_uninitDetectFaceIF( T_NUM_8_uninitDetectFaceIF );
			break;
		case T_NUM_9_addFrame : 
			pDriver = new  ST_9_addFrame( T_NUM_9_addFrame );
			break;
		case T_NUM_10_getDetectConfig : 
			pDriver = new  ST_10_getDetectConfig( T_NUM_10_getDetectConfig );
			break;
		case T_NUM_11_setDetectConfig : 
			pDriver = new  ST_11_setDetectConfig( T_NUM_11_setDetectConfig );
			break;
		case T_NUM_12_addDetectRegion : 
			pDriver = new  ST_12_addDetectRegion( T_NUM_12_addDetectRegion );
			break;
		case T_NUM_13_deletetDetectRegion:
			pDriver = new ST_13_deletetDetectRegion(T_NUM_13_deletetDetectRegion);
			break;
		case T_NUM_14_addShieldRegion:
			pDriver = new ST_14_addShieldRegion(T_NUM_14_addShieldRegion);
			break;
		case T_NUM_15_deletetShieldRegion:
			pDriver = new ST_15_deletetShieldRegion(T_NUM_15_deletetShieldRegion);
			break;
		case T_NUM_16_softUpgrade:
			pDriver = new ST_16_startUpgrade(T_NUM_16_softUpgrade);
			break;
		case T_NUM_18_setLine:
			pDriver = new ST_18_setLine(T_NUM_18_setLine);
			break;
		case T_NUM_19_resetCounter:
			pDriver = new ST_19_resetCounter(T_NUM_19_resetCounter);
			break;
		case I_NUM_01_HeatBeat_01:
			pDriver = new IT_01_heatBeat_01 (I_NUM_01_HeatBeat_01);
			break;
		case I_NUM_01_HeatBeat_02:
			pDriver = new IT_01_heatBeat_02 (I_NUM_01_HeatBeat_02);
			break;
		case I_NUM_01_HeatBeat_03:
			pDriver = new IT_01_heatBeat_03 (I_NUM_01_HeatBeat_03);
			break;
		case I_NUM_01_HeatBeat_04:
			pDriver = new IT_01_heatBeat_04 (I_NUM_01_HeatBeat_04);
			break;
		case I_NUM_01_HeatBeat_05:
			pDriver = new IT_01_heatBeat_05 (I_NUM_01_HeatBeat_05);
			break;
		case I_NUM_01_HeatBeat_06:
			pDriver = new IT_01_heatBeat_06 (I_NUM_01_HeatBeat_06);
			break;
		case I_NUM_01_HeatBeat_07:
			pDriver = new IT_01_heatBeat_07 (I_NUM_01_HeatBeat_07);
			break;
		case I_NUM_01_HeatBeat_08:
			pDriver = new IT_01_heatBeat_08 (I_NUM_01_HeatBeat_08);
			break;
		
		case I_NUM_02_DetectFace_01:
			pDriver = new IT_02_detectFace_01 (I_NUM_02_DetectFace_01);
			break;
		case I_NUM_02_DetectFace_02:
			pDriver = new IT_02_detectFace_02 (I_NUM_02_DetectFace_02);
			break;
		case I_NUM_02_DetectFace_03:
			pDriver = new IT_02_detectFace_03 (I_NUM_02_DetectFace_03);
			break;
		case I_NUM_02_DetectFace_04:
			pDriver = new IT_02_detectFace_04 (I_NUM_02_DetectFace_04);
			break;
		case I_NUM_02_DetectFace_05:
			pDriver = new IT_02_detectFace_05 (I_NUM_02_DetectFace_05);
			break;

		case I_NUM_03_FaceRegion_01:
			pDriver = new IT_03_faceRegion_01 (I_NUM_03_FaceRegion_01);
			break;
		case I_NUM_03_FaceRegion_02:
			pDriver = new IT_03_faceRegion_02 (I_NUM_03_FaceRegion_02);
			break;
		case I_NUM_03_FaceRegion_03:
			pDriver = new IT_03_faceRegion_03 (I_NUM_03_FaceRegion_03);
			break;
		case I_NUM_03_FaceRegion_04:
			pDriver = new IT_03_faceRegion_04 (I_NUM_03_FaceRegion_04);
			break;
		case I_NUM_03_FaceRegion_05:
			pDriver = new IT_03_faceRegion_05 (I_NUM_03_FaceRegion_05);
			break;
		case I_NUM_03_FaceRegion_06:
			pDriver = new IT_03_faceRegion_06 (I_NUM_03_FaceRegion_06);
			break;
		case I_NUM_03_FaceRegion_07:
			pDriver = new IT_03_faceRegion_07 (I_NUM_03_FaceRegion_07);
			break;
		case I_NUM_03_FaceRegion_08:
			pDriver = new IT_03_faceRegion_08 (I_NUM_03_FaceRegion_08);
			break;
		case I_NUM_03_FaceRegion_09:
			pDriver = new IT_03_faceRegion_09 (I_NUM_03_FaceRegion_09);
			break;

		case I_NUM_04_FaceSize_01:
			pDriver = new IT_04_faceSize_01 (I_NUM_04_FaceSize_01);
			break;
		case I_NUM_04_FaceSize_02:
			pDriver = new IT_04_faceSize_02 (I_NUM_04_FaceSize_02);
			break;
		case I_NUM_04_FaceSize_03:
			pDriver = new IT_04_faceSize_03 (I_NUM_04_FaceSize_03);
			break;
		case I_NUM_04_FaceSize_04:
			pDriver = new IT_04_faceSize_04 (I_NUM_04_FaceSize_04);
			break;
		case I_NUM_04_FaceSize_05:
			pDriver = new IT_04_faceSize_05 (I_NUM_04_FaceSize_05);
			break;
		case I_NUM_04_FaceSize_06:
			pDriver = new IT_04_faceSize_06 (I_NUM_04_FaceSize_06);
			break;

		case I_NUM_05_faceDetectCapture_01:
			pDriver = new IT_05_faceDetectCapture_01 (I_NUM_05_faceDetectCapture_01);
			break;
		case I_NUM_05_faceDetectCapture_02:
			pDriver = new IT_05_faceDetectCapture_02 (I_NUM_05_faceDetectCapture_02);
			break;
		case I_NUM_05_faceDetectCapture_03:
			pDriver = new IT_05_faceDetectCapture_03 (I_NUM_05_faceDetectCapture_03);
			break;
		case I_NUM_05_faceDetectCapture_04:
			pDriver = new IT_05_faceDetectCapture_04 (I_NUM_05_faceDetectCapture_04);
			break;
		case I_NUM_05_faceDetectCapture_05:
			pDriver = new IT_05_faceDetectCapture_05 (I_NUM_05_faceDetectCapture_05);
			break;
		case I_NUM_05_faceDetectCapture_06:
			pDriver = new IT_05_faceDetectCapture_06 (I_NUM_05_faceDetectCapture_06);
			break;
		case I_NUM_05_faceDetectCapture_07:
			pDriver = new IT_05_faceDetectCapture_07 (I_NUM_05_faceDetectCapture_07);
			break;
		case I_NUM_05_faceDetectCapture_08:
			pDriver = new IT_05_faceDetectCapture_08 (I_NUM_05_faceDetectCapture_08);
			break;
		case I_NUM_05_faceDetectCapture_09:
			pDriver = new IT_05_faceDetectCapture_09 (I_NUM_05_faceDetectCapture_09);
			break;

		case I_NUM_06_CaptureDetectThreshold_01:
			pDriver = new IT_06_CaptureDetectThreshold_01 (I_NUM_06_CaptureDetectThreshold_01);
			break;
		case I_NUM_06_CaptureDetectThreshold_02:
			pDriver = new IT_06_CaptureDetectThreshold_02 (I_NUM_06_CaptureDetectThreshold_02);
			break;
		case I_NUM_06_CaptureDetectThreshold_03:
			pDriver = new IT_06_CaptureDetectThreshold_03 (I_NUM_06_CaptureDetectThreshold_03);
			break;
		case I_NUM_06_CaptureDetectThreshold_04:
			pDriver = new IT_06_CaptureDetectThreshold_04 (I_NUM_06_CaptureDetectThreshold_04);
			break;
		case I_NUM_06_CaptureDetectThreshold_05:
			pDriver = new IT_06_CaptureDetectThreshold_05 (I_NUM_06_CaptureDetectThreshold_05);
			break;
		case I_NUM_06_CaptureDetectThreshold_06:
			pDriver = new IT_06_CaptureDetectThreshold_06 (I_NUM_06_CaptureDetectThreshold_06);
			break;
		case I_NUM_06_CaptureDetectThreshold_07:
			pDriver = new IT_06_CaptureDetectThreshold_07 (I_NUM_06_CaptureDetectThreshold_07);
			break;
		case I_NUM_06_CaptureDetectThreshold_08:
			pDriver = new IT_06_CaptureDetectThreshold_08 (I_NUM_06_CaptureDetectThreshold_08);
			break;
		case I_NUM_06_CaptureDetectThreshold_09:
			pDriver = new IT_06_CaptureDetectThreshold_09 (I_NUM_06_CaptureDetectThreshold_09);
			break;
		case I_NUM_06_CaptureDetectThreshold_10:
			pDriver = new IT_06_CaptureDetectThreshold_10 (I_NUM_06_CaptureDetectThreshold_10);
			break;
		
		case I_NUM_07_BackgroundSwitchUpload_01:
			pDriver = new IT_07_BackgroundSwitchUpload_01 (I_NUM_07_BackgroundSwitchUpload_01);
			break;
		case I_NUM_07_BackgroundSwitchUpload_02:
			pDriver = new IT_07_BackgroundSwitchUpload_02 (I_NUM_07_BackgroundSwitchUpload_02);
			break;
		case I_NUM_07_BackgroundSwitchUpload_03:
			pDriver = new IT_07_BackgroundSwitchUpload_03 (I_NUM_07_BackgroundSwitchUpload_03);
			break;
		case I_NUM_07_BackgroundSwitchUpload_04:
			pDriver = new IT_07_BackgroundSwitchUpload_04 (I_NUM_07_BackgroundSwitchUpload_04);
			break;
		case I_NUM_07_BackgroundSwitchUpload_05:
			pDriver = new IT_07_BackgroundSwitchUpload_05 (I_NUM_07_BackgroundSwitchUpload_05);
			break;
		case I_NUM_07_BackgroundSwitchUpload_06:
			pDriver = new IT_07_BackgroundSwitchUpload_06 (I_NUM_07_BackgroundSwitchUpload_06);
			break;
		case I_NUM_07_BackgroundSwitchUpload_07:
			pDriver = new IT_07_BackgroundSwitchUpload_07 (I_NUM_07_BackgroundSwitchUpload_07);
			break;
		case I_NUM_07_BackgroundSwitchUpload_08:
			pDriver = new IT_07_BackgroundSwitchUpload_08 (I_NUM_07_BackgroundSwitchUpload_08);
			break;
		case I_NUM_07_BackgroundSwitchUpload_09:
			pDriver = new IT_07_BackgroundSwitchUpload_09 (I_NUM_07_BackgroundSwitchUpload_09);
			break;
		case I_NUM_07_BackgroundSwitchUpload_10:
			pDriver = new IT_07_BackgroundSwitchUpload_10 (I_NUM_07_BackgroundSwitchUpload_10);
			break;

		case I_NUM_08_FaceRectDraw_01:
			pDriver = new IT_08_FaceRectDraw_01 (I_NUM_08_FaceRectDraw_01);
			break;
		case I_NUM_08_FaceRectDraw_02:
			pDriver = new IT_08_FaceRectDraw_02 (I_NUM_08_FaceRectDraw_02);
			break;
		case I_NUM_08_FaceRectDraw_03:
			pDriver = new IT_08_FaceRectDraw_03 (I_NUM_08_FaceRectDraw_03);
			break;
		case I_NUM_08_FaceRectDraw_04:
			pDriver = new IT_08_FaceRectDraw_04 (I_NUM_08_FaceRectDraw_04);
			break;
		case I_NUM_08_FaceRectDraw_05:
			pDriver = new IT_08_FaceRectDraw_05 (I_NUM_08_FaceRectDraw_05);
			break;
		case I_NUM_08_FaceRectDraw_06:
			pDriver = new IT_08_FaceRectDraw_06 (I_NUM_08_FaceRectDraw_06);
			break;
		case I_NUM_08_FaceRectDraw_07:
			pDriver = new IT_08_FaceRectDraw_07 (I_NUM_08_FaceRectDraw_07);
			break;
		case I_NUM_08_FaceRectDraw_08:
			pDriver = new IT_08_FaceRectDraw_08 (I_NUM_08_FaceRectDraw_08);
			break;
		case I_NUM_08_FaceRectDraw_09:
			pDriver = new IT_08_FaceRectDraw_09 (I_NUM_08_FaceRectDraw_09);
			break;
		case I_NUM_08_FaceRectDraw_10:
			pDriver = new IT_08_FaceRectDraw_10 (I_NUM_08_FaceRectDraw_10);
			break;
		
		
		case I_NUM_09_BackPicCompress_01:
			pDriver = new IT_09_BackPicCompress_01 (I_NUM_09_BackPicCompress_01);
			break;
		case I_NUM_09_BackPicCompress_02:
			pDriver = new IT_09_BackPicCompress_02 (I_NUM_09_BackPicCompress_02);
			break;
		case I_NUM_09_BackPicCompress_03:
			pDriver = new IT_09_BackPicCompress_03 (I_NUM_09_BackPicCompress_03);
			break;
		case I_NUM_09_BackPicCompress_04:
			pDriver = new IT_09_BackPicCompress_04 (I_NUM_09_BackPicCompress_03);
			break;
		case I_NUM_09_BackPicCompress_05:
			pDriver = new IT_09_BackPicCompress_05 (I_NUM_09_BackPicCompress_05);
			break;
		case I_NUM_09_BackPicCompress_06:
			pDriver = new IT_09_BackPicCompress_06 (I_NUM_09_BackPicCompress_06);
			break;
		case I_NUM_09_BackPicCompress_07:
			pDriver = new IT_09_BackPicCompress_07 (I_NUM_09_BackPicCompress_07);
			break;
		case I_NUM_09_BackPicCompress_08:
			pDriver = new IT_09_BackPicCompress_08 (I_NUM_09_BackPicCompress_08);
			break;
		case I_NUM_09_BackPicCompress_09:
			pDriver = new IT_09_BackPicCompress_05 (I_NUM_09_BackPicCompress_09);
			break;
		case I_NUM_09_BackPicCompress_10:
			pDriver = new IT_09_BackPicCompress_06 (I_NUM_09_BackPicCompress_10);
			break;
		case I_NUM_09_BackPicCompress_11:
			pDriver = new IT_09_BackPicCompress_07 (I_NUM_09_BackPicCompress_11);
			break;
		case I_NUM_09_BackPicCompress_12:
			pDriver = new IT_09_BackPicCompress_08 (I_NUM_09_BackPicCompress_12);
			break;
		
		
		case I_NUM_10_faceCompress_01:
			pDriver = new IT_10_faceCompress_01 (I_NUM_10_faceCompress_01);
			break;
		case I_NUM_10_faceCompress_02:
			pDriver = new IT_10_faceCompress_02 (I_NUM_10_faceCompress_02);
			break;
		case I_NUM_10_faceCompress_03:
			pDriver = new IT_10_faceCompress_03 (I_NUM_10_faceCompress_03);
			break;
		case I_NUM_10_faceCompress_04:
			pDriver = new IT_10_faceCompress_04 (I_NUM_10_faceCompress_04);
			break;
		case I_NUM_10_faceCompress_05:
			pDriver = new IT_10_faceCompress_05 (I_NUM_10_faceCompress_05);
			break;
		case I_NUM_10_faceCompress_06:
			pDriver = new IT_10_faceCompress_06 (I_NUM_10_faceCompress_06);
			break;
		case I_NUM_10_faceCompress_07:
			pDriver = new IT_10_faceCompress_07 (I_NUM_10_faceCompress_07);
			break;
		case I_NUM_10_faceCompress_08:
			pDriver = new IT_10_faceCompress_08 (I_NUM_10_faceCompress_08);
			break;
		case I_NUM_10_faceCompress_09:
			pDriver = new IT_10_faceCompress_09 (I_NUM_10_faceCompress_09);
			break;
		case I_NUM_10_faceCompress_10:
			pDriver = new IT_10_faceCompress_10 (I_NUM_10_faceCompress_10);
			break;
		case I_NUM_10_faceCompress_11:
			pDriver = new IT_10_faceCompress_11 (I_NUM_10_faceCompress_11);
			break;
		case I_NUM_10_faceCompress_12:
			pDriver = new IT_10_faceCompress_12 (I_NUM_10_faceCompress_12);
			break;
		case I_NUM_10_faceCompress_13:
			pDriver = new IT_10_faceCompress_13 (I_NUM_10_faceCompress_13);
			break;


			

		case I_NUM_11_faceSizeCtrl_01:
			pDriver = new IT_11_faceSizeCtrl_01 (I_NUM_11_faceSizeCtrl_01);
			break;
		case I_NUM_11_faceSizeCtrl_02:
			pDriver = new IT_11_faceSizeCtrl_02 (I_NUM_11_faceSizeCtrl_02);
			break;
		case I_NUM_11_faceSizeCtrl_03:
			pDriver = new IT_11_faceSizeCtrl_03 (I_NUM_11_faceSizeCtrl_03);
			break;
		case I_NUM_11_faceSizeCtrl_04:
			pDriver = new IT_11_faceSizeCtrl_04 (I_NUM_11_faceSizeCtrl_04);
			break;
		case I_NUM_11_faceSizeCtrl_05:
			pDriver = new IT_11_faceSizeCtrl_05 (I_NUM_11_faceSizeCtrl_05);
			break;
		case I_NUM_11_faceSizeCtrl_06:
			pDriver = new IT_11_faceSizeCtrl_06 (I_NUM_11_faceSizeCtrl_06);
			break;
		case I_NUM_11_faceSizeCtrl_07:
			pDriver = new IT_11_faceSizeCtrl_07 (I_NUM_11_faceSizeCtrl_07);
			break;
		case I_NUM_11_faceSizeCtrl_08:
			pDriver = new IT_11_faceSizeCtrl_08 (I_NUM_11_faceSizeCtrl_08);
			break;
		case I_NUM_11_faceSizeCtrl_09:
			pDriver = new IT_11_faceSizeCtrl_09 (I_NUM_11_faceSizeCtrl_09);
			break;
		case I_NUM_11_faceSizeCtrl_10:
			pDriver = new IT_11_faceSizeCtrl_10 (I_NUM_11_faceSizeCtrl_10);
			break;
		case I_NUM_11_faceSizeCtrl_11:
			pDriver = new IT_11_faceSizeCtrl_11 (I_NUM_11_faceSizeCtrl_11);
			break;
		case I_NUM_11_faceSizeCtrl_12:
			pDriver = new IT_11_faceSizeCtrl_12 (I_NUM_11_faceSizeCtrl_12);
			break;
		case I_NUM_11_faceSizeCtrl_13:
			pDriver = new IT_11_faceSizeCtrl_13 (I_NUM_11_faceSizeCtrl_13);
			break;



		case I_NUM_12_ModleShow_01:
			pDriver = new IT_12_ModleShow_01 (I_NUM_12_ModleShow_01);
			break;
		case I_NUM_12_ModleShow_02:
			pDriver = new IT_12_ModleShow_02 (I_NUM_12_ModleShow_02);
			break;
		case I_NUM_12_ModleShow_03:
			pDriver = new IT_12_ModleShow_03 (I_NUM_12_ModleShow_03);
			break;
		case I_NUM_12_ModleShow_04:
			pDriver = new IT_12_ModleShow_04 (I_NUM_12_ModleShow_04);
			break;
		case I_NUM_12_ModleShow_05:
			pDriver = new IT_12_ModleShow_05 (I_NUM_12_ModleShow_05);
			break;
		case I_NUM_12_ModleShow_06:
			pDriver = new IT_12_ModleShow_06 (I_NUM_12_ModleShow_06);
			break;
		case I_NUM_12_ModleShow_07:
			pDriver = new IT_12_ModleShow_07 (I_NUM_12_ModleShow_07);
			break;
		case I_NUM_12_ModleShow_08:
			pDriver = new IT_12_ModleShow_08 (I_NUM_12_ModleShow_08);
			break;
		case I_NUM_12_ModleShow_09:
			pDriver = new IT_12_ModleShow_09 (I_NUM_12_ModleShow_09);
			break;
		case I_NUM_12_ModleShow_10:
			pDriver = new IT_12_ModleShow_10 (I_NUM_12_ModleShow_10);
			break;
		case I_NUM_12_ModleShow_11:
			pDriver = new IT_12_ModleShow_11 (I_NUM_12_ModleShow_11);
			break;

		case I_NUM_13_Upgrade_01:
			pDriver = new IT_13_Upgrade_01 (I_NUM_13_Upgrade_01);
			break;
		case I_NUM_13_Upgrade_02:
			pDriver = new IT_13_Upgrade_02 (I_NUM_13_Upgrade_02);
			break;
		case I_NUM_13_Upgrade_03:
			pDriver = new IT_13_Upgrade_03 (I_NUM_13_Upgrade_03);
			break;

		
			

		case I_NUM_14_CountPeople_01:
			pDriver = new IT_14_CountPeople_01 (I_NUM_14_CountPeople_01);
			break;
		case I_NUM_14_CountPeople_02:
			pDriver = new IT_14_CountPeople_02 (I_NUM_14_CountPeople_02);
			break;
		case I_NUM_14_CountPeople_03:
			pDriver = new IT_14_CountPeople_03 (I_NUM_14_CountPeople_03);
			break;
		case I_NUM_14_CountPeople_04:
			pDriver = new IT_14_CountPeople_04 (I_NUM_14_CountPeople_04);
			break;
		case I_NUM_14_CountPeople_05:
			pDriver = new IT_14_CountPeople_05 (I_NUM_14_CountPeople_05);
			break;
		case I_NUM_14_CountPeople_06:
			pDriver = new IT_14_CountPeople_06 (I_NUM_14_CountPeople_06);
			break;
		case I_NUM_14_CountPeople_07:
			pDriver = new IT_14_CountPeople_07 (I_NUM_14_CountPeople_07);
			break;
		case I_NUM_14_CountPeople_08:
			pDriver = new IT_14_CountPeople_08 (I_NUM_14_CountPeople_08);
			break;
		case I_NUM_14_CountPeople_09:
			pDriver = new IT_14_CountPeople_09 (I_NUM_14_CountPeople_09);
			break;
		case I_NUM_14_CountPeople_10:
			pDriver = new IT_14_CountPeople_10 (I_NUM_14_CountPeople_10);
			break;
		case I_NUM_14_CountPeople_11:
			pDriver = new IT_14_CountPeople_11 (I_NUM_14_CountPeople_11);
			break;


		case I_NUM_15_UpgradeFunction_01:
			pDriver = new IT_15_UpgradeFunction_01 (I_NUM_15_UpgradeFunction_01);
			break;
		case I_NUM_15_UpgradeFunction_02:
			pDriver = new IT_15_UpgradeFunction_02 (I_NUM_15_UpgradeFunction_02);
			break;
		case I_NUM_15_UpgradeFunction_03:
			pDriver = new IT_15_UpgradeFunction_03 (I_NUM_15_UpgradeFunction_03);
			break;
		case I_NUM_15_UpgradeFunction_04:
			pDriver = new IT_15_UpgradeFunction_04 (I_NUM_15_UpgradeFunction_04);
			break;
		case I_NUM_15_UpgradeFunction_05:
			pDriver = new IT_15_UpgradeFunction_05 (I_NUM_15_UpgradeFunction_05);
			break;
		case I_NUM_15_UpgradeFunction_06:
			pDriver = new IT_15_UpgradeFunction_06 (I_NUM_15_UpgradeFunction_06);
			break;
		case I_NUM_15_UpgradeFunction_07:
			pDriver = new IT_15_UpgradeFunction_07 (I_NUM_15_UpgradeFunction_07);
			break;
		case I_NUM_15_UpgradeFunction_08:
			pDriver = new IT_15_UpgradeFunction_08 (I_NUM_15_UpgradeFunction_08);
			break;
		case I_NUM_15_UpgradeFunction_09:
			pDriver = new IT_15_UpgradeFunction_09 (I_NUM_15_UpgradeFunction_09);
			break;
		case I_NUM_15_UpgradeFunction_10:
			pDriver = new IT_15_UpgradeFunction_10 (I_NUM_15_UpgradeFunction_10);
			break;
		case I_NUM_15_UpgradeFunction_11:
			pDriver = new IT_15_UpgradeFunction_11 (I_NUM_15_UpgradeFunction_11);
			break;
		case I_NUM_15_UpgradeFunction_12:
			pDriver = new IT_15_UpgradeFunction_12 (I_NUM_15_UpgradeFunction_12);
			break;
		case I_NUM_15_UpgradeFunction_13:
			pDriver = new IT_15_UpgradeFunction_13 (I_NUM_15_UpgradeFunction_13);
			break;
		
		default:
			break;
	};

	return pDriver;
}

TestDriverFactory::~TestDriverFactory()
{
	
}

