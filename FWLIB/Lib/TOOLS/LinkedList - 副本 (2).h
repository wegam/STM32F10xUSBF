#ifndef __FIFO_H
#define __FIFO_H


typedef struct _LINK_NODE
{	
	unsigned long 	DataLen;		//存储的数据长度
	char* 					DataAddr;			//数据动态存储空间
	struct _LINK_NODE* PrevNode;	//上一个节点:仅双向链表时使用
	struct _LINK_NODE* NextNode;	//下一个节点	:
}LINK_NODE;


//typedef struct _LINK_LIST
//{
//	unsigned long	*HeadNodeAddr;	//头结点地址
//	unsigned long	*LastNodeAddr;	//尾结点地址
//	
//	unsigned long 	Length;			//待保存的数据长度
//	char* 					DataAddr;		//待保存数据地址
//}LINK_LIST;

typedef struct _LINK_LIST
{
	LINK_NODE	*HeadNode;		//头结点--
	LINK_NODE	*LastNode;		//尾结点--保存原链表最后一个结点的地址
	
	unsigned long 	Length;			//待保存的数据长度
	char* 					DataAddr;		//待保存数据地址
}LINK_LIST;


LINK_NODE *CreateNode(unsigned long DataLength);																	//创建结点
LINK_NODE *InsertNode(LINK_NODE* HeadNODEx,LINK_NODE* NewNODEx);									//插入结点
LINK_NODE *AddNode(LINK_NODE* HeadNODEx,LINK_NODE* NewNODEx);											//增加结点
LINK_NODE *DeleteNode(LINK_NODE* DelNODEx);																				//删除结点


unsigned long GetListLength(LINK_NODE* HeadNODEx);																		//获取链表长度
unsigned long FIFO_IN		(LINK_LIST	*NODEx,char* SaveAddr,unsigned long SaveLength);	//FIFO存储数据---First in, First out
unsigned long FIFO_OUT	(LINK_LIST	*DATAx,char* ReadAddr);														//FIFO输出数据---First in, First out
unsigned long FIFO_DEL	(LINK_LIST	*NODEx,char* DataAddr,unsigned long SaveLength);	//FIFO删除相应数据的结点

unsigned long LIFO_IN		(LINK_LIST	*LISTx,char* SaveAddr,unsigned long SaveLength);	//LIFO存储数据---Last in, First out
unsigned long LIFO_OUT	(LINK_LIST	*LISTx,char* ReadAddr);														//LIFO输出数据---Last in, First out




#endif


