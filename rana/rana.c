/***************************************************************************//**
  @file     rana.c
  @brief    Declarancion del objeto rana y definiciones de funciones globales 
  		que modifican los atributos de la rana
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "rana.h"


/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
/*
 * @brief: Declaracion del objeto rana_frg propia del modulo,
 * 		pero con persistencia global.	
 * */ 
static rana_be_t rana_frg;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void rana_init(uint8_t posx_in,uint8_t posy_in,uint8_t vidas_in,uint8_t llegadas_in){
	rana_frg.desborde=NDESBR;
	rana_frg.pos_x=posx_in;
	rana_frg.pos_y=posy_in;
	rana_frg.vidas=vidas_in;
	rana_frg.llegadas=llegadas_in;
}

rana_be_t * rana_frogger(char mov){
	switch(mov){
		case 'r':
			if(rana_frg.pos_x<(CANTCOLS-1)){/*si no hay posible desborde*/
				rana_frg.pos_x++;   /*muevo rana hacia la derecha*/
			}
			else{
				rana_frg.desborde=DESBR;
			}

			break;
		case 'l':	
			if(rana_frg.pos_x>0){	  /*si no hay posible desborde*/	
				rana_frg.pos_x--; /*muevo rana hacia la izquierda*/
			}
			else{
				rana_frg.desborde=DESBR;
			}
			break;
		case 'u':	
			if(rana_frg.pos_y<(CANTFILS-1)){ /*si no hay posible desborde*/
				rana_frg.pos_y++;        /*muevo rana hacia arriba*/
			}
			else{
				rana_frg.desborde=DESBR;
			}
			break;
		case 'd':	
			if(rana_frg.pos_y>0){	  /*si no hay posible desborde*/
				rana_frg.pos_y--; /*muevo rana hacia abajo*/
			} 
			else{
				rana_frg.desborde=DESBR;
			}

			break;
		case '0':
			/*jugador aun no mueve rana*/
			break;
	}
	if(rana_frg.pos_y>(CANTFILS-1)|| rana_frg.pos_y<0){
		rana_frg.desborde=DESBR;
	}
	else if(rana_frg.pos_x>(CANTCOLS-1) || rana_frg.pos_x<0){
		rana_frg.desborde=DESBR;
	}

	return &rana_frg;
}






