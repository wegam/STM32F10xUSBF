/******************************** User_library *********************************
* �ļ��� 	: STM32_SDCard.H
* ����   	: wegam@sina.com
* �汾   	: V
* ����   	: 2016/01/01
* ˵��   	: 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


#include "STM32_TIM.H"
#include "STM32_WOW.H"
//#include "STM32F10x_BitBand.H"



///* ��ʱ���ṹ�� */
///* TIM Time Base Init structure definition */
//typedef struct
//{
//  u16 TIM_Prescaler;								//-------��Ƶϵ��,=======ȡֵ0x0000~0xFFFF��������ƵTIM clock
//  u16 TIM_CounterMode;							//-------������ʽ========TIM_CounterMode_Up(���ϼ���ģʽ),TIM_CounterMode_Down(���¼���ģʽ),
																			//-----------------------TIM_CounterMode_CenterAligned1(�������ģʽ1����ģʽ),TIM_CounterMode_CenterAligned2(�������ģʽ2����ģʽ),TIM_CounterMode_CenterAligned3(�������ģʽ3����ģʽ)
//  u16 TIM_Period;										//-------����ֵ==========��һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ,ȡֵ0x0000~0xFFFF,����TIM_Period+1�����ĺ�������
//  u16 TIM_ClockDivision;						//-------�趨ʱ��ָ�ֵ===Ĭ��Ϊ0,���ⳡ��ʱTIM_ClockDivision������һ����ʱ,TIM_CKD_DIV1,TIM_CKD_DIV2,TIM_CKD_DIV3
//  u8 TIM_RepetitionCounter;					//-------�ظ���������=====�ظ����ٴ������Ŵ���һ������жϣ�
//} TIM_TimeBaseInitTypeDef;



/*******************************************************************************
*������		: TIM_Configuration
*��������	:��ʱʱ���趨
*����			:TIMx--TIMx--��ʱ����
						x����Ϊ1,2,3,4,5,6,7����8
						Prescaler---��Ƶϵ��,(ȡֵ0x0000~0xFFFF)������ƵTIM clock
						Period	---����ֵ,(ȡֵ0x0000~0xFFFF),����TIM_Period+1�����ĺ������?
						TIM_ClockDivision----�趨ʱ��ָ�ֵ,(Ĭ��Ϊ0,���ⳡ��ʱTIM_ClockDivision������һ����ʱ)
						TIM_CounterMode---������ʽ
						TIM_RepetitionCounter---�ظ���������,(�ظ����ٴ������Ŵ���һ������ж�)
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
void TIM_Configuration(TIM_TypeDef* TIMx,u16 Prescaler,u16 Period)	//��ʱʱ���趨
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;	//��ʱ���ṹ�嶨��
	NVIC_InitTypeDef	NVIC_InitStructure;						//�жϽṹ��
	u8 TIM_IRQChannel=0;
	//1)**********������ر���	
	switch (*(u32*)&TIMx)
	{
		case TIM1_BASE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
			TIM_IRQChannel=TIM1_UP_IRQChannel;
			break;
		
		case TIM2_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			TIM_IRQChannel=TIM2_IRQChannel;
			break;
		
		case TIM3_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			TIM_IRQChannel=TIM3_IRQChannel;
			break;
		
		case TIM4_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			TIM_IRQChannel=TIM4_IRQChannel;
			break;
		
		case TIM5_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
			TIM_IRQChannel=TIM5_IRQChannel;
			break;
		
		case TIM6_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
			TIM_IRQChannel=TIM6_IRQChannel;
			break;
		
		case TIM7_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
			TIM_IRQChannel=TIM7_IRQChannel;
			break;
		
		case TIM8_BASE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
			TIM_IRQChannel=TIM8_UP_IRQChannel;
			break;
		
		default:
			break;
		
	}
	
	TIMx_RCC_ENABLE(TIMx);													//����Ӧ��ʱ��ʱ��
	//1)**********������ر���	
	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler-1; 		// �趨��Ƶֵ
	TIM_TimeBaseStructure.TIM_Period = Period-1;        	//�趨�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //���ָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
	//	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	//????,???????????????????-??????????
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);		//��ʼ��
	//1)**********������ر���	
	TIM_ARRPreloadConfig(TIMx, ENABLE);
	TIM_Cmd(TIMx, ENABLE); 
	
	//1)**********������ر���	
	NVIC_InitStructure.NVIC_IRQChannel = TIM_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Clear TIMx update pending flag[���TIMx����ж�] */
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);

	/* Enable TIM2 Update interrupt [TIMx����ж�����]*/
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE); 

	/* TIM2 enable counter [ʹ��TIMx����]*/
	TIM_Cmd(TIMx, ENABLE);	//ʹ��TIMx����
}
/*******************************************************************************
*������		:TIMx_RCC_ENABLE
*��������	:����Ӧ��ʱ��ʱ��
*����			:TIMx--��ʱ����
						x����Ϊ1,2,3,4,5,6,7����8
*���			:��
*����ֵ		:��
*����			:TIMx_RCC_ENABLE(TIM1);
*******************************************************************************/
void TIMx_RCC_ENABLE(TIM_TypeDef* TIMx)	//����Ӧ��ʱ��ʱ��
{
	assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
	switch (*(u32*)&TIMx)
	{
		case TIM1_BASE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
			break;
		
		case TIM2_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
			break;
		
		case TIM3_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
			break;
		
		case TIM4_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
			break;
		
		case TIM5_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
			break;
		
		case TIM6_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
			break;
		
		case TIM7_BASE:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
			break;
		
		case TIM8_BASE:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
			break;
		
		default:
break;
		
	}
}
/*******************************************************************************
*������		: TIM_TIME_SET
*��������	:��ʱʱ���趨
*����			:TIMx--TIMx--��ʱ����
					 x����Ϊ1,2,3,4,5,6,7����8
					Prescaler---��Ƶϵ��,(ȡֵ0x0000~0xFFFF)������ƵTIM clock
					 Period	---����ֵ,(ȡֵ0x0000~0xFFFF),����TIM_Period+1�����ĺ�������
					TIM_ClockDivision----�趨ʱ��ָ�ֵ,(Ĭ��Ϊ0,���ⳡ��ʱTIM_ClockDivision������һ����ʱ)
					TIM_CounterMode---������ʽ
					TIM_RepetitionCounter---�ظ���������,(�ظ����ٴ������Ŵ���һ������ж�)
*���			:��
*����ֵ		:��
*����			:
*******************************************************************************/
void TIM_TIME_SET(TIM_TypeDef* TIMx,u16 Prescaler,u16 Period)	//��ʱʱ���趨
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;	//��ʱ���ṹ�嶨��

	TIM_TimeBaseStructure.TIM_Prescaler = Prescaler-1; 		// �趨��Ƶֵ
	TIM_TimeBaseStructure.TIM_Period = Period-1;        	//�趨�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  //���ָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
	//	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	//????,???????????????????-??????????
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);		//��ʼ��
//	TIM_PrescalerConfig(TIM2,Prescaler,TIM_PSCReloadMode_Update);
	TIM_ARRPreloadConfig(TIMx, ENABLE);
//	TIM_Cmd(TIM2, ENABLE);
//	TIM_SetCompare1(TIM2,4000);
}
/*******************************************************************************
*������		:TIM_Interrupt
*��������	:ADS1230�ܽų�ʼ��
*����			:��
*���			:��
*����ֵ		:��
*����			��
*******************************************************************************/
void TIM_Interrupt(TIM_TypeDef* TIMx,u16 Prescaler,u16 Period)
{
	NVIC_InitTypeDef	NVIC_InitStructure;
	u8 TIM_IRQChannel=0;
	assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
	
	TIM_Configuration(TIMx,Prescaler,Period);
	
	switch (*(u32*)&TIMx)
	{
		case TIM1_BASE:
			TIM_IRQChannel=TIM1_UP_IRQChannel;
			break;
		
		case TIM2_BASE:
			TIM_IRQChannel=TIM2_IRQChannel;
			break;
		
		case TIM3_BASE:
			TIM_IRQChannel=TIM3_IRQChannel;
			break;
		
		case TIM4_BASE:
			TIM_IRQChannel=TIM4_IRQChannel;
			break;
		
		case TIM5_BASE:
			TIM_IRQChannel=TIM5_IRQChannel;
			break;
		
		case TIM6_BASE:
			TIM_IRQChannel=TIM6_IRQChannel;
			break;
		
		case TIM7_BASE:
			TIM_IRQChannel=TIM7_IRQChannel;
			break;
		
		case TIM8_BASE:
			TIM_IRQChannel=TIM8_UP_IRQChannel;
			break;
		
		default:
			break;
		
	}
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Clear TIMx update pending flag[���TIMx����ж�] */
	TIM_ClearFlag(TIMx, TIM_FLAG_Update);

	/* Enable TIM2 Update interrupt [TIMx����ж�����]*/
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE); 

	/* TIM2 enable counter [ʹ��TIMx����]*/
	TIM_Cmd(TIMx, ENABLE);	//ʹ��TIMx����
		
}
/*******************************************************************************
*������		:TIM_Server
*��������	:ADS1230�ܽų�ʼ��
*����			:��
*���			:��
*����ֵ		:��
*����			��
*******************************************************************************/
void TIM_Server(void)
{
	WOW_Server();															//������
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);
	TIM_ClearFlag(TIM8, TIM_FLAG_Update);
}





