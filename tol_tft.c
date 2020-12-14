/*
 * tol_tft.c
 *
 *  Created on: 5 May 2018
 *      Author: Tolga
 */

#include "main.h"
#include "stm32f4xx_hal.h"
#include "tol_tft.h"
#include "string.h"
#include "Tahoma10x11.h"
#include "Tahoma18x16.h"
#include "Tahoma24x24.h"

extern NOR_HandleTypeDef hsram3;

unsigned char lcd_yonu=1;

extern uint8_t carpti;
extern uint8_t yesilde;
extern uint8_t yandi;
extern uint8_t puan;
extern uint8_t zipla;
extern uint8_t karakter_zipla;

extern uint16_t zemin_renk;
extern uint16_t duvar_renk;
extern uint16_t dusman_renk;
extern uint16_t zipla_renk;
extern uint16_t puan_renk;
extern uint16_t puan_kordinat_x;
extern uint16_t puan_kordinat_y;


#define MAX_POLY_CORNERS   200
#define POLY_Y(Z)          ((int32_t)((Points + Z)->X))
#define POLY_X(Z)          ((int32_t)((Points + Z)->Y))
#define ABS(X)  ((X) > 0 ? (X) : -(X))


typedef struct{
  __IO uint16_t LCD_REG;
  __IO uint16_t LCD_RAM;
} LCD_TypeDef;


/** @defgroup STM324xG_EVAL_LCD_Private_Defines
  * @{
  */
/* Note: LCD /CS is NE3 - Bank 3 of NOR/SRAM Bank 1~4 */
#define LCD_BASE           ((uint32_t)(0x60000000 | 0x08000000))
#define LCD                ((LCD_TypeDef *) LCD_BASE)
#define MAX_POLY_CORNERS   200
#define POLY_Y(Z)          ((int32_t)((Points + Z)->X))
#define POLY_X(Z)          ((int32_t)((Points + Z)->Y))




/**
  * @brief  Initializes the LCD.
  * @param  None
  * @retval None
  */
void tft_ili9325_init(void){

/* Configure the LCD Control pins --------------------------------------------*/
  LCD_CtrlLinesConfig();
/* Configure the FSMC Parallel interface -------------------------------------*/
  LCD_FSMCConfig();

  HAL_Delay(50); /* delay 50 ms */

    /* Start Initial Sequence ------------------------------------------------*/
    LCD_WriteReg(LCD_REG_0, 0x0001); /* Start internal OSC. */
    LCD_WriteReg(LCD_REG_1, 0x0100); /* Set SS and SM bit */
    LCD_WriteReg(LCD_REG_2, 0x0700); /* Set 1 line inversion */
    LCD_WriteReg(LCD_REG_3, 0x1018); /* Set GRAM write direction and BGR=1. */
    LCD_WriteReg(LCD_REG_4, 0x0000); /* Resize register */
    LCD_WriteReg(LCD_REG_8, 0x0202); /* Set the back porch and front porch */
    LCD_WriteReg(LCD_REG_9, 0x0000); /* Set non-display area refresh cycle ISC[3:0] */
    LCD_WriteReg(LCD_REG_10, 0x0000); /* FMARK function */
    LCD_WriteReg(LCD_REG_12, 0x0000); /* RGB interface setting */
    LCD_WriteReg(LCD_REG_13, 0x0000); /* Frame marker Position */
    LCD_WriteReg(LCD_REG_15, 0x0000); /* RGB interface polarity */

    /* Power On sequence -----------------------------------------------------*/
    LCD_WriteReg(LCD_REG_16, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
    LCD_WriteReg(LCD_REG_17, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
    LCD_WriteReg(LCD_REG_18, 0x0000); /* VREG1OUT voltage */
    LCD_WriteReg(LCD_REG_19, 0x0000); /* VDV[4:0] for VCOM amplitude */
    HAL_Delay(200);                      /* Dis-charge capacitor power voltage (200ms) */
    LCD_WriteReg(LCD_REG_16, 0x17B0); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
    LCD_WriteReg(LCD_REG_17, 0x0137); /* DC1[2:0], DC0[2:0], VC[2:0] */
    HAL_Delay(50);                       /* Delay 50 ms */
    LCD_WriteReg(LCD_REG_18, 0x0139); /* VREG1OUT voltage */
    HAL_Delay(50);                       /* Delay 50 ms */
    LCD_WriteReg(LCD_REG_19, 0x1d00); /* VDV[4:0] for VCOM amplitude */
    LCD_WriteReg(LCD_REG_41, 0x0013); /* VCM[4:0] for VCOMH */
    HAL_Delay(50);                       /* Delay 50 ms */
    LCD_WriteReg(LCD_REG_32, 0x0000); /* GRAM horizontal Address */
    LCD_WriteReg(LCD_REG_33, 0x0000); /* GRAM Vertical Address */

    /* Adjust the Gamma Curve (ILI9325)---------------------------------------*/
    LCD_WriteReg(LCD_REG_48, 0x0007);
    LCD_WriteReg(LCD_REG_49, 0x0302);
    LCD_WriteReg(LCD_REG_50, 0x0105);
    LCD_WriteReg(LCD_REG_53, 0x0206);
    LCD_WriteReg(LCD_REG_54, 0x0808);
    LCD_WriteReg(LCD_REG_55, 0x0206);
    LCD_WriteReg(LCD_REG_56, 0x0504);
    LCD_WriteReg(LCD_REG_57, 0x0007);
    LCD_WriteReg(LCD_REG_60, 0x0105);
    LCD_WriteReg(LCD_REG_61, 0x0808);

    /* Set GRAM area ---------------------------------------------------------*/
    LCD_WriteReg(LCD_REG_80, 0x0000); /* Horizontal GRAM Start Address */
    LCD_WriteReg(LCD_REG_81, 0x00EF); /* Horizontal GRAM End Address */
    LCD_WriteReg(LCD_REG_82, 0x0000); /* Vertical GRAM Start Address */
    LCD_WriteReg(LCD_REG_83, 0x013F); /* Vertical GRAM End Address */

    LCD_WriteReg(LCD_REG_96,  0xA700); /* Gate Scan Line(GS=1, scan direction is G320~G1) */
    LCD_WriteReg(LCD_REG_97,  0x0001); /* NDL,VLE, REV */
    LCD_WriteReg(LCD_REG_106, 0x0000); /* set scrolling line */

    /* Partial Display Control -----------------------------------------------*/
    LCD_WriteReg(LCD_REG_128, 0x0000);
    LCD_WriteReg(LCD_REG_129, 0x0000);
    LCD_WriteReg(LCD_REG_130, 0x0000);
    LCD_WriteReg(LCD_REG_131, 0x0000);
    LCD_WriteReg(LCD_REG_132, 0x0000);
    LCD_WriteReg(LCD_REG_133, 0x0000);

    /* Panel Control ---------------------------------------------------------*/
    LCD_WriteReg(LCD_REG_144, 0x0010);
    LCD_WriteReg(LCD_REG_146, 0x0000);
    LCD_WriteReg(LCD_REG_147, 0x0003);
    LCD_WriteReg(LCD_REG_149, 0x0110);
    LCD_WriteReg(LCD_REG_151, 0x0000);
    LCD_WriteReg(LCD_REG_152, 0x0000);

    /* set GRAM write direction and BGR = 1 */
    /* I/D=00 (Horizontal : increment, Vertical : decrement) */
    /* AM=1 (address is updated in vertical writing direction) */
    LCD_WriteReg(LCD_REG_3, 0x1018);

    LCD_WriteReg(LCD_REG_7, 0x0133); /* 262K color and display ON */

  //LCD_SetFont(&LCD_DEFAULT_FONT);
}


void tft_ili9325_setcursor(unsigned int xpos,unsigned int ypos){
	if(lcd_yonu==1){
		LCD_WriteReg(LCD_REG_32, ypos);
		LCD_WriteReg(LCD_REG_33, 319-xpos);
	}
	if(lcd_yonu==2){
		 LCD_WriteReg(LCD_REG_32, xpos);
		 LCD_WriteReg(LCD_REG_33, ypos);
	}
	if(lcd_yonu==3){
		 LCD_WriteReg(LCD_REG_32, 239-ypos);
		 LCD_WriteReg(LCD_REG_33, xpos);
	}
	if(lcd_yonu==4){
		LCD_WriteReg(LCD_REG_32, 239-xpos);
		LCD_WriteReg(LCD_REG_33, 319-ypos);
	}

}

void tft_ili9325_cerceve(unsigned int xstart,unsigned int xstop,unsigned int ystart,unsigned int ystop){

  if(lcd_yonu==1){
	  LCD_WriteReg(LCD_REG_80, ystart);
	  LCD_WriteReg(LCD_REG_81, ystop);
	  LCD_WriteReg(LCD_REG_82, 319-xstop);
	  LCD_WriteReg(LCD_REG_83, 319-xstart);
  }
  if(lcd_yonu==2){
	  LCD_WriteReg(LCD_REG_80, xstart);
	  LCD_WriteReg(LCD_REG_81, xstop);
	  LCD_WriteReg(LCD_REG_82, ystart);
	  LCD_WriteReg(LCD_REG_83, ystop);
  }
  if(lcd_yonu==3){
	  LCD_WriteReg(LCD_REG_80, 239-ystop);
	  LCD_WriteReg(LCD_REG_81, 239-ystart);
	  LCD_WriteReg(LCD_REG_82, xstart);
	  LCD_WriteReg(LCD_REG_83, xstop);
  }
  if(lcd_yonu==4){
	  LCD_WriteReg(LCD_REG_80, 239-xstop);
	  LCD_WriteReg(LCD_REG_81, 239-xstart);
	  LCD_WriteReg(LCD_REG_82, 319-ystop);
	  LCD_WriteReg(LCD_REG_83, 319-ystart);
  }

}

void tft_ili9325_tarama_yonu(unsigned char yon){

	lcd_yonu=yon;

	if(lcd_yonu==1){
		LCD_WriteReg(LCD_REG_3, 0x1018);
		tft_ili9325_cerceve(0,319,0,239);
	}
	if(lcd_yonu==2){
		LCD_WriteReg(LCD_REG_3, 0x1020);
		tft_ili9325_cerceve(0,239,0,319);
	}
	if(lcd_yonu==3){
		LCD_WriteReg(LCD_REG_3, 0x1028);
		tft_ili9325_cerceve(0,319,0,239);
	}
	if(lcd_yonu==4){
		LCD_WriteReg(LCD_REG_3, 0x1000);
		tft_ili9325_cerceve(0,239,0,319);
	}
}

void tft_ili9325_writepixel(unsigned int xpos,unsigned int ypos,unsigned int rgb_data){

   tft_ili9325_cerceve(xpos,xpos+1,ypos,ypos+1);
   tft_ili9325_setcursor(xpos,ypos);
   LCD_WriteReg(LCD_REG_34, rgb_data);

}

void tft_ili9325_yataycizgi(unsigned int xpos, unsigned int ypos, unsigned int length , unsigned int rgb_code){
  unsigned int i;

  for(i = 0; i < length; i++){
      xpos++;
      tft_ili9325_writepixel(xpos,ypos,rgb_code);
    }
}


void tft_ili9325_dikeycizgi(unsigned int xpos, unsigned int ypos, unsigned int length , unsigned int rgb_code){
  unsigned int i = 0;

  for(i = 0; i < length; i++){
      ypos++;
      tft_ili9325_writepixel(xpos,ypos,rgb_code);
   }
}


void tft_ili9325_dolukare(unsigned int xstart , unsigned int xstop , unsigned int ystart , unsigned int ystop , unsigned int rgb_code){
  unsigned long int i ,j;
  unsigned int x , y;

  if(lcd_yonu==1){
    tft_ili9325_cerceve(0,319,0,239);
  }
  if(lcd_yonu==2){
    tft_ili9325_cerceve(0,239,0,319);
  }
  if(lcd_yonu==3){
    tft_ili9325_cerceve(0,319,0,239);
  }
  if(lcd_yonu==4){
    tft_ili9325_cerceve(0,239,0,319);
  }

  x=xstop-xstart; x++;
  y=ystop-ystart; y++;
  j=x*y;

  tft_ili9325_cerceve(xstart,xstop,ystart,ystop);
  tft_ili9325_setcursor(xstart, ystart);
  LCD_WriteRAM_Prepare();

  for(i=0;i<j;i++){
	  LCD_WriteRAM(rgb_code);
  }
}

void tft_ili9325_boskare(unsigned int xstart , unsigned int xstop , unsigned int ystart , unsigned int ystop , unsigned int kalinlik , unsigned int rgb_code){

   unsigned int x,y,i,yar;
   x=xstop-xstart;
   y=ystop-ystart;

   yar=ystart;
   for(i=0;i<kalinlik;i++){
     tft_ili9325_yataycizgi(xstart,yar ,x,rgb_code);
     yar++;
   }
   yar=ystop;
   for(i=0;i<kalinlik;i++){
     tft_ili9325_yataycizgi(xstart,yar ,x,rgb_code);
     yar--;
   }
   yar=xstart;
   for(i=0;i<kalinlik;i++){
     tft_ili9325_dikeycizgi(yar,ystart ,y,rgb_code);
     yar++;
   }
   yar=xstop;
   for(i=0;i<kalinlik;i++){
     tft_ili9325_dikeycizgi(yar,ystart ,y,rgb_code);
     yar--;
   }
}

void tft_ili9325_harfyaz(unsigned int xpos,unsigned int ypos,unsigned int harf, uint8_t font[],unsigned int rgb_code){


   unsigned int yar3,dongu1=0,kod=0,harf_adr=0,harf_adrs=0,i=0,x2=0,y2=0;
   unsigned int yar1=0, yar2=0;

   y2=font[6];
   yar2=harf-32;
   yar1=(yar2*4)+8;
   x2=font[yar1];

   if (x2<=8){
     x2=7;
   }
   if (x2>8 && x2<=16){
     x2=15;
   }
   if (x2>16 && x2<=24){
     x2=23;
   }
   if (x2>24 && x2<=32){
     x2=31;
   }

   yar1++;
   yar2=font[yar1];
   yar1++;
   kod=font[yar1];
   harf_adr=(kod*256)+yar2;
   yar1=yar1+3;
   yar2=font[yar1];
   yar1++;
   kod=font[yar1];
   harf_adrs=(kod*256)+yar2;

   tft_ili9325_cerceve(xpos,xpos+x2,ypos,ypos+y2);
   tft_ili9325_setcursor(xpos,ypos);
   LCD_WriteRAM_Prepare();

   for(dongu1=harf_adr;dongu1<harf_adrs;dongu1++){
      kod=font[dongu1];

      yar3=kod & 1;
      if(yar3==1){
    	  LCD_WriteRAM(rgb_code);
      }else{
    	  i=LCD_ReadRAM();
    	  LCD_WriteRAM(i);
      }
      yar3=kod & 2;
      if(yar3==2){
    	  LCD_WriteRAM(rgb_code);
      }else{
    	  i=LCD_ReadRAM();
    	  LCD_WriteRAM(i);
      }
      yar3=kod & 4;
      if(yar3==4){
    	  LCD_WriteRAM(rgb_code);
      }else{
    	  i=LCD_ReadRAM();
    	  LCD_WriteRAM(i);
      }
      yar3=kod & 8;
      if(yar3==8){
    	  LCD_WriteRAM(rgb_code);
      }else{
    	  i=LCD_ReadRAM();
    	  LCD_WriteRAM(i);
      }
      yar3=kod & 16;
      if(yar3==16){
    	  LCD_WriteRAM(rgb_code);
      }else{
    	  i=LCD_ReadRAM();
    	  LCD_WriteRAM(i);
      }
      yar3=kod & 32;
      if(yar3==32){
    	  LCD_WriteRAM(rgb_code);
      }else{
    	  i=LCD_ReadRAM();
    	  LCD_WriteRAM(i);
      }
      yar3=kod & 64;
      if(yar3==64){
    	  LCD_WriteRAM(rgb_code);
      }else{
    	  i=LCD_ReadRAM();
    	  LCD_WriteRAM(i);
      }
      yar3=kod & 128;
      if(yar3==128){
    	  LCD_WriteRAM(rgb_code);
      }else{
    	  i=LCD_ReadRAM();
    	  LCD_WriteRAM(i);
      }
   }
}


void tft_ili9325_yaziyaz(unsigned int xpos,unsigned int ypos,uint8_t txt[],uint8_t font[],unsigned int rgb_code){

   unsigned int yar1,yar2,dongu1,dongu_txt,kod,harf_adr,harf_adrs,i,x1,x2,x3,y2,len,yar3;
   char harf;

   y2=font[6];
   len = strlen(txt);

   for (dongu_txt=0; dongu_txt<len;dongu_txt++){

       harf=txt[dongu_txt];
       yar2=harf-32;
       yar1=(yar2*4)+8;
       x2=font[yar1];
       x3=x2+1;

       if (x2<=8){
          x2=7;
       }
       if (x2>8 & x2<=16){
          x2=15;
       }
       if (x2>16 & x2<=24){
          x2=23;
       }
       if (x2>24 & x2<=32){
          x2=31;
       }

       yar1++;
       yar2=font[yar1];
       yar1++;
       kod=font[yar1];
       harf_adr=(kod*256)+yar2;
       yar1=yar1+3;
       yar2=font[yar1];
       yar1++;
       kod=font[yar1];
       harf_adrs=(kod*256)+yar2;

       tft_ili9325_cerceve(xpos,(xpos+x2),ypos,(ypos+y2));
       tft_ili9325_setcursor(xpos,ypos);
       LCD_WriteRAM_Prepare();

       for(dongu1=harf_adr;dongu1<harf_adrs;dongu1++){
         kod=font[dongu1];
         yar3=kod & 1;
         if(yar3==1){
        	 LCD_WriteRAM(rgb_code);
         }else{
        	 i=LCD_ReadRAM();
        	 LCD_WriteRAM(i);
         }
         yar3=kod & 2;
         if(yar3==2){
             LCD_WriteRAM(rgb_code);
         }else{
             i=LCD_ReadRAM();
             LCD_WriteRAM(i);
         }
         yar3=kod & 4;
         if(yar3==4){
             LCD_WriteRAM(rgb_code);
         }else{
             i=LCD_ReadRAM();
             LCD_WriteRAM(i);
         }
         yar3=kod & 8;
         if(yar3==8){
             LCD_WriteRAM(rgb_code);
         }else{
             i=LCD_ReadRAM();
             LCD_WriteRAM(i);
         }
         yar3=kod & 16;
         if(yar3==16){
             LCD_WriteRAM(rgb_code);
         }else{
             i=LCD_ReadRAM();
             LCD_WriteRAM(i);
         }
         yar3=kod & 32;
         if(yar3==32){
        	  LCD_WriteRAM(rgb_code);
         }else{
             i=LCD_ReadRAM();
             LCD_WriteRAM(i);
         }
         yar3=kod & 64;
         if(yar3==64){
             LCD_WriteRAM(rgb_code);
         }else{
             i=LCD_ReadRAM();
             LCD_WriteRAM(i);
         }
         yar3=kod & 128;
         if(yar3==128){
        	 LCD_WriteRAM(rgb_code);
         }else{
        	 i=LCD_ReadRAM();
        	 LCD_WriteRAM(i);
         }
       }
      xpos=xpos+x3;
   }
}

void tft_ili9325_yaziyaztum(unsigned int xpos,unsigned int ypos,uint8_t txt[],uint8_t font[],unsigned int rgb_code ,unsigned int rgb_code1){

   unsigned int yar1,yar2,yar3,dongu1,dongu_txt,kod,harf_adr,harf_adrs,x1,x2,x3,y1,y2,len;
   char harf;

   y2=font[6];
   len = strlen(txt);

   for (dongu_txt=0; dongu_txt<len;dongu_txt++){

       harf=txt[dongu_txt];
       yar2=harf-32;
       yar1=(yar2*4)+8;
       x2=font[yar1];
       x3=x2+1;

       if (x2<=8){
          x2=7;
       }
       if (x2>8 & x2<=16){
          x2=15;
       }
       if (x2>16 & x2<=24){
          x2=23;
       }
       if (x2>24 & x2<=32){
          x2=31;
       }

       yar1++;
       yar2=font[yar1];
       yar1++;
       kod=font[yar1];
       harf_adr=(kod*256)+yar2;
       yar1=yar1+3;
       yar2=font[yar1];
       yar1++;
       kod=font[yar1];
       harf_adrs=(kod*256)+yar2;

       tft_ili9325_cerceve(xpos,xpos+x2,ypos,ypos+y2);
       tft_ili9325_setcursor(xpos,ypos);
       LCD_WriteRAM_Prepare();

       for(dongu1=harf_adr;dongu1<harf_adrs;dongu1++){
         kod=font[dongu1];
         yar3=kod & 1;
         if(yar3==1){
             LCD_WriteRAM(rgb_code);
         }else{
             LCD_WriteRAM(rgb_code1);
         }
         yar3=kod & 2;
         if(yar3==2){
             LCD_WriteRAM(rgb_code);
         }else{
             LCD_WriteRAM(rgb_code1);
         }
         yar3=kod & 4;
         if(yar3==4){
             LCD_WriteRAM(rgb_code);
         }else{
             LCD_WriteRAM(rgb_code1);
         }
         yar3=kod & 8;
         if(yar3==8){
             LCD_WriteRAM(rgb_code);
         }else{
             LCD_WriteRAM(rgb_code1);
         }
         yar3=kod & 16;
         if(yar3==16){
             LCD_WriteRAM(rgb_code);
         }else{
             LCD_WriteRAM(rgb_code1);
         }
         yar3=kod & 32;
         if(yar3==32){
             LCD_WriteRAM(rgb_code);
         }else{
             LCD_WriteRAM(rgb_code1);
         }
         yar3=kod & 64;
         if(yar3==64){
             LCD_WriteRAM(rgb_code);
         }else{
             LCD_WriteRAM(rgb_code1);
         }
         yar3=kod & 128;
         if(yar3==128){
             LCD_WriteRAM(rgb_code);
         }else{
        	 LCD_WriteRAM(rgb_code1);
         }
       }
      xpos=xpos+x3;
   }
}

void tft_ili9325_bmpciz(unsigned int xpos,unsigned int ypos,uint8_t resim[]){

    unsigned long dongu_bmp,sayici,x2,y2;
    unsigned int bmp_data;
    uint8_t low_data,high_data;

    low_data=resim[2];
    high_data=resim[3];
    y2=(high_data*256)+low_data;
    low_data=resim[4];
    high_data=resim[5];
    x2=(high_data*256)+low_data;
    sayici= (x2*y2)*2+6;

    tft_ili9325_cerceve(xpos,xpos+x2-1,ypos,ypos+y2-1);
    tft_ili9325_setcursor(xpos,ypos);
    LCD_WriteRAM_Prepare();

    for (dongu_bmp=6;dongu_bmp<sayici;dongu_bmp++){
       low_data=resim[dongu_bmp];
       dongu_bmp++;
       high_data=resim[dongu_bmp];
       bmp_data=(high_data*256)+low_data;
       LCD_WriteRAM(bmp_data++);
    }
}

void tft_ili9325_bmpciz_ozel(unsigned int xpos,unsigned int ypos,unsigned int sil_renk,unsigned int yaz_renk,unsigned int fon_renk,uint8_t resim[]){

    unsigned long dongu_bmp,sayici,x2,y2;
    unsigned int bmp_data,i;
    uint8_t low_data,high_data;

    low_data=resim[2];
    high_data=resim[3];
    y2=(high_data*256)+low_data;
    low_data=resim[4];
    high_data=resim[5];
    x2=(high_data*256)+low_data;
    sayici= (x2*y2)*2+6;

    tft_ili9325_cerceve(xpos,xpos+x2-1,ypos,ypos+y2-1);
    tft_ili9325_setcursor(xpos,ypos);
    LCD_WriteRAM_Prepare();

    for (dongu_bmp=6;dongu_bmp<sayici;dongu_bmp++){
       low_data=resim[dongu_bmp];
       dongu_bmp++;
       high_data=resim[dongu_bmp];
       bmp_data=(high_data*256)+low_data;
       if (bmp_data==fon_renk){
    	   i=LCD_ReadRAM();
    	   LCD_WriteRAM(i);
       }else if(bmp_data==sil_renk){
    	   i=LCD_ReadRAM();
    	   LCD_WriteRAM(yaz_renk);
       }else{
    	   LCD_WriteRAM(bmp_data);
       }
    }
}

void tft_ili9325_bmpciz_karakter(unsigned int xpos,unsigned int ypos,unsigned int fon_renk,unsigned int arka_renk,uint8_t resim[]){

    unsigned long dongu_bmp,sayici,x2,y2;
    uint16_t bmp_data,i;
    uint16_t okunan_data=0;
    uint8_t low_data,high_data;
    uint8_t hata=0;

    low_data=resim[2];
    high_data=resim[3];
    y2=(high_data*256)+low_data;
    low_data=resim[4];
    high_data=resim[5];
    x2=(high_data*256)+low_data;
    sayici= (x2*y2)*2+6;

    tft_ili9325_cerceve(xpos,xpos+x2-1,ypos,ypos+y2-1);
    tft_ili9325_setcursor(xpos,ypos);
    LCD_WriteRAM_Prepare();


    for (dongu_bmp=6;dongu_bmp<sayici;dongu_bmp++){
    	low_data=resim[dongu_bmp];
    	dongu_bmp++;
    	high_data=resim[dongu_bmp];
    	bmp_data=(high_data*256)+low_data;

    		hata=0;
    		okunan_data=LCD_ReadReg(LCD_REG_34);
    		if(LCD_ReadReg(LCD_REG_34)==duvar_renk){
    			LCD_WriteReg(LCD_REG_34,duvar_renk);
    			carpti=1;
    			hata=1;
    		}
    		if(LCD_ReadReg(LCD_REG_34)==zemin_renk){
    			LCD_WriteReg(LCD_REG_34,zemin_renk);
    			yesilde=1;
    			hata=1;
    			//return;
    		}
    		if(LCD_ReadReg(LCD_REG_34)==zipla_renk){
    			LCD_WriteReg(LCD_REG_34,zipla_renk);
    			zipla=1;
    			karakter_zipla=1;
    			hata=1;
    		}
    		if(LCD_ReadReg(LCD_REG_34)==puan_renk){
    			puan=1;
    			puan_kordinat_x=xpos;
    			puan_kordinat_y=ypos;
    		}
    		if(hata==0){
    	    	if (bmp_data==fon_renk){
    	    		LCD_WriteReg(LCD_REG_34,arka_renk);
    	    	}else{
    	    		if(LCD_ReadReg(LCD_REG_34)==dusman_renk){
    	    			LCD_WriteReg(LCD_REG_34,dusman_renk);
    	    			yandi=1;
    	    			//hata=1;
    	    			return;
    	    		}else{
    	    			LCD_WriteReg(LCD_REG_34, bmp_data);
    	    		}
    	    	}
    		}

    }
}

/*
void tft_ili9325_bmpciz_karakter(unsigned int xpos,unsigned int ypos,unsigned int fon_renk,unsigned int arka_renk,uint8_t resim[]){

    unsigned long dongu_bmp,sayici,x2,y2;
    uint16_t bmp_data,i;
    uint16_t okunan_data=0;
    uint8_t low_data,high_data;
    uint8_t hata=0;

    low_data=resim[2];
    high_data=resim[3];
    y2=(high_data*256)+low_data;
    low_data=resim[4];
    high_data=resim[5];
    x2=(high_data*256)+low_data;
    sayici= (x2*y2)*2+6;

    tft_ili9325_cerceve(xpos,xpos+x2-1,ypos,ypos+y2-1);
    tft_ili9325_setcursor(xpos,ypos);
    LCD_WriteRAM_Prepare();


    for (dongu_bmp=6;dongu_bmp<sayici;dongu_bmp++){
    	low_data=resim[dongu_bmp];
    	dongu_bmp++;
    	high_data=resim[dongu_bmp];
    	bmp_data=(high_data*256)+low_data;

    		hata=0;
    		okunan_data=LCD_ReadReg(LCD_REG_34);
    		if(LCD_ReadReg(LCD_REG_34)==duvar_renk){
    			LCD_WriteReg(LCD_REG_34,duvar_renk);
    			carpti=1;
    			hata=1;
    		}
    		if(LCD_ReadReg(LCD_REG_34)==zemin_renk){
    			LCD_WriteReg(LCD_REG_34,zemin_renk);
    			yesilde=1;
    			hata=1;
    			//return;
    		}
    		if(LCD_ReadReg(LCD_REG_34)==zipla_renk){
    			LCD_WriteReg(LCD_REG_34,zipla_renk);
    			zipla=1;
    			karakter_zipla=1;
    			hata=1;
    		}
    		if(LCD_ReadReg(LCD_REG_34)==puan_renk){
    			puan=1;
    			puan_kordinat_x=xpos;
    			puan_kordinat_y=ypos;
    		}
    		if(hata==0){
    	    	if (bmp_data==fon_renk){
    	    		LCD_WriteReg(LCD_REG_34,arka_renk);
    	    	}else{
    	    		if(LCD_ReadReg(LCD_REG_34)==dusman_renk){
    	    			LCD_WriteReg(LCD_REG_34,dusman_renk);
    	    			yandi=1;
    	    			//hata=1;
    	    			return;
    	    		}else{
    	    			LCD_WriteReg(LCD_REG_34, bmp_data);
    	    		}
    	    	}
    		}

    }
}

*/




/**
  * @brief  Writes to the selected LCD register.
  * @param  LCD_Reg: address of the selected register.
  * @param  LCD_RegValue: value to write to the selected register.
  * @retval None
  */
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue){
  /* Write 16-bit Index, then Write Reg */
  LCD->LCD_REG = LCD_Reg;
  /* Write 16-bit Reg */
  LCD->LCD_RAM = LCD_RegValue;

}
/**
  * @brief  Reads the selected LCD Register.
  * @param  LCD_Reg: address of the selected register.
  * @retval LCD Register Value.
  */
uint16_t LCD_ReadReg(uint8_t LCD_Reg){
  /* Write 16-bit Index (then Read Reg) */
  LCD->LCD_REG = LCD_Reg;
  /* Read 16-bit Reg */
  return (LCD->LCD_RAM);
}
/**
  * @brief  Prepare to write to the LCD RAM.
  * @param  None
  * @retval None
  */
void LCD_WriteRAM_Prepare(void){
  LCD->LCD_REG = LCD_REG_34;
}
/**
  * @brief  Writes to the LCD RAM.
  * @param  RGB_Code: the pixel color in RGB mode (5-6-5).
  * @retval None
  */
void LCD_WriteRAM(uint16_t RGB_Code){
  /* Write 16-bit GRAM Reg */
  LCD->LCD_RAM = RGB_Code;
}
/**
  * @brief  Reads the LCD RAM.
  * @param  None
  * @retval LCD RAM Value.
  */
uint16_t LCD_ReadRAM(void){
  /* Write 16-bit Index (then Read Reg) */
  LCD->LCD_REG = LCD_REG_34; /* Select GRAM Reg */
  /* Read 16-bit Reg */
  return LCD->LCD_RAM;
}

/**
  * @brief  Power on the LCD.
  * @param  None
  * @retval None
  */
void LCD_PowerOn(void){
/* Power On sequence ---------------------------------------------------------*/
  LCD_WriteReg(LCD_REG_16, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  LCD_WriteReg(LCD_REG_17, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
  LCD_WriteReg(LCD_REG_18, 0x0000); /* VREG1OUT voltage */
  LCD_WriteReg(LCD_REG_19, 0x0000); /* VDV[4:0] for VCOM amplitude*/
  HAL_Delay(200);                 /* Dis-charge capacitor power voltage (200ms) */
  LCD_WriteReg(LCD_REG_16, 0x17B0); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  LCD_WriteReg(LCD_REG_17, 0x0137); /* DC1[2:0], DC0[2:0], VC[2:0] */
  HAL_Delay(50);                  /* Delay 50 ms */
  LCD_WriteReg(LCD_REG_18, 0x0139); /* VREG1OUT voltage */
  HAL_Delay(50);               /* Delay 50 ms */
  LCD_WriteReg(LCD_REG_19, 0x1d00); /* VDV[4:0] for VCOM amplitude */
  LCD_WriteReg(LCD_REG_41, 0x0013); /* VCM[4:0] for VCOMH */
  HAL_Delay(50);                  /* Delay 50 ms */
  LCD_WriteReg(LCD_REG_7, 0x0173);  /* 262K color and display ON */
}

/**
  * @brief  Enables the Display.
  * @param  None
  * @retval None
  */
void LCD_DisplayOn(void){
  /* Display On */
  LCD_WriteReg(LCD_REG_7, 0x0173); /* 262K color and display ON */
}

/**
  * @brief  Disables the Display.
  * @param  None
  * @retval None
  */
void LCD_DisplayOff(void){
  /* Display Off */
  LCD_WriteReg(LCD_REG_7, 0x0);
}

void LCD_CtrlLinesConfig(void){
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();

    GPIO_InitStruct.Pin = D2_Pin | D3_Pin | FSMC_NOE_Pin | FSMC_NWE_Pin | D13_Pin | D14_Pin | D15_Pin | D0_Pin | D1_Pin;
    //GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = D4_Pin | D5_Pin |  D6_Pin | D7_Pin | D8_Pin | D9_Pin | D10_Pin | D11_Pin | D12_Pin;
    //GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = A0_Pin;
    //GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = FSMC_NE3_Pin;
    //GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

}

/**
  * @brief  Configures the Parallel interface (FSMC) for LCD(Parallel mode)
  * @param  None
  * @retval None
  */
void LCD_FSMCConfig(void){


	FSMC_NORSRAM_TimingTypeDef Timing;

	  /** Perform the SRAM3 memory initialization sequence
	  */
	  hsram3.Instance = FSMC_NORSRAM_DEVICE;
	  hsram3.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	  /* hsram3.Init */
	  hsram3.Init.NSBank = FSMC_NORSRAM_BANK3;
	  hsram3.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
	  hsram3.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
	  hsram3.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
	  hsram3.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
	  hsram3.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	  hsram3.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
	  hsram3.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
	  hsram3.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	  hsram3.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
	  hsram3.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
	  hsram3.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	  hsram3.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
	  hsram3.Init.PageSize = FSMC_PAGE_SIZE_NONE;
	  /* Timing */
	  Timing.AddressSetupTime = 5;
	  Timing.AddressHoldTime = 0;
	  Timing.DataSetupTime = 9;
	  Timing.BusTurnAroundDuration = 0;
	  Timing.CLKDivision = 0;
	  Timing.DataLatency = 0;
	  Timing.AccessMode = FSMC_ACCESS_MODE_A;


	  if (HAL_SRAM_Init(&hsram3, &Timing, NULL) != HAL_OK){
	      _Error_Handler(__FILE__, __LINE__);
	   }

}
