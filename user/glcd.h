#ifndef __glcd_H__
#define __glcd_H__

#define LCDHEIGHT	64
#define LCDWIDTH	128
#define CHIPWIDTH	64

unsigned char currentX;
unsigned char currentY;
unsigned char currentOffset;

void toggleE(void);
void initLCD(void);
void dwLCD(unsigned char c);
void rwLCD(unsigned char c);
void barDataHoriz(unsigned char per, unsigned char width, unsigned char strt);
void barDataVert(unsigned char per, unsigned char width, unsigned char strt);
void clrLCD(void);
void setPos(unsigned char x, unsigned char y);
void printChar(char c,char invert);
void printStr(char *c, char invert);
void printImage(unsigned char *c,char width, char height, char invert);
void delay(int dly);

#endif
