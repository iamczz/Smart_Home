#ifndef __GSM_H__
#define	__GSM_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "mutexs.h"
#include "uart_api.h"
#include "zgb.h"


struct gsm_buf_t {
	int rflag;
	char rnum[16];
	char rmsg[140];

	int sflag;
	char snum[16];
	char smsg[140];
};

extern int gsm_fd;			//gsm串口描述符 
extern char gsm_cmd[512];	//gsm接收指令 
extern struct gsm_buf_t gsm;
void *thread_gsm(void *arg);

#endif
