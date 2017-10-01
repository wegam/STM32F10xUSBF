/******************************** User_library *********************************
* �ļ��� 	: SPI_FLASH.C
* ����   	: wegam@sina.com
* �汾   	: V
* ����   	: 2017/04/16
* ˵��   	: 
********************************************************************************
SPI_FLASHʹ�ù��ܲ���
1����Ҫ�궨�� SPI_FLASH
2��ʹ��USB_TEST �����
3����Ҫ�궨��SPI����
--------------------------------FLASH�����������-----------------------------
Sector����BLOCK��С�ĵ�λ
Block��	�󲿷ֵ�FLASH����64KΪ��λ�ռ�ERASE
Chip��	��ƬFLASH
Page��
����(256-byte per page)(4K-byte per sector/16page per sector)(64K-byte per block/16-sector per block)(8-block per chip)
*******************************************************************************/
#ifdef OFFLINE_ISP							//���������SPI_FLASH �˹�����Ч

#include "OFFLINE_ISP.H"

#include "SPI_FLASH.H"
#include "USART_ISP.H"
#include "STM32_TOOLS.H"

#include "STM32_PWM.H"
#include "STM32_SYS.H"
#include "STM32_GPIO.H"
#include "STM32_SYSTICK.H"
#include "STM32_USART.H"
#include "STM32_WDG.H"

#include "string.h"

#define 	STM32_CDC

//#define 	USART_BufferSize				512



//ISP_Conf_TypeDef 	ISP_Conf;
//SPI_FLASH_TypeDef	SPI_FLASH;

//u8 RxdBuffe[USART_BufferSize]={0};
//u8 RevBuffe[USART_BufferSize]={0};
u16 PWM_Ratio=0;
u8 flag=0;
u32 timecunt=0;		//���м�ʱ

typedef struct
{
	ISP_Conf_TypeDef 	ISP_Conf;
	BSP_Conf_TypeDef	BSP_Conf;
	
	SPI_FLASH_TypeDef	SPI_FLASH;
	
}OFFLINE_ISP_TypeDef;

OFFLINE_ISP_TypeDef OFFLINE_Cof;


/*******************************************************************************
* ������		:	
* ��������	:	 
* ����		:	
* ���		:
* ���� 		:
*******************************************************************************/
void OFFLINE_ISP_Configuration(void)
{
	SYS_Configuration();											//ϵͳ���� STM32_SYS.H	
	GPIO_DeInitAll();													//�����е�GPIO�ر�----V20170605
	PWM_OUT(TIM2,PWM_OUTChannel1,1,900);			//PWM�趨-20161127�汾
	
	OFFLINE_ISP_Conf();
	
	IWDG_Configuration(2000);					//�������Ź�����---������λms	
	SysTick_Configuration(10);				//ϵͳ���ʱ������72MHz,��λΪuS
	timecunt=0;		//���м�ʱ
//	OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qCERASE;
}
/*******************************************************************************
* ������		:
* ��������	:
* ����		:
* ���		:
* ���� 		:
*******************************************************************************/
void OFFLINE_ISP_Server(void)
{
//	u16 RxNum=0;
	IWDG_Feed();								//�������Ź�ι��	
	timecunt++;		//���м�ʱ
	if(timecunt>=0xFFFFFFF5)
	{
	}
	BSP_Process(&(OFFLINE_Cof.BSP_Conf));								//�¼���������
	
//	OFFLINE_ISP_StatusProcess();		//״̬����
//	Usart_ISP_Process(&(OFFLINE_Cof.ISP_Conf));
//	SPI_FLASH_Process(&(OFFLINE_Cof.SPI_FLASH));			//FLASH���ݴ��������е�FLASH��������ӿ�
}
/*******************************************************************************
*������			:	function
*��������		:	��������˵��
*����				: 
*����ֵ			:	��
*******************************************************************************/
void OFFLINE_ISP_StatusProcess(void)		//״̬����
{
	if(Usart_ISP_GetSlaveStatus(&(OFFLINE_Cof.ISP_Conf))==ISP_STATUS_WaitReadData)	//���شӻ�״ֵ̬
	{
//		memcpy((OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_TvBuffer), OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_RvBuffer, OFFLINE_Cof.ISP_Conf.ISP_DATA.WriteLen+1);	//��������
		if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_IDLE)
		{
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_READ;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_ReadAdrr=OFFLINE_Cof.ISP_Conf.ISP_DATA.ReadAddr-OFFLINE_Cof.ISP_Conf.ISP_DATA.OffsetAddr+OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_OffsetAdrr;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead=OFFLINE_Cof.ISP_Conf.ISP_DATA.USARTSendLen+1;
		}
		else if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Steps==Step_IDLE)	//���������
		{
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;
			memcpy(OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_TvBuffer, OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer, OFFLINE_Cof.ISP_Conf.ISP_DATA.USARTSendLen+1);	//��������
			Usart_ISP_SetSlaveStatus(&(OFFLINE_Cof.ISP_Conf),ISP_STATUS_WaitSData);	//���ôӻ�״̬
		}
	}
	else if(Usart_ISP_GetSlaveStatus(&(OFFLINE_Cof.ISP_Conf))==ISP_STATUS_WaitWrite)	//ISP�ȴ�д����
	{
		timecunt=0;
		if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_IDLE)
		{
			
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_WRITE;
			memcpy(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer,OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_RvBuffer,  OFFLINE_Cof.ISP_Conf.ISP_DATA.WriteLen+1);	//��������
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_WriteAdrr=OFFLINE_Cof.ISP_Conf.ISP_DATA.WriteAddr-OFFLINE_Cof.ISP_Conf.ISP_DATA.OffsetAddr+OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_OffsetAdrr;			
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToWrite=OFFLINE_Cof.ISP_Conf.ISP_DATA.WriteLen+1;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_SumReceived+=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToWrite;			//��������
		}
		else if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Steps==Step_WRITE)
		{
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Steps=Step_IDLE;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qIDLE;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;
			
			Usart_ISP_SetSlaveStatus(&(OFFLINE_Cof.ISP_Conf),ISP_STATUS_WaitWrited);	//���ôӻ�״̬--ISP�ȴ�д�����
			
		}
//		Usart_ISP_SetSlaveStatus(&(OFFLINE_Cof.ISP_Conf),ISP_STATUS_WaitWrited);	//���ôӻ�״̬--ISP�ȴ�д�����

//		memcpy((OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_TvBuffer), OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_RvBuffer, OFFLINE_Cof.ISP_Conf.ISP_DATA.WriteLen+1);	//��������
//		Usart_ISP_SetSlaveStatus(&(OFFLINE_Cof.ISP_Conf),ISP_STATUS_WaitWrited);	//���ôӻ�״̬--ISP�ȴ�д�����
	}
	else if(Usart_ISP_GetSlaveStatus(&(OFFLINE_Cof.ISP_Conf))==ISP_STATUS_Eraseing)	//ISP���ڲ���---SPI_FLASHִ�в�������
	{
//		if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_IDLE)
//		{
//			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_ERASE;		//����
//			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qCERASE;	//��Ƭ����
//		}
//		else if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Request==SPI_FLASH_qIDLE)
//		{
//			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;
//			Usart_ISP_SetSlaveStatus(&(OFFLINE_Cof.ISP_Conf),ISP_STATUS_WaitErased);			//���ôӻ�״̬--�ȴ��������
//		}
		Usart_ISP_SetSlaveStatus(&(OFFLINE_Cof.ISP_Conf),ISP_STATUS_WaitErased);			//���ôӻ�״̬--�ȴ��������
		
	}
	else if(OFFLINE_Cof.ISP_Conf.Connected==ISP_STATUS_WaitConnect||OFFLINE_Cof.ISP_Conf.Connected==ISP_STATUS_Connectting)			//ISP�ȴ����ӣ���Ϊ�ӻ�)
	{
		if(OFFLINE_Cof.ISP_Conf.Connected==ISP_STATUS_WaitConnect)
		{
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qCERASE;
			OFFLINE_Cof.ISP_Conf.Connected=ISP_STATUS_Connectting;						//ISP�������ӣ���Ϊ�ӻ�)---ִ�и�ʽ��
		}
		else if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Request==SPI_FLASH_qIDLE)
		{
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;
			OFFLINE_Cof.ISP_Conf.Connected=ISP_STATUS_Connectted;						//ISP�������ӣ���Ϊ�ӻ�)---ִ�и�ʽ��
//			Usart_ISP_SetSlaveStatus(&(OFFLINE_Cof.ISP_Conf),ISP_STATUS_WaitErased);			//���ôӻ�״̬--�ȴ��������
		}
	}
	else if(Usart_ISP_GetSlaveStatus(&(OFFLINE_Cof.ISP_Conf))==ISP_STATUS_WaitGoAddr)
	{
		if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_IDLE)
		{
			u32 temp=0;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qWRITE;
//			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_SumReceived;																							//�ܹ����յ������ݸ���---4�ֽ�0x00-0x03�洢
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_ISPStartAddr=OFFLINE_Cof.ISP_Conf.ISP_DATA.OffsetAddr;		//��¼ʱ����ʼ��ַ---4�ֽ�0x04-0x07�洢
			
			
			temp=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_SumReceived;																							//�ܹ����յ������ݸ���---4�ֽ�0x00-0x03�洢
			memset(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer, 0xFF, 256);
			
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer[0]=0xFF&(temp>>24);
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer[1]=0xFF&(temp>>16);
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer[2]=0xFF&(temp>>8);
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer[3]=0xFF&(temp>>0);
			
			
			
			temp=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_ISPStartAddr;																							//��¼ʱ����ʼ��ַ---4�ֽ�0x04-0x07�洢
			
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer[4]=0xFF&(temp>>24);
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer[5]=0xFF&(temp>>16);
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer[6]=0xFF&(temp>>8);
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MOSI_Buffer[7]=0xFF&(temp>>0);

			
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToWrite=8;		//��д�볤��
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_WriteAdrr=0;				//FLASH��ʼ��ַ
		}
		else if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Steps==Step_WRITE)
		{
//			u32 timedelay=0;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Steps=Step_IDLE;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Request=SPI_FLASH_qIDLE;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;
			Usart_ISP_ACK(&(OFFLINE_Cof.ISP_Conf));	//ISPӦ��
			Usart_ISP_SetSlaveStatus(&(OFFLINE_Cof.ISP_Conf),ISP_STATUS_WaitReset);	//���ôӻ�״̬
//			while(timedelay++<0xFFFF);
//			Usart_ISP_Reset(&(OFFLINE_Cof.ISP_Conf));						//���ñ����---�ָ����в���ΪĬ��ֵ
//			Usart_ISP_SetSlaveStatus(&(OFFLINE_Cof.ISP_Conf),ISP_STATUS_IDLE);	//ISP����״̬�����Զ�д
		}		
	}
	else if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_SumOfData==0)//�ܹ����յ������ݸ���---4�ֽ�0x00-0x03�洢//�տ���ʱ��Ӧ��Ϊ0
	{
		if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_IDLE)
		{
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_READ;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_ReadAdrr=0;				//����д������Ϣ��0��ʼ
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead=10;
			memset(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer, 0xFF, 256);
		}
		else if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Steps==Step_IDLE)
		{
			u32 temp=0;
			temp+=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer[0];
			temp=(temp<<8)|OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer[1];
			temp=(temp<<8)|OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer[2];
			temp=(temp<<8)|OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer[3];

			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_SumOfData=temp;			//��Ҫд������ݸ���
			
			
			temp=0;
			temp+=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer[4];
			temp=(temp<<8)|OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer[5];
			temp=(temp<<8)|OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer[6];
			temp=(temp<<8)|OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer[7];
			
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_ISPStartAddr=temp;			//��Ҫд�����ʼ��ַ
			
			
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;
			memcpy(OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_TvBuffer, OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer, OFFLINE_Cof.ISP_Conf.ISP_DATA.USARTSendLen+1);	//��������
		}
	}
	
//-----------------------------����
	else if(Usart_MISP_GetStatus(&(OFFLINE_Cof.ISP_Conf))==ISP_MSTATUS_GetFirmwareInf||Usart_MISP_GetStatus(&(OFFLINE_Cof.ISP_Conf))==ISP_MSTATUS_WaitFirmwareInf)
	{
		OFFLINE_Cof.ISP_Conf.ISP_MDATA.StartAddr	=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_ISPStartAddr;		//��ʼ��ַ
		OFFLINE_Cof.ISP_Conf.ISP_MDATA.GoAddr			=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_ISPStartAddr;		//�����е�ַ
		OFFLINE_Cof.ISP_Conf.ISP_MDATA.FirmwareLen=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_SumOfData;			//��ҪISP���ص������ݴ�С
		OFFLINE_Cof.ISP_Conf.ISP_MASTER_STATUS=ISP_MSTATUS_GetedFirmwareInf;			//״̬---�ѻ�ȡ���̼���Ϣ
	}
	else if(Usart_MISP_GetStatus(&(OFFLINE_Cof.ISP_Conf))==ISP_MSTATUS_ReadFirmware||Usart_MISP_GetStatus(&(OFFLINE_Cof.ISP_Conf))==ISP_MSTATUS_WaitFirmware)
	{
//		OFFLINE_Cof.ISP_Conf.ISP_MASTER_STATUS=ISP_MSTATUS_GettedFirmware;							//״̬---�ѻ�ȡ���̼�
//		OFFLINE_Cof.ISP_Conf.ISP_MDATA.WriteLen=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead;
		if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus==SPI_FLASH_IDLE)
		{
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_READ;
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_ReadAdrr=OFFLINE_Cof.ISP_Conf.ISP_MDATA.WriteAddr-OFFLINE_Cof.ISP_Conf.ISP_MDATA.OffsetAddr+OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_OffsetAdrr;
			if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_SumOfData-OFFLINE_Cof.ISP_Conf.ISP_MDATA.SumHaveWritten>256)
			{
				OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead=0xFF+1;
			}
			else
			{
				OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_SumOfData-OFFLINE_Cof.ISP_Conf.ISP_MDATA.SumHaveWritten+1;
			}
		}
		else if(OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Steps==Step_IDLE)	//���������
		{
			unsigned char Bcc=0xFF;
			
			OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_Staus=SPI_FLASH_IDLE;
			OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_TvBuffer[0]=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead-1;			//���ݳ���
			
			memcpy(&(OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_TvBuffer[1]), OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.MISO_Buffer, OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead+1);	//��������
			
			Bcc=BCC8(OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_TvBuffer,OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead+1);		//���У��;
			OFFLINE_Cof.ISP_Conf.ISP_DATA.ISP_TvBuffer[OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead+1]=Bcc;
			
			OFFLINE_Cof.ISP_Conf.ISP_MASTER_STATUS=ISP_MSTATUS_GettedFirmware;							//״̬---�ѻ�ȡ���̼�
			OFFLINE_Cof.ISP_Conf.ISP_MDATA.WriteLen=OFFLINE_Cof.SPI_FLASH.SPI_FLASH_Info.SPI_FLASH_LenghToRead+2;
		}
	}
}

/*******************************************************************************
* ������		:
* ��������	:
* ����		:
* ���		:
* ���� 		:
*******************************************************************************/
void OFFLINE_ISP_Conf(void)
{
	OFFLINE_Cof.ISP_Conf.USARTx=USART1;
	
	OFFLINE_Cof.ISP_Conf.RESET_CTL_PORT=GPIOB;
	OFFLINE_Cof.ISP_Conf.RESET_CTL_Pin=GPIO_Pin_0;
	
	OFFLINE_Cof.ISP_Conf.BOOT0_CTL_PORT=GPIOA;
	OFFLINE_Cof.ISP_Conf.BOOT0_CTL_Pin=GPIO_Pin_8;
	
	OFFLINE_Cof.BSP_Conf.BSP_Port.USARTx=USART1;
	OFFLINE_Cof.BSP_Conf.BSP_Port.BOOT0_CTL_PORT=GPIOB;
	OFFLINE_Cof.BSP_Conf.BSP_Port.BOOT0_CTL_Pin=GPIO_Pin_0;
	OFFLINE_Cof.BSP_Conf.BSP_Port.RESET_CTL_PORT=GPIOA;
	OFFLINE_Cof.BSP_Conf.BSP_Port.RESET_CTL_Pin=GPIO_Pin_8;
	
	BSP_Configuration(&(OFFLINE_Cof.BSP_Conf));									//���ú���
	
	SPI_FLASH_Conf(&(OFFLINE_Cof.SPI_FLASH));
//	Usart_ISP_Cof(&(OFFLINE_Cof.ISP_Conf));	
}




#endif