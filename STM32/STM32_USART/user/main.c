#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "LED.h"
#include "stdio.h"
/*

Keil: MDK5.10.0.2
MCU:stm32f103c8
硬件接法：
GY-tof---STM32
1,通过电脑配置好模块波特率9600，连续输出，起始寄存器0x08,数据量5，保存设置
2、STM32_TX---FT232,STM32将数据上传给串口助手
3、GY-tof_TX---STM32_RX，接收模块数据
软件说明:
该程序采用串口方式获取模块数据，波特率9600

注：中断函数位于stm32f10x_it.c
联系方式：
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
int fputc(int ch, FILE *f)
{
    while (!(USART1->SR & USART_FLAG_TXE));
    USART_SendData(USART1, (unsigned char) ch);// USART1 可以换成 USART2 等
    return (ch);
}
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_X;

    /* 4个抢占优先级，4个响应优先级 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    /*抢占优先级可打断中断级别低的中断*/
    /*响应优先级按等级执行*/
    NVIC_X.NVIC_IRQChannel = USART1_IRQn;//中断向量
    NVIC_X.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
    NVIC_X.NVIC_IRQChannelSubPriority = 0;//响应优先级
    NVIC_X.NVIC_IRQChannelCmd = ENABLE;//使能中断响应
    NVIC_Init(&NVIC_X);
}
typedef struct
{
    float distance;
    uint16_t amp;
    int8_t temp;
} gytof;
int main(void)
{
    u8 sum=0,i=0;
    int16_t data=0;
    gytof my_tof= {0,0,0};
    delay_init(72);
    NVIC_Configuration();
    Usart_Int(9600);
    delay_ms(300);//等待模块初始化完成
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
