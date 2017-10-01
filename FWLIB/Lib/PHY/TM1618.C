#include "TM1618.H"
#include "STM32_GPIO.H"
//#include "STM32F10x_BitBand.H"

#define	TM1618_DelayTime	1

unsigned char TM1618_Daddr[]={0XC0,0XC2,0XC4,0XC6,0XC8,0XCA,0XCC,0XCE};			//��ʾ��ַ
unsigned char TM1618_SegCode[]={0x3F,0X06,0X5B,0X4F,0x66,0X6D,0X7D,0x07,0x7f,0x6f,0x00,0xff};
unsigned char TM1618_CovSeg[8]={0};			//������ʾʱת������ʾ����
//unsigned char d[8]={0};


void TM1618_Delay(u16 time);
void TM1618_WriteByte(TM1618_Pindef *Pinfo,unsigned char byted);
void TM1618_WriteCommand(TM1618_Pindef *Pinfo,unsigned char command);
void TM1618_WriteCommandN(TM1618_Pindef *Pinfo,unsigned char command);
void TM1618_DataCov(u16 number);			//������ʾ����ת��

/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void TM1618_Delay(u16 time)
{
	while(time--);
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void TM1618_PinConf(TM1618_Pindef *Pinfo)
{
	GPIO_Configuration_OPP50	(Pinfo->TM1618_DIO_PORT,		Pinfo->TM1618_DIO_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->TM1618_CLK_PORT,		Pinfo->TM1618_CLK_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->TM1618_STB_PORT,		Pinfo->TM1618_STB_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void TM1618_WriteByte(TM1618_Pindef *Pinfo,unsigned char byted)
{
	unsigned char i=0;
	for(i=0;i<8;i++)
	{
		GPIO_ResetBits(Pinfo->TM1618_CLK_PORT, Pinfo->TM1618_CLK_Pin);
		TM1618_Delay(TM1618_DelayTime);
		if((byted&0x01)==0x01)						 //ȷ�����λ�����ӵ�λ��ʼд��
		{
			GPIO_SetBits(Pinfo->TM1618_DIO_PORT, Pinfo->TM1618_DIO_Pin);
		}
		else
		{
			GPIO_ResetBits(Pinfo->TM1618_DIO_PORT, Pinfo->TM1618_DIO_Pin);
		}
		TM1618_Delay(TM1618_DelayTime);
		GPIO_SetBits(Pinfo->TM1618_CLK_PORT, Pinfo->TM1618_CLK_Pin);
		TM1618_Delay(TM1618_DelayTime);
		byted=byted>>1;							 //����һλ		
	}
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void TM1618_WriteCommand(TM1618_Pindef *Pinfo,unsigned char command)
{
//	stb=1;
	GPIO_ResetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);			//stb=0;
	TM1618_Delay(TM1618_DelayTime);
	TM1618_WriteByte(Pinfo,0x44);	//�̶���ַģʽ
	TM1618_Delay(TM1618_DelayTime);
	GPIO_SetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);		//stb=1;
	TM1618_Delay(TM1618_DelayTime);
	
	GPIO_ResetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);			//stb=0;
	TM1618_Delay(TM1618_DelayTime);
	TM1618_WriteByte(Pinfo,command);				//����������� 0x80���أ�<0x88<0x89<0x8a<0x8b<0x8c<0x8d<0x8e<0x8f
	TM1618_Delay(TM1618_DelayTime);
	GPIO_SetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);		//stb=1;
	TM1618_Delay(TM1618_DelayTime);
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void TM1618_WriteCommandN(TM1618_Pindef *Pinfo,unsigned char command)
{
//	stb=1;
	GPIO_SetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);			//stb=0;
	TM1618_Delay(TM1618_DelayTime);
	TM1618_WriteByte(Pinfo,0x44);																			//�̶���ַģʽ
	TM1618_Delay(TM1618_DelayTime);
	GPIO_ResetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);		//stb=1;
	TM1618_Delay(TM1618_DelayTime);
	
	GPIO_SetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);			//stb=0;
	TM1618_Delay(TM1618_DelayTime);
	TM1618_WriteByte(Pinfo,command);				//�����������(����) 0x80���أ�<0x88<0x89<0x8a<0x8b<0x8c<0x8d<0x8e<0x8f
	TM1618_Delay(TM1618_DelayTime);
	GPIO_ResetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);		//stb=1;
	TM1618_Delay(TM1618_DelayTime);
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void TM1618_WriteData(TM1618_Pindef *Pinfo,u16 Data)
{
	unsigned char i=0;
	TM1618_WriteCommand(Pinfo,0X88);			//����
	TM1618_Delay(TM1618_DelayTime);
	for(i=0;i<8;i++)
	{
		GPIO_ResetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);	//stb=0;
		TM1618_Delay(TM1618_DelayTime);
		TM1618_WriteByte(Pinfo,Data);				//д���ַ
		TM1618_WriteByte(Pinfo,Data);				//д������
		TM1618_Delay(TM1618_DelayTime);
		GPIO_SetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);		//stb=1;
	}
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void TM1618_WriteDataN(TM1618_Pindef *Pinfo,u16 Data)
{
	unsigned char i=0;
	TM1618_DataCov(Data);
	TM1618_WriteCommandN(Pinfo,0X8A);			//����
	TM1618_Delay(TM1618_DelayTime);
	for(i=0;i<8;i++)
	{
		GPIO_SetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);			//stb=0;
		TM1618_WriteByte(Pinfo,TM1618_Daddr[i]);				//д���ַ
		TM1618_Delay(TM1618_DelayTime);
		TM1618_WriteByte(Pinfo,TM1618_CovSeg[i]);				//д������
		TM1618_Delay(TM1618_DelayTime);
		GPIO_ResetBits(Pinfo->TM1618_STB_PORT, Pinfo->TM1618_STB_Pin);		//stb=1;
	}
}
/*******************************************************************************
*������			:	function
*��������		:	������ʾ����ת��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void TM1618_DataCov(u16 number)
{
	unsigned char s[4];
	unsigned char	i,j;
	s[0]=TM1618_SegCode[number/1000];
	s[1]=TM1618_SegCode[number%1000/100];
	s[2]=TM1618_SegCode[number%100/10];
	s[3]=TM1618_SegCode[number%10];
	
//	if(number<10)
//	{
//		s[0]=TM1618_SegCode[10];
//		s[1]=TM1618_SegCode[10];
//		s[2]=TM1618_SegCode[10];
//	}
//	else if(100>number&&number>9)
//	{
//		s[0]=TM1618_SegCode[10];
//		s[1]=TM1618_SegCode[10];
//	}
//	else if(1000>number&&number>99)
//		s[0]=TM1618_SegCode[10];
	
	
	
	for(i=0;i<8;i++)
	{
		for(j=0;j<4;j++)
		{
			TM1618_CovSeg[i]<<=1;
			if(s[j]&0x01)
			TM1618_CovSeg[i]|=0x01;
			s[j]>>=1;
			
		}
	}

}


