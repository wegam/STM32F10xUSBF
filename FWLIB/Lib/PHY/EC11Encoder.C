#include "EC11Encoder.H"


#include "STM32_GPIO.H"

#include "stm32f10x_gpio.h"


/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void EC11_PinConf(EC11_ConfTypeDef *EC11_Conf)
{
	GPIO_Configuration_IPU	(EC11_Conf->EC11_Button_PORT,		EC11_Conf->EC11_Button_Pin);		//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU	(EC11_Conf->EC11_A_PORT,				EC11_Conf->EC11_A_Pin);					//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	GPIO_Configuration_IPU	(EC11_Conf->EC11_B_PORT,				EC11_Conf->EC11_B_Pin);					//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	
	//ʱ�����Ĭ�����ã�//���������1usʱ�����
	if(EC11_Conf->LongPressFlag!=0)			//��������
	{
		if(EC11_Conf->LongPressStartTime==0)	//������Ч������ʼʱ��δ�趨��3��󰴼�δ�ɿ���ʾ����
		{
			EC11_Conf->LongPressStartTime=3000000;	//3x1000x1000		3��
		}
		if(EC11_Conf->LongPressEffectiveTime==0)	//������Ч���ǲ���ʱ��δ�趨��ÿ0.5��500ms)��һ��
		{
			EC11_Conf->LongPressEffectiveTime=500000;	//5x100x1000		0.5��
		}
	}
	if(EC11_Conf->ButtonEffectiveTime==0)	//��������Чʱ��--0.05��
	{
		EC11_Conf->ButtonEffectiveTime=50000;			//1x100x1000
	}
	if(EC11_Conf->EncoderEffectiveTime==0)	//��������Чʱ��---1mS
	{
		EC11_Conf->EncoderEffectiveTime=800;				//1x800us
	}
	EC11_Conf->ButtonActiveFlag=0;			//������Ч��־----��Ե��ΰ���,ButtonActiveFlag==0;����ʱ��δ����������Ч��ButtonActiveFlag==1��������Ч��ButtonTimeCount���ټ�ʱ����ֹ���
}
/*******************************************************************************
*������			:	EC11_GetStatus
*��������		:	��ȡ��������״ֵ̬
*����				: 
*����ֵ			:	��
*******************************************************************************/
EC11_StatusTypeDef EC11_GetStatus(EC11_ConfTypeDef *EC11_Conf)
{
	EC11_StatusTypeDef	EC11_Status=EC11_IDLE;	//�޴���
	//--------------��������
	if(GPIO_ReadInputDataBit(EC11_Conf->EC11_Button_PORT,EC11_Conf->EC11_Button_Pin)==0)		//����͵�ƽ---����ѹ
	{
//		EC11_Conf->ButtonTimeCount++;		//������ʱ
		//------���ΰ���
		if(EC11_Conf->LongPressFlag==0&&EC11_Conf->ButtonActiveFlag==0)
		{			
			if(EC11_Conf->ButtonTimeCount<EC11_Conf->ButtonEffectiveTime)	//��ʱδ���ﰴ����Чʱ��
			{
				EC11_Conf->ButtonTimeCount++;		//������ʱ
			}
			else
			{
				EC11_Conf->ButtonActiveFlag=1;	//������Ч��־----��Ե��ΰ���,ButtonActiveFlag==0;����ʱ��δ����������Ч��ButtonActiveFlag==1��������Ч��ButtonTimeCount���ټ�ʱ����ֹ���
				return(EC11_Button);		//����һ����Ч����
			}
		}
		//------����
		else	if(EC11_Conf->LongPressFlag==1)
		{
			EC11_Conf->ButtonTimeCount++;		//������ʱ
			if(EC11_Conf->ButtonTimeCount>=EC11_Conf->ButtonEffectiveTime&&EC11_Conf->ButtonActiveFlag==0)	//��ʱ���ﰴ����Чʱ����δ�������δ���
			{
				EC11_Conf->ButtonActiveFlag=1;	//������Ч��־----��Ե��ΰ���,ButtonActiveFlag==0;����ʱ��δ����������Ч��ButtonActiveFlag==1��������Ч��ButtonTimeCount���ټ�ʱ����ֹ���
				return(EC11_Button);		//����һ����Ч����
			}
			else if(EC11_Conf->ButtonTimeCount>=EC11_Conf->LongPressStartTime&&EC11_Conf->ButtonActiveFlag==1)	//��ʱ���ﰴ����Чʱ�����ѷ������δ�����������Ϊ������������
			{
				if(EC11_Conf->ButtonTimeCount>=EC11_Conf->LongPressStartTime+EC11_Conf->LongPressEffectiveTime)		//����һ��������ʱ
				{
					EC11_Conf->ButtonTimeCount-=EC11_Conf->LongPressEffectiveTime;			//�ָ�һ��������ʱ��׼����һ��������ʱ 0.2��һ����������
					return(EC11_Button);		//����һ����Ч����
				}				
			}
		}
	}
	else		//�����ɿ�
	{
		EC11_Conf->ButtonActiveFlag=0;	//������Ч��־----��Ե��ΰ���,ButtonActiveFlag==0;����ʱ��δ����������Ч��ButtonActiveFlag==1��������Ч��ButtonTimeCount���ټ�ʱ����ֹ���
		EC11_Conf->ButtonTimeCount=0;		//���������ʱ
	}
	//--------------����������
	if(GPIO_ReadInputDataBit(EC11_Conf->EC11_A_PORT,EC11_Conf->EC11_A_Pin)==0)		//A�ż�⵽�͵�ƽ
	{
		EC11_Conf->EncoderTimeCountA++;		//����A��ʱʱ��---��ʱ��EncoderEffectiveTime��ʾ������Ч�����˸���ʹ��
	}
	if(GPIO_ReadInputDataBit(EC11_Conf->EC11_B_PORT,EC11_Conf->EC11_B_Pin)==0)		//B�ż�⵽�͵�ƽ
	{
		EC11_Conf->EncoderTimeCountB++;		//����B��ʱʱ��---��ʱ��EncoderEffectiveTime��ʾ������Ч�����˸���ʹ��
	}
	if(		(GPIO_ReadInputDataBit(EC11_Conf->EC11_A_PORT,EC11_Conf->EC11_A_Pin)==1)	\
			&&(GPIO_ReadInputDataBit(EC11_Conf->EC11_B_PORT,EC11_Conf->EC11_B_Pin)==1)	\
			&&(EC11_Conf->EncoderTimeCountA>EC11_Conf->EncoderEffectiveTime							\
			&&EC11_Conf->EncoderTimeCountB>EC11_Conf->EncoderEffectiveTime)
		)
	{
		if(EC11_Conf->EncoderTimeCountA>EC11_Conf->EncoderTimeCountB)
		{
			EC11_Status=EC11_ClockWise;					//CW˳ʱ���ź�
		}
		else
		{
			EC11_Status=EC11_AntiClockWise;				//CCW��ʱ���ź�
		}		
		EC11_Conf->EncoderTimeCountA=0;		//����A��ʱʱ��---��ʱ��EncoderEffectiveTime��ʾ������Ч�����˸���ʹ��
		EC11_Conf->EncoderTimeCountB=0;		//����B��ʱʱ��---��ʱ��EncoderEffectiveTime��ʾ������Ч�����˸���ʹ��
		return(EC11_Status);
	}
	return(EC11_Status);
}














