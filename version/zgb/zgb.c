#include "zgb.h"

void *thread_zgb(void *arg)
{	
	//printf("init zgb_fd = %d\n",zgb_fd);
	env_recv(zgb_fd); 						//获取环境信息
	
	printf("zigbee thread exited\n");
	pthread_exit(NULL);
}
