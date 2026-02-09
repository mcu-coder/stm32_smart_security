#include "main.h"
#include "rtc.h"
#include "oled.h"
#include "bmp.h"
#include "usart.h"
uchar dis_time_buf[16]={0};
uchar xingqi = 0;
u8 i=0,butt;
volatile u8 x=15,y=32,passw[4],j=0,zd=0,zd1=0;
volatile u16 conut=0;
extern volatile u8 bootconst;
extern  u8 jingbflag;
//uchar time_buf[8] = {0x20,0x10,0x06,0x01,0x23,0x59,0x55,0x02};//初始时间2010年6月1号23点59分55秒 星期二
//初始时间定义
volatile u8 time_buf[8] = {0x20,0x21,0x05,0x06,0x08,0x25,0x00,0x04};//初始时间2010年6月1号23点59分55秒 星期二
//DS1302引脚定义
#define RTC_RST_Clr() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)
#define RTC_RST_Set() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)
#define RTC_IO_Clr()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)
#define RTC_IO_Set()  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET)
#define RTC_SCK_Clr() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET)
#define RTC_SCK_Set() HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET)
//DS1302地址定义
#define ds1302_sec_add			0x80		//秒数据地址
#define ds1302_min_add			0x82		//分数据地址
#define ds1302_hr_add			0x84		//时数据地址
#define ds1302_date_add			0x86		//日数据地址
#define ds1302_month_add		0x88		//月数据地址
#define ds1302_day_add			0x8a		//星期数据地址
#define ds1302_year_add			0x8c		//年数据地址
#define ds1302_control_add		0x8e		//控制数据地址
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe

   	//DS1302初始化函数
void ds1302_init(void) 
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	RTC_RST_Clr();
	RTC_SCK_Clr();
}

void buzz_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
}

void ir_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void mq2_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
}

void butt_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
}

u8 getbuttdata(void)
{
	
	while(1)
	{
		
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8) == 0)
		{
			HAL_Delay(20);
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8) == 0)
			{
			while(!(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)));
			return 2;
			}
		}
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15) == 0)
		{
			HAL_Delay(20);
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15) == 0)
			{
            while(!(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)));
			return 1;
			}
		}
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) == 0)
		{
						HAL_Delay(20);
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) == 0)
			{
						while(!(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)));
			return 4;
			}
		}
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4) == 0)
		{
						HAL_Delay(20);
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4) == 0)
			{
						while(!(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)));
			return 3;
			}
		}
	
	}
	

}

void RTC_IO_IN()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void RTC_IO_OUT()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

//向DS1302写入一字节数据
void ds1302_write_byte(u8 addr, u8 d) 
{
	u8 i;
	RTC_RST_Set();				//启动DS1302总线	
	//写入目标地址：addr
	addr = addr & 0xFE;   //最低位置零，寄存器0位为0时写，为1时读
	for (i = 0; i < 8; i ++) {
		if (addr & 0x01) {
			RTC_IO_Set();
			}
		else {
			RTC_IO_Clr();
			}
		RTC_SCK_Set();      //产生时钟
		RTC_SCK_Clr();
		addr = addr >> 1;
		}	
	//写入数据：d
	for (i = 0; i < 8; i ++) {
		if (d & 0x01) {
			RTC_IO_Set();
			}
		else {
			RTC_IO_Clr();
			}
		RTC_SCK_Set();      //产生时钟
		RTC_SCK_Clr();
		d = d >> 1;
		}
	RTC_RST_Clr();		//停止DS1302总线
}

//从DS1302读出一字节数据
u8 ds1302_read_byte(u8 addr) {

	u8 i,temp;	
	RTC_RST_Set();						//启动DS1302总线
	//写入目标地址：addr
	addr = addr | 0x01;    //最低位置高，寄存器0位为0时写，为1时读
	for (i = 0; i < 8; i ++) {
		if (addr & 0x01) {
			RTC_IO_Set();	
			}
		else {
			RTC_IO_Clr();	
			}
		RTC_SCK_Set();      //产生时钟
		RTC_SCK_Clr();
		addr = addr >> 1;
		}
	RTC_IO_IN();	
	//输出数据：temp
	for (i = 0; i < 8; i ++) {
		temp = temp >> 1;
		if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)) {
			temp |= 0x80;
			}
		else {
			temp &= 0x7F;
			}
		RTC_SCK_Set();      //产生时钟
		RTC_SCK_Clr();
		}	
	RTC_RST_Clr();					//停止DS1302总线
	RTC_IO_OUT();
	return temp;
}

//向DS302写入时钟数据
void ds1302_write_time(void) 
{
	ds1302_write_byte(ds1302_control_add,0x00);			//关闭写保护 
	ds1302_write_byte(ds1302_sec_add,0x80);				//暂停时钟 
	//ds1302_write_byte(ds1302_charger_add,0xa9);	    //涓流充电 
	ds1302_write_byte(ds1302_year_add,time_buf[1]);		// 
	ds1302_write_byte(ds1302_month_add,time_buf[2]);	// 
	ds1302_write_byte(ds1302_date_add,time_buf[3]);		// 
	ds1302_write_byte(ds1302_hr_add,time_buf[4]);		// 
	ds1302_write_byte(ds1302_min_add,time_buf[5]);		//
	
	ds1302_write_byte(ds1302_day_add,time_buf[7]);		//

}
void ds1302_write_time2(void) 
{
	ds1302_write_byte(ds1302_control_add,0x00);			//关闭写保护 
	//ds1302_write_byte(ds1302_sec_add,0x80);				//暂停时钟 
	//ds1302_write_byte(ds1302_charger_add,0xa9);	    //涓流充电 
	ds1302_write_byte(ds1302_year_add,time_buf[1]);		 
	ds1302_write_byte(ds1302_sec_add,time_buf[6]);
	ds1302_write_byte(ds1302_control_add,0x80);			//打开写保护     
}
//从DS302读出时钟数据
void ds1302_read_time(void)  
{
	time_buf[1]=ds1302_read_byte(ds1302_year_add);		//年 
	time_buf[2]=ds1302_read_byte(ds1302_month_add);		//月 
	time_buf[3]=ds1302_read_byte(ds1302_date_add);		//日 
	time_buf[4]=ds1302_read_byte(ds1302_hr_add);		//时 
	time_buf[5]=ds1302_read_byte(ds1302_min_add);		//分 
	time_buf[6]=(ds1302_read_byte(ds1302_sec_add))&0x7f;//秒，屏蔽秒的第7位，避免超出59
	time_buf[7]=ds1302_read_byte(ds1302_day_add);		//周 	
}

u8 b_h(u8 a)
{
	u8 b;
  	   switch(a)
	   {
	   		case 0x00 :  b=0;break;
			case 0x01 :  b=1;break;
			case 0x02 :  b=2;break;
			case 0x03 :  b=3;break;
			case 0x04 :  b=4;break;
			case 0x05 :  b=5;break;
			case 0x06 :  b=6;break;
			case 0x07 :  b=7;break;
			case 0x08 :  b=8;break;
			case 0x09 :  b=9;break;
			case 0x10 :  b=10;break;
			case 0x11 :  b=11;break;
			case 0x12 :  b=12;break;
			case 0x13 :  b=13;break;
			case 0x14 :  b=14;break;
			case 0x15 :  b=15;break;
			case 0x16 :  b=16;break;
			case 0x17 :  b=17;break;
			case 0x18 :  b=18;break;
			case 0x19 :  b=19;break;
			case 0x20 :  b=20;break;
			case 0x21 :  b=21;break;
			case 0x22 :  b=22;break;
			case 0x23 :  b=23;break;
			case 0x24 :  b=24;break;
			case 0x25 :  b=25;break;
			case 0x26 :  b=26;break;
			case 0x27 :  b=27;break;
			case 0x28 :  b=28;break;
			case 0x29 :  b=29;break;
			case 0x30 :  b=30;break;
			case 0x31 :  b=31;break;
			case 0x32 :  b=32;break;
			case 0x33 :  b=33;break;
			case 0x34 :  b=34;break;
			case 0x35 :  b=35;break;
			case 0x36 :  b=36;break;
			case 0x37 :  b=37;break;
			case 0x38 :  b=38;break;
			case 0x39 :  b=39;break;
			case 0x40 :  b=40;break;
			case 0x41 :  b=41;break;
			case 0x42 :  b=42;break;
			case 0x43 :  b=43;break;
			case 0x44 :  b=44;break;
			case 0x45 :  b=45;break;
			case 0x46 :  b=46;break;
			case 0x47 :  b=47;break;
			case 0x48 :  b=48;break;
			case 0x49 :  b=49;break;
			case 0x50 :  b=50;break;
			case 0x51 :  b=51;break;
			case 0x52 :  b=52;break;
			case 0x53 :  b=53;break;
			case 0x54 :  b=54;break;
			case 0x55 :  b=55;break;
			case 0x56 :  b=56;break;
			case 0x57 :  b=57;break;
			case 0x58 :  b=58;break;
			case 0x59 :  b=59;break;
			default   : break;
	   }
	   return b;
}
void showbegin(void)//周
{
  //OLED_DrawBMP(0,0,128,8,BMP19);

  
		OLED_Clear();
	OLED_ShowCHinese(15,6,24);//
	OLED_ShowCHinese(40,6,24);
	OLED_ShowCHinese(65,6,24);
	OLED_ShowCHinese(90,6,24);
	
	
	while(1)
	{
				butt=getbuttdata();//获取按键
	
		//显示密码1.5秒后变为*
		passw[j]=butt;
		switch(butt)
		{
			case 0 : OLED_DrawBMP(x,2,y,6,BMP10);zd=1;conut=0;break;
			case 1 : OLED_DrawBMP(x,2,y,6,BMP1);zd=1;conut=0;break;
			case 2 : OLED_DrawBMP(x,2,y,6,BMP2);zd=1;conut=0;break;
			case 3 : OLED_DrawBMP(x,2,y,6,BMP3);zd=1;conut=0;break;
			case 4 : OLED_DrawBMP(x,2,y,6,BMP4);zd=1;conut=0;break;
			case 5 : OLED_DrawBMP(x,2,y,6,BMP5);zd=1;conut=0;break;
			case 6 : OLED_DrawBMP(x,2,y,6,BMP6);zd=1;conut=0;break;
			case 7 : OLED_DrawBMP(x,2,y,6,BMP7);zd=1;conut=0;break;
			case 8 : OLED_DrawBMP(x,2,y,6,BMP8);zd=1;conut=0;break;
			case 9 : OLED_DrawBMP(x,2,y,6,BMP9);zd=1;conut=0;break;
		}
		if(x>25)
			OLED_DrawBMP(x-25,2,y-25,6,BMP11);
		x+=25;
		y+=25;
		j++;
		if(j==4)
		{
		   if(passw[0]==1&&passw[1]==1&&passw[2]==4&&passw[3]==4)//密码确认
			 {
				 OLED_Clear();
				 break;
			 }
			 else//密码错误
			 {
				    HAL_Delay(500);
						OLED_Clear();
						OLED_DrawBMP(10,2,42,6,BMP14);
						OLED_DrawBMP(37,2,69,6,BMP15);
						OLED_DrawBMP(67,2,99,6,BMP16);
						OLED_DrawBMP(92,2,124,6,BMP17);
						HAL_Delay(2000);
						OLED_Clear();
				    OLED_ShowCHinese(15,6,24);
						OLED_ShowCHinese(40,6,24);
						OLED_ShowCHinese(65,6,24);
						OLED_ShowCHinese(90,6,24);
						x=15;
						y=32;
				    j=0;
			 }
		}
	
	}
	
  OLED_DrawBMP(0,0,128,8,BMP20);
	
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET);
  HAL_Delay(50);
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET);
  HAL_Delay(200);
  OLED_Clear();
	
	//wifi初始化
	OLED_ShowChar(45,0,'W');
	OLED_ShowChar(52,0,'I');
	OLED_ShowChar(59,0,'F');
	OLED_ShowChar(66,0,'I');
	OLED_ShowCHinese(30,2,53);//
	OLED_ShowCHinese(47,2,54);//
	OLED_ShowCHinese(64,2,55);//
	HAL_UART_Transmit_IT(&huart1,"AT\r\n",sizeof("AT\r\n"));
	HAL_Delay(500);
	OLED_ShowCHinese(81,2,56);//
	HAL_UART_Transmit_IT(&huart1,"AT\r\n",sizeof("AT\r\n"));//AT指令
	HAL_Delay(500);
	OLED_ShowCHinese(98,2,56);//
	HAL_UART_Transmit_IT(&huart1,"AT+CWMODE=2\r\n",sizeof("AT+CWMODE=2\r\n"));
	HAL_Delay(500);
	OLED_ShowCHinese(115,2,56);//
	HAL_UART_Transmit_IT(&huart1,"AT+CWSAP=\"ESP8266_001\",\"1234567890\",11,4\r\n",sizeof("AT+CWSAP=\"ESP8266_001\",\"1234567890\",11,4\r\n"));//设置密码。名称
	HAL_Delay(500);
	OLED_ShowCHinese(81,2,57);//
	OLED_ShowCHinese(98,2,57);//
	OLED_ShowCHinese(115,2,57);//
	HAL_UART_Transmit_IT(&huart1,"AT+CIPMUX=1\r\n",sizeof("AT+CIPMUX=1\r\n"));
	HAL_Delay(500);
	OLED_ShowCHinese(81,2,56);//
	HAL_UART_Transmit_IT(&huart1,"AT+CIPSERVER=1,5000\r\n",sizeof("AT+CIPSERVER=1,5000\r\n"));//.设置端口号
	HAL_Delay(500);
	OLED_ShowCHinese(98,2,56);//
	HAL_UART_Transmit_IT(&huart1,"AT+CIPSTO=0\r\n",sizeof("AT+CIPSTO=0\r\n"));

  OLED_Clear();


}

void showhuo(void)
{
	OLED_DrawBMP(64,2,128,8,BMP23);
}

void showmei(void)
{
	OLED_DrawBMP(64,0,122,8,BMP24);
}

void showman(void)
{
	OLED_DrawBMP(64,0,104,8,BMP25);
}

void showBMP11(void)
{
	OLED_DrawBMP(x-25,2,y-25,6,BMP11);
}


void showtime(void)
{


	OLED_ShowChar(0,0,'2');
	OLED_ShowChar(8,0,'0');
//	OLED_ShowChar(16,0,'2');
//	OLED_ShowChar(24,0,'1');
	
    switch((b_h(time_buf[1])/10))
	{
		case 0 :  	OLED_ShowChar(16,0,'0'); break;
		case 1 :  	OLED_ShowChar(16,0,'1'); break;
		case 2 :  	OLED_ShowChar(16,0,'2'); break;
		case 3 :  	OLED_ShowChar(16,0,'3'); break;
		case 4 :  	OLED_ShowChar(16,0,'4'); break;
		case 5 :  	OLED_ShowChar(16,0,'5'); break;
		case 6 :  	OLED_ShowChar(16,0,'6'); break;
		case 7 :  	OLED_ShowChar(16,0,'7'); break;
		case 8 :  	OLED_ShowChar(16,0,'8'); break;
		case 9 :  	OLED_ShowChar(16,0,'9'); break;
	}
	    switch((b_h(time_buf[1])%10))
	{
		case 0 :  	OLED_ShowChar(24,0,'0'); break;
		case 1 :  	OLED_ShowChar(24,0,'1'); break;
		case 2 :  	OLED_ShowChar(24,0,'2'); break;
		case 3 :  	OLED_ShowChar(24,0,'3'); break;
		case 4 :  	OLED_ShowChar(24,0,'4'); break;
		case 5 :  	OLED_ShowChar(24,0,'5'); break;
		case 6 :  	OLED_ShowChar(24,0,'6'); break;
		case 7 :  	OLED_ShowChar(24,0,'7'); break;
		case 8 :  	OLED_ShowChar(24,0,'8'); break;
		case 9 :  	OLED_ShowChar(24,0,'9'); break;
	}
	OLED_ShowChar(32,0,'-');

	switch((b_h(time_buf[2])/10))
	{
		case 0 :  	OLED_ShowChar(40,0,'0'); break;
		case 1 :  	OLED_ShowChar(40,0,'1'); break;
	}
	switch((b_h(time_buf[2])%10))
	{
		case 0 :  	OLED_ShowChar(48,0,'0'); break;
		case 1 :  	OLED_ShowChar(48,0,'1'); break;
		case 2 :  	OLED_ShowChar(48,0,'2'); break;
		case 3 :  	OLED_ShowChar(48,0,'3'); break;
		case 4 :  	OLED_ShowChar(48,0,'4'); break;
		case 5 :  	OLED_ShowChar(48,0,'5'); break;
		case 6 :  	OLED_ShowChar(48,0,'6'); break;
		case 7 :  	OLED_ShowChar(48,0,'7'); break;
		case 8 :  	OLED_ShowChar(48,0,'8'); break;
		case 9 :  	OLED_ShowChar(48,0,'9'); break;
	}
	OLED_ShowChar(56,0,'-');
	
	switch((b_h(time_buf[3])/10))
	{
		case 0 :  	OLED_ShowChar(64,0,'0'); break;
		case 1 :  	OLED_ShowChar(64,0,'1'); break;
		case 2 :  	OLED_ShowChar(64,0,'2'); break;
		case 3 :  	OLED_ShowChar(64,0,'3'); break;
	}
	switch((b_h(time_buf[3])%10))
	{
		case 0 :  	OLED_ShowChar(72,0,'0'); break;
		case 1 :  	OLED_ShowChar(72,0,'1'); break;
		case 2 :  	OLED_ShowChar(72,0,'2'); break;
		case 3 :  	OLED_ShowChar(72,0,'3'); break;
		case 4 :  	OLED_ShowChar(72,0,'4'); break;
		case 5 :  	OLED_ShowChar(72,0,'5'); break;

	}

	if(b_h(time_buf[1]) != bootconst)
	{
		OLED_DrawBMP(0,0,128,8,BMP20);
		jingbflag = 0;
	}
	else
	{
		jingbflag = 1;
	}
	switch((b_h(time_buf[4])/10))
	{
		case 0 :  OLED_DrawBMP(0,2,17,6,BMP10); break;
		case 1 :  OLED_DrawBMP(0,2,17,6,BMP1); break;
		case 2 :  OLED_DrawBMP(0,2,17,6,BMP2); break;
	}
	switch((b_h(time_buf[4])%10))
	{
		case 0 :  OLED_DrawBMP(15,2,32,6,BMP10); break;
		case 1 :  OLED_DrawBMP(15,2,32,6,BMP1); break;
		case 2 :  OLED_DrawBMP(15,2,32,6,BMP2); break;
		case 3 :  OLED_DrawBMP(15,2,32,6,BMP3); break;
		case 4 :  OLED_DrawBMP(15,2,32,6,BMP4); break;
		case 5 :  OLED_DrawBMP(15,2,32,6,BMP5); break;
		case 6 :  OLED_DrawBMP(15,2,32,6,BMP6); break;
		case 7 :  OLED_DrawBMP(15,2,32,6,BMP7); break;
		case 8 :  OLED_DrawBMP(15,2,32,6,BMP8); break;
		case 9 :  OLED_DrawBMP(15,2,32,6,BMP9); break;
	}
	
	OLED_DrawBMP(30,2,47,6,BMP18);
	
	switch((b_h(time_buf[5])/10))
	{
		case 0 :  OLED_DrawBMP(47,2,64,6,BMP10); break;
		case 1 :  OLED_DrawBMP(47,2,64,6,BMP1); break;
		case 2 :  OLED_DrawBMP(47,2,64,6,BMP2); break;
		case 3 :  OLED_DrawBMP(47,2,64,6,BMP3); break;
		case 4 :  OLED_DrawBMP(47,2,64,6,BMP4); break;
		case 5 :  OLED_DrawBMP(47,2,64,6,BMP5); break;
		case 6 :  OLED_DrawBMP(47,2,64,6,BMP6); break;
		case 7 :  OLED_DrawBMP(47,2,64,6,BMP7); break;
		case 8 :  OLED_DrawBMP(47,2,64,6,BMP8); break;
		case 9 :  OLED_DrawBMP(47,2,64,6,BMP9); break;
	}
	switch((b_h(time_buf[5])%10))
	{
		case 0 :  OLED_DrawBMP(62,2,79,6,BMP10); break;
		case 1 :  OLED_DrawBMP(62,2,79,6,BMP1); break;
		case 2 :  OLED_DrawBMP(62,2,79,6,BMP2); break;
		case 3 :  OLED_DrawBMP(62,2,79,6,BMP3); break;
		case 4 :  OLED_DrawBMP(62,2,79,6,BMP4); break;
		case 5 :  OLED_DrawBMP(62,2,79,6,BMP5); break;
		case 6 :  OLED_DrawBMP(62,2,79,6,BMP6); break;
		case 7 :  OLED_DrawBMP(62,2,79,6,BMP7); break;
		case 8 :  OLED_DrawBMP(62,2,79,6,BMP8); break;
		case 9 :  OLED_DrawBMP(62,2,79,6,BMP9); break;
	}
	OLED_DrawBMP(79,2,96,6,BMP18);
	
	switch((b_h(time_buf[6])/10))
	{
		case 0 :  OLED_DrawBMP(96,2,113,6,BMP10); break;
		case 1 :  OLED_DrawBMP(96,2,113,6,BMP1); break;
		case 2 :  OLED_DrawBMP(96,2,113,6,BMP2); break;
		case 3 :  OLED_DrawBMP(96,2,113,6,BMP3); break;
		case 4 :  OLED_DrawBMP(96,2,113,6,BMP4); break;
		case 5 :  OLED_DrawBMP(96,2,113,6,BMP5); break;
		case 6 :  OLED_DrawBMP(96,2,113,6,BMP6); break;
		case 7 :  OLED_DrawBMP(96,2,113,6,BMP7); break;
		case 8 :  OLED_DrawBMP(96,2,113,6,BMP8); break;
		case 9 :  OLED_DrawBMP(96,2,113,6,BMP9); break;
	}
	switch((b_h(time_buf[6])%10))
	{
		case 0 :  OLED_DrawBMP(111,2,128,6,BMP10); break;
		case 1 :  OLED_DrawBMP(111,2,128,6,BMP1); break;
		case 2 :  OLED_DrawBMP(111,2,128,6,BMP2); break;
		case 3 :  OLED_DrawBMP(111,2,128,6,BMP3); break;
		case 4 :  OLED_DrawBMP(111,2,128,6,BMP4); break;
		case 5 :  OLED_DrawBMP(111,2,128,6,BMP5); break;
		case 6 :  OLED_DrawBMP(111,2,128,6,BMP6); break;
		case 7 :  OLED_DrawBMP(111,2,128,6,BMP7); break;
		case 8 :  OLED_DrawBMP(111,2,128,6,BMP8); break;
		case 9 :  OLED_DrawBMP(111,2,128,6,BMP9); break;
	}
	
	switch(b_h(time_buf[7]))
	{
		case 1 :  OLED_ShowCHinese(81,0,39);OLED_ShowCHinese(96,0,40);OLED_ShowCHinese(113,0,41); break;
		case 2 :  OLED_ShowCHinese(81,0,39);OLED_ShowCHinese(96,0,40);OLED_ShowCHinese(113,0,42); break;
		case 3 :  OLED_ShowCHinese(81,0,39);OLED_ShowCHinese(96,0,40);OLED_ShowCHinese(113,0,43); break;
		case 4 :  OLED_ShowCHinese(81,0,39);OLED_ShowCHinese(96,0,40);OLED_ShowCHinese(113,0,44); break;
		case 5 :  OLED_ShowCHinese(81,0,39);OLED_ShowCHinese(96,0,40);OLED_ShowCHinese(113,0,45); break;
		case 6 :  OLED_ShowCHinese(81,0,39);OLED_ShowCHinese(96,0,40);OLED_ShowCHinese(113,0,46); break;
		case 7 :  OLED_ShowCHinese(81,0,39);OLED_ShowCHinese(96,0,40);OLED_ShowCHinese(113,0,47); break;
	}

}
