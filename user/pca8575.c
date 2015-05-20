/*
    I2C Commands for pca8575
*/

#include "pca8575.h"

/**
 * Write PCA8575 Port
 */
void ICACHE_FLASH_ATTR writePCA8575(pcaPort _port)
{
	char _data[2];
	_data[0]=_port.portATris | _port.portA;
	_data[1]=_port.portBTris | _port.portB;
	I2CwriteBytes(_port.pcaAddr,2,_data);
}

void ICACHE_FLASH_ATTR readPCA8575(pcaPort *_port)
{
	uint8 *portData;

	portData = I2CreadBytesNoReg(_port->pcaAddr ,2);

	if (portData != NULL){
		_port->portA=portData[0];
		_port->portB=portData[1];
	}
	else{
		os_printf("Port Data Error\r\n");
	}
	os_printf("Read A: 0x%x\r\n",_port->portA);
	os_printf("Read B: 0x%x\r\n",_port->portB);

}
