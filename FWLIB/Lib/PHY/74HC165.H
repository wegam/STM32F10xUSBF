
#ifndef __74HC165_H
#define __74HC165_H

#include "stm32f10x_gpio.h"

//HC165_CONF	HC165x_TEST;			//定义结构体变量



typedef struct _HC165_DATA
{
	u8	NumOfHC165;		//串接的74HC165芯片个数,一个8位----此处设定最多4个,就是32位
	struct
	{
		unsigned long D0:1;
		unsigned long D1:1;
		unsigned long D2:1;
		unsigned long D3:1;
		unsigned long D4:1;
		unsigned long D5:1;
		unsigned long D6:1;
		unsigned long D7:1;
		unsigned long D8:1;
		unsigned long D9:1;
		unsigned long D10:1;
		unsigned long D11:1;
		unsigned long D12:1;
		unsigned long D13:1;
		unsigned long D14:1;
		unsigned long D15:1;
		unsigned long D16:1;
		unsigned long D17:1;
		unsigned long D18:1;
		unsigned long D19:1;
		unsigned long D20:1;
		unsigned long D21:1;
		unsigned long D22:1;
		unsigned long D23:1;
		unsigned long D24:1;
		unsigned long D25:1;
		unsigned long D26:1;
		unsigned long D27:1;
		unsigned long D28:1;
		unsigned long D29:1;
		unsigned long D30:1;
		unsigned long D31:1;
	}Data32;
	
}HC165_DATA;			//根据输入的时间相关参数计算出来的相关运行参数值

typedef struct _HC165_CONF
{

	//____________异步并行加入端(低电平有效)
	GPIO_TypeDef* 	PL_PORT;				//GPIOX
	unsigned short 	PL_Pin;					//GPIO_Pin_x
	
	//____________时钟输入端(低到高边沿触发--上升沿)
	GPIO_TypeDef* 	CP_PORT;				//GPIOX
	unsigned short 	CP_Pin;					//GPIO_Pin_x
	
	//____________时钟使能端(低电平有效)
	GPIO_TypeDef* 	CE_PORT;				//GPIOX
	unsigned short 	CE_Pin;					//GPIO_Pin_x
	
	//____________串行数据输出端(读串行数据端口)
	GPIO_TypeDef* 	Q7_PORT;				//GPIOX
	unsigned short 	Q7_Pin;					//GPIO_Pin_x
	
	HC165_DATA	HC165_Data;
}HC165_CONF;




void HC165Conf(HC165_CONF *HC165x);		//
unsigned long HC165Read(HC165_CONF *HC165x);		//
void HC165_TEST(HC165_CONF *HC165x);

#endif


