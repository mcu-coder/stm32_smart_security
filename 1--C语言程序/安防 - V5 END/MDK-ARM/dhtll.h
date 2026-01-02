#ifndef __OLEDFONT_H
#define __OLEDFONT_H 
#include <main.h>
void dht11_init(void);
void delay_us(u16 i);
unsigned char CalBCC(unsigned char *buf, int n);
int32_t dht11_read(uint8_t *pdht_data);
#endif


