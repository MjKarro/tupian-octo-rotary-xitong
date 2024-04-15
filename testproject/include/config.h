#ifndef __CONFIG_H__
#define __CONFIG_H__

//#define DEBUG
//*******************************�궨��*******************************************
#ifdef DEBUG
#define debug(...)															 \
	fprintf(stderr, "[DEBUG  ]%s %s(Line %d): ",__FILE__,__FUNCTION__,__LINE__); \
	fprintf(stderr, __VA_ARGS__); 										
#else
#define debug(...) 
#endif

//�涨һ������ͼƬ1920*1080,BPP���24
#define BMP_MAX_RESOLUTION	(1920*1080)
#define BMP_BUF_SIZE			(BMP_MAX_RESOLUTION*3)

//�涨���֧��100��ͼƬ
#define MAX_IMAGE_CNT	100

#define DEVICE_TOUCHSCREEN  "/dev/input/event1"

#define TOUCH_WIDTH 	200


//*******************************�ṹ�嶨��*******************************************
typedef struct picture_info
{
	char * pathname;
	unsigned int width;
	unsigned int height;
	unsigned int bpp;
	char * pData;
} picture_info;


//*******************************��������*******************************************

int is_png(const char *path);
int is_bmp(const char *path);
int is_jpg(const char *path);

int display_bmp(const char *pathname);
int display_jpg(const char *pathname);
int display_png(const char *pathname);

int scan_image(const char *path);
int scan_image2(const char *path);

void show_images(void);
int ts_updown(void);
//*******************************ȫ�ֱ���*******************************************
char rgb_buf[BMP_BUF_SIZE];
#endif
























