/********************************************************************
* Name      :  TestDriverFactory.h                                  
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-07-02                                                 
********************************************************************/

#ifndef TESTDRIVERFACTORY_H
#define TESTDRIVERFACTORY_H

#include "Driver.h"

class TestDriverFactory
{
 public:
  typedef enum
  {
  	T_NUM_1_initDetectFaceIF = 1,
	T_NUM_2_registerCallback = 2,
	T_NUM_3_startSDK = 3,
	T_NUM_4_stoptSDK = 4,
	T_NUM_5_getClientSDKVersion = 5,
	T_NUM_6_getFaceDetectVersionInfo = 6,
	T_NUM_8_uninitDetectFaceIF = 8,
	T_NUM_9_addFrame = 9,
	T_NUM_10_getDetectConfig = 10,
	T_NUM_11_setDetectConfig= 11,
	T_NUM_12_addDetectRegion = 12,
	T_NUM_13_deletetDetectRegion = 13,
	T_NUM_14_addShieldRegion = 14,
	T_NUM_15_deletetShieldRegion = 15,
	T_NUM_16_softUpgrade = 16,
	T_NUM_18_setLine = 18,
	T_NUM_19_resetCounter = 19,

	
	I_NUM_01_HeatBeat_01 = 61,
	I_NUM_01_HeatBeat_02 = 62,
	I_NUM_01_HeatBeat_03 = 63,
	I_NUM_01_HeatBeat_04 = 64,
	I_NUM_01_HeatBeat_05 = 65,
	I_NUM_01_HeatBeat_06 = 66,
	I_NUM_01_HeatBeat_07 = 67,
	I_NUM_01_HeatBeat_08 = 68,

	I_NUM_02_DetectFace_01 = 71,
	I_NUM_02_DetectFace_02 = 72,
	I_NUM_02_DetectFace_03 = 73,
	I_NUM_02_DetectFace_04 = 74,
	I_NUM_02_DetectFace_05 = 75,

	I_NUM_03_FaceRegion_01 = 81,
	I_NUM_03_FaceRegion_02 = 82,
	I_NUM_03_FaceRegion_03 = 83,
	I_NUM_03_FaceRegion_04 = 84,
	I_NUM_03_FaceRegion_05 = 85,
	I_NUM_03_FaceRegion_06 = 86,
	I_NUM_03_FaceRegion_07 = 87,
	I_NUM_03_FaceRegion_08 = 88,
	I_NUM_03_FaceRegion_09 = 89,

	I_NUM_04_FaceSize_01 = 91,
	I_NUM_04_FaceSize_02 = 92,
	I_NUM_04_FaceSize_03 = 93,
	I_NUM_04_FaceSize_04 = 94,
	I_NUM_04_FaceSize_05 = 95,
	I_NUM_04_FaceSize_06 = 96,
	I_NUM_04_FaceSize_07 = 97,
	I_NUM_04_FaceSize_08 = 98,

	I_NUM_05_faceDetectCapture_01 = 101,
	I_NUM_05_faceDetectCapture_02 = 102,
	I_NUM_05_faceDetectCapture_03 = 103,
	I_NUM_05_faceDetectCapture_04 = 104,
	I_NUM_05_faceDetectCapture_05 = 105,
	I_NUM_05_faceDetectCapture_06 = 106,
	I_NUM_05_faceDetectCapture_07 = 107,
	I_NUM_05_faceDetectCapture_08 = 108,
	I_NUM_05_faceDetectCapture_09 = 109,

	I_NUM_06_CaptureDetectThreshold_01 = 111,
	I_NUM_06_CaptureDetectThreshold_02 = 112,
	I_NUM_06_CaptureDetectThreshold_03 = 113,
	I_NUM_06_CaptureDetectThreshold_04 = 114,
	I_NUM_06_CaptureDetectThreshold_05 = 115,
	I_NUM_06_CaptureDetectThreshold_06 = 116,
	I_NUM_06_CaptureDetectThreshold_07 = 117,
	I_NUM_06_CaptureDetectThreshold_08 = 118,
	I_NUM_06_CaptureDetectThreshold_09 = 119,
	I_NUM_06_CaptureDetectThreshold_10 = 120,

	I_NUM_07_BackgroundSwitchUpload_01 = 121,
	I_NUM_07_BackgroundSwitchUpload_02 = 122,
	I_NUM_07_BackgroundSwitchUpload_03 = 123,
	I_NUM_07_BackgroundSwitchUpload_04 = 124,
	I_NUM_07_BackgroundSwitchUpload_05 = 125,
	I_NUM_07_BackgroundSwitchUpload_06 = 126,
	I_NUM_07_BackgroundSwitchUpload_07 = 127,
	I_NUM_07_BackgroundSwitchUpload_08 = 128,
	I_NUM_07_BackgroundSwitchUpload_09 = 129,
	I_NUM_07_BackgroundSwitchUpload_10 = 130,

	I_NUM_08_FaceRectDraw_01 = 131,
	I_NUM_08_FaceRectDraw_02 = 132,
	I_NUM_08_FaceRectDraw_03 = 133,
	I_NUM_08_FaceRectDraw_04 = 134,
	I_NUM_08_FaceRectDraw_05 = 135,
	I_NUM_08_FaceRectDraw_06 = 136,
	I_NUM_08_FaceRectDraw_07 = 137,
	I_NUM_08_FaceRectDraw_08 = 138,
	I_NUM_08_FaceRectDraw_09 = 139,
	I_NUM_08_FaceRectDraw_10 = 140,
	
	I_NUM_09_BackPicCompress_01 = 141,
	I_NUM_09_BackPicCompress_02 = 142,
	I_NUM_09_BackPicCompress_03 = 143,
	I_NUM_09_BackPicCompress_04 = 144,
	I_NUM_09_BackPicCompress_05 = 145,
	I_NUM_09_BackPicCompress_06 = 146,
	I_NUM_09_BackPicCompress_07 = 147,
	I_NUM_09_BackPicCompress_08 = 148,
	I_NUM_09_BackPicCompress_09 = 149,
	I_NUM_09_BackPicCompress_10 = 150,
	I_NUM_09_BackPicCompress_11 = 151,
	I_NUM_09_BackPicCompress_12 = 152,


	I_NUM_10_faceCompress_01 = 161,
	I_NUM_10_faceCompress_02 = 162,
	I_NUM_10_faceCompress_03 = 163,
	I_NUM_10_faceCompress_04 = 164,
	I_NUM_10_faceCompress_05 = 165,
	I_NUM_10_faceCompress_06 = 166,
	I_NUM_10_faceCompress_07 = 167,
	I_NUM_10_faceCompress_08 = 168,
	I_NUM_10_faceCompress_09 = 169,
	I_NUM_10_faceCompress_10 = 170,
	I_NUM_10_faceCompress_11 = 171,
	I_NUM_10_faceCompress_12 = 172,
	I_NUM_10_faceCompress_13 = 173,


	I_NUM_11_faceSizeCtrl_01 = 175,
	I_NUM_11_faceSizeCtrl_02 = 176,
	I_NUM_11_faceSizeCtrl_03 = 177,
	I_NUM_11_faceSizeCtrl_04 = 178,
	I_NUM_11_faceSizeCtrl_05 = 179,
	I_NUM_11_faceSizeCtrl_06 = 180,
	I_NUM_11_faceSizeCtrl_07 = 181,
	I_NUM_11_faceSizeCtrl_08 = 182,
	I_NUM_11_faceSizeCtrl_09 = 183,
	I_NUM_11_faceSizeCtrl_10 = 184,
	I_NUM_11_faceSizeCtrl_11 = 185,
	I_NUM_11_faceSizeCtrl_12 = 186,
	I_NUM_11_faceSizeCtrl_13 = 187,

	I_NUM_12_ModleShow_01 = 191,
	I_NUM_12_ModleShow_02 = 192,
	I_NUM_12_ModleShow_03 = 193,
	I_NUM_12_ModleShow_04 = 194,
	I_NUM_12_ModleShow_05 = 195,
	I_NUM_12_ModleShow_06 = 196,
	I_NUM_12_ModleShow_07 = 197,
	I_NUM_12_ModleShow_08 = 198,
	I_NUM_12_ModleShow_09 = 199,
	I_NUM_12_ModleShow_10 = 200,
	I_NUM_12_ModleShow_11 = 201,
	
	
	I_NUM_13_Upgrade_01 = 210,
	I_NUM_13_Upgrade_02 = 211,
	I_NUM_13_Upgrade_03 = 213,

	
	I_NUM_14_CountPeople_01 = 220,
	I_NUM_14_CountPeople_02 = 222,
	I_NUM_14_CountPeople_03 = 223,
	I_NUM_14_CountPeople_04 = 224,
	I_NUM_14_CountPeople_05 = 225,
	I_NUM_14_CountPeople_06 = 226,
	I_NUM_14_CountPeople_07 = 227,
	I_NUM_14_CountPeople_08 = 228,
	I_NUM_14_CountPeople_09 = 229,
	I_NUM_14_CountPeople_10 = 230,
	I_NUM_14_CountPeople_11 = 231,

	I_NUM_15_UpgradeFunction_01 = 251,
	I_NUM_15_UpgradeFunction_02 = 252,
	I_NUM_15_UpgradeFunction_03 = 253,
	I_NUM_15_UpgradeFunction_04 = 254,
	I_NUM_15_UpgradeFunction_05 = 255,
	I_NUM_15_UpgradeFunction_06 = 256,
	I_NUM_15_UpgradeFunction_07 = 257,
	I_NUM_15_UpgradeFunction_08 = 258,
	I_NUM_15_UpgradeFunction_09 = 259,
	I_NUM_15_UpgradeFunction_10 = 260,
	I_NUM_15_UpgradeFunction_11 = 261,
	I_NUM_15_UpgradeFunction_12 = 262,
	I_NUM_15_UpgradeFunction_13 = 263,

	
	Case_Sample = 888,
	Case02_10_01 = 1001,
	Case02_10_05 = 1002,
	Case02_10_16 = 1003,
	Case02_10_17 = 1004,
	Case02_10_18 = 1005,
	Case02_10_32 = 1006,
	Case02_10_33 = 1007,
	Case02_10_37 = 1008,
	Case02_10_39 = 1009,
	Case02_10_59 = 1010,
	Case02_10_62 = 1011,
	Case02_10_79 = 1012,
	Case02_10_115 = 1013,
	Case02_10_136 = 1014,
	Case02_10_156 = 1015,
	Case02_10_160 = 1016,
	Case02_10_170 = 1017,
	Case02_10_180 = 1018,
	CaseTestCommunicate=1099,
	CaseIDMax
  }CaseID;
  
  TestDriverFactory();
  static Driver * createDriver( CaseID nCaseId );
  virtual ~TestDriverFactory();
 protected:
 private:
};

#endif
