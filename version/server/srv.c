/*
*	Qt / Andriod 服务器
*	功能：create/bind/listen 套接字，客户端连接成功后，开一子线程来处理；
*/
#include "srv.h"

void *thread_srv(void *arg)
{
	int lisfd,confd;
	int len;
	int ret;
	SA_in servaddr, cliaddr;
	pthread_t client_tid;
	
	//创建一个套接字；
	lisfd = socket(AF_INET, SOCK_STREAM, 0);
	if(lisfd < 0)
	{
		perror("Socket create");
		pthread_exit(NULL);
	}
	
	//设置sockadd_in结构体中相关的参数
	bzero(&servaddr, sizeof(servaddr));  //把其他不用的位清零
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);	//端口号
//	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);	//任意可用的IP地址；
	servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);	//固定IP地址

	//绑定套接字属性
	if((bind(lisfd,(SA*)&servaddr, sizeof(servaddr))) < 0)
	{
		perror("bind");
		pthread_exit(NULL);
	}
	
	//设置监听队列
	if(listen(lisfd, 10) < 0)
	{
		perror("listen");
		pthread_exit(NULL);
	}

	while(1) 
	{
		//等待客户端连接；
		len = sizeof(cliaddr);
		if( (confd = accept(lisfd , (SA *)&cliaddr, &len)) < 0)
		{
			perror("accept");
			pthread_exit(NULL);
		}
		/* 子线程并发处理客户端请求*/
		//把confd传参给client_handler;
		ret = pthread_create(&client_tid, NULL, thread_client_handler, &confd);  
		if (ret) 
		{
			errno = ret;
			perror("create client_handler thread");
			pthread_exit(NULL);
		} 
		else
			printf("create client_handler thread success\n");

		ret = pthread_detach(client_tid); //解除父子关系,由系统自动回收资源;   
		if (ret) 
		{    				  	  //返回非零表示解除失败
			errno = ret;
			perror("detach client_handler thread");
			pthread_exit(NULL);
		} 
		else
			printf("detach client_handler thread success\n");
	}

	printf("server thread exited\n");
	pthread_exit(NULL);
}
