#ifdef ADC_TEST
#include "ADC_TEST.H"


#define ADC_TEST_BUFFERSIZE 128

u8 ADC_TEST_BUFFER[ADC_TEST_BUFFERSIZE]={0};
u8 nump=0;
u16 tema;
u16 tempr=30;
u16 ADCD=30;
u16 DACD=30;

void ADC1_GPIO_Config(void);
void ADC1_Mode_Config(void);
void ADC1_Init(void);
/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void ADC_TEST_Configuration(void)
{
	TIM_Configuration(TIM3,7200,10000);	//��ʱʱ���趨
	PWM_Configuration(TIM2,7200,200,10);
	USART_DMA_Configuration(USART1,115200,1,1,(u32*)ADC_TEST_BUFFER,(u32*)ADC_TEST_BUFFER,ADC_TEST_BUFFERSIZE);	//USART_DMA����
	
	
//	ADC1_DiscConfiguration((u32*)&ADCD,1,ADC_Channel_16,1,1,ADC_SampleTime_239Cycles5);				//ADC1����ͨ��������
	
//	ADC_TempSensorVrefintCmd(ENABLE);
//	ADC1_Cmd(ENABLE);					//����ADC1ת��
//	DAC_Configuration();
//	DAC_DMA_Configuration((u32*)&DACD);
//	ADC1_Cmd(ENABLE);					//����ADC1ת��
	
	ADC_TempSensorConfiguration((u32*)&ADCD);		//STM32�ڲ��¶ȴ���������
	
//		ADC1_Init();
//	GPIO_Configuration(GPIOE,GPIO_Pin_All,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
	GPIO_Configuration(GPIOA,GPIO_Pin_3,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
	SysTick_Configuration(10000);	//ϵͳ���ʱ������72MHz,��λΪuS
	
}
/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void ADC_TEST_Server(void)
{
	u32 num=0;
	float Temperature=0.0; 
	num=USART_RX_FlagClear(USART1);						//�������DMA����ȫ���жϱ�־
	if(num!=0)
	{
		USART_DMAPrintf(USART1,"���յ������ݸ���:%d;���յ�������:%s\n",num,ADC_TEST_BUFFERSIZE);
		tema=0;
	}
	else if(!USART_TX_DMAFlagClear(USART1))
	{
		memset(ADC_TEST_BUFFER,0,ADC_TEST_BUFFERSIZE);			//��ʼ������
		Temperature=Get_ADC_Temperature(ADCD);										//��ȡ�ڲ��¶ȴ������¶�
		
		if(DACD<=4095)
		{		
//			USART_DMASend(USART1,"����DAC���ֵ:%d,ADC��ȡֵ%d\n",DACD,ADCD);
//			USART_DMASend(USART1,"%d\n",ADCD);
			USART_DMAPrintf(USART1,"��ǰSTM32�ڲ��¶�Ϊ��%4.1f��\n",Temperature);
//			USART_DMASend(USART1,"%d\n",DMA1_Channel1->CMAR);
			
			DACD++;
		}
		else
			DACD=0;
	}
	nump++;
	if(nump<=20)
	{
//		GPIO_Write(GPIOE,0XAAAA);
//		GPIO_SetBits(GPIOA,GPIO_Pin_3);
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);
//		nump++;
	}
	else
	{
//		GPIO_Write(GPIOE,0X5555);
		GPIO_SetBits(GPIOA,GPIO_Pin_3);
//		GPIO_ResetBits(GPIOA,GPIO_Pin_3);
		if(nump>40)
		{
			nump=0;
		}
		
	}
}


//////////////////////////////////////////////////////////
/*
����ѧϰSTM32���ڲ��Դ�12λADC��

1���ص㣺

��1��12λ�ֱ��ʣ����ʱ������Ϊ14����ʱ�����ڿɵ����ֱ���Ե���Ϊ14��20��26��41��54��68��252����˵�ʱ��Ϊ14MHzʱ�����ת��ʱ��Ϊ1us��

��2�������ѹΪ2.4V��3.6V��ע��stm32����͹����ѹ������2V������ʹ��ADCʱ�򣬱���ﵽ2.4V���ϣ�

��3�������ѹ��Χ��Vref- 

��4����С������λΪ��LSB=Vref+/4096mV;

��5��Ϊ��αȽ���AD��

2������ADת���ķ�����������һ���ǳ��õ��жϣ���һ����DMA�������˵��DMAģʽ�µ�Ч��Ҫ�ߣ����������о�DMA�µ�ADC��

3��ʵ��Ŀ�꣺ͨ��ADC1��11ͨ���ɼ��ⲿ��ѹ��Ȼ����DMA���͵����棬Ȼ��ͨ�����ڷ�����PC�ϡ�

4��ʵ�����


//#include"stm32f10x.h"

//#include"adc.h"

*/ 

 /******************

  function name��ADC1_Init()

  discribe ��ADC1��ʼ���������������ú�����ģʽѡ����

  input��null

  output:null

 *******************/

  void ADC1_Init(void)

{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC1_GPIO_Config();

	ADC1_Mode_Config(); 

}

 

/******************

  function name��ADC1_GPIO_Config()

  discribe ��ADC1 config function

     (1)enable ADC,GPIO,DMA's clock,

(2)ADC1's IO is PC1,Mode is analog input(AIN)

  input��null

  output:null

  return: null

 *******************/

 void ADC1_GPIO_Config(void)

 {
GPIO_InitTypeDef GPIO_InitStructure;					//GPIO�ṹ��
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);		//ʹ��GPIOAʱ��
	 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;							//GPIO_Pin_x
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;					//ģ������
	GPIO_Init(GPIOA,&GPIO_InitStructure);	   

 }

 

/******************

  function name��ADC1_Mode_Config()

  discribe ��ADC1  mode configuration

     (1)config DMA

(2)config ADC1

  input��null

  output:null

  return:null

 *******************/

 void ADC1_Mode_Config(void)

 {

DMA_InitTypeDef DMA_InitStructure;

ADC_InitTypeDef ADC_InitStructure;

 

/*******config DMA channel 1 ********/

DMA_DeInit(DMA1_Channel1);   //reset DMA channel register

 //define the peripheral base address

DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)&ADC1->DR; 

//define the memory base address

DMA_InitStructure.DMA_MemoryBaseAddr=(u32)&tempr;

//choose the peripheral is the destination or source

DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;

//set the DMA buffer size �����С

DMA_InitStructure.DMA_BufferSize=1; //16bits

//set the peripheral address register is incremented or not �������

DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;

//set the memory address register is incremented or not 

DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Disable;

//set the peripheral data size as 16bits(halfword)

DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;

//set the memory data size 

DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;

//set DMA mode

DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;

//set the channel priority 

DMA_InitStructure.DMA_Priority=DMA_Priority_High;

//enable the DMA memory to memory transfer

DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;

 

DMA_Init(DMA1_Channel1,&DMA_InitStructure);

/*****enable channel 1******/

DMA_Cmd(DMA1_Channel1,ENABLE);

 

/*****independent scanmode none external ,right align,***/

ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;

ADC_InitStructure.ADC_ScanConvMode=ENABLE;

ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;

ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;

ADC_InitStructure.ADC_NbrOfChannel=1;

ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;

ADC_Init(ADC1, &ADC_InitStructure);
 

 //set the ADC clock

RCC_ADCCLKConfig(RCC_PCLK2_Div8);//PCLK2/8=9Mhz

 //set the AD sampling time  55.5����������

ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_55Cycles5);

 //enable the ADC begins DMA transfer

ADC_DMACmd(ADC1,ENABLE);

//enable the ADC1

ADC_Cmd(ADC1,ENABLE);

//enable ADC1 reset the calibration register

ADC_ResetCalibration(ADC1);

//when reset is ok ,it returns 0;if not ,wait

while(ADC_GetResetCalibrationStatus(ADC1));

 

//begin ADC1 calibration

ADC_StartCalibration(ADC1);

//wait untill the calibration is finished when it will return 0

while(ADC_GetCalibrationStatus(ADC1));

//software starts the conversation 

ADC_SoftwareStartConvCmd(ADC1,ENABLE);

}








#endif