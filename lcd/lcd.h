/*********************************************************************************************************************
* Author : Alsayed Alsisi
* Date   : Saturday, April 10, 2021
* Version: 2.0
* Contact: alsayed.alsisi@gmail.com
* License:
* You have the right to use the file as you wish in any educational or commercial purposes under the following
* conditions:
* - This file is to be used as is. No modifications are to be made to any part of the file, including this section.
* - This section is not to be removed under any circumstances.
* - Parts of the file may be used separately under the condition they are not modified, and preceded by this section.
* - Any bug encountered in this file or parts of it should be reported to the email address given above to be fixed.
* - No warranty is expressed or implied by the publication or distribution of this source code.
*********************************************************************************************************************/
/*********************************************************************************************************************
* File Information:
** File Name:
*  lcd.h
* 
** Description:
*  This file contains the public programming interfaces for the LCD device driver for ATmega32 family of microcontro-
*  llers. This file can be used with any of the following members of this family:
*  ATmega32, ATmega16, ATmega16A
*********************************************************************************************************************/


/*********************************************************************************************************************
                                               << Header Guard >>
*********************************************************************************************************************/
#ifndef LCD_H_
#define LCD_H_

/*********************************************************************************************************************
                                               << File Inclusions >>
*********************************************************************************************************************/


/*********************************************************************************************************************
                                               << Public Constants >>
*********************************************************************************************************************/


/*********************************************************************************************************************
                                               << Public Data Types >>
*********************************************************************************************************************/
typedef enum
{
	LCD_OK = 0,
	LCD_NOT_OK
} lcd_std_error_type_t;

typedef enum
{
	LCD_CLEAR_ALL = 0x00,
} lcd_command_t;

/*********************************************************************************************************************
                                          << Public Variable Declarations >>
*********************************************************************************************************************/


/*********************************************************************************************************************
                                   << Public Function Declarations (Programming Interfaces) >>
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
extern void lcd_init(void);

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
extern lcd_std_error_type_t lcd_gotoxy(uint8_t x, uint8_t y);

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
extern lcd_std_error_type_t lcd_command_send(lcd_command_t lcd_command);

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
extern void lcd_character_write(uint8_t data_character);

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
*
** Example:
*  int   my_num = 12345;
*  char* my_string[6];
*  lcd_int_to_string(my_num, 5, my_string);
*  Note: my_string now contains "12345"
*********************************************************************************************************************/
extern void lcd_int_to_string(uint32_t int_number, uint8_t number_of_digits, char* conversion_string);



#endif /* LCD_H_ */
/*********************************************************************************************************************
                                               << End of File >>
*********************************************************************************************************************/
