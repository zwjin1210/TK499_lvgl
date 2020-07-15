#include "test.h"


/**
  * @brief  ESP8266 ��Sta Tcp Client��͸��
  * @param  ��
  * @retval ��
  */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	uint8_t ucId, ucLen;
	uint8_t ucLed1Status = 0, ucLed2Status = 0, ucLed3Status = 0, ucBuzzerStatus = 0;

	char cStr [ 100 ] = { 0 }, cCh;

    char * pCh, * pCh1;

    printf ( "\r\n�������� ESP8266 ......\r\n" );

	macESP8266_CH_ENABLE();
	
	ESP8266_AT_Test ();
	
	ESP8266_Net_Mode_Choose ( AP );

    while ( ! ESP8266_CIPAP ( macUser_ESP8266_TcpServer_IP ) );

    while ( ! ESP8266_BuildAP ( macUser_ESP8266_BulitApSsid, macUser_ESP8266_BulitApPwd, macUser_ESP8266_BulitApEcn ) );	
	
	ESP8266_Enable_MultipleId ( ENABLE );
	
	while ( !	ESP8266_StartOrShutServer ( ENABLE, macUser_ESP8266_TcpServer_Port, macUser_ESP8266_TcpServer_OverTime ) );

	ESP8266_Inquire_ApIp ( cStr, 20 );
	printf ( "\r\n��ģ��WIFIΪ%s�����뿪��\r\nAP IP Ϊ��%s�������Ķ˿�Ϊ��%s\r\n�ֻ������������Ӹ� IP �Ͷ˿ڣ���������5���ͻ���\r\n",
           macUser_ESP8266_BulitApSsid, cStr, macUser_ESP8266_TcpServer_Port );
	
	
	while ( 1 )
	{		
		if (Ux_rxdata.Flag)
		{
			UART_ITConfig(UART2, UART_IT_RXIEN, DISABLE); //���ô��ڽ����ж�
			Ux_rxdata.Data_rx_buf[Ux_rxdata.Data_count] = '\0';
			printf("%s\n",Ux_rxdata.Data_rx_buf);
			
			if ( ( pCh = strstr ( Ux_rxdata.Data_rx_buf, "S" ) ) != 0 ) 
			{
				cCh = * ( pCh + 8 );
				printf("%c\n",cCh);
				switch(cCh)
				{
					case 'd':GPIOA->ODR &=~ (1<<15);break;
					case 'e':GPIOA->ODR |= 1<<15;break;
					default:break;
 				}
			}
		    Ux_rxdata.Data_count = 0;
	        Ux_rxdata.Flag = 0;	
			
			UART_ITConfig(UART2, UART_IT_RXIEN, ENABLE); //���ô��ڽ����ж�; //ʹ�ܴ��ڽ����ж�
		}
	}
	
		
}


