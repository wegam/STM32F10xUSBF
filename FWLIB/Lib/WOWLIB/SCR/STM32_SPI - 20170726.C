/******************************** User_library *********************************
* �ļ��� 	: STM32_SPI.H
* ����   	: wegam@sina.com
* �汾   	: V
* ����   	: 2016/01/01
* ˵��   	: 
********************************************************************************
SPI����
1��ʹ��ǰ��Ҫ���ýṹ����� SPI_ConfTypeDef
SPI_ConfTypeDef����GPIO�ṹ�壨4��GPIO�ܽţ���SPI�ṹ��,DMA�ṹ�壬���ջ����������ͻ�������DMAʹ������־

SPI��Ҫ���û������ݣ�
1)GPIO
2)SPI
��ʹ��DMAʱ��
3)DMA


*******************************************************************************/
#include "STM32_SPI.H"
#include "stm32f10x_dma.h"
#include "stm32f10x_exti.h"
#include "STM32_EXTI.H"

#include "STM32_WOW.H"
//#include "STM32F10x_BitBand.H"

char *SPI_BUFFER;
u32	SPI_BUFFERSIZE;
//#define	SPI_BUFFERSIZE	4

//char SPI_TX_Buffer[4]={0X9F,0X9F,0X9F,0X9F};
//char SPI_RX_Buffer[4]={0};
/*******************************************************************************
*������		:	SPI_Configuration
*��������	:	��������
*����			:	MemoryAddr--�ڴ��ַ,
						BufferSize--�����С
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
void SPI_BASIC_Configuration(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)							//SPI����ʹ�÷�ʽ����
{
	//1)**********SPI�ܽ����ü���ʼ��
	SPI_PinSet(SPIx,SPI_InitStruct);																//SPI�ܽ����ó�ʼ��
	//2)**********�ж�SPI�����Ƿ���SPI_NSSӲ��ʹ�����	
	//ʹ��SPIx
	SPI_Cmd(SPIx, ENABLE); 
}
void SPI_Server(void)
{
	WOW_Server(); 
}
/*******************************************************************************
*������		:	SPI_DMA_Configuration
*��������	:	��������˵��
*����			: 
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
void SPI_DMA_Configuration(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct,u32* SPI_TX_Buffer,u32* SPI_RX_Buffer,u32 SPI_BUFFERSIZE)		//SPI_DMA��ʽ����
{
/**-----------------------------------------------------------------------------------------------------
	********SPI_DMA��ͨ�Ź���********
	�� ���������ַ
	�� ���ô洢����ַ
	�� ���ô���������
	�� ����ͨ����������Ϣ
	�� ʹ��DMAͨ������������
	
	�� ����ʱ����ÿ��TXE������Ϊ��1��ʱ����DMA����DMA��������д������SPI_DR�Ĵ�����TXE��־��˶��������
	�� ����ʱ����ÿ��RXNE������Ϊ��1��ʱ����DMA����DMA���������SPI_DR�Ĵ����������ݣ�RXNE��־��˶��������
-----------------------------------------------------------------------------------------------------**/
	//1)**********������ؽṹ��
	DMA_InitTypeDef	DMA_Initstructure;
//	NVIC_InitTypeDef NVIC_Initstructure;
	
//	u32 *SPI_RX_Buffer;
//	u32 *SPI_TX_Buffer;
//	u32 SPI_BUFFERSIZE=5;

	DMA_Channel_TypeDef* DMAx_Channeltx=0;			//DMA����ͨ�������ź�---��DMA���ڷ����������ʱ���ᷢ��DMA�ж�
	DMA_Channel_TypeDef* DMAx_Channelrx=0;			//DMA����ͨ�������ź�---DMA���ڽ����ɴ��ڷ����жϣ���˴˴�����ͨ���жϲ�ʹ��
//	u8 DMAx_Channelx_IRQChannel=0;							//DMA�ж�Դ
	u8 Conf_Flag=0;															//��Ҫ���ñ�־�����SPIx�Ϸ�����Conf_Flag==1��Ȼ�������һ��DMA������
//	u32 DMAx_FLAG_GLtx=0;												//DMA���ڷ����ж�ȫ�ֱ���			
//	u32 DMAx_FLAG_GLrx=0;												//DMA���ڽ����ж�ȫ�ֱ���
	
//	SPI_BUFFERSIZE=BufferSize;
	
//	assert_param(IS_SPI_ALL_PERIPH(SPI5));
	
	//2)**********SPI�ܽ����ü���ʼ��
	SPI_PinSet(SPIx,SPI_InitStruct);																		//SPI�ܽ����ó�ʼ��
	
	//3)**********SPIͨ��ѡ��
	if(SPIx==SPI1)
	{
		DMAx_Channeltx=DMA1_Channel3;
		DMAx_Channelrx=DMA1_Channel2;
//		DMAx_Channelx_IRQChannel=DMA1_Channel3_IRQChannel;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
		Conf_Flag=1;																			//��Ҫ���ñ�־�����SPIx�Ϸ�����Conf_Flag==1��Ȼ�������һ��DMA������
	}
	else if(SPIx==SPI2)
	{
		DMAx_Channeltx=DMA1_Channel5;
		DMAx_Channelrx=DMA1_Channel4;
//		DMAx_Channelx_IRQChannel=DMA1_Channel5_IRQChannel;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
		Conf_Flag=1;																			//��Ҫ���ñ�־�����SPIx�Ϸ�����Conf_Flag==1��Ȼ�������һ��DMA������		
	}
	else if(SPIx==SPI3)
	{
		DMAx_Channeltx=DMA2_Channel2;
		DMAx_Channelrx=DMA2_Channel1;
//		DMAx_Channelx_IRQChannel=DMA2_Channel2_IRQChannel;
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
		Conf_Flag=1;																			//��Ҫ���ñ�־�����SPIx�Ϸ�����Conf_Flag==1��Ȼ�������һ��DMA������
	}
	else
	{
		Conf_Flag=0;																			//��Ҫ���ñ�־�����SPIx�Ϸ�����Conf_Flag==1��Ȼ�������һ��DMA������
	}
	
	//4)**********SPI_DMA����
	if(Conf_Flag==1)																		//��Ҫ���ñ�־�����SPIx�Ϸ�����Conf_Flag==1��Ȼ�������һ��DMA������
	{
		//5)**********DMA���ͳ�ʼ����������ΪDMA��Ŀ�Ķ�
		DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&SPIx->DR);						//DMA����Դ��ַ
		DMA_Initstructure.DMA_MemoryBaseAddr     = (u32)SPI_TX_Buffer;							//DMA�����ڴ��ַ
		DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralDST;											//DMA_DIR_PeripheralDST��������ΪDMA��Ŀ�Ķˣ���DMA_DIR_PeripheralSRC��������Ϊ���ݴ������Դ��
		DMA_Initstructure.DMA_BufferSize = SPI_BUFFERSIZE; 											//ָ��DMAͨ����DMA����Ĵ�С
	//	DMA_Initstructure.DMA_BufferSize = sizeof(MemoryAddr[0]); 						//ָ��DMAͨ����DMA����Ĵ�С
		DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable�������ַ�Ĵ�����������DMA_PeripheralInc_Disable�������ַ�Ĵ������䣩��
		DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable���ڴ��ַ�Ĵ�����������DMA_MemoryInc_Disable���ڴ��ַ�Ĵ������䣩
		DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݿ���--DMA_PeripheralDataSize_Byte�����ݿ���Ϊ8λ����DMA_PeripheralDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_PeripheralDataSize_Word�����ݿ���Ϊ32λ��
		DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//�ڴ����ݿ���--DMA_MemoryDataSize_Byte�����ݿ���Ϊ8λ����DMA_MemoryDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_MemoryDataSize_Word�����ݿ���Ϊ32λ��
		DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;														//DMA����ģʽ--DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular����ͣ�ش��ͣ�
		DMA_Initstructure.DMA_Priority = DMA_Priority_High; 										//DMAͨ����ת�����ȼ�--DMA_Priority_VeryHigh���ǳ��ߣ�DMA_Priority_High����)��DMA_Priority_Medium���У���DMA_Priority_Low���ͣ�
		DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;														//DMAͨ�����ڴ浽�ڴ洫��--DMA_M2M_Enable(����Ϊ�ڴ浽�ڴ洫��)��DMA_M2M_Disable�����ڴ浽�ڴ洫�䣩
		DMA_Init(DMAx_Channeltx,&DMA_Initstructure);														//��ʼ��DMA

		//6)**********DMA���ճ�ʼ����������ΪDMA��Դ��
		DMA_Initstructure.DMA_PeripheralBaseAddr =  (u32)(&SPIx->DR);					//DMA����Դ��ַ
		DMA_Initstructure.DMA_MemoryBaseAddr     = 	(u32)SPI_RX_Buffer;						//DMA�����ڴ��ַ
		DMA_Initstructure.DMA_DIR = DMA_DIR_PeripheralSRC;											//DMA_DIR_PeripheralDST��������ΪDMA��Ŀ�Ķˣ���DMA_DIR_PeripheralSRC��������Ϊ���ݴ������Դ��
		DMA_Initstructure.DMA_BufferSize = SPI_BUFFERSIZE; 													//ָ��DMAͨ����DMA����Ĵ�С
	//	DMA_Initstructure.DMA_BufferSize = sizeof(MemoryAddr); 								//ָ��DMAͨ����DMA����Ĵ�С
		DMA_Initstructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable�������ַ�Ĵ�����������DMA_PeripheralInc_Disable�������ַ�Ĵ������䣩��
		DMA_Initstructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable���ڴ��ַ�Ĵ�����������DMA_MemoryInc_Disable���ڴ��ַ�Ĵ������䣩
		DMA_Initstructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݿ���--DMA_PeripheralDataSize_Byte�����ݿ���Ϊ8λ����DMA_PeripheralDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_PeripheralDataSize_Word�����ݿ���Ϊ32λ��
		DMA_Initstructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//�ڴ����ݿ���--DMA_MemoryDataSize_Byte�����ݿ���Ϊ8λ����DMA_MemoryDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_MemoryDataSize_Word�����ݿ���Ϊ32λ��
		DMA_Initstructure.DMA_Mode = DMA_Mode_Normal;														//DMA����ģʽ--DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular����ͣ�ش��ͣ�
		DMA_Initstructure.DMA_Priority = DMA_Priority_High; 										//DMAͨ����ת�����ȼ�--DMA_Priority_VeryHigh���ǳ��ߣ�DMA_Priority_High����)��DMA_Priority_Medium���У���DMA_Priority_Low���ͣ�
		DMA_Initstructure.DMA_M2M = DMA_M2M_Disable;														//DMAͨ�����ڴ浽�ڴ洫��--DMA_M2M_Enable(����Ϊ�ڴ浽�ڴ洫��)��DMA_M2M_Disable�����ڴ浽�ڴ洫�䣩
		DMA_Init(DMAx_Channelrx,&DMA_Initstructure);														//��ʼ��DMA
		
//		//7)**********DMAͨ���жϳ�ʼ��---��ΪDMA�����ж�----DMA��������ж�
//		NVIC_Initstructure.NVIC_IRQChannel = DMA1_Channel4_IRQChannel;       		//ͨ������Ϊ����1�ж�
//		NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority=1;
//		NVIC_Initstructure.NVIC_IRQChannelSubPriority = 1; 			//�ж���Ӧ���ȼ�0	
//		NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;        									//���ж�
//		NVIC_Init(&NVIC_Initstructure);
//		
//		DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);	//ʹ�ܻ���ʧ��ָ����ͨ��x�ж�
//		DMA_Cmd(DMA1_Channel4, ENABLE);
			
		SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Tx, ENABLE);
		SPI_I2S_DMACmd(SPIx, SPI_I2S_DMAReq_Rx, ENABLE);
		
		
		
		
		//ʹ��SPIx
		SPI_Cmd(SPIx, ENABLE);
		
		//9.2)**********ʹ�����DMAͨ����������ж�
//		DMA_ITConfig(DMAx_Channeltx,DMA_IT_TC, ENABLE);
			/* ����DMA1ͨ��5*/
		DMA_Cmd(DMAx_Channelrx,DISABLE);	
		DMA_Cmd(DMAx_Channeltx,DISABLE);
		

		//9.2)**********ʹ�����DMAͨ����������ж�
//		DMA_ITConfig(DMAx_Channeltx,DMA_IT_TC, ENABLE);
	}
		
//	NVIC_Initstructure.NVIC_IRQChannel = SPI_I2S_IT_TXE;       		//ͨ������Ϊ����1�ж�
//	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority=1;
//	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 1; 			//�ж���Ӧ���ȼ�0	
//	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;        									//���ж�
//	NVIC_Init(&NVIC_Initstructure);
//	
//	SPI_I2S_ITConfig(SPIx,SPI_I2S_IT_TXE,ENABLE);
	
	//ʹ��SPIx
	SPI_Cmd(SPIx, DISABLE);
}
/*******************************************************************************
*������		:	SPI_PinSet
*��������	:	SPI�ܽ����ó�ʼ��
*����			: 
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
void SPI_PinSet(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)													//SPI�ܽ����ü���ʼ��
{
	//1)**********������ؽṹ��
//	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	//2)**********���GPIO����
	if(SPIx==SPI1)
	{
		//2.1)**********��SPIʱ��	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);		
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		if(SPI_InitStruct->SPI_NSS == SPI_NSS_Soft)			//���SPI_NSSΪSPI_NSS_Soft���������Ʒ�ʽ��
		{
			
			//2.2)**********SPI_NSS_Hard����
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		}
	}
	else if(SPIx==SPI2)
	{
		//2.2)**********��SPIʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);			
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
		if(SPI_InitStruct->SPI_NSS == SPI_NSS_Soft)			//���SPI_NSSΪSPI_NSS_Soft���������Ʒ�ʽ��
		{
			//2.2)**********SPI_NSS����
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
			GPIO_Init(GPIOB, &GPIO_InitStructure);
		}
	}
		else if(SPIx==SPI3)
	{
		//2.2)**********��SPIʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3 ,ENABLE);			
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		if(SPI_InitStruct->SPI_NSS == SPI_NSS_Soft)			//���SPI_NSSΪSPI_NSS_Soft���������Ʒ�ʽ��
		{			
			//2.2)**********SPI_NSS����		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		}
		else	
		{			
			//2.2)**********SPI_NSS����
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		}			
	}
	//3)**********SPI����ѡ��
//	SPI_InitStruct->SPI_Direction = SPI_Direction_2Lines_FullDuplex;				//���÷���				��2��ȫ˫����2��ֻ���ա�һ�߷��͡�һ�߽��գ�
//	SPI_InitStruct->SPI_Mode = SPI_Mode_Master;															//ģʽ         	���ӻ����豸��
//	SPI_InitStruct->SPI_DataSize = SPI_DataSize_8b;													//����         	��8��16λ��
//	SPI_InitStruct->SPI_CPOL = SPI_CPOL_High;																//ʱ�Ӽ���     	���ͻ�ߣ�
//	SPI_InitStruct->SPI_CPHA = SPI_CPHA_2Edge;															//ʱ����λ     	����һ����ڶ��������أ�
////	SPI_InitStructure->SPI_NSS = SPI_NSS_Soft;																	//Ƭѡ��ʽ     	��Ӳ����������ʽ��
//	SPI_InitStruct->SPI_NSS = SPI_NSS_Hard;																	//Ƭѡ��ʽ     	��Ӳ����������ʽ��
//	SPI_InitStruct->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;				//������Ԥ��Ƶ 	����2---256��Ƶ��
//	SPI_InitStruct->SPI_FirstBit = SPI_FirstBit_MSB;												//���ȷ��͵�λ 	�����λ���������λ���ȣ�
//	SPI_InitStruct->SPI_CRCPolynomial = 7;																	//����crc����ʽ	�����֣���7
	SPI_Init(SPIx, SPI_InitStruct);	
	
	//3)**********ʹ��SPIx_NESSΪ�����ģʽ
	if((SPIx->CR1&0X0200)!=SPI_NSS_Soft)						//���������ģʽ�µ�Ƭѡ��ʽΪӲ����SPI_NSS_Hard����ʽ���˴�����򿪣�����NSS���ź�
	{
		SPI_SSOutputCmd(SPIx, ENABLE);
	}

}
/*******************************************************************************
*������		: function
*��������	:	SPI_DMA���ͺ���
*����			: 
*���			:	��
*����ֵ		:	��
*����			:	USART_DMASend(USART2,"����ENG=%d\n",num);
*�ر�˵��	:	��DMA������ɺ���Ҫ�ͷŶ�̬�ռ䣬free(USART_BUFFER);
					:	USART_BUFFER������STM32_USART.H
*******************************************************************************/
int SPI_DMAPrintf(SPI_TypeDef* SPIx,const char *format,...)		//SPI_DMA���ͺ���----��ߵ�ʡ�Ժž��ǿɱ����
{
	
//		va_list ap; 										//VA_LIST ����C�����н����������һ��꣬����ͷ�ļ���#include <stdarg.h>,���ڻ�ȡ��ȷ�������Ĳ���
//    static char string[ 256 ];			//�������飬
//    va_start( ap, format );
//    vsprintf( string , format, ap );    
//    va_end( ap );
//	char *SPI_BUFFER;
	//1)**********��ȡ���ݿ���
	u16 num=strlen((const char*)format);		//��ȡ���ݿ���
	//2)**********���建������С����
	unsigned int BufferSize;
	//3)**********argsΪ�����һ��ָ��ɱ�����ı�����va_list�Լ��±�Ҫ�õ���va_start,va_end�������ڶ��壬�ɱ���������б���Ҫ�õ��꣬ ��stdarg.hͷ�ļ��ж���
	va_list args;                                        
	//4)**********���붯̬�ռ�
	SPI_BUFFER = (char*)malloc(sizeof(char) * num);	
	//5)**********��ʼ��args�ĺ�����ʹ��ָ��ɱ�����ĵ�һ��������format�ǿɱ������ǰһ������
	va_start(args, format);
	//6)**********��������·��������ִ��ĳ���(��ȥ\0),����������ظ�ֵ
	BufferSize = vsprintf(SPI_BUFFER, format, args);
	//7)**********�����ɱ�����Ļ�ȡ
	va_end(args);                                      		
	//8)**********���ȷ��ͻ�������С�����ݸ���������������ַ����DMA��������
	//8)**********DMA������ɺ�ע��Ӧ���ͷŻ�������free(USART_BUFFER);
	if(SPIx==SPI1)
	{
		DMA1_Channel3->CNDTR =BufferSize;										//�趨�����ͻ�������С
		DMA1_Channel3->CMAR =(u32)SPI_BUFFER;							//���ͻ�����	
		DMA_Cmd(DMA1_Channel3,ENABLE);											//DMA���Ϳ���3
	}
	else if(SPIx==SPI2)
	{
		DMA1_Channel5->CNDTR =BufferSize;										//�趨�����ͻ�������С
		DMA1_Channel5->CMAR =(u32)SPI_BUFFER;							//���ͻ�����	
		DMA_Cmd(DMA1_Channel5,ENABLE);											//DMA���Ϳ���3
	}
	else if(SPIx==SPI3)
	{
		DMA2_Channel2->CNDTR =BufferSize;										//�趨�����ͻ�������С
		DMA2_Channel2->CMAR =(u32)SPI_BUFFER;							//���ͻ�����	
		DMA_Cmd(DMA2_Channel2,ENABLE);											//DMA���Ϳ���3
	}
	return BufferSize;
}
/*******************************************************************************
*������		: function
*��������	:	SPI_DMA���ͺ���
*����			: 
*���			:	��
*����ֵ		:	��
*����			:	USART_DMASend(USART2,"����ENG=%d\n",num);
*�ر�˵��	:	��DMA������ɺ���Ҫ�ͷŶ�̬�ռ䣬free(USART_BUFFER);
					:	USART_BUFFER������STM32_USART.H
*******************************************************************************/
int SPI_DMA_BufferWrite(SPI_TypeDef* SPIx,u32 *SPI_TX_Buffer,u16 BufferSize)		//SPI_DMA���ͺ���----��ߵ�ʡ�Ժž��ǿɱ����
{              		
	//8)**********���ȷ��ͻ�������С�����ݸ���������������ַ����DMA��������
	//8)**********DMA������ɺ�ע��Ӧ���ͷŻ�������free(USART_BUFFER);
	if(SPIx==SPI1)
	{
		DMA1_Channel3->CNDTR =BufferSize;										//�趨�����ͻ�������С
		DMA1_Channel3->CMAR =(u32)SPI_TX_Buffer;						//���ͻ�����	
//		DMA1_Channel2->CNDTR =BufferSize;										//�趨�����ͻ�������С
//		DMA1_Channel2->CMAR =(u32)SPI_RX_Buffer;						//���ͻ�����	
//		DMA_Cmd(DMA1_Channel2,ENABLE);
		DMA_Cmd(DMA1_Channel3,ENABLE);											//DMA���Ϳ���3
	}
	else if(SPIx==SPI2)
	{
		DMA1_Channel5->CNDTR =BufferSize;										//�趨�����ͻ�������С
		DMA1_Channel5->CMAR =(u32)SPI_TX_Buffer;							//���ͻ�����	
		DMA_Cmd(DMA1_Channel5,ENABLE);											//DMA���Ϳ���3
	}
	else if(SPIx==SPI3)
	{
		DMA1_Channel2->CNDTR =BufferSize;										//�趨�����ͻ�������С
		DMA1_Channel2->CMAR =(u32)SPI_TX_Buffer;							//���ͻ�����	
		DMA_Cmd(DMA2_Channel2,ENABLE);											//DMA���Ϳ���3
	}
	return BufferSize;
}
/*******************************************************************************
*������		: function
*��������	:	SPI_DMA_BufferRead���ͺ���
*����			: 
*���			:	��
*����ֵ		:	��
*����			:	USART_DMASend(USART2,"����ENG=%d\n",num);
*�ر�˵��	:	��DMA������ɺ���Ҫ�ͷŶ�̬�ռ䣬free(USART_BUFFER);
					:	USART_BUFFER������STM32_USART.H
*******************************************************************************/
void SPI_DMA_BufferRead(u32 *ReadAddr)
{
	SPI_InitTypeDef  SPI_InitStructure;
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	/* Send "Read from Memory " instruction */
	SPI_I2S_SendData(SPI1,0X9F);
	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_RxOnly;
	SPI_Init(SPI1, &SPI_InitStructure);

	DMA1_Channel2->CNDTR =20;						//���ͻ�����
	DMA1_Channel2->CMAR =(u32)ReadAddr;						//���ͻ�����

	//SPI_FLASH_SendByte(0xff);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);

	while(DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
}
/*******************************************************************************
*������		:	SPI_RX_FlagClear
*��������	:	��������˵��
*����			: 
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
u32 SPI_RX_FlagClear(SPI_TypeDef* SPIx)											//SPI DMA���ü���ʼ��
{
	u32 num=0;
	if(SPIx==SPI1)
	{
	}
	else if(SPIx==SPI2)
	{
	}
	else if(SPIx==SPI3)
	{
	}
	else
	{
		num=0;
	}
	return num;
}
/*******************************************************************************
*������		:	SPI_TX_DMAFlagClear
*��������	:	���SPI_DMA��������жϱ�־
*����			: 
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
ErrorStatus SPI_TX_DMAFlagClear(SPI_TypeDef* SPIx)					//���SPI_DMA��������жϱ�־
{
	ErrorStatus status = ERROR;
	if(SPIx==SPI1)
	{
		if(DMA_GetITStatus(DMA1_IT_TC3))
		{
			DMA_ClearFlag(DMA1_IT_TC3);
			DMA_Cmd(DMA1_Channel3,DISABLE);			//DMA���͹ر�
			status = SUCCESS;
		}
		else
		{
			status = ERROR;
		}
	}
	else if(SPIx==SPI2)
	{
		if(DMA_GetITStatus(DMA1_IT_TC5))
		{
			DMA_ClearFlag(DMA1_IT_TC5);
			DMA_Cmd(DMA1_Channel5,DISABLE);			//DMA���͹ر�
			status = SUCCESS;
		}
		else
		{
			status = ERROR;
		}
	}
	else if(SPIx==SPI3)
	{
		if(DMA_GetITStatus(DMA2_IT_TC2))
		{
			DMA_ClearFlag(DMA2_IT_TC2);
			DMA_Cmd(DMA2_Channel2,DISABLE);			//DMA���͹ر�
			status = SUCCESS;
		}
		else
		{
			status = ERROR;
		}
	}
	else
	{
		status = ERROR;
	}
	return status;
}
/*******************************************************************************
*������		:	
*��������	:	
*����			: 
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
u8 SPI_SendByte(u8 byte)
{
  /* �ȴ�DR�Ĵ����� */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(SPI1, byte);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}

/*********************************************************************************************************************/
/*******************************************************************************
*������		:	
*��������	:	
*����			: 
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
//unsigned char SPI_Read(unsigned char* data,unsigned char bytesNumber)
// {
//     unsigned char byte = 0;
//     for(byte = 0; byte < bytesNumber; byte++)
//     {
//				SPI_Buffer_Tx[byte]=0;
//     }
//     GPIO_ResetBits(GPIOB,GPIO_Pin_12);
//     SPI2_ReceiveSendByte(bytesNumber);
//     GPIO_SetBits(GPIOB, GPIO_Pin_12);
//     for(byte = 0; byte < bytesNumber; byte++)
//     {
//				data[byte]=SPI_Buffer_Rx[byte];
//     }
//     return bytesNumber;
// }
/*******************************************************************************
*������		:	
*��������	:	
*����			: 
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
 void SPI_DMA_ReceiveSendByte(SPI_TypeDef* SPIx,u16 num)			//DMA�շ�����
 {
	SPI_Cmd(SPIx, ENABLE);
	SPI_DMA_Conf(SPIx,num);											//����SPI_DMA�����û����С�����DMA��־

	SPIx->DR ;             											//����ǰ��һ��SPI1->DR����֤���ջ�����Ϊ��

	while((SPIx->SR&1<<1)==0);										//TXE���ȴ����ͻ���Ϊ�� (Transmit buffer empty)
	SPI_DMA_Cmd(SPIx,ENABLE);										//SPI_DMAʹ�ܻ���ʧ��---����DMA

//     while( !DMA_GetFlagStatus(DMA1_FLAG_TC4));		//�ȴ��������
	while(SPI_DMA_TX_Flag(SPI1)==0);					//��ѯSPI_DMA������ɱ�־;0--δ��ɣ�1--���
	 
	SPI_DMA_Cmd(SPIx,DISABLE);										//SPI_DMAʹ�ܻ���ʧ��---�ر�DMA
	SPI_Cmd(SPIx, DISABLE);
 }
/*******************************************************************************
*������		:	SPI_DMA_Conf
*��������	:	����SPI_DMA�����û����С�����DMA��־
*����			: 
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
void SPI_DMA_Conf(SPI_TypeDef* SPIx,u16 num)		//����SPI_DMA�����û����С�����DMA��־
{
	if(SPIx==SPI1)
	{
		//1)**********���û����С
		DMA1_Channel3->CNDTR =num;										//�趨�����ͻ�������С
		DMA1_Channel2->CNDTR =num;										//�趨�����ջ�������С
		//2)**********���DMA��־
		DMA_ClearFlag(DMA1_FLAG_GL3|DMA1_FLAG_TC3|DMA1_FLAG_HT3|DMA1_FLAG_TE3);		//���DMA���ͱ�־
		DMA_ClearFlag(DMA1_FLAG_GL2|DMA1_FLAG_TC2|DMA1_FLAG_HT2|DMA1_FLAG_TE2);		//���DMA���ձ�־
	}
	else if(SPIx==SPI2)
	{
		//1)**********���û����С
		DMA1_Channel5->CNDTR =num;										//�趨�����ͻ�������С
		DMA1_Channel4->CNDTR =num;										//�趨�����ջ�������С
		//2)**********���DMA��־
		DMA_ClearFlag(DMA1_FLAG_GL5|DMA1_FLAG_TC5|DMA1_FLAG_HT5|DMA1_FLAG_TE5);		//���DMA���ͱ�־
		DMA_ClearFlag(DMA1_FLAG_GL4|DMA1_FLAG_TC4|DMA1_FLAG_HT4|DMA1_FLAG_TE4);		//���DMA���ձ�־
	}
	else if(SPIx==SPI3)
	{
		//1)**********���û����С
		DMA2_Channel2->CNDTR =num;										//�趨�����ͻ�������С
		DMA2_Channel1->CNDTR =num;										//�趨�����ջ�������С
		//2)**********���DMA��־
		DMA_ClearFlag(DMA2_FLAG_GL2|DMA2_FLAG_TC2|DMA2_FLAG_HT2|DMA2_FLAG_TE2);		//���DMA���ͱ�־
		DMA_ClearFlag(DMA2_FLAG_GL1|DMA2_FLAG_TC1|DMA2_FLAG_HT1|DMA2_FLAG_TE1);		//���DMA���ձ�־
	}
}
/*******************************************************************************
*������		:	function
*��������	:	SPI_DMAʹ�ܻ���ʧ��
*����			: 
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
void SPI_DMA_Cmd(SPI_TypeDef* SPIx,FunctionalState NewState)		//SPI_DMAʹ�ܻ���ʧ��
{
	//1)ʹ��SPI_DMAͨ��
	if (NewState != DISABLE)
	{
		if(SPIx==SPI1)
		{
			DMA_Cmd(DMA1_Channel3, ENABLE);
			DMA_Cmd(DMA1_Channel2, ENABLE);
		}
		else if(SPIx==SPI2)
		{
			DMA_Cmd(DMA1_Channel5, ENABLE);
			DMA_Cmd(DMA1_Channel4, ENABLE);
		}
		else if(SPIx==SPI3)
		{
			DMA_Cmd(DMA2_Channel2, ENABLE);
			DMA_Cmd(DMA2_Channel1, ENABLE);
		}
	}
	//2)�ر�SPI_DMAͨ��
	else
	{
		if(SPIx==SPI1)
		{
			DMA_Cmd(DMA1_Channel3, DISABLE);
			DMA_Cmd(DMA1_Channel2, DISABLE);
		}
		else if(SPIx==SPI2)
		{
			DMA_Cmd(DMA1_Channel5, DISABLE);
			DMA_Cmd(DMA1_Channel4, DISABLE);
		}
		else if(SPIx==SPI3)
		{
			DMA_Cmd(DMA2_Channel2, DISABLE);
			DMA_Cmd(DMA2_Channel1, DISABLE);
		}
	}
}
/*******************************************************************************
*������		:	SPI_DMA_TX_Flag
*��������	:	��ѯSPI_DMA������ɱ�־
*����			: 
*���			:	��
*����ֵ		:	0--δ��ɣ�1--���
*����			:
*******************************************************************************/
unsigned char SPI_DMA_TX_Flag(SPI_TypeDef* SPIx)		//��ѯSPI_DMA������ɱ�־
{
	u8 DMA_TX_Complete_Flag=0;							//DMAͨ��������ɱ�־������0--δ��ɣ�1--���
	if(SPIx==SPI1)
	{
		if(DMA_GetFlagStatus(DMA1_FLAG_TC2)!=RESET)		//�������
		{
			DMA_TX_Complete_Flag=1;							//DMAͨ��������ɱ�־������0--δ��ɣ�1--���
			
			//2)**********���DMA��־
		DMA_ClearFlag(DMA1_FLAG_GL3|DMA1_FLAG_TC3|DMA1_FLAG_HT3|DMA1_FLAG_TE3);		//���DMA���ͱ�־
		DMA_ClearFlag(DMA1_FLAG_GL2|DMA1_FLAG_TC2|DMA1_FLAG_HT2|DMA1_FLAG_TE2);		//���DMA���ձ�־
			
		}
		else		//����δ���
		{
			DMA_TX_Complete_Flag=0;							//DMAͨ��������ɱ�־������0--δ��ɣ�1--���
		}
	}
	else if(SPIx==SPI2)
	{
		if(DMA_GetFlagStatus(DMA1_FLAG_TC4)!=RESET)
		{
			DMA_TX_Complete_Flag=1;							//DMAͨ��������ɱ�־������0--δ��ɣ�1--���
			
			//2)**********���DMA��־
		DMA_ClearFlag(DMA1_FLAG_GL5|DMA1_FLAG_TC5|DMA1_FLAG_HT5|DMA1_FLAG_TE5);		//���DMA���ͱ�־
		DMA_ClearFlag(DMA1_FLAG_GL4|DMA1_FLAG_TC4|DMA1_FLAG_HT4|DMA1_FLAG_TE4);		//���DMA���ձ�־
		}
		else		//����δ���
		{
			DMA_TX_Complete_Flag=0;							//DMAͨ��������ɱ�־������0--δ��ɣ�1--���
		}
	}
	else if(SPIx==SPI3)
	{
		if(DMA_GetFlagStatus(DMA2_FLAG_TC1)!=RESET)
		{
			DMA_TX_Complete_Flag=1;							//DMAͨ��������ɱ�־������0--δ��ɣ�1--���
			
			//2)**********���DMA��־
		DMA_ClearFlag(DMA2_FLAG_GL2|DMA2_FLAG_TC2|DMA2_FLAG_HT2|DMA2_FLAG_TE2);		//���DMA���ͱ�־
		DMA_ClearFlag(DMA2_FLAG_GL1|DMA2_FLAG_TC1|DMA2_FLAG_HT1|DMA2_FLAG_TE1);		//���DMA���ձ�־
		}
		else		//����δ���
		{
			DMA_TX_Complete_Flag=0;							//DMAͨ��������ɱ�־������0--δ��ɣ�1--���
		}
	}
	return	DMA_TX_Complete_Flag;							//DMAͨ��������ɱ�־������0--δ��ɣ�1--���

}

void SPI2_Conf_TEST(u32* SPI_TX_Buffer,u32* SPI_RX_Buffer,u32 SPI_BUFFERSIZE)
{
	DMA_InitTypeDef  DMA_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	NVIC_InitTypeDef NVIC_Initstructure;
	EXTI_InitTypeDef EXTI_Initstructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 

	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;			//���÷���				��2��ȫ˫����2��ֻ���ա�һ�߷��͡�һ�߽��գ�
//	SPI_InitStructure.SPI_Direction=SPI_Direction_1Line_Rx;			//���÷���				��ֻ����
//	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;														//ģʽ         	���ӻ����豸��
	SPI_InitStructure.SPI_Mode=SPI_Mode_Slave;													//ģʽ         	���ӻ����豸��
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;												//����         	��8��16λ��
//	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;															//ʱ�Ӽ���     	���ͻ�ߣ�
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;															//ʱ�Ӽ���     	���ͻ�ߣ�
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;														//ʱ����λ     	����һ����ڶ��������أ�
//	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;														//ʱ����λ     	����һ����ڶ��������أ�
//	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;															//Ƭѡ��ʽ     	��Ӳ����������ʽ����������
	SPI_InitStructure.SPI_NSS=SPI_NSS_Hard;																//Ƭѡ��ʽ     	��Ӳ����������ʽ��Ӳ������
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;		//������Ԥ��Ƶ 	����2---256��Ƶ��
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;											//���ȷ��͵�λ 	�����λ���������λ���ȣ�
	SPI_InitStructure.SPI_CRCPolynomial=0X07;															//����crc����ʽ	���临λֵΪ0x0007������Ӧ�ÿ�������������ֵ����
	
	SPI_Init(SPI2, &SPI_InitStructure);
	
	
	SPI_Cmd(SPI2, ENABLE);

	SPI_PinSet(SPI2,&SPI_InitStructure);	
	
	
	
	//5)**********DMA���ͳ�ʼ����������ΪDMA��Ŀ�Ķ�
	DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)(&SPI2->DR);						//DMA����Դ��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr     = (u32)SPI_TX_Buffer;							//DMA�����ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;											//DMA_DIR_PeripheralDST��������ΪDMA��Ŀ�Ķˣ���DMA_DIR_PeripheralSRC��������Ϊ���ݴ������Դ��
	DMA_InitStructure.DMA_BufferSize = SPI_BUFFERSIZE; 													//ָ��DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable�������ַ�Ĵ�����������DMA_PeripheralInc_Disable�������ַ�Ĵ������䣩��
	DMA_InitStructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable���ڴ��ַ�Ĵ�����������DMA_MemoryInc_Disable���ڴ��ַ�Ĵ������䣩
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݿ���--DMA_PeripheralDataSize_Byte�����ݿ���Ϊ8λ����DMA_PeripheralDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_PeripheralDataSize_Word�����ݿ���Ϊ32λ��
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//�ڴ����ݿ���--DMA_MemoryDataSize_Byte�����ݿ���Ϊ8λ����DMA_MemoryDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_MemoryDataSize_Word�����ݿ���Ϊ32λ��
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;														//DMA����ģʽ--DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular����ͣ�ش��ͣ�
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 										//DMAͨ����ת�����ȼ�--DMA_Priority_VeryHigh���ǳ��ߣ�DMA_Priority_High����)��DMA_Priority_Medium���У���DMA_Priority_Low���ͣ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														//DMAͨ�����ڴ浽�ڴ洫��--DMA_M2M_Enable(����Ϊ�ڴ浽�ڴ洫��)��DMA_M2M_Disable�����ڴ浽�ڴ洫�䣩
	DMA_Init(DMA1_Channel5,&DMA_InitStructure);														//��ʼ��DMA
		
//6)**********DMA���ճ�ʼ����������ΪDMA��Դ��--SPI����
	DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&SPI2->DR;					//DMA����Դ��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr     = 	(u32)SPI_RX_Buffer;						//DMA�����ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;											//DMA_DIR_PeripheralDST��������ΪDMA��Ŀ�Ķˣ���DMA_DIR_PeripheralSRC��������Ϊ���ݴ������Դ��
	DMA_InitStructure.DMA_BufferSize = SPI_BUFFERSIZE; 													//ָ��DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//DMA_PeripheralInc_Enable�������ַ�Ĵ�����������DMA_PeripheralInc_Disable�������ַ�Ĵ������䣩��
	DMA_InitStructure.DMA_MemoryInc =DMA_MemoryInc_Enable;									//DMA_MemoryInc_Enable���ڴ��ַ�Ĵ�����������DMA_MemoryInc_Disable���ڴ��ַ�Ĵ������䣩
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//�������ݿ���--DMA_PeripheralDataSize_Byte�����ݿ���Ϊ8λ����DMA_PeripheralDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_PeripheralDataSize_Word�����ݿ���Ϊ32λ��
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					//�ڴ����ݿ���--DMA_MemoryDataSize_Byte�����ݿ���Ϊ8λ����DMA_MemoryDataSize_HalfWord�����ݿ���Ϊ16λ����DMA_MemoryDataSize_Word�����ݿ���Ϊ32λ��
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;														//DMA����ģʽ--DMA_Mode_Normal��ֻ����һ�Σ�, DMA_Mode_Circular����ͣ�ش��ͣ�
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 										//DMAͨ����ת�����ȼ�--DMA_Priority_VeryHigh���ǳ��ߣ�DMA_Priority_High����)��DMA_Priority_Medium���У���DMA_Priority_Low���ͣ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														//DMAͨ�����ڴ浽�ڴ洫��--DMA_M2M_Enable(����Ϊ�ڴ浽�ڴ洫��)��DMA_M2M_Disable�����ڴ浽�ڴ洫�䣩
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);														//��ʼ��DMA


//	//7)**********DMAͨ���жϳ�ʼ��---��ΪDMA�����ж�----DMA��������ж�
//	NVIC_Initstructure.NVIC_IRQChannel = SPI2_IRQChannel;       		//ͨ������Ϊ����1�ж�
//	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority=1;
//	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 1; 			//�ж���Ӧ���ȼ�0	
//	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;        									//���ж�
//	NVIC_Init(&NVIC_Initstructure);
//	SPI_I2S_ITConfig(SPI2,SPI_I2S_IT_RXNE, ENABLE);			//��������ж�

	
	//NSS�������ж�************************************************************************
	EXTI_Configuration(GPIOB,GPIO_Pin_12,EXTI_Mode_Interrupt);
	
	EXTI_Initstructure.EXTI_Trigger=EXTI_Trigger_Rising;					//������ʽ-
	EXTI_Initstructure.EXTI_LineCmd=ENABLE;												//����ʹ��
	EXTI_Init(&EXTI_Initstructure);
	
	EXTI_GenerateSWInterrupt(EXTI_Line12);													//ʹ���ж�


	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);

	
	EXTI_Initstructure.EXTI_Line=EXTI_Line12;												//�ⲿ�ж���·
	EXTI_Initstructure.EXTI_Mode=EXTI_Mode_Interrupt;						//�ж�ģʽ
//	EXTI_Initstructure.EXTI_Mode=EXTI_Mode_Event;						//�ж�ģʽ
	EXTI_Initstructure.EXTI_Trigger=EXTI_Trigger_Rising;					//������ʽ-
	EXTI_Initstructure.EXTI_LineCmd=ENABLE;												//����ʹ��
	EXTI_Init(&EXTI_Initstructure);
	
	EXTI_GenerateSWInterrupt(EXTI_Line12);													//ʹ���ж�
	
	//7)**********�����ж�
	NVIC_Initstructure.NVIC_IRQChannel = EXTI15_10_IRQChannel;					//ѡ���ж�ͨ��-�ж�Դ
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority = 0;			//��ռ���ȼ�
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 0;						//��Ӧ���ȼ�
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ��
	NVIC_Init(&NVIC_Initstructure);	

	EXTI_GenerateSWInterrupt(EXTI_Line12);													//ʹ���ж�
	EXTI_ClearITPendingBit(EXTI_Line12);
	//NSS�������ж�************************************************************************	
	
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);	
	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);
	
	DMA_Cmd(DMA1_Channel5,ENABLE);			//SPI-TX
	DMA_Cmd(DMA1_Channel4,ENABLE);			//SPI-RX
	
	SPI_Cmd(SPI2, ENABLE);
	
	

}



/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void SPI_Configuratio(SPI_TypeDef* SPIx)
{

}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void SPI_General_Configuratio(void)
{

}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void SPI_GPIO_Configuratio(SPI_TypeDef* SPIx)
{


}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void SPI_DMA_Configuratio(SPI_TypeDef* SPIx)
{

}
























