#ifndef __pca8575_H__
#define __pca8575_H__

#include "driver/i2c.h"


typedef struct {
	uint8 portATris;
	uint8 portBTris;
	uint8 portA;
	uint8 portB;
	uint8 pcaAddr;
} pcaPort;

void writePCA8575(pcaPort _port);
void readPCA8575(pcaPort *_port);

#endif
