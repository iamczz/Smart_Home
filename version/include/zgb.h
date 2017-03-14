#ifndef _ZGB_H_
#define _ZGB_H_

#include "zgb_cmd.h"
#include "mutexs.h"
#include "uart_api.h"

extern message_t zgb_env;	//接收缓存	
extern int zgb_fd;			//zigbee串口描述符 

void *thread_zgb(void *arg);

#endif

