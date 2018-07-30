#include "16f722.h"
#include "c_16f722.h"

#pragma config = ( HS_ & WDTDIS_ & PWRTEN_ & MCLREN_ & UNPROTECT_ & BOREN_ & BORV25_ & PLLDIS_ & DEBUGDIS_ )
#pragma bit MAX7219_CS_PIN	@ PORTC.2
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
#pragma bit TP		@ PORTC.1
#pragma bit SensorLED		@ PORTC.0

#pragma wideConstData p	

/* global variables */
#define NText 	12
const char text[NText] = "Hello world!";

//	OPTION	= 0x82, 	1:8, 10M/4/8=312.5K => 3.2us
//	delay_us(256-200); measurement: 800us -> 4us
#define	DELAY_100U		(unsigned char)(256 -(100.0/4.0) +2) 	//98us

/* key value */
uns8 previousTMR0;
uns8 subClock;
uns8 timer1, timer2L, timer2H;
bit timeout1, timeout2;

uns8	sensorCnt_0;
uns8	sensorCnt_1;
uns8	cntScore;
bit    	portb4_0;
bit    	portb4_1;

#include "int16CXX.h"
#pragma origin 4
interrupt int_server(void)
{
	int_save_registers    // W, STATUS (and PCLATH)
  	uns8 valportb,sv_FSR;
  	sv_FSR = FSR;

	if(RBIF){
		RBIF	= 0;
		valportb = PORTB;

		if( valportb & 0x10)
			portb4_0 = 1;
		else
			portb4_0 = 0;

		//RB4 falling
		if( (portb4_1==1) && (portb4_0==0) )
			sensorCnt_0++;

		portb4_1 = portb4_0;

	}
	FSR = sv_FSR;
   	int_restore_registers // W, STATUS (and PCLATH)
}

#include "out.h"
#include "common.c"
#include "sci-lib.c"
#include "max7219-1chip.c"
#include "fsm.c"

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
	/* RC0: SensorLED */
	PORTC 	= 0b.0000.0100;
	TRISC 	= 0b.1101.0000; /* RC2: output */
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
	init_max7129();
	init_fsm();
	init_app();

	//OPTION  = 0x81 ;  1:4 prescaler ,20M/4/4=1.25M = 0.8u
    //          0x82 ;  1:8                            1.6u
    //          0x83 ;  1:16            20M/4/16 = 312.5K = 3.2us
    //          0x84 ;  1:32            20M/4/32 = 156.25K  = 6.4us   
    //          0x85 ;  1:64            20M/4/64 =  78.125K = 12.8us
    //          0x86 ;  1:128          20M/4/128 =   39.0625K = 25.6us
	/*
	OPTION	= 0x83, 	1:16, 10M/4/16=156.25K => 6.4us
	delay_us(256-200); measurement: 1600us -> 8us

	OPTION	= 0x82, 	1:8, 10M/4/8=312.5K => 3.2us
	delay_us(256-200); measurement: 800us -> 4us

	OPTION	= 0x81, 	1:4, 10M/4/4=625K => 1.6us
	delay_us(256-200); measurement: 404us -> 2.02us	

	test code
		TP = 0;
		delay_us(256-200);
		TP = 1;
		delay_us(256-200);	
	*/
	OPTION	= 0x82;

	/*	RB4 IR0 Sensor	*/
	PORTB 	= 0b.0000.0000;
	ANSELB	= 0b.0000.0000; /* digital */
	TRISB 	= 0b.0001.0000; /* intput */
	IOCB	= 0b.0001.0000;

	sensorCnt_0	= 0;
	sensorCnt_1 = 0;
	cntScore  	= 0;
	TP 		  	= 1;
	SensorLED 	= 0;

	delayms(500);
	RBIE	= 1;
	GIE		= 1;
	
	while(1){
		timerTick();
        /* Not more than 1 (2) millisec.
           between each call to timerTick() */

        /* .. sample analog channels */
        /* .. do other IO communication */
        /* .. do global processing and testing */
		//fsm1();
		//fsm2();
		app_main();
	}
}