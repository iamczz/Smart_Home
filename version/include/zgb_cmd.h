/*************************************************************************
    > File Name: zgb_cmd.h
    > Author: 龙创科技
    > Mail: ouxiaolong88@163.com 
    > Created Time: 2016年07月03日 星期日 03时38分44秒
 ************************************************************************/
#ifndef _ZGB_CMD_H_
#define _ZGB_CMD_H_

#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <poll.h>
#include <sys/ioctl.h>


typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

#define  start_machine_t 0xAA	 //开机
#define  data_flow_t	 0xBB	 //数据采集
#define  rfid_msg_t      0xCC	 //rfid信息
#define  command_tag_t	 0xDD	 //命令
#define  key_msg_t	     0xEE    //按键
#define  other_type_t	 0x00	 //其他（未定义）
#define  MO_DEV_ID       0x05	 //M0的ID号

typedef struct _command_t{
	uint8_t operate_id;	//define the object operated
	uint8_t operation;	//define the action object should do 
}command_t;

typedef enum _message_tag_t{
	start_machine	= start_machine_t,
	data_flow	= data_flow_t,	
	rfid_msg	= rfid_msg_t,
	command		= command_tag_t,	
	key		= key_msg_t,
	other_type	= other_type_t
}message_tag_t;			//标记消息类型

typedef struct _tem_t{//温度信息(2bytes)
	uint8_t ltem;
	uint8_t htem;
}tem_t;					

typedef struct _hum_t{//湿度信息(2bytes)
	uint8_t lhum;
	uint8_t hhum;
}hum_t;

typedef struct _light_t{//光照信息(4bytes)	
	uint32_t light;
}light_t;

typedef struct _acc_t{	//三轴加速度信息(3bytes)
	char x;
	char y;
	char z;
}acc_t;
		
typedef struct _adc_t{	//A/D转换信息(8bytes)	
	uint32_t ad0;		//data from A/D channel 0
	uint32_t ad3;		//data from A/D channel 1
	
}adc_t;
	
typedef struct _state_t	{//设备状态(4bytes)
	uint8_t led_state;
	uint8_t fan_state;
	uint8_t buzz_state;
	uint8_t seven_led_state;
}state_t;
		
typedef struct _env_msg_t{//环境信息
	tem_t tem;		//温度（2bytes）
	hum_t hum;		//湿度（2bytes）
	acc_t acc;		//三轴加速度(3bytes)
	adc_t adc;		//A/D转换(8bytes)
	light_t light;		//光照值(4bytes)
	state_t state;		//设备状态(4bytes)
}env_msg_t;

typedef union _data_t {//可选择发送的信息类型
	command_t command;
	env_msg_t env_msg;	
}data_t;

typedef struct	{//发送消息格式		
	uint8_t tag;		//数据类型(1byte)	
	uint8_t slave_address;	//从设备地址(1byte)		
	uint8_t data_length;	//数据长度  (1byte)
	data_t data;		//数据正文(23bytes)	
	uint16_t crc;		//2bytes			
}message_t;
		
#define BUFFER_SIZE  36
#define M0_DEV_ID 0X05           //MO的ID号

//控制命令ID
#define on_led1			0
#define off_led1		1
#define on_speaker		2
#define off_speaker		3
#define on_fan			4
#define off_fan		    5	
#define on_seven_led    6
#define off_seven_led	7

int env_send(int fd ,char ch);
int env_recv(int fd);

#endif


