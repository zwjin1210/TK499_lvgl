#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "TK499_Timer.h"
#include "HAL_conf.h"
#include "UART.h"
#include "string.h"
#include "stdio.h"



#define   macESP8266_CH_ENABLE()             GPIO_SetBits ( GPIOD, GPIO_Pin_1 )
#define   macESP8266_CH_DISABLE()            GPIO_ResetBits ( GPIOD, GPIO_Pin_1 )

#define   macESP8266_RST_HIGH_LEVEL()        GPIO_SetBits ( GPIOD, GPIO_Pin_0 )
#define   macESP8266_RST_LOW_LEVEL()         GPIO_ResetBits ( GPIOD, GPIO_Pin_0 )

typedef enum{
	STA,
  AP,
  STA_AP  
} ENUM_Net_ModeTypeDef;

typedef enum{
	 enumTCP,
	 enumUDP,
} ENUM_NetPro_TypeDef;

typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;
	

typedef enum{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;

void ESP8266_GPIO_Config(void);
void                     ESP8266_Init                        ( void );
void                     ESP8266_Rst                         ( void );
u8                     ESP8266_Cmd                         ( char * cmd, char * reply1, char * reply2, u32 waittime );
void                     ESP8266_AT_Test                     ( void );
u8                     ESP8266_Net_Mode_Choose             ( ENUM_Net_ModeTypeDef enumMode );
u8                     ESP8266_JoinAP                      ( char * pSSID, char * pPassWord );
u8                     ESP8266_BuildAP                     ( char * pSSID, char * pPassWord, ENUM_AP_PsdMode_TypeDef enunPsdMode );
u8                     ESP8266_Enable_MultipleId           ( FunctionalState enumEnUnvarnishTx );
u8                     ESP8266_Link_Server                 ( ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
u8                     ESP8266_StartOrShutServer           ( FunctionalState enumMode, char * pPortNum, char * pTimeOver );
uint8_t                  ESP8266_Get_LinkStatus              ( void );
uint8_t                  ESP8266_Get_IdLinkStatus            ( void );
uint8_t                  ESP8266_Inquire_ApIp                ( char * pApIp, uint8_t ucArrayLength );
u8                     ESP8266_UnvarnishSend               ( void );
void                     ESP8266_ExitUnvarnishSend           ( void );
u8                     ESP8266_SendString                  ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId );
char *                   ESP8266_ReceiveString               ( FunctionalState enumEnUnvarnishTx );

uint8_t                  ESP8266_CWLIF                       ( char * pStaIp );
uint8_t                  ESP8266_CIPAP                       ( char * pApIp );












	
#endif

