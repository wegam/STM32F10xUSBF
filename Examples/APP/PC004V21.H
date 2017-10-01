#ifndef __PC004V21_H
#define __PC004V21_H

#include "stm32f10x_type.h"

void PC004V21_Configuration(void);
void PC004V21_Server(void);
void PC004V21_PinSet(void);


void PC004V21_KEYData(void);		//������ҩ��������
	


u8 PC004V10_485_TR(void);				//485�շ�
void PC004V10_CAN_RX(void);			//��Ҫ��������ܰ��ַ��
void PC004V10_CAN_TX(void);			//CAN�������ݣ���ַ���������ͣ�����
void PC004V10_CAN_COMMAD(void);	//CAN�������������ַ���������ͣ�����--ʱ��ͬ��

u8 PC004V21_GetSwitchID(void);				//��ȡ��ǰ�豸ID//�����

void Self_Test(u16 time);			//���뿪��Ϊ0ʱ��������ʾ����

void PC004V10_delay(u16 time);


#endif
