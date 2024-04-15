/*
*解码png图片，并调用fb.c中的显示接口来显示到lcd上
*
*
*/
#include <stdio.h>
#include <string.h>

#include <config.h>

#include <png.h>
#include <pngstruct.h>
#include <pnginfo.h>

#define PNG_BYTES_TO_CHECK 8

//函数功能:判断图片是不是png
//返回值是返回0，不是1,错误-1
//采用缓冲
int is_png(const char *path)
{
	FILE *fp=NULL;
	char buf[PNG_BYTES_TO_CHECK];   
	/* Open the prospective PNG file. */   
	if ((fp = fopen(path, "rb")) == NULL)      
	return -1;  
	/* Read in some of the signature bytes. */   
	if (fread(buf, 1, PNG_BYTES_TO_CHECK, fp) != PNG_BYTES_TO_CHECK)      
	return -1;   
	/* Compare the first PNG_BYTES_TO_CHECK bytes of the signature.    * Return nonzero (true) if they match.    */   
	return(png_sig_cmp(buf, 0, PNG_BYTES_TO_CHECK));

}

/*
*函数功能:解码png图片，并存储
*函数参数:pPic,记录源png图片，解码出来的图片宽高，图片数据缓冲区等信息
*返回值:成功返回0，失败-1
*/

static int png_analyze(struct picture_info *pPic)
{
	FILE *fp;
   	png_structp png_ptr;   
   	png_infop info_ptr;
	int color_type;
	png_bytep* row_pointers;
	unsigned long len;
	int i,j,pos;
	
	if ((fp = fopen(pPic->pathname, "rb")) == NULL) 
  	{
  		fprintf(stderr, "can't open %s\n", pPic->pathname);    
  		return -1;  
  	}
	//第1步 :相关数据结构实例化
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,0, 0, 0);
	if (png_ptr == 0)   
	{
	   	fclose(fp);
		return -1;   
	}

	info_ptr = png_create_info_struct(png_ptr);   
	if (info_ptr ==0)   
	{
		png_destroy_read_struct(&png_ptr, 0, 0);
		fclose(fp);
		return -1;
	}
	//第2步:设置错误处理 
	if (setjmp(png_jmpbuf(png_ptr)))
	{         
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		fclose(fp);
		return -1;
	}

	//第3步:将要解码的png文件指针和png解码器绑定
	png_init_io(png_ptr, fp);

	//第4步:读取png图片信息
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND |PNG_TRANSFORM_STRIP_ALPHA , 0);

	//第5步:相关图片信息打印
	color_type=info_ptr->color_type;
	debug("color_type= %d.\n",color_type);

	pPic->width=info_ptr->width;
	pPic->height=info_ptr->height;
	pPic->bpp=info_ptr->pixel_depth;
	len =info_ptr->width*info_ptr->height*info_ptr->pixel_depth/8;
	debug("width= %u,height= %u,bpp= %u.\n",pPic->width,pPic->height,pPic->bpp);

	//第6步:读取信息
	row_pointers = png_get_rows(png_ptr, info_ptr);
	
	//第7步:只处理RGB24位真彩色图片
	if (color_type == PNG_COLOR_TYPE_RGB)
	{
		//memcpy(pPic->pData,row_pointers,len); 

		for(i=0;i<pPic->height;i++)
		{
			for(j=0;j<3*pPic->width;j+=3)
			{
				pPic->pData[pos++]=row_pointers[i][j+2];//blue
				pPic->pData[pos++]=row_pointers[i][j+1];//green
				pPic->pData[pos++]=row_pointers[i][j+0];//red
			}
		}

		
	}
	
	//第8步:关闭
	png_destroy_read_struct(&png_ptr, &info_ptr,0);
	fclose(fp);
	return 0;
}


//封装的对外使用的png现实函数
int display_png(const char *pathname)
{
	int ret=-1;
	struct picture_info picture;//要显示的图片
	
	//第一步 检查图片是否是jpg
	ret=is_png(pathname);
	if(ret!=0)
	{
		debug("They are not png Picture.\n ");
		return -1;
	}
	debug("%s are  png Picture .\n ",pathname);

	//第2步:解析png
	picture.pathname=pathname;
	picture.pData=rgb_buf;
	png_analyze(&picture);

	//第3步:显示
	fb_draw3(&picture);
	return 0;
}


































