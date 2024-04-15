/*
*fb.c ����framebuffer�Ļ������룬����fb�Ĵ򿪣�ioctl�Ļ�ȡ��Ϣ
*�����Ĳ���fb��ʾ����
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

unsigned int *pfb = NULL;      //ָ��framebuffer�ڴ��׵�ַ
int fbfd=-1;					//��fb��õ���fd



int fb_open(void)
{
	struct fb_fix_screeninfo finfo;
	struct fb_var_screeninfo vinfo;
	
	// 1���豸
	int ret=-1;
	
	fbfd=open(FBDEVICE,O_RDWR);
	if(fbfd<0)
	{
		perror("open error");
		return -1;
	}
	debug("open %s success.\n",FBDEVICE);
	
	// 2��ȡ�豸��Ϣ
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
#if 0

//����1024*600�ֱ��ʵ�ͼƬ
void fb_draw_picture(void)
{
	const unsigned char *pData= gImage_1024_600;
	unsigned int *p=pfb;

	unsigned int i,j;
	unsigned int cnt;
	
	
	for(i=0;i<HEIGHT;i++)
		{
			for (j=0;j<WIDTH;j++)
				{
					cnt =WIDTH * i + j;  	//	��ǰ���ص�����λ��
					cnt *=3;	 			//	��ǰ���ص����������±�
										//	��ǰ���ض�Ӧ�����ݵ�RGBΪ
										//	pData[cnt+0] 		pData[cnt+1]		pData[cnt+2]
					//	��ǰ���ص������
					*p=	((pData[cnt+0]<<16) |(pData[cnt+1]<<8)|(pData[cnt+2]<<0));
					p++;
			}	
	}
}

void fb_draw_picture2(void)
{
	const unsigned char *pData= gImage_1024_600;

	unsigned int x,y;
	unsigned int cnt;
	
	
	for(y=0;y<HEIGHT;y++)
		{
			for (x=0;x<WIDTH;x++)
				{
					cnt = WIDTH * y + x;
					//ָ��ָ�����ص� ���ص��3��ʾһ�����ɺ��������
					*(pfb+cnt)=	((pData[3*cnt+0]<<16) |(pData[3*cnt+1]<<8)|(pData[3*cnt+2]<<0));
					
			}	
	}
}

void fb_draw_picture3(void)
{
	const unsigned char *pData= gImage_1024_600;

	unsigned int x,y;
	unsigned int cnt;
	unsigned int a=0;
	
	
	for(y=0;y<HEIGHT;y++)
		{
			for (x=0;x<WIDTH;x++)
				{
					cnt = WIDTH * y + x;
					//*(pfb+cnt)=	((pData[3*cnt+0]<<16) |(pData[3*cnt+1]<<8)|(pData[3*cnt+2]<<0));
					*(pfb+cnt)=	((pData[a+0]<<16) |(pData[a+1]<<8)|(pData[a+2]<<0));
					a+=3; //���ص����3λ
					
			}	
	}
}

void fb_draw_picture4(void)
{
	const unsigned char *pData= gImage_640360;

	unsigned int x,y;
	unsigned int cnt;
	unsigned int a=0;
	
	
	for(y=0;y<360;y++)
		{
			for (x=0;x<640;x++)
				{
					cnt = 1024 * y + x;
					//*(pfb+cnt)=	((pData[3*cnt+0]<<16) |(pData[3*cnt+1]<<8)|(pData[3*cnt+2]<<0));
					*(pfb+cnt)=	((pData[a+0]<<16) |(pData[a+1]<<8)|(pData[a+2]<<0));
					a+=3; //���ص����3λ
					
			}	
	}
}

//���������ʾͼƬ
void fb_draw_picture5(unsigned int x0, unsigned int y0)
{
	const unsigned char *pData= gImage_640360;

	unsigned int x,y;
	unsigned int cnt,cnt2;
	
	
	
	for(y=y0;y<y0+360;y++)
		{
			for (x=x0;x<x0+640;x++)
				{
					cnt = 1024 * y + x;
					cnt2=(y - y0) * 640+(x - x0);
					//*(pfb+cnt)=	((pData[3*cnt+0]<<16) |(pData[3*cnt+1]<<8)|(pData[3*cnt+2]<<0));
					//��ֵ��fb�ڴ���ƫ����
					//��ֵ����ǰ���ص���ͼ�����������е��±�
					*(pfb+cnt)=	((pData[3*cnt2+0]<<16) |(pData[3*cnt2+1]<<8)|(pData[3*cnt2+2]<<0));
					
			}	
	}
}


void fb_draw_picture6(unsigned int x0,unsigned int y0)
{
	const unsigned char *pData= gImage_640360;

	unsigned int x,y;
	unsigned int cnt;
	unsigned int a=0;
	
	
	for(y=y0;y<y0+360;y++)
		{

			if(y >= HEIGHT)
				{
					break;//���һ��ֱ������
				}
			for (x=x0;x<x0+640;x++)
				{
				if(x >=WIDTH)
					{
						a+=3;
						continue;//������Ļ��Χ�ĵ㣬��ֵ��Ӧ�ĵ�����
	
					}
				
					cnt = WIDTH* y + x;
					*(pfb+cnt)=	((pData[a+0]<<16) |(pData[a+1]<<8)|(pData[a+2]<<0));
					a+=3; //���ص����3λ
					
				}	
		}
}


void fb_draw_picture7(unsigned int x0, unsigned int y0)
{
	const unsigned char *pData= gImage_640360;

	unsigned int x,y;
	unsigned int cnt,cnt2;
	
	
	
	for(y=y0;y<y0+360;y++)
		{
			if(y>=HEIGHT)
				{
					break;
				}
			for (x=x0;x<x0+640;x++)
				{
					if(x >=WIDTH)
						{
							continue;
						}
					cnt = 1024 * y + x;
					cnt2=(y - y0) * 640+(x - x0);
					//*(pfb+cnt)=	((pData[3*cnt+0]<<16) |(pData[3*cnt+1]<<8)|(pData[3*cnt+2]<<0));
					//��ֵ��fb�ڴ���ƫ����
					//��ֵ����ǰ���ص���ͼ�����������е��±�
					*(pfb+cnt)=	((pData[3*cnt2+0]<<16) |(pData[3*cnt2+1]<<8)|(pData[3*cnt2+2]<<0));
					
			}	
	}
}

#endif

void fb_draw(const struct picture_info *pPic)
{
	char *pData= pPic->pData;  //ָ��rgb_bufͼ������

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

			for (x=0;x<pPic->width;x++)// ���ɵݼ�x=0��ϴ���
			{

					cnt = WIDTH* y + pPic->width-x;
					*(pfb+cnt)=	((pData[a+2]<<16) |(pData[a+1]<<8)|(pData[a+0]<<0));
					a=a-3;
					
			}	
	}
}

void fb_draw2(const struct picture_info *pPic)
{
	char *pData= pPic->pData;  //ָ��rgb_bufͼ������

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

			for (x=0;x<pPic->width;x++)// ���ɵݼ�x=0��ϴ���
			{

					cnt = WIDTH* y + pPic->width-x;
					*(pfb+cnt)=	((pData[a+0]<<16) |(pData[a+1]<<8)|(pData[a+2]<<0));
					a=a+3;
					
			}	
	}
}
void fb_draw3(const struct picture_info *pPic)
{
	char *pData= pPic->pData;  //ָ��rgb_bufͼ������

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

			for (x=0;x<pPic->width;x++)// ���ɵݼ�x=0��ϴ���
			{

					cnt = WIDTH* y + x;
					*(pfb+cnt)=	((pData[a+2]<<16) |(pData[a+1]<<8)|(pData[a+0]<<0));
					a=a+3;
					
			}	
	}
}
/*
void fb_draw2(const unsigned char * buf)
{
	const unsigned char *pData= buf;

	unsigned int x,y;
	unsigned int cnt=0 ,a=0;
	
	a=320*320*3-3;
	
	for(y=0;y<320;y++)
	{
			for (x=0;x<320;x++)
			{
					cnt = 1024 * y + 320-x;
					
					//*(pfb+cnt)=	((pData[3*cnt+0]<<16) |(pData[3*cnt+1]<<8)|(pData[3*cnt+2]<<0));
					*(pfb+cnt)=	((pData[a+2]<<16) |(pData[a+1]<<8)|(pData[a+0]<<0));
					a=a-3;
					
			}	
	}
}
*/




































