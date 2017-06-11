// HD44780
//
// lcd.h
//
// 11-Jun-2017  :3  toxcat
//



#include <stdint.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>



//*****************************************************************************

#define LCD_RS_PORT  PORTD
#define LCD_RS_PIN   2

#define LCD_EN_PORT  PORTD
#define LCD_EN_PIN   3


#define LCD_D4_PORT  PORTD
#define LCD_D4_PIN   4

#define LCD_D5_PORT  PORTD
#define LCD_D5_PIN   5

#define LCD_D6_PORT  PORTD
#define LCD_D6_PIN   6

#define LCD_D7_PORT  PORTD
#define LCD_D7_PIN   7

//*****************************************************************************



///////////////////////////////////////////////////////////////////////////////

#ifndef SET_BIT
#define SET_BIT(reg, bit) (reg |= (1<<bit))
#endif

#ifndef CLR_BIT
#define CLR_BIT(reg, bit) (reg &= (~(1<<bit)))
#endif

#ifndef BIT_IS_SET
#define BIT_IS_SET(reg, bit) ((reg>>bit)&1)
#endif

#ifndef BIT_IS_CLR
#define BIT_IS_CLR(reg, bit) (!((reg>>bit)&1))
#endif


#ifndef DDR_REG
#define DDR_REG(x)  (*(&x-1))
#endif

#ifndef PIN_REG
#define PIN_REG(x)  (*(&x-2))
#endif

///////////////////////////////////////////////////////////////////////////////



#define RS_SET  SET_BIT(LCD_RS_PORT, LCD_RS_PIN)
#define RS_CLR  CLR_BIT(LCD_RS_PORT, LCD_RS_PIN)
#define RS_OUT  SET_BIT(DDR_REG(LCD_RS_PORT), LCD_RS_PIN)

#define EN_SET  SET_BIT(LCD_EN_PORT, LCD_EN_PIN)
#define EN_CLR  CLR_BIT(LCD_EN_PORT, LCD_EN_PIN)
#define EN_OUT  SET_BIT(DDR_REG(LCD_EN_PORT), LCD_EN_PIN)

#define D4_SET  SET_BIT(LCD_D4_PORT, LCD_D4_PIN)
#define D4_CLR  CLR_BIT(LCD_D4_PORT, LCD_D4_PIN)
#define D4_OUT  SET_BIT(DDR_REG(LCD_D4_PORT), LCD_D4_PIN)

#define D5_SET  SET_BIT(LCD_D5_PORT, LCD_D5_PIN)
#define D5_CLR  CLR_BIT(LCD_D5_PORT, LCD_D5_PIN)
#define D7_OUT  SET_BIT(DDR_REG(LCD_D7_PORT), LCD_D7_PIN)

#define D6_SET  SET_BIT(LCD_D6_PORT, LCD_D6_PIN)
#define D6_CLR  CLR_BIT(LCD_D6_PORT, LCD_D6_PIN)
#define D6_OUT  SET_BIT(DDR_REG(LCD_D6_PORT), LCD_D6_PIN)

#define D7_SET  SET_BIT(LCD_D7_PORT, LCD_D7_PIN)
#define D7_CLR  CLR_BIT(LCD_D7_PORT, LCD_D7_PIN)
#define D5_OUT  SET_BIT(DDR_REG(LCD_D5_PORT), LCD_D5_PIN)



//******************************  commands  ***********************************

#define DDRAM_WRITE  0b10000000

#define CGRAM_WRITE  0b01000000
#define CGRAM_ADDR   0b00000000

#define DISPLAY_CLR  0b00000001

#define CURSOR_ON    0b00000010
#define CURSOR_OFF   0b00000000

#define BLINK_ON     0b00000001
#define BLINK_OFF    0b00000000

#define SCR_LEFT     0b00011000
#define SCR_RIGHT    0b00011100

//*****************************************************************************



const uint8_t custchar[8*8] PROGMEM =  //64-byte array for 8 custom characters
 {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,
  0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,
  0x00,0x00,0x00,0x00,0x00,0xff,0xff,0xff,
  0x00,0x00,0x00,0x00,0xff,0xff,0xff,0xff,
  0x00,0x00,0x00,0xff,0xff,0xff,0xff,0xff,
  0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,
  0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
  0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
 };



//****************************  prototypes  ***********************************
void lcd_init(void);

void lcd_bus(uint8_t data);
void lcd_wrt(uint8_t data);
void lcd_cmd(uint8_t data);
void lcd_dat(uint8_t data);

void lcd_clear(void);
void lcd_goto(uint8_t line, uint8_t column);

void lcd_print(const char *str);

void lcd_custom(const uint8_t *arr);

inline void lcd_right(void);
inline void lcd_left(void);

//*****************************************************************************



//-----------------------------------------------------------------------------
void lcd_init(void)  //init display
	{
	RS_CLR; RS_OUT;
	EN_CLR; EN_OUT;
	D4_CLR; D4_OUT;
	D5_CLR; D5_OUT;
	D6_CLR; D6_OUT;
	D7_CLR; D7_OUT;

	_delay_ms(20); //delay on power up

	lcd_cmd(0b00000011);
	_delay_ms(5); //wait for the instruction to complete

	lcd_cmd(0b00000011);
	_delay_us(200); //wait for the instruction to complete

	lcd_cmd(0b00000011);
	_delay_us(200);

	lcd_cmd(0b00000010);  //enable 4-bit mode
	lcd_cmd(0b00001000); //4-bit mode, 2-line, 5x8 font
	lcd_cmd(0b00001000); //display off

	lcd_clear();

	lcd_cmd(0b00000110); //entry mode set
	lcd_cmd(0b00001100 | CURSOR_OFF | BLINK_OFF); //display on

	lcd_custom(custchar);

	lcd_goto(0,0);
	}


//-----------------------------------------------------------------------------
void lcd_bus(uint8_t data)  //write four bit
	{
	(data & 0b0001) ? D4_SET : D4_CLR;
	(data & 0b0010) ? D5_SET : D5_CLR;
	(data & 0b0100) ? D6_SET : D6_CLR;
	(data & 0b1000) ? D7_SET : D7_CLR;

	EN_SET;
	_delay_us(100);
	EN_CLR;
	}


//-----------------------------------------------------------------------------
void lcd_wrt(uint8_t data)  //write data
	{
	lcd_bus(data>>4);
	lcd_bus(data);
	_delay_us(100);
	}


//-----------------------------------------------------------------------------
void lcd_cmd(uint8_t data)  //write a command
	{
	RS_CLR;
	lcd_wrt(data);
	}


//-----------------------------------------------------------------------------
void lcd_dat(uint8_t data)  //write data to the display RAM
	{
	RS_SET;
	lcd_wrt(data);
	}


//-----------------------------------------------------------------------------
void lcd_clear(void)  //clear the screen
	{
	lcd_cmd(DISPLAY_CLR);
	_delay_ms(5);
	}


//-----------------------------------------------------------------------------
void lcd_goto(uint8_t line, uint8_t column)  //line 0..1, column 0..39
	{
	lcd_cmd(DDRAM_WRITE + ((line & 0x01)<<6) + column);
	}


//-----------------------------------------------------------------------------
void lcd_print(const char *str)  //print a string
	{
	while(*str) lcd_dat(*str++);
	}


//-----------------------------------------------------------------------------
void lcd_custom(const uint8_t *arr)  //loading an array of custom characters
	{
	lcd_cmd(CGRAM_WRITE);
	for(uint8_t k=0; k<64; k++) lcd_dat(pgm_read_byte(&arr[k]));
	}


//-----------------------------------------------------------------------------
inline void lcd_right(void)  //shift right
	{
	lcd_cmd(SCR_RIGHT);
	}


//-----------------------------------------------------------------------------
inline void lcd_left(void)  //shift left
	{
	lcd_cmd(SCR_LEFT);
	}


