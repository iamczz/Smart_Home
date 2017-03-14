#include "get_time.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

//获取时间
char *get_time(char *p_time, struct fb_lcd *fb_info)
{
    struct tm *t = NULL;
    time_t tt;	
    time(&tt);
    t = localtime(&tt);
    strftime(p_time, 60, "%Y-%m-%d %H:%M:%S", t);
	
    return p_time;
}

Clock *clock_time(Clock *clock,struct fb_lcd *fb_info)
{
	sleep(1); 
	clock->second++;
	clear_screen(470,46, 540, 50, WHITE, fb_info);
	if(clock->second == 60)  			  
	{	 
		clock->second = 0;
		clear_screen(405,46, 540, 50, WHITE, fb_info);
		clock->minute++;
	}	  
	if(clock->minute == 60)  		
	{					
		clock->minute = 0; 
		clear_screen(340,46, 540, 50, WHITE, fb_info);
		clock->hour++;  
	}  
	if(clock->hour == 24)
	{
		clock->hour = 0; 
	}
}


