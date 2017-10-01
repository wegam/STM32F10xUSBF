#ifdef TM1618

#include "TM1618_TEST.H"

#include "TM1618.H"

#include "STM32_PWM.H"
#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_WDG.H"
#include "STM32_SYSTICK.H"
#include "STM32F10x_BitBand.H"

//#include "stdio.h"
//#include "stm32f10x_type.h"
//#include "STM32_SPI.H"
//#include "STM32_PWM.H"
//#include "STM32_ADC.H"
//#include "STM32_USART.H"
//#include "STM32F10x_BitBand.H"
//#include "STM32_SYSTICK.H"

//#include "STM32_SPI.H"

/******************************MX25L4006E�����*******************************/


//sbit stb=P3^4;		//Ƭѡ�����������½��س�ʼ�����нӿڣ����ȴ�����ָ�
//									//STBΪ�ͺ�ĵ�һ���ֽ���Ϊָ�������ָ��ʱ����ǰ������
//									//������ֹ����STB Ϊ��ʱ��CLK ������
////_________________________________________________________________________________________________
//sbit clk=P3^0;		//ʱ�����������봮�����ݡ�
////__________________________________________________________________________________________________
//sbit dio=P3^1;	  	//��ʱ�����������봮�����ݣ��ӵ�λ��ʼ��

#define clk	PC8
#define	dio	PC9
#define	stb	PC10

#define	SEGEN		PC12
#define SEG1EN	PC10
#define	SEG2EN	PC11

#define	delayt	5

unsigned char seg4[]={0x3F,0X06,0X5B,0X4F,0x66,0X6D,0X7D,0x07,0x7f,0x6f,0x00,0xff};
unsigned char addr[]={0XC0,0XC2,0XC4,0XC6,0XC8,0XCA,0XCC,0XCE};
unsigned char s[8]={0};
unsigned char d[8]={0};
unsigned char flag=0;
		

TM1618_Pindef	TM1618_1,TM1618_2;

u32	dspdata=0;
u16 us=0;
u16	mm=0;
u8	ss=0;
u8	hh=0;
/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void TM1618_Configuration(void)
{	
	SYS_Configuration();					//ϵͳ����---��ϵͳʱ�� STM32_SYS.H
	
	GPIO_DeInitAll();							//�����е�GPIO�ر�----V20170605
	
	TM1618_PinSet();
	
	SysTick_Configuration(1000);	//ϵͳ���ʱ������72MHz,��λΪuS
	
	IWDG_Configuration(1000);			//�������Ź�����---������λms	
	
	PWM_OUT(TIM2,PWM_OUTChannel1,5000,20);						//PWM�趨-20161127�汾
	
//	TM1618_WriteByte(&TM1618_1,5);
	

}
/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void TM1618_Server(void)
{
	IWDG_Feed();								//�������Ź�ι��
	TM1618_DIS();
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void TM1618_PinSet(void)
{
	TM1618_1.TM1618_CLK_PORT=GPIOC;
	TM1618_1.TM1618_CLK_Pin=GPIO_Pin_8;
	
	TM1618_1.TM1618_DIO_PORT=GPIOC;
	TM1618_1.TM1618_DIO_Pin=GPIO_Pin_9;	
	
	TM1618_1.TM1618_STB_PORT=GPIOC;
	TM1618_1.TM1618_STB_Pin=GPIO_Pin_10;
	
	TM1618_2.TM1618_CLK_PORT=GPIOC;
	TM1618_2.TM1618_CLK_Pin=GPIO_Pin_8;
	
	TM1618_2.TM1618_DIO_PORT=GPIOC;
	TM1618_2.TM1618_DIO_Pin=GPIO_Pin_9;	
	
	TM1618_2.TM1618_STB_PORT=GPIOC;
	TM1618_2.TM1618_STB_Pin=GPIO_Pin_11;
	
	TM1618_PinConf(&TM1618_1);
	TM1618_PinConf(&TM1618_2);
	
	GPIO_Configuration_OPP50	(GPIOC,	GPIO_Pin_12);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�50MHz----V20170605
	
	PC12=1;
}

/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void TM1618_DIS(void)
{
	if(us++>=1000)
	{
		us=0;
		mm++;
		if(mm>=60)
		{
			mm=0;
			ss++;
		}
		if(ss>=60)
		{
			ss=0;
			hh++;
		}
		if(hh>=24)
		{
			hh=0;
		}
		
		dspdata=(u32)mm+(u32)ss*100+(u32)((ss%10)*10+(ss/10))*10000+(u32)((mm%10)*10+(mm/10))*1000000;
		TM1618_WriteDataN(&TM1618_1,dspdata/10000);
		TM1618_WriteDataN(&TM1618_2,dspdata%10000);	
	}
}


#endif

