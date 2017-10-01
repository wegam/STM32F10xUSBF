/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32f10x_usart.h
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : This file contains all the functions prototypes for the
*                      USART firmware library.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_USART_H
#define __STM32F10x_USART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_map.h"

/* Exported types ------------------------------------------------------------*/
/* USART Init Structure definition */
typedef struct
{
  u32 USART_BaudRate;			//USART����Ĳ����ʣ������ʿ��������¹�ʽ���㣺IntegerDivider = ((APBClock) / (16 * (USART_InitStruct->USART_BaudRate)))��FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 16) + 0.5
  u16 USART_WordLength;		//��һ��֡�д�����߽��յ�������λ��		��Line62:USART Word Length
  u16 USART_StopBits;			//���͵�ֹͣλ��Ŀ
  u16 USART_Parity;				//��żУ��ģʽ
  u16 USART_Mode;					//ʹ�ܻ���ʧ�ܷ��ͺͽ���ģʽ
  u16 USART_HardwareFlowControl; 	//Ӳ��������ģʽ 
} USART_InitTypeDef;

/* USART Clock Init Structure definition */
typedef struct
{
  u16 USART_Clock;
  u16 USART_CPOL;
  u16 USART_CPHA;
  u16 USART_LastBit;
} USART_ClockInitTypeDef;

/* Exported constants --------------------------------------------------------*/
#define IS_USART_ALL_PERIPH(PERIPH) (((*(u32*)&(PERIPH)) == USART1_BASE) || \
                                     ((*(u32*)&(PERIPH)) == USART2_BASE) || \
                                     ((*(u32*)&(PERIPH)) == USART3_BASE) || \
                                     ((*(u32*)&(PERIPH)) == UART4_BASE) || \
                                     ((*(u32*)&(PERIPH)) == UART5_BASE))

#define IS_USART_123_PERIPH(PERIPH) (((*(u32*)&(PERIPH)) == USART1_BASE) || \
                                     ((*(u32*)&(PERIPH)) == USART2_BASE) || \
                                     ((*(u32*)&(PERIPH)) == USART3_BASE))

#define IS_USART_1234_PERIPH(PERIPH) (((*(u32*)&(PERIPH)) == USART1_BASE) || \
                                      ((*(u32*)&(PERIPH)) == USART2_BASE) || \
                                      ((*(u32*)&(PERIPH)) == USART3_BASE) || \
                                      ((*(u32*)&(PERIPH)) == UART4_BASE))

/* USART Word Length ---------------------------------------------------------*/
#define USART_WordLength_8b                  ((u16)0x0000)		//����λ-8λ
#define USART_WordLength_9b                  ((u16)0x1000)		//����λ-9λ
                                    
#define IS_USART_WORD_LENGTH(LENGTH) (((LENGTH) == USART_WordLength_8b) || \
                                      ((LENGTH) == USART_WordLength_9b))

/* USART Stop Bits -----------------------------------------------------------*/
#define USART_StopBits_1                     ((u16)0x0000)	//ֹͣλ����֡��β����1��ֹͣλ
#define USART_StopBits_0_5                   ((u16)0x1000)	//ֹͣλ����֡��β����0.5��ֹͣλ
#define USART_StopBits_2                     ((u16)0x2000)	//ֹͣλ����֡��β����2��ֹͣλ
#define USART_StopBits_1_5                   ((u16)0x3000)	//ֹͣλ����֡��β����1.5��ֹͣλ

#define IS_USART_STOPBITS(STOPBITS) (((STOPBITS) == USART_StopBits_1) || \
                                     ((STOPBITS) == USART_StopBits_0_5) || \
                                     ((STOPBITS) == USART_StopBits_2) || \
                                     ((STOPBITS) == USART_StopBits_1_5))
/* USART Parity --------------------------------------------------------------*/
#define USART_Parity_No                      ((u16)0x0000)	//��żУ��-�ر�
#define USART_Parity_Even                    ((u16)0x0400)	//��żУ��-żУ��
#define USART_Parity_Odd                     ((u16)0x0600) 	//��żУ��-��У��

#define IS_USART_PARITY(PARITY) (((PARITY) == USART_Parity_No) || \
                                 ((PARITY) == USART_Parity_Even) || \
                                 ((PARITY) == USART_Parity_Odd))

/* USART Mode ----------------------------------------------------------------*/
#define USART_Mode_Rx                        ((u16)0x0004)	//����ʹ��
#define USART_Mode_Tx                        ((u16)0x0008)	//����ʹ��

#define IS_USART_MODE(MODE) ((((MODE) & (u16)0xFFF3) == 0x00) && ((MODE) != (u16)0x00))

/* USART Hardware Flow Control -----------------------------------------------*/
#define USART_HardwareFlowControl_None       ((u16)0x0000)	//Ӳ��������ʧ��
#define USART_HardwareFlowControl_RTS        ((u16)0x0100)	//��������RTSʹ��
#define USART_HardwareFlowControl_CTS        ((u16)0x0200)	//�������CTSʹ��
#define USART_HardwareFlowControl_RTS_CTS    ((u16)0x0300)	//RTS��CTSʹ��

#define IS_USART_HARDWARE_FLOW_CONTROL(CONTROL)\
                              (((CONTROL) == USART_HardwareFlowControl_None) || \
                               ((CONTROL) == USART_HardwareFlowControl_RTS) || \
                               ((CONTROL) == USART_HardwareFlowControl_CTS) || \
                               ((CONTROL) == USART_HardwareFlowControl_RTS_CTS))

#define IS_USART_PERIPH_HFC(PERIPH, HFC) ((((*(u32*)&(PERIPH)) != UART4_BASE) && \
                                          ((*(u32*)&(PERIPH)) != UART5_BASE)) \
                                          || ((HFC) == USART_HardwareFlowControl_None))                                

/* USART Clock ---------------------------------------------------------------*/
#define USART_Clock_Disable                  ((u16)0x0000)
#define USART_Clock_Enable                   ((u16)0x0800)

#define IS_USART_CLOCK(CLOCK) (((CLOCK) == USART_Clock_Disable) || \
                               ((CLOCK) == USART_Clock_Enable))

/* USART Clock Polarity ------------------------------------------------------*/
#define USART_CPOL_Low                       ((u16)0x0000)
#define USART_CPOL_High                      ((u16)0x0400)

#define IS_USART_CPOL(CPOL) (((CPOL) == USART_CPOL_Low) || ((CPOL) == USART_CPOL_High))
                               
/* USART Clock Phase ---------------------------------------------------------*/
#define USART_CPHA_1Edge                     ((u16)0x0000)
#define USART_CPHA_2Edge                     ((u16)0x0200)
#define IS_USART_CPHA(CPHA) (((CPHA) == USART_CPHA_1Edge) || ((CPHA) == USART_CPHA_2Edge))

/* USART Last Bit ------------------------------------------------------------*/
#define USART_LastBit_Disable                ((u16)0x0000)
#define USART_LastBit_Enable                 ((u16)0x0100)

#define IS_USART_LASTBIT(LASTBIT) (((LASTBIT) == USART_LastBit_Disable) || \
                                   ((LASTBIT) == USART_LastBit_Enable))

/* USART Interrupt definition ------------------------------------------------*/
#define USART_IT_PE                          ((u16)0x0028)					//��ż�����ж�
#define USART_IT_TXE                         ((u16)0x0727)					//�����ж�
#define USART_IT_TC                          ((u16)0x0626)					//��������ж�
#define USART_IT_RXNE                        ((u16)0x0525)					//�����ж�
#define USART_IT_IDLE                        ((u16)0x0424)					//USART�����ж�->����⵽���߿���ʱ����λ��Ӳ����λ�����USART_CR1�е�IDLEIEΪ��1����������жϡ���������������λ(�ȶ�USART_SR��Ȼ���USART_DR)��
#define USART_IT_LBD                         ((u16)0x0846)					//LIN�жϼ���ж�
#define USART_IT_CTS                         ((u16)0x096A)					//CTS�ж�
#define USART_IT_ERR                         ((u16)0x0060)					//�����ж�
#define USART_IT_ORE                         ((u16)0x0360)					//���ش��� (Overrun error)
#define USART_IT_NE                          ((u16)0x0260)					//���������־ (Noise error flag)
#define USART_IT_FE                          ((u16)0x0160)					//֡���� (Framing error)

#define IS_USART_CONFIG_IT(IT) (((IT) == USART_IT_PE) || ((IT) == USART_IT_TXE) || \
                               ((IT) == USART_IT_TC) || ((IT) == USART_IT_RXNE) || \
                               ((IT) == USART_IT_IDLE) || ((IT) == USART_IT_LBD) || \
                               ((IT) == USART_IT_CTS) || ((IT) == USART_IT_ERR))

#define IS_USART_IT(IT) (((IT) == USART_IT_PE) || ((IT) == USART_IT_TXE) || \
                         ((IT) == USART_IT_TC) || ((IT) == USART_IT_RXNE) || \
                         ((IT) == USART_IT_IDLE) || ((IT) == USART_IT_LBD) || \
                         ((IT) == USART_IT_CTS) || ((IT) == USART_IT_ORE) || \
                         ((IT) == USART_IT_NE) || ((IT) == USART_IT_FE))

#define IS_USART_CLEAR_IT(IT) (((IT) == USART_IT_PE) || ((IT) == USART_IT_TC) || \
                               ((IT) == USART_IT_RXNE) || ((IT) == USART_IT_IDLE) || \
                               ((IT) == USART_IT_LBD) || ((IT) == USART_IT_CTS) ||  \
                               ((IT) == USART_IT_ORE) || ((IT) == USART_IT_NE) || \
                               ((IT) == USART_IT_FE))

#define IS_USART_PERIPH_IT(PERIPH, USART_IT) ((((*(u32*)&(PERIPH)) != UART4_BASE) && \
                                              ((*(u32*)&(PERIPH)) != UART5_BASE)) \
                                              || ((USART_IT) != USART_IT_CTS))                                                                           

/* USART DMA Requests --------------------------------------------------------*/
#define USART_DMAReq_Tx                      ((u16)0x0080)
#define USART_DMAReq_Rx                      ((u16)0x0040)

#define IS_USART_DMAREQ(DMAREQ) ((((DMAREQ) & (u16)0xFF3F) == 0x00) && ((DMAREQ) != (u16)0x00))

/* USART WakeUp methods ------------------------------------------------------*/
#define USART_WakeUp_IdleLine                ((u16)0x0000)
#define USART_WakeUp_AddressMark             ((u16)0x0800)

#define IS_USART_WAKEUP(WAKEUP) (((WAKEUP) == USART_WakeUp_IdleLine) || \
                                 ((WAKEUP) == USART_WakeUp_AddressMark))

/* USART LIN Break Detection Length ------------------------------------------*/
#define USART_LINBreakDetectLength_10b      ((u16)0x0000)
#define USART_LINBreakDetectLength_11b      ((u16)0x0020)

#define IS_USART_LIN_BREAK_DETECT_LENGTH(LENGTH) \
                               (((LENGTH) == USART_LINBreakDetectLength_10b) || \
                                ((LENGTH) == USART_LINBreakDetectLength_11b))

/* USART IrDA Low Power ------------------------------------------------------*/
#define USART_IrDAMode_LowPower              ((u16)0x0004)
#define USART_IrDAMode_Normal                ((u16)0x0000)

#define IS_USART_IRDA_MODE(MODE) (((MODE) == USART_IrDAMode_LowPower) || \
                                  ((MODE) == USART_IrDAMode_Normal))

/* USART Flags ---------------------------------------------------------------*/
#define USART_FLAG_CTS                       ((u16)0x0200)		//CTS ��־ (CTS flag)	
																															//���������CTSEλ����nCTS����仯״̬ʱ����λ��Ӳ���øߡ�������������㡣���USART_CR3�е�CTSIEΪ��1����������жϡ�
																															//0��nCTS״̬����û�б仯��
																															//1��nCTS״̬���Ϸ����仯��
																															//ע��UART4��UART5�ϲ�������һλ��
#define USART_FLAG_LBD                       ((u16)0x0100)		//LIN�Ͽ�����־ (LIN break detection flag)
																															//��̽�⵽LIN�Ͽ�ʱ����λ��Ӳ���á�1����������塯0��(���λд0)�����USART_CR3�е�LBDIE = 1��������жϡ�
																															//0��û�м�⵽LIN�Ͽ���
																															//1����⵽LIN�Ͽ���
																															//ע�⣺��LBDIE=1����LBDΪ��1��ʱҪ�����жϡ�
#define USART_FLAG_TXE                       ((u16)0x0080)		//TXE:�������ݼĴ����� (Transmit data register empty)
																															//��TDR�Ĵ����е����ݱ�Ӳ��ת�Ƶ���λ�Ĵ�����ʱ�򣬸�λ��Ӳ����λ�����USART_CR1�Ĵ����е�TXEIEΪ1��������жϡ���USART_DR��д����������λ���㡣
																															//0�����ݻ�û�б�ת�Ƶ���λ�Ĵ�����
																															//1�������Ѿ���ת�Ƶ���λ�Ĵ�����
																															//ע�⣺��������������ʹ�ø�λ��
#define USART_FLAG_TC                        ((u16)0x0040)		//������� (Transmission complete)
																															//�����������ݵ�һ֡������ɺ󣬲���TXE=1ʱ����Ӳ������λ�á�1�������USART_CR1�е�TCIEΪ��1����������жϡ�
																															//��������������λ(�ȶ�USART_SR��Ȼ��д��USART_DR)��TCλҲ����ͨ��д�롯0���������ֻ���ڶ໺��ͨѶ�в��Ƽ������������
																															//0�����ͻ�δ��ɣ�
																															//1��������ɡ�
#define USART_FLAG_RXNE                      ((u16)0x0020)		//RXNE�������ݼĴ����ǿ� (Read data register not empty)
																															//��RDR��λ�Ĵ����е����ݱ�ת�Ƶ�USART_DR�Ĵ����У���λ��Ӳ����λ�����USART_CR1�Ĵ����е�RXNEIEΪ1��������жϡ�
																															//��USART_DR�Ķ��������Խ���λ���㡣RXNEλҲ����ͨ��д��0�������ֻ���ڶ໺��ͨѶ�в��Ƽ������������
																															//0������û���յ���
																															//1���յ����ݣ����Զ�����
#define USART_FLAG_IDLE                      ((u16)0x0010)		//IDLE����⵽���߿��� (IDLE line detected)
																															//����⵽���߿���ʱ����λ��Ӳ����λ�����USART_CR1�е�IDLEIEΪ��1����������жϡ���������������λ(�ȶ�USART_SR��Ȼ���USART_DR)��
																															//0��û�м�⵽�������ߣ�
																															//1����⵽�������ߡ�
																															//ע�⣺IDLEλ�����ٴα��ø�ֱ��RXNEλ������(���ּ�⵽һ�ο�������)
#define USART_FLAG_ORE                       ((u16)0x0008)		//���ش��� (Overrun error)
																															//��RXNE��Ȼ�ǡ�1����ʱ�򣬵�ǰ����������λ�Ĵ����е����ݣ���Ҫ������RDR�Ĵ���ʱ��Ӳ������λ��λ��
																															//���USART_CR1�е�RXNEIEΪ��1���Ļ���������жϡ���������н�������(�ȶ�USART_SR��Ȼ���USART_CR)��
																															//0��û�й��ش���
																															//1����⵽���ش���
																															//ע�⣺��λ����λʱ��RDR�Ĵ����е�ֵ���ᶪʧ��������λ�Ĵ����е����ݻᱻ���ǡ����������EIEλ���ڶ໺����ͨ��ģʽ�£�ORE��־��λ������жϵġ�
#define USART_FLAG_NE                        ((u16)0x0004)		//���������־ (Noise error flag)
																															//�ڽ��յ���֡��⵽����ʱ����Ӳ���Ը�λ��λ����������ж�������(�ȶ�USART_SR���ٶ�USART_DR)��
																															//0��û�м�⵽������
																															//1����⵽������
																															//ע�⣺��λ��������жϣ���Ϊ����RXNEһ����֣�Ӳ����������RXNE��־ʱ�����жϡ��ڶ໺����ͨ��ģʽ�£����������EIEλ��������NE��־ʱ������жϡ�
#define USART_FLAG_FE                        ((u16)0x0002)		//֡���� (Framing error)
																															//����⵽ͬ����λ��������������߼�⵽�Ͽ�������λ��Ӳ����λ����������н�������(�ȶ�USART_SR���ٶ�USART_DR)��
																															//0��û�м�⵽֡����
																															//1����⵽֡�������break����
#define USART_FLAG_PE                        ((u16)0x0001)		//У����� (Parity error)
																															//�ڽ���ģʽ�£����������żУ�����Ӳ���Ը�λ��λ����������ж�������(���ζ�USART_SR��USART_DR)�������PEλǰ���������ȴ�RXNE��־λ���á�1�������USART_CR1�е�PEIEΪ��1����������жϡ�

#define IS_USART_FLAG(FLAG) (((FLAG) == USART_FLAG_PE) || ((FLAG) == USART_FLAG_TXE) || \
                             ((FLAG) == USART_FLAG_TC) || ((FLAG) == USART_FLAG_RXNE) || \
                             ((FLAG) == USART_FLAG_IDLE) || ((FLAG) == USART_FLAG_LBD) || \
                             ((FLAG) == USART_FLAG_CTS) || ((FLAG) == USART_FLAG_ORE) || \
                             ((FLAG) == USART_FLAG_NE) || ((FLAG) == USART_FLAG_FE))
                              
#define IS_USART_CLEAR_FLAG(FLAG) ((((FLAG) & (u16)0xFC80) == 0x00) && ((FLAG) != (u16)0x00))

#define IS_USART_PERIPH_FLAG(PERIPH, USART_FLAG) ((((*(u32*)&(PERIPH)) != UART4_BASE) &&\
                                                  ((*(u32*)&(PERIPH)) != UART5_BASE)) \
                                                  || ((USART_FLAG) != USART_FLAG_CTS)) 

#define IS_USART_BAUDRATE(BAUDRATE) (((BAUDRATE) > 0) && ((BAUDRATE) < 0x0044AA21))
#define IS_USART_ADDRESS(ADDRESS) ((ADDRESS) <= 0xF)
#define IS_USART_DATA(DATA) ((DATA) <= 0x1FF)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void USART_DeInit(USART_TypeDef* USARTx);
void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct);
void USART_StructInit(USART_InitTypeDef* USART_InitStruct);
void USART_ClockInit(USART_TypeDef* USARTx, USART_ClockInitTypeDef* USART_ClockInitStruct);
void USART_ClockStructInit(USART_ClockInitTypeDef* USART_ClockInitStruct);
void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_ITConfig(USART_TypeDef* USARTx, u16 USART_IT, FunctionalState NewState);
void USART_DMACmd(USART_TypeDef* USARTx, u16 USART_DMAReq, FunctionalState NewState);
void USART_SetAddress(USART_TypeDef* USARTx, u8 USART_Address);
void USART_WakeUpConfig(USART_TypeDef* USARTx, u16 USART_WakeUp);
void USART_ReceiverWakeUpCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_LINBreakDetectLengthConfig(USART_TypeDef* USARTx, u16 USART_LINBreakDetectLength);
void USART_LINCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_SendData(USART_TypeDef* USARTx, u16 Data);
u16 USART_ReceiveData(USART_TypeDef* USARTx);
void USART_SendBreak(USART_TypeDef* USARTx);
void USART_SetGuardTime(USART_TypeDef* USARTx, u8 USART_GuardTime);
void USART_SetPrescaler(USART_TypeDef* USARTx, u8 USART_Prescaler);
void USART_SmartCardCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_SmartCardNACKCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_HalfDuplexCmd(USART_TypeDef* USARTx, FunctionalState NewState);
void USART_IrDAConfig(USART_TypeDef* USARTx, u16 USART_IrDAMode);
void USART_IrDACmd(USART_TypeDef* USARTx, FunctionalState NewState);
FlagStatus USART_GetFlagStatus(USART_TypeDef* USARTx, u16 USART_FLAG);
void USART_ClearFlag(USART_TypeDef* USARTx, u16 USART_FLAG);
ITStatus USART_GetITStatus(USART_TypeDef* USARTx, u16 USART_IT);
void USART_ClearITPendingBit(USART_TypeDef* USARTx, u16 USART_IT);

#endif /* __STM32F10x_USART_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
