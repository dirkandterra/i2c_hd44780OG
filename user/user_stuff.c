/*
 * user_stuff.c
 *
 *  Created on: May 20, 2015
 *      Author: dricker
 */

#include "user_stuff.h"
#include "pca8575.h"
#include "user_config.h"

pcaPort Port0;

void ICACHE_FLASH_ATTR WYM_init(){
	initLCD();
}

uint8 ICACHE_FLASH_ATTR ExtIO_init(){

	Port0.pcaAddr=0x20;
	Port0.portATris=TRISA;
	Port0.portBTris=TRISB;
	Port0.invOutput=0;

	Port0.portA=0xFF;
	Port0.portB=0xFF;
	writePCA8575(Port0);

    return 1;
}

uint8 ICACHE_FLASH_ATTR ExtIO_high(){
	char c[10];
	setPos(0,0x00);
	strcpy(c, "High Level");
	printStr(c,0);
	//readPCA8575(&Port0);
	//os_printf("PortA -> 0x%x  PortB-> 0x%x\r\n",Port0.portA,Port0.portB);
	return 1;

}

uint8 ICACHE_FLASH_ATTR ExtIO_low(){
	char c[10];
	setPos(0,0x00);
	strcpy(c, "Low  Level");
	printStr(c,0);
	//readPCA8575(&Port0);
	//os_printf("PortA -> 0x%x  PortB-> 0x%x\r\n",Port0.portA,Port0.portB);
	return 1;

}
