/*********************************************
*步进电机驱动
*结构体参数配置形式
*驱动模式---计时
*特殊功能:超时,加速
*最高优先级:停止--任何情况下,有停止命令,优先执行停止
*其它:刹车
**********************************************/

#include "74HC165.H"

#include "STM32_GPIO.H"



/****************************************************************
*函数名:
*描述:
*
******************************************************************/
void HC165Conf(HC165_CONF *HC165x)
{
	//____________异步并行加入端(低电平有效)
	GPIO_Configuration_OPP50	(HC165x->PL_PORT,		HC165x->PL_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	//____________时钟输入端(低到高边沿触发--上升沿)
	GPIO_Configuration_OPP50	(HC165x->CP_PORT,		HC165x->CP_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	//____________时钟使能端(低电平有效)
	GPIO_Configuration_OPP50	(HC165x->CE_PORT,		HC165x->CE_Pin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
	//____________串行数据输出端(读串行数据端口)
	GPIO_Configuration_IPU	(HC165x->Q7_PORT,		HC165x->Q7_Pin);				//将GPIO相应管脚配置为上拉输入模式----V20170605
}
/****************************************************************
*函数名:
*描述:
*
******************************************************************/
unsigned long HC165Read(HC165_CONF *HC165x)
{
	unsigned long i=0;
	unsigned long	reValue=0;	
	unsigned long *P=(unsigned long*)&HC165x->HC165_Data.Data32;		//获取数据结构体地址,以便将读取的数据保存	
	//CE-L
	HC165x->CE_PORT->BRR 	= HC165x->CE_Pin;		//CE-L
	HC165x->PL_PORT->BRR	=	HC165x->PL_Pin;		//PL-L
	__nop();			//delay
	HC165x->PL_PORT->BSRR	=	HC165x->PL_Pin;		//PL-H
	for(i=0;i<HC165x->HC165_Data.NumOfHC165*8;i++)
	{
		reValue<<=1;			//先读高位(芯片输出顺序为先高位后低位)
		HC165x->CP_PORT->BRR	=	HC165x->CP_Pin;	//CP-L
		__nop();					//delay
		if(HC165x->Q7_PORT->IDR&HC165x->Q7_Pin)
		{
			reValue|=0x01;
		}
		HC165x->CP_PORT->BSRR	=	HC165x->CP_Pin;	//CP-H
	}	
	HC165x->CE_PORT->BSRR	=	HC165x->CE_Pin;		//CD-H
	*P=reValue;
	return	reValue;	//将数据返回(如果需要直接获取读取值可以使用返回值)
}
/****************************************************************
*函数名:
*描述:
*
******************************************************************/
void HC165_TEST(HC165_CONF *HC165x)
{
	//____________异步并行加入端(低电平有效)
	HC165x->PL_PORT=GPIOC;
	HC165x->PL_Pin=GPIO_Pin_0;
	
	//____________时钟输入端(低到高边沿触发--上升沿)
	HC165x->CP_PORT=GPIOC;
	HC165x->CP_Pin=GPIO_Pin_1;
	
	//____________时钟使能端(低电平有效)
	HC165x->CE_PORT=GPIOC;
	HC165x->CE_Pin=GPIO_Pin_2;
	
	//____________串行数据输出端(读串行数据端口)
	HC165x->Q7_PORT=GPIOC;
	HC165x->Q7_Pin=GPIO_Pin_3;
	
	HC165x->HC165_Data.NumOfHC165=2;		//串接的74HC165芯片个数,一个8位:2个
	
	HC165Conf(HC165x);
//	HC165Read(HC165x);
}
