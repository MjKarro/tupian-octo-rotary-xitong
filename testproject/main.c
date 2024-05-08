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


	scan_image2("image");


	ts_updown();

	fb_close();
	
	return 0;
}











































