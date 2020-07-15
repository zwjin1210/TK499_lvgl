#include "tk499.h" 
#include "LCD.h"
#include "ASCII.h"
#include "GB1616.h"	//16*16汉字字模
#include "GB4848.h"	//48*48汉字字模

__align(256) u32 LTDC_Buf[XSIZE_PHYS*YSIZE_PHYS];

volatile void LCD_delay(volatile int time)  //简单软件延时
{
	volatile u32 i;	
	while(time--)
	for(i=500;i>0;i--);
}
void LTDC_Clock_Set(void)	//设置LTDC时钟
{
	RCC->AHB1ENR |= 1<<31;
	RCC->CR |= 1<<28;
	RCC->PLLDCKCFGR = 0x1<<16;  //分频系数 0~3 --> 2,4,6,8
	RCC->PLLLCDCFGR = 6<<6;   	//倍频系数
}
void GPIO_RGB_INIT(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;//定义GPIO初始化结构体变量

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE, ENABLE);
	
	//     配置4根同步信号线        DE=PB4, PCLK=PB5, HSYNC=PB6, VSYNC=PB7
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB,  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7, GPIO_AF_LTDC); //PB4~7复用为LTDC的同步信号线
	GPIO_PinAFConfig(GPIOE,  GPIO_Pin_All , GPIO_AF_LTDC); //GPIOE所有的IO全部复用为LTDC的数据线
	
	GPIO_PinAFConfig(GPIOE, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_16 | GPIO_Pin_17 , GPIO_AF_GPIO); //把GPIOE中没用于液晶屏的IO设置为普通 IO
	
	//                              SPI_SDA      SPI_CLK         LED       LCD_SPI_CS       BL         LCD_RST
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_16 | GPIO_Pin_17;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	  
}
static void LCD_Reset(void)
{
	//注意，现在科学发达，有的屏不用复位也行 
		LCD_RST(0);
    LCD_delay(200);					   
    LCD_RST(1);		 	 
    LCD_delay(200);					   

}
void LCD_WriteByteSPI(unsigned char byte)
{
    unsigned char n;
   
    for(n=0; n<8; n++)			
    {  
        if(byte&0x80) SPI_SDA(1)
        else SPI_SDA(0)
        byte<<= 1;
        SPI_DCLK(0);
        SPI_DCLK(1);
    }
}
void SPI_WriteComm(u16 CMD)//3线8bit 串行接口
{			
	LCD_SPI_CS(0);
	LCD_WriteByteSPI(0x70);
	LCD_WriteByteSPI(CMD);
	LCD_SPI_CS(1);
}
void SPI_WriteData(u16 tem_data)
{			
	LCD_SPI_CS(0);
	LCD_WriteByteSPI(0x72);
	LCD_WriteByteSPI(tem_data);
	LCD_SPI_CS(1);
}
void Lcd_Initialize(void)
{
    LCD_SPI_CS(1);
    LCD_delay(20);
    LCD_SPI_CS(0);
    LCD_Reset();

	
		SPI_WriteComm(0x20);//exit_invert_mode
		SPI_WriteComm(0x29);//set_display_on

		SPI_WriteComm(0xB1);//RGB Interface Setting
		SPI_WriteData(0x00);
		SPI_WriteData(0x14);
		SPI_WriteData(0x06);

		SPI_WriteComm(0xB2);//Panel Characteristics Setting
		SPI_WriteData(0x10);//480 pixels
		SPI_WriteData(0xC8);//800 pixels

		SPI_WriteComm(0xB3);//Panel Drive Setting    Set the inversion mode


		SPI_WriteData(0x00);//1-dot inversion 0x01

		SPI_WriteComm(0xB4);//Display Mode Control
		SPI_WriteData(0x04);//Dither disable.

		SPI_WriteComm(0xB5);//Display Mode and Frame Memory Write Mode Setting
		SPI_WriteData(0x10);
		SPI_WriteData(0x30);
		SPI_WriteData(0x30);
		SPI_WriteData(0x00);
		SPI_WriteData(0x00);

		SPI_WriteComm(0xB6);//Display Control 2 ( GIP Specific )
		SPI_WriteData(0x01);
		SPI_WriteData(0x18);
		SPI_WriteData(0x02);
		SPI_WriteData(0x40);
		SPI_WriteData(0x10);
		SPI_WriteData(0x00);

		SPI_WriteComm(0xc0);
		SPI_WriteData(0x01);
		SPI_WriteData(0x18);


		SPI_WriteComm(0xC3); 
		SPI_WriteData(0x03);
		SPI_WriteData(0x04);
		SPI_WriteData(0x03);
		SPI_WriteData(0x03);
		SPI_WriteData(0x03);

		LCD_delay(10);

		SPI_WriteComm(0xC4);//VDD Regulator setting
		SPI_WriteData(0x02);
		SPI_WriteData(0x23);//GDC AP
		SPI_WriteData(0x11);//VRH1  Vreg1out=1.533xVCI(10)
		SPI_WriteData(0x12);//VRH2  Vreg2out=-1.533xVCI(10)
		SPI_WriteData(0x02);//BT 5 VGH/VGL  6/-4
		SPI_WriteData(0x77);//DDVDH 6C//0x56
		LCD_delay(10);

		SPI_WriteComm(0xC5);
		SPI_WriteData(0x73);
		LCD_delay(10);

		SPI_WriteComm(0xC6);
		SPI_WriteData(0x24);//VCI 23
		SPI_WriteData(0x60);//RESET RCO 53
		SPI_WriteData(0x00);//SBC GBC
		LCD_delay(10);
		//GAMMA SETTING
		SPI_WriteComm(0xD0);
		SPI_WriteData(0x14);
		SPI_WriteData(0x01);
		SPI_WriteData(0x53);
		SPI_WriteData(0x25);
		SPI_WriteData(0x02);
		SPI_WriteData(0x02);
		SPI_WriteData(0x66);
		SPI_WriteData(0x14);
		SPI_WriteData(0x03);

		SPI_WriteComm(0xD1);
		SPI_WriteData(0x14);
		SPI_WriteData(0x01);
		SPI_WriteData(0x53);
		SPI_WriteData(0x07);
		SPI_WriteData(0x02);
		SPI_WriteData(0x02);
		SPI_WriteData(0x66);
		SPI_WriteData(0x14);
		SPI_WriteData(0x03);



		SPI_WriteComm(0xD2);
		SPI_WriteData(0x14);
		SPI_WriteData(0x01);
		SPI_WriteData(0x53);
		SPI_WriteData(0x25);
		SPI_WriteData(0x02);
		SPI_WriteData(0x02);
		SPI_WriteData(0x66);
		SPI_WriteData(0x14);
		SPI_WriteData(0x03);

		SPI_WriteComm(0xD3);
		SPI_WriteData(0x14);
		SPI_WriteData(0x01);
		SPI_WriteData(0x53);
		SPI_WriteData(0x07);
		SPI_WriteData(0x02);
		SPI_WriteData(0x02);
		SPI_WriteData(0x66);
		SPI_WriteData(0x14);
		SPI_WriteData(0x03);


		SPI_WriteComm(0xD4);
		SPI_WriteData(0x14);
		SPI_WriteData(0x01);
		SPI_WriteData(0x53);
		SPI_WriteData(0x25);
		SPI_WriteData(0x02);
		SPI_WriteData(0x02);
		SPI_WriteData(0x66);
		SPI_WriteData(0x14);
		SPI_WriteData(0x03);

		SPI_WriteComm(0xD5);
		SPI_WriteData(0x14);
		SPI_WriteData(0x01);
		SPI_WriteData(0x53);
		SPI_WriteData(0x07);
		SPI_WriteData(0x02);
		SPI_WriteData(0x02);
		SPI_WriteData(0x66);
		SPI_WriteData(0x14);
		SPI_WriteData(0x03);
		SPI_WriteComm(0x11);

		LCD_delay(10);

		SPI_WriteComm(0x3A); SPI_WriteData(0x66);//set_pixel_format 0X60 26k
		SPI_WriteComm(0x36); SPI_WriteData(0x2A);
		SPI_WriteComm(0x29);SPI_WriteComm(0x2c);
}
void set_resolutionXX(LCD_FORM_TypeDef* LCD_FORM)
{
    u32 aHorStart;
    u32 aHorEnd;
    u32 aVerStart;
    u32 aVerEnd;

    aHorStart = LCD_FORM->blkHorEnd + 1;
    aHorEnd = aHorStart + LCD_FORM->aHorLen;  
    aVerStart = LCD_FORM->blkVerEnd + 1 ;
    aVerEnd = aVerStart + LCD_FORM->aVerLen;

		LTDC->P_HOR = aHorEnd;//总宽度
    LTDC->HSYNC = (LCD_FORM->sHsyncStart <<16 )|LCD_FORM->sHsyncEnd;//水平同步信号起始和结束，位于背景色中间
    LTDC->A_HOR = (aHorStart<<16)|aHorEnd;//水平激活起始和结束
    LTDC->A_HOR_LEN = LCD_FORM->aHorLen ;//水平激活域宽度
    LTDC->BLK_HOR = (0<<16)|LCD_FORM->blkHorEnd;//背景开始和结束宽度0~激活地址	
    LTDC->P_VER =  aVerEnd;
    LTDC->VSYNC = (LCD_FORM->sVsyncStart<<16)|LCD_FORM->sVsyncEnd;
    LTDC->A_VER = (aVerStart<<16)|aVerEnd;
    LTDC->A_VER_LEN = LCD_FORM->aVerLen ;
		LTDC->BLK_VER = (0<<16)|LCD_FORM->blkVerEnd;
}


void Set_LCD_Timing_to_LTDC(void)
{
    LCD_FORM_TypeDef LCD_FORM;
    LTDC->OUT_EN = 0;
    LTDC->DP_ADDR0 = (u32)LTDC_Buf;//第0层地址
//    LTDC->DP_ADDR1 = (u32)(LTDC_Buf + SDRAM_RGB_OFFSET);//第一层地址
    LTDC->BLK_DATA = 0x0000;//背景色

		LCD_FORM.sHsyncStart = 0x2;  //水平激活起始
    LCD_FORM.sHsyncEnd = 0x3;    //水平激活结束
    LCD_FORM.aHorLen = 480 - 1;  //水平分辨率
    LCD_FORM.blkHorEnd = 0x15;    //水平消隐

    LCD_FORM.sVsyncStart = 0x2;  //垂直激活起始
    LCD_FORM.sVsyncEnd = 0x8;    //垂直激活结束
    LCD_FORM.aVerLen= 800 - 1; 	 //垂直分辨率
    LCD_FORM.blkVerEnd = 0xf;   //垂直消隐
	
    set_resolutionXX(&LCD_FORM);

		LTDC->VI_FORMAT = 0;
		LTDC->POL_CTL = 0x2+8;
		LTDC->OUT_EN |= 0x107;
}
void LTDC_IRQHandler(void)
{
    LTDC->INTR_CLR = 2;
    LTDC->DP_SWT ^= 1;//连续显示两幅图片
    if(LTDC->DP_SWT !=0 )
    {
//        fun_test(SDRAM_RGB_ADD);
    }
    else
    {
//        fun_test(SDRAM_RGB_ADD+SDRAM_RGB_OFFSET);
    }
//    if(LTDC->INTR_STA & 2)
//    {

//    }
}
/**********************************************
函数名：Lcd矩形填充函数

入口参数：xStart x方向的起始点
          ySrart y方向的终止点
          xLong 要选定矩形的x方向长度
          yLong  要选定矩形的y方向长度
返回值：无
***********************************************/
void Lcd_ColorBox(u16 xStart,u16 yStart,u16 xLong,u16 yLong,u32 Color)
{
	u16 i,j;
	u32 temp;
	temp = YSIZE_PHYS*xStart;
	for(i=0;i<yLong;i++)
	{
		for(j=0;j<xLong;j++)
		LTDC_Buf[yStart+i+YSIZE_PHYS*j+temp]=Color;
	}
	
//	u16 i,j;
//	u32 temp;
//	temp = XSIZE_PHYS*yStart;
//	for(i=0;i<yLong;i++)
//	{
//		for(j=0;j<xLong;j++)
//		LTDC_Buf[xStart+j+XSIZE_PHYS*i+temp]=Color;
//	}
}
void LCD_Initial(void) //LCD初始化函数
{    
	GPIO_RGB_INIT();//初始化液晶屏相关GPIO
	LTDC_Clock_Set();
	Set_LCD_Timing_to_LTDC();
	Lcd_Initialize();
	Lcd_Light_ON;//打开背光
}
/******************************************
函数名：Lcd图像填充100*100
功能：向Lcd指定位置填充图像
入口参数：
******************************************/
void LCD_Fill_Pic(u16 x, u16 y,u16 pic_H, u16 pic_V, u32* pic)
{
	u16 i,j;
	u32 Xstart,k=0;
	Xstart = YSIZE_PHYS*x;
	for(i=0;i<pic_V;i++)
	{
		for(j=0;j<pic_H;j++)
		LTDC_Buf[Xstart+i+YSIZE_PHYS*j+y]=pic[k++];
	}
	
//	u16 i,j;
//	u32 Ystart,k=0;
//	Ystart = XSIZE_PHYS*y;
//	for(i=0;i<pic_V;i++)
//	{
//		for(j=0;j<pic_H;j++)
//		LTDC_Buf[x+j+XSIZE_PHYS*i+Ystart]=pic[k++];
//	}
}
//=============== 在x，y 坐标上打一个颜色为Color的点 ===============
void DrawPixel(u16 x, u16 y, int Color)
{

	LTDC_Buf[y+YSIZE_PHYS*x] = Color;
						  
}
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
//8*16字体 ASCII码 显示
//函数名：SPILCD_ShowChar
//参  数：
//(x,y): 
//num:要显示的字符:" "--->"~"
//flag:有背景色(1)无背景色(0)
void SPILCD_ShowChar(unsigned short x,unsigned short y,unsigned char num, unsigned int fColor, unsigned int bColor,unsigned char flag) 
{       
	unsigned char temp;
	unsigned int pos,i,j;  

	num=num-' ';//得到偏移后的值
	i=num*16; 	
	for(pos=0;pos<16;pos++)
		{
			temp=nAsciiDot[i+pos];	//调通调用ASCII字体
			for(j=0;j<8;j++)
		   {                 
		        if(temp&0x80)
							DrawPixel(x+j,y,fColor);
						else 
							if(flag) DrawPixel(x+j,y,bColor); //如果背景色标志flag为1
							temp<<=1; 
		    }
			 y++;
		}		 
}  

//写一个16x16的汉字
void PutGB1616(unsigned short x, unsigned short  y, unsigned char c[2], unsigned int fColor,unsigned int bColor,unsigned char flag)
{
	unsigned int i,j,k;
	unsigned short m;
	for (k=0;k<64;k++) { //64标示自建汉字库中的个数，循环查询内码
	  if ((codeGB_16[k].Index[0]==c[0])&&(codeGB_16[k].Index[1]==c[1]))
			{ 
    	for(i=0;i<32;i++) 
			{
				m=codeGB_16[k].Msk[i];
				for(j=0;j<8;j++) 
				{		
					if((m&0x80)==0x80) {
						DrawPixel(x+j,y,fColor);
						}
					else {
						if(flag) DrawPixel(x+j,y,bColor);
						}
					m=m<<1;
				} 
				if(i%2){y++;x=x-8;}
				else x=x+8;
		  }
		}  
	  }	
	}

//显示一串字
void LCD_PutString(unsigned short x, unsigned short y, char *s, unsigned int fColor, unsigned int bColor,unsigned char flag) 
	{
		unsigned char l=0;
		while(*s) 
			{
				if( *s < 0x80) 
						{
							SPILCD_ShowChar(x+l*8,y,*s,fColor,bColor,flag);
							s++;l++;
						}
				else
						{
							PutGB1616(x+l*8,y,(unsigned char*)s,fColor,bColor,flag);
							s+=2;l+=2;
						}
			}
	}
//写一个n*n的汉字
void Put_GB_n(unsigned short x, unsigned short y, unsigned char c[2], unsigned int fColor,unsigned int bColor,unsigned char flag,unsigned int n)
{
	unsigned int i,j,k,a=0;
	unsigned short m;
	for (k=0;k<12;k++) { //12标示自建汉字库中的个数，循环查询内码
	  if ((codeGB_48[k].Index[0]==c[0])&&(codeGB_48[k].Index[1]==c[1]))
			{ 
    	for(i=0;i<((n*n)>>3);i++) 
			{
				for(a=0;a<(n>>3);a++)
				{
					m=codeGB_48[k].Msk[i+a];
					for(j=0;j<8;j++) 
					{		
						if((m&0x80)==0x80) {
							DrawPixel(x+j,y,fColor);
							}
						else {
							if(flag) DrawPixel(x+j,y,bColor);
							}
						m=m<<1;
					} 
					 x=x+8;
				}
				i = i+a-1;
				y++;x=x-8*a;
		  }
		}  
	  }	
	}
//显示一串汉字，字号为n
void LCD_PutString_GB_n(unsigned short x, unsigned short y, char *s, unsigned int fColor, unsigned int bColor,unsigned char flag,unsigned int n) 
	{
		unsigned int l=0;
		while(*s) 
			{
				Put_GB_n(x+l,y,(unsigned char*)s,fColor,bColor,flag,n);
				s+=2;l+=n;;
			}
	}