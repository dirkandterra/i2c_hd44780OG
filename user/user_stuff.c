/*
 * user_stuff.c
 *
 *  Created on: May 20, 2015
 *      Author: dricker
 */

#include "user_stuff.h"
#include "pca8575.h"

pcaPort Port0;

uint8 ICACHE_FLASH_ATTR ExtIO_init(){

	Port0.pcaAddr=0x20;
	Port0.portATris=0x0F;
	Port0.portBTris=0x0F;

	Port0.portA=0xFF;
	Port0.portB=0xFF;
	writePCA8575(Port0);

    return 1;
}

uint8 ICACHE_FLASH_ATTR ExtIO_high(){
	Port0.portA=0x80;
	Port0.portB=0xC0;
	writePCA8575(Port0);
	Port0.portA=0x00;
	readPCA8575(&Port0);
	os_printf("PortA -> 0x%x  PortB-> 0x%x\r\n",Port0.portA,Port0.portB);
	return 1;

}

uint8 ICACHE_FLASH_ATTR ExtIO_low(){
	Port0.portA=0xFF;
	Port0.portB=0xFF;
	writePCA8575(Port0);
	Port0.portA=0xFF;
	readPCA8575(&Port0);
	os_printf("PortA -> 0x%x\r\n",Port0.portA);
	return 1;

}
