#include "driver/i2c_hd44780.h"
#include "pca8575.h"

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





/*********** mid level commands, for sending data/cmds */

void ICACHE_FLASH_ATTR LCD_command(uint8 rs, uint8 value) {
	I2CwriteByte(LCD_ADDRESS,rs,value);
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
    	I2CwriteByte(LCD_ADDRESS,0x40, data[i]);
    }
}

void ICACHE_FLASH_ATTR LCD_setCursor(uint8 col, uint8 row){
    int row_offsets[] = { 0x00, 0x40};
    _numlines = LCD_ROW;
    if ( row > _numlines ) {
        row = _numlines-1;    // we count rows starting w/0
    }
    I2CwriteByte(LCD_ADDRESS,0x00, (LCD_SETDDRAMADDR | (col + row_offsets[row])));
}

uint8 ICACHE_FLASH_ATTR LCD_clear(){
	I2CwriteByte(LCD_ADDRESS,0x00,0x01);
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

    I2CwriteBytes(LCD_ADDRESS,10,txData);

    char page_buffer[20];
    os_sprintf(page_buffer,"Connecting WiFi...");

    LCD_print(page_buffer);

    return 1;
}

uint8 ICACHE_FLASH_ATTR ExtIO_init(){
	char d[2];
	char c[2];
	d[0]=0xFF;
	d[1]=0xFF;
	c[0]=0x00;
	c[1]=0x00;

	writePCA8575(c);
	writePCA8575(d);
	writePCA8575(c);


    return 1;
}

uint8 ICACHE_FLASH_ATTR ExtIO_high(){
	char d[2];
	d[0]=0xFF;
	d[1]=0xFF;
	writePCA8575(d);
	return 1;

}

