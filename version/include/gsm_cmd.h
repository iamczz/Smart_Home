#ifndef _GSM_H_
#define _GSM_H_

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h>
#include <iconv.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include "gsm.h"

#define OUTLEN 255	

#if 0
struct content{
	int which;    			 //选择消息的序列号
	char to_message[1024];   //消息的内容
}content_message[]={
	{0, "002367E58BE2002072B66001"},		//#查询 
	{1, "002398CE62470020006F006E"},     	//#风扇 on
	{2, "002398CE62470020006F00660066"},    //#风扇 off
	{3, "00237535706F0020006F006E"},     	//#电灯 on
	{4, "00237535706F0020006F00660066"},    //#电灯 off
	{5, "002A62A58B6675358BDD002000310033003800380030003700390038003400340036"},     	//*报警电话 13880798446 
	{6, "002A4E2D5FC375358BDD002000310033003800380030003700390038003400340036"},		//*中心电话 13880798446
	{7, "002A6E295EA6002000330030002E00300030"},			//*温度 30.00
	{8, "002A6E7F5EA6002000330030002E00300030"} 			//*湿度 30.00
};
#endif

#if 1
struct send_to_message{
	int key;    				//选择消息的序列号
	char form_message[1024];    //发送消息的内容
}to_from[]={
	{0, "7535706F0020006F00660066"},		 //电灯 off
	{1, "7535706F0020006F006E"},    		 //电灯 on
	{2, "98CE62470020006F00660066"},		 //风扇 off
	{3, "98CE62470020006F006E"},   		 	 //风扇 on
	{4, "87029E2356680020006F00660066"},     //蜂鸣器 off
	{5, "87029E2356680020006F006E"},     	 //蜂鸣器 on
	{6, "4E098F7452A0901F5EA64FE1606F00200058003D"},     //三轴加速度信息 X=
	{7, "4E098F7452A0901F5EA64FE1606F00200059003D"},     //三轴加速度信息 Y=
	{8, "4E098F7452A0901F5EA64FE1606F0020005A003D"},     //三轴加速度信息 Z= 
	{9, "5F53524D6E295EA60020"},				//当前温度 
	{10,"5F53524D6E7F5EA60020"} 				//当前湿度
};
#endif

#if 1
struct number{
	int num;
	char ser_num[4];
}key_number[]={
	{0, "0030"},			//数字0
	{1, "0031"},			//数字1
	{2, "0032"},			//数字2
	{3, "0033"},			//数字3
	{4, "0034"},			//数字4
	{5, "0035"},			//数字5
	{6, "0036"},			//数字6
	{7, "0037"},			//数字7
	{8, "0038"},			//数字8
	{9, "0039"},			//数字9
	{10, "002D"},			//符号-
	{11, "002E"},			//小数点.
	{12, "2103"},			//符号℃
	{13, "0025"},			//符号%
	{14, "000A"}			//回车
};
#endif

struct message_info{
	char notephone[16];	//服务中心号码
	char phone[16];		//接收方号码
	char message[128];	//发送内容
        };

struct pdu_info {
        char notephone_swap[16];//服务中心号码
        char phone_swap[16];//接收方号码
        };
		
int head_len(char * head, int a);
void invert_swap(char *swap, int len);
int iconv_fun_to_unicode(char *input, char **output, int str_len);   
int covert(char *tocode, char *fromcode, char *input, size_t ilen, char *output, size_t olen);
unsigned long strtox(char* str);
int iconv_fun_to_utf8(char *input, char **output);
void swap_back(char number[], char swap[]);
void swap(char number[],char swap[]);
int test_unicode_to_utf8();
int test_utf8_to_unicode();

int gsm_send(int fd,char *cmgf,char *cmgs,char *message);
int gsm_recv(int fd,char *key,char *num);
int gsm_send_english_message(int fd,char phone[16], char message[100]);
int gsm_send_chinese_message(int fd, char phone[16], char notephone[16], char message[100]);

#endif 
