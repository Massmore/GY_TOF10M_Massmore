#include <reg52.h>
#include "usart.h"
#include "iic.h"  
#include "stdio.h"
/*
Keil: MDK5.10.0.2
MCU:stm32f103c8
GYTOF10M---stc52
1,通过电脑配置好模块波特率9600，连续输出，起始寄存器0x08,数据量5，保存设置
硬件接法：
GYTOG10M---stc52
1、GYTOG10M_RX---STC52_TX
2、GYTOG10M_TX---USB_TTL_RX, 将数据上传给上位机

软件说明:
该程序采用串口方式获取模块数据，波特率9600行以上操作：
注：中断函数位于stc_it.c
联系方式：

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
    float distance;
    uint16_t amp;
    uint16_t temp;
} gytof;
int main(void)
{

    u8 sum=0,i=0;
	gytof my_tof={0,0,0};
	Usart_Int(9600);
 	while(1)
	{
		 if(Receive_ok)//串口接收完毕
        {
					   
            for(sum=0,i=0; i<(usart_rx_data[3]+4); i++) //rgb_data[3]=3
                sum+=usart_rx_data[i];
            if(sum==usart_rx_data[i])//校验和判断
            {
                my_tof.distance=(usart_rx_data[4]<<8)|usart_rx_data[5];
                my_tof.amp=(usart_rx_data[6]<<8)|usart_rx_data[7];
                my_tof.temp=usart_rx_data[8];
                
                printf("distance: %.1f",my_tof.distance/10);
                printf(" cm, amp: %d,",my_tof.amp);
                printf("  temp: %d\r\n ",my_tof.temp);
        
            }
						else
						{
							printf(" sum %d\r\n ",sum);
						  printf(" count %d\r\n ",usart_rx_data[3]+4);
						}
            Receive_ok=0;//处理数据完毕标志
        }
    
	}
}
