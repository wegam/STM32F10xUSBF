/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_def.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Definitions related to USB Core
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DEF_H
#define __USB_DEF_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum _RECIPIENT_TYPE
{
  DEVICE_RECIPIENT,     //DEVICE_RECIPIENT==00	�����豸/* Recipient device */
  INTERFACE_RECIPIENT,  //INTERFACE_RECIPIENT==01	/* Recipient interface */
  ENDPOINT_RECIPIENT,   //ENDPOINT_RECIPIENT==02	/* Recipient endpoint */
  OTHER_RECIPIENT				//OTHER_RECIPIENT==03
} RECIPIENT_TYPE;


typedef enum _STANDARD_REQUESTS
{
  GET_STATUS = 0,			//GET_STATUS==00���������ض������ߵ�״̬
  CLEAR_FEATURE,			//CLEAR_FEATURE==01����������ֹ�����ߵ�ĳЩ����
  RESERVED1,							
  SET_FEATURE,				//SET_FEATURE==03�������û򼤻���������ߵ�ĳЩ����				
  RESERVED2,
  SET_ADDRESS,				//SET_ADDRESS==05�������豸�����ַ
  GET_DESCRIPTOR,			//GET_DESCRIPTOR==06����������ȡ�豸���ض�������
  SET_DESCRIPTOR,			//SET_DESCRIPTOR==07�޸��豸���йص������������������µ�������
  GET_CONFIGURATION,	//GET_CONFIGURATION==08����������ȡ�豸��ǰ�豸������ֵ��עͬ����Ĳ�ͬ�� 
  SET_CONFIGURATION,	//SET_CONFIGURATION==09��������ָʾ�豸���õ�Ҫ�������
  GET_INTERFACE,			//GET_INTERFACE==0A���ڻ�ȡ��ǰĳ���ӿ����������
  SET_INTERFACE,			//SET_INTERFACE==0B��������Ҫ���豸��ĳ���������������ӿ�
  TOTAL_sREQUEST,  /* Total number of Standard request */
  SYNCH_FRAME = 12		//SYNCH_FRAME==0C�����豸���úͱ���һ���˵��ͬ��֡
} STANDARD_REQUESTS;

/* Definition of "USBwValue" */
typedef enum _DESCRIPTOR_TYPE
{
  DEVICE_DESCRIPTOR = 1,		//DEVICE_DESCRIPTOR==01	�豸������
  CONFIG_DESCRIPTOR,				//CONFIG_DESCRIPTOR==02	����������
  STRING_DESCRIPTOR,				//STRING_DESCRIPTOR==03	�ַ���������
  INTERFACE_DESCRIPTOR,			//INTERFACE_DESCRIPTOR==04	�ӿ�������
  ENDPOINT_DESCRIPTOR				//ENDPOINT_DESCRIPTOR==05	�˵�������
} DESCRIPTOR_TYPE;

/* Feature selector of a SET_FEATURE or CLEAR_FEATURE */
typedef enum _FEATURE_SELECTOR
{
  ENDPOINT_STALL,
  DEVICE_REMOTE_WAKEUP
} FEATURE_SELECTOR;

/* Exported constants --------------------------------------------------------*/
/* Definition of "USBbmRequestType" */
#define REQUEST_TYPE      0x60  /* Mask to get request type */
#define STANDARD_REQUEST  0x00 	// STANDARD_REQUEST == 00 ��׼���� /* Standard request */		// STANDARD_REQUEST == 00 ��׼����
#define CLASS_REQUEST     0x20  /* Class request */
#define VENDOR_REQUEST    0x40  /* Vendor request */

#define RECIPIENT         0x1F  /* Mask to get recipient */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __USB_DEF_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
