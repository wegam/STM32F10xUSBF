/************************************ TOOLS ************************************
* 文件名 	: LinkedList链表
* 作者   	: wegam@sina.com
* 版本   	: V
* 日期   	: 2017/09/11
* 说明   	: 
********************************************************************************
其它说明:
*
*
*
*
*
*
*
*
*******************************************************************************/
#include 	"LinkedList.H"

#include	"stdio.h"				//用于printf
#include	"string.h"			//用于printf
#include	"stdarg.h"			//用于获取不确定个数的参数
#include	"stdlib.h"			//malloc动态申请内存空间



//LINK_NODE	*HeadNode	=	NULL;		//头结点--
//LINK_NODE	*LastNode	=	NULL;		//尾结点--保存原链表最后一个结点的地址
//===============================================================================
//函数:	CreateNode
//描述:	创建节点
//输入:	DataLenth-待存储的数据长度
//返回:	NULL--空地址,表示申请失败;否则返回创建的节点地址
//===============================================================================
LINK_NODE *CreateNode(unsigned long DataLength)
{
	unsigned short Len=0;
	LINK_NODE *NewNode = NULL; 									//NewNode保存创建的新结点的地址
	Len=sizeof(LINK_NODE);
	if(DataLength==0)			//数据长度为0,不执行
	{
		return	NULL;
	}
	//==================为此结点申请动态空间
//	NewNode = (LINK_NODE *) malloc (sizeof(LINK_NODE)); 	//开辟一个新结点(申请动态存储空间)
	NewNode = (LINK_NODE *) malloc (Len); 	//开辟一个新结点(申请动态存储空间)
	if (NewNode == NULL)	//申请失败
	{
		return NULL;				//返回空地址
	}
	else									//申请成功:下一步申请数据空间
	{
		NewNode->DataLen=DataLength;				//此结点需要存储的数据大小
		//==================为此结点申请数据存储空间
		NewNode->DataAddr	= (char *) malloc (NewNode->DataLen);			//申请数据存储动态存储空间
		if(NewNode->DataAddr	== NULL)	//数据空间申请失败:释放结点,返回NULL
		{
			free(NewNode);								//释放建立的新结点
			return NULL;									//返回空地址
		}
	}
	return	NewNode;					//返回尾结点的地址:准备结尾结点添加数据
}
//===============================================================================
//函数:	InsertNode
//描述:	在头结点前面插入新结点
//输入:	HeadNODEx-头结点,NewNODEx-待插入的结点
//返回:	插入的结点地址
//===============================================================================
LINK_NODE *InsertNode(LINK_NODE* HeadNODEx,LINK_NODE* NewNODEx)
{
	if(HeadNODEx==NULL)						//头结点为空结点----新链表
	{
		NewNODEx->PrevNode	=	NULL;	//第一个结点的上结点为空结点
		NewNODEx->NextNode	=	NULL;	//新建立的链表的下一结点为空结点
		return	NewNODEx;
	}
	//==================非空结点
	else													//头结点非空结点
	{		
		if(HeadNODEx->PrevNode	==NULL)			//头结点的上结点为空
		{
			NewNODEx->PrevNode	=	NULL;				//新结点的上结点为空
			NewNODEx->NextNode	=	HeadNODEx;	//新结点的下结点到头结点
			HeadNODEx->PrevNode	=	NewNODEx;		//头结点的上结点连接到新结点
		}
		else																											//头结点的上结点非空
		{
			NewNODEx->PrevNode	=	HeadNODEx->PrevNode->NextNode;		//新结点	的上结点连接到头结点的上结点
			NewNODEx->NextNode	=	HeadNODEx;												//新结点的下结点连接到头结点
			HeadNODEx->PrevNode->NextNode=NewNODEx;									//头结点的上结点的下结点连接到新结点
			HeadNODEx->PrevNode=NewNODEx;														//头结点的上结点连接到新结点
		}
		return	NewNODEx;
	}
}
//===============================================================================
//函数:	InsertNode
//描述:	在头结点后添加新结点
//输入:	HeadNODEx-头结点,NewNODEx-待添加的结点
//返回:	新的头结点地址
//===============================================================================
LINK_NODE *AddNode(LINK_NODE* HeadNODEx,LINK_NODE* NewNODEx)
{
	if(HeadNODEx==NULL)						//头结点为空结点----新链表
	{
		NewNODEx->PrevNode	=	NULL;	//第一个结点的上结点为空结点
		NewNODEx->NextNode	=	NULL;	//新建立的链表的下一结点为空结点
		HeadNODEx	=	NewNODEx;				//
		return	NewNODEx;
	}
	//==================非空结点
	else													//头结点非空结点
	{		
		NewNODEx->NextNode=HeadNODEx->NextNode;		//新结点的下结点连接到头结点的下结点
		HeadNODEx->NextNode=NewNODEx;							//头结点的下结点连接到新结点
		NewNODEx->PrevNode=HeadNODEx;							//新结点的上结点连接到头结点
		return	NewNODEx;
	}
}
//===============================================================================
//函数:	DeleteNode
//描述:	在头结点后插入新结点
//输入:	HeadNODEx-头结点,NewNODEx-待插入的结点
//返回:	头结点
//===============================================================================
LINK_NODE *DeleteNode(LINK_NODE* DelNODEx)
{
	LINK_NODE* pNODE1;
	pNODE1=DelNODEx;
	if(pNODE1->PrevNode==NULL)	//上一个结点为空
	{
		if(pNODE1->NextNode==NULL)										//上一个结点为空,下一个结点为空
		{
			free(pNODE1->DataAddr);											//释放结点内的数据空间
			free(pNODE1);																//释放结点
			pNODE1=NULL;
			DelNODEx=NULL;
		}
		else											//上一个结点为空,下一个结点非空
		{
			DelNODEx=pNODE1->NextNode;
			DelNODEx->PrevNode=NULL;
			free(pNODE1->DataAddr);											//释放结点内的数据空间
			free(pNODE1);																//释放结点
			pNODE1=NULL;
		}
	}
	else												//上一个结点非空,下一个结点为空
	{
		if(pNODE1->NextNode==NULL)										//上一个结点非空,下一个结点为空
		{
			pNODE1->PrevNode->NextNode=	NULL;
			free(pNODE1->DataAddr);											//释放结点内的数据空间
			free(pNODE1);																//释放结点
			pNODE1=NULL;
			DelNODEx=NULL;
		}
		else											//上一个结点非空,下一个结点非空
		{
			pNODE1->PrevNode->NextNode=pNODE1->NextNode;
			pNODE1->NextNode->PrevNode=pNODE1->PrevNode;
			
			free(pNODE1->DataAddr);											//释放结点内的数据空间
			free(pNODE1);																//释放结点
			pNODE1=NULL;
			DelNODEx=NULL;
		}
	}
	return DelNODEx;				//返回下一结点地址
}

//===============================================================================
//函数:	GetListLength
//描述:	获取链表长度
//输入:	HeadNODEx-链表头结点:第一个结点
//返回:	链表的长度
//===============================================================================
unsigned long GetListLength(LINK_NODE* HeadNODEx)
{
	if(HeadNODEx==NULL)			//结点为空
	{
		return 0;					//
	}
	return	1+GetListLength(HeadNODEx->NextNode);
}
//===============================================================================
//函数:	FIFO_IN--入列
//描述:	存入数据
//输入:	DataAddr-待存储的数据地址
//输入:	DataLenth-待存储的数据长度
//返回:	入列结果0--失败,SaveLength--已经保存的数据大小
//===============================================================================
unsigned long FIFO_IN(LINK_LIST	*LISTx,char* SaveAddr,unsigned long SaveLength)
{
	LINK_NODE *NewNode = NULL; 											//临时数据
	LISTx->Length	=	SaveLength;											//保存待存储的数据大小
	NewNode	=	CreateNode(SaveLength);								//得到新申请的结点地址
						
	if(NewNode==NULL)		//申请失败
	{
		return 0;					//
	}
	memcpy(NewNode->DataAddr,SaveAddr,SaveLength);	//保存数据到新建的结点中
	
	if(LISTx->HeadNode==NULL)												//空链表,建立表头
	{
		LISTx->HeadNode=NewNode;											//将新结点添加到链表前端
		LISTx->HeadNode->PrevNode=NULL;
		LISTx->HeadNode->NextNode=NULL;
		LISTx->LastNode=NULL;
		return	SaveLength;
	}
	else if(LISTx->LastNode==NULL)									//建立与链表头的连接
	{
		LISTx->LastNode=NewNode;
		LISTx->LastNode->PrevNode=LISTx->HeadNode;
		LISTx->LastNode->NextNode=NULL;
		LISTx->HeadNode->NextNode=LISTx->LastNode;
		return	SaveLength;
	}
	LISTx->LastNode=AddNode(LISTx->LastNode,NewNode);					//将新结点添加到链表尾端
	return	SaveLength;
}

//===============================================================================
//函数:	FIFO_OUT--出列
//描述:	读出数据--从HeadNode读取,读取完,HeadNode指向下一个地址后释放已读取完的结点
//输入:	DataAddr--数据地址
//返回:	DataLenth---存储的数据大小
//===============================================================================
unsigned long FIFO_OUT(LINK_LIST	*LISTx,char* ReadAddr)
{
	//==================空结点
	if(LISTx->HeadNode==NULL)							//链表头结点为空---空链表
	{
		LISTx->Length	=	0;									//读取的数据长度为0
		free(LISTx->DataAddr);							//先释放之前链表内数据存储空间
		LISTx->DataAddr		=	NULL;						//数据地址为空
		return 0;														//退出
	}
	//==================非空结点
	else		
	{
		free(LISTx->DataAddr);																						//先释放之前链表内数据存储空间
		
		LISTx->Length	=	LISTx->HeadNode->DataLen;													//结点内数据长度
		
		LISTx->DataAddr =	(char*)malloc (LISTx->Length); 									//根据数据的大小重新申请链表内数据存储空间
		
		memcpy(LISTx->DataAddr,LISTx->HeadNode->DataAddr,LISTx->Length);	//复制结点内数据到链表缓存
		
		memcpy(ReadAddr,LISTx->HeadNode->DataAddr,LISTx->Length);					//复制结点内数据到外部缓存
		
		LISTx->HeadNode=DeleteNode((LISTx->HeadNode));										//删除结点
		
		if(LISTx->HeadNode->NextNode	==	NULL)
		{
			LISTx->LastNode	=	NULL;
		}		
		return LISTx->Length;																							//返回数据宽度
	}
}
//===============================================================================
//函数:	STACK_PUSH--入栈:单向链表
//描述:	与FIFO_IN不同之处为在链表头添加数据
//输入:	DataAddr-待存储的数据地址
//输入:	DataLenth-待存储的数据长度
//返回:	入列结果0--失败,SaveLength--已经保存的数据大小
//===============================================================================
unsigned long STACK_PUSH(LINK_LIST	*LISTx,char* SaveAddr,unsigned long SaveLength)
{
	LINK_NODE *NewNode = NULL; 											//临时数据
	LISTx->Length	=	SaveLength;											//保存待存储的数据大小
	NewNode	=	CreateNode(SaveLength);								//得到新申请的结点地址
						
	if(NewNode==NULL)		//申请失败
	{
		return 0;					//
	}
	memcpy(NewNode->DataAddr,SaveAddr,SaveLength);	//保存数据到新建的结点中
	
	if(LISTx->HeadNode==NULL)												//空链表,建立表头
	{
		LISTx->HeadNode=NewNode;											//将新结点添加到链表前端
		LISTx->HeadNode->PrevNode=NULL;
		LISTx->HeadNode->NextNode=NULL;
		LISTx->LastNode=NULL;
		return	SaveLength;
	}
	LISTx->HeadNode=InsertNode(LISTx->HeadNode,NewNode);					//将新结点添加到链表尾端
	return	SaveLength;
}

//===============================================================================
//函数:	STACK_POP--出栈:单向链表
//描述:	出栈,与FIFO_OUT相同
//输入:	DataAddr--数据地址
//返回:	DataLenth---存储的数据大小
//===============================================================================
unsigned long STACK_POP(LINK_LIST	*LISTx,char* ReadAddr)
{
	//==================空结点
	if(LISTx->HeadNode==NULL)							//链表头结点为空---空链表
	{
		LISTx->Length	=	0;									//读取的数据长度为0
		free(LISTx->DataAddr);							//先释放之前链表内数据存储空间
		LISTx->DataAddr		=	NULL;						//数据地址为空
		return 0;														//退出
	}
	//==================非空结点
	else		
	{
		free(LISTx->DataAddr);																						//先释放之前链表内数据存储空间
		
		LISTx->Length	=	LISTx->HeadNode->DataLen;													//结点内数据长度
		
		LISTx->DataAddr =	(char*)malloc (LISTx->Length); 									//根据数据的大小重新申请链表内数据存储空间
		
		memcpy(LISTx->DataAddr,LISTx->HeadNode->DataAddr,LISTx->Length);	//复制结点内数据到链表缓存
		
		memcpy(ReadAddr,LISTx->HeadNode->DataAddr,LISTx->Length);					//复制结点内数据到外部缓存
		
		LISTx->HeadNode=DeleteNode((LISTx->HeadNode));										//删除结点
		
		return LISTx->Length;																							//返回数据宽度
	}
}




