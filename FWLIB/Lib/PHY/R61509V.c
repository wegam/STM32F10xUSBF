/**************************************************************************************************
* Copyright (C), 1997-$(Year),  HAICHUANG IOT Tech. Co., Ltd.
* �ļ�<$(filename)>
* ģ������ :	���塢���ܡ�˵��
* ��ʷ�޸ļ�¼
*     <��Ҫ�޸���>  <ʱ��>   <�汾 >   <˵��>
*     xlyan        $(Date)   1.0.0    �����ļ�
**************************************************************************************************/
#include "R61509V.h"

#include "STM32_GPIO.H"
#include "STM32_PWM.H"
#include "font.h"

//#ifdef LCD_61509_EN

t_LcdCfg g_LcdCfg;

u16 Sreen_Rotate=SCREEN_ROTATE_270D;			//������

R61509V_Pindef *R61509V_sPinfo=0;

u16 R61509V_BACK_COLOR	=	R61509V_BLACK;		//����ɫ
u16 R61509V_POINT_COLOR	=	R61509V_BLUE;   	//����ɫ

/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void R61509V_Delay(u32 xms)
{
	while(xms--);
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void R61509V_PinConf(R61509V_Pindef *Pinfo)
{
	GPIO_Configuration_OPP50	(Pinfo->R61509V_sBL_PORT,				Pinfo->R61509V_sBL_Pin);		//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->R61509V_sRD_PORT,				Pinfo->R61509V_sRD_Pin);					//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->R61509V_sREST_PORT,			Pinfo->R61509V_sREST_Pin);				//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->R61509V_sRS_PORT,				Pinfo->R61509V_sRS_Pin);					//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->R61509V_sWR_PORT,				Pinfo->R61509V_sWR_Pin);					//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->R61509V_sCS_PORT,				Pinfo->R61509V_sCS_Pin);					//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->R61509V_sTE_PORT,				Pinfo->R61509V_sTE_Pin);					//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	GPIO_Configuration_OPP50	(Pinfo->R61509V_sDATABUS_PORT,	Pinfo->R61509V_sDATABUS_Pin);			//��GPIO��Ӧ�ܽ�����ΪPP(����)���ģʽ������ٶ�2MHz----V20170605
	
	R61509V_sPinfo=Pinfo;
	
	R61509V_BL_ON;
	
	R61509V_Initialize();		////��������оƬR61509V��power supply on sequence ��������
	
//	R61509V_Delay(1000);
//	R61509V_Delay(1000);
//	R61509V_Delay(1000);
//	R61509V_Delay(1000);
//	R61509V_Delay(1000);
//	R61509V_Delay(1000);
//	R61509V_Delay(1000);
//	R61509V_Delay(1000);
	
//	R61509V_PowerOn();
}
/**************************************************************************************************
* [Function] LCD_GetCfg:  �������ܡ�ע������ȵ�����
* [No param][return]*: description
**************************************************************************************************/
t_LcdCfg * LCD_GetCfg( void )
{
	return(&g_LcdCfg);
}


/**************************************************************************************************
* [Function] LCD_WriteCom:  �������ܡ�ע������ȵ�����
* [param01]u16 Com: description
**************************************************************************************************/
void R61509V_Write_Bus( u16 Com )
{
	R61509V_CS_LOW;
	R61509V_RD_HIGH;
	R61509V_RS_LOW;
	R61509V_DATABUS_PORT->ODR = Com;
	R61509V_WR_LOW;
	R61509V_WR_HIGH;
	R61509V_CS_HIGH;
}

#if 1

/**************************************************************************************************
* [Function] LCD_WriteData:  �������ܡ�ע������ȵ�����
* [param01]u16 Dat: description
**************************************************************************************************/
void R61509V_WriteData( u16 Dat )
{
	R61509V_CS_LOW;
	R61509V_RD_HIGH;
	R61509V_RS_HIGH;
	R61509V_DATABUS_PORT->ODR = Dat;
	R61509V_WR_LOW;
	R61509V_WR_HIGH;
	R61509V_CS_HIGH;
}

#endif


/**************************************************************************************************
* [Function] LCD_WriteCMD: ��lcdд��ָ������
* [param01]u16 addr: ָ��
* [param02]u16 Data: ָ������
**************************************************************************************************/
void R61509V_WriteCMD( u16 addr, u16 Data )
{
/*
 * LCD_WriteCom( addr );
 * LCD_WriteData( Data );
 */

	R61509V_CS_LOW;
	R61509V_RD_HIGH;
	R61509V_RS_LOW;
	R61509V_DATABUS_PORT->ODR = addr;
	R61509V_WR_LOW;
	R61509V_WR_HIGH;
	R61509V_CS_HIGH;
	/*Delay_us( 200 ); */

	R61509V_CS_LOW;
	R61509V_RD_HIGH;
	R61509V_RS_HIGH;
	R61509V_DATABUS_PORT->ODR = Data;
	R61509V_WR_LOW;
	R61509V_WR_HIGH;
	R61509V_CS_HIGH;
}


/**************************************************************************************************
* [Function] LCD_ReadData:  ��LCD��ȡ����
* [No param][return]u16: ��������
**************************************************************************************************/
u16 R61509V_ReadData( void )
{
	u16 Dat;

	R61509V_CS_LOW;
	R61509V_WR_HIGH;
	R61509V_RS_HIGH;
	Dat = R61509V_DATABUS_PORT->IDR;
	R61509V_RD_LOW;
	R61509V_RD_HIGH;
	R61509V_CS_HIGH;

	return(Dat);
}


/**************************************************************************************************
* [Function] LCD_SetDrawWindow:  LCD ����ʾɨ�跽ʽ
* [param01]t_Sreen_Rotate ScrnRotat: ��������д����� ��ת�Ƕ�ֵ
* [param02]t_PixCfg PixCfg: ��ʾ��������
* [param03]t_Point top_p: ��������
* [param04]u16 bottom_p: �ײ�����
**************************************************************************************************/
void R61509V_SetDrawWindow( t_Point top_p, t_Point bottom_p )
{
	u16 HSA, HEA, VSA, VEA;

	switch ( LCD_GetCfg()->ScrnRotat )
	{
	case SCREEN_ROTATE_0D:
	{
		HSA	= top_p.x;
		HEA	= bottom_p.x;
		VSA	= top_p.y;
		VEA	= bottom_p.y;
		break;
	}
	case SCREEN_ROTATE_90D:
	{
		HSA	= LCD_GetCfg()->PixCfg.YPixNum - bottom_p.y - 1;
		HEA	= LCD_GetCfg()->PixCfg.YPixNum - top_p.y - 1;
		VSA	= top_p.x;
		VEA	= bottom_p.x;
		break;
	}
	case SCREEN_ROTATE_180D:
	{
		HSA	= LCD_GetCfg()->PixCfg.XPixNum - bottom_p.x - 1;
		HEA	= LCD_GetCfg()->PixCfg.XPixNum - top_p.x - 1;
		VSA	= LCD_GetCfg()->PixCfg.YPixNum - bottom_p.y - 1;
		VEA	= LCD_GetCfg()->PixCfg.YPixNum - top_p.y - 1;
		break;
	}
	case SCREEN_ROTATE_270D:
	{
		HSA	= top_p.y;
		HEA	= bottom_p.y;
		VSA	= LCD_GetCfg()->PixCfg.XPixNum - bottom_p.x - 1;
		VEA	= LCD_GetCfg()->PixCfg.XPixNum - top_p.x - 1;
		break;
	}
	default: break;
	}
	R61509V_WriteCMD( R61509V_R210_HSA, HSA );
	R61509V_WriteCMD( R61509V_R211_HEA, HEA );
	R61509V_WriteCMD( R61509V_R212_VSA, VSA );
	R61509V_WriteCMD( R61509V_R213_VEA, VEA );
}


/**************************************************************************************************
* [Function] LCD_DrawDot:  ����
* [param01]t_Point StartPos: description
* [param02]u16 color: description
**************************************************************************************************/
void R61509V_DrawDotest(u16 x,u16 y, u16 color )
{
	/*
	 * t_Point endpos;
	 * u16 FWidth=1,FHeight=1;
	 * endpos.x	= StartPos.x + FWidth - 1;
	 * endpos.y	= StartPos.y + FHeight - 1;
	 */
//	if ( LCD_TRANSPARENT_COLOR != color )
//	{

		u16 HSA, HEA, VSA, VEA;
			HSA	= x;
			HEA	= x;
			VSA	= y;
			VEA	= y;
//#define Y_H	399     /*LCD_PHY_HEIGHT-1  CD_GetCfg()->PixCfg.YPixNum-1 */
//#define X_W	239     /*LCD_PHY_WIDTH-1  CD_GetCfg()->PixCfg.xPixNum-1 */

//		switch ( LCD_GetCfg()->ScrnRotat )
//		{
//		case SCREEN_ROTATE_0D:
//		{
//			HSA	= StartPos.x;
//			HEA	= StartPos.x;
//			VSA	= StartPos.y;
//			VEA	= StartPos.y;
//			break;
//		}
//		case SCREEN_ROTATE_90D:
//		{
//			HSA	= LCD_GetCfg()->PixCfg.YPixNum - StartPos.y-1;
//			HEA	= LCD_GetCfg()->PixCfg.YPixNum - StartPos.y-1;
//			VSA	= StartPos.x;
//			VEA	= StartPos.x;
//			break;
//		}
//		case SCREEN_ROTATE_180D:
//		{
//			HSA	= LCD_GetCfg()->PixCfg.YPixNum - StartPos.x-1;
//			HEA	= LCD_GetCfg()->PixCfg.YPixNum - StartPos.x-1;
//			VSA	= LCD_GetCfg()->PixCfg.YPixNum - StartPos.y-1;
//			VEA	= LCD_GetCfg()->PixCfg.YPixNum - StartPos.y-1;
//			break;
//		}
//		case SCREEN_ROTATE_270D:
//		{
//			HSA	= StartPos.y;
//			HEA	= StartPos.y;
//			VSA	= LCD_GetCfg()->PixCfg.XPixNum - StartPos.x-1;
//			VEA	= LCD_GetCfg()->PixCfg.XPixNum - StartPos.x-1;
//			break;
//		}
//		default: break;
//		}
		R61509V_WriteCMD( R61509V_R210_HSA, HSA );
		R61509V_WriteCMD( R61509V_R211_HEA, HEA );
		R61509V_WriteCMD( R61509V_R212_VSA, VSA );
		R61509V_WriteCMD( R61509V_R213_VEA, VEA );


		R61509V_WriteCMD( R61509V_R200_HA, HSA );
		R61509V_WriteCMD( R61509V_R201_VA, VSA );
		R61509V_Write_Bus( R61509V_R202_GDRW );


		R61509V_WriteData(color);

}
/**************************************************************************************************
* [Function] LCD_DrawDot:  ����
* [param01]t_Point StartPos: description
* [param02]u16 color: description
**************************************************************************************************/
void R61509V_DrawDotB( t_Point StartPos, u16 color )
{
	/*
	 * t_Point endpos;
	 * u16 FWidth=1,FHeight=1;
	 * endpos.x	= StartPos.x + FWidth - 1;
	 * endpos.y	= StartPos.y + FHeight - 1;
	 */
	if ( LCD_TRANSPARENT_COLOR != color )
	{
	#if 1
		u16 HSA, HEA, VSA, VEA;
#define Y_H	399     /*LCD_PHY_HEIGHT-1  CD_GetCfg()->PixCfg.YPixNum-1 */
#define X_W	239     /*LCD_PHY_WIDTH-1  CD_GetCfg()->PixCfg.xPixNum-1 */

		switch ( LCD_GetCfg()->ScrnRotat )
		{
		case SCREEN_ROTATE_0D:
		{
			HSA	= StartPos.x;
			HEA	= StartPos.x;
			VSA	= StartPos.y;
			VEA	= StartPos.y;
			break;
		}
		case SCREEN_ROTATE_90D:
		{
			HSA	= LCD_GetCfg()->PixCfg.YPixNum - StartPos.y-1;
			HEA	= LCD_GetCfg()->PixCfg.YPixNum - StartPos.y-1;
			VSA	= StartPos.x;
			VEA	= StartPos.x;
			break;
		}
		case SCREEN_ROTATE_180D:
		{
			HSA	= LCD_GetCfg()->PixCfg.YPixNum - StartPos.x-1;
			HEA	= LCD_GetCfg()->PixCfg.YPixNum - StartPos.x-1;
			VSA	= LCD_GetCfg()->PixCfg.YPixNum - StartPos.y-1;
			VEA	= LCD_GetCfg()->PixCfg.YPixNum - StartPos.y-1;
			break;
		}
		case SCREEN_ROTATE_270D:
		{
			HSA	= StartPos.y;
			HEA	= StartPos.y;
			VSA	= LCD_GetCfg()->PixCfg.XPixNum - StartPos.x-1;
			VEA	= LCD_GetCfg()->PixCfg.XPixNum - StartPos.x-1;
			break;
		}
		default: break;
		}
		R61509V_WriteCMD( R61509V_R210_HSA, HSA );			//Window Horizontal RAM Address Start (R210h),
		R61509V_WriteCMD( R61509V_R211_HEA, HEA );			//Window Horizontal RAM Address End (R211h)
		R61509V_WriteCMD( R61509V_R212_VSA, VSA );			//Window Vertical RAM Address Start (R212h),
		R61509V_WriteCMD( R61509V_R213_VEA, VEA );			//Window Vertical RAM Address End (R213h)


		R61509V_WriteCMD( R61509V_R200_HA, HSA );			//RAM Address Set (Horizontal Address) (R200h)
		R61509V_WriteCMD( R61509V_R201_VA, VSA );			//RAM Address Set (Vertical Address) (R201h)
		R61509V_Write_Bus( R61509V_R202_GDRW );
#else
		/*
		 * LCD_SetDrawWindow( StartPos, StartPos);
		 * LCD_SetGramAddress( StartPos );
		 */
#endif
		R61509V_WriteData(color);
/*
		LCD_CS_LOW;
		LCD_RD_HIGH;
		LCD_RS_HIGH;

		LCD_DATA_PORT->ODR = color;
		LCD_WR_LOW;
		LCD_WR_HIGH;
		LCD_CS_HIGH;*/
	}
}

void R61509V_EndDrawDot(void)
{
	R61509V_CS_HIGH;

}
/**************************************************************************************************
* [Function] LCD_SetGramAddress:  ���õ�ǰ���������
* [param01]t_Sreen_Rotate ScrnRotat: ��������д����� ��ת�Ƕ�ֵ
* [param02]t_PixCfg PixCfg: ��ʾ��������
* [param03]t_Point point: �����ֵ
**************************************************************************************************/
void R61509V_SetGramAddress( t_Point point )
{
	u16 HA0 = 0, VA0 = 0;


	switch ( LCD_GetCfg()->ScrnRotat )
	{
	case SCREEN_ROTATE_0D:
	{
		HA0	= point.x;
		VA0	= point.y;
		break;
	}
	case SCREEN_ROTATE_90D:
	{
		HA0	= LCD_GetCfg()->PixCfg.YPixNum - point.y - 1;
		VA0	= point.x;
		break;
	}
	case SCREEN_ROTATE_180D:
	{
		HA0	= LCD_GetCfg()->PixCfg.XPixNum - point.x - 1;
		VA0	= LCD_GetCfg()->PixCfg.YPixNum - point.y - 1;
		break;
	}
	case SCREEN_ROTATE_270D:
	{	

		HA0	= point.y;
		VA0	= LCD_GetCfg()->PixCfg.XPixNum - point.x - 1;
		break;
	}
	default: break;
	}

	R61509V_WriteCMD( R61509V_R200_HA, HA0 );
	R61509V_WriteCMD( R61509V_R201_VA, VA0 );
	R61509V_Write_Bus( R61509V_R202_GDRW );
}


/**************************************************************************************************
* [Function] LCD_Setbackground:  ���ñ�����ɫ
* [param01]u16 BackColor: ������ɫֵ
**************************************************************************************************/
void R61509V_Setbackground( u32 TotalPix, u16 BackColor )
{
	u32 i;

	R61509V_CS_LOW;
	R61509V_RD_HIGH;
	R61509V_RS_HIGH;

	for ( i = 0; i < TotalPix; i++ )
	{
		R61509V_DATABUS_PORT->ODR = BackColor;
		R61509V_WR_LOW;
		R61509V_WR_HIGH;
	}
	R61509V_CS_HIGH;
}


/**************************************************************************************************
* [Function] LCD_SetDrawMode:  �������ܡ�ע������ȵ�����
* [No param]
**************************************************************************************************/
void R61509V_SetDrawMode( void )
{
#define BGR	( (u16) 1 << 12)        /*RGB˳�� */
#define DFM	( (u16) 1 << 14)        /*16-bit��ɫ��ֻ��ʹ��16-bit/8-bit�ӿ� */
#define TRI	( (u16) 0 << 15)        /*8-bit�ӿ������δ��䣬16-bit�ӿ���һ�δ��� */

	u8 Dat, HR, VR, AM, ORG;

	const u8 VHM[4][4][2] = { 6, 4, 5, 7, 0, 2, 3, 1,
				  5, 1, 0, 4, 3, 7, 6, 2,
				  0, 2, 3, 1, 6, 4, 5, 7,
				  3, 7, 6, 2, 5, 1, 0, 4 };


	if ( LCD_GetCfg()->DrawMode.Mode == 0 )
	{
		ORG	= 0;
		VR	= 0;
		HR	= 0;
		AM	= 0;
	}else {
		ORG	= 0;
		Dat	= VHM[LCD_GetCfg()->ScrnRotat][LCD_GetCfg()->DrawMode.Rotat][LCD_GetCfg()->DrawMode.Mirror];
		if ( Dat & 0x04 )
			VR = 1;
		else
			VR = 0;
		if ( Dat & 0x02 )
			HR = 1;
		else
			HR = 0;
		if ( Dat & 0x01 )
			AM = 1;
		else
			AM = 0;
	}

	R61509V_WriteCMD( R61509V_R003_EM, TRI | DFM | BGR | (ORG << 7) | (VR << 5) | (HR << 4) | (AM << 3) );
}


/**************************************************************************************************
* [Function] LCD_PowerOff:  �ر�LCD ��Դ
* [No param]
**************************************************************************************************/
void R61509V_PowerOff( void )
{
	R61509V_WriteCMD( R61509V_R009_DC3, 0 );
	R61509V_Delay( 1 );
	R61509V_BL_OFF;
}


/**************************************************************************************************
* [Function] LCD_DispOff:  �ر�LCD��ʾ( ����?)
* [No param]
**************************************************************************************************/
void R61509V_DispOff( void )
{
	R61509V_WriteCMD( R61509V_R007_DC1, 0x0000 );
}


/**************************************************************************************************
* [Function] LCD_PowerOn: LCD �ϵ粢��ʼ����ؼĴ���
* [No param]
**************************************************************************************************/
void R61509V_PowerOn( void )
{
	R61509V_RST_HIGH;
	R61509V_Delay( 10 );
	R61509V_RST_LOW;
	R61509V_Delay( 20 );
	R61509V_RST_HIGH;
	R61509V_Delay( 50 );

	R61509V_BL_ON;
	
	R61509V_WriteCMD( R61509V_R000_IR, 0x0000 );
	R61509V_WriteCMD( R61509V_R000_IR, 0x0000 );
	R61509V_WriteCMD( R61509V_R000_IR, 0x0000 );
	R61509V_WriteCMD( R61509V_R000_IR, 0x0000 );
	R61509V_Delay( 100 );
	R61509V_WriteCMD( R61509V_R400_BINL, 0x6400 );   /*50*8 lines LCD,start scan at line 0 */
	R61509V_WriteCMD( R61509V_R008_DC2, 0x0808 );    /*8 lines for front and back porch period */
	
	R61509V_WriteCMD( R61509V_R300_YC1, 0x0C00 );    /*gammar correction */
	R61509V_WriteCMD( R61509V_R301_YC2, 0x5A0B );
	R61509V_WriteCMD( R61509V_R302_YC3, 0x0906 );
	R61509V_WriteCMD( R61509V_R303_YC4, 0x1017 );
	R61509V_WriteCMD( R61509V_R304_YC5, 0x2300 );
	R61509V_WriteCMD( R61509V_R305_YC6, 0x1700 );
	R61509V_WriteCMD( R61509V_R306_YC7, 0x6309 );
	R61509V_WriteCMD( R61509V_R307_YC8, 0x0C09 );
	R61509V_WriteCMD( R61509V_R308_YC9, 0x100C );
	R61509V_WriteCMD( R61509V_R309_YC10, 0x2232 );
	
	R61509V_WriteCMD( R61509V_R010_PIC1, 0x0019 );   /*25 clocks per line */
	
	R61509V_WriteCMD( R61509V_R100_PC1, 0x0330 );    /*LCD in highest current,set-up factor */
	R61509V_WriteCMD( R61509V_R101_PC2, 0x0247 );    /*set-up 1 and 2 clock frequence,VCI voltage */
	R61509V_WriteCMD( R61509V_R102_PC3, 0xC1B0 );    /*power on */
	
	R61509V_Delay( 50 );
	
	R61509V_WriteCMD( R61509V_R009_DC3, 0x0001 );    /*source output level to drive non-display V0 to V63 */
	R61509V_WriteCMD( R61509V_R00B_ECC, 0x0000 );    /*262k colors */
	
	R61509V_WriteCMD( R61509V_R00C_EDIC1, 0x0000 );  /*18-bit RGB interface,interal clock interface,RAM access via system interface,RAM write cycle 1frame */
	R61509V_WriteCMD( R61509V_R00F_EDIC2, 0x0000 );  /*DOTCLOCK HSYNCX VSYNCX polarity(RGB IF) */
	
	R61509V_WriteCMD( R61509V_R011_PIC2, 0x0101 );   /* 1clock in adjacent gate,1clock source output delay period */
	R61509V_WriteCMD( R61509V_R012_PIC3, 0x0000 );   /*0 clock VCOM equalize period,0clock source equalize period */
	R61509V_WriteCMD( R61509V_R013_PIC4, 0x0001 );   /* 1clock VCOM alternating timing */
	R61509V_WriteCMD( R61509V_R014_PIC5, 0x0055 );   /*dotclock high and low period ratio(RGB IF) */
	R61509V_WriteCMD( R61509V_R020_PIC6, 0x0019 );   /*DIV of dotclock,dotclock in 1line period (RGB IF) */
	R61509V_WriteCMD( R61509V_R021_PIC7, 0x0101 );   /* 1 clock in non-overlap of adjacent gate output and source output delay */
	R61509V_WriteCMD( R61509V_R022_PIC8, 0x0000 );   /*0 clock in low power VCOM drive and source equalize(RGB IF) */
	R61509V_WriteCMD( R61509V_R023_PIC9, 0x0001 );   /* 1 clock VCOM alternating point (RGB IF) */
	
	R61509V_WriteCMD( R61509V_R090_FMC, 0x0000 );    /* */
	R61509V_WriteCMD( R61509V_R103_PC4, 0x0D00 );    /*factor of VREG1OUT for VCOM alternating voltage */
	R61509V_WriteCMD( R61509V_R401_BIDC, 0x0000 );   /*non-lit display level,vertical roll OFF,display invert OFF */
	R61509V_WriteCMD( R61509V_R002_DWC, 0x0100 );    /*line inversion */
	R61509V_WriteCMD( R61509V_R001_DOC, 0x0100 );    /*source driver shift form s1 to s720, */
	R61509V_WriteCMD( R61509V_R007_DC1, 0x0000 );    /*partial display disable,base image display disbale */
}




/**************************************************************************************************
* [Function] LCD_DrawPixelEx:  �������ܡ�ע������ȵ�����
* [param01]u16 x: description
* [param02]u16 y: description
* [param03]u16 color: description
**************************************************************************************************/
void R61509V_DrawPixelEx( u16 x, u16 y, u16 color )
{
	t_Point p;
	p.x	= x;
	p.y	= y;
//	R61509V_DrawDot(p,color);		//����
}


/**************************************************************************************************
* [Function] LCD_GetPixel:  �������ܡ�ע������ȵ�����
* [param01]u16 X0: description
* [param02]u16 Y0: description
* [return]u16: description
**************************************************************************************************/
u16 R61509V_GetPixel( t_Sreen_Rotate ScrnRotat, t_PixCfg PixCfg, t_Point p )
{
	u16 Dat = 0;
	R61509V_SetGramAddress( p );
	R61509V_ReadData();
	Dat = R61509V_ReadData();
	return(Dat);
}


/**************************************************************************************************
* [Function] LCD_DrawHLine:  �������ܡ�ע������ȵ�����
* [param01]u16 x: description
* [param02]u16 y: description
* [param03]u16 Len: description
* [param04]u16 color: description
**************************************************************************************************/
void R61509V_DrawHLine( u16 x, u16 y, u16 Len, u16 color )
{
	u16 i;

	for ( i = 0; i < Len; i++ )
		R61509V_DrawPixelEx( i + x, y, color );
}


/**************************************************************************************************
* [Function] LCD_DrawVLine:  �������ܡ�ע������ȵ�����
* [param01]u16 x: description
* [param02]u16 y: description
* [param03]u16 Len: description
* [param04]u16 color: description
**************************************************************************************************/
void R61509V_DrawVLine( u16 x, u16 y, u16 Len, u16 color )
{
	u16 i;
	for ( i = 0; i < Len; i++ )
		R61509V_DrawPixelEx( x, i + y, color );
}


/**************************************************************************************************
* [Function] LCD_SetDispalyRange:  �������ܡ�ע������ȵ�����
* [param01]t_LcdCfg * pLcdCfg: description
* [param02]u16 StartLine: description
* [param03]u16 LineNum: description
**************************************************************************************************/
void R61509V_SetDispalyRange( u16 StartLine, u16 LineNum )
{
	u16	DC1 = 0;
	u16	Width, Height;

	if ( LineNum < LCD_PHY_HEIGHT )
	{
		DC1 = (u16) LCD_DC1_IMAGE_ENABLE_BIT;
		R61509V_WriteCMD( R61509V_R500_PTDP, StartLine );
		R61509V_WriteCMD( R61509V_R501_PTSA, 0 );
		R61509V_WriteCMD( R61509V_R502_PTEA, LineNum - 1 );
		Height	= LineNum;
		Width	= LCD_PHY_WIDTH;
	}else {
		DC1	= (u16) LCD_DC1_PARTIAL_ENABLE_BIT;
		Height	= LCD_PHY_HEIGHT;
		Width	= LCD_PHY_WIDTH;
	}

	if ( (LCD_GetCfg()->ScrnRotat == SCREEN_ROTATE_0D) || (LCD_GetCfg()->ScrnRotat == SCREEN_ROTATE_180D) )
	{
		LCD_GetCfg()->PixCfg.XPixNum	= Width;
		LCD_GetCfg()->PixCfg.YPixNum	= Height;
	}else {
		LCD_GetCfg()->PixCfg.XPixNum	= Height;
		LCD_GetCfg()->PixCfg.YPixNum	= Width;
	}

	R61509V_WriteCMD( R61509V_R007_DC1, DC1 );
}


/**************************************************************************************************
* [Function] LCD_DrawBox:  ͬ������͵ײ����껭һ������
* [param01]t_WinCfg cfg: LCD ������ò���
* [param02]t_Point * pTop: description
* [param03]t_Point * pBottom: description
**************************************************************************************************/
void R61509V_DrawBox( t_Point Top, t_Point Bottom )
{
	u32 i, Cnt;

	Cnt = (Bottom.x - Top.x + 1) * (Bottom.y - Top.y + 1);

	R61509V_SetDrawWindow( Top, Bottom );
	R61509V_SetGramAddress( Top );

	R61509V_CS_LOW;
	R61509V_RS_HIGH;
	R61509V_RD_HIGH;


	for ( i = 0; i < Cnt; i++ )
	{
	
		R61509V_DATABUS_PORT->ODR = LCD_GetCfg()->ForeColor;
		R61509V_WR_LOW;
		R61509V_WR_HIGH;
	}
	R61509V_CS_HIGH;
}


///**************************************************************************************************
//* [Function] LCD_DrawLine:  AB �������껭һ��ֱ��
//* [param01]t_Point * pPointA: A������
//* [param02]t_Point * pPointB: b������
//* [param03]u16 color: ��ɫ
//**************************************************************************************************/
//void R61509V_DrawLine( t_Point *pPointA, t_Point *pPointB, u16 color )
//{
//	s16	x, y, dx, dy, s1, s2;
//	s16	i, p, temp, interchange;


//	x	= pPointA->x;
//	y	= pPointA->y;

//	dx = pPointB->x - pPointA->x;
//	if ( dx < 0 )
//		dx = -dx;
//	dy = pPointB->y - pPointA->y;
//	if ( dy < 0 )
//		dy = -dy;

//	if ( pPointB->x > pPointA->x )
//		s1 = 1;
//	else
//		s1 = -1;

//	if ( pPointB->y > pPointA->y )
//		s2 = 1;
//	else
//		s2 = -1;

//	if ( dy > dx )
//	{
//		temp		= dx;
//		dx		= dy;
//		dy		= temp;
//		interchange	= 1;
//	}else {
//		interchange = 0;
//	}

//	p = 2 * dy - dx;

//	for ( i = 1; i <= dx; i++ )
//	{
//		R61509V_DrawPixelEx( x, y, color );
//		if ( p >= 0 )
//		{
//			if ( interchange == 0 )
//				y += s2;
//			else
//				x += s1;
//			p -= 2 * dx;
//		}
//		if ( interchange == 0 )
//			x += s1;
//		else
//			y += s2;
//		p += 2 * dy;
//	}
//}


///**************************************************************************************************
//* [Function] LCD_DrawRectangle:  ��һ�¾��ο�
//* [param01]t_Point top_p: ��������ֵ
//* [param02]t_Point botton_p : �ذ�����ֵ
//**************************************************************************************************/
//void R61509V_DrawRectangle( t_Point *pTop_p, t_Point *pBotton_p, u16 color )
//{
//	R61509V_DrawHLine( pTop_p->x, pTop_p->y, pBotton_p->x - pTop_p->x + 1, color );
//	R61509V_DrawVLine( pTop_p->x, pTop_p->y, pBotton_p->y - pTop_p->y + 1, color );
//	R61509V_DrawHLine( pTop_p->x, pBotton_p->y, pBotton_p->x - pTop_p->x + 1, color );
//	R61509V_DrawVLine( pBotton_p->x, pTop_p->y, pBotton_p->y - pTop_p->y + 1, color );
//}


///**************************************************************************************************
//* [Function] LCD_DrawCircle:  �������ܡ�ע������ȵ�����
//* [param01]t_Point point: description
//* [param02]u16 R: description
//* [param03]uint8_t Filled: description
//* [param04]u16 color: description
//**************************************************************************************************/
//void R61509V_DrawCircle( t_Point point, u16 R, u8 Filled, u16 color )
//{
//	s16 x, y, d;

//	x	= 0;
//	y	= R;
//	d	= 1 - R;

//	while ( x < y )
//	{
//		if ( Filled )
//		{
//			R61509V_DrawHLine( -y + point.x, x + point.y, 2 * y, color );
//			R61509V_DrawHLine( -x + point.x, y + point.y, 2 * x, color );
//			R61509V_DrawHLine( -x + point.x, -y + point.y, 2 * x, color );
//			R61509V_DrawHLine( -y + point.x, -x + point.y, 2 * y, color );
//		}
//		else
//		{
//			R61509V_DrawPixelEx( y + point.x, x + point.y, color );
//			R61509V_DrawPixelEx( x + point.x, y + point.y, color );
//			R61509V_DrawPixelEx( x + point.x, -y + point.y, color );
//			R61509V_DrawPixelEx( y + point.x, -x + point.y, color );
//			R61509V_DrawPixelEx( -y + point.x, x + point.y, color );
//			R61509V_DrawPixelEx( -x + point.x, y + point.y, color );
//			R61509V_DrawPixelEx( -x + point.x, -y + point.y, color );
//			R61509V_DrawPixelEx( -y + point.x, -x + point.y, color );
//		}

//		if ( d < 0 )
//			d += 2 * x + 3;
//		else{
//			d += 2 * (x - y) + 5;
//			y--;
//		}
//		x++;
//	}
//}


/**************************************************************************************************
* [Function] LCD_DrawFullFillBuf:  ȫ�����ص㵥ɫ�����ʾ
* [param01]u8 * pdata: ����ڴ����� ÿλ Ϊ 0 ����ʾ Ϊ1��ʾ�����ɫ ע��������ݰ��������ֽ�������
* [param02]u16 color: �����ɫֵ
**************************************************************************************************/
void R61509V_DrawFullFillBuf( u8 *pdata, u16 color )
{
	u16	i, j;
	u8	*pBuf		= pdata;
	u8	rowcount	= (LCD_PHY_WIDTH + 7) / 8;/*ÿ���ֽ��� */
	t_Point p;
	p.x	= 0;
	p.y	= 0;
	R61509V_SetGramAddress( p );

	R61509V_CS_LOW;
	R61509V_RS_HIGH;
	R61509V_RD_HIGH;

	for ( j = 0; j < LCD_PHY_HEIGHT; j++ )
	{
		for ( i = 0; i < LCD_PHY_WIDTH; i++ )
		{
			if ( pBuf[i / 8] & (0x80 >> (i % 8) ) )
			{
				R61509V_DATABUS_PORT->ODR = color;
				R61509V_WR_LOW;
				R61509V_WR_HIGH;
			}
		}
		pBuf += rowcount;
	}


	R61509V_CS_HIGH;
}


/**************************************************************************************************
* [Function] LCD_DrawFullFillColorBuf:  �������ܡ�ע������ȵ�����
* [param01]u8 * pdata: description
**************************************************************************************************/
void R61509V_DrawFullFillColorBuf( u8 *pdata )
{
	u16	i, j;
	u8	*pBuf = pdata;
	t_Point pos;
	pos.x	= 0;
	pos.y	= 0;
	R61509V_SetGramAddress( pos );

	R61509V_CS_LOW;
	R61509V_RS_HIGH;
	R61509V_RD_HIGH;

	for ( j = 0; j < LCD_PHY_HEIGHT; j++ )
	{
		for ( i = 0; i < LCD_PHY_WIDTH; i++ )
		{
			u16 color = pBuf[j * LCD_PHY_WIDTH + 2 * i] | pBuf[j * LCD_PHY_WIDTH + 2 * i + 1];


			R61509V_DATABUS_PORT->ODR = color;

			R61509V_WR_LOW;
			R61509V_WR_HIGH;
			/*  pBuf += 2; */
		}
	}

	R61509V_CS_HIGH;
}


/**************************************************************************************************
* [Function] LCD_DrawRectangleFillBuf:   �������ص㵥ɫ�����ʾ
* [param01]t_Point top: �����ο�ʼ����
* [param02]t_Point bottom: �����ν�������
* [param03]u8 * pdata: ����ڴ����� ÿλ Ϊ 0 ����ʾ Ϊ1��ʾ�����ɫ ע��������ݰ��������ֽ�������
* [param04]u16 color: �����ɫֵ
**************************************************************************************************/
void R61509V_DrawRectangleFillBuf( t_LcdCfg *pcfg, t_Rect *pRect, u8 *pdata, u16 color )
{
	u16	i, j;
	u8	*pBuf		= pdata;
	u8	rowcount	= (pRect->bottom.x - pRect->top.x + 7) / 8;/*ÿ���ֽ��� */

	t_Point pos;
	pos.x	= 0;
	pos.y	= 0;
	R61509V_SetGramAddress( pos );


	R61509V_CS_LOW;
	R61509V_RS_HIGH;
	R61509V_RD_HIGH;

	for ( j = 0; j < LCD_PHY_HEIGHT; j++ )
	{
		for ( i = 0; i < LCD_PHY_WIDTH; i++ )
		{
			if ( pBuf[i / 8] & (0x80 >> (i % 8) ) )
			{
				R61509V_DATABUS_PORT->ODR = color;
				R61509V_WR_LOW;
				R61509V_WR_HIGH;
			}/*else {
			  * LCD_DATA_PORT->ODR = color;
			  * LCD_WR_LOW;
			  * LCD_WR_HIGH;
			  * }*/
		}
		pBuf += rowcount;
	}

	R61509V_CS_HIGH;
}


/**************************************************************************************************
* [Function] LCD_DrawRectangleColorBuf:  �������ܡ�ע������ȵ�����
* [param01]t_Rect * pRect: description
* [param02]u8 * pdata: description
**************************************************************************************************/
void R61509V_DrawRectangleColorBuf( t_Rect *pRect, u8 *pdata )
{
	u16	i, j;
	u8	*pBuf = pdata;

	u16	Height, Width;
	t_Point pos;
	pos.x	= 0;
	pos.y	= 0;
	R61509V_SetGramAddress( pos );
	Width	= pRect->bottom.x - pRect->top.x;
	Height	= pRect->bottom.y - pRect->top.y;
	/*u8	rowcount	= ( + 7) / 8;/ *ÿ���ֽ��� * / */

	R61509V_CS_LOW;
	R61509V_RS_HIGH;
	R61509V_RD_HIGH;

	for ( j = 0; j < Height; j++ )
	{
		for ( i = 0; i < Width; i++ )
		{
			u16 color = (pBuf[j * Width + 2 * i] << 8) | pBuf[j * Width + 2 * i + 1];

			R61509V_DATABUS_PORT->ODR = color;


			R61509V_WR_LOW;
			R61509V_WR_HIGH;
		}
	}

	R61509V_CS_HIGH;
}


/**************************************************************************************************
* [Function] LCD_Init:  �������ܡ�ע������ȵ�����
* [param01]t_LcdCfg * pLcdpara: description
**************************************************************************************************/
void R61509V_Init( t_LcdCfg **pLcdpara )
{
//	GPIO_InitTypeDef GPIO_InitStructure;
	(*pLcdpara) = LCD_GetCfg();

//	GPIO_InitStructure.GPIO_Pin	= LCD_CS_PIN ;
//	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
//	GPIO_Init( LCD_CS_PORT, &GPIO_InitStructure );
//		
//	GPIO_InitStructure.GPIO_Pin	=  LCD_RS_PIN ;
//	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
//	GPIO_Init( LCD_RS_PORT, &GPIO_InitStructure );
//	GPIO_InitStructure.GPIO_Pin	= LCD_WR_PIN ;
//	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
//	GPIO_Init( LCD_WR_PORT, &GPIO_InitStructure );

//	GPIO_InitStructure.GPIO_Pin	= LCD_RD_PIN ;
//	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
//	GPIO_Init( LCD_RD_PORT, &GPIO_InitStructure );

//	GPIO_InitStructure.GPIO_Pin	=  LCD_RST_PIN ;
//	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
//	GPIO_Init( LCD_RST_PORT, &GPIO_InitStructure );

/*	GPIO_InitStructure.GPIO_Pin	=  LCD_BL_PIN ;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init( LCD_BL_PORT, &GPIO_InitStructure );*/

    PWM_OUT((TIM_TypeDef*) TIM2_BASE,PWM_OUTChannel4,2000,500);


//	GPIO_InitStructure.GPIO_Pin	=  LCD_TE_PIN;
//	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
//	GPIO_Init( LCD_TE_PORT, &GPIO_InitStructure );

//	GPIO_InitStructure.GPIO_Pin	= GPIO_Pin_All;
//	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
//	GPIO_Init( LCD_DATA_PORT, &GPIO_InitStructure );


	/*
	 * (*pLcdpara)->PixCfg.XPixNum	= LCD_PHY_WIDTH;
	 * (*pLcdpara)->PixCfg.YPixNum	= LCD_PHY_HEIGHT;
	 */

	(*pLcdpara)->ForeColor		= R61509V_POINT_COLOR;
	(*pLcdpara)->ScrnRotat		= SCREEN_ROTATE_270D;//SCREEN_ROTATE_90D;
	(*pLcdpara)->DrawMode.Rotat	= DRAW_ROTATE_0D;
	(*pLcdpara)->DrawMode.Mirror	= 0;
	(*pLcdpara)->DrawMode.Rev	= 0;
	(*pLcdpara)->DrawMode.Mode	= 0;
}

/*******************************************************************************
*������		:	Address_set
*��������	:	STM32�ڲ��¶ȴ���������
*����			: 
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
void R61509V_Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	R61509V_Write_Bus(0x002A);			//�����е�ַ
	R61509V_WriteData(x1>>8);	    
	R61509V_WriteData(x1&0x00ff);
	R61509V_WriteData(x2>>8);	    
	R61509V_WriteData(x2&0x00ff);
	R61509V_Write_Bus(0x002b);			//����ҳ��ַ	
	R61509V_WriteData(y1>>8);	    
	R61509V_WriteData(y1&0x00ff);
	R61509V_WriteData(y2>>8);	    
	R61509V_WriteData(y2&0x00ff);
	R61509V_Write_Bus(0x002c);			//д�ڴ���ʼ��ַ 					 						 
}
///*******************************************************************************
//*������		:	LCD_Fill
//*��������	:	��ָ�����������ָ����ɫ
//*����			: �����С:(xend-xsta)*(yend-ysta)
//*���			:	��
//*����ֵ		:	��
//*����			:
//*******************************************************************************/
//void R61509V_Fill(
//							u16 xsta,u16 ysta,
//							u16 xend,u16 yend,u16 color
//)
//{          
//	u16 i,j; 
//	R61509V_Address_set(xsta,ysta,xend,yend);      //���ù��λ�� 
//	for(i=ysta;i<=yend;i++)
//	{													   	 	
//		for(j=xsta;j<=xend;j++)
//			R61509V_WriteData(color);//���ù��λ�� 	    
//	} 					  	    
//}

/*************************************************************************************************************************************************/
/*************************************************************************************************************************************************/
/*************************************************************************************************************************************************/
/*************************************************************************************************************************************************/
/*************************************************************************************************************************************************/

#if 1
//д16λ���ݣ�
void R61509V_WriteData16(unsigned int dashuju)			//д16λ����
{
	R61509V_CS_LOW; //Ƭѡ��
	R61509V_RS_HIGH; //����
	R61509V_WR_LOW;
	R61509V_Delay(20);
	R61509V_DATABUS_PORT->ODR = dashuju;
	R61509V_Delay(20);
	R61509V_WR_HIGH;
	R61509V_CS_HIGH;
	R61509V_Delay(20);
}

//д16λ���
void R61509V_WriteIndex16(unsigned int damingling)	//д16λ�����������
{
	R61509V_CS_LOW; //Ƭѡ��
	R61509V_RS_LOW; //����
	R61509V_WR_LOW;
	R61509V_Delay(20);
	R61509V_DATABUS_PORT->ODR = damingling;
	R61509V_Delay(20);
	R61509V_WR_HIGH;
	R61509V_CS_HIGH;
	R61509V_Delay(20);
}

#else
//д8λ���ݣ�
void R61509V_WriteData16(unsigned int dashuju)			//д16λ����
{
	//****д���λ
	R61509V_CS_LOW; //Ƭѡ��
	R61509V_RS_HIGH; //����
	R61509V_WR_LOW;
	DATA=dashuju>>8;
	R61509V_WR_HIGH;
//****д���λ
	R61509V_WR_LOW;
	DATA=dashuju;
	R61509V_WR_HIGH;
	R61509V_CS_HIGH;
}

//д16λ���
void R61509V_WriteIndex16(unsigned int damingling)		//д16λ�����������
{
	//****д���λ
	R61509V_CS_LOW; //Ƭѡ��
	R61509V_RS_LOW; //����
	R61509V_WR_LOW;
	DATA=damingling>>8;
	R61509V_WR_HIGH;
	//****д���λ
	R61509V_WR_LOW;
	DATA=damingling;
	R61509V_WR_HIGH;
	R61509V_CS_HIGH;
}
#endif
/*******************************************************************************
*������			:	R61509V_WriteCommand
*��������		:	д������������
*����				: 
*����ֵ			:	��
*******************************************************************************/
void R61509V_WriteCommand(												//д������������
													unsigned int index,			//�Ĵ�������
													unsigned int Command		//����
													)	//д������������
{
	R61509V_WriteIndex16(index);		//д16λ�����������
	R61509V_Delay(10);
	R61509V_WriteData16(Command);
}
/*******************************************************************************
*������			:	R61509V_SetWindowAddress
*��������		:	���ô��ڵ�ַ
*����				: 
*����ֵ			:	��
*******************************************************************************/
void R61509V_SetWindowAddress(			//���ô��ڵ�ַ
															unsigned int x1,
															unsigned int y1,
															unsigned int x2,
															unsigned int y2
															)
{	
#if 0	
	R61509V_WriteCommand(0x210,x1);			//Window Horizontal RAM Address Start(R210h)
//	R61509V_Delay(100);
	R61509V_WriteCommand(0x211,x2);			//Window Horizontal RAM Address End(R211h)
//	R61509V_Delay(100);
	R61509V_WriteCommand(0x212,y1);			//Window Vertical RAM Address Start (R212h)
//	R61509V_Delay(100);
	R61509V_WriteCommand(0x213,y2);			//Window Vertical RAM Address End (R213h)
//	R61509V_Delay(100);
	
	R61509V_WriteCommand(0x200,x1);			//RAM Address Set (Horizontal Address) (R200h)
//	R61509V_Delay(100);
	R61509V_WriteCommand(0x201,y1);			//RAM Address Set (Vertical Address) (R201h)
//	R61509V_Delay(100);
	R61509V_WriteIndex16(0x0202);				//GRAM(Graphics RAM--ͼ���ڴ�) Data Write (R202h)׼��д��
//	R61509V_Delay(100);		//׼��д

#else
//	R61509V_W	400
//	R61509V_H 240
	R61509V_WriteCommand(0x210,y1);			//Window Horizontal RAM Address Start(R210h)
	R61509V_WriteCommand(0x211,y2);			//Window Horizontal RAM Address End(R211h)
	R61509V_WriteCommand(0x212,R61509V_W-x2-1);			//Window Vertical RAM Address Start (R212h)
	R61509V_WriteCommand(0x213,R61509V_W-x1-1);			//Window Vertical RAM Address End (R213h)
	
	R61509V_WriteCommand(0x200,y1);			//RAM Address Set (Horizontal Address) (R200h)
	R61509V_WriteCommand(0x201,R61509V_W-x2-1);			//RAM Address Set (Vertical Address) (R201h)
	
	R61509V_WriteCommand(0x003,0X5018);			//RAM Address Set (Vertical Address) (R201h)
	
	R61509V_WriteIndex16(0x0202);				//GRAM(Graphics RAM--ͼ���ڴ�) Data Write (R202h)׼��д��
#endif
}


/*******************************************************************************
*������			:	R61509V_Initialize
*��������		:	IC��ʼ������������оƬR61509V��power supply on sequence ��������
*����				: 
*����ֵ			:	��
*******************************************************************************/
void R61509V_Initialize(void)			//��������оƬR61509V��power supply on sequence ��������
{
	u32 dtime=500;
	#if 1
	R61509V_WR_HIGH;
	R61509V_RD_HIGH;
	R61509V_CS_LOW;
	R61509V_RST_HIGH;
	R61509V_Delay(dtime);
	R61509V_RST_LOW;
	R61509V_Delay(dtime);
	R61509V_RST_HIGH;
	
	R61509V_WriteCommand(R61509V_R000_IR		,	0x0000);		R61509V_Delay(dtime); //�Ĵ�д0x00
	R61509V_WriteCommand(R61509V_R000_IR		,	0x0000);		R61509V_Delay(dtime); //�Ĵ�д0x00
	R61509V_WriteCommand(R61509V_R000_IR		,	0x0000);		R61509V_Delay(dtime); //�Ĵ�д0x00
	R61509V_WriteCommand(R61509V_R000_IR		,	0x0000);		R61509V_Delay(dtime); //�Ĵ�д0x00
	
	R61509V_WriteCommand(R61509V_R001_DOC		,	0x0100);		R61509V_Delay(dtime); //--------��ʾ�������When SS = ��1�� and BGR = ��1��, RGB dots are assigned one to one from S720 to S1.
	R61509V_WriteCommand(R61509V_R002_DWC		,	0x0100);		R61509V_Delay(dtime); //--------	
	R61509V_WriteCommand(R61509V_R003_EM		,	0X5030);		R61509V_Delay(dtime); //--------RGB��Ϊ5-6-5ģʽ------------0X5030
	R61509V_WriteCommand(R61509V_R007_DC1		,	0x0100);		R61509V_Delay(dtime); //--------������ʾ����base image�Ĺ��뿪
	R61509V_WriteCommand(R61509V_R008_DC2		,	0X0808);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R009_DC3		,	0x0001);		R61509V_Delay(dtime); //--------
	
	
	
	R61509V_WriteCommand(R61509V_R400_BINL	,	0X6400);		R61509V_Delay(dtime); //base image ���� 400��
	
	
	R61509V_WriteCommand(R61509V_R300_YC1		,	0x0C00);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R301_YC2		,	0x5A0B);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R302_YC3		,	0x0906);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R303_YC4		,	0x1017);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R304_YC5		,	0x2300);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R305_YC6		,	0x1700);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R306_YC7		,	0x6309);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R307_YC8		,	0x0C09);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R308_YC9		,	0x100C);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R309_YC10	,	0x2232);		R61509V_Delay(dtime); //--------
	
	R61509V_WriteCommand(R61509V_R010_PIC1	,	0x001F);		R61509V_Delay(dtime); //--------���Ƶ��--0x19
	R61509V_WriteCommand(R61509V_R100_PC1		,	0x0330);		R61509V_Delay(dtime); //--------���õ�Դ���ƣ�ƽ����ʾ�������
	R61509V_WriteCommand(R61509V_R101_PC2		,	0x0247);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R102_PC3		,	0xC1B0);		R61509V_Delay(dtime); //--------PSON,PON ����Ϊ1����Դ��Ӧ��
	
	
	R61509V_WriteCommand(R61509V_R00B_ECC		,	0x0001);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R00C_EDIC1	,	0x0000);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R00F_EDIC2	,	0x0000);		R61509V_Delay(dtime); //--------
	
	R61509V_WriteCommand(R61509V_R011_PIC2	,	0x0101);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R012_PIC3	,	0x0000);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R013_PIC4	,	0x0001);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R014_PIC5	,	0x0055);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R020_PIC6	,	0x0019);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R021_PIC7	,	0x0101);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R022_PIC8	,	0x0000);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R023_PIC9	,	0x0001);		R61509V_Delay(dtime); //--------
	
	R61509V_WriteCommand(R61509V_R090_FMC		,	0x8000);		R61509V_Delay(dtime); //--------���������ƣ�FMARK interface
	R61509V_WriteCommand(R61509V_R103_PC4		,	0x0D00);		R61509V_Delay(dtime); //--------
	R61509V_WriteCommand(R61509V_R401_BIDC	,	0x0004);		R61509V_Delay(dtime); //--------����base image ����ģʽ����������(��ɫ�������ã�

	
	R61509V_WriteCommand(R61509V_R280_NVM		,	0xB000);		R61509V_Delay(dtime);	//--------VCM	
	R61509V_WriteCommand(R61509V_R502_PTEA	,	0x005f);		R61509V_Delay(dtime);
	R61509V_WriteCommand(R61509V_R404_BIVSC	,	0x0000);		R61509V_Delay(dtime);	//--------��0�߿�ʼ��
	R61509V_WriteCommand(R61509V_R200_HA		,	0x0000);		R61509V_Delay(dtime);	//--------RAM address ����
	R61509V_WriteCommand(R61509V_R201_VA		,	0x0000);		R61509V_Delay(dtime);	//--------
	
	
	//-----------------���Կ�ʼ	
//	R61509V_WriteCommand(R61509V_R210_HSA		,	0x0015);		R61509V_Delay(dtime);	//--------RAM address ����
//	R61509V_WriteCommand(R61509V_R211_HEA		,	0x00E0);		R61509V_Delay(dtime);	//--------
//	
//	R61509V_WriteCommand(R61509V_R212_VSA		,	0x0015);		R61509V_Delay(dtime);	//--------RAM address ����
//	R61509V_WriteCommand(R61509V_R213_VEA		,	0x00FF);		R61509V_Delay(dtime);	//--------
//	
//	R61509V_WriteCommand(R61509V_R200_HA		,	0x0015);		R61509V_Delay(dtime);	//--------RAM address ����
//	R61509V_WriteCommand(R61509V_R201_VA		,	0x0015);		R61509V_Delay(dtime);	//--------
	//-----------------���Խ���

	R61509V_WriteIndex16(0X202);					R61509V_Delay(dtime);		//׼��д��

//	Sreen_Rotate=SCREEN_ROTATE_0D;				//��ʾ��ʽ
	
	
	
	
	
	
	
	
	
	
	
	


















//----����201707181400
//	R61509V_WR_HIGH;
//	R61509V_RD_HIGH;
//	R61509V_CS_LOW;
//	R61509V_RST_HIGH;
//	R61509V_Delay(dtime);
//	R61509V_RST_LOW;
//	R61509V_Delay(dtime);
//	R61509V_RST_HIGH;
//	
//	R61509V_WriteCommand(R61509V_R000_IR		,		0x00);		R61509V_Delay(dtime); //�Ĵ�д0x00
//	R61509V_WriteCommand(R61509V_R000_IR		,		0x00);		R61509V_Delay(dtime); //�Ĵ�д0x00
//	R61509V_WriteCommand(R61509V_R000_IR		,		0x00);		R61509V_Delay(dtime); //�Ĵ�д0x00
//	R61509V_WriteCommand(R61509V_R000_IR		,		0x00);		R61509V_Delay(dtime); //�Ĵ�д0x00
//	
//	R61509V_WriteCommand(R61509V_R400_BINL	,	0X6400);		R61509V_Delay(dtime); //base image ���� 400��
//	R61509V_WriteCommand(R61509V_R008_DC2		,	0X0808);		R61509V_Delay(dtime); //--------
//	
//	R61509V_WriteCommand(R61509V_R300_YC1		,	0x0C00);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R301_YC2		,	0x5A0B);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R302_YC3		,	0x0906);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R303_YC4		,	0x1017);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R304_YC5		,	0x2300);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R305_YC6		,	0x1700);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R306_YC7		,	0x6309);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R307_YC8		,	0x0C09);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R308_YC9		,	0x100C);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R309_YC10	,	0x2232);		R61509V_Delay(dtime); //--------
//	
//	R61509V_WriteCommand(R61509V_R010_PIC1	,	0x0019);		R61509V_Delay(dtime); //--------���Ƶ��
//	R61509V_WriteCommand(R61509V_R100_PC1		,	0x0330);		R61509V_Delay(dtime); //--------���õ�Դ���ƣ�ƽ����ʾ�������
//	R61509V_WriteCommand(R61509V_R101_PC2		,	0x0247);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R102_PC3		,	0xC1B0);		R61509V_Delay(dtime); //--------PSON,PON ����Ϊ1����Դ��Ӧ��
//	
//	R61509V_WriteCommand(R61509V_R009_DC3		,	0x0001);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R00B_ECC		,	0x0001);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R00C_EDIC1	,	0x0000);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R00F_EDIC2	,	0x0000);		R61509V_Delay(dtime); //--------
//	
//	R61509V_WriteCommand(R61509V_R011_PIC2	,	0x0101);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R012_PIC3	,	0x0000);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R013_PIC4	,	0x0001);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R014_PIC5	,	0x0055);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R020_PIC6	,	0x0019);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R021_PIC7	,	0x0101);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R022_PIC8	,	0x0000);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R023_PIC9	,	0x0001);		R61509V_Delay(dtime); //--------
//	
//	R61509V_WriteCommand(R61509V_R090_FMC		,	0x8000);		R61509V_Delay(dtime); //--------���������ƣ�FMARK interface
//	R61509V_WriteCommand(R61509V_R103_PC4		,	0x0D00);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R401_BIDC	,	0x0004);		R61509V_Delay(dtime); //--------����base image ����ģʽ����������(��ɫ�������ã�
//	R61509V_WriteCommand(R61509V_R002_DWC		,	0x0100);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(R61509V_R001_DOC		,	0x0100);		R61509V_Delay(dtime); //--------��ʾ�������When SS = ��1�� and BGR = ��1��, RGB dots are assigned one to one from S720 to S1.
//	R61509V_WriteCommand(R61509V_R003_EM		,	0X5030);		R61509V_Delay(dtime); //--------RGB��Ϊ5-6-5ģʽ------------0X5030
//	R61509V_WriteCommand(R61509V_R007_DC1		,	0x0100);		R61509V_Delay(dtime); //--------������ʾ����base image�Ĺ��뿪
//	
//	R61509V_WriteCommand(R61509V_R280_NVM		,	0xB000);		R61509V_Delay(dtime);	//--------VCM	
//	R61509V_WriteCommand(R61509V_R502_PTEA	,	0x005f);		R61509V_Delay(dtime);
//	R61509V_WriteCommand(R61509V_R404_BIVSC	,	0x0000);		R61509V_Delay(dtime);	//--------��0�߿�ʼ��
//	R61509V_WriteCommand(R61509V_R200_HA		,	0x0000);		R61509V_Delay(dtime);	//--------RAM address ����
//	R61509V_WriteCommand(R61509V_R201_VA		,	0x0000);		R61509V_Delay(dtime);	//--------


//	R61509V_WriteIndex16(0X202);					R61509V_Delay(dtime);		//׼��д��

//	Sreen_Rotate=SCREEN_ROTATE_270D;				//��ʾ��ʽ
	



//	R61509V_WR_HIGH;
//	R61509V_RD_HIGH;
//	R61509V_CS_LOW;
//	R61509V_RST_HIGH;
//	R61509V_Delay(dtime);
//	R61509V_RST_LOW;
//	R61509V_Delay(dtime);
//	R61509V_RST_HIGH;
//	
//	R61509V_WriteCommand(R61509V_R000_IR,0x00);			R61509V_Delay(dtime); //�Ĵ�д0x00
//	R61509V_WriteCommand(R61509V_R000_IR,0x00);			R61509V_Delay(dtime); //�Ĵ�д0x00
//	R61509V_WriteCommand(R61509V_R000_IR,0x00);			R61509V_Delay(dtime); //�Ĵ�д0x00
//	R61509V_WriteCommand(R61509V_R000_IR,0x00);			R61509V_Delay(dtime); //�Ĵ�д0x00
//	
//	R61509V_WriteCommand(R61509V_R400_BINL,0X6400);		R61509V_Delay(dtime); //base image ���� 400��
//	R61509V_WriteCommand(0X008,0X0808);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(0X300,0X0C00);		R61509V_Delay(dtime); //--------
//	R61509V_WriteCommand(0X010,0X0019);		R61509V_Delay(dtime); //���Ƶ��
//	R61509V_WriteCommand(0X100,0X330);		R61509V_Delay(dtime);	//���õ�Դ���ƣ�ƽ����ʾ�������
//	R61509V_WriteCommand(0x102,0xd1b0);		R61509V_Delay(dtime);	//PSON,PON ����Ϊ1����Դ��Ӧ��
//	R61509V_WriteCommand(0x280,0xbf00);		R61509V_Delay(dtime);	//VCM


//	
//	R61509V_WriteCommand(0X0003,0X5030);	R61509V_Delay(500);	//RGB��Ϊ5-6-5ģʽ------------0X5030

//	R61509V_WriteCommand(0X0001,0X0100);	R61509V_Delay(dtime);	//When SS = ��1�� and BGR = ��1��, RGB dots are assigned one to one from S720 to S1.
//	R61509V_WriteCommand(0X007,0X0100);		R61509V_Delay(dtime);	//������ʾ����base image�Ĺ��뿪

//	R61509V_WriteCommand(0X200,0X00);			R61509V_Delay(dtime); //����RAM ADDRESS

//	R61509V_WriteCommand(0X201,0X00);			R61509V_Delay(dtime); 


//	R61509V_WriteCommand(0X401,0X01);			R61509V_Delay(dtime); //����base image ����ģʽ����������

//	R61509V_WriteCommand(0x0C,0x0000);		R61509V_Delay(dtime); //�ⲿ��ʾ�ӿڿ��ƣ�Ĭ��ֵ
//	R61509V_WriteCommand(0x090,0x8000);		R61509V_Delay(dtime); //���������ƣ�FMARK interface
//	R61509V_WriteCommand(0X404,0X00);			R61509V_Delay(dtime);  //��0�߿�ʼ��
//	

//	
//	
//	R61509V_WriteCommand(0X502,0X5f);			R61509V_Delay(dtime);
//	
//	
//	
//	R61509V_WriteCommand(0x200,0x0000);		R61509V_Delay(dtime);		//RAM address ����

//	R61509V_WriteCommand(0x201,0x0000);		R61509V_Delay(dtime);

//	R61509V_WriteIndex16(0X202);					R61509V_Delay(dtime);		//׼��д��


//	Sreen_Rotate=SCREEN_ROTATE_270D;				//��ʾ��ʽ
	
	
	#else
	
	
	R61509V_WR_HIGH;
	R61509V_RD_HIGH;
	R61509V_CS_LOW;
	R61509V_RST_HIGH;
	R61509V_Delay(200);
	R61509V_RST_LOW;
	R61509V_Delay(2000);
	R61509V_RST_HIGH;
	
	
	R61509V_WriteCommand(0x00,0x00);R61509V_Delay(500); //�Ĵ�д0x00
	R61509V_WriteCommand(0x00,0x00);R61509V_Delay(500);
	R61509V_WriteCommand(0x00,0x00);R61509V_Delay(500);
	R61509V_WriteCommand(0x00,0x00);R61509V_Delay(500);
	R61509V_WriteCommand(0X400,0X6200); //base image ���� 400��
	R61509V_WriteCommand(0X010,0X0019); //���Ƶ��
	R61509V_WriteCommand(0X100,0X330);	//���õ�Դ���ƣ�ƽ����ʾ�������
	R61509V_WriteCommand(0x102,0xd1b0);R61509V_Delay(500);//PSON,PON ����Ϊ1����Դ��Ӧ��
	R61509V_WriteCommand(0x280,0xbf00);R61509V_Delay(500);//VCM
	R61509V_Delay(500);
	R61509V_WriteCommand(0X0003,0X5030);R61509V_Delay(500);//RGB��Ϊ5-6-5ģʽ
	R61509V_Delay(200);
	R61509V_WriteCommand(0X0001,0X0100);//When SS = ��1�� and BGR = ��1��, RGB dots are assigned one to one from S720 to S1.
	R61509V_WriteCommand(0X007,0X0100);//������ʾ����base image�Ĺ��뿪
	R61509V_Delay(200);
	R61509V_Delay(200);
	R61509V_WriteCommand(0X200,0X00); //����RAM ADDRESS
	R61509V_Delay(200);
	R61509V_WriteCommand(0X201,0X00); 
	R61509V_Delay(200);


	R61509V_Delay(200);
	R61509V_WriteCommand(0X401,0X01); 										//����base image ����ģʽ����������
	R61509V_Delay(200);
	R61509V_WriteCommand(0x0C,0x0000);R61509V_Delay(1000); //�ⲿ��ʾ�ӿڿ��ƣ�Ĭ��ֵ
	R61509V_WriteCommand(0x090,0x8000);R61509V_Delay(1000); //���������ƣ�FMARK interface
	R61509V_WriteCommand(0X404,0X00);R61509V_Delay(1000);  //��0�߿�ʼ��
	R61509V_WriteCommand(0X502,0X5f);R61509V_Delay(1000);
	R61509V_WriteCommand(0x200,0x0000);		//RAM address ����
	R61509V_Delay(1000);
	R61509V_WriteCommand(0x201,0x0000);
	R61509V_Delay(1000);
	R61509V_Delay(1000);
	R61509V_WriteIndex16(0X202);		//׼��д��
	R61509V_Delay(1000);
	#endif
}
/*******************************************************************************
*������			:	R61509V_Initialize
*��������		:	IC��ʼ������������оƬR61509V��power supply on sequence ��������
*����				: 
*����ֵ			:	��
*******************************************************************************/
void R61509V_InitializeB(void)			//��������оƬR61509V��power supply on sequence ��������
{
//	#if 1
//	R61509V_WR_HIGH;
//	R61509V_RD_HIGH;
//	R61509V_CS_LOW;
//	R61509V_RST_HIGH;
//	R61509V_Delay(200);
//	R61509V_RST_LOW;
//	R61509V_Delay(2000);
//	R61509V_RST_HIGH;
//	
//	R61509V_WriteCommand(0x00,0x00);R61509V_Delay(500); //�Ĵ�д0x00
//	R61509V_WriteCommand(0x00,0x00);R61509V_Delay(500);
//	R61509V_WriteCommand(0x00,0x00);R61509V_Delay(500);
//	R61509V_WriteCommand(0x00,0x00);R61509V_Delay(500);
//	R61509V_WriteCommand(0X400,0X6200); //base image ���� 400��
//	R61509V_WriteCommand(0X010,0X0019); //���Ƶ��
//	R61509V_WriteCommand(0X100,0X330);	//���õ�Դ���ƣ�ƽ����ʾ�������
//	R61509V_WriteCommand(0x102,0xd1b0);R61509V_Delay(500);//PSON,PON ����Ϊ1����Դ��Ӧ��
//	R61509V_WriteCommand(0x280,0xbf00);R61509V_Delay(500);//VCM
//	R61509V_Delay(500);
//	R61509V_WriteCommand(0X0003,0X5030);R61509V_Delay(500);//RGB��Ϊ5-6-5ģʽ
//	R61509V_Delay(200);
//	R61509V_WriteCommand(0X0001,0X0100);//When SS = ��1�� and BGR = ��1��, RGB dots are assigned one to one from S720 to S1.
//	R61509V_WriteCommand(0X007,0X0100);//������ʾ����base image�Ĺ��뿪
//	R61509V_Delay(200);
//	R61509V_Delay(200);
//	R61509V_WriteCommand(0X200,0X00); //����RAM ADDRESS
//	R61509V_Delay(200);
//	R61509V_WriteCommand(0X201,0X00); 
//	R61509V_Delay(200);


//	R61509V_Delay(200);
//	R61509V_WriteCommand(0X401,0X01); //����base image ����ģʽ����������
//	R61509V_Delay(200);
//	R61509V_WriteCommand(0x0C,0x0000);R61509V_Delay(1000); //�ⲿ��ʾ�ӿڿ��ƣ�Ĭ��ֵ
//	R61509V_WriteCommand(0x090,0x8000);R61509V_Delay(1000); //���������ƣ�FMARK interface
//	R61509V_WriteCommand(0X404,0X00);R61509V_Delay(1000);  //��0�߿�ʼ��
//	R61509V_WriteCommand(0X502,0X5f);R61509V_Delay(1000);
//	R61509V_WriteCommand(0x200,0x0000);		//RAM address ����
//	R61509V_Delay(1000);
//	R61509V_WriteCommand(0x201,0x0000);
//	R61509V_Delay(1000);
//	R61509V_Delay(1000);
//	R61509V_WriteIndex16(0X202);		//׼��д��
//	R61509V_Delay(1000);
//	
//	#else
//	R61509V_WR_HIGH;
//	R61509V_RD_HIGH;
//	R61509V_CS_LOW;
//	R61509V_RST_HIGH;
//	R61509V_Delay(200);
//	R61509V_RST_LOW;
//	R61509V_Delay(2000);
//	R61509V_RST_HIGH;
//	
//	
//	R61509V_WriteCommand(0x00,0x00);R61509V_Delay(500); //�Ĵ�д0x00
//	R61509V_WriteCommand(0x00,0x00);R61509V_Delay(500);
//	R61509V_WriteCommand(0x00,0x00);R61509V_Delay(500);
//	R61509V_WriteCommand(0x00,0x00);R61509V_Delay(500);
//	R61509V_WriteCommand(0X400,0X6200); //base image ���� 400��
//	R61509V_WriteCommand(0X010,0X0019); //���Ƶ��
//	R61509V_WriteCommand(0X100,0X330);	//���õ�Դ���ƣ�ƽ����ʾ�������
//	R61509V_WriteCommand(0x102,0xd1b0);R61509V_Delay(500);//PSON,PON ����Ϊ1����Դ��Ӧ��
//	R61509V_WriteCommand(0x280,0xbf00);R61509V_Delay(500);//VCM
//	R61509V_Delay(500);
//	R61509V_WriteCommand(0X0003,0X5030);R61509V_Delay(500);//RGB��Ϊ5-6-5ģʽ
//	R61509V_Delay(200);
//	R61509V_WriteCommand(0X0001,0X0100);//When SS = ��1�� and BGR = ��1��, RGB dots are assigned one to one from S720 to S1.
//	R61509V_WriteCommand(0X007,0X0100);//������ʾ����base image�Ĺ��뿪
//	R61509V_Delay(200);
//	R61509V_Delay(200);
//	R61509V_WriteCommand(0X200,0X00); //����RAM ADDRESS
//	R61509V_Delay(200);
//	R61509V_WriteCommand(0X201,0X00); 
//	R61509V_Delay(200);


//	R61509V_Delay(200);
//	R61509V_WriteCommand(0X401,0X01); //����base image ����ģʽ����������
//	R61509V_Delay(200);
//	R61509V_WriteCommand(0x0C,0x0000);R61509V_Delay(1000); //�ⲿ��ʾ�ӿڿ��ƣ�Ĭ��ֵ
//	R61509V_WriteCommand(0x090,0x8000);R61509V_Delay(1000); //���������ƣ�FMARK interface
//	R61509V_WriteCommand(0X404,0X00);R61509V_Delay(1000);  //��0�߿�ʼ��
//	R61509V_WriteCommand(0X502,0X5f);R61509V_Delay(1000);
//	R61509V_WriteCommand(0x200,0x0000);		//RAM address ����
//	R61509V_Delay(1000);
//	R61509V_WriteCommand(0x201,0x0000);
//	R61509V_Delay(1000);
//	R61509V_Delay(1000);
//	R61509V_WriteIndex16(0X202);		//׼��д��
//	R61509V_Delay(1000);
//	#endif
}
/*******************************************************************************
*������			:	R61509V_Clean
*��������		:	�������������Ļ����---�ñ���ɫ�����Ļ
*����				: 
*����ֵ			:	��
*******************************************************************************/
void R61509V_Clean(u16 COLOR)	//�����Ļ����
{
	unsigned char i;
	unsigned int j;

	R61509V_SetWindowAddress(0x00,0x00,R61509V_W-1,R61509V_H-1);
	R61509V_CS_LOW; //Ƭѡ��
	R61509V_RS_HIGH; //����
	for(j=1;j<=R61509V_W;j++)
	{
		for(i=1;i<=R61509V_H;i++)
		{
			R61509V_WR_LOW;
			R61509V_DATABUS_PORT->ODR = COLOR;
			R61509V_WR_HIGH;
		}
	}
	R61509V_CS_HIGH;
} 
/*******************************************************************************
*������			:	R61509V_Clean
*��������		:	��ʾ���ֳ���
*����				: 
*����ֵ			:	��
*******************************************************************************/
void R61509V_CHINESE(unsigned char zimo[720],unsigned int backcolor)  //д���ַ�
{
	unsigned char j,flag=0;
	unsigned char *point;
	unsigned int i,x1=0x00,x2=0x17,y1=0x00,y2=0x17;
	unsigned int colors[10]=
	{
		0xf800,0x07e0,0x001f,0xffe0,0x0000,0x07ff,0xf81f,0xb81f,0x7f7,0xfec0
	};
	point=&zimo[0];
	for(i=1;i<=720;i++,point++)
	{  
		for(j=0;j<=7;j++)
		{
			if((*point<<j)&0x80)
			{
				R61509V_WriteData16(colors[flag]);
				R61509V_Delay(1);
			}
			else
			{
				R61509V_WriteData16(backcolor);
				R61509V_Delay(1);
			}
		}   
		if(i%72==0)
		{
			x1=x1+24;x2=x2+24;y1=y1+24;y2=y2+24;
			flag=flag+1;
			R61509V_SetWindowAddress(x1,y1,x2,y2);
		}   
	}
}
/*******************************************************************************
*������			:	R61509V_DrawPoint
*��������		:	����
*����				: 
*����ֵ			:	��
*******************************************************************************/
void R61509V_DrawDot(u16 x,u16 y,u16 color)
{
//	R61509V_W 400
//	R61509V_H 240
	u16 x1=0,y1=0;
	switch(Sreen_Rotate)
	{
		case SCREEN_ROTATE_0D:{}
			break;
		case SCREEN_ROTATE_90D:{x1=R61509V_H-y;y=x;x=x1;}
			break;
		case SCREEN_ROTATE_180D:{x1=R61509V_H-x;y=R61509V_W-y;x=x1;}
			break;
		case	SCREEN_ROTATE_270D:{x1=y;y=R61509V_W-x;x=x1;}
			break;
		default: break;			
	}

	R61509V_SetWindowAddress(x,y,x,y);	//���ù��λ�� 
	R61509V_WriteData16(color); 	//�ʻ���ɫ	    
}
/*******************************************************************************
*������			:	R61509V_DrawPoint_big
*��������		:	��һ�����
*����				: 
*����ֵ			:	��
*******************************************************************************/
void R61509V_DrawDot_big(u16 x,u16 y,u16 color)
{
//	if((x<=5)&&(y<=5))
//	{
//		R61509V_Fill(0,0,x+10,y+10,color);
//	}
//	else if((x>5)&&(y<=5))
//	{
//		R61509V_Fill(x-5,0,x+5,y+10,color);
//	}
//	else if((x<=5)&&(y>5))
//	{
//		R61509V_Fill(0,y-5,x+10,y+5,color);
//	}
//	else if((x>5)&&(y>5))
//	{
//		R61509V_Fill(x-5,y-5,x+5,y+5,color);
//	}
	
	if((x==0)&&(y==0))
	{
		R61509V_Fill(0,0,x+2,y+2,color);
	}
	else if((x>0)&&(y>0))
	{
		R61509V_Fill(x-1,y-1,x+1,y+1,color);
	}
	
//	R61509V_Fill(x-1,y-1,x+1,y+1,color);
}
/*******************************************************************************
*������			:	R61509V_DrawPoint_big
*��������		:	��ָ�����������ָ����ɫ;�����С:(xend-xsta)*(yend-ysta)
*����				: 
*����ֵ			:	��
*******************************************************************************/
void R61509V_Fill(
							u16 xsta,u16 ysta,
							u16 xend,u16 yend,
							u16 color
)
{          
	unsigned int i;
	unsigned int j;	
	u16 x1=0,y1=0,x2=0,y2=0;

//	switch(Sreen_Rotate)
//	{
//		case 	SCREEN_ROTATE_0D:		
//					{
//						xend=xend-1;
//						yend=yend-1;
//					}
//			break;
//		case 	SCREEN_ROTATE_90D:	
//					{
//						x1=xsta;x2=xend;y1=ysta;y2=yend;	
//						xsta=R61509V_H-y2;	xend=R61509V_H-y1-1;
//						ysta=x1;	yend=x2-1;
//					}
//			break;
//		case 	SCREEN_ROTATE_180D:	
//					{
//						x1=xsta;x2=xend;y1=ysta;y2=yend;
//						xsta=R61509V_H-x2;	xend=R61509V_H-x1-1;
//						ysta=R61509V_W-y2;	yend=R61509V_W-y1-1;
//					}
//			break;
//		case	SCREEN_ROTATE_270D:	
//					{
//						x1=xsta;x2=xend;y1=ysta;y2=yend;
//						xsta=y1;	xend=y2-1;
//						ysta=R61509V_W-x2;	yend=R61509V_W-x1-1;
//					}
//			break;
//		default: break;			
//	}
	
//	switch(Sreen_Rotate)
//	{
//		case SCREEN_ROTATE_0D:{}
//			break;
//		case SCREEN_ROTATE_90D:{x1=R61509V_H-y;y=x;x=x1;}
//			break;
//		case SCREEN_ROTATE_180D:{x1=R61509V_H-x;y=R61509V_W-y;x=x1;}
//			break;
//		case	SCREEN_ROTATE_270D:{x1=y;y=R61509V_W-x;x=x1;}
//			break;
//		default: break;			
//	}

	R61509V_SetWindowAddress(xsta,ysta,xend,yend);
	R61509V_CS_LOW; 					//Ƭѡ��
	R61509V_RS_HIGH; 					//����
	for(i=0;i<xend-xsta;i++)
	{
		for(j=0;j<yend-ysta;j++)
		{
			R61509V_WR_LOW;
			R61509V_DATABUS_PORT->ODR = color;
			R61509V_WR_HIGH;
		}
	}
	R61509V_CS_HIGH;	
}
/*******************************************************************************
*������			:	R61509V_DrawPoint_big
*��������		:	��ָ�����������ָ����ɫ;�����С:(xend-xsta)*(yend-ysta)
*����				: 
*����ֵ			:	��
*******************************************************************************/
/*******************************************************************************
*������		:	LCD_DrawLine
*��������	:	����
*����			: x1,y1:�������
						x2,y2:�յ�����
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
void R61509V_DrawLine(
									u16 x1, u16 y1, 	//x1,y1:�������
									u16 x2, u16 y2,		//x2,y2:�յ�����
									u16 color					//��ɫ
)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)
		incx=1; //���õ������� 
	else if(delta_x==0)
		incx=0;//��ֱ�� 
	else
		{
			incx=-1;
			delta_x=-delta_x;
		} 
	if(delta_y>0)
		incy=1; 
	else if(delta_y==0)
		incy=0;//ˮƽ�� 
	else
		{incy=-1;delta_y=-delta_y;} 
		
	if( delta_x>delta_y)
		distance=delta_x; 								//ѡȡ�������������� 
	else
		distance=delta_y; 
	for(t=0;t<distance+1;t++ )					//������� 
	{  
		R61509V_DrawDot(uRow,uCol,color);	//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}
/**************************************************************************************************
* [Function] LCD_DrawCircle:  �������ܡ�ע������ȵ�����
* [param01]t_Point point: description
* [param02]u16 R: description
* [param03]uint8_t Filled: description
* [param04]u16 color: description
**************************************************************************************************/
void R61509V_DrawCircle(
												u16 x,u16 y,		//Բ������ԭ��
												u16 r,					//�뾶
												u8 Filled,			//�Ƿ����
												u16 color				//��ɫ
												)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		if(Filled)	//���ͻ���
		{
			R61509V_DrawLine(x,y,x-b,y-a,color);             //3           
			R61509V_DrawLine(x,y,x+b,y-a,color);             //0           
			R61509V_DrawLine(x,y,x-a,y+b,color);             //1       
			R61509V_DrawLine(x,y,x-b,y-a,color);             //7           
			R61509V_DrawLine(x,y,x-a,y-b,color);             //2             
			R61509V_DrawLine(x,y,x+b,y+a,color);             //4               
			R61509V_DrawLine(x,y,x+a,y-b,color);             //5
			R61509V_DrawLine(x,y,x+a,y+b,color);             //6 
			R61509V_DrawLine(x,y,x-b,y+a,color);             
			a++;
			//ʹ��Bresenham�㷨��Բ     
			if(di<0)
				di +=4*a+6;	  
			else
			{
				di+=10+4*(a-b);   
				b--;
			}
			R61509V_DrawLine(x,y,x+a,y+b,color);				//AB �������껭һ��ֱ��

		}
		else
		{
			R61509V_DrawDot(x-b,y-a,color);             //3           
			R61509V_DrawDot(x+b,y-a,color);             //0           
			R61509V_DrawDot(x-a,y+b,color);             //1       
			R61509V_DrawDot(x-b,y-a,color);             //7           
			R61509V_DrawDot(x-a,y-b,color);             //2             
			R61509V_DrawDot(x+b,y+a,color);             //4               
			R61509V_DrawDot(x+a,y-b,color);             //5
			R61509V_DrawDot(x+a,y+b,color);             //6 
			R61509V_DrawDot(x-b,y+a,color);             
			a++;
			//ʹ��Bresenham�㷨��Բ     
			if(di<0)
				di +=4*a+6;	  
			else
			{
				di+=10+4*(a-b);   
				b--;
			}
				R61509V_DrawDot(x+a,y+b,color);
		}
	}
}
/**************************************************************************************************
* [Function] LCD_DrawRectangle:  ��һ�¾��ο�
* [param01]t_Point top_p: ��������ֵ
* [param02]t_Point botton_p : �ذ�����ֵ
**************************************************************************************************/
void R61509V_DrawRectangle(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	R61509V_DrawLine( x1, y1,	x1,	y2, color );
	R61509V_DrawLine( x1, y1,	x2,	y1, color );
	R61509V_DrawLine( x2, y1,	x2,	y2, color );
	R61509V_DrawLine( x1, y2,	x2,	y2, color );
}
/*******************************************************************************
* ������			:	function
* ��������		:	��������˵�� 
* ����			: void
* ����ֵ			: void
*******************************************************************************/
void R61509V_ShowChar(
										u16 x,			//x				:���x����
										u16 y,			//y				:���y����
										u8 font,		//font		:�����С
										u8 num,			//num			:�ֽ���
										u8 *Buffer	//Buffer	:��ʾ�����ݻ���
)		//��ͨ�ֿ���Գ���
{
	u8 temp;
//	u8 mode=0;
	u8 i=0,j=0;
//	u8 num=0;
//	u16 x0=x;
	u16 colortemp=R61509V_POINT_COLOR;
	if(font==12)
	{
		if(x>R61509V_W-12||y>R61509V_H-12)
			return;
		R61509V_SetWindowAddress(x,y,x+12-1,y+12-1);      //���ù��λ�� 
	}
	if(font==16)
	{
		if(x>R61509V_W-16||y>R61509V_H-16)
			return;
		R61509V_SetWindowAddress(x,y,x+16-1,y+16-1);      //���ù��λ�� 
	}
	if(font==24)
	{
		if(x>R61509V_W-24||y>R61509V_H-24)
			return;
		R61509V_SetWindowAddress(x,y,x+24-1,y+24-1);      //���ù��λ�� 
	}
	else if(font==32)
	{
		if(x>R61509V_W-32||y>R61509V_H-32)
			return;
		if(num==128)
		{
			R61509V_SetWindowAddress(x,y,x+32-1,y+32-1);      //���ù��λ��
		}
		else
		{
			R61509V_SetWindowAddress(x,y,x+16-1,y+32-1);      //���ù��λ�� 
		}
	}
	else
	{
//		return ;
	}
//	SSD1963_DC_Data;			//����/�����л�	//1-���ݣ�0-����
//	SSD1963_WR_Write; 		//0--д������1--д�ر�
	i=0;
	for(i=0;i<num;i++)
	{ 
		u16 R61509V_PEN_COLOR	=	R61509V_BLUE;   	//����ɫ	
		temp=Buffer[i];		 					//����1608����--��ά������ʽ--�ֿ�ʹ��ʱȡ�� 	
		for(j=0;j<8;j++)
		{
			if((temp&0x80)==0X80)
			{
				R61509V_PEN_COLOR=R61509V_WHITE;
			}
			else
				R61509V_PEN_COLOR=R61509V_BLACK;
			
			R61509V_WriteData16(R61509V_PEN_COLOR);
//			SSD1963_WR_Write; 		//0--д������1--д�ر�
//			GPIO_Write(SSD1963_sPinfo->SSD1963_sDATABUS_PORT,POINT_COLOR);
//			SSD1963_WR_Read; 			//0--д������1--д�ر�
//			LCD_WR_DATA(POINT_COLOR);
//			{
//				LCD_RS(1);
////				LCD_Writ_Bus(POINT_COLOR);
//				{
//					LCD_WR(0); 		//0--д������1--д�ر�
//					GPIO_Write(LCD_DATABUS_PORT,POINT_COLOR);
//					LCD_WR(1); 		//0--д������1--д�ر�
//				}
//			}
			temp=temp<<1;
		}
		R61509V_POINT_COLOR=colortemp;	
	}	
//	SSD1963_WR_Read; 		//0--д������1--д�ر�
//	R61509V_POINT_COLOR=colortemp;	
}

/*******************************************************************************
*������		:	LCD_ShowChar
*��������	:	��ָ��λ����ʾһ���ַ�
*����			: x,y		:�������
						num		:Ҫ��ʾ���ַ�:" "--->"~"
						mode	:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
						num		:Ҫ��ʾ���ַ�:" "--->"~"
*���			:	��
*����ֵ		:	��
*����			:
*******************************************************************************/
void R61509V_ShowCharT(u16 x,u16 y,u8 num,u8 mode)
{
//	R61509V_BACK_COLOR	//����ɫ
//	R61509V_POINT_COLOR	//����ɫ
	u8 temp;
	u8 pos,t;
	u16 x0=x;
	u16 colortemp=R61509V_POINT_COLOR; 
	if(x>R61509V_W-16||y>R61509V_H-16)
		return;	    
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
//	num=41;//�õ�ƫ�ƺ��ֵ
	
	
//	R61509V_SetWindowAddress(x,y,x+8-1,y+16-1);      //���ù��λ�� 
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=code_asc2_1608[(u16)num*16+pos];		 		//����1608����
//			temp=code_num_1608[(u16)num*16+pos];		 //����1608����
			for(t=0;t<8;t++)
			{                 
				if((temp&0x01)==0x01)
					R61509V_POINT_COLOR=colortemp;
				else
					R61509V_POINT_COLOR=R61509V_BACK_COLOR;

				R61509V_DrawDot(x,y,R61509V_POINT_COLOR);//��һ���� 
				
				temp=temp>>1; 
				x++;
			}
			x=x0;
			y++;
		}	
	}
	else//���ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{
			temp=code_asc2_1608[(u16)num*16+pos];		 //����1608����
			for(t=0;t<8;t++)
			{                 
				if(temp&0x01)
					R61509V_DrawDot(x+t,y+pos,R61509V_POINT_COLOR);//��һ����     
				temp>>=1; 
			}
		}
	}
	R61509V_POINT_COLOR=colortemp;	    	   	 	  
}













u32 mypow1(u8 m,u8 n)		//�˷�
{
	u32 result=1;	 
	while(n--)
		result*=m;    
	return result;
}

void R61509V_ShowEn(u16 x,u16 y,u32 num)
{         	
	u8 t=0,len=0;
	u32 num2=num;
	u8 enshow=0;
	while(num2)
	{
		num2=num2/10;
		len++;		
	}
	for(t=len;t>0;t--)
	{
		num2=mypow1(10,t-1);
		if(num>9)
		{
			enshow=num/num2;
			num=num%num2;
		}
		else
			enshow=(char)num;
	 	R61509V_ShowCharT(x+8*(len-t),y,enshow+48,0); 
	}
}



//#endif//LCD_61509_EN
/************************************** The End Of FILE **************************************/


