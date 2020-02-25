/********************************************************************
* Name      :  HaisiEcode.cpp                                  
* Param     :                                                        
* Return    :                                                        
* Describ   : to recode heat beat                                                       
* Author    :  liuxin                                        
* Date      :  2018-06-25                                                 
********************************************************************/
#include <stdio.h>
#include "HaisiEcode.h"
#include "sample_comm.h"
#include <pthread.h>


HaisiEcode* HaisiEcode::m_this =  nullptr;


HaisiEcode* HaisiEcode::getInstance()
{
	if( nullptr == m_this )
	{
		m_this = new HaisiEcode;
	}

	return m_this;
}


int HaisiEcode::initBaseHaisiEncode(int width,int height,VENC_CHN VencChn)
{
	m_height = height;
	m_width = width;
	m_vencChn = VencChn;
    VB_CONF_S stVbConf;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;


    memset(&stVbConf,0,sizeof(VB_CONF_S));

    stVbConf.u32MaxPoolCnt = 128;
	//>==================SAMPLE_COMM_SYS_CalcPicVbBlkSize====================================
	HI_U32 u32VbSize;
	HI_U32 u32HeaderSize;

	u32VbSize = (CEILING_2_POWER(m_width, SAMPLE_SYS_ALIGN_WIDTH) * \
				 CEILING_2_POWER(m_height, SAMPLE_SYS_ALIGN_WIDTH) * \
				 ((PIXEL_FORMAT_YUV_SEMIPLANAR_422 == SAMPLE_PIXEL_FORMAT) ? 2 : 1.5));

	VB_PIC_HEADER_SIZE(m_width, m_height, SAMPLE_PIXEL_FORMAT, u32HeaderSize);
	u32VbSize += u32HeaderSize;
	u32BlkSize = u32VbSize;
	//<=======================================================

	stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
	stVbConf.astCommPool[0].u32BlkCnt = 20;

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        SAMPLE_COMM_SYS_Exit();
		return -1;
    }


	m_pstFrame = (VIDEO_FRAME_INFO_S *)malloc(sizeof(VIDEO_FRAME_INFO_S));	//初始化发送的块空间
	//printf("m_pstFrame = %x,\n",m_pstFrame);

	

	return 0;
}


int HaisiEcode::uninitHaisiEncode(VENC_CHN VencChn)
{
	SAMPLE_COMM_VENC_Stop(VencChn);
	SAMPLE_COMM_SYS_Exit();
	if(NULL != m_pstFrame)
	{
		free(m_pstFrame);
		m_pstFrame = NULL;
	}
}


int HaisiEcode:: initChnVENC(int width, int height,VENC_CHN VencChn)
{
	
	//>-----------------------SAMPLE_COMM_VENC_Start-----------------------------------------------
		VENC_CHN_ATTR_S stVencChnAttr;
	
		VENC_ATTR_JPEG_S stJpegAttr;
	
		stVencChnAttr.stVeAttr.enType = PT_JPEG;
		switch (PT_JPEG)
		{
			case PT_JPEG:
				stJpegAttr.u32PicWidth	= (width>>2)<<2 ;
				stJpegAttr.u32PicHeight = (height>>2)<<2;
				stJpegAttr.u32MaxPicWidth  = (width>>2)<<2;
				stJpegAttr.u32MaxPicHeight = (height>>2)<<2;
				stJpegAttr.u32BufSize	= width * height * 2;
				stJpegAttr.bByFrame 	= HI_TRUE;/*get stream mode is field mode  or frame mode*/
				stJpegAttr.bSupportDCF	= HI_FALSE;
				memcpy(&stVencChnAttr.stVeAttr.stAttrJpeg, &stJpegAttr, sizeof(VENC_ATTR_JPEG_S));
				break;
			default:
				{
					SAMPLE_COMM_VENC_Stop(VencChn);
					SAMPLE_COMM_SYS_Exit();
					return -1;
				}
		}
	
		int s32Ret = HI_MPI_VENC_CreateChn(VencChn, &stVencChnAttr);
		if (HI_SUCCESS != s32Ret)
		{
			SAMPLE_PRT("HI_MPI_VENC_CreateChn [%d] faild with %#x!\n", \
					   VencChn, s32Ret);
			SAMPLE_COMM_VENC_Stop(VencChn);
			SAMPLE_COMM_SYS_Exit();
			return -1;
		}
	
		s32Ret = HI_MPI_VENC_StartRecvPic(VencChn);
		if (HI_SUCCESS != s32Ret)
		{
			SAMPLE_PRT("HI_MPI_VENC_StartRecvPic faild with%#x!\n", s32Ret);
			SAMPLE_COMM_VENC_Stop(VencChn);
			SAMPLE_COMM_SYS_Exit();
			return -1;
		}

	//<----------------------------------------------------------------------
	
	return 0;
}




int HaisiEcode::initHaisiEcode(void *priData )
{

	if( nullptr == priData )
	{
		printf("ERR: %d %s priData=%P\n", __LINE__, __func__, priData );
		
		return -1;
	}
	
	
	return 0;

}



HI_S32 HaisiEcode::savePicData(char * saveDate, VENC_STREAM_S* pstStream, int *outLen)
{
	VENC_PACK_S*  pstData;
    HI_U32 i;
	int len = 0;
    for (i = 0; i < pstStream->u32PackCount; i++)
    {
    	//printf("pstStream->u32PackCount = %d,pstData->u32Len = %d,pstData->u32Offset = %d,len = %d\n",
				//pstStream->u32PackCount,pstData->u32Len,pstData->u32Offset,len);
        pstData = &pstStream->pstPack[i];
        memcpy(saveDate + len, pstData->pu8Addr + pstData->u32Offset, pstData->u32Len - pstData->u32Offset);
		len += pstData->u32Len - pstData->u32Offset;
		
    }
	//printf("len = %d\n",len);
	*outLen = len;

    return HI_SUCCESS;
}


int HaisiEcode::setCompressValue(int compressValue,VENC_CHN VencChn )
{
	VENC_PARAM_JPEG_S  stParamJpeg;
	HI_S32 s32Ret = HI_MPI_VENC_GetJpegParam(VencChn, &stParamJpeg);
	if (HI_SUCCESS != s32Ret)
	{
		printf("HI_MPI_VENC_GetJpegParam err 0x%x\n",s32Ret);
		return HI_FAILURE;
	}
	//设置JPEG参数
	stParamJpeg.u32Qfactor = compressValue;
	s32Ret = HI_MPI_VENC_SetJpegParam(VencChn, &stParamJpeg);
	if (HI_SUCCESS != s32Ret)
	{
		printf("HI_MPI_VENC_SetJpegParam err 0x%x\n",s32Ret);
		return -1;
	}

 	return 0;
}

int HaisiEcode::HSStopGetStream()
{
	return SAMPLE_COMM_VENC_StopGetStream();
}

int HaisiEcode::HSVENCStop(VENC_CHN VencChn)
{
	return SAMPLE_COMM_VENC_Stop(VencChn);
}

int  HaisiEcode::HSSYS_Exit()
{
	SAMPLE_COMM_SYS_Exit();
	return 0;
}

int HaisiEcode::initSendFrame(int width, int height, VENC_CHN VencChn)
{
	HI_U32 phyYaddr;
	
	

	/* 分配物理buffer并且映射到用户空间 */
	do
	{
	   m_handleY = HI_MPI_VB_GetBlock(VB_INVALID_POOLID, width * height * 3 / 2, NULL);
	}
	while (VB_INVALID_HANDLE == m_handleY);   
	if( m_handleY == VB_INVALID_HANDLE)
	{
	   printf("getblock for y failed\n");
	   return -1;
	}

	VB_POOL poolID =  HI_MPI_VB_Handle2PoolId (m_handleY);

	phyYaddr = HI_MPI_VB_Handle2PhysAddr(m_handleY);
	if( phyYaddr == 0)
	{
	   printf("HI_MPI_VB_Handle2PhysAddr for m_handleY failed\n");
	   return -1;
	}

	m_pVirYaddr = (HI_U32*) HI_MPI_SYS_Mmap(phyYaddr, width * height * 3 / 2);

	/*--- 图像帧结构初始化， 填充数据----- */
	memset(&(m_pstFrame->stVFrame),0x00,sizeof(VIDEO_FRAME_S));

	m_pstFrame->stVFrame.u32Width = width;
	m_pstFrame->stVFrame.u32Height = height;
	m_pstFrame->stVFrame.enPixelFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
	m_pstFrame->u32PoolId = poolID;
	m_pstFrame->stVFrame.u32PhyAddr[0] = phyYaddr;//物理地址开始处
	m_pstFrame->stVFrame.u32PhyAddr[1] = phyYaddr + width * height;// 物理地址尾部
	m_pstFrame->stVFrame.u32PhyAddr[2] = phyYaddr + width * height +  width * height / 4 ;

	m_pstFrame->stVFrame.pVirAddr[0] = m_pVirYaddr;
	m_pstFrame->stVFrame.pVirAddr[1] = m_pVirYaddr + width * height;
	m_pstFrame->stVFrame.pVirAddr[1] = m_pVirYaddr + width * height +  width * height /4;

	m_pstFrame->stVFrame.u32Stride[0] = width ;
	m_pstFrame->stVFrame.u32Stride[1] = width ;
	m_pstFrame->stVFrame.u32Stride[2] = width ;

	m_pstFrame->stVFrame.u32Field	   = VIDEO_FIELD_FRAME;/* Intelaced D1,otherwise VIDEO_FIELD_FRAME */

	m_pstFrame->stVFrame.enCompressMode = COMPRESS_MODE_NONE;
	m_pstFrame->stVFrame.enVideoFormat  = VIDEO_FORMAT_LINEAR;
	m_pstFrame->stVFrame.u64pts	 = 0;//frame * 40;
	m_pstFrame->stVFrame.u32TimeRef = 0;//0;frame * 2;
	   
	return 0;

}

//获取数据
int HaisiEcode::getOutStream(HI_U32 *outstream, VENC_CHN VencChn, int width, int height, int *outLen)
{
	struct timeval TimeoutVal;
    fd_set read_fds;
    
	VENC_STREAM_S stStream;
	VENC_CHN_STAT_S stStat;

	m_VencFd = HI_MPI_VENC_GetFd(VencChn);
    if (m_VencFd < 0)
    {
        SAMPLE_PRT("HI_MPI_VENC_GetFd failed with %#x!\n",m_VencFd);
        return -1;
    }

	FD_ZERO(&read_fds);
    FD_SET(m_VencFd, &read_fds);

    TimeoutVal.tv_sec  = 2;
    TimeoutVal.tv_usec = 0;
    int s32Ret = select(m_VencFd + 1, &read_fds, NULL, NULL, &TimeoutVal);
    if (s32Ret < 0)
    {
        SAMPLE_PRT("select failed!\n");
        return -1;
    }
    else if (s32Ret == 0)
    {
        SAMPLE_PRT("get venc stream time out, exit thread\n");
         return -1;
    }
    else
    {
        if (FD_ISSET(m_VencFd, &read_fds))
        {
			 //printf("select has data can read\n");
            /*******************************************************
             step 2.1 : query how many packs in one-frame stream.
            *******************************************************/
            memset(&stStream, 0, sizeof(stStream));
            s32Ret = HI_MPI_VENC_Query(VencChn, &stStat);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_PRT("HI_MPI_VENC_Query chn[%d] failed with %#x!\n", VencChn, s32Ret);
                 return -1;
            }
			
			/*******************************************************
			step 2.2 :suggest to check both u32CurPacks and u32LeftStreamFrames at the same time,for example:
			*******************************************************/
			if(0 == stStat.u32CurPacks)
			{
				  SAMPLE_PRT("NOTE: Current  frame is NULL!\n");
				   return -1;
			}
            /*******************************************************
             step 2.3 : malloc corresponding number of pack nodes.
            *******************************************************/
            stStream.pstPack = (VENC_PACK_S*)malloc(sizeof(VENC_PACK_S) * stStat.u32CurPacks);
            if (NULL == stStream.pstPack)
            {
                SAMPLE_PRT("malloc stream pack failed!\n");
                 return -1;
            }

            /*******************************************************
             step 2.4 : call mpi to get one-frame stream
            *******************************************************/
            stStream.u32PackCount = stStat.u32CurPacks;
            s32Ret = HI_MPI_VENC_GetStream(VencChn, &stStream, HI_TRUE);
            if (HI_SUCCESS != s32Ret)
            {
                free(stStream.pstPack);
                stStream.pstPack = NULL;
                SAMPLE_PRT("HI_MPI_VENC_GetStream failed with %#x!\n", \
                           s32Ret);
                 return -1;
            }
            //printf("HI_MPI_VENC_GetStream ok \n");
            /*******************************************************
             step 2.5 : save frame to file
            *******************************************************/
			//s32Ret = SAMPLE_COMM_VENC_SaveSnap(&stStream);
			savePicData((char * )outstream, &stStream, outLen);
            /*******************************************************
             step 2.6 : release stream
            *******************************************************/
            s32Ret = HI_MPI_VENC_ReleaseStream(VencChn, &stStream);
            if (HI_SUCCESS != s32Ret)
            {
                free(stStream.pstPack);
                stStream.pstPack = NULL;
                return -1;
            }
            /*******************************************************
             step 2.7 : free pack nodes
            *******************************************************/
            free(stStream.pstPack);
            stStream.pstPack = NULL;

			//printf("relse ok \n");
			/* 释放掉获取的vb物理地址和虚拟地址 */
		    HI_MPI_SYS_Munmap(m_pVirYaddr, width * height * 3 / 2);
            HI_MPI_VB_ReleaseBlock(m_handleY);
			m_handleY = VB_INVALID_HANDLE;
        }
    }
	return 0;
}

int HaisiEcode::setCropSize( int x, int y, int width, int height , int VencChn)
{
	HI_S32 ret = 0;
	VENC_CROP_CFG_S stCropCfg;
	ret =  HI_MPI_VENC_GetCrop(VencChn, &stCropCfg);
	if(ret != 0)
	{
		printf("ERR! ret = %x\n", ret);
		return -1;
	}
	//printf("stCropCfg.bEnable = %d\n",stCropCfg.bEnable);
	//printf("x = %d,y = %d, h = %d,w = %d \n",stCropCfg.stRect.s32X,stCropCfg.stRect.s32Y,stCropCfg.stRect.u32Height,stCropCfg.stRect.u32Width);

	stCropCfg.bEnable = HI_TRUE;
	stCropCfg.stRect.s32X = (x>>4)<<4;
	stCropCfg.stRect.s32Y = (y>>1)<<1;
	stCropCfg.stRect.u32Height = height;
	stCropCfg.stRect.u32Width = width;
	ret = HI_MPI_VENC_SetCrop(VencChn, &stCropCfg);
	//printf("stCropCfg.bEnable = %d\n",stCropCfg.bEnable);
	//printf("x = %d,y = %d, h = %d,w = %d \n",stCropCfg.stRect.s32X,stCropCfg.stRect.s32Y,stCropCfg.stRect.u32Height,stCropCfg.stRect.u32Width);
	if(ret != 0)
	{
		printf("ERR! ret = %x\n", ret);
		return -1;
	}

	m_pstFrame->stVFrame.s16OffsetLeft= height;//图像左侧剪裁宽度
	m_pstFrame->stVFrame.s16OffsetTop = width;//图像顶部剪裁宽度
	m_pstFrame->stVFrame.s16OffsetRight = height;//图像右侧剪裁宽度
	m_pstFrame->stVFrame.s16OffsetBottom = width;//图像底部剪裁宽度
	return 0;
}


int HaisiEcode::haisiEncodeVENC(int VencChn, int x,int y, int width, int height, HI_U32* instream, HI_U32 * outstream, int *outLen, int compress )
{
	HI_S32 s32Ret ;
	
	initSendFrame( width,  height,  VencChn);

	
	/*	从原始文件读取yuv420sp帧，然后调用给编码接口 */
	memcpy(m_pVirYaddr, instream, width * height * 3 / 2); //获取数据


	/*ERR >>>开始发送读到的数据到编码接口  ---->>>>*/
	int uMy32Ret = HI_MPI_VENC_SendFrame(VencChn, m_pstFrame, 50);
	//printf("[LX] uMy32Ret = %x, uMy32Ret = %d\n",uMy32Ret,uMy32Ret);
	if(HI_SUCCESS != uMy32Ret)
	{
		printf("HI_MPI_VENC_SendFrame failed! ERR_NO = %x\n",uMy32Ret);
		SAMPLE_COMM_VENC_Stop(VencChn);
		return -1;
	}    
    getOutStream(outstream, VencChn, width, height,outLen);

	
	SAMPLE_COMM_VENC_Stop(VencChn);
	//SAMPLE_COMM_SYS_Exit();
	//HI_MPI_VENC_DestroyChn(VencChn);
		

    return HI_SUCCESS;
}



int HaisiEcode::haiSiSendfream (VENC_CHN VeChn)
{
	int ret32 = HI_MPI_VENC_SendFrame(VeChn, m_pstFrame, 50);
	if ( ret32 != 0 )
	{
		return -1;
	}
	return 0;
}

/*
int HaisiEcode::test1(int flag)
{
	printf("test \n");
    VENC_CHN VencChn = 1;
    HI_S32 s32Ret = HI_SUCCESS;
	int width = 768;
  	int height = 576;
	int compressValue = 55;

	
	printf("init haisiCode ok\n");
	

    FILE *pFile;
	pFile = fopen("/dev/768_576.yuv","r+");
	if(pFile == NULL)
	{
		printf("open yuv420sp file failed\n");
		return -1;
	}
	printf("init fopen ok\n");
	int len = 768 * 576 * 1.5;
	char *picData = new  char[len];
	s32Ret = fread(picData,width * height * 3 / 2, 1, pFile);
	if(s32Ret < 0)
	{
		printf("fread yuv420sp failed\n");
		return -1;
	}
	printf("fread ok\n");

	int BGRlen = width * height * 3;
	char *picBGR = new char [BGRlen]; 
	HI_U32 *instream = (HI_U32 *)picData;
	HI_U32 *outstream = (HI_U32 *)picBGR;

	s32Ret = haisiEncodeVENC(VencChn,0, 0, width, height, instream, outstream, compressValue);
    if( s32Ret != HI_SUCCESS )
    {
        printf("Start Venc failed---!\n");
        return -1;
    }
	printf("SAMPLE_COMM_VENC_GetVencStreamProcEx ok\n");

		
	FILE* pFileW;

	char picName[40]  = {0};
    snprintf(picName, 40, "/dev/testPic_%d.jpg", flag);
	
	pFileW = fopen(picName, "wb");
	fwrite(outstream, BGRlen , 1, pFileW);

	delete []picData;
	delete []picBGR;
	fclose(pFile);
	fclose(pFileW);

	printf("test ok\n");
	
    return s32Ret;    
}

int HaisiEcode::test2(int flag)
{
	printf("test \n");
    VENC_CHN VencChn = 1;
    HI_S32 s32Ret = HI_SUCCESS;
	int width = 320;
  	int height = 240;
	int compressValue = 77;

	//s32Ret =   initChnVENC( width, height, VencChn);
	if(s32Ret != 0)
	{
		return -1;
	}
	printf("init haisiCode ok\n");
	

    FILE *pFile;
	pFile = fopen("/dev/320_240.yuv","r+");
	if(pFile == NULL)
	{
		printf("open yuv420sp file failed\n");
		return -1;
	}
	printf("init fopen ok\n");
	int len = width * height * 1.5;
	char *picData = new  char[len];
	s32Ret = fread(picData,width * height * 3 / 2, 1, pFile);
	if(s32Ret < 0)
	{
		printf("fread yuv420sp failed\n");
		return -1;
	}
	printf("fread ok\n");

	int BGRlen = width * height * 3;
	char *picBGR = new char [BGRlen]; 
	HI_U32 *instream = (HI_U32 *)picData;
	HI_U32 *outstream = (HI_U32 *)picBGR;

	s32Ret = haisiEncodeVENC(VencChn,0, 0, width, height, instream, outstream, compressValue);
    if( s32Ret != HI_SUCCESS )
    {
        printf("Start Venc failed---!\n");
        return -1;
    }
	printf("SAMPLE_COMM_VENC_GetVencStreamProcEx ok\n");

		
	FILE* pFileW;

	char picName[40]  = {0};
    snprintf(picName, 40, "/dev/testPic_%d.jpg", flag);
	
	pFileW = fopen(picName, "wb");
	fwrite(outstream, BGRlen , 1, pFileW);

	delete []picData;
	delete []picBGR;
	fclose(pFile);
	fclose(pFileW);

	printf("test ok\n");
	
    return s32Ret;    
}


int HaisiEcode::test()
{
	m_width = 768;
	m_height = 576;

	int width = 320;
  	int height = 240;

	int compress = 99;
	VENC_CHN VencChn  = 1;
	 
	int s32Ret =  initBaseHaisiEncode( 768, 576, VencChn);

	int value = 1;
	s32Ret =  initChnVENC( 300, 200, VencChn);//初始化通道
	setCompressValue(compress, VencChn);
	setCropSize( 300, 200, 300,  200 , VencChn);//截图
	
	
	int uMy32Ret = HI_MPI_VENC_SendFrame(VencChn, m_pstFrame, 50);
	if(s32Ret != 0)
	{
		return -1;
	}
	test1(value);
	printf("pic1 ok \n\n");


	value = 4;
	s32Ret =  initChnVENC( 200, 200, VencChn);//初始化通道
	setCompressValue(compress, VencChn);
	setCropSize( 300, 200, 200,  200, VencChn);////截图
	if(s32Ret != 0)
	{
		return -1;
	}
	test1(value);
	printf("pic2 ok \n\n");

	

	value = 2;
	initChnVENC( 320, 240, VencChn);
	setCompressValue(compress, VencChn);
	setCropSize( 100, 100, 50,  50 , VencChn);
	test2(value);
	printf("pic2 ok、\n\n");

	value = 3;
	s32Ret =  initChnVENC( 768, 576, VencChn);
	setCompressValue(compress, VencChn);
	setCropSize( 100, 100, 200,  200 , VencChn);
	if(s32Ret != 0)
	{
		return -1;
	}
	test1(value);
	printf("pic3 ok \n\n");

	
    return 0;    
}
*/
/*
int HaisiEcode::ecodeHaisiFun(int x, int y, int width, int height, int compress, int VencChn,HI_U32* instream, HI_U32 * outstream)
{


	int compress = 99;
	VENC_CHN VencChn  = 1;
	 
	//int s32Ret =  initBaseHaisiEncode( 768, 576, VencChn);

	int value = 1;
	s32Ret =  initChnVENC( width, height, VencChn);//初始化通道
	setCompressValue(compress, VencChn);
	setCropSize( x, y, width,  height , VencChn);//截图
	
	
	int uMy32Ret = HI_MPI_VENC_SendFrame(VencChn, m_pstFrame, 50);
	if(s32Ret != 0)
	{
		return -1;
	}
	test1(value);
	printf("pic1 ok \n\n");


    return 0;    
}
*/

HaisiEcode::~HaisiEcode()
{
	
	uninitHaisiEncode(m_vencChn);
}


HaisiEcode::HaisiEcode()
{
}


