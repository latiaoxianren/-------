
#include "LED/LED.h"
#include "stm32f10x.h"
#include "timer.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "duoji.h"
#include "duoji.h"
#include "rtc.h"

int main(void)
{
 	u16 t,data2num,data3num=0;  
	u16 len;	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200	
//	TIM3_PWM_Init(19999,71);
	initLED(); //初始化LED
	RTC_Init();
	while (1)
	{
//		printf("test\r\n");
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			for(t=0;t<len;t++)
			{
				data2num=USART_RX_BUF[t];
				if(data2num>='0'&&data2num<='9')
				{
					data3num=data3num*10+(data2num-'0');
				}
			}
			for(t=0;t<len;t++)
			{
				data2num=USART_RX_BUF[t];
				if(data2num>='A'&&data2num<='B')
				{
					if(data3num<20000)
					{
						switch(data2num)
						{
							case 'A':
								{
									if(data3num==800)
									{
										GPIO_SetBits(GPIOA,GPIO_Pin_6);
										GPIO_SetBits(GPIOA,GPIO_Pin_7);
									}
									else
									{
										GPIO_ResetBits(GPIOA,GPIO_Pin_6);
										GPIO_ResetBits(GPIOA,GPIO_Pin_7);
									}
								}break;
							case 'B':
								{ 
								//https://blog.csdn.net/dengchonglin/article/details/6223859
									RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
									 /* Allow access to BKP Domain */
									PWR_BackupAccessCmd(ENABLE);
									 /* Wait until last write operation on RTC registers has finished */
									RTC_SetCounter (0);
									RTC_WaitForLastTask();	
									RTC_Alarm_Set(data3num);	
								} break;
						}
					}
				}
			}
			data3num=0;
			USART_RX_STA=0;
		}
	}
}

