#include "gsm.h"

struct gsm_buf_t gsm;

void *thread_gsm(void *arg)
{	
	char cmd[512] = {0};

	while(1) 
	{			
		gsm_recv(gsm_fd, "0", "1");			//接收指令
		memset(cmd, 0, sizeof(cmd));
		pthread_rwlock_rdlock(&gsmcmd_mutex);
		memcpy(cmd,gsm_cmd,strlen(gsm_cmd));
		pthread_rwlock_unlock(&gsm_mutex);
		
		if(strncmp(cmd, "0", 1) == 0)	
		{
			pthread_mutex_lock(&M0ctr_mutex);
			env_send(zgb_fd, '0'); 			//打开led
			pthread_mutex_unlock(&M0ctr_mutex);
			printf("led on\n");
			continue;
		}
		if(strncmp(cmd, "1", 1) == 0)
		{	
			pthread_mutex_lock(&M0ctr_mutex);
			env_send(zgb_fd, '1');    			//关闭led
			pthread_mutex_unlock(&M0ctr_mutex);
			printf("led off\n");
			continue;
		}
		if(strncmp(cmd, "2", 1) == 0)  		//蜂鸣器开
		{	
			pthread_mutex_lock(&M0ctr_mutex);
			env_send(zgb_fd, '2');
			pthread_mutex_unlock(&M0ctr_mutex);
			printf("hum on\n");
			continue;
		}
		if(strncmp(cmd, "3", 1) == 0)		//蜂鸣器关
		{
			pthread_mutex_lock(&M0ctr_mutex);
			env_send(zgb_fd, '3');
			pthread_mutex_unlock(&M0ctr_mutex);
			printf("hum off\n");
			continue;
		}
		if(strncmp(cmd, "4", 1) == 0)  		//风扇开
		{
			pthread_mutex_lock(&M0ctr_mutex);
			env_send(zgb_fd, '4');
			pthread_mutex_unlock(&M0ctr_mutex);
			printf("fan on\n");
			continue;
		}
		if(strncmp(cmd, "5", 1) == 0) 		//风扇关
		{
			pthread_mutex_lock(&M0ctr_mutex);
			env_send(zgb_fd, '5');
			pthread_mutex_unlock(&M0ctr_mutex);
			printf("fan off\n");
			continue;
		}
		if(strncmp(cmd, "6", 1) == 0) 		//数码管开
		{
			pthread_mutex_lock(&M0ctr_mutex);
			env_send(zgb_fd, '6');
			pthread_mutex_unlock(&M0ctr_mutex);
			printf("fan off\n");
			continue;
		}
		if(strncmp(cmd, "7", 1) == 0) 		//数码管关
		{
			pthread_mutex_lock(&M0ctr_mutex);
			env_send(zgb_fd, '7');
			pthread_mutex_unlock(&M0ctr_mutex);
			printf("fan off\n");
			continue;
		}
	}
	printf("gsm thread exited\n");
	pthread_exit(NULL);
}
