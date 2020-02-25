/********************************************************************
* Name      :  DrawRect.cpp                                  
* Describ   :  the defination of the class
* Author    :  Zhang Botong                                          
* Date      :  2018-04-11                                                 
********************************************************************/

#include <stdlib.h>
#include "DrawRect.h"
#include <stdio.h>

DrawRect::DrawRect():m_lineY(144),
					 m_lineU(53),
					 m_lineV(34),
					 m_imgType(YUV420SP),
					 m_imgWidth( 1920 ),
					 m_imgHeight( 1080)
{
}

int DrawRect::drawRect( unsigned char *input_image,
						FD_RGN_CROP_UNIT *pcrop_unit,
						unsigned short face_num,
						unsigned char color )
{
	if( ( NULL == input_image ) || ( NULL == pcrop_unit ) )
	{
		return -1;
	}


	for( int nCnt = 0; nCnt < face_num; nCnt++ )
	{
		drawRect( input_image, pcrop_unit[nCnt] );

		
		FD_RGN_CROP_UNIT rect;
		rect.crop_start_x = ( ( 0  ==  pcrop_unit[nCnt].crop_start_x ) ? \
							  0 : (pcrop_unit[nCnt].crop_start_x - 1 ) );
		rect.crop_start_y = ( ( 0  ==  pcrop_unit[nCnt].crop_start_y ) ?	\
							  0 : (pcrop_unit[nCnt].crop_start_y - 1 ) );

		if( pcrop_unit[nCnt].crop_width  < (m_imgWidth - rect.crop_start_x) )
		{
			rect.crop_width = pcrop_unit[nCnt].crop_width + 1;
		}
		else
		{
			rect.crop_width =  m_imgWidth - rect.crop_start_x;
		}

		if( pcrop_unit[nCnt].crop_height  < (m_imgHeight - rect.crop_start_y) )
		{
			rect.crop_height = pcrop_unit[nCnt].crop_height + 1;
		}
		else
		{
			rect.crop_height =  m_imgHeight - rect.crop_start_y;
		}

		drawRect( input_image, pcrop_unit[nCnt] );



		rect.crop_start_x = pcrop_unit[nCnt].crop_start_x + 1;
		rect.crop_start_y = pcrop_unit[nCnt].crop_start_y + 1;
		rect.crop_width = pcrop_unit[nCnt].crop_width - 1;
		rect.crop_height = pcrop_unit[nCnt].crop_height - 1;

		drawRect( input_image, pcrop_unit[nCnt] );
	}


	return 0;
}

int DrawRect::drawRect( unsigned char *pImg, FD_RGN_CROP_UNIT &rect )
{
	if( NULL == pImg )
	{
		return -1;
	}
	int left = rect.crop_start_x;
	int right = left+ rect.crop_width;
	int top = rect.crop_start_y;
	int bottom = top + rect.crop_height;
	int img_size = m_imgWidth * m_imgHeight;

	for (int y = top; y < bottom; ++y)
	{
		 //当前像素点y分量的位置
		int p_y = y * m_imgWidth + left;
		//当前像素点v分量的位置
		int p_v = img_size + (y >> 1) * m_imgWidth + ( left& ~1) + 0;
		 //当前像素点u分量的位置
		int p_u = img_size + (y >> 1) * m_imgWidth + ( left& ~1) + 1;
		pImg[p_y] = m_lineY;
		pImg[p_v] = m_lineV;
		pImg[p_u] = m_lineU;
		 //当前像素点y分量的位置
		p_y = y * m_imgWidth + right;
		//当前像素点v分量的位置
		p_v = img_size + (y >> 1) * m_imgWidth + ( right& ~1) + 0;
		 //当前像素点u分量的位置
		p_u = img_size + (y >> 1) * m_imgWidth + ( right& ~1) + 1;
		pImg[p_y] = m_lineY;
		pImg[p_v] = m_lineV;
		pImg[p_u] = m_lineU;

	}
	for (int x = left; x < right; ++x)
	{
		//当前像素点y分量的位置
		int p_y = top * m_imgWidth + x;
		//当前像素点v分量的位置
		int p_v = img_size + (top >> 1) * m_imgWidth + ( x & ~1) + 0;
		 //当前像素点u分量的位置
		int p_u = img_size + (top >> 1) * m_imgWidth + ( x & ~1) + 1;
		
		pImg[p_y] = m_lineY;
		pImg[p_v] = m_lineV;
		pImg[p_u] = m_lineU;


			//当前像素点y分量的位置
		p_y = bottom * m_imgWidth + x;
		//当前像素点v分量的位置
		p_v = img_size + (bottom >> 1) * m_imgWidth + ( x & ~1) + 0;
		 //当前像素点u分量的位置
		p_u = img_size + (bottom >> 1) * m_imgWidth + ( x & ~1) + 1;
		
		pImg[p_y] = m_lineY;
		pImg[p_v] = m_lineV;
		pImg[p_u] = m_lineU;
	}

	return 0;
}
  
int DrawRect::setLineColor( unsigned char nY, unsigned char nU, unsigned char nV )
{
	m_lineY = nY;
	m_lineU = nU;
	m_lineV = nV;

	return 0;
}

int DrawRect::setImageSize( unsigned int nWidth, unsigned int nHeight )
{
	m_imgWidth = nWidth;
	m_imgHeight = nHeight;

	return 0;
}

int DrawRect::setImageType( ImageType type )
{
	m_imgType = type;

	return 0;
}

DrawRect::~DrawRect()
{
}
