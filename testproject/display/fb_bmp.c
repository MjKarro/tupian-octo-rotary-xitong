//解析bmp格式图片并在fb显示
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <fb.h>
#include "bmp.h"
#include <config.h>



//判断是否是bmp格式
//是返回0，否-1
int is_bmp(const char *path)
{
	int fd =-1;
	unsigned char buf[2]={0};//bmp头文件大小
	ssize_t ret=0;
	

	//第一步 打开bmp 格式图片
	fd=open(path,O_RDONLY);
	if(fd<0)
	{
			fprintf(stderr,"open %s error .\n",path);
			return -1;
	}

	
	//第二步 读取头文件 
	ret =read( fd, buf, 2);
	if(ret!=2)
	{
		fprintf(stderr,"read file header error .\n");
		close(fd);
		return -1;
	}

	//第三 步解析头文件判断是否是bmp图片
	if(buf[0] != 'B' || buf[1] != 'M')
	{
		fprintf(stderr,"file %s is not bmp picture .\n",path);
		return -1;		

	}

	close(fd);
	return 0;	
	
	
}



//path =路径+文件名
//解析path的bmp图片，并且将解析的数据丢到bmp_buf中
//返回值错误返回-1,正确返回0
 static int bmp_analyze(struct picture_info *pPic)
{
	int fd =-1;
	BitMapFileHeader fHeader;
	BitMapInfoHeader info;
	unsigned long len;
	//unsigned short temp;

	//第一步 打开bmp 格式图片
	fd=open(pPic->pathname,O_RDONLY);
	if(fd<0)
	{
			fprintf(stderr,"open %s error .\n",pPic->pathname);
			return -1;
	}

	
	//第二步 读取头文件 
	//read(fd, &temp, sizeof(temp));
	read (fd,&fHeader,sizeof(fHeader));
	debug("bfSize=%ld.\n",fHeader.bfSize);
	debug("bfOffbits=%ld.\n",fHeader.bfOffBits);
	

	read (fd,&info,sizeof(info));
	debug("picture resolution is %ld * %ld.\n",info.biWidth,info.biHeight);
	debug("bpp is %hd .\n",info.biBitCount);
	pPic->width=info.biWidth;
	pPic->height=info.biHeight;
	pPic->bpp=info.biBitCount;
	//pPic->pData=rgb_buf;
	//第三步读取图片有效信息
	//移动指针到信息的偏移量处
	
	lseek(fd, fHeader.bfOffBits, SEEK_SET);
	len=info.biHeight*info.biWidth*info.biBitCount/3;
	read(fd,rgb_buf,len);

	//第四步，放进fb显示
	fb_draw(pPic);
	close(fd);
	return 0;
	

}
//封装的对外使用的bmp现实函数
int display_bmp(const char *pathname)
{
	int ret=-1;
	struct picture_info picture;//要显示的图片
	
	//第一步 检查图片是否是bmp
	ret=is_bmp(pathname);
	if(ret!=0)
	{
		return -1;
	}

	//显示bmp
	picture.pathname=pathname;
	picture.pData=rgb_buf;
	bmp_analyze(&picture);
}























