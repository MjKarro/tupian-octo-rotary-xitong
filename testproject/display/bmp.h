#ifndef __BMP_H__
#define __BMP_H__






//�ļ�ͷ�ṹ��
typedef struct 
{
	unsigned short	bfType;	 //�ļ�����
	unsigned long	bfSize;	 //λͼ��С
	unsigned short bfReserved1;  //λ0 
	unsigned short bfReserved2;  //λ0���� 
	unsigned long	bfOffBits;	//������ƫ����
} __attribute__((packed))  BitMapFileHeader; 


 //��Ϣͷ�ṹ��
 typedef struct 
 { 
	unsigned long  biSize;
	long			biWidth; 
	long 			biHeight;
	unsigned short biPlanes; 
	unsigned short biBitCount; 
	unsigned long 	biCompression;
	unsigned long 	biSizeImage; 
	long 			biXPelsPerMeter;
	long 			biYPelsPerMeter;
	unsigned long 	biClrUsed;
	unsigned long 	biClrImportant;
 } __attribute__((packed)) BitMapInfoHeader; 
 
#endif


















