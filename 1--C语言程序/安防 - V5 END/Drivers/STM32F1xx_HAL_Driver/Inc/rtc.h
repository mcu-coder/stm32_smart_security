#ifndef __RTC_H
#define __RTC_H 
#define uchar unsigned char
#define uint  unsigned int			    
void showtime(void);
uchar b_h(uchar a);
void b_h2(uchar a);
void ds1302_read_time(void);
void ds1302_write_time(void); 
void ds1302_init(void);
void buzz_init(void);
void showbegin(void);
void ir_init(void);
void mq2_init(void);
void showhuo(void);
void showmei(void);
void showman(void);
void butt_init(void);
void showBMP11(void);
uchar ds1302_read_byte(uchar addr);
void ds1302_write_byte(uchar addr, uchar d);
void ds1302_write_time2(void);
#endif
