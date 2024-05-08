#include <config.h>

#define FBDEVICE "/dev/fb0"


#define WIDTH		1024	
#define HEIGHT		600


#define WHITE		0xffffffff			// test ok
#define BLACK		0x00000000
#define RED			0xffff0000
#define GREEN		0xff00ff00			// test ok
#define BLUE		0xff0000ff	


// º¯ÊýÉùÃ÷
int fb_open(void);
void fb_close(void);

void fb_draw_back(unsigned int width, unsigned int height, unsigned int color);
void fb_draw_line(unsigned int color);


void fb_draw_bmp(const struct picture_info *pPic);
void fb_draw_png(const struct picture_info *pPic);
void fb_draw_jpg(const struct picture_info *pPic);





