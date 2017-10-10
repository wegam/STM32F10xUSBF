/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : PC001V21.c
* Author             : WOW
* Version            : V2.0.1
* Date               : 06/26/2017
* Description        : PC001V21层控制板.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#ifdef STM32F103C8T6			//G,H版本智能药盒辅控板

#include "STM32F103C8T6.H"


#include "A3987.H"

#include "STM32F10x_BitBand.H"
#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_WDG.H"
#include "STM32_PWM.H"
#include "STM32_USART.H"

#include "string.h"				//串和内存操作函数头文件
#include "stm32f10x_dma.h"


u16	SYSTime=0;

u16 Lock_Toggle_CNT=0;

void Lock_Toggle(void);			//双向电子锁控制


/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void STM32F103C8T6_Configuration(void)
{
	SYS_Configuration();					//系统配置---打开系统时钟 STM32_SYS.H
	
	GPIO_DeInitAll();							//将所有的GPIO关闭----V20170605
	
	STM32F103C8T6_PinSet();
	
	SysTick_Configuration(1000);	//系统嘀嗒时钟配置72MHz,单位为uS
	
	IWDG_Configuration(1000);			//独立看门狗配置---参数单位ms	
	
	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);						//PWM设定-20161127版本
	
//	PD003VG_USART_Conf();
	
//	RS485_Bus_TXEN;		//rs485总线接收使能
//	USART_DMASend(USART3,(u32*)txBuffer_PD,BufferSize);			//自定义printf串口DMA发送程序

}
/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void STM32F103C8T6_Server(void)
{	
	u16 len	=	0;
	IWDG_Feed();								//独立看门狗喂狗
	SYSTime++;
	if(SYSTime>=500)
		SYSTime	=	0;
	if(SYSTime	==	0)
	{
		//激光输出
		GPIO_Toggle	(GPIOA,	GPIO_Pin_7);		//将GPIO相应管脚输出翻转----V20170605
		
		//运行灯
		GPIO_Toggle	(GPIOC,	GPIO_Pin_13);		//将GPIO相应管脚输出翻转----V20170605
	}

}

/*******************************************************************************
* 函数名		:	
* 功能描述	:	 
* 输入		:	
* 输出		:
* 返回 		:
*******************************************************************************/
void STM32F103C8T6_PinSet(void)
{	
	//传感器输入
	GPIO_Configuration_INF(GPIOB,	GPIO_Pin_12);			//将GPIO相应管脚配置为浮空输入模式----V20170605
	GPIO_Configuration_INF(GPIOB,	GPIO_Pin_13);			//将GPIO相应管脚配置为浮空输入模式----V20170605---中间传感器
	GPIO_Configuration_INF(GPIOB,	GPIO_Pin_14);			//将GPIO相应管脚配置为浮空输入模式----V20170605---最外边传感器

	//激光输出
	GPIO_Configuration_OPP50	(GPIOA,	GPIO_Pin_7);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	
	//运行灯
	GPIO_Configuration_OPP50	(GPIOC,	GPIO_Pin_13);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	
	
}

/*******************************************************************************
* 函数名			:	function
* 功能描述		:	函数功能说明 
* 输入			: void
* 返回值			: void
*******************************************************************************/
void Lock_Toggle(void)
{

}
#endif
