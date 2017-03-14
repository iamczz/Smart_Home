#ifndef __SRV_H__
#define	__SRV_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "cam.h"
#include "zgb.h"
#include "srv.h"
#include "mutexs.h"

#define CMD_BUF_SIZE 128			//接收客户端请求的buf大小
#define DATA_BUF_SIZE 1024			//发送数组大小
#define SERVER_PORT 33333
#define SERVER_IP "192.168.5.250"

typedef struct sockaddr SA;
typedef struct sockaddr_in SA_in;

void *thread_srv(void *arg);
void *thread_vcr_send(void *arg);
void *thread_client_handler(void *arg);
ssize_t readn(int fd,void *vptr,size_t n);
ssize_t writen(int fd, const void *vptr, size_t n); 

#endif
