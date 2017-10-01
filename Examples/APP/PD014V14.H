#ifndef __PD014V14_H
#define __PD014V14_H


//��ҩ���������
#define	ct_pmos1	PA7
#define	ct_pmos2	PC5
#define	ct_pmos3	PB1
#define	ct_pmos4	PB11
#define	ct_pmos5	PB13
#define	ct_pmos6	PB15
#define	ct_pmos7	PC7
#define	ct_pmos8	PC9

//��ҩ���������翪��
#define	P_Sens	PA5

//��ҩ������
#define	Sens_In1	PA6in
#define	Sens_In2	PC4in
#define	Sens_In3	PB0in
#define	Sens_In4	PB10in
#define	Sens_In5	PB12in
#define	Sens_In6	PB14in
#define	Sens_In7	PC6in
#define	Sens_In8	PC8in

//���뿪�� S1~S3Ϊ��� 0x00~0x03��S4~S7Ϊ��ţ�S8ΪCAN�������ú�
#define	PD014_Switch1	PB7in		//LSB
#define	PD014_Switch2	PB6in		
#define	PD014_Switch3	PB5in
#define	PD014_Switch4	PB4in
#define	PD014_Switch5	PB3in
#define	PD014_Switch6	PC12in
#define	PD014_Switch7	PC11in
#define	PD014_Switch8	PC10in	//MSB



#define RS485BufferSize 32

//���ݸ�ʽ:B0-SWITCHID��B1-CMD��B2~B9:���ݣ�B10:ǰ�������������У��


typedef	enum _PD014_COMMAND		//PD014����(״̬��ѯ������ҩͷ����ҩ���ϱ���ҩ��������õ��������ʱ�䡢���ã�
{
//˵����
//1)��ҩ���ư幤���ڴӻ�ģʽ�����еĶ��������ϲ���ƣ����������·��ͽ���ϱ���ͨ��������ִ��
//2)Э��ִ�г��򣬽��յ���λ�������
//3)
	PD014_CMD_IDLE,				//������--����Ҫִ�еĶ���
	PD014_CMD_GetDevice,	//��ȡ���߷�ҩͷ���ɴ����������������������Ϊģʽ��
	PD014_CMD_GetStatus,	//��ȡ״̬
	PD014_CMD_GetSED,			//��ȡ�ѷ�ҩ����
	
	PD014_CMD_SetStatus,	//����״̬
	PD014_CMD_SetWSD,			//���÷�ҩ��������
}PD014_CMD_TypeDef;

typedef	enum _PD014_STATUS		//PD014״̬(���С�æ�����ڷ�ҩ����ҩ��ɡ�ȱҩ�����ò������ϱ����ݡ��ϱ���ɣ�
{
//˵����
//1)��ҩ���ư幤���ڴӻ�ģʽ�����еĶ��������ϲ���ƣ����������·��ͽ���ϱ���ͨ��������ִ��
//2)Э��ִ�г��򣬽��յ���λ�������
//3)	
	PD014_STA_IDLE,				//����----���Խ����������ִ�ж���
	PD014_STA_BUSY,				//æ
	PD014_STA_UPDATA,			//�����ϱ�����
	PD014_STA_SEND,				//��ҩ
	PD014_STA_SENDING,		//���ڷ�ҩ
	PD014_STA_SENDED,			//��ҩ���
	PD014_STA_SHORTAGE,		//ȱҩ
	PD014_STA_CLOGGED,		//��ҩ/��ҩ
	PD014_STA_CONF,				//���ò���
	PD014_STA_CONFING,		//�������ò���
	PD014_STA_WaitAck			//�������ò���
}PD014_STATUS_TypeDef;

typedef	enum _PD014_Answer	//USART Ӧ��
{
	PD014_ACK			=0x79,		//ACK Ӧ��(�����ܣ�
	PD014_NACK		=0x1F,		//NACK Ӧ�𣨲����ܻ��߲�ʶ��--����)
	PD014_NoError,					//ACK Ӧ��(�����ܣ�
	PD014_BccError,				//NACK Ӧ�𣨲����ܻ��߲�ʶ��--����)
	PD014_BUSY							//�ӻ�æ״̬
}PD014_ANSWER_TypeDef;

typedef	enum _PD014_MODE	//USART Ӧ��
{
	NORMEL,									//ACK Ӧ��(�����ܣ�
	TEST										//NACK Ӧ�𣨲����ܻ��߲�ʶ��--����)
}PD014_MODE_TypeDef;

typedef	struct _PD014_DATA	//USART Ӧ��
{	
	unsigned char 	CANFLG;					//CAN����---CANFLG==1��ʹ��CAN--Ϊ���ϲ����ӵİ�
	unsigned char 	SWITCHID;				//���뿪��
	unsigned char 	DeviceOnlie;		//��ҩͷ��־---�洢�Ƿ���뷢ҩͷ���У���ǰλΪ1������Ϊ0	//1(�����Ϊ���λ)
	
	unsigned int TimeCount;					//���д�����ʱ��
	unsigned int TimeOut;						//֧�г�ʱ��ʱ��
	
	unsigned char 	WSD[8];					//����ҩ����
	unsigned char 	SED[8];					//�ѷ�ҩ����	
	
	unsigned short 	RELAYTIME[8];		//�̵�����Ҫ����ʱ��---Ĭ��100ms
	unsigned short 	RELAYCOUNT[8];	//�̵����Ѿ�����ʱ��---�ﵽRELAYTIME���ͷ�
	
	unsigned short 	SENSTIME[8];		//��������Ӧʱ��----��NOISETIME����ʹ�ü��㴫������Ӧʱ�䣬��Ҫ���˸����ź�
	unsigned short 	NOISETIME[8];		//����������������ʱ��	
	
	
	unsigned char RxdBuffe[RS485BufferSize];
	unsigned char RevBuffe[RS485BufferSize];
	unsigned char TxdBuffe[RS485BufferSize];
	unsigned char TevBuffe[RS485BufferSize];

	
	PD014_CMD_TypeDef Command;						//����
	PD014_STATUS_TypeDef	STATUS[8];			//״̬
	
}PD014_DATA_TypeDef;


typedef	struct _PD014_INF				//
{
	PD014_MODE_TypeDef MODE;			//����ģʽ��NORMEL��������ģʽ��TEST����ģʽ
	PD014_DATA_TypeDef PD014_DATA;
}PD014_INF_TypeDef;


void PD014V14_Configuration(void);
void PD014V14_Server(void);
void PD014V14_Process(void);		//PD014V14���а��ڴ�����������


void PD014V14_PinSet(void);
void Lock_Toggle(void);			//˫�����������

void PD014V14_GetSwitchID(void);				//��ȡ���뿪�ص�ַ
void PD014V14_GetOnlieDevice(void);			//��ȡ���߷�ҩͷ
void PD014V14_ACK(void);
void PD014V14_NACK(void);
void PD014V14_SendM(void);							//��ҩ
void PD014V14_ResetData(void);					//��λ����


#endif
