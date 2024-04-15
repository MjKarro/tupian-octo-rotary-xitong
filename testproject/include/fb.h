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
/*
void fb_draw_picture(void);
void fb_draw_picture2(void);
void fb_draw_picture3(void);
void fb_draw_picture4(void);
void fb_draw_picture5(unsigned int x0, unsigned int y0);
void fb_draw_picture6(unsigned int x0,unsigned int y0);
void fb_draw_picture7(unsigned int x0,unsigned int y0);
*/

void fb_draw(const struct picture_info *pPic);
void fb_draw2(const struct picture_info *pPic);
void fb_draw3(const struct picture_info *pPic);





