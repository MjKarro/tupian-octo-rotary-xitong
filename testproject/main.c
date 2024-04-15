#include <stdio.h>

#include <fb.h>
#include <config.h>
#include <unistd.h>


int main(void)
{
	int ret =-1;
	struct picture_info picture;//ÒªÏÔÊ¾µÄÍ¼Æ¬

	printf("image decode player.\n");
	ret=fb_open();
	if (ret <0)
	{
		printf("fb_open error.\n");
		return -1;
	}
	
	//fb_draw_back(1024,600, RED);
	 //fb_draw_picture6(700,420);
	 //²âÊÔbmp
	//is_bmp("2.bmp");
	//picture.pathname="3.bmp";
	//bmp_analyze(&picture);


	//²âÊÔjpg
	//display_bmp("1.bmp");

	//sleep(3);
	//display_jpg("1024600.jpg");

	//²âÊÔpng
	//display_png("2.png");


	scan_image2("image");
//	while(1)
//		show_images();

	ts_updown();

	fb_close();
	
	return 0;
}











































