/*
*����pngͼƬ��������fb.c�е���ʾ�ӿ�����ʾ��lcd��
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

//��������:�ж�ͼƬ�ǲ���png
//����ֵ�Ƿ���0������1,����-1
//���û���
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
*��������:����pngͼƬ�����洢
*��������:pPic,��¼ԴpngͼƬ�����������ͼƬ��ߣ�ͼƬ���ݻ���������Ϣ
*����ֵ:�ɹ�����0��ʧ��-1
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
	//��1�� :������ݽṹʵ����
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
	//��2��:���ô����� 
	if (setjmp(png_jmpbuf(png_ptr)))
	{         
		png_destroy_read_struct(&png_ptr, &info_ptr, 0);
		fclose(fp);
		return -1;
	}

	//��3��:��Ҫ�����png�ļ�ָ���png��������
	png_init_io(png_ptr, fp);

	//��4��:��ȡpngͼƬ��Ϣ
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND |PNG_TRANSFORM_STRIP_ALPHA , 0);

	//��5��:���ͼƬ��Ϣ��ӡ
	color_type=info_ptr->color_type;
	debug("color_type= %d.\n",color_type);

	pPic->width=info_ptr->width;
	pPic->height=info_ptr->height;
	pPic->bpp=info_ptr->pixel_depth;
	len =info_ptr->width*info_ptr->height*info_ptr->pixel_depth/8;
	debug("width= %u,height= %u,bpp= %u.\n",pPic->width,pPic->height,pPic->bpp);

	//��6��:��ȡ��Ϣ
	row_pointers = png_get_rows(png_ptr, info_ptr);
	
	//��7��:ֻ����RGB24λ���ɫͼƬ
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
	
	//��8��:�ر�
	png_destroy_read_struct(&png_ptr, &info_ptr,0);
	fclose(fp);
	return 0;
}


//��װ�Ķ���ʹ�õ�png��ʵ����
int display_png(const char *pathname)
{
	int ret=-1;
	struct picture_info picture;//Ҫ��ʾ��ͼƬ
	
	//��һ�� ���ͼƬ�Ƿ���jpg
	ret=is_png(pathname);
	if(ret!=0)
	{
		debug("They are not png Picture.\n ");
		return -1;
	}
	debug("%s are  png Picture .\n ",pathname);

	//��2��:����png
	picture.pathname=pathname;
	picture.pData=rgb_buf;
	png_analyze(&picture);

	//��3��:��ʾ
	fb_draw3(&picture);
	return 0;
}


































