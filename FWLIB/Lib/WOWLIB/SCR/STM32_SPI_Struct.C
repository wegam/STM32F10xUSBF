///******************************** User_library *********************************
//* �ļ��� 	: STM32_SDCard.H
//* ����   	: wegam@sina.com
//* �汾   	: V
//* ����   	: 2016/01/01
//* ˵��   	: 
//********************************************************************************
//* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
//* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
//* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
//* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
//* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
//* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
//*******************************************************************************/


//#include "STM32_SPI_Struct.H"
////#include "STM32F10x_BitBand.H"

////char *SPI_BUFFER;
////u32	SPI_BUFFERSIZE;




///*******************************************************************************
//*������		:	SPI_Configuration
//*��������	:	��������
//*����			:	MemoryAddr--�ڴ��ַ,
//						BufferSize--�����С
//*���			:	��
//*����ֵ		:	��
//*����			:
//*******************************************************************************/
//void SPIT_Configuration(SPI_TypeDef* SPIx)							//SPI����ʹ�÷�ʽ����
//{	

//}

///*******************************************************************************
//*������		:	SPI_PinSet
//*��������	:	SPI�ܽ����ó�ʼ��
//*����			: 
//*���			:	��
//*����ֵ		:	��
//*����			:
//*******************************************************************************/
//void SPIT_PinSet(SPI_TypeDef* SPIx)													//SPI�ܽ����ü���ʼ��
//{
//	//1)**********������ؽṹ��
//	SPI_InitTypeDef  SPI_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;

//	//2)**********���GPIO����
//	if(SPIx==SPI1)
//	{
//		//2.1)**********��SPIʱ��	
//			
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);
//		
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
////		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
////		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////		GPIO_Init(GPIOA, &GPIO_InitStructure);
//		
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
//		
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
//	}
//	else if(SPIx==SPI2)
//	{
//		//2.2)**********��SPIʱ��
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);
//			
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
//		
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14| GPIO_Pin_15;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
//		GPIO_Init(GPIOB, &GPIO_InitStructure);	
//		
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_14| GPIO_Pin_15;
////		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
////		GPIO_Init(GPIOB, &GPIO_InitStructure);	
////		
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
////		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
////		GPIO_Init(GPIOB, &GPIO_InitStructure);	
//	}
//		else if(SPIx==SPI3)
//	{
//		//2.2)**********��SPIʱ��
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3 ,ENABLE);
//			
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
//		
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 |GPIO_Pin_4| GPIO_Pin_5;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
//		GPIO_Init(GPIOB, &GPIO_InitStructure);	
//		
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
//		GPIO_Init(GPIOA, &GPIO_InitStructure);	
//		
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 |GPIO_Pin_4| GPIO_Pin_5;
////		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
////		GPIO_Init(GPIOB, &GPIO_InitStructure);	
////		
////		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
////		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
////		GPIO_Init(GPIOA, &GPIO_InitStructure);	
//	}
//	//3)**********SPI����ѡ��
//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;				//���÷���				��2��ȫ˫����2��ֻ���ա�һ�߷��͡�һ�߽��գ�
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;															//ģʽ         	���ӻ����豸��
//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;													//����         	��8��16λ��
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;																//ʱ�Ӽ���     	���ͻ�ߣ�
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;															//ʱ����λ     	����һ����ڶ��������أ�
////	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;																	//Ƭѡ��ʽ     	��Ӳ����������ʽ��
//	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;																	//Ƭѡ��ʽ     	��Ӳ����������ʽ��
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;				//������Ԥ��Ƶ 	����2---256��Ƶ��
//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;												//���ȷ��͵�λ 	�����λ���������λ���ȣ�
//	SPI_InitStructure.SPI_CRCPolynomial = 7;																	//����crc����ʽ	�����֣���7
//	SPI_Init(SPIx, &SPI_InitStructure);

//}

///*******************************************************************************
//*������		:	
//*��������	:	
//*����			: 
//*���			:	��
//*����ֵ		:	��
//*����			:
//*******************************************************************************/
//u8 SPIT_SendByte(u8 byte)
//{
//  /* �ȴ�DR�Ĵ����� */
//  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

//  /* Send byte through the SPI1 peripheral */
//  SPI_I2S_SendData(SPI1, byte);

//  /* Wait to receive a byte */
//  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

//  /* Return the byte read from the SPI bus */
//  return SPI_I2S_ReceiveData(SPI1);
//}

