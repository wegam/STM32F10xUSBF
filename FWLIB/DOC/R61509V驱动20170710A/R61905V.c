#include "type_def.h"
#include "STC12.h"
#include "R61905V.h"
#include "delay.h"


#define PORTH P2	 
#define PORTL P0	  


sbit LCD_RD  = P3^7;   		    
sbit LCD_WR  = P3^6;   		 
sbit LCD_RS  = P5^2;  	   
sbit LCD_CS  = P5^1;   	     	
sbit LCD_RST = P5^3; 


/*******************************************************************************
* �� �� ��:  lcd_write_com
* ��    ��:  ��Һ��R61509Vд������
* ��ڲ���:  reg_add,R61509V�Ĵ�����ַ
* ���ڲ���:  ��
* �� �� ֵ:  ��
*******************************************************************************/

void lcd_write_com(u16 reg_add)	 
{	
    LCD_RS=0;
	LCD_CS=0;	 
	PORTH=reg_add>>8;	
	PORTL=reg_add;		
	LCD_WR=0;
	LCD_WR=1;
	LCD_CS=1;	
}

/*******************************************************************************
* �� �� ��:  lcd_write_dat
* ��    ��:  ��Һ��R61509Vд������
* ��ڲ���:  reg_dat,��д�������
* ���ڲ���:  ��
* �� �� ֵ:  ��
*******************************************************************************/

void lcd_write_dat(u16 reg_dat)    
{
    LCD_RS=1;
	LCD_CS=0;	  				
	PORTH=reg_dat>>8;	
	PORTL=reg_dat;					
	LCD_WR=0;
	LCD_WR=1;
	LCD_CS=1;	
}

/*******************************************************************************
* �� �� ��:  lcd_write_reg
* ��    ��:  ָ��R61509V�Ĵ���д������
* ��ڲ���:  reg_add,R61509V�Ĵ�����ַ
			 reg_dat,��д�������
* ���ڲ���:  ��
* �� �� ֵ:  ��
*******************************************************************************/

void lcd_write_reg(u16 reg_add,u16 reg_dat)
{
    LCD_RS=0;
	LCD_CS=0;	 
	PORTH=reg_add>>8;	
	PORTL=reg_add;		
	LCD_WR=0;
	LCD_WR=1;
	LCD_CS=1;
	
    LCD_RS=1;
	LCD_CS=0;	  				
	PORTH=reg_dat>>8;	
	PORTL=reg_dat;					
	LCD_WR=0;
	LCD_WR=1;
	LCD_CS=1;	
}

/*******************************************************************************
* �� �� ��:  lcd_set_window
* ��    ��:  ����R61509V�����GDDRAM��ʼ��ַ
* ��ڲ���:  x1,�������������
			 y1,��������������
			 x2,������յ������
			 y2,������յ�������
			    ����(0<=x1<=x2<=319,0<=y2<=y1<=239)
* ���ڲ���:  ��
* �� �� ֵ:  ��
*******************************************************************************/

void lcd_set_window(u16 x1,u16 y1,u16 x2,u16 y2) 
{ 
//����
/*	lcd_write_reg(0x0003,0x0028);
	lcd_write_reg(0x0050,y2);
	lcd_write_reg(0x0051,y1);
	lcd_write_reg(0x0052,x1);
	lcd_write_reg(0x0053,x2);  
  	lcd_write_reg(0x0020,y1);	  
    lcd_write_reg(0x0021,x1); 
    lcd_write_com(0x0022);
*/

//����
	lcd_write_reg(0x0050,x1);
	lcd_write_reg(0x0051,x2);
	lcd_write_reg(0x0052,y1);
	lcd_write_reg(0x0053,y2);  
  	lcd_write_reg(0x0020,x1);	  
    lcd_write_reg(0x0021,y1); 
    lcd_write_com(0x0022);

}

/*******************************************************************************
* �� �� ��:  lcd_init
* ��    ��:  ��ʼ��TFTҺ��R61509V�Ĵ�������
* ��ڲ���:  ��
* ���ڲ���:  ��
* �� �� ֵ:  ��
*******************************************************************************/
	
void lcd_init()
{
	LCD_RST=1;
	LCD_RST=0;
	delay_ms(5);
	LCD_RST=1;
	LCD_CS=1;
	LCD_RD=1;
	LCD_WR=1;
	delay_ms(5);
	lcd_write_reg(0x00,0x0000);		
	lcd_write_reg(0x0000,0x0000);		
	lcd_write_reg(0x0000,0x0000);	
	lcd_write_reg(0x0000,0x0001);	
	lcd_write_reg(0x00A4,0x0001);
	delay_ms(10);		
	lcd_write_reg(0x0060,0x2700);			
	lcd_write_reg(0x0008,0x0808);		
	lcd_write_reg(0x0030,0x0214);
	lcd_write_reg(0x0031,0x3715);
	lcd_write_reg(0x0032,0x0604);
	lcd_write_reg(0x0033,0x0E16);
	lcd_write_reg(0x0034,0x2211);
	lcd_write_reg(0x0035,0x1500);
	lcd_write_reg(0x0036,0x8507);
	lcd_write_reg(0x0037,0x1407);
	lcd_write_reg(0x0038,0x1403);
	lcd_write_reg(0x0039,0x0020);
	lcd_write_reg(0x0090,0x0015);
	lcd_write_reg(0x0010,0x0410);
	lcd_write_reg(0x0011,0x0237);
	lcd_write_reg(0x0029,0x0046);
	lcd_write_reg(0x002A,0x0046);
	lcd_write_reg(0x0007,0x0000);
	lcd_write_reg(0x0012,0x0189);
	lcd_write_reg(0x0013,0x1100);
	delay_ms(150);
	lcd_write_reg(0x0012,0x01B9);
	lcd_write_reg(0x0001,0x0100);
	lcd_write_reg(0x0002,0x0200);
	lcd_write_reg(0x0003,0x1030);
	lcd_write_reg(0x0009,0x0001);
	lcd_write_reg(0x000A,0x0000);
	lcd_write_reg(0x000D,0x0000);
	lcd_write_reg(0x000E,0x0030);
	lcd_write_reg(0x0050,0x0000);
	lcd_write_reg(0x0051,0x00EF);
	lcd_write_reg(0x0052,0x0000);
	lcd_write_reg(0x0053,0x013F);
	lcd_write_reg(0x0061,0x0001);
	lcd_write_reg(0x006A,0x0000);
	lcd_write_reg(0x0080,0x0000);
	lcd_write_reg(0x0081,0x0000);
	lcd_write_reg(0x0082,0x005F);
	lcd_write_reg(0x0092,0x0100);
	lcd_write_reg(0x0093,0x0701);
	delay_ms(80);
	lcd_write_reg(0x0007,0x0100);
	lcd_write_reg(0x0020,0x0000);
	lcd_write_reg(0x0021,0x0000);
	lcd_write_com(0x0022);
}




