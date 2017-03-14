#include "cam.h"
#include "cam_cmd.h"
#include "lcd.h"
#include "zgb.h"
#include "zgb_cmd.h"
#include "get_time.h"

unsigned char buff[1024*60];

void *thread_cam(void *arg)
{
    v4l2_use.jpeg = &buff;
	unsigned char img_buf[200*260*3 + 54*8];	//54字节位bmp格式图片的格式头。
	struct stat filestatus;
	
	int ret = stat("log.bmp", &filestatus);
	if(ret < 0)
	{
		perror("get file stat failed!");
		return -1;
	}
	int imgsize = filestatus.st_size;
	FILE *imgfp = fopen("log.bmp","r");
	if(imgfp < 0)
	{
		perror("open log.bmp failed!");
		return -1;
	}
	if (fread(img_buf, imgsize, 1, imgfp) < 0)//将test.bmp图片数据保存在img_buf指向的数组中，由于test.bmp另外还包含54字节的格式头，数组大小设为640*480*4+54字节
	{
		printf("read log.bmp data error!\n");
		return -1;
	}	
	
	display_pic(80, 200, 70, 260, img_buf+54, &fb_info);
    while(1)
    {
		v4l2_use.read();
		v4l2_use.write(WIDTH, HEIGHT, v4l2_use.rgb_buffers, &fb_info);
    }
    v4l2_use.close(&fb_info);
  
    pthread_exit(NULL);
}
