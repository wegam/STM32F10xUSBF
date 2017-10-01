#include "CS5530.H"

#include "STM32_GPIO.H"

#include <stdint.h>


//#define CS5530_CS_LOW()			((pInfo->pGPIOxCS)->BRR = pInfo->pinCS)
//#define CS5530_CS_HIGH()		((pInfo->pGPIOxCS)->BSRR = pInfo->pinCS)
//#define CS5530_SDI_LOW()		((g_CS5530PinInfo->pGPIOxSDI)->BRR = g_CS5530PinInfo->pinSDI)
//#define CS5530_SDI_HIGH()		((g_CS5530PinInfo->pGPIOxSDI)->BSRR = g_CS5530PinInfo->pinSDI)
//#define CS5530_SDO_STATE()	((g_CS5530PinInfo->pGPIOxSDO)->IDR & g_CS5530PinInfo->pinSDO)
//#define CS5530_SCLK_LOW()		((g_CS5530PinInfo->pGPIOxSCLK)->BRR = g_CS5530PinInfo->pinSCLK)
//#define CS5530_SCLK_HIGH()	((g_CS5530PinInfo->pGPIOxSCLK)->BSRR = g_CS5530PinInfo->pinSCLK)
unsigned long CS5530_Status=0x00;



void CS5530_Delay(u32 time);
void CS5530_CS_LOW(CS5530_Pindef *Pinfo);
void CS5530_CS_HIGH(CS5530_Pindef *Pinfo);
void CS5530_SDI_LOW(CS5530_Pindef *Pinfo);
void CS5530_SDI_HIGH(CS5530_Pindef *Pinfo);
void CS5530_SCLK_LOW(CS5530_Pindef *Pinfo);
void CS5530_SCLK_HIGH(CS5530_Pindef *Pinfo);
u8 CS5530_SDO_STATE(CS5530_Pindef *Pinfo);

void CS5530_WriteOneByte(CS5530_Pindef *Pinfo, u8 dat);
void CS5530_WriteRegister(CS5530_Pindef *Pinfo, u8 command,u32 setData);
//void CS5530_WriteCommand(CS5530_Pindef *Pinfo,u8 command);

unsigned char CS5530_ReadOneByte(CS5530_Pindef *Pinfo);
u32 CS5530_GetADData(CS5530_Pindef *Pinfo);
u32 CS5530_ReadRegister(CS5530_Pindef *Pinfo, u8 command);
void CS5530_ClearBuf(CS5530_Pindef *Pinfo);


void CS5530_PowerUp(CS5530_Pindef *Pinfo);		//CS5530�ϵ缰��ʼ��



/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void CS5530_Delay(u32 time)
{
	while(time--);
}
/*******************************************************************************
* ������			:	function
* ��������		:	���ذ�
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void CS5530_CS_LOW(CS5530_Pindef *Pinfo)
{	
	GPIO_ResetBits(Pinfo->CS5530_CS_PORT, Pinfo->CS5530_CS_Pin);
}
/*******************************************************************************
* ������			:	function
* ��������		:	���ذ�
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void CS5530_CS_HIGH(CS5530_Pindef *Pinfo)
{
	GPIO_SetBits(Pinfo->CS5530_CS_PORT, Pinfo->CS5530_CS_Pin);
}
/*******************************************************************************
* ������			:	function
* ��������		:	���ذ�
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void CS5530_SDI_LOW(CS5530_Pindef *Pinfo)
{
	GPIO_ResetBits(Pinfo->CS5530_SDI_PORT, Pinfo->CS5530_SDI_Pin);
}
/*******************************************************************************
* ������			:	function
* ��������		:	���ذ�
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void CS5530_SDI_HIGH(CS5530_Pindef *Pinfo)
{
	GPIO_SetBits(Pinfo->CS5530_SDI_PORT, Pinfo->CS5530_SDI_Pin);
}
/*******************************************************************************
* ������			:	function
* ��������		:	���ذ�
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void CS5530_SCLK_LOW(CS5530_Pindef *Pinfo)
{
	GPIO_ResetBits(Pinfo->CS5530_SCLK_PORT, Pinfo->CS5530_SCLK_Pin);
}
/*******************************************************************************
* ������			:	function
* ��������		:	���ذ�
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void CS5530_SCLK_HIGH(CS5530_Pindef *Pinfo)
{
	GPIO_SetBits(Pinfo->CS5530_SCLK_PORT, Pinfo->CS5530_SCLK_Pin);
}
/*******************************************************************************
* ������			:	function
* ��������		:	���ذ�
* ����			: void
* ����ֵ			: void
*******************************************************************************/
u8 CS5530_SDO_STATE(CS5530_Pindef *Pinfo)
{
	return(GPIO_ReadInputDataBit(Pinfo->CS5530_SDO_PORT, Pinfo->CS5530_SDO_Pin));
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void CS5530_PinConf(CS5530_Pindef *Pinfo)
{
	GPIO_Configuration_OPP50	(Pinfo->CS5530_CS_PORT,		Pinfo->CS5530_CS_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->CS5530_SDI_PORT,	Pinfo->CS5530_SDI_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->CS5530_SCLK_PORT,	Pinfo->CS5530_SCLK_Pin);		//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_IPU	(Pinfo->CS5530_SDO_PORT,	Pinfo->CS5530_SDO_Pin);			//��GPIO��Ӧ�ܽ�����Ϊ��������ģʽ----V20170605
	
	CS5530_PowerUp(Pinfo);		//CS5530�ϵ缰��ʼ��
}
/*******************************************************************************
* ������			:	function
* ��������		:	���ذ�
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void CS5530_WriteOneByte(CS5530_Pindef *Pinfo, uint8_t dat)
{
	uint8_t i;

	for(i=0; i<8; i++)
	{
		if((dat&0x80) != 0)
			CS5530_SDI_HIGH(Pinfo);
		else
			CS5530_SDI_LOW(Pinfo);
		__nop();
		__nop();
		__nop();
		__nop();
//		CS5530_Delay(1);
		CS5530_SCLK_HIGH(Pinfo);
		__nop();
		__nop();
		__nop();
		__nop();
//		CS5530_Delay(1);
		CS5530_SCLK_LOW(Pinfo);
		__nop();
		__nop();
//		__nop();
//		__nop();
//		CS5530_Delay(1);
		dat <<= 1;
	}
}

/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void CS5530_WriteCommand(CS5530_Pindef *Pinfo,u8 command)
{
	CS5530_WriteOneByte(Pinfo,command);
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void CS5530_WriteRegister(CS5530_Pindef *Pinfo, u8 command,u32 setData)
{
//	unsigned long temp=0xFFFFFFFF;
	CS5530_WriteOneByte(Pinfo, command);
	CS5530_WriteOneByte(Pinfo, (uint8_t)(setData>>24));
	CS5530_WriteOneByte(Pinfo, (uint8_t)(setData>>16));
	CS5530_WriteOneByte(Pinfo, (uint8_t)(setData>>8));
	CS5530_WriteOneByte(Pinfo, (uint8_t)(setData>>0));
}

/*******************************************************************************
* ������			:	function
* ��������		:	���ذ�
* ����			: void
* ����ֵ			: void
*******************************************************************************/
unsigned char CS5530_ReadOneByte(CS5530_Pindef *Pinfo)
{
	uint8_t i;
	uint8_t reValue=0;

	CS5530_SDI_LOW(Pinfo);
	for(i=0; i<8; i++)
	{
		__nop();
		__nop();
		//Delay_us(100);
		CS5530_SCLK_HIGH(Pinfo);
		__nop();
		__nop();
		__nop();
		__nop();
		//Delay_us(100);
		reValue <<= 1;
		if(CS5530_SDO_STATE(Pinfo))
			reValue++;
		CS5530_SCLK_LOW(Pinfo);
		__nop();
		__nop();
	}

	return reValue;
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
u32 CS5530_GetADData(CS5530_Pindef *Pinfo)
{
	u32 reValue=0;
	
	CS5530_CS_LOW(Pinfo);
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	//Delay_us(100);
	if(CS5530_SDO_STATE(Pinfo) == 0)
	{
		CS5530_WriteOneByte(Pinfo, CS5530_CONTINUOUS_ON);		///*��������ת��ģʽ*/
		reValue = ((u32)CS5530_ReadOneByte(Pinfo)<<24)
				+((uint32_t)CS5530_ReadOneByte(Pinfo)<<16)
				+((uint32_t)CS5530_ReadOneByte(Pinfo)<<8)
				+(uint32_t)CS5530_ReadOneByte(Pinfo);
	}
	else
		reValue = 0xFFFFFFFF;
	__nop();
	__nop();
	__nop();
	__nop();
	//Delay_us(100);
	CS5530_CS_HIGH(Pinfo);

	return reValue;
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
u32 CS5530_ReadRegister(CS5530_Pindef *Pinfo, u8 command)
{
	volatile unsigned long temp=0x00;
	
	CS5530_WriteOneByte(Pinfo, command);
	temp = (CS5530_ReadOneByte(Pinfo)<<24)
			+(CS5530_ReadOneByte(Pinfo)<<16)
			+(CS5530_ReadOneByte(Pinfo)<<8)
			+CS5530_ReadOneByte(Pinfo);
	
	return temp;
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void CS5530_ClearBuf(CS5530_Pindef *Pinfo)
{
//	uint8_t i;

//	__disable_irq();
//	pInfo->continuousOFCount = 0;
//	for(i=0; i<pInfo->dataBufSize; i++)
//		pInfo->pDataBuf[i] = 0xFFFFFFFF;
//	pInfo->dataPtr = 0;
//	__enable_irq();
}
/*******************************************************************************
* ������			:	CS5530_PowerUp
* ��������		:	CS5530�ϵ缰��ʼ��
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void CS5530_PowerUp(CS5530_Pindef *Pinfo)
{
	u8 num=0;
	CS5530_CS_LOW(Pinfo);
	CS5530_Delay(50000);CS5530_Delay(50000);CS5530_Delay(50000);CS5530_Delay(50000);CS5530_Delay(50000);CS5530_Delay(50000);
	//1)��ϵͳ����ADC���͸�λ����
	//1.1��************д��SYNC1
	for(num=0;num<15;num++)
	CS5530_WriteOneByte(Pinfo, CS5530_SYNC1);
	CS5530_Delay(50);
	//1.2��************д��SYNC0
	CS5530_WriteOneByte(Pinfo, CS5530_SYNC0);
	CS5530_Delay(50);
	//2)д���üĴ��� д��CS5530��λ���� RSΪ1
	CS5530_WriteRegister(Pinfo,CS5530_WRITE_CONFIG,CS5530_CONF_SYSTEM_RESET);
	CS5530_Delay(500);
	CS5530_WriteOneByte(Pinfo, CS5530_NULL_BYTE);
	CS5530_Delay(50);
	CS5530_WriteRegister(Pinfo,CS5530_WRITE_CONFIG,CS5530_CONF_NORMAL_MODE);
	CS5530_Delay(50);
	
	//���RV�Ƿ�Ϊ1(��λ�� ����Ϊ1),�����Ϊ1�ټ�����ȡ���üĴ���		
		CS5530_Delay(10);
		do
		{ 
			CS5530_Status = CS5530_ReadRegister(Pinfo, CS5530_READ_CONFIG);
		}
		while((CS5530_Status & CS5530_CONF_RESET_STATUS) != 0);
	
//	CS5530_Delay(50);		
//	CS5530_WriteRegister(Pinfo,CS5530_WRITE_CONFIG,CS5530_CONF_NORMAL_MODE);
//	CS5530_Delay(50);
	
//	while(CS5530_ReadRegister(Pinfo,CS5530_READ_CONFIG)==0x00000000);
//	while(CS5530_ReadRegister(Pinfo,CS5530_READ_OFFSET)==0x00000000);
//	while(CS5530_ReadRegister(Pinfo,CS5530_READ_GAIN)==0x01000000);
		
//	CS5530_Status=0xFFFFFFFF;
	CS5530_Status=CS5530_ReadRegister(Pinfo,CS5530_READ_CONFIG);			//���üĴ���
//	CS5530_Status=0xFFFFFFFF;
	CS5530_Status=CS5530_ReadRegister(Pinfo,CS5530_READ_OFFSET);			//ƫ�ƼĴ���
//	CS5530_Status=0xFFFFFFFF;
	CS5530_Status=CS5530_ReadRegister(Pinfo,CS5530_READ_GAIN);				//����Ĵ���
		
		
//	CS5530_Status=0xFFFFFFFF;
		
	CS5530_Status=CS5530_Status|CS5530_CONF_SHORT_INPUTS;								//����Ϊ�����·

	CS5530_Status=CS5530_ReadRegister(Pinfo,CS5530_READ_CONFIG);				//����Ĵ���

	CS5530_Status=CS5530_Status|0x2000;																	//7.5sps
	
//	CS5530_Status=CS5530_Status|0x1800;																	//15sps
	
//	CS5530_Status=CS5530_Status|0x0800;																	//60sps
	
////	CS5530_Status=CS5530_Status|0x4000;																	//3840sps
	
//	CS5530_Status=CS5530_Status|0x6000;																	//240sps
	
	CS5530_WriteCommand(Pinfo,CS5530_SYSTEM_OFFSET_CAL);								//ִ��ƫ��У׼
	
	CS5530_WriteCommand(Pinfo,CS5530_SYSTEM_GAIN_CAL);									//ִ��ϵͳ����У׼	
	
	CS5530_Status=CS5530_Status|CS5530_CONF_UNIPOLAR_MODE;							//����ѡ�񵥼���ģʽ
	
	CS5530_WriteRegister(Pinfo,CS5530_WRITE_CONFIG,CS5530_Status);			//���üĴ���
	
	CS5530_Status=CS5530_ReadRegister(Pinfo,CS5530_READ_CONFIG);				//���üĴ���
	
	CS5530_Status=CS5530_Status&(~CS5530_CONF_SHORT_INPUTS);						//��������,ͨ�����벻�̽�
	
	CS5530_WriteRegister(Pinfo,CS5530_WRITE_CONFIG,CS5530_Status);			//д���üĴ���
	
	CS5530_WriteRegister(Pinfo,CS5530_WRITE_GAIN,0x00FFFFFF);						//д����Ĵ���
	
//	CS5530_WriteRegister(Pinfo,CS5530_WRITE_GAIN,0x00100000);					//д����Ĵ���
	
	CS5530_WriteCommand(Pinfo,CS5530_START_CONTINUOUS);									//ִ������ת��
	
	CS5530_Delay(500);
	CS5530_CS_HIGH(Pinfo);
}
/*******************************************************************************
*������			:	CS5530_ReadData
*��������		:	��ȡADֵ���������0xFFFFFFFF,��δ��ȡ��24λADֵ
*����				: 
*����ֵ			:	��
*******************************************************************************/
u32	CS5530_ReadData(CS5530_Pindef *Pinfo)
{
	u32 ADC_Value=0xFFFFFFFF;
	
//	CS5530_CS_HIGH(Pinfo);
	CS5530_CS_LOW(Pinfo);		
	if(CS5530_SDO_STATE(Pinfo) == 0)
	{			
		ADC_Value=CS5530_GetADData(Pinfo)>>8;
	}
	return ADC_Value;

}







/************************************************************************************/
