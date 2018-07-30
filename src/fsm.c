
#define startTimer1(c)		\
	timer1 		= c;		\
	timeout1	= 0;

#define startTimer2(c)		\
	timer2L		= (c)%256;	\
	timer2H		= (c)/256;	\
	timeout2	= 0;

#define startTimer3(c)		\
	timer3L		= (c)%256;	\
	timer3H		= (c)/256;	\
	timeout3	= 0;

#define startTimer4(c)		\
	timer4L		= (c)%256;	\
	timer4H		= (c)/256;	\
	timeout4	= 0;

void init_fsm(void){
    timeout1	= 1;
    timeout2 	= 1;
    timeout3    = 1;
    timeout4    = 1;
}

void timerTick()
/*
 - decrements active timers
 - 10 MHz oscillator
 - period: 1 millisec. (prescaler divide by 8)
 - subClock: 0 .. 251
*/
{
    char sample, incr;

    sample = TMR0;  /* sampling the timer */
    incr = sample - previousTMR0;
    previousTMR0 = sample;

    subClock -= incr;
	
	if ( !Carry)  {
    /* new 1 millisec. tick */
		subClock += 251;		//measurement: 1mus

		if(!timeout1){
			timer1--;
			if(timer1 == 255)
				timeout1 = 1;
		}
		if(!timeout2){
			if(--timer2L == 255){
				if(--timer2H == 255)
					timeout2 = 1;
			}
		}		
		if(!timeout3){
			if(--timer3L == 255){
				if(--timer3H == 255)
					timeout3 = 1;
			}
		}		
		if(!timeout4){
			if(--timer4L == 255){
				if(--timer4H == 255)
					timeout4 = 1;
			}
		}				
	}
}


void init_app(){
	startTimer1(50);
	startTimer2(1000);
}

void app_main(){
	uns8 v;

	if(timeout1){
		startTimer1(150);

		v = sensorCnt_0;
		if(v != sensorCnt_1){
			cntScore++;
			if(cntScore > 99 ) cntScore=1;
			sensorCnt_1 = v;
			//SensorLED = !SensorLED;
		}
	}


	//1S,time clock
	if(timeout2){
		startTimer2(1000);

		cntTimer++;
		if(cntTimer > 99 ) cntTimer=0;
	}

	//5S,timer show
	if(timeout3){
		startTimer3(5000);
		startTimer4(800);

		set10base(cntTimer);
		DisplayNumber(v10base[1],v10base[0]);
	}

	//1S,cntScore Show
	if(timeout4){
		startTimer4(600);

		set10base(cntScore);
		DisplayNumber(v10base[1],v10base[0]);
	}

}
