#include "driver/i2c_hd44780.h"

//Part of this code comes from http://hmario.home.xs4all.nl/arduino/LiquidCrystal_I2C/
//And from https://github.com/vanluynm/LiquidCrystal_I2C/

/*
I2C Expander map
Func   Bit 
RS     0
RW     1
Enable 2
BL     3
D4     4
D5     5
D6     6
*/

uint8 _displayfunction;
uint8 _displaycontrol;
uint8 _displaymode;
uint8 _backlightval;
uint8 _numlines;
uint8 txData[11];


/************ low level data pushing commands **********/
uint8 ICACHE_FLASH_ATTR LCD_writeByte(uint8 _rs, uint8 _data){
    i2c_start();
    i2c_writeByte(LCD_ADDRESS << 1);
    if (!i2c_check_ack()) 
    {
    	os_printf("No Ack Addr\n\r");
        i2c_stop();
        return 0;
    }

	i2c_writeByte((uint8)(_rs));
    if (!i2c_check_ack())
    {
    	os_printf("No Ack data byte\n\r");
		//i2c_stop();
		//return 0;
	}


    i2c_writeByte((uint8)(_data));
    if (!i2c_check_ack())
    {
    	os_printf("No Ack data byte\n\r");
		//i2c_stop();
		//return 0;
	}
    i2c_stop();
    return 1;
}

/************ low level data pushing commands **********/
uint8 ICACHE_FLASH_ATTR LCD_writeBytes(uint8 _numBytes, uint8 *_data){

    i2c_start();
    i2c_writeByte(LCD_ADDRESS << 1);
    if (!i2c_check_ack())
    {
    	os_printf("No Ack Addr\n\r");
        i2c_stop();
        return 0;
    }
    int ii=0;
    while (ii<_numBytes){
    	//os_printf("Byte %d\n\r",_data[ii]);
    	i2c_writeByte(_data[ii]);
		if (!i2c_check_ack())
		{
			os_printf("No Ack data byte\n\r");
			i2c_stop();
			return 0;
		}
		ii++;
    }

    i2c_stop();
    return 1;
}

uint8 ICACHE_FLASH_ATTR LCD_readByte(uint8 _addr, uint8 _data){
	return 0;
}


/*********** mid level commands, for sending data/cmds */

void ICACHE_FLASH_ATTR LCD_command(uint8 rs, uint8 value) {
	LCD_writeByte(rs,value);
}
uint8 ICACHE_FLASH_ATTR LCD_read(uint8 addr) {
    return 0;
}

void ICACHE_FLASH_ATTR LCD_print(char data[])
{
    uint8 size;
    size = strlen(data);
    uint8 i;
    for (i = 0; i < size; i++) {
    	LCD_writeByte(0x40, data[i]);
    }
}

void ICACHE_FLASH_ATTR LCD_setCursor(uint8 col, uint8 row){
    int row_offsets[] = { 0x00, 0x40};
    _numlines = LCD_ROW;
    if ( row > _numlines ) {
        row = _numlines-1;    // we count rows starting w/0
    }
    LCD_writeByte(0x00, (LCD_SETDDRAMADDR | (col + row_offsets[row])));
}

uint8 ICACHE_FLASH_ATTR LCD_clear(){
	LCD_writeByte(0x00,0x01);
	return 0;
}
uint8 ICACHE_FLASH_ATTR
LCD_init(){

    // Now we pull both RS and R/W low to begin commands
    os_delay_us(2000000);


    txData[0]=0x00;
    txData[1]=0x38;
    txData[2]=0x39;
    txData[3]=0x14;
    txData[4]=0x78;
    txData[5]=0x5E;
    txData[6]=0x6D;
    txData[7]=0x0C;
    txData[8]=0x01;
    txData[9]=0x06;


/*
	LCD_command(0x00,0x38);			// Function Set (8 bit, 2 Line, Single height, instruction table (0,0))
	LCD_command(0x00,0x39);			// Function Set (8 bit, 2 Line, Single height, instruction table (0,1))
	LCD_command(0x00,0x14);			// 1/5 Bias
	LCD_command(0x00,0x78);			// Contrast set
	LCD_command(0x00,0x5E);			// Icon on, Booster on, c5 set for internal follower mode
	LCD_command(0x00,0x6D);			// Follower Circuit on
	LCD_command(0x00,0x0C);			// Display on
	LCD_command(0x00,0x01);			// Clear Display
	LCD_command(0x00,0x06);			// Increment address, no shift
	LCD_command(0x40,0x49);			// Print 'I'
	LCD_command(0x40,0x32);			// Print '2'
	LCD_command(0x40,0x43);			// Print 'C'
	LCD_command(0x40,0x20);			// Print ' '
	LCD_command(0x40,0x52);			// Print 'R'
	LCD_command(0x40,0x6f);			// Print 'o'
	LCD_command(0x40,0x63);			// Print 'c'
	LCD_command(0x40,0x6b);			// Print 'k'
	LCD_command(0x40,0x73);			// Print 's'
	LCD_command(0x40,0x21);			// Print '!'
*/

    LCD_writeBytes(10,txData);

    char page_buffer[20];
    os_sprintf(page_buffer,"Connecting WiFi...");

    LCD_print(page_buffer);

    return 1;
}
