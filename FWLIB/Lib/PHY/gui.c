#include "type_def.h"
#include "SD.h"
#include "FAT32.h"
#include "R61509V.h"
#include "gui.h"
#include "ASCII(8x16).h"
#include "ASCII(12x24).h"
//#include "ASCII(16x32).h"
//#include "ASCII(20x40).h"



extern PART_INFO_type xdata FAT32_info;
extern ITEM_INFO_type xdata HZK16;
extern ITEM_INFO_type xdata HZK24;
//extern ITEM_INFO_type xdata HZK32;
//extern ITEM_INFO_type xdata HZK40;






//ȫ���ַ����ֺŶ�Ӧ���ַ���Ⱥ���ģ���ֽ���
u8 code full_char[2][5]=
{
	{8,16,24, 32, 40},	   //��ģ���
	{8,32,72,128,200}	   //��ģ���ֽ���
};



//����ַ����ֺŶ�Ӧ���ַ���ȡ��߶Ⱥ��ֽ�����
u8 code half_char[2][5]=
{
	{4, 8,12,16, 20},		//��ģ���
	{8,16,24,36, 40}		//��ģ�߶�
	//{8,16,48,64,128}		//��ģ���ֽ���
}; 



/*******************************************************************************
* �� �� ��:  lcd_paint
* ��    ��:  Һ��ȫ��ˢ��
* ��ڲ���:  color,ˢ����ɫ
* ���ڲ���:  ��
* �� �� ֵ:  ��
*******************************************************************************/

void lcd_paint(u16 color)
{
	u8 x,y;

	lcd_set_window(0,0,239,319);		
    for(y=160;y>0;y--)			 //Һ����������320*240
	{
		for (x=240;x>0;x--)
	   	{
         lcd_write_dat(color);	 
		 lcd_write_dat(color);
	    }
	}		
}

/*******************************************************************************/

void lcd_show_half_char(u16 x,u16 y,u8 size,u16 f_color,u16 b_color,u8 pchar)
{
	u8 dat,height,n8,i,*ppchar;

	//size-=1;
	pchar-=32;

	height=half_char[1][size-1];	  //ȡ����Ӧ�ֺŵ��ַ��߶�

	lcd_set_window( x , y , x+half_char[0][size-1]-1 , y+half_char[1][size-1]-1 );//�ó���㡢�յ����겢���û���ں������û����

	switch (size-1)
	{
		case 0:break;							//��Ӧ1���֣���δ����1���ַ���ģ���ʲ�����
		case 1:ppchar=ASCII_8x16[pchar];break;  //��Ӧ2���֣�ȡ����Ӧ�ֺ���ģͷָ��
		case 2:ppchar=ASCII_12x24[pchar];break; //��Ӧ3���֣�ȡ����Ӧ�ֺ���ģͷָ��
		//case 3:ppchar=ASCII_16x32[pchar];break; //��Ӧ4���֣�ȡ����Ӧ�ֺ���ģͷָ��
		//case 4:ppchar=ASCII_20x40[pchar];break; //��Ӧ5���֣�ȡ����Ӧ�ֺ���ģͷָ��
		default:return;
	}

	for(;height>0;height--)				  	
	{
		for(n8=(half_char[0][size-1]>>3);n8>0;n8--)	//������Ӧ�ֺŵ���ģ��ȵó�д8���������ݵĴ���
		{
			dat=*(ppchar++);		 //ȡ��ASCII����ģ����
			for(i=8;i>0;i--)		 //ѭ��8��д��8������
			{
				if(dat&0x80)
				{
					lcd_write_dat(f_color);	
				}
				else 
				{
					lcd_write_dat(b_color);
				}
				dat<<=1;				//��λ1׼��д��һ�µ�����
			}
		}
	
		if( half_char[0][size-1]&0x07 )	  //�����ֺŵó��Ƿ��������Чλ���ֽ�
		{
			dat=*(ppchar++);			  //ȡ����ģ����
			for(i=4;i>0;i--)			  //��Ч������4�㣬д��4�㣬����4��
			{
				if(dat&0x80)
				{
					lcd_write_dat(f_color);	
				}
				else 
				{
					lcd_write_dat(b_color);
				}
				dat<<=1;
			}
		}
	}
}

/*******************************************************************************/
/*
void lcd_show_half_str(u16 x,u16 y,u8 size,u8 *str,u16 color)
{
	//size-=1;

	while( (*str)!='\0' )	 //�ж��Ƿ񵽴��ַ���ĩβ
	{
		lcd_show_half_char(x,y,size,*str,color);   
		x+=half_char[0][size-1];				  
		str++;									   
	}
}*/

/*******************************************************************************/
/*
void lcd_show_full_char(u16 x,u16 y,u8 size,u8 *pchar,u16 color)
{
	u8 total;
	u8 i,dat; 

	//size-=1;

	lcd_set_window( x ,y,x+full_char[0][size-1]-1 , y+full_char[0][size-1]-1 );	//�����������ó��յ����겢���ûҺ������
								 										
	LCD_RS=1;	   
	LCD_CS=0;	  

	for(total=0;total<full_char[1][size-1];total++)	 //�õ��ֺ���Ӧ����ģ���ֽ���
	{
		dat=*(pchar++);		 //ȡ����ģ����
		for(i=0;i<8;i++)	 //ѭ��8��д��8������
		{
			if(dat&0x80)
			{
				PORTH=color>>8;	//��������ɫ,��8λ
				PORTL=color;   	//��������ɫ,��8λ
			}
			else 
			{
				PORTH=0x00;		 //�ֵı���Ϊ��ɫ
				PORTL=0x00;
			}
			LCD_WR=0;
			LCD_WR=1;				//����дҺ������
			dat<<=1;
		}
	}						   //�ͷ�R61509V
	LCD_CS=1;			
}*/

/*******************************************************************************/
/*
void lcd_show_full_str(u16 x,u16 y,u8 size,u8 length,u8 *pstr,u16 color)
{
	u8 n,k;

	//size-=1;

	k=full_char[1][size-1];				 //ȡ���ֶ�Ӧ�ֺŵ���ģ���ֽ���
	
	for(n=length;n>0;n--)						   //�ж����ַ���ѭ�����ٴ�
	{
		lcd_show_full_char(x,y,size,pstr,color);   //���õ���ȫ���ַ���ʾ�����ֱ���ʾ�ַ�
		x+=full_char[0][size-1];				 	//�ó���һ���ַ���������
		pstr+=k;								   //ָ��������һ���ַ�ͷָ��
	}	
}*/

/***************************************************************************/

void lcd_show_full_char(u16 x,u16 y,u8 size,u16 f_color,u16 b_color,u8 *str)
{
	u8 xdata buf[72];
	//u8 buf[32];
	u8 total;
	u8 i,dat; 
	ITEM_INFO_type *HZK;

	switch (size-1)
	{
		case 0:break;
		case 1:HZK=&HZK16;break;
		case 2:HZK=&HZK24;break;
		//case 3:HZK=&HZK32;break;
		//case 4:HZK=&HZK40;break;
		default:return;
	}
	//SD_read_block((cluster2sector(HZK16.start_cluster,&FAT32_info)<<9)+((((u32)str[0]-0xa1)*94+((u32)str[1]-0xa1))<<5),32,buf); 
	SD_read_block((cluster2sector(HZK->start_cluster,&FAT32_info)<<9)+((((u32)str[0]-0xa1)*94+((u32)str[1]-0xa1))*full_char[1][size-1]),full_char[1][size-1],buf);

	lcd_set_window( x ,y,x+full_char[0][size-1]-1 , y+full_char[0][size-1]-1 );	

	for(total=0;total<full_char[1][size-1];total++)	 
	{
		dat=buf[total];		 
		for(i=0;i<8;i++)	
		{
			if(dat&0x80)
			{
				lcd_write_dat(f_color);	
			}
			else 
			{
				lcd_write_dat(b_color);
			}				
			dat<<=1;
		}
	}						   		
}

/*******************************************************************************/
/*
void lcd_show_full_str(u16 x,u16 y,u8 size,u8 *str,u16 color)
{				
	while(*str)						
	{
		lcd_show_full_char(x,y,size,str,color);   
		x+=full_char[0][size-1];				 	
		str+=2;								 
	}	
}*/

/********************************************************************************/

void lcd_show_str(u16 x,u16 y,u8 size,u16 f_color,u16 b_color,u8 *str)
{
	while(*str)
	{
		if(*str<0x80)	  //��ASCII��
		{
			lcd_show_half_char(x,y,size,f_color,b_color,*str);
			x+=half_char[0][size-1];				  
			str++;		
		}
		else			  //�Ǻ�������
		{
			lcd_show_full_char(x,y,size,f_color,b_color,str);   
			x+=full_char[0][size-1];				 	
			str+=2;				
		}
	}
}

/*******************************************************************************/

void lcd_draw_box(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 i,j;
	
	lcd_set_window(x1,y1,x2,y2);

	for(i=x1;i<=x2;i++)
		for(j=y1;j<=y2;j++)
			lcd_write_dat(color);
		
}

/***************************************************************************/
/*
void lcd_draw_line(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	
}*/

/**********************************************************************/
/*
void lcd_draw_hline(u16 x1,u16 y1,u16 length,u16 color)
{
	lcd_set_window(x1,y1,x1+length-1,y1);
	while(length)
	{
		lcd_write_dat(color);
		length--;
	}	
}*/

/**********************************************************************/
/*
void lcd_draw_vline(u16 x1,u16 y1,u16 length,u16 color)
{
	lcd_set_window(x1,y1,x1,y1+length-1);
	while(length)
	{
		lcd_write_dat(color);
		length--;
	}	
}*/

/******************************************************************/
/*
void lcd_draw_rect(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	lcd_draw_hline(x1,y1,x2-x1+1,color);
	lcd_draw_hline(x1,y2,x2-x1+1,color);
	lcd_draw_vline(x1,y1,y2-y1+1,color);
	lcd_draw_vline(x2,y1,y2-y1+1,color);
}*/

/*******************************************************************************/
/*
void lcd_mono_map(u16 x,u16 y,u16 width,u8 height,u8 *map,u16 color)
{
	u8 dat,m,n,i;
	
	lcd_set_window(x,y,x+width-1,y-height+1);  //��������յ����û����

	LCD_RS=1;			 
	LCD_CS=0;			

	for(m=16;m>0;m--)
	{
		for(n=128;n>0;n--)
		{
			dat=*(map++);		 //ȡ��ͼƬ����
			for(i=8;i>0;i--)	 //һ��д��8������
			{
				if(dat&0x80)
				{
					PORTH=color>>8;	//ͼƬ��ɫ��8λ
					PORTL=color;	//ͼƬ��ɫ��8λ
				}
				else 
				{
					PORTH=0x00;		 //ͼƬ����Ϊ��ɫ
					PORTL=0x00;
				}				
				LCD_WR=0;
				LCD_WR=1;				//����дҺ������
				dat<<=1;				//��λ�Ա�дһ�¸�����
			}
		}
	}
	LCD_CS=1;						  //�ͷ�R61509V
}*/

/************************************************************/

void lcd_show_map(u16 x,u16 y,u16 width,u16 height,ITEM_INFO_type *file_info)
{
	u8 xdata buf[512];
	u32 cluster,sector,size=0;
	u8 i,j;

	cluster=file_info->start_cluster;	

	lcd_set_window(x,y,x+width-1,y+height-1);
	while(1)
	{
		sector=cluster2sector(cluster,&FAT32_info);
		for(j=0;j<FAT32_info.cluster_size;j++)
		{
			SD_read_sector(sector++,buf);
			//sector++;
			for(i=0;i<128;i++)
			{
				lcd_write_dat(buf[i<<2]+(buf[(i<<2)+1]<<8));
				lcd_write_dat(buf[(i<<2)+2]+(buf[(i<<2)+3]<<8));
				size+=4;
				if(size>=file_info->size)
					return;	
			}
		}
		cluster=next_cluster(cluster,&FAT32_info); 
	}	
}

/*******************************************************************/
/*
void lcd_show_map(u16 x,u16 y,u16 width,u16 height,u8 *dat)
{
	u32 dots;

	dots=width*height;
	lcd_set_window(x,y,x+width-1,y+height-1);
	for(;dots>0;dots--)
	{
		lcd_write_dat(dat[0]+(dat[1]<<8));
		dat+=2;
	}	
}*/




