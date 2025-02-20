#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "LED.h"
#include "stdio.h"
/*

Keil: MDK5.10.0.2
MCU:stm32f103c8
Ӳ���ӷ���
GY-tof---STM32
1,ͨ���������ú�ģ�鲨����9600�������������ʼ�Ĵ���0x08,������5����������
2��STM32_TX---FT232,STM32�������ϴ�����������
3��GY-tof_TX---STM32_RX������ģ������
���˵��:
�ó�����ô��ڷ�ʽ��ȡģ�����ݣ�������9600

ע���жϺ���λ��stm32f10x_it.c
��ϵ��ʽ��
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
int fputc(int ch, FILE *f)
{
    while (!(USART1->SR & USART_FLAG_TXE));
    USART_SendData(USART1, (unsigned char) ch);// USART1 ���Ի��� USART2 ��
    return (ch);
}
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_X;

    /* 4����ռ���ȼ���4����Ӧ���ȼ� */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    /*��ռ���ȼ��ɴ���жϼ���͵��ж�*/
    /*��Ӧ���ȼ����ȼ�ִ��*/
    NVIC_X.NVIC_IRQChannel = USART1_IRQn;//�ж�����
    NVIC_X.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
    NVIC_X.NVIC_IRQChannelSubPriority = 0;//��Ӧ���ȼ�
    NVIC_X.NVIC_IRQChannelCmd = ENABLE;//ʹ���ж���Ӧ
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
    delay_ms(300);//�ȴ�ģ���ʼ�����
    while(1)
    {
        if(Receive_ok)//���ڽ������
        {
					   
            for(sum=0,i=0; i<(usart_rx_data[3]+4); i++) //rgb_data[3]=3
                sum+=usart_rx_data[i];
            if(sum==usart_rx_data[i])//У����ж�
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
            Receive_ok=0;//����������ϱ�־
        }
    }
}
