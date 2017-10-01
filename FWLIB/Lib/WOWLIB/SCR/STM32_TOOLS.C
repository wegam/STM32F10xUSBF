/******************************** User_library *********************************
* 文件名 	: STM32_SDCard.H
* 作者   	: wegam@sina.com
* 版本   	: V
* 日期   	: 2016/01/01
* 说明   	: 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


#include "STM32_TOOLS.H"

#include <stdlib.h>

#include <stdio.h>


#define NULL 0

#define LEN sizeof(struct student)

struct student
{
	long num; 						/*学号 */
	float score; 					/*分数，其他信息可以继续在下面增加字段 */
	struct student *next; /*指向下一结点的指针 */
};
int n; /*结点总数 */


/////////////////////////////////CRC//////////////////////////////////////////
/*******************************************************************************
* 函数名		:	8位异或校验
* 功能描述	: 
* 输入			: *P 指针，numb数据长度（个数）
* 输出			: 计算结果
* 返回			: 无
*******************************************************************************/
u8 BCC8(u8 *Pointer,u16 num)			//异或校验
{
	u16 i_Num=0;
	u16 temp=0;
	for(i_Num=0;i_Num<num;i_Num++)
	{
		temp=temp^*Pointer;
		Pointer++;	
	}
	return temp;
}
/*******************************************************************************
* 函数名		:	异或校验
* 功能描述	: 
* 输入			: 
* 输出			: 计算结果
* 返回			: 无
*******************************************************************************/
u8 CRC8(u8 *Pointer)			//循环冗余校验
{
	return 0;
}
/*******************************************************************************
* 函数名		:	纵向冗余校验
* 功能描述	: 
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
u8 LRC8(u8 *Pointer)		//纵向冗余校验
{
	return 0;
}
/////////////////////////////////LIST链表//////////////////////////////////////////
/*******************************************************************************
* 函数名			:	CreateNode
* 功能描述		: 创建节点
* 输入			: 无
* 输出			: 无
* 返回			: 无
*******************************************************************************/
//LINK_LIST *CreateNode(u8 *Pointer)		
//{
//	return 0;
//}




//==========================
//功能：创建结点
//返回：指向链表表头的指针
//==========================

struct student *Create ()
{
	struct student *head; 			/*头结点 */
	struct student *p1 = NULL; 	/*p1保存创建的新结点的地址 */
	struct student *p2 = NULL; 	/*p2保存原链表最后一个结点的地址 */
	n = 0; 											/*创建前链表的结点总数为0：空链表 */
	p1 = (struct student *) malloc (LEN); 	/*开辟一个新结点 */
	p2 = p1;						/*如果结点开辟成功，则p2先把它的指针保存下来以备后用 */
	if (p1 == NULL) 		/*结点开辟不成功 */
	{
		printf ("\nCann't create it, try it again in a moment!\n");
		return NULL;
	}
	else 						/*结点开辟成功 */
	{
		head = NULL; 	/*开始head指向NULL */
		printf ("Please input %d node -- num,score: ", n + 1);
		scanf ("%ld,%f", &(p1->num), &(p1->score)); /*录入数据 */
	}
	while (p1->num != 0) /*只要学号不为0，就继续录入下一个结点 */
	{
		n += 1; 			/*结点总数增加1个 */
		if (n == 1) 	/*如果结点总数是1，则head指向刚创建的结点p1 */
		{
			head = p1;
			/*
			注意：
			此时的p2就是p1,也就是p1->next指向NULL。
			这样写目的是与下面else保持一致。
			*/
			p2->next = NULL;		//head==p1;
		}
		else
		{
			p2->next = p1; 	/*指向上次下面刚开辟的结点 */		//head->next==p1;
		}
		p2 = p1; 					/*把p1的地址给p2保留，然后p1去产生新结点 */
		p1 = (struct student *) malloc (LEN);
		printf ("Please input %d node -- num,score: ", n + 1);
		scanf ("%ld,%f", &(p1->num), &(p1->score));
	}
	p2->next = NULL; 	/*此句就是根据单向链表的最后一个结点要指向NULL */		//head->next==p1,p1->next==null
	free (p1); 				/*释放p1。用malloc()、calloc()的变量都要free() */
	p1 = NULL; 				/*特别不要忘记把释放的变量清空置为NULL,否则就变成"野指针"，即地址不确定的指针。 */
	return head; 			/*返回创建链表的头指针*/
}
//===========================
//功能：输出结点
//返回： void
//===========================
void Print (struct student *head)
{
	struct student *p;
	printf ("\nNow , These %d records are:\n", n);
	p = head;
	if (head != NULL) /*只要不是空链表，就输出链表中所有结点 */
	{
		printf ("head is %o\n", head); /*输出头指针指向的地址 */
		do
		{
		/*
		输出相应的值：当前结点地址、各字段值、当前结点的下一结点地址。
		这样输出便于读者形象看到一个单向链表在计算机中的存储结构，和我们
		设计的图示是一模一样的。
		*/
		printf ("%o %ld %5.1f %o\n", p, p->num, p->score, p->next);
		p = p->next; /*移到下一个结点 */
		}while (p != NULL);
	}
}
//==========================
//功能：删除指定结点
//(此例中是删除指定学号的结点)
//返回：指向链表表头的指针
//==========================
struct student *Del (struct student *head, long num)
{
	struct student *p1; /*p1保存当前需要检查的结点的地址 */
	struct student *p2; /*p2保存当前检查过的结点的地址 */
	if (head == NULL) /*是空链表（结合图3理解） */
	{
		printf ("\nList is null!\n");
		return head;
	}
	/*定位要删除的结点 */
	p1 = head;
	while (p1->num != num && p1->next != NULL) /*p1指向的结点不是所要查找的，并且它不是最后一个结点，就继续往下找 */
	{
		p2 = p1; /*保存当前结点的地址 */
		p1 = p1->next; /*后移一个结点 */
	}
	if (num == p1->num) /*找到了。（结合图4、5理解） */
	{
		if (p1 == head) /*如果要删除的结点是第一个结点 */
		{
			head = p1->next; /*头指针指向第一个结点的后一个结点，也就是第二个结点。这样第一个结点就不在链表中，即删除。 */
		}
		else /*如果是其它结点，则让原来指向当前结点的指针，指向它的下一个结点，完成删除 */
		{
			p2->next = p1->next;
		}
		free (p1); /*释放当前结点 */
		p1 = NULL;
		printf ("\ndelete %ld success!\n", num);
		n -= 1; /*结点总数减1个 */
	}
	else /*没有找到 */
	{
		printf ("\n%ld not been found!\n", num);
	}
	return head;
}
//==========================
//功能：插入指定结点的后面
//(此例中是指定学号的结点)
//返回：指向链表表头的指针
//==========================
struct student *Insert (struct student *head, long num, struct student *node)
{
	struct student *p1; /*p1保存当前需要检查的结点的地址 */
	if (head == NULL) /*（结合图示7理解） */
	{
		head = node;
		node->next = NULL;
		n += 1;
		return head;
	}
	p1 = head;
	while (p1->num != num && p1->next != NULL) /*p1指向的结点不是所要查找的，并且它不是最后一个结点，继续往下找 */
	{
		p1->next; /*后移一个结点 */
	}
	if (num == p1->num) /*找到了（结合图示8理解） */
	{
		node->next = p1->next; /*显然node的下一结点是原p1的next */
		p1->next = node; /*插入后，原p1的下一结点就是要插入的node */
		n += 1; /*结点总数增加1个 */
	}
	else
	{
		printf ("\n%ld not been found!\n", num);
	}
	return head;
}
//==========================
//功能：反序结点
//(链表的头变成链表的尾，链表的尾变成头)
//返回：指向链表表头的指针
//==========================
struct student *Reverse (struct student *head)
{
	struct student *p; /*临时存储 */
	struct student *p1; /*存储返回结果 */
	struct student *p2; /*源结果结点一个一个取 */
	p1 = NULL; /*开始颠倒时，已颠倒的部分为空 */
	p2 = head; /*p2指向链表的头结点 */
	while (p2 != NULL)
	{
		p = p2->next;
		p2->next = p1;
		p1 = p2;
		p2 = p;
	}
	head = p1;
	return head;
}
//==========================
//功能：选择排序(由小到大)
//返回：指向链表表头的指针
//==========================
struct student *SelectSort (struct student *head)
{
	struct student *first; /*排列后有序链的表头指针 */
	struct student *tail; /*排列后有序链的表尾指针 */
	struct student *p_min; /*保留键值更小的结点的前驱结点的指针 */
	struct student *min; /*存储最小结点 */
	struct student *p; /*当前比较的结点 */
	first = NULL;
	while (head != NULL) /*在链表中找键值最小的结点。 */
	{
		/*注意：这里for语句就是体现选择排序思想的地方 */
		for (p = head, min = head; p->next != NULL; p = p->next) /*循环遍历链表中的结点，找出此时最小的结点。 */
		{
			if (p->next->num < min->num) /*找到一个比当前min小的结点。 */
			{
				p_min = p; /*保存找到结点的前驱结点：显然p->next的前驱结点是p。 */
				min = p->next; /*保存键值更小的结点。 */
			}
		}
		/*上面for语句结束后，就要做两件事；一是把它放入有序链表中；二是根据相应的条件判断，安排它离开原来的链表。 */
		/*第一件事 */
		if (first == NULL) /*如果有序链表目前还是一个空链表 */
		{
			first = min; /*第一次找到键值最小的结点。 */
			tail = min; /*注意：尾指针让它指向最后的一个结点。 */
		}
		else /*有序链表中已经有结点 */
		{
			tail->next = min; /*把刚找到的最小结点放到最后，即让尾指针的next指向它。 */
			tail = min; /*尾指针也要指向它。 */
		}
		/*第二件事 */
		if (min == head) /*如果找到的最小结点就是第一个结点 */
		{
			head = head->next; /*显然让head指向原head->next,即第二个结点，就OK */
		}
		else /*如果不是第一个结点 */
		{
			p_min->next = min->next; /*前次最小结点的next指向当前min的next,这样就让min离开了原链表。 */
		}
	}
	if (first != NULL) /*循环结束得到有序链表first */
	{
		tail->next = NULL; /*单向链表的最后一个结点的next应该指向NULL */
	}
	head = first;
	return head;
}
//==========================
//功能：直接插入排序(由小到大)
//返回：指向链表表头的指针
//==========================















