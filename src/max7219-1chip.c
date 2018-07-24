
#define send_spi(C)\
	SSPBUF = C;\
	SSPIF = 0 ;\
    while ( !SSPIF );  


/*
http://www.zonnepanelen.wouterlood.com/7-controlling-a-max7219-dot-led-matrix-module-with-an-arduino-nano/
*/
const char smiley[8] = // fill array for the smiley
{
0B00111110,
0B01000001,
0B10010101,
0B10100001,
0B10100001,
0B10010101,
0B01000001,
0B00111110
};

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
void max7219test(void){
	uns8	i,j,k;
	for(i=0; i<8; i++){
		j=i+1;
		k=smiley[i];
		setCmdChip(j,k);
	}
}

void max7129_init(void){
	setCmdChip(max7219_reg_scanLimit, 0x07);
	//setCmdChip1(max7219_reg_decodeMode, 0xff);  // Code B decode for digits 7–0	
	setCmdChip(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
	setCmdChip(max7219_reg_shutdown, 0x01);    // not in shutdown mode
	setCmdChip(max7219_reg_displayTest, 0x00); // no display test
	max7219test();
}