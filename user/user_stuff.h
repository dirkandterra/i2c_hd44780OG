/*
 * user_stuff.h
 *
 *  Created on: May 20, 2015
 *      Author: dricker
 */

#ifndef USER_USER_STUFF_H_
#define USER_USER_STUFF_H_

#include "driver/i2c.h"

uint8 ExtIO_init();
void WYM_init();
uint8 ICACHE_FLASH_ATTR ExtIO_high();
uint8 ICACHE_FLASH_ATTR ExtIO_low();

#endif /* USER_USER_STUFF_H_ */
