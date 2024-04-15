//����bmp��ʽͼƬ����fb��ʾ
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <fb.h>
#include "bmp.h"
#include <config.h>



//�ж��Ƿ���bmp��ʽ
//�Ƿ���0����-1
int is_bmp(const char *path)
{
	int fd =-1;
	unsigned char buf[2]={0};//bmpͷ�ļ���С
	ssize_t ret=0;
	

	//��һ�� ��bmp ��ʽͼƬ
	fd=open(path,O_RDONLY);
	if(fd<0)
	{
			fprintf(stderr,"open %s error .\n",path);
			return -1;
	}

	
	//�ڶ��� ��ȡͷ�ļ� 
	ret =read( fd, buf, 2);
	if(ret!=2)
	{
		fprintf(stderr,"read file header error .\n");
		close(fd);
		return -1;
	}

	//���� ������ͷ�ļ��ж��Ƿ���bmpͼƬ
	if(buf[0] != 'B' || buf[1] != 'M')
	{
		fprintf(stderr,"file %s is not bmp picture .\n",path);
		return -1;		

	}

	close(fd);
	return 0;	
	
	
}



//path =·��+�ļ���
//����path��bmpͼƬ�����ҽ����������ݶ���bmp_buf��
//����ֵ���󷵻�-1,��ȷ����0
 static int bmp_analyze(struct picture_info *pPic)
{
	int fd =-1;
	BitMapFileHeader fHeader;
	BitMapInfoHeader info;
	unsigned long len;
	//unsigned short temp;

	//��һ�� ��bmp ��ʽͼƬ
	fd=open(pPic->pathname,O_RDONLY);
	if(fd<0)
	{
			fprintf(stderr,"open %s error .\n",pPic->pathname);
			return -1;
	}

	
	//�ڶ��� ��ȡͷ�ļ� 
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
	//��������ȡͼƬ��Ч��Ϣ
	//�ƶ�ָ�뵽��Ϣ��ƫ������
	
	lseek(fd, fHeader.bfOffBits, SEEK_SET);
	len=info.biHeight*info.biWidth*info.biBitCount/3;
	read(fd,rgb_buf,len);

	//���Ĳ����Ž�fb��ʾ
	fb_draw(pPic);
	close(fd);
	return 0;
	

}
//��װ�Ķ���ʹ�õ�bmp��ʵ����
int display_bmp(const char *pathname)
{
	int ret=-1;
	struct picture_info picture;//Ҫ��ʾ��ͼƬ
	
	//��һ�� ���ͼƬ�Ƿ���bmp
	ret=is_bmp(pathname);
	if(ret!=0)
	{
		return -1;
	}

	//��ʾbmp
	picture.pathname=pathname;
	picture.pData=rgb_buf;
	bmp_analyze(&picture);
}























