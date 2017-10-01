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
unsigned long FIFO_IN		(LINK_LIST	*NODEx,char* SaveAddr,unsigned long SaveLength);	//存储数据
unsigned long FIFO_OUT	(LINK_LIST	*DATAx,char* ReadAddr);														//输出数据
unsigned long STACK_PUSH(LINK_LIST	*LISTx,char* SaveAddr,unsigned long SaveLength);	//入栈
unsigned long STACK_POP	(LINK_LIST	*LISTx,char* ReadAddr);														//出栈




#endif


