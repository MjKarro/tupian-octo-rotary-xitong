/*
*解码jpg图片，并调用fb.c中的显示接口来显示到lcd上
*
*
*/
#include <stdio.h>
#include <config.h>
#include <jpeglib.h>
#include <jerror.h>
 #include <string.h>

//函数功能:判断图片是不是jpg
//返回值是返回0，不是1,错误-1
//采用缓冲
int is_jpg(const char *path)
{
	FILE *file=NULL;
	char buf[2]={0};

	file=fopen(path,"rb");
	if(NULL==file)
	{
		fprintf("fopen %s error .\n",path);
	}

	fread(buf,1,2,file);
	debug("read 0x%x%x.\n ",buf[0],buf[1]);
	if(!((buf[0]==0xff)&&(buf[1]==0xd8)))
	{
		fclose(file);
		return 1;//不是
	}
	//是继续判断
	//文件指针移动到倒数两个字节位置

	fseek(file,-2,SEEK_END);
	fread(buf,1,2,file);
	debug("read 0x%x%x.\n ",buf[0],buf[1]);
	if(!((buf[0]==0xff)&&(buf[1]==0xd9)))
	{
		fclose(file);
		return 1;//不是
	}
	fclose(file);
	return 0;

}

struct my_error_mgr 
{  
	struct jpeg_error_mgr pub;	/* "public" fields */  
	//jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

//自己定义的错误处理函数
METHODDEF(void)my_error_exit (j_common_ptr cinfo)
{ 
	//my_error_ptr myerr = (my_error_ptr) cinfo->err;  
	//(*cinfo->err->output_message) (cinfo); 
	fprintf(stderr,"my_error_exit\n");
	//longjmp(myerr->setjmp_buffer, 1);
}

/*
*函数功能:解码jpg图片，并存储
*函数参数:pPic,记录源jpg图片，解码出来的图片宽高，图片数据缓冲区等信息
*返回值:成功返回0，失败-1
*/
static int jpg_analyze(struct picture_info *pPic)
 {
 	struct jpeg_decompress_struct cinfo;		//信息记录和传递的结构体
 	struct my_error_mgr jerr;				//错误处理
 	FILE * infile;							//  指向源文件
 	//JSAMPARRAY buffer=NULL;					//  解码一行数据指针
 	char * buffer=NULL;
 	int row_stride;						// 解码出来的一行的长度

	
 	 if ((infile = fopen(pPic->pathname, "rb")) == NULL) 
  	{
  		fprintf(stderr, "can't open %s\n", pPic->pathname);    
  		return -1;  
  	}
	//step1 :错误处理函数部分绑定
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	//给解码器做必要的内存分配和初始化
	jpeg_create_decompress(&cinfo);

	//step2:将fopen打开的源jpg图片和解码器相关联
	jpeg_stdio_src(&cinfo, infile);

	//step3:读jpg文件头
	jpeg_read_header(&cinfo, TRUE);

	//step4:启动解码器
	jpeg_start_decompress(&cinfo);
	
	debug("image resolution:%d * %d ,bpp/8 is %d.\n",
		 cinfo.output_width,cinfo.output_height,cinfo.output_components);

	//解码一行字节数
	row_stride = cinfo.output_width * cinfo.output_components;
	//buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
	buffer = (char *)malloc(row_stride);
	if(NULL==buffer)
	{
		fprintf(stderr,"cinfo.mem->alloc_sarray error .\n");
		return -1;
	}
	
	//step5:一行行的读到buffer，将buffer一行中数据移走暂存或者使用
	while (cinfo.output_scanline < cinfo.output_height) 
	{
		//(void) jpeg_read_scanlines(&cinfo, buffer, 1);
		//解码一行信息丢到buffer
		(void) jpeg_read_scanlines(&cinfo, &buffer, 1);
		//将buffer中的一行数据移走暂存或者使用，腾出buffer空间。此处将buffer复制到pPic结构体的pData
		memcpy(pPic->pData+(cinfo.output_scanline-1) * row_stride,buffer,row_stride);
	}

	//step6:结束解码器
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);

	//记录jpg数据
	pPic->width=cinfo.output_width;
	pPic->height=cinfo.output_height;
	pPic->bpp=cinfo.output_components*8;
	


	return 0;

 }

//封装的对外使用的jpg现实函数
int display_jpg(const char *pathname)
{
	int ret=-1;
	struct picture_info picture;//要显示的图片
	
	//第一步 检查图片是否是jpg
	ret=is_jpg(pathname);
	if(ret!=0)
	{
		return -1;
	}

	//第2步:解析jpg
	picture.pathname=pathname;
	picture.pData=rgb_buf;
	jpg_analyze(&picture);

	//第3步:显示
	fb_draw2(&picture);
}



































