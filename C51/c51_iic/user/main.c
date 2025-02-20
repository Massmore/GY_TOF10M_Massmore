#include <reg52.h>
#include "usart.h"
#include "iic.h"  
 #include "stdio.h"
/*
请先使用USB转TTL模块设置好IIC模式
硬件接法：
GYTOF10M----C51
VCC----5V
RC---P3^6
TD---P3^7
I/O--P3^5(iic中断引脚)
GND---GND


C51---USB_TTL
TX ---RX
RX ---TX
软件说明：

注：
	中断函数位于stc_it.c
联系方式：
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
char putchar (char c)   
{        
    ES=0;        
    SBUF = c;        
    while(TI==0);        
    TI=0;        
    ES=1;        
    return 0;
}
typedef struct
{
    uint16_t distance;
   unsigned short int amp;
    int temp;
} gytof;
void delay_ms(unsigned int x)
{
	while(x--);
}short
int main(void)
{
	u8  raw_data[9]={0};
	uint16_t datas[4]={0},delay_t;

	gytof my_tof;

	u8 td=0;

	Usart_Int(9600);
	delay_ms(300);//等待模块初始化完成
	td=2;
	Single_WriteI2C_byte(0xa4,0x02,2);//设置更新频率50hz
    switch(td)
    {
    case 0:
        delay_t=995;
        break;
    case 1:
        delay_t=95;
        break;
    case 2:
        delay_t=18;
        break;
    case 3:
        delay_t=4;
        break;
    }
 

    delay_ms(1);
	while(1)
	{
		
		 if (read_pin==0)
			 {  delay_ms(3); //等待更新
        if(Single_ReadI2C(0xa4,0x08,raw_data,5))
        {
            my_tof.distance=(raw_data[0]<<8)|raw_data[1];
            my_tof.amp=((short int)raw_data[2]<<8)|raw_data[3];
            my_tof.temp=raw_data[4];

            printf("distance: %.1f",(float)my_tof.distance/10);
            printf(" cm, amp: %ud,",my_tof.amp);
            printf("  temp: %d\r\n ",my_tof.temp);
        }
        
	
		
		
	}
}
}