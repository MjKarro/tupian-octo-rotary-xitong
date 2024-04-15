#ifndef __BMP_H__
#define __BMP_H__






//文件头结构体
typedef struct 
{
	unsigned short	bfType;	 //文件类型
	unsigned long	bfSize;	 //位图大小
	unsigned short bfReserved1;  //位0 
	unsigned short bfReserved2;  //位0　　 
	unsigned long	bfOffBits;	//到数据偏移量
} __attribute__((packed))  BitMapFileHeader; 


 //信息头结构体
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


















