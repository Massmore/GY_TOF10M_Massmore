#include "stc_it.h"
#include "usart.h"
#include "string.h"
//�ⲿ�ж�1
uint8_t stata=0;
void EXTI1_IRQHandler(void)interrupt 2
{
}
//�����ж�
void USART_IRQHandler(void)interrupt 4
{
	static uint8_t i=0,rebuf[30]={0};
	uint8_t sum=0;
    if(TI)//������ɱ�־
	{
	  TI=0;//�巢����ɱ�־
	  send_ok=0;//�����־��0 
	}
	if(RI)//������ɱ�־
	{
		rebuf[i++]=SBUF;
		RI=0;//���жϽ��ձ�־
        if (rebuf[0]!=add)//֡ͷ����
            i=0;
        else if ((i==2)&&(rebuf[1]!=0x03))//֡ͷ����
            i=0;
        else if ((i==3)&&(rebuf[2]>16))//֡ͷ����
            i=0;
        else if ((i==4)&&(rebuf[3]>16))//֡ͷ����
            i=0;
        if(i>3)//i����4ʱ���Ѿ����յ��������ֽ�rebuf[3]
        {
            if(i<21)
            {
                if(i==(rebuf[3]+5))//�ж��Ƿ����һ֡�������
                {

                    if(!Receive_ok)//�����ݴ�����ɺ�Ž����µ�����
                    {
                        memcpy(usart_rx_data,rebuf,i);//�������յ�������
                        Receive_ok=1;//������ɱ�־
                    }
                    i=0;//������0

                }
            }
						else
						i=0;
        }
	
	}

}
