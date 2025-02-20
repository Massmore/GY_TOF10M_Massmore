#include "stm32f10x.h"
#include "LED.h"
#include "IIC.h"
#include "delay.h"
#include "usart.h"
#include "stdio.h"
/*
Keil: MDK5.10.0.2
MCU:stm32f103c8
����ʹ��USBתTTLģ�����ú�IICģʽ
Ӳ���ӷ���
GYTOF10M---STM32
VIN---5V
RC---PB6
TD---PB7
GND--GND

STM32 USART1---FT232
TX---RX
RX---TX
���˵��:
1\������λ����ģ�� iic��ַ0xa4, IIC��ʽ�ó�special��out mode ���ó�nointerrupt_read����������
ע��
	�жϺ���λ��stm32f10x_it.c
��ϵ��ʽ��
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
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
int fputc(int ch, FILE *f)
{
    while (!(USART1->SR & USART_FLAG_TXE));
    USART_SendData(USART1, (unsigned char) ch);// USART1 ���Ի��� USART2 ��
    return (ch);
}
int main(void)
{
	uint16_t diatance=0;
  u8 i=0;
	delay_init(72);
	LED_Int(GPIOB,GPIO_Pin_9,RCC_APB2Periph_GPIOB);
	NVIC_Configuration();
	Usart_Int(115200);
	printf("GY TOF TEST\r\n");
	I2C_GPIO_Config();
	
	 delay_ms(1000);//�ȴ�ģ���ʼ�����
	// changeAddress(0xe0,0x40);//����IIC��ַ
	while(1)
	{

		requestRange(0Xa5,&diatance);
		takeRangeReading(0Xa4);

		 printf("distance: %d cm\r\n",diatance);
		delay_ms(100);
		
	}
}
