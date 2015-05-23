#include "glcd.h"
#include "user_config.h"
#include "string.h"
#include "pca8575.h"


unsigned char currentX=0;
unsigned char currentY=0;
unsigned char currentOffset;

static const unsigned char cool[95][5]=
{
    { 0x00, 0x00, 0x00, 0x00, 0x00 },  // sp
    { 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0xc4, 0xc8, 0x10, 0x26, 0x46 },   // %
    { 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x50, 0x30, 0x00 },   // ,
    { 0x10, 0x10, 0x10, 0x10, 0x10 },   // -
    { 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
    { 0x7E, 0x11, 0x11, 0x11, 0x7E },   // A
    { 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
    { 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
    { 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x0C, 0x52, 0x52, 0x52, 0x3E },   // g
    { 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x20, 0x40, 0x44, 0x3D, 0x00 },   // j
    { 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x7C, 0x14, 0x14, 0x14, 0x08 },   // p
    { 0x08, 0x14, 0x14, 0x18, 0x7C },   // q
    { 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x0C, 0x50, 0x50, 0x50, 0x3C },   // y
    { 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
    { 0x00, 0x08, 0x63, 0x41, 0x41 },   // { 
    { 0x00, 0x00, 0x7F, 0x00, 0x00 },   // | 
    { 0x41, 0x41, 0x63, 0x08, 0x00 },   // } 
    { 0x7E, 0x7E, 0x7E, 0x7E, 0x7E } 
};


extern pcaPort Port0;

void toggleE(void);
void initLCD(void);
void dwLCD(unsigned char c);
void rwLCD(unsigned char c);
void barDataHoriz(unsigned char per, unsigned char width, unsigned char strt);
void barDataVert(unsigned char per, unsigned char width, unsigned char strt);
void clrLCD(void);
void printChar(char c,char invert);
void printStr(char *c, char invert);
void setPos(unsigned char x, unsigned char y);
void delay (int dly);



/***************************************
void initLCD(){
	C3();
	delay(500);
	rwLCD(0x3F);		// Turn Display on
	rwLCD(0xC0);		// set first page to top
	rwLCD(0xb8);		// Select First Page
	rwLCD(0x40);		// Select First Row

	C1();
}
	
/***************************************/
void toggleE(){
	delay(5);
	LCD_E=1;
	delay(5);
	LCD_E=0;	
}



/***************************************
void dwLCD(unsigned char c){
	LCD_RW=0;
	LCD_RS=1;
	if (currentX<64) {
		C1();
	}
	else {
		C2();
		if (currentX==64)
		{
			setPos(currentX,currentY);	//switching chips, get C2 position set
			LCD_RW=0;
			LCD_RS=1;
		}
	}
	LCData = c;
	toggleE();
	currentX++;
	if (currentX>(LCDWIDTH-1)) 
	{	
		currentX=0;
	}
}

/***************************************
void rwLCD(unsigned char c){
	LCD_RW=0;
	LCD_RS=0;
	LCData = c;
	toggleE();
}

/***************************************
void clrLCD(void)
{
	unsigned char page = 0;
	unsigned char cc = 0;	
	while (page<8)
	{
		cc=0;
		setPos(0,page*8);
		while(cc<128)
		{
			dwLCD(0x00);
			cc++;
		}
		page++;
	}
}
/********************************************
void printStr(char *c, char invert){
	int i=strlen(c);
	int j;
	for(j=0;j<i;j++){
		printChar(c[j],invert);
	}
}
/********************************************
void printChar(char c,char invert){
	char ii;
	char jj=0x00;
	c-=0x20;
	if (invert){jj=0xFF;}
	if (c>=0 && c<0x7F){
		if(currentX>123){
			while(currentX>123){
				dwLCD(jj);
			}
			jj=currentY+8;
			if (jj>56){jj=0;}
			setPos(0,jj);
		}
		for(ii=0; ii<5; ii++)
		{
			LCData=cool[c][ii];
			LCData=LCData<<1;
			if (invert){LCData=~LCData;}
			dwLCD(LCData);
		}
		if(invert){dwLCD(0xFF);}
		else {dwLCD(0x00);}
	}
}

/********************************************
void printImage(unsigned char *c,char width, char height, char invert){
	char startX=currentX;
	char startY=currentY;
	int ii;
	int jj;

	for(ii=0; ii<height; ii+=8)
	{
		setPos(startX,startY+ii);
		for(jj=0; jj<width; jj++)
			{
				LCData=c[ii*width+jj];						
				if (invert){LCData=~LCData;}
				dwLCD(LCData);
			}
	}

}

/***************************************
void setPos(unsigned char x, unsigned char y)
{
	unsigned char page;

	if (x>(LCDWIDTH-1)) 
	{	
		return;
	}
	if (y>(LCDHEIGHT-1)) 
	{
		return;
	}
	currentX=x;
	currentY=y;

	if (x>(CHIPWIDTH-1)) {
		C2();
		x-=CHIPWIDTH;
	}
	else{ 
		C1();
	}

	page = y/8;
	currentOffset = y%8;

	rwLCD(page+0xb8);		// Select Page
	rwLCD(0x40+x);		// Select Row

}

/***************************************
void barDataHoriz(unsigned char per, unsigned char width, unsigned char strt){
	float f;
	int ii;
	ii=0;
	f=(float)per*128/100;
	while (width>0)
	{
		width-=8;
		setPos(0,strt);
		strt+=8;
		while(ii<128)
		{
			if (ii<(int)f) dwLCD(0xFF);
			else dwLCD(0x00);
			ii++;
		}
		ii=0;
	}
}

/***************************************
void barDataVert(unsigned char per, unsigned char width, unsigned char strt){
	float f;
	int ii;
	unsigned char temp;
	ii=0;
	f=(float)per*64/100;
	while (width>0)
	{
		width-=1;

		while(ii<64)
		{
			setPos(strt,ii);
			if (f<(56-ii+1))		//segment is all blank
			{
				dwLCD(0x00);
			}
			else {
				if (f>=(64-ii))		//segment is all full
				{
					dwLCD(0xFF);
				}
				else				//segment is partial
				{
					temp = (f-(56-ii));
					temp = 8-temp;	
					temp = 0XFF<<temp;				
					dwLCD(temp);
				}
			}
			ii=ii+8;
		}
		strt++;
		ii=0;
	}
}

/***************************************/
void delay(int dly){
	int ii=0;
	while (ii<dly){
		ii++;
	}
}