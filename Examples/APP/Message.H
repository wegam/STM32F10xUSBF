#ifndef __Message_H
#define __Message_H


//----------通用应答
typedef	enum	_GENERAL_ACK	
{
	APP_GENERAL_ACK	=	0xFF,		//网关与下级之间的通用应答
	APP_GENERAL_PAK	=	0x55,		//网关与下级之间的正应答
	APP_GENERAL_NAK	=	0xAA		//网关与下级之间的负应答
}GeneralAck;
//----------命令码
typedef	enum	_APP_CMD
{
	APP_CMD_GETTANKINFO			=	0x01,		//得到槽位信息
	APP_CMD_TANKINFOREP			=	0x81,		//槽位信息上报
	APP_CMD_DISPEN					=	0x02,		//发药
	APP_CMD_DISPENREP				=	0x82,		//发药结果上报
	APP_CMD_TIEONCARD				=	0x03,		//绑卡
	APP_CMD_TIEONCARDREP		=	0x83,		//绑卡结果上报
	APP_CMD_SENDBASKET			=	0x04,		//送蓝
	APP_CMD_SENDBASKETREP		=	0x84,		//送蓝结果上报
	APP_CMD_SEATREP					=	0x85,		//状态或者报警信息上报
	APP_CMD_STARTEXTTRANS		=	0x86,		//启动外部传输线
	APP_CMD_CABASBASKETREP	=	0x87,		//缓存架上报提篮篮子号
	APP_CMD_MAJ2MINSENDINFO	=	0x88,		//M型主缓存架向辅缓存架发送篮子信息
	APP_CMD_CacheData				=	0x30,		//篮子信息
	APP_CMD_ACMotorCtl			=	0x31,		//传送线电机控制信息
	APP_CMD_GetMasterID			=	0x32,		//请求主机地址
	APP_CMD_SendMasterID		=	0x33		//发送主机地址
}AppCmd;
//----------设备状态消息：编号从0x00到0x3F
typedef	enum	_MSG_STATUS	
{
	MSG_STATUS_OK							=	0x00,	//正常状态，命令正常无误的被处理
	MSG_STATUS_BUSY						=	0x01,	//忙，无法对命令进行处理
	MSG_STATUS_CACHEFULL			=	0x02,	//缓存结构已满
	MSG_STATUS_CACHENONEMPTY	=	0x03	//缓存结构非满
}MsgStatus;
//----------错误消息：编号从0x40到0x7F
typedef	enum	_MSG_ERROR	
{
	MSG_ERROR_CMD							=	0x40,	//命令错误，接收到非法的命令
	MSG_ERROR_PARAMETER				=	0x41,	//参数错误，接收到非法的参数
	MSG_ERROR_BASKETNONSELF		=	0x42	//缓存架所提的篮子窗口号与当前位置不匹配
}MsgError;
//----------报警消息：编号从0x80到0xBF
typedef	enum	_MSG_ALARM	
{
	MSG_ALARM_DRUGSTUCK				=	0x80,		//药品被卡住
	MSG_ALARM_LACKDRUG				=	0x81,		//缺药
	MSG_ALARM_WAITRESOT				=	0x82,		//等待命令结果超时
	MSG_ALARM_SEPARATEMOTOR		=	0x83,		//分篮电机卡死
	MSG_ALARM_FALLMOTOR				=	0x84,		//落篮电机卡死
	MSG_ALARM_EMPTYBASKET			=	0x85,		//分篮结构空蓝
	MSG_ALARM_LACKBASKET			=	0x86,		//分篮结构缺蓝
	MSG_ALARM_STUCKBASKET_PB	=	0x87,		//分篮结构卡篮,存在分篮和送蓝相关流程
	MSG_ALARM_BASKETREMOVED		=	0x88,		//分篮结构篮子已经被取走
	MSG_ALARM_STUCKBASKET_TD	=	0x89,		//补药区下坡卡篮
	MSG_ALARM_STUCKBASKET_TC	=	0x8A,		//补药区提篮结构卡篮
	MSG_ALARM_STUCKBASKET_C1	=	0x8B,		//缓存架提篮电机1卡死
	MSG_ALARM_STUCKBASKET_C2	=	0x8C,		//缓存架提篮电机2卡死
	MSG_ALARM_READCARD_FAIL		=	0x8D,		//读IC卡失败
	MSG_ALARM_WRITECARD_FAIL	=	0x8E,		//写IC卡失败,数据没有写入卡中
	MSG_ALARM_WRITECARD_ERR		=	0x8F		//写IC卡错误,写入卡中的数据不对
}MsgAlarm;
//----------设备异常消息：编号从0xC0到0xFF
typedef	enum	_MSG_TROUBLE	
{
	MSG_TROUBLE_UNITCONTROL		=	0xC0,	/*单元柜控制器通信异常*/
	MSG_TROUBLE_DISPENLAYER		=	0xC1,	/*发药层控制器通信异常*/
	MSG_TROUBLE_TIEONCARDOT		=	0xC2,	/*绑卡超时,长时间无信息返回*/
	MSG_TROUBLE_SENDBASKETOT	=	0xC3,	/*送蓝超时,长时间无信息返回*/
	MSG_TROUBLE_READERCOMM		=	0xC4,	/*读卡器通信异常*/
	MSG_TROUBLE_READER				=	0xC5	/*读卡器异常*/
}MsgTrouble;

//----------以上内容打包
typedef struct	_SYS_INFO
{
	GeneralAck 	SYSACK;		//----------通用应答
	AppCmd			SYSCMD;		//----------命令码
	MsgStatus		SYSMSG;		//----------设备状态消息：编号从0x00到0x3F
	MsgAlarm		SYSALM;		//----------报警消息：编号从0x80到0xBF
	MsgTrouble	SYSTRB;		//----------设备异常消息：编号从0xC0到0xFF
}SYS_INFO;







#endif

