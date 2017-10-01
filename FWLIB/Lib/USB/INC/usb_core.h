/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_core.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Standard protocol processing functions prototypes
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_CORE_H
#define __USB_CORE_H
#include "usb_type.h"			//��Ҫ���� typedefΪ stm32֧�ֵ���������ȡһЩ�µ�����/
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum _CONTROL_STATE
{
  WAIT_SETUP,       /* 0 */		//�ȴ�SETUP
  SETTING_UP,       /* 1 */		//����SETUP
  IN_DATA,          /* 2 */
  OUT_DATA,         /* 3 */
  LAST_IN_DATA,     /* 4 */		//��������
  LAST_OUT_DATA,    /* 5 */		//��������
  WAIT_STATUS_IN,   /* 7 */		//�ȴ�״̬
  WAIT_STATUS_OUT,  /* 8 */		//�ȴ�״̬
  STALLED,          /* 9 */		//��ֹ���ͺͽ���
  PAUSE             /* 10 */
} CONTROL_STATE;    /* The state machine states of a control pipe */

typedef struct OneDescriptor
{
  u8 *Descriptor;
  u16 Descriptor_Size;
}
ONE_DESCRIPTOR, *PONE_DESCRIPTOR;
/* All the request process routines return a value of this type
   If the return value is not SUCCESS or NOT_READY,
   the software will STALL the correspond endpoint */
typedef enum _RESULT
{
  USB_SUCCESS = 0,    /* Process sucessfully */			//����ɹ�����
  USB_ERROR,																				//����
  USB_UNSUPPORT,																		//����֧��
  USB_NOT_READY       /* The process has not been finished, endpoint will be
                         NAK to further rquest */		//������̻�û�����,
} RESULT;


/*-*-*-*-*-*-*-*-*-*-* Definitions for endpoint level -*-*-*-*-*-*-*-*-*-*-*-*/
typedef struct _ENDPOINT_INFO
{
  /* When send data out of the device,
   CopyData() is used to get data buffer 'Length' bytes data
   if Length is 0,
    CopyData() returns the total length of the data
    if the request is not supported, returns 0
    (NEW Feature )
     if CopyData() returns -1, the calling routine should not proceed
     further and will resume the SETUP process by the class device
   if Length is not 0,
    CopyData() returns a pointer to indicate the data location
   Usb_wLength is the data remain to be sent,
   Usb_wOffset is the Offset of original data
  When receive data from the host,
   CopyData() is used to get user data buffer which is capable
   of Length bytes data to copy data from the endpoint buffer.
   if Length is 0,
    CopyData() returns the available data length,
   if Length is not 0,
    CopyData() returns user buffer address
   Usb_rLength is the data remain to be received,
   Usb_rPointer is the Offset of data buffer
  */
  u16  Usb_wLength;
  u16  Usb_wOffset;
  u16  PacketSize;
  u8   *(*CopyData)(u16 Length);	//����һ������ָ�붨�塣
}ENDPOINT_INFO;

/*-*-*-*-*-*-*-*-*-*-*-* Definitions for device level -*-*-*-*-*-*-*-*-*-*-*-*/
//�ں˽��豸����Ϣ�������豸��ṹ�У��豸��ṹ��DEVICE ���ͣ����Ͷ������£�
typedef struct _DEVICE		//�豸��ṹ--�ں˽��豸����Ϣ�������豸��ṹ��
{
  u8 Total_Endpoint;     /* Number of endpoints that are used */	//����USB Ӧ��ʹ�õĶ˵�����
  u8 Total_Configuration;/* Number of configuration available */	//����USB Ӧ��ʹ�õ���������
}
DEVICE;

typedef union		//Ϊ�˼򻯶�DEVICE_INFO �ṹ�е�ĳЩ�ֶεķ��ʣ���u16 ��u8 ��ʽ��, ������һ��������u16_u8��
{
  u16 w;
  struct BW
  {
    u8 bb1;
    u8 bb0;
  }
  bw;
} u16_u8;

typedef struct _DEVICE_INFO		//�豸��Ϣ�ṹ--USB �ں˽��������͹���������ʵ��USB �豸�����ð��������豸��Ϣ�ṹ�У��ýṹ������
{
  u8 USBbmRequestType;       /* bmRequestType */		//���ð��е�bmRequestType �ĸ���
  u8 USBbRequest;            /* bRequest */					//���ð��е�bRequest �ĸ���
  u16_u8 USBwValues;         /* wValue */						//���ð��е�wValue �ĸ���
  u16_u8 USBwIndexs;         /* wIndex */						//���ð��е�wIndex �ĸ���
  u16_u8 USBwLengths;        /* wLength */

  u8 ControlState;           /* of type CONTROL_STATE */			//�ں˵�״̬
  u8 Current_Feature;																					//��ӳ�˵�ǰ�豸������
  u8 Current_Configuration;   /* Selected configuration */		//ָ�������豸������
  u8 Current_Interface;       /* Selected interface of current configuration */		//��ѡ�еĽӿڡ�
  u8 Current_AlternateSetting;/* Selected Alternate Setting of current						//��Ϊ��ǰ�������úͽӿڵ�ѡ�еı�������
                                     interface*/

  ENDPOINT_INFO Ctrl_Info;
}DEVICE_INFO;

typedef struct _DEVICE_PROP			//�豸���Խṹ--USB �ں��ڱ�Ҫʱ������Ȩ�����û������û����������Device_Property ����������ýṹ����DEVICE_PROP ���ͣ����Ͷ������£�
{
  void (*Init)(void);        /* Initialize the device */		 			/*USB IP �ĳ�ʼ������. ��Ӧ�ÿ�ʼʱ����һ�Σ����ڹ����ʼ�����̡�*/
  void (*Reset)(void);       /* Reset routine of this device */		/*USB IP ��λ����. ���굥Ԫ�յ�RESET �ź�ʱ���ã��û�����Ӧ���ڴ˹��������ö˵㣨����Ĭ�ϵĿ��ƶ˵㲢ʹ�ܽ��գ���*/
	
  /* Device dependent process after the status stage */	/*�ڿ��ƴ����з��������̣�1.�������̣�2����ѡ�����ݹ��̣�3��״̬����*/
  void (*Process_Status_IN)(void);			/*״̬�����У�����IN���ư�*/
  void (*Process_Status_OUT)(void);			/*״̬�����У�����OUT���ư�*/

  /* Procedure of process on setup stage of a class specified request with data stage */	/*�ڽ����׶εĹ����У����кܶ���������������ݽ׶�stage */
  /*���������ݽ׶ε�������������Class_Data_Setup()�����д���
		Class_Data_Setup() ����Ӧȥ������е�����������ͬʱ�����������ENDPOINT_INFO�ṹ��Ϣ
		���IN���ư������������ư�����wLength��wOffset�������ֱ�����Ҫ���͵����ֽ�����Ҫ��ʼ�����λ��
    ���OUT���ư������ε����ư�����rLength��rOffser����ֱ�����Ҫ���յ����ֽ�����Ҫ�������ݵĻ�������ʼ��ַ
    ���������Ч��Class_Data_Setup����SUCCESS�����򷵻�UNSUPPORT
   ע�⣺
    ��ΪGET_CONFIGURATION��GET_INTERFACE������������������ϵ���У��������ǻ��ڸú����м��ʹ���*/
  RESULT (*Class_Data_Setup)(u8 RequestNo);

  /* Procedure of process on setup stage of a class specified request without data stage */	/*�ڽ��������У����кܶ�����������������ݽ׶�*/
  /*���е�û�����ݽ׶ε�����������Class_NoData_Setup��������д���
   Class_NoData_Setup() 
   ����Ӧȥ����������������󣬲���ִ������

   ע�⣺
    ��ΪSET_CONFIGURATION��SET_INTERFACE��������������������ϵ���У����Ƕ����ڸú����б����ʹ���*/

  RESULT (*Class_NoData_Setup)(u8 RequestNo);

  /*Class_Get_Interface_Setting()
   �������ʱ��usb_core.c�ļ��б�����������Ӧ�ó����Ƿ�֧�ֱ�ѡ�еĽӿںͱ��ýӿ�
   �������ʱ���û�д�ġ����Ӧ�ó���֧�ֽӿںͱ��ýӿڣ�����뷵��"SUCCESS"�����򣬷���"UNSUPPORT"*/

  RESULT  (*Class_Get_Interface_Setting)(u8 Interface, u8 AlternateSetting);

  u8* (*GetDeviceDescriptor)(u16 Length);			//�ں˵õ��豸������
  u8* (*GetConfigDescriptor)(u16 Length);			//�ں˵õ�����������
  u8* (*GetStringDescriptor)(u16 Length);			//�ں˵õ��ַ���������
	
	/* ����ֶβ������ڵ�ǰ��汾������ֻ���ּ�����ǰ�İ汾*/
  u8* RxEP_buffer;
  u8 MaxPacketSize;			//���İ�����Ϊ64�ֽ�

}DEVICE_PROP;

typedef struct _USER_STANDARD_REQUESTS			//�û���׼����ṹ--�û���׼����ṹ���û�����ͱ�׼�������֮��Ľӿڣ�
{
  void (*User_GetConfiguration)(void);       /* Get Configuration */		/*��ȡ����*/	//���յ� Get Configuration��������ã���׼�������á�
  void (*User_SetConfiguration)(void);       /* Set Configuration */		/*��������*/	//���յ� Set Configuration���������ã���׼�������á�
  void (*User_GetInterface)(void);           /* Get Interface */				/*��ȡ�ӿ�*/	//���յ� Get interface����ȡ�ӿڣ���׼�������á�
  void (*User_SetInterface)(void);           /* Set Interface */				/*���ýӿ�*/	//���յ� Set interface�����ýӿڣ���׼�������á�
  void (*User_GetStatus)(void);              /* Get Status */						/*��ȡ״̬*/	//���յ� Get interface�����״̬����׼�������á�
  void (*User_ClearFeature)(void);           /* Clear Feature */				/*�������*/	//���յ� Clear Feature������� ��׼�������á�
  void (*User_SetEndPointFeature)(void);     /* Set Endpoint Feature */	/*���ö˵�����*/	//���յ� Set Feature�����ã� ��׼�������á�
  void (*User_SetDeviceFeature)(void);       /* Set Device Feature */		/*�����豸����*/	//���յ� Set Feature�����ã� ��׼�������á�
  void (*User_SetDeviceAddress)(void);       /* Set Device Address */		/*�����豸��ַ*/	//���յ�set Address�����õ�ַ�� ��׼�������á�
}
USER_STANDARD_REQUESTS;

/* Exported constants --------------------------------------------------------*/
#define Type_Recipient (pInformation->USBbmRequestType & (REQUEST_TYPE | RECIPIENT))

#define Usb_rLength Usb_wLength
#define Usb_rOffset Usb_wOffset

#define USBwValue		USBwValues.w
#define USBwValue0	USBwValues.bw.bb0
#define USBwValue1	USBwValues.bw.bb1
#define USBwIndex		USBwIndexs.w
#define USBwIndex0	USBwIndexs.bw.bb0
#define USBwIndex1	USBwIndexs.bw.bb1
#define USBwLength	USBwLengths.w
#define USBwLength0	USBwLengths.bw.bb0
#define USBwLength1	USBwLengths.bw.bb1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u8 Setup0_Process(void);
u8 Post0_Process(void);
u8 Out0_Process(void);
u8 In0_Process(void);

RESULT Standard_SetEndPointFeature(void);
RESULT Standard_SetDeviceFeature(void);

u8 *Standard_GetConfiguration(u16 Length);	//���'Length'ֵΪ0ʱ��������Ч������0	//���Ȳ�Ϊ0ʱ���������ñ����ĵ�ַ
RESULT Standard_SetConfiguration(void);			//�ú���������������ֵ���������͸������usb�豸Ҫ�����Լ����豸ֵ
u8 *Standard_GetInterface(u16 Length);			//��'Length'��ֵΪ0��������Ч������0�������Ȳ�Ϊ0�����ر��ýӿڵĵ�ַ
RESULT Standard_SetInterface(void);
u8 *Standard_GetDescriptorData(u16 Length, PONE_DESCRIPTOR pDesc);

u8 *Standard_GetStatus(u16 Length);					//�����豸�������ݵ�"StatusInfo buffer".//������������ݿ��ĩ�ˣ�����'Length'Ϊ0�ǣ�������Ч���򷵻�0
RESULT Standard_ClearFeature(void);					//������ֹĳ��ָ��������	//������ִ�У�����Return USB_SUCCESS,���������Ч������USB_UNSUPPORT
RESULT Standard_SetEndPointFeature(void);		//���û�ʹ�ܶ˵��ָ��������	//�������ִ�У�����USB_SUCCESS,���������Ч���򷵻�USB_UNSUPPORT
RESULT Standard_SetDeviceFeature(void);			//���û�ʹ���豸ָ��������		//�������ִ�У�����USB_SUCCESS,���������Ч���򷵻�USB_UNSUPPORT
void SetDeviceAddress(u8);
void NOP_Process(void);

extern DEVICE_PROP Device_Property;
extern  USER_STANDARD_REQUESTS User_Standard_Requests;
extern  DEVICE  Device_Table;
extern DEVICE_INFO Device_Info;

/* cells saving status during interrupt servicing */
extern u16 SaveRState;
extern u16 SaveTState;

#endif /* __USB_CORE_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
