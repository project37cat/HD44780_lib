// HD44780 LCD TEST
//
// main.c
//


#include <util/delay.h>


#include "lcd.h"



//-----------------------------------------------------------------------------
int main(void)
	{
	lcd_init();

	lcd_print("12345678");

	lcd_goto(1,0);

	lcd_print("TEST");

	lcd_dat(1); //custom char
	lcd_dat(3);
	lcd_dat(5);
	lcd_dat(7);

	for(;;)
		{


		}
	}
