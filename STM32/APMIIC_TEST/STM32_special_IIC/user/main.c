#include "stm32f10x.h"
#include "LED.h"
#include "IIC.h"
#include "delay.h"
#include "usart.h"
#include "stdio.h"
/*
Keil: MDK5.10.0.2
MCU:stm32f103c8
请先使用USB转TTL模块设置好IIC模式
硬件接法：
GYTOF10M---STM32
VIN---5V
RC---PB6
TD---PB7
GND--GND

STM32 USART1---FT232
TX---RX
RX---TX
软件说明:
1\电脑上位机将模块 iic地址0xa4, IIC格式置成special，out mode 设置成nointerrupt_read，保存设置
注：
	中断函数位于stm32f10x_it.c
联系方式：
http://shop62474960.taobao.com/?spm=a230r.7195193.1997079397.2.9qa3Ky&v=1
*/
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
int fputc(int ch, FILE *f)
{
    while (!(USART1->SR & USART_FLAG_TXE));
    USART_SendData(USART1, (unsigned char) ch);// USART1 可以换成 USART2 等
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
	
	 delay_ms(1000);//等待模块初始化完成
	// changeAddress(0xe0,0x40);//更改IIC地址
	while(1)
	{

		requestRange(0Xa5,&diatance);
		takeRangeReading(0Xa4);

		 printf("distance: %d cm\r\n",diatance);
		delay_ms(100);
		
	}
}
