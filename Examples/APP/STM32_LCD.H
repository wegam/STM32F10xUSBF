#ifndef __STM32_LCD_H
#define __STM32_LCD_H


//#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
//#include "stm32f10x_nvic.h"
//#include "stm32f10x_spi.h"

//#include "STM32_GPIO.H"


//#include	"stdio.h"			//����printf
//#include	"string.h"			//����printf
//#include	"stdarg.h"			//���ڻ�ȡ��ȷ�������Ĳ���
//#include	"stdlib.h"			//malloc��̬�����ڴ�ռ�


#define SD_CLK_SpeedL	0
#define SD_CLK_SpeedH	1

#define LCD_SD_CS_PORT				GPIOC
#define LCD_SD_CS_PIN					GPIO_Pin_6	

extern unsigned char STM32_LCD_Steep;			//�������н׶�--LCD����
extern unsigned char STM32_SD_Steep;			//�������н׶�--SD������

typedef enum
{
//#define 	STM32_SD_CMD0 (unsigned char)0x40;
//Class0 :(����ʶ�𡢳�ʼ���Ȼ������)
//Class2 (�������):
//Class4(д�����) :
//Class5 (���������):
//Class6(д�������):
//class7�����������������������
//class8�������ض������
//class10 ��11 ������
//���С�class1,    class3,class9��SPIģʽ��֧��
	
	//**************Class0
	STM32_SD_CMD0=(unsigned char)0x40,			//Class0	��λSD��
	STM32_SD_CMD1,													//Class0	��OCR�Ĵ���
	STM32_SD_CMD2,													//
	STM32_SD_CMD3,													//
	STM32_SD_CMD4,													//
	STM32_SD_CMD5,													//
	STM32_SD_CMD6,													//
	STM32_SD_CMD7,													//
	STM32_SD_CMD8,													//
	STM32_SD_CMD9,													//Class0	��CSD�Ĵ���
	STM32_SD_CMD10,													//Class0	��CID�Ĵ���
	STM32_SD_CMD11,													//
	STM32_SD_CMD12,													//Class0	ֹͣ�����ʱ�����ݴ���
	STM32_SD_CMD13,													//Class0	�� Card_Status �Ĵ���
	STM32_SD_CMD14,													//
	STM32_SD_CMD15,													//
	//**************Class2
	STM32_SD_CMD16,													//Class2	���ÿ�ĳ���
	STM32_SD_CMD17,													//Class2	������
	STM32_SD_CMD18,													//Class2	�����,ֱ����������CMD12Ϊֹ
	//**************Class3
	STM32_SD_CMD19,													//
	STM32_SD_CMD20,													//
	STM32_SD_CMD21,													//
	STM32_SD_CMD22,													//
	STM32_SD_CMD23,													//
	//**************Class4
	STM32_SD_CMD24,													//Class4	д����
	STM32_SD_CMD25,													//Class4	д���
	STM32_SD_CMD26,													//
	STM32_SD_CMD27,													//Class4	дCSD�Ĵ��� 
	STM32_SD_CMD28,													//Class6	����д������ĵ�ַ
	STM32_SD_CMD29,													//Class6	����д������ĵ�ַ
	STM32_SD_CMD30,													//Class6	Ask the card for the status of the write protection bits
	STM32_SD_CMD31,													//
	STM32_SD_CMD32,													//Class5	���ò��������ʼ��ַ
	STM32_SD_CMD33,													//Class5	���ò��������ֹ��ַ
	STM32_SD_CMD34,													//
	STM32_SD_CMD35,													//
	STM32_SD_CMD36,													//
	STM32_SD_CMD37,													//
	STM32_SD_CMD38,													//Class5	������ѡ��Ŀ�
	STM32_SD_CMD39,													//
	STM32_SD_CMD40,													//
	STM32_SD_CMD41,													//
	STM32_SD_CMD42,													//
	STM32_SD_CMD43,													//
	STM32_SD_CMD44,													//
	STM32_SD_CMD45,													//
	STM32_SD_CMD46,													//
	STM32_SD_CMD47,													//
	STM32_SD_CMD48,													//
	STM32_SD_CMD49,													//
	STM32_SD_CMD50,													//
	STM32_SD_CMD51,													//
	STM32_SD_CMD52,													//
	STM32_SD_CMD53,													//
	STM32_SD_CMD54,													//
	STM32_SD_CMD55,													//
	STM32_SD_CMD56,													//
	STM32_SD_CMD57,													//
	STM32_SD_CMD58,													//
	STM32_SD_CMD69,													//
	STM32_SD_CMD60,													//
	STM32_SD_CMD61													//
}STM32_SD_CMD_Typedef;

extern void STM32_LCD_Configuration(void);
extern void STM32_LCD_Server(void);
void STM32_LCD_PinConf(void);
void TM1618_PinSet(void);
void SSD1963_PinSet(void);
void TM1618_DIS(void);

extern void STM32_LCD_SPI_Configuration(char Speed);		//SPI�ӿ�����
extern void STM32_SD_CS(char Status);						//SD_CS�ſ���
extern void STM32_SD_SendByte(u16 Data);					//����һ�ֽ�����
extern unsigned char STM32_SD_ReceiveByte(void);
extern void STM32_SD_SendData(unsigned int BufferSize);		//DMA��ʽ��SDд�����ݣ�����DMA���ͺͽ��� 
extern unsigned char STM32_SD_Command(STM32_SD_CMD_Typedef CMD,u32 arg,u8 crc);
//unsigned char STM32_SD_Command(unsigned char *STM32_LCD_SD_CMD);
extern void STM32_SD_GetCID(void);			//��ȡCID��Ϣ������128λ��16�ֽ�
extern u8 STM32_SD_GetCSD(void);			//��ȡCSD��Ϣ������128λ��16�ֽ�
extern u32 STM32_SD_GetCapacity(void);	//��ȡSD��������
extern u8 STM32_SD_ReadSingleBlock(u32 sector);//��SD����һ��block
extern unsigned char STM32_SD_Init(void);
unsigned int LCD_PrintfString(u16 x,u16 y,u8 font,const char *format,...);			//��ߵ�ʡ�Ժž��ǿɱ����
#endif