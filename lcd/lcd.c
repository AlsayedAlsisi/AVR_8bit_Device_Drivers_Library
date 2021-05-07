/*********************************************************************************************************************
* Author : Alsayed Alsisi
* Date   : Saturday, April 10, 2021
* Version: 2.0
* Contact: alsayed.alsisi@gmail.com
* License:
* You have the right to use the file as you wish in any educational or commercial purposes under the following
* conditions:
* [1] This file is to be used as is. No modifications are to be made to any part of the file, including this section.
* [2] This section is not to be removed under any circumstances.
* [3] Parts of the file may be used separately under the condition they are not modified, and preceded by this section.
* [4] Any bug encountered in this file or parts of it should be reported to the email address given above to be fixed.
* [5] No warranty is expressed or implied by the publication or distribution of this source code.
*********************************************************************************************************************/
/*********************************************************************************************************************
* File Information:
** File Name:
*  lcd.c
* 
** Description:
*  This file contains the implementation for the LCD device driver for ATmega32 family of microcontrollers. 
*  This file can be used with any of the following members of this family:
*  ATmega32, ATmega16, ATmega16A
*********************************************************************************************************************/


/*********************************************************************************************************************
                                               << File Inclusions >>
*********************************************************************************************************************/
#include "lcd_config.h"  /* Note: It also contains the definition of F_CPU required for the delay functions. */
#include <util/delay.h>
#include <stdint.h>
#include "lcd.h"
#include "bit_math.h"
#include "gpio_atmega32.h"

/*********************************************************************************************************************
                                              << Private Constants >>
*********************************************************************************************************************/
#define   LCD_4BIT_OPERATION   (0)
#define   LCD_8BIT_OPERATION   (1)
#define   HALF_BYTE   (4U)

/*********************************************************************************************************************
                                              << Private Data Types >>
*********************************************************************************************************************/


/*********************************************************************************************************************
                                          << Private Variable Definitions >>
*********************************************************************************************************************/


/*********************************************************************************************************************
                                          << Public Variable Definitions >>
*********************************************************************************************************************/


/*********************************************************************************************************************
                                         << Private Functions Prototypes >>
*********************************************************************************************************************/
static void lcd_4bit_init(void);
static void lcd_8bit_init(void);

static void lcd_8bit_command_send(lcd_command_t lcd_command);
static void lcd_4bit_command_send(lcd_command_t lcd_command);
static void lcd_8bit_character_write(uint8_t data_character);
static void lcd_4bit_character_write(uint8_t data_character);
/*********************************************************************************************************************
                                          << Public Function Definitions >>
*********************************************************************************************************************/
/*********************************************************************************************************************
** Function Name:
*  lcd_init
*
** Description:
*  This function starts the LCD initialization with the certain LCD configurations selected.
*
** Input Parameters:
*  - void
*
** Return Value:
*  - void
*********************************************************************************************************************/
void lcd_init(void)
{
	#if   (LCD_8BIT_OPERATION == LCD_MODE)
		lcd_8bit_init();
		
	#elif (LCD_4BIT_OPERATION == LCD_MODE)
		lcd_4bit_init();
		
	#else
	#error You need to specify the operation mode of the LCD. Choose between 8 bit and 4 bit modes in "lcd_config.h"

    #endif

}

/*********************************************************************************************************************
** Function Name:
*  lcd_gotoxy
*
** Description:
*  This function moves the LCD cursor to a certain location based on the passed arguments.
*
** Input Parameters:
*  - x: uint8_t
*    Passes the desired character column number to the function.
*  - y: uint8_t
*    Passes the desired character line number to the function.
*
** Return Value:
*  - lcd_std_error_type_t
*    Returns 'LCD_OK' for correct character coordinates, and 'LCD_NOT_OK' if the x or y coordinates is wrong.
*********************************************************************************************************************/
lcd_std_error_type_t lcd_gotoxy(uint8_t x, uint8_t y)
{
  lcd_std_error_type_t return_error = LCD_OK; /* To do*/
	uint8_t first_char_address[]={0x80, 0xC0, 0x90, 0xD0}; /* Fits 16x2 and 16x4 LCDs */
	lcd_command_send(first_char_address[y]+(x));
	_delay_us(100);	
  
  return return_error;
}

/*********************************************************************************************************************
** Function Name:
*  lcd_command_send
*
** Description:
*  This function sends a certain command to the LCD.
*
** Input Parameters:
*  - lcd_command: lcd_command_t
*    Command to send to the LCD.
*
** Return Value:
*  - lcd_std_error_type_t
*    Returns 'LCD_OK' for correct command, and 'LCD_NOT_OK' if the passed command isn't any of the available commands.
*
*********************************************************************************************************************/
lcd_std_error_type_t lcd_command_send(lcd_command_t lcd_command)
{
    #if   (LCD_8BIT_OPERATION == LCD_MODE)
    lcd_8bit_command_send(lcd_command);
    
    #elif (LCD_4BIT_OPERATION == LCD_MODE)
    lcd_4bit_command_send(lcd_command);
    
    #else
    #error You need to specify the operation mode of the LCD. Choose between 8 bit and 4 bit modes in "lcd_config.h"
	
	#endif
}

/*********************************************************************************************************************
** Function Name:
*  lcd_character_write
*
** Description:
*  This function writes a character on the LCD.
*
** Input Parameters:
*  - data_character: uint8_t
*    This is the ASCII decimal value of the character to be printed on the LCD.
*
** Return Value:
*  - void
*********************************************************************************************************************/
void lcd_character_write(uint8_t data_character)
{
	/* For 8 bit LCD operation: */
	#if   (LCD_8BIT_OPERATION == LCD_MODE)
	lcd_8bit_character_write(data_character);
	
	#elif (LCD_4BIT_OPERATION == LCD_MODE)
	lcd_4bit_character_write(data_character);
	
	#else
	#error You need to specify the operation mode of the LCD. Choose between 8 bit and 4 bit modes in "lcd_config.h"
	
	#endif

}

/*********************************************************************************************************************
** Function Name:
*  lcd_int_to_string
*
** Description:
*  This function is used to convert the digits of an integer into a string of characters. This is a necessary step
*  when an integer is desired to be printed on the LCD.
*
** Input Parameters:
*  - int_number: uint32_t
*    The number that is required to be converted to a string.
*  - number_of_digits: uint8_t
*    This is the number of digits of the previous number that you want to convert.
*  - coversion_string: char*
*    This is a pointer to the string the will store the conversion result. Note that the size of the string need to
*    consider the NULL character that will be added by the function at the end of the converted string.
*
** Return Value:
*  - void
*********************************************************************************************************************/
void lcd_int_to_string(uint32_t int_number, uint8_t number_of_digits, char* conversion_string)
{
	uint8_t digit;
	for(int8_t i=(number_of_digits-1); i>=0; i--)
	{
		digit = int_number%10;
		int_number = int_number/10;
		conversion_string[i] = digit + 48; /* 48 is the decimal value of ASCII '0' */
	}
	
	conversion_string[number_of_digits]=0; /* Append '\0' to  the string */
}

/*********************************************************************************************************************
                                          << Private Function Definitions >>
*********************************************************************************************************************/
/*********************************************************************************************************************
** Function Name:
*  lcd_4bit_init
*
** Description:
*  This function initializes the LCD for the 4 bit operation mode.
*
** Input Parameters:
*  - void
*
** Return Value:
*  - void 
*********************************************************************************************************************/
static void lcd_4bit_init(void)
{
	_delay_ms(15);
	/* Configure data pins: */
	gpio_pins_config(LCD_DATA_PORT, ((1<<LCD_D7)|(1<<LCD_D6)|(1<<LCD_D5)|(1<<LCD_D4)), GPIO_OUTPUT, 0x00);
	/* Configure control pins: */
	gpio_pin_config(LCD_RS_CNTRL_PORT, LCD_RS, GPIO_OUTPUT, GPIO_PIN_LOW);
    gpio_pin_config(LCD_RW_CNTRL_PORT, LCD_RW, GPIO_OUTPUT, GPIO_PIN_LOW);
	gpio_pin_config(LCD_EN_CNTRL_PORT, LCD_EN, GPIO_OUTPUT, GPIO_PIN_LOW);
	lcd_command_send(0x33); 
	_delay_us(100);
	lcd_command_send(0x32); //for 4-bit mode
	_delay_us(100);
	lcd_command_send(0x28); //for 4-bit mode
	_delay_us(100);
	lcd_command_send(0x0E); //display on, cursor on.
	_delay_us(100);
	lcd_command_send(0x06); //No shift and auto increment right
	_delay_us(100);
	lcd_command_send(0x01); //clear lcd
	_delay_us(1500); /* This is necessary for the LCD to work correctly.*/
}

/*********************************************************************************************************************
** Function Name:
*  lcd_8bit_init
*
** Description:
*  This function initializes the LCD for the 8 bit operation mode.
*
** Input Parameters:
*  - void
*
** Return Value:
*  - void
*
*********************************************************************************************************************/
static void lcd_8bit_init(void)
{
	_delay_ms(15);
	/* Configuring data pins as output: */
	gpio_port_config(LCD_DATA_PORT, GPIO_OUTPUT, 0x00);
	/* Configuring control pins as output: */
	gpio_pin_config(LCD_RS_CNTRL_PORT, LCD_RS, GPIO_OUTPUT, GPIO_PIN_LOW);
	gpio_pin_config(LCD_RW_CNTRL_PORT, LCD_RW, GPIO_OUTPUT, GPIO_PIN_LOW);
	gpio_pin_config(LCD_EN_CNTRL_PORT, LCD_EN, GPIO_OUTPUT, GPIO_PIN_LOW);

	lcd_command_send(0x38); 
	_delay_us(100);
	lcd_command_send(0x0E); //display on, cursor on.
	_delay_us(100);
	lcd_command_send(0x06); //No shift and auto increment right
	_delay_us(100);
	lcd_command_send(0x01); //clear lcd
	_delay_us(1500); /* This is necessary for the LCD to work correctly.*/
	

}

/*********************************************************************************************************************
** Function Name:
*  lcd_8bit_command_send
*
** Description:
*  This function is used when the LCD is configured to work in 8 bit mode to send a command to the LCD.
*
** Input Parameters:
*  - lcd_command: lcd_command_t
*    Passed the desired command to the LCD.
*
** Return Value:
*  - void
*
*********************************************************************************************************************/
static void lcd_8bit_command_send(lcd_command_t lcd_command)
{
	/* For 8 bit LCD operation: */
	gpio_port_write(LCD_DATA_PORT, lcd_command);
	
	gpio_pin_write(LCD_RW_CNTRL_PORT, LCD_RW, GPIO_PIN_LOW);
	gpio_pin_write(LCD_RS_CNTRL_PORT, LCD_RS, GPIO_PIN_LOW);
	gpio_pin_write(LCD_EN_CNTRL_PORT, LCD_EN, GPIO_PIN_HIGH);
	_delay_us(40);
	gpio_pin_write(LCD_EN_CNTRL_PORT, LCD_EN, GPIO_PIN_LOW);
	_delay_us(100);
}


/*********************************************************************************************************************
** Function Name:
*  lcd_4bit_command_send
*
** Description:
*  This function is used when the LCD is configured to work in 4 bit mode to send a command to the LCD.
*
** Input Parameters:
*  - lcd_command: lcd_command_t
*    Passes the desired command to the function.
*
** Return Value:
*  - void
*
*********************************************************************************************************************/
static void lcd_4bit_command_send(lcd_command_t lcd_command)
{
	/* Sending the first 4 bits (MSB bits) */
	gpio_pins_write(LCD_DATA_PORT, ((1<<LCD_D7)|(1<<LCD_D6)|(1<<LCD_D5)|(1<<LCD_D4)), ((lcd_command>>HALF_BYTE)<<LCD_D4));
	gpio_pin_write(LCD_RW_CNTRL_PORT, LCD_RW, GPIO_PIN_LOW);
	gpio_pin_write(LCD_RS_CNTRL_PORT, LCD_RS, GPIO_PIN_LOW);
	gpio_pin_write(LCD_EN_CNTRL_PORT, LCD_EN, GPIO_PIN_HIGH);	/* Start of a pulse on the Enable pin: */
	_delay_us(40);
	gpio_pin_write(LCD_EN_CNTRL_PORT, LCD_EN, GPIO_PIN_LOW);  	/* End of the pulse on the Enable pin */
	
	_delay_us(100);
	
	/* Sending the last 4 bits (LSB bits): */
	gpio_pins_write(LCD_DATA_PORT, ((1<<LCD_D7)|(1<<LCD_D6)|(1<<LCD_D5)|(1<<LCD_D4)), (lcd_command << LCD_D4)); 
	gpio_pin_write(LCD_EN_CNTRL_PORT, LCD_EN, GPIO_PIN_HIGH);	/* Start of a pulse on the Enable pin: */
	_delay_us(40);
	gpio_pin_write(LCD_EN_CNTRL_PORT, LCD_EN, GPIO_PIN_LOW);  	/* End of the pulse on the Enable pin */
}

/*********************************************************************************************************************
** Function Name:
*  lcd_8bit_character_write
*
** Description:
*  This function is used when the LCD is configured to work in 8 bit mode to write a character to the LCD.
*
** Input Parameters:
*  - data_character: uint8_t
*    ASCII decimal value of the desired character.
*
** Return Value:
*  - void
*
*********************************************************************************************************************/
static void lcd_8bit_character_write(uint8_t data_character)
{
	gpio_port_write(LCD_DATA_PORT, data_character);
	gpio_pin_write(LCD_RS_CNTRL_PORT, LCD_RS, GPIO_PIN_HIGH);
	gpio_pin_write(LCD_RW_CNTRL_PORT, LCD_RW, GPIO_PIN_LOW);
	gpio_pin_write(LCD_EN_CNTRL_PORT, LCD_EN, GPIO_PIN_HIGH);
	_delay_us(40);
	gpio_pin_write(LCD_EN_CNTRL_PORT, LCD_EN, GPIO_PIN_LOW);
	_delay_us(100);
}

/*********************************************************************************************************************
** Function Name:
*  lcd_4bit_character_write
*
** Description:
*  This function is used when the LCD is configured to work in 4 bit mode to write a character to the LCD.
*
** Input Parameters:
*  - data_character: uint8_t
*    ASCII decimal value of the desired character.
*
** Return Value:
*  - void
*
*********************************************************************************************************************/
static void lcd_4bit_character_write(uint8_t data_character)
{
	/* Sending the first 4 bits (MSB bits) */
	gpio_pins_write(LCD_DATA_PORT, ((1<<LCD_D7)|(1<<LCD_D6)|(1<<LCD_D5)|(1<<LCD_D4)), ((data_character>>HALF_BYTE)<<LCD_D4));
	gpio_pin_write(LCD_RS_CNTRL_PORT, LCD_RS, GPIO_PIN_HIGH);
	gpio_pin_write(LCD_RW_CNTRL_PORT, LCD_RW, GPIO_PIN_LOW);
	gpio_pin_write(LCD_EN_CNTRL_PORT, LCD_EN, GPIO_PIN_HIGH);
	_delay_us(40);
	gpio_pin_write(LCD_EN_CNTRL_PORT, LCD_EN, GPIO_PIN_LOW);
	
	_delay_us(100);
	
	/* Sending the last 4 bits (LSB bits) */
	gpio_pins_write(LCD_DATA_PORT, ((1<<LCD_D7)|(1<<LCD_D6)|(1<<LCD_D5)|(1<<LCD_D4)), (data_character << LCD_D4));
	gpio_pin_write(LCD_EN_CNTRL_PORT, LCD_EN, GPIO_PIN_HIGH);	/* Start of a pulse on the Enable pin: */
	_delay_us(40);
	gpio_pin_write(LCD_EN_CNTRL_PORT, LCD_EN, GPIO_PIN_LOW);  	/* End of a pulse on the Enable pin: */
}



/*********************************************************************************************************************
                                                << End of File >>
*********************************************************************************************************************/
