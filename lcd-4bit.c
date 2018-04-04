

#define F_CPU  8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include <stdint.h> 

/*----------------------------------------------------------------
--------------------- Private Constants --------------------------
----------------------------------------------------------------*/
#define LCD_PRT   PORTA  //lcd data port
#define LCD_DDR   DDRA   //lcd data ddr
#define LCD_PIN   PINA	 //lcd data pin
#define LCD_RS		0
#define LCD_RW		1
#define LCD_EN		2

/*----------------------------------------------------------------
--------------------- Public Function Definitions ----------------
----------------------------------------------------------------*/

void lcd_init(void)
{
	LCD_DDR = 0xFF;
	LCD_PRT &= ~((1<<LCD_RS)|(1<<LCD_RW));
	LCD_PRT &=  ~(1<<LCD_EN);
	_delay_us(2000);
	lcd_command_send(0x33); //for 4-bit mode
	_delay_us(100);
	lcd_command_send(0x32); //for 4-bit mode
	_delay_us(100);
	lcd_command_send(0x28); //for 4-bit mode
	_delay_us(100);
	lcd_command_send(0x0e); //display on, cursor on.
	_delay_us(100);
	lcd_command_send(0x01); //clear lcd
	_delay_us(2000);
	lcd_command_send(0x06); //No shift and auto increment right
	_delay_us(100);
}


void lcd_command_send(char cmnd)
{
	LCD_PRT = (LCD_PRT & 0x0F) | (cmnd & 0xF0);
	LCD_PRT &= ~((1<<LCD_RS)|(1<<LCD_RW));
	LCD_PRT |=  (1<<LCD_EN);
	_delay_us(100);
	LCD_PRT &=  ~(1<<LCD_EN);
	_delay_us(200);
	LCD_PRT = (LCD_PRT & 0x0F) | (cmnd << 4);
	LCD_PRT |=  (1<<LCD_EN);
	_delay_us(100);
	LCD_PRT &=  ~(1<<LCD_EN);
}

void lcd_data_send(char data)
{
	LCD_PRT = (LCD_PRT & 0x0F) | (data & 0xF0);
	LCD_PRT |= (1<<LCD_RS);
	LCD_PRT &= ~(1<<LCD_RW);
	LCD_PRT |=  (1<<LCD_EN);
	_delay_us(100);
	LCD_PRT &=  ~(1<<LCD_EN);
	_delay_us(200);
	LCD_PRT = (LCD_PRT & 0x0F) | (data << 4);
	LCD_PRT |=  (1<<LCD_EN);
	_delay_us(100);
	LCD_PRT &=  ~(1<<LCD_EN);
}

void lcd_gotoxy(uint8_t x, uint8_t y)
{
	uint8_t first_char_address[]={0x80, 0xC0,0x90, 0xD0}; //This is for 16x4 LCD. For 20x4, put it on this way: {0x80, 0xC0,0x94, 0xD4}
	lcd_command_send(first_char_address[y]+(x));
	_delay_us(100);	
}



void lcd_print_string(char* str)
{
	uint8_t i=0;
	while (str[i]!=0)
	{
		lcd_data_send(str[i]);
		i++;
	}
}