/******************************** User_library *********************************
* �ļ��� 	: STM32_SDCard.H
* ����   	: wegam@sina.com
* �汾   	: V
* ����   	: 2016/01/01
* ˵��   	: 
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
	long num; 						/*ѧ�� */
	float score; 					/*������������Ϣ���Լ��������������ֶ� */
	struct student *next; /*ָ����һ����ָ�� */
};
int n; /*������� */


/////////////////////////////////CRC//////////////////////////////////////////
/*******************************************************************************
* ������		:	8λ���У��
* ��������	: 
* ����			: *P ָ�룬numb���ݳ��ȣ�������
* ���			: ������
* ����			: ��
*******************************************************************************/
u8 BCC8(u8 *Pointer,u16 num)			//���У��
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
* ������		:	���У��
* ��������	: 
* ����			: 
* ���			: ������
* ����			: ��
*******************************************************************************/
u8 CRC8(u8 *Pointer)			//ѭ������У��
{
	return 0;
}
/*******************************************************************************
* ������		:	��������У��
* ��������	: 
* ����			: ��
* ���			: ��
* ����			: ��
*******************************************************************************/
u8 LRC8(u8 *Pointer)		//��������У��
{
	return 0;
}
/////////////////////////////////LIST����//////////////////////////////////////////
/*******************************************************************************
* ������			:	CreateNode
* ��������		: �����ڵ�
* ����			: ��
* ���			: ��
* ����			: ��
*******************************************************************************/
//LINK_LIST *CreateNode(u8 *Pointer)		
//{
//	return 0;
//}




//==========================
//���ܣ��������
//���أ�ָ��������ͷ��ָ��
//==========================

struct student *Create ()
{
	struct student *head; 			/*ͷ��� */
	struct student *p1 = NULL; 	/*p1���洴�����½��ĵ�ַ */
	struct student *p2 = NULL; 	/*p2����ԭ�������һ�����ĵ�ַ */
	n = 0; 											/*����ǰ�����Ľ������Ϊ0�������� */
	p1 = (struct student *) malloc (LEN); 	/*����һ���½�� */
	p2 = p1;						/*�����㿪�ٳɹ�����p2�Ȱ�����ָ�뱣�������Ա����� */
	if (p1 == NULL) 		/*��㿪�ٲ��ɹ� */
	{
		printf ("\nCann't create it, try it again in a moment!\n");
		return NULL;
	}
	else 						/*��㿪�ٳɹ� */
	{
		head = NULL; 	/*��ʼheadָ��NULL */
		printf ("Please input %d node -- num,score: ", n + 1);
		scanf ("%ld,%f", &(p1->num), &(p1->score)); /*¼������ */
	}
	while (p1->num != 0) /*ֻҪѧ�Ų�Ϊ0���ͼ���¼����һ����� */
	{
		n += 1; 			/*�����������1�� */
		if (n == 1) 	/*������������1����headָ��մ����Ľ��p1 */
		{
			head = p1;
			/*
			ע�⣺
			��ʱ��p2����p1,Ҳ����p1->nextָ��NULL��
			����дĿ����������else����һ�¡�
			*/
			p2->next = NULL;		//head==p1;
		}
		else
		{
			p2->next = p1; 	/*ָ���ϴ�����տ��ٵĽ�� */		//head->next==p1;
		}
		p2 = p1; 					/*��p1�ĵ�ַ��p2������Ȼ��p1ȥ�����½�� */
		p1 = (struct student *) malloc (LEN);
		printf ("Please input %d node -- num,score: ", n + 1);
		scanf ("%ld,%f", &(p1->num), &(p1->score));
	}
	p2->next = NULL; 	/*�˾���Ǹ��ݵ������������һ�����Ҫָ��NULL */		//head->next==p1,p1->next==null
	free (p1); 				/*�ͷ�p1����malloc()��calloc()�ı�����Ҫfree() */
	p1 = NULL; 				/*�ر�Ҫ���ǰ��ͷŵı��������ΪNULL,����ͱ��"Ұָ��"������ַ��ȷ����ָ�롣 */
	return head; 			/*���ش���������ͷָ��*/
}
//===========================
//���ܣ�������
//���أ� void
//===========================
void Print (struct student *head)
{
	struct student *p;
	printf ("\nNow , These %d records are:\n", n);
	p = head;
	if (head != NULL) /*ֻҪ���ǿ���������������������н�� */
	{
		printf ("head is %o\n", head); /*���ͷָ��ָ��ĵ�ַ */
		do
		{
		/*
		�����Ӧ��ֵ����ǰ����ַ�����ֶ�ֵ����ǰ������һ����ַ��
		����������ڶ������󿴵�һ�����������ڼ�����еĴ洢�ṹ��������
		��Ƶ�ͼʾ��һģһ���ġ�
		*/
		printf ("%o %ld %5.1f %o\n", p, p->num, p->score, p->next);
		p = p->next; /*�Ƶ���һ����� */
		}while (p != NULL);
	}
}
//==========================
//���ܣ�ɾ��ָ�����
//(��������ɾ��ָ��ѧ�ŵĽ��)
//���أ�ָ��������ͷ��ָ��
//==========================
struct student *Del (struct student *head, long num)
{
	struct student *p1; /*p1���浱ǰ��Ҫ���Ľ��ĵ�ַ */
	struct student *p2; /*p2���浱ǰ�����Ľ��ĵ�ַ */
	if (head == NULL) /*�ǿ����������ͼ3���⣩ */
	{
		printf ("\nList is null!\n");
		return head;
	}
	/*��λҪɾ���Ľ�� */
	p1 = head;
	while (p1->num != num && p1->next != NULL) /*p1ָ��Ľ�㲻����Ҫ���ҵģ��������������һ����㣬�ͼ��������� */
	{
		p2 = p1; /*���浱ǰ���ĵ�ַ */
		p1 = p1->next; /*����һ����� */
	}
	if (num == p1->num) /*�ҵ��ˡ������ͼ4��5���⣩ */
	{
		if (p1 == head) /*���Ҫɾ���Ľ���ǵ�һ����� */
		{
			head = p1->next; /*ͷָ��ָ���һ�����ĺ�һ����㣬Ҳ���ǵڶ�����㡣������һ�����Ͳ��������У���ɾ���� */
		}
		else /*�����������㣬����ԭ��ָ��ǰ����ָ�룬ָ��������һ����㣬���ɾ�� */
		{
			p2->next = p1->next;
		}
		free (p1); /*�ͷŵ�ǰ��� */
		p1 = NULL;
		printf ("\ndelete %ld success!\n", num);
		n -= 1; /*���������1�� */
	}
	else /*û���ҵ� */
	{
		printf ("\n%ld not been found!\n", num);
	}
	return head;
}
//==========================
//���ܣ�����ָ�����ĺ���
//(��������ָ��ѧ�ŵĽ��)
//���أ�ָ��������ͷ��ָ��
//==========================
struct student *Insert (struct student *head, long num, struct student *node)
{
	struct student *p1; /*p1���浱ǰ��Ҫ���Ľ��ĵ�ַ */
	if (head == NULL) /*�����ͼʾ7���⣩ */
	{
		head = node;
		node->next = NULL;
		n += 1;
		return head;
	}
	p1 = head;
	while (p1->num != num && p1->next != NULL) /*p1ָ��Ľ�㲻����Ҫ���ҵģ��������������һ����㣬���������� */
	{
		p1->next; /*����һ����� */
	}
	if (num == p1->num) /*�ҵ��ˣ����ͼʾ8���⣩ */
	{
		node->next = p1->next; /*��Ȼnode����һ�����ԭp1��next */
		p1->next = node; /*�����ԭp1����һ������Ҫ�����node */
		n += 1; /*�����������1�� */
	}
	else
	{
		printf ("\n%ld not been found!\n", num);
	}
	return head;
}
//==========================
//���ܣ�������
//(������ͷ���������β��������β���ͷ)
//���أ�ָ��������ͷ��ָ��
//==========================
struct student *Reverse (struct student *head)
{
	struct student *p; /*��ʱ�洢 */
	struct student *p1; /*�洢���ؽ�� */
	struct student *p2; /*Դ������һ��һ��ȡ */
	p1 = NULL; /*��ʼ�ߵ�ʱ���ѵߵ��Ĳ���Ϊ�� */
	p2 = head; /*p2ָ��������ͷ��� */
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
//���ܣ�ѡ������(��С����)
//���أ�ָ��������ͷ��ָ��
//==========================
struct student *SelectSort (struct student *head)
{
	struct student *first; /*���к��������ı�ͷָ�� */
	struct student *tail; /*���к��������ı�βָ�� */
	struct student *p_min; /*������ֵ��С�Ľ���ǰ������ָ�� */
	struct student *min; /*�洢��С��� */
	struct student *p; /*��ǰ�ȽϵĽ�� */
	first = NULL;
	while (head != NULL) /*���������Ҽ�ֵ��С�Ľ�㡣 */
	{
		/*ע�⣺����for����������ѡ������˼��ĵط� */
		for (p = head, min = head; p->next != NULL; p = p->next) /*ѭ�����������еĽ�㣬�ҳ���ʱ��С�Ľ�㡣 */
		{
			if (p->next->num < min->num) /*�ҵ�һ���ȵ�ǰminС�Ľ�㡣 */
			{
				p_min = p; /*�����ҵ�����ǰ����㣺��Ȼp->next��ǰ�������p�� */
				min = p->next; /*�����ֵ��С�Ľ�㡣 */
			}
		}
		/*����for�������󣬾�Ҫ�������£�һ�ǰ����������������У����Ǹ�����Ӧ�������жϣ��������뿪ԭ���������� */
		/*��һ���� */
		if (first == NULL) /*�����������Ŀǰ����һ�������� */
		{
			first = min; /*��һ���ҵ���ֵ��С�Ľ�㡣 */
			tail = min; /*ע�⣺βָ������ָ������һ����㡣 */
		}
		else /*�����������Ѿ��н�� */
		{
			tail->next = min; /*�Ѹ��ҵ�����С���ŵ���󣬼���βָ���nextָ������ */
			tail = min; /*βָ��ҲҪָ������ */
		}
		/*�ڶ����� */
		if (min == head) /*����ҵ�����С�����ǵ�һ����� */
		{
			head = head->next; /*��Ȼ��headָ��ԭhead->next,���ڶ�����㣬��OK */
		}
		else /*������ǵ�һ����� */
		{
			p_min->next = min->next; /*ǰ����С����nextָ��ǰmin��next,��������min�뿪��ԭ������ */
		}
	}
	if (first != NULL) /*ѭ�������õ���������first */
	{
		tail->next = NULL; /*�������������һ������nextӦ��ָ��NULL */
	}
	head = first;
	return head;
}
//==========================
//���ܣ�ֱ�Ӳ�������(��С����)
//���أ�ָ��������ͷ��ָ��
//==========================














