#include <reg52.h>
#include "usart.h"
#include "iic.h"  
#include "stdio.h"
/*
����ʹ��USBתTTLģ�����ú�IICģʽ
Ӳ���ӷ���
GYTOF10M----C51
VCC----5V
RC---P3^6
TD---P3^7
GND---GND


C51---USB_TTL
TX ---RX
RX ---TX
���˵����

ע��
	�жϺ���λ��stc_it.c
��ϵ��ʽ��
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
void delay(unsigned int x)
{
	while(x--);
}
int main(void)
{
uint16_t distance=0;
	Usart_Int(9600);
 	while(1)
	{
	   	
	    requestRange((0xa5),&distance);		 //��ȡ��������
		takeRangeReading(0xa4);				 //���Ͳ�������

		  printf("distance: %d",distance);
		delay(60000);	
	}
}
