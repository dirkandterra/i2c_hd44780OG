#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#define SSID "SureFireEngineering"
#define SSID_PWD "sfire500"

#define SSID2 "Ricker"
#define SSID_PWD2 "104Ricker"

#define user_procTaskPrio        0
#define user_procTaskQueueLen    1

#define TRISA 0x00
#define TRISB 0x00



#define LCD_Eh()			GPIO_OUTPUT_SET(5, 1);
#define LCD_El()			GPIO_OUTPUT_SET(5, 0);
#define LCData				Port0.portA
#define LCD_RW 				Port0.B0
#define LCD_RS 				Port0.B1
#define LCD_C1				Port0.B2
#define LCD_C2				Port0.B3

#define C1()				LCD_C1=1; LCD_C2=0;
#define C2()				LCD_C1=0; LCD_C2=1;
#define C3()				LCD_C1=1; LCD_C2=1;

#endif
