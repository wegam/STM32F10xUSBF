#ifdef STM32_LCD

#include "STM32_LCD.H"

#include "string.h"				//�����ڴ��������ͷ�ļ�
#include "stm32f10x_dma.h"




#include "STM32F10x_BitBand.H"
#include "STM32_GPIO.H"
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32_WDG.H"
#include "STM32_PWM.H"
#include "STM32_PWM.H"
#include "STM32_GPIO.H"
#include "STM32_USART.H"
#include "STM32_DMA.H"


#include "TM1618.H"
#include "SSD1963.H"
#include "STM32_SDCard.H"
#include "GT32L32M0180.H"

//#define SDCardTest
//#define GT32L32M0180Test	
//


GT32L32_Init_TypeDef 	GT32L32_Init;
GT32L32_Info_TypeDef	GT32L32_Info;
SSD1963_Pindef 				SSD1963_Pinfo;
TM1618_Pindef					TM1618_1,TM1618_2;

u16 millisecond=0;
u8 hour=23,min=04,second=00;

//u8 GTBuffer[512]={0};		//�������ݴ洢�ռ�

u32	dspdata=0;
u16 us=0;
u16	mm=0;
u8	ss=0;
u8	hh=0;


void GT32L32_PinSet(void);
	
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->������		:	
//->��������	:	 
//->����		:	
//->���		:
//->���� 		:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
void STM32_LCD_Configuration(void)
{	
	SYS_Configuration();					//ϵͳ����---��ϵͳʱ�� STM32_SYS.H	
	GPIO_DeInitAll();							//�����е�GPIO�ر�----V20170605	
	STM32_LCD_PinConf();
	
	SSD1963_Configuration();					//LCD�ӿ�����
	
//	SSD1963_PinConf(&SSD1963_Pinfo);
//	GT32L32_Configuration();

	LCD_PrintfString(10,10,32,"Aʱ��A");					//��ߵ�ʡ�Ժž��ǿɱ����
	LCD_PrintfString(300,230,32,"%02d:",hour);		//��ߵ�ʡ�Ժž��ǿɱ����
	LCD_PrintfString(348,230,32,"%02d:",min);			//��ߵ�ʡ�Ժž��ǿɱ����
	LCD_PrintfString(396,230,32,"%02d",second);		//��ߵ�ʡ�Ժž��ǿɱ����

//		SSD1963_DrawCircle(400,240,100);
//	SSD1963_DrawRectangle(100,100,400,200);
	SSD1963_DrawLine(700,10,700,470);

	SysTick_Configuration(1000);											//ϵͳ���ʱ������72MHz,��λΪuS
	
	IWDG_Configuration(1000);													//�������Ź�����---������λms
	
	PWM_OUT(TIM2,PWM_OUTChannel1,5000,10);						//PWM�趨-20161127�汾
	
	SSD1963_DrawRectangle(100,100,750,200);
//	SSD1963_DrawLine(10,100,790,100);
//	SSD1963_DrawLine(400,100,400,200);
	
//	SSD1963_Clear(0x0000);	//�Ա���ɫ����
	
//	SSD1963_DrawLine(30, 50,500, 50);						//��ֱ��


	
}
//=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>=>
//->������		:	
//->��������	:	 
//->����		:	
//->���		:
//->���� 		:
//<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=<=
void STM32_LCD_Server(void)
{
	u16 delat=1000;
	IWDG_Feed();								//�������Ź�ι��
	
	millisecond++;
	if(millisecond>999)
	{
//		SSD1963_DrawRectangle(100,100,110,110);
		millisecond=0;
		second++;
		if(second>59)
		{
			second=0;
			min++;			
			if(min>59)
			{
				min=0;
				hour++;
				if(hour>23)
				{
					hour=0;
				}
				LCD_PrintfString(300,230,32,"%02d:",hour);		//��ߵ�ʡ�Ժž��ǿɱ����
				
			}
			LCD_PrintfString(348,230,32,"%02d:",min);		//��ߵ�ʡ�Ժž��ǿɱ����
		}
		LCD_PrintfString(396,230,32,"%02d",second);		//��ߵ�ʡ�Ժž��ǿɱ����
		while(delat--);
	}
//	TM1618_DIS();

//	LCD_PrintfString(600,230,32,"%03d",millisecond);		//��ߵ�ʡ�Ժž��ǿɱ����
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void STM32_LCD_PinConf(void)
{
	TM1618_PinSet();
	SSD1963_PinSet();
	GT32L32_PinSet();
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void TM1618_PinSet(void)
{
	TM1618_1.TM1618_CLK_PORT	=GPIOC;
	TM1618_1.TM1618_CLK_Pin		=GPIO_Pin_8;
	
	TM1618_1.TM1618_DIO_PORT	=GPIOC;
	TM1618_1.TM1618_DIO_Pin		=GPIO_Pin_9;	
	
	TM1618_1.TM1618_STB_PORT	=GPIOC;
	TM1618_1.TM1618_STB_Pin		=GPIO_Pin_10;
	
	TM1618_2.TM1618_CLK_PORT	=GPIOC;
	TM1618_2.TM1618_CLK_Pin		=GPIO_Pin_8;
	
	TM1618_2.TM1618_DIO_PORT	=GPIOC;
	TM1618_2.TM1618_DIO_Pin		=GPIO_Pin_9;	
	
	TM1618_2.TM1618_STB_PORT	=GPIOC;
	TM1618_2.TM1618_STB_Pin		=GPIO_Pin_11;
	
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
void SSD1963_PinSet(void)
{
	SSD1963_Pinfo.SSD1963_sBACKLIGHT_PORT	=	GPIOC;
	SSD1963_Pinfo.SSD1963_sBACKLIGHT_Pin	=	GPIO_Pin_0;
	
	SSD1963_Pinfo.SSD1963_sRD_PORT				=	GPIOC;
	SSD1963_Pinfo.SSD1963_sRD_Pin					=	GPIO_Pin_1;	
	
	SSD1963_Pinfo.SSD1963_sREST_PORT			=	GPIOC;
	SSD1963_Pinfo.SSD1963_sREST_Pin				=	GPIO_Pin_2;
	
	SSD1963_Pinfo.SSD1963_sDC_PORT				=	GPIOC;
	SSD1963_Pinfo.SSD1963_sDC_Pin					=	GPIO_Pin_3;
	
	SSD1963_Pinfo.SSD1963_sWR_PORT				=	GPIOC;
	SSD1963_Pinfo.SSD1963_sWR_Pin					=	GPIO_Pin_4;	
	
	SSD1963_Pinfo.SSD1963_sCS_PORT				=	GPIOC;
	SSD1963_Pinfo.SSD1963_sCS_Pin					=	GPIO_Pin_5;
	
	SSD1963_Pinfo.SSD1963_sDATABUS_PORT		=	GPIOE;
	SSD1963_Pinfo.SSD1963_sDATABUS_Pin		=	GPIO_Pin_All;
	
	SSD1963_PinConf(&SSD1963_Pinfo);
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void GT32L32_PinSet(void)
{
	GT32L32_Init.sSPIx=SPI1;
	GT32L32_Init.sGT32L32_CS_PORT=GPIOB;
	GT32L32_Init.sGT32L32_CS_PIN=GPIO_Pin_14;
	GT32L32_Init.SPI_BaudRatePrescaler_x=SPI_BaudRatePrescaler_128;
	
	GT32L32_ConfigurationNR(&GT32L32_Init);				//��ͨSPIͨѶ��ʽ����
//	GT32L32_PinConf(&GT32L32_Init);
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
		
		dspdata=(u32)mm+(u32)ss*100+(u32)hh*10000;
		TM1618_WriteDataN(&TM1618_1,dspdata/10000);
		TM1618_WriteDataN(&TM1618_2,dspdata%10000);	
	}
}
/*******************************************************************************
*������		:	LCD_ShowString
*��������	:	��ʾ�ַ�����ͨ�ֿ�
*����			: x,y:�������
						*p:�ַ�����ʼ��ַ
						��16����
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
unsigned int LCD_PrintfString(u16 x,u16 y,u8 font,const char *format,...)				//��ߵ�ʡ�Ժž��ǿɱ����
{ 
		
//		va_list ap; 										//VA_LIST ����C�����н����������һ��꣬����ͷ�ļ���#include <stdarg.h>,���ڻ�ȡ��ȷ�������Ĳ���
//		static char string[ 256 ];			//�������飬
//  	va_start( ap, format );
//		vsprintf( string , format, ap );    
//		va_end( ap );
	
	char	*Char_Buffer=NULL;		//��¼format����
	u16 i=0;		//��ʾ

	//1)**********��ȡ���ݿ���
	u16 num=strlen((const char*)format);		//��ȡ���ݿ���
	//2)**********���建������С����
	unsigned int BufferSize;
	//3)**********argsΪ�����һ��ָ��ɱ�����ı�����va_list�Լ��±�Ҫ�õ���va_start,va_end�������ڶ��壬�ɱ���������б���Ҫ�õ��꣬ ��stdarg.hͷ�ļ��ж���
	va_list args;                                        
	//4)**********���붯̬�ռ�
	Char_Buffer = (char*)malloc(sizeof(char) * num);
	if(Char_Buffer==NULL)
	{
		Char_Buffer=NULL;
		return 0;
	}
	//5)**********��ʼ��args�ĺ�����ʹ��ָ��ɱ�����ĵ�һ��������format�ǿɱ������ǰһ������
	va_start(args, format);
	//6)**********��������·��������ִ��ĳ���(��ȥ\0),����������ظ�ֵ
	BufferSize = vsprintf(Char_Buffer, format, args);
	num=BufferSize;
	//7)**********�����ɱ�����Ļ�ȡ
	va_end(args);                                      		
	//8)**********���ȷ��ͻ�������С�����ݸ���������������ַ����DMA��������
//	while(*Char_Buffer!='\0')
	for(i=0;i<num;i++)
	{ 
		unsigned char dst=Char_Buffer[i];
		u8 GTBuffer[512]={0};		//�������ݴ洢�ռ�
		u32 lengh=0;						//���ֵ�������ݳ���		
		if(dst>0x80)		//˫�ֽ�--����
		{
			u16 word=dst<<8;			
//			Char_Buffer++;
			dst=Char_Buffer[i+1];
			word=word|dst;			
			//��ʾ�����ж�
			if(x>LCD_W-16)
			{
				x=0;
				y+=32;
			}
			if(y>LCD_H-32)
			{
				y=x=0;
				SSD1963_Clear(SSD1963_RED);
			}
			lengh=GT32L32_ReadBuffer(&GT32L32_Init,font,word,GTBuffer);		//���ֿ��ж����ݺ���
			//д����Ļ
			SSD1963_ShowString(x,y,font,lengh,GTBuffer);
			//��ʾ��ַ����	
			if(font==12)
			{
				x+=12;
			}
			else if(font==16)
			{
				x+=16;
			}
			else if(font==24)
			{
				x+=24;
			}
			else if(font==32)
			{
				x+=32;
			}
//			Char_Buffer++;
			i++;		//˫�ֽڣ�������
		}
		else		//���ֽ�
		{			
			if(x>LCD_W-16)
			{
				x=0;
				y+=32;
			}
			if(y>LCD_H-32)
			{
				y=x=0;
				SSD1963_Clear(SSD1963_RED);
			}
			lengh=GT32L32_ReadBuffer(&GT32L32_Init,font,(u16)dst,GTBuffer);		//���ֿ��ж����ݺ���
//			//д����Ļ
			SSD1963_ShowString(x,y,font,lengh,GTBuffer);			
			//��ʾ��ַ����
			if(font==12)
			{
				x+=6;
			}
			else if(font==16)
			{
				x+=8;
			}
			else if(font==24)
			{
				x+=12;
			}
			else if(font==32)
			{
				x+=16;
			}			
//			Char_Buffer++;
//			i++;		//˫�ֽڣ�������
		}
	}
	//9)**********DMA������ɺ�ע��Ӧ���ͷŻ�������free(USART_BUFFER);
	free(Char_Buffer);		//������ɺ�ע��Ӧ���ͷŻ�������free(Char_Buffer); 
	return BufferSize;
}

#endif