#include "image_manage.h"
#include <config.h>
#include <fb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>




//����洢ͼƬ������

image_info_t images [MAX_IMAGE_CNT];
unsigned int image_index=0;
//images���鱾���ǿյģ�Ȼ������ʼ��ʱ��ȥ
//һ������Լ���õ�Ŀ¼(imageĿ¼)ȥ�ݹ���������ļ������ļ���
//�����ռ�������ͼƬ��䵽images
//Ȼ����ʾͼƬ�߼�������ȥ���ͼƬ���ó���Ӧ��ͼƬ����

//path��Ҫ�������ļ��е�pathname
int scan_image(const char *path)
{
	//�ڱ������еݹ����path �ļ��У�����������ͼƬ��䵽images������
	DIR *dir;
	struct dirent *ptr;
	char base[1000];

	if ((dir=opendir(path)) == NULL)
	{
		perror("Open dir error...");
		exit(1);
	}

	//readdir ����ÿ����һ�ξͻ᷵��opendir�򿪵�pathĿ¼�µ�һ���ļ�
	//ֱ��pathĿ¼�������ļ����귵��NULL
	while ((ptr=readdir(dir)) != NULL)
	{
		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
			continue;
		switch (ptr->d_type)
		{
			case DT_REG: 		//file
				printf("d_name:%s/%s\n",path,ptr->d_name);
				break;
			case DT_DIR:			//dir
				memset(base,'\0',sizeof(base));
				strcpy(base,path);
				strcat(base,"/");
				strcat(base,ptr->d_name);
				scan_image(base);
				break;
			case DT_UNKNOWN:	//unknow ��ʶ���ļ�
				printf("unknow file type.\n");
				break;
			default:
				break;
		}
								
	}


}
//��������2
int scan_image2(const char *path)
{
	//�ڱ������еݹ����path �ļ��У�����������ͼƬ��䵽images������
	DIR *dir;
	struct dirent *ptr;
	char base[1000];
	struct stat sta;
	
	if ((dir=opendir(path)) == NULL)
	{
		perror("Open dir error...");
		exit(1);
	}

	//readdir ����ÿ����һ�ξͻ᷵��opendir�򿪵�pathĿ¼�µ�һ���ļ�
	//ֱ��pathĿ¼�������ļ����귵��NULL
	while ((ptr=readdir (dir)) != NULL)
	{

		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
			continue;
		
		memset(base,'\0',sizeof(base));
		strcpy(base,path);
		strcat(base,"/");
		strcat(base,ptr->d_name);		
		lstat(base, &sta);
		
		if(S_ISREG(sta.st_mode))
		{
			//printf("regular file.\n");
			//printf("d_name:%s/%s.\n",path,ptr->d_name);
			//��ͨ�ļ�����:
			//���ж��Ƿ�������֪ĳ��ͼƬ��ʽ:����ŵ�image ���飬�����ڲ���
			if(!is_bmp(base))
			{
				strcpy(images[image_index].pathname,base);
				images[image_index].type=IMAGE_TYPE_BMP;
			}
			if(!is_png(base))
			{
				strcpy(images[image_index].pathname,base);
				images[image_index].type=IMAGE_TYPE_PNG;
			}			
			if(!is_jpg(base))
			{
				strcpy(images[image_index].pathname,base);
				images[image_index].type=IMAGE_TYPE_JPG;
			}
			image_index++;

		}

		if(S_ISDIR(sta.st_mode))
		{
			//printf("directory.\n");
			//printf("d_name:%s/%s.\n",path,ptr->d_name);	
			scan_image2(base);
		}
									
	}


}



void show_images(void)
{
	int i;

	for(i=0;i<image_index;i++)
	{
		switch(images[i].type)
		{
			case IMAGE_TYPE_BMP:
				display_bmp(images[i].pathname);	break;
			case IMAGE_TYPE_PNG:
				display_png(images[i].pathname);	break;
			case IMAGE_TYPE_JPG:
				display_jpg(images[i].pathname);	break;
			default:
				break;
		}
		sleep(2);
	}
}

static void show_imag(int index)
{
	switch(images[index].type)
	{
		case IMAGE_TYPE_BMP:
			display_bmp(images[index].pathname);	break;
		case IMAGE_TYPE_PNG:
			display_png(images[index].pathname);	break;
		case IMAGE_TYPE_JPG:
			display_jpg(images[index].pathname);	break;
		default:
			break;
	}

}

//ʵ�ִ�������ͼƬ��ҳ����
int ts_updown(void)
{
	//��1��:�������������

	struct input_event env;
	int fd=-1,ret=-1;
	int i=0;			//������ʾ�ڼ���ͼƬ
	
	fd=open(DEVICE_TOUCHSCREEN,O_RDONLY);
	if(fd<0)
	{
		perror("open");
		return -1;
	}
	while(1)
	{
		memset(&env,0,sizeof(env));
		ret=read(fd,&env,sizeof(env));
		if(ret!=sizeof(env))
		{
			perror("read"); 
			close(fd);
			return -1;
		}

	//��2��:���ݴ���������ͼƬ����
	if((env.type==EV_ABS)&&(env.code==ABS_X))
	{
		//x����
		if((env.value>0)&&(env.value<TOUCH_WIDTH))
		{
			//��ҳ
			if(i--<0)
			{
				i=image_index -1;
			}
			
		}
		else if ((env.value>WIDTH-TOUCH_WIDTH)&&(env.value<WIDTH))
		{
			//�� ��ҳ
			if(i++>=image_index)
			{
				i=0;
			}
		}
		show_imag(i-1);
		
	}
	}
	close(fd);
	return 0;
}


























