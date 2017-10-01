/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32f10x_can.h
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : This file contains all the functions prototypes for the
*                      CAN firmware library.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/*��������������������������������������������������CAN����������������������������������������/
*	��������ΪCAN����Ľ��棬֧��CAN Э��2.0A��2.0B���������Ŀ���ǣ�����С��CPU ��������Ч����
*	�����յ��ı��ġ���Ҳ֧�ֱ��ķ��͵����ȼ�Ҫ�����ȼ����Կ�������ã���
������������������������������������������������������������������������������������*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_CAN_H
#define __STM32F10x_CAN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_map.h"

/* Exported types ------------------------------------------------------------*/
/* CAN init structure definition */
typedef struct
{
  FunctionalState CAN_TTCM;						//CAN_TTCM����ʹ�ܻ���ʧ��ʱ�䴥��ͨѶģʽ�������������������ֵΪENABLE����DISABLE��
  FunctionalState CAN_ABOM;						//CAN_ABOM����ʹ�ܻ���ʧ���Զ����߹��������������������ֵΪENABLE����DISABLE��
  FunctionalState CAN_AWUM;						//CAN_AWUM����ʹ�ܻ���ʧ���Զ�����ģʽ�������������������ֵΪENABLE����DISABLE��
  FunctionalState CAN_NART;						//CAN_NARM����ʹ�ܻ���ʧ�ܷ��Զ��ش���ģʽ�������������������ֵΪENABLE����DISABLE��
  FunctionalState CAN_RFLM;						//CAN_RFLM����ʹ�ܻ���ʧ�ܽ���FIFO����ģʽ�������������������ֵΪENABLE����DISABLE��
  FunctionalState CAN_TXFP;						//CAN_TXFP����ʹ�ܻ���ʧ�ܷ���FIFO���ȼ��������������������ֵΪENABLE����DISABLE��
  u8 CAN_Mode;												//CAN_Mode������CAN�Ĺ���ģʽ��<CAN operating mode>.�����˸ò�����ȡ��ֵ
  u8 CAN_SJW;													//CAN_SJW����������ͬ����Ծ���(SJW)������ÿλ�п����ӳ������̶��ٸ�ʱ�䵥λ�����ޣ�<CAN synchronisation jump width>�����˸ò�����ȡ��ֵ
  u8 CAN_BS1;													//CAN_BS1�趨��ʱ���1��ʱ�䵥λ��Ŀ��<time quantum in bit segment 1>�����˸ò�����ȡ��ֵ
  u8 CAN_BS2;													//CAN_BS2�趨��ʱ���1��ʱ�䵥λ��Ŀ��<time quantum in bit segment 2>�����˸ò�����ȡ��ֵ
  u16 CAN_Prescaler;									//CAN_Prescaler�趨��һ��ʱ�䵥λ�ĳ��ȣ����ķ�Χ��1��1024��
} CAN_InitTypeDef;											

/* CAN filter init structure definition */
typedef struct
{
  u8 CAN_FilterNumber;									//CAN_FilterNumberָ���˴���ʼ���Ĺ����������ķ�Χ��0��13��
  u8 CAN_FilterMode;										//CAN_FilterModeָ���˹�����������ʼ������ģʽ��<CAN_FilterMode>�����˸ò�����ȡ��ֵ
  u8 CAN_FilterScale;										//CAN_FilterScale�����˹�����λ��<CAN_FilterScale>�����˸ò�����ȡ��ֵ
  u16 CAN_FilterIdHigh;									//CAN_FilterIdHigh�����趨��������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ���������ķ�Χ��0x0000��0xFFFF��
  u16 CAN_FilterIdLow;									//CAN_FilterIdHigh�����趨��������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ����������ķ�Χ��0x0000��0xFFFF��
  u16 CAN_FilterMaskIdHigh;							//CAN_FilterMaskIdHigh�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ���������ķ�Χ��0x0000��0xFFFF��
  u16 CAN_FilterMaskIdLow;							//CAN_FilterMaskIdLow�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ����������ķ�Χ��0x0000��0xFFFF��
  u16 CAN_FilterFIFOAssignment;					//CAN_FilterFIFO�趨��ָ���������FIFO��0��1����<CAN_FilterFIFO>�����˸ò�����ȡ��ֵ
  FunctionalState CAN_FilterActivation;	//CAN_FilterActivationʹ�ܻ���ʧ�ܹ��������ò�����ȡ��ֵΪENABLE����DISABLE��
} CAN_FilterInitTypeDef;

/* CAN Tx message structure definition */
typedef struct
{
  u32 StdId;														//StdId�����趨��׼��ʶ��������ȡֵ��ΧΪ0��0x7FF
  u32 ExtId;														//ExtId�����趨��չ��ʶ��������ȡֵ��ΧΪ0��0x3FFFF
  u8 IDE;																//IDE�����趨��Ϣ��ʶ�������ͣ�<CAN identifier type>�����˸ò�����ȡ��ֵ
  u8 RTR;																//RTR�����趨��������Ϣ��֡���͡�����������Ϊ����֡����Զ��֡<CAN remote transmission request>��
  u8 DLC;																//DLC�����趨��������Ϣ��֡���ȡ�����ȡֵ��Χ��0��0x8��
  u8 Data[8];														//Data[8]�����˴��������ݣ�����ȡֵ��ΧΪ0��0xFF��
} CanTxMsg;

/* CAN Rx message structure definition */
typedef struct
{
  u32 StdId;														//StdId�����趨��׼��ʶ��������ȡֵ��ΧΪ0��0x7FF
  u32 ExtId;														//ExtId�����趨��չ��ʶ��������ȡֵ��ΧΪ0��0x3FFFF
  u8 IDE;																//IDE�����趨��Ϣ��ʶ�������ͣ�<CAN identifier type>�����˸ò�����ȡ��ֵ
  u8 RTR;																//RTR�����趨��������Ϣ��֡���͡�����������Ϊ����֡����Զ��֡<CAN remote transmission request>��
  u8 DLC;																//DLC�����趨��������Ϣ��֡���ȡ�����ȡֵ��Χ��0��0x8��
  u8 Data[8];														//Data[8]�����˴��������ݣ�����ȡֵ��ΧΪ0��0xFF��
  u8 FMI;																//FMI�趨Ϊ��Ϣ��Ҫͨ���Ĺ�������������Щ��Ϣ�洢�������С��ò���ȡֵ��Χ0��0xFF��
} CanRxMsg;

/* Exported constants --------------------------------------------------------*/

/* CAN sleep constants */
#define CANINITFAILED              ((u8)0x00) /* CAN initialization failed */
#define CANINITOK                  ((u8)0x01) /* CAN initialization failed */

/* CAN operating mode */
#define CAN_Mode_Normal             ((u8)0x00)  /* normal mode */														//CANӲ������������ģʽ
#define CAN_Mode_LoopBack           ((u8)0x01)  /* loopback mode */													//CANӲ�������ڻ���ģʽ
#define CAN_Mode_Silent             ((u8)0x02)  /* silent mode */														//CANӲ�������ھ�Ĭģʽ
#define CAN_Mode_Silent_LoopBack    ((u8)0x03)  /* loopback combined with silent mode */		//CANӲ�������ھ�Ĭ����ģʽ

#define IS_CAN_MODE(MODE) (((MODE) == CAN_Mode_Normal) || ((MODE) == CAN_Mode_LoopBack)|| \
                           ((MODE) == CAN_Mode_Silent) || ((MODE) == CAN_Mode_Silent_LoopBack))

/* CAN synchronisation jump width */	//����ͬ����Ծ���(SJW)����������ÿλ�п����ӳ������̶��ٸ�ʱ�䵥Ԫ�����ޡ���ֵ���Ա��Ϊ1��4��ʱ�䵥Ԫ��
#define CAN_SJW_1tq                 ((u8)0x00)  /* 1 time quantum */		//����ͬ����Ծ���1��ʱ�䵥λ
#define CAN_SJW_2tq                 ((u8)0x01)  /* 2 time quantum */		//����ͬ����Ծ���2��ʱ�䵥λ
#define CAN_SJW_3tq                 ((u8)0x02)  /* 3 time quantum */		//����ͬ����Ծ���3��ʱ�䵥λ
#define CAN_SJW_4tq                 ((u8)0x03)  /* 4 time quantum */		//����ͬ����Ծ���4��ʱ�䵥λ

#define IS_CAN_SJW(SJW) (((SJW) == CAN_SJW_1tq) || ((SJW) == CAN_SJW_2tq)|| \
                         ((SJW) == CAN_SJW_3tq) || ((SJW) == CAN_SJW_4tq))

/* time quantum in bit segment 1 */	//ʱ���1(BS1)������������λ�á�������CAN��׼���PROP_SEG��PHASE_SEG1��
																		//��ֵ���Ա��Ϊ1��16��ʱ�䵥Ԫ����Ҳ���Ա��Զ��ӳ����Բ�����Ϊ�����в�ͬ�ڵ��Ƶ�ʲ�������ɵ���λ������Ư�ơ�
#define CAN_BS1_1tq                 ((u8)0x00)  /* 1 time quantum */		//ʱ���1Ϊ1��ʱ�䵥λ
#define CAN_BS1_2tq                 ((u8)0x01)  /* 2 time quantum */
#define CAN_BS1_3tq                 ((u8)0x02)  /* 3 time quantum */
#define CAN_BS1_4tq                 ((u8)0x03)  /* 4 time quantum */
#define CAN_BS1_5tq                 ((u8)0x04)  /* 5 time quantum */
#define CAN_BS1_6tq                 ((u8)0x05)  /* 6 time quantum */
#define CAN_BS1_7tq                 ((u8)0x06)  /* 7 time quantum */
#define CAN_BS1_8tq                 ((u8)0x07)  /* 8 time quantum */
#define CAN_BS1_9tq                 ((u8)0x08)  /* 9 time quantum */
#define CAN_BS1_10tq                ((u8)0x09)  /* 10 time quantum */
#define CAN_BS1_11tq                ((u8)0x0A)  /* 11 time quantum */
#define CAN_BS1_12tq                ((u8)0x0B)  /* 12 time quantum */
#define CAN_BS1_13tq                ((u8)0x0C)  /* 13 time quantum */
#define CAN_BS1_14tq                ((u8)0x0D)  /* 14 time quantum */
#define CAN_BS1_15tq                ((u8)0x0E)  /* 15 time quantum */
#define CAN_BS1_16tq                ((u8)0x0F)  /* 16 time quantum */		//ʱ���1Ϊ16��ʱ�䵥λ

#define IS_CAN_BS1(BS1) ((BS1) <= CAN_BS1_16tq)

/* time quantum in bit segment 2 */	//ʱ���2(BS2)�����巢�͵��λ�á�������CAN��׼���PHASE_SEG2��
																		//��ֵ���Ա��Ϊ1��8��ʱ�䵥Ԫ����Ҳ���Ա��Զ������Բ�����λ�ĸ���Ư�ơ�
#define CAN_BS2_1tq                 ((u8)0x00)  /* 1 time quantum */		//ʱ���2Ϊ1��ʱ�䵥λ
#define CAN_BS2_2tq                 ((u8)0x01)  /* 2 time quantum */
#define CAN_BS2_3tq                 ((u8)0x02)  /* 3 time quantum */
#define CAN_BS2_4tq                 ((u8)0x03)  /* 4 time quantum */
#define CAN_BS2_5tq                 ((u8)0x04)  /* 5 time quantum */
#define CAN_BS2_6tq                 ((u8)0x05)  /* 6 time quantum */
#define CAN_BS2_7tq                 ((u8)0x06)  /* 7 time quantum */
#define CAN_BS2_8tq                 ((u8)0x07)  /* 8 time quantum */		//ʱ���2Ϊ8��ʱ�䵥λ

#define IS_CAN_BS2(BS2) ((BS2) <= CAN_BS2_8tq)

/* CAN clock prescaler */
#define IS_CAN_PRESCALER(PRESCALER) (((PRESCALER) >= 1) && ((PRESCALER) <= 1024))

/* CAN filter number */
#define IS_CAN_FILTER_NUMBER(NUMBER) ((NUMBER) <= 13)

/* CAN filter mode */
#define CAN_FilterMode_IdMask       ((u8)0x00)  /* id/mask mode */					//��ʶ������λģʽ
#define CAN_FilterMode_IdList       ((u8)0x01)  /* identifier list mode */	//��ʶ���б�ģʽ---Ϊ�˹��˳�һ����ʶ����Ӧ�����ù������鹤���ڱ�ʶ���б�ģʽ��

#define IS_CAN_FILTER_MODE(MODE) (((MODE) == CAN_FilterMode_IdMask) || \
                                  ((MODE) == CAN_FilterMode_IdList))

/* CAN filter scale */
#define CAN_FilterScale_16bit       ((u8)0x00) /* 16-bit filter scale */		//2��16λ������
#define CAN_FilterScale_32bit       ((u8)0x01) /* 2-bit filter scale */			//1��32λ������

#define IS_CAN_FILTER_SCALE(SCALE) (((SCALE) == CAN_FilterScale_16bit) || \
                                    ((SCALE) == CAN_FilterScale_32bit))

/* CAN filter FIFO assignation */
#define CAN_FilterFIFO0             ((u8)0x00)  /* Filter FIFO 0 assignment for filter x */		//������FIFO0ָ�������0
#define CAN_FilterFIFO1             ((u8)0x01)  /* Filter FIFO 1 assignment for filter x */		//������FIFO0ָ�������1

#define IS_CAN_FILTER_FIFO(FIFO) (((FIFO) == CAN_FilterFIFO0) || \
                                  ((FIFO) == CAN_FilterFIFO1))

/* CAN Tx */
#define IS_CAN_TRANSMITMAILBOX(TRANSMITMAILBOX) ((TRANSMITMAILBOX) <= ((u8)0x02))
#define IS_CAN_STDID(STDID)   ((STDID) <= ((u32)0x7FF))
#define IS_CAN_EXTID(EXTID)   ((EXTID) <= ((u32)0x1FFFFFFF))
#define IS_CAN_DLC(DLC)       ((DLC) <= ((u8)0x08))

/* CAN identifier type */
#define CAN_ID_STD                 ((u32)0x00000000)  /* Standard Id */		//ʹ�ñ�׼֡��ʶ��
#define CAN_ID_EXT                 ((u32)0x00000004)  /* Extended Id */		//ʹ����չ֡��ʶ��

#define IS_CAN_IDTYPE(IDTYPE) (((IDTYPE) == CAN_ID_STD) || ((IDTYPE) == CAN_ID_EXT))

/* CAN remote transmission request */
#define CAN_RTR_DATA                ((u32)0x00000000)  /* Data frame */		//����֡
#define CAN_RTR_REMOTE              ((u32)0x00000002)  /* Remote frame */	//Զ��֡

#define IS_CAN_RTR(RTR) (((RTR) == CAN_RTR_DATA) || ((RTR) == CAN_RTR_REMOTE))

/* CAN transmit constants */
#define CANTXFAILED                 ((u8)0x00) /* CAN transmission failed */
#define CANTXOK                     ((u8)0x01) /* CAN transmission succeeded */
#define CANTXPENDING                ((u8)0x02) /* CAN transmission pending */
#define CAN_NO_MB                   ((u8)0x04) /* CAN cell did not provide an empty mailbox */

/* CAN receive FIFO number constants */
#define CAN_FIFO0                 ((u8)0x00) /* CAN FIFO0 used to receive */
#define CAN_FIFO1                 ((u8)0x01) /* CAN FIFO1 used to receive */

#define IS_CAN_FIFO(FIFO) (((FIFO) == CAN_FIFO0) || ((FIFO) == CAN_FIFO1))

/* CAN sleep constants */
#define CANSLEEPFAILED              ((u8)0x00) /* CAN did not enter the sleep mode */
#define CANSLEEPOK                  ((u8)0x01) /* CAN entered the sleep mode */

/* CAN wake up constants */
#define CANWAKEUPFAILED             ((u8)0x00) /* CAN did not leave the sleep mode */
#define CANWAKEUPOK                 ((u8)0x01) /* CAN leaved the sleep mode */

/* CAN flags */
#define CAN_FLAG_EWG                ((u32)0x00000001) /* Error Warning Flag */
#define CAN_FLAG_EPV                ((u32)0x00000002) /* Error Passive Flag */
#define CAN_FLAG_BOF                ((u32)0x00000004) /* Bus-Off Flag */

#define IS_CAN_FLAG(FLAG) (((FLAG) == CAN_FLAG_EWG) || ((FLAG) == CAN_FLAG_EPV) ||\
                           ((FLAG) == CAN_FLAG_BOF))

/* CAN interrupts */					//CAN�ж�Դ
#define CAN_IT_RQCP0                ((u32)0x00000005) /* Request completed mailbox 0 */			//����1�������
#define CAN_IT_RQCP1                ((u32)0x00000006) /* Request completed mailbox 1 */			//����2�������
#define CAN_IT_RQCP2                ((u32)0x00000007) /* Request completed mailbox 2 */			//����3�������			
#define CAN_IT_TME                  ((u32)0x00000001) /* Transmit mailbox empty */					//����������ж�����
#define CAN_IT_FMP0                 ((u32)0x00000002) /* FIFO 0 message pending */					//FIFO0��Ϣ�Һ��ж�����
#define CAN_IT_FF0                  ((u32)0x00000004) /* FIFO 0 full */											//FIFO0���ж�����
#define CAN_IT_FOV0                 ((u32)0x00000008) /* FIFO 0 overrun */									//FIFO0����ж�����
#define CAN_IT_FMP1                 ((u32)0x00000010) /* FIFO 1 message pending */					//FIFO1��Ϣ�Һ��ж�����
#define CAN_IT_FF1                  ((u32)0x00000020) /* FIFO 1 full */											//FIFO1���ж�����
#define CAN_IT_FOV1                 ((u32)0x00000040) /* FIFO 1 overrun */									//FIFO1����ж�����
#define CAN_IT_EWG                  ((u32)0x00000100) /* Error warning */										//���󾯸��ж�����
#define CAN_IT_EPV                  ((u32)0x00000200) /* Error passive */										//���󱻶��ж�����
#define CAN_IT_BOF                  ((u32)0x00000400) /* Bus-off */													//�����ж�����
#define CAN_IT_LEC                  ((u32)0x00000800) /* Last error code */									//�ϴδ�����ж�����
#define CAN_IT_ERR                  ((u32)0x00008000) /* Error */														//�����ж�����
#define CAN_IT_WKU                  ((u32)0x00010000) /* Wake-up */													//�����ж�����
#define CAN_IT_SLK                  ((u32)0x00020000) /* Sleep */														//˯�߱�־λ�ж�����

#define IS_CAN_ITConfig(IT) (((IT) == CAN_IT_TME)   || ((IT) == CAN_IT_FMP0)  ||\
                             ((IT) == CAN_IT_FF0)   || ((IT) == CAN_IT_FOV0)  ||\
                             ((IT) == CAN_IT_FMP1)  || ((IT) == CAN_IT_FF1)   ||\
                             ((IT) == CAN_IT_FOV1)  || ((IT) == CAN_IT_EWG)   ||\
                             ((IT) == CAN_IT_EPV)   || ((IT) == CAN_IT_BOF)   ||\
                             ((IT) == CAN_IT_LEC)   || ((IT) == CAN_IT_ERR)   ||\
                             ((IT) == CAN_IT_WKU)   || ((IT) == CAN_IT_SLK))

#define IS_CAN_ITStatus(IT) (((IT) == CAN_IT_RQCP0)  || ((IT) == CAN_IT_RQCP1)  ||\
                             ((IT) == CAN_IT_RQCP2)  || ((IT) == CAN_IT_FF0)    ||\
                             ((IT) == CAN_IT_FOV0)   || ((IT) == CAN_IT_FF1)    ||\
                             ((IT) == CAN_IT_FOV1)   || ((IT) == CAN_IT_EWG)    ||\
                             ((IT) == CAN_IT_EPV)    || ((IT) == CAN_IT_BOF)    ||\
                             ((IT) == CAN_IT_WKU)    || ((IT) == CAN_IT_SLK))

/* Exported macro ------------------------------------------------------------*/
/* Exported function protypes ----------------------------------------------- */
void CAN_DeInit(void);																						//������CAN��ȫ���Ĵ�������Ϊȱʡֵ
u8 CAN_Init(CAN_InitTypeDef* CAN_InitStruct);											//����CAN_InitStruct��ָ���Ĳ�����ʼ������CAN�ļĴ���
void CAN_FilterInit(CAN_FilterInitTypeDef* CAN_FilterInitStruct);	//����CAN_FilterInitStruct��ָ���Ĳ�����ʼ������CAN�ļĴ���
void CAN_StructInit(CAN_InitTypeDef* CAN_InitStruct);
void CAN_ITConfig(u32 CAN_IT, FunctionalState NewState);					//ʹ�ܻ���ʧ��ָ����CAN�ж�,�������CAN_ITΪ��ʹ�ܻ���ʧ�ܵ�CAN�жϡ�
u8 CAN_Transmit(CanTxMsg* TxMessage);															//��ʼһ����Ϣ�Ĵ���
u8 CAN_TransmitStatus(u8 TransmitMailbox);												//�����Ϣ�����״̬
void CAN_CancelTransmit(u8 Mailbox);															//ȡ��һ����������
void CAN_FIFORelease(u8 FIFONumber);															//�ͷ�һ��FIFO
u8 CAN_MessagePending(u8 FIFONumber);															//���عҺŵ���Ϣ����
void CAN_Receive(u8 FIFONumber, CanRxMsg* RxMessage);							//����һ����Ϣ
u8 CAN_Sleep(void);																								//ʹCAN����͹���ģʽ
u8 CAN_WakeUp(void);																							//��CAN����
FlagStatus CAN_GetFlagStatus(u32 CAN_FLAG);												//���ָ����CAN��־λ���������
void CAN_ClearFlag(u32 CAN_FLAG);																	//���CAN�Ĵ������־λ
ITStatus CAN_GetITStatus(u32 CAN_IT);															//���ָ����CAN�жϷ������
void CAN_ClearITPendingBit(u32 CAN_IT);														//���CAN�жϴ������־λ

#endif /* __STM32F10x_CAN_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
