#ifndef __pca8575_H__
#define __pca8575_H__

#include "driver/i2c.h"


typedef struct {
	uint8 portATris;
	uint8 portBTris;
	union{
		uint8 portA;
		struct{
			uint8 A0:1;
			uint8 A1:1;
			uint8 A2:1;
			uint8 A3:1;
			uint8 A4:1;
			uint8 A5:1;
			uint8 A6:1;
			uint8 A7:1;
		};
	};
	union{
		uint8 portB;
		struct{
			uint8 B0:1;
			uint8 B1:1;
			uint8 B2:1;
			uint8 B3:1;
			uint8 B4:1;
			uint8 B5:1;
			uint8 B6:1;
			uint8 B7:1;
		};
	};

	uint8 pcaAddr;
	uint8 invOutput;
} pcaPort;

void writePCA8575(pcaPort _port);
void readPCA8575(pcaPort *_port);


#endif
