/*
 * oyun_motoru.c
 *
 *  Created on: 8 Ara 2020
 *      Author: Tolga
 */

#include "main.h"
#include "stm32f4xx_hal.h"
#include "tol_tft.h"
#include "string.h"
#include "Tahoma10x11.h"
#include "Tahoma18x16.h"
#include "Tahoma24x24.h"
#include "resimler/duvar.h"
#include "resimler/cop.h"
#include "resimler/can.h"
#include "resimler/cit.h"
#include "resimler/kaya.h"
#include "resimler/kaygan.h"
#include "resimler/ziplama.h"
#include "resimler/puan1.h"
#include "resimler/puan2.h"
#include "resimler/puan3.h"
#include "resimler/kedi11.h"
#include "resimler/kedi12.h"
#include "resimler/kedi13.h"
#include "resimler/kedi14.h"
#include "resimler/kedi15.h"
#include "resimler/kedi0.h"
#include "resimler/kedi21.h"
#include "resimler/kedi22.h"
#include "resimler/kedi23.h"
#include "resimler/kedi24.h"
#include "resimler/kedi25.h"
#include "resimler/kopek10.h"
#include "resimler/kopek11.h"
#include "resimler/kopek12.h"
#include "resimler/kopek13.h"
#include "resimler/kopek20.h"
#include "resimler/kopek21.h"
#include "resimler/kopek22.h"
#include "resimler/kopek23.h"


static void ekran_ciz(uint16_t karakter_x , uint16_t karakter_y);
static void yandi_animasyon (uint16_t karakter_x , uint16_t karakter_y);
static void dusman_olustur(uint16_t kordinat_x , uint16_t kordinat_y,uint8_t dusman_sayi);
static void kuyu_olustur(uint16_t kordinat_x , uint16_t kordinat_y, uint8_t kuyu_en);
static void yapi_olustur(uint16_t x_baslangic , uint16_t y_baslangic , uint8_t adet);
static void cop_olustur(uint16_t x_baslangic , uint16_t y_baslangic);
static void cit_olustur(uint16_t x_baslangic , uint16_t y_baslangic);
static void kaya_olustur(uint16_t x_baslangic , uint16_t y_baslangic);
static void kaygan_olustur(uint16_t x_baslangic , uint16_t y_baslangic);
static void ziplama_olustur(uint16_t x_baslangic , uint16_t y_baslangic);
static void puan_olustur(uint16_t kordinat_x , uint16_t kordinat_y,uint8_t puan_tip);


uint8_t karakter_zipla=0;
uint8_t karakter_yon=0;
uint8_t carpti=0;
uint8_t yandi=0;
uint8_t yesilde=0;
uint8_t puan=0;
uint8_t zipla=0;
uint8_t bolum=1;
uint8_t yukari_asagi=0;
uint8_t ziplama_sayici=0;
uint8_t dusman_hareket=0;
uint8_t dusman_hareket_sayici=0;
uint8_t dusman_hiz=0;
uint8_t dusman_hiz_sayici=0;
uint8_t dusman_hareket_yon=0;
uint8_t dusman_var=0;
uint8_t can=9;
uint8_t gosterge=0;
uint8_t dusman_sol_sayici=0;
uint8_t dusman_sag_sayici=0;
uint8_t karakter_sol_sayici=0;
uint8_t karakter_sag_sayici=0;
uint8_t zipla_rakam=60;


uint16_t karakter_kordinat_x=5;
uint16_t karakter_kordinat_y=154;
uint16_t arkaplan_renk=renk_turkuaz;
uint16_t karakter_renk=renk_mor;
uint16_t zemin_renk=renk_koyuyesil;
uint16_t duvar_renk=renk_kahverengi;
uint16_t dusman_renk=renk_kirmizi;
uint16_t puan_renk=renk_sari;
uint16_t zipla_renk=renk_mor;
uint16_t yazi_renk=renk_siyah;
uint16_t puan_kordinat_x=0;
uint16_t puan_kordinat_y=0;
uint16_t oyun_puan=0;

uint16_t dusman_kordinat[11];

char txt[25];


void oyun_dongu(void){

	//------------------------------------------------------------------
	if(karakter_zipla==1){
		 if(yukari_asagi==0 && ziplama_sayici==0) yukari_asagi=1;
		 if(yukari_asagi==1 && ziplama_sayici>=zipla_rakam) yukari_asagi=2;
		 if(yukari_asagi==1){
		  	ziplama_sayici++;
		  	karakter_kordinat_y--;
		 }
		 if(yukari_asagi==2){
		  	ziplama_sayici--;
		  	karakter_kordinat_y++;
		 }
	}else{
		 ziplama_sayici=0;
		 yukari_asagi=0;
	}
	// ----------------------------------------------------------------
	if(karakter_kordinat_x<5) karakter_kordinat_x=5;  // önceki bölüme geçmesin istemiyorsan bunu aktif et
	/*
	if(karakter_kordinat_x<5){						// önceki bölüme geçsin istiyorsan bunu aktif et
		karakter_kordinat_x=286;
		bolum--;
		if(bolum==0){
		  	bolum=1;
		  	karakter_kordinat_x=5;
		}
		ekran_olustur(karakter_kordinat_x,karakter_kordinat_y);
	}
	*/
	if(karakter_kordinat_x>286){
		karakter_kordinat_x=5;
		bolum++;
		oyun_puan=oyun_puan+100;
		ekran_olustur(karakter_kordinat_x,karakter_kordinat_y);
	}
	// ----------------------------------------------------------------
	ekran_ciz(karakter_kordinat_x,karakter_kordinat_y);
	// ----------------------------------------------------------------
	if(yandi==1){
		yandi_animasyon(karakter_kordinat_x,karakter_kordinat_y);
		karakter_kordinat_x=5;
		karakter_kordinat_y=50;
		karakter_zipla=0;
		karakter_yon=0;
		ziplama_sayici=0;
		karakter_yon=0;
		yukari_asagi=0;
		dusman_hareket_sayici=0;
		dusman_hiz_sayici=0;
		dusman_hareket_yon=0;
		can--;
		if(can==0){
			HAL_Delay(1000);
			while(1){
				tft_ili9325_dolukare(0,319,0,239,renk_siyah);
		  		HAL_Delay(500);
		  		tft_ili9325_yaziyaztum(90,100,"OYUN      BiTTi",Tahoma24x24,renk_beyaz,renk_siyah);
		  		HAL_Delay(1000);
		  	}
		}
	  	ekran_olustur(karakter_kordinat_x,karakter_kordinat_y);
	}
	if(yesilde==1){
		zipla_rakam=60;
		if(yukari_asagi==2){
			karakter_zipla=0;
			ziplama_sayici=0;
			yukari_asagi=0;
		  	karakter_kordinat_y--;
		}
	}else{
		if(yukari_asagi==0){
			karakter_kordinat_y++;
			yukari_asagi=2;
		}
	}
	if(carpti==1){
		yukari_asagi=2;
		if(karakter_yon==0)  karakter_kordinat_x=karakter_kordinat_x-1;
		if(karakter_yon==1)  karakter_kordinat_x=karakter_kordinat_x-1;
		if(karakter_yon==2)  karakter_kordinat_x=karakter_kordinat_x+1;;
	}
	if(puan==1){
		oyun_puan=oyun_puan+10;
		gosterge=1;
	}
	if(zipla==1){
		zipla_rakam=100;
	}

}



//-------------------------------------------------------------------------------
void ekran_olustur(uint16_t karakter_x , uint16_t karakter_y){


	yesilde=0;
	carpti=0;
	yandi=0;
	zipla=0;
	dusman_var=0;
	dusman_hareket_sayici=0;
	dusman_hiz_sayici=0;
	dusman_hareket_yon=0;

	for(uint16_t i=0;i<10;i++){
		dusman_kordinat[i]=0;
	}

	tft_ili9325_dolukare(0,319,0,239,arkaplan_renk);
	yapi_olustur(0,219,17);
	yapi_olustur(0,200,17);
	yapi_olustur(0,181,17);


	itoa(oyun_puan,txt,10);
	tft_ili9325_yaziyaztum(10,5,"Puan: ",Tahoma10x11,yazi_renk,arkaplan_renk);
	tft_ili9325_yaziyaztum(40,5,txt,Tahoma10x11,yazi_renk,arkaplan_renk);
	itoa(bolum,txt,10);
	tft_ili9325_yaziyaztum(100,5,"Bölüm: ",Tahoma10x11,yazi_renk,arkaplan_renk);
	tft_ili9325_yaziyaztum(140,5,txt,Tahoma10x11,yazi_renk,arkaplan_renk);
	//itoa(can,txt,10);
	//tft_ili9325_yaziyaztum(170,5,"Can: ",Tahoma10x11,yazi_renk,arkaplan_renk);
	//tft_ili9325_yaziyaztum(200,5,txt,Tahoma10x11,yazi_renk,arkaplan_renk);
	for(uint8_t i=0;i<can;i++){
		tft_ili9325_bmpciz_ozel((170+(i*15)),3,renk_beyaz,arkaplan_renk,arkaplan_renk,can_bmp);
	}

	tft_ili9325_yataycizgi(1,20,317,duvar_renk);
	tft_ili9325_yataycizgi(1,21,317,duvar_renk);

	if(bolum==1){
		yapi_olustur(120,164,6);
		yapi_olustur(160,145,4);
		yapi_olustur(200,126,2);
		puan_olustur(130,150,2);
	}
	if(bolum==2){
		puan_olustur(75,167,2);
		cit_olustur(105,136);
		dusman_olustur(195,150,1);
		cit_olustur(250,136);
		dusman_hareket=35;
		dusman_hiz=6;
	}
	if(bolum==3){
		kuyu_olustur(110,181,50);
		puan_olustur(120,130,3);
		kaya_olustur(160,160);
		dusman_olustur(270,150,1);
		dusman_hareket=80;
		dusman_hiz=4;
	}
	if(bolum==4){
		ziplama_olustur(78,171);
		kuyu_olustur(100,181,219);
		yapi_olustur(220,130,1);
		yapi_olustur(250,80,1);
		yapi_olustur(290,145,1);
	}
	if(bolum==5){
		yapi_olustur(80,125,8);
		dusman_olustur(180,94,1);
		dusman_olustur(140,150,2);
		puan_olustur(90,75,3);
		puan_olustur(180,50,3);
		puan_olustur(240,45,3);
		puan_olustur(225,165,2);
		dusman_hareket=80;
		dusman_hiz=3;
	}
	if(bolum==6){
		kuyu_olustur(40,181,45);
		puan_olustur(70,105,3);
		kuyu_olustur(100,181,50);
		puan_olustur(130,110,3);
		kuyu_olustur(185,181,55);
		puan_olustur(200,115,3);
		dusman_olustur(275,152,0);
	}
	if(bolum==7){
		yapi_olustur(60,162,12);
		yapi_olustur(90,143,10);
		yapi_olustur(120,124,8);
		yapi_olustur(150,105,6);
		kuyu_olustur(180,105,60);
		puan_olustur(190,55,3);
		puan_olustur(210,35,3);
		puan_olustur(235,55,3);
	}
	if(bolum==8){
		puan_olustur(60,168,1);
		yapi_olustur(60,133,1);
		yapi_olustur(120,90,1);
		dusman_olustur(123,150,1);
		puan_olustur(175,166,2);
		puan_olustur(210,120,1);
		cop_olustur(200,133);
		dusman_olustur(270,150,2);
		dusman_hareket=28;
		dusman_hiz=10;
	}
	if(bolum==9){
		ziplama_olustur(50,171);
		kuyu_olustur(75,181,65);
		puan_olustur(100,80,3);
		kaygan_olustur(140,170);
		dusman_olustur(250,150,1);
		puan_olustur(300,168,1);
		dusman_hareket=50;
		dusman_hiz=5;
	}
	if(bolum==10){
		yapi_olustur(40,85,1);
		yapi_olustur(40,105,1);
		yapi_olustur(40,125,7);
		yapi_olustur(230,128,4);
		dusman_olustur(163,93,1);
		dusman_olustur(270,150,2);
		puan_olustur(120,60,3);
		puan_olustur(250,113,2);
		dusman_hareket=100;
		dusman_hiz=3;
	}

	tft_ili9325_bmpciz_karakter(karakter_x,karakter_y,renk_beyaz,arkaplan_renk,kedi21_bmp);

}
//-------------------------------------------------------------------------------
static void ekran_ciz(uint16_t karakter_x , uint16_t karakter_y){

	 yesilde=0;
	 carpti=0;
	 yandi=0;
	 puan=0;
	 zipla=0;

	 if(gosterge==1){
		 gosterge=0;
		 tft_ili9325_dolukare(1,300,5,25,arkaplan_renk);
		 itoa(oyun_puan,txt,10);
		 tft_ili9325_yaziyaztum(10,5,"Puan: ",Tahoma10x11,yazi_renk,arkaplan_renk);
		 tft_ili9325_yaziyaztum(40,5,txt,Tahoma10x11,yazi_renk,arkaplan_renk);
		 itoa(bolum,txt,10);
		 tft_ili9325_yaziyaztum(100,5,"Bölüm: ",Tahoma10x11,yazi_renk,arkaplan_renk);
		 tft_ili9325_yaziyaztum(140,5,txt,Tahoma10x11,yazi_renk,arkaplan_renk);
		 //itoa(can,txt,10);
		 //tft_ili9325_yaziyaztum(170,5,"Can: ",Tahoma10x11,yazi_renk,arkaplan_renk);
		 //tft_ili9325_yaziyaztum(200,5,txt,Tahoma10x11,yazi_renk,arkaplan_renk);
		 for(uint8_t i=0;i<can;i++){
			tft_ili9325_bmpciz_ozel((170+(i*15)),3,renk_beyaz,arkaplan_renk,arkaplan_renk,can_bmp);
		 }

		 tft_ili9325_yataycizgi(1,20,317,duvar_renk);
		 tft_ili9325_yataycizgi(1,21,317,duvar_renk);
	 }

	 if(yukari_asagi==0){
		 if(karakter_yon==0){
			 tft_ili9325_bmpciz_karakter(karakter_x,karakter_y,renk_beyaz,arkaplan_renk,kedi21_bmp);
		 }else{
			 karakter_sol_sayici++;
			 if(karakter_sol_sayici>=15) karakter_sol_sayici=0;
			 karakter_sag_sayici++;
			 if(karakter_sag_sayici>=15) karakter_sag_sayici=0;

			 if(karakter_yon==1){
				 if(karakter_sag_sayici>=0 && karakter_sag_sayici<=4) tft_ili9325_bmpciz_karakter(karakter_x,karakter_y,renk_beyaz,arkaplan_renk,kedi21_bmp);
				 if(karakter_sag_sayici>=5 && karakter_sag_sayici<=9) tft_ili9325_bmpciz_karakter(karakter_x,karakter_y,renk_beyaz,arkaplan_renk,kedi22_bmp);
				 if(karakter_sag_sayici>=10 && karakter_sag_sayici<=14) tft_ili9325_bmpciz_karakter(karakter_x,karakter_y,renk_beyaz,arkaplan_renk,kedi23_bmp);
			 }
			 if(karakter_yon==2){
				 if(karakter_sol_sayici>=0 && karakter_sol_sayici<=4) tft_ili9325_bmpciz_karakter(karakter_x,karakter_y,renk_beyaz,arkaplan_renk,kedi11_bmp);
				 if(karakter_sol_sayici>=5 && karakter_sol_sayici<=9) tft_ili9325_bmpciz_karakter(karakter_x,karakter_y,renk_beyaz,arkaplan_renk,kedi12_bmp);
				 if(karakter_sol_sayici>=10 && karakter_sol_sayici<=14) tft_ili9325_bmpciz_karakter(karakter_x,karakter_y,renk_beyaz,arkaplan_renk,kedi13_bmp);
			 }
		 }
	 }
	 if(yukari_asagi==1){
		 if(karakter_yon==0){
	 		 tft_ili9325_bmpciz_karakter(karakter_x,karakter_y,renk_beyaz,arkaplan_renk,kedi24_bmp);
		 }
		 if(karakter_yon==1){
	 		 tft_ili9325_bmpciz_karakter(karakter_x,karakter_y,renk_beyaz,arkaplan_renk,kedi24_bmp);
		 }
		 if(karakter_yon==2){
	 		 tft_ili9325_bmpciz_karakter(karakter_x,karakter_y,renk_beyaz,arkaplan_renk,kedi14_bmp);
		 }
	 }
	 if(yukari_asagi==2){
		 if(karakter_yon==0){
	 		 tft_ili9325_bmpciz_karakter(karakter_x,karakter_y,renk_beyaz,arkaplan_renk,kedi25_bmp);
		 }
		 if(karakter_yon==1){
	 		 tft_ili9325_bmpciz_karakter(karakter_x,karakter_y,renk_beyaz,arkaplan_renk,kedi25_bmp);
		 }
		 if(karakter_yon==2){
	 		 tft_ili9325_bmpciz_karakter(karakter_x,karakter_y,renk_beyaz,arkaplan_renk,kedi15_bmp);
		 }
	 }


	 if(dusman_var==1){
		 dusman_hiz_sayici++;
		 if(dusman_hiz_sayici==dusman_hiz){
			 dusman_hiz_sayici=0;
			 dusman_hareket_sayici++;
			 if(dusman_hareket_sayici>=dusman_hareket){
				 dusman_hareket_sayici=0;
				 dusman_hareket_yon++;
				 if(dusman_hareket_yon==2) dusman_hareket_yon=0;
			 }
			 dusman_sol_sayici++;
			 if(dusman_sol_sayici>=9) dusman_sol_sayici=0;
			 dusman_sag_sayici++;
			 if(dusman_sag_sayici>=9) dusman_sag_sayici=0;

			 if(dusman_kordinat[0]>0 && dusman_kordinat[1]>0){
				 if(dusman_hareket_yon==0){
					 dusman_kordinat[0]--;
				 	 if(dusman_sol_sayici==0){
				 		 tft_ili9325_dolukare(dusman_kordinat[0],dusman_kordinat[0]+47,dusman_kordinat[1],dusman_kordinat[1]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[0],dusman_kordinat[1],renk_siyah,dusman_renk,renk_beyaz,kopek11_bmp);
				 	 }
				 	 if(dusman_sol_sayici==3){
				 		 tft_ili9325_dolukare(dusman_kordinat[0],dusman_kordinat[0]+47,dusman_kordinat[1],dusman_kordinat[1]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[0],dusman_kordinat[1],renk_siyah,dusman_renk,renk_beyaz,kopek12_bmp);
				 	 }
				 	 if(dusman_sol_sayici==6){
				 		 tft_ili9325_dolukare(dusman_kordinat[0],dusman_kordinat[0]+47,dusman_kordinat[1],dusman_kordinat[1]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[0],dusman_kordinat[1],renk_siyah,dusman_renk,renk_beyaz,kopek13_bmp);
				 	 }
				 }
				 if(dusman_hareket_yon==1){
					 dusman_kordinat[0]++;
				 	 if(dusman_sag_sayici==0){
				 		 tft_ili9325_dolukare(dusman_kordinat[0]-3,dusman_kordinat[0]+47,dusman_kordinat[1],dusman_kordinat[1]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[0],dusman_kordinat[1],renk_siyah,dusman_renk,renk_beyaz,kopek21_bmp);
				 	 }
				 	 if(dusman_sag_sayici==3){
				 		 tft_ili9325_dolukare(dusman_kordinat[0]-3,dusman_kordinat[0]+47,dusman_kordinat[1],dusman_kordinat[1]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[0],dusman_kordinat[1],renk_siyah,dusman_renk,renk_beyaz,kopek22_bmp);
				 	 }
				 	 if(dusman_sag_sayici==6){
				 		 tft_ili9325_dolukare(dusman_kordinat[0]-3,dusman_kordinat[0]+47,dusman_kordinat[1],dusman_kordinat[1]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[0],dusman_kordinat[1],renk_siyah,dusman_renk,renk_beyaz,kopek23_bmp);
				 	 }
				 }
			 }
			 if(dusman_kordinat[2]>0 && dusman_kordinat[3]>0){
				 if(dusman_hareket_yon==0){
					 dusman_kordinat[2]--;
				 	 if(dusman_sol_sayici==0){
				 		 tft_ili9325_dolukare(dusman_kordinat[2],dusman_kordinat[2]+47,dusman_kordinat[3],dusman_kordinat[3]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[2],dusman_kordinat[3],renk_siyah,dusman_renk,renk_beyaz,kopek11_bmp);
				 	 }
				 	 if(dusman_sol_sayici==3){
				 		 tft_ili9325_dolukare(dusman_kordinat[2],dusman_kordinat[2]+47,dusman_kordinat[3],dusman_kordinat[3]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[2],dusman_kordinat[3],renk_siyah,dusman_renk,renk_beyaz,kopek12_bmp);
				 	 }
				 	 if(dusman_sol_sayici==6){
				 		 tft_ili9325_dolukare(dusman_kordinat[2],dusman_kordinat[2]+47,dusman_kordinat[3],dusman_kordinat[3]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[2],dusman_kordinat[3],renk_siyah,dusman_renk,renk_beyaz,kopek13_bmp);
				 	 }
				 }
				 if(dusman_hareket_yon==1){
					 dusman_kordinat[2]++;
				 	 if(dusman_sag_sayici==0){
				 		 tft_ili9325_dolukare(dusman_kordinat[2]-3,dusman_kordinat[2]+47,dusman_kordinat[3],dusman_kordinat[3]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[2],dusman_kordinat[3],renk_siyah,dusman_renk,renk_beyaz,kopek21_bmp);
				 	 }
				 	 if(dusman_sag_sayici==3){
				 		 tft_ili9325_dolukare(dusman_kordinat[2]-3,dusman_kordinat[2]+47,dusman_kordinat[3],dusman_kordinat[3]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[2],dusman_kordinat[3],renk_siyah,dusman_renk,renk_beyaz,kopek22_bmp);
				 	 }
				 	 if(dusman_sag_sayici==6){
				 		 tft_ili9325_dolukare(dusman_kordinat[2]-3,dusman_kordinat[2]+47,dusman_kordinat[3],dusman_kordinat[3]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[2],dusman_kordinat[3],renk_siyah,dusman_renk,renk_beyaz,kopek23_bmp);
				 	 }
				 }
			 }
			 if(dusman_kordinat[4]>0 && dusman_kordinat[5]>0){
				 if(dusman_hareket_yon==0){
					 dusman_kordinat[4]--;
				 	 if(dusman_sol_sayici==0){
				 		 tft_ili9325_dolukare(dusman_kordinat[4],dusman_kordinat[4]+47,dusman_kordinat[5],dusman_kordinat[5]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[4],dusman_kordinat[5],renk_siyah,dusman_renk,renk_beyaz,kopek11_bmp);
				 	 }
				 	 if(dusman_sol_sayici==3){
				 		 tft_ili9325_dolukare(dusman_kordinat[4],dusman_kordinat[4]+47,dusman_kordinat[5],dusman_kordinat[5]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[4],dusman_kordinat[5],renk_siyah,dusman_renk,renk_beyaz,kopek12_bmp);
				 	 }
				 	 if(dusman_sol_sayici==6){
				 		 tft_ili9325_dolukare(dusman_kordinat[4],dusman_kordinat[4]+47,dusman_kordinat[5],dusman_kordinat[5]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[4],dusman_kordinat[5],renk_siyah,dusman_renk,renk_beyaz,kopek13_bmp);
				 	 }
				 }
				 if(dusman_hareket_yon==1){
					 dusman_kordinat[4]++;
				 	 if(dusman_sag_sayici==0){
				 		 tft_ili9325_dolukare(dusman_kordinat[4]-3,dusman_kordinat[4]+47,dusman_kordinat[5],dusman_kordinat[5]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[4],dusman_kordinat[5],renk_siyah,dusman_renk,renk_beyaz,kopek21_bmp);
				 	 }
				 	 if(dusman_sag_sayici==3){
				 		 tft_ili9325_dolukare(dusman_kordinat[4]-3,dusman_kordinat[4]+47,dusman_kordinat[5],dusman_kordinat[5]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[4],dusman_kordinat[5],renk_siyah,dusman_renk,renk_beyaz,kopek22_bmp);
				 	 }
				 	 if(dusman_sag_sayici==6){
				 		 tft_ili9325_dolukare(dusman_kordinat[4]-3,dusman_kordinat[4]+47,dusman_kordinat[5],dusman_kordinat[5]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[4],dusman_kordinat[5],renk_siyah,dusman_renk,renk_beyaz,kopek23_bmp);
				 	 }
				 }
			 }
			 if(dusman_kordinat[6]>0 && dusman_kordinat[7]>0){
				 if(dusman_hareket_yon==0){
					 dusman_kordinat[6]--;
				 	 if(dusman_sol_sayici==0){
				 		 tft_ili9325_dolukare(dusman_kordinat[6],dusman_kordinat[6]+47,dusman_kordinat[7],dusman_kordinat[7]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[6],dusman_kordinat[7],renk_siyah,dusman_renk,renk_beyaz,kopek11_bmp);
				 	 }
				 	 if(dusman_sol_sayici==3){
				 		 tft_ili9325_dolukare(dusman_kordinat[6],dusman_kordinat[6]+47,dusman_kordinat[7],dusman_kordinat[7]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[6],dusman_kordinat[7],renk_siyah,dusman_renk,renk_beyaz,kopek12_bmp);
				 	 }
				 	 if(dusman_sol_sayici==6){
				 		 tft_ili9325_dolukare(dusman_kordinat[6],dusman_kordinat[6]+47,dusman_kordinat[7],dusman_kordinat[7]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[6],dusman_kordinat[7],renk_siyah,dusman_renk,renk_beyaz,kopek13_bmp);
				 	 }
				 }
				 if(dusman_hareket_yon==1){
					 dusman_kordinat[6]++;
				 	 if(dusman_sag_sayici==0){
				 		 tft_ili9325_dolukare(dusman_kordinat[6]-3,dusman_kordinat[6]+47,dusman_kordinat[7],dusman_kordinat[7]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[6],dusman_kordinat[7],renk_siyah,dusman_renk,renk_beyaz,kopek21_bmp);
				 	 }
				 	 if(dusman_sag_sayici==3){
				 		 tft_ili9325_dolukare(dusman_kordinat[6]-3,dusman_kordinat[6]+47,dusman_kordinat[7],dusman_kordinat[7]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[6],dusman_kordinat[7],renk_siyah,dusman_renk,renk_beyaz,kopek22_bmp);
				 	 }
				 	 if(dusman_sag_sayici==6){
				 		 tft_ili9325_dolukare(dusman_kordinat[6]-3,dusman_kordinat[6]+47,dusman_kordinat[7],dusman_kordinat[7]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[6],dusman_kordinat[7],renk_siyah,dusman_renk,renk_beyaz,kopek23_bmp);
				 	 }
				 }
			 }
			 if(dusman_kordinat[8]>0 && dusman_kordinat[9]>0){
				 if(dusman_hareket_yon==0){
					 dusman_kordinat[8]--;;
				 	 if(dusman_sol_sayici==0){
				 		 tft_ili9325_dolukare(dusman_kordinat[8],dusman_kordinat[8]+47,dusman_kordinat[9],dusman_kordinat[9]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[8],dusman_kordinat[9],renk_siyah,dusman_renk,renk_beyaz,kopek11_bmp);
				 	 }
				 	 if(dusman_sol_sayici==3){
				 		 tft_ili9325_dolukare(dusman_kordinat[8],dusman_kordinat[8]+47,dusman_kordinat[9],dusman_kordinat[9]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[8],dusman_kordinat[9],renk_siyah,dusman_renk,renk_beyaz,kopek12_bmp);
				 	 }
				 	 if(dusman_sol_sayici==6){
				 		 tft_ili9325_dolukare(dusman_kordinat[8],dusman_kordinat[8]+47,dusman_kordinat[9],dusman_kordinat[9]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[8],dusman_kordinat[9],renk_siyah,dusman_renk,renk_beyaz,kopek13_bmp);
				 	 }
				 }
				 if(dusman_hareket_yon==1){
					 dusman_kordinat[8]++;
				 	 if(dusman_sag_sayici==0){
				 		 tft_ili9325_dolukare(dusman_kordinat[8]-3,dusman_kordinat[8]+47,dusman_kordinat[9],dusman_kordinat[9]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[8],dusman_kordinat[9],renk_siyah,dusman_renk,renk_beyaz,kopek21_bmp);
				 	 }
				 	 if(dusman_sag_sayici==3){
				 		 tft_ili9325_dolukare(dusman_kordinat[8]-3,dusman_kordinat[8]+47,dusman_kordinat[9],dusman_kordinat[9]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[8],dusman_kordinat[9],renk_siyah,dusman_renk,renk_beyaz,kopek22_bmp);
				 	 }
				 	 if(dusman_sag_sayici==6){
				 		 tft_ili9325_dolukare(dusman_kordinat[8]-3,dusman_kordinat[8]+47,dusman_kordinat[9],dusman_kordinat[9]+30,arkaplan_renk);
				 		 tft_ili9325_bmpciz_ozel(dusman_kordinat[8],dusman_kordinat[9],renk_siyah,dusman_renk,renk_beyaz,kopek23_bmp);
				 	 }
				 }
			 }
		 }
	 }



}
//-------------------------------------------------------------------------------
static void dusman_olustur(uint16_t kordinat_x , uint16_t kordinat_y, uint8_t dusman_sayi){

	tft_ili9325_bmpciz_ozel(kordinat_x,kordinat_y,renk_siyah,dusman_renk,renk_beyaz,kopek10_bmp);

	if(dusman_sayi==1){
		dusman_kordinat[0]=kordinat_x;
		dusman_kordinat[1]=kordinat_y;
	}
	if(dusman_sayi==2){
		dusman_kordinat[2]=kordinat_x;
		dusman_kordinat[3]=kordinat_y;
	}
	if(dusman_sayi==3){
		dusman_kordinat[4]=kordinat_x;
		dusman_kordinat[5]=kordinat_y;
	}
	if(dusman_sayi==4){
		dusman_kordinat[6]=kordinat_x;
		dusman_kordinat[7]=kordinat_y;
	}
	if(dusman_sayi==5){
		dusman_kordinat[8]=kordinat_x;
		dusman_kordinat[9]=kordinat_y;
	}

	dusman_var=1;

}
//-------------------------------------------------------------------------------
static void kuyu_olustur(uint16_t kordinat_x , uint16_t kordinat_y, uint8_t kuyu_en){

	tft_ili9325_dolukare(kordinat_x-2,kordinat_x+kuyu_en+2,kordinat_y+1,238,duvar_renk);
	tft_ili9325_dolukare(kordinat_x,kordinat_x+kuyu_en,kordinat_y,238,arkaplan_renk);
	tft_ili9325_dolukare(kordinat_x,kordinat_x+kuyu_en,237,238,dusman_renk);
}
//-------------------------------------------------------------------------------
static void yapi_olustur(uint16_t x_baslangic , uint16_t y_baslangic , uint8_t adet){
	uint16_t yar1=0;

	yar1=x_baslangic;
	for(uint16_t i=0;i<adet;i++){
		tft_ili9325_bmpciz(yar1,y_baslangic,duvar_bmp);
		tft_ili9325_boskare(yar1,yar1+20,y_baslangic,y_baslangic+20,2,duvar_renk);
		tft_ili9325_yataycizgi(yar1-1,y_baslangic,21,zemin_renk);
		tft_ili9325_yataycizgi(yar1-1,y_baslangic+1,21,zemin_renk);
		yar1=yar1+20;
	}

}
//-------------------------------------------------------------------------------
static void cop_olustur(uint16_t x_baslangic , uint16_t y_baslangic){
	tft_ili9325_bmpciz_ozel(x_baslangic,y_baslangic,renk_siyah,duvar_renk,renk_beyaz,cop_bmp);
	tft_ili9325_yataycizgi(x_baslangic,y_baslangic,39,zemin_renk);
}
//-------------------------------------------------------------------------------
static void cit_olustur(uint16_t x_baslangic , uint16_t y_baslangic){
	tft_ili9325_bmpciz_ozel(x_baslangic,y_baslangic,renk_siyah,duvar_renk,renk_beyaz,cit_bmp);
	tft_ili9325_yataycizgi(x_baslangic,y_baslangic,54,zemin_renk);
}
//-------------------------------------------------------------------------------
static void kaya_olustur(uint16_t x_baslangic , uint16_t y_baslangic){
	tft_ili9325_bmpciz_ozel(x_baslangic,y_baslangic,renk_siyah,duvar_renk,renk_beyaz,kaya_bmp);
	tft_ili9325_yataycizgi(x_baslangic+5,y_baslangic,6,zemin_renk);
}
//-------------------------------------------------------------------------------
static void kaygan_olustur(uint16_t x_baslangic , uint16_t y_baslangic){
	tft_ili9325_bmpciz_ozel(x_baslangic,y_baslangic,renk_siyah,duvar_renk,renk_beyaz,kaygan_bmp);
}
//-------------------------------------------------------------------------------
static void ziplama_olustur(uint16_t x_baslangic , uint16_t y_baslangic){
	tft_ili9325_bmpciz_ozel(x_baslangic,y_baslangic,renk_siyah,zipla_renk,renk_beyaz,ziplama_bmp);
}
//-------------------------------------------------------------------------------
static void puan_olustur(uint16_t kordinat_x , uint16_t kordinat_y,uint8_t puan_tip){

	if(puan_tip==1){
		tft_ili9325_bmpciz_ozel(kordinat_x,kordinat_y,renk_siyah,puan_renk,renk_beyaz,puan1_bmp);
	}
	if(puan_tip==2){
		tft_ili9325_bmpciz_ozel(kordinat_x,kordinat_y,renk_siyah,puan_renk,renk_beyaz,puan2_bmp);
	}
	if(puan_tip==3){
		tft_ili9325_bmpciz_ozel(kordinat_x,kordinat_y,renk_siyah,puan_renk,renk_beyaz,puan3_bmp);
	}

}

//-------------------------------------------------------------------------------
static void yandi_animasyon (uint16_t karakter_x , uint16_t karakter_y){

	HAL_Delay(500);

	while(karakter_y>23){
		karakter_y--;
		tft_ili9325_bmpciz_karakter(karakter_x,karakter_y,renk_beyaz,arkaplan_renk,kedi0_bmp);
		HAL_Delay(8);
	}

	HAL_Delay(1000);
}
