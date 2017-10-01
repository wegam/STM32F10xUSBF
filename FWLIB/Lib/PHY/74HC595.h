
#ifndef __74HC595_H
#define __74HC595_H
//#include <stdint.h>
#include "stm32f10x_gpio.h"



typedef struct	_HC595_Pin
{
//	//SDA-PIN		//14�ţ�������������
	GPIO_TypeDef* 	HC595_SDA_PORT;				//GPIOX
	unsigned short 	HC595_SDA_Pin;				//GPIO_Pin_x
	
	//SCK-PIN		//11�ţ���������ʱ����,��������λ
	GPIO_TypeDef* 	HC595_SCK_PORT;				//GPIOX
	unsigned short 	HC595_SCK_Pin;				//GPIO_Pin_x
	
	//RCK-PIN		//12�ţ�����洢������ʱ���ߣ�������ʱ��λ�Ĵ��������ݽ������ݴ洢�Ĵ������½���ʱ�洢�Ĵ������ݲ���
	GPIO_TypeDef* 	HC595_RCK_PORT;				//GPIOX
	unsigned short 	HC595_RCK_Pin;				//GPIO_Pin_x
	
	//CLR-PIN		//10�ţ�����λ���͵�ƽ�����͵�ƽʱ����λ�Ĵ������������㡣ͨ��������Vcc��
	GPIO_TypeDef* 	HC595_CLR_PORT;				//GPIOX
	unsigned short 	HC595_CLR_Pin;				//GPIO_Pin_x
	
	//OE-PIN		//13�ţ������Ч���͵�ƽ�����ߵ�ƽʱ��ֹ���(����̬)�������Ƭ�������Ų����ţ�
	//																		��һ�����ſ����������Է���ز�����˸��Ϩ��Ч������ͨ�����ݶ���λ����Ҫʡʱʡ����
	GPIO_TypeDef* 	HC595_OE_PORT;				//GPIOX
	unsigned short 	HC595_OE_Pin;				//GPIO_Pin_x

}HC595_Pindef;





void HC595_PinConf(HC595_Pindef *Pinfo);
void HC595_WriteNumber(HC595_Pindef *Pinfo,u8 num);
void HC595_WriteNumberN(HC595_Pindef *Pinfo,u8 num);		//�����ź�д��











#endif


