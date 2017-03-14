/*	
*	Qt / Andriod 服务器的子线程
*	功能:解析处理客户端发来的命令，并处理客户端请求
*/

#include "srv.h"
unsigned char pic_tem[1024*100];

void *thread_client_handler(void *arg)
{
	
	int android_ret,ret,i;
	int confd = *(int *)arg;
	char cmd_buf[CMD_BUF_SIZE];
	char data_buf[DATA_BUF_SIZE];	
	unsigned char android_recv_buf[9];
	unsigned char android_send_buf[9];	
	unsigned char *android_cmd_buf = android_send_buf[4];
	pthread_t vcr_tid;
	
	while(1)
	{	
		/*--------------------------Android---------------------------*/
		if((android_ret = recv(confd, android_recv_buf, 7, 0) ) < 0)
		{
			perror("android client_handler recv：");
			pthread_exit(NULL);
		}
		if(0 == android_ret)
		{
			printf("android client shutdown connection\n");
			pthread_exit(NULL);
		}
		for (i=0;i<7;i++)
		{
			printf("android_recv_buf =%d\n",android_recv_buf[i]);
		}	
			
		/*-----------------------------QT---------------------------*/

		/*等待客户端请求*/
		if((ret = recv(confd, cmd_buf, CMD_BUF_SIZE-1, 0) ) < 0)
		{
			perror("client_handler recv：");
			pthread_exit(NULL);
		}
		if(0 == ret)
		{
			printf("client shutdown connection\n");
			pthread_exit(NULL);
		}
		
		/*命令解析*/
		if(strncmp(cmd_buf, "tem", 3) == 0)		//温度 temperature
		{
			pthread_rwlock_rdlock(&zgb_mutex);
			memset(data_buf, 0, DATA_BUF_SIZE);
			sprintf(data_buf, "%s", "tem");		//加协议头
			printf("tem: %u\n", zgb_env.data.env_msg.tem.htem);
			sprintf(data_buf+3, "%u", zgb_env.data.env_msg.tem.htem); 
			sprintf(data_buf+5, "%s", "wet");		//加协议头
			sprintf(data_buf+8, "%u",  zgb_env.data.env_msg.hum.hhum);
			pthread_rwlock_unlock(&zgb_mutex);
			writen(confd, data_buf, strlen(data_buf)+1);	//强制发送n个数据
			printf("client get temperature and humidity successfully\n");
			continue;
		}
#if 0
		if(strncmp(cmd_buf, "wet", 3) == 0)		//湿度 wet
		{	
			memset(data_buf, 0, DATA_BUF_SIZE);		
			sprintf(data_buf, "%s", "wet");		//加协议头
			printf("hum: %u\n", zgb_env.data.env_msg.hum.hhum);
			sprintf(data_buf+3, "%u",  zgb_env.data.env_msg.hum.hhum);
			writen(confd, data_buf, strlen(data_buf)+1);
			printf("client get wet successfully\n");
			continue;
		}
#endif
	
		if(strncmp(cmd_buf, "led_on", 6) == 0)	
		{
			pthread_mutex_lock(&M0ctr_mutex);
			env_send(zgb_fd, '0');   			//打开led
			pthread_mutex_unlock(&M0ctr_mutex);
			printf("led on\n");
			continue;
		}
		if(strncmp(cmd_buf, "led_off", 7) == 0)
		{
			pthread_mutex_lock(&M0ctr_mutex);
			env_send(zgb_fd, '1');    			//关闭led
			pthread_mutex_unlock(&M0ctr_mutex);
			printf("led off\n");
			continue;
		}
		if(strncmp(cmd_buf, "fan_on", 6) == 0)  //风扇开
		{
			pthread_mutex_lock(&M0ctr_mutex);
			env_send(zgb_fd, '4');
			pthread_mutex_unlock(&M0ctr_mutex);
			printf("fan on\n");
			continue;
		}
		if(strncmp(cmd_buf, "fan_off", 7) == 0) //风扇关
		{
			pthread_mutex_lock(&M0ctr_mutex);
			env_send(zgb_fd, '5');
			pthread_mutex_unlock(&M0ctr_mutex);
			printf("fan off\n");
			continue;
		}
		if(strncmp(cmd_buf, "hum_on", 6) == 0)  //蜂鸣器开
		{	
			pthread_mutex_lock(&M0ctr_mutex);
			env_send(zgb_fd, '2');
			pthread_mutex_unlock(&M0ctr_mutex);
			printf("hum on\n");
			continue;
		}
		if(strncmp(cmd_buf, "hum_off", 7) == 0)	//蜂鸣器关
		{
			pthread_mutex_lock(&M0ctr_mutex);
			env_send(zgb_fd, '3');
			pthread_mutex_unlock(&M0ctr_mutex);
			printf("hum off\n");
			continue;
		}
		if(strncmp(cmd_buf, "dig_on", 6) == 0) //数码管开
		{
			pthread_mutex_lock(&M0ctr_mutex);
			env_send(zgb_fd, '6');
			pthread_mutex_unlock(&M0ctr_mutex);
			printf("dig on\n");
			continue;
		}
		if(strncmp(cmd_buf, "dig_off", 7) == 0) //数码管关
		{
			pthread_mutex_lock(&M0ctr_mutex);
			env_send(zgb_fd, '7');
			pthread_mutex_unlock(&M0ctr_mutex);
			printf("dig off\n");
			continue;
		}
	
		if(strncmp(cmd_buf, "vcr", 3) == 0)	//视频(发一张图片)
		{
			#if 1
			//printf("client want video\n");
			pthread_rwlock_rdlock(&cam_jpeg_mutex); 
			sprintf(pic_tem, "%ld", v4l2_use.jpeg_size);
			//sprintf(pic_tem, "%ld",1024);
			memcpy(pic_tem+32, (unsigned char *)v4l2_use.jpeg, v4l2_use.jpeg_size);
			//writen(confd, (unsigned char *)v4l2_use.jpeg, v4l2_use.jpeg_size);
			writen(confd, pic_tem,sizeof(pic_tem));
			pthread_rwlock_unlock(&cam_jpeg_mutex); 
			#endif	
			continue;
		}
	}
}

#if 0
//视频发送线程
void *thread_vcr_send(void *arg)
{
	int ret;
	int confd = *(int *)arg;
	char vcr_cmd[10];
	while(1)
	{
		pthread_rwlock_rdlock(&cam_jpeg_mutex); 
		writen(confd, (unsigned char *)v4l2_use.jpeg, v4l2_use.jpeg_size);
		pthread_rwlock_unlock(&cam_jpeg_mutex); 
	
		/*等待客户端请求*/
		if((ret = recv(confd, vcr_cmd, CMD_BUF_SIZE-1, 0) ) < 0)
		{
			perror("client_handler recv：");
			pthread_exit(NULL);
		}
		if(0 == ret)
		{
			printf("client shutdown connection\n");
			pthread_exit(NULL);
		}
		if(strncmp(vcr_cmd, "vcr", 3) == 0)
		{
			continue;
		}
		break;
	}
	printf("rgb thread exit\n");
	pthread_exit(NULL);
}
#endif