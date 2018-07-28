
#define send_spi(C)\
	SSPBUF = C;\
	SSPIF = 0 ;\
    while ( !SSPIF );  


#define max7219_reg_noop        0x00
#define max7219_reg_digit0      0x01
#define max7219_reg_digit1      0x02
#define max7219_reg_digit2      0x03
#define max7219_reg_digit3      0x04
#define max7219_reg_digit4      0x05
#define max7219_reg_digit5      0x06
#define max7219_reg_digit6      0x07
#define max7219_reg_digit7      0x08
#define max7219_reg_decodeMode  0x09
#define max7219_reg_intensity   0x0a
#define max7219_reg_scanLimit   0x0b
#define max7219_reg_shutdown    0x0c
#define max7219_reg_displayTest 0x0f

void setCmdChip(uns8 cmd,uns8 value){
	MAX7219_CS_PIN = 0;

	send_spi(cmd);
	send_spi(value);	

	MAX7219_CS_PIN = 0;
	MAX7219_CS_PIN = 1;	/* max7219 , load rising active*/
}
void max7219test(){
	uns8	i,j,k;
	for(i=0; i<8; i++){
		j=i+1;
		k=numtest[i];
		setCmdChip(j,k);
	}
}


void DisplayNumber(uns8 v10,uns8 v1){
	uns8 v,i,j,n;
	uns8 clear10,clear1;

	if(v10 > 9 ) v10 = 9;
	j = 4*v10;
	for(i=1;i<5;i++){
		v = numTable[j];
		j = j+1;
		setCmdChip(i,v);
	}

	if(v1 > 9 ) v1 = 9;
	j = 4*v1;
	for(i=5;i<9;i++){
		v = numTable[j];
		j = j+1;
		setCmdChip(i,v);
	}

}

void init_max7129(void){
	setCmdChip(max7219_reg_scanLimit, 0x07);
	//setCmdChip1(max7219_reg_decodeMode, 0xff);  // Code B decode for digits 7–0	
	setCmdChip(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
	setCmdChip(max7219_reg_intensity, 0x0d);
	setCmdChip(max7219_reg_shutdown, 0x01);    // not in shutdown mode
	setCmdChip(max7219_reg_displayTest, 0x00); // no display test

	//max7219test();
	DisplayNumber(0,0);
}