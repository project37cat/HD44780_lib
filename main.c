// HD44780 LCD TEST
//
// main.c
//



#include "lcd.h"



//-----------------------------------------------------------------------------
int main(void)
	{
	lcd_init();

	lcd_print("TEST1234");

	lcd_goto(1,0);

	lcd_dat(0);  //custom char
	lcd_dat(1);
	lcd_dat(2);
	lcd_dat(3);
	lcd_dat(4);
	lcd_dat(5);
	lcd_dat(6);
	lcd_dat(7);

	for(;;)
		{

		}
	}
