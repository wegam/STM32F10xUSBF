/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_int.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Endpoint CTR (Low and High) interrupt's service routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u16 SaveRState;
u16 SaveTState;

/* Extern variables ----------------------------------------------------------*/
extern void (*pEpInt_IN[7])(void);    /*  Handles IN  interrupts   */
extern void (*pEpInt_OUT[7])(void);   /*  Handles OUT interrupts   */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : CTR_LP.
* Description    : ����˵����ݴ�������ж�
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CTR_LP(void)			//�����ã�usb_istr.c			//���ڶ˵��������������жϴ�
{
  u32 wEPVal = 0;
  /* stay in loop while pending ints */
  while (((wIstr = _GetISTR()) & ISTR_CTR) != 0)	//��ȡ��ȷ�Ĵ���
  {
    _SetISTR((u16)CLR_CTR); /* clear CTR flag */	//�����ȷ�Ĵ����־ (Correct transfer)
    /* extract highest priority endpoint number */
    EPindex = (u8)(wIstr & ISTR_EP_ID);						//�ҵ��ڵ�������//�˵�ID (Endpoint Identifier)////��ȡ���ݴ�����ԵĶ˵��
    if (EPindex == 0)			//0�Ŷ˵�--���ƴ���
    {
      /* Decode and service control endpoint interrupt */
      /* calling related service routine */
      /* (Setup0_Process, In0_Process, Out0_Process) */

      /* save RX & TX status */
      /* and set both to NAK */
      SaveRState = _GetEPRxStatus(ENDP0);		//������ն˵�״̬
      SaveTState = _GetEPTxStatus(ENDP0);		//���淢�Ͷ˵�״̬
      _SetEPRxStatus(ENDP0, EP_RX_NAK);			//�˵���NAK������Ӧ���еķ�������
      _SetEPTxStatus(ENDP0, EP_TX_NAK);			//�˵���NAK������Ӧ���еĽ�������


      /* DIR bit = origin of the interrupt */
			//DIR��0:��־һ��IN����(���ݴ�USBģ�鴫�䵽PC����)�Ĵ������;DIR��1:��־һ��OUT����(���ݴ�PC�������䵽USBģ��)�Ĵ������
      if ((wIstr & ISTR_DIR) == 0)		//����//���䷽�� (Direction of transaction)
      {
        /* DIR = 0 */

        /* DIR = 0      => IN  int */
        /* DIR = 0 implies that (EP_CTR_TX = 1) always  */


        _ClearEP_CTR_TX(ENDP0);		//������ն˵�״̬
        In0_Process();						//usb_core.c//�˵�0���ʹ��������������գ�

           /* before terminate set Tx & Rx status */
          _SetEPRxStatus(ENDP0, SaveRState);		//��ԭ״̬
          _SetEPTxStatus(ENDP0, SaveTState);		//��ԭ״̬
          return;
      }
      else			//����
      {
        /* DIR = 1 */

        /* DIR = 1 & CTR_RX       => SETUP or OUT int */
        /* DIR = 1 & (CTR_TX | CTR_RX) => 2 int pending */

        wEPVal = _GetENDPOINT(ENDP0);		//��ȡ�˵�״̬
        if ((wEPVal & EP_CTR_TX) != 0)	//���ڷ���״̬
        {
          _ClearEP_CTR_TX(ENDP0);
          In0_Process();						//usb_core.c//�˵�0���ʹ��������������գ�
          /* before terminate set Tx & Rx status */
          _SetEPRxStatus(ENDP0, SaveRState);
          _SetEPTxStatus(ENDP0, SaveTState);
          return;
        }
        else if ((wEPVal &EP_SETUP) != 0)		//������״̬
        {
          _ClearEP_CTR_RX(ENDP0); /* SETUP bit kept frozen while CTR_RX = 1 */
          Setup0_Process();				//usb_core.c//������
          /* before terminate set Tx & Rx status */
          _SetEPRxStatus(ENDP0, SaveRState);
          _SetEPTxStatus(ENDP0, SaveTState);
          return;
        }

        else if ((wEPVal & EP_CTR_RX) != 0)		//����
        {
          _ClearEP_CTR_RX(ENDP0);
          Out0_Process();			//usb_core.c//�˵�0���봦�������������ͣ�
          /* before terminate set Tx & Rx status */
          _SetEPRxStatus(ENDP0, SaveRState);
          _SetEPTxStatus(ENDP0, SaveTState);
          return;
        }
      }
    }/* if(EPindex == 0) */
    else		//�����ڵ�����
    {
      /* Decode and service non control endpoints interrupt  */

      /* process related endpoint register */
      wEPVal = _GetENDPOINT(EPindex);
      if ((wEPVal & EP_CTR_RX) != 0)		//���ս��ջ�����������
      {
        /* clear int flag */
        _ClearEP_CTR_RX(EPindex);				//������˵�״̬

        /* call OUT service function */
        (*pEpInt_OUT[EPindex-1])();			//�����������

      } /* if((wEPVal & EP_CTR_RX) */

      if ((wEPVal & EP_CTR_TX) != 0)
      {
        /* clear int flag */
        _ClearEP_CTR_TX(EPindex);

        /* call IN service function */
        (*pEpInt_IN[EPindex-1])();
      } /* if((wEPVal & EP_CTR_TX) != 0) */

    }/* if(EPindex == 0) else */

  }/* while(...) */
}

/*******************************************************************************
* Function Name  : CTR_HP.
* Description    : High Priority Endpoint Correct Transfer interrupt's service 
*                  routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CTR_HP(void)
{
  u32 wEPVal = 0;

  while (((wIstr = _GetISTR()) & ISTR_CTR) != 0)
  {
    _SetISTR((u16)CLR_CTR); /* clear CTR flag */
    /* extract highest priority endpoint number */
    EPindex = (u8)(wIstr & ISTR_EP_ID);
    /* process related endpoint register */
    wEPVal = _GetENDPOINT(EPindex);
    if ((wEPVal & EP_CTR_RX) != 0)
    {
      /* clear int flag */
      _ClearEP_CTR_RX(EPindex);

      /* call OUT service function */
      (*pEpInt_OUT[EPindex-1])();

    } /* if((wEPVal & EP_CTR_RX) */
    else if ((wEPVal & EP_CTR_TX) != 0)
    {
      /* clear int flag */
      _ClearEP_CTR_TX(EPindex);

      /* call IN service function */
      (*pEpInt_IN[EPindex-1])();


    } /* if((wEPVal & EP_CTR_TX) != 0) */

  }/* while(...) */
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
