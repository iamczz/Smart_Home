#ifndef _CAM_CMD_H
#define _CAM_CMD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>           
#include <fcntl.h>             
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>         
#include <linux/videodev2.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include "lcd.h"
#include "mutexs.h"
#include "srv.h"

typedef struct v4l2_result
{
    void* jpeg_buf;
    unsigned char* stre_buf;
    long jpeg_buf_size;

} V4L2_STRU;

#define CLEAR(x) memset(&(x), 0, sizeof(x))//清内存
#define WIDTH 640                       // 源图片宽度
#define HEIGHT 480                      // 源图片高度
#define IMG_SIZE (WIDTH * HEIGHT * 3)   // 源图片大小
#define BITCOUNT 24                     // 源图片位深读
#define DEST_BUF_SIZE (((WIDTH * BITCOUNT +31) / 32) * 4 * HEIGHT)//目标图片大小
#define DEVICE_VIDEO "/dev/video0"//摄像头设备名
#define DEVICE_LCD "/dev/fb0"//LCD设备名

struct buffer
{
    unsigned char * start;
    size_t length;

};

typedef struct v4l2
{
    unsigned char *dest_buffers;
    unsigned char *rgb_buffers;
    void *jpeg;
    long jpeg_size;
    void (*open)(void);
    int (*read)(void);
    void (*write)(int x_wide, int y_high, unsigned char video[], struct fb_lcd *fb_info);
    void (*close)(struct fb_lcd *lcdone);
}V4L2;

void v4l2_lcd_close(struct fb_lcd *lcdone);
int v4l2_lcd_init(void);
int v4l2_open(char* dev_name);
int v4l2_capture_rgb(void);
int read_frame(void);
void v4l2_close(void);

#endif
