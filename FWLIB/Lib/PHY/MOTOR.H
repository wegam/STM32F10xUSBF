
#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x_gpio.h"

//typedef enum	_STEP_MOTO_STATUS
//{
//	MOTOR_STATUS_RUN,				//����״̬
//	MOTOR_STATUS_PAUSE,			//��ͣ״̬
//	MOTOR_STATUS_STOP				//ֹͣ״̬
//}STEP_MOTO_STATUS;				//==========�������״̬

//typedef enum	_STEP_MOTO_CTLSTATUS
//{
//	MOTOR_WAIT_START,				//�ȴ�����:����˵��ԭ�㴫�������߲���������ʱ,��Ҫ���ת���޸�Ӧ�źŴ���ʾ����Ѿ�����
//	MOTOR_RUNNING,					//��ʾ�����������
//	MOTOR_WAIT_STOP					//������к�,��Ҫ�ȴ�ֹͣ�ź�
//}STEP_MOTO_CTLSTATUS;			//==========��������������״̬//Ϊ�˲�Ӱ��������ִ��,����ѡ���״̬����

typedef enum	_STEP_MOTO_COMMAND
{
//	MOTO_COMMAND_IDLE,			//������
	MOTO_COMMAND_RUN,				//��������
	MOTO_COMMAND_PAUSE,			//��ͣ����
	MOTO_COMMAND_STOP				//ֹͣ����
}STEP_MOTO_COMMAND;				//==========�����������

typedef struct _STEP_TIME_CONF
{
	u32	StepTimeOut;			//��ʱʱ��---��λms
	u32	StepFreq;					//����Ƶ��---��λHz
	u32	StartFreq;				//��ʼƵ��---��λHz
	u32	BaseTime;					//ʱ��---ɨ������,��λus----Ĭ�����ö�ʱ��ÿ1usɨ��һ��
}STEP_TIME_CONF;				//==========��Ҫ�����ʱ����ز���

typedef struct _STEP_TIME_RUN
{
	u32	StepTimeCount;		//���м�ʱ---������˳�ʱʱ��StepTimeOut,��ǿ��ֹͣ
	u32	StepCycle;				//��������Ƶ�ʼ��������������ʱ�ķ�תʱ��
	u32	StartCycle;				//����StepFreq��ʼƵ�ʼ��������ʼ��תʱ��
	u32	StartSteps;				//���ٲ���:��0.5���ڽ�������������ٶȴ�StartCycle���е�StepCycle
	u32	RunCycle;					//��ʱ���ݵ�ǰƵ��ת������ʱ������	
	u32	HalfCycle;				//�������ڼ�����İ�����	
	u32	ToggleTime;				//����HalfCycle��ת	
}STEP_TIME_RUN;					//==========���������ʱ����ز������������������в���ֵ

typedef struct _STEP_MOTO_CONF
{
	//Ĭ�ϼ���ʱ��0.5S
		//____________PULת�ٿ���
	GPIO_TypeDef* 	PUL_PORT;				//GPIOX
	unsigned short 	PUL_Pin;				//GPIO_Pin_x
	
	//____________DIR�������
	GPIO_TypeDef* 	DIR_PORT;				//GPIOX
	unsigned short 	DIR_Pin;				//GPIO_Pin_x
	
	//____________ENʹ��
	GPIO_TypeDef* 	EN_PORT;				//GPIOX
	unsigned short 	EN_Pin;					//GPIO_Pin_x
	
	//____________Brakeɲ������
	GPIO_TypeDef* 	Brake_PORT;			//GPIOX
	unsigned short 	Brake_Pin;			//GPIO_Pin_x
	
	STEP_TIME_CONF			TIME_CONF;	//��Ҫ���õĲ���		
	STEP_TIME_RUN				TIME_RUN;		//���в���---����Ҫ����
	
	STEP_MOTO_COMMAND		MOTO_COMMAND;				//���ִ������

}STEP_MOTO_CONF;



void StepMotoConf(STEP_MOTO_CONF *STEP_MOTOx);		//
void StepMotoContrl(STEP_MOTO_CONF *STEP_MOTOx);	//


void StepMotoRun(STEP_MOTO_CONF *STEP_MOTOx);			//
void StepMotoStop(STEP_MOTO_CONF *STEP_MOTOx);		//
void StepMotoSever(STEP_MOTO_CONF *STEP_MOTOx);		//

#endif

