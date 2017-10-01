
#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x_gpio.h"

//typedef enum	_STEP_MOTO_STATUS
//{
//	MOTOR_STATUS_RUN,				//运行状态
//	MOTOR_STATUS_PAUSE,			//暂停状态
//	MOTOR_STATUS_STOP				//停止状态
//}STEP_MOTO_STATUS;				//==========步进电机状态

//typedef enum	_STEP_MOTO_CTLSTATUS
//{
//	MOTOR_WAIT_START,				//等待启动:比如说有原点传感器或者步长传感器时,需要电机转到无感应信号处表示电机已经运行
//	MOTOR_RUNNING,					//表示电机正在运行
//	MOTOR_WAIT_STOP					//电机运行后,需要等待停止信号
//}STEP_MOTO_CTLSTATUS;			//==========步进电机额外控制状态//为了不影响正常的执行,另外选配的状态参数

typedef enum	_STEP_MOTO_COMMAND
{
//	MOTO_COMMAND_IDLE,			//无命令
	MOTO_COMMAND_RUN,				//运行命令
	MOTO_COMMAND_PAUSE,			//暂停命令
	MOTO_COMMAND_STOP				//停止命令
}STEP_MOTO_COMMAND;				//==========步进电机命令

typedef struct _STEP_TIME_CONF
{
	u32	StepTimeOut;			//超时时间---单位ms
	u32	StepFreq;					//驱动频率---单位Hz
	u32	StartFreq;				//起始频率---单位Hz
	u32	BaseTime;					//时基---扫描周期,单位us----默认设置定时器每1us扫描一次
}STEP_TIME_CONF;				//==========需要输入的时间相关参数

typedef struct _STEP_TIME_RUN
{
	u32	StepTimeCount;		//运行计时---如果到了超时时间StepTimeOut,则强制停止
	u32	StepCycle;				//根据驱动频率计算出的正常运行时的翻转时间
	u32	StartCycle;				//根据StepFreq起始频率计算出的起始翻转时间
	u32	StartSteps;				//加速步数:在0.5秒内将步进电机驱动速度从StartCycle运行到StepCycle
	u32	RunCycle;					//起动时根据当前频率转换来的时间周期	
	u32	HalfCycle;				//根据周期计算出的半周期	
	u32	ToggleTime;				//根据HalfCycle翻转	
}STEP_TIME_RUN;					//==========根据输入的时间相关参数计算出来的相关运行参数值

typedef struct _STEP_MOTO_CONF
{
	//默认加速时间0.5S
		//____________PUL转速控制
	GPIO_TypeDef* 	PUL_PORT;				//GPIOX
	unsigned short 	PUL_Pin;				//GPIO_Pin_x
	
	//____________DIR方向控制
	GPIO_TypeDef* 	DIR_PORT;				//GPIOX
	unsigned short 	DIR_Pin;				//GPIO_Pin_x
	
	//____________EN使能
	GPIO_TypeDef* 	EN_PORT;				//GPIOX
	unsigned short 	EN_Pin;					//GPIO_Pin_x
	
	//____________Brake刹车控制
	GPIO_TypeDef* 	Brake_PORT;			//GPIOX
	unsigned short 	Brake_Pin;			//GPIO_Pin_x
	
	STEP_TIME_CONF			TIME_CONF;	//需要配置的参数		
	STEP_TIME_RUN				TIME_RUN;		//运行参数---不需要配置
	
	STEP_MOTO_COMMAND		MOTO_COMMAND;				//电机执行命令

}STEP_MOTO_CONF;



void StepMotoConf(STEP_MOTO_CONF *STEP_MOTOx);		//
void StepMotoContrl(STEP_MOTO_CONF *STEP_MOTOx);	//


void StepMotoRun(STEP_MOTO_CONF *STEP_MOTOx);			//
void StepMotoStop(STEP_MOTO_CONF *STEP_MOTOx);		//
void StepMotoSever(STEP_MOTO_CONF *STEP_MOTOx);		//

#endif


