//функции для работы с дисплеем на HD44780
//версия 1 от 31.10.2014
//компилятор avr-gcc


//назначаем куда будет подключен дисплей (вывод R/W подключается к земле)
//можно использовать любые свободные пины мк, они инициализируются в функции lcd_init

#define LCD_RS_PORT  PORTC
#define LCD_RS_PIN   3

#define LCD_EN_PORT  PORTC
#define LCD_EN_PIN   2

#define LCD_D4_PORT  PORTB
#define LCD_D4_PIN   4
#define LCD_D5_PORT  PORTC
#define LCD_D5_PIN   1
#define LCD_D6_PORT  PORTB
#define LCD_D6_PIN   2
#define LCD_D7_PORT  PORTB
#define LCD_D7_PIN   3

///////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef set_bit
#define set_bit(reg,bit) reg |= (1<<(bit))
#endif

#ifndef clear_bit
#define clear_bit(reg,bit) reg &= (~(1<<(bit)))
#endif


#define RS_H set_bit(LCD_RS_PORT, LCD_RS_PIN)
#define RS_L clear_bit(LCD_RS_PORT, LCD_RS_PIN)

#define EN_H set_bit(LCD_EN_PORT, LCD_EN_PIN)
#define EN_L clear_bit(LCD_EN_PORT, LCD_EN_PIN)

#define D4_H set_bit(LCD_D4_PORT, LCD_D4_PIN)
#define D4_L clear_bit(LCD_D4_PORT, LCD_D4_PIN)

#define D5_H set_bit(LCD_D5_PORT, LCD_D5_PIN)
#define D5_L clear_bit(LCD_D5_PORT, LCD_D5_PIN)

#define D6_H set_bit(LCD_D6_PORT, LCD_D6_PIN)
#define D6_L clear_bit(LCD_D6_PORT, LCD_D6_PIN)

#define D7_H set_bit(LCD_D7_PORT, LCD_D7_PIN)
#define D7_L clear_bit(LCD_D7_PORT, LCD_D7_PIN)


#ifndef DDR
#define DDR(x) (*(&x-1))
#endif

#define RS_DDR set_bit(DDR(LCD_RS_PORT), LCD_RS_PIN)
#define EN_DDR set_bit(DDR(LCD_EN_PORT), LCD_EN_PIN)

#define D4_DDR set_bit(DDR(LCD_D4_PORT), LCD_D4_PIN)
#define D5_DDR set_bit(DDR(LCD_D5_PORT), LCD_D5_PIN)
#define D6_DDR set_bit(DDR(LCD_D6_PORT), LCD_D6_PIN)
#define D7_DDR set_bit(DDR(LCD_D7_PORT), LCD_D7_PIN)


void lcd_init(void);
void lcd_data(char data);
void lcd_cmd(char data);
void lcd_clear(void);
void lcd_goto(char line, char column);
void lcd_char(char sign);
void lcd_print(const char *str);
void lcd_right(void);
void lcd_left(void);


///////////////////////////////////////////////////////////////////////////////////////////////////
void lcd_init(void) //инициализация дисплея
{
RS_DDR;  EN_DDR;  D4_DDR;  D5_DDR;  D6_DDR;  D7_DDR;
RS_L;    EN_L;    D4_L;    D5_L;    D6_L;    D7_L;

// последовательность команд и величины задержек взяты из статьи:
// http://web.alfredstate.edu/weimandn/lcd/lcd_initialization/lcd_initialization_index.html

_delay_ms(20); //delay on power up
lcd_cmd(0b0011);
_delay_ms(5); //wait for the instruction to complete
lcd_cmd(0b0011);
_delay_us(200); //wait for the instruction to complete
lcd_cmd(0b0011);
_delay_us(200);

lcd_cmd(0b0010);  //enable 4-bit mode
_delay_us(50);

lcd_cmd(0b0010);
lcd_cmd(0b1000); //4-bit mode, 2-line, 5x8 font
_delay_us(50);

lcd_cmd(0b0000);
lcd_cmd(0b1000); //display off
_delay_us(50);

lcd_clear();

lcd_cmd(0b0000);
lcd_cmd(0b0110); //entry mode set
_delay_us(50);

lcd_cmd(0b0000);
lcd_cmd(0b1100); //display on, cursor off, blink off
_delay_us(50);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void lcd_data(char data) //передача данных в дисплей
{

if(data & 1) D4_H;
else D4_L;
	
if(data & 2) D5_H;
else D5_L;
	
if(data & 4) D6_H;
else D6_L;
	
if(data & 8) D7_H;
else D7_L;

EN_H;
_delay_us(100); //можно уменьшить, если дисплей работает корректно
EN_L;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void lcd_cmd(char data) //передача команды
{ 
RS_L;
lcd_data(data);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void lcd_clear(void) //очистка экрана
{
lcd_cmd(0b0000);
lcd_cmd(0b0001);
_delay_ms(5);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void lcd_goto(char line, char column) //установка позиции курсора //строка 0..1, колонка 0..15
{
if(line) //строка 1
	{
	lcd_cmd((0xC0+column) >> 4);
	lcd_cmd((0xC0+column) & 0x0F);
	}
else //строка 0
	{
	lcd_cmd((0x80+column) >> 4);
	lcd_cmd((0x80+column) & 0x0F);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void lcd_char(char sign) //вывод символа
{
RS_H;
lcd_data((sign&0xF0)>>4);
lcd_data(sign&0x0F);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void lcd_print(const char *str) //вывод строки
{
for(uint8_t i=0; str[i]!='\0'; i++)
lcd_char(str[i]);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void lcd_right() //сдвиг экрана вправо
{
lcd_cmd(0x01);
lcd_cmd(0x0C);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
void lcd_left() //сдвиг влево
{
lcd_cmd(0x01);
lcd_cmd(0x08);
}
