#ifdef USART_TEST

#include "USART_TEST.H"

#include "STM32_USART.H"
#include "STM32_DMA.H"
#include "STM32_TIM.H"
#include "STM32_WDG.H"
#include "STM32_PWM.H"
#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"

#define	BufferSize 1000		//DMA1�����С

u32	num_temp=0;
u16	tema=0;

u32 DMASTAST=0;
ErrorStatus status = ERROR;

u8	txflg1=0;	//USART1���ͱ�־
u16	tx1_tcont=0;	//USART1���ͳ�ʱ-��ʱ

u8 rxBuffer1[BufferSize]={0};
u8 txBuffer1[BufferSize]={0};
u8 num=0;
u8 ch[120]="USART_BASIC_Configuration(USART_TypeDef* USARTx,u32 USART_BaudRate,u8 NVICPreemptionPriority,u8 NVIC_SubPriority)\n";

//u8 itf=0;
/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void Usart_test_Configuration(void)
{
	SYS_Configuration();					//ϵͳ����---��ϵͳʱ�� STM32_SYS.H
	
	GPIO_DeInitAll();							//�����е�GPIO�ر�----V20170605
	
	SysTick_Configuration(1000);	//ϵͳ���ʱ������72MHz,��λΪuS
	
	IWDG_Configuration(1000);			//�������Ź�����---������λms	
	
	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);						//PWM�趨-20161127�汾	
	
	
	USART_DMA_Configuration(USART1,256000,1,1,(u32*)rxBuffer1,(u32*)txBuffer1,BufferSize);	//USART_DMA����
	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC, DISABLE);
	USART_ITConfig(USART1,USART_IT_IDLE, DISABLE);				//ʹ�ÿ����жϣ�DMA�Զ����գ���⵽���߿��б�ʾ���Ͷ��Ѿ�������ɣ����ݱ�����DMA��������
}
/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void Usart_test_Server(void)
{	
	IWDG_Feed();								//�������Ź�ι��
	Usart_test();
	

}
void Usart_test(void)
{
	u16 num=0;
	u8	flg_tx;	
	
	num=USART_RX_FlagClear(USART1);		
	if(num!=0xFFFF)
	{
		num=BufferSize-num;																	//��ȡ���ճ���
		memcpy(txBuffer1, rxBuffer1,num);										//��������
		memset(rxBuffer1, 0x00, BufferSize);								//�������
		USART_DMA_RxEN(USART1,(u32*)rxBuffer1,BufferSize);	//�����趨���ջ�������ַ����С��ʹ��DMA����
		USART_DMASend(USART1,(u32*)txBuffer1,num);					//����DMA���ͳ���
	}

	flg_tx=USART_TX_FlagClear(USART1);										//�������DMA���������ر�־
	if(flg_tx!=0)
	{
		memset(txBuffer1, 0x00, BufferSize);
	}			

	
//	u32 num=0;
//	//*********************BTL��USART1)***********************************
//	if(USART_GetFlagStatus(USART1,USART_FLAG_IDLE))
//	{
//		USART_ClearFlag(USART1,USART_FLAG_IDLE); 										//������д��ڱ�־λ
//		DMA_Cmd(DMA1_Channel5,DISABLE);    													//�رս���DMA
//		num = USART1->DR; 																					//������������������־			
//		num = BufferSize -  DMA_GetCurrDataCounter(DMA1_Channel5);	//�õ������������ݸ���
//		DMA1_Channel5->CMAR=(u32)rxBuffer1;													//��������DMA���յ�ַ
//		DMA1_Channel5->CNDTR=BufferSize;														//�������ý������ݸ���			
//		DMA_Cmd(DMA1_Channel5,ENABLE);  														//��������DMA	

//		txflg1=1;
//		tx1_tcont=0;
//		memcpy(txBuffer1, rxBuffer1,num);
////		memset(rxBuffer1, 0x00, num);
////		USART_DMA_RxEN(USART1,(u32*)rxBuffer1,BufferSize);	//�����趨���ջ�������ַ����С��ʹ��DMA����
////		USART_DMASend(USART1,(u32*)txBuffer1,num);
//		DMA1_Channel4->CNDTR =num;										//�趨�����ͻ�������С
//		DMA1_Channel4->CMAR =(u32)txBuffer1;							//���ͻ�����	
//		DMA_Cmd(DMA1_Channel4,ENABLE);											//DMA���Ϳ���3
//		memset(rxBuffer1, 0x00, BufferSize);
//		
//	}
//	if(txflg1==1)
//	{
//		if(tx1_tcont++>100)		//100ms
//		{
//			txflg1=0;
//			tx1_tcont=0;
//			USART_ClearFlag(USART1,USART_FLAG_TC); 										//������д��ڱ�־λ
//			DMA_ClearFlag(DMA1_FLAG_GL4);
//			DMA_Cmd(DMA1_Channel4,DISABLE);  													//�ر�DMA����
//			memset(txBuffer1, 0x00, BufferSize);
//		}
//		else
//		{
//			if(DMA_GetFlagStatus(DMA1_FLAG_GL4))
//			{
//				if(USART_GetFlagStatus(USART1,USART_FLAG_TC))
//				{
//					txflg1=0;
//					tx1_tcont=0;
//					USART_ClearFlag(USART1,USART_FLAG_TC); 										//������д��ڱ�־λ
//					DMA_ClearFlag(DMA1_FLAG_GL4);
//					DMA_Cmd(DMA1_Channel4,DISABLE);  													//�ر�DMA����
//					memset(txBuffer1, 0x00, BufferSize);
//				}
//			}
//		}
//	}
	
	

	
	
	
//	ErrorStatus status = ERROR;
//	u32 num=0;
//	num_temp=USART_RX_FlagClear(USART1);						//�������DMA����ȫ���жϱ�־
//	num_temp=USART_RX_FlagClearADDR(USART1,(u32*)Usart_Test_Buffer);
	
//	status=USART_TX_DMAFlagClear(USART1);						//�������DMA����ȫ���жϱ�־
//	num_temp=USART_RX_FlagClear(USART1);						//�������DMA����ȫ���жϱ�־
//	status=USART_TX_DMAFlagClear(USART1);						//�������DMA����ȫ���жϱ�־
//	DMASTAST=*(vu32*)DMA1_Channel4_BASE;
//	if(num_temp!=0)
//	{
//		memcpy(Usart_TXBuffer,Usart_RXBuffer,num_temp);
//		USART_DMAPrintf(USART1,"���յ������ݸ���:%d;���յ�������:%s\n",num_temp,Usart_TXBuffer);
//		tema=0;
//	}
//	else if(!USART_TX_DMAFlagClear(USART1))
//	{
//		memset(Usart_TXBuffer,0,DMA1_BufferSize);			//��ʼ������
//		if(tema<=100)
//		{		
//			USART_DMAPrintf(USART1,"���Կ�ʼ:%d\n",tema);
//			tema++;
//		}
//	}
	
//	if(num_temp)
//	{
////		num_temp=USART_RX_FlagClear(USART1);						//�������DMA����ȫ���жϱ�־
//		memcpy(Usart_TXBuffer,Usart_RXBuffer,num_temp);
//		USART_DMASend(USART1,(u32*)Usart_TXBuffer,num_temp);
////		num_temp=0;
//	}
//	else if(!USART_TX_DMAFlagClear(USART1))
//	{
//		memset(Usart_Test_Buffer,0,DMA1_BufferSize);			//��ʼ������
//		if(tema<=100)
//		{		
//			USART_DMAPrintf(USART1,"���Կ�ʼ:%d\n",tema);
////			USART_DMAPrintf(USART1,"���Կ�ʼ:%s\n",ch);
////			printf("���Կ�ʼ:%d\n",tema);
//			tema++;
//		}
////		else
////			tema=0;
//	}
	
/*
	ErrorStatus status = ERROR;
	status=USART_TX_DMAFlagClear(USART1);						//�������DMA����ȫ���жϱ�־
	USART_RX_FlagClear(USART1);				//������ڽ����жϱ�־
	if(status==SUCCESS)
	{
//		free(USART_BUFFER);						//�ͷŶ�̬�ռ�
	}
	else
	{
		if(tema<=100)
		{		
//			USART_DMAPrintf(USART1,"���Կ�ʼ:%d\n",tema);
			USART_DMAPrintf(USART1,"���Կ�ʼ:%s\n",ch);
//			printf("���Կ�ʼ:%d\n",tema);
			tema++;
		}
		else
			tema=0;
	}
*/
//	IWDG_Feed();
/*	//USART1
	if(USART_GetITStatus(USART1,USART_IT_IDLE))
  {
			USART_ClearITPendingBit(USART1,USART_IT_IDLE); 					//������д��ڱ�־λ
			num = USART1->DR; 												//������������������־
			DMA_Cmd(DMA1_Channel5,DISABLE);    										//�رս���DMA
			num = DMA1_BufferSize -  DMA_GetCurrDataCounter(DMA1_Channel6);	//�õ������������ݸ���
			DMA1_Channel4->CNDTR =num;										//�趨���������ݸ���
			DMA1_Channel5->CNDTR=DMA1_BufferSize;       					//�������ý������ݸ���				
			DMA_Cmd(DMA1_Channel5,ENABLE);  								//��������DMA			
			DMA_Cmd(DMA1_Channel4,ENABLE);									//DMA���Ϳ���
			tema=0;
	}
	else if(DMA_GetFlagStatus(DMA1_FLAG_GL4))
	{
		DMA_ClearFlag(DMA1_FLAG_GL4);
		DMA_Cmd(DMA1_Channel4,DISABLE);			//DMA���͹ر�
		free(USART_BUFFER);						//�ͷŶ�̬�ռ�
	}
	else
	{
		if(tema<=100)
		{		
//			USART_DMASend(USART2,"���Կ�ʼ:%d\n",tema);
			printf("���Կ�ʼ:%d\n",tema);
			tema++;
		}
	}
*/
/*
	//USART2
	if(USART_GetITStatus(USART2,USART_IT_IDLE))
  {
			USART_ClearITPendingBit(USART2,USART_IT_IDLE); 					//������д��ڱ�־λ
			num = USART2->DR; 												//������������������־
			DMA_Cmd(DMA1_Channel6,DISABLE);    										//�رս���DMA
			num = DMA1_BufferSize -  DMA_GetCurrDataCounter(DMA1_Channel6);	//�õ������������ݸ���
			DMA1_Channel7->CNDTR =num;										//�趨���������ݸ���
			DMA1_Channel6->CNDTR=DMA1_BufferSize;       	//�������ý������ݸ���				
			DMA_Cmd(DMA1_Channel6,ENABLE);  							//��������DMA			
			DMA_Cmd(DMA1_Channel7,ENABLE);								//DMA���Ϳ���
			tema=0;
	}
	if(DMA_GetFlagStatus(DMA1_FLAG_TC7))
	{
		DMA_ClearFlag(DMA1_FLAG_TC7);
		DMA_Cmd(DMA1_Channel7,DISABLE);			//DMA���͹ر�
		free(USART_BUFFER);						//�ͷŶ�̬�ռ�
	}
	else
	{
		if(tema<=1000)
		{		
			USART_DMASend(USART2,"���Կ�ʼ:%d\n",tema);
//			printf("���Կ�ʼ:%d\n",tema);
			tema++;
		}
		else
			tema=0;
	}
*/

//	if(tema<30000)
//		tema++;
//	else
//		tema=0;
	
//	if(DMA_GetFlagStatus(DMA1_FLAG_TC7))
//	{
//		DMA_ClearFlag(DMA1_FLAG_TC7);
//		DMA_Cmd(DMA1_Channel7,DISABLE);			//DMA���͹ر�
//		free(USART_BUFFER);									//�ͷŶ�̬�ռ�
//	}
//	else
//	{
//			USART_DMASend(USART2,"���Կ�ʼ:%d\n",tema);
////			printf("���Կ�ʼ:%d\n",tema);
//	}
//	if(num!=0)
//	{
//		GPIO_SetBits(GPIOA,GPIO_Pin_1);
////		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
//		GPIO_Write(GPIOE,0XAAAA);
//		num=0;
//	}
//	else
//	{
////		GPIO_SetBits(GPIOA,GPIO_Pin_2);
//		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
//		GPIO_Write(GPIOE,0X5555);
//		num=1;
//	}
}
#endif
