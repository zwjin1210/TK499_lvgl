#ifndef _TEST_H_
#define _TEST_H_

#include "HAL_conf.h"
#include "UART.h"
#include "TK499_Timer.h"
#include "esp8266.h"
#include "string.h"
#include "led.h"
/********************************** �û���Ҫ���õĲ���**********************************/
#define   macUser_ESP8266_BulitApSsid         "BinghuoLink"      //Ҫ�������ȵ������
#define   macUser_ESP8266_BulitApEcn           OPEN               //Ҫ�������ȵ�ļ��ܷ�ʽ
#define   macUser_ESP8266_BulitApPwd           "wildfire"         //Ҫ�������ȵ����Կ

#define   macUser_ESP8266_TcpServer_IP         "192.168.123.169"      //������������IP��ַ
#define   macUser_ESP8266_TcpServer_Port       "6666"             //�����������Ķ˿�   

#define   macUser_ESP8266_TcpServer_OverTime   "1800"             //��������ʱʱ�䣨��λ���룩





/********************************** �ⲿȫ�ֱ��� ***************************************/
extern volatile uint8_t ucTcpClosedFlag;



/********************************** ���Ժ������� ***************************************/
void                     ESP8266_StaTcpClient_UnvarnishTest  ( void );






#endif


