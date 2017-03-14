/************************************************************************
 *开发者：龙创科技
 *开发平台：A9
 *File Name: gsm_cmd.c
 *日期：2016.7
 *内容：GSM相关
 ***********************************************************************/
#include "gsm_cmd.h"
#include "uart_api.h"

/***********************************************************************
 * @函数名：gsm_send
 * @输入参数：
 * @		fd：串口文件描述符
 * @		cmgf： 发送模式0是中文;1是英文
 * @		cmgs： 
 * @		message: 发送内容
 * @函数功能：发送函数
 ***********************************************************************/
int gsm_send(int fd,char *cmgf,char *cmgs,char *message)
{
    int nread,nwrite;
    char send_buff[128];
    char recv_buff[128];
	char buff[1]={0x1a};

	//AT
	memset(send_buff,0,sizeof(send_buff));
	strcpy(send_buff,"AT\r");
    nwrite = write(fd,send_buff,strlen(send_buff));
    printf("nwrite=%d,%s\n",nwrite,send_buff);

	//waiting for AT's ACK等待回应
	memset(recv_buff,0,sizeof(recv_buff));
    sleep(1);
    nread = read(fd,recv_buff,sizeof(recv_buff));
	printf("nread=%d,%s\n",nread,recv_buff);

	//AT +CMGF=0,发送PDU"确定模式；0中文；1英文
	memset(send_buff,0,sizeof(send_buff));
    strcpy(send_buff,"AT+CMGF=");
    strcat(send_buff,cmgf);
    strcat(send_buff,"\r");
    nwrite = write(fd,send_buff,strlen(send_buff));
    printf("nwrite=%d,%s\n",nwrite,send_buff);

	//waiting for AT's ACK等待回应
	memset(recv_buff,0,sizeof(recv_buff));
    sleep(1);
    nread = read(fd,recv_buff,sizeof(recv_buff));
    printf("nread=%d,%s\n",nread,recv_buff);

	//AT + CMGF=1,set to phone发送号码;AT+CMGF=0,set to len/2
	memset(send_buff,0,sizeof(send_buff));
    strcpy(send_buff,"AT+CMGS=");
    strcat(send_buff,cmgs);
    strcat(send_buff,"\r");
    nwrite = write(fd,send_buff,strlen(send_buff));
    printf("nwrite=%d,%s\n",nwrite,send_buff);
	printf("================3======================\n");
	//waiting for AT's ACK等待回应
	memset(recv_buff,0,sizeof(recv_buff));
    sleep(1);
    nread = read(fd,recv_buff,sizeof(recv_buff));
    printf("nread=%d,%s\n",nread,recv_buff);
	printf("=================2===================\n");
	//AT+CMGS=27or29发送字节位27或29
	memset(send_buff,0,sizeof(send_buff));
    strcpy(send_buff,message);
    nwrite = write(fd,send_buff,strlen(send_buff));
    printf("nwrite=%d,%s\n",nwrite,send_buff);
	printf("=================1===================\n");

	//waiting for AT+CMGS=27or9's ACK等待回应
	memset(recv_buff,0,sizeof(recv_buff));
    sleep(1);
    nread = read(fd,recv_buff,sizeof(recv_buff));
    printf("nread=%d,%s\n",nread,recv_buff);

	//发送结束符
	memset(send_buff,0,sizeof(send_buff));
    strcpy(send_buff,buff);
	//strcat(send_buff,"\x1A");
    nwrite = write(fd,send_buff,strlen(send_buff));
    printf("nwrite=%d,%s\n",nwrite,send_buff);
	
	//等待回应
	memset(recv_buff,0,sizeof(recv_buff));
    sleep(10);
    nread = read(fd,recv_buff,sizeof(recv_buff));
    printf("nread=%d,%s\n",nread,recv_buff);
		
	printf("----------send msg success-----------\n");
}

/***********************************************************************
 * @函数名：gsm_recv
 * @输入参数：
 * @		fd：串口文件描述符
 * @		key ： 
 * @		num ： 
 * @函数功能：接收函数
 ***********************************************************************/
int gsm_recv(int fd,char *key,char *num)
{
	int nread,nwrite;
    char send_buff[128];
    char recv_buff[1024];
	char recv_phonenumber[14];
	char phone_buff[1024];
	char buff[1024];
	int m = 0, i= 0 ,j =0;
	char cmd_buff[1024];
	char check_buff[1024];
		
	memset(send_buff,0,sizeof(send_buff));
	strcpy(send_buff,"AT\r");
    nwrite = write(fd,send_buff,strlen(send_buff));
    printf("nwrite=%d,%s\n",nwrite,send_buff);

	//waiting for AT's ACK等待回应
	memset(recv_buff,0,sizeof(recv_buff));
    sleep(1);
    nread = read(fd,recv_buff,sizeof(recv_buff));
	printf("nread=%d,%s\n",nread,recv_buff);
		
	//AT +CMGF=0,发送PDU"确定模式；0中文；1英文
	memset(send_buff,0,sizeof(send_buff));
    strcpy(send_buff,"AT+CMGF=");
    strcat(send_buff,key);
    strcat(send_buff,"\r");
    nwrite = write(fd,send_buff,strlen(send_buff));
    printf("nwrite=%d,%s\n",nwrite,send_buff);

	//waiting for AT's ACK等待回应
	memset(recv_buff,0,sizeof(recv_buff));
    sleep(1);
    nread = read(fd,recv_buff,sizeof(recv_buff));
    printf("nread=%d,%s\n",nread,recv_buff);
		
	//读取短信
	while(nread < 50 )
	{
		memset(send_buff,0,sizeof(send_buff));
		strcpy(send_buff,"AT+CMGR=");
		strcat(send_buff,num);
		strcat(send_buff,"\r");
		nwrite = write(fd,send_buff,strlen(send_buff));
		//printf("nwrite=%d,send_buff=%s\n",nwrite,send_buff);

		//waiting for AT's ACK等待回应
		memset(recv_buff,0,sizeof(recv_buff));
		sleep(4);
		nread = read(fd,recv_buff,sizeof(recv_buff));
		//printf("nread=%d,recv_buff=%s\n",nread,recv_buff);
	}	
	
	while(recv_buff[j+55] != '\0')
	{
		check_buff[j] = recv_buff[j+55];
		j++;
	}
	check_buff[2]='\0';
	printf("1---%d,get_check=%s\n",strlen(check_buff),check_buff);
	
	if ( 0 == strcmp(check_buff,"68"))
	{
		//提取短信内容编码(移动)
		while(recv_buff[m+90] != '\0')
		{
			buff[m] = recv_buff[m+90];
			m++;
		}
		printf("The phone is mobile.\n");	
		//提取发信人信息+短信内容
		while(recv_buff[i+57] != '\0')//移动
		{
			phone_buff[i] = recv_buff[i+57];
			i++;
		}		
	}
	else
	{
		//提取短信内容编码(联通)
		while(recv_buff[m+88] != '\0')
		{
			buff[m] = recv_buff[m+88];
			m++;
		}
		printf("The phone is unicom\n");
		while(recv_buff[i+55] != '\0')//联通
		{
			phone_buff[i] = recv_buff[i+55];
			i++;
		}
	}

	//解析指令
	printf("1---%d,get_code=%s\n",strlen(buff),buff);
	strcpy(cmd_buff, buff);
	
	pthread_rwlock_wrlock(&gsmcmd_mutex);
	memcpy(gsm_cmd, cmd_buff, sizeof(cmd_buff));				//拷贝指令
	pthread_rwlock_unlock(&gsm_mutex);
	
	printf("The cmd is %s\n",cmd_buff);
	//解析电话号码
	buff[m-8]='\0';
    printf("1---%d,get_phone_msg=%s\n",strlen(phone_buff),phone_buff);
	phone_buff[i-m-20]='\0';
	swap_back(recv_phonenumber, phone_buff);
	printf("2---%d,get_code_phonenumber=%s\n",strlen(phone_buff),phone_buff);
	printf("2---%d,get_phone_number=%s\n",strlen(recv_phonenumber), recv_phonenumber);
	
#if 0 	
	//删除短信
	memset(send_buff,0,sizeof(send_buff));
    strcpy(send_buff,"AT+CMGD=1");
    strcat(send_buff,"\r");
    nwrite = write(fd,send_buff,strlen(send_buff));
    printf("nwrite=%d,%s\n",nwrite,send_buff);

	//删除短信等待回应
	memset(recv_buff,0,sizeof(recv_buff));
    sleep(1);
    nread = read(fd,recv_buff,sizeof(recv_buff));
    printf("nread=%d,%s\n",nread,recv_buff);
#endif		
	printf("----------recv msg success-----------\n");
}

/***********************************************************************
 * @函数名：gsm_send_english_message
 * @输入参数：
 * @		fd：串口文件描述符
 * @		phone:  接收方号码
 * @		message: 发送内容
 * @函数功能：发送英文函数
 ***********************************************************************/
int  gsm_send_english_message(int fd,char phone[16], char message[100])
{
        assert(NULL != phone &&  NULL != message);/* 检查传入参数*/
        struct message_info info;
        strcpy(info.phone, phone);
        strcpy(info.message, message);
        char cmgf[] = "1";
        char cmgs[16] = {'\0'};
		
		strcat(cmgs,info.phone);
		send(fd,cmgf,cmgs,info.message);
}

/***********************************************************************
 * @函数名：gsm_send_chinese_message
 * @输入参数：
 * @		fd：串口fd
 * @		phone:  接收方号码
 * @		notephone：短信服务中心号码
 * @		message: 发送内容
 * @函数功能：发送中文函数
 ***********************************************************************/
int gsm_send_chinese_message(int fd, char phone[16], char notephone[16], char message[100])
{
	assert(NULL != phone && NULL != notephone && NULL != message);/* 检查传入参数,它的条件返回错误，则终止程序执行*/
	struct message_info info;
	strcpy(info.phone, phone);
	strcpy(info.notephone, notephone);
	strcpy(info.message, message);
	
	char cmgf[] = "0";
	char cmgs[4] = {'\0'};
	char ch2[] = "0891";
	char ch3[] = "1100";
	char ch4[] = "000800";
	char ch5[] = "0D91";
	char final[128];
	struct pdu_info pdu;
	int len;

	memset(final,0,128);
	swap(info.phone,pdu.phone_swap);
	swap(info.notephone,pdu.notephone_swap);

	strcpy(final,ch2);
	strcat(final,pdu.notephone_swap);
	strcat(final,ch3);
	strcat(final,ch5);
	strcat(final,pdu.phone_swap);
	strcat(final,ch4);
	strcat(final,info.message); 

	len = strlen(ch3)+ strlen(ch4)+ strlen(ch5)+strlen(pdu.phone_swap)+ strlen(info.message);
	sprintf(cmgs,"%d",len/2);
	send(fd,cmgf,cmgs,final);
}

