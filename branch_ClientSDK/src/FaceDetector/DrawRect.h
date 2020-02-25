/********************************************************************
* Name      :  DrawRect.h                                  
* Param     :                                                        
* Return    :                                                        
* Describ   :                                                        
* Author    :  Zhang Botong                                          
* Date      :  2018-04-11                                                 
********************************************************************/

#ifndef DRAWRECT_H
#define DRAWRECT_H

typedef struct _FD_RGN_CROP_UNIT
{	
	int crop_start_x;
	int crop_start_y;
	int crop_width;
	int crop_height;
	unsigned char type;
	
}FD_RGN_CROP_UNIT;	

class DrawRect
{
 public:
  typedef enum
  {
	YUV420SP,
	YUV420,
	BGR888
  }ImageType;
  
  DrawRect();

  int drawRect( unsigned char *input_image,
				FD_RGN_CROP_UNIT *pcrop_unit,
				unsigned short face_num,
				unsigned char color );

  int drawRect( unsigned char *pImg, FD_RGN_CROP_UNIT &rect );
  
  int setLineColor( unsigned char nY, unsigned char nU, unsigned char nV );

  int setImageSize( unsigned int nWidth, unsigned int nHeight );

  int setImageType( ImageType type );
  
  virtual ~DrawRect();

 protected:

 private:
  unsigned char m_lineY;
  unsigned char m_lineU;
  unsigned char m_lineV;
  ImageType m_imgType;
  unsigned int m_imgWidth;
  unsigned int m_imgHeight;
};
#endif
