#ifndef _GET_TIME_H_
#define _GET_TIME_H_ 

#include <stdio.h>
#include <malloc.h>
#include <iconv.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <stropts.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "lcd.h"

typedef struct Clock  
{
	int hour;  
	int minute;  
	int second;  
}Clock; 

char *get_time(char *p_time, struct fb_lcd *fb_info);
Clock *clock_time(Clock *clock,struct fb_lcd *fb_info);

#endif
