#ifndef __PD008V11_H
#define __PD008V11_H

#include	"stdio.h"			//����printf
#include	"string.h"		//����printf
#include	"stdarg.h"		//���ڻ�ȡ��ȷ�������Ĳ���
#include	"stdlib.h"		//malloc��̬�����ڴ�ռ�

#include "LinkedList.H"

#include "Message.H"			//��Ϣ:�����������,״̬����ϱ���ʶ
#include "STM32_SYS.H"
#include "STM32_SYSTICK.H"
#include "STM32F10x_BitBand.H"

#include "STM32_GPIO.H"
#include "STM32_PWM.H"
#include "STM32_USART.H"
#include "STM32_CAN.H"
#include "STM32_WDG.H"

#include "MOTOR.H"
#include "74HC165.H"
#include "SWITCHID.H"		//���뿪��

//-----------------------------���뿪��
//#define	PD008_Switch1			PB9in		//LSB
//#define	PD008_Switch2			PB8in		
//#define	PD008_Switch3			PB7in
//#define	PD008_Switch4			PB6in
//#define	PD008_Switch5			PB5in
//#define	PD008_Switch6			PB4in		//MSB

////-----------------------------������--ͼֽ
//#define	SENSORD15_S1J1		HC165x2.HC165_Data.Data32.D15		//S1��������ԭ�㴫����---0:�и�Ӧ,1-�޸�Ӧ
//#define	SENSORD14_S1J2		HC165x2.HC165_Data.Data32.D14		//S2��������ԭ�㴫����---0:�и�Ӧ,1-�޸�Ӧ
//#define	SENSORD13_S2J1		HC165x2.HC165_Data.Data32.D13		//S3���ӵ�λ������(С����)---0:����δ��,1:������
//#define	SENSORD12_S2J2		HC165x2.HC165_Data.Data32.D12		//S4���Ӹ�λ������(ҩ����������)---0:�����ѵ����λ(������),1:���������δ��,���Լ�����������
//#define	SENSORD11_S3J1		HC165x2.HC165_Data.Data32.D11		//S5���������ӵ�λ������---0:����δ��,1-�����ѵ�,������
//#define	SENSORD10_S3J2		HC165x2.HC165_Data.Data32.D10		//���������ӵ�λ������--����
//#define	SENSORD9_S4J1			HC165x2.HC165_Data.Data32.D9		//��ҩ����������---����
//#define	SENSORD8_S4J2			HC165x2.HC165_Data.Data32.D8		//��ҩ����������---����

//-----------------------------������--����
#define	SENSORD15_S1J1		HC165x2.HC165_Data.Data32.D15		//S1��������ԭ�㴫����---0:�и�Ӧ,1-�޸�Ӧ
#define	SENSORD14_S1J2		HC165x2.HC165_Data.Data32.D14		//S2��������ԭ�㴫����---0:�и�Ӧ,1-�޸�Ӧ
#define	SENSORD13_S2J1		HC165x2.HC165_Data.Data32.D12		//S3���ӵ�λ������(С����)---0:����δ��,1:������
#define	SENSORD12_S2J2		HC165x2.HC165_Data.Data32.D8		//S4���Ӹ�λ������(ҩ����������)---0:�����ѵ����λ(������),1:���������δ��,���Լ�����������
#define	SENSORD11_S3J1		HC165x2.HC165_Data.Data32.D10		//S5���������ӵ�λ������---0:����δ��,1-�����ѵ�,������
#define	SENSORD10_S3J2		HC165x2.HC165_Data.Data32.D10		//���������ӵ�λ������--����
#define	SENSORD9_S4J1			HC165x2.HC165_Data.Data32.D9		//��ҩ����������---����
#define	SENSORD8_S4J2			HC165x2.HC165_Data.Data32.D8		//��ҩ����������---����



//-----------------------------PMOS���(ZP1~ZP4)&�����ߵ�����ƽӿ�
#define	ZP1_PORT	GPIOC
#define	ZP1_PIN		GPIO_Pin_15

#define	ZP2_PORT	GPIOC
#define	ZP2_PIN		GPIO_Pin_14

#define	ZP3_PORT	GPIOC
#define	ZP3_PIN		GPIO_Pin_13

#define	ZP4_PORT	GPIOA
#define	ZP4_PIN		GPIO_Pin_1

#define	ACMOTOR1_PORT		ZP4_PORT		//�����߱�Ƶ��1
#define	ACMOTOR1_PIN		ZP4_PIN

#define	ACMOTOR2_PORT		ZP3_PORT		//�����߱�Ƶ��2
#define	ACMOTOR2_PIN		ZP3_PIN

#define	ACMOTOR3_PORT		ZP2_PORT		//�����ߵ�����
#define	ACMOTOR3_PIN		ZP2_PIN

#define	ACMOTOR4_PORT		ZP1_PORT		//�����ߵ�����
#define	ACMOTOR4_PIN		ZP1_PIN
//-----------------------------������ PC12
#define	BuzzerPort			GPIOC
#define	BuzzerPin				GPIO_Pin_12


typedef	enum	_Cache_Type				//���������
{
	CacheTypeN	=	0x00,		//N�ͻ����(���һ�������)
	CacheTypeM	=	0x01,		//M�ͻ��������(��������)
	CacheTypeS	=	0x02		//M�ͻ���ܴӻ�(����������)
}Cache_Type;

typedef	enum	_CAN_CMD				//���������
{
	//====================ͨ��
	IDLE									=	0x00,		//����ģʽ---�޶�Ӧ�˻���ܵ����ݻ���������޷�ʶ������
	RunTranLine						=	0x01,		//����������
	StopTranLine					=	0x02,		//ֹͣ������
	//====================����
	SendBasketInfo				=	0x03,		//����������Ϣ:���Ӻźʹ��ں�,5byte
	SendCatchedBasket			=	0x04,		//������ϱ��������Ӻ�
	SendCacheStatusOk			=	0x05,		//����OK״̬
	SendCacheStatusBusy		=	0x06,		//����æ״̬
	SendCacheStatusFull		=	0x07,		//���ͻ�����״̬
	SendCacheStatusEmpty	=	0x08,		//���ͻ������״̬
	//====================����
	ReceBasketInfo				=	0x09,		//���յ�������Ϣ
	FreeBasketBuff				=	0x0A		//�ͷ�һ�����ӻ���
}CAN_CMD;
typedef	enum	_Basket_CMD				//���������
{
	BasketIDLE						=	0x00,		//��ָ��
	BasketSave						=	0x01,		//����������Ϣ
	BasketRead						=	0x02,		//����������Ϣ
	BasketDelete					=	0x03,		//ɾ��������Ϣ---ɾ��һ��������Ϣ
	BasketClear						=	0x04		//���������Ϣ---�ͷ����ӻ���
}Basket_CMD;
typedef	enum	TranLine_CMD				//���������
{
	MotorIDLE							=	0x00,		//��ָ��
	
	RunTranLine1					=	0x01,		//����������1
	RunTranLine2					=	0x02,		//����������2
	RunTranLine3					=	0x03,		//����������3
	
	StopTranLine1					=	0x04,		//ֹͣ������1
	StopTranLine2					=	0x05,		//ֹͣ������2
	StopTranLine3					=	0x06,		//ֹͣ������3
	
	RunStepMotorUp				=	0x07,		//������������
	RunStepMotorDw				=	0x08,		//������������
	
	StopStepMotorUp				=	0x09,		//ֹͣ��������
	StopStepMotorDw				=	0x0A,		//ֹͣ��������
	
	RunTranLineAll				=	0x0B,		//ȫ������������
	StopTranLineAll				=	0x0C,		//ȫ��������ֹͣ
	PauseTranLineAll			=	0x0D,		//ȫ����������ͣ
	ResumeTranLineAll			=	0x0E		//ȫ�������߼�������
}TranLine_CMD;

typedef struct _SENS_FLAG
{
	unsigned short S1J1:1;
	unsigned short S1J2:1;
	unsigned short S2J1:1;
	unsigned short S2J2:1;
	unsigned short S3J1:1;
	unsigned short S3J2:1;
	unsigned short S4J1:1;
	unsigned short S4J2:1;
	unsigned short S5J1:1;
	unsigned short S5J2:1;
	unsigned short S6J1:1;
	unsigned short S6J2:1;
	unsigned short S7J1:1;
	unsigned short S7J2:1;
	unsigned short S8J1:1;
	unsigned short S8J2:1;
}SENS_FLAG;
typedef struct _ERROR_FLAG
{
	unsigned short StepM1:1;
	unsigned short StepM2:1;
	unsigned short S1J1:1;
	unsigned short S1J2:1;
	unsigned short S2J1:1;
	unsigned short S2J2:1;
	unsigned short S3J1:1;
	unsigned short S3J2:1;
	unsigned short S4J1:1;
	unsigned short S4J2:1;
	unsigned short S5J1:1;
	unsigned short S5J2:1;
	unsigned short S6J1:1;
	unsigned short S6J2:1;
	unsigned short S7J1:1;
	unsigned short S7J2:1;
	unsigned short S8J1:1;
	unsigned short S8J2:1;
}ERROR_FLAG;

//typedef enum	_STEP_MOTO_STATUS
//{
//	MOTOR_IDLE,				//����״̬
//	MOTOR_PAUSE,			//��ͣ״̬
//	MOTOR_Wait,				//����״̬	
//	MOTOR_STOP				//ֹͣ״̬
//}MOTO_STATUS;				//==========�������״̬


typedef enum cache_run_state
{
	RUN_IDLE=0,								//����״̬*/
	RUN_START,					//�������1����*/
	RUN_NOSIG,		//�ȴ��������1���е����ź�λ��*/
	RUN_SIG,			//�ȴ��������1���е����ź�λ��*/
} MOTO_STATUS;


extern HC165_CONF				HC165x2;						//����ṹ�����---�����������źŶ�ȡ
extern STEP_MOTO_CONF 	STEP_MOTO_1UP;			//������1---��������
extern STEP_MOTO_CONF 	STEP_MOTO_2DOWN;		//������2---��������


void PD008V11_Configuration(void);
void PD008V11_Server(void);

void PowerUp(void);					//�ϵ�

void SYS_Server(void);			//��������ݴ��������Ʒ������

void SENSOR_Configuration(void);
void SENSOR_Server(void);

void USART_Configuration(void);			//���ڴ�ӡ���úͶ������ӿ�����
void USART_Server(void);

void MOTOR_Configuration(void);			//�������:�������&�����ߵ��
void MOTOR_Sever(void);							//�������
void TranLine_Contrl(TranLine_CMD	CMD);	//�����߿���

void CAN_Configuration(void);
void CANDATA_Server(void);
void SET_CAN(CAN_CMD CMD);					//ͨ����CANд����������������úͷ���
CAN_CMD	GET_CAN(void);							//����CAN��������

void SWITCHID_Configuration(void);
void SWITCHID_Server(void);

bool Basket_Manage(Basket_CMD CMD);	//���ӹ���:������Ϣ�Ĵ洢�����
void TimeServer(void);							//��ʱ����

void Buzzer_Configuration(void);		//������
void Buzzer_Server(void);						//������
#endif
