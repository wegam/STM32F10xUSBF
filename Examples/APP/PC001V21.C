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
/********************************��������*************************************/
//	����	SwitchID	����
//	��1��07 ID 01	
//	��2��07 ID 02
//	��3��07 ID 03
//	��4��07 ID 04

/********************************��������*************************************/
//	����	SwitchID
//	05 ID	


#ifdef PC001V21				//����ҩ�����ذ�

#include "PC001V21.H"



#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_WDG.H"
#include "STM32_USART.H"
#include "STM32_PWM.H"
#include "STM32F10x_BitBand.H"

#include "string.h"				//�����ڴ��������ͷ�ļ�
#include "stm32f10x_dma.h"

#ifdef	USART_TO_RS485
	#define BufferSize	1024
#else
	#define BufferSize	34
#endif
#define Command_ReadData	0x05
#define Command_SendData	0x06
#define DR_CTRL	0x07			//PMOS����

#define	DR_OUT1	PA6
#define	DR_OUT2	PA5
#define	DR_OUT3	PA4
#define	DR_OUT4	PA7

#define	DR_IN1	PC13in
#define	DR_IN2	PC14in
#define	DR_IN3	PC15in
#define	DR_IN4	PB0in

u16 dr_ctime=0;

//#define	USART_TO_RS485		//USARTת485��485תUSART����

u16 sysledcnt=0;			//ϵͳ����ָʾ��ɨ�����  0.5��

u8 txBuffer1[BufferSize]={0};			//USART1-TXBF
u8 rxBuffer1[BufferSize]={0};			//USART1-RXBF

u8 txBuffer_Bus[BufferSize]={0};			//USART3-TXBF
u8 rxBuffer_Bus[BufferSize]={0};			//USART3-RXBF

u8 txBuffer_PD[BufferSize]={0};				//USART2-TXBF
u8 rxBuffer_PD[BufferSize]={0};				//USART2-RXBF

u8	txflg1=0;	//USART1���ͱ�־
u8	txflg2=0;	//485BUS���ͱ�־
u8	txflg3=0;	//485PD���ͱ�־

u8	flg3_txEN=0;	//����ذ巢������ʹ��
u8	flg3_txtime=0;	//����ذ巢������ʹ��

u8	rxflg=0;	//���ձ�־
u8	bpflg=0;	//������ʹ�ܱ�־

u8 SwitchID=0;	//���뿪�ص�ַ
u16	tx1_tcont=0;	//USART1���ͳ�ʱ-��ʱ
u16	tx2_tcont=0;	//485BUS���ͳ�ʱ-��ʱ
u16	tx3_tcont=0;	//485PD���ͳ�ʱ-��ʱ

u16	getArray_tcont=0;	//��ȡADֵ͢ʱ

#define RS485_Bus_RXEN	PB1=0			//rs485���߽���ʹ��
#define RS485_Bus_TXEN	PB1=1			//rs485���߷���ʹ��
u8	RS485_Bus_txflg=0;						//���ͱ�־
u8	RS485_Bus_rxflg=0;						//���ձ�־
u8	Command_RS485_Bus=0;

#define RS485_PD_RXEN		PA1=0				//rs485���ճ��ذ�ʹ��
#define RS485_PD_TXEN		PA1=1				//rs485���ͳ��ذ�ʹ��

u8	RS485_PD_txflg=0;							//���ͱ�־
u8	RS485_PD_rxflg=0;							//���ձ�־
u8	RS485_PD_txAddr=0;						//�������
u8	RS485_PD_rxAddr=0;						//�������
u8	RS485_PD_tryCont=0;						//���Լ���
u16	RS485_PD_trytime=0;						//��ʱ��ʱ
u16	GetBufferDelayTime=0;						//��ʱ��ʱ

/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void PC001V21_Configuration(void)
{
	SYS_Configuration();					//ϵͳ����---��ϵͳʱ�� STM32_SYS.H
	
	GPIO_DeInitAll();							//�����е�GPIO�ر�----V20170605
	
	PC001V21_PinSet();
	
	SysTick_Configuration(100);		//ϵͳ���ʱ������72MHz,��λΪuS
	
	IWDG_Configuration(1000);			//�������Ź�����---������λms	
	
	GPIO_Configuration_OPP2	(GPIOC,GPIO_Pin_1);			//BUZZER//��GPIO��Ӧ�ܽ�����ΪOD(��©)���ģʽ������ٶ�2MHz----V20170605
	
	GPIO_Configuration_IPU	(GPIOB,GPIO_Pin_9);			//S1//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU	(GPIOB,GPIO_Pin_8);			//S2//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU	(GPIOB,GPIO_Pin_7);			//S3//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU	(GPIOB,GPIO_Pin_6);			//S4//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU	(GPIOB,GPIO_Pin_5);			//S5//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU	(GPIOB,GPIO_Pin_4);			//S6//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU	(GPIOB,GPIO_Pin_3);			//S7//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU	(GPIOA,GPIO_Pin_15);		//S8//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	
	
	PC001V21_USART_Conf();
	
	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);						//PWM�趨-20161127�汾
}
/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void PC001V21_Server(void)
{	
	IWDG_Feed();													//�������Ź�ι��
	SwitchID=PC001V21_GetSwitchID();			//��ȡ���뿪�ص�ַ
	PC001V21_USART1_Server();
	PC001V21_4851_Server();
	PC001V21_4852_Server();
	DR_LOCK_Check();//������״̬���	

	if(GetBufferDelayTime++>1000)			//100ms
	{

//		RS485_PD_rxAddr=0;
//		RS485_PD_txAddr=0;
//		getArray_tcont=0;
//		RS485_PD_trytime=0;
//		flg3_txEN=1;
//		RS485_PD_tryCont=0;
		PC001V21_GetBufferArray();						//��ȡ�������ADֵ
	}
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void PC001V21_PinSet(void)
{
	//pmos�������
	GPIO_Configuration_OPP50	(GPIOA,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);					//485(uart3-en)//��GPIO��Ӧ�ܽ�����ΪOD(��©)���ģʽ������ٶ�2MHz----V20170605
	//pmos״̬���
	GPIO_Configuration_IPU(GPIOB,	GPIO_Pin_0);			//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU(GPIOC,	GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);			//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
}

/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void PC001V21_USART_Conf(void)
{
	//********************BTL��USART1)***********************************
	USART_DMA_Configuration(USART1,19200,1,1,(u32*)rxBuffer1,(u32*)txBuffer1,BufferSize);	//USART_DMA����
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC, DISABLE);
	USART_ITConfig(USART1,USART_IT_IDLE, DISABLE);				//ʹ�ÿ����жϣ�DMA�Զ����գ���⵽���߿��б�ʾ���Ͷ��Ѿ�������ɣ����ݱ�����DMA��������
	
	//*********************����485���뵥Ԫ������--USART3)***********************************
	USART_DMA_Configuration(USART3,19200,1,1,(u32*)rxBuffer_Bus,(u32*)txBuffer_Bus,BufferSize);	//USART_DMA����
	DMA_ITConfig(DMA1_Channel2,DMA_IT_TC, DISABLE);
	USART_ITConfig(USART3,USART_IT_IDLE, DISABLE);				//ʹ�ÿ����жϣ�DMA�Զ����գ���⵽���߿��б�ʾ���Ͷ��Ѿ�������ɣ����ݱ�����DMA��������
	
	//*********************�ڲ�485�����������--USART2)***********************************
	USART_DMA_Configuration(USART2,19200,1,1,(u32*)rxBuffer_PD,(u32*)txBuffer_PD,BufferSize);	//USART_DMA����
	DMA_ITConfig(DMA1_Channel7,DMA_IT_TC, DISABLE);
	USART_ITConfig(USART2,USART_IT_IDLE, DISABLE);				//ʹ�ÿ����жϣ�DMA�Զ����գ���⵽���߿��б�ʾ���Ͷ��Ѿ�������ɣ����ݱ�����DMA��������
	
	GPIO_Configuration_OPP50	(GPIOA,GPIO_Pin_1);					//485(uart2-en)//��GPIO��Ӧ�ܽ�����ΪOD(��©)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(GPIOB,GPIO_Pin_1);					//485(uart3-en)//��GPIO��Ӧ�ܽ�����ΪOD(��©)���ģʽ������ٶ�2MHz----V20170605
	
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void PC001V21_USART1_Server(void)
{
	u16 num=0;
	num=USART_RX_FlagClear(USART1);																//������ڽ����жϱ�־--����DMAʣ�໺������С
	//*********************BTL��USART1)***********************************
	if(num!=0xFFFF)
	{
		num=BufferSize-num;																	//�õ������������ݸ���

#ifdef	USART_TO_RS485
		txflg2=1;																						//���ͱ�־
		tx2_tcont=0;
		RS485_Bus_TXEN;																			//rs485���߷���ʹ��
		memcpy(txBuffer_Bus, rxBuffer1,num);
		USART_DMASend(USART3,(u32*)txBuffer_Bus,num);				//����DMA���ͳ���
#else
		if((rxBuffer1[0]==Command_ReadData)&&((rxBuffer1[1]==SwitchID)||(rxBuffer_Bus[1]==0x00)))
		{
			txflg1=1;	//���ͱ�־
			tx1_tcont=0;
			txBuffer1[0]=Command_ReadData;
			txBuffer1[1]=SwitchID;			
			USART_DMASend(USART1,(u32*)txBuffer1,BufferSize);	//����DMA���ͳ���						
		}
		else if((rxBuffer1[0]==DR_CTRL)&&(rxBuffer1[1]==SwitchID))
		{
			DR_LOCK_OUT(rxBuffer1[2]);
		}
#endif
		memset(rxBuffer1, 0x00, BufferSize);
		USART_DMA_RxEN(USART1,(u32*)rxBuffer1,BufferSize);	//�����趨���ջ�������ַ����С��ʹ��DMA����		
	}
	//*********************������ͱ�־***********************************
	if(txflg1==1)
	{
		if(tx1_tcont++>250)		//100ms---��ʱ
		{
			txflg1=0;
			tx1_tcont=0;
			USART_ClearFlag(USART1,USART_FLAG_TC); 										//������д��ڱ�־λ
			DMA_ClearFlag(DMA1_FLAG_GL4);
			DMA_Cmd(DMA1_Channel4,DISABLE);  													//�ر�DMA����
			memset(txBuffer1, 0x00, BufferSize);
		}
		else
		{
			u8 flg_tx=0;
			flg_tx=USART_TX_FlagClear(USART1);									//�������DMA����ȫ���жϱ�־
			if(flg_tx)
			{
					txflg1=0;
					tx1_tcont=0;
					memset(txBuffer1, 0x00, BufferSize);
			}
		}
	}	
}
/*******************************************************************************
* ������			:	function
* ��������		:	���ذ�
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void PC001V21_4851_Server(void)
{
	u16 num=0;
	num=USART_RX_FlagClear(USART2);																//������ڽ����жϱ�־--����DMAʣ�໺������С
	//*********************DMA���մ���***********************************
	if(num!=0xFFFF)
	{
		num=BufferSize-num;																	//�õ������������ݸ���
		//-----------------------���ճ��ذ��ϱ�������
		if(rxBuffer_PD[0]==0x05)
		{
			u8 bufBase=0;
			u8 i=0;
			RS485_PD_rxAddr=rxBuffer_PD[1];
			if(rxBuffer_PD[1]==0x01)		//����1˫ͨ��
			{
				bufBase=1;
			}
			else if(rxBuffer_PD[1]==0x03)	//����2˫ͨ��
			{
				bufBase=9;
			}
			else if(rxBuffer_PD[1]==0x05)	//����3˫ͨ��
			{
				bufBase=17;
			}
			else if(rxBuffer_PD[1]==0x07)	//����4˫ͨ��
			{
				bufBase=25;
			}
			
			for(i=1;i<9;i++)							//�����ݷ���������Ӧλ��(RS485���߻�������USART1��---����ʽ����
			{
				txBuffer_Bus[bufBase+i]=rxBuffer_PD[i];				//RS485-BUS
				txBuffer1[bufBase+i]=rxBuffer_PD[i];					//USART1
			}		
		}
		memset(rxBuffer_PD, 0x00, num);
		USART_DMA_RxEN(USART2,(u32*)rxBuffer_PD,BufferSize);	//�����趨���ջ�������ַ����С��ʹ��DMA����		
	}
	//*********************DMA���ʹ���***********************************
	if(flg3_txEN)
	{
		RS485_PD_TXEN;
		txflg3=1;											//���ͱ�־
		flg3_txEN=0;
		txBuffer_PD[0]=Command_ReadData;
		txBuffer_PD[1]=RS485_PD_txAddr;
		USART_DMASend(USART2,(u32*)txBuffer_PD,2);						//�Զ���printf����DMA���ͳ���
	}
	if(txflg3==1)
	{
		if(tx3_tcont++>500)		//50ms
		{
			txflg3=0;
			tx3_tcont=0;
			flg3_txEN=0;
			USART_ClearFlag(USART2,USART_FLAG_TC); 										//������д��ڱ�־λ
			DMA_ClearFlag(DMA1_FLAG_GL7);
			DMA_Cmd(DMA1_Channel7,DISABLE);  													//�ر�DMA����
			memset(txBuffer_PD, 0x00, BufferSize);
			RS485_PD_RXEN;
		}	
		else			
		{
			u8 flg_tx=0;
			flg_tx=USART_TX_FlagClear(USART2);									//�������DMA����ȫ���жϱ�־
			if(flg_tx)
			{
					txflg3=0;
					tx3_tcont=0;
					flg3_txEN=0;
					memset(txBuffer_PD, 0x00, BufferSize);
					RS485_PD_RXEN;
			}
		}
	}
}
/*******************************************************************************
* ������			:	function
* ��������		:	RS485�ⲿ���� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void PC001V21_4852_Server(void)
{
	u16 num=0;
	num=USART_RX_FlagClear(USART3);												//������ڽ����жϱ�־--����DMAʣ�໺������С
	//*********************DMA���մ���***********************************
	if(num!=0xFFFF)
	{
		num=BufferSize-num;																	//�õ������������ݸ���

#ifdef	USART_TO_RS485
		txflg1=1;																						//���ͱ�־
		tx1_tcont=0;
		memcpy(txBuffer1, rxBuffer_Bus,num);
		USART_DMASend(USART1,(u32*)txBuffer1,num);				//����DMA���ͳ���		
#else
		if((rxBuffer_Bus[0]==Command_ReadData)&&((rxBuffer_Bus[1]==SwitchID)||(rxBuffer_Bus[1]==0x00)))
		{
			txflg2=1;	//���ͱ�־
			tx2_tcont=0;
			RS485_Bus_TXEN;		//rs485���߷���ʹ��
			txBuffer_Bus[0]=Command_ReadData;
			txBuffer_Bus[1]=SwitchID;
			USART_DMASend(USART3,(u32*)txBuffer_Bus,BufferSize);			//�Զ���printf����DMA���ͳ���
		}
#endif
		memset(rxBuffer_Bus, 0x00, BufferSize);
		USART_DMA_RxEN(USART3,(u32*)rxBuffer_Bus,BufferSize);	//�����趨���ջ�������ַ����С��ʹ��DMA����
		
	}
	//*********************DMA���ʹ���***********************************
	else if(txflg2==1)
	{
		if(tx2_tcont++>100)		//100ms
		{
			txflg2=0;
			tx2_tcont=0;
			RS485_Bus_RXEN;		//rs485���߽���ʹ��
			USART_ClearFlag(USART3,USART_FLAG_TC); 										//������д��ڱ�־λ
			DMA_ClearFlag(DMA1_FLAG_GL2);
			DMA_Cmd(DMA1_Channel2,DISABLE);  													//�ر�DMA����
			memset(txBuffer_Bus, 0x00, BufferSize);
		}
		else
		{
			u8 flg_tx=0;
			flg_tx=USART_TX_FlagClear(USART3);									//�������DMA����ȫ���жϱ�־
			if(flg_tx)
			{
				txflg2=0;
				tx2_tcont=0;
				RS485_Bus_RXEN;		//rs485���߽���ʹ��
				memset(txBuffer_Bus, 0x00, BufferSize);
			}
		}
	}	
}
/*******************************************************************************
* ������			:	function
* ��������		: ��ȡ���뿪�ص�ַ
* ����			: void
* ����ֵ			: void
*******************************************************************************/
u8 PC001V21_GetSwitchID(void)
{
	u8 SwitchTemp=0;	
	
//	if(PB9in==0)				//S1
//		SwitchTemp|=1<<0;
//	if(PB8in==0)				//S2
//		SwitchTemp|=1<<1;
//	if(PB7in==0)				//S3
//		SwitchTemp|=1<<2;
//	if(PB6in==0)				//S4
//		SwitchTemp|=1<<3;
//	if(PB5in==0)				//S5
//		SwitchTemp|=1<<4;
//	if(PB4in==0)				//S6
//		SwitchTemp|=1<<5;
//	if(PB3in==0)				//S7
//		SwitchTemp|=1<<6;
//	if(PA15in==0)				//S8
//		SwitchTemp|=1<<7;
	

		SwitchTemp|=PB9in<<0;		//S1
		SwitchTemp|=PB8in<<1;		//S2
		SwitchTemp|=PB7in<<2;		//S3
		SwitchTemp|=PB6in<<3;		//S4
		SwitchTemp|=PB5in<<4;		//S5
		SwitchTemp|=PB4in<<5;		//S6
		SwitchTemp|=PB3in<<6;		//S7
		SwitchTemp|=PA15in<<7;	//S8
		
		SwitchTemp=~SwitchTemp;
	
	return SwitchTemp;
}
/*******************************************************************************
* ������			:	PC001V21_GetBufferArray
* ��������		: ��ȡ4������8��ͨ����ADֵ��һ��һ�������ȡ
* ����			: void
* ����ֵ			: void
*******************************************************************************/
u8 PC001V21_GetBufferArray(void)
{
//	RS485_PD_tryCont;
	//���͵�ַ���� 
	if(RS485_PD_rxAddr==0x00)			//��ʼ��һ������
	{
//		RS485_PD_tryCont=0;
		RS485_PD_txAddr=0x01;				//��һ�������ַ
	}
	else if(RS485_PD_rxAddr==0x01)	//��һ��������ִ�У���ʼ�ڶ�������
	{
		RS485_PD_txAddr=0x03;				//�ڶ��������ַ
	}
	else if(RS485_PD_rxAddr==0x03)	//�ڶ���������ִ�У���ʼ����������
	{
		RS485_PD_txAddr=0x05;				//�����������ַ
	}
	else if(RS485_PD_rxAddr==0x05)	//������������ִ�У���ʼ���ĸ�����
	{
		RS485_PD_txAddr=0x07;				//���ĸ������ַ
	}
	else if(RS485_PD_rxAddr==0x07)	//�ĸ����붼ִ�����
	{
		GetBufferDelayTime=0;
		RS485_PD_txAddr=0x00;				
		RS485_PD_rxAddr=0x00;	
	}
	else
	{
//		GetBufferDelayTime=0;
		RS485_PD_txAddr=0x00;				
		RS485_PD_rxAddr=0x00;	
	}
	
	//�쳣���Է��ͣ�����50msδ���յ���Ӧ�������ݵ���ͨѶʧ�ܣ�����3��
	
	if((flg3_txEN==0)&&(GetBufferDelayTime>=1000))
	{
		if(getArray_tcont++>200)
		{
			txflg3=0;											//���ͱ�־
			flg3_txEN=1;
			getArray_tcont=0;
			RS485_PD_trytime=0;
		}
		if(RS485_PD_rxAddr!=RS485_PD_txAddr)		//����
		{
			if(RS485_PD_tryCont==0)
			{
				RS485_PD_tryCont++;
				RS485_PD_trytime=0;
				getArray_tcont=0;
				flg3_txEN=1;
			}
			if(RS485_PD_trytime++>100)		//����50msδ���յ���Ӧ�������ݵ���ͨѶʧ�ܣ�����3��
			{
				RS485_PD_trytime=0;
				getArray_tcont=0;
				flg3_txEN=1;
				if(RS485_PD_tryCont++>=3)
				{
					txflg3=0;
					tx3_tcont=0;
					RS485_PD_tryCont=0;
					getArray_tcont=0;
					RS485_PD_rxAddr=RS485_PD_txAddr;		//������ǿ���޸�RS485_PD_rxAddr
				}
			}
		}
		
	}
	return 0;
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void DR_LOCK_OUT(unsigned char dr)
{
	dr_ctime=100;
	switch(dr)
	{
		case 0:DR_OUT1=0;DR_OUT2=0;DR_OUT3=0;DR_OUT4=0;
		break;
		case 1:DR_OUT1=1;
		break;
		case 2:DR_OUT2=1;
		break;
		case 3:DR_OUT3=1;
		break;
		case 4:DR_OUT4=1;
		break;
		default:DR_OUT1=0;DR_OUT2=0;DR_OUT3=0;DR_OUT4=0;
		break;
	}
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void DR_LOCK_Check(void)
{
	if(dr_ctime>0)
	{
		if(--dr_ctime==0)
		{
			DR_OUT1=0;
			DR_OUT2=0;
			DR_OUT3=0;
			DR_OUT4=0;
		}
	}
	if(DR_IN1==0)
	{
		DR_OUT1=0;
	}
	if(DR_IN2==0)
	{
		DR_OUT2=0;
	}
	if(DR_IN3==0)
	{
		DR_OUT3=0;
	}
	if(DR_IN4==0)
	{
		DR_OUT4=0;
	}
	
}
#endif