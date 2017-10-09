#ifndef __FIFO_H
#define __FIFO_H


typedef struct _LINK_NODE
{	
	unsigned long 	DataLen;		//�洢�����ݳ���
	char* 					DataAddr;			//���ݶ�̬�洢�ռ�
	struct _LINK_NODE* PrevNode;	//��һ���ڵ�:��˫������ʱʹ��
	struct _LINK_NODE* NextNode;	//��һ���ڵ�	:
}LINK_NODE;


//typedef struct _LINK_LIST
//{
//	unsigned long	*HeadNodeAddr;	//ͷ����ַ
//	unsigned long	*LastNodeAddr;	//β����ַ
//	
//	unsigned long 	Length;			//����������ݳ���
//	char* 					DataAddr;		//���������ݵ�ַ
//}LINK_LIST;

typedef struct _LINK_LIST
{
	LINK_NODE	*HeadNode;		//ͷ���--
	LINK_NODE	*LastNode;		//β���--����ԭ�������һ�����ĵ�ַ
	
	unsigned long 	Length;			//����������ݳ���
	char* 					DataAddr;		//���������ݵ�ַ
}LINK_LIST;


LINK_NODE *CreateNode(unsigned long DataLength);																	//�������
LINK_NODE *InsertNode(LINK_NODE* HeadNODEx,LINK_NODE* NewNODEx);									//������
LINK_NODE *AddNode(LINK_NODE* HeadNODEx,LINK_NODE* NewNODEx);											//���ӽ��
LINK_NODE *DeleteNode(LINK_NODE* DelNODEx);																				//ɾ�����


unsigned long GetListLength(LINK_NODE* HeadNODEx);																		//��ȡ������
unsigned long FIFO_IN		(LINK_LIST	*NODEx,char* SaveAddr,unsigned long SaveLength);	//FIFO�洢����---First in, First out
unsigned long FIFO_OUT	(LINK_LIST	*DATAx,char* ReadAddr);														//FIFO�������---First in, First out
unsigned long FIFO_DEL	(LINK_LIST	*NODEx,char* DataAddr,unsigned long SaveLength);	//FIFOɾ����Ӧ���ݵĽ��

unsigned long LIFO_IN		(LINK_LIST	*LISTx,char* SaveAddr,unsigned long SaveLength);	//LIFO�洢����---Last in, First out
unsigned long LIFO_OUT	(LINK_LIST	*LISTx,char* ReadAddr);														//LIFO�������---Last in, First out




#endif


