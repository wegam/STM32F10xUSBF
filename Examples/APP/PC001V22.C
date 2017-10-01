/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : PC001V21.c
* Author             : WOW
* Version            : V2.0.1
* Date               : 06/26/2017
* Description        : PC001V21����ư�.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#ifdef PC001V22			//С�Ƴ����ذ�

#include "PC001V22.H"

#include "STM32_USART.H"

#include "STM32F10x_BitBand.H"
#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_WDG.H"
#include "STM32_PWM.H"

#include "string.h"				//�����ڴ��������ͷ�ļ�
#include "stm32f10x_dma.h"

#define RxBufferSize	512

u8 RevBuffer[RxBufferSize]={0};
u8 RxdBuffer[RxBufferSize]={0};

u8 RevBuffer2[RxBufferSize]={0};
u8 RxdBuffer2[RxBufferSize]={0};

u8 TevBuffer[RxBufferSize]={0};
u8 TxdBuffer[RxBufferSize]={0};

u8 strings[]="���ڿ���ģʽ�����ڽ��ջ���������������ݣ������ݿ�����RevBuffer,�����ؽ��յ������ݸ�����Ȼ�����½����ջ�������ַָ��RxdBuffer\n";

u16 RxNum=0;
u16 RxNum2=0;

u32	RuningTime=0;
u16 Tcnt=0;

RS485_TypeDef RS4851,RS4852;
void PC001V22_Conf(void);

/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void PC001V22_Configuration(void)
{
	SYS_Configuration();							//ϵͳ����---��ϵͳʱ�� STM32_SYS.H
	
	GPIO_DeInitAll();									//�����е�GPIO�ر�----V20170605
	
	PC001V22_Conf();
	
	SysTick_Configuration(100);				//ϵͳ���ʱ������72MHz,��λΪuS
	
	IWDG_Configuration(1000);					//�������Ź�����---������λms	
	
	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);						//PWM�趨-20161127�汾--SYS-LED

}
/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void PC001V22_Server(void)
{	
	IWDG_Feed();								//�������Ź�ι��
	RuningTime++;
	PA1=1;
	if(RuningTime>=200)				//1��
	{
		RuningTime=0;
		Tcnt++;
		if(Tcnt>=1000)
			Tcnt=0;
//		RS485_TX_EN(&RS4852);
		RS485_DMAPrintf(&RS4852,"printf-%d",Tcnt);					//�Զ���printf����DMA���ͳ���,��ߵ�ʡ�Ժž��ǿɱ����
//		USART_DMAPrintf(USART2,"���ڿ���ģʽ�����ڽ��ջ���������������ݣ������ݿ�����RevBuffer,�����ؽ��յ������ݸ�����Ȼ�����½����ջ�������ַָ��RxdBuffer---%4d\n",Tcnt);					//�Զ���printf����DMA���ͳ���,��ߵ�ʡ�Ժž��ǿɱ����
//		USART_DMASend(USART2,(u32*)strings,strlen(strings));											//����DMA���ͳ���
	}
//	RxNum=RS485_ReadBufferIDLE(&RS4851,(u32*)RevBuffer2,(u32*)RxdBuffer);	//���ڿ���ģʽ�����ڽ��ջ���������������ݣ������ݿ�����RevBuffer,�����ؽ��յ������ݸ�����Ȼ�����½����ջ�������ַָ��RxdBuffer
	if(RxNum)
	{
//		USART_DMASend(USART1,(u32*)RevBuffer2,RxNum);											//����DMA���ͳ���
	}
	
//	RxNum2=USART_ReadBufferIDLE(USART1,(u32*)RevBuffer,(u32*)RxdBuffer2);	//���ڿ���ģʽ�����ڽ��ջ���������������ݣ������ݿ�����RevBuffer,�����ؽ��յ������ݸ�����Ȼ�����½����ջ�������ַָ��RxdBuffer
	if(RxNum2)
	{
//		RS485_DMASend							(&RS4851,(u32*)RevBuffer,RxNum2);	//RS485-DMA���ͳ���
	}
	
//	RS485_RX_EN(&RS4851);		//��ʹ�ܣ��Ѿ�����Ϊ����״̬����1�����򷵻�0
	
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void PC001V22_Conf(void)
{
//	RS4851.USARTx=UART4;
//	
//	RS4851.RS485_CTL_PORT=GPIOA;
//	RS4851.RS485_CTL_Pin=GPIO_Pin_15;
//	
//	RS485_DMA_ConfigurationNR	(&RS4851,19200,(u32*)RxdBuffer,RxBufferSize);	//USART_DMA����--��ѯ��ʽ�������ж�
//	USART_DMA_ConfigurationNR	(USART1,115200,(u32*)RxdBuffer2,RxBufferSize);	//USART_DMA����--��ѯ��ʽ�������ж�
	
	
	RS4852.USARTx=USART2;
	
	RS4852.RS485_CTL_PORT=GPIOA;
	RS4852.RS485_CTL_Pin=GPIO_Pin_1;
	
	RS485_DMA_ConfigurationNR	(&RS4852,19200,(u32*)RxdBuffer,RxBufferSize);	//USART_DMA����--��ѯ��ʽ�������ж�
}










#endif