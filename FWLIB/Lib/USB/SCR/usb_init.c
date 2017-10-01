/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_init.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Initialization routines & global variables
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
#include "usb_desc.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/*  The number of current endpoint, it will be used to specify an endpoint */
 u8	EPindex;
/*  The number of current device, it is an index to the Device_Table */
/* u8	Device_no; */
/*  Points to the DEVICE_INFO structure of current device */
/*  The purpose of this register is to speed up the execution */
DEVICE_INFO *pInformation;		//�豸״̬
/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */
DEVICE_PROP *pProperty;
/*  Temporary save the state of Rx & Tx status. */
/*  Whenever the Rx or Tx state is changed, its value is saved */
/*  in this variable first and will be set to the EPRB or EPRA */
/*  at the end of interrupt process */
u16	SaveState ;
u16  wInterrupt_Mask;
DEVICE_INFO	Device_Info;
USER_STANDARD_REQUESTS  *pUser_Standard_Requests;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : USB_Init
* Description    : USB system initialization
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Init(void)
{
  pInformation = &Device_Info;			//������豸���ϲ���,�����ȫ����������̬��	//pInformation ��һ��ָ��Device_Info ��ָ��
  pInformation->ControlState = 2;
  pProperty = &Device_Property;			//pProperty ��һ��ָ�룬ָ��DEVICE_PROP ������豸���Բ��֣���usb_prop.c->Line 60
																		//�豸���Բ��ְ���һЩ��������������Ҳ��������������һ���ǽ������Ļ�������ַ��һ��
																		//�����İ��ĳ��ȡ��������ֽڱ�ʾ�ġ�
																		
  pUser_Standard_Requests = &User_Standard_Requests;		//��׼����
  /* Initialize devices one by one */
  pProperty->Init();			//��ʼ��--��ʼ����ɺ󣬺�����¼��������жϴ���������,usb_istr.c->USB_Istr->CTR_LP()		
													//usb_prop.c->Device_Property->Virtual_Com_Port_init;	Virtual_Com_Port_init
													//���������һ��ͨ�õ����ԣ���ָ���൱��ʵ�����׵�USB ���ͷ��ȥ�������������￴
													//��ȥ����һ��pProperty->Init();ʵ�������е���DEVICE_PROP �е�
													//Virtual_Com_Port_init�������������
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
