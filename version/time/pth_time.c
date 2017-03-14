/*************************************************************************
    > File Name: pth_time.c
    > Author: 龙创科技
    > Mail: ouxiaolong88@163.com 
    > Created Time: 2016年07月18日 星期一 09时12分22秒
 ************************************************************************/
#include "lcd.h"
#include "get_time.h"
#include "pth_time.h"
#include "zgb.h"
#include "zgb_cmd.h"

void *thread_time(void *arg)
{
	char p_time[60];
	uint8_t tem[50];
	uint8_t hum[50];
	uint8_t clock_hour[10];
	uint8_t clock_minute[10];
	uint8_t clock_second[10];
	Clock clock;
	struct tm *q;	
	memset(&clock,'\0',sizeof(Clock));//时钟初始化
	
    while(1)
    {
		//显示时间
#if 0
		clear_sector(340, 540, WIDTH, 60, &fb_info);//在背景图中清除一块640*480区域显示采集到的图像
		get_time(p_time, &fb_info);
		show_string(340, 540, p_time, &fb_info, LAVNDER);
#endif
#if 0
		time_t t=time(NULL);
		q=localtime(&t);
		
		if ((int)q->tm_min == 59)
		{
			clear_screen(340,46, 540, 50, WHITE, &fb_info);
		}
		printf("%d\n",(int)q->tm_sec);
		if((int)q->tm_sec == 59)
		{
			clear_screen(405,46, 540, 50, WHITE, &fb_info);
		}
		clear_screen(470,46, 540, 50, WHITE, &fb_info);
		
		//printf("%d-%d-%d ",1900+q->tm_year,q->tm_mon,q->tm_mday);
		//printf("%d:%d:%d\n",q->tm_hour,q->tm_min,q->tm_sec);
		sprintf(clock_hour, "%u", (uint8_t)q->tm_hour);
		sprintf(clock_minute, "%u", (uint8_t)q->tm_min);
		sprintf(clock_second, "%u", (uint8_t)q->tm_sec);
		show_string(340, 540, clock_hour, &fb_info, LAVNDER);
		show_string(385, 540,":", &fb_info, LAVNDER);
		show_string(405, 540, clock_minute, &fb_info, LAVNDER);
		show_string(450, 540,":", &fb_info, LAVNDER);
		show_string(470, 540, clock_second, &fb_info, LAVNDER);
#endif 
#if 1
		clock_time(&clock,&fb_info);
		sprintf(clock_hour, "%u", clock.hour);
		sprintf(clock_minute, "%u", clock.minute);
		sprintf(clock_second, "%u", clock.second);
		show_string(340, 540, clock_hour, &fb_info, LAVNDER);
		show_string(385, 540,":", &fb_info, LAVNDER);
		show_string(405, 540, clock_minute, &fb_info, LAVNDER);
		show_string(450, 540,":", &fb_info, LAVNDER);
		show_string(470, 540, clock_second, &fb_info, LAVNDER);
		//printf("\r%02d:%02d:%02d",clock.hour,clock.minute,clock.second);//%02d中0 输出数值时指定左面不使用的空位置自动填0，达到00:00:00效果  
		fflush(stdout);//printf属于行缓冲，遇到\n或程序结束才会输出，这里没有\n，所以用fflush刷新；   		
#endif
		//显示温湿度
		sprintf(tem, "%u", zgb_env.data.env_msg.tem.htem);
		sprintf(hum, "%u", zgb_env.data.env_msg.hum.hhum);
		clear_screen(190,46, 400, 50, WHITE, &fb_info);
		show_string(190, 400, tem, &fb_info, GREEN_YELLOW);//显示温度
		clear_screen(190,48, 480, 50, WHITE, &fb_info);
		show_string(190, 480, hum, &fb_info,GREEN_YELLOW);//显示湿度
    }
    pthread_exit(NULL);
}


