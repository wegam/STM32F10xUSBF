/*********************************************
*�����������
*�ṹ�����������ʽ
*����ģʽ---��ʱ
*���⹦��:��ʱ,����
*������ȼ�:ֹͣ--�κ������,��ֹͣ����,����ִ��ֹͣ
*����:ɲ��
**********************************************/

#include "MOTOR.H"

#include "STM32_GPIO.H"
//-----------------------------�������
/****************************************************************
*������:
*����:
*
******************************************************************/
void StepMotoConf(STEP_MOTO_CONF *STEP_MOTOx)
{
	//____________PULת�ٿ���
	GPIO_Configuration_OPP50	(STEP_MOTOx->PUL_PORT,	STEP_MOTOx->PUL_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	//____________DIR�������
	GPIO_Configuration_OPP50	(STEP_MOTOx->DIR_PORT,	STEP_MOTOx->DIR_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	//____________ENʹ��
	GPIO_Configuration_OPP50	(STEP_MOTOx->EN_PORT,		STEP_MOTOx->EN_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	//____________Brakeɲ������
	GPIO_Configuration_OPP50	(STEP_MOTOx->Brake_PORT,	STEP_MOTOx->Brake_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	
	//------
	STEP_MOTOx->TIME_RUN.StepCycle=1000000/(STEP_MOTOx->TIME_CONF.BaseTime)/(STEP_MOTOx->TIME_CONF.StepFreq);				//������������=1S/ʱ��/Ƶ��
	if(STEP_MOTOx->TIME_RUN.StepCycle<2)
	{
		STEP_MOTOx->TIME_RUN.StepCycle=2;
	}
	else if((STEP_MOTOx->TIME_RUN.StepCycle%2)!=0)
	{
		STEP_MOTOx->TIME_RUN.StepCycle-=1;
	}
	STEP_MOTOx->TIME_RUN.StartCycle=1000000/(STEP_MOTOx->TIME_CONF.BaseTime)/(STEP_MOTOx->TIME_CONF.StartFreq);			//��ʼ��������=1S/ʱ��/��ʼƵ��
	STEP_MOTOx->TIME_RUN.StartSteps=(STEP_MOTOx->TIME_RUN.StartCycle-STEP_MOTOx->TIME_RUN.StepCycle)/500+0.5;				//��500������
	STEP_MOTOx->TIME_RUN.RunCycle=STEP_MOTOx->TIME_RUN.StartCycle;		//�ָ���ʼʱ��
	STEP_MOTOx->TIME_RUN.HalfCycle=STEP_MOTOx->TIME_RUN.RunCycle/2;		//������ʱ��
	if(STEP_MOTOx->TIME_RUN.StartSteps<2)
	{
		STEP_MOTOx->TIME_RUN.StartSteps=2;
	}
	
	STEP_MOTOx->MOTO_COMMAND		=	MOTO_COMMAND_STOP;		//
}
/****************************************************************
*������:
*����:
*
******************************************************************/
void StepMotoContrl(STEP_MOTO_CONF *STEP_MOTOx)
{

}
/****************************************************************
*������:
*����:
*
******************************************************************/
void StepMotoRun(STEP_MOTO_CONF *STEP_MOTOx)
{
	STEP_MOTOx->TIME_RUN.StepTimeCount++;
	if(STEP_MOTOx->TIME_RUN.StepTimeCount	<=	2)								//����DIRʱ��
	{
		GPIO_SetBits(STEP_MOTOx->DIR_PORT,STEP_MOTOx->DIR_Pin);
	}
	if(STEP_MOTOx->TIME_RUN.StepTimeCount	<	STEP_MOTOx->TIME_CONF.StepTimeOut)				//δ��ʱ
	{
		//1------------���ٽ׶�:��ǰ�������ڱ�Ԥ�����ڳ���ʾ���ڼ��ٽ׶�
		//-------------����ʱ�����Ϊ500ms,ÿ1ms����һ��Ƶ�ʶ�,ֱ�����ٵ�Ԥ���ٶ�,��ز���:StartSteps
		//-------------Ƶ�����Ӻ����Ƿ�������,���Խ�ʡ������ɺ��ʱ��
		if(STEP_MOTOx->TIME_RUN.RunCycle	>	STEP_MOTOx->TIME_RUN.StepCycle)
		{
			if((STEP_MOTOx->TIME_RUN.StepTimeCount*STEP_MOTOx->TIME_CONF.BaseTime)%1000==0)		//1ms���ټ�ʱ
			{
				STEP_MOTOx->TIME_RUN.RunCycle		-=	2;
				STEP_MOTOx->TIME_RUN.HalfCycle	=		(STEP_MOTOx->TIME_RUN.RunCycle/2);		//������ʱ��
			}
			//����Ƿ�������:����������,�����������в��ٽ���˲����Դﵽ��ʡ����ʱ���Ŀ��
			if(STEP_MOTOx->TIME_RUN.RunCycle	<=	STEP_MOTOx->TIME_RUN.StepCycle)			//���ٹ������
			{
				STEP_MOTOx->TIME_RUN.RunCycle		=		STEP_MOTOx->TIME_RUN.StepCycle;				//������������������
				STEP_MOTOx->TIME_RUN.HalfCycle	=		(STEP_MOTOx->TIME_RUN.RunCycle/2);		//������ʱ��
			}
		}
		//1============================PWM�����������
		//1.1-----------���������תʱ��
		if(STEP_MOTOx->TIME_RUN.ToggleTime++	>=	STEP_MOTOx->TIME_RUN.RunCycle-1)	//��ת��ʱ:ǰ������->�������״̬�л�,�������һ��Ԥ������,�������¼�ʱ
		{
			STEP_MOTOx->TIME_RUN.HalfCycle	=	(STEP_MOTOx->TIME_RUN.RunCycle/2);			//������ʱ��
			STEP_MOTOx->TIME_RUN.ToggleTime	=	0;			//����
		}
		//1.2-----------�����ת����(ģ��PWM)		
		if(STEP_MOTOx->TIME_RUN.ToggleTime	<	STEP_MOTOx->TIME_RUN.HalfCycle)			//��תʱ��ǰ�����������ƽ
		{
			GPIO_SetBits(STEP_MOTOx->PUL_PORT,STEP_MOTOx->PUL_Pin);
		}
		else					//��תʱ�ĺ�����������ƽ
		{
			GPIO_ResetBits(STEP_MOTOx->PUL_PORT,STEP_MOTOx->PUL_Pin);
		}
//		STEP_MOTOx->MOTO_STATUS	=	MOTOR_STATUS_RUN;										//��ʱ��־
	}
	else		//��ʱ
	{
		StepMotoStop(STEP_MOTOx);																		//ֹͣ���
	}
}
/****************************************************************
*������:
*����:
*
******************************************************************/
void StepMotoStop(STEP_MOTO_CONF *STEP_MOTOx)
{
	STEP_MOTOx->TIME_RUN.StepTimeCount=0;														//���м�ʱ����
	STEP_MOTOx->TIME_RUN.ToggleTime=0;															//��תʱ������
	STEP_MOTOx->TIME_RUN.RunCycle=STEP_MOTOx->TIME_RUN.StartCycle;	//�ָ���ʼʱ��
	
	GPIO_ResetBits(STEP_MOTOx->DIR_PORT,STEP_MOTOx->DIR_Pin);
	GPIO_ResetBits(STEP_MOTOx->PUL_PORT,STEP_MOTOx->PUL_Pin);				//ֹͣ
	
//	STEP_MOTOx->MOTO_STATUS=MOTOR_STATUS_STOP;											//״̬Ϊֹͣ״̬
//	STEP_MOTOx->MOTO_CTLSTATUS=MOTOR_WAIT_START;
}
/****************************************************************
*������:
*����:
*
******************************************************************/
void StepMotoPause(STEP_MOTO_CONF *STEP_MOTOx)
{
	STEP_MOTOx->TIME_RUN.StepTimeCount=0;														//���м�ʱ����
	STEP_MOTOx->TIME_RUN.ToggleTime=0;															//��תʱ������
	STEP_MOTOx->TIME_RUN.RunCycle=STEP_MOTOx->TIME_RUN.StartCycle;	//�ָ���ʼʱ��
	
	GPIO_ResetBits(STEP_MOTOx->DIR_PORT,STEP_MOTOx->DIR_Pin);
	GPIO_ResetBits(STEP_MOTOx->PUL_PORT,STEP_MOTOx->PUL_Pin);				//ֹͣ
	
//	STEP_MOTOx->MOTO_STATUS=MOTOR_STATUS_STOP;											//״̬Ϊֹͣ״̬
//	STEP_MOTOx->MOTO_CTLSTATUS=MOTOR_WAIT_START;
}
/****************************************************************
*������:
*����:
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

////-----------------------�������
///****************************************************************
//*������:
//*����:
//*
//******************************************************************/
//void ACMotoConf(STEP_MOTO_CONF *STEP_MOTOx)
//{
//	//____________PULת�ٿ���
//	GPIO_Configuration_OPP50	(STEP_MOTOx->PUL_PORT,	STEP_MOTOx->PUL_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
//	//____________DIR�������
//	GPIO_Configuration_OPP50	(STEP_MOTOx->DIR_PORT,	STEP_MOTOx->DIR_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
//	//____________ENʹ��
//	GPIO_Configuration_OPP50	(STEP_MOTOx->EN_PORT,		STEP_MOTOx->EN_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
//	//____________Brakeɲ������
//	GPIO_Configuration_OPP50	(STEP_MOTOx->Brake_PORT,	STEP_MOTOx->Brake_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
//	
//	//------
//	STEP_MOTOx->TIME_RUN.StepCycle=1000000/(STEP_MOTOx->TIME_CONF.BaseTime)/(STEP_MOTOx->TIME_CONF.StepFreq);				//������������=1S/ʱ��/Ƶ��
//	if(STEP_MOTOx->TIME_RUN.StepCycle<2)
//	{
//		STEP_MOTOx->TIME_RUN.StepCycle=2;
//	}
//	else if((STEP_MOTOx->TIME_RUN.StepCycle%2)!=0)
//	{
//		STEP_MOTOx->TIME_RUN.StepCycle-=1;
//	}
//	STEP_MOTOx->TIME_RUN.StartCycle=1000000/(STEP_MOTOx->TIME_CONF.BaseTime)/(STEP_MOTOx->TIME_CONF.StartFreq);			//��ʼ��������=1S/ʱ��/��ʼƵ��
//	STEP_MOTOx->TIME_RUN.StartSteps=(STEP_MOTOx->TIME_RUN.StartCycle-STEP_MOTOx->TIME_RUN.StepCycle)/500+0.5;				//��500������
//	STEP_MOTOx->TIME_RUN.RunCycle=STEP_MOTOx->TIME_RUN.StartCycle;		//�ָ���ʼʱ��
//	STEP_MOTOx->TIME_RUN.HalfCycle=STEP_MOTOx->TIME_RUN.RunCycle/2;		//������ʱ��
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
//*������:
//*����:
//*
//******************************************************************/
//void ACMotoRun(STEP_MOTO_CONF *STEP_MOTOx)
//{
//	STEP_MOTOx->TIME_RUN.StepTimeCount++;
//	if(STEP_MOTOx->TIME_RUN.StepTimeCount<=2)
//	{
//		GPIO_SetBits(STEP_MOTOx->DIR_PORT,STEP_MOTOx->DIR_Pin);
//	}
//	if(STEP_MOTOx->TIME_RUN.StepTimeCount<STEP_MOTOx->TIME_CONF.StepTimeOut)				//δ��ʱ
//	{
//		//1------------���ٽ׶�:��ǰ�������ڱ�Ԥ�����ڳ���ʾ���ڼ��ٽ׶�
//		//-------------����ʱ�����Ϊ500ms,ÿ1ms����һ��Ƶ�ʶ�,ֱ�����ٵ�Ԥ���ٶ�,��ز���:StartSteps
//		//-------------Ƶ�����Ӻ����Ƿ�������,���Խ�ʡ������ɺ��ʱ��
//		if(STEP_MOTOx->TIME_RUN.RunCycle>STEP_MOTOx->TIME_RUN.StepCycle)
//		{
//			if((STEP_MOTOx->TIME_RUN.StepTimeCount*STEP_MOTOx->TIME_CONF.BaseTime)%1000==0)		//1ms���ټ�ʱ
//			{
//				STEP_MOTOx->TIME_RUN.RunCycle-=2;
//				STEP_MOTOx->TIME_RUN.HalfCycle=(STEP_MOTOx->TIME_RUN.RunCycle/2);		//������ʱ��
////				STEP_MOTOx->TIME_RUN.ToggleTime=0;																//��תʱ������
//			}
//			//����Ƿ�������:����������,�����������в��ٽ���˲����Դﵽ��ʡ����ʱ���Ŀ��
//			if(STEP_MOTOx->TIME_RUN.RunCycle<=STEP_MOTOx->TIME_RUN.StepCycle)		//���ٹ������
//			{
//				STEP_MOTOx->TIME_RUN.RunCycle=STEP_MOTOx->TIME_RUN.StepCycle;			//������������������
//				STEP_MOTOx->TIME_RUN.HalfCycle=(STEP_MOTOx->TIME_RUN.RunCycle/2);		//������ʱ��
////				STEP_MOTOx->TIME_RUN.ToggleTime=0;																//��תʱ������
//			}
//		}
//		//1-------------PWM�����������
//		//1.1-----------���������תʱ��
//		if(STEP_MOTOx->TIME_RUN.ToggleTime++>=STEP_MOTOx->TIME_RUN.RunCycle-1)		//��ת��ʱ:ǰ������->�������״̬�л�,�������һ��Ԥ������,�������¼�ʱ
//		{
//			STEP_MOTOx->TIME_RUN.HalfCycle=(STEP_MOTOx->TIME_RUN.RunCycle/2);		//������ʱ��
//			STEP_MOTOx->TIME_RUN.ToggleTime=0;			//����
//		}
//		//1.2-----------�����ת����(ģ��PWM)		
//		if(STEP_MOTOx->TIME_RUN.ToggleTime<STEP_MOTOx->TIME_RUN.HalfCycle)		//��תʱ��ǰ�����������ƽ
//		{
//			GPIO_SetBits(STEP_MOTOx->PUL_PORT,STEP_MOTOx->PUL_Pin);
//		}
//		else			//��תʱ�ĺ�����������ƽ
//		{
//			GPIO_ResetBits(STEP_MOTOx->PUL_PORT,STEP_MOTOx->PUL_Pin);
//		}
//		STEP_MOTOx->MOTO_STATUS=MOTOR_STATUS_RUN;								//��ʱ��־
//	}
//	else		//��ʱ
//	{
//		StepMotoStop(STEP_MOTOx);																		//ֹͣ���
//	}
//}
///****************************************************************
//*������:
//*����:
//*
//******************************************************************/
//void ACMotoStop(STEP_MOTO_CONF *STEP_MOTOx)
//{
//	STEP_MOTOx->TIME_RUN.StepTimeCount=0;														//���м�ʱ����
//	STEP_MOTOx->TIME_RUN.ToggleTime=0;															//��תʱ������
//	STEP_MOTOx->TIME_RUN.RunCycle=STEP_MOTOx->TIME_RUN.StartCycle;	//�ָ���ʼʱ��
//	
//	GPIO_ResetBits(STEP_MOTOx->DIR_PORT,STEP_MOTOx->DIR_Pin);
//	GPIO_ResetBits(STEP_MOTOx->PUL_PORT,STEP_MOTOx->PUL_Pin);							//ֹͣ
//	
//	STEP_MOTOx->MOTO_STATUS=MOTOR_STATUS_STOP;			//״̬Ϊֹͣ״̬	
//}
///****************************************************************
//*������:
//*����:
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