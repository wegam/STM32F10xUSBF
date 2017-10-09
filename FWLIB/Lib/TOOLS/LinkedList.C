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

//LINK_NODE** pNODE;
//===============================================================================
//函数:	CreateNode
//描述:	创建节点
//输入:	DataLenth-待存储的数据长度
//返回:	NULL--空地址,表示申请失败;否则返回创建的节点地址
//===============================================================================
LINK_NODE *CreateNode(char* DataAddr,unsigned long DataLength)
{
	LINK_NODE *NewNode; 		//NewNode保存创建的新结点的地址
	if(DataLength==0	||	DataAddr==NULL)				//数据长度为0,不执行
	{
		return	NULL;
	}
	//==================为此结点申请动态空间
	NewNode = (LINK_NODE *) malloc (sizeof(LINK_NODE)); 	//开辟一个新结点(申请动态存储空间)

	if (NewNode == NULL)	//申请失败
	{
		return NULL;				//返回空地址
	}
	else									//申请成功:下一步申请数据空间
	{
		//==================为此结点申请数据存储空间----使用动态存储空间方式
//		NewNode->DataAddr	= (char *) malloc (DataLength);			//申请数据存储动态存储空间
//		if(NewNode->DataAddr	== NULL)	//数据空间申请失败:释放结点,返回NULL
//		{
//			free(NewNode);								//释放建立的新结点
//			return NULL;									//返回空地址
//		}
		
		NewNode->DataLen=DataLength;											//此结点存储的数据大小
		memcpy(NewNode->DataAddr,DataAddr,DataLength);		//复制数据		
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
		NewNODEx->Serial	=	HeadNODEx->Serial+1;

		return	NewNODEx;
	}
}
////===============================================================================
////函数:	DeleteNode
////描述:	删除本结点
////输入:	HeadNODEx-头结点,NewNODEx-待插入的结点
////返回:	头结点
////===============================================================================
//LINK_NODE *DeleteNode(LINK_NODE* DelNODEx)
//{
//	LINK_NODE* pNODE1;
//	pNODE1=DelNODEx;
//	if(DelNODEx	==	NULL)
//	{
//		return NULL;
//	}
//	if(pNODE1->PrevNode==NULL)	//上一个结点为空
//	{
//		if(pNODE1->NextNode==NULL)										//上一个结点为空,下一个结点为空
//		{
////			free(pNODE1->DataAddr);											//释放结点内的数据空间
//			free(pNODE1);																//释放结点
//			pNODE1=NULL;
////			DelNODEx=NULL;
//		}
//		else											//上一个结点为空,下一个结点非空
//		{
//			DelNODEx=pNODE1->NextNode;
//			DelNODEx->PrevNode=NULL;
////			free(pNODE1->DataAddr);											//释放结点内的数据空间
//			free(pNODE1);																//释放结点
////			pNODE1=NULL;
//		}
//	}
//	else												//上一个结点非空,下一个结点为空
//	{
//		if(pNODE1->NextNode==NULL)										//上一个结点非空,下一个结点为空
//		{
//			pNODE1->PrevNode->NextNode=	NULL;
////			free(pNODE1->DataAddr);											//释放结点内的数据空间
//			free(pNODE1);																//释放结点
//			free(DelNODEx);															//释放结点
////			pNODE1=NULL;
////			DelNODEx=NULL;
//		}
//		else											//上一个结点非空,下一个结点非空
//		{
//			pNODE1->PrevNode->NextNode=pNODE1->NextNode;
//			pNODE1->NextNode->PrevNode=pNODE1->PrevNode;
//			
////			free(pNODE1->DataAddr);											//释放结点内的数据空间
//			free(pNODE1);																//释放结点
//			pNODE1=NULL;
//			free(DelNODEx);															//释放结点
//			DelNODEx=NULL;
//		}
//	}
//	return DelNODEx;				//返回下一结点地址
//}
//===============================================================================
//函数:	DeleteNode
//描述:	删除本结点
//输入:	HeadNODEx-删除的结点
//返回:	
//===============================================================================
LINK_NODE *DeleteNode(LINK_NODE** DelNODEx)
{
	LINK_NODE* pNODE;
	
	pNODE=*DelNODEx;
	//==================判断结点是否为空
	if(pNODE	==	NULL)
	{
		return NULL;
	}
//	//==================动态数据方式:需要释放数据空间
//	free(pNODE->DataAddr);										//释放数据空间
	//==================结点头尾判断
	if(pNODE->PrevNode==NULL)			//----------------上一个结点为空
	{
		if(pNODE->NextNode==NULL)										//上一个结点为空,下一个结点为空
		{
			free(pNODE);															//释放已删除结点
			*DelNODEx	=	NULL;													//删除地址
		}
		else																				//上一个结点为空,下一个结点非空
		{
			(*DelNODEx)	=	(*DelNODEx)->NextNode;				//待删除结点地址更新为下一结点地址
			(*DelNODEx)->PrevNode		=	NULL;	//下一结点的头结点更改为空结点
			free(pNODE);															//释放已删除结点
		}
	}
	else													//----------------上一个结点非空
	{
		if(pNODE->NextNode==NULL)										//上一个结点非空,下一个结点为空
		{
			(*DelNODEx)->PrevNode->NextNode	=	NULL;		//上结点的尾结点设置为空
			(*DelNODEx)	=	(*DelNODEx)->PrevNode;			//此结点地址更新为上一结点地址
			free(pNODE);															//释放已删除结点
		}
		else											//----------------上一个结点非空,下一个结点非空
		{
			(*DelNODEx)->PrevNode->NextNode=(*DelNODEx)->NextNode;	//上结点尾结点连接到此结点的下结点
			(*DelNODEx)->NextNode->PrevNode=(*DelNODEx)->PrevNode;	//下结点的头结点连接上结点的尾结点
			(*DelNODEx)	=	(*DelNODEx)->PrevNode;			//此结点地址更新为上一结点地址
			free(pNODE);															//释放已删除结点
		}
	}
	return *DelNODEx;						//返回新结点地址
}
//===============================================================================
//函数:	DeleteNode
//描述:	删除本结点
//输入:	HeadNODEx-头结点,NewNODEx-待插入的结点
//返回:	头结点
//===============================================================================
LINK_NODE *GetEndNode(LINK_NODE* DelNODEx)
{
	LINK_NODE* pNODE;
	pNODE	=	DelNODEx;
	if(pNODE	==	NULL)
	{
		return NULL;
	}
	if(pNODE->NextNode	==NULL)
	{
		return pNODE;
	}
	else
	{
		GetEndNode(pNODE->NextNode);
	}
	return pNODE;
}
//===============================================================================
//函数: FindData
//描述: 查找相关数据结点
//输入: HeadNODEx-头结点,DataAddr-待查找的数据,DataLength-数据长度
//返回: 查找的结点
//===============================================================================
LINK_NODE **FindData(LINK_LIST	*LISTx,char* DataAddr,unsigned long DataLength)
{
	unsigned short i	=0;
	LINK_NODE** DelNODEx	=	NULL;
	
	if(LISTx->HeadNode	==	NULL)
	{
		return NULL;
	}
	*DelNODEx	=	LISTx->HeadNode;
	for(i=0;i<0xFF;i++)
	{
		if((*DelNODEx)	==	NULL)
		{
			return NULL;
		}
		else if(memcmp((*DelNODEx)->DataAddr,DataAddr,DataLength)	==	0)				//查找到数据//比较内存数据相等
		{
			return	DelNODEx;
		}
		else
		{
			(*DelNODEx)	=	(*DelNODEx)->NextNode;
		}
	}
	return NULL;
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
//函数:	FIFO_IN--入列:FIFO存储数据---First in, First out
//描述:	存入数据
//输入:	DataAddr-待存储的数据地址
//输入:	DataLenth-待存储的数据长度
//返回:	入列结果0--失败,SaveLength--已经保存的数据大小
//===============================================================================
unsigned long FIFO_IN(LINK_LIST	*LISTx,char* SaveAddr,unsigned long SaveLength)
{
	//==================临时变量
	LINK_NODE *NewNode = NULL; 											//临时数据
	//==================创建结点:返回结点地址
	NewNode	=	CreateNode(SaveAddr,SaveLength);			//得到新申请的结点地址
	//==================判断申请结点结果			
	if(NewNode==NULL)		//申请失败
	{
		return 0;					//退出,返回0
	}
	//==================判断此链表是否为空链表(头结点是否为空)
	if(LISTx->HeadNode==NULL)												//空链表,建立表头
	{
		LISTx->HeadNode	=	NewNode;										//将新结点添加到链表前端
		LISTx->HeadNode->PrevNode	=	NULL;							//上结点设置为空
		LISTx->HeadNode->NextNode	=	NULL;							//下结点设置为空
		LISTx->HeadNode->Serial	=	1;									//顺序序号
		LISTx->ListLength	=	1;												//链表长度计数--起始为1
		return	1;
	}
	//==================创建链表尾结点:FIFO
	else if(LISTx->LastNode==NULL)									//建立与链表头的连接
	{
		LISTx->LastNode=NewNode;											//结点地址
		//----------------与链表头连接
		LISTx->LastNode->PrevNode	=	LISTx->HeadNode;	//上结点连接链表头
		LISTx->LastNode->NextNode	=	NULL;							//下结点为空
		//----------------链表头与链表尾连接
		LISTx->HeadNode->NextNode	=	LISTx->LastNode;	//头结点与尾结点连接
		LISTx->LastNode->Serial		=	2;								//第二个结点
		LISTx->ListLength	=	2;												//链表长度2
		return	1;
	}
	//==================以下为链表已建立后的添加结点过程
	//增加结点从尾结点后端增加
	//增加完后更新尾结点地址
	LISTx->LastNode=AddNode(LISTx->LastNode,NewNode);	//将新结点添加到链表尾端
	LISTx->ListLength	+=	1;												//链表长度计数
	return	1;
}

//===============================================================================
//函数:	FIFO_OUT--出列:FIFO输出数据---First in, First out
//描述:	读出数据--从HeadNode读取,读取完,HeadNode指向下一个地址后释放已读取完的结点
//输入:	DataAddr--数据地址
//返回:	DataLenth---存储的数据大小
//===============================================================================
unsigned long FIFO_OUT(LINK_LIST	*LISTx,char* ReadAddr)
{
	//==================空结点
	if(LISTx->HeadNode==NULL)							//链表头结点为空---空链表
	{
		return 0;														//退出
	}
	//==================非空结点
	else		
	{
//		//==================释放旧数据缓存
//		free(LISTx->DataAddr);
//		//==================获取新数据长度
//		LISTx->Length	=	LISTx->HeadNode->DataLen;													//结点内数据长度
//		//==================申请新数据存储空间
//		LISTx->DataAddr =	(char*)malloc (LISTx->Length); 									//根据数据的大小重新申请链表内数据存储空间
//		
//		if(LISTx->DataAddr	==	NULL)		//申请失败
//		{
//			return 0;
//		}
		
		
		//==================获取新数据长度
		LISTx->DataLength	=	LISTx->HeadNode->DataLen;													//结点内数据长度
		//==================复制数据
		memcpy(LISTx->DataAddr,LISTx->HeadNode->DataAddr,LISTx->DataLength);	//复制结点内数据到链表缓存		
		memcpy(ReadAddr,LISTx->HeadNode->DataAddr,LISTx->DataLength);					//复制结点内数据到外部缓存
		ReadAddr[0]	=	LISTx->HeadNode->Serial;
		//==================链表尾处理
		if(LISTx->HeadNode->NextNode->NextNode	==	NULL)		//删除的是链表尾结点
		{
			LISTx->LastNode	=	NULL;					//尾结点地址清除
		}
		//==================删除结点
		DeleteNode(&(LISTx->HeadNode));		//删除结点
		LISTx->ListLength	-=	1;					//链表长度减1
		
		return LISTx->DataLength;					//返回数据宽度
	}
}
//===============================================================================
//函数:	FIFO_DEL
//描述:	FIFO删除相应数据的结点
//输入:	HeadNODEx-头结点,NewNODEx-待插入的结点
//返回:	头结点
//===============================================================================
unsigned long FIFO_DEL(LINK_LIST	*LISTx,char* DataAddr,unsigned long DataLength)
{
	LINK_NODE** pNODE	=	NULL;

	if(LISTx->HeadNode	==	NULL)
	{
		return 0;
	}

	//==================查找数据
	pNODE	=	FindData(LISTx,DataAddr,DataLength);
	//==================查找结果处理
	if(*pNODE	==	NULL)			//未找到数据
	{
		return 0;
	}
	//==================链表尾处理
	if(
			((*pNODE)->NextNode==NULL	&&	(*pNODE)->PrevNode->PrevNode==NULL)	//两个结点时删除尾结点
		||((*pNODE)->PrevNode==NULL	&&	(*pNODE)->NextNode->NextNode==NULL)	//两个结点时删除头结点
		)//只有两个结点时需要将链表尾删除
	{
		LISTx->LastNode	=	NULL;
	}
	else if((*pNODE)->NextNode==NULL	&&	(*pNODE)->PrevNode!=NULL)		//如果是尾部结点,需要将链表尾连接上尾部结点前端
	{
		LISTx->LastNode	=	(*pNODE)->PrevNode;
	}
	DataAddr[0]=(*pNODE)->Serial;
	
//	pNODE	=	&HeadNODEx;
	//==================删除结点
	DeleteNode(pNODE);					//删除结点
	LISTx->ListLength	-=	1;		//链表长度减1
	return 1;	
}
//===============================================================================
//函数:	LIFO_IN--入栈:单向链表:LIFO存储数据---Last in, First out
//描述:	与FIFO_IN不同之处为在链表头添加数据
//输入:	DataAddr-待存储的数据地址
//输入:	DataLenth-待存储的数据长度
//返回:	入列结果0--失败,SaveLength--已经保存的数据大小
//===============================================================================
unsigned long LIFO_IN(LINK_LIST	*LISTx,char* SaveAddr,unsigned long SaveLength)
{
	LINK_NODE *NewNode = NULL; 											//临时数据
	LISTx->DataLength	=	SaveLength;											//保存待存储的数据大小
	NewNode	=	CreateNode(SaveAddr,SaveLength);								//得到新申请的结点地址
						
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
//函数:	LIFO_OUT--出栈:单向链表:LIFO输出数据---Last in, First out
//描述:	出栈,与FIFO_OUT相同
//输入:	DataAddr--数据地址
//返回:	DataLenth---存储的数据大小
//===============================================================================
unsigned long LIFO_OUT(LINK_LIST	*LISTx,char* ReadAddr)
{
	//==================空结点
	if(LISTx->HeadNode==NULL)							//链表头结点为空---空链表
	{
		LISTx->DataLength	=	0;									//读取的数据长度为0
//		free(LISTx->DataAddr);							//先释放之前链表内数据存储空间
//		LISTx->DataAddr		=	NULL;						//数据地址为空
		return 0;														//退出
	}
	//==================非空结点
	else		
	{
		free(LISTx->DataAddr);																								//先释放之前链表内数据存储空间
		
		LISTx->DataLength	=	LISTx->HeadNode->DataLen;													//结点内数据长度
		
//		LISTx->DataAddr =	(char*)malloc (LISTx->Length); 										//根据数据的大小重新申请链表内数据存储空间
		
		memcpy(LISTx->DataAddr,LISTx->HeadNode->DataAddr,LISTx->DataLength);	//复制结点内数据到链表缓存
		
		memcpy(ReadAddr,LISTx->HeadNode->DataAddr,LISTx->DataLength);					//复制结点内数据到外部缓存
		
		LISTx->HeadNode=DeleteNode(&(LISTx->HeadNode));												//删除结点
		
		return LISTx->DataLength;																							//返回数据宽度
	}
}
//===============================================================================
//函数:	ListTest
//描述:	
//输入:	
//返回:	
//===============================================================================
unsigned long LinkListTest(LINK_LIST	*LISTx,char* DataAddr,unsigned long SaveLength,unsigned char CMD)
{
	unsigned long	RevLength	=	0;
	//===========================FIFO测试
	//--------------------入列			----命令0x01;
	if(CMD	==	0x01)
	{
		RevLength	=	FIFO_IN(LISTx,DataAddr,SaveLength);		//FIFO存储数据---First in, First out
	}
	//--------------------出列			----命令0x02
	else if(CMD	==	0x02)
	{
		RevLength	=	FIFO_OUT(LISTx,DataAddr);							//FIFO输出数据---First in, First out
	}	
	//--------------------删除结点		----命令0x03
	else if(CMD	==	0x03)
	{
		RevLength	=	FIFO_DEL(LISTx,DataAddr,SaveLength);	//FIFO删除相应数据的结点
	}
	//--------------------查找数据		----命令0x04
	else if(CMD	==	0x04)
	{
	}
	//--------------------清空链表		----命令0x05
	else if(CMD	==	0x05)
	{
	}
	
	
	//===========================LIFO测试
	//--------------------入列			----命令0x11
	else if(CMD	==	0x11)
	{
	}
	//--------------------出列			----命令0x12
	else if(CMD	==	0x11)
	{
	}
	//--------------------删除结点		----命令0x13
	else if(CMD	==	0x11)
	{
	}
	//--------------------查找数据		----命令0x14
	else if(CMD	==	0x11)
	{
	}
	//--------------------清空链表		----命令0x15
	else if(CMD	==	0x11)
	{
	}
	return RevLength;
}



