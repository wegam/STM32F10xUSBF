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

#ifdef PD003V11			//G,H�汾����ҩ�и��ذ�

#include "PD003V11.H"


#include "A3987.H"

#include "STM32F10x_BitBand.H"
#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_WDG.H"
#include "STM32_PWM.H"
#include "STM32_USART.H"

#include "string.h"				//�����ڴ��������ͷ�ļ�
#include "stm32f10x_dma.h"

#define RS485_Bus_RXEN	PA1=0			//rs485���߽���ʹ��
#define RS485_Bus_TXEN	PA1=1			//rs485���߷���ʹ��

#define BufferSize	256
u8 txBuffer_PD[BufferSize]={0};				//USART2-TXBF
u8 rxBuffer_PD[BufferSize]={0};				//USART2-RXBF

u8 RevBuffer[BufferSize]={0};
u8 RxdBuffer[BufferSize]={0};

u16	DelayTime=0;
u16	StepTime=0;

u16 Lock_Toggle_CNT=0;


u8 testFlg=0;
u8 txflg=0;
RS485_TypeDef RS4852;
A3987_Pindef	A3987_Pin1;
A3987_Pindef	A3987_Pin2;
void Lock_Toggle(void);			//˫�����������
void PD003VG_USART_Conf(void);
void USART2_RS485_Conf(void);
void PD003VG_485_Sens(void);

/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void PD003V11_Configuration(void)
{
	SYS_Configuration();					//ϵͳ����---��ϵͳʱ�� STM32_SYS.H
	
	GPIO_DeInitAll();							//�����е�GPIO�ر�----V20170605
	
	PD003V11_PinSet();
	
	USART2_RS485_Conf();
	
	SysTick_Configuration(1000);	//ϵͳ���ʱ������72MHz,��λΪuS
	
	IWDG_Configuration(1000);			//�������Ź�����---������λms	
	
	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);						//PWM�趨-20161127�汾
	
//	PD003VG_USART_Conf();
	
	RS485_Bus_TXEN;		//rs485���߽���ʹ��
//	USART_DMASend(USART3,(u32*)txBuffer_PD,BufferSize);			//�Զ���printf����DMA���ͳ���

}
/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void PD003V11_Server(void)
{	
	u16 len	=	0;
	IWDG_Feed();								//�������Ź�ι��
	DelayTime++;
	if(DelayTime>=1000)
		DelayTime	=	0;
//	if(DelayTime	==	0)
//	{
//		RS485_DMAPrintf(&RS4852,"����o����123\n\r");					//�Զ���printf����DMA���ͳ���,��ߵ�ʡ�Ժž��ǿɱ����
//		return;
//	}
//	len	=	RS485_ReadBufferIDLE(&RS4852,(u32*)RevBuffer,(u32*)RxdBuffer);
//	if(len)
//	{
//		RS485_DMAPrintf(&RS4852,"���յ����ݸ���:%d\n\r",len);					//�Զ���printf����DMA���ͳ���,��ߵ�ʡ�Ժž��ǿɱ����
//	}
//	StepTime++;
//	PD003VG_485_Sens();
	A3987_StepDriver(&A3987_Pin1);
	A3987_StepDriver(&A3987_Pin2);
//	Lock_Toggle();			//˫�����������
//	GPIO_Toggle	(GPIOC,	GPIO_Pin_1);	//������
//	if(DelayTime>=500)
//	{
//		DelayTime=0;
//		
//	}
//	if(StepTime>=2000)
//	{
//		StepTime=0;
////		GPIO_Toggle	(GPIOA,	GPIO_Pin_5);		//��GPIO��Ӧ�ܽ������ת----V20170605
//	}
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void USART2_RS485_Conf(void)
{
	RS4852.USARTx=USART2;
	
	RS4852.RS485_CTL_PORT=GPIOA;
	RS4852.RS485_CTL_Pin=GPIO_Pin_1;
	
	RS485_DMA_ConfigurationNR	(&RS4852,115200,(u32*)RxdBuffer,BufferSize);	//USART_DMA����--��ѯ��ʽ�������ж�
}
/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void PD003V11_PinSet(void)
{	
	
	A3987_Pin1.A3987_PUL_PORT		=	GPIOA;
	A3987_Pin1.A3987_PUL_Pin		=	GPIO_Pin_8;
	
	A3987_Pin1.A3987_DIR_PORT		=	GPIOC;
	A3987_Pin1.A3987_DIR_Pin		=	GPIO_Pin_9;
	
	A3987_Pin1.A3987_EN_PORT		=	GPIOC;
	A3987_Pin1.A3987_EN_Pin			=	GPIO_Pin_8;
	
	A3987_Pin1.A3987_MS1_PORT		=	GPIOC;
	A3987_Pin1.A3987_MS1_Pin		=	GPIO_Pin_7;
	
	A3987_Pin1.A3987_MS2_PORT		=	GPIOC;
	A3987_Pin1.A3987_MS2_Pin		=	GPIO_Pin_6;
	
	A3987_PinConf(&A3987_Pin1);
	
	PC9=0;
	
	
	A3987_Pin2.A3987_PUL_PORT		=	GPIOB;
	A3987_Pin2.A3987_PUL_Pin		=	GPIO_Pin_6;
	
	A3987_Pin2.A3987_DIR_PORT		=	GPIOB;
	A3987_Pin2.A3987_DIR_Pin		=	GPIO_Pin_5;
	
	A3987_Pin2.A3987_EN_PORT		=	GPIOB;
	A3987_Pin2.A3987_EN_Pin			=	GPIO_Pin_7;
	
	A3987_Pin2.A3987_MS1_PORT		=	GPIOB;
	A3987_Pin2.A3987_MS1_Pin		=	GPIO_Pin_8;
	
	A3987_Pin2.A3987_MS2_PORT		=	GPIOB;
	A3987_Pin2.A3987_MS2_Pin		=	GPIO_Pin_9;
	
	A3987_PinConf(&A3987_Pin2);
	


	//����������
	GPIO_Configuration_INF(GPIOB,	GPIO_Pin_12);			//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_INF(GPIOB,	GPIO_Pin_13);			//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605---�м䴫����
	GPIO_Configuration_INF(GPIOB,	GPIO_Pin_14);			//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605---����ߴ�����

	//˫�������1
	GPIO_Configuration_OPP50	(GPIOB,	GPIO_Pin_12);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,	GPIO_Pin_13);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	
	//˫�������1
	GPIO_Configuration_OPP50	(GPIOB,	GPIO_Pin_14);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,	GPIO_Pin_15);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	
	//������
	GPIO_Configuration_OPP50	(GPIOC,	GPIO_Pin_1);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	
	//���е�
	GPIO_Configuration_OPP50	(GPIOC,	GPIO_Pin_0);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	
	
}

/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void Lock_Toggle(void)
{
	Lock_Toggle_CNT++;
	if(Lock_Toggle_CNT>=5000)
	{
		Lock_Toggle_CNT=0;
		PB12=0;
		PB13=0;
		PB14=0;
		PB15=0;
	}
	else if(Lock_Toggle_CNT==100)
	{
		PB12=0;
		PB13=1;
		PB14=0;
		PB15=1;
		GPIO_Toggle	(GPIOC,	GPIO_Pin_1);	//������
		GPIO_Toggle	(GPIOC,	GPIO_Pin_0);	//���е�
	}
	else if(Lock_Toggle_CNT==200)
	{
		PB12=1;
		PB13=0;
		PB14=1;
		PB15=0;		
	}
	else if(Lock_Toggle_CNT>=300)
	{
		PB12=0;
		PB13=0;
		PB14=0;
		PB15=0;
		
		PC1=0;		//������
		PC0=1;		//���е�---�ر�
	}

}
#endif