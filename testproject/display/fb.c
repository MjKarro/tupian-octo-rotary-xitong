/*
*fb.c 操作framebuffer的基础代码，包括fb的打开，ioctl的获取信息
*基本的测试fb显示代码
*
*/

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <unistd.h>
#include <config.h>

#include <fb.h>

unsigned int *pfb = NULL;      //指向framebuffer内存首地址
int fbfd=-1;					//打开fb后得到的fd



int fb_open(void)
{
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
	
	// 1打开设备
	int ret=-1;
	
	fbfd=open(FBDEVICE,O_RDWR);
	if(fbfd<0)
	{
		perror("open error");
		return -1;
	}
	debug("open %s success.\n",FBDEVICE);
	
	// 2获取设备信息
	ret=ioctl(fbfd,FBIOGET_FSCREENINFO,&finfo);
	if(ret<0)
	{
		perror("ioctl");
		return -1;
	}
	debug("smem_start=0x%lx,smem_len=%d.\n",finfo.smem_start,finfo.smem_len);
	
	ret=ioctl(fbfd,FBIOGET_VSCREENINFO,&vinfo);
	if(ret<0)
	{
		perror("ioctl");
		return -1;
	}
	debug("xres=%d,yres=%d.\n",vinfo.xres,vinfo.yres);
	debug("xres_virtual=%d,yres_virtual=%d.\n",vinfo.xres_virtual,vinfo.yres_virtual);
	debug("bits_per_pixel=%d,grayscale=%d.\n",vinfo.bits_per_pixel,vinfo.grayscale);
	

	
	// 3mmap
	pfb=mmap(NULL,finfo.smem_len,PROT_READ|PROT_WRITE,MAP_SHARED,fbfd,0);
	if(NULL==pfb)
	{
		perror("mmap error");
		return -1;
	}
	debug("pfb=%p.\n",pfb);
	
	
	return 0;
}

void fb_close(void)
{
	close(fbfd);
}

void fb_draw_back(unsigned int width,unsigned int height,unsigned int color)
{
	unsigned int x,y;
	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			*(pfb+y*WIDTH+x)=color;
		}
	}
	
	
}
void fb_draw_line(unsigned int color)
{
	unsigned int x;
	for(x=50;x<600;x++)
	{
		*(pfb+200*WIDTH+x)=color;
	}
}

void fb_draw_bmp(const struct picture_info *pPic)
{
	char *pData= pPic->pData;  //指向rgb_buf图像数组

	unsigned int x,y;
	unsigned int cnt=0 ,a=0;
	debug("image resolution:%d * %d ,bpp is %d.\n",
		 pPic->width,pPic->height,pPic->bpp);

	if ((pPic->bpp!=32)&&(pPic->bpp!=24))
	{
		fprintf("bpp %d is not support.\n",pPic->bpp);
		return;
	}
	a=pPic->height*pPic->width*3;
	for(y=0;y<pPic->height;y++)
		{

			for (x=0;x<pPic->width;x++)// 换成递减x=0会断错误
			{

					cnt = WIDTH* y + pPic->width-x;
					*(pfb+cnt)=	((pData[a+2]<<16) |(pData[a+1]<<8)|(pData[a+0]<<0));
					a=a-3;
					
			}	
	}
}

void fb_draw_jpg(const struct picture_info *pPic)
{
	char *pData= pPic->pData;  //指向rgb_buf图像数组

	unsigned int x,y;
	unsigned int cnt=0 ,a=0;
	debug("image resolution:%d * %d ,bpp is %d.\n",
		 pPic->width,pPic->height,pPic->bpp);

	if ((pPic->bpp!=32)&&(pPic->bpp!=24))
	{
		fprintf("bpp %d is not support.\n",pPic->bpp);
		return;
	}
	for(y=0;y<pPic->height;y++)
		{

			for (x=0;x<pPic->width;x++)// 换成递减x=0会断错误
			{

					cnt = WIDTH* y + pPic->width-x;
					*(pfb+cnt)=	((pData[a+0]<<16) |(pData[a+1]<<8)|(pData[a+2]<<0));
					a=a+3;
					
			}	
	}
}
void fb_draw_png(const struct picture_info *pPic)
{
	char *pData= pPic->pData;  //指向rgb_buf图像数组

	unsigned int x,y;
	unsigned int cnt=0 ,a=0;
	debug("image resolution:%d * %d ,bpp is %d.\n",
		 pPic->width,pPic->height,pPic->bpp);

	if ((pPic->bpp!=32)&&(pPic->bpp!=24))
	{
		fprintf("bpp %d is not support.\n",pPic->bpp);
		return;
	}
	for(y=0;y<pPic->height;y++)
		{

			for (x=0;x<pPic->width;x++)// 换成递减x=0会断错误
			{

					cnt = WIDTH* y + x;
					*(pfb+cnt)=	((pData[a+2]<<16) |(pData[a+1]<<8)|(pData[a+0]<<0));
					a=a+3;
					
			}	
	}
}




































