#include "stm32f10x.h"
#include "LED.h"
#include "IIC.h"
#include "delay.h"
#include "usart.h"
#include "stdio.h"
/*
Keil: MDK5.10.0.2
MCU:stm32f103c8
模块设置：
连续输出模式，普通IIC模式，IIC地址，默认0xA4
请先使用USB转TTL模块设置好IIC模式
硬件接法：
GYTOF10M---STM32
VIN---5V
RC---PB6
TD---PB7
I/O--PA4 (iic中断引脚)
GND--GND

软件说明:
	IIC时钟频率需低于400Khz；
	中断函数位于stm32f10x_it.c
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
void I2C_GPIO_Config(void);

int main(void)
{
    u8  raw_data[9]= {0},data=0;
    u16 datas[4]= {0},delay_t;

    gytof my_tof;

    u8 td=0;
    delay_init(72);

    LED_Int(GPIOB,GPIO_Pin_9,RCC_APB2Periph_GPIOB);
		READ_pin_Init(GPIOA,GPIO_Pin_4,RCC_APB2Periph_GPIOA);
    NVIC_Configuration();
    Usart_Int(115200);
    I2C_GPIO_Config();

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
      if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==0)
			 {  delay_ms(3); //等待更新
        if(Single_ReadI2C(0xa4,0x08,raw_data,5))
        {
            my_tof.distance=(raw_data[0]<<8)|raw_data[1];
            my_tof.amp=((uint16_t)raw_data[2]<<8)|raw_data[3];
            my_tof.temp=raw_data[4];

            printf("distance: %.1f",my_tof.distance/10);
            printf(" cm, amp: %d,",my_tof.amp);
            printf("  temp: %d\r\n ",my_tof.temp);
        }
        
			}
     //delay_ms(delay_t); //等待更新

    }
}
// funtion SCL_H,SCL_L,SDA_H,SDA_L;
// funtion1 SCL_read,SDA_read;
//funtion2 sum;
void I2C_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能与 I2C有关的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );

    /* PC3-I2C_SCL、PC5-I2C_SDA*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);

}