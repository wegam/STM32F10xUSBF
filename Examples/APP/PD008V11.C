/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : PC001V21.c
* Author             : WOW
* Version            : V2.0.1
* Date               : 06/26/2017
* Description        : PC001V21层控制板.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#ifdef PD008V11			//4路电机控制板

#include "PD008V11.H"

//#define WOW_DBG


//#define DEBUG_INFO_OUTPUT										//串口打印输出宏定义:打印输出用DMA输出USART_DMAPrintf(USART_TypeDef* USARTx,const char *format,...);

#define APP_CACHE_START_ADDR	0x00000043		//缓存架起始地址

#ifdef	WOW_DBG
	#define	SysTickTime				1000				//嘀嗒时钟 ----扫描周期,单位us
#else
	#define	SysTickTime				100				//嘀嗒时钟 ----扫描周期,单位us
#endif
#define	SysTimeOut					1000			//运行超时:配置看门狗使用,单位ms
#define	TranTimeOut					40000			//传送线运行超时:TranTimeCount配合使用,单位ms
#define USARTBufferSize			32				//定义串口缓冲区数据大小
//-----------------------------CAN参数
#define	CAN1_BaudRate	100000		//CAN波特率	100K
//-----------------------------步进电机参数
#define STEP_MOTO_1UP_RUNFREQ				200		//上提篮机正常运行频率---单位HZ
#define STEP_MOTO_1UP_STARTFREQ			200		//上提篮机启动频率-------单位HZ

#define STEP_MOTO_2DOWN_RUNFREQ			1000	//下提篮机正常运行频率---单位HZ
#define STEP_MOTO_2DOWN_STARTFREQ		1000	//下提篮机启动频率-------单位HZ
//=======================================蜂鸣器频率
#define	BuzzerFrq1										1		//蜂鸣器预警频率,单位Hz
#define	BuzzerFrq2										6		//蜂鸣器报警频率,单位Hz

//=======================================调试测试临时数据
u8 CANTestFlag	=	0;								//CAN测试标志,1--发数据,
u8 CANTestID		=	0x00;							//测试CANID地址
u8 ACMOTORTESTFlag	=	0;						//传送线电机行动标志:1-运行,
u32 ListLength	=	0	;								//测试链表长度
u8	ListTest	=	0;									//数据存储测试:0-不操作,1-存,2-读
u16	SensorCount	=	0;								//传感器测试计数
u32	TestTime	=	0;									//传感器测试计数
//LINK_LIST				StackList;				//篮子号--链表数据
//=======================================相关配置数据结构体定义
HC165_CONF			HC165x2;						//定义结构体变量---传感器输入信号读取
STEP_MOTO_CONF 	STEP_MOTO_1UP;			//提篮机1---上提篮机
STEP_MOTO_CONF 	STEP_MOTO_2DOWN;		//提篮机2---下提篮机
CanRxMsg 				RxMessage;					//CAN接收数据结构体
Cache_Type			CacheType;					//缓存架类型:N型读卡器SW6为ON,M型读卡器--在读到第一张卡时确定为M型,否则为S型,上电默认为S型

SWITCHID_CONF		SWITCHIDx;					//拔码开关
SYS_INFO				SYSINF;							//消息结构体
LINK_LIST				BaskeList;					//篮子号--链表数据
CAN_CMD					SET_CMD;						//设置CAN命令
SENS_FLAG				SensFlg;						//传感器读到标志:对应位为0--未处理,对应位为1--已处理
ERROR_FLAG			ErrorFlg;						//错误标志
MOTO_STATUS			M1STAS;							//上提篮电机状态
MOTO_STATUS			M2STAS;							//下提篮电机状态
//CAN_CMD					GET_CMD;					//获取CAN命令

//FIFO_DATA				BaskeFlag;					//存储是否提篮标识队列--链表数据

//=======================================相关标志位
u8 WForwardFlag			=	0;		//窗口号增量标志(根据读卡器的位置--通过接收篮子时的窗口地址比较):0-递增,读卡器窗口最小号;1-递减,读卡器在最大号窗口
u8 CANTXFlag				=	0;		//需要CAN发送数据标志;0-没有待发送数据;1-有待发送数据
u8 CANRXFlg					=	0;		//CAN接收数据标志;0-没有接收到数据;1-有接收到数据待处理
u8 CARDRXFlg				=	0;		//读卡器接收数据标志;0-没有接收到数据;1-有接收到数据待处理
u8 ACMOTOR_RunFlag	=	0;		//传送线所有电机运行标志:0-空闲,1-运行,2-停止,3-暂停
u8 CarryBasketFlag	=	0;		//
u8 InitFlag					=	0;		//初始化标志:	0-未初始化,1-已初始化
u8 CacheFullFlag		=	0;		//缓存架满标志:0-未满,1-预警2-已满			

//=======================================系统运行相关数据
u32	SYSRunTime			=	0;		//循环计时变量
//u32	CacheArray		=	0;		//待提篮队列,按位移,最大可以支持32们篮子在队列中等待提篮
u8	SWITCH_ID				=	0;		//拨码开关地址
u8	WINDOWS_ID			=	0;		//缓存架号(窗口号)拔码地址+1:拔码0~15,对应窗口号1~16,也就是可以支持16个窗口
u8	CAN_ID					=	0;		//CAN地址,在拔码处理程序设置,拔码地址加CAN起始地址APP_CACHE_START_ADDR
u8	RXCANID					=	0;		//当接收到篮子号信息时存储,判断窗口顺序使用
u32 TranTimeCount		=	0;		//传送线运行计时,当达到TranTimeOut后停止传送线
u16	BuzzerTime			=	0;		//蜂鸣器输出频率计数

//=======================================数据缓冲区
u8 CANRxBuffer[8]	=	{0};						//CAN接收BUFFER
u8 CANTxBuffer[8]	=	{0};						//CAN发送BUFFER
u8 CANRxDATA[8]		=	{0};						//CAN接收到的待处理数据
u8 CANTxDATA[8]		=	{0};						//CAN准备的待发送数据

u8 RevBuffer[USARTBufferSize]={0};	//串口接收数据备份区,程序使用数据由此缓冲区提供
u8 RxdBuffer[USARTBufferSize]={0};	//串口接收数据缓冲区,DMA空闲模式接收,如果串口有接收空闲标志,表示有数据接收到,将此缓冲区的数据拷贝到RevBuffer待处理
u8 TxdBuffer[USARTBufferSize]={0};	//串口发送数据缓冲区,缓存架主要为串口打印使用
u8 BasketData[5]={0};								//接收到的篮子信息---前4位为篮子号,最后一位为窗口号;数据源:读卡器,CAN,篮子信息队列
//u8 BasketDataB[6]={0};						//篮子号备份区
u8 CatchBasketInfo1[6]={0};					//待处理的篮子数据1,前4byte篮子号,byte5为窗口号,byte6为提篮状态--1排前	
u8 CatchBasketInfo2[6]={0};					//待处理的篮子数据2,前4byte篮子号,byte5为窗口号,byte6为提篮状态		

/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void PD008V11_Configuration(void)
{
	SYS_Configuration();											//系统配置系统运行时钟72M	

	GPIO_DeInitAll();													//将所有的GPIO关闭----V20170605

	SWITCHID_Configuration();									//配置拨码开关

	SENSOR_Configuration();										//传感器配置	

	MOTOR_Configuration();										//电机配置	

	USART_Configuration();										//串口配置	

	CAN_Configuration();											//CAN配置

	PWM_OUT(TIM2,PWM_OUTChannel1,1,500);			//系统运行LED灯 频率1HZ,占空比500/1000
	Buzzer_Configuration();										//蜂鸣器
	
	
	SysTick_Configuration(SysTickTime);				//系统嘀嗒时钟配置72MHz,单位为uS----软件运行以定时扫描模式,定时时间为SysTickTime

//	IWDG_Configuration(SysTimeOut);						//独立看门狗配置,超时复位，单位ms
	
//	ACMotorTimeCount	=	sizeof(LINK_NODE);
//	
//	BaskeList.HeadNode	=	NULL;								//篮子列表初始化
//	
//	memset(CANRxBuffer,0x8F,8);
	memset(BasketData,0X09,5);		//读到的篮子号	
}

/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void PD008V11_Server(void)
{
//	IWDG_Feed();						//独立看门狗喂狗
	PowerUp();								//上电:初始化操作及相数据初始化
	SENSOR_Server();					//处理传感器信号
	SWITCHID_Server();				//拔码开关动态处理程序(拔码更改后不需要重启复位):更新缓存架类型和缓存架号(窗口号)及CAN地址设置
	USART_Server();						//读卡器(读取IC卡内篮子号和窗口号)
	CANDATA_Server();					//接收主机传送过来的篮子号和窗口号及数据上报
	SYS_Server();							//主执行程序---处理读卡器、CAN、传感器信号，设置电机运行命令
	MOTOR_Sever();						//提篮电机
	TimeServer();							//计时管理
	Buzzer_Server();					//蜂鸣器
	
}
/*******************************************************************************
*函数名		:	SYS_Server
*功能描述	:	缓存架数据处理及控制服务程序---主执行程序
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SYS_Server(void)
{
	//==================================测试代码
#ifdef	WOW_DBG
	
	TestTime++;
	if(TestTime>=100)
	{
		TestTime=0;
		ListTest=1;
	}
	
//	SENSOR_Server();
//	//---------------CAN测试
//	if(CANTestFlag==1)
//	{
//		CANTestFlag=0;	//清除指令
//		ACMOTOR_RunFlag	=	1;		//传送线所有电机运行标志:0-空闲,1-运行,2-停止,3-暂停
//		CANTxBuffer[0]=APP_CMD_ACMotorCtl;	//传送线电机控制信息
//		CANTxBuffer[1]=0x01;
//		CANTXFlag	=	1;		//需要CAN发送数据标志;0-没有待发送数据;1-有待发送数据
//	}
//	else	if(CANTestFlag==2)
//	{
//		CANTestFlag=0;					//清除指令
//		ACMOTOR_RunFlag	=	2;		//传送线所有电机运行标志:0-空闲,1-运行,2-停止,3-暂停
//		CANTxBuffer[0]=APP_CMD_ACMotorCtl;	//传送线电机控制信息
//		CANTxBuffer[1]=0x00;
//		CANTXFlag	=	1;		//需要CAN发送数据标志;0-没有待发送数据;1-有待发送数据
//	}
//	//---------------篮子号存储测试
	if(ListTest==1)	//存
	{
		ListTest=0;
		memset(BasketData,0x45,5);
		FIFO_IN(&BaskeList,(char*)BasketData,5);
//		STACK_PUSH(&StackList,(char*)BasketDataS,5);//入栈
		ListLength=GetListLength((&BaskeList)->HeadNode);			//获取链表长度
	}
//	else if(ListTest==2)//取
//	{
//		ListTest=0;
//		FIFO_OUT(&BaskeList,(char*)BasketDataS);							//输出数据
////		STACK_POP(&StackList,(char*)BasketDataS);						//出栈
//	}
#else

#endif
}
/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SENSOR_Configuration(void)
{
	//____________异步并行加入端(低电平有效)
	HC165x2.PL_PORT=GPIOC;
	HC165x2.PL_Pin=GPIO_Pin_0;
	
	//____________时钟输入端(低到高边沿触发--上升沿)
	HC165x2.CP_PORT=GPIOC;
	HC165x2.CP_Pin=GPIO_Pin_1;
	
	//____________时钟使能端(低电平有效)
	HC165x2.CE_PORT=GPIOC;
	HC165x2.CE_Pin=GPIO_Pin_2;
	
	//____________串行数据输出端(读串行数据端口)
	HC165x2.Q7_PORT=GPIOC;
	HC165x2.Q7_Pin=GPIO_Pin_3;
	
	HC165x2.HC165_Data.NumOfHC165=2;		//串接的74HC165芯片个数,一个8位:2个
	
	HC165Conf(&HC165x2);
}
/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SENSOR_Server(void)
{
	//============================传感器说明:
	//数量:5个
	//S1:上提篮机原点传感器
	//S2:下提篮机原点传感器
	//S3:缓存满传感器(篮子最高位传感器):原程序--感应到篮子,此窗口再来新篮,报缓存满错误;新-感应到篮子,预警,当此窗口再次来篮子,上提篮机
	//S4:篮子高位传感器(药篮满传感器):任何情况下,此传感器触发后停止上提篮机,解除触发后恢复上提篮机原有状态
	//S5:传送线篮子到位传感器:篮子到位后查询篮子队列是否为本窗口篮子,如果是,暂停传送线,上篮,下提篮机提篮完成后,恢复传送线,上提篮机提篮
	HC165Read(&HC165x2);		//读传感器信号
	
	//=+=+=+=+=+=+=+=传感器数据处理
	//============================S4篮子高位传感器(药篮满传感器)---0:篮子已到最高位(篮子满),1:缓存架篮子未满,可以继续接收篮子
	if(SENSORD12_S2J2==0)
	{
		//**************检测到药篮满时,停止提篮,上报药缓存满
		//**************当缓存架恢复为非满状态时,上报一个非满状态
	
		if(M1STAS	!=	RUN_IDLE)
		{			
			STEP_MOTO_1UP.MOTO_COMMAND=MOTO_COMMAND_STOP;			//上提篮电机停止
		}
		if(SensFlg.S2J2	==	0)
		{
			SET_CAN(SendCacheStatusFull);			//通过对CAN写入命令进行数据配置和发送
			SensFlg.S2J2	=	1;
		}
		CacheFullFlag		=	2;								////缓存架满标志:0-未满,1-预警2-已满
	}
	else
	{	
		if(SensFlg.S2J2	==	1)
		{
			SET_CAN(SendCacheStatusEmpty);		//通过对CAN写入命令进行数据配置和发送
			if(M1STAS	!=	RUN_IDLE)
			{
				STEP_MOTO_1UP.MOTO_COMMAND=MOTO_COMMAND_RUN;			//上提篮电机停止
			}
			SensFlg.S2J2	=	0;
		}
//		CacheFullFlag		=	0;							//缓存架满标志:0-未满,1-预警2-已满
	}
	//============================S2下提篮机原点传感器---0:有感应,1-无感应
	if(SENSORD14_S1J2==0)
	{ 
		//*****************初始化:上电有感应,需要初始化转一步长(上电有感应且下提篮电机在MOTO_COMMAND_STOP状态),启动下提篮机
		//*****************提篮控制:在MOTO_COMMAND_RUN状态时,表示在提篮,到达原点,则停止下提篮机,开启上提篮机
		//*****************InitFlag初始化标志:	0-未初始化,1-已初始化
		if(M2STAS	==	RUN_START)	//启动
		{
			M2STAS	=	RUN_NOSIG;
		}
		else if(M2STAS	==	RUN_SIG)
		{
			M2STAS	=	RUN_IDLE;
			M1STAS	=	RUN_START;
			SET_CAN(RunTranLine);															//通过对CAN写入命令进行数据配置和发送---停止传送线
			TranLine_Contrl(RunTranLineAll);	//传送线控制
			STEP_MOTO_2DOWN.MOTO_COMMAND	=		MOTO_COMMAND_STOP;			//停止下提篮机
			STEP_MOTO_1UP.MOTO_COMMAND		=		MOTO_COMMAND_RUN;				//下提篮完成提篮后上提篮开始提篮
		}
	}
	else
	{
		//*****************初始化:上电无感应,需要初始化转一步长(上电无感应且下提篮电机在MOTO_COMMAND_STOP状态)启动下提篮机
		//*****************提篮控制:在MOTO_COMMAND_RUN状态时,表示在提篮,到达原点,则停止下提篮机,开启上提篮机
		if(M2STAS==RUN_NOSIG)
		{
			M2STAS	=	RUN_SIG;
		}
	}
	//============================S1上提篮机原点传感器---0:有感应,1-无感应
	if(SENSORD15_S1J1==0)
	{
		if(M1STAS	==	RUN_START)	//启动
		{
			M1STAS	=	RUN_NOSIG;
		}
		else if(M1STAS	==	RUN_SIG)
		{
			M1STAS	=	RUN_IDLE;
			STEP_MOTO_1UP.MOTO_COMMAND	=		MOTO_COMMAND_STOP;			//停止下提篮机
			if(InitFlag==0)
			{
				InitFlag=1;
			}
		}
	}
	else
	{
		if(M1STAS==RUN_NOSIG)
		{
			M1STAS	=	RUN_SIG;
		}
	}
	//============================S3缓存满传感器(小窗口)
	if(SENSORD13_S2J1	==1	&&	M1STAS==RUN_IDLE)						//S3篮子到位传感器(小窗口)---0:缓存未满,1:缓存满
	{
//		SET_CAN(SendCacheStatusFull);			//通过对CAN写入命令进行数据配置和发送
		CacheFullFlag		=	1;							////缓存架满标志:0-未满,1-预警2-已满	
		SensFlg.S2J1	=	1;								//上报报警
	}
	else	if(SENSORD13_S2J1	==0	&&	M1STAS==RUN_IDLE)			//S3篮子到位传感器(小窗口)---0:缓存未满,1:缓存满
	{
//		SET_CAN(SendCacheStatusEmpty);		//通过对CAN写入命令进行数据配置和发送
		CacheFullFlag		=	0;							////缓存架满标志:0-未满,1-预警2-已满	
		SensFlg.S2J1	=	0;
	}
	//============================S5传送线篮子到位传感器---0:篮子未到,1-篮子已到,待提篮:最后一个缓存架无条件提篮
	if(SENSORD11_S3J1	==	1)
	{	
		if(SensFlg.S3J1	==	0)					//
		{
			Basket_Manage(BasketRead);			
			if(BasketData[4]	==	WINDOWS_ID||CacheType==CacheTypeN)	//本窗口CacheTypeN;		//N型缓存架(最后一个缓存架)
			{
				SensFlg.S3J1	=	1;
				memset(BasketData,0x00,5);
				SET_CAN(StopTranLine);															//通过对CAN写入命令进行数据配置和发送---停止传送线
				TranLine_Contrl(PauseTranLineAll);									//传送线控制---暂停				
			}
		}
		else																					//待处理提篮
		{
			//CacheFullFlag		=	0;											//缓存架满标志:0-未满,1-预警2-已满	
			if(M2STAS==RUN_IDLE	&&	CacheFullFlag!=2)		//现在为停止命令
			{
				M2STAS	=	RUN_START;
				STEP_MOTO_2DOWN.MOTO_COMMAND		=		MOTO_COMMAND_RUN;			//启动提篮电机
			}		
		}
	}
	else
	{
		SensFlg.S3J1	=0;
	}
}
/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void USART_Configuration(void)
{
	USART_DMA_ConfigurationNR	(USART1,115200,(u32*)RxdBuffer,1024);						//USART_DMA配置--查询方式，不开中断
	USART_DMA_ConfigurationNR	(UART4,19200,(u32*)RxdBuffer,USARTBufferSize);	//USART_DMA配置--查询方式，不开中断
}
/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void USART_Server(void)
{
	//============================串口说明:传送线第一个读卡器读篮子数据(M型)
	//篮子数据长度:22byte,byte3~byte6为篮子号,byte7为窗口号,提取数据byte3~byte7共5byte
	//接收到IC卡数据:判断本窗口是否为M型,如果不是主机,丢弃数据,不处理,如果是M型,提取篮子数据加入待提篮数据队列及通过CAN发送出去
	//此处不对篮子数据进行判断是否为本窗口数据,一致将篮子数据往外发

	u16 len=0;
	len= USART_ReadBufferIDLE(UART4,(u32*)RevBuffer,(u32*)RxdBuffer);	//串口空闲模式读串口接收缓冲区，如果有数据，将数据拷贝到RevBuffer,并返回接收到的数据个数，然后重新将接收缓冲区地址指向RxdBuffer
	if(len==22)	//IC卡信息长度22字节
	{
//		CacheType	=	CacheTypeM;				//M型缓存架主机(一定要带读卡器)
		#ifdef DEBUG_INFO_OUTPUT
//		char i=0;
//		memcpy(TxdBuffer,RevBuffer,len);
//		USART_DMAPrintf(USART1,"读卡器读到数据:");
//		USART_DMASend(USART1,(u32*)TxdBuffer,len);	//串口DMA发送程序
//		for(i=0;i<len;i++)
//		{
//			USART_DMAPrintf(USART1,"读卡器读到数据:");
////			
//			USART_DMAPrintf(USART1,"%02X ",TxdBuffer[i]);
//		}
		#endif		
		if(CacheType	!=	CacheTypeM)				//缓存架类型:N型读卡器SW6为ON,M型读卡器--在读到第一张卡时确定为M型,否则为S型,上电默认为S型
		{
			return;														//非M型缓存架----不接收读卡器信号
		}
		//---------------提取篮子信息,将篮子信息存入待提篮数据队列
		memcpy(BasketData,&RevBuffer[3],5);	//提取关键篮子信息--从byte3开始的4字节为篮子号,byte7为窗口号
		Basket_Manage(BasketSave);					//篮子管理:篮子信息的存储与输出
		//---------------通过CAN发送篮子信息
		memcpy(BasketData,&RevBuffer[3],5);	//提取关键篮子信息--从byte3开始的4字节为篮子号,byte7为窗口号
		SET_CAN(SendBasketInfo);						//通过对CAN写入命令进行数据配置和发送
	}
}
/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void MOTOR_Configuration(void)
{
//	STEP_MOTO_CONF STEP_MOTO_1UP;		//提篮机1---上提篮机
//	STEP_MOTO_CONF STEP_MOTO_2DOWN;	//提篮机2---下提篮机

//------- STEP_MOTO_1UP;		//提篮机1---上提篮机
	STEP_MOTO_1UP.PUL_PORT=GPIOB;
	STEP_MOTO_1UP.PUL_Pin=GPIO_Pin_15;
	
	STEP_MOTO_1UP.DIR_PORT=GPIOC;
	STEP_MOTO_1UP.DIR_Pin=GPIO_Pin_6;
	
	STEP_MOTO_1UP.TIME_CONF.StepTimeOut	=	10*100*(1000/SysTickTime);	//超时时间---单位ms
	STEP_MOTO_1UP.TIME_CONF.StepFreq		=	STEP_MOTO_1UP_RUNFREQ;			//上提篮机正常运行频率---单位HZ
	STEP_MOTO_1UP.TIME_CONF.StartFreq		=	STEP_MOTO_1UP_STARTFREQ;		//上提篮机启动频率-------单位HZ
	STEP_MOTO_1UP.TIME_CONF.BaseTime		=	SysTickTime;								//时基---扫描周期,单位us----默认设置定时器每1us扫描一次
	StepMotoConf(&STEP_MOTO_1UP);

//------- STEP_MOTO_2DOWN;	//提篮机2---下提篮机	
	STEP_MOTO_2DOWN.PUL_PORT=GPIOC;
	STEP_MOTO_2DOWN.PUL_Pin=GPIO_Pin_8;
	
	STEP_MOTO_2DOWN.DIR_PORT=GPIOA;
	STEP_MOTO_2DOWN.DIR_Pin=GPIO_Pin_15;
	
	STEP_MOTO_2DOWN.TIME_CONF.StepTimeOut	=	20*1000*(1000/SysTickTime);		//超时时间---单位ms
	STEP_MOTO_2DOWN.TIME_CONF.StepFreq		=	STEP_MOTO_2DOWN_RUNFREQ;			//下提篮机正常运行频率---单位HZ
	STEP_MOTO_2DOWN.TIME_CONF.StartFreq		=	STEP_MOTO_2DOWN_STARTFREQ;		//下提篮机启动频率-------单位HZ
	STEP_MOTO_2DOWN.TIME_CONF.BaseTime		=	SysTickTime;									//时基---扫描周期,单位us----默认设置定时器每1us扫描一次
	StepMotoConf(&STEP_MOTO_2DOWN);		//

//-------
//	GPIO_Configuration_OPP50	(GPIOC,	GPIO_Pin_15);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//	GPIO_Configuration_OPP50	(GPIOC,	GPIO_Pin_13);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//	GPIO_Configuration_OPP50	(GPIOC,	GPIO_Pin_14);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//	GPIO_Configuration_OPP50	(GPIOA,	GPIO_Pin_1);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605


//-------传送线变频器1:ZP4--PA1
	GPIO_Configuration_OPP50	(ACMOTOR1_PORT,	ACMOTOR1_PIN);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//-------传送线变频器2:ZP3--PC13
	GPIO_Configuration_OPP50	(ACMOTOR2_PORT,	ACMOTOR2_PIN);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//-------传送线调速器:ZP2--PC14
	GPIO_Configuration_OPP50	(ACMOTOR3_PORT,	ACMOTOR3_PIN);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
//-------步进电机刹车:ZP1--PA1
	GPIO_Configuration_OPP50	(ACMOTOR4_PORT,	ACMOTOR4_PIN);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
}
/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void MOTOR_Sever(void)							//
{
	//--------------------------提篮结构相关运动代码
	StepMotoSever(&STEP_MOTO_2DOWN);			//下提篮机
	StepMotoSever(&STEP_MOTO_1UP);				//上提篮机
}
/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void TranLine_Contrl(TranLine_CMD	CMD)
{	
	//============================传送线电机控制代码
	//控制类型:启动,停止,暂停,恢复运行
	//启动:设置运行标志,清零计时(超时计时),开启输出
	//停止:设置停止标志,清零计时(超时计时),停止输出
	//暂停:设置暂停标志,停止输出
	//恢复:暂停后恢复,设置运行标志,开启输出
	if(CMD	==	RunTranLineAll)						//启动
	{
		ACMOTOR_RunFlag=1;									//传送线所有电机运行标志:0-空闲,1-运行,2-停止,3-暂停
		TranTimeCount	=	0;									//重新计时
		ACMOTOR1_PORT->BSRR=ACMOTOR1_PIN;		//高电平//ZP4传送线变频器1
		ACMOTOR2_PORT->BSRR=ACMOTOR2_PIN;		//高电平//ZP3传送线变频器2
		ACMOTOR3_PORT->BSRR=ACMOTOR3_PIN;		//高电平//ZP2传送线调速器
		ACMOTOR4_PORT->BSRR=ACMOTOR4_PIN;		//高电平//ZP1传送线调速器
	}
	else if(CMD	==	StopTranLineAll)			//停止
	{
		TranTimeCount=0;										//传送线运行计时,当达到ACMotorTimeOut后停止传送线
		ACMOTOR_RunFlag=0;									//传送线所有电机运行标志:0-空闲,1-运行,2-停止,3-暂停
		ACMOTOR1_PORT->BRR=ACMOTOR1_PIN;		//低电平//ZP4传送线变频器1
		ACMOTOR2_PORT->BRR=ACMOTOR2_PIN;		//低电平//ZP3传送线变频器2
		ACMOTOR3_PORT->BRR=ACMOTOR3_PIN;		//低电平//ZP2传送线调速器
		ACMOTOR4_PORT->BRR=ACMOTOR4_PIN;		//低电平//ZP1传送线调速器
	}
	else if(CMD	==	PauseTranLineAll)			//暂停
	{
		ACMOTOR_RunFlag=3;									//传送线所有电机运行标志:0-空闲,1-运行,2-停止,3-暂停
		ACMOTOR1_PORT->BRR=ACMOTOR1_PIN;		//低电平//ZP4传送线变频器1
		ACMOTOR2_PORT->BRR=ACMOTOR2_PIN;		//低电平//ZP3传送线变频器2
		ACMOTOR3_PORT->BRR=ACMOTOR3_PIN;		//低电平//ZP2传送线调速器
		ACMOTOR4_PORT->BRR=ACMOTOR4_PIN;		//低电平//ZP1传送线调速器
	}
	else if(CMD	==	ResumeTranLineAll)		//暂停过后继续运行
	{
		ACMOTOR_RunFlag=1;									//传送线所有电机运行标志:0-空闲,1-运行,2-停止,3-暂停
		ACMOTOR1_PORT->BSRR=ACMOTOR1_PIN;		//低电平//ZP4传送线变频器1
		ACMOTOR2_PORT->BSRR=ACMOTOR2_PIN;		//低电平//ZP3传送线变频器2
		ACMOTOR3_PORT->BSRR=ACMOTOR3_PIN;		//低电平//ZP2传送线调速器
		ACMOTOR4_PORT->BSRR=ACMOTOR4_PIN;		//低电平//ZP1传送线调速器
	}
}
/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void CAN_Configuration(void)
{
	CAN_Configuration_NR(CAN1_BaudRate);						//CAN1配置---标志位查询方式，不开中断
	CAN_FilterInitConfiguration_StdData(0x00,0x00,0x00);	//CAN滤波器配置---标准数据帧模式,不做过滤
//	CAN_FilterInitConfiguration_StdData(0x01,0x47,0x47);	//CAN滤波器配置---标准数据帧模式,不做过滤
}
/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void CANDATA_Server(void)
{
	//============================CAN数据处理:接收篮子号,接收控制传输线电机运转信号
	//*****CAN接收数据处理:接收缓冲区CANRxBuffer;发送缓冲区CANTxBuffer
	//*****接收篮子号:判断是否为本窗口篮子,设置相关待提篮标志位,存储篮子号
	//*****控制整个传送线的运转:接收到启动传输线信号时,判断本窗口是否可以启动传送线,不可以的话,返回停止传输线命令,否则启动
	//*****根据主机发送篮子号时,比较本窗口(通过CAN_ID确定)与主机窗口号的大小来确定窗口排序设置WForwardFlag
	//*****打包上报信息
	unsigned char flag	=	0;				//CAN接收到数据标志
	CAN_CMD GetCMD	=	IDLE;					//空闲模式---无对应此缓存架的数据或命令或者无法识别命令
	
	flag=CAN_RX_DATA(&RxMessage);		//查询CAN有无接收到数据
	if(flag	==	0)
	{
		return;												//未接收到数据,退出
	}
	USART_DMAPrintf(USART1,"CAN数据:ID-%04X::%4.2X%4.2X%4.2X%4.2X%4.2X%4.2X%4.2X%4.2X\r\n",RxMessage.StdId,RxMessage.Data[0],RxMessage.Data[1],RxMessage.Data[2],RxMessage.Data[3],RxMessage.Data[4],RxMessage.Data[5],RxMessage.Data[6],RxMessage.Data[7]);
	GetCMD	=	GET_CAN();						//接收CAN接收命令----分析CAN数据,返回相应的操作码
	
	if(GetCMD	==	IDLE)							//无可执行命令
	{
		return;
	}
	
	if(GetCMD	==	ReceBasketInfo)								//接收到篮子信息
	{
		if(CAN_ID<((u8)RxMessage.StdId&0xFF))			//读卡器在大号窗口,篮子流通顺序为减
		{
			WForwardFlag	=	1;											//窗口号增量标志(根据读卡器的位置--通过接收篮子时的窗口地址比较):0-递增,读卡器窗口最小号;1-递减,读卡器在最大号窗口
		}
		else
		{
			WForwardFlag	=	0;											//窗口号增量标志(根据读卡器的位置--通过接收篮子时的窗口地址比较):0-递增,读卡器窗口最小号;1-递减,读卡器在最大号窗口
		}
		//---------------篮子信息管理:存储
//		BasketData
		memcpy(BasketData,(u8*)&(RxMessage.Data[1]),5);	//复制待存储的篮子数据
		Basket_Manage(BasketSave);								//存储管理
	}
	else if(GetCMD	==	FreeBasketBuff)					//接收到篮子信息
	{
		memcpy(BasketData,(u8*)&(RxMessage.Data[1]),5);	//复制待存储的篮子数据
		Basket_Manage(BasketDelete);							//删除管理
	}
	else if(GetCMD	==	RunTranLine)				//启动传送线
	{
		TranLine_Contrl(RunTranLineAll);			//全部传送线运行
	}
	else if(GetCMD	==	StopTranLine)				//停止传送线
	{
		TranLine_Contrl(StopTranLineAll);			//全部传送线停止
	}
}
//===============================================================================
//函数:	SET_CAN
//描述:	通过对CAN写入命令进行数据配置和发送
//返回:
//===============================================================================
void SET_CAN(CAN_CMD CMD)
{
	//============================设置CAN命令及发送数据
	//根据命令参数类型,与协议匹配设置相关待发送数据
	//数据中转Buffer:BasketData--读取或者写入buffer,读取完后清除内容
	//CatchBasketInfo1,CatchBasketInfo2:篮子信息含窗口号,2个缓存,从待提篮队列中读取数据时存入,发送提篮结果时,拷贝完数据后清零
	memset(CANTxBuffer,0x00,8);	//清空缓存
	if(CMD==IDLE)								//空闲模式---无对应此缓存架的数据
	{
		return;
	}
	else if(CMD==RunTranLine)		//运行传送线
	{
		CANTxBuffer[0]	=	APP_CMD_ACMotorCtl;
		CANTxBuffer[1]	=	0x01;
	}
	else if(CMD==StopTranLine)	//停止传送线
	{
		CANTxBuffer[0]	=	APP_CMD_ACMotorCtl;
		CANTxBuffer[1]	=	0x00;
	}
	else if(CMD==SendBasketInfo)										//发送篮子信息:篮子数据 byte0:命令,byte1~byte4:篮子号,byte5:窗口号
	{
		//CatchBasketData:待处理的篮子数据,前4byte篮子号,byte5为窗口号
		CANTxBuffer[0]	=	APP_CMD_MAJ2MINSENDINFO;		//M型主缓存架向辅缓存架发送篮子信息
		memcpy(&CANTxBuffer[1],BasketData,5);					//复制数据
		memset(BasketData,0x00,5);										//清除数据
	}
	else if(CMD==SendCatchedBasket)									//缓存架上报提篮篮子号:byte0-标识,byte1~byte4-篮子号,byte5-窗口号,byte6-状态
	{	
		//CatchBasketInfo:正在提篮的篮子信息,前4byte篮子号,byte5为窗口号,byte6为提篮状态		
		CANTxBuffer[0]	=	APP_CMD_CABASBASKETREP;			//缓存架上报提篮篮子号
		
		memcpy(&CANTxBuffer[1],CatchBasketInfo1,6);		//复制数据
		
		if(CatchBasketInfo2[4]	!=	0x00)							//还有待处理数据
		{
			memcpy(CatchBasketInfo1,CatchBasketInfo2,6);	//待处理数据前移
			memset(CatchBasketInfo2,0x00,6);							//清除第二个待处理数据
		}
		else																						//没有待处理数据
		{
			memset(CatchBasketInfo1,0x00,6);							//清除待处理数据
		}
	}
	else if(CMD==SendCacheStatusOk)									//发送OK状态
	{
		CANTxBuffer[0]	=	APP_CMD_SEATREP;						//状态报警信息上报
		CANTxBuffer[1]	=	MSG_STATUS_OK;							//正常状态，命令正常无误的被处理
	}
	else if(CMD==SendCacheStatusBusy)								//发送忙状态
	{
		CANTxBuffer[0]	=	APP_CMD_SEATREP;						//状态报警信息上报
		CANTxBuffer[1]	=	MSG_STATUS_BUSY;						//忙，无法对命令进行处理
	}
	else if(CMD==SendCacheStatusFull)								//发送缓存满状态
	{
		CANTxBuffer[0]	=	APP_CMD_SEATREP;						//状态报警信息上报
		CANTxBuffer[1]	=	MSG_STATUS_CACHEFULL;				//缓存结构已满
	}
	else if(CMD==SendCacheStatusEmpty)							//发送缓存非满状态
	{
		CANTxBuffer[0]	=	APP_CMD_SEATREP;						//状态报警信息上报
		CANTxBuffer[1]	=	MSG_STATUS_CACHENONEMPTY;		//缓存结构已满
	}
	else
	{
		return;
	}	
	//=========================CAN发送数据
	CAN_StdTX_DATA(CAN_ID,8,CANTxBuffer);						//CAN使用标准帧发送数据
}
//===============================================================================
//函数:	接收CAN接收命令
//描述:
//返回:
//===============================================================================
CAN_CMD	GET_CAN(void)
{
	//============================解释接收到的CAN数据
	//根据原协议对接收到的CAN数据进行解释,返回相关标志
	//其它

	CAN_CMD GetCMD	=	IDLE;					//空闲模式---无对应此缓存架的数据或命令或者无法识别命令
	
	GetCMD	=	(CAN_CMD)RxMessage.Data[0];			//获取命令

	if(GetCMD==APP_CMD_STARTEXTTRANS)					//启动外部传输线----兼容旧程序命令
	{
		GetCMD	=	RunTranLine;							//启动传送线
	}
	else if(GetCMD==APP_CMD_ACMotorCtl)				//传送线电机控制信息
	{
		if(RxMessage.Data[0]==0x00)							//传送线运行命令内容---停止
		{
			GetCMD	=	StopTranLine;								//停止传送线
		}
		else if(RxMessage.Data[0]==0x01)				//传送线运行命令内容---启动
		{
			GetCMD	=	RunTranLine;								//启动传送线
		}
		else
		{
			GetCMD	=	IDLE;												//空闲模式---无对应此缓存架的数据或命令或者无法识别命令
		}
	}
	else if(GetCMD==APP_CMD_MAJ2MINSENDINFO)	//M型主缓存架向辅缓存架发送篮子信息---旧协议
	{
		GetCMD	=	ReceBasketInfo;								//接收到篮子信息
	}
	else if(GetCMD==APP_CMD_CacheData)				//篮子信息
	{
		GetCMD	=	ReceBasketInfo;								//接收到篮子信息
	}
	else if(GetCMD==APP_CMD_CABASBASKETREP)		//缓存架上报提篮篮子号
	{
		GetCMD	=	FreeBasketBuff;								//释放一个篮子缓存
	}
	else					//无法识别
	{
		GetCMD	=	IDLE;													//空闲模式---无对应此缓存架的数据或命令或者无法识别命令
	}	
	return	GetCMD;
}
/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SWITCHID_Configuration(void)
{
	SWITCHIDx.NumOfSW		=	6;		//拔码开关位数为6位
	
	//SW1
	SWITCHIDx.SW1_PORT	=	GPIOB;
	SWITCHIDx.SW1_Pin		=	GPIO_Pin_9;
	//SW1
	SWITCHIDx.SW2_PORT	=	GPIOB;
	SWITCHIDx.SW2_Pin		=	GPIO_Pin_8;
	//SW1
	SWITCHIDx.SW3_PORT	=	GPIOB;
	SWITCHIDx.SW3_Pin		=	GPIO_Pin_7;
	//SW1
	SWITCHIDx.SW4_PORT	=	GPIOB;
	SWITCHIDx.SW4_Pin		=	GPIO_Pin_6;
	//SW1
	SWITCHIDx.SW5_PORT	=	GPIOB;
	SWITCHIDx.SW5_Pin		=	GPIO_Pin_5;
	//SW1
	SWITCHIDx.SW6_PORT	=	GPIOB;
	SWITCHIDx.SW6_Pin		=	GPIO_Pin_4;

	SWITCHIDConf(&SWITCHIDx);			//配置拔码开关
//	SWITCHIDRead(&SWITCHIDx);			//读取拔码开关地址
}
/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
void SWITCHID_Server(void)
{
	//============================拔码说明:新拔码规则
	//拔码标识:最后一个缓存架,主缓存架(带读卡器)--一条传送线只使用一个读卡器,窗口号
	//SW6:最后一个缓存架标志:OFF--非最后一个缓存架;ON--最后一个缓存架,最后一个缓存架感应到篮子到位就提篮,不对篮子进行判断
	//SW5:区分主机与副机(有无读卡器的区别):OFF--副机(无读卡器);ON--主机(有读卡器)(最后一个窗口无读卡器)
	//SW4~SW1:窗口号:拔码0~15,对应窗口号1~16,也就是可以支持16个窗口窗口号为拔码地址+1,与篮子号对应
	
	/*
	拨码规则:
	1、bit0-bit2位是拨对应缓存架地址1-8；
	2、bit3~4位是缓存架类型标志:00-N型缓存架;01-M型缓存架主机;10-M型缓存架辅机;；
	3、bit5位是最后一个缓存架指示，若最后一个缓存架是M型缓存架，
		则主、辅控制器都要将该位置ON，主机是窗口号小的那个；
	*/

	u8 tempID=0;
	tempID=SWITCHIDRead(&SWITCHIDx);			//读取拔码开关地址
	//---------地址更新
	if(SWITCH_ID!=tempID)		//拔码开关地址有变化
	{
		SWITCH_ID		=	tempID;							//更新ID
		WINDOWS_ID	=	(tempID&0x0F)+1;		//缓存架号(窗口号)拔码地址+1:拔码0~15,对应窗口号1~16,也就是可以支持16个窗口
		//-----判断缓存架类型
		if(SWITCHIDx.sID_Data16.Data16.D5)	//SW6最后一个窗口位:此位判断优先,最后一个窗口位不分主机副机
		{
			CacheType	=	CacheTypeN;		//N型缓存架(最后一个缓存架)
		}
		else if(SWITCHIDx.sID_Data16.Data16.D4)	//SW4 判断是主机还是副机
		{
			CacheType	=	CacheTypeM;		//M型缓存架主机(一定要带读卡器)
		}
		else
		{
			CacheType	=	CacheTypeS;		//M型缓存架从机(可以不带读卡器)
		}
		//-----CAN_ID更新	
		CAN_ID	=	(tempID&0x0F)+APP_CACHE_START_ADDR;		//CAN地址,在拔码处理程序设置,拔码地址加CAN起始地址APP_CACHE_START_ADDR
		RXCANID=0;
	}
}
/*******************************************************************************
*函数名		:	function
*功能描述	:	函数功能说明
*输入			: 
*输出			:	无
*返回值		:	无
*例程			:
*******************************************************************************/
bool Basket_Manage(Basket_CMD CMD)
{
	unsigned long	len	=	0;			//
	if(CMD == BasketIDLE)					//空指令
	{
		return (bool)0;
	}
	
	if(CMD == BasketSave)						//==================保存篮子信息
	{
		//**************将篮子信息加入列表
		//**************信息来源为读卡器或者主缓存架往外发送的待提取的篮子信息
		len	=	FIFO_IN(&BaskeList,(char*)BasketData,5);		//篮子信息入列:读卡器读到篮子或者M型主缓存架向辅缓存架发送篮子信息时,篮子信息入列
		memset(BasketData,0x00,5);												//清除篮子数据
	}
	else if(CMD == BasketRead)			//==================读出篮子信息:传送线传感器有感应篮子到位时需要读出列表数据检查是否为本窗口篮子
	{
		//**************读出数据判断是否为本窗口药篮
		//**************如果为本窗口药篮,判断是否还有待处理药篮信息来存储新药篮信息
		memset(BasketData,0x00,5);		//清除数据
		len	=	FIFO_OUT(&BaskeList,(char*)BasketData);			//篮子信息出列:接到在本窗口前面的提篮自信上报或者本窗口本应到篮子时出列
		if(len	&&	BasketData[4]==WINDOWS_ID)						//数据列表内有数据且窗口号符合---将数据放入待处理篮子列表中
		{
			if(CatchBasketInfo1[4]==0)	//-----------------已有待处理数据:将数据存入列表2
			{
				memcpy(CatchBasketInfo1,BasketData,5);
			}
			else												//-----------------已有待处理数据:将数据存入列表2
			{
				memcpy(CatchBasketInfo2,BasketData,5);
			}
		}		
	}
	else if(CMD == BasketDelete)		//==================删除篮子信息:收到其它窗口提篮完成上报后需要从列表中删除一个数据
	{
		//**************当总线接收到有本缓存架前面的缓存架发出的已提篮信息时,需要从待提篮列表中减掉一个篮子信息
		//**************根据篮子的流通方向判断是否为前面缓存架:如果是前面缓存架则从列表中删除一个篮子信息
		//**************WForwardFlag:窗口号增量标志(根据读卡器的位置--通过接收篮子时的窗口地址比较):0-递增,读卡器窗口最小号;1-递减,读卡器在最大号窗口
		if(WForwardFlag==0	&&	BasketData[4]<WINDOWS_ID)				//缓存架号递增:上报的为前缓存架信息
		{
			len	=	FIFO_OUT(&BaskeList,(char*)BasketData);					//篮子信息出列:接到在本窗口前面的提篮自信上报或者本窗口本应到篮子时出列
		}
		else	if(WForwardFlag==1	&&	BasketData[4]>WINDOWS_ID)	//缓存架号递减:上报的为前缓存架信息
		{
			len	=	FIFO_OUT(&BaskeList,(char*)BasketData);					//篮子信息出列:接到在本窗口前面的提篮自信上报或者本窗口本应到篮子时出列
		}
		memset(BasketData,0x00,5);															//清除数据
	}
	else if(CMD == BasketClear)			//==================清除篮子信息---释放篮子缓存
	{
		len	=	FIFO_OUT(&BaskeList,(char*)BasketData);			//篮子信息出列:接到在本窗口前面的提篮自信上报或者本窗口本应到篮子时出列
		memset(BasketData,0x00,5);												//清除篮子数据
	}
	ListLength=GetListLength((&BaskeList)->HeadNode);								//获取链表长度
	if(ListLength)
	{
		USART_DMAPrintf(USART1,"当前链表长度:%2d",ListLength);
	}
	if(len)
	{
		return (bool)1;
	}
	else
	{
		return (bool)0;
	}
}
//===============================================================================
//函数:
//描述:
//返回:
//===============================================================================
void TimeServer(void)
{
	//============================时间管理
	//传送线运行计时:超时后停止传送线
	//其它时间


	if(ACMOTOR_RunFlag==1)			//传送线所有电机运行标志:0-空闲,1-运行,2-停止,3-暂停
	{
		TranTimeCount++;
		if((TranTimeCount/(1000/SysTickTime))>=TranTimeOut)
		{			
//			TranTimeCount=0;									//传送线运行计时,当达到ACMotorTimeOut后停止传送线
//			ACMOTOR_RunFlag	=	0;							//传送线所有电机运行标志:0-空闲,1-运行,2-停止,3-暂停
			TranLine_Contrl(StopTranLineAll);		//传送线控制
		}
	}

}
//===============================================================================
//函数:
//描述:
//返回:
//===============================================================================
void PowerUp(void)
{
	if(InitFlag==0	&&	M1STAS==RUN_IDLE&&	M2STAS==RUN_IDLE)		//现在为停止命令
	{
		M2STAS	=	RUN_START;
		STEP_MOTO_2DOWN.MOTO_COMMAND		=		MOTO_COMMAND_RUN;			//启动提篮电机
	}
}

//===============================================================================
//函数:
//描述:
//返回:
//===============================================================================
void Buzzer_Configuration(void)
{
	GPIO_Configuration_OPP50	(BuzzerPort,	BuzzerPin);			//将GPIO相应管脚配置为PP(推挽)输出模式，最大速度50MHz----V20170605
}
//===============================================================================
//函数:
//描述:
//返回:
//===============================================================================
void Buzzer_Server(void)
{
	//BuzzerFrq				//蜂鸣器频率,单位Hz
	//BuzzerTime			//蜂鸣器输出频率计数
	//SysTickTime			//嘀嗒时钟 ----扫描周期,单位us
	//CacheFullFlag		//缓存架满标志:0-未满,1-预警2-已满
	if(CacheFullFlag	==	1)
	{
		BuzzerTime++;
		if(BuzzerTime	>=	(1000*(1000/SysTickTime)/(2*BuzzerFrq1)))
		{
			BuzzerTime	=	0;
			GPIO_Toggle	(BuzzerPort,	BuzzerPin);		//将GPIO相应管脚输出翻转----V20170605
		}
	}
	else if(CacheFullFlag	==	2)
	{
		BuzzerTime++;
		if(BuzzerTime	>=	(1000*(1000/SysTickTime)/(2*BuzzerFrq2)))
		{
			BuzzerTime	=	0;
			GPIO_Toggle	(BuzzerPort,	BuzzerPin);		//将GPIO相应管脚输出翻转----V20170605
		}
	}
	else
	{
		GPIO_ResetBits(BuzzerPort, BuzzerPin);
	}
	
}







#endif
