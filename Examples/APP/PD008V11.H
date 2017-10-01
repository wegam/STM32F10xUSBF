#ifndef __PD008V11_H
#define __PD008V11_H

#include	"stdio.h"			//用于printf
#include	"string.h"		//用于printf
#include	"stdarg.h"		//用于获取不确定个数的参数
#include	"stdlib.h"		//malloc动态申请内存空间

#include "LinkedList.H"

#include "Message.H"			//消息:包括错误代码,状态码等上报标识
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
#include "SWITCHID.H"		//拔码开关

//-----------------------------拨码开关
//#define	PD008_Switch1			PB9in		//LSB
//#define	PD008_Switch2			PB8in		
//#define	PD008_Switch3			PB7in
//#define	PD008_Switch4			PB6in
//#define	PD008_Switch5			PB5in
//#define	PD008_Switch6			PB4in		//MSB

////-----------------------------传感器--图纸
//#define	SENSORD15_S1J1		HC165x2.HC165_Data.Data32.D15		//S1上提篮机原点传感器---0:有感应,1-无感应
//#define	SENSORD14_S1J2		HC165x2.HC165_Data.Data32.D14		//S2下提篮机原点传感器---0:有感应,1-无感应
//#define	SENSORD13_S2J1		HC165x2.HC165_Data.Data32.D13		//S3篮子到位传感器(小窗口)---0:缓存未满,1:缓存满
//#define	SENSORD12_S2J2		HC165x2.HC165_Data.Data32.D12		//S4篮子高位传感器(药篮满传感器)---0:篮子已到最高位(篮子满),1:缓存架篮子未满,可以继续接收篮子
//#define	SENSORD11_S3J1		HC165x2.HC165_Data.Data32.D11		//S5传送线篮子到位传感器---0:篮子未到,1-篮子已到,待提篮
//#define	SENSORD10_S3J2		HC165x2.HC165_Data.Data32.D10		//传送线篮子到位传感器--主机
//#define	SENSORD9_S4J1			HC165x2.HC165_Data.Data32.D9		//上药篮满传感器---副机
//#define	SENSORD8_S4J2			HC165x2.HC165_Data.Data32.D8		//上药篮满传感器---主机

//-----------------------------传感器--测试
#define	SENSORD15_S1J1		HC165x2.HC165_Data.Data32.D15		//S1上提篮机原点传感器---0:有感应,1-无感应
#define	SENSORD14_S1J2		HC165x2.HC165_Data.Data32.D14		//S2下提篮机原点传感器---0:有感应,1-无感应
#define	SENSORD13_S2J1		HC165x2.HC165_Data.Data32.D12		//S3篮子到位传感器(小窗口)---0:缓存未满,1:缓存满
#define	SENSORD12_S2J2		HC165x2.HC165_Data.Data32.D8		//S4篮子高位传感器(药篮满传感器)---0:篮子已到最高位(篮子满),1:缓存架篮子未满,可以继续接收篮子
#define	SENSORD11_S3J1		HC165x2.HC165_Data.Data32.D10		//S5传送线篮子到位传感器---0:篮子未到,1-篮子已到,待提篮
#define	SENSORD10_S3J2		HC165x2.HC165_Data.Data32.D10		//传送线篮子到位传感器--主机
#define	SENSORD9_S4J1			HC165x2.HC165_Data.Data32.D9		//上药篮满传感器---副机
#define	SENSORD8_S4J2			HC165x2.HC165_Data.Data32.D8		//上药篮满传感器---主机



//-----------------------------PMOS输出(ZP1~ZP4)&传送线电机控制接口
#define	ZP1_PORT	GPIOC
#define	ZP1_PIN		GPIO_Pin_15

#define	ZP2_PORT	GPIOC
#define	ZP2_PIN		GPIO_Pin_14

#define	ZP3_PORT	GPIOC
#define	ZP3_PIN		GPIO_Pin_13

#define	ZP4_PORT	GPIOA
#define	ZP4_PIN		GPIO_Pin_1

#define	ACMOTOR1_PORT		ZP4_PORT		//传送线变频器1
#define	ACMOTOR1_PIN		ZP4_PIN

#define	ACMOTOR2_PORT		ZP3_PORT		//传送线变频器2
#define	ACMOTOR2_PIN		ZP3_PIN

#define	ACMOTOR3_PORT		ZP2_PORT		//传送线调速器
#define	ACMOTOR3_PIN		ZP2_PIN

#define	ACMOTOR4_PORT		ZP1_PORT		//传送线调速器
#define	ACMOTOR4_PIN		ZP1_PIN
//-----------------------------蜂鸣器 PC12
#define	BuzzerPort			GPIOC
#define	BuzzerPin				GPIO_Pin_12


typedef	enum	_Cache_Type				//缓存架类型
{
	CacheTypeN	=	0x00,		//N型缓存架(最后一个缓存架)
	CacheTypeM	=	0x01,		//M型缓存架主机(带读卡器)
	CacheTypeS	=	0x02		//M型缓存架从机(不带读卡器)
}Cache_Type;

typedef	enum	_CAN_CMD				//缓存架类型
{
	//====================通用
	IDLE									=	0x00,		//空闲模式---无对应此缓存架的数据或命令或者无法识别命令
	RunTranLine						=	0x01,		//启动传送线
	StopTranLine					=	0x02,		//停止传送线
	//====================发送
	SendBasketInfo				=	0x03,		//发送篮子信息:篮子号和窗口号,5byte
	SendCatchedBasket			=	0x04,		//缓存架上报提篮篮子号
	SendCacheStatusOk			=	0x05,		//发送OK状态
	SendCacheStatusBusy		=	0x06,		//发送忙状态
	SendCacheStatusFull		=	0x07,		//发送缓存满状态
	SendCacheStatusEmpty	=	0x08,		//发送缓存非满状态
	//====================接收
	ReceBasketInfo				=	0x09,		//接收到篮子信息
	FreeBasketBuff				=	0x0A		//释放一个篮子缓存
}CAN_CMD;
typedef	enum	_Basket_CMD				//缓存架类型
{
	BasketIDLE						=	0x00,		//空指令
	BasketSave						=	0x01,		//保存篮子信息
	BasketRead						=	0x02,		//读出篮子信息
	BasketDelete					=	0x03,		//删除篮子信息---删除一个篮子信息
	BasketClear						=	0x04		//清除篮子信息---释放篮子缓存
}Basket_CMD;
typedef	enum	TranLine_CMD				//缓存架类型
{
	MotorIDLE							=	0x00,		//空指令
	
	RunTranLine1					=	0x01,		//启动传送线1
	RunTranLine2					=	0x02,		//启动传送线2
	RunTranLine3					=	0x03,		//启动传送线3
	
	StopTranLine1					=	0x04,		//停止传送线1
	StopTranLine2					=	0x05,		//停止传送线2
	StopTranLine3					=	0x06,		//停止传送线3
	
	RunStepMotorUp				=	0x07,		//运行上提篮机
	RunStepMotorDw				=	0x08,		//运行下提篮机
	
	StopStepMotorUp				=	0x09,		//停止上提篮机
	StopStepMotorDw				=	0x0A,		//停止下提篮机
	
	RunTranLineAll				=	0x0B,		//全部传送线运行
	StopTranLineAll				=	0x0C,		//全部传送线停止
	PauseTranLineAll			=	0x0D,		//全部传送线暂停
	ResumeTranLineAll			=	0x0E		//全部传送线继续运行
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
//	MOTOR_IDLE,				//运行状态
//	MOTOR_PAUSE,			//暂停状态
//	MOTOR_Wait,				//运行状态	
//	MOTOR_STOP				//停止状态
//}MOTO_STATUS;				//==========步进电机状态


typedef enum cache_run_state
{
	RUN_IDLE=0,								//空闲状态*/
	RUN_START,					//提篮电机1提篮*/
	RUN_NOSIG,		//等待提篮电机1运行到无信号位置*/
	RUN_SIG,			//等待提篮电机1运行到有信号位置*/
} MOTO_STATUS;


extern HC165_CONF				HC165x2;						//定义结构体变量---传感器输入信号读取
extern STEP_MOTO_CONF 	STEP_MOTO_1UP;			//提篮机1---上提篮机
extern STEP_MOTO_CONF 	STEP_MOTO_2DOWN;		//提篮机2---下提篮机


void PD008V11_Configuration(void);
void PD008V11_Server(void);

void PowerUp(void);					//上电

void SYS_Server(void);			//缓存架数据处理及控制服务程序

void SENSOR_Configuration(void);
void SENSOR_Server(void);

void USART_Configuration(void);			//串口打印配置和读卡器接口配置
void USART_Server(void);

void MOTOR_Configuration(void);			//电机配置:步进电机&传送线电机
void MOTOR_Sever(void);							//提篮电机
void TranLine_Contrl(TranLine_CMD	CMD);	//传送线控制

void CAN_Configuration(void);
void CANDATA_Server(void);
void SET_CAN(CAN_CMD CMD);					//通过对CAN写入命令进行数据配置和发送
CAN_CMD	GET_CAN(void);							//接收CAN接收命令

void SWITCHID_Configuration(void);
void SWITCHID_Server(void);

bool Basket_Manage(Basket_CMD CMD);	//篮子管理:篮子信息的存储与输出
void TimeServer(void);							//计时管理

void Buzzer_Configuration(void);		//蜂鸣器
void Buzzer_Server(void);						//蜂鸣器
#endif

