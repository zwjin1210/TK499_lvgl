#ifndef _TEST_H_
#define _TEST_H_

#include "HAL_conf.h"
#include "UART.h"
#include "TK499_Timer.h"
#include "esp8266.h"
#include "string.h"
#include "led.h"
/********************************** 用户需要设置的参数**********************************/
#define   macUser_ESP8266_BulitApSsid         "BinghuoLink"      //要建立的热点的名称
#define   macUser_ESP8266_BulitApEcn           OPEN               //要建立的热点的加密方式
#define   macUser_ESP8266_BulitApPwd           "wildfire"         //要建立的热点的密钥

#define   macUser_ESP8266_TcpServer_IP         "192.168.123.169"      //服务器开启的IP地址
#define   macUser_ESP8266_TcpServer_Port       "6666"             //服务器开启的端口   

#define   macUser_ESP8266_TcpServer_OverTime   "1800"             //服务器超时时间（单位：秒）





/********************************** 外部全局变量 ***************************************/
extern volatile uint8_t ucTcpClosedFlag;



/********************************** 测试函数声明 ***************************************/
void                     ESP8266_StaTcpClient_UnvarnishTest  ( void );






#endif


