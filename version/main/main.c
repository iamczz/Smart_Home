#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "cam.h"
#include "zgb.h"
#include "gsm.h"
#include "lcd.h"
#include "srv.h"
#include "cam_cmd.h"
#include "pth_time.h"

typedef struct sockaddr SA;
typedef struct sockaddr_in SA_in;

//线程号
pthread_t cam_tid; 
pthread_t time_tid;     
pthread_t zgb_tid;
pthread_t gsm_tid;
pthread_t srv_tid;

pthread_rwlock_t cam_mutex;
pthread_rwlock_t zgb_mutex;
pthread_rwlock_t gsm_mutex;
pthread_rwlock_t zgbenv_mutex;		//zgb采集的环境信息读写锁
pthread_rwlock_t gsmcmd_mutex;		//gsm采集的指令读写锁
pthread_rwlock_t cam_jpeg_mutex;	//camare rgb的缓存读写锁

pthread_mutex_t M0ctr_mutex;
pthread_mutex_t sync_mutex;

int zgb_fd = -1;					//zigbee串口描述符；
int gsm_fd = -1;					//gsm串口描述符

message_t zgb_env;					//zigbee接收缓存；
char gsm_cmd[512];					//GSM接收指令
V4L2 v4l2_use;						//图片缓存；

int main(int argc, char *argv[])
{
	int ret;
	
	pthread_rwlock_init(&cam_mutex, NULL);
	pthread_rwlock_init(&zgb_mutex, NULL);
	pthread_rwlock_init(&gsm_mutex, NULL);	
	pthread_mutex_init(&sync_mutex, NULL);
	
	/*================ zigbee模块 ==================*/
#if 1	
	zgb_fd = init_port(DEVICE_M0);      //初始化zgb串口
	printf("init zgb_port success\n");
	if(zgb_fd < 0)
	{
		printf("open zgb_port failed\n");
	}
	else
	{
		/* zigbee thread */
		ret = pthread_create(&zgb_tid, NULL, thread_zgb, NULL);
		if (ret) 
		{
			errno = ret;
			perror("create zigbee thread");
			exit(EXIT_FAILURE);
		} 
		else
			printf("create zigbee thread success\n");
	}
	
	/*================== gsm模块 =====================*/
	gsm_fd = init_port(DEVICE_GSM);     //初始化gsm串口
	printf("init gsm_port success\n");
	if( (gsm_fd < 0 ) || (zgb_fd < 0) )
	{
		printf("open gsm_port or zgb_port failed\n");
	}
	else
	{
		/* gsm thread */
		ret = pthread_create(&gsm_tid, NULL, thread_gsm, NULL);
		if (ret) 
		{
			errno = ret;
			perror("create gsm thread");
			exit(EXIT_FAILURE);
		} 
		else
		printf("create gsm thread success\n");
	}
	if((zgb_fd < 0) && (gsm_fd < 0))
	{
		exit(EXIT_FAILURE);
	}
#endif
	
	/*================== camera模块 =====================*/	
#if 1
	v4l2_use.open();//初始化cam串口
	ret = pthread_create(&cam_tid, NULL, thread_cam, NULL);
	if (ret) 
	{
		errno = ret;
		perror("create camera thread");
		exit(EXIT_FAILURE);
	} 
	else
		printf("create camera thread success\n");
#endif

	/*================== time模块 =====================*/	
#if 1
	ret = pthread_create(&time_tid, NULL, thread_time, NULL);
	if (ret) 
	{
		errno = ret;
		perror("create time thread");
		exit(EXIT_FAILURE);
	} 
	else
		printf("create time thread success\n");
#endif

	/*==============android/Qt 服务器模块=================*/
#if 1
	ret = pthread_create(&srv_tid, NULL, thread_srv, NULL);
	if (ret) 
	{
		errno = ret;
		perror("create android/Qt server thread\n");
		exit(EXIT_FAILURE);
	} else
		printf("create android/Qt server thread success\n");
#endif
	
	/* main thread, process environment data form m0 board or process short message */
	while (1) 
	{
		struct gsm_buf_t msg;
		message_t env;
		message_t zgb;

		pthread_rwlock_rdlock(&zgb_mutex);   
		memcpy(&env, &zgb, sizeof(message_t));
		pthread_rwlock_unlock(&zgb_mutex);
		pthread_rwlock_rdlock(&gsm_mutex);	
		memcpy(&msg, &gsm, sizeof(struct gsm_buf_t));
		pthread_rwlock_unlock(&gsm_mutex);

		if (msg.rflag) 
		{
			msg.rflag = 0;
		}
		sleep(1);
	}

	close(zgb_fd);
	close(gsm_fd);
	
	pthread_rwlock_destroy(&cam_mutex);
	pthread_rwlock_destroy(&zgb_mutex);
	pthread_rwlock_destroy(&gsm_mutex);

	exit(EXIT_SUCCESS);
}
