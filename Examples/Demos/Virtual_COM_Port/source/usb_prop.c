/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_prop.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : All processing related to Virtual Com Port Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


/******************************** ˵��20160912**********************************
********************************************************************************
* ���ܣ������ϲ�Э�鴦������ HIDЭ�飬�������洢�豸Э��
* 
* 
* 
* 
* 
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 Request = 0;

LINE_CODING linecoding =
  {
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* no. of bits 8*/
  };

/* -------------------------------------------------------------------------- */
/*  Structures initializations */
/* -------------------------------------------------------------------------- */

DEVICE Device_Table =
  {
    EP_NUM,				//��ʹ�õĶ˵���
    1							//����ʹ�õĶ˵���
  };

/*�豸����*/
DEVICE_PROP Device_Property =				//
  {
    Virtual_Com_Port_init,									//Virtual_Com_Port�ĳ�ʼ������
    Virtual_Com_Port_Reset,									//Virtual_Com_Port�ĸ�λ����
    Virtual_Com_Port_Status_In,							//CustomVirtual_Com_PortHID״̬���뺯��
    Virtual_Com_Port_Status_Out,						//CustomHID״̬�������
    Virtual_Com_Port_Data_Setup,						//CustomHID�Ĵ��������ݽ׶ε�������������
    Virtual_Com_Port_NoData_Setup,					//CustomHID�Ĵ���û�����ݽ׶ε�������������
    Virtual_Com_Port_Get_Interface_Setting,	//CustomHID��ȡ�ӿڼ����ýӿ����ã��Ƿ���ã�
    Virtual_Com_Port_GetDeviceDescriptor,		//CustomHID��ȡ�豸������
    Virtual_Com_Port_GetConfigDescriptor,		//CustomHID��ȡ����������
    Virtual_Com_Port_GetStringDescriptor,		//CustomHID��ȡ�ַ���������
    0,																			//��ǰ��δʹ��
    0x40 /*MAX PACKET SIZE*/								//���İ�����Ϊ64�ֽ�
  };

/*ע��USB��׼�����ʵ�ֺ���*/
USER_STANDARD_REQUESTS User_Standard_Requests =		
  {
    Virtual_Com_Port_GetConfiguration,			//��ȡ��������
    Virtual_Com_Port_SetConfiguration,			//������������
    Virtual_Com_Port_GetInterface,					//��ȡ�ӿ�����
    Virtual_Com_Port_SetInterface,					//���ýӿ�����
    Virtual_Com_Port_GetStatus,							//��ȡ״̬����
    Virtual_Com_Port_ClearFeature,					//�����������
    Virtual_Com_Port_SetEndPointFeature,		//���ö˵���������
    Virtual_Com_Port_SetDeviceFeature,			//�����豸��������
    Virtual_Com_Port_SetDeviceAddress				//�����豸��ַ����
  };

/*ע���豸��������Ϣ*/
ONE_DESCRIPTOR Device_Descriptor =
  {
    (u8*)Virtual_Com_Port_DeviceDescriptor,	//ע���豸����������
    VIRTUAL_COM_PORT_SIZ_DEVICE_DESC				//�豸�������ĳ���
  };
	
/*ע���豸��������Ϣ*/
ONE_DESCRIPTOR Qualifier_Descriptor =
  {
    (u8*)USBD_DeviceQualifier,	//ע���豸����������
    10				//�豸�������ĳ���
  };

/*ע��������������Ϣ*/
ONE_DESCRIPTOR Config_Descriptor =
  {
    (u8*)Virtual_Com_Port_ConfigDescriptor,										//ע����������������
    VIRTUAL_COM_PORT_SIZ_CONFIG_DESC												//�����������ĳ���
  };

/*ע���ַ�������������������ID�����̡���Ʒ�����к�������*/
ONE_DESCRIPTOR String_Descriptor[4] =
  {
    {(u8*)Virtual_Com_Port_StringLangID, VIRTUAL_COM_PORT_SIZ_STRING_LANGID},		//ע�������ַ�������������
    {(u8*)Virtual_Com_Port_StringVendor, VIRTUAL_COM_PORT_SIZ_STRING_VENDOR},		//ע�᳧���ַ�������������
    {(u8*)Virtual_Com_Port_StringProduct, VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT},	//ע���Ʒ�ַ�������������
    {(u8*)Virtual_Com_Port_StringSerial, VIRTUAL_COM_PORT_SIZ_STRING_SERIAL}		//ע�����к��ַ�������������
  };

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Virtual_Com_Port_init.
* Description    : Virtual COM Port Mouse init routine.	��ʼ��
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_init(void)
{

  /* Update the serial number string descriptor with the data from the unique
  ID*/
  Get_SerialNum();	//**************************************��ȡ���к�,����оƬ���кţ����������е�����STM ���ַ����޸�û̫�����塣

  pInformation->Current_Configuration = 0;	//����״̬

  /* Connect the device */
  PowerOn();				//*******�ϵ�usb_pwr.c->Line59:����USB������ǿ��USB��λ����������ж�ʹ�ܱ�־��CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM;		//���������ж�
  /* USB interrupts initialization */
  /* clear pending interrupts */
  _SetISTR(0);			//**************************************ִ�л����ĳ�ʼ������������˵�豸IP�Ͷ˵�0�ĳ�ʼ��
  wInterrupt_Mask = IMR_MSK;		//#define IMR_MSK (CNTR_CTRM  | CNTR_SOFM  | CNTR_RESETM )
																//CNTR_CTRM����ȷ����(CTR)�ж�����λ (Correct transfer interrupt mask)
																//CNTR_SOFM��֡���ж�����λ (Start of frame interrupt mask)
																//CNTR_RESETM��USB��λ�ж�����λ (USB reset interrupt mask)
  /* set interrupts mask */
  _SetCNTR(wInterrupt_Mask);		////ʹ����Ӧ�ж�

  /* configure the USART 1 to the default settings */
  USART_Config_Default();		//���ô�����ȱʡ״̬---�����ﲨ���ʱ���Ϊ9600�����������˽����жϡ������ж�û������

  bDeviceState = UNCONNECTED;		//usb_pwr.h->DEVICE_STATE**************************����ǰ��״̬����Ϊδ����״̬
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Reset
* Description    : Virtual_Com_Port Mouse reset routine	��λ
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_Reset(void)
{
  /* Set Virtual_Com_Port DEVICE as not configured */
  pInformation->Current_Configuration = 0;				//���õ�ǰ������Ϊ0����ʾû�����ù�

  /* Current Feature initialization */
  pInformation->Current_Feature = Virtual_Com_Port_ConfigDescriptor[7];	//��ǰ�����ԣ�bmAttributes:�豸��һЩ���ԣ�0xc0��ʾ�Թ��磬��֧��Զ�̻���

  /* Set Virtual_Com_Port DEVICE with the default Interface*/
  pInformation->Current_Interface = 0;
  SetBTABLE(BTABLE_ADDRESS);

  /* Initialize Endpoint 0 */
	/* *****��ʼ���˵�0***** */
  SetEPType(ENDP0, EP_CONTROL);													//���ö˵�0Ϊ���ƶ˵�
  SetEPTxStatus(ENDP0, EP_TX_STALL);										//���ö˵�0������ʱ
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);											//���ö˵�0�Ľ��ջ�������ַ
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);											//���ö˵�0�ķ��ͻ�������ַ
  Clear_Status_Out(ENDP0);															//����˵�0��״̬
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);		//���ö˵�0�Ľ�������
  SetEPRxValid(ENDP0);																	//ʹ�ܽ���״̬

  /* Initialize Endpoint 1 */
	/* *****��ʼ���˵�1***** */
  SetEPType(ENDP1, EP_BULK);														//���ö˵�1Ϊ����������
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);											//���ö˵㷢�͵�ַ
  SetEPTxStatus(ENDP1, EP_TX_NAK);											//���ö˵�1�ķ��Ͳ���Ӧ
  SetEPRxStatus(ENDP1, EP_RX_DIS);											//���ö˵�1������

  /* Initialize Endpoint 2 */
	/* *****��ʼ���˵�2***** */
  SetEPType(ENDP2, EP_INTERRUPT);												//���ö˵�2Ϊ�жϴ���
  SetEPTxAddr(ENDP2, ENDP2_TXADDR);											//���ö˵�2���͵�ַ
  SetEPRxStatus(ENDP2, EP_RX_DIS);											//���ö˵�2������״̬
  SetEPTxStatus(ENDP2, EP_TX_NAK);											//���ö˵�2�˵�2Ϊ���ղ���Ӧ

  /* Initialize Endpoint 3 */
	/* *****��ʼ���˵�3***** */
  SetEPType(ENDP3, EP_BULK);														//���ö˵�3Ϊ����������
  SetEPRxAddr(ENDP3, ENDP3_RXADDR);											//���ö˵�3���յ�ַ
  SetEPRxCount(ENDP3, VIRTUAL_COM_PORT_DATA_SIZE);			//���ö˵�3�ļ���ֵ
  SetEPRxStatus(ENDP3, EP_RX_VALID);										//���ö˵�3������Ч
  SetEPTxStatus(ENDP3, EP_TX_DIS);											//���ö˵�3������

  /* Set this device to response on default address */
  SetDeviceAddress(0);																	//�����豸ΪĬ�ϵ�ַΪ0

  bDeviceState = ATTACHED;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_SetConfiguration.
* Description    : Udpade the device state to configured.�����豸����״̬
* ����			    	: ����
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_SetConfiguration(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_SetConfiguration.
* Description    : Udpade the device state to addressed.�����豸�ı�ַ״̬
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Status_In.
* Description    : Virtual COM Port Status In Routine.״̬���뺯��
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_Status_In(void)
{
  if (Request == SET_LINE_CODING)
  {
    USART_Config();
    Request = 0;
  }
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Status_Out
* Description    : Virtual COM Port Status OUT Routine.״̬�������
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_Status_Out(void)
{}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Data_Setup
* Description    : handle the data class specific requests ���������ݽ׶ε�����������
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT Virtual_Com_Port_Data_Setup(u8 RequestNo)
{
  u8    *(*CopyRoutine)(u16);

  CopyRoutine = NULL;
	//������ֶ�
  if (RequestNo == GET_LINE_CODING)		//****************************��ȡ����ͨ����Ϣ����														
  {
    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))	//����������Ľ������ǽӿ�
    {
      CopyRoutine = Virtual_Com_Port_GetLineCoding;	//**************ָ�뺯��ָ��Virtual_Com_Port_GetLineCoding����
    }
  }
  else if (RequestNo == SET_LINE_CODING)	//************************���ô���ͨѶ��Ϣ����
  {
    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))	//����������Ľ������ǽӿ�
    {
      CopyRoutine = Virtual_Com_Port_SetLineCoding;	//**************ָ�뺯��ָ��Virtual_Com_Port_SetLineCoding����
    }
    Request = SET_LINE_CODING;
  }

  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  }

  pInformation->Ctrl_Info.CopyData = CopyRoutine;		//**************ע��ָ��ָ��ĺ���
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);					//************************************���øú���
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_NoData_Setup.
* Description    : handle the no data class specific requests.����û�����ݽ׶�����������
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT Virtual_Com_Port_NoData_Setup(u8 RequestNo)
{

  if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))	//***����������Ľ������ǽӿ�
  {
    if (RequestNo == SET_COMM_FEATURE)		//*************************�������⴮������
    {
      return USB_SUCCESS;
    }
    else if (RequestNo == SET_CONTROL_LINE_STATE)		//***************���ÿ�����Ϣ״̬
    {
      return USB_SUCCESS;
    }
  }

  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetDeviceDescriptor.
* Description    : Gets the device descriptor. 	����������--��ȡ�豸������
* Input          : Length.
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
u8 *Virtual_Com_Port_GetDeviceDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}
/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetDeviceDescriptor.
* Description    : Gets the device descriptor. 	����������--��ȡ�豸������
* Input          : Length.
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
u8 *Qualifier_GetDeviceDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(10,&Qualifier_Descriptor);
}
/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetConfigDescriptor.
* Description    : get the configuration descriptor.����������--��ȡ����������
* Input          : Length.
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
u8 *Virtual_Com_Port_GetConfigDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetStringDescriptor
* Description    : Gets the string descriptors according to the needed index	����������--����������ȡ�ַ�������
* Input          : Length.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
u8 *Virtual_Com_Port_GetStringDescriptor(u16 Length)
{
  u8 wValue0 = pInformation->USBwValue0;
  if (wValue0 > 4)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Get_Interface_Setting.
* Description    : test the interface and the alternate setting according to the
*                  supported one.		���Խӿڼ��䱸�ýӿ��Ƿ����
* Input1         : u8: Interface : interface number.
* Input2         : u8: AlternateSetting : Alternate Setting number.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
RESULT Virtual_Com_Port_Get_Interface_Setting(u8 Interface, u8 AlternateSetting)
{
  if (AlternateSetting > 0)		//���õı�Ŵ���0
  {
    return USB_UNSUPPORT;
  }
  else if (Interface > 1)			//�ӿڵı�Ŵ���1
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetLineCoding.
* Description    : send the linecoding structure to the PC host.	����linecoding�ṹ�嵽PC��
* Input          : Length.
* Output         : None.
* Return         : Inecoding structure base address.  Linecoding�ṹ��Ļ���ַ
*******************************************************************************/
u8 *Virtual_Com_Port_GetLineCoding(u16 Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = sizeof(linecoding);
    return NULL;
  }
  return(u8 *)&linecoding;			//����ͨ����Ϣ
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_SetLineCoding.
* Description    : Set the linecoding structure fields.	����linecoding�ṹ��
* Input          : Length.
* Output         : None.
* Return         : Linecoding structure base address.		Linecoding�ṹ��Ļ���ַ.
*******************************************************************************/
u8 *Virtual_Com_Port_SetLineCoding(u16 Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = sizeof(linecoding);
    return NULL;
  }
  return(u8 *)&linecoding;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

