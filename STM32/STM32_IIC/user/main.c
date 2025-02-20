#include "stm32f10x.h"
#include "LED.h"
#include "IIC.h"
#include "delay.h"
#include "usart.h"
#include "stdio.h"
/*
Keil: MDK5.10.0.2
MCU:stm32f103c8
ģ�����ã�
�������ģʽ����ͨIICģʽ��IIC��ַ��Ĭ��0xA4
����ʹ��USBתTTLģ�����ú�IICģʽ
Ӳ���ӷ���
GYTOF10M---STM32
VIN---5V
RC---PB6
TD---PB7
I/O--PA4 (iic�ж�����)
GND--GND

���˵��:
	IICʱ��Ƶ�������400Khz��
	�жϺ���λ��stm32f10x_it.c
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

    delay_ms(300);//�ȴ�ģ���ʼ�����
    td=2;
    Single_WriteI2C_byte(0xa4,0x02,2);//���ø���Ƶ��50hz
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
			 {  delay_ms(3); //�ȴ�����
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
     //delay_ms(delay_t); //�ȴ�����

    }
}
// funtion SCL_H,SCL_L,SDA_H,SDA_L;
// funtion1 SCL_read,SDA_read;
//funtion2 sum;
void I2C_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ʹ���� I2C�йص�ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );

    /* PC3-I2C_SCL��PC5-I2C_SDA*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6| GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);

}