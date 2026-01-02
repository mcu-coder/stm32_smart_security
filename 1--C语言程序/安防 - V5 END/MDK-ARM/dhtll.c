
#include "main.h"

void delay_us(u16 i)
{
	while(i--)
	{
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	}
}

void dht11_init(void)
{

 	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);

}


void dht11_pin_mode(uint32_t gpio_mode)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = gpio_mode;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void PIN3out(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}
	

void PIN3in(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
}
	


int32_t dht11_read(uint8_t *pdht_data)
{
	uint32_t t=0;
	
	uint8_t d;
	
	int32_t i=0;
	int32_t j=0;
	uint32_t check_sum=0;
	
	//±£Ö¤Òý½ÅÎªÊä³öÄ£Ê½
	PIN3out();

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
	
	HAL_Delay(90);
	
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);	
	
	delay_us(60);
	
	
	//±£Ö¤Òý½ÅÎªÊäÈëÄ£Ê½
	PIN3in();

	
	//µÈ´ýDHT11ÏìÓ¦£¬µÈ´ýµÍµçÆ½³öÏÖ
	t=0;
	while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10))
	{
	
		t++;
		
		delay_us(1);
		
		if(t >= 4000)
			return 1;
	}
	
	//ÈôµÍµçÆ½³¬³ö100us
	t=0;
	while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==0)
	{
	
		t++;
		
		delay_us(1);
		
		if(t >= 100)
			return 2;
	}
	
	
	//Èô¸ßµçÆ½³¬³ö100us
	t=0;
	while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10))
	{
	
		t++;
		
		delay_us(1);
		
		if(t >= 100)
			return 3;
	}
	
	//Á¬Ðø½ÓÊÕ5¸ö×Ö½Ú
	for(j=0; j<5; j++)
	{
		d = 0;
		//Íê³É8¸öbitÊý¾ÝµÄ½ÓÊÕ£¬¸ßÎ»ÓÅÏÈ
		for(i=7; i>=0; i--)
		{
			//µÈ´ýµÍµçÆ½³ÖÐøÍê±Ï
			t=0;
			while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10)==0)
			{
			
				t++;
				
				delay_us(1);
				
				if(t >= 100)
					return 4;
			}	
			
			
			delay_us(40);
			
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10))
			{
				d|=1<<i;
				
				//µÈ´ýÊý¾Ý1µÄ¸ßµçÆ½Ê±¼ä³ÖÐøÍê±Ï
				t=0;
				while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10))
				{
				
					t++;
					
					delay_us(1);
					
					if(t >= 100)
						return 5;
				}			
			
			}
		}	
	
		pdht_data[j] = d;
	}
	
	
	//Í¨ÐÅµÄ½áÊø
	delay_us(800);
	
	//¼ÆËãÐ£ÑéºÍ
	check_sum=pdht_data[0]+pdht_data[1]+pdht_data[2]+pdht_data[3];
	
	
	
	check_sum = check_sum & 0xFF;
	
	if(check_sum != pdht_data[4])
		return 6;
	
	return 0;
}