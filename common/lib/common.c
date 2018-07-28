/*******************************************************************************
 * c Common Library
 *******************************************************************************/
/*
define DELAY_100 for device
*/
#define delay_us(C)\
	TMR0 = C;\
	T0IF = 0 ;\
    while ( !T0IF );
    
void delay1s(void){
	uns8 i,j;
	//1s
	for(j=0; j<100; j++){
		//10ms
		for(i=0; i<100; i++){
			delay_us(DELAY_100U);	//0.1ms
		}
	}
}
