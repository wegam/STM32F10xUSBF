#ifdef PWM_TEST
#include "PWM_TEST.H"

#include "string.h"
#include "math.h"


#include "STM32_EXTI.H"
#include "STM32_USART.H"

#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_GPIO.H"
#include "STM32F10x_BitBand.H"

#define RxBufferSize	16

u16 PWM_Ratio=600;		//ռ�ձ�
u16 TIMECUNT=0;		//ռ�ձ�
u8 ADDflg=0;
u8 Ecodata=0;			//0--�ޱ仯��1--�ӣ�2--����3--����
u8 Aflg=0;	//A����
u8 Bflg=0;	//B����
u8 Buttonflg=0;	//����
u32 KeyCount=0;

u32 Key1Flag=0;
u32 Key2Count=0;
u32 Key2Flag=0;
u32 ECount=0;
u32 ECountA=0;
u32 ECountB=0;
u32 TimeOut=0;
u8 PWonFlg=0;
u8 RevBuffer[RxBufferSize]={0};
u8 RxdBuffer[RxBufferSize]={0};
u8 RxNum=0;
void PWM_ECODE(void);			//������
void PWM_ECODE2(void);			//������
void PWM_DELAY(u32 delay);
/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void PWM_TEST_Configuration(void)
{
	SYS_Configuration();				//ϵͳ����
	GPIO_DeInitAll();													//�����е�GPIO�ر�----V20170605
	SysTick_Configuration(100);	//ϵͳ���ʱ������72MHz,��λΪuS
	
//	GPIO_Configuration0();
//	TIM_Configuration(TIM1,7200,3000);	//��ʱʱ���趨
//	PWM_Configuration(TIM2,7200,10000,51);
//	PWM_OUT(TIM1,PWM_OUTChannel1,20000,50);		//PWM�趨
//	PWM_OUT(TIM2,PWM_OUTChannel1,5,100);	//PWM�趨
//	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);		//sys_led
//	PWM_OUT(TIM1,PWM_OUTChannel1,1200,500);		//PWM�趨
//	PWM_OUT2(TIM1,PWM_OUTChannel1,2400,600);		//PWM�趨
	PWM_OUT2(TIM3,PWM_OUTChannel3,2400,600);		//PWM�趨
	PWM_OUT2(TIM4,PWM_OUTChannel2,2400,600);		//PWM�趨
//	TIM1->CCR1 = 40000;
//	GPIO_Configuration_APP50	(GPIOB,	GPIO_Pin_14);			//��GPIO��Ӧ�ܽ�����ΪAPP(��������)���ģʽ������ٶ�50MHz----V20170605
	
//	GPIO_Configuration_IPU(GPIOA,	GPIO_Pin_4);			//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605--BUTTON
//	GPIO_Configuration_IPU(GPIOA,	GPIO_Pin_5);			//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605--A
//	GPIO_Configuration_IPU(GPIOA,	GPIO_Pin_6);			//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605--B
	
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_12);			//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605--BUTTON
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_13);			//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605--A
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_14);			//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605--B
	
//	EXTI_Configuration(GPIOB,GPIO_Pin_12,EXTI_Mode_Interrupt);
//	EXTI_Configuration2(GPIOB,GPIO_Pin_13,EXTI_Mode_Interrupt);
	
	USART_DMA_ConfigurationNR	(USART1,115200,(u32*)RxdBuffer,RxBufferSize);	//USART_DMA����--��ѯ��ʽ�������ж�
	
	USART_DMAPrintf(USART1,"�������÷���1��ʮ������:FE A9 XX XX��\r\n");					//�Զ���printf����DMA���ͳ���,��ߵ�ʡ�Ժž��ǿɱ����
	
	for(ECount=0;ECount<0xFFFF;ECount++)
	{
	}
	USART_DMAPrintf(USART1,"�������÷���2��SET:xx (xxȡֵΪ0~60000,50�ı�ֵΪ1/1200����\r\n");					//�Զ���printf����DMA���ͳ���,��ߵ�ʡ�Ժž��ǿɱ����
	for(ECount=0;ECount<0xFFFF;ECount++)
	{
	}
	USART_DMAPrintf(USART1,"D4J1-D4Q3ΪPWM���\r\n");					//�Զ���printf����DMA���ͳ���,��ߵ�ʡ�Ժž��ǿɱ����
	for(ECount=0;ECount<0xFFFF;ECount++)
	{
	}
	USART_DMAPrintf(USART1,"D4J1Ϊ���������룬���������ڲ���Ϊ1/1200\r\n");					//�Զ���printf����DMA���ͳ���,��ߵ�ʡ�Ժž��ǿɱ����
	for(ECount=0;ECount<0xFFFF;ECount++)
	{
	}
	
//	TIM3->CCR3 =10;
//	TIM4->CCR2 =10;
	
	PWonFlg=1;
	
//	GPIO_Configuration_IPD(GPIOA,	GPIO_Pin_4);			//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605--BUTTON
//	GPIO_Configuration_IPD(GPIOA,	GPIO_Pin_5);			//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
//	GPIO_Configuration_IPD(GPIOA,	GPIO_Pin_6);			//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	
//	PWM_OUT(TIM2,PWM_OUTChannel1,5,500);		//sys_led
//	PWM_OUT(TIM3,PWM_OUTChannel1,20000,30);		//PWM�趨
//	PWM_OUT(TIM4,PWM_OUTChannel1,20000,40);		//PWM�趨
//	
//	PWM_OUT(TIM1,PWM_OUTChannel2,20000,50);		//PWM�趨
//	PWM_OUT(TIM2,PWM_OUTChannel2,20000,500);	//PWM�趨

}
/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void PWM_TEST_Server(void)
{
	
//	SetPWM_Ratio(PWM_Ratio);		//����ռ�ձ�
	
	TIMECUNT++;
	if(TIMECUNT>=10)
	{
		TIMECUNT=0;
	}
	RxNum=USART_ReadBufferIDLE	(USART1,(u32*)RevBuffer,(u32*)RxdBuffer);	//���ڿ���ģʽ�����ڽ��ջ���������������ݣ������ݿ�����RevBuffer,�����ؽ��յ������ݸ�����Ȼ�����½����ջ�������ַָ��RxdBuffer
	if(RxNum)
	{
		if(RevBuffer[0]==0xFE&&RevBuffer[1]==0xA9)
		{
			PWM_Ratio=(RevBuffer[2]<<8)|(RevBuffer[3]<<0);
			
			if(PWM_Ratio>1200)
				PWM_Ratio=1200;
			USART_DMAPrintf(USART1,"��ǰ���ռ�ձ�:%4d/1200\r\n",PWM_Ratio);					//�Զ���printf����DMA���ͳ���,��ߵ�ʡ�Ժž��ǿɱ����
			TIM1->CCR1 =PWM_Ratio*50;
			TIM3->CCR3 =PWM_Ratio*50;
			TIM4->CCR2 =PWM_Ratio*50;
		}
		else if(RevBuffer[0]==0x53&&RevBuffer[1]==0x45&&RevBuffer[2]==0x54&&RevBuffer[3]==0x3A)
		{
			u8 i=0;
			double Npow=0;			
			PWM_Ratio=0;
			if(RxNum>=9&&RevBuffer[4]>=0x36)
			{
				PWM_Ratio=60000;
			}
			else
			{
				RxNum=RxNum-4;
				while(RxNum>i)
				{
					i++;
					Npow=(double)pow((float)10,(float)(RxNum-i));
					
					PWM_Ratio+=(u16)(RevBuffer[i+3]-0x30)*Npow;				
				}
			}
			TIM1->CCR1 =PWM_Ratio;
			TIM3->CCR3 =PWM_Ratio;
			TIM4->CCR2 =PWM_Ratio;
			Npow=(double)PWM_Ratio/50.0;
			PWM_Ratio	=(u16)Npow;
			USART_DMAPrintf(USART1,"��ǰ���ռ�ձ�:%4.4f/1200\r\n",Npow);					//�Զ���printf����DMA���ͳ���,��ߵ�ʡ�Ժž��ǿɱ����
		}
		memset(RevBuffer, 0x00,16);
	}
//	if(PWonFlg)
	PWM_ECODE2();			//������
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void PWM_ECODE2(void)
{
	if(PB12in==0)			//EXTI_GetFlagStatus(u32 EXTI_Line);//PA6in==0
	{
		if(Key1Flag==0)
		{
			KeyCount++;
			if(KeyCount>=200)
			{
				Key1Flag=1;
				if(PWM_Ratio<=1100)		//������ʶ
				{
					PWM_Ratio+=100;
				}
				else
				{
					PWM_Ratio=0;
				}	
				USART_DMAPrintf(USART1,"��ǰ���ռ�ձ�:%4d/1200\r\n",PWM_Ratio);					//�Զ���printf����DMA���ͳ���,��ߵ�ʡ�Ժž��ǿɱ����				
				TIM3->CCR3 =PWM_Ratio*50;
				TIM4->CCR2 =PWM_Ratio*50;
				
			}
		}
	}
	else
	{
		KeyCount=0;
		Key1Flag=0;
	}
	
	if(PB13in==0)			//EXTI_GetFlagStatus(u32 EXTI_Line);//PA6in==0
	{
		ECountA++;
		if(PB14in==0)
		{
			ECountB++;
		}
		else
		{
			ECountB=0;
		}
	}	
	else
	{
		if(ECountA>=200)
		{
			if(ECountA>ECountB+20)
			{
				if(PWM_Ratio<1200)
					PWM_Ratio+=1;
			}
			else
			{
				if(PWM_Ratio>=1)
					PWM_Ratio-=1;
			}	
			USART_DMAPrintf(USART1,"��ǰ���ռ�ձ�:%4d/1200\r\n",PWM_Ratio);					//�Զ���printf����DMA���ͳ���,��ߵ�ʡ�Ժž��ǿɱ����			
			TIM3->CCR3 =PWM_Ratio*50;
			TIM4->CCR2 =PWM_Ratio*50;
			
		}
		ECountA=0;
		ECountB=0;
	}
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void PWM_ECODE(void)
{
	if(EXTI_GetITStatus(EXTI_Line12)==SET)			//EXTI_GetFlagStatus(u32 EXTI_Line);//PA6in==0
	{
		KeyCount=0;
		while(KeyCount++<0xFFFF);
		if(PWM_Ratio<=1100)		//������ʶ
		{
			PWM_Ratio+=100;
		}
		else
		{
			PWM_Ratio=0;
		}
		USART_DMAPrintf(USART1,"��ǰ���ռ�ձ�:%4d/1200\r\n",PWM_Ratio);					//�Զ���printf����DMA���ͳ���,��ߵ�ʡ�Ժž��ǿɱ����
//		TIM1->CCR1 =PWM_Ratio*50;
		TIM3->CCR3 =PWM_Ratio*50;
		TIM4->CCR2 =PWM_Ratio*50;
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	
	if(EXTI_GetITStatus(EXTI_Line13)==SET)			//EXTI_GetFlagStatus(u32 EXTI_Line);//PA6in==0
	{
		TimeOut=0;
		Bflg=0;
		ECount=0;
		ECountA=0;
		ECountB=0;
		
		while(((PB13in==0||PB14in==0)||Bflg==0)&&(TimeOut<=0xFFF))
		{
			TimeOut++;
			if((PB13in==0&&PB14in==0)&&ECountA==0)
			{
				ECountA++;
				ECountB++;
			}
			else if(ECountA!=0)
			{
				if(PB13in==0)
				{
					ECountA++;
					ECount=0;
				}
				else
				{
					ECount++;				
				}
				if(PB14in==0)
				{
					ECountB++;
					ECount=0;
				}
				else
				{
					ECount++;				
				}
				if(ECount>=200)
				{
					Bflg=1;
				}
			}
		}
		
		if(ECountA>ECountB)
		{
			if(PWM_Ratio<1200)
				PWM_Ratio+=1;
		}
		else
		{
			if(PWM_Ratio>=1)
				PWM_Ratio-=1;
		}		
		EXTI_ClearITPendingBit(EXTI_Line13);
		
		USART_DMAPrintf(USART1,"��ǰ���ռ�ձ�:%4d/1200\r\n",PWM_Ratio);					//�Զ���printf����DMA���ͳ���,��ߵ�ʡ�Ժž��ǿɱ����
//		TIM1->CCR1 =PWM_Ratio*50;
		TIM3->CCR3 =PWM_Ratio*50;
		TIM4->CCR2 =PWM_Ratio*50;
//		EXTI_ClearITPendingBit(EXTI_Line13);
	}
}

/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void PWM_DELAY(u32 delay)
{
	while(delay--);
}


#endif