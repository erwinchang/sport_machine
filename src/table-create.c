#include <stdio.h>
#include <string.h>

#define uns8				unsigned char
#include "number.h"
#define default_outfile 	"number_out.h"


uns8 getXYbit(uns8 *val,uns8 x,uns8 y){
	uns8 v,bit,gy;

	if(x >=8 ){
		printf("!! error: getXYbit(x:%d >=8)",x);		x=0;
	}
	if(y >=8 ){
		printf("!! error: getXYbit(y:%d>=8)",y);		y=0;		
	}

	v=val[x];
	gy = (7-y);
	bit = ( (v >>gy) & 0x1 );
	//printf("v:0x%x,x:%d,gy:%d,bit:%d\n",v,x,gy,bit);

	if (bit) return 1;
	return 0;
}
void setXYbit(uns8 *val,uns8 x,uns8 y,uns8 bit){
	uns8 v,sy;


	if(x >=8 ){
		printf("!! error: setXYbit(x:%d >=8)",x);		x=0;
	}
	if(y >=8 ){
		printf("!! error: setXYbit(y:%d>=8)",y);		y=0;		
	}	

	v = val[x];
	sy = (7-y);

	if(bit){
		v |= (0x1 << sy);
	}else{
		v &= ~(0x1 << sy);
	}
	val[x] = v;
}
void rotate_left90(uns8 *in,uns8 *out){
/*
(xy)
	x0y0,x0y1,x0y2

before:
00000000	00 01 02 03 04 05 06 07
00000110    10 11 12 13 14 15 16 17
00000010    20 21 22 23 24 25 26 27
00000010    30 31 32 33 34 35 36 37
00000010    40 41 42 43 44 45 46 47
00000010    50 51 52 53 54 55 56 57
00111110    60 61 62 63 64 65 66 67
00000000    70 71 72 73 74 75 76 77

after:
00000000	07 17 27 37 47 57 67 77
00000110    06 16 26 36 46 56 66 76
00000010    05 15 25 35 45 55 65 75
00000010    04 14 24 34 44 54 64 74
00000010    03 13 23 33 43 53 63 73
00000010    02 12 22 32 42 52 62 72
00111110    01 11 21 31 41 51 61 71
00000000    00 10 20 30 40 50 60 70
*/
	uns8 y=0,x=0,bit=0,v=0;
	uns8 sx=0,sy=0;

	//clear out
	for(x=0;x<8;x++)
		out[x]=0;

	for(x=0;x<8;x++){
		sy = x;		//7

		for(y=0;y<8;y++){
			sx = 7 - y;		//0,1,2,..7
			bit = getXYbit(in,x,y);
			//printf("get(x:%d,y:%d), set(x:%d,y:%d,bit:%d)\n",x,y,sx,sy,bit);
			setXYbit(out,sx,sy,bit);
		}
	}	
}
void rotate_right90(uns8 *in,uns8 *out){
	uns8 y=0,x=0,bit=0,v=0;
	uns8 sx=0,sy=0;

	//clear out
	for(x=0;x<8;x++)
		out[x]=0;
/*
(xy)
	x0y0,x0y1,x0y2

before:
00000000	00 01 02 03 04 05 06 07
00000110    10 11 12 13 14 15 16 17
00000010    20 21 22 23 24 25 26 27
00000010    30 31 32 33 34 35 36 37
00000010    40 41 42 43 44 45 46 47
00000010    50 51 52 53 54 55 56 57
00111110    60 61 62 63 64 65 66 67
00000000    70 71 72 73 74 75 76 77

after:
00000000	70 60 50 40 30 20 10 00
00000000    71 61 51 41 31 21 11 01 
00000010    72 62 52 42 32 22 12 02 
00000010    73 63 53 43 33 23 13 03 
00000010    74 64 54 44 34 24 14 04 
01000010    75 65 55 45 35 25 15 05
01111110    76 66 56 46 36 26 16 06
00000000	77 67 57 47 37 27 17 07
*/
	for(x=0;x<8;x++){
		sy = 7 - x;		//7,6,5,4..0

		for(y=0;y<8;y++){
			sx = y;		//0,1,2,..7
			bit = getXYbit(in,x,y);
			//printf("get(x:%d,y:%d), set(x:%d,y:%d,bit:%d)\n",x,y,sx,sy,bit);
			setXYbit(out,sx,sy,bit);
		}
	}
}

void mirror_y(uns8 *in,uns8 *out){
/*
(xy)
	x0y0,x0y1,x0y2

before:
00000000	00 01 02 03 04 05 06 07
00000110    10 11 12 13 14 15 16 17
00000010    20 21 22 23 24 25 26 27
00000010    30 31 32 33 34 35 36 37
00000010    40 41 42 43 44 45 46 47
00000010    50 51 52 53 54 55 56 57
00111110    60 61 62 63 64 65 66 67
00000000    70 71 72 73 74 75 76 77

after:
00000000	07 06 05 04 03 02 01 00
01100000    17 16 15 14 13 12 11 10
01000000    27 26 25 24 23 22 21 20
01000000    37 36 35 34 33 32 31 30
01000000    47 46 45 44 43 42 41 40
01000000    57 56 55 54 53 52 51 50
01111100    67 66 65 64 63 62 61 60
00000000    77 76 75 74 73 72 71 70
*/
	uns8 y=0,x=0,bit=0,v=0;
	uns8 sx=0,sy=0;

	//clear out
	for(x=0;x<8;x++)
		out[x]=0;

	for(x=0;x<8;x++){
		sx = x;		//7,6,5,4..0

		for(y=0;y<8;y++){
			sy = (7-y);		//0,1,2,..7
			bit = getXYbit(in,x,y);
			//printf("get(x:%d,y:%d), set(x:%d,y:%d,bit:%d)\n",x,y,sx,sy,bit);
			setXYbit(out,sx,sy,bit);
		}
	}	
}

void num_converter(uns8 *in,uns8 *out){
	uns8 vout0[8];
	mirror_y(in,vout0);
	//rotate_right90(in,out);
	rotate_left90(vout0,out);
}

unsigned char num_string[256];
uns8 get_num_string(uns8 *v,uns8 x_start,uns8 *st){
	uns8 i=0,bit,x,y;
	uns8  n=0;
	
	st[0] = 0;
	n=0;
	x=x_start;
	for(;x<8;x++){
		st[n++] = '0';
		st[n++] = 'B';
		for(y=0;y<8;y++){
			bit = getXYbit(v,x,y);
			if(bit)
				st[n++] = '1';
			else
				st[n++] = '0';
		}
		if(x != 7){
			st[n++] = ',';
			st[n++] = '\n';
		}
	}
	st[n] = 0;
	return n;
}

unsigned char display_string[256];
uns8 get_display_string(uns8 *val,uns8 *head,uns8 *st){
	uns8 tmp[256];
	uns8 n=0;
	uns8 len,i;

	st[0] = 0;
	len = strlen(head);
	for(i=0;i<len;i++)
		st[n++] = head[i];
	st[n++] = '\n';
	st[n++] = '{';
	st[n++] = '\n';

	len = get_num_string(val,0,tmp);
	for(i=0;i<len;i++)
		st[n++] = tmp[i];

	st[n++] = '\n';
	st[n++] = '}';
	st[n++] = ';';
	st[n] = 0;

	return n;
}

uns8 fwrite_display_num_table_string(FILE *fp,uns8 num[10][8],uns8 *head){
	uns8 tmp[256];
	int n=0;
	uns8 len,i,j;
	uns8 *st;
	st = display_string;

	st[0] = 0;
	len = strlen(head);
	for(i=0;i<len;i++)
		st[n++] = head[i];
	st[n++] = '\n';
	st[n++] = '{';
	st[n++] = '\n';
	st[n] = 0;
	fprintf(fp,"%s",st);

	for(i=0;i<10;i++){
		n = 0;
		len = get_num_string(num[i],4,tmp);
		st[n++] = '\n';
		st[n++] = '/';
		st[n++] = '*';
		st[n++] = 0x30 + i;
		st[n++] = '*';
		st[n++] = '/';
		st[n++] = '\n';
		for(j=0;j<len;j++)
			st[n++] = tmp[j];

		if( i!= 9)
			st[n++] = ',';

		st[n++] = '\n';
		st[n]=0;
		fprintf(fp,"%s",st);
	}

	n = 0;
	st[n++] = '\n';
	st[n++] = '}';
	st[n++] = ';';
	st[n] = 0;
	fprintf(fp,"%s",st);

	return n;
}

void display(uns8 *val,uns8 *head){
	uns8 len;
	len = get_display_string(val,head,display_string);
	printf("%s\n",display_string);
}

void writeFileNumber(FILE *fp,uns8 *val,uns8 *head){
	uns8 len;
	fprintf(fp, "\n");
	get_display_string(val,head,display_string);
	fprintf(fp,"%s",display_string);
	fprintf(fp, "\n");
}

uns8 num_table[10][8];
void writeFileNumTable(FILE *fp){
	uns8 len;
	fprintf(fp, "\n");
	fwrite_display_num_table_string(fp,num_table,"const uns8 numTable[10*4] =");
	fprintf(fp, "\n");
}

void create_numtable(uns8 num[10][8]){
	uns8 vout[8];
	int i,j;
	for(i=0;i<10;i++)
		for(j=0;j<8;j++)
			num_table[i][j] = 0;

    num_converter(num0,num[0]);
    num_converter(num01,num[1]);
    num_converter(num02,num[2]);
    num_converter(num03,num[3]);
    num_converter(num04,num[4]);
    num_converter(num05,num[5]);
    num_converter(num06,num[6]);
    num_converter(num07,num[7]);
    num_converter(num08,num[8]);                                
    num_converter(num09,num[9]);    
}

void createfile(char *file){
	uns8 vout[8];
	FILE *fp;
	printf("writefile: %s\n",file);

	create_numtable(num_table);

    fp = fopen(file,"w+");
    fprintf(fp, "/*----------------\n");
    writeFileNumber(fp,num_table[0],"const uns8 num0[8] =");
    writeFileNumber(fp,num_table[1],"const uns8 num01[8] =");
    writeFileNumber(fp,num_table[2],"const uns8 num02[8] =");
    writeFileNumber(fp,num_table[3],"const uns8 num03[8] =");
    writeFileNumber(fp,num_table[4],"const uns8 num04[8] =");
    writeFileNumber(fp,num_table[5],"const uns8 num05[8] =");
    writeFileNumber(fp,num_table[6],"const uns8 num06[8] =");
    writeFileNumber(fp,num_table[7],"const uns8 num07[8] =");
    writeFileNumber(fp,num_table[8],"const uns8 num08[8] =");
    writeFileNumber(fp,num_table[9],"const uns8 num09[8] =");
    fprintf(fp, "--------------*/\n");

	num_converter(numtest,vout);
    writeFileNumber(fp,vout,"const uns8 numtest[8] =");

    writeFileNumTable(fp);
	fclose(fp);
}

int main(int argc,char **argv){
	uns8 vout[8];
	char *outfile = default_outfile;

    if(argc == 2) outfile = argv[1];
    createfile(outfile);
	return 0;
}



