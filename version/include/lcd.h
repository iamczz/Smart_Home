#ifndef _LCD_H_
#define _LCD_H_ 

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <iconv.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <stropts.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <pthread.h>





#define RGB565_MASK_RED     0xF800
#define RGB565_MASK_GREEN   0x07E0
#define RGB565_MASK_BLUE    0x001F

#define WHITE 16777215 //白色
#define RED 16711680 //红色
#define GREEN 65280 //绿色
#define BLUE 255 //蓝色
#define YELLOW 16776960 //黄色
#define GOLDEN 13467442//金色
#define GREEN_YELLOW 9689968//青黄色
#define LAVNDER 14381275//淡紫色

#define	uint_8	unsigned char
typedef struct RGB32 {
uint_8 b; // 蓝色分量
uint_8 g; // 绿色分量
uint_8 r; // 红色分量
uint_8 reserved; // 保留字节（用作Alpha通道或忽略）
} rgb32;

typedef struct RGB24 {
uint_8 b;	// 蓝色分量
uint_8 g;	// 绿色分量
uint_8 r;	// 红色分量
} rgb24;

struct fb_lcd
{	
	char *fbp;
	int fbfd;   
	long int screensize;
};
struct fb_lcd fb_info;

int lcd_init(char *device_name, struct fb_lcd *fb_info);
void surface_init(struct fb_lcd *fb_info);
void show_image(int x_wide, int y_high, const unsigned int image[], struct fb_lcd *fb_info);
void lcd_display(int x_wide, int y_high, unsigned char video[], struct fb_lcd *fb_info);
void clear_screen(int x0, int x_wide, int y0, int y_hight, unsigned color, struct fb_lcd *fb_info);
void draw_picture( int x0, int x_wide, int y0, int y_hight, const unsigned int picture[], struct fb_lcd *fb_info);
void clear_sector(int x_pos, int y_pos, int x_size, int y_size, struct fb_lcd *fb_info);
void show_H_words(int x_pos, int y_pos, const char incode[], struct fb_lcd *fb_info, unsigned color);
void show_V_words(int x_pos, int y_pos, const char incode[], struct fb_lcd *fb_info, unsigned color);
void show_string(int x_pos, int y_pos, char *input, struct fb_lcd *fb_info, unsigned color);
void show_number(char number[], struct fb_lcd *fb_info, unsigned color);
void lcd_close(struct fb_lcd *fb_info);
void display_pic(int x0,int x_wide,int y0, int y_high, unsigned char video[], struct fb_lcd *fb_info);

#endif
