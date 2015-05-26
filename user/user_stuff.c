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
	char c[5];
	setPos(0,0x00);
	strcpy(c, "High");
	printStr(c,0);
	Port0.portA=0x80;
	Port0.portB=0xC0;
	writePCA8575(Port0);
	readPCA8575(&Port0);
	os_printf("PortA -> 0x%x  PortB-> 0x%x\r\n",Port0.portA,Port0.portB);
	return 1;

}

uint8 ICACHE_FLASH_ATTR ExtIO_low(){
	char c[5];
	setPos(0,0x00);
	strcpy(c, "Low");
	printStr(c,0);
	Port0.portA=0xFF;
	Port0.portB=0x00;
	writePCA8575(Port0);
	readPCA8575(&Port0);
	os_printf("PortA -> 0x%x  PortB-> 0x%x\r\n",Port0.portA,Port0.portB);
	return 1;

}
