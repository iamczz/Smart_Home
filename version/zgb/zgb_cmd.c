/************************************************************************
 *开发者：龙创科技
 *开发平台：A9
 *File Name: zigbee_cmd.c
 *日期：2016.7
 *内容：ZigBee设置发送与接收
 ***********************************************************************/
#include "zgb_cmd.h"
#include "zgb.h"

/***********************************************************************
 * @函数名：env_recv
 * @输入参数：fd
 * @函数功能：ZigBee接收函数
 ***********************************************************************/
int env_recv(int fd)
{
	static uint8_t msgbuf[BUFFER_SIZE];
	message_t *msg_r;
	uint8_t devid =0;
	printf("zgb start to receive data\n");							
	
	while(1)
	{			
		//接收环境信息	
		memset(msgbuf, 0, BUFFER_SIZE);					
		int len, num;							
		uint8_t *p = msgbuf;										
		num = read(fd, p, 1);		//读阻塞								
		if (0 > num)
		{												
			printf("recv fail.\n");											
		}												
		if (data_flow_t != p[0])
		{													
			printf("msg_r->tag = %.2x\n", p[0]);
		}														
		p++;
		len = BUFFER_SIZE-1;												
		while(len) 
		{	
			num = read(fd, p, len);
			if(0 > num)
			{
				printf("recv fail1.\n");
				return -1;
			}														
			len -= num;													
			p += num;													
		}
#if 1
		printf("recv:");										
		int i;
		for (i=0; i<BUFFER_SIZE; i++) 
		{
																		
			printf("%.2X", msgbuf[i]);
		}															
		printf("\n");
#endif 		
		msg_r = (message_t *)msgbuf;

		if (data_flow_t != msg_r->tag) 
		{														
			printf("msg_r->tag(%d) = %.2x\n", sizeof(msg_r->tag), msg_r->tag);
		}
#if 0														
		printf("env data:\n");	
		devid = msg_r->slave_address;
		printf("\tdev id: %x\n", msg_r->slave_address);
		printf("\ttotal len: %ubytes\n", msg_r->data_length);
		printf("\ttem: low=%u,hig=%u\n",\
				msg_r->data.env_msg.tem.ltem, msg_r->data.env_msg.tem.htem);
		printf("\thum: low=%u, hig=%u\n",\
				msg_r->data.env_msg.hum.lhum, msg_r->data.env_msg.hum.hhum);
		printf("\tlight =%d\n", msg_r->data.env_msg.light);
		printf("\tx=%d, y=%d, z=%d\n", \
				msg_r->data.env_msg.acc.x, \
				msg_r->data.env_msg.acc.y, \
				msg_r->data.env_msg.acc.z);
		printf("\tled_state=%x, fan_state=%x, buzz_state=%x, seven_led_state=%x\n",\
				msg_r->data.env_msg.state.led_state,\
				msg_r->data.env_msg.state.fan_state,\
				msg_r->data.env_msg.state.buzz_state,\
				msg_r->data.env_msg.state.seven_led_state);
		printf("\n\n");
#endif	
		pthread_rwlock_wrlock(&zgb_mutex);	
		memcpy(&zgb_env, msg_r, sizeof(message_t)); 	//把接收到的数据拷贝到全局区；
		pthread_rwlock_unlock(&zgb_mutex);
	}
	return 0;
}

/***********************************************************************
 * @函数名：env_send
 * @输入参数：fd
 * @函数功能：ZigBee发送函数
 ***********************************************************************/	
int env_send(int fd ,char ch)
{
	uint8_t msgbuf[BUFFER_SIZE];
	uint8_t devid = MO_DEV_ID;
	message_t *msg_s;
	int j;	
	
	//发送命令
	memset(msgbuf, 0, BUFFER_SIZE);
	msg_s = (message_t *)msgbuf;
	msg_s->tag = command_tag_t;	
	msg_s->slave_address = devid;
	msg_s->data_length = BUFFER_SIZE;
#if 0
	printf("0:on_led1.\n");         //开LED1灯
	printf("1:off_led1.\n");        //关LED1灯
	printf("2:on_speaker.\n");      //开蜂鸣器
	printf("3:off_speaker.\n");     //关蜂鸣器
	printf("4:on_fan.\n");          //开风扇
	printf("5:off_fan.\n");         //关风扇
	printf("6:on_seven_led.\n");    //开数码管
	printf("7:off_seven_led.\n");   //关数码管
#endif

	ch -= '0';
	if (0 <= ch && ch<= 4)
	{															
		printf("send:");
		msg_s->data.command.operate_id = ch;	
		int num = write(fd, msgbuf, BUFFER_SIZE);	
		for (j=0; j<BUFFER_SIZE; j++)
		{
			printf("%.2X", msgbuf[j]);
		}							
		printf("\n");		
		if (BUFFER_SIZE != num)
		{		
			printf("send fail.\n");
			return -1;;						
		}				
	}
	if (5<=ch && ch<=7)
	{
		printf("send:");
		msg_s->data.command.operate_id = (ch+3);
		int num = write(fd, msgbuf, BUFFER_SIZE);
		for (j=0; j<BUFFER_SIZE; j++) 
		{
			printf("%.2X", msgbuf[j]);
		}
		printf("\n");
		
		if (BUFFER_SIZE!= num) 
		{	
			printf("send fail.\n");
			return -1;						
		}
	}
	printf("\n\n");
	return 0;
}	

