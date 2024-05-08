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




//定义存储图片的数组

image_info_t images [MAX_IMAGE_CNT];
unsigned int image_index=0;
//images数组本来是空的，然后程序初始化时会去
//一个事先约定好的目录(image目录)去递归检索所有文件及子文件夹
//并将收集的所有图片填充到images
//然后显示图片逻辑部分再去这个图片库拿出对应的图片即可

//path是要检索的文件夹的pathname
int scan_image(const char *path)
{
	//在本函数中递归检索path 文件夹，将其中所有图片填充到images数组中
	DIR *dir;
	struct dirent *ptr;
	char base[1000];

	if ((dir=opendir(path)) == NULL)
	{
		perror("Open dir error...");
		exit(1);
	}

	//readdir 函数每调用一次就会返回opendir打开的path目录下的一个文件
	//直到path目录下所有文件读完返回NULL
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
			case DT_UNKNOWN:	//unknow 不识别文件
				printf("unknow file type.\n");
				break;
			default:
				break;
		}
								
	}


}
//检索方法2
int scan_image2(const char *path)
{
	//在本函数中递归检索path 文件夹，将其中所有图片填充到images数组中
	DIR *dir;
	struct dirent *ptr;
	char base[1000];
	struct stat sta;
	
	if ((dir=opendir(path)) == NULL)
	{
		perror("Open dir error...");
		exit(1);
	}

	//readdir 函数每调用一次就会返回opendir打开的path目录下的一个文件
	//直到path目录下所有文件读完返回NULL
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
			//普通文件处理:
			//先判断是否属于已知某种图片格式:是则放到image 数组，不属于不管
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

//实现触摸屏对图片翻页功能
int ts_updown(void)
{
	//第1步:触摸屏操作检测

	struct input_event env;
	int fd=-1,ret=-1;
	int i=0;			//用来显示第几个图片
	
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

	//第2步:根据触摸屏进行图片操作
	if((env.type==EV_ABS)&&(env.code==ABS_X))
	{
		//x坐标
		if((env.value>0)&&(env.value<TOUCH_WIDTH))
		{
			//左翻页
			if(i--<0)
			{
				i=image_index -1;
			}
			
		}
		else if ((env.value>WIDTH-TOUCH_WIDTH)&&(env.value<WIDTH))
		{
			//右 翻页
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


























