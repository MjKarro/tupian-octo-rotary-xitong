/*
*����jpgͼƬ��������fb.c�е���ʾ�ӿ�����ʾ��lcd��
*
*
*/
#include <stdio.h>
#include <config.h>
#include <jpeglib.h>
#include <jerror.h>
 #include <string.h>

//��������:�ж�ͼƬ�ǲ���jpg
//����ֵ�Ƿ���0������1,����-1
//���û���
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
		return 1;//����
	}
	//�Ǽ����ж�
	//�ļ�ָ���ƶ������������ֽ�λ��

	fseek(file,-2,SEEK_END);
	fread(buf,1,2,file);
	debug("read 0x%x%x.\n ",buf[0],buf[1]);
	if(!((buf[0]==0xff)&&(buf[1]==0xd9)))
	{
		fclose(file);
		return 1;//����
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

//�Լ�����Ĵ�������
METHODDEF(void)my_error_exit (j_common_ptr cinfo)
{ 
	//my_error_ptr myerr = (my_error_ptr) cinfo->err;  
	//(*cinfo->err->output_message) (cinfo); 
	fprintf(stderr,"my_error_exit\n");
	//longjmp(myerr->setjmp_buffer, 1);
}

/*
*��������:����jpgͼƬ�����洢
*��������:pPic,��¼ԴjpgͼƬ�����������ͼƬ��ߣ�ͼƬ���ݻ���������Ϣ
*����ֵ:�ɹ�����0��ʧ��-1
*/
static int jpg_analyze(struct picture_info *pPic)
 {
 	struct jpeg_decompress_struct cinfo;		//��Ϣ��¼�ʹ��ݵĽṹ��
 	struct my_error_mgr jerr;				//������
 	FILE * infile;							//  ָ��Դ�ļ�
 	//JSAMPARRAY buffer=NULL;					//  ����һ������ָ��
 	char * buffer=NULL;
 	int row_stride;						// ���������һ�еĳ���

	
 	 if ((infile = fopen(pPic->pathname, "rb")) == NULL) 
  	{
  		fprintf(stderr, "can't open %s\n", pPic->pathname);    
  		return -1;  
  	}
	//step1 :�����������ְ�
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	//������������Ҫ���ڴ����ͳ�ʼ��
	jpeg_create_decompress(&cinfo);

	//step2:��fopen�򿪵�ԴjpgͼƬ�ͽ����������
	jpeg_stdio_src(&cinfo, infile);

	//step3:��jpg�ļ�ͷ
	jpeg_read_header(&cinfo, TRUE);

	//step4:����������
	jpeg_start_decompress(&cinfo);
	
	debug("image resolution:%d * %d ,bpp/8 is %d.\n",
		 cinfo.output_width,cinfo.output_height,cinfo.output_components);

	//����һ���ֽ���
	row_stride = cinfo.output_width * cinfo.output_components;
	//buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
	buffer = (char *)malloc(row_stride);
	if(NULL==buffer)
	{
		fprintf(stderr,"cinfo.mem->alloc_sarray error .\n");
		return -1;
	}
	
	//step5:һ���еĶ���buffer����bufferһ�������������ݴ����ʹ��
	while (cinfo.output_scanline < cinfo.output_height) 
	{
		//(void) jpeg_read_scanlines(&cinfo, buffer, 1);
		//����һ����Ϣ����buffer
		(void) jpeg_read_scanlines(&cinfo, &buffer, 1);
		//��buffer�е�һ�����������ݴ����ʹ�ã��ڳ�buffer�ռ䡣�˴���buffer���Ƶ�pPic�ṹ���pData
		memcpy(pPic->pData+(cinfo.output_scanline-1) * row_stride,buffer,row_stride);
	}

	//step6:����������
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);

	//��¼jpg����
	pPic->width=cinfo.output_width;
	pPic->height=cinfo.output_height;
	pPic->bpp=cinfo.output_components*8;
	


	return 0;

 }

//��װ�Ķ���ʹ�õ�jpg��ʵ����
int display_jpg(const char *pathname)
{
	int ret=-1;
	struct picture_info picture;//Ҫ��ʾ��ͼƬ
	
	//��һ�� ���ͼƬ�Ƿ���jpg
	ret=is_jpg(pathname);
	if(ret!=0)
	{
		return -1;
	}

	//��2��:����jpg
	picture.pathname=pathname;
	picture.pData=rgb_buf;
	jpg_analyze(&picture);

	//��3��:��ʾ
	fb_draw2(&picture);
}



































