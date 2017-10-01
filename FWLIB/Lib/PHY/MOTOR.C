/*********************************************
*步进电机驱动
*结构体参数配置形式
*驱动模式---计时
*特殊功能:超时,加速
*最高优先级:停止--任何情况下,有停止命令,优先执行停止
*其它:刹车
**********************************************/

#include "MOTOR.H"

#include "STM32_GPIO.H"
//-----------------------------步进电机
/****************************************************************
*函数名:
*描述:
*
******************************************************************/
void StepMotoConf(STEP_MOTO_CONF *STEP_MOTOx)
{
	//____________PUL转速控制
	GPIO_Configuration_OPP50	(STEP_MOTOx->PUL_PORT,	STEP_MOTOx->PUL_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	//____________DIR方向控制
	GPIO_Configuration_OPP50	(STEP_MOTOx->DIR_PORT,	STEP_MOTOx->DIR_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	//____________EN使能
	GPIO_Configuration_OPP50	(STEP_MOTOx->EN_PORT,		STEP_MOTOx->EN_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	//____________Brake刹车控制
	GPIO_Configuration_OPP50	(STEP_MOTOx->Brake_PORT,	STEP_MOTOx->Brake_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	
	//------
	STEP_MOTOx->TIME_RUN.StepCycle=1000000/(STEP_MOTOx->TIME_CONF.BaseTime)/(STEP_MOTOx->TIME_CONF.StepFreq);				//正常运行周期=1S/时基/频率
	if(STEP_MOTOx->TIME_RUN.StepCycle<2)
	{
		STEP_MOTOx->TIME_RUN.StepCycle=2;
	}
	else if((STEP_MOTOx->TIME_RUN.StepCycle%2)!=0)
	{
		STEP_MOTOx->TIME_RUN.StepCycle-=1;
	}
	STEP_MOTOx->TIME_RUN.StartCycle=1000000/(STEP_MOTOx->TIME_CONF.BaseTime)/(STEP_MOTOx->TIME_CONF.StartFreq);			//起始运行周期=1S/时基/起始频率
	STEP_MOTOx->TIME_RUN.StartSteps=(STEP_MOTOx->TIME_RUN.StartCycle-STEP_MOTOx->TIME_RUN.StepCycle)/500+0.5;				//分500步加速
	STEP_MOTOx->TIME_RUN.RunCycle=STEP_MOTOx->TIME_RUN.StartCycle;		//恢复起始时间
	STEP_MOTOx->TIME_RUN.HalfCycle=STEP_MOTOx->TIME_RUN.RunCycle/2;		//半周期时间
	if(STEP_MOTOx->TIME_RUN.StartSteps<2)
	{
		STEP_MOTOx->TIME_RUN.StartSteps=2;
	}
	
	STEP_MOTOx->MOTO_COMMAND		=	MOTO_COMMAND_STOP;		//
}
/****************************************************************
*函数名:
*描述:
*
******************************************************************/
void StepMotoContrl(STEP_MOTO_CONF *STEP_MOTOx)
{

}
/****************************************************************
*函数名:
*描述:
*
******************************************************************/
void StepMotoRun(STEP_MOTO_CONF *STEP_MOTOx)
{
	STEP_MOTOx->TIME_RUN.StepTimeCount++;
	if(STEP_MOTOx->TIME_RUN.StepTimeCount	<=	2)								//开启DIR时间
	{
		GPIO_SetBits(STEP_MOTOx->DIR_PORT,STEP_MOTOx->DIR_Pin);
	}
	if(STEP_MOTOx->TIME_RUN.StepTimeCount	<	STEP_MOTOx->TIME_CONF.StepTimeOut)				//未超时
	{
		//1------------加速阶段:当前运行周期比预定周期长表示还在加速阶段
		//-------------加速时间最高为500ms,每1ms增加一个频率段,直到加速到预定速度,相关参数:StartSteps
		//-------------频率增加后检查是否加速完成,可以节省加速完成后的时间
		if(STEP_MOTOx->TIME_RUN.RunCycle	>	STEP_MOTOx->TIME_RUN.StepCycle)
		{
			if((STEP_MOTOx->TIME_RUN.StepTimeCount*STEP_MOTOx->TIME_CONF.BaseTime)%1000==0)		//1ms加速计时
			{
				STEP_MOTOx->TIME_RUN.RunCycle		-=	2;
				STEP_MOTOx->TIME_RUN.HalfCycle	=		(STEP_MOTOx->TIME_RUN.RunCycle/2);		//半周期时间
			}
			//检查是否加速完成:如果加速完成,后期正常运行不再进入此步骤以达到节省运算时间的目的
			if(STEP_MOTOx->TIME_RUN.RunCycle	<=	STEP_MOTOx->TIME_RUN.StepCycle)			//加速过程完成
			{
				STEP_MOTOx->TIME_RUN.RunCycle		=		STEP_MOTOx->TIME_RUN.StepCycle;				//按正常运行周期运行
				STEP_MOTOx->TIME_RUN.HalfCycle	=		(STEP_MOTOx->TIME_RUN.RunCycle/2);		//半周期时间
			}
		}
		//1============================PWM脉冲输出控制
		//1.1-----------计算输出翻转时间
		if(STEP_MOTOx->TIME_RUN.ToggleTime++	>=	STEP_MOTOx->TIME_RUN.RunCycle-1)	//翻转计时:前半周期->后半周期状态切换,如果到达一个预定周期,清零重新计时
		{
			STEP_MOTOx->TIME_RUN.HalfCycle	=	(STEP_MOTOx->TIME_RUN.RunCycle/2);			//半周期时间
			STEP_MOTOx->TIME_RUN.ToggleTime	=	0;			//清零
		}
		//1.2-----------输出翻转控制(模拟PWM)		
		if(STEP_MOTOx->TIME_RUN.ToggleTime	<	STEP_MOTOx->TIME_RUN.HalfCycle)			//翻转时的前半周期输出电平
		{
			GPIO_SetBits(STEP_MOTOx->PUL_PORT,STEP_MOTOx->PUL_Pin);
		}
		else					//翻转时的后半周期输出电平
		{
			GPIO_ResetBits(STEP_MOTOx->PUL_PORT,STEP_MOTOx->PUL_Pin);
		}
//		STEP_MOTOx->MOTO_STATUS	=	MOTOR_STATUS_RUN;										//超时标志
	}
	else		//超时
	{
		StepMotoStop(STEP_MOTOx);																		//停止电机
	}
}
/****************************************************************
*函数名:
*描述:
*
******************************************************************/
void StepMotoStop(STEP_MOTO_CONF *STEP_MOTOx)
{
	STEP_MOTOx->TIME_RUN.StepTimeCount=0;														//运行计时清零
	STEP_MOTOx->TIME_RUN.ToggleTime=0;															//翻转时间清零
	STEP_MOTOx->TIME_RUN.RunCycle=STEP_MOTOx->TIME_RUN.StartCycle;	//恢复起始时间
	
	GPIO_ResetBits(STEP_MOTOx->DIR_PORT,STEP_MOTOx->DIR_Pin);
	GPIO_ResetBits(STEP_MOTOx->PUL_PORT,STEP_MOTOx->PUL_Pin);				//停止
	
//	STEP_MOTOx->MOTO_STATUS=MOTOR_STATUS_STOP;											//状态为停止状态
//	STEP_MOTOx->MOTO_CTLSTATUS=MOTOR_WAIT_START;
}
/****************************************************************
*函数名:
*描述:
*
******************************************************************/
void StepMotoPause(STEP_MOTO_CONF *STEP_MOTOx)
{
	STEP_MOTOx->TIME_RUN.StepTimeCount=0;														//运行计时清零
	STEP_MOTOx->TIME_RUN.ToggleTime=0;															//翻转时间清零
	STEP_MOTOx->TIME_RUN.RunCycle=STEP_MOTOx->TIME_RUN.StartCycle;	//恢复起始时间
	
	GPIO_ResetBits(STEP_MOTOx->DIR_PORT,STEP_MOTOx->DIR_Pin);
	GPIO_ResetBits(STEP_MOTOx->PUL_PORT,STEP_MOTOx->PUL_Pin);				//停止
	
//	STEP_MOTOx->MOTO_STATUS=MOTOR_STATUS_STOP;											//状态为停止状态
//	STEP_MOTOx->MOTO_CTLSTATUS=MOTOR_WAIT_START;
}
/****************************************************************
*函数名:
*描述:
*
******************************************************************/
void StepMotoSever(STEP_MOTO_CONF *STEP_MOTOx)
{
	if(STEP_MOTOx->MOTO_COMMAND==MOTO_COMMAND_RUN)
	{
		StepMotoRun(STEP_MOTOx);			//		
	}
	else if(STEP_MOTOx->MOTO_COMMAND==MOTO_COMMAND_STOP)
	{
		StepMotoStop(STEP_MOTOx);			//
		
	}
}

////-----------------------交流电机
///****************************************************************
//*函数名:
//*描述:
//*
//******************************************************************/
//void ACMotoConf(STEP_MOTO_CONF *STEP_MOTOx)
//{
//	//____________PUL转速控制
//	GPIO_Configuration_OPP50	(STEP_MOTOx->PUL_PORT,	STEP_MOTOx->PUL_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//	//____________DIR方向控制
//	GPIO_Configuration_OPP50	(STEP_MOTOx->DIR_PORT,	STEP_MOTOx->DIR_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//	//____________EN使能
//	GPIO_Configuration_OPP50	(STEP_MOTOx->EN_PORT,		STEP_MOTOx->EN_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//	//____________Brake刹车控制
//	GPIO_Configuration_OPP50	(STEP_MOTOx->Brake_PORT,	STEP_MOTOx->Brake_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//	
//	//------
//	STEP_MOTOx->TIME_RUN.StepCycle=1000000/(STEP_MOTOx->TIME_CONF.BaseTime)/(STEP_MOTOx->TIME_CONF.StepFreq);				//正常运行周期=1S/时基/频率
//	if(STEP_MOTOx->TIME_RUN.StepCycle<2)
//	{
//		STEP_MOTOx->TIME_RUN.StepCycle=2;
//	}
//	else if((STEP_MOTOx->TIME_RUN.StepCycle%2)!=0)
//	{
//		STEP_MOTOx->TIME_RUN.StepCycle-=1;
//	}
//	STEP_MOTOx->TIME_RUN.StartCycle=1000000/(STEP_MOTOx->TIME_CONF.BaseTime)/(STEP_MOTOx->TIME_CONF.StartFreq);			//起始运行周期=1S/时基/起始频率
//	STEP_MOTOx->TIME_RUN.StartSteps=(STEP_MOTOx->TIME_RUN.StartCycle-STEP_MOTOx->TIME_RUN.StepCycle)/500+0.5;				//分500步加速
//	STEP_MOTOx->TIME_RUN.RunCycle=STEP_MOTOx->TIME_RUN.StartCycle;		//恢复起始时间
//	STEP_MOTOx->TIME_RUN.HalfCycle=STEP_MOTOx->TIME_RUN.RunCycle/2;		//半周期时间
//	if(STEP_MOTOx->TIME_RUN.StartSteps<2)
//	{
//		STEP_MOTOx->TIME_RUN.StartSteps=2;
//	}
//	
//	STEP_MOTOx->MOTO_COMMAND=MOTO_COMMAND_IDLE;
//	STEP_MOTOx->MOTO_STATUS=MOTOR_STATUS_STOP;
//	STEP_MOTOx->MOTO_CTLSTATUS=MOTOR_WAIT_START;
//}
///****************************************************************
//*函数名:
//*描述:
//*
//******************************************************************/
//void ACMotoRun(STEP_MOTO_CONF *STEP_MOTOx)
//{
//	STEP_MOTOx->TIME_RUN.StepTimeCount++;
//	if(STEP_MOTOx->TIME_RUN.StepTimeCount<=2)
//	{
//		GPIO_SetBits(STEP_MOTOx->DIR_PORT,STEP_MOTOx->DIR_Pin);
//	}
//	if(STEP_MOTOx->TIME_RUN.StepTimeCount<STEP_MOTOx->TIME_CONF.StepTimeOut)				//未超时
//	{
//		//1------------加速阶段:当前运行周期比预定周期长表示还在加速阶段
//		//-------------加速时间最高为500ms,每1ms增加一个频率段,直到加速到预定速度,相关参数:StartSteps
//		//-------------频率增加后检查是否加速完成,可以节省加速完成后的时间
//		if(STEP_MOTOx->TIME_RUN.RunCycle>STEP_MOTOx->TIME_RUN.StepCycle)
//		{
//			if((STEP_MOTOx->TIME_RUN.StepTimeCount*STEP_MOTOx->TIME_CONF.BaseTime)%1000==0)		//1ms加速计时
//			{
//				STEP_MOTOx->TIME_RUN.RunCycle-=2;
//				STEP_MOTOx->TIME_RUN.HalfCycle=(STEP_MOTOx->TIME_RUN.RunCycle/2);		//半周期时间
////				STEP_MOTOx->TIME_RUN.ToggleTime=0;																//翻转时间清零
//			}
//			//检查是否加速完成:如果加速完成,后期正常运行不再进入此步骤以达到节省运算时间的目的
//			if(STEP_MOTOx->TIME_RUN.RunCycle<=STEP_MOTOx->TIME_RUN.StepCycle)		//加速过程完成
//			{
//				STEP_MOTOx->TIME_RUN.RunCycle=STEP_MOTOx->TIME_RUN.StepCycle;			//按正常运行周期运行
//				STEP_MOTOx->TIME_RUN.HalfCycle=(STEP_MOTOx->TIME_RUN.RunCycle/2);		//半周期时间
////				STEP_MOTOx->TIME_RUN.ToggleTime=0;																//翻转时间清零
//			}
//		}
//		//1-------------PWM脉冲输出控制
//		//1.1-----------计算输出翻转时间
//		if(STEP_MOTOx->TIME_RUN.ToggleTime++>=STEP_MOTOx->TIME_RUN.RunCycle-1)		//翻转计时:前半周期->后半周期状态切换,如果到达一个预定周期,清零重新计时
//		{
//			STEP_MOTOx->TIME_RUN.HalfCycle=(STEP_MOTOx->TIME_RUN.RunCycle/2);		//半周期时间
//			STEP_MOTOx->TIME_RUN.ToggleTime=0;			//清零
//		}
//		//1.2-----------输出翻转控制(模拟PWM)		
//		if(STEP_MOTOx->TIME_RUN.ToggleTime<STEP_MOTOx->TIME_RUN.HalfCycle)		//翻转时的前半周期输出电平
//		{
//			GPIO_SetBits(STEP_MOTOx->PUL_PORT,STEP_MOTOx->PUL_Pin);
//		}
//		else			//翻转时的后半周期输出电平
//		{
//			GPIO_ResetBits(STEP_MOTOx->PUL_PORT,STEP_MOTOx->PUL_Pin);
//		}
//		STEP_MOTOx->MOTO_STATUS=MOTOR_STATUS_RUN;								//超时标志
//	}
//	else		//超时
//	{
//		StepMotoStop(STEP_MOTOx);																		//停止电机
//	}
//}
///****************************************************************
//*函数名:
//*描述:
//*
//******************************************************************/
//void ACMotoStop(STEP_MOTO_CONF *STEP_MOTOx)
//{
//	STEP_MOTOx->TIME_RUN.StepTimeCount=0;														//运行计时清零
//	STEP_MOTOx->TIME_RUN.ToggleTime=0;															//翻转时间清零
//	STEP_MOTOx->TIME_RUN.RunCycle=STEP_MOTOx->TIME_RUN.StartCycle;	//恢复起始时间
//	
//	GPIO_ResetBits(STEP_MOTOx->DIR_PORT,STEP_MOTOx->DIR_Pin);
//	GPIO_ResetBits(STEP_MOTOx->PUL_PORT,STEP_MOTOx->PUL_Pin);							//停止
//	
//	STEP_MOTOx->MOTO_STATUS=MOTOR_STATUS_STOP;			//状态为停止状态	
//}
///****************************************************************
//*函数名:
//*描述:
//*
//******************************************************************/
//void ACMotoSever(STEP_MOTO_CONF *STEP_MOTOx)
//{
//	if(STEP_MOTOx->MOTO_COMMAND==MOTO_COMMAND_RUN)
//	{
//		StepMotoRun(STEP_MOTOx);			//		
//	}
//	else if(STEP_MOTOx->MOTO_COMMAND==MOTO_COMMAND_STOP)
//	{
//		StepMotoStop(STEP_MOTOx);			//
//		
//	}
//}
