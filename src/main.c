#include "16f722.h"
#include "c_16f722.h"

#pragma config = ( HS_ & WDTDIS_ & PWRTEN_ & MCLREN_ & UNPROTECT_ & BOREN_ & BORV25_ & PLLDIS_ & DEBUGDIS_ )

/*
RC2      : max7219 load (active low),cs
RC3 SCK  : max7219 sck
RC4 SDI  : no used
RC5 SDO  : max7219 DI
RC6	TX   : debug tty
RC7 RX   : debug tty
*/
#pragma bit TRISC6  @ PORTC.6
#pragma bit TRISC7  @ PORTC.7
#pragma bit CS		@ PORTC.2
#pragma bit SCK		@ PORTC.3
#pragma bit SDO		@ PORTC.5

#pragma wideConstData p	

/* global variables */
#define NText 	12
const char text[NText] = "Hello world!";

#include "sci-lib.c"

void init_comms(void){
	TRISC6 = 1;		// TX
/*
9600 = 8M/[16*(SPBRG+1)] =>   SPBRG = 8M/[16*9600] - 1 = 51
BuadRate = 8M/[16*(51+1)] = 9615
*/	
	SPBRG = 52;	// 8MHz,9600,16F722	
/*
SYNC=0,BRG=1 , BuadRate = Fsoc/[16*(SPBRG+1)] , 20M/[16*(129+1)] = 9615,  (9615-9600)/9600 = 0.16%
*/	
//	SPBRG = 129;	// 20MHz,9600,16F722	20M/16*(129+1) = 
//	RCSTA = 0x90;
//	TXSTA = 0x24;

	SPEN = 1;		//Serial Port Enable Bit
	SYNC = 0;		//0:Asychronous
	BRGH = 1;		//1:High Speed
	TXEN = 1;

	TRISC7 = 1;		// RX
	CREN = 1;
}

//RC2      --> max7219 load (active low)
//RC3 SCK  --> max7219 sck
//RC4 SDI no used
//RC5 SDO  --> max7219 DI
void init_spi(void){

	//SSPCON	= 0b.0010.0000;
	/*
	bit7   WCOL:  0
	bit6   SSPOV: 0 ,no receive overflow indicator bit(無使用SDI)
	bit5   SSPEN: 1 ,enable SCK,SDO,SDI
	bit4   CKP: 0, idle state for clk is a low level
	bit0-3 SSPM<3:0>: 0000 SPI Master, clk = fosc/4, 20MHZ/4=5Mhz
	                  0001 5M/16
	                  0010 5M/64

	max7219 clk 上升取DI，無動作clk為low
	16f726 => master, CKP = 0,CKE = 1
	*/
	SSPCON	= 0b.0010.0010;	// 1.25MHz

	CKE 	= 1;
	/*
	CKE   1,Data stable on rising edge of SCK
	SSPSTAT.6
	*/

	/* low active, CS */
	/* RC2: CS = 1 */
	PORTC 	= 0b.0000.0000;
	TRISC 	= 0b.1101.0001; /* RC2: output */
}

void main( void)
{
	uns8	i,j;
    if (TO == 1 && PD == 1 /* power up */)  {
    //   WARM_RESET:
         clearRAM(); // clear all RAM
    }
	TRISC	= 0xD0;			//TX,RX,SDI -input
	PIE1 	= 0;
	INTCON	= 0;
	init_comms();
	init_spi();
	crlf();
	for(i=0; i<NText; i++) putch(text[i]);	crlf();
	for(i=0; i<NText; i++) putch(text[i]);  crlf();
	for(i=0; i<NText; i++) putch(text[i]);  crlf();
	crlf();	
	while(1);
}